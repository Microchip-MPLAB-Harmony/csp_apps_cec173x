---
parent: Harmony 3 peripheral library application examples for CEC173x family
title: SPI Host Target Write Read 
has_children: false
has_toc: false
---

[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)

# SPI Host Target Write Read

This example application demonstrates how to SPI host can write and read data from a SPI target (SPT).

## Description

This example uses the QMPSI peripheral in SPI mode as SPI host and SPT peripheral as SPI target. The application enables Memory region 0 and Memory region 1 on SPI target and then enters the SPI host state machine. The SPI host sends commands to the SPI target to read and write from Memory region 0. The host also demonstrates enabling host interrupt in the SPI target peripheral by sending the relevant commands. Note that both the SPI host and target are implemented on the same development board. Also, note that the SPI host uses DMA mode for data transfer.

## Downloading and building the application

To clone or download this application from Github, go to the [main page of this repository](https://github.com/Microchip-MPLAB-Harmony/csp_apps_cec173x) and then click **Clone** button to clone this repository or download as zip file.
This content can also be downloaded using content manager by following these [instructions](https://github.com/Microchip-MPLAB-Harmony/contentmanager/wiki).

Path of the application within the repository is **apps/spt/spi_host_target_write_read/firmware** .

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

### Setting up [CEC 1736 Development Board](https://www.microchip.com/en-us/development-tool/EV19K07A)

- Make the following connections:
| Pin  | SPI host (QMSPI in SPI mode) | SPI target (SPT) |
| ---- | ---------------------------- | ---------------- |
| SCK  | J61, Pin 4 | P5, Pin 7 | 
| MOSI | J61, Pin 6 | P5, Pin 2 |
| MISO | J61, Pin 5 | P5, Pin 4|
| CS   | P4, Pin 28 | P5, Pin 5 |
| SPT Interrupt | P4, Pin 26| P5, Pin 13 |
|||

- Connect the Debug USB port on the board to the computer using a micro USB cable

## Running the Application

1. Build and Program the application using its IDE
2. LED indicates success or failure:
    - The LED is turned ON the data written and read from the SPI target matches on the SPI host side

The following table provides the LED name:

| Board      | LED Name |
| ---------- | ---------------- |
| [CEC 1736 Development Board](https://www.microchip.com/en-us/development-tool/EV19K07A) | LED6 |
|||
