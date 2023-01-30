/*******************************************************************************
  RTOS Timer Peripheral Library Interface Source File

  Company
    Microchip Technology Inc.

  File Name
    plib_rtos.c

  Summary
    RTOS peripheral library source file.

  Description
    This file implements the interface to the RTOS Timer peripheral library.  This
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


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "device.h"
#include "interrupts.h"
#include "plib_rtos_tmr.h"
#include "peripheral/ecia/plib_ecia.h"

static RTOS_TMR_OBJECT rtosTimerObj;

void RTOSTimer_Initialize(void)
{
    /* Disable RTOS timer. All registers are reset to the default state. */
    RTOS_REGS->RTOS_CTRL &= ~RTOS_CTRL_BLK_EN_Msk;
    
    RTOS_REGS->RTOS_PRLD = 32768;
    
    /* Enable timer module. Set Auto-reload and hardware halt values based on user settings. Do not start the timer. */
    RTOS_REGS->RTOS_CTRL = RTOS_CTRL_BLK_EN_Msk  | RTOS_CTRL_AU_RELOAD_Msk  ;
}


void RTOSTimer_Start(void)
{
    /* Loads pre-load register to the counter and starts the timer */
    RTOS_REGS->RTOS_CTRL |= RTOS_CTRL_TMR_STRT_Msk;
}

void RTOSTimer_Stop (void)
{
    /* Stops the timer, clears the counter to 0. Does not generate interrupt. */
    RTOS_REGS->RTOS_CTRL &= ~RTOS_CTRL_TMR_STRT_Msk;
}

void RTOSTimer_Halt(void)
{
    /* Loads preload register to the counter and starts the timer */
    RTOS_REGS->RTOS_CTRL |= RTOS_CTRL_FW_TMR_HALT_Msk;
}

void RTOSTimer_Resume(void)
{
    /* Stops the timer, clears the counter to 0. Does not generate interrupt. */
    RTOS_REGS->RTOS_CTRL &= ~RTOS_CTRL_FW_TMR_HALT_Msk;
}

void RTOSTimer_PeriodSet(uint32_t period)
{
    RTOS_REGS->RTOS_PRLD = period;
}

uint32_t RTOSTimer_PeriodGet(void)
{
    return RTOS_REGS->RTOS_PRLD;
}

uint32_t RTOSTimer_CounterGet(void)
{
    return RTOS_REGS->RTOS_CNT;
}

uint32_t RTOSTimer_FrequencyGet(void)
{
    return 32768;
}

void RTOSTimer_CallbackRegister( RTOS_TMR_CALLBACK callback_fn, uintptr_t context )
{
    rtosTimerObj.callback_fn = callback_fn;
    rtosTimerObj.context = context;
}

void RTMR_InterruptHandler(void)
{
    if (ECIA_GIRQResultGet(ECIA_DIR_INT_SRC_RTMR) != 0U)
    {
        ECIA_GIRQSourceClear(ECIA_DIR_INT_SRC_RTMR);

        if(rtosTimerObj.callback_fn != NULL)
        {
            rtosTimerObj.callback_fn(rtosTimerObj.context);
        }    
    }
}
