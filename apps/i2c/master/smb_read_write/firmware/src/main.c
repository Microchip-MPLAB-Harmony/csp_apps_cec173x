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

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes


// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

#define I2C_SLAVE_ADDR                 0x54
#define LED_ON()                       LED5_Set()
#define LED_OFF()                      LED5_Clear()

typedef enum
{
    HOST_STATE_WRITE_BYTE = 0,
    HOST_STATE_WRITE_WORD,
    HOST_STATE_WRITE_BLOCK,
    HOST_STATE_READ_BYTE,
    HOST_STATE_READ_WORD,
    HOST_STATE_READ_BLOCK,
    HOST_STATE_WRITE_READ_BLOCK,
    HOST_STATE_WAIT_FOR_TRANSFER_DONE,
    HOST_STATE_ERROR,
    HOST_STATE_SUCCESS,
}HOST_STATE;

typedef enum
{
    HOST_CMD_WRITE_BYTE,
    HOST_CMD_WRITE_WORD,
    HOST_CMD_WRITE_BLOCK,
    HOST_CMD_READ_BYTE,
    HOST_CMD_READ_WORD,
    HOST_CMD_READ_BLOCK,
    HOST_CMD_WRITE_READ_BLOCK,
}HOST_CMD;

volatile static bool transferDone = false;
volatile static uint32_t nBytesRead = 0;
static bool blockRead = false;
static uint32_t nBytesRequested = 0;
static uint8_t wrBuffer[50] = {0};
static uint8_t rdBuffer[50] = {0};

static volatile HOST_STATE state = HOST_STATE_WRITE_BYTE;
static HOST_STATE nextState;

void i2cSMB0HostEventHandler (I2C_SMB_HOST_TRANSFER_EVENT event, uintptr_t contextHandle)
{
    switch(event)
    {
        case I2C_SMB_HOST_TRANSFER_EVENT_RX_READY:
            nBytesRead = I2CSMB0_HostBufferRead(rdBuffer);
            break;

        case I2C_SMB_HOST_TRANSFER_EVENT_DONE:
            transferDone = true;
            break;

        case I2C_SMB_HOST_TRANSFER_EVENT_ERROR:
            state = HOST_STATE_ERROR;
            break;
            
        default:
            break;
    }
        
}

int main ( void )
{        
    uint32_t i = 0;
    uint32_t data = 1;
    
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    
    I2CSMB0_HostCallbackRegister(i2cSMB0HostEventHandler, 0);
    
    LED_OFF();
    
    while (1)
    {
        switch(state)
        {
            case HOST_STATE_WAIT_FOR_TRANSFER_DONE:
                if (transferDone == true)
                {
                    transferDone = false;
                    state = nextState;

                    if (nBytesRequested > 0)
                    {
                        i = 0;
                        data = 1;
                        
                        if (blockRead == true)
                        {
                            // for block reads, the first byte indicates the number of bytes transmitted by the slave
                            nBytesRead =  rdBuffer[i++];    
                            blockRead = false;
                        }                        
                        if (nBytesRead != nBytesRequested)
                        {
                            state = HOST_STATE_ERROR;
                            break;
                        }
                        while (nBytesRequested--)
                        {
                            if (rdBuffer[i++] != data++)
                            {
                                state = HOST_STATE_ERROR;
                                break;
                            }                            
                        }

                        nBytesRequested = 0;
                    }
                }
                break;
                
            case HOST_STATE_WRITE_BYTE:
                wrBuffer[0] = 1;
                I2CSMB0_HostWriteByte(I2C_SLAVE_ADDR, HOST_CMD_WRITE_BYTE, wrBuffer);
                state = HOST_STATE_WAIT_FOR_TRANSFER_DONE;
                nextState = HOST_STATE_READ_BYTE;
                break;
                
            case HOST_STATE_READ_BYTE:
                nBytesRequested = 1;
                I2CSMB0_HostReadByte(I2C_SLAVE_ADDR, HOST_CMD_READ_BYTE);
                state = HOST_STATE_WAIT_FOR_TRANSFER_DONE;
                nextState = HOST_STATE_WRITE_WORD;
                break;

            case HOST_STATE_WRITE_WORD:
                wrBuffer[0] = 1;
                wrBuffer[1] = 2;
                I2CSMB0_HostWriteWord(I2C_SLAVE_ADDR, HOST_CMD_WRITE_WORD, wrBuffer);
                state = HOST_STATE_WAIT_FOR_TRANSFER_DONE;
                nextState = HOST_STATE_READ_WORD;
                break;
                
            case HOST_STATE_READ_WORD:
                nBytesRequested = 2;
                I2CSMB0_HostReadWord(I2C_SLAVE_ADDR, HOST_CMD_READ_WORD);
                state = HOST_STATE_WAIT_FOR_TRANSFER_DONE;
                nextState = HOST_STATE_WRITE_BLOCK;
                break;

            case HOST_STATE_WRITE_BLOCK:
                for (uint32_t i = 0; i < 32; i++)
                {
                    wrBuffer[i] = i+1;
                }
                I2CSMB0_HostWriteBlock(I2C_SLAVE_ADDR, HOST_CMD_WRITE_BLOCK, wrBuffer, 32);
                state = HOST_STATE_WAIT_FOR_TRANSFER_DONE;
                nextState = HOST_STATE_READ_BLOCK;
                break;
                
            case HOST_STATE_READ_BLOCK:
                nBytesRequested = 32;
                blockRead = true;
                I2CSMB0_HostReadBlock(I2C_SLAVE_ADDR, HOST_CMD_READ_BLOCK);
                state = HOST_STATE_WAIT_FOR_TRANSFER_DONE;
                nextState = HOST_STATE_WRITE_READ_BLOCK;
                break;
                     
            case HOST_STATE_WRITE_READ_BLOCK:
                nBytesRequested = 32;
                blockRead = true;
                I2CSMB0_HostWriteReadBlock(I2C_SLAVE_ADDR, HOST_CMD_WRITE_READ_BLOCK, wrBuffer, 32);
                state = HOST_STATE_WAIT_FOR_TRANSFER_DONE;
                nextState = HOST_STATE_SUCCESS;
                break;

            case HOST_STATE_SUCCESS:
                LED_ON();
                break;
                
            case HOST_STATE_ERROR:
                LED_OFF();
                break;
        }
    }
    

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

