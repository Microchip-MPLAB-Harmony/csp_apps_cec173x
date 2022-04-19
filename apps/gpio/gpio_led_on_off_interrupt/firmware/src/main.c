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

void timeout_handler(uintptr_t context)
{
	GPIO_PinToggle(GPIO_PIN_GPIO030);
}

void gpioCallbackHandler(GPIO_PIN pin, uintptr_t context)
{
    if(GPIO_PinRead(GPIO_PIN_GPIO132) == 1)
    {
        LED5_Clear();
    }
    else
    {
        LED5_Set();
    }
}
// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    
    GPIO_PinInterruptCallbackRegister(GPIO_PIN_GPIO132, gpioCallbackHandler, (uintptr_t)NULL);
    
    SYSTICK_TimerCallbackSet(&timeout_handler, (uintptr_t) NULL);
    
    SYSTICK_TimerStart();

    

    while ( true )
    {
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

