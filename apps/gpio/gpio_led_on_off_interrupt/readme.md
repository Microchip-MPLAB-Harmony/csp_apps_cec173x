---
parent: Harmony 3 peripheral library application examples for CEC173x family
title: GPIO interrupt 
has_children: false
has_toc: false
---

[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)

# GPIO interrupt

This example application demonstrate how to generate GPIO interrupt on a pin that is configured to generate an interrupt on either (rising or falling) edge, and indicate the pin status using the LED.

## Description

This application uses the GPIO Peripheral library to generate callback on when a pin is driven high or low, and drives the GPIO pin connected to the LED to indicate the pin status. Systick timer is configured to generate a periodic interrupt. A GPIO pin (configured as output) is toggled in the systick timer interrupt. The GPIO output pin is connected to the GPIO input pin. A LED is toggled when the GPIO input pin is driven high or low.

## Downloading and building the application

To clone or download this application from Github, go to the [main page of this repository](https://github.com/Microchip-MPLAB-Harmony/csp_apps_cec173x) and then click **Clone** button to clone this repository or download as zip file.
This content can also be downloaded using content manager by following these [instructions](https://github.com/Microchip-MPLAB-Harmony/contentmanager/wiki).

Path of the application within the repository is **apps/gpio/gpio_led_on_off_interrupt/firmware** .

To build the application, refer to the following table and open the project using its IDE.

| Project Name      | Description                                    |
| ----------------- | ---------------------------------------------- |
| cec1736_evb.X | MPLABX project for [CEC 1736 Development Board](https://www.microchip.com/en-us/development-tool/EV19K07A)     |
|||

## Setting up the hardware

The following table shows the target hardware for the application projects.

| Project Name| Board|
|:---------|:---------:|
| cec1736_evb.X | MPLABX project for [CEC 1736 Development Board](https://www.microchip.com/en-us/development-tool/EV19K07A)     |
|||

### Setting up [CEC 1736 Development Board](https://www.microchip.com/en-us/development-tool/EV19K07A)

- Connect the USB port P2 on the board to the computer using a micro USB cable
- Connect the GPIO030 (output pin, on J11 connector, pin 1) to GPIO132 (input pin, on J11 connector, pin 3)

## Running the Application

1. Build and program the application project using its IDE

2. The LED is toggled every 250 milliseconds

The following table provides the switch and the LED name:

| Board | LED name |
| ----- | -------- |
|[CEC 1736 Development Board](https://www.microchip.com/en-us/development-tool/EV19K07A) | LED5 |
|||
