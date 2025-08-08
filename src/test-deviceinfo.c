/*
 * SPDX-License-Identifier: LicenseRef-TQSPSLA-1.0.3
 *
 * More license information can be found in the root folder.
 * This file is part of libdeviceinfo.
 *
 * Copyright (c) 2018-2025 TQ-Systems GmbH <license@tq-group.com>, D-82229 Seefeld, Germany. All rights reserved.
 * Author: Matthias Schiffer
 */

#include <deviceinfo.h>
#include <stdio.h>


int main() {
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

	return 0;
}
