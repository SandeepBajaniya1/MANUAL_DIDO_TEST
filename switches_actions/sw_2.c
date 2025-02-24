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

void sw_2_operation(void)
{
    byTestResult = 0;

     if (g_enableDebug)
    {

        pc_comm_sendDataToPC("$Starting SW2 Operation#\n");
    }
    RELAY_POWER_ON();
    delay_ms(1000);
    RELAY_RESET_ON();
    byTestResult = 0;
    wTimeout = 0;
    while (1)
    {
        // if (0 != by10MsTick2)
        {
            wTimeout++;
            // by10MsTick2 = 0;
        }
        if (wTimeout > 4500)
        {
            break;
        }

        fADCReading = getAdcValue2(VOLTAGE_RESET_CHK); // Takes Approx 63ms time
        // if ((0 == RESET_CHECK_IN) &&  (0 == byTestResult))
        if ((fADCReading < 1.2) && (0 == byTestResult))
        {
            byTestResult = 1;
        }

        // if ((1 == RESET_CHECK_IN) &&  (1 == byTestResult))
        if ((fADCReading > 1.6) && (1 == byTestResult))
        {
            byTestResult = 2;
            break;
        }
    }
    if (2 == byTestResult)
    {
        byLedDisplayResult[4] = 0;
    }
    else
    {
        byLedDisplayResult[4] = 1;
    }
    RELAY_RESET_OFF();
    if (g_enableDebug)
    {
        pc_comm_sendDataToPC("$SW2 Operation Completed#\n");
    }
}