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

void sw_9_operation(void)
{ 
     if (g_enableDebug)
    {

        pc_comm_sendDataToPC("$Starting SW9 Operation#\n");
    }

    if (0 == SW_IN_1)
    {
        while (0 == SW_IN_1)
            ;
        byCmdPtr = &byCmdDfFrmt[0];
        if (FALSE == checkCommandResp(0, 150))
        {
            byTestResult = 1;
        }
    }
     if (g_enableDebug)
    {
        pc_comm_sendDataToPC("$SW9 Operation Completed#\n");
    }
}