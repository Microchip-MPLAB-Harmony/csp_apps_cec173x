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

volatile uint8_t targetRxBuffer[100];
volatile uint8_t targetTxBuffer[100];
volatile uint32_t nBytesRead = 0;
volatile uint32_t nDataBytes = 0;
volatile HOST_CMD command;

bool i2cSMB0TargetEventHandler (I2C_SMB_TARGET_TRANSFER_EVENT event, uintptr_t contextHandle)
{
    switch (event)
    {
        case I2C_SMB_TARGET_TRANSFER_EVENT_RX_READY:
            nBytesRead = I2CSMB0_TargetBufferRead((void*)targetRxBuffer);
            command = targetRxBuffer[1];
            if (command == HOST_CMD_WRITE_BYTE)
            {
                nDataBytes = 1;        
                targetTxBuffer[0] = targetRxBuffer[2];
            }
            else if (command == HOST_CMD_WRITE_WORD)
            {
                nDataBytes = 2;            
                targetTxBuffer[0] = targetRxBuffer[2];
                targetTxBuffer[1] = targetRxBuffer[3];
            }
            else if ((command == HOST_CMD_WRITE_BLOCK) || (command == HOST_CMD_WRITE_READ_BLOCK))
            {
                nDataBytes = targetRxBuffer[2];
                targetTxBuffer[0] = nDataBytes;
                memcpy((void*)&targetTxBuffer[1], (void*)&targetRxBuffer[3], nDataBytes);
                nDataBytes = nDataBytes + 1;
            }                        
            break;
        case I2C_SMB_TARGET_TRANSFER_EVENT_TX_READY:
            I2CSMB0_TargetBufferWrite((void*)targetTxBuffer, nDataBytes);
            break;
        case I2C_SMB_TARGET_TRANSFER_EVENT_ERROR:
            break;
        case I2C_SMB_TARGET_TRANSFER_EVENT_DONE:
            break;
        default:
            break;
    }
    return true;
}

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    
    I2CSMB0_TargetCallbackRegister(i2cSMB0TargetEventHandler, 0);
    
    I2CSMB0_TargetStart();

    while (1)
    {
        /* Do nothing */
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

