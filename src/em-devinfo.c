/*
 * SPDX-License-Identifier: LicenseRef-TQSPSLA-1.0.3
 *
 * More license information can be found in the root folder.
 * This file is part of libdeviceinfo.
 *
 * Copyright (c) 2025 TQ-Systems GmbH <license@tq-group.com>, D-82229 Seefeld, Germany. All rights reserved.
 * Author: Gregor Herburger
 */

#include <deviceinfo.h>
#include <stdio.h>
#include <string.h>

static void print_help() {
	printf("Usage: em-devinfo [OPTION]...\n");
	printf("Print device information.\n\n");
	printf("Options:\n");
	printf("  --manufacturer-id         Print manufacturer ID\n");
	printf("  --manufacturer-name       Print manufacturer name\n");
	printf("  --manufacturer-url        Print manufacturer URL\n");
	printf("  --product-id              Print product ID\n");
	printf("  --product-code            Print product code\n");
	printf("  --product-name            Print product name\n");
	printf("  --device-type             Print device type string\n");
	printf("  --firmware-version-id     Print firmware version ID\n");
	printf("  --firmware-version        Print firmware version string\n");
	printf("  --hardware-revision-id    Print hardware revision ID\n");
	printf("  --hardware-revision       Print hardware revision string\n");
	printf("  --serial                  Print serial string\n");
	printf("  --creation-year           Print creation year\n");
	printf("  -h, --help                Show this help message\n");
}

static void dump_info() {
	printf("Manufacturer ID: %04X\n", deviceinfo_get_manufacturer_id());
	printf("Manufacturer name: %s\n", deviceinfo_get_manufacturer_name());
	printf("Manufacturer URL: %s\n", deviceinfo_get_manufacturer_url());
	printf("Product ID: %04X\n", deviceinfo_get_product_id());
	printf("Product code: %s\n", deviceinfo_get_product_code());
	printf("Product name: %s\n", deviceinfo_get_product_name());
	printf("Device type string: %s\n", deviceinfo_get_device_type_str());
	printf("Firmware version ID: %04X\n", deviceinfo_get_firmware_version_id());
	printf("Firmware version string: %s\n", deviceinfo_get_firmware_version_str());
	printf("Hardware revision ID: %04X\n", deviceinfo_get_hardware_revision_id());
	printf("Hardware revision string: %s\n", deviceinfo_get_hardware_revision_str());
	printf("Serial string: %s\n", deviceinfo_get_serial_str());
	printf("Creation year: %s\n", deviceinfo_get_creation_year());
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        dump_info();
        return 0;
    }

    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        print_help();
        return 0;
    }

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--manufacturer-id") == 0) {
            printf("%04X\n", deviceinfo_get_manufacturer_id());
        } else if (strcmp(argv[i], "--manufacturer-name") == 0) {
            printf("%s\n", deviceinfo_get_manufacturer_name());
        } else if (strcmp(argv[i], "--manufacturer-url") == 0) {
            printf("%s\n", deviceinfo_get_manufacturer_url());
        } else if (strcmp(argv[i], "--product-id") == 0) {
            printf("%04X\n", deviceinfo_get_product_id());
        } else if (strcmp(argv[i], "--product-code") == 0) {
            printf("%s\n", deviceinfo_get_product_code());
        } else if (strcmp(argv[i], "--product-name") == 0) {
            printf("%s\n", deviceinfo_get_product_name());
        } else if (strcmp(argv[i], "--device-type") == 0) {
            printf("%s\n", deviceinfo_get_device_type_str());
        } else if (strcmp(argv[i], "--firmware-version-id") == 0) {
            printf("%04X\n", deviceinfo_get_firmware_version_id());
        } else if (strcmp(argv[i], "--firmware-version") == 0) {
            printf("%s\n", deviceinfo_get_firmware_version_str());
        } else if (strcmp(argv[i], "--hardware-revision-id") == 0) {
            printf("%04X\n", deviceinfo_get_hardware_revision_id());
        } else if (strcmp(argv[i], "--hardware-revision") == 0) {
            printf("%s\n", deviceinfo_get_hardware_revision_str());
        } else if (strcmp(argv[i], "--serial") == 0) {
            printf("%s\n", deviceinfo_get_serial_str());
        } else if (strcmp(argv[i], "--creation-year") == 0) {
            printf("%s\n", deviceinfo_get_creation_year());
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            return 1;
        }
    }
    return 0;
}
