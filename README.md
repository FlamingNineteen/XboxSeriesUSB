# XboxSeriesUSB Plugin

This plugin interprets Xbox Series S or X controllers plugged into the USB ports at the front of the Wii U and assigns them as if they were connected like native Wii U wireless controllers.

*Note: this is an early version and this plugin could act unstable or may not function at all.*

## Installation
(`[ENVIRONMENT]` is a placeholder for the actual environment name.)

1. Copy the file `XboxSeriesUSB.wps` into `sd:/wiiu/environments/[ENVIRONMENT]/plugins`.
2. Requires the [WiiUPluginLoaderBackend](https://github.com/wiiu-env/WiiUPluginLoaderBackend) in `sd:/wiiu/environments/[ENVIRONMENT]/modules`.
3. Requires the [NotificationModule](https://github.com/wiiu-env/NotificationModule) in `sd:/wiiu/environments/[ENVIRONMENT]/modules`.

## Usage
Connect an Xbox Series S or Xbox Series X controller to one of the front USB ports of the Wii U with a USB-C to USB-A cord. Then turn on the controller.

## Why not Bluetooth?
Xbox Series S and Xbox Series X controllers do not support Bluetooth LE, the Bluetooth version used by the Wii U.

## Building

For building you need:
- [wut](https://github.com/decaf-emu/wut)
- [wups](https://github.com/wiiu-env/WiiUPluginSystem)
- [SDL2](https://github.com/yawut/SDL)
- [libnotifications](https://github.com/wiiu-env/libnotifications)
