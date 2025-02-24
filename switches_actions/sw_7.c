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

float fADCReading;

void sw_7_operationBatteryVoltage(void)
{
     if (g_enableDebug)
    {

        pc_comm_sendDataToPC("$Starting SW7 Operation#\n");
    }
    // Battery Voltages
    for (byTestResult = 0; byTestResult < 8; byTestResult++)
    {
        unVFS.byVFBuf[byTestResult] = 0;
    }

    byTestResult = 0;
    byTestByte = 0;
    do
    {
        delay_ms(200);
        fADCReading = getAdcValue(VOLTAGE_3_0);
        unBoardValues.Item.fOFF_3_0 = fADCReading;
        if (fADCReading >= RTC_BAT_VTG_THRESHOLD)
        {
            break;
        }
        byTestByte++;
    } while (byTestByte < 5);
    if (byTestByte >= 5)
    {
        byTestResult = 1;
        unVFS.Item.byOFF_3_0 = 1;
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // fADCReading = getAdcValue(VOLTAGE_3_6);
    // unBoardValues.Item.fOFF_3_6 = fADCReading;
    // if (fADCReading < 2.80)
    //{
    //     byTestResult = 1;
    //     unVFS.Item.byOFF_3_6 = 1;
    // }
    byTestByte = 0;
    do
    {
        fADCReading = getAdcValue(VOLTAGE_3_6);
        unBoardValues.Item.fOFF_3_6 = fADCReading;
        if (fADCReading >= MAIN_BAT_VTG_THRESHOLD)
        {
            break;
        }
        delay_ms(200);
        byTestByte++;
    } while (byTestByte < 5);
    if (byTestByte >= 5)
    {
        byTestResult = 1;
        unVFS.Item.byOFF_3_6 = 1;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Battery Current
    delay_ms(25000);
    RELAY_BAT_CUR_CHK_OFF();
    delay_ms(500);
    byTestByte = 0;
    do
    {
        fADCReading = getAdcValue(VOLTAGE_BAT_1);
        unBoardValues.Item.fSleepCurrent1 = fADCReading;
        fADCReading = getAdcValue(VOLTAGE_BAT_2);
        unBoardValues.Item.fSleepCurrent2 = fADCReading;
        if (((unBoardValues.Item.fSleepCurrent2 - unBoardValues.Item.fSleepCurrent1) < MAIN_BAT_SLEEP_CURRENT_MAX) && ((unBoardValues.Item.fSleepCurrent2 - unBoardValues.Item.fSleepCurrent1) > 0))
        {
            break;
        }
        delay_ms(1000);
        byTestByte++;
    } while (byTestByte < 10);
    if (byTestByte >= 10)
    {
        byTestResult = 1;
        unVFS.Item.bySleepCurrent = 1;
    }

    RELAY_BAT_CUR_CHK_ON();

    RELAY_POWER_ON();
    delay_ms(1000);
    byTestResult = 0;
    fADCReading = getAdcValue(VOLTAGE_3_3);
    unBoardValues.Item.fON_3_3 = fADCReading;
    if (fADCReading < DVCC_THRESHOLD)
    {
        byTestResult = 1;
        unVFS.Item.byON_3_3 = 1;
    }

    fADCReading = getAdcValue(VOLTAGE_6_5);
    unBoardValues.Item.fON_6_5 = fADCReading;
    if (fADCReading < THRESHOLD_6_5)
    {
        byTestResult = 1;
        unVFS.Item.byON_6_5 = 1;
    }

    ////////////////////////////////////////////////////////

    // byLedDisplayResult[10] = byTestResult;
     if (g_enableDebug)
    {
        pc_comm_sendDataToPC("$SW7 Operation Completed#\n");
    }
}

void sw_7_operationSuperCapacitor(void)
{
    // This test is done after turning OFF the meter
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////Set Offset here
    byTestResult = 0;

     if (g_enableDebug)
    {

        pc_comm_sendDataToPC("$Starting SW1 Supercap Operation#\n");
    }
    RELAY_POWER_OFF();
    ////////////////////////////////////////////////////////
    // SuperCap Testing
    delay_ms(2000);
    byTestByte = 0;
    do
    {
        fADCReading = getAdcValue(VOLTAGE_5_0_RF);
        unBoardValues.Item.fSuperCapPower = fADCReading;
        if (fADCReading > SUPERCAP_PWR_THRESHOLD)
        {
            break;
        }
        byTestByte++;
    } while (byTestByte < 2);
    if (byTestByte >= 2)
    {
        byTestResult = 1;
        unVFS.Item.bySuperCapPower = 1;
    }

    if ((unVFS.Item.byON_3_3) || (unVFS.Item.byON_6_5) || (unVFS.Item.byOFF_3_0) || (unVFS.Item.byOFF_3_6) || (unVFS.Item.bySleepCurrent))
    {
        byTestResult = 1;
    }

    sendPCData(VOLTAGE_FAIL_PKT);
    RELAY_POWER_ON();
    // if (0 != SW_IN_10_PC)
    {
        delay_ms(1000);
        sendPCData(BOARD_VOLTAGES_DATA);
        delay_ms(1000);
    }
    byLedDisplayResult[10] = byTestResult;

      if (g_enableDebug)
    {
        pc_comm_sendDataToPC("$SW7 Supercap Operation Completed#\n");
    }

}
