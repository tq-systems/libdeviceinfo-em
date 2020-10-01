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

	if (snprintf(cmdstr, sizeof(cmdstr), "fw_printenv -n '%s' 2>/dev/null", key) >= (int)sizeof(cmdstr))
		return NULL;

	return read_cmd(cmdstr);
}

static inline bool line_starts_with(const char *line, const char *prefix) {
	return strncmp(line, prefix, strlen(prefix)) == 0;
}

static uint16_t read_product_id(void) {
	char *compatible = read_file("/sys/firmware/devicetree/base/compatible");
	if (!compatible)
		return 0;

	uint16_t ret = 0;

	if (strcmp(compatible, "tq,em300") == 0 ||
	    strcmp(compatible, "tqs,energymanager300") == 0)
		ret = 0x4842;
	else if (strcmp(compatible, "tq,em310") == 0 ||
		 strcmp(compatible, "tqs,energymanager310") == 0)
		ret = 0x4852;
	else if (strcmp(compatible, "tq,em4xx") == 0)
		ret = 0x4862;

	free(compatible);
	return ret;
}

static uint16_t read_hardware_revision(void) {
	FILE *f;
	uint32_t rev;

	f = fopen("/sys/firmware/devicetree/base/tq,revision", "r");
	if (!f)
		f = fopen("/sys/firmware/devicetree/base/tqs,revision", "r");
	if (!f)
		return 0;

	if (fread(&rev, sizeof(rev), 1, f) != 1)
		rev = 0;

	fclose(f);

	/* Value is passed as 32bit big-endian by U-boot */
	return ntohl(rev);
}

__attribute__((constructor)) static void init(void) {
	product_info = json_load_file(PRODUCT_INFO_FILE, 0, NULL);
	serial = read_fwenv("serial");
	product_id = read_product_id();
	hardware_revision = read_hardware_revision();

	snprintf(hardware_revision_str, sizeof(hardware_revision_str), "%04X", hardware_revision);
}

__attribute__((destructor)) static void deinit(void) {
	json_decref(product_info);
	product_info = NULL;

	free(serial);
	serial = NULL;
}


uint16_t deviceinfo_get_manufacturer_id(void) {
	return 0x5233;
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
