/*******************************************************************************
  QMSPI0 SPI Peripheral Library Source File

  Company
    Microchip Technology Inc.

  File Name
    plib_qmspi0_spi.c

  Summary
    QMSPI0 SPI mode peripheral library interface.

  Description

  Remarks:

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
#include "interrupts.h"
#include "plib_qmspi0_spi.h"
#include "../ecia/plib_ecia.h"

typedef struct
{
    uint8_t*                pTxBuffer;
    uint8_t*                pRxBuffer;
    size_t                  txSize;
    size_t                  rxSize;
    size_t                  dummySize;
    size_t                  rxCount;
    size_t                  txCount;
    bool                    transferIsBusy;
    QMSPI_SPI_CALLBACK      callback;
    uintptr_t               context;

} QMSPI_SPI_OBJECT ;


/* Global object to save SPI Exchange related data */
volatile static QMSPI_SPI_OBJECT qmspi0Obj;

// *****************************************************************************
// *****************************************************************************
// QMSPI0 PLIB Interface Routines
// *****************************************************************************
// *****************************************************************************


void QMSPI0_SPI_Initialize(void)
{
    /* Reset the QMSPI Block */
    QMSPI0_REGS->QMSPI_MODE = QMSPI_MODE_SOFT_RESET_Msk;

    QMSPI0_REGS->QMSPI_MODE = QMSPI_MODE_CLK_DIV(8U) | QMSPI_MODE_CHPA_MOSI(0) | QMSPI_MODE_CHPA_MISO(0) | QMSPI_MODE_CS(0);

    /* Activate the QMSPI Block */
    QMSPI0_REGS->QMSPI_MODE |= QMSPI_MODE_ACT_Msk;
}

bool QMSPI0_SPI_TransferSetup (QMSPI_SPI_TRANSFER_SETUP *setup)
{
    uint32_t clock_divide;
    bool setupStatus = false;

    if (setup != NULL)
    {
        clock_divide = 96000000U / setup->clockFrequency;

        if (clock_divide >= 256U)
        {
            clock_divide = 0;
        }

        QMSPI0_REGS->QMSPI_MODE =
        ( (QMSPI0_REGS->QMSPI_MODE & ~(QMSPI_MODE_CLK_DIV_Msk | QMSPI_MODE_CHPA_MOSI_Msk | QMSPI_MODE_CHPA_MISO_Msk | QMSPI_MODE_CPOL_Msk)) | (QMSPI_MODE_CLK_DIV(clock_divide) | (uint32_t)setup->clockPhase | (uint32_t)setup->clockPolarity));

        setupStatus = true;
    }
    return setupStatus;
}

bool QMSPI0_SPI_Write(void* pTransmitData, size_t txSize)
{
    return(QMSPI0_SPI_WriteRead(pTransmitData, txSize, NULL, 0));
}

bool QMSPI0_SPI_Read(void* pReceiveData, size_t rxSize)
{
    return(QMSPI0_SPI_WriteRead(NULL, 0, pReceiveData, rxSize));
}


void QMSPI0_SPI_CallbackRegister(QMSPI_SPI_CALLBACK callback, uintptr_t context)
{
    qmspi0Obj.callback = callback;
    qmspi0Obj.context = context;
}

bool QMSPI0_SPI_IsBusy (void)
{
    bool transferIsBusy = qmspi0Obj.transferIsBusy;

    return transferIsBusy;
}


bool QMSPI0_SPI_IsTransmitterBusy(void)
{
    return ((QMSPI0_REGS->QMSPI_STS & QMSPI_STS_TRANS_ACTIV_Msk) != 0U);
}



bool QMSPI0_SPI_WriteRead (void* pTransmitData, size_t txSize, void* pReceiveData, size_t rxSize)
{
    bool isRequestAccepted = false;
    size_t txCount = 0;
    size_t dummySize = 0;

    /* Verify the request */
    if((qmspi0Obj.transferIsBusy == false) && (((txSize > 0U) && (pTransmitData != NULL)) || ((rxSize > 0U) && (pReceiveData != NULL))))
    {
        if(pTransmitData == NULL)
        {
            txSize = 0U;
        }

        if(pReceiveData == NULL)
        {
            rxSize = 0U;
        }

        qmspi0Obj.txSize = txSize;
        qmspi0Obj.rxSize = rxSize;
        qmspi0Obj.pTxBuffer = pTransmitData;
        qmspi0Obj.pRxBuffer = pReceiveData;
        qmspi0Obj.rxCount = 0;
        qmspi0Obj.txCount = 0;
        qmspi0Obj.dummySize = 0;

        qmspi0Obj.transferIsBusy = true;

        /* Flush out any unread data in SPI DATA Register from the previous transfer */
        QMSPI0_REGS->QMSPI_EXE = QMSPI_EXE_CLR_DAT_BUFF_Msk;

        if(rxSize > txSize)
        {
            dummySize = rxSize - txSize;
        }

        while (((QMSPI0_REGS->QMSPI_STS & QMSPI_STS_TX_BUFF_FULL_Msk) == 0U) && ((txCount != txSize) || (dummySize > 0U)))
        {
            if (txCount != txSize)
            {
                *((volatile uint8_t*)&QMSPI0_REGS->QMSPI_TX_FIFO[0]) = qmspi0Obj.pTxBuffer[txCount];
                txCount++;
            }
            else if (dummySize > 0U)
            {
                *((volatile uint8_t*)&QMSPI0_REGS->QMSPI_TX_FIFO[0]) = 0xFFU;
                dummySize--;
            }
            else
            {
                /* Do nothing */
            }
        }

        uint32_t transferCount = QMSPI0_REGS->QMSPI_BUF_CNT_STS & QMSPI_BUF_CNT_STS_TX_BUFF_CNT_Msk;

        QMSPI0_REGS->QMSPI_CTRL = QMSPI_CTRL_TRANS_UNITS(0x01) | QMSPI_CTRL_TRANS_LEN(transferCount) | QMSPI_CTRL_TX_TRANS_EN(0x01) | QMSPI_CTRL_RX_TRANS_EN_Msk;

        if ((txCount == txSize) && (dummySize == 0U))
        {
            QMSPI0_REGS->QMSPI_CTRL |= QMSPI_CTRL_CLOSE_TRANS_EN_Msk;
        }

        qmspi0Obj.txCount = txCount;
        qmspi0Obj.dummySize = dummySize;

        QMSPI0_REGS->QMSPI_IEN = QMSPI_IEN_TRANS_COMPL_EN_Msk;
        QMSPI0_REGS->QMSPI_EXE = QMSPI_EXE_START_Msk;

        isRequestAccepted = true;
    }

    return isRequestAccepted;
}

void __attribute__((used)) QMSPI0_InterruptHandler(void)
{
    volatile uint8_t receivedData;
    uint32_t rxSize = qmspi0Obj.rxSize;
    uint32_t rxCount = qmspi0Obj.rxCount;
    uint32_t txSize = qmspi0Obj.txSize;
    uint32_t txCount = qmspi0Obj.txCount;
    uint32_t dummySize = qmspi0Obj.dummySize;

    if (ECIA_GIRQResultGet(ECIA_DIR_INT_SRC_QMSPI0) != 0U)
    {
        ECIA_GIRQSourceClear(ECIA_DIR_INT_SRC_QMSPI0);

        if ((QMSPI0_REGS->QMSPI_STS & QMSPI_STS_TRANS_COMPL_Msk) != 0U)
        {
            QMSPI0_REGS->QMSPI_STS |= QMSPI_STS_TRANS_COMPL_Msk;

            /* Read the received data from the FIFO */
            while ((QMSPI0_REGS->QMSPI_STS & QMSPI_STS_RX_BUFF_EMP_Msk) == 0U)
            {
                receivedData = *((volatile uint8_t*)&QMSPI0_REGS->QMSPI_RX_FIFO[0]);
                if(rxCount < rxSize)
                {
                    qmspi0Obj.pRxBuffer[rxCount] = (uint8_t)receivedData;
                    rxCount++;
                }
            }

            qmspi0Obj.rxCount = rxCount;

            if ((txCount == txSize) && (rxCount == rxSize))
            {
                QMSPI0_REGS->QMSPI_IEN &= ~QMSPI_IEN_TRANS_COMPL_EN_Msk;

                qmspi0Obj.transferIsBusy = false;

                if (qmspi0Obj.callback != NULL)
                {
                    uintptr_t context = qmspi0Obj.context;

                    qmspi0Obj.callback(context);
                }
            }
            else
            {
                while ( ((QMSPI0_REGS->QMSPI_STS & QMSPI_STS_TX_BUFF_FULL_Msk) == 0U) && ((txCount != txSize) || (dummySize > 0U)) )
                {
                    if (txCount != txSize)
                    {
                        *((volatile uint8_t*)&QMSPI0_REGS->QMSPI_TX_FIFO[0]) = qmspi0Obj.pTxBuffer[txCount];
                        txCount++;
                    }
                    else if (dummySize > 0U)
                    {
                        *((volatile uint8_t*)&QMSPI0_REGS->QMSPI_TX_FIFO[0]) = 0xFFU;
                        dummySize--;
                    }
                    else
                    {
                        /* Do nothing */
                    }
                }

                qmspi0Obj.txCount = txCount;
                qmspi0Obj.dummySize = dummySize;

                uint32_t transferCount = QMSPI0_REGS->QMSPI_BUF_CNT_STS & QMSPI_BUF_CNT_STS_TX_BUFF_CNT_Msk;

                QMSPI0_REGS->QMSPI_CTRL = QMSPI_CTRL_TRANS_UNITS(0x01) | QMSPI_CTRL_TRANS_LEN(transferCount) | QMSPI_CTRL_TX_TRANS_EN(0x01) | QMSPI_CTRL_RX_TRANS_EN_Msk;

                if ((txCount == txSize) && (dummySize == 0U))
                {
                    QMSPI0_REGS->QMSPI_CTRL |= QMSPI_CTRL_CLOSE_TRANS_EN_Msk;
                }

                QMSPI0_REGS->QMSPI_IEN = QMSPI_IEN_TRANS_COMPL_EN_Msk;
                QMSPI0_REGS->QMSPI_EXE = QMSPI_EXE_START_Msk;
            }
        }
    }
}





/*******************************************************************************
 End of File
*/
