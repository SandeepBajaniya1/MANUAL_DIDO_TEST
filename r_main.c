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
 * File Name    : r_main.c
 * Version      : CodeGenerator for RL78/G13 V2.05.03.01 [12 Nov 2018]
 * Device(s)    : R5F100MF
 * Tool-Chain   : CA78K0R
 * Description  : This file implements main function.
 * Creation Date: 2/2/2021
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
#include "I2C.h"
#include "string.h"
#include "aes.h"
#include "rtc_prod.h"
#include "meter_comm.h"
#include "pc_comm.h"
#include "switches.h"
#include "adc_operations.h"
#include "leds_operations.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"
#include <stdio.h>
#include "error_codes.h"
// #include "jig_operations.h"
#include "SerialData.h"
#include "r_main.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
// extern COMM_PROCESS stComm[4];
// BYTE by10MsTick, by10MsTick2;
extern unsigned char t_hr, t_min, t_sec, prev_sec, d_day, d_mnth, d_yr;
float fVoltage, fCurrentPhase, fCurrentNeutral, fkWh;
uint32_t dMtrSerial = 0xFFFFFFFE;
WORD wRunLEDTime = 0;
WORD wNextPktTime = 0;
WORD wWaitTime = 1500;
BYTE byIndex, byIndex1, byIndex2;
BYTE bySndPktIndex = 0;
BYTE byJmpModePktIndex = 0;
BYTE byLatchTestPass = 0;
BYTE byRFTest = 0;
WORD wPercent;

BYTE byIn1LastState = 0;
BYTE byIn2LastState = 0;
BYTE byIn3LastState = 0;

BYTE byIn1State = 0;
BYTE byIn2State = 0;
BYTE byIn3State = 0;

BYTE byIn1Time = 0;
BYTE byIn2Time = 0;
BYTE byIn3Time = 0;

BYTE byIn1Count = 0;
BYTE byIn2Count = 0;
BYTE byIn3Count = 0;

BYTE byTestResult = 0;

uint16_t wTimeout = 0;
BYTE byAuthanticationOK = 0;

BYTE byRespBuf[64];

uint8_t byTestByte = 0;
BYTE *byCmdPtr, *byRspPtr;

float fTemp;
ST_RELAY_TEST stRlyChk[MAX_RELAY_COUNT];

UN_VOLTAGE_FAIL_STEP unVFS;
UN_BOARD_TEST_VALUES unBoardValues;

// global variable used in new arch
e_jigErrorCodes gRTC_ret = RTC_CALIB_ERROR;
uint8_t g_pcb_bat_status = 0;
uint8_t g_pcb_mainDcVtg_status = 0;
uint8_t g_pcb_sleepCurrent_status = 0;
uint8_t g_pcb_superCapVtg_status = 0;
uint8_t g_KWH_status  = 0;
uint8_t g_kVarh_status  = 0; 


/* End user code. Do not edit comment generated here */
void R_MAIN_UserInit(void);

/***********************************************************************************************************************
 * Function Name: main
 * Description  : This function implements main function.
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
void main(void)
{

    char tempStr[30];
    uint8_t byMainIndex;
    unsigned int i;

    R_MAIN_UserInit();
    /* Start user code. Do not edit comment generated here */
    //RELAY_BAT_CUR_CHK_ON();
    // RELAY_SCAN_SWITCH_ON();

    // RELAY_BAT_CUR_CHK_OFF();
   // wTimeout = Crc_fromBuffer(&byCmdDfFrmt[1], 5);
    
    
    /*only for dido test*/

    manual_jig_funcTest_Digital_IO();
    glow_allPassLed();
    while(1)
    {
        delay_ms(1000);
    }



    // For version 00X architecture
    if (0 == SW_IN_9)
    {
        old_arch();
    }

    // RELAY_POWER_ON();
    //  Without PC operation with new arch
    if (0 != SW_IN_10_PC)
    {
        without_PC_operation();
    }

    while (1U)
    {
        // For new version 10x architecture
        pc_comm_startPcOperation();
        delay_ms(100);
    }
}

/***********************************************************************************************************************
 * Function Name: R_MAIN_UserInit
 * Description  : This function adds user code before implementing main function.
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
void R_MAIN_UserInit(void)
{
    /* Start user code. Do not edit comment generated here */
    hdwinit();
    R_IT_Start();
    R_UART0_Start();
    R_UART1_Start();
    // Port1RxEnable();
    R_UART2_Start();
    R_UART3_Start();
    // Port3RxEnable();
    // RTC_Init();
    EI();

    getSwitchStatus(); // Update switches status
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

void delay_ms(uint16_t wDelay)
{
    do
    {
        by10MsTick = 0;
        while (0 == by10MsTick)
            ;
        if (0 != wDelay)
        {
            --wDelay;
        }
    } while (wDelay);
}

void old_arch()
{
    //--
    if (0 == SW_IN_7)
    {
        // Battery Voltages
        sw_7_operationBatteryVoltage();
    }

    if (0 == SW_IN_10_PC)
    {
        /**
         1.Get the data from meter and RTC calibration is shifted in single while loop.
         2.We can also bypass the RTC calibration
         */

        RELAY_POWER_ON();
        delay_ms(2000);
        // while (GET_INT_FIRM_VERSION != checkPCComm())
        //     ;

        // pc_comm_startPcOperation();

        while (START_TEST != checkPCComm())
            ; // Wait for start again from PC if version of meter matches
    }
    // RELAY_SCAN_SWITCH_OFF();

    /*Task 1*/
    if (0 == SW_IN_1)
    {
        // Format Meter
        sw_1_operation();
    }

    /*Task 6*/
    if (0 == SW_IN_2)
    {
        // Meter Reset
        sw_2_operation();
    }
    RELAY_POWER_ON();
    // rtc_prod_rtcCalibrate(RTC_Comp_Pulse_start, 0); // Initiate 1-Hz Pulse in Meter
    if (g_ret != RTC_CALIB_ERROR)
    {
        uint8_t rtcRetry = 0;
        delay_ms(10000);
        while (1)
        {
            meter_comm_setInstantParam(RTC_Comp_Pulse_start, 0, 1,NORM_RESP); // Initiate Calibration in Meter
                                                                    // Wait for meter acknowledgement
            if (meter_comm_checkMeterResponse(OPTICAL_PORT, 15, NORM_RESP) == METER_RESPONSE_TIMEOUT)
            {
                rtcRetry++;
                if (rtcRetry > 5)
                {
                    g_ret = RTC_CALIB_ERROR;
                    break;
                }
                delay_ms(1000);
                continue;
            }
            break;
        }
    }

    /*Task 7: This test will be done for Non-LTCT meters only*/
    if (0 == SW_IN_3_RELAY) // Relay
    {
        // Relay Test
        sw_3_operation();
    }

    /*Task 5*/
    if (0 == SW_IN_4)
    {
        // Current and voltage Test
        sw_4_operation();
    }

    /*Task 8*/
    if (0 == SW_IN_5) // RF + IOs
    {
        // RF + IO Test
        sw_5_operation();
    }

    if (0 == SW_IN_6) // Switch check
    {
        sw_6_operation();
    }

    // If above three switches are OPEN then we may get wrong value
    if (g_ret != RTC_CALIB_ERROR)
    {
        R_UART2_Stop();
        R_UART3_Stop();
        R_SAU1_Create(9600);
        R_UART3_Start();
        delay_ms(15000);
        if ((SW_IN_5 != 0) || (SW_IN_6 != 0))
        {
            delay_ms(15000);
        }
        g_ret = rtc_prod_getAvgPPM(g_data, &g_dataLen);
        R_UART2_Stop();
        R_UART3_Stop();
        R_SAU1_Create(19200);
        R_UART2_Start();
        R_UART3_Start();
    }
    if (0 == SW_IN_7)
    {
        // Super Capacitor Testing
        sw_7_operationSuperCapacitor();
    }

    /*Task 4*/
    sw_3_finalTask();

    RELAY_POWER_ON();
    // Sending final Packet
    // if (0 == SW_IN_10_PC)
    {
        sendPCData(FINAL_PKT);
    }

    while (1)
    {
        // checkPCComm();
        pc_comm_startPcOperation();
    }
}
/* End user code. Do not edit comment generated here */