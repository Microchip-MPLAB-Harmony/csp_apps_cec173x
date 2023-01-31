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
#include <string.h>
#include "definitions.h"                // SYS function prototypes

char wdtDemoMessage[] = 
{
    "\n\r -------------------------------------------------------------"
    "\n\r                           WDT DEMO                           "
    "\n\r -------------------------------------------------------------)"
    "\n\r Enter a character to emulate deadlock "
};

char wdtDeadlockMessage[] = 
{
    "\n\r Emulating deadlock................ "
    "\n\r WDT should reset device in 4 seconds "
};
// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    
    UART0_Write(wdtDemoMessage, strlen(wdtDemoMessage));
    
    SYSTICK_TimerStart();
    
    WDT_Enable();
    
    while ( true )
    {
        if(UART0_ReceiverIsReady() == false)
        {
            if(SYSTICK_TimerPeriodHasExpired())
            {
                LED0_Toggle();
                WDT_Clear();
            }
        }
        else
        {   
            (void)UART0_ReadByte();
            UART0_Write(wdtDeadlockMessage, strlen(wdtDeadlockMessage));
            while(1);
        }
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

