# Libdeviceinfo

A library providing common functions to query properties of an Energy Manager
device.

## Usage in sandboxed apps

When this library is used in sandboxed apps, the following aspects have to be
considered when writing the Systemd service file:

### `init()`

The following files will be read and thus must be added to `ReadOnlyPaths`:

```
/etc/product-info.json
/sys/firmware/devicetree/base/compatible
/sys/firmware/devicetree/base/tq,revision
/sys/firmware/devicetree/base/tqs,revision
```

### `read_fwenv()`

 Add `em-group-sudo-fw_printenv` to `SupplementaryGroups` to allow
 using `sudo` to run the external `fw_printenv` binary.

`/dev/null` is used, so you cannot set `PrivateDevices` to `true`. Use `DevicePolicy=closed` instead.
