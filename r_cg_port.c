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
* File Name    : r_cg_port.c
* Version      : CodeGenerator for RL78/G13 V2.05.03.01 [12 Nov 2018]
* Device(s)    : R5F100MF
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for PORT module.
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
#include "r_cg_port.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_PORT_Create
* Description  : This function initializes the Port I/O.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_PORT_Create(void)
{
#ifdef WCM_3P
      P0 = _01_Pn0_OUTPUT_1 | _02_Pn1_OUTPUT_1 | _00_Pn4_OUTPUT_0 | _20_Pn5_OUTPUT_1 | _40_Pn6_OUTPUT_1;
      P1 = _00_Pn5_OUTPUT_0 | _00_Pn6_OUTPUT_0 | _00_Pn7_OUTPUT_0;
      P3 = _01_Pn0_OUTPUT_1;
      P4 = _00_Pn3_OUTPUT_0 | _00_Pn4_OUTPUT_0 | _00_Pn5_OUTPUT_0;
      P6 = _00_Pn1_OUTPUT_0 | _08_Pn3_OUTPUT_1;
      P7 = _01_Pn0_OUTPUT_1 | _02_Pn1_OUTPUT_1 | _04_Pn2_OUTPUT_1 | _08_Pn3_OUTPUT_1 | _10_Pn4_OUTPUT_1 |
           _20_Pn5_OUTPUT_1;
      P10 = _01_Pn0_OUTPUT_1;
      P12 = _00_Pn0_OUTPUT_0;
      P14 = _00_Pn1_OUTPUT_0 | _04_Pn2_OUTPUT_1 | _40_Pn6_OUTPUT_1 | _80_Pn7_OUTPUT_1;
      P15 = _00_Pn3_OUTPUT_0 | _01_Pn0_OUTPUT_1;
      PU4 = _00_PUn0_PULLUP_OFF;
      PU6 = _40_PUn6_PULLUP_ON | _80_PUn7_PULLUP_ON;
      PU7 = _40_PUn6_PULLUP_ON | _80_PUn7_PULLUP_ON;
      PMC10 = _00_PMCn0_DI_ON | _FE_PMC10_DEFAULT;
      PMC12 = _00_PMCn0_DI_ON | _FE_PMC12_DEFAULT;
      PMC14 = _00_PMCn7_DI_ON | _7F_PMC14_DEFAULT;
      ADPC = _09_ADPC_DI_ON;
      PM0 = _00_PMn0_MODE_OUTPUT | _00_PMn1_MODE_OUTPUT | _04_PMn2_NOT_USE | _08_PMn3_NOT_USE | _00_PMn4_MODE_OUTPUT |
            _00_PMn5_MODE_OUTPUT | _00_PMn6_MODE_OUTPUT | _80_PM0_DEFAULT;
      PM1 = _01_PMn0_MODE_INPUT | _02_PMn1_NOT_USE | _04_PMn2_NOT_USE | _08_PMn3_NOT_USE | _10_PMn4_NOT_USE |
            _00_PMn5_MODE_OUTPUT | _00_PMn6_MODE_OUTPUT | _00_PMn7_MODE_OUTPUT;
      PM3 = _00_PMn0_MODE_OUTPUT | _02_PMn1_MODE_INPUT | _00_PMn4_MODE_OUTPUT | _EC_PM3_DEFAULT;
      PM4 = _01_PMn0_NOT_USE | _02_PMn1_MODE_INPUT | _04_PMn2_MODE_INPUT | _00_PMn3_MODE_OUTPUT | _00_PMn4_MODE_OUTPUT |
            _00_PMn5_MODE_OUTPUT | _C0_PM4_DEFAULT;
      PM5 = _01_PMn0_MODE_INPUT | _02_PMn1_MODE_INPUT | _04_PMn2_MODE_INPUT | _08_PMn3_MODE_INPUT | _10_PMn4_MODE_INPUT |
            _20_PMn5_MODE_INPUT | _C0_PM5_DEFAULT;
      PM6 = _01_PMn0_MODE_INPUT | _00_PMn1_MODE_OUTPUT | _04_PMn2_MODE_INPUT | _00_PMn3_MODE_OUTPUT |
            _10_PMn4_MODE_INPUT | _20_PMn5_MODE_INPUT | _40_PMn6_MODE_INPUT | _80_PMn7_MODE_INPUT;
      PM7 = _00_PMn0_MODE_OUTPUT | _00_PMn1_MODE_OUTPUT | _00_PMn2_MODE_OUTPUT | _00_PMn3_MODE_OUTPUT |
            _00_PMn4_MODE_OUTPUT | _00_PMn5_MODE_OUTPUT | _40_PMn6_MODE_INPUT | _80_PMn7_MODE_INPUT;
      PM10 = _00_PMn0_MODE_OUTPUT | _FE_PM10_DEFAULT;
      PM11 = _01_PMn0_MODE_INPUT | _02_PMn1_MODE_INPUT | _FC_PM11_DEFAULT;
      PM12 = _01_PMn0_MODE_INPUT | _FE_PM12_DEFAULT;
      PM14 = _01_PMn0_MODE_INPUT | _00_PMn1_MODE_OUTPUT | _00_PMn2_MODE_OUTPUT | _08_PMn3_NOT_USE | _10_PMn4_NOT_USE |
             _00_PMn6_MODE_OUTPUT | _00_PMn7_MODE_OUTPUT | _20_PM14_DEFAULT;
      PM15 = _02_PMn1_MODE_INPUT | _04_PMn2_MODE_INPUT | _00_PMn3_MODE_OUTPUT | _F0_PM15_DEFAULT;
#elif defined(LTCT_3P) || defined(HTCT_3P)
      P0 = _01_Pn0_OUTPUT_1 | _02_Pn1_OUTPUT_1 | _00_Pn4_OUTPUT_0 | _20_Pn5_OUTPUT_1 | _40_Pn6_OUTPUT_1;
      P1 = _00_Pn5_OUTPUT_0 | _00_Pn6_OUTPUT_0 | _00_Pn7_OUTPUT_0;
      P3 = _01_Pn0_OUTPUT_1;
      P4 = _00_Pn2_OUTPUT_0 | _00_Pn3_OUTPUT_0 | _00_Pn4_OUTPUT_0 | _00_Pn5_OUTPUT_0;
      P6 = _00_Pn1_OUTPUT_0 | _00_Pn2_OUTPUT_0 | _00_Pn3_OUTPUT_0;
      P7 = _01_Pn0_OUTPUT_1 | _02_Pn1_OUTPUT_1 | _04_Pn2_OUTPUT_1 | _08_Pn3_OUTPUT_1 | _10_Pn4_OUTPUT_1 |
           _20_Pn5_OUTPUT_1;
      P10 = _01_Pn0_OUTPUT_1;
      P12 = _00_Pn0_OUTPUT_0;
      P14 = _00_Pn0_OUTPUT_0 | _00_Pn1_OUTPUT_0 | _04_Pn2_OUTPUT_1 | _40_Pn6_OUTPUT_1 | _80_Pn7_OUTPUT_1;
      P15 = _00_Pn3_OUTPUT_0 | _01_Pn0_OUTPUT_1;
      PU4 = _00_PUn0_PULLUP_OFF;
      PU6 = _40_PUn6_PULLUP_ON | _80_PUn7_PULLUP_ON;
      PU7 = _40_PUn6_PULLUP_ON | _80_PUn7_PULLUP_ON;
      PMC10 = _00_PMCn0_DI_ON | _FE_PMC10_DEFAULT;
      PMC12 = _00_PMCn0_DI_ON | _FE_PMC12_DEFAULT;
      PMC14 = _00_PMCn7_DI_ON | _7F_PMC14_DEFAULT;
      ADPC = _09_ADPC_DI_ON;
      PM0 = _00_PMn0_MODE_OUTPUT | _00_PMn1_MODE_OUTPUT | _04_PMn2_NOT_USE | _08_PMn3_NOT_USE | _00_PMn4_MODE_OUTPUT |
            _00_PMn5_MODE_OUTPUT | _00_PMn6_MODE_OUTPUT | _80_PM0_DEFAULT;
      PM1 = _01_PMn0_MODE_INPUT | _02_PMn1_NOT_USE | _04_PMn2_NOT_USE | _08_PMn3_NOT_USE | _10_PMn4_NOT_USE |
            _00_PMn5_MODE_OUTPUT | _00_PMn6_MODE_OUTPUT | _00_PMn7_MODE_OUTPUT;
      PM3 = _00_PMn0_MODE_OUTPUT | _02_PMn1_MODE_INPUT | _00_PMn4_MODE_OUTPUT | _EC_PM3_DEFAULT;
      PM4 = _01_PMn0_NOT_USE | _02_PMn1_MODE_INPUT | _00_PMn2_MODE_OUTPUT | _00_PMn3_MODE_OUTPUT | _00_PMn4_MODE_OUTPUT |
            _00_PMn5_MODE_OUTPUT | _C0_PM4_DEFAULT;
      PM5 = _01_PMn0_MODE_INPUT | _02_PMn1_MODE_INPUT | _04_PMn2_MODE_INPUT | _08_PMn3_MODE_INPUT | _10_PMn4_MODE_INPUT |
            _20_PMn5_MODE_INPUT | _C0_PM5_DEFAULT;
      PM6 = _01_PMn0_MODE_INPUT | _00_PMn1_MODE_OUTPUT | _00_PMn2_MODE_OUTPUT | _00_PMn3_MODE_OUTPUT |
            _10_PMn4_MODE_INPUT | _20_PMn5_MODE_INPUT | _40_PMn6_MODE_INPUT | _80_PMn7_MODE_INPUT;
      PM7 = _00_PMn0_MODE_OUTPUT | _00_PMn1_MODE_OUTPUT | _00_PMn2_MODE_OUTPUT | _00_PMn3_MODE_OUTPUT |
            _00_PMn4_MODE_OUTPUT | _00_PMn5_MODE_OUTPUT | _40_PMn6_MODE_INPUT | _80_PMn7_MODE_INPUT;
      PM10 = _00_PMn0_MODE_OUTPUT | _FE_PM10_DEFAULT;
      PM11 = _01_PMn0_MODE_INPUT | _02_PMn1_MODE_INPUT | _FC_PM11_DEFAULT;
      PM12 = _01_PMn0_MODE_INPUT | _FE_PM12_DEFAULT;
      PM14 = _00_PMn0_MODE_OUTPUT | _00_PMn1_MODE_OUTPUT | _00_PMn2_MODE_OUTPUT | _08_PMn3_NOT_USE | _10_PMn4_NOT_USE |
             _00_PMn6_MODE_OUTPUT | _00_PMn7_MODE_OUTPUT | _20_PM14_DEFAULT;
      PM15 = _02_PMn1_MODE_INPUT | _04_PMn2_MODE_INPUT | _00_PMn3_MODE_OUTPUT | _F0_PM15_DEFAULT;
#else
      P0 = _01_Pn0_OUTPUT_1 | _02_Pn1_OUTPUT_1 | _00_Pn4_OUTPUT_0 | _20_Pn5_OUTPUT_1 | _40_Pn6_OUTPUT_1;
      P1 = _00_Pn5_OUTPUT_0 | _00_Pn6_OUTPUT_0 | _00_Pn7_OUTPUT_0;
      P3 = _01_Pn0_OUTPUT_1;
      P4 = _00_Pn2_OUTPUT_0 | _00_Pn3_OUTPUT_0 | _00_Pn4_OUTPUT_0 | _00_Pn5_OUTPUT_0;
      P6 = _00_Pn1_OUTPUT_0 | _04_Pn2_OUTPUT_1 | _08_Pn3_OUTPUT_1;
      P7 = _01_Pn0_OUTPUT_1 | _02_Pn1_OUTPUT_1 | _04_Pn2_OUTPUT_1 | _08_Pn3_OUTPUT_1 | _10_Pn4_OUTPUT_1 |
           _20_Pn5_OUTPUT_1;
      P10 = _01_Pn0_OUTPUT_1;
      P12 = _00_Pn0_OUTPUT_0;
      P14 = _00_Pn0_OUTPUT_0 | _00_Pn1_OUTPUT_0 | _04_Pn2_OUTPUT_1 | _40_Pn6_OUTPUT_1 | _80_Pn7_OUTPUT_1;
      P15 = _00_Pn3_OUTPUT_0 | _01_Pn0_OUTPUT_1;
      PU4 = _00_PUn0_PULLUP_OFF;
      PU6 = _40_PUn6_PULLUP_ON | _80_PUn7_PULLUP_ON;
      PU7 = _40_PUn6_PULLUP_ON | _80_PUn7_PULLUP_ON;
      PMC10 = _00_PMCn0_DI_ON | _FE_PMC10_DEFAULT;
      PMC12 = _00_PMCn0_DI_ON | _FE_PMC12_DEFAULT;
      PMC14 = _00_PMCn7_DI_ON | _7F_PMC14_DEFAULT;
      ADPC = _09_ADPC_DI_ON;
      PM0 = _00_PMn0_MODE_OUTPUT | _00_PMn1_MODE_OUTPUT | _04_PMn2_NOT_USE | _08_PMn3_NOT_USE | _00_PMn4_MODE_OUTPUT |
            _00_PMn5_MODE_OUTPUT | _00_PMn6_MODE_OUTPUT | _80_PM0_DEFAULT;
      PM1 = _01_PMn0_MODE_INPUT | _02_PMn1_NOT_USE | _04_PMn2_NOT_USE | _08_PMn3_NOT_USE | _10_PMn4_NOT_USE |
            _00_PMn5_MODE_OUTPUT | _00_PMn6_MODE_OUTPUT | _00_PMn7_MODE_OUTPUT;
      PM3 = _00_PMn0_MODE_OUTPUT | _02_PMn1_MODE_INPUT | _00_PMn4_MODE_OUTPUT | _EC_PM3_DEFAULT;
      PM4 = _01_PMn0_NOT_USE | _02_PMn1_MODE_INPUT | _00_PMn2_MODE_OUTPUT | _00_PMn3_MODE_OUTPUT | _00_PMn4_MODE_OUTPUT |
            _00_PMn5_MODE_OUTPUT | _C0_PM4_DEFAULT;
      PM5 = _01_PMn0_MODE_INPUT | _02_PMn1_MODE_INPUT | _04_PMn2_MODE_INPUT | _08_PMn3_MODE_INPUT | _10_PMn4_MODE_INPUT |
            _20_PMn5_MODE_INPUT | _C0_PM5_DEFAULT;
      PM6 = _01_PMn0_MODE_INPUT | _00_PMn1_MODE_OUTPUT | _00_PMn2_MODE_OUTPUT | _00_PMn3_MODE_OUTPUT |
            _10_PMn4_MODE_INPUT | _20_PMn5_MODE_INPUT | _40_PMn6_MODE_INPUT | _80_PMn7_MODE_INPUT;
      PM7 = _00_PMn0_MODE_OUTPUT | _00_PMn1_MODE_OUTPUT | _00_PMn2_MODE_OUTPUT | _00_PMn3_MODE_OUTPUT |
            _00_PMn4_MODE_OUTPUT | _00_PMn5_MODE_OUTPUT | _40_PMn6_MODE_INPUT | _80_PMn7_MODE_INPUT;
      PM10 = _00_PMn0_MODE_OUTPUT | _FE_PM10_DEFAULT;
      PM11 = _01_PMn0_MODE_INPUT | _02_PMn1_MODE_INPUT | _FC_PM11_DEFAULT;
      PM12 = _01_PMn0_MODE_INPUT | _FE_PM12_DEFAULT;
      PM14 = _00_PMn0_MODE_OUTPUT | _00_PMn1_MODE_OUTPUT | _00_PMn2_MODE_OUTPUT | _08_PMn3_NOT_USE | _10_PMn4_NOT_USE |
             _00_PMn6_MODE_OUTPUT | _00_PMn7_MODE_OUTPUT | _20_PM14_DEFAULT;
      PM15 = _02_PMn1_MODE_INPUT | _04_PMn2_MODE_INPUT | _00_PMn3_MODE_OUTPUT | _F0_PM15_DEFAULT;
#endif
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
