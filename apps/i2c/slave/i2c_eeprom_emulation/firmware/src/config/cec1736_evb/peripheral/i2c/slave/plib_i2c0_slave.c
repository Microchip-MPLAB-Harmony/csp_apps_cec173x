/*******************************************************************************
  Inter-Integrated Circuit (I2C) Library
  Source File

  Company:
    Microchip Technology Inc.

  File Name:
    plib_i2c0_slave.c

  Summary:
    I2C PLIB Slave Implementation file

  Description:
    This file defines the interface to the I2C peripheral library.
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
#include "../plib_i2c_smb_common.h"
#include "plib_i2c0_slave.h"
#include "../../ecia/plib_ecia.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************
#define NOP()    asm("NOP")

static I2C_SLAVE_OBJ i2c0Obj;

void I2C0_Initialize(void)
{
    /* Reset I2C */
    SMB0_REGS->SMB_CFG[0] = SMB_CFG_RST_Msk;

    /* Reset bit must remain asserted for at-least 1 Baud clock period */
    NOP();NOP();NOP();NOP();NOP();
    NOP();NOP();NOP();NOP();NOP();
    NOP();NOP();NOP();NOP();NOP();
    NOP();NOP();NOP();NOP();NOP();

    /* Set the port associated with this instance of I2C peripheral */
    SMB0_REGS->SMB_CFG[0] = SMB_CFG_PORT_SEL(6);

    /* Set Slave address */
    SMB0_REGS->SMB_OWN_ADDR = SMB_OWN_ADDR_ADDR1(0x54) | SMB_OWN_ADDR_ADDR2(0x55);

    /* Repeated start hold time setup */
    SMB0_REGS->SMB_RSHTM = I2C_SMB_RecommendedValues[2][0];

    /* Data timing register */
    SMB0_REGS->SMB_DATATM = I2C_SMB_RecommendedValues[1][0];

    /* Enable I2C peripheral */
    SMB0_REGS->SMB_CFG[0] |= SMB_CFG_EN_Msk;

    /* Enable Serial output, enable I2C interrupt and set ACK bit */
    SMB0_REGS->SMB_WCTRL = (SMB_WCTRL_ESO_Msk | SMB_WCTRL_ENI_Msk | SMB_WCTRL_ACK_Msk);

    i2c0Obj.callback = NULL;
    i2c0Obj.error = I2C_SLAVE_ERROR_NONE;
}

/* I2C slave state machine */
static void I2C0_TransferSM(void)
{
    uint32_t i2c_addr;
    uint8_t dummy;

    if ((SMB0_REGS->SMB_RSTS & SMB_RSTS_BER_Msk) != 0U)
    {
        i2c0Obj.error = I2C_SLAVE_ERROR_BUS_COLLISION;

        if (i2c0Obj.callback != NULL)
        {
            /* In the callback, slave must read out the error by calling I2Cx_ErrorGet() */
            (void)i2c0Obj.callback(I2C_SLAVE_TRANSFER_EVENT_ERROR, i2c0Obj.context);
        }

        return;
    }
    /* Check if addressed as slave (AAS) bit is set */
    if ((SMB0_REGS->SMB_RSTS & SMB_RSTS_AAS_Msk) != 0U)
    {
        i2c_addr = SMB0_REGS->SMB_I2CDATA;

        i2c0Obj.transferDir = ((i2c_addr & 0x01U) != 0U)? I2C_SLAVE_TRANSFER_DIR_READ : I2C_SLAVE_TRANSFER_DIR_WRITE;

        if (i2c0Obj.callback != NULL)
        {
            (void)i2c0Obj.callback(I2C_SLAVE_TRANSFER_EVENT_ADDR_MATCH, i2c0Obj.context);

            if (i2c0Obj.transferDir == I2C_SLAVE_TRANSFER_DIR_READ)
            {
                /* I2C master wants to read (slave transmit) */
                /* In the callback, slave must write to transmit register by calling I2Cx_WriteByte() */
                (void)i2c0Obj.callback(I2C_SLAVE_TRANSFER_EVENT_TX_READY, i2c0Obj.context);
            }
        }
    }
    // Master reading from slave
    else if (i2c0Obj.transferDir == I2C_SLAVE_TRANSFER_DIR_READ)
    {
        /* I2C master reading from slave */
        if ((SMB0_REGS->SMB_RSTS & SMB_RSTS_LRB_AD0_Msk) == 0U)
        {
            /* ACK received, continue transmitting */
            if (i2c0Obj.callback != NULL)
            {
                /* In the callback, slave must write to transmit register by calling I2Cx_WriteByte() */
                (void)i2c0Obj.callback(I2C_SLAVE_TRANSFER_EVENT_TX_READY, i2c0Obj.context);
            }
        }
        else
        {
            /* NAK received, stop transmission. Dummy write to clear the PIN status bit */
            SMB0_REGS->SMB_I2CDATA = 0xFFU;

            if (i2c0Obj.callback != NULL)
            {
                (void)i2c0Obj.callback(I2C_SLAVE_TRANSFER_EVENT_NAK_RECEIVED, i2c0Obj.context);
            }
        }
    }
    // Master writing to slave
    else
    {
        /* I2C master writing to slave */
        if ((SMB0_REGS->SMB_RSTS & SMB_RSTS_STS_Msk) != 0U)
        {
            /* Stop received, do a dummy read to clear the PIN status bit */
            dummy = SMB0_REGS->SMB_I2CDATA;
            dummy = dummy;

            if (i2c0Obj.callback != NULL)
            {
                (void)i2c0Obj.callback(I2C_SLAVE_TRANSFER_EVENT_STOP_BIT_RECEIVED, i2c0Obj.context);
            }
        }
        else
        {
            if (i2c0Obj.callback != NULL)
            {
                /* In the callback, slave must read data by calling I2Cx_ReadByte()  */
                (void)i2c0Obj.callback(I2C_SLAVE_TRANSFER_EVENT_RX_READY, i2c0Obj.context);
            }
        }
    }
}

uint8_t I2C0_ReadByte(void)
{
    return SMB0_REGS->SMB_I2CDATA;
}

void I2C0_WriteByte(uint8_t wrByte)
{
    SMB0_REGS->SMB_I2CDATA = wrByte;
}

uint8_t I2C0_StatusFlagsGet(void)
{
    return (uint8_t)(SMB0_REGS->SMB_RSTS & SMB_RSTS_Msk);
}

void I2C0_StatusFlagsReset(void)
{
    SMB0_REGS->SMB_WCTRL |= SMB_WCTRL_PIN_Msk;
}

I2C_SLAVE_ACK_STATUS I2C0_LastByteAckStatusGet(void)
{
    return ((SMB0_REGS->SMB_RSTS & SMB_RSTS_LRB_AD0_Msk) != 0U) ? I2C_SLAVE_ACK_STATUS_RECEIVED_NAK : I2C_SLAVE_ACK_STATUS_RECEIVED_ACK;
}

void I2C0_CallbackRegister(I2C_SLAVE_CALLBACK callback, uintptr_t contextHandle)
{
    if (callback == NULL)
    {
        return;
    }

    i2c0Obj.callback = callback;
    i2c0Obj.context = contextHandle;
}

bool I2C0_IsBusy(void)
{
    if ((SMB0_REGS->SMB_RSTS & SMB_RSTS_NBB_Msk) == 0U)
    {
        return true;
    }
    else
    {
        return false;
    }
}

I2C_SLAVE_TRANSFER_DIR I2C0_TransferDirGet(void)
{
    return i2c0Obj.transferDir;
}

I2C_SLAVE_ERROR I2C0_ErrorGet(void)
{
    I2C_SLAVE_ERROR error;

    error = i2c0Obj.error;
    i2c0Obj.error = I2C_SLAVE_ERROR_NONE;

    return error;
}

static void I2C0_SLAVE_InterruptHandler(void)
{
    I2C0_TransferSM();
}

void I2CSMB0_InterruptHandler(void)
{
    if (ECIA_GIRQResultGet(ECIA_DIR_INT_SRC_I2CSMB0))
    {
        I2C0_SLAVE_InterruptHandler();
        
        ECIA_GIRQSourceClear(ECIA_DIR_INT_SRC_I2CSMB0);
    }
}