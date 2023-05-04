/*******************************************************************************
  Hibernation Timer Peripheral Library Interface Source File

  Company
    Microchip Technology Inc.

  File Name
    plib_htm0.c

  Summary
    HTM0 peripheral library source file.

  Description
    This file implements the interface to the Hibernation Timer peripheral library.
    This library provides access to and control of the associated peripheral
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
#include "plib_htm0.h"
#include "peripheral/ecia/plib_ecia.h"

volatile static HTM_TMR_OBJECT htm0Obj;

void HTM0_Initialize(void)
{

    HTM0_REGS->HTM_PRLD = 32768;
}


void HTM0_PeriodSet(uint16_t preload_val)
{
    /* Writing non-zero value resets and starts the hibernation timer */
    HTM0_REGS->HTM_PRLD = preload_val;
}

uint16_t HTM0_PeriodGet(void)
{
    return HTM0_REGS->HTM_PRLD;
}

uint16_t HTM0_CountGet (void)
{
    return HTM0_REGS->HTM_CNT;
}

void HTM0_ResolutionSet(HTM_TMR_RESOLUTION resolution)
{
    HTM0_REGS->HTM_CTRL = ((uint16_t)resolution << HTM_CTRL_CTRL_Pos);
}

void HTM0_CallbackRegister( HTM_TMR_CALLBACK callback_fn, uintptr_t context )
{
    htm0Obj.callback_fn = callback_fn;
    htm0Obj.context = context;
}

void __attribute__((used)) HTMR0_InterruptHandler(void)
{
    if (ECIA_GIRQResultGet(ECIA_DIR_INT_SRC_HTMR0) != 0U)
    {
        ECIA_GIRQSourceClear(ECIA_DIR_INT_SRC_HTMR0);

        if(htm0Obj.callback_fn != NULL)
        {
            uintptr_t context = htm0Obj.context;
            htm0Obj.callback_fn(context);
        }
    }
}
