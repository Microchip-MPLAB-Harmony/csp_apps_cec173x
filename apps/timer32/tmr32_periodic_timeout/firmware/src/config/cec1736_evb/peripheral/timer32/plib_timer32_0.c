/*******************************************************************************
  32-bit Timer Peripheral Library Interface Source File

  Company
    Microchip Technology Inc.

  File Name
    plib_timer32_0.c

  Summary
    TIMER32_0 peripheral library source file.

  Description
    This file implements the interface to the 32-bit timer peripheral library.  This
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
#include "plib_timer32_0.h"
#include "peripheral/ecia/plib_ecia.h"

volatile static TMR32_OBJECT timer32_0Obj;

void TIMER32_0_Initialize(void)
{
    TIMER32_0_REGS->TIMER32_CTRL = TIMER32_CTRL_PRESCALE(0)  | TIMER32_CTRL_AU_RESTRT_Msk   | TIMER32_CTRL_EN_Msk;
    
    TIMER32_0_REGS->TIMER32_PRLD = 48000000;
    
    TIMER32_0_REGS->TIMER32_CNT = 48000000;
    
    TIMER32_0_REGS->TIMER32_IEN = TIMER32_IEN_Msk;
}

void TIMER32_0_PrescalerSet(uint16_t prescale_val)
{
    TIMER32_0_REGS->TIMER32_CTRL = (TIMER32_0_REGS->TIMER32_CTRL & ~TIMER32_CTRL_PRESCALE_Msk) | ((uint32_t)prescale_val << TIMER32_CTRL_PRESCALE_Pos);
}

uint32_t TIMER32_0_FrequencyGet(void)
{
    uint32_t prescale = ((TIMER32_0_REGS->TIMER32_CTRL & TIMER32_CTRL_PRESCALE_Msk) >> TIMER32_CTRL_PRESCALE_Pos) + 1U;
    return (48000000U/prescale);
}

void TIMER32_0_PreLoadCountSet(uint32_t count)
{
    TIMER32_0_REGS->TIMER32_PRLD = count;
}

uint32_t TIMER32_0_PreLoadCountGet(void)
{
    return TIMER32_0_REGS->TIMER32_PRLD;
}

void TIMER32_0_CounterSet(uint32_t count)
{
    TIMER32_0_REGS->TIMER32_CNT  = count;
}

uint32_t TIMER32_0_CounterGet(void)
{
    return TIMER32_0_REGS->TIMER32_CNT;
}

void TIMER32_0_Start(void)
{
    TIMER32_0_REGS->TIMER32_CTRL |= TIMER32_CTRL_STRT_Msk;
}

void TIMER32_0_Stop(void)
{
    TIMER32_0_REGS->TIMER32_CTRL &= ~TIMER32_CTRL_STRT_Msk;
}

void TIMER32_0_Reload(void)
{
    TIMER32_0_REGS->TIMER32_CTRL |= TIMER32_CTRL_RLD_Msk;
}

void TIMER32_0_AutoReStartEnable(void)
{
    TIMER32_0_REGS->TIMER32_CTRL |= TIMER32_CTRL_AU_RESTRT_Msk;
}

void TIMER32_0_AutoReStartDisable(void)
{
    TIMER32_0_REGS->TIMER32_CTRL &= ~TIMER32_CTRL_AU_RESTRT_Msk;
}

void TIMER32_0_CallbackRegister(TMR32_CALLBACK callback_fn, uintptr_t context )
{
    /* Save callback_fn and context in local memory */
    timer32_0Obj.callback_fn = callback_fn;
    timer32_0Obj.context = context;
}

void __attribute__((used)) TIMER32_0_InterruptHandler(void)
{
    if (ECIA_GIRQResultGet(ECIA_DIR_INT_SRC_TIMER32_0) != 0U)
    {
        ECIA_GIRQSourceClear(ECIA_DIR_INT_SRC_TIMER32_0);
        uint32_t status = TIMER32_0_REGS->TIMER32_STS;
        
        /* Clear the interrupt status bit */
        TIMER32_0_REGS->TIMER32_STS = TIMER32_STS_EVT_INT_Msk;
        
        if (timer32_0Obj.callback_fn != NULL)
        {
            uintptr_t context = timer32_0Obj.context;
            timer32_0Obj.callback_fn(status, context);
        }
    }
}
