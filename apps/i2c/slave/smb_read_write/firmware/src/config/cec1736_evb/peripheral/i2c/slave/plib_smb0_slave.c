/*******************************************************************************
  Inter-Integrated Circuit (I2C) Library
  Source File

  Company:
    Microchip Technology Inc.

  File Name:
    plib_smb0_slave.c

  Summary:
    I2C PLIB SMBUS Slave Implementation file

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
#include "plib_smb0_slave.h"

#include "../plib_i2c_smb_common.h"
#include "../../ecia/plib_ecia.h"
#include "../../dma/plib_dma.h"
// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************
#define NOP()    asm("NOP")
#define SMB0_STXB   (uint32_t*)(SMB0_BASE_ADDRESS + SMB_SLV_TXB_REG_OFST)
#define SMB0_SRXB   (uint32_t*)(SMB0_BASE_ADDRESS + SMB_SLV_RXB_REG_OFST)

volatile static I2C_SMB_TARGET_OBJ smb0TargetObj;
volatile static uint8_t i2csmb0TargetWrBuffer[64];
volatile static uint8_t i2csmb0TargetRdBuffer[64];

void I2CSMB0_Initialize(void)
{
    /* Reset I2C */
    SMB0_REGS->SMB_CFG[0] = SMB_CFG_RST_Msk;

    /* Reset bit must remain asserted for at-least 1 Baud clock period */
    NOP();NOP();NOP();NOP();NOP();

    SMB0_REGS->SMB_CFG[0] &= ~SMB_CFG_RST_Msk;

    /* Set the port associated with this instance of I2C peripheral */
    SMB0_REGS->SMB_CFG[0] = SMB_CFG_PORT_SEL(6);

    /* Set Slave address */
    SMB0_REGS->SMB_OWN_ADDR = SMB_OWN_ADDR_ADDR1(0x54) | SMB_OWN_ADDR_ADDR2(0x55);

    SMB0_REGS->SMB_PEC = 0;

    /* Repeated start hold time setup */
    SMB0_REGS->SMB_RSHTM = I2C_SMB_RecommendedValues[2][0];

    /* Data timing register */
    SMB0_REGS->SMB_DATATM = I2C_SMB_RecommendedValues[1][0];


    /* Enable I2C peripheral */
    SMB0_REGS->SMB_CFG[0] |= SMB_CFG_EN_Msk | SMB_CFG_PECEN_Msk  ;

    /* Enable Serial output, enable I2C interrupt and set ACK bit */
    SMB0_REGS->SMB_WCTRL = (SMB_WCTRL_ESO_Msk | SMB_WCTRL_ACK_Msk);

    smb0TargetObj.callback = NULL;
    smb0TargetObj.error = I2C_SMB_TARGET_ERROR_NONE;
}


static void I2CSMB0_TargetReInitialize(void)
{
    /* Reset I2C */
    SMB0_REGS->SMB_CFG[0] = SMB_CFG_RST_Msk;

    /* Reset bit must remain asserted for at-least 1 Baud clock period */
    NOP();NOP();NOP();NOP();NOP();

    SMB0_REGS->SMB_CFG[0] &= ~SMB_CFG_RST_Msk;

    /* Set the port associated with this instance of I2C peripheral */
    SMB0_REGS->SMB_CFG[0] = SMB_CFG_PORT_SEL(6);

    /* Set Slave address */
    SMB0_REGS->SMB_OWN_ADDR = SMB_OWN_ADDR_ADDR1(0x54) | SMB_OWN_ADDR_ADDR2(0x55);

    SMB0_REGS->SMB_PEC = 0;

    /* Repeated start hold time setup */
    SMB0_REGS->SMB_RSHTM = I2C_SMB_RecommendedValues[2][0];

    /* Data timing register */
    SMB0_REGS->SMB_DATATM = I2C_SMB_RecommendedValues[1][0];


    /* Enable I2C peripheral */
    SMB0_REGS->SMB_CFG[0] |= SMB_CFG_EN_Msk | SMB_CFG_PECEN_Msk  ;

    /* Enable Serial output, enable I2C interrupt and set ACK bit */
    SMB0_REGS->SMB_WCTRL = (SMB_WCTRL_ESO_Msk | SMB_WCTRL_ACK_Msk);
}

void I2CSMB0_TargetCallbackRegister(I2C_SMB_TARGET_CALLBACK callback, uintptr_t contextHandle)
{
    if (callback == NULL)
    {
        return;
    }

    smb0TargetObj.callback = callback;
    smb0TargetObj.context = contextHandle;
}

bool I2CSMB0_TargetIsBusy(void)
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

I2C_SMB_TARGET_TRANSFER_DIR I2CSMB0_TargetTransferDirGet(void)
{
    return smb0TargetObj.transferDir;
}

I2C_SMB_TARGET_ERROR I2CSMB0_TargetErrorGet(void)
{
    I2C_SMB_TARGET_ERROR error;

    error = smb0TargetObj.error;
    smb0TargetObj.error = I2C_SMB_TARGET_ERROR_NONE;

    return error;
}

void I2CSMB0_TargetStart(void)
{
    uint8_t PECConfig = ((SMB0_REGS->SMB_CFG[0] & SMB_CFG_PECEN_Msk) != 0U)? 1U: 0U;

    smb0TargetObj.dmaDir = I2C_SMB_TARGET_DMA_DIR_PER_TO_MEM;

    /* Configure DMA for Slave RX */
    (void)DMA_ChannelTransfer(DMA_CHANNEL_0, SMB0_SRXB, i2csmb0TargetRdBuffer, sizeof(i2csmb0TargetRdBuffer));

    /* Enable SDONE (Slave Done) interrupt */
    SMB0_REGS->SMB_CFG[0] |= SMB_CFG_ENSI_Msk;

    /* Set the target command and start the transfer */
    SMB0_REGS->SMB_SCMD[0] = SMB_SCMD_RD_CNT(sizeof(i2csmb0TargetRdBuffer)) | SMB_SCMD_SRUN_Msk | SMB_SCMD_SPROCEED_Msk | SMB_SCMD_PEC(PECConfig);
}

uint32_t I2CSMB0_TargetBufferRead(void* pBuffer)
{
    uint32_t i;
    uint32_t numBytesAvailable = smb0TargetObj.rxCount;

    for (i = 0; i < numBytesAvailable; i++)
    {
        ((uint8_t*)pBuffer)[i] = i2csmb0TargetRdBuffer[i];
    }

    smb0TargetObj.rxCount = 0;

    return numBytesAvailable;
}

void I2CSMB0_TargetBufferWrite(void* pBuffer, uint32_t nBytes)
{
    uint32_t i = 0;

    for (i = 0; i < nBytes; i++)
    {
        i2csmb0TargetWrBuffer[i] = ((uint8_t*)pBuffer)[i];
    }

    smb0TargetObj.txCount = nBytes;
}

void __attribute__((used)) I2CSMB0_TargetInterruptHandler(uint32_t completion_reg)
{
    uint8_t PECConfig = ((SMB0_REGS->SMB_CFG[0] & SMB_CFG_PECEN_Msk) != 0U)? 1U: 0U;
    uintptr_t context = smb0TargetObj.context;

    if ((completion_reg & SMB_COMPL_SDONE_Msk) != 0U)
    {
        DMA_ChannelTransferAbort(DMA_CHANNEL_0);

        if ((completion_reg & SMB_COMPL_BER_Msk) != 0U)
        {
            smb0TargetObj.error |= I2C_SMB_TARGET_ERROR_BUS_COLLISION;
        }
        if ((completion_reg & SMB_COMPL_TIMERR_Msk) != 0U)
        {
            smb0TargetObj.error |= I2C_SMB_TARGET_ERROR_TIMEOUT;
        }

        I2C_SMB_TARGET_ERROR error = smb0TargetObj.error;

        if (((SMB0_REGS->SMB_SCMD[0] & SMB_SCMD_SRUN_Msk) != 0U) && (error == I2C_SMB_TARGET_ERROR_NONE))
        {
            if ((SMB0_REGS->SMB_SCMD[0] & SMB_SCMD_SPROCEED_Msk) == 0U)
            {
                if (((completion_reg & SMB_COMPL_REP_RD_Msk) != 0U) || ((completion_reg & SMB_COMPL_REP_WR_Msk) != 0U))
                {
                    if (smb0TargetObj.dmaDir == I2C_SMB_TARGET_DMA_DIR_PER_TO_MEM)
                    {
                        /* Read DMA transfer count and discard the address byte and the address byte received after repeated start. Additionally discard the PEC register written by slave state machine if PEC is enabled. */
                        smb0TargetObj.rxCount = (DMA_ChannelGetTransferredCount(DMA_CHANNEL_0) - 2U) - ((PECConfig == 1U)? 1U : 0U);

                        if (smb0TargetObj.callback != NULL)
                        {
                            (void)smb0TargetObj.callback(I2C_SMB_TARGET_TRANSFER_EVENT_RX_READY, context);
                        }
                    }
                    else
                    {
                        /* Target was transmitting. Check and notify if WR_COUNT became 0 before Host sent NAK or Host sent NAK before WR_COUNT became 0. */
                        if ((completion_reg & SMB_COMPL_SPROT_Msk) != 0U)
                        {
                            smb0TargetObj.error |= I2C_SMB_TARGET_ERROR_SPROT;
                            (void)smb0TargetObj.callback(I2C_SMB_TARGET_TRANSFER_EVENT_ERROR, context);
                        }
                    }
                    if ((completion_reg & SMB_COMPL_REP_RD_Msk) != 0U)
                    {
                        if (smb0TargetObj.callback != NULL)
                        {
                            /* Application is expected to frame the response and make the response available by calling the I2CSMB0_TargetBufferWrite() API */
                            (void)smb0TargetObj.callback(I2C_SMB_TARGET_TRANSFER_EVENT_TX_READY, context);
                        }

                        smb0TargetObj.transferDir = I2C_SMB_TARGET_TRANSFER_DIR_READ;
                    }
                    else
                    {
                        smb0TargetObj.transferDir = I2C_SMB_TARGET_TRANSFER_DIR_WRITE;
                    }
                }
                else if (((SMB0_REGS->SMB_RSTS & SMB_RSTS_AAS_Msk) != 0U) && ((SMB0_REGS->SMB_SHDW_DATA & 0x01U) != 0U))
                {
                    /* This is executed when Host sends a Read request after a normal start bit (not repeated start). Application is expected to frame the response and make the response available by calling the I2CSMB0_SMBUSBufferWrite() API */
                    (void)smb0TargetObj.callback(I2C_SMB_TARGET_TRANSFER_EVENT_TX_READY, context);
                    smb0TargetObj.transferDir = I2C_SMB_TARGET_TRANSFER_DIR_READ;
                }
                else
                {
                    /* Do nothing */
                }

                smb0TargetObj.error = I2C_SMB_TARGET_ERROR_NONE;

                if (smb0TargetObj.transferDir == I2C_SMB_TARGET_TRANSFER_DIR_WRITE)
                {
                    /* Enter here when a Repeated Write request is received. Configure DMA for Target RX (peripheral to memory). */
                    (void)DMA_ChannelTransfer(DMA_CHANNEL_0, SMB0_SRXB, i2csmb0TargetRdBuffer, sizeof(i2csmb0TargetRdBuffer));

                    /* Set the target command and start the transfer */
                    SMB0_REGS->SMB_SCMD[0] = SMB_SCMD_RD_CNT(sizeof(i2csmb0TargetRdBuffer)) | SMB_SCMD_SRUN_Msk | SMB_SCMD_SPROCEED_Msk | SMB_SCMD_PEC(PECConfig);

                    smb0TargetObj.dmaDir = I2C_SMB_TARGET_DMA_DIR_PER_TO_MEM;
                }
                else
                {
                    /* Enter here when Repeated Read request is received. Configure DMA for Target TX (memory to peripheral) */
                    (void)DMA_ChannelTransfer(DMA_CHANNEL_0, i2csmb0TargetWrBuffer, SMB0_STXB, sizeof(i2csmb0TargetWrBuffer));

                    /* Set the target command and start the transfer */
                    SMB0_REGS->SMB_SCMD[0] = SMB_SCMD_WR_CNT(smb0TargetObj.txCount) | SMB_SCMD_SRUN_Msk | SMB_SCMD_SPROCEED_Msk | SMB_SCMD_PEC(PECConfig);

                    smb0TargetObj.dmaDir = I2C_SMB_TARGET_DMA_DIR_MEM_TO_PER;
                }
            }
        }
        else
        {
            if (smb0TargetObj.error == I2C_SMB_TARGET_ERROR_NONE)
            {
                if (smb0TargetObj.dmaDir == I2C_SMB_TARGET_DMA_DIR_PER_TO_MEM)
                {
                    /* Target was receiving. Discard the address byte */
                    smb0TargetObj.rxCount = DMA_ChannelGetTransferredCount(DMA_CHANNEL_0) - 1U;

                    if (PECConfig == 1U)
                    {
                        if (SMB0_REGS->SMB_SLV_RXB != 0U)
                        {
                            smb0TargetObj.error |= I2C_SMB_TARGET_ERROR_PEC;
                        }
                        /* Discard the PEC byte sent by master (last byte) */
                        smb0TargetObj.rxCount -= 1U;
                    }

                    if (smb0TargetObj.callback != NULL)
                    {
                        if (smb0TargetObj.error == I2C_SMB_TARGET_ERROR_NONE)
                        {
                            (void)smb0TargetObj.callback(I2C_SMB_TARGET_TRANSFER_EVENT_RX_READY, context);
                        }
                    }
                }
                else
                {
                    /* Target was transmitting. Check and notify if WR_COUNT became 0 before Host sent NAK or Host sent NAK before WR_COUNT became 0. */
                    if ((completion_reg & SMB_COMPL_SPROT_Msk) != 0U)
                    {
                        smb0TargetObj.error |= I2C_SMB_TARGET_ERROR_SPROT;
                    }
                }
            }

            if ((smb0TargetObj.error & (I2C_SMB_TARGET_ERROR_TIMEOUT | I2C_SMB_TARGET_ERROR_BUS_COLLISION)) != 0U)
            {
                /* Controller must be reset and then re-initialized */
                I2CSMB0_TargetReInitialize();

                /* Enable SDONE (Slave Done) interrupt */
                SMB0_REGS->SMB_CFG[0] |= SMB_CFG_ENSI_Msk;
            }

            if (smb0TargetObj.callback != NULL)
            {
                if (smb0TargetObj.error == I2C_SMB_TARGET_ERROR_NONE)
                {
                    (void)smb0TargetObj.callback(I2C_SMB_TARGET_TRANSFER_EVENT_DONE, context);
                }
                else
                {
                    (void)smb0TargetObj.callback(I2C_SMB_TARGET_TRANSFER_EVENT_ERROR, context);
                }
            }

            smb0TargetObj.error = I2C_SMB_TARGET_ERROR_NONE;
            smb0TargetObj.dmaDir = I2C_SMB_TARGET_DMA_DIR_PER_TO_MEM;

            (void)DMA_ChannelTransfer(DMA_CHANNEL_0, SMB0_SRXB, i2csmb0TargetRdBuffer, sizeof(i2csmb0TargetRdBuffer));
            /* Set the target command and start the transfer */
            SMB0_REGS->SMB_SCMD[0] = SMB_SCMD_RD_CNT(sizeof(i2csmb0TargetRdBuffer)) | SMB_SCMD_SRUN_Msk | SMB_SCMD_SPROCEED_Msk | SMB_SCMD_PEC(PECConfig);
        }
    }
}

void __attribute__((used)) I2CSMB0_InterruptHandler(void)
{
    uint32_t completion_reg;

    if (ECIA_GIRQResultGet(ECIA_DIR_INT_SRC_I2CSMB0) != 0U)
    {
        completion_reg = SMB0_REGS->SMB_COMPL[0];

        /* Clear the status bits */
        SMB0_REGS->SMB_COMPL[0] = completion_reg;

        I2CSMB0_TargetInterruptHandler(completion_reg);

        ECIA_GIRQSourceClear(ECIA_DIR_INT_SRC_I2CSMB0);
    }
}
