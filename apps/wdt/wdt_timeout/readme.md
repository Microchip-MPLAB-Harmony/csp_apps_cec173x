---
parent: Harmony 3 peripheral library application examples for CEC173x family
title: WDT Timeout
has_children: false
has_toc: false
---

[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)

# WDT Timeout

This example application demonstrates how to use the WDT peripheral.

## Description

This example application configures the WDT peripheral to generate a reset after 4 seconds. The application toggles an LED and keeps refreshing the WDT every 1 seconds. User can enter a character on the UART terminal to emulate a lockup situation. The application stops toggling the LED and stops refreshing the WDT upon receiving a character on the UART terminal. After 4 seconds, the WDT timer expires and resets the application.

## Downloading and building the application

To clone or download this application from Github, go to the [main page of this repository](https://github.com/Microchip-MPLAB-Harmony/csp_apps_cec173x) and then click **Clone** button to clone this repository or download as zip file.
This content can also be downloaded using content manager by following these [instructions](https://github.com/Microchip-MPLAB-Harmony/contentmanager/wiki).

Path of the application within the repository is **apps/wdt/wdt_timeout/firmware** .

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

1. Open the Terminal application (Ex.:Tera term) on the computer
2. Connect to the Virtual COM port and configure the serial settings as follows:
    - Baud : 115200
    - Data : 8 Bits
    - Parity : None
    - Stop : 1 Bit
    - Flow Control : None
3. Build and Program the application using its IDE
4. The console displays the following message

    ![output](images/output1.png)
5. Observe the LED toogle every 1 second.
6. Enter a character on the terminal to emulate a lockup situation
7. Observe the following output on the terminal. WDT should reset the application in 4 seconds.

    ![output](images/output2.png)

| Board      | LED Name                                    |
| ----------------- | ---------------------------------------------- |
| [CEC 1736 Development Board](https://www.microchip.com/en-us/development-tool/EV19K07A) |LED6 |