/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
#include <string.h>

typedef enum
{
    APP_STATE_POSTED_W32,
    APP_STATE_POSTED_R32,
    APP_SATE_FIFO_READ,
    APP_STATE_NON_POSTED_W32,
    APP_STATE_NON_POSTED_R32,
    APP_STATE_READ_STATUS,
    APP_SATE_EN_HOST_INTERRUPT,            
    APP_STATE_CLR_INTERRUPT_STATUS,
    APP_STATE_CHK_INT_LINE,
    APP_STATE_SUCCESS,
    APP_STATE_FAILED,
}APP_STATE;

#define LED_Off()                                 LED6_Clear()
#define LED_On()                                  LED6_Set()

#define CMD_MEM_W8              0x21
#define CMD_MEM_W32             0x23
#define CMD_MEM_R8              0x25
#define CMD_MEM_R32             0x27
#define CMD_RD_SNGL_FIFO32      0x2B
#define CMD_SREG_W8             0x09
#define CMD_POLL_LOW            0x2c

#define TAR_TIME_CYCLES         1   //8 cycles = 1 byte
#define WAIT_TIME_CYCLES        4  

#define SPT_INT_EN_REG_OFF      0x0C
#define SPT_STATUS_REG_OFF      0x04
#define SPT_STATUS_MEM_WR_DONE  0x01
#define SPT_STATUS_MEM_RD_DONE  0x02

const char* txStr1 = "Test";
const char* txStr2 = "mchp";
const char* cmpTxStr1 = "tseT";
const char* cmpTxStr2 = "phcm";

/* Global variables */
static uint8_t txBuff[100];
static uint8_t rxBuff[100];
static uint8_t txData[100];
static uint8_t transferLen = 0;
volatile bool transferStatus = false;
static uint16_t mem_addr = 0x0010;
static uint16_t sreg_addr;
static APP_STATE state = APP_STATE_POSTED_W32;
static APP_STATE nextState;
static bool hostInterruptEn = false;
// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

__ALIGNED(32) uint8_t buffer0[1024];   //Host write
__ALIGNED(32) uint8_t buffer1[1024];   //Host read
volatile uint32_t cntr = 0;
volatile uint32_t status_arr[100];
volatile uint32_t status_index = 0;
// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************
void spt1_event_handler(uint32_t status, uintptr_t context)
{
    /* SPT event handler */
}

void APP_SPIEventHandler(uintptr_t context )
{       
    transferStatus = true;
    
    SPI_CS_Set();
}

uint8_t spi_packet_form(uint8_t* txBuffer, uint8_t cmd, uint16_t* addr, uint8_t* data, uint8_t datalen)
{
    uint8_t i = 0;
    uint8_t* pData = &data[datalen-1];
    
    txBuffer[i++] = cmd;
    
    if (addr)
    {
        txBuffer[i++] = *addr >> 8;
        txBuffer[i++] = *addr;
    }
    if (data != NULL && datalen != 0)
    {
        for (; i < datalen + 3; i++)
        {
            txBuffer[i] = *pData--;
        }
    }
    
    return i;
}

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    
    for (uint32_t i = 0; i < 1024; i++)
    {
        buffer0[i] = i;
    }
    
    for (uint32_t i = 0; i < 1024; i++)
    {
        buffer1[i] = i;
    }
    
    /* Setup SPT */
    SPT1_CallbackRegister(spt1_event_handler, 0);
    
    SPT1_MEM0Config((uint32_t)buffer0, sizeof(buffer0), sizeof(buffer0));
    
    SPT1_MEM1Config((uint32_t)buffer1, sizeof(buffer1), sizeof(buffer1));
    
    /* Setup QMSPI master */
    QMSPI0_SPI_CallbackRegister(APP_SPIEventHandler, (uintptr_t) 0);
    
    while(1)
    {    
        switch(state)
        {
            case APP_STATE_POSTED_W32:                         
                mem_addr = 0x0010;        
                transferLen = spi_packet_form(txBuff, CMD_MEM_W32, &mem_addr, (uint8_t*)txStr1, 4);
                SPI_CS_Clear();
                transferStatus = false;
                QMSPI0_SPI_Write(txBuff, transferLen);
                while (transferStatus == false);

                state = APP_STATE_READ_STATUS;
                nextState = APP_STATE_POSTED_R32;

                break;
            case APP_STATE_POSTED_R32:
                mem_addr = 0x0010;
                transferLen = spi_packet_form(txBuff, CMD_MEM_R32, &mem_addr, NULL, 0);
                SPI_CS_Clear();
                transferStatus = false;
                QMSPI0_SPI_Write(txBuff, transferLen);
                while (transferStatus == false);

                if (hostInterruptEn == true)
                {
                    state = APP_STATE_CHK_INT_LINE;
                }
                else
                {
                    state = APP_STATE_READ_STATUS;
                    nextState = APP_SATE_FIFO_READ;
                }
                break;

            case APP_SATE_FIFO_READ:
                mem_addr = 0x0010;
                transferLen = spi_packet_form(txBuff, CMD_RD_SNGL_FIFO32, NULL, NULL, 0);
                SPI_CS_Clear();
                transferStatus = false;
                QMSPI0_SPI_WriteRead(txBuff, transferLen, rxBuff, transferLen + TAR_TIME_CYCLES + 4);
                while (transferStatus == false);
                
                if (memcmp(&rxBuff[transferLen + TAR_TIME_CYCLES], cmpTxStr1, 4) == 0)
                {
                    if (hostInterruptEn == true)
                    {
                        state = APP_STATE_SUCCESS;
                    }
                    else
                    {
                        state = APP_STATE_NON_POSTED_W32;
                    }
                }
                else
                {
                    state = APP_STATE_FAILED;
                }

                break;
            case APP_STATE_NON_POSTED_W32:                
                mem_addr = 0x0020;
                transferLen = spi_packet_form(txBuff, CMD_MEM_W32, &mem_addr, (uint8_t*)txStr2, 4);
                SPI_CS_Clear();
                transferStatus = false;
                QMSPI0_SPI_WriteRead(txBuff, transferLen, rxBuff, transferLen + TAR_TIME_CYCLES + WAIT_TIME_CYCLES + 2);
                while (transferStatus == false);

                state = APP_STATE_NON_POSTED_R32;

                break;
            case APP_STATE_NON_POSTED_R32:
                mem_addr = 0x0020;
                transferLen = spi_packet_form(txBuff, CMD_MEM_R32, &mem_addr, NULL, 0);
                SPI_CS_Clear();
                transferStatus = false;
                QMSPI0_SPI_WriteRead(txBuff, transferLen, rxBuff, transferLen + TAR_TIME_CYCLES + WAIT_TIME_CYCLES + 2 + 4);
                while (transferStatus == false);
                
                if (memcmp(&rxBuff[transferLen + TAR_TIME_CYCLES + WAIT_TIME_CYCLES + 2], cmpTxStr2, 4) != 0)
                {
                    state = APP_STATE_FAILED;
                }
                else
                {
                    state = APP_SATE_EN_HOST_INTERRUPT;
                }

                break;

            case APP_SATE_EN_HOST_INTERRUPT:

                txData[0] = SPT_STATUS_MEM_RD_DONE;     //Enable Memory Read Done Interrupt
                sreg_addr = SPT_INT_EN_REG_OFF;            
                transferLen = spi_packet_form(txBuff, CMD_SREG_W8, &sreg_addr, txData, 1);
                SPI_CS_Clear();
                transferStatus = false;
                QMSPI0_SPI_Write(txBuff, transferLen);
                while (transferStatus == false);
                hostInterruptEn = true;
                state = APP_STATE_READ_STATUS;
                nextState = APP_STATE_POSTED_R32;

                break;
            case APP_STATE_CLR_INTERRUPT_STATUS:

                txData[0] = (SPT_STATUS_MEM_WR_DONE | SPT_STATUS_MEM_RD_DONE);        
                sreg_addr = SPT_STATUS_REG_OFF;
                transferLen = spi_packet_form(txBuff, CMD_SREG_W8, &sreg_addr, txData, 1);
                SPI_CS_Clear();
                transferStatus = false;
                QMSPI0_SPI_Write(txBuff, transferLen);
                while (transferStatus == false);        
                state = nextState;
                break;

            case APP_STATE_READ_STATUS:

                transferLen = spi_packet_form(txBuff, CMD_POLL_LOW, NULL, NULL, 0);
                SPI_CS_Clear();
                transferStatus = false;
                QMSPI0_SPI_WriteRead(txBuff, transferLen, rxBuff, transferLen + TAR_TIME_CYCLES + 1);
                while (transferStatus == false);

                if ((rxBuff[transferLen + TAR_TIME_CYCLES] & (SPT_STATUS_MEM_WR_DONE | SPT_STATUS_MEM_RD_DONE)) != 0)
                {
                    state = APP_STATE_CLR_INTERRUPT_STATUS;
                }

                break;
            case APP_STATE_CHK_INT_LINE:
                if (SPT_INT_Get())
                {
                    state = APP_STATE_CLR_INTERRUPT_STATUS;
                    nextState = APP_SATE_FIFO_READ;
                }
                break;

            case APP_STATE_SUCCESS:
                LED_On();
                break;
                
            case APP_STATE_FAILED:
                LED_Off();
                break;
        }
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

