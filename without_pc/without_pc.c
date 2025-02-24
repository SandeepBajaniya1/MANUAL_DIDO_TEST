
#include "without_pc.h"

uint8_t GetRTC_ParRetry = 0;

#define AVGPPM_OFFSET_MIN -8.0
#define AVGPPM_OFFSET_MAX 8.0
#define PPM_OFFSET_MIN -35.0
#define PPM_OFFSET_MAX 65.0
#define OPERATIONALPPM_OFFSET_MIN -35.0
#define OPERATIONALPPM_OFFSET_MAX 65.0
#define TEMPERATURE_OFFSET_MIN -20.0
#define TEMPERATURE_OFFSET_MAX 20.0
#define CURRENTTEMPERATURE_OFFSET_MIN 15.0
#define CURRENTTEMPERATURE_OFFSET_MAX 45.0
#define CURRENTPPM_MIN -35.0
#define CURRENTPPM_MAX 65.0

float favgPPM_offset = 0.0;
float fPPM_Offset = 0.0;
float foperationalPPM_offset = 0.0;
float ftemperature_offset = 0.0;
float fcurrentTemperature = 0.0;
float fcurrentPPM = 0.0;

/**
 *
 */
void without_PC_operation(void)
{
    e_jigErrorCodes ret = JIG_COMMNAD_OK;
    char data[PC_DATA_MAX_LEN];
    uint8_t dataLen = 0;
    uint8_t *dataPtr;
    char tempStr[30];
    char meterResponseBuf[METER_RESPONSE_SIZE];
    t_txFrameInfo txPcFrame;
    e_jigErrorCodes RTC_ret;

    RELAY_POWER_ON();
    delay_ms(10000);

    without_pc_rtcCalibration();

    /* if (SW_IN_7 == 0)
      {
          txPcFrame.rxCommand = GET_METER_PCB_BAT_VTG;
          ret = pcb_tests_getBatVoltage(data, &dataLen);
          pc_comm_createResponseFrame(data, dataLen, ret, &txPcFrame); // Update response structure
          ret = pc_comm_sendPcResponse(COMPUTER_PORT, &txPcFrame);

          dataLen = 0;
          txPcFrame.rxCommand = GET_MAIN_BAT_SLEEP_CURRENT;
          ret = pcb_tests_getSleepcurrent_SPH(data, &dataLen);
          pc_comm_createResponseFrame(data, dataLen, ret, &txPcFrame); // Update response structure
          ret = pc_comm_sendPcResponse(COMPUTER_PORT, &txPcFrame);     // Send response to PC

          dataLen = 0;
          txPcFrame.rxCommand = GET_METER_PCB_MAINS_DC_VTG;
          ret = pcb_tests_mainsDcVtg(data, &dataLen);
          pc_comm_createResponseFrame(data, dataLen, ret, &txPcFrame); // Update response structure
          ret = pc_comm_sendPcResponse(COMPUTER_PORT, &txPcFrame);
      }
      if (SW_IN_7 != 0)
      {
          delay_ms(4000);
      }
      delay_ms(3000);
      dataLen = 0;
      */
    if (SW_IN_4 == 0)
    {
        // dataLen = 0;
        // txPcFrame.rxCommand = GET_STATUS_VTG_AND_CURRENT;
        // ret = funcTest_vtgAndCurrent_LTCT_3p(data, &dataLen);
        // pc_comm_createResponseFrame(data, dataLen, ret, &txPcFrame);
        // pc_comm_sendPcResponse(COMPUTER_PORT, &txPcFrame);
        delay_ms(1000);
        funcTest_kwh();
        delay_ms(1000);
    }
    if (SW_IN_1 == 0)
    {
        dataLen = 0;
        txPcFrame.rxCommand = GET_STATUS_METER_I2C_CHECK;
        ret = funcTest_I2cCheck();
        pc_comm_createResponseFrame(data, dataLen, ret, &txPcFrame);
        ret = pc_comm_sendPcResponse(COMPUTER_PORT, &txPcFrame);

        delay_ms(2000);
        dataLen = 0;
        txPcFrame.rxCommand = GET_STATUS_METER_CLEAR1;
        ret = funcTest_clear1();
        if (ret != JIG_COMMNAD_OK)
        {
            delay_ms(2000);
            dataLen = 0;
            txPcFrame.rxCommand = GET_STATUS_METER_CLEAR1;
            ret = funcTest_clear1();
        }
        pc_comm_createResponseFrame(data, dataLen, ret, &txPcFrame);
        pc_comm_sendPcResponse(COMPUTER_PORT, &txPcFrame);
        if (ret == JIG_COMMNAD_OK)
        {
            //  delay_ms(60000);
            //  delay_ms(25000);
        }
    }

    /*
        if (SW_IN_2 == 0)
        {
            dataLen = 0;
            txPcFrame.rxCommand = GET_STATUS_METER_RESET;
            ret = pcb_tests_meter_reset(data, &dataLen);
            pc_comm_createResponseFrame(data, dataLen, ret, &txPcFrame);
            ret = pc_comm_sendPcResponse(COMPUTER_PORT, &txPcFrame);
        }

        if (RTC_ret == RTC_CALIB_OK)
        {
            uint8_t rtcRetry = 0;
            delay_ms(10000);
            while (1)
            {
                meter_comm_setInstantParam(RTC_Comp_Pulse_start, 0, 1); // Initiate Calibration in Meter
                // Wait for meter acknowledgement
                if (meter_comm_checkMeterResponse(OPTICAL_PORT, 15, NORM_RESP) == METER_RESPONSE_TIMEOUT)
                {
                    rtcRetry++;
                    if (rtcRetry > 5)
                    {
                        byLedDisplayResult[2] = 1;
                        break;
                    }
                    delay_ms(1000);
                    continue;
                }
                break;
            }
        }

        if (SW_IN_3_RELAY == 0)
        {
            dataLen = 0;
            //  ret = funcTest_relay();
            txPcFrame.rxCommand = GET_STATUS_RELAY_TEST;
            pc_comm_createResponseFrame(data, dataLen, ret, &txPcFrame);
            ret = pc_comm_sendPcResponse(COMPUTER_PORT, &txPcFrame);
        }

        if (SW_IN_4 == 0)
        {
            dataLen = 0;
            // ret = funcTest_vtgAndCurrent(data, &dataLen);
            txPcFrame.rxCommand = GET_STATUS_VTG_AND_CURRENT;
            pc_comm_createResponseFrame(data, dataLen, ret, &txPcFrame);
            ret = pc_comm_sendPcResponse(COMPUTER_PORT, &txPcFrame);
        }

        if (SW_IN_5 == 0)
        {
            dataLen = 0;
        }

        if (SW_IN_6 == 0)
        {
            dataLen = 0;
        }

        if (RTC_ret == RTC_CALIB_OK)
        {
            RTC_ret = rtc_prod_getAvgPPM(g_data, &g_dataLen);
        }

        if (SW_IN_7 == 0)
        {
            dataLen = 0;
        }

        do
        {
            GetRTC_ParRetry++;
            dataLen = 0;
            ret = meter_data_getRtcParam(data, &dataLen);
            delay_ms(1000);
            dataLen = 0;
            ret = meter_data_getRtcParam(data, &dataLen);
            RTC_ret = ret;
            memcpy(g_data + 4, data, 20);
            g_dataLen = dataLen + 4;
            if (RTC_ret == RTC_GET_DATA_ERROR)
            {
                ret = RTC_ret;
            }

            if (RTC_ret == RTC_READ_OK)
            {
                memcpy(&favgPPM_offset, &g_data[0], sizeof(float));
                memcpy(&fPPM_Offset, &g_data[4], sizeof(float));
                memcpy(&foperationalPPM_offset, &g_data[8], sizeof(float));
                memcpy(&ftemperature_offset, &g_data[12], sizeof(float));
                memcpy(&fcurrentTemperature, &g_data[16], sizeof(float));
                memcpy(&fcurrentPPM, &g_data[20], sizeof(float));


                if (favgPPM_offset < AVGPPM_OFFSET_MIN || favgPPM_offset > AVGPPM_OFFSET_MAX)
                {
                    byLedDisplayResult[2] = 1;
                }
                if (fPPM_Offset < PPM_OFFSET_MIN || fPPM_Offset > PPM_OFFSET_MAX)
                {
                    byLedDisplayResult[2] = 1;
                }
                if (foperationalPPM_offset < OPERATIONALPPM_OFFSET_MIN || foperationalPPM_offset > OPERATIONALPPM_OFFSET_MAX)
                {
                    byLedDisplayResult[2] = 1;
                }
                if (ftemperature_offset < TEMPERATURE_OFFSET_MIN || ftemperature_offset > TEMPERATURE_OFFSET_MAX)
                {
                    byLedDisplayResult[2] = 1;
                }
                if (fcurrentTemperature < CURRENTTEMPERATURE_OFFSET_MIN || fcurrentTemperature > CURRENTTEMPERATURE_OFFSET_MAX)
                {
                    byLedDisplayResult[2] = 1;
                }
                if (fcurrentPPM < CURRENTPPM_MIN || fcurrentPPM > CURRENTPPM_MAX)
                {
                    byLedDisplayResult[2] = 1;
                }

                if (byLedDisplayResult[2] == 0)
                {
            txPcFrame.rxCommand = GET_RTC_PARAMETERS;
                    pc_comm_createResponseFrame(g_data, g_dataLen, ret, &txPcFrame); // Update response structure
                    ret = pc_comm_sendPcResponse(COMPUTER_PORT, &txPcFrame);         // Send response to PC
                    break;
                }
            }
            if(GetRTC_ParRetry == 2)
            {
            txPcFrame.rxCommand = GET_RTC_PARAMETERS;
            pc_comm_createResponseFrame(g_data, g_dataLen, ret, &txPcFrame); // Update response structure
            ret = pc_comm_sendPcResponse(COMPUTER_PORT, &txPcFrame);         // Send response to PC
            }

        } while (GetRTC_ParRetry <= 2);
    */
    glow_allPassLed();
}

void without_pc_rtcCalibration(void)
{
    e_jigErrorCodes ret;
    char data[PC_DATA_MAX_LEN];
    uint8_t dataLen = 0;
    t_txFrameInfo txPcFrame;

    // this is needed when communicating with RTC board.
    R_UART2_Stop();
    R_UART3_Stop();
    R_SAU1_Create(9600);
    R_UART3_Start();
    txPcFrame.rxCommand = START_RTC_CALIB;
    ret = rtc_prod_initiateRTCCalibration(data, &dataLen);       // Initialize the RTC Process
    pc_comm_createResponseFrame(data, dataLen, ret, &txPcFrame); // Update response structure
    txPcFrame.rxCommand = START_RTC_CALIB;
    pc_comm_sendPcResponse(COMPUTER_PORT, &txPcFrame); // Send response to PC
    R_UART2_Stop();
    R_UART3_Stop();
    R_SAU1_Create(19200);
    R_UART2_Start();
    R_UART3_Start();

    // Temporary using Date-Time LED for RTC Status
    if (ret != RTC_CALIB_OK)
    {
        leds_operations_updateLedStatus(DATE_TIME, TEST_FAIL);
    }
    else
    {
        leds_operations_updateLedStatus(DATE_TIME, TEST_PASS);
    }
}

void glow_allPassLed(void)
{
    uint8_t i = 0;
    for (i = 0; i < 16; i++)
    {
        uint8_t status;
        status = byLedDisplayResult[i];
        if (status ==1)
        {
            byLedDisplayResult[12] = TEST_FAIL;
        }
    }
    if (byLedDisplayResult[12] == 2)
    {
        byLedDisplayResult[12] = TEST_PASS;
    }
}