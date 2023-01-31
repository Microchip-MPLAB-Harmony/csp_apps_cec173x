/*******************************************************************************
  WDT Peripheral Library

  Company:
    Microchip Technology Inc.

  File Name:
    plib_wdt.c

  Summary:
    WDT Source File

  Description:
    None

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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "device.h"
#include "interrupts.h"
#include "peripheral/ecia/plib_ecia.h"
#include "plib_wdt.h"

// *****************************************************************************
// *****************************************************************************
// Section: WDT Implementation
// *****************************************************************************
// *****************************************************************************





void WDT_Initialize( void )
{
    WDT_REGS->WDT_LOAD = 3972;



}
void WDT_Enable(void)
{
    WDT_REGS->WDT_CTRL |= WDT_CTRL_WDT_EN_Msk;
}

void WDT_Disable(void)
{
    WDT_REGS->WDT_CTRL &= ~WDT_CTRL_WDT_EN_Msk;
}

bool WDT_IsEnabled( void )
{
    return ((WDT_REGS->WDT_CTRL & WDT_CTRL_WDT_EN_Msk) != 0U);
}

void WDT_Clear(void)
{
    /* Clear WDT timer */
    WDT_REGS->WDT_KICK = 0;
}

uint16_t WDT_CountGet(void)
{
    /* Return WDT timer counter */
    return WDT_REGS->WDT_CNT;
}

bool WDT_isPowerFailWDTEventSet(void)
{
    bool check = false;
    if ((VTR_REG_BANK_REGS->VTR_REG_BANK_PFRS & VTR_REG_BANK_PFRS_WDT_EVT_Msk) != 0U)
    {
        check = true;
    }
    
    return check;
    
}

void WDT_PowerFailWDTEventClear(void)
{
    if ((VTR_REG_BANK_REGS->VTR_REG_BANK_PFRS & VTR_REG_BANK_PFRS_WDT_EVT_Msk) != 0U)
    {
        /* Write 1 to clear this bit */
        VTR_REG_BANK_REGS->VTR_REG_BANK_PFRS |= VTR_REG_BANK_PFRS_WDT_EVT_Msk;
    }
}

void WDT_PeriodLoad(uint16_t period)
{
    WDT_REGS->WDT_LOAD = period;
}

