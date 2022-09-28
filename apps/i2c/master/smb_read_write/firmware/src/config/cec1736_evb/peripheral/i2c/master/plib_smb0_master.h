/*******************************************************************************
  Serial Communication Interface Inter-Integrated Circuit (I2C) Library
  Instance Header File

  Company:
    Microchip Technology Inc.

  File Name:
    plib_smb0_master.h

  Summary:
    I2C PLIB Master Mode Header file

  Description:
    This file defines the interface to the I2C peripheral library. This
    library provides access to and control of the associated peripheral
    instance.
*******************************************************************************/
// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018-2019 Microchip Technology Inc. and its subsidiaries.
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

#ifndef PLIB_SMB0_MASTER_H
#define PLIB_SMB0_MASTER_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
/* This section lists the other files that are included in this file.
*/

#include "plib_smb_master_common.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// *****************************************************************************


void I2CSMB0_Initialize(void);
bool I2CSMB0_HostIsBusy(void);
void I2CSMB0_HostCallbackRegister(I2C_SMB_HOST_CALLBACK callback, uintptr_t contextHandle);
I2C_SMB_HOST_ERROR I2CSMB0_HostErrorGet(void);
bool I2CSMB0_HostTransferSetup(I2C_SMB_HOST_TRANSFER_SETUP* setup, uint32_t srcClkFreq );
void I2CSMB0_HostWriteByte(uint8_t address, uint8_t cmd, void* pWrdata);
void I2CSMB0_HostWriteWord(uint8_t address, uint8_t cmd, void* pWrdata);
void I2CSMB0_HostWriteBlock(uint8_t address, uint8_t cmd, void* pWrdata, uint32_t nWrBytes);
void I2CSMB0_HostReadByte(uint8_t address, uint8_t cmd);
void I2CSMB0_HostReadWord(uint8_t address, uint8_t cmd);
void I2CSMB0_HostReadBlock(uint8_t address, uint8_t cmd);
void I2CSMB0_HostWriteReadBlock(uint8_t address, uint8_t cmd, void* pWrdata, uint32_t nWrBytes);
uint32_t I2CSMB0_HostTransferCountGet(void);
uint32_t I2CSMB0_HostBufferRead(void* pBuffer);
void I2CSMB0_HostInterruptHandler(uint32_t completion_reg);

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
}
#endif
// DOM-IGNORE-END

#endif /* PLIB_SMB0_MASTER_H */





















