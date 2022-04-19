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

#define LED_ON()        LED5_Clear()
#define LED_OFF()       LED5_Set()
#define LED_TOGGLE()    LED5_Toggle()

#define RX_BUFFER_SIZE 256

char messageStart[] = "**** UART Line Echo Demo: Blocking Transfer without the interrupt ****\r\n\
**** Type a line of characters and press the Enter key. **** \r\n\
**** Entered line will be echoed back, and the LED is toggled. ****\r\n";
char newline[] = "\r\n";
char errorMessage[] = "\r\n**** UART error has occurred ****\r\n";
char receiveBuffer[RX_BUFFER_SIZE] = {};
char data = 0;

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main ( void )
{
    uint8_t rxCounter=0;

    /* Initialize all modules */
    SYS_Initialize ( NULL );

    /* Send start message */
    UART0_Write(&messageStart, sizeof(messageStart));

    while ( true )
    {
        /* Check if there is a received character */
        if(UART0_ReceiverIsReady() == true)
        {
            if(UART0_ErrorGet() == UART_ERROR_NONE)
            {
                UART0_Read(&data, 1);

                if((data == '\n') || (data == '\r'))
                {
                    UART0_Write(newline,sizeof(newline));
                    UART0_Write(receiveBuffer,rxCounter);
                    UART0_Write(newline,sizeof(newline));
                    rxCounter = 0;     
                    LED_TOGGLE();
                }
                else
                {
                    receiveBuffer[rxCounter++] = data;
                }
            }
            else
            {
                UART0_Write(errorMessage,sizeof(errorMessage));
            }
        }
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}

/*******************************************************************************
 End of File
*/

