/*******************************************************************************
  PCR PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_pcr.h

  Summary:
    PCR PLIB Header File.

  Description:
    The PCR PLIB initializes all the oscillators based on the
    requirements.

*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
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

#ifndef PLIB_PCR_H
#define PLIB_PCR_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
/* This section lists the other files that are included in this file.
*/
#include <device.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus // Provide C++ Compatibility
extern "C" {
#endif

// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************


typedef enum
{
    PCR_SLEEP_EN0_STAP_SLP_EN = PCR_SLP_EN_0_STAP_SLP_EN_Msk,









}PCR_SLEEP_EN0;

typedef enum
{
    PCR_SLEEP_EN1_INT_SLP_EN = PCR_SLP_EN_1_INT_SLP_EN_Msk,












}PCR_SLEEP_EN1;

typedef enum
{
    PCR_SLEEP_EN3_HTM_0_SLP_EN = PCR_SLP_EN_3_HTM_0_SLP_EN_Msk,











}PCR_SLEEP_EN3;

typedef enum
{
    PCR_SLEEP_EN4_SECMON0_SLP_EN = PCR_SLP_EN_4_SECMON0_SLP_EN_Msk,










}PCR_SLEEP_EN4;


typedef enum
{
    PCR_PRIV_EN0_OTP = PCR_EC_PRIV_EN0_OTP_Msk,






}PCR_PRIV_EN0;

typedef enum
{
    PCR_PRIV_EN1_INTR = PCR_EC_PRIV_EN1_INTR_Msk,











}PCR_PRIV_EN1;

typedef enum
{
    PCR_PRIV_EN3_HIB_TIM0 = PCR_EC_PRIV_EN3_HIB_TIM0_Msk,











}PCR_PRIV_EN3;

typedef enum
{
    PCR_PRIV_EN4_SPIMON0 = PCR_EC_PRIV_EN4_SPIMON0_Msk,










}PCR_PRIV_EN4;


typedef enum
{
    PCR_RESET_EN0_JTAG_STAP_CLK_REQ = PCR_RST_EN_0_JTAG_STAP_CLK_REQ_Msk,







}PCR_RESET_EN0;

typedef enum
{
    PCR_RESET_EN1_INT_RST_EN = PCR_RST_EN_1_INT_RST_EN_Msk,









}PCR_RESET_EN1;

typedef enum
{
    PCR_RESET_EN3_HTM_0_RST_EN = PCR_RST_EN_3_HTM_0_RST_EN_Msk,











}PCR_RESET_EN3;

typedef enum
{
    PCR_RESET_EN4_SECMON0_RST_EN = PCR_RST_EN_4_SECMON0_RST_EN_Msk,










}PCR_RESET_EN4;


// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// *****************************************************************************
/* The following functions make up the methods (set of possible operations) of
this interface.
*/

// *****************************************************************************
/* Function:
    void PCR_Initialize (void);

  Summary:
    Initializes all the modules related to PCR.

  Description:
    This function initializes the clock as defined by the MCC and Clock Manager
    selections.

  Precondition:
    MCC GUI should be configured with the right values. Incorrect configuration
    of the Clock will result in incorrect peripheral behavior or a non
    functional device.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
        PCR_Initialize();
    </code>

  Remarks:
    This function should be called before calling other Clock library functions.
*/

void PCR_Initialize (void);

void PCR_PeripheralResetLock (void);

void PCR_PeripheralResetUnLock (void);

void PCR_PrivilegeEnLock (void);

void PCR_PrivilegeEnUnLock (void);

void PCR_SleepEnable0 (PCR_SLEEP_EN0 blockId);
void PCR_SleepDisable0 (PCR_SLEEP_EN0 blockId);
void PCR_SleepEnable1 (PCR_SLEEP_EN1 blockId);
void PCR_SleepDisable1 (PCR_SLEEP_EN1 blockId);
void PCR_SleepEnable3 (PCR_SLEEP_EN3 blockId);
void PCR_SleepDisable3 (PCR_SLEEP_EN3 blockId);
void PCR_SleepEnable4 (PCR_SLEEP_EN4 blockId);
void PCR_SleepDisable4 (PCR_SLEEP_EN4 blockId);

void PCR_PrivilegeEnable0 (PCR_PRIV_EN0 blockId);
void PCR_PrivilegeDisable0 (PCR_PRIV_EN0 blockId);
void PCR_PrivilegeEnable1 (PCR_PRIV_EN1 blockId);
void PCR_PrivilegeDisable1 (PCR_PRIV_EN1 blockId);
void PCR_PrivilegeEnable3 (PCR_PRIV_EN3 blockId);
void PCR_PrivilegeDisable3 (PCR_PRIV_EN3 blockId);
void PCR_PrivilegeEnable4 (PCR_PRIV_EN4 blockId);
void PCR_PrivilegeDisable4 (PCR_PRIV_EN4 blockId);

void PCR_ResetEnable0 (PCR_RESET_EN0 blockId);
void PCR_ResetDisable0 (PCR_RESET_EN0 blockId);
void PCR_ResetEnable1 (PCR_RESET_EN1 blockId);
void PCR_ResetDisable1 (PCR_RESET_EN1 blockId);
void PCR_ResetEnable3 (PCR_RESET_EN3 blockId);
void PCR_ResetDisable3 (PCR_RESET_EN3 blockId);
void PCR_ResetEnable4 (PCR_RESET_EN4 blockId);
void PCR_ResetDisable4 (PCR_RESET_EN4 blockId);


#ifdef __cplusplus // Provide C++ Compatibility
}
#endif

#endif /* PLIB_CLOCK_H */