/*
 * Copyright © 2018 TQ-Systems GmbH <info@tq-group.com>
 * Author: Matthias Schiffer
 */

#ifndef _LIBDEVICEINFO_DEVICEINFO_H_
#define _LIBDEVICEINFO_DEVICEINFO_H_

#include <stddef.h>
#include <stdint.h>

/** Numerical manufacturer ID; always 0x5233 */
uint16_t deviceinfo_get_manufacturer_id(void);
const char * deviceinfo_get_manufacturer_name(void);
const char * deviceinfo_get_manufacturer_url(void);

/** Numerical product ID; 0x4842 for TQ Energy Manager devices */
uint16_t deviceinfo_get_product_id(void);

/** Short product name like "EM400" */
const char * deviceinfo_get_product_code(void);

/** Long product name line "Energy Manager 400" */
const char * deviceinfo_get_product_name(void);

/** Device type like "hw0100", which is mapped directly via machine like "em310" */
const char * deviceinfo_get_device_type_str(void);

/**  Creation year like "2022" */
const char * deviceinfo_get_creation_year(void);

/**
 * Returns the firmware major and minor version as a uint16_t
 * 
 * The two bytes of the uint16_t contain the major and minor version of the
 * firmware. Sub-minor/patchlevel components of the version string are omitted.
 * 0x0000 is returned if the version number does not follow the common scheme;
 * this is the case of development (snapshot) versions.
 */
uint16_t deviceinfo_get_firmware_version_id(void);
const char * deviceinfo_get_firmware_version_str(void);

uint16_t deviceinfo_get_hardware_revision_id(void);
const char * deviceinfo_get_hardware_revision_str(void);

const char * deviceinfo_get_serial_str(void);

#endif /* _LIBDEVICEINFO_DEVICEINFO_H_ */
