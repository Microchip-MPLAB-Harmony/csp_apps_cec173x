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

/* Application's state machine enum */
typedef enum
{
    APP_STATE_INITIALIZE,
    APP_STATE_RESET,
    APP_STATE_GLOBAL_BLK_PROTECTION_UNLOCK,
    APP_STATE_JEDEC_ID_READ,
    APP_STATE_SECTOR_ERASE,
    APP_STATE_READ_STATUS,
    APP_STATE_PAGE_PROGRAM,
    APP_STATE_MEMORY_READ,
    APP_STATE_VERIFY,
    APP_STATE_XFER_SUCCESSFUL,
    APP_STATE_XFER_ERROR
} APP_STATES;

/* Flash Commands */
#define APP_CMD_ENABLE_RESET                      0x66
#define APP_CMD_MEMORY_RESET                      0x99
#define APP_CMD_STATUS_REG_READ                   0x05
#define APP_CMD_CONFIG_REG_READ                   0x35
#define APP_CMD_MEMORY_READ                       0x03
#define APP_CMD_MEMORY_FAST_READ_QUAD             0x6B
#define APP_CMD_ENABLE_WRITE                      0x06
#define APP_CMD_DISABLE_WRITE                     0x04
#define APP_CMD_4KB_SECTOR_ERASE                  0x20
#define APP_CMD_BLOCK_ERASE                       0xD8
#define APP_CMD_CHIP_ERASE                        0xC7
#define APP_CMD_QUAD_INPUT_PAGE_PROGRAM           0x32
#define APP_CMD_JEDEC_ID_READ                     0x9F
#define APP_CMD_GLOBAL_BLOCK_PROTECTION_UNLOCK    0x98

#define APP_STATUS_BIT_WEL                        (0x01 << 1)
#define APP_STATUS_BIT_BUSY                       (0x01 << 0)

#define APP_PAGE_PROGRAM_SIZE_BYTES               256
#define APP_MEM_ADDR                              0x10000
#define LED_Off()                                 LED5_Clear()
#define LED_On()                                  LED5_Set()

typedef struct
{
    APP_STATES          state;
    APP_STATES          nextState;
    uint8_t             transmitBuffer[APP_PAGE_PROGRAM_SIZE_BYTES + 5];
    uint8_t             manufacturerID;
    uint16_t            deviceID;
}APP_DATA;

APP_DATA                appData;
uint8_t                 writeDataBuffer[APP_PAGE_PROGRAM_SIZE_BYTES];
uint8_t                 readDataBuffer[APP_PAGE_PROGRAM_SIZE_BYTES];

void APP_Reset(void)
{
    QMSPI_XFER_T qmspiXfer;

    memset(&qmspiXfer, 0x00, sizeof(QMSPI_XFER_T));

    qmspiXfer.command = APP_CMD_ENABLE_RESET;
    QMSPI0_Write(&qmspiXfer, NULL, 0);

    qmspiXfer.command = APP_CMD_MEMORY_RESET;
    QMSPI0_Write(&qmspiXfer, NULL, 0);
}

void APP_WriteEnable(void)
{
    QMSPI_XFER_T qmspiXfer;

    memset(&qmspiXfer, 0x00, sizeof(QMSPI_XFER_T));

    qmspiXfer.command = APP_CMD_ENABLE_WRITE;
    QMSPI0_Write(&qmspiXfer, NULL, 0);
}

void APP_WriteDisable(void)
{
    QMSPI_XFER_T qmspiXfer;

    memset(&qmspiXfer, 0x00, sizeof(QMSPI_XFER_T));

    qmspiXfer.command = APP_CMD_DISABLE_WRITE;
    QMSPI0_Write(&qmspiXfer, NULL, 0);
}

void APP_SectorErase(uint32_t address)
{
    QMSPI_XFER_T qmspiXfer;

    APP_WriteEnable();

    memset(&qmspiXfer, 0x00, sizeof(QMSPI_XFER_T));

    qmspiXfer.command = APP_CMD_4KB_SECTOR_ERASE;
    qmspiXfer.address_enable = true;
    qmspiXfer.address = address;
    QMSPI0_Write(&qmspiXfer, NULL, 0);
}

void APP_ChipErase(void)
{
    QMSPI_XFER_T qmspiXfer;

    APP_WriteEnable();

    memset(&qmspiXfer, 0x00, sizeof(QMSPI_XFER_T));

    qmspiXfer.command = APP_CMD_CHIP_ERASE;
    QMSPI0_Write(&qmspiXfer, NULL, 0);
}

void APP_PageProgram(uint32_t address, uint8_t* pPageData)
{
    uint32_t i;
    QMSPI_DESCRIPTOR_XFER_T qmspiDescXfer;

    APP_WriteEnable();

    memset(&qmspiDescXfer, 0x00, sizeof(QMSPI_DESCRIPTOR_XFER_T));

    qmspiDescXfer.command = APP_CMD_QUAD_INPUT_PAGE_PROGRAM;
    qmspiDescXfer.address = address;
    qmspiDescXfer.qmspi_ifc_mode = QUAD_OUTPUT;
    qmspiDescXfer.ldma_enable = true;
    qmspiDescXfer.ldma_channel_num = QMSPI_LDMA_CHANNEL_0;

    for (i = 0; i < APP_PAGE_PROGRAM_SIZE_BYTES; i++)
    {
        appData.transmitBuffer[i] = pPageData[i];
    }
    QMSPI0_DMATransferWrite(&qmspiDescXfer, appData.transmitBuffer, APP_PAGE_PROGRAM_SIZE_BYTES);
}

void APP_MemoryRead(uint32_t address, uint8_t* pReadBuffer, uint32_t nBytes)
{
    QMSPI_DESCRIPTOR_XFER_T qmspiDescXfer;

    memset(&qmspiDescXfer, 0x00, sizeof(QMSPI_DESCRIPTOR_XFER_T));

    qmspiDescXfer.address = address;
    qmspiDescXfer.qmspi_ifc_mode = QUAD_OUTPUT;
    qmspiDescXfer.ldma_enable = true;
    qmspiDescXfer.ldma_channel_num = QMSPI_LDMA_CHANNEL_0;
    qmspiDescXfer.command = APP_CMD_MEMORY_FAST_READ_QUAD;
    qmspiDescXfer.num_of_dummy_byte = 4;

    QMSPI0_DMATransferRead(&qmspiDescXfer, pReadBuffer, nBytes);
}

uint8_t APP_StatusRead(void)
{
    uint8_t status;
    QMSPI_XFER_T qmspiXfer;

    memset(&qmspiXfer, 0x00, sizeof(QMSPI_XFER_T));

    qmspiXfer.command = APP_CMD_STATUS_REG_READ;
    QMSPI0_Read(&qmspiXfer, &status, 1);

    return status;
}

void APP_JEDEC_ID_Read(uint8_t* manufacturerID, uint16_t* deviceID)
{
    uint8_t jdec_id[3];
    QMSPI_XFER_T qmspiXfer;

    memset(&qmspiXfer, 0x00, sizeof(QMSPI_XFER_T));

    qmspiXfer.command = APP_CMD_JEDEC_ID_READ;

    QMSPI0_Read(&qmspiXfer, &jdec_id[0], 3);

    *manufacturerID = jdec_id[0];
    *deviceID = (jdec_id[1] << 8UL) | jdec_id[2];
}

void APP_GlobalWriteProtectionUnlock(void)
{
    QMSPI_XFER_T qmspiXfer;

    APP_WriteEnable();

    memset(&qmspiXfer, 0x00, sizeof(QMSPI_XFER_T));

    qmspiXfer.command = APP_CMD_GLOBAL_BLOCK_PROTECTION_UNLOCK;
    QMSPI0_Write(&qmspiXfer, NULL, 0);

}

void APP_Initialize (void)
{
    uint32_t i;

    appData.state = APP_STATE_INITIALIZE;

    /* Fill up the test data */
    for (i = 0; i < APP_PAGE_PROGRAM_SIZE_BYTES; i++)
    {
        writeDataBuffer[i] = i;
    }
}
// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main ( void )
{
    uint8_t status_reg;

    /* Initialize all modules */
    SYS_Initialize ( NULL );

    while(1)
    {
        /* Check the application's current state. */
        switch (appData.state)
        {
            case APP_STATE_INITIALIZE:
                APP_Initialize();
                appData.state = APP_STATE_RESET;
                break;

            case APP_STATE_RESET:
                APP_Reset();
                appData.state = APP_STATE_GLOBAL_BLK_PROTECTION_UNLOCK;
                break;

            case APP_STATE_GLOBAL_BLK_PROTECTION_UNLOCK:
                APP_GlobalWriteProtectionUnlock();
                appData.state = APP_STATE_JEDEC_ID_READ;
                break;

            case APP_STATE_JEDEC_ID_READ:
                APP_JEDEC_ID_Read(&appData.manufacturerID, &appData.deviceID);
                appData.state = APP_STATE_SECTOR_ERASE;
                break;

            case APP_STATE_SECTOR_ERASE:
                APP_SectorErase(APP_MEM_ADDR);
                appData.state = APP_STATE_READ_STATUS;
                appData.nextState = APP_STATE_PAGE_PROGRAM;
                break;

            case APP_STATE_READ_STATUS:
                status_reg = APP_StatusRead();
                if ((status_reg & APP_STATUS_BIT_BUSY) == 0)
                {
                    appData.state = appData.nextState;
                }
                break;

            case APP_STATE_PAGE_PROGRAM:
                APP_PageProgram(APP_MEM_ADDR, &writeDataBuffer[0]);
                appData.state = APP_STATE_READ_STATUS;
                appData.nextState = APP_STATE_MEMORY_READ;
                break;

            case APP_STATE_MEMORY_READ:
                APP_MemoryRead(APP_MEM_ADDR, readDataBuffer, APP_PAGE_PROGRAM_SIZE_BYTES);
                appData.state = APP_STATE_VERIFY;
                break;

            case APP_STATE_VERIFY:
                if (memcmp(writeDataBuffer, readDataBuffer, APP_PAGE_PROGRAM_SIZE_BYTES) == 0)
                {
                    appData.state = APP_STATE_XFER_SUCCESSFUL;
                }
                else
                {
                    appData.state = APP_STATE_XFER_ERROR;
                }
                break;

            case APP_STATE_XFER_SUCCESSFUL:
                LED_On();
                break;

            case APP_STATE_XFER_ERROR:
                LED_Off();
                break;

            default:
                break;
        }
    }
}