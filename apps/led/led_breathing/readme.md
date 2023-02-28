---
parent: Harmony 3 peripheral library application examples for CEC173x family
title: LED Breathing
has_children: false
has_toc: false
---

[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)

# LED Breathing

This example application demonstrates how to generate a breathing LED pattern using the Breathing LED peripheral.

## Description

This example application generates a symmetrical breathing LED output. The peripheral is configured to generate a breathing LED output with a ramp up and ramp down time of 1.84 seconds with a total period of 5.26 seconds. The high delay (PWM cycles to wait once the duty cycle reaches MAX value) is set to 100. Similarly the low delay (PWM cycles to wait once the duty cycle reaches MIN value) is set to 100.

## Downloading and building the application

To clone or download this application from Github, go to the [main page of this repository](https://github.com/Microchip-MPLAB-Harmony/csp_apps_cec173x) and then click **Clone** button to clone this repository or download as zip file.
This content can also be downloaded using content manager by following these [instructions](https://github.com/Microchip-MPLAB-Harmony/contentmanager/wiki).

Path of the application within the repository is **apps/led/led_breathing/firmware** .

To build the application, refer to the following table and open the project using its IDE.

| Project Name      | Description                                    |
| ----------------- | ---------------------------------------------- |
| cec1736_evb.X | MPLABX project for [CEC 1736 Development Board](https://www.microchip.com/en-us/development-tool/EV19K07A)     |

## Setting up the hardware

The following table shows the target hardware for the application projects.

| Project Name| Board|
|:---------|:---------:|
| cec1736_evb.X | MPLABX project for [CEC 1736 Development Board](https://www.microchip.com/en-us/development-tool/EV19K07A)   |

### Setting up [CEC 1736 Development Board](https://www.microchip.com/en-us/development-tool/EV19K07A)

- No additional setup is requred to run this demo.

## Running the Application

1. Build and Program the application using its IDE
2. Observe the breathing LED pattern

| Board      | LED Name                                    |
| ----------------- | ---------------------------------------------- |
| [CEC 1736 Development Board](https://www.microchip.com/en-us/development-tool/EV19K07A) |LED6 |