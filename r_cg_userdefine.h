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
* File Name    : r_cg_userdefine.h
* Version      : CodeGenerator for RL78/G13 V2.05.03.01 [12 Nov 2018]
* Device(s)    : R5F100MF
* Tool-Chain   : CA78K0R
* Description  : This file includes user definition.
* Creation Date: 8/3/2021
***********************************************************************************************************************/

#ifndef _USER_DEF_H
#define _USER_DEF_H

/***********************************************************************************************************************
User definitions
***********************************************************************************************************************/

/* Start user code for function. Do not edit comment generated here */
//#define WCM_3P
#define LTCT_3P
//#define HTCT_3P


typedef unsigned char	BYTE;
typedef unsigned short	WORD;
typedef unsigned long	DWORD;

#define SYNC_CHECK	20230228_1441

#define FALSE                   0
#define TRUE                    1
#define COMM_TEST_PKTS_CNT		2000
#define COMM_TEST_PKTS_TIME		300

#define SW_IN_1                 P12.4
#define SW_IN_2                 P12.3
#define SW_IN_3_RELAY           P13.7
#define SW_IN_4                 P3.1
#define SW_IN_5                 P6.4
#define SW_IN_6                 P6.5
#define SW_IN_7                 P6.6
#define SW_IN_8                 P6.7
#define SW_IN_9                 P7.7
#define SW_IN_10_PC             P7.6
#define SW_IN_11_1PH_3PH        P15.2       //0 == 3Phase
#define SW_IN_12_9600_BAUD      P15.1           // 0 = 19200
//#define SW_IN_13                P15.0
#define TOTAL_SWITCHES  12


#define LED_1		P7.5
#define LED_2		P7.4
#define LED_3		P7.3
#define LED_4		P7.2
#define LED_5		P7.1
#define LED_6		P7.0
#define LED_7		P0.6
#define LED_8		P0.5
#define LED_9		P3.0
#define LED_10		P14.6
#define LED_11		P0.1
#define LED_12		P0.0
#define LED_13		P14.2
#define LED_14		P13.0
#define LED_15		P13.0
#define LED_16		P13.0

#define MAX_LED         16

#define RUN_LED         P10.0

#define LDR_REACTIVE	P12.0
#define LDR_ACTIVE	    P6.0


//DIDO  pins
#define RELAY_SCAN          P0.4             
#define RELAY_CT		    P4.4        
#define RELAY_SHUNT		        P4.5            
#define DIDO_FOURTH         P14.7            


#define RELAY_SWITCH_CHK	    P6.1
#define RELAY_RESET		    P4.3
//#define RELAY_CT		    P4.4
//#define RELAY_SHUNT		    P4.5
//#define RELAY_FORMAT		      P4.2//P14.0
#define RELAY_POWER		    P14.1

#define RELAY_BAT_CUR_CHK   P15.0
//#define RELAY_SCAN          P0.4

#define RELAY_R		        P4.4
//#define RELAY_Y		        P4.5
#define RELAY_B		        P6.1

#define RELAY_R_ON()	        RELAY_R = 1
#define RELAY_Y_ON()	        RELAY_Y = 1
#define RELAY_B_ON()	        RELAY_B = 1

#define RELAY_R_OFF()	        RELAY_R = 0
#define RELAY_Y_OFF()	        RELAY_Y = 0
#define RELAY_B_OFF()	        RELAY_B = 0

#define RELAY_SWITCH_CHK_ON()	RELAY_SWITCH_CHK = 1	
#define RELAY_RESET_ON()	    RELAY_RESET = 1
#define RELAY_CT_ON()		    RELAY_CT = 1
#define RELAY_SHUNT_ON()	    RELAY_SHUNT = 1	
#define RELAY_NMISS_ON()	    RELAY_NMISS = 1	
#define RELAY_FORMAT_ON()	    RELAY_FORMAT = 1	
#define RELAY_POWER_ON()        RELAY_POWER = 1
#define RELAY_BAT_CUR_CHK_ON()  RELAY_BAT_CUR_CHK = 1
#define RELAY_SCAN_SWITCH_ON()  RELAY_SCAN = 1

#define RELAY_SWITCH_CHK_OFF()	RELAY_SWITCH_CHK = 0	
#define RELAY_RESET_OFF()	    RELAY_RESET = 0
#define RELAY_CT_OFF()		    RELAY_CT = 0
#define RELAY_SHUNT_OFF()	    RELAY_SHUNT = 0
#define RELAY_NMISS_OFF()	    RELAY_NMISS = 0	
#define RELAY_FORMAT_OFF()	    RELAY_FORMAT = 0	
#define RELAY_POWER_OFF()       RELAY_POWER = 0
#define RELAY_BAT_CUR_CHK_OFF() RELAY_BAT_CUR_CHK = 0
#define RELAY_SCAN_SWITCH_OFF() RELAY_SCAN = 0

#define MAX_RELAY_COUNT 6  //each relay have 2 condition connnect/disconnect
#define RELAY_IN_USE_1P 2
#define RELAY_IN_USE_3P 3


#define RELAY_CHECK_IN1 P1.0
#define RELAY_CHECK_IN2 P11.0
#define RELAY_CHECK_IN3 P11.1
#ifdef WCM_3P
    #define RELAY_CHECK_IN4 P14.0
    #define RELAY_CHECK_IN5 P4.2
    #define RELAY_CHECK_IN6 P6.2
#elif defined(LTCT_3P) || defined(HTCT_3P)
    #define RELAY_D11 P14.0
    #define RELAY_D12 P4.2
    #define RELAY_D13 P6.2
    #define RELAY_D14 P6.3
#else
    #error "No configuration defined. Define WCM_3P, LTCT_3P, or HTCT_3P."
#endif

#define RESET_CHECK_IN          P4.1
#define BLINK_CHECK_IN          SW_IN_13

#define RF_IN_ZCROSS 		P5.0
#define RF_IN_RTS 		P5.1
#define RF_IN_METER_BUSY 	P5.2
#define RF_IN_POWER_NOT 	P5.3
#define RF_IN_EVENT_NOT 	P5.4
#define RF_IN_RESET_RF 		P5.5

#define RF_OUT_NET_STAT 	P1.5
#define RF_OUT_LINK_STAT 	P1.6
#define RF_OUT_CTS 		P1.7

#define ADC_CH_AN1              _07_AD_INPUT_CHANNEL_7
#define ADC_CH_AN2              _06_AD_INPUT_CHANNEL_6
#define ADC_CH_AN5              _05_AD_INPUT_CHANNEL_5
#define ADC_CH_AN4              _04_AD_INPUT_CHANNEL_4
#define ADC_CH_AN3              _03_AD_INPUT_CHANNEL_3
#define ADC_CH_AN6              _02_AD_INPUT_CHANNEL_2
#define ADC_CH_AN7              _01_AD_INPUT_CHANNEL_1
#define ADC_CH_AN8              _00_AD_INPUT_CHANNEL_0


#define VOLTAGE_3_3             ADC_CH_AN7
#define VOLTAGE_6_5             ADC_CH_AN1      //2.4 V
#define VOLTAGE_3_0             ADC_CH_AN2      //0.89
#define VOLTAGE_3_6             ADC_CH_AN5      
#define VOLTAGE_5_0_RF          ADC_CH_AN4

#define VOLTAGE_BAT_1           ADC_CH_AN5
#define VOLTAGE_BAT_2           ADC_CH_AN6

#define VOLTAGE_RESET_CHK       ADC_CH_AN8



#define Port2TxIntEnable()	STIF2 = 1U; STMK2 = 0U  /* clear INTST2 interrupt flag */  /* Enable INTST2 interrupt */ 
#define Port2TxIntDisable()	STMK2 = 1U; STIF2 = 0U  /* disable INTST2 interrupt */ /* clear INTST2 interrupt flag */

#define Port0TxIntEnable()	STIF0 = 1U; STMK0 = 0U  /* clear INTST2 interrupt flag */  /* Enable INTST2 interrupt */ 
#define Port0TxIntDisable()	STMK0 = 1U; STIF0 = 0U  /* disable INTST2 interrupt */ /* clear INTST2 interrupt flag */

#define Port1TxIntEnable()	STIF1 = 1U; STMK1 = 0U  /* clear INTST2 interrupt flag */  /* Enable INTST2 interrupt */ 
#define Port1TxIntDisable()	STMK1 = 1U; STIF1 = 0U  /* disable INTST2 interrupt */ /* clear INTST2 interrupt flag */

#define Port3TxIntEnable()	STIF3 = 1U; STMK3 = 0U  /* clear INTST2 interrupt flag */  /* Enable INTST2 interrupt */ 
#define Port3TxIntDisable()	STMK3 = 1U; STIF3 = 0U  /* disable INTST2 interrupt */ /* clear INTST2 interrupt flag */

#define Port1RxEnable()		P0.1 = 0
#define Port1TxEnable()		P0.1 = 1

#define Port3RxEnable()		P14.2 = 0
#define Port3TxEnable()		P14.2 = 1

#define RX_TX_BUF       250
typedef struct
{
	WORD wRcvTimeOut;
	WORD wPktSndTime;
	WORD wSndPktCnt;
	WORD wRcvPktCnt;
	BYTE byRdPtr;
	BYTE byWrPtr;
	BYTE byPktRcvd;
	BYTE bySndPkt;
	BYTE bySndPtr;
	BYTE bySndLen;
	BYTE byRdBuf[RX_TX_BUF];
	BYTE byWrBuf[RX_TX_BUF];
}COMM_PROCESS;

typedef struct
{
        BYTE byState;
        BYTE byCount;
        BYTE byResult;
        WORD wTime;
}ST_RELAY_TEST;

typedef union
{
        struct
        {
                BYTE byON_3_3;
                BYTE byON_6_5;
                BYTE byOFF_3_0;
                BYTE byOFF_3_6;
                BYTE bySuperCapPower;
                BYTE bySuperCapOFF;
                BYTE byOFF_3_3;
                BYTE bySleepCurrent;
        }Item;
        BYTE byVFBuf[8];
}UN_VOLTAGE_FAIL_STEP;

typedef union
{
    struct
    {
        float fON_3_3;
        float fON_6_5;
        float fOFF_3_0;
        float fOFF_3_6;
        float fSuperCapPower;
        float fSleepCurrent1;
        float fSleepCurrent2;
    }Item;
    BYTE byVFBuf[28];
}UN_BOARD_TEST_VALUES;


typedef union
{
        struct
        {
                float fVRMS_R;
                float fVRMS_Y;
                float fVRMS_B;
                float fIRMS_R;
                float fIRMS_Y;
                float fIRMS_B;
                float fIRMS_N;
        }st3PElements;
        struct
        {
                float fVRMS;
                float fIRMS;
                float fIRMS_N;
        }st1PElements;
        BYTE byVFBuf[32];
}UN_VI_VALUES;

// #define SERIAL_ID_PKT	        0
// #define VERSION_PKT		        1
// #define DATE_TIME_PKT	        2
// #define CURRENT_PKT		        3 
// #define BOARD_ID_PKT	        5 
// #define SWITCH_CONNECT_CHK_PKT	6
// #define SWITCH_RELEASE_CHK_PKT	7
// #define SINGLE_WIRE_OFFSET_PKT	8
// #define SWITCH_STATUS_PKT	    9

// #define SET_SERIAL_ID_PKT			0x10
// #define SET_DATE_TIME_PKT			0x12 
// #define SET_BOARD_ID_PKT			0x15
// #define SET_SINGLE_WIRE_OFFSET_PKT	0x18

// #define VOLTAGE_FAIL_PKT        0x20

// #define FINAL_PKT               0x30
// #define START_TEST              0x31

// #define GET_VERSION_PKT         0x41
// #define GET_DATE_TIME_PKT       0x42 
// #define GET_BOARD_ID_PKT        0x45

// #define GET_INT_FIRM_VERSION	0x50

// #define BOARD_VOLTAGES_DATA     0x99
// #define RESET_JIG               0xA0


// to be added afterwards
typedef enum
{
        SERIAL_ID_PKT = 0,
        VERSION_PKT = 1,
        DATE_TIME_PKT = 2,
        CURRENT_PKT = 3,
        BOARD_ID_PKT = 5,
        SWITCH_CONNECT_CHK_PKT = 6,
        SWITCH_RELEASE_CHK_PKT = 7,
        SINGLE_WIRE_OFFSET_PKT = 8,
        SWITCH_STATUS_PKT = 9,

        SET_SERIAL_ID_PKT = 0x10,
        SET_DATE_TIME_PKT = 0x12,
        SET_BOARD_ID_PKT = 0x15,
        SET_SINGLE_WIRE_OFFSET_PKT = 0x18,

        VOLTAGE_FAIL_PKT = 0x20,

        FINAL_PKT = 0x30,
        START_TEST = 0x31,

        GET_VERSION_PKT = 0x41,
        GET_DATE_TIME_PKT = 0x42,
        GET_BOARD_ID_PKT = 0x45,

        GET_INT_FIRM_VERSION = 0x50,

        BOARD_VOLTAGES_DATA = 0x99,
        RESET_JIG = 0xA0,

        GET_JIG_FIRM_VER = 0xA1,
        ENABLE_DEBUG = 0xA2,
}e_userPacketsInfo;



typedef enum
{
        OPTICAL_PORT,
        COMPUTER_PORT,
        RF_PORT, 
        FREQUENCY_COUNTER_PORT = 3, 
}e_commPorts;

/*---------Extern Global Variables-----------*/
extern unsigned int g_swStatus;

/*--------Functions----------------------------*/
float getAdcValue(BYTE byChannel);
float getAdcValue2(BYTE byChannel);
void delay_ms(WORD wDelay);
BYTE checkBlinking(void);
BYTE checkCommandResp(BYTE byPort, WORD wRespWaitTime);
void SetLED(BYTE byNumber, BYTE byStatus);
BYTE checkRelayStatus(BYTE byResulCompare);
BYTE checkRFPins(BYTE byResulCompare);
WORD Crc_fromBuffer(BYTE *buf, BYTE byLength);
BYTE getResposnseFromMeter(BYTE byPort, WORD wRespWaitTime,BYTE byItem, BYTE *byRspBuf);
BYTE checkPCComm(void);
void sendPCData(BYTE bySPCDTag);
void loadAuthCommand(void);
void hdwinit(void);

void getSwitchStatus(void);


/* End user code. Do not edit comment generated here */
#endif
