# Embedded Systems

## Getting started

See [Getting Started with the Raspberry Pi Pico](https://rptl.io/pico-get-started) and the README in the [pico-sdk](https://github.com/raspberrypi/pico-sdk) for information
on getting up and running.

### First  Examples

App|Description | Link to prebuilt UF2
---|---|---
[hello_serial](hello_world/serial) | The obligatory Hello World program for Pico (Output over serial version) |
[hello_usb](hello_world/usb) | The obligatory Hello World program for Pico (Output over USB version) | https://rptl.io/pico-hello-usb
[blink](blink) | Blink an LED on and off. | https://rptl.io/pico-blink


build as targets named `tinyusb_dual_<example_name>`, e.g. https://github.com/hathach/tinyusb/tree/master/examples/dual/host_hid_to_device_cdc
is built as `tinyusb_dual_host_hid_to_device_cdc`.
