/*******************************************************************************
  Breathing/Blinking LED Peripheral Library Interface Source File

  Company
    Microchip Technology Inc.

  File Name
    plib_led0.c

  Summary
    LED0 peripheral library source file.

  Description
    This file implements the interface to the Breathing/Blinking LED peripheral library.
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
#include "plib_led0.h"

#define NOP  asm("nop")

void LED0_Initialize(void)
{
    LED0_REGS->LED_CFG |= LED_CFG_RST_Msk;

    NOP;NOP;NOP;NOP;NOP;

    LED0_REGS->LED_CFG = LED_CFG_CTRL(1) | LED_CFG_SYMMETRY(0) | LED_CFG_PWM_SIZE(0x0);

    LED0_REGS->LED_LIMIT = LED_LIMIT_MIN(20) | LED_LIMIT_MAX(255);

    LED0_REGS->LED_DLY = LED_DLY_HIGH_PULSE(100) | LED_DLY_LOW_PULSE(100);

    LED0_REGS->LED_STEP = LED_STEP_S0(1) | LED_STEP_S1(2) | LED_STEP_S2(3) | LED_STEP_S3(4) | LED_STEP_S4(5) | LED_STEP_S5(6) | LED_STEP_S6(7) | LED_STEP_S7(8);

    LED0_REGS->LED_INTRVL = LED_INTRVL_I0(1) | LED_INTRVL_I1(2) | LED_INTRVL_I2(3) | LED_INTRVL_I3(4) | LED_INTRVL_I4(5) | LED_INTRVL_I5(6) | LED_INTRVL_I6(7) | LED_INTRVL_I7(8);


    LED0_REGS->LED_CFG |= LED_CFG_EN_UPDATE_Msk;
}

void LED0_DutyCycleMinSet(uint8_t min)
{
    LED0_REGS->LED_LIMIT = (LED0_REGS->LED_LIMIT & LED_LIMIT_MIN_Msk) | min;
}

void LED0_DutyCycleMaxSet(uint8_t max)
{
    LED0_REGS->LED_LIMIT = (LED0_REGS->LED_LIMIT & ~LED_LIMIT_MAX_Msk) | ((uint32_t)max << LED_LIMIT_MAX_Pos);
}

void LED0_LowDelaySet(uint16_t ld)
{
    LED0_REGS->LED_DLY = (LED0_REGS->LED_DLY & ~LED_DLY_LOW_PULSE_Msk) | (ld << LED_DLY_LOW_PULSE_Pos);
}

void LED0_HighDelaySet(uint16_t hd)
{
    LED0_REGS->LED_DLY = (LED0_REGS->LED_DLY & ~LED_DLY_HIGH_PULSE_Msk) | (hd << LED_DLY_HIGH_PULSE_Pos);
}

void LED0_SymmetrySet(LED_SYM sym)
{
    LED0_REGS->LED_CFG = (LED0_REGS->LED_CFG & ~LED_CFG_SYMMETRY_Msk) | ((uint32_t)sym << LED_CFG_SYMMETRY_Pos);
}

void LED0_SegmentsStepSizeSet(uint32_t stepSzVal)
{
    LED0_REGS->LED_STEP = stepSzVal;
}

void LED0_SegmentsIntervalSet(uint32_t intervalVal)
{
    LED0_REGS->LED_INTRVL = intervalVal;
}


void LED0_Update(void)
{
    LED0_REGS->LED_CFG |= LED_CFG_EN_UPDATE_Msk;
}



