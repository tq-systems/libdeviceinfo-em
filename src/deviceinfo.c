/*
 * Copyright © 2014, 2018 TQ-Systems GmbH <info@tq-group.com>
 * Author: Michael Heimpold, Matthias Schiffer
 */

#include <deviceinfo.h>

#include <jansson.h>

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
static char *hardware_revision;


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

static char * read_cpuinfo(const char *key) {
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	char *start;
	ssize_t c;
	char *ret = NULL;

	fp = fopen("/proc/cpuinfo", "r");
	if (fp == NULL)
		return NULL;

	while ((c = getline(&line, &len, fp)) != -1) {
		/* skip lines not starting with $key */
		if (!line_starts_with(line, key))
			continue;

		/* look for ':' */
		start = strchr(line, ':');
		if (!start)
			continue;
		start++;

		/* trim leading whitespacec */
		while (*start && (*start == ' ' || *start == '\t'))
			start++;

		trim_trail(start);
		ret = strdup(start);

		/* found so skip remaining file */
		break;
	}

	fclose(fp);
	free(line);

	return ret;
}

static uint16_t read_product_id(void) {
	char *compatible = read_file("/sys/firmware/devicetree/base/compatible");
	if (!compatible)
		return 0;

	uint16_t ret = 0;

	if (strcmp(compatible, "tqs,energymanager300") == 0)
		ret = 0x4842;
	else if (strcmp(compatible, "tqs,energymanager310") == 0)
		ret = 0x4852;

	free(compatible);
	return ret;
}


__attribute__((constructor)) static void init(void) {
	product_info = json_load_file(PRODUCT_INFO_FILE, 0, NULL);
	serial = read_fwenv("serial");
	product_id = read_product_id();
	hardware_revision = read_cpuinfo("Revision");
}

__attribute__((destructor)) static void deinit(void) {
	json_decref(product_info);
	product_info = NULL;

	free(serial);
	serial = NULL;

	free(hardware_revision);
	hardware_revision = NULL;
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
	uint16_t rev = 0;
	if (hardware_revision)
		sscanf(hardware_revision, "%04"SCNx16, &rev);

	return rev;
}

const char * deviceinfo_get_hardware_revision_str(void) {
	return hardware_revision;
}

const char * deviceinfo_get_serial_str(void) {
	return serial;
}
