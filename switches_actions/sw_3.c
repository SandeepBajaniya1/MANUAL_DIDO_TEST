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

void sw_3_operation(void)
{
    byTestResult = 0;

     if (g_enableDebug)
    {

        pc_comm_sendDataToPC("$Starting SW3 Operation#\n");
    }
    RELAY_POWER_ON();
    delay_ms(5000);

    stRlyChk[0].byCount = 0;
    stRlyChk[1].byCount = 0;
    stRlyChk[2].byCount = 0;
    byCmdPtr = &byCmdConnect[0];
    if (FALSE == checkCommandResp(0, 50))
    {
        byTestResult = 1;
    }
    // else if ((0 != RELAY_CHECK_IN1) || (0 != RELAY_CHECK_IN2))          //Pulses check
    else if (FALSE == checkRelayStatus(1))
    {
        byTestResult = 1;
    }

    delay_ms(2000);
    stRlyChk[0].byCount = 0;
    stRlyChk[1].byCount = 0;
    stRlyChk[2].byCount = 0;
    byCmdPtr = &byCmdDisConnect[0];
    if (FALSE == checkCommandResp(0, 50))
    {
        byTestResult = 1;
    }
    // else if ((1 != RELAY_CHECK_IN1) || (1 != RELAY_CHECK_IN2))          //Pulses check
    else if (FALSE == checkRelayStatus(0))
    {
        byTestResult = 1;
    }

    delay_ms(2000);
    stRlyChk[0].byCount = 0;
    stRlyChk[1].byCount = 0;
    stRlyChk[2].byCount = 0;
    byCmdPtr = &byCmdConnect[0];
    if (FALSE == checkCommandResp(0, 50))
    {
        byTestResult = 1;
    }
    // else if ((0 != RELAY_CHECK_IN1) || (0 != RELAY_CHECK_IN2))          //Pulses check
    else if (FALSE == checkRelayStatus(1))
    {
        byTestResult = 1;
    }
    byLedDisplayResult[5] = byTestResult;


 if (g_enableDebug)
    {
        pc_comm_sendDataToPC("$SW3 Operation Completed#\n");
    }
    
}

void sw_3_finalTask(void)
{
    checkRelayStatus(1);
    for (byTestResult = 0; byTestResult < MAX_LED; byTestResult++)
    {
        if (1 == byLedDisplayResult[byTestResult])
        {
            break;
        }
    }
    byLedDisplayResult[12] = 1;
    if (byTestResult == MAX_LED)
    {
        byLedDisplayResult[12] = 0;
    }

    byCmdPtr = &byCmdConnect[0];
    checkCommandResp(0, 50);
}

uint8_t checkRelayStatus(BYTE byResulCompare)
{
    uint8_t byCRSIndex, byCRSIndex2, byRelayInUse;
    wTimeout = 0;
    if (0 != SW_IN_11_1PH_3PH)
    {
        byRelayInUse = RELAY_IN_USE_1P;
    }
    else
    {
        byRelayInUse = RELAY_IN_USE_3P;
    }
    if (0 == SW_IN_3_RELAY)
    {
        for (byCRSIndex = 0; byCRSIndex < byRelayInUse; byCRSIndex++)
        {
            stRlyChk[byCRSIndex].wTime = 0;
            stRlyChk[byCRSIndex].byCount = 0;
        }
        while (1)
        {
            delay_ms(2);
            wTimeout++;
            for (byCRSIndex = 0; byCRSIndex < byRelayInUse; byCRSIndex++)
            {
                stRlyChk[byCRSIndex].wTime++;
                switch (byCRSIndex)
                {
                case 0:
                    if (stRlyChk[byCRSIndex].byState != RELAY_CHECK_IN1)
                    {
                        stRlyChk[byCRSIndex].byState = RELAY_CHECK_IN1;
                        if (stRlyChk[byCRSIndex].byCount < 200)
                        {
                            stRlyChk[byCRSIndex].byCount++;
                        }

                        stRlyChk[byCRSIndex].wTime = 0;
                    }
                    break;
                case 1:
                    if (stRlyChk[byCRSIndex].byState != RELAY_CHECK_IN2)
                    {
                        stRlyChk[byCRSIndex].byState = RELAY_CHECK_IN2;
                        if (stRlyChk[byCRSIndex].byCount < 200)
                        {
                            stRlyChk[byCRSIndex].byCount++;
                        }

                        stRlyChk[byCRSIndex].wTime = 0;
                    }
                    break;
                case 2:
                    if (stRlyChk[byCRSIndex].byState != RELAY_CHECK_IN3)
                    {
                        stRlyChk[byCRSIndex].byState = RELAY_CHECK_IN3;
                        if (stRlyChk[byCRSIndex].byCount < 200)
                        {
                            stRlyChk[byCRSIndex].byCount++;
                        }
                        stRlyChk[byCRSIndex].wTime = 0;
                    }
                    break;
                default:
                    break;
                }
                if (stRlyChk[byCRSIndex].wTime > 200)
                {
                    stRlyChk[byCRSIndex].byCount = 0;
                }
            }
            if (wTimeout > 2000)
            {
                break;
            }
        }
        if (0 == byResulCompare)
        {
            if ((stRlyChk[0].wTime < 200) || (stRlyChk[1].wTime < 200))
            {
                return FALSE;
            }
        }
        else
        {
            if ((stRlyChk[0].byCount < 50) || (stRlyChk[1].byCount < 50))
            {
                return FALSE;
            }
        }
    }
    else
    {
        while (1)
        {
            wTimeout++;
            if (0 != byResulCompare)
            {
                if (stRlyChk[0].byCount >= 10)
                {
                    break;
                }
            }
            else
            {
                if (stRlyChk[1].byCount >= 10)
                {
                    break;
                }
            }
            delay_ms(1);
            if (wTimeout > 5000)
            {
                break;
            }
        }
        if (wTimeout > 5000)
        {
            return FALSE;
        }
    }
    return TRUE;
}
