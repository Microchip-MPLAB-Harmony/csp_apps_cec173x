/*******************************************************************************
  SPI Peripheral Target Peripheral Library Interface Header File

  Company
    Microchip Technology Inc.

  File Name
    plib_spt1.h

  Summary
    SPT1 peripheral library interface.

  Description
    This file defines the interface to the SPI Peripheral Target library.  This
    library provides access to and control of the associated peripheral
    instance.

******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2022 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
// DOM-IGNORE-END

#ifndef PLIB_SPT1_H    // Guards against multiple inclusion
#define PLIB_SPT1_H


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

/*  This section lists the other files that are included in this file.
*/

#include "plib_spt_common.h"
#include "device.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif

// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
/*  The following data type definitions are used by the functions in this
    interface and should be considered part it.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// *****************************************************************************
/* The following functions make up the methods (set of possible operations) of
   this interface.
*/
void SPT1_Initialize(void);


void SPT1_WaitTimeSet(uint8_t wait_time);

void SPT1_TARTimeSet(uint8_t tar_cycles);

void SPT1_QuadModeEnable(void);

void SPT1_QuadModeDisable(void);

void SPT1_ECInterruptEnable(SPT_EC_INT int_en);

void SPT1_CallbackRegister( SPT_CALLBACK callback, uintptr_t context );

void SPT1_MEM0Config(uint32_t bar, uint32_t wr_lim, uint32_t rd_lim);

void SPT1_MEM1Config(uint32_t bar, uint32_t wr_lim, uint32_t rd_lim);


uint32_t SPT1_ECStatusRegGet(void);

void SPT1_ECStatusRegClear(uint32_t bitmask);

void SPT1_MEM0Enable(void);

void SPT1_MEM1Enable(void);

void SPT1_MEM0Disable(void);

void SPT1_MEM1Disable(void);

uint32_t SPT1_RXFIFOByteCountGet(void);

uint32_t SPT1_TXFIFOByteCountGet(void);

uint32_t SPT1_RXFIFOBaseAddrGet(void);

uint32_t SPT1_TXFIFOBaseAddrGet(void);

void SPT1_Enable(void);

void SPT1_Disable(void);


uint32_t SPT1_HostToECMBXRead(void);

void SPT1_HostToECMBXClr(void);

void SPT1_ECToHostMBXWrite(uint32_t val);

uint32_t SPT1_ECToHostMBXRead(void);



#ifdef __cplusplus // Provide C++ Compatibility
}
#endif

#endif //PLIB_SPT1_H

/**
 End of File
*/
