# Arduino Core for WCH CH32V

## Description

This repository aims to build an Arduino core based on WCH's "EVT" / None OS SDK that would work on all CH32V devices.

## Using this Core

For now, only a PlatformIO builder script is provided to work with https://github.com/Community-PIO-CH32V/platform-ch32v.

Actual files needed for the Arduino IDE will be added later when the core is more fleshed out.

For an example project with this core, see ....

## Current State

**This core is an active development and in an extremely early state.**

* Board support
  - [] CH32V003
  - [] CH32V103
  - [] CH32V203
  - [] CH32V208
  - [] CH32V303
  - [] CH32V305
  - [X] CH32V307 (EXPERIMENTAL)
  - [] CH56x
  - [] CH57x
  - [] CH58x

# Related Projects

For CH32V003 only: https://github.com/AlexanderMandera/arduino-wch32v003
For CH58x only: https://github.com/ElectronicCats/arduino-wch58x

# Copyright and Authors

* ArduinoCore-API: https://github.com/arduino/ArduinoCore-API/ (LGPL 2.1)
* WCH NoneOS SDK: Proprietary, Nanjing Qinheng Microelectronics Co., Ltd.
* My additions: same as ArduinoCore-API