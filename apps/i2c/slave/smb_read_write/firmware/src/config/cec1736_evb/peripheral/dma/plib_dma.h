/*******************************************************************************
  DMAC Peripheral Library Interface Header File

  Company:
    Microchip Technology Inc.

  File Name:
    plib_dma.h

  Summary:
    DMA peripheral library interface.

  Description:
    This file defines the interface to the DMAC peripheral library. This
    library provides access to and control of the DMAC controller.

  Remarks:
    None.

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

#ifndef PLIB_DMA_H    // Guards against multiple inclusion
#define PLIB_DMA_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

/*  This section lists the other files that are included in this file.
*/
#include <device.h>
#include <string.h>
#include <stdbool.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************

typedef enum
{
    /* DMA Channel 0 */
    DMA_CHANNEL_0 = 0,
} DMA_CHANNEL;

typedef enum
{
    /* No event */
    DMA_TRANSFER_EVENT_NONE = 0,

    /* Data was transferred successfully. */
    DMA_TRANSFER_EVENT_COMPLETE = 1,

    /* Error while processing the request */
    DMA_TRANSFER_EVENT_ERROR = 2

} DMA_TRANSFER_EVENT;

typedef uint8_t DMA_INT;

#define DMA_INT_BUS_ERROR                                   (DMA_CHAN00_ISTS_BUS_ERR_Msk)
#define DMA_INT_HW_FLOW_CONTROL                             (DMA_CHAN00_ISTS_FLOW_CTRL_Msk)
#define DMA_INT_TRANSFER_DONE                               (DMA_CHAN00_ISTS_DONE_Msk)

typedef uint32_t DMA_CHANNEL_CONFIG;

typedef void (*DMA_CHANNEL_CALLBACK) (DMA_TRANSFER_EVENT event, uintptr_t contextHandle);

void DMA_Initialize( void );
bool DMA_ChannelTransfer( DMA_CHANNEL channel, const void *srcAddr, const void *destAddr, size_t blockSize );
void DMA_ChannelCallbackRegister( DMA_CHANNEL channel, const DMA_CHANNEL_CALLBACK callback, const uintptr_t context );
bool DMA_ChannelIsBusy ( DMA_CHANNEL channel );
void DMA_ChannelDisable ( DMA_CHANNEL channel );
void DMA_ChannelEnable ( DMA_CHANNEL channel );
void DMA_ChannelTransferAbort ( DMA_CHANNEL channel );
uint32_t DMA_ChannelGetTransferredCount( DMA_CHANNEL channel );
void DMA_ChannelInterruptEnable ( DMA_CHANNEL channel, DMA_INT intSources );
void DMA_ChannelInterruptDisable ( DMA_CHANNEL channel, DMA_INT intSources );
DMA_INT DMA_ChannelInterruptFlagsGet ( DMA_CHANNEL channel );
DMA_CHANNEL_CONFIG DMA_ChannelSettingsGet (DMA_CHANNEL channel);
bool DMA_ChannelSettingsSet (DMA_CHANNEL channel, DMA_CHANNEL_CONFIG setting);



// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
// DOM-IGNORE-END

#endif //PLIB_DMA_H
