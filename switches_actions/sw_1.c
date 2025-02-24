#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_serial.h"
#include "r_cg_adc.h"
#include "r_cg_it.h"
#include "r_cg_userdefine.h"
/* Start user code for include. Do not edit comment generated here */
#include "r_main.h"
#include "I2C.h"
#include "string.h"
#include "aes.h"
#include "rtc_prod.h"
#include "meter_comm.h"
#include "pc_comm.h"
#include "adc_operations.h"
#include "leds_operations.h"
/* End user code. Do not edit comment generated here */
#include <stdio.h>
#include "error_codes.h"
#include "jig_operations.h"
#include "SerialData.h"
#include "switches.h"

void sw_1_operation(void)
{
    if (g_enableDebug)
    {

        pc_comm_sendDataToPC("$Starting SW1 Operation#\n");
    }

    RELAY_POWER_ON();
    delay_ms(7000); // To be verified for fresh meter

    delay_ms(1000);
    byTestResult = 0;
    byCmdPtr = &byCmdI2CChk[0];
    if (FALSE == checkCommandResp(0, 150))
    {
        byTestResult = 1;
    }

    byLedDisplayResult[1] = byTestResult;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////Set RTC Here
    if (0 == SW_IN_10_PC)
    {
        byTestResult = 0;
        sendPCData(GET_DATE_TIME_PKT);
        wTimeout = 0;
        while (SET_DATE_TIME_PKT != checkPCComm())
        {
            // Some timeout
            delay_ms(100);
            if (wTimeout++ > 100)
            {
                byTestResult = 1;
                break;
            }
        }
        // if not timeout
        byLedDisplayResult[2] = byTestResult;
    }
    else {
        without_pc_rtcCalibration();
    }

    /*Task 2*/
    byTestResult = 0;
    // RELAY_FORMAT_ON();
    delay_ms(100);
    byCmdPtr = &byCmdDfFrmt[0];
    if (FALSE == checkCommandResp(0, 150))
    {
        byTestResult = 1;
    }
    delay_ms(60000); // To be verified
    if (0 == SW_IN_11_1PH_3PH)
    {
        delay_ms(15000); // Double time for 3 phase
    }
    // RELAY_FORMAT_OFF();

    /*Task 3*/
    byLedDisplayResult[3] = byTestResult;
    //////////////////////////////////////////////////////////////////////////////////////////////////////////// Get Board serial number here
    // Send to PC.. 35 sec
    // if (0 == SW_IN_10_PC)
    //{
    //     sendPCData(GET_BOARD_ID_PKT);
    //     wTimeout = 0;
    //     while (SET_BOARD_ID_PKT != checkPCComm())
    //     {
    //         delay_ms(100);
    //         if (wTimeout++ > 100)
    //         {
    //             //byTestResult = 1;
    //             break;
    //         }
    //     }
    // }
    // getResposnseFromMeter(0, 30, SET_SINGLE_WIRE_OFFSET_PKT, byRespBuf);

    if (g_enableDebug)
    {
        pc_comm_sendDataToPC("$SW1 Operation Completed#\n");
    }
}