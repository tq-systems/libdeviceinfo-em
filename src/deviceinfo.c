/*
 * Copyright © 2014, 2018 TQ-Systems GmbH <info@tq-group.com>
 * Author: Michael Heimpold, Matthias Schiffer
 */

#include <deviceinfo.h>

#include <jansson.h>

#include <arpa/inet.h>
#include <ctype.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define PRODUCT_INFO_FILE "/etc/product-info.json"


static char *device_compatible;
static json_t *product_info;
static char *serial;

static uint16_t product_id;
static uint16_t hardware_revision;
static char hardware_revision_str[5];


/* Removes trailing whitespace */
static void trim_trail(char *str) {
	char *p = str + strlen(str);

	while (p > str) {
		p--;
		if (!isspace(*p))
			break;

		*p = 0;
	}
}

/**
 * Reads a file and returns the first line of output
 *
 * Trailing whitespace is removed.
 */
static char * read_file(const char *filename) {
	char *line = NULL;
	size_t len = 0;
	FILE *f;
	ssize_t rv;

	if ((f = fopen(filename, "r")) == NULL)
		return NULL;

	rv = getline(&line, &len, f);
	fclose(f);

	if (rv == -1) {
		free(line);
		return NULL;
	}

	/* trim away newline character(s) */
	trim_trail(line);

	return line;
}

/**
 * Executes a command and returns the first line of output
 *
 * Trailing whitespace is removed. When there is no output,
 * NULL is returned.
 */
static char * read_cmd(const char *cmd) {
	char *line = NULL;
	size_t len = 0;
	FILE *f;
	ssize_t rv;

	if ((f = popen(cmd, "r")) == NULL)
		return NULL;

	rv = getline(&line, &len, f);
	pclose(f);

	if (rv == -1) {
		free(line);
		return NULL;
	}

	/* trim away newline character(s) */
	trim_trail(line);

	return line;
}

/**
 * Reads a key from the u-boot environment
 */
static char * read_fwenv(const char *key) {
	char cmdstr[64];
	int cmdstr_len = -1;

	/* Catch encoding errors or insufficient buffer size */
	cmdstr_len = snprintf(cmdstr, sizeof(cmdstr), "sudo -n fw_printenv -n '%s' 2>/dev/null", key);
	if (cmdstr_len < 0 || cmdstr_len >= (int)sizeof(cmdstr))
		return NULL;

	return read_cmd(cmdstr);
}

static inline bool line_starts_with(const char *line, const char *prefix) {
	return strncmp(line, prefix, strlen(prefix)) == 0;
}

static char * read_serial(void) {
	char *value;

	value = read_file("/proc/device-tree/serial-number");
	if (!value)
		read_fwenv("serial");

	return value;
}

static uint16_t read_product_id(void) {
	json_t *product_id_info;
	uint16_t ret = 0;

	if (!device_compatible)
		return 0;

	product_id_info = json_object_get(product_info, "product_id");
	ret = json_integer_value(json_object_get(product_id_info, device_compatible));

	if (!ret) {
		if (strcmp(device_compatible, "tq,em300") == 0 ||
		    strcmp(device_compatible, "tqs,energymanager300") == 0)
			ret = 0x4842;
		else if (strcmp(device_compatible, "tq,em310") == 0 ||
			 strcmp(device_compatible, "tqs,energymanager310") == 0)
			ret = 0x4852;
		else if (strcmp(device_compatible, "tq,em4xx") == 0)
			ret = 0x4862;
	}

	return ret;
}

static uint16_t read_hardware_revision(void) {
	FILE *f;
	uint32_t rev;

	f = fopen("/proc/device-tree/tq,revision", "r");
	if (!f)
		f = fopen("/proc/device-tree/tqs,revision", "r");
	if (!f)
		return 0;

	if (fread(&rev, sizeof(rev), 1, f) != 1)
		rev = 0;

	fclose(f);

	/* Value is passed as 32bit big-endian by U-boot */
	return ntohl(rev);
}

__attribute__((constructor)) static void init(void) {
	device_compatible = read_file("/proc/device-tree/compatible");

	product_info = json_load_file(PRODUCT_INFO_FILE, 0, NULL);
	serial = read_serial();
	product_id = read_product_id();
	hardware_revision = read_hardware_revision();

	snprintf(hardware_revision_str, sizeof(hardware_revision_str), "%04X", hardware_revision);
}

__attribute__((destructor)) static void deinit(void) {
	json_decref(product_info);
	product_info = NULL;

	free(serial);
	serial = NULL;

	free(device_compatible);
	device_compatible = NULL;
}


uint16_t deviceinfo_get_manufacturer_id(void) {
	uint16_t ret;

	ret = json_integer_value(json_object_get(product_info, "manufacturer_id"));
	if (!ret)
		ret = 0x5233;

	return ret;
}

const char * deviceinfo_get_manufacturer_name(void) {
	return json_string_value(json_object_get(product_info, "manufacturer"));
}

const char * deviceinfo_get_manufacturer_url(void) {
	return json_string_value(json_object_get(product_info, "manufacturer_url"));
}

uint16_t deviceinfo_get_product_id(void) {
	return product_id;
}

const char * deviceinfo_get_product_code(void) {
	return json_string_value(json_object_get(product_info, "code"));
}

const char * deviceinfo_get_product_name(void) {
	return json_string_value(json_object_get(product_info, "name"));
}

const char * deviceinfo_get_device_type_str(void) {
	return json_string_value(json_object_get(product_info, "devicetype"));
}

uint16_t deviceinfo_get_firmware_version_id(void) {
	uint8_t major = 0, minor = 0;
	const char *firmware_version = deviceinfo_get_firmware_version_str();
	if (firmware_version)
		sscanf(firmware_version, "%02"SCNx8".%02"SCNx8, &major, &minor);

	return (major << 8) | minor;
}

const char * deviceinfo_get_firmware_version_str(void) {
	return json_string_value(json_object_get(product_info, "software_version"));
}

uint16_t deviceinfo_get_hardware_revision_id(void) {
	return hardware_revision;
}

const char * deviceinfo_get_hardware_revision_str(void) {
	return hardware_revision_str;
}

const char * deviceinfo_get_serial_str(void) {
	return serial;
}

const char * deviceinfo_get_creation_year(void) {
	return json_string_value(json_object_get(product_info, "creation_year"));
}
