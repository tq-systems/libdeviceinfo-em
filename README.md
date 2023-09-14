# Libdeviceinfo

A library providing common functions to query properties of an Energy Manager
device.

## Usage in sandboxed apps

When this library is used in sandboxed apps, the following aspects have to be
considered when writing the Systemd service file:

### `init()`

The following files will be read and thus must be added to `ReadOnlyPaths` if
`ProtectSystem=strict` is in use:

```
/etc/product-info.json
/sys/firmware/devicetree/base/compatible
/sys/firmware/devicetree/base/tq,revision
/sys/firmware/devicetree/base/tqs,revision
```

They provide information for the most public functions of this library, namely
for:

```C
uint16_t deviceinfo_get_manufacturer_id(void)
const char * deviceinfo_get_manufacturer_name(void)
const char * deviceinfo_get_manufacturer_url(void)
uint16_t deviceinfo_get_product_id(void)
const char * deviceinfo_get_product_code(void)
const char * deviceinfo_get_product_name(void)
const char * deviceinfo_get_device_type_str(void)
uint16_t deviceinfo_get_firmware_version_id(void)
const char * deviceinfo_get_firmware_version_str(void)
uint16_t deviceinfo_get_hardware_revision_id(void)
const char * deviceinfo_get_hardware_revision_str(void)
const char * deviceinfo_get_creation_year(void)
```

### `devicedeviceinfo_get_serial_str()`

For this function to work properly in sandboxed apps, additional configuration
is required:

- append `em-group-sudo-fw_printenv` to `SupplementaryGroups=`
    - this allows the service to use `sudo` to run the external `fw_printenv`
      binary
- append `/usr/bin/fw_printenv` to `ExecPaths=`
    - required if `NoExecPaths=/` or similar is in use
- add `DeviceAllow=/dev/mmcblk0`
    - required when using `DevicePolicy=closed` or similar
    - allows `fw_printenv` to access the mmc block device to extract the
      required information from the U-boot environment
- add `DeviceAllow=/dev/null`
    - required when using `DevicePolicy=strict`
    - alternatively, use `DevicePolicy=closed`, which automatically grants
      access to selected devices like `/dev/null`

The following sandboxing features *cannot* be used:
- `SecureBits`: remove `noroot` and `noroot-locked` options
    - `sudo` needs to spawn a process with root permissions
- `NoNewPrivileges=true`: set to `false` or remove entirely
- `PrivateDevices=true`: set to `false` or remove entirely
    - access to `/dev/null` is required
- `ProtectKernelTunables=true`: set to `false` or remove entirely
- `ProtectKernelModules=true`: set to `false` or remove entirely
