/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the XMC MCU: ERU external interrupt example
*              for ModusToolbox.
*              External signal is feed to Event Request Unit (ERU) using one of the GPIO pin.
*              Service request interrupt is generated based on the falling edge of the external signal.
*              LED is toggle at every falling edge of the external signal inside the ERU service request interrupt.
*
* Related Document: See README.md
*
******************************************************************************
*
* Copyright (c) 2015-2021, Infineon Technologies AG
* All rights reserved.                        
*                                             
* Boost Software License - Version 1.0 - August 17th, 2003
* 
* Permission is hereby granted, free of charge, to any person or organization
* obtaining a copy of the software and accompanying documentation covered by
* this license (the "Software") to use, reproduce, display, distribute,
* execute, and transmit the Software, and to prepare derivative works of the
* Software, and to permit third-parties to whom the Software is furnished to
* do so, all subject to the following:
* 
* The copyright notices in the Software and this entire statement, including
* the above license grant, this restriction and the following disclaimer,
* must be included in all copies of the Software, in whole or in part, and
* all derivative works of the Software, unless such copies or derivative
* works are solely in the form of machine-executable object code generated by
* a source language processor.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
* SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
* FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*                                                                              
*****************************************************************************/

#include "cybsp.h"
#include "cy_utils.h"
#include "xmc_eru.h"

/*******************************************************************************
* Macros
*******************************************************************************/

#ifdef  TARGET_KIT_XMC14_BOOT_001
#define EXTERNAL_INPUT_SIGNAL_PIN     ERU0_ETL1_INPUTA_P2_5
#define ERU_GROUP_ETL_CHANNEL         ERU0_ETL1
#define ERU_GROUP_OGU_CHANNEL         ERU0_OGU0
#define INTERRUPT_PRIORITY_NODE_ID    IRQ3_IRQn
#define INTERRUPT_EVENT_PRIORITY      (3U)
#define ERU_EXTERNAL_EVENT_HANDLER    IRQ_Hdlr_3
#endif

#ifdef  TARGET_KIT_XMC47_RELAX_V1
#define EXTERNAL_INPUT_SIGNAL_PIN     ERU1_ETL1_INPUTA_P1_15
#define ERU_GROUP_ETL_CHANNEL         ERU1_ETL1
#define ERU_GROUP_OGU_CHANNEL         ERU1_OGU0
#define INTERRUPT_PRIORITY_NODE_ID    ERU1_0_IRQn
#define INTERRUPT_EVENT_PRIORITY      (63U)
#define ERU_EXTERNAL_EVENT_HANDLER    IRQ_Hdlr_5
#endif

/*******************************************************************************
* Data Structure
*******************************************************************************/
/*Structure for initializing ERUx_ETLy module*/
XMC_ERU_ETL_CONFIG_t button_event_generator_config =
{
    .input                  = EXTERNAL_INPUT_SIGNAL_PIN,            /*Configures input signal for Event request source unit */
    .source                 = XMC_ERU_ETL_SOURCE_A,                 /*Input path combination along with polarity for event generation.*/
    .edge_detection         = XMC_ERU_ETL_EDGE_DETECTION_FALLING,   /*Configure the event trigger edge(FE, RE)*/
    .status_flag_mode       = XMC_ERU_ETL_STATUS_FLAG_MODE_HWCTRL,  /*Enables the status flag for sticky mode or non sticky mode*/
    .enable_output_trigger  = true,                                 /*Enables the generation of trigger pulse(PE), for the configured edge detection*/
    .output_trigger_channel = XMC_ERU_ETL_OUTPUT_TRIGGER_CHANNEL0   /*Output channel select(OCS) for ETLx output trigger pulse*/
};

/*Structure for initializing ERUx_OGUy module.*/
XMC_ERU_OGU_CONFIG_t button_event_detection_config =
{
    .service_request        = XMC_ERU_OGU_SERVICE_REQUEST_ON_TRIGGER /*Gating(GP) on service request generation for pattern detection result*/
};

/*******************************************************************************
* Function Name: ERU_EXTERNAL_EVENT_HANDLER
********************************************************************************
* Summary:
* This is the interrupt handler function for the ERU external interrupt.
*
* Parameters:
*  none
*
* Return:
*  void
*
*******************************************************************************/
void ERU_EXTERNAL_EVENT_HANDLER(void)
{
    /*Toggle the LED*/
    XMC_GPIO_ToggleOutput(CYBSP_USER_LED_PORT, CYBSP_USER_LED_PIN);
}

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function.
*  1. It demonstrate the usage of Event Request Unit(ERU).
*  2. ERU consist of Event Trigger logic (ETL), which is configured to trigger an event during the falling edge of an external signal.
*  3. Trigger output from ETL is directed to one of the Output Gating Unit(OGU), which is configured to generate a service request.
*  4. For every falling edge of the external signal, LED is toggled inside the interrupt service request.
*
* Parameters:
*  none
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{
    cy_rslt_t result;

    /* Initialize the device and board peripherals */
    result = cybsp_init();
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /*Initializes the selected ERU_ETLx channel with the configuration structure*/
    XMC_ERU_ETL_Init(ERU_GROUP_ETL_CHANNEL, &button_event_generator_config);

    /*Initializes the selected ERU_OGUy channel with the configuration structure*/
    XMC_ERU_OGU_Init(ERU_GROUP_OGU_CHANNEL, &button_event_detection_config);
    
#ifdef TARGET_KIT_XMC14_BOOT_001
    /*Set Priority for IRQ*/
    NVIC_SetPriority(INTERRUPT_PRIORITY_NODE_ID,INTERRUPT_EVENT_PRIORITY);
#endif

#ifdef TARGET_KIT_XMC47_RELAX_V1
    /*Set Priority for IRQ*/
    NVIC_SetPriority(INTERRUPT_PRIORITY_NODE_ID,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),INTERRUPT_EVENT_PRIORITY, 0));
#endif

    /*Enable the Interrupt*/
    NVIC_EnableIRQ(INTERRUPT_PRIORITY_NODE_ID);

    /*Infinite loop */
    while(1)
    {
     
    }
}

/* [] END OF FILE */
