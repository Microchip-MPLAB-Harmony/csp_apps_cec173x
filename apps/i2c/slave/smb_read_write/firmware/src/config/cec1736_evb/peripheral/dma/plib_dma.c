/*******************************************************************************
  Direct Memory Access Controller (DMA) PLIB

  Company
    Microchip Technology Inc.

  File Name
    plib_dma.c

  Summary
    Source for DMA peripheral library interface Implementation.

  Description
    This file defines the interface to the DMA peripheral library. This
    library provides access to and control of the DMA controller.

  Remarks:
    None.

*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2021 Microchip Technology Inc. and its subsidiaries.
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

#include "plib_dma.h"
#include "interrupts.h"
#include "../ecia/plib_ecia.h"


// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************

#define DMA_CHANNELS_NUMBER        (1U)


/* DMA channels object configuration structure */
typedef struct
{
    DMA_CHANNEL_CALLBACK  callback;
    uintptr_t             context;
    bool                  busyStatus;
    uint32_t              mstartAddr;
} DMA_CH_OBJECT ;

/* DMA Channels object information structure */
static DMA_CH_OBJECT dmaChannelObj[DMA_CHANNELS_NUMBER];

#define NOP()    asm("NOP")

static dma_chan00_registers_t* DMA_ChannelBaseAddrGet(DMA_CHANNEL ch)
{
    return (dma_chan00_registers_t*)(DMA_CHAN00_BASE_ADDRESS + ((uint32_t)ch * 64U));
}

void DMA_Initialize( void )
{
    dma_chan00_registers_t* dmaChRegs;

    /* Initialize DMA Channel objects */
    for(uint32_t channel = 0U; channel < DMA_CHANNELS_NUMBER; channel++)
    {
        dmaChannelObj[channel].callback = NULL;
        dmaChannelObj[channel].context = 0U;
        dmaChannelObj[channel].busyStatus = false;
    }

    /* Reset DMA module */
    DMA_MAIN_REGS->DMA_MAIN_ACTRST = DMA_MAIN_ACTRST_SOFT_RST_Msk;

   /***************** Configure DMA channel 0 ********************/
   dmaChRegs = DMA_ChannelBaseAddrGet(DMA_CHANNEL_0);

   dmaChRegs->DMA_CHAN00_CTRL = DMA_CHAN00_CTRL_TX_DIR(0x0) | DMA_CHAN00_CTRL_DIS_HW_FLOW_CTRL(0) | DMA_CHAN00_CTRL_HW_FLOW_CTRL_DEV(0) | DMA_CHAN00_CTRL_TRANS_SIZE(0x1) | DMA_CHAN00_CTRL_INC_DEV_ADDR(false) | DMA_CHAN00_CTRL_INC_MEM_ADDR(true);
   dmaChRegs->DMA_CHAN00_ACTIVATE = DMA_CHAN00_ACTIVATE_CHN_Msk;


    /* Global enable */
    DMA_MAIN_REGS->DMA_MAIN_ACTRST = DMA_MAIN_ACTRST_ACT_Msk;
}

bool DMA_ChannelTransfer( DMA_CHANNEL channel, const void *srcAddr, const void *destAddr, size_t blockSize )
{
    bool returnStatus = false;
    dma_chan00_registers_t* dmaChRegs = DMA_ChannelBaseAddrGet(channel);

    uint32_t src_addr = (uint32_t)((const uint32_t*)srcAddr);
    uint32_t dst_addr = (uint32_t)((const uint32_t*)destAddr);
    uint32_t transferDir = ((src_addr & EC_DEVICE_REGISTERS_ADDR) != 0U)? 0U:1U;


    if (dmaChannelObj[channel].busyStatus == false)
    {
        dmaChannelObj[channel].busyStatus = true;

        /* Activate channel */
        dmaChRegs->DMA_CHAN00_ACTIVATE |= DMA_CHAN00_ACTIVATE_CHN_Msk;

        if (transferDir == 0U)
        {
            /* Peripheral to memory transfer */
            dmaChRegs->DMA_CHAN00_DSTART = src_addr;
            dmaChRegs->DMA_CHAN00_MSTART = dst_addr;
            dmaChRegs->DMA_CHAN00_MEND = (uint32_t)(dst_addr + blockSize);
        }
        else
        {
            /* Memory to peripheral transfer */
            dmaChRegs->DMA_CHAN00_DSTART = dst_addr;
            dmaChRegs->DMA_CHAN00_MSTART = src_addr;
            dmaChRegs->DMA_CHAN00_MEND = (uint32_t)(src_addr + blockSize);
        }

        dmaChannelObj[channel].mstartAddr = dmaChRegs->DMA_CHAN00_MSTART;

        /* Set the transfer direction */
        dmaChRegs->DMA_CHAN00_CTRL = (dmaChRegs->DMA_CHAN00_CTRL & ~DMA_CHAN00_CTRL_TX_DIR_Msk) | (transferDir << DMA_CHAN00_CTRL_TX_DIR_Pos);

        if ((uint32_t)channel == 0U)
        {
            /* If CRC is enabled, initialize the CRC generator */
            if ((dmaChRegs->DMA_CHAN00_CRC_EN & DMA_CHAN00_CRC_EN_MODE_Msk) != 0U)
            {
                dmaChRegs->DMA_CHAN00_CRC_DATA = 0xFFFFFFFFU;
            }
        }

        /* Enable transfer done and bus error interrupts */
        dmaChRegs->DMA_CHAN00_IEN = (DMA_CHAN00_IEN_STS_EN_DONE_Msk | DMA_CHAN00_IEN_STS_EN_BUS_ERR_Msk);

        /* Start the transfer, check if device is the DMA host or firmware is the DMA host */
        if ((dmaChRegs->DMA_CHAN00_CTRL & DMA_CHAN00_CTRL_DIS_HW_FLOW_CTRL_Msk) != 0U)
        {
            /* DMA controller is under the control of firmware (not the device host) */
            dmaChRegs->DMA_CHAN00_CTRL |= DMA_CHAN00_CTRL_TRANS_GO_Msk;
        }
        else
        {
            /* DMA controller is under the control of device */
            dmaChRegs->DMA_CHAN00_CTRL |= DMA_CHAN00_CTRL_RUN_Msk;
        }

        returnStatus = true;
    }

    return returnStatus;
}

void DMA_ChannelCallbackRegister( DMA_CHANNEL channel, const DMA_CHANNEL_CALLBACK callback, const uintptr_t context )
{
    dmaChannelObj[channel].callback = callback;
    dmaChannelObj[channel].context  = context;
}

bool DMA_ChannelIsBusy ( DMA_CHANNEL channel )
{
    return (dmaChannelObj[channel].busyStatus);
}

void DMA_ChannelEnable ( DMA_CHANNEL channel )
{
    dma_chan00_registers_t* dmaChRegs = DMA_ChannelBaseAddrGet(channel);

    /* Activate the DMA channel */
    dmaChRegs->DMA_CHAN00_ACTIVATE |= DMA_CHAN00_ACTIVATE_CHN_Msk;
}

void DMA_ChannelDisable ( DMA_CHANNEL channel )
{
    dma_chan00_registers_t* dmaChRegs = DMA_ChannelBaseAddrGet(channel);

    dmaChRegs->DMA_CHAN00_ACTIVATE &= ~DMA_CHAN00_ACTIVATE_CHN_Msk;

    dmaChannelObj[channel].busyStatus=false;
}

void DMA_ChannelTransferAbort ( DMA_CHANNEL channel )
{
    dma_chan00_registers_t* dmaChRegs = DMA_ChannelBaseAddrGet(channel);

    dmaChRegs->DMA_CHAN00_CTRL |= DMA_CHAN00_CTRL_TRANS_ABORT_Msk;

    NOP();NOP();NOP();NOP();NOP();NOP();

    dmaChRegs->DMA_CHAN00_CTRL &= ~DMA_CHAN00_CTRL_TRANS_ABORT_Msk;

    dmaChannelObj[channel].busyStatus=false;
}

uint32_t DMA_ChannelGetTransferredCount( DMA_CHANNEL channel )
{
    dma_chan00_registers_t* dmaChRegs = DMA_ChannelBaseAddrGet(channel);

    uint32_t transferWidth = (dmaChRegs->DMA_CHAN00_CTRL & DMA_CHAN00_CTRL_TRANS_SIZE_Msk) >> DMA_CHAN00_CTRL_TRANS_SIZE_Pos;

    return ((dmaChRegs->DMA_CHAN00_MSTART - dmaChannelObj[channel].mstartAddr)/transferWidth);
}

void DMA_ChannelInterruptEnable ( DMA_CHANNEL channel, DMA_INT intSources )
{
    dma_chan00_registers_t* dmaChRegs = DMA_ChannelBaseAddrGet(channel);

    /* Enable transfer done and bus error interrupts */
    dmaChRegs->DMA_CHAN00_IEN |= intSources;
}

void DMA_ChannelInterruptDisable ( DMA_CHANNEL channel, DMA_INT intSources )
{
    dma_chan00_registers_t* dmaChRegs = DMA_ChannelBaseAddrGet(channel);

    /* Enable transfer done and bus error interrupts */
    dmaChRegs->DMA_CHAN00_IEN &= ~(intSources);
}

DMA_INT DMA_ChannelInterruptFlagsGet ( DMA_CHANNEL channel )
{
    dma_chan00_registers_t* dmaChRegs = DMA_ChannelBaseAddrGet(channel);

    return (dmaChRegs->DMA_CHAN00_ISTS & DMA_CHAN00_ISTS_Msk);
}

DMA_CHANNEL_CONFIG DMA_ChannelSettingsGet (DMA_CHANNEL channel)
{
    dma_chan00_registers_t* dmaChRegs = DMA_ChannelBaseAddrGet(channel);

    return dmaChRegs->DMA_CHAN00_CTRL;
}

bool DMA_ChannelSettingsSet (DMA_CHANNEL channel, DMA_CHANNEL_CONFIG setting)
{
    dma_chan00_registers_t* dmaChRegs = DMA_ChannelBaseAddrGet(channel);

    dmaChRegs->DMA_CHAN00_CTRL = setting;

    return true;
}



static void DMA_interruptHandler(DMA_CHANNEL channel)
{
    DMA_CH_OBJECT  *dmacChObj = NULL;
    volatile uint8_t chIntFlagStatus = 0U;

    DMA_TRANSFER_EVENT event = 0U;

    dmacChObj = (DMA_CH_OBJECT *)&dmaChannelObj[channel];

    dma_chan00_registers_t* dmaChRegs = DMA_ChannelBaseAddrGet(channel);

    /* Get the DMA channel interrupt flag status */
    chIntFlagStatus = dmaChRegs->DMA_CHAN00_ISTS;

    if (((chIntFlagStatus & DMA_CHAN00_ISTS_DONE_Msk) != 0U) && ((dmaChRegs->DMA_CHAN00_IEN & DMA_CHAN00_IEN_STS_EN_DONE_Msk) != 0U))
    {
        event = DMA_TRANSFER_EVENT_COMPLETE;
    }
    else if (((chIntFlagStatus & DMA_CHAN00_ISTS_BUS_ERR_Msk) != 0U) && ((dmaChRegs->DMA_CHAN00_IEN & DMA_CHAN00_IEN_STS_EN_BUS_ERR_Msk) != 0U))
    {
        event = DMA_TRANSFER_EVENT_ERROR;
    }
    else
    {
        /* Do nothing */
    }

    /* Clear the interrupt status flags (Write 1 to clear) */
    dmaChRegs->DMA_CHAN00_ISTS = chIntFlagStatus;

    dmaChannelObj[channel].busyStatus=false;

    /* Execute the callback function */
    if ((dmacChObj->callback != NULL) && ((uint32_t)event != 0U))
    {
        dmacChObj->callback (event, dmacChObj->context);
    }
}

void DMA_CH00_InterruptHandler( void )
{
    ECIA_GIRQSourceClear(ECIA_DIR_INT_SRC_DMA_CH00);
    DMA_interruptHandler(DMA_CHANNEL_0);
}

