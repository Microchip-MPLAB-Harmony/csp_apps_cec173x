/*******************************************************************************
* Copyright (C) 2023 Microchip Technology Inc. and its subsidiaries.
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
char input = 0;
int16_t onTime = 0;
int16_t offTime = 0;
float dutyCycle = 0;
uint32_t onePercDutyCycleCnt = 0;
uint32_t period;

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    
    printf("\n\r---------------------------------------------------------");
    printf("\n\r                    PWM Demo                     ");
    printf("\n\r  Press '+'/'-' to increment/decrement duty cycle by 1%% ");
    printf("\n\r---------------------------------------------------------\n\r");
    
    period = (PWM0_OnCountGet() + 1 + PWM0_OffCountGet() + 1);
    dutyCycle = ((PWM0_OnCountGet() + 1)/(float)period) * 100.0;
    onePercDutyCycleCnt = (period)/100;
       
    printf("Frequency = %ld Hz\r\n", 48000000/(period));
    printf("Duty Cycle = %2.2f%%\r\n", dutyCycle);
                
    PWM0_Start();

    while ( true )
    {
        scanf("%c", &input);
        
        if ((input == '+') && (dutyCycle <= 99.0))
        {
            dutyCycle += 1;
        }
        else if ((input == '-') && (dutyCycle >= 1.0))
        {
            dutyCycle -= 1;
        }
        
        onTime = onePercDutyCycleCnt * dutyCycle;
        offTime = period - onTime;
        
        if (onTime >= 0 && onTime <= 65535 && offTime >= 0 && offTime <= 65535)
        {
            PWM0_OnCountSet(onTime);
            PWM0_OffCountSet(offTime);
            dutyCycle = (float)(onTime + 1)/(onTime + 1 + offTime + 1);
            dutyCycle *= 100;
            printf("Duty Cycle = %2.2f%%\r\n", dutyCycle);
        }
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

