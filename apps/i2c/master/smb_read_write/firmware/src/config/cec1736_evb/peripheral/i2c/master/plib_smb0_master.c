/*******************************************************************************
  Inter-Integrated Circuit (I2C) Library
  Source File

  Company:
    Microchip Technology Inc.

  File Name:
    plib_smb0_master.c

  Summary:
    I2C PLIB Master Mode Implementation file

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
#include "plib_smb0_master.h"
#include "../plib_i2c_smb_common.h"
#include "../../ecia/plib_ecia.h"
#include "../../dma/plib_dma.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************
#define NOP()    asm("NOP")
#define SMB0_MTXB   (uint32_t*)(SMB0_BASE_ADDRESS + SMB_MTR_TXB_REG_OFST)
#define SMB0_MRXB   (uint32_t*)(SMB0_BASE_ADDRESS + SMB_MTR_RXB_REG_OFST)

volatile static I2C_SMB_HOST_OBJ smb0HostObj;
static uint8_t i2csmb0HostWrBuffer[64];
volatile static uint8_t i2csmb0HostRdBuffer[64];

void I2CSMB0_Initialize(void)
{
    /* Reset I2C */
    SMB0_REGS->SMB_CFG[0] = SMB_CFG_RST_Msk;

    /* Reset bit must remain asserted for at-least 1 Baud clock period */
    NOP();NOP();NOP();NOP();NOP();

    SMB0_REGS->SMB_CFG[0] &= ~SMB_CFG_RST_Msk;

    /* Set the port associated with this instance of I2C peripheral */
    SMB0_REGS->SMB_CFG[0] = SMB_CFG_PORT_SEL(6);

    SMB0_REGS->SMB_PEC = 0;

    /* Repeated start hold time setup */
    SMB0_REGS->SMB_RSHTM = I2C_SMB_RecommendedValues[2][1];

    /* Data timing register */
    SMB0_REGS->SMB_DATATM = I2C_SMB_RecommendedValues[1][1];


    /* Enable I2C peripheral */
    SMB0_REGS->SMB_CFG[0] |= SMB_CFG_EN_Msk | SMB_CFG_PECEN_Msk  ;

    /* I2C bus clock frequency */
    SMB0_REGS->SMB_BUSCLK = SMB_BUSCLK_HIGH_PER(19) | SMB_BUSCLK_LOW_PER(19);

    /* Enable Serial output and set ACK bit */
    SMB0_REGS->SMB_WCTRL = (SMB_WCTRL_ESO_Msk | SMB_WCTRL_ACK_Msk);

    smb0HostObj.callback = NULL;

    smb0HostObj.state = I2C_SMB_HOST_STATE_IDLE;

    smb0HostObj.error = I2C_SMB_HOST_ERROR_NONE;
}


static void I2CSMB0_HostReInitialize(void)
{
    /* Reset I2C */
    SMB0_REGS->SMB_CFG[0] = SMB_CFG_RST_Msk;

    /* Reset bit must remain asserted for at-least 1 Baud clock period */
    NOP();NOP();NOP();NOP();NOP();

    SMB0_REGS->SMB_CFG[0] &= ~SMB_CFG_RST_Msk;

    /* Set the port associated with this instance of I2C peripheral */
    SMB0_REGS->SMB_CFG[0] = SMB_CFG_PORT_SEL(6);

    SMB0_REGS->SMB_PEC = 0;

    /* Repeated start hold time setup */
    SMB0_REGS->SMB_RSHTM = I2C_SMB_RecommendedValues[2][1];

    /* Data timing register */
    SMB0_REGS->SMB_DATATM = I2C_SMB_RecommendedValues[1][1];


    /* Enable I2C peripheral */
    SMB0_REGS->SMB_CFG[0] |= SMB_CFG_EN_Msk | SMB_CFG_PECEN_Msk  ;

    /* I2C bus clock frequency */
    SMB0_REGS->SMB_BUSCLK = SMB_BUSCLK_HIGH_PER(19) | SMB_BUSCLK_LOW_PER(19);

    /* Enable Serial output and set ACK bit */
    SMB0_REGS->SMB_WCTRL = (SMB_WCTRL_ESO_Msk | SMB_WCTRL_ACK_Msk);
}

bool I2CSMB0_HostIsBusy(void)
{
    if((SMB0_REGS->SMB_COMPL[0] & SMB_COMPL_IDLE_Msk) != 0U)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void I2CSMB0_HostCallbackRegister(I2C_SMB_HOST_CALLBACK callback, uintptr_t contextHandle)
{
    if (callback == NULL)
    {
        return;
    }

    smb0HostObj.callback = callback;
    smb0HostObj.context = contextHandle;
}

I2C_SMB_HOST_ERROR I2CSMB0_HostErrorGet(void)
{
    I2C_SMB_HOST_ERROR error;

    error = smb0HostObj.error;
    smb0HostObj.error = I2C_SMB_HOST_ERROR_NONE;

    return error;
}

bool I2CSMB0_HostTransferSetup(I2C_SMB_HOST_TRANSFER_SETUP* setup, uint32_t srcClkFreq )
{
    uint32_t high_low_period;
    uint32_t i2cBusFreq;
    uint32_t timingValuesIndex = 0;
    float temp;

    if ((smb0HostObj.state != I2C_SMB_HOST_STATE_IDLE) || (setup == NULL))
    {
        return false;
    }

    i2cBusFreq = setup->clkSpeed;

    /* Maximum I2C clock speed cannot be greater than 1 MHz */
    if (i2cBusFreq > 1000000U)
    {
        return false;
    }

    if( srcClkFreq == 0U)
    {
        srcClkFreq = 16000000UL;
    }

    temp = ((((float)srcClkFreq/(float)i2cBusFreq)/2.0f) - 1.0f);

    high_low_period = (uint32_t)temp;

    /* high/low baud period value cannot be greater than 255 */
    if (high_low_period > 255U)
    {
        high_low_period = 255U;
    }

    /* Clear the ESO bit(Serial output) before changing the baud value */
    SMB0_REGS->SMB_WCTRL &= ~SMB_WCTRL_ESO_Msk;

    SMB0_REGS->SMB_BUSCLK = SMB_BUSCLK_HIGH_PER(high_low_period) | SMB_BUSCLK_LOW_PER(high_low_period);

    if (i2cBusFreq < 250000U)
    {
        timingValuesIndex = 0;
    }
    else if (i2cBusFreq < 750000U)
    {
        timingValuesIndex = 1U;
    }
    else
    {
        timingValuesIndex  = 2U;
    }

    /* Repeated start hold time setup */
    SMB0_REGS->SMB_RSHTM = I2C_SMB_RecommendedValues[2][timingValuesIndex];

    /* Data timing register */
    SMB0_REGS->SMB_DATATM = I2C_SMB_RecommendedValues[1][timingValuesIndex];

    /* Timeout scaling register setup */
    SMB0_REGS->SMB_TMOUTSC = I2C_SMB_RecommendedValues[4][timingValuesIndex];

    /* Set the ESO bit (Serial output) after changing the baud value */
    SMB0_REGS->SMB_WCTRL |= SMB_WCTRL_ESO_Msk;

    return true;
}

static uint8_t I2CSMB0_HostPacketForm(I2C_SMB_HOST_PROTOCOL protocol, uint8_t address, uint8_t cmd, void* pWrdata, uint32_t nWrBytes)
{
    uint8_t* pProtoBuff = i2csmb0HostWrBuffer;
    uint8_t len = 0;

    pProtoBuff[len++] = (address << 1) | (uint8_t)I2C_SMB_HOST_TRANSFER_TYPE_WRITE;
    pProtoBuff[len++] = cmd;

    switch(protocol)
    {
        case I2C_SMB_HOST_PROTOCOL_WR_BYTE:
        case I2C_SMB_HOST_PROTOCOL_WR_WORD:
            /* Nothing to do here */
            break;
        case I2C_SMB_HOST_PROTOCOL_WR_BLK:
        case I2C_SMB_HOST_PROTOCOL_WR_BLK_RD_BLK:
            pProtoBuff[len++] = (uint8_t)nWrBytes;
            break;
        case I2C_SMB_HOST_PROTOCOL_RD_BYTE:
        case I2C_SMB_HOST_PROTOCOL_RD_WORD:
        case I2C_SMB_HOST_PROTOCOL_RD_BLK:
            pProtoBuff[len++] = (address << 1) | (uint8_t)I2C_SMB_HOST_TRANSFER_TYPE_READ;
            break;
        default:
            /* Default case, should not enter here. */
            break;
    }

    if (nWrBytes != 0U)
    {
        (void)memcpy((void*)&pProtoBuff[len], (const void*)pWrdata, nWrBytes);
        len += (uint8_t)nWrBytes;
    }
    if (protocol == I2C_SMB_HOST_PROTOCOL_WR_BLK_RD_BLK)
    {
        pProtoBuff[len++] = (address << 1) | I2C_SMB_HOST_TRANSFER_TYPE_READ;
    }

    return len;
}

static void I2CSMB0_HostWriteRead(I2C_SMB_HOST_PROTOCOL protocol, uint8_t nWrBytes, uint32_t nRdBytes)
{
    uint32_t hostCmd = 0;
    uint8_t PECConfig = ((SMB0_REGS->SMB_CFG[0] & SMB_CFG_PECEN_Msk) != 0U)? 1U: 0U;

    smb0HostObj.dmaDirChange = false;

    smb0HostObj.error = I2C_SMB_HOST_ERROR_NONE;

    /* Start by flushing off host transmit and receive buffers */
    SMB0_REGS->SMB_CFG[0] |= SMB_CFG_FLUSH_MRBUF_Msk | SMB_CFG_FLUSH_MXBUF_Msk;

    /* Configure DMA transfer for the write part (memory to peripheral) of the transfer. DMA channel will be reconfigured for reading (Peripheral to memory ) in the I2CSMB interrupt, once the write part is complete. */
    (void)DMA_ChannelTransfer(DMA_CHANNEL_0, i2csmb0HostWrBuffer, SMB0_MTXB, nWrBytes);

    smb0HostObj.state = I2C_SMB_HOST_STATE_TRANSMIT;

    hostCmd = (SMB_MCMD_WR_CNT(nWrBytes) | SMB_MCMD_START0_Msk | SMB_MCMD_MPROCEED_Msk | SMB_MCMD_MRUN_Msk | SMB_MCMD_STOP_Msk);

    if ((protocol == I2C_SMB_HOST_PROTOCOL_RD_BYTE) || (protocol == I2C_SMB_HOST_PROTOCOL_RD_WORD) || (protocol == I2C_SMB_HOST_PROTOCOL_RD_BLK) || (protocol == I2C_SMB_HOST_PROTOCOL_WR_BLK_RD_BLK))
    {
        /* Specify the number of bytes to read from target, generate repeated start and read PEC byte when RD_CNT becomes 0 */
        hostCmd |= (SMB_MCMD_RD_CNT(nRdBytes) | SMB_MCMD_STARTN_Msk) | SMB_MCMD_READ_PEC(PECConfig);
        smb0HostObj.dmaRdBytes = nRdBytes + (PECConfig == 1U? 2U : 0U);
        smb0HostObj.dmaDirChange = true;
    }
    else
    {
        /* Transmit PEC once the WR_CNT becomes 0 */
        hostCmd |= SMB_MCMD_PEC_TERM(PECConfig);
    }

    if ((protocol == I2C_SMB_HOST_PROTOCOL_RD_BLK) || (protocol == I2C_SMB_HOST_PROTOCOL_WR_BLK_RD_BLK))
    {
        /* Set RD_CNT with max possible read count. It will be over-written (READM) by the first byte received from I2C slave. */
        hostCmd |= SMB_MCMD_READM_Msk;
    }

    smb0HostObj.protocol = protocol;

    /* Enable MDONE interrupt */
    SMB0_REGS->SMB_CFG[0] |= SMB_CFG_ENMI_Msk;

    /* If timeout check is enabled, then enable timeout related interrupts */
    if ((SMB0_REGS->SMB_CFG[0] & SMB_CFG_TCEN_Msk) != 0U)
    {
        SMB0_REGS->SMB_COMPL[0] |= SMB_COMPL_DTEN_Msk | SMB_COMPL_MCEN_Msk | SMB_COMPL_BIDEN_Msk;
    }
    else
    {
        SMB0_REGS->SMB_COMPL[0] &= ~(SMB_COMPL_DTEN_Msk | SMB_COMPL_MCEN_Msk | SMB_COMPL_BIDEN_Msk);
    }

    /* Set the host command and start the transfer */
    SMB0_REGS->SMB_MCMD[0] = hostCmd;
}

void I2CSMB0_HostWriteByte(uint8_t address, uint8_t cmd, void* pWrdata)
{
    /* <slave_add> <cmd> <data0> */
    uint8_t wrLen = I2CSMB0_HostPacketForm(I2C_SMB_HOST_PROTOCOL_WR_BYTE, address, cmd, pWrdata, 1);
    I2CSMB0_HostWriteRead(I2C_SMB_HOST_PROTOCOL_WR_BYTE, wrLen, 0);
}

void I2CSMB0_HostWriteWord(uint8_t address, uint8_t cmd, void* pWrdata)
{
    /* <slave_add> <cmd> <data0> <data1> */
    uint8_t wrLen = I2CSMB0_HostPacketForm(I2C_SMB_HOST_PROTOCOL_WR_WORD, address, cmd, pWrdata, 2);
    I2CSMB0_HostWriteRead(I2C_SMB_HOST_PROTOCOL_WR_WORD, wrLen, 0);
}

void I2CSMB0_HostWriteBlock(uint8_t address, uint8_t cmd, void* pWrdata, uint32_t nWrBytes)
{
    /* <slave_add> <cmd> <wr_block_sz> <data0> <data1> .. <datan>*/
    uint8_t wrLen = I2CSMB0_HostPacketForm(I2C_SMB_HOST_PROTOCOL_WR_BLK, address, cmd, pWrdata, nWrBytes);
    I2CSMB0_HostWriteRead(I2C_SMB_HOST_PROTOCOL_WR_BLK, wrLen, 0);
}

void I2CSMB0_HostReadByte(uint8_t address, uint8_t cmd)
{
    /* <slave_add> <cmd> <slave_add> <data0> */
    uint8_t wrLen = I2CSMB0_HostPacketForm(I2C_SMB_HOST_PROTOCOL_RD_BYTE, address, cmd, NULL, 0);
    I2CSMB0_HostWriteRead(I2C_SMB_HOST_PROTOCOL_RD_BYTE, wrLen, 1);
}

void I2CSMB0_HostReadWord(uint8_t address, uint8_t cmd)
{
    /* <slave_add> <cmd> <slave_add> <data0> <data1> */
    uint8_t wrLen = I2CSMB0_HostPacketForm(I2C_SMB_HOST_PROTOCOL_RD_WORD, address, cmd, NULL, 0);
    I2CSMB0_HostWriteRead(I2C_SMB_HOST_PROTOCOL_RD_WORD, wrLen, 2);
}

void I2CSMB0_HostReadBlock(uint8_t address, uint8_t cmd)
{
    /* <slave_add> <cmd> <slave_add> <rd_block_sz> <data0> <data1> .. <datan>*/
    uint8_t wrLen = I2CSMB0_HostPacketForm(I2C_SMB_HOST_PROTOCOL_RD_BLK, address, cmd, NULL, 0);
    /* Set nRdBytes with max possible read count. It will be over-written by the first byte received from I2C slave. */
    I2CSMB0_HostWriteRead(I2C_SMB_HOST_PROTOCOL_RD_BLK, wrLen, sizeof(i2csmb0HostRdBuffer));
}

void I2CSMB0_HostWriteReadBlock(uint8_t address, uint8_t cmd, void* pWrdata, uint32_t nWrBytes)
{
    /* <slave_add> <cmd> <wr_block_sz> <data0> <data1> .. <datan> <slave_add> <rd_block_sz> <data0> <data1> .. <datan>*/
    uint8_t wrLen = I2CSMB0_HostPacketForm(I2C_SMB_HOST_PROTOCOL_WR_BLK_RD_BLK, address, cmd, pWrdata, nWrBytes);
    /* Set nRdBytes with max possible read count. It will be over-written by the first byte received from I2C slave. */
    I2CSMB0_HostWriteRead(I2C_SMB_HOST_PROTOCOL_WR_BLK_RD_BLK, wrLen, sizeof(i2csmb0HostRdBuffer));
}

uint32_t I2CSMB0_HostTransferCountGet(void)
{
    return DMA_ChannelGetTransferredCount(DMA_CHANNEL_0);
}

uint32_t I2CSMB0_HostBufferRead(void* pBuffer)
{
    uint32_t i;
    uint32_t numBytesAvailable = DMA_ChannelGetTransferredCount(DMA_CHANNEL_0);
    uint8_t PECConfig = ((SMB0_REGS->SMB_CFG[0] & SMB_CFG_PECEN_Msk) != 0U)? 1U: 0U;

    /* Discard the PEC byte received from the slave and the PEC register copied by the Master state machine */
    numBytesAvailable -= (PECConfig == 1U)? 2U : 0U;

    /* First byte in i2csmb0HostRdBuffer is always the address byte and hence not copied to application buffer */
    for (i = 0; i < numBytesAvailable; i++)
    {
        ((uint8_t*)pBuffer)[i] = i2csmb0HostRdBuffer[i];
    }

    return numBytesAvailable;
}

void __attribute__((used)) I2CSMB0_HostInterruptHandler(uint32_t completion_reg)
{
    uint8_t PECConfig = ((SMB0_REGS->SMB_CFG[0] & SMB_CFG_PECEN_Msk) != 0U)? 1U: 0U;
    uintptr_t context = smb0HostObj.context;

    if ((completion_reg & SMB_COMPL_MDONE_Msk) != 0U)
    {
        if ((completion_reg & SMB_COMPL_BER_Msk) != 0U)
        {
            smb0HostObj.error = ((uint32_t)smb0HostObj.error) | I2C_SMB_HOST_ERROR_BUS_COLLISION;
        }
        if ((completion_reg & SMB_COMPL_LAB_Msk) != 0U)
        {
            smb0HostObj.error = ((uint32_t)smb0HostObj.error) | I2C_SMB_HOST_ERROR_ARBITRATION_LOST;
        }
        if ((completion_reg & SMB_COMPL_MNAKX_Msk) != 0U)
        {
            smb0HostObj.error = ((uint32_t)smb0HostObj.error) | I2C_SMB_HOST_ERROR_NACK;
        }
        if ((completion_reg & SMB_COMPL_TIMERR_Msk) != 0U)
        {
            smb0HostObj.error = ((uint32_t)smb0HostObj.error) | I2C_SMB_HOST_ERROR_TIMEOUT;
        }

        if (smb0HostObj.error == I2C_SMB_HOST_ERROR_NONE)
        {
            if ((SMB0_REGS->SMB_MCMD[0] & SMB_MCMD_MRUN_Msk) != 0U)
            {
                if ((SMB0_REGS->SMB_MCMD[0] & SMB_MCMD_MPROCEED_Msk) == 0U)
                {
                    if (smb0HostObj.dmaDirChange)
                    {
                        smb0HostObj.dmaDirChange = false;

                        smb0HostObj.state = I2C_SMB_HOST_STATE_RECEIVE;

                        (void)DMA_ChannelTransfer(DMA_CHANNEL_0, SMB0_MRXB, i2csmb0HostRdBuffer, smb0HostObj.dmaRdBytes);

                        /* Re-start the paused host state machine */
                        SMB0_REGS->SMB_MCMD[0] |= SMB_MCMD_MPROCEED_Msk;
                    }
                }
            }
            else
            {
                /* Transfer completed without error. Abort the transfer on DMA channel for bulk read transfers. */
                I2C_SMB_HOST_PROTOCOL protocol = smb0HostObj.protocol;

                if ((DMA_ChannelIsBusy(DMA_CHANNEL_0)) && ((protocol == I2C_SMB_HOST_PROTOCOL_RD_BLK) || (protocol == I2C_SMB_HOST_PROTOCOL_WR_BLK_RD_BLK)))
                {
                    DMA_ChannelTransferAbort(DMA_CHANNEL_0);
                }

                if (smb0HostObj.state == I2C_SMB_HOST_STATE_RECEIVE)
                {
                    /* Check for PEC error here if enabled. The RXB register will be loaded with the PEC register by the Host state machine and the PEC register will be 0 if there is no PEC error. */
                    if ((PECConfig == 1U) && (SMB0_REGS->SMB_MTR_RXB != 0U))
                    {
                        smb0HostObj.error |= I2C_SMB_HOST_ERROR_PEC;
                    }
                    else
                    {
                        if (smb0HostObj.callback != NULL)
                        {
                            smb0HostObj.callback(I2C_SMB_HOST_TRANSFER_EVENT_RX_READY, context);
                        }
                    }
                }

                smb0HostObj.state = I2C_SMB_HOST_STATE_IDLE;

                if (smb0HostObj.callback != NULL)
                {
                    if (smb0HostObj.error == I2C_SMB_HOST_ERROR_NONE)
                    {
                        (void)smb0HostObj.callback(I2C_SMB_HOST_TRANSFER_EVENT_DONE, context);
                    }
                    else
                    {
                        (void)smb0HostObj.callback(I2C_SMB_HOST_TRANSFER_EVENT_ERROR, context);
                    }
                }
            }
        }
        else
        {
            /* Transfer complete with error. Abort the DMA channel and give callback */
            if (DMA_ChannelIsBusy(DMA_CHANNEL_0))
            {
                DMA_ChannelTransferAbort(DMA_CHANNEL_0);
            }

            if ((smb0HostObj.error & (I2C_SMB_HOST_ERROR_BUS_COLLISION | I2C_SMB_HOST_ERROR_TIMEOUT)) != 0U)
            {
                I2CSMB0_HostReInitialize();
            }

            smb0HostObj.state = I2C_SMB_HOST_STATE_IDLE;

            if (smb0HostObj.callback != NULL)
            {
                smb0HostObj.callback(I2C_SMB_HOST_TRANSFER_EVENT_ERROR, context);
            }
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

        I2CSMB0_HostInterruptHandler(completion_reg);

        ECIA_GIRQSourceClear(ECIA_DIR_INT_SRC_I2CSMB0);
    }
}
