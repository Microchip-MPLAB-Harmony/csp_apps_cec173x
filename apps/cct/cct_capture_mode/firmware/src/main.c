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


// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************
volatile uint32_t capTimerCnt = 0;
volatile uint32_t diffCnt = 0;
volatile bool capInterrupt = false;
uint32_t freq = 0;

void cct_capture_callback(uintptr_t context)
{
    uint32_t timerCnt = CCT_FreeRunningTimerGet();
    if (timerCnt > capTimerCnt)
    {
        diffCnt = timerCnt - capTimerCnt;
    }
    else
    {
        diffCnt = (0xFFFFFFFF - capTimerCnt) + timerCnt;
    }
    capTimerCnt = timerCnt;
    capInterrupt = true;
}

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    
    printf("\n\r---------------------------------------------------------");
    printf("\n\r                    CCT Capture Demo                     ");
    printf("\n\r---------------------------------------------------------\n\r");
    
    CCT_Capture0CallbackRegister(cct_capture_callback, 0);
    
    CCT_FreeRunningTimerStart();
    
    PWM0_Start();

    while ( true )
    {
        if (capInterrupt == true)
        {
            freq = CCT_FrequencyGet()/diffCnt;
            printf("Frequency : %ld Hz\r\n", freq);
            capInterrupt = false;
        }
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

