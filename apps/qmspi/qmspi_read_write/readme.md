---
parent: Harmony 3 peripheral library application examples for CEC173x family
title: QMSPI flash read write
has_children: false
has_toc: false
---

[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)

# QMSPI flash read write

This example shows how to use the QMSPI Peripheral library to perform erase, write and read operation with the QMSPI Serial Flash memory in Quad IO mode.

## Description

This example uses the QMSPI peripheral library to erase and write an array of values to the QMSPI Serial Flash memory. It verifies the value written by reading the values back and comparing it to the value written.

## Downloading and building the application

To clone or download this application from Github, go to the [main page of this repository](https://github.com/Microchip-MPLAB-Harmony/csp_apps_cec173x) and then click **Clone** button to clone this repository or download as zip file.
This content can also be downloaded using content manager by following these [instructions](https://github.com/Microchip-MPLAB-Harmony/contentmanager/wiki).

Path of the application within the repository is **apps/qmspi/qmspi_read_write/firmware** .

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

- Connect the USB port P2 on the board to the computer using a micro USB cable

## Running the Application

1. Build and Program the application using its IDE
2. LED indicates success or failure:
    - The LED is turned ON when the value read from the QSPI Serial Flash memory matched with the written value
    - The LED is turned OFF when the value read from the QSPI Serial Flash did not match with the written value

The following table provides the LED names

| Board                                                                                   | LED Name |
| --------------------------------------------------------------------------------------- | -------- |
| [CEC 1736 Development Board](https://www.microchip.com/en-us/development-tool/EV19K07A) | LED5     |
