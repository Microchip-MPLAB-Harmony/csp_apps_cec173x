---
parent: Harmony 3 peripheral library application examples for  CEC 173X family
title: SMB master read write 
has_children: false
has_toc: false
---

[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)

# SMBUS master read write

This example application demonstrates how to use the SMBUS peripheral to read and write data in master mode.

## Description

This example uses the SMBUS peripheral library and demonstrates how application can use the various APIs available to read and write data when the peripheral is configured in master mode

## Downloading and building the application

To clone or download this application from Github, go to the [main page of this repository](https://github.com/Microchip-MPLAB-Harmony/csp_apps_cec173x) and then click **Clone** button to clone this repository or download as zip file.
This content can also be downloaded using content manager by following these [instructions](https://github.com/Microchip-MPLAB-Harmony/contentmanager/wiki).

Path of the application within the repository is **apps/i2c/master/smb_read_write/firmware** .

To build the application, refer to the following table and open the project using its IDE.

| Project Name      | Description                                    |
| ----------------- | ---------------------------------------------- |
| cec1736_evb.X | MPLABX project for [CEC 1736 Development Board](https://www.microchip.com/en-us/development-tool/EV19K07A)     |
|||

## Setting up the hardware

The following table shows the target hardware for the application projects.

| Project Name| Board|
|:---------|:---------:|
| cec1736_evb.X | [CEC 1736 Development Board](https://www.microchip.com/en-us/development-tool/EV19K07A) 
|||

Hardware setup requires two boards, one acting as the SMBUS Master and the other as the SMBUS slave.

### Setting up [CEC 1736 Development Board](https://www.microchip.com/en-us/development-tool/EV19K07A)

- Make the following connections between the two [CEC 1736 Development Boards](https://www.microchip.com/en-us/development-tool/EV19K07A):
    - Connect a wire between the GPIO140 (I2C06_SCL) pin of both the boards. GPIO140 pin is available on Pin 28 of P4 header
    - Connect a wire between the GPIO132 (I2C06_SDA) pin of both the boards. GPIO132 pin is available on Pin 26 of P4 header
    - Connect a GND wire between the two boards
- Connect the Debug USB port on the board to the computer using a micro USB cable

## Running the Application

1. Build and program *apps/i2c/master/smb_read_write/firmware*, on the board acting as SMBUS master
2. Build and Program *apps/i2c/slave/smb_read_write/firmware*, on the board acting as SMBUS slave
3. Run application on SMBUS slave board and then run the application on SMBUS master board
4. LED on the master board indicates success or failure:

    - The LED is turned ON when the value read from the SMBUS slave matches with the written value
    - The LED is turned OFF when the value read from the SMBUS slave did not match with the written value

The following table provides the LED name:

| Board      | LED Name |
| ---------- | ---------------- |
| [CEC 1736 Development Board](https://www.microchip.com/en-us/development-tool/EV19K07A) | LED5 |
|||
