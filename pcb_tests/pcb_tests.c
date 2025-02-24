#include "pcb_tests.h"

/**
 * This function returns the Main and RTC battery voltages without meter ON.
 */
e_jigErrorCodes pcb_tests_getBatVoltage(char *data, uint8_t *dataLen)
{
    uint8_t status = 0;
    float fADCReading_rtc_bat = 0.0;
    float fADCReading_main_bat = 0.0;
    RELAY_POWER_OFF();
    if (0 != SW_IN_7)
    {
        return REQ_SW_NOT_ON;
    }

    // RTC battery voltage reading
    delay_ms(200);
    fADCReading_rtc_bat = getAdcValue(VOLTAGE_3_0);
    if (fADCReading_rtc_bat < RTC_BAT_VTG_THRESHOLD)
    {
        status = 1;
    }

    // Main battery voltage reading
    fADCReading_main_bat = getAdcValue(VOLTAGE_3_6);
    if (fADCReading_main_bat < MAIN_BAT_VTG_THRESHOLD)
    {
        status = 1;
    }

    memcpy(&data[*dataLen], &fADCReading_rtc_bat, sizeof(fADCReading_rtc_bat));
    *dataLen += sizeof(fADCReading_rtc_bat);

    memcpy(&data[*dataLen], &fADCReading_main_bat, sizeof(fADCReading_main_bat));
    *dataLen += sizeof(fADCReading_main_bat);

    if (status)
    {
        leds_operations_updateLedStatus(BOARD_VTG_STATUS, TEST_FAIL);
        g_pcb_bat_status = 1;
    }
    else
    {
        if ((g_pcb_mainDcVtg_status == 1) || (g_pcb_sleepCurrent_status == 1) || (g_pcb_superCapVtg_status == 1))
        {
            leds_operations_updateLedStatus(BOARD_VTG_STATUS, TEST_FAIL);
        }
        else
        {
            g_pcb_bat_status = 0;
            leds_operations_updateLedStatus(BOARD_VTG_STATUS, TEST_PASS);
        }
    }

    return JIG_COMMNAD_OK;
}

/*
 *The "pcb_tests_getSleepcurrent" function is used for get diff voltage of main battery with load without meter ON.
 */
e_jigErrorCodes pcb_tests_getSleepcurrent_SPH(char *data, uint8_t *dataLen)
{
    uint8_t status = 0;
    float fADCReading_main_bat = 0.0;
    float fADCReading_main_bat_withLoad = 0.0;
    float fDiff_voltage = 0;
    uint8_t retry = 0;
    if (0 != SW_IN_7)
    {
        return REQ_SW_NOT_ON;
    }
    RELAY_POWER_OFF();
    // Battery Current
    delay_ms(20000); // for LTCT 20 sec
    RELAY_BAT_CUR_CHK_OFF();
    delay_ms(500);

    do
    {
        fADCReading_main_bat = getAdcValue(VOLTAGE_BAT_1);
        fADCReading_main_bat_withLoad = getAdcValue(VOLTAGE_BAT_2);
        // minimium_Bat_sleep_current = -0.1 used only for 3P otherwise 0 used
        if (((fADCReading_main_bat_withLoad - fADCReading_main_bat) < MAIN_BAT_SLEEP_CURRENT_MAX) && ((fADCReading_main_bat_withLoad - fADCReading_main_bat) > 0))
        {
            break;
        }
        delay_ms(1000);
        retry++;
    } while (retry < 5);
    if (retry >= 5)
    {
        status = 1;
    }
    RELAY_BAT_CUR_CHK_ON();

    fDiff_voltage = (fADCReading_main_bat_withLoad - fADCReading_main_bat);
    memcpy(&data[*dataLen], &fADCReading_main_bat, sizeof(fADCReading_main_bat));
    *dataLen += sizeof(fADCReading_main_bat);
    memcpy(&data[*dataLen], &fADCReading_main_bat_withLoad, sizeof(fADCReading_main_bat_withLoad));
    *dataLen += sizeof(fADCReading_main_bat_withLoad);

    if (status)
    {
        leds_operations_updateLedStatus(BOARD_VTG_STATUS, TEST_FAIL);
        g_pcb_sleepCurrent_status = 1;
    }
    else
    {
        if ((g_pcb_bat_status == 1) || (g_pcb_mainDcVtg_status == 1) || (g_pcb_superCapVtg_status == 1))
        {
            leds_operations_updateLedStatus(BOARD_VTG_STATUS, TEST_FAIL);
        }
        else
        {
            g_pcb_sleepCurrent_status = 0;
            leds_operations_updateLedStatus(BOARD_VTG_STATUS, TEST_PASS);
        }
    }
    return JIG_COMMNAD_OK;
}

e_jigErrorCodes pcb_tests_getSleepcurrent_LTCT_3p(char *data, uint8_t *dataLen)
{
    uint8_t status = 0;
    float fADCReading_main_bat = 0.0;
    float fADCReading_main_bat_withLoad = 0.0;
    float fDiff_voltage = 0;
    uint8_t retry = 0;
    if (0 != SW_IN_7)
    {
        return REQ_SW_NOT_ON;
    }
    RELAY_POWER_OFF();
    // Battery Current
    delay_ms(20000); // for LTCT 20 sec
    RELAY_BAT_CUR_CHK_OFF();
    delay_ms(500);

    do
    {
        fADCReading_main_bat = getAdcValue(VOLTAGE_BAT_1);
        fADCReading_main_bat_withLoad = getAdcValue(VOLTAGE_BAT_2);
        // minimium_Bat_sleep_current = -0.1 used only for 3P otherwise 0 used
        if (((fADCReading_main_bat_withLoad - fADCReading_main_bat) < MAIN_BAT_SLEEP_CURRENT_MAX) && ((fADCReading_main_bat_withLoad - fADCReading_main_bat) > -0.1))
        {
            break;
        }
        delay_ms(1000);
        retry++;
    } while (retry < 5);
    if (retry >= 5)
    {
        status = 1;
    }
    RELAY_BAT_CUR_CHK_ON();

    fDiff_voltage = (fADCReading_main_bat_withLoad - fADCReading_main_bat);
    memcpy(&data[*dataLen], &fADCReading_main_bat, sizeof(fADCReading_main_bat));
    *dataLen += sizeof(fADCReading_main_bat);
    memcpy(&data[*dataLen], &fADCReading_main_bat_withLoad, sizeof(fADCReading_main_bat_withLoad));
    *dataLen += sizeof(fADCReading_main_bat_withLoad);

    if (status)
    {
        leds_operations_updateLedStatus(BOARD_VTG_STATUS, TEST_FAIL);
        g_pcb_sleepCurrent_status = 1;
    }
    else
    {
        if ((g_pcb_bat_status == 1) || (g_pcb_mainDcVtg_status == 1) || (g_pcb_superCapVtg_status == 1))
        {
            leds_operations_updateLedStatus(BOARD_VTG_STATUS, TEST_FAIL);
        }
        else
        {
            g_pcb_sleepCurrent_status = 0;
            leds_operations_updateLedStatus(BOARD_VTG_STATUS, TEST_PASS);
        }
    }
    return JIG_COMMNAD_OK;
}

/*
 *This function is used for to get DVCC(3.3v) board voltage and 6.5v after meter ON.
 */
e_jigErrorCodes pcb_tests_mainsDcVtg(char *data, uint8_t *dataLen)
{
    float fADCReading_DVCC = 0.0;
    float fADCReading_6_5V = 0.0;
    uint8_t status = 0;
    if (0 != SW_IN_7)
    {
        return REQ_SW_NOT_ON;
    }
    RELAY_POWER_ON();
    delay_ms(1000);

    fADCReading_DVCC = getAdcValue(VOLTAGE_3_3);
    if (fADCReading_DVCC < DVCC_THRESHOLD)
    {
        status = 1;
    }

    fADCReading_6_5V = getAdcValue(VOLTAGE_6_5);
    if (fADCReading_6_5V < THRESHOLD_6_5)
    {
        status = 1;
    }

    memcpy(&data[*dataLen], &fADCReading_DVCC, sizeof(fADCReading_DVCC));
    *dataLen += sizeof(fADCReading_DVCC);
    memcpy(&data[*dataLen], &fADCReading_6_5V, sizeof(fADCReading_6_5V));
    *dataLen += sizeof(fADCReading_6_5V);

    if (status)
    {
        leds_operations_updateLedStatus(BOARD_VTG_STATUS, TEST_FAIL);
        g_pcb_mainDcVtg_status = 1;
    }
    else
    {
        if ((g_pcb_bat_status == 1) || (g_pcb_sleepCurrent_status == 1) || (g_pcb_superCapVtg_status == 1))
        {
            leds_operations_updateLedStatus(BOARD_VTG_STATUS, TEST_FAIL);
        }
        else
        {
            g_pcb_mainDcVtg_status = 0;
            leds_operations_updateLedStatus(BOARD_VTG_STATUS, TEST_PASS);
        }
    }
    return JIG_COMMNAD_OK;
}
/*
 *This function is used for to get supercap volage after all tests.
 */
e_jigErrorCodes pcb_tests_superCapVtg(char *data, uint8_t *dataLen)
{
    uint8_t testStatus = 0;
    float fADCReading_supercap = 0.0;
    if (0 != SW_IN_7)
    {
        return REQ_SW_NOT_ON;
    }
    RELAY_POWER_OFF();

    delay_ms(2000);

    fADCReading_supercap = getAdcValue(VOLTAGE_5_0_RF);
    if (fADCReading_supercap <= SUPERCAP_PWR_THRESHOLD)
    {
        testStatus = 1;
    }
    RELAY_POWER_ON();

    memcpy(&data[*dataLen], &fADCReading_supercap, sizeof(fADCReading_supercap));
    *dataLen += sizeof(fADCReading_supercap);

    if (testStatus)
    {
        leds_operations_updateLedStatus(BOARD_VTG_STATUS, TEST_FAIL);
        g_pcb_superCapVtg_status = 1;
    }
    else
    {
        if ((g_pcb_bat_status == 1) || (g_pcb_mainDcVtg_status == 1) || (g_pcb_sleepCurrent_status == 1))
        {
            leds_operations_updateLedStatus(BOARD_VTG_STATUS, TEST_FAIL);
        }
        else
        {
            g_pcb_mainDcVtg_status = 0;
            leds_operations_updateLedStatus(BOARD_VTG_STATUS, TEST_PASS);
        }
    }
    return JIG_COMMNAD_OK;
}

e_jigErrorCodes pcb_tests_meter_reset(char *data, uint8_t *dataLen)
{
    float fADCReading_reset = 0.0;
    char dummy_data[PC_DATA_MAX_LEN]; // only used for 1hz generation
    uint8_t dummy_dataLen = 0;
    uint16_t counter = 0;
    wTimeout = 0;

    if (0 != SW_IN_2)
    {
        return REQ_SW_NOT_ON;
    }
    if (RELAY_POWER == 0)
    {
        RELAY_POWER_ON();
        delay_ms(1000);
    }
    RELAY_RESET_ON();
    while (1)
    {
        if (counter >= 2500)
        {
            leds_operations_updateLedStatus(RESET_STATUS, TEST_FAIL);
            break;
        }
        fADCReading_reset = getAdcValue2(VOLTAGE_RESET_CHK); // approx 6ms
        if (fADCReading_reset < 1.2)
        {
            counter = 1;
            break;
        }
        counter++;
    }

    while (counter)
    {
        fADCReading_reset = getAdcValue2(VOLTAGE_RESET_CHK);
        if (fADCReading_reset > 1.6)
        {
            leds_operations_updateLedStatus(RESET_STATUS, TEST_PASS);
            counter = 1;
            break;
        }
        if (counter > 25)
        {
            leds_operations_updateLedStatus(RESET_STATUS, TEST_FAIL);
            break;
        }
        counter++;
    }
    RELAY_RESET_OFF();
    memcpy(&data[*dataLen], &fADCReading_reset, sizeof(fADCReading_reset));
    *dataLen += sizeof(fADCReading_reset);
    ////////////////////////////////////////////////////////////1hz GENERATE////////

    if (gRTC_ret == RTC_CALIB_OK)
    {
        delay_ms(4000);
        if (JIG_COMMNAD_OK != (meter_data_getMeterParam(dummy_data, &dummy_dataLen, START_1HZ))) // 1hz generation
        {
            if (JIG_COMMNAD_OK != (meter_data_getMeterParam(dummy_data, &dummy_dataLen, START_1HZ)))
            {
                gRTC_ret = FREQUENCY_NOT_GENERATED;
            }
        }
        gRTC_ret = FREQUENCY_GENERATION_SUCCESS;
    }
    return JIG_COMMNAD_OK;
}