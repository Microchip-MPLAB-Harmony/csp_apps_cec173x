/*******************************************************************************
  Serial Communication Interface Inter-Integrated Circuit (I2C) Library
  Instance Header File

  Company:
    Microchip Technology Inc.

  File Name:
    plib_smb0_slave.h

  Summary:
    I2C PLIB SMBUS Slave Header file

  Description:
    This file defines the interface to the I2C peripheral library. This
    library provides access to and control of the associated peripheral
    instance.
*******************************************************************************/
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

#ifndef PLIB_SMB0_SLAVE_H
#define PLIB_SMB0_SLAVE_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
/* This section lists the other files that are included in this file.
*/

#include "plib_smb_slave_common.h"

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

/*
 * The following functions make up the methods (set of possible operations) of
 * this interface.
 */
void I2CSMB0_Initialize(void);
void I2CSMB0_TargetCallbackRegister(I2C_SMB_TARGET_CALLBACK callback, uintptr_t contextHandle);
bool I2CSMB0_TargetIsBusy(void);
I2C_SMB_TARGET_TRANSFER_DIR I2CSMB0_TargetTransferDirGet(void);
I2C_SMB_TARGET_ERROR I2CSMB0_TargetErrorGet(void);
void I2CSMB0_TargetStart(void);
uint32_t I2CSMB0_TargetBufferRead(void* pBuffer);
void I2CSMB0_TargetBufferWrite(void* pBuffer, uint32_t nBytes);
void I2CSMB0_TargetInterruptHandler(uint32_t completion_reg);

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
}
#endif
// DOM-IGNORE-END

#endif /* PLIB_SMB0_SLAVE_H */