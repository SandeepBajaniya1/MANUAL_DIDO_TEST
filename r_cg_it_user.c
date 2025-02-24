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
* File Name    : r_cg_it_user.c
* Version      : CodeGenerator for RL78/G13 V2.05.03.01 [12 Nov 2018]
* Device(s)    : R5F100MF
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for IT module.
* Creation Date: 8/3/2021
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt INTIT r_it_interrupt
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_it.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"
#include "leds_operations.h"
#include "SerialData.h"


/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
BYTE by10MsTick = 0, by10MsTick2 = 0;
extern WORD wRunLEDTime;
// extern COMM_PROCESS stComm[4];
extern BYTE byLedDisplayResult[];
extern ST_RELAY_TEST stRlyChk[];
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_it_interrupt
* Description  : This function is INTIT interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_it_interrupt(void)
{
	/* Start user code. Do not edit comment generated here */
	BYTE byIndex, byIndex2;
	by10MsTick = 1;
	by10MsTick2 = 1;
	if (stComm[0].wRcvTimeOut < 100)
	{
		stComm[0].wRcvTimeOut++;
	}
	if (stComm[1].wRcvTimeOut < 100)
	{
		stComm[1].wRcvTimeOut++;
	}
	if (stComm[2].wRcvTimeOut < 100)
	{
		stComm[2].wRcvTimeOut++;
	}
#ifdef WCM_3P
	if (stComm[3].wRcvTimeOut < 100)
	{
		stComm[3].wRcvTimeOut++;
	}
	if (stComm[4].wRcvTimeOut < 100)
	{
		stComm[4].wRcvTimeOut++;
	}
	if (stComm[5].wRcvTimeOut < 100)
	{
		stComm[5].wRcvTimeOut++;
	}
#endif
	wRunLEDTime++;
	if (wRunLEDTime > 499)
	{
		wRunLEDTime = 0;
		RUN_LED ^= 1;
		for (byIndex = 0; byIndex < MAX_LED; byIndex++)
		{
			SetLED(byIndex, byLedDisplayResult[byIndex]);
		}
	}

	if ((0 == RELAY_CHECK_IN1))
	{
		if (stRlyChk[0].byCount < 50)
		{
			stRlyChk[0].byCount++;
		}
	}
	if ((0 == RELAY_CHECK_IN2))
	{
		if (stRlyChk[1].byCount < 50)
		{
			stRlyChk[1].byCount++;
		}
	}
	if ((0 == RELAY_CHECK_IN3))
	{
		if (stRlyChk[2].byCount < 50)
		{
			stRlyChk[2].byCount++;
		}
	}
#ifdef WCM_3P
	if ((0 == RELAY_CHECK_IN4))
	{
		if (stRlyChk[3].byCount < 50)
		{
			stRlyChk[3].byCount++;
		}
	}
	if ((0 == RELAY_CHECK_IN5))
	{
		if (stRlyChk[4].byCount < 50)
		{
			stRlyChk[4].byCount++;
		}
	}
	if ((0 == RELAY_CHECK_IN6))
	{
		if (stRlyChk[5].byCount < 50)
		{
			stRlyChk[5].byCount++;
		}
	}
#endif
	/* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
