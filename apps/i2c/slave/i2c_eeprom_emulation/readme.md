---
parent: Harmony 3 peripheral library application examples for CEC173x family
title: I2C EEPROM emulation (I2C slave) 
has_children: false
has_toc: false
---

[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)

# I2C EEPROM emulation (I2C slave)

This example application demonstrates how to use the I2C peripheral in slave mode.

## Description

This example uses the I2C peripheral library in slave mode and emulates an EEPROM of 512 bytes. There are two pages each of size 256 bytes. I2C slave expects two bytes of memory address from the I2C master and the memory address can range from 0x00 to 0x1FF.
I2C slave application supports following:

**Byte Write**: The I2C master sends the slave address, followed by two bytes of memory address. The slave provides the data present at the requested memory address.

**Page Write**: A page write is initiated the same way as a byte write, but the I2C master can write up-to 256 bytes (1 page). If more than 256 bytes are sent by the I2C master to the I2C slave, the memory address will “roll over” and previous data will be overwritten. The address “roll over” during write is from the last byte of the current page to the first byte of the same page.

**Current Address Read**: The internal memory address counter maintains the last address accessed during the last read or write operation, incremented by one. Once the device address with the read/write bit set to one is clocked in and acknowledged by the EEPROM, the data byte at the current address is serially clocked out. After reading the data from the current address, the I2C master sends NAK and generates a STOP condition.

**Random Read**: The I2C master writes the 2 byte memory address and then reads the data from that memory address. After reading the data, the I2C master sends NAK and generates a STOP condition.

**Sequential Read**: Sequential reads are initiated by either a current address read or a random address read. As long as the EEPROM receives an acknowledge, it will continue to increment the memory address and serially clock out sequential data bytes. When the memory address limit is reached (0x1FF), the memory address will “roll over” to 0x00 and the sequential read will continue. The sequential read operation is terminated when the I2C master sends a NAK and generates a STOP condition.

To run the application, two evaluation kits will be required - one acting as the I2C master and the other as the I2C slave. The I2C master application to be used for this demo application is available under **apps/i2c/master/i2c_eeprom/firmware** folder. I2C master writes an array of values to the I2C slave and verifies the value written by reading the values back and comparing it to the value written.

## Downloading and building the application

To clone or download this application from Github, go to the [main page of this repository](https://github.com/Microchip-MPLAB-Harmony/csp_apps_cec173x) and then click **Clone** button to clone this repository or download as zip file.
This content can also be downloaded using content manager by following these [instructions](https://github.com/Microchip-MPLAB-Harmony/contentmanager/wiki).

Path of the application within the repository is **apps/i2c/slave/i2c_eeprom_emulation/firmware** .

To build the application, refer to the following table and open the project using its IDE.

| Project Name      | Description                                    |
| ----------------- | ---------------------------------------------- |
| cec1736_evb.X | MPLABX project for [CEC 1736 Development Board](https://www.microchip.com/en-us/development-tool/EV19K07A) |
|||

## Setting up the hardware

The following table shows the target hardware for the application projects.

| Project Name| Board|
|:---------|:---------:|
| cec1736_evb.X | [CEC 1736 Development Board](https://www.microchip.com/en-us/development-tool/EV19K07A)
|||

Hardware setup requires two boards, one acting as the I2C Master and the other as the I2C slave.

### Setting up [CEC 1736 Development Board](https://www.microchip.com/en-us/development-tool/EV19K07A)

- Make the following connections between the two [CEC 1736 Development Boards](https://www.microchip.com/en-us/development-tool/EV19K07A):
    - Connect a wire between the GPIO140 (I2C06_SCL) pin of both the boards. GPIO140 pin is available on Pin 28 of P4 header
    - Connect a wire between the GPIO132 (I2C06_SDA) pin of both the boards. GPIO132 pin is available on Pin 26 of P4 header
    - Connect a GND wire between the two boards
- Connect the Debug USB port on the board to the computer using a micro USB cable

## Running the Application

1. Build and program I2C EEPROM application from *apps/i2c/master/i2c_eeprom/firmware*, used as I2C master
2. Build and Program I2C slave application, used as I2C slave
3. Run application on I2C slave board and then run the application on I2C master board
4. LED on the I2C master board indicates success or failure:

    - The LED is turned ON when the value read from the I2C slave matches with the written value
    - The LED is turned OFF when the value read from the I2C slave did not match with the written value
