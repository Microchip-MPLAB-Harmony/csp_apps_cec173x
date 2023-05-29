---
title: Harmony 3 peripheral library application examples for CEC173x family
nav_order: 1
has_children: true
has_toc: false
---
[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)

# Harmony 3 peripheral library application examples for CEC173x family

MPLAB® Harmony 3 is an extension of the MPLAB® ecosystem for creating embedded firmware solutions for Microchip 32-bit SAM and PIC® microcontroller and microprocessor devices.  Refer to the following links for more information.

- [Microchip 32-bit MCUs](https://www.microchip.com/design-centers/32-bit)
- [Microchip 32-bit MPUs](https://www.microchip.com/design-centers/32-bit-mpus)
- [Microchip MPLAB X IDE](https://www.microchip.com/mplab/mplab-x-ide)
- [Microchip MPLAB® Harmony](https://www.microchip.com/mplab/mplab-harmony)
- [Microchip MPLAB® Harmony Pages](https://microchip-mplab-harmony.github.io/)

This repository contains the MPLAB® Harmony 3 peripheral library application examples for CEC173x family

- [Release Notes](release_notes.md)
- [MPLAB® Harmony License](mplab_harmony_license.md)

To clone or download these applications from Github, go to the [main page of this repository](https://github.com/Microchip-MPLAB-Harmony/csp_app_cec173x) and then click **Clone** button to clone this repository or download as zip file.
This content can also be downloaded using content manager by following these [instructions](https://github.com/Microchip-MPLAB-Harmony/contentmanager/wiki).

## Contents Summary

| Folder     | Description                             |
| ---        | ---                                     |
| apps       | Contains peripheral library example applications |

## Code Examples

The following applications are provided to demonstrate the typical or interesting usage models of one or more peripheral libraries.

| Name | Description |
| ---- | ----------- |
| [CCT Capture mode](apps/cct/cct_capture_mode/readme.md) | This example application demonstrates how to use the Input Capture and Compare Timer peripheral in capture mode |
| [GPIO interrupt](apps/gpio/gpio_led_on_off_interrupt/readme.md) | This example application demonstrate how to generate GPIO interrupt on a pin that is configured to generate an interrupt on either (rising or falling) edge, and indicate the pin status using the LED |
| [HTM (Hibernation Timer) Timeout](apps/htm/htm_timeout/readme.md) | This example application demonstrates how to generate periodic timeout using the Hibernation timer peripheral |
| [I2C EEPROM read write](apps/i2c/master/i2c_eeprom/readme.md) | This example application demonstrates how to use the I2C peripheral to write and read from the I2C serial EEPROM memory |
| [SMBUS master read write](apps/i2c/master/smb_read_write/readme.md) | This example application demonstrates how to use the SMBUS peripheral to read and write data in master mode |
| [I2C EEPROM emulation (I2C slave)](apps/i2c/slave/i2c_eeprom_emulation/readme.md) | This example application demonstrates how to use the I2C peripheral in slave mode |
| [SMBUS slave read write](apps/i2c/slave/smb_read_write/readme.md) | This example application demonstrates how to use the SMBUS peripheral to read and write data in slave mode |
| [LED Breathing](apps/led/led_breathing/readme.md) | This example application demonstrates how to generate a breathing LED pattern using the Breathing LED peripheral |
| [PWM Duty Cycle](apps/pwm/pwm_duty_cycle/readme.md) | This example application demonstrates how to generate PWM output using the PWM peripheral |
| [QMSPI flash read write](apps/qmspi/qmspi_read_write/readme.md) | This example shows how to use the QMSPI Peripheral library to perform erase, write and read operation with the QMSPI Serial Flash memory in Quad IO mode |
| [RTOS Timer](apps/rtos/rtos_timer/readme.md) | This example application demonstrates how to generate periodic timeout using the RTOS timer peripheral |
| [Timer32 Periodic Timeout](apps/timer32/tmr32_periodic_timeout/readme.md) | This example application demonstrates how to generate periodic timeout using the TIMER32 timer peripheral |
| [UART echo blocking](apps/uart/uart_echo_blocking/readme.md) | This example application demonstrates how to use the UART peripheral to transfer a block of data in a blocking manner |
| [UART ring buffer](apps/uart/uart_ring_buffer_interrupt/readme.md) | This example application demonstrates how to use the UART peripheral in ring buffer mode |
| [WDT Timeout](apps/wdt/wdt_timeout/readme.md) | This example application demonstrates how to use the WDT peripheral |

____

[![License](https://img.shields.io/badge/license-Harmony%20license-orange.svg)](https://github.com/Microchip-MPLAB-Harmony/csp_apps_cec173x/blob/master/mplab_harmony_license.md)
[![Latest release](https://img.shields.io/github/release/Microchip-MPLAB-Harmony/csp_apps_cec173x.svg)](https://github.com/Microchip-MPLAB-Harmony/csp_apps_cec173x/releases/latest)
[![Latest release date](https://img.shields.io/github/release-date/Microchip-MPLAB-Harmony/csp_apps_cec173x.svg)](https://github.com/Microchip-MPLAB-Harmony/csp_apps_cec173x/releases/latest)
[![Commit activity](https://img.shields.io/github/commit-activity/y/Microchip-MPLAB-Harmony/csp_apps_cec173x.svg)](https://github.com/Microchip-MPLAB-Harmony/csp_apps_cec173x/graphs/commit-activity)
[![Contributors](https://img.shields.io/github/contributors-anon/Microchip-MPLAB-Harmony/csp_apps_cec173x)]()

____

[![Follow us on Youtube](https://img.shields.io/badge/Youtube-Follow%20us%20on%20Youtube-red.svg)](https://www.youtube.com/user/MicrochipTechnology)
[![Follow us on LinkedIn](https://img.shields.io/badge/LinkedIn-Follow%20us%20on%20LinkedIn-blue.svg)](https://www.linkedin.com/company/microchip-technology)
[![Follow us on Facebook](https://img.shields.io/badge/Facebook-Follow%20us%20on%20Facebook-blue.svg)](https://www.facebook.com/microchiptechnology/)
[![Follow us on Twitter](https://img.shields.io/twitter/follow/MicrochipTech.svg?style=social)](https://twitter.com/MicrochipTech)

[![](https://img.shields.io/github/stars/Microchip-MPLAB-Harmony/csp_apps_cec173x.svg?style=social)]()
[![](https://img.shields.io/github/watchers/Microchip-MPLAB-Harmony/csp_apps_cec173x.svg?style=social)]()