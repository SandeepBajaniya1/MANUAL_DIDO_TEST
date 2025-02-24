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

BYTE *pbyTmpPtr;
UN_VI_VALUES unVIVal;

void sw_4_operation(void)
{
    uint8_t byMainIndex;
    byTestResult = 0;

 if (g_enableDebug)
    {

        pc_comm_sendDataToPC("$Starting SW4 Operation#\n");
    }
    RELAY_POWER_ON();
    RELAY_SHUNT_ON();
    RELAY_CT_ON();
    delay_ms(3000);
    // if (FALSE == checkBlinking())	//Check reading
    //{
    //         byTestResult = 1;
    // }
    byTestResult = 0;
    getResposnseFromMeter(0, 150, CURRENT_PKT, byRespBuf);
    // if (0 == SW_IN_10_PC)
    {
        sendPCData(CURRENT_PKT);
    }

    pbyTmpPtr = (uint8_t *)&unVIVal.byVFBuf[0];
    for (byMainIndex = 0; byMainIndex < 12; byMainIndex++)
    {
        pbyTmpPtr[byMainIndex] = byRespBuf[6 + byMainIndex];
    }
    if (unVIVal.st1PElements.fVRMS < 150)
    {
        byTestResult = 1;
    }
    if (unVIVal.st1PElements.fIRMS < 1.0)
    {
        byTestResult = 1;
    }
    if (unVIVal.st1PElements.fIRMS_N < 1.0)
    {
        byTestResult = 1;
    }
    byLedDisplayResult[6] = byTestResult;

    if (FALSE == checkBlinking())
    {
        byLedDisplayResult[7] = 1;
    }
    else
    {
        byLedDisplayResult[7] = 0;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////Check LED blinking here

    RELAY_SHUNT_OFF();
    RELAY_CT_OFF();
     if (g_enableDebug)
    {
        pc_comm_sendDataToPC("$SW4 Operation Completed#\n");
    }
}

uint8_t checkBlinking(void)
{
    uint8_t u8LEDBlinkCount = 0;
    uint8_t u8LEDStateCheck = 0;
    wTimeout = 0;
    while (1)
    {
        if (0 != by10MsTick)
        {
            wTimeout++;
            by10MsTick = 0;
        }
        if (wTimeout > 10000)
        {
            return FALSE;
        }
        if (LDR_ACTIVE != u8LEDStateCheck)
        {
            u8LEDStateCheck = LDR_ACTIVE;
            u8LEDBlinkCount++;
            if (u8LEDBlinkCount > 4)
            {
                return TRUE;
            }
        }
    }
}