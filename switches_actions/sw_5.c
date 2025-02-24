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

void sw_5_operation(void)
{
   uint8_t rf_rtry_counter =3;
    if (g_enableDebug)
    {

        pc_comm_sendDataToPC("$Starting SW5 Operation#\n");
    }
   do
   {
    rf_rtry_counter--;
    RELAY_POWER_ON();
    // delay_ms(10000);

    byTestResult = 0;
    byCmdPtr = &byCmdRFIOLow[0];
    RF_OUT_NET_STAT = 0;
    RF_OUT_LINK_STAT = 1; // inverted logic
    RF_OUT_CTS = 0;
    byTestByte = 0;
    if (FALSE == checkCommandResp(2, 100))
    {
        byTestResult = 1;
    }
    // else if (/*(1 != RF_IN_1) ||*/ (1 != RF_IN_2) || (1 != RF_IN_3) || (1 != RF_IN_4) || (1 != RF_IN_5) || (1 != RF_IN_6))
    else if (FALSE == checkRFPins(1))
    {
        byTestResult = 1;
        byTestByte = 3;
    }

    delay_ms(2000);
    byCmdPtr = &byCmdRFIOHigh[0];
    RF_OUT_NET_STAT = 1;
    RF_OUT_LINK_STAT = 0;
    RF_OUT_CTS = 1;
    if (FALSE == checkCommandResp(2, 100))
    {
        byTestResult = 1;
    }
    // else if (/*(0 != RF_IN_1) || */(0 != RF_IN_2) || (0 != RF_IN_3) || (0 != RF_IN_4) || (0 != RF_IN_5) || (0 != RF_IN_6))
    else if (FALSE == checkRFPins(0))
    {
        byTestResult = 1;
        byTestByte = 3;
    }

    delay_ms(2000);
    byCmdPtr = &byCmdRFIOLow[0];
    RF_OUT_NET_STAT = 0;
    RF_OUT_LINK_STAT = 1;
    RF_OUT_CTS = 0;
    if (FALSE == checkCommandResp(2, 100))
    {
        byTestResult = 1;
    }
    // else if (/*(1 != RF_IN_1) || */(1 != RF_IN_2) || (1 != RF_IN_3) || (1 != RF_IN_4) || (1 != RF_IN_5) || (1 != RF_IN_6))
    else if (FALSE == checkRFPins(1))
    {
        byTestResult = 1;
        byTestByte = 3;
    }
    if (0== byTestResult)
    {break;}
   }while(0 != rf_rtry_counter);
   
    byLedDisplayResult[8] = byTestResult;

    RF_OUT_LINK_STAT = 0;
    // if (byTestByte == 1)
    //{
    //         byLedDisplayResult[13] = 0;
    // }
    // else if (byTestByte == 2)
    //{
    //         byLedDisplayResult[13] = 1;
    // }
    // else if (byTestByte == 3)
    //{
    //         byLedDisplayResult[13] = 2;
    // }
     if (g_enableDebug)
    {
        pc_comm_sendDataToPC("$SW5 Operation Completed#\n");
    }
}


uint8_t checkRFPins(BYTE byResulCompare)
{
    uint8_t byCRSIndex, byCRSIndex2, byRelayInUse;
    delay_ms(10);
    if (0 != SW_IN_11_1PH_3PH)
    {
        if (0 == byResulCompare)
        {
            if ((0 != RF_IN_RTS) /* || (0 != RF_IN_METER_BUSY) || (0 != RF_IN_POWER_NOT)*/ || (0 != RF_IN_EVENT_NOT) || (0 != RF_IN_RESET_RF))
            {
                return FALSE;
            }
        }
        else
        {
            if ((1 != RF_IN_RTS) /* || (1 != RF_IN_METER_BUSY) || (1 != RF_IN_POWER_NOT)*/ || (1 != RF_IN_EVENT_NOT) || (1 != RF_IN_RESET_RF))
            {
                return FALSE;
            }
        }
    }
    else
    {
        if (0 == byResulCompare)
        {
            if ((0 != RF_IN_RTS) || (0 != RF_IN_EVENT_NOT) || (0 != RF_IN_RESET_RF))
            {
                return FALSE;
            }
        }
        else
        {
            if ((1 != RF_IN_RTS) || (1 != RF_IN_EVENT_NOT) || (1 != RF_IN_RESET_RF))
            {
                return FALSE;
            }
        }
    }
    return TRUE;
}
