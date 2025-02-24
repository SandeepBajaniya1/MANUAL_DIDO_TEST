/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011, 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_systeminit.c
* Version      : CodeGenerator for RL78/G13 V2.05.03.01 [12 Nov 2018]
* Device(s)    : R5F100MF
* Tool-Chain   : CA78K0R
* Description  : This file implements system initializing function.
* Creation Date: 8/3/2021
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_serial.h"
#include "r_cg_adc.h"
#include "r_cg_it.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
unsigned int g_swStatus = 0;
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Systeminit
* Description  : This function initializes every macro.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Systeminit(void)
{
    PIOR = 0x15U;
    R_CGC_Get_ResetSource();
    R_CGC_Create();
    R_PORT_Create();
    R_SAU0_Create();
    R_SAU1_Create(19200);
    R_ADC_Create();
    R_IT_Create();

    IAWCTL = 0x80U;
}


/***********************************************************************************************************************
* Function Name: hdwinit
* Description  : This function initializes hardware setting.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void hdwinit(void)
{
    DI();
    R_Systeminit();
}

/* Start user code for adding. Do not edit comment generated here */

/**
 * This function stores the staus of switches (0 or 1) in variable.
 */
void getSwitchStatus(void)
{
    unsigned int mask = 1;

    if(SW_IN_1 == 0)
        g_swStatus |= mask << 0;
    
    if(SW_IN_2 == 0)
        g_swStatus |= mask << 1;

    if(SW_IN_3_RELAY == 0)
        g_swStatus |= mask << 2;

    if(SW_IN_4 == 0)
        g_swStatus |= mask << 3;
    
    if(SW_IN_5 == 0)
        g_swStatus |= mask << 4;
    
    if(SW_IN_6 == 0)
        g_swStatus |= mask << 5;
    
    if(SW_IN_7 == 0)
        g_swStatus |= mask << 6;
    
    if(SW_IN_8 == 0)
        g_swStatus |= mask << 7;
    
    if(SW_IN_9 == 0)
        g_swStatus |= mask << 8;

    if(SW_IN_10_PC == 0)
        g_swStatus |= mask << 9;
    
    if(SW_IN_11_1PH_3PH == 0)
        g_swStatus |= mask << 10;
    
    if(SW_IN_12_9600_BAUD == 0)
        g_swStatus |= mask << 11;
    
    g_swStatus = ~g_swStatus; // 0: ON, 1: OFF
}


/* End user code. Do not edit comment generated here */
