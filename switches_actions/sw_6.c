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

BYTE bySwitchStatus = 0;

void sw_6_operation(void)
{
    uint8_t byMainIndex;

     if (g_enableDebug)
    {

        pc_comm_sendDataToPC("$Starting SW6 Operation#\n");
    }
    RELAY_POWER_ON();
    // delay_ms(10000);

    byTestResult = 0;
    bySwitchStatus = 0;
    RELAY_SWITCH_CHK_OFF();

    byMainIndex = 0;
    while (byMainIndex < 5)
    {
        delay_ms(200);
        if (TRUE == getResposnseFromMeter(0, 150, SWITCH_RELEASE_CHK_PKT, byRespBuf))
        {
            if (0 != SW_IN_8) // Only switch check, else Switch + magnet
            {
                byRespBuf[6] &= 0x07;
            }
            if (0 == byRespBuf[6])
            {
                break;
            }
        }
        else
        {
            byTestResult = 1;
            break;
        }
        byMainIndex++;
    }
    bySwitchStatus |= byRespBuf[6];

    RELAY_SWITCH_CHK_ON();

    byMainIndex = 0;
    while (byMainIndex < 5)
    {
        delay_ms(200);
        if (TRUE == getResposnseFromMeter(0, 150, SWITCH_CONNECT_CHK_PKT, byRespBuf))
        {
            if (0 != SW_IN_8) // Only switch check, else Switch + magnet
            {
                byRespBuf[6] &= 0x07;
		        //  byRespBuf[6] &= 0x06; // WITOUT COVEROPEN BUT PRESS+BOTH BUTTON PRESS
		
            }
            if (0 == byRespBuf[6])
            {
                break;
            }
        }
        else
        {
            byTestResult = 1;
            break;
        }
        byMainIndex++;
    }
    bySwitchStatus |= byRespBuf[6];

    RELAY_SWITCH_CHK_OFF();

    if (0 != bySwitchStatus)
    {
        byTestResult = 1;
    }
    byLedDisplayResult[9] = byTestResult;

    if (0 == SW_IN_10_PC)
    {
        byRespBuf[6] = bySwitchStatus;
        sendPCData(SWITCH_CONNECT_CHK_PKT);
    }
     if (g_enableDebug)
    {
        pc_comm_sendDataToPC("$SW6 Operation Completed#\n");
    }
}