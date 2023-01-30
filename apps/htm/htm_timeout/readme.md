---
parent: Harmony 3 peripheral library application examples for CEC173x family
title: HTM Timeout
has_children: false
has_toc: false
---

[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)

# HTM (Hibernation Timer) Timeout

This example application demonstrates how to generate periodic timeout using the Hibernation timer peripheral.

## Description

This example application generates periodic timeout using the Hibernation peripheral. The Hibernation peripheral is configured to generate a timeout after 1 second. A callback is registered with the Hibernation timer peripheral. In the callback an LED is toggled and the timer period is re-configured to generate another timeout after 1 second thereby resulting in a periodic toggling of LED every 1 second.

## Downloading and building the application

To clone or download this application from Github, go to the [main page of this repository](https://github.com/Microchip-MPLAB-Harmony/csp_apps_cec173x) and then click **Clone** button to clone this repository or download as zip file.
This content can also be downloaded using content manager by following these [instructions](https://github.com/Microchip-MPLAB-Harmony/contentmanager/wiki).

Path of the application within the repository is **apps/htm/htm_timeout/firmware** .

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
2. Observe the LED6 (connected to GPIO156) blink at the rate of 1 second

| Board      | LED Name                                    |
| ----------------- | ---------------------------------------------- |
| [CEC 1736 Development Board](https://www.microchip.com/en-us/development-tool/EV19K07A) |LED6 |