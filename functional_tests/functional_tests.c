#include "functional_tests.h"

e_jigErrorCodes funcTest_I2cCheck()
{
    uint8_t testStatus = 0;
    e_jigErrorCodes ret = JIG_COMMNAD_OK;
    if (SW_IN_1 != 0)
    {
        return REQ_SW_NOT_ON;
    }

    if (RELAY_POWER == 0)
    {
        RELAY_POWER_ON();
        delay_ms(8000);
    }

    ret = meter_comm_invokeCommand(METER_CMD_FORMAT_ACTION_MEM_TEST_I2C, OPTICAL_PORT);
    if (ret != METER_RESPONSE_OK)
    {
        byLedDisplayResult[1] = 1;
        return ret;
    }

    byLedDisplayResult[1] = testStatus;
    return JIG_COMMNAD_OK;
}

e_jigErrorCodes funcTest_clear1()
{
    if (SW_IN_1 != 0)
    {
        return REQ_SW_NOT_ON;
    }
    if (RELAY_POWER == 0)
    {
        RELAY_POWER_ON();
        delay_ms(8000);
    }

    if (meter_comm_invokeCommand(METER_CMD_MEM_TYPE_DATAFLASH, OPTICAL_PORT) != METER_RESPONSE_OK)
    {
        leds_operations_updateLedStatus(CLEAR_STATUS, TEST_FAIL);
        return METER_RESPONSE_TIMEOUT;
    }

    leds_operations_updateLedStatus(CLEAR_STATUS, TEST_PASS);
    return JIG_COMMNAD_OK;
}

e_jigErrorCodes funcTest_clear2()
{
    if (SW_IN_1 != 0)
    {
        return REQ_SW_NOT_ON;
    }
    if (RELAY_POWER == 0)
    {
        RELAY_POWER_ON();
        delay_ms(8000);
    }

    if (meter_comm_invokeCommand(METER_CMD_MEM_TYPE_EEPROM, OPTICAL_PORT) != METER_RESPONSE_OK)
    {
        leds_operations_updateLedStatus(CLEAR_STATUS, TEST_FAIL);
        return METER_RESPONSE_TIMEOUT;
    }

    leds_operations_updateLedStatus(CLEAR_STATUS, TEST_PASS);
    return JIG_COMMNAD_OK;
}

e_jigErrorCodes funcTest_relay()
{
    e_jigErrorCodes ret;
    uint8_t loopindex = 0;

    // if (SW_IN_3_RELAY != 0)
    //{
    //     return REQ_SW_NOT_ON;
    // }

    RELAY_POWER_ON();
    if (SW_IN_1 != 0)
    {
        delay_ms(3000);
    }
    delay_ms(2000);
    for (loopindex = 0; loopindex < MAX_RELAY_COUNT; loopindex++)
    {
        stRlyChk[loopindex].byCount = 0;
    }
    if ((ret = meter_comm_invokeCommand(METER_CMD_FORMAT_ACTION_RELAY_CONNECT, OPTICAL_PORT)) != METER_RESPONSE_OK)
    {
        leds_operations_updateLedStatus(RELAY_STATUS, TEST_FAIL);
        return ret;
    }
    // else if ((0 != RELAY_CHECK_IN1) || (0 != RELAY_CHECK_IN2))          //Pulses check
    else if (FALSE == checkRelayStatus_WCM_3p(1))
    {
        leds_operations_updateLedStatus(RELAY_STATUS, TEST_FAIL);
        return METER_PIN_TEST_FAILED;
    }

    delay_ms(2000);
    for (loopindex = 0; loopindex < MAX_RELAY_COUNT; loopindex++)
    {
        stRlyChk[loopindex].byCount = 0;
    }
    if ((ret = meter_comm_invokeCommand(METER_CMD_FORMAT_ACTION_RELAY_DISCONNECT, OPTICAL_PORT)) != METER_RESPONSE_OK)
    {
        leds_operations_updateLedStatus(RELAY_STATUS, TEST_FAIL);
        return ret;
    }
    // else if ((1 != RELAY_CHECK_IN1) || (1 != RELAY_CHECK_IN2))          //Pulses check
    else if (FALSE == checkRelayStatus_WCM_3p(0))
    {
        leds_operations_updateLedStatus(RELAY_STATUS, TEST_FAIL);
        return METER_PIN_TEST_FAILED;
    }

    delay_ms(2000);
    for (loopindex = 0; loopindex < MAX_RELAY_COUNT; loopindex++)
    {
        stRlyChk[loopindex].byCount = 0;
    }
    if ((ret = meter_comm_invokeCommand(METER_CMD_FORMAT_ACTION_RELAY_CONNECT, OPTICAL_PORT)) != METER_RESPONSE_OK)
    {
        leds_operations_updateLedStatus(RELAY_STATUS, TEST_FAIL);
        return ret;
    }
    // else if ((0 != RELAY_CHECK_IN1) || (0 != RELAY_CHECK_IN2))          //Pulses check
    else if (FALSE == checkRelayStatus_WCM_3p(1))
    {
        leds_operations_updateLedStatus(RELAY_STATUS, TEST_FAIL);
        return METER_PIN_TEST_FAILED;
    }

    leds_operations_updateLedStatus(RELAY_STATUS, TEST_PASS);
    return JIG_COMMNAD_OK;
}

e_jigErrorCodes funcTest_vtgAndCurrent_SPH(char *data, uint8_t *dataLen)
{
    e_jigErrorCodes ret = JIG_COMMNAD_OK;
    uint8_t testStatus = 0;

    float fVRMS_R = 0.0;
    float fIRMS_R = 0.0;
    float fVRMS_Y = 0.0;
    float fIRMS_Y = 0.0;
    float fVRMS_B = 0.0;
    float fIRMS_B = 0.0;
    float fIRMS_N = 0.0;
    if (SW_IN_4 != 0)
    {
        return REQ_SW_NOT_ON;
    }

    if (RELAY_POWER == 0)
    {
        RELAY_POWER_ON();
        delay_ms(8000);
    }
    RELAY_SHUNT_ON();
    RELAY_CT_ON();
    delay_ms(3000);
    meter_data_getMeterParam(data, dataLen, GET_STATUS_VTG_AND_CURRENT);
    if ((ret = meter_data_getMeterParam(data, dataLen, GET_STATUS_VTG_AND_CURRENT)) != JIG_COMMNAD_OK)
    {
        RELAY_SHUNT_OFF();
        RELAY_CT_OFF();
        byLedDisplayResult[6] = 1;
        return ret;
    }
    if (*dataLen >= sizeof(float) * 7)
    {
        memcpy(&fVRMS_R, &data[0], sizeof(float));
        memcpy(&fIRMS_R, &data[4], sizeof(float));
        memcpy(&fIRMS_N, &data[8], sizeof(float));

        if ((fVRMS_R < VRMS_MIN) | (fIRMS_R < IRMS_MIN) | (fIRMS_N < IRMS_N_MIN))
        {
            leds_operations_updateLedStatus(CUR_VTG_CHECK, TEST_FAIL);
            *dataLen = 0;
            RELAY_SHUNT_OFF();
            RELAY_CT_OFF();
            return METER_RESPONSE_ERROR;
        }
        else
        {
            leds_operations_updateLedStatus(CUR_VTG_CHECK, TEST_PASS);
        }
    }
    else
    {
        *dataLen = 0;
        RELAY_SHUNT_OFF();
        RELAY_CT_OFF();
        leds_operations_updateLedStatus(CUR_VTG_CHECK, TEST_FAIL);
        return METER_RESPONSE_ERROR;
    }

    RELAY_SHUNT_OFF();
    RELAY_CT_OFF();
    return JIG_COMMNAD_OK;
}

e_jigErrorCodes funcTest_vtgAndCurrent_LTCT_3p(char *data, uint8_t *dataLen)
{
    e_jigErrorCodes ret = JIG_COMMNAD_OK;
    uint8_t testStatus = 0;

    float fVRMS_R = 0.0;
    float fIRMS_R = 0.0;
    float fVRMS_Y = 0.0;
    float fIRMS_Y = 0.0;
    float fVRMS_B = 0.0;
    float fIRMS_B = 0.0;
    float fIRMS_N = 0.0;
    if (SW_IN_4 != 0)
    {
        return REQ_SW_NOT_ON;
    }

    if (RELAY_POWER == 0)
    {
        RELAY_POWER_ON();
        delay_ms(8000);
    }
    RELAY_SHUNT_ON();
    RELAY_CT_ON();
    delay_ms(3000);
    meter_data_getMeterParam(data, dataLen, GET_STATUS_VTG_AND_CURRENT);
    if ((ret = meter_data_getMeterParam(data, dataLen, GET_STATUS_VTG_AND_CURRENT)) != JIG_COMMNAD_OK)
    {
        RELAY_SHUNT_OFF();
        RELAY_CT_OFF();
        byLedDisplayResult[6] = 1;
        return ret;
    }
    if (*dataLen >= sizeof(float) * 7)
    {
        memcpy(&fVRMS_R, &data[0], sizeof(float));
        memcpy(&fVRMS_Y, &data[4], sizeof(float));
        memcpy(&fVRMS_B, &data[8], sizeof(float));
        memcpy(&fIRMS_R, &data[12], sizeof(float));
        memcpy(&fIRMS_Y, &data[16], sizeof(float));
        memcpy(&fIRMS_B, &data[20], sizeof(float));
        memcpy(&fIRMS_N, &data[24], sizeof(float));

        if ((fVRMS_R < VRMS_MIN) | (fIRMS_R < IRMS_MIN) |
            (fVRMS_Y < VRMS_MIN) | (fIRMS_Y < IRMS_MIN) |
            (fVRMS_B < VRMS_MIN) | (fIRMS_B < IRMS_MIN) | (fIRMS_N < IRMS_N_MIN))
        {
            leds_operations_updateLedStatus(CUR_VTG_CHECK, TEST_FAIL);
            *dataLen = 0;
            RELAY_SHUNT_OFF();
            RELAY_CT_OFF();
            return METER_RESPONSE_ERROR;
        }
        else
        {
            leds_operations_updateLedStatus(CUR_VTG_CHECK, TEST_PASS);
        }
    }
    else
    {
        *dataLen = 0;
        RELAY_SHUNT_OFF();
        RELAY_CT_OFF();
        leds_operations_updateLedStatus(CUR_VTG_CHECK, TEST_FAIL);
        return METER_RESPONSE_ERROR;
    }

    RELAY_SHUNT_OFF();
    RELAY_CT_OFF();
    return JIG_COMMNAD_OK;
}

e_jigErrorCodes funcTest_kwh()
{
    e_jigErrorCodes ret = JIG_COMMNAD_OK;
    if (SW_IN_4 != 0)
    {
        return REQ_SW_NOT_ON;
    }
    if (RELAY_POWER == 0)
    {
        RELAY_POWER_ON();
        delay_ms(5000);
    }
    RELAY_SHUNT_ON();
    RELAY_CT_ON();
    delay_ms(500);
    if (FALSE == checkBlinking_Kwh())
    {
        leds_operations_updateLedStatus(KWH_CHECK, TEST_FAIL);
        ret = METER_KWH_TEST_FAILED;
        g_KWH_status = 1;
    }
    else
    {
        if (g_kVarh_status == 0)
        {
            g_KWH_status = 0;
            leds_operations_updateLedStatus(KWH_CHECK, TEST_PASS);
        }
        else
        {
            leds_operations_updateLedStatus(KWH_CHECK, TEST_FAIL);
        }
    }
    RELAY_SHUNT_OFF();
    RELAY_CT_OFF();
    return ret;
}

e_jigErrorCodes funcTest_kVarh()
{
    e_jigErrorCodes ret = JIG_COMMNAD_OK;
    if (SW_IN_4 != 0)
    {
        return REQ_SW_NOT_ON;
    }
    if (RELAY_POWER == 0)
    {
        RELAY_POWER_ON();
        delay_ms(5000);
    }
    RELAY_SHUNT_ON();
    RELAY_CT_ON();
    delay_ms(500);
    if (FALSE == checkBlinking_KVarh())
    {
        leds_operations_updateLedStatus(KWH_CHECK, TEST_FAIL);
        ret = METER_KVARH_TEST_FAILED;
        g_kVarh_status = 1;
    }
    else
    {
        if (g_KWH_status == 0)
        {
            g_kVarh_status = 0;
            leds_operations_updateLedStatus(KWH_CHECK, TEST_PASS);
        }
        else
        {
            leds_operations_updateLedStatus(KWH_CHECK, TEST_FAIL);
        }
    }
    RELAY_SHUNT_OFF();
    RELAY_CT_OFF();
    return ret;
}

e_jigErrorCodes funcTest_rf()
{
    e_jigErrorCodes ret;
    if (SW_IN_5 != 0)
    {
        return REQ_SW_NOT_ON;
    }
    if (RELAY_POWER == 0)
    {
        RELAY_POWER_ON();
        delay_ms(8000);
    }

    RF_OUT_NET_STAT = 0;
    RF_OUT_LINK_STAT = 1; // inverted logic
    RF_OUT_CTS = 0;
    if ((ret = meter_comm_invokeCommand(METER_CMD_FORMAT_ACTION_RF_IO_LOW_CHECK, RF_PORT)) != METER_RESPONSE_OK)
    {
        leds_operations_updateLedStatus(RF_STATUS, TEST_FAIL);
        return ret;
    }
    else if (FALSE == checkRFPins(1))
    {
        leds_operations_updateLedStatus(RF_STATUS, TEST_FAIL);
        return METER_PIN_TEST_FAILED;
    }

    delay_ms(2000);
    RF_OUT_NET_STAT = 1;
    RF_OUT_LINK_STAT = 0;
    RF_OUT_CTS = 1;
    if ((ret = meter_comm_invokeCommand(METER_CMD_FORMAT_ACTION_RF_IO_HIGH_CHECK, RF_PORT)) != METER_RESPONSE_OK)
    {
        leds_operations_updateLedStatus(RF_STATUS, TEST_FAIL);
        return ret;
    }
    else if (FALSE == checkRFPins(0))
    {
        leds_operations_updateLedStatus(RF_STATUS, TEST_FAIL);
        return METER_PIN_TEST_FAILED;
    }

    delay_ms(2000);
    RF_OUT_NET_STAT = 0;
    RF_OUT_LINK_STAT = 1;
    RF_OUT_CTS = 0;
    if ((ret = meter_comm_invokeCommand(METER_CMD_FORMAT_ACTION_RF_IO_LOW_CHECK, RF_PORT)) != METER_RESPONSE_OK)
    {
        leds_operations_updateLedStatus(RF_STATUS, TEST_FAIL);
        return ret;
    }
    else if (FALSE == checkRFPins(1))
    {
        leds_operations_updateLedStatus(RF_STATUS, TEST_FAIL);
        return METER_PIN_TEST_FAILED;
    }

    leds_operations_updateLedStatus(RF_STATUS, TEST_PASS);
    return JIG_COMMNAD_OK;
}

e_jigErrorCodes funcTest_meter_switch(char *data, uint8_t *dataLen)
{
    e_jigErrorCodes ret;
    uint8_t switch_data = 0;
    if (SW_IN_6 != 0)
    {
        return REQ_SW_NOT_ON;
    }
    if (RELAY_POWER == 0)
    {
        RELAY_POWER_ON();
        delay_ms(8000);
    }

    RELAY_SWITCH_CHK_OFF();
    delay_ms(200);

    if ((ret = meter_comm_getInstantParam(HW_Switch_Release_Chk, (uint8_t *)data, 0x01, NORM_RESP)) != METER_RESPONSE_OK)
    {
        leds_operations_updateLedStatus(SW_STATUS, TEST_FAIL);
        return ret;
    }
    else
    {
        if (0 != SW_IN_8) // Only switch check, else Switch + magnet
        {
            stComm[OPTICAL_PORT].byRdBuf[6] &= 0x07;
        }
    }
    switch_data |= stComm[OPTICAL_PORT].byRdBuf[6];

    RELAY_SWITCH_CHK_ON();
    delay_ms(200);
    if ((ret = meter_comm_getInstantParam(HW_Switch_Connect_Chk, (uint8_t *)data, 0x01, NORM_RESP)) != METER_RESPONSE_OK)
    {
        leds_operations_updateLedStatus(SW_STATUS, TEST_FAIL);
        RELAY_SWITCH_CHK_OFF();
        return ret;
    }
    else
    {
        if (0 != SW_IN_8) // Only switch check, else Switch + magnet
        {
            stComm[OPTICAL_PORT].byRdBuf[6] &= 0x07;
        }
    }
    switch_data |= stComm[OPTICAL_PORT].byRdBuf[6];

    if (0 != switch_data)
    {
        leds_operations_updateLedStatus(SW_STATUS, TEST_FAIL);
    }
    else
    {

        leds_operations_updateLedStatus(SW_STATUS, TEST_PASS);
    }
    memcpy(&data[*dataLen], &switch_data, sizeof(switch_data));
    *dataLen += sizeof(switch_data);
    RELAY_SWITCH_CHK_OFF();
    return JIG_COMMNAD_OK;
}

e_jigErrorCodes funcTest_getRTC_parameters(char *data, uint8_t *dataLen)
{
    e_jigErrorCodes ret = JIG_COMMNAD_OK;
    float PPM_Offset = 0xffffffff;        // set using rtc calib process
    float operational_PPM = 0xffffffff;   // operational ppm ,currently using the meter
    float temprature_Offset = 0xffffffff; // set using rtc calib process

    float current_temprature = 0xffffffff; // environment temp, sensed by meter
    float current_PPM = 0xffffffff;        // actual ppm computed by meter
    float averagePPM_Offset = 0xffffffff;  // current ppm , calculated using rtc calib process
    if (gRTC_ret != FREQUENCY_GENERATION_SUCCESS)
    {
        return gRTC_ret;
    }
    meter_data_getMeterParam(data, dataLen, GET_RTC_PARAMETERS); // to correction of data
    if ((ret = meter_data_getMeterParam(data, dataLen, GET_RTC_PARAMETERS)) != JIG_COMMNAD_OK)
    {
        return ret;
    }
    memcpy(&PPM_Offset, &data[0], sizeof(float));
    memcpy(&operational_PPM, &data[4], sizeof(float));
    memcpy(&temprature_Offset, &data[8], sizeof(float));
    memcpy(&current_temprature, &data[12], sizeof(float));
    memcpy(&current_PPM, &data[16], sizeof(float));

    if ((ret = rtc_prod_getAvgPPM(data, dataLen)) != RTC_READ_OK)
    {
        *dataLen = 0;
        return ret;
    }
    memcpy(&averagePPM_Offset, &data[20], sizeof(float));

    return JIG_COMMNAD_OK;
}

e_jigErrorCodes funcTest_allPassLed(char *data, uint8_t *dataLen)
{
    e_jigErrorCodes ret = JIG_COMMNAD_OK;

    if (*dataLen == 1)
    {

        if (((uint8_t) * (&data[0])) == 1)
        {
            leds_operations_updateLedStatus(ALL_PASS, TEST_PASS);
            leds_operations_updateLedStatus(CAP_ALL_PASS, TEST_PASS);
        }
        else
        {
            leds_operations_updateLedStatus(ALL_PASS, TEST_FAIL);
            leds_operations_updateLedStatus(CAP_ALL_PASS, TEST_FAIL);
        }
    }
    else
    {
        return JIG_WRONG_COMMAND;
    }
    return JIG_COMMNAD_OK;
}

e_jigErrorCodes funcTest_Digital_IO(char *data, uint8_t *dataLen)
{
    e_jigErrorCodes ret;
    char testByte[4];
    if (SW_IN_3_RELAY != 0)
    {
        return REQ_SW_NOT_ON;
    }

    if (RELAY_POWER == 0)
    {
        RELAY_POWER_ON();
        delay_ms(8000);
    }

#if defined(LTCT_3P) || defined(HTCT_3P)
    RELAY_D11 = 1;
    RELAY_D12 = 0;
    RELAY_D13 = 0;
    RELAY_D14 = 0;
#endif

    delay_ms(1000);
    if ((ret = meter_comm_getInstantParam(Digital_IO, (uint8_t *)data, 0x01, NORM_RESP)) != METER_RESPONSE_OK)
    {
        testByte[0] = 0;
        leds_operations_updateLedStatus(RELAY_STATUS, TEST_FAIL);
    }
    else
    {
        testByte[0] = ((stComm[OPTICAL_PORT].byRdBuf[6]) == 0x01 ? 0x01 : 0);
    }
#if defined(LTCT_3P) || defined(HTCT_3P)
    RELAY_D11 = 0;
    RELAY_D12 = 1;
#endif

    delay_ms(1000);
    if ((ret = meter_comm_getInstantParam(Digital_IO, (uint8_t *)data, 0x01, NORM_RESP)) != METER_RESPONSE_OK)
    {
        testByte[1] = 0;
        leds_operations_updateLedStatus(RELAY_STATUS, TEST_FAIL);
    }
    else
    {
        testByte[1] = ((stComm[OPTICAL_PORT].byRdBuf[6]) == 0x02 ? 0x01 : 0);
    }
#if defined(LTCT_3P) || defined(HTCT_3P)
    RELAY_D12 = 0;
    RELAY_D13 = 1;
#endif
    delay_ms(1000);
    if ((ret = meter_comm_getInstantParam(Digital_IO, (uint8_t *)data, 0x01, NORM_RESP)) != METER_RESPONSE_OK)
    {
        testByte[2] = 0;
        leds_operations_updateLedStatus(RELAY_STATUS, TEST_FAIL);
    }
    else
    {
        testByte[2] = ((stComm[OPTICAL_PORT].byRdBuf[6]) == 0x04 ? 0x01 : 0);
    }
#if defined(LTCT_3P) || defined(HTCT_3P)
    RELAY_D13 = 0;
    RELAY_D14 = 1;
#endif

    delay_ms(1000);
    if ((ret = meter_comm_getInstantParam(Digital_IO, (uint8_t *)data, 0x01, NORM_RESP)) != METER_RESPONSE_OK)
    {
        testByte[3] = 0;
        leds_operations_updateLedStatus(RELAY_STATUS, TEST_FAIL);
    }
    else
    {
        testByte[3] = ((stComm[OPTICAL_PORT].byRdBuf[6]) == 0x08 ? 0x01 : 0);
    }
#if defined(LTCT_3P) || defined(HTCT_3P)
    RELAY_D14 = 0;
#endif

    memcpy(&data[*dataLen], &testByte[0], sizeof(testByte));
    *dataLen += sizeof(testByte);
    if (testByte[0] & testByte[1] & testByte[2] & testByte[3])
    {
        leds_operations_updateLedStatus(RELAY_STATUS, TEST_PASS);
    }
    else
    {
        leds_operations_updateLedStatus(RELAY_STATUS, TEST_FAIL);
    }

    return JIG_COMMNAD_OK;
}

uint8_t checkBlinking_Kwh(void)
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
int8_t checkBlinking_KVarh(void)
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
        if (LDR_REACTIVE != u8LEDStateCheck)
        {
            u8LEDStateCheck = LDR_REACTIVE;
            u8LEDBlinkCount++;
            if (u8LEDBlinkCount > 4)
            {
                return TRUE;
            }
        }
    }
}

uint8_t checkRelayStatus_WCM_3p(BYTE event)
{
    uint8_t byCRSIndex, byCRSIndex2, byRelayInUse;
    uint8_t relay_status = 0;
    wTimeout = 0;
    byRelayInUse = RELAY_IN_USE_3P; // no. of relays

    while (1)
    {
        wTimeout++;
        if (1 == event) // Relay connect
        {
            if (stRlyChk[1].byCount <= 10)
            {
                relay_status = 1;
            }
            if (stRlyChk[3].byCount <= 10)
            {
                relay_status = 1;
            }
            if (stRlyChk[5].byCount <= 10)
            {
                relay_status = 1;
            }
        }
        if (0 == event) // Relay disconnect
        {
            if (stRlyChk[0].byCount <= 10)
            {
                relay_status = 1;
            }
            if (stRlyChk[2].byCount <= 10)
            {
                relay_status = 1;
            }
            if (stRlyChk[4].byCount <= 10)
            {
                relay_status = 1;
            }
        }
        if (relay_status == 0)
        {
            break;
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
    return TRUE;
}

e_jigErrorCodes manual_jig_funcTest_Digital_IO(void)
{
    e_jigErrorCodes ret;
    char testByte[4];
    char data[PC_DATA_MAX_LEN];
    char data1[] = {0x01, 0x02, 0x03, 0x04}; // TEMPORAY DATA FOR TESTING
    uint8_t dataLen = 0;
    uint8_t *dataPtr;
    char tempStr[30];
    char meterResponseBuf[METER_RESPONSE_SIZE];
    uint8_t retry_count = 0;

    if (RELAY_POWER == 0)
    {
        // RELAY_POWER_ON();
    }
    delay_ms(3000); // 1st

    RELAY_SHUNT = 1; // 1st
    RELAY_CT = 0;    // 2nd
    RELAY_SCAN = 0;  // 3rd dido
    DIDO_FOURTH = 0; // 4th dido

    delay_ms(3000);
    do
    {
        if ((ret = meter_comm_getInstantParam(Digital_IO, (uint8_t *)data, 0x01, NORM_RESP)) == METER_RESPONSE_OK)
        {
            testByte[0] = ((stComm[OPTICAL_PORT].byRdBuf[6]) == 0x01 ? 0x01 : 0);
            if (testByte[0] == 1)
            {
                byLedDisplayResult[1] = TEST_PASS;
                break;
            }
            else
            {
                byLedDisplayResult[1] = TEST_FAIL;
            }
        }
        else
        {
            byLedDisplayResult[1] = TEST_FAIL;
        }
        delay_ms(3000);
        retry_count++;
    } while (retry_count < 3);

    RELAY_SHUNT = 0; // 1st
    delay_ms(500);
    RELAY_CT = 1;    // 2nd
    RELAY_SCAN = 0;  // 3rd dido
    DIDO_FOURTH = 0; // 4th dido

    delay_ms(2000); // 2nd
    if ((ret = meter_comm_getInstantParam(Digital_IO, (uint8_t *)data, 0x01, NORM_RESP)) != METER_RESPONSE_OK)
    {
        testByte[1] = 0;
        byLedDisplayResult[2] = TEST_FAIL;
    }
    else
    {
        testByte[1] = ((stComm[OPTICAL_PORT].byRdBuf[6]) == 0x02 ? 0x01 : 0);
        if (testByte[1] == 1)
        {
            byLedDisplayResult[2] = TEST_PASS;
        }
        else
        {
            byLedDisplayResult[2] = TEST_FAIL;
        }
    }
    RELAY_SHUNT = 0; // 1st
    RELAY_CT = 0;    // 2nd
    delay_ms(500);
    RELAY_SCAN = 1;  // 3rd dido
    DIDO_FOURTH = 0; // 4th dido

    delay_ms(2000); // 3rd
    if ((ret = meter_comm_getInstantParam(Digital_IO, (uint8_t *)data, 0x01, NORM_RESP)) != METER_RESPONSE_OK)
    {
        testByte[2] = 0;
        byLedDisplayResult[3] = TEST_FAIL;
    }
    else
    {
        testByte[2] = ((stComm[OPTICAL_PORT].byRdBuf[6]) == 0x04 ? 0x01 : 0);
        if (testByte[2] == 1)
        {
            byLedDisplayResult[3] = TEST_PASS;
        }
        else
        {
            byLedDisplayResult[3] = TEST_FAIL;
        }
    }

    RELAY_SHUNT = 0; // 1st
    RELAY_CT = 0;    // 2nd
    RELAY_SCAN = 0;  // 3rd dido
    delay_ms(500);
    DIDO_FOURTH = 1; // 4th dido

    delay_ms(2000); // 4th
    if ((ret = meter_comm_getInstantParam(Digital_IO, (uint8_t *)data, 0x01, NORM_RESP)) != METER_RESPONSE_OK)
    {
        testByte[3] = 0;
        byLedDisplayResult[4] = TEST_FAIL;
    }
    else
    {
        testByte[3] = ((stComm[OPTICAL_PORT].byRdBuf[6]) == 0x08 ? 0x01 : 0);
        if (testByte[3] == 1)
        {
            byLedDisplayResult[4] = TEST_PASS;
        }
        else
        {
            byLedDisplayResult[4] = TEST_FAIL;
        }
    }

    RELAY_CT = 0;
    RELAY_SHUNT = 0;
    RELAY_SCAN = 0;  // 3rd dido
    DIDO_FOURTH = 0; // 4th dido

    return JIG_COMMNAD_OK;
}