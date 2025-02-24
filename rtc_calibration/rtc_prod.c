#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_serial.h"
#include "r_cg_adc.h"
#include "r_cg_it.h"
#include <string.h>
#include <stdio.h>
#include "r_cg_userdefine.h"
#include "rtc_prod.h"

int g_temperature = -10000;
int g_PPM = -10000;
int g_actualPPM = -30000;
int g_avgPPM = -10000;

/**
 * This function sends the reset command to STM32 board prior to start computing the PPM from specific meter board
 */
static void rtc_prod_resetPPMBoard(void)
{
    // This is demo function to send the data to PC
    uint8_t byLength = 1;
    stComm[RTC_PPM_TEMP_PORT].byWrBuf[0] = 0x01;
    // memcpy(&stComm[RTC_PPM_TEMP_PORT].byWrBuf[0], str, 1);
    stComm[RTC_PPM_TEMP_PORT].bySndLen = byLength;
    stComm[RTC_PPM_TEMP_PORT].bySndPtr = 0;
    // stComm[RTC_PPM_TEMP_PORT].byWrPtr = 0;

    Port3TxIntEnable();
    // bySPCDIndex = 0;
    while ((stComm[RTC_PPM_TEMP_PORT].bySndPtr < stComm[RTC_PPM_TEMP_PORT].bySndLen))
    {
        delay_ms(100);
    }
}

/**
 * This function checks the parameter for its range.
 * Note: keep track of data type
 */
static void rtc_prod_checkRange(int *param, int th)
{
    if (abs(*param) > th)
        *param = -10000;
}

/**
 * This function extract the PPM and temperature data sent by STM32 and assign them to variables.
 */
static void rtc_prod_setPpmTempData(int portNo, int wRespWaitTime)
{
    int meterNo = 0;
    int newLine = 0;
    int i = 0;
    static char rdBuf[PPPM_TEMP_DATA_BUF];
    g_temperature = -10000;
    g_PPM = -10000;
    g_actualPPM = -30000;
    g_avgPPM = -10000;

#ifdef DEBUG_MSG
    char buf[20];
#endif

    stComm[portNo].byWrPtr = 0; // clear Buffer

    // wait to arrive till entire data is available or new line character is arrived
    wTimeout = 0;
    while ((newLine <= 1) && (wTimeout < wRespWaitTime))
    {
        // scan the received data for new line
        if (i < stComm[portNo].byWrPtr)
        {
            if (stComm[portNo].byRdBuf[i] == '\n')
            {
                newLine++;
                if (newLine > 1)
                {
                    break;
                }
                else
                {
                    // flush out possible incomplete data
                    stComm[portNo].byWrPtr = 0;
                    i = 0;
                }
            }
            i++;
            continue;
        }
        wTimeout++;
        delay_ms(100);
    }

    if (wTimeout >= wRespWaitTime)
        return;

    newLine = 0;

    if (stComm[portNo].byWrPtr > 0)
    {
        // uint8_t byLength = (uint8_t)strlen((const char *)stComm[portNo].byRdBuf);
        // memcpy(rdBuf, &stComm[portNo].byRdBuf[0], byLength);
        memcpy(rdBuf, &stComm[portNo].byRdBuf[0], PPPM_TEMP_DATA_BUF);
#ifdef DEBUG_MSG
        rtc_prod_sendDataToPC(rdBuf, COMPUTER_PORT, sizeof(rdBuf));
#endif
        meterNo = atoi(strtok(rdBuf, ";"));
        g_PPM = atoi(strtok(NULL, ";"));
        g_temperature = atoi(strtok(NULL, ";"));
        g_actualPPM = atoi(strtok(NULL, ";"));
        g_avgPPM = atoi(strtok(NULL, ";"));

        if ((g_PPM < PPM_MIN) || (g_PPM > PPM_MAX))
            g_PPM = -10000;
        if ((g_temperature < TEMPERATURE_MIN) || (g_temperature > TEMPERATURE_MAX))
            g_temperature = -10000;
        if ((g_actualPPM < PPM_MIN) || (g_actualPPM > PPM_MAX))
            g_actualPPM = -10000;
        if ((g_avgPPM < AVG_PPM_MIN) || (g_avgPPM > AVG_PPM_MAX))
            g_avgPPM = -10000;
            // rtc_prod_checkRange(&g_PPM, PPM_MAX);
            // rtc_prod_checkRange(&g_temperature, TEMPERATURE_MAX);

#ifdef DEBUG_MSG
        sprintf(buf, "M: %d\n", meterNo);
        rtc_prod_sendDataToPC(buf, COMPUTER_PORT, strlen(buf));
        sprintf(buf, "P: %d\n", g_PPM);
        rtc_prod_sendDataToPC(buf, COMPUTER_PORT, strlen(buf));
        sprintf(buf, "T: %d\n", g_temperature);
        rtc_prod_sendDataToPC(buf, COMPUTER_PORT, strlen(buf));
#endif
        stComm[portNo].byWrPtr = 0;
    }
}

/**
 * This functiona starts the RTC calibration process
 */
e_jigErrorCodes rtc_prod_initiateRTCCalibration(char *data, uint8_t *dataLen)
{
    float ppmVal;
    float temperatureVal;
    float storedData = 0;
    uint8_t *tempData, loopCounter = 0;
    int wTimeout1 = 0;
#ifdef DEBUG_MSG
    char tempStr[20];
#endif

    // reset the variables in STM32 board
    rtc_prod_resetPPMBoard();
    // Start Meter in Calibration Mode
#ifdef DEBUG_MSG
    rtc_prod_sendDataToPC("Sending Calibration Command\n", COMPUTER_PORT, 30);
#endif
    delay_ms(100);

    meter_comm_setInstantParam(RTC_CALIBRATE, (uint8_t *)data, 4,NORM_RESP); // Initiate Calibration in Meter

    // Wait for meter acknowledgement
    if (meter_comm_checkMeterResponse(OPTICAL_PORT, 15, NORM_RESP) != METER_RESPONSE_OK)
        return RTC_CALIB_ERROR;
    delay_ms(METER_1_HZ_DELAY); // give some time to meter to start and measure the 1-Hz pulse

#ifdef DEBUG_MSG
    rtc_prod_sendDataToPC("Sent Calibration Command\n", COMPUTER_PORT, 30);
#endif
    wTimeout1 = 0;
    while (1)
    {
        rtc_prod_setPpmTempData(RTC_PPM_TEMP_PORT, 25);
        // check data validity
        if ((g_PPM == -10000) || (g_temperature == -10000) || (g_PPM == 0))
        {
#ifdef DEBUG_MSG
            rtc_prod_sendDataToPC("Checking Data Again...\n", COMPUTER_PORT, 20);
#endif
            wTimeout1++;
            if (wTimeout1 > 5)
                return RTC_GET_DATA_ERROR; // Check for 5 sec for correct data
            delay_ms(500);
            continue;
        }

// Send PPM Data
#ifdef DEBUG_MSG
        rtc_prod_sendDataToPC("Sending PPM Data\n", COMPUTER_PORT, 15);
#endif

        ppmVal = ((float)g_PPM / PPM_DIVISIOR);
        meter_comm_setInstantParam(PPM_OFFSET, (uint8_t *)&ppmVal, sizeof(float),NORM_RESP);
        // Wait for meter acknowledgement
        if (meter_comm_checkMeterResponse(OPTICAL_PORT, 15, NORM_RESP) != METER_RESPONSE_OK)
            return RTC_PPM_ERROR;
#ifdef DEBUG_MSG
        delay_ms(2000);
        rtc_prod_sendDataToPC("Sent PPM Data\n", COMPUTER_PORT, 14);
#endif

// Send Temperature Data
#ifdef DEBUG_MSG
        rtc_prod_sendDataToPC("Sending Temperature Data\n", COMPUTER_PORT, 25);
#endif
        temperatureVal = ((float)g_temperature / TEMP_DIVISIOR);
        meter_comm_setInstantParam(TEMPERATURE_OFFSET, (uint8_t *)&temperatureVal, sizeof(float),NORM_RESP);
        // Wait for meter acknowledgement
        if (meter_comm_checkMeterResponse(OPTICAL_PORT, 15, NORM_RESP) != METER_RESPONSE_OK)
            return RTC_TEMP_ERROR;
#ifdef DEBUG_MSG
        delay_ms(2000);
        rtc_prod_sendDataToPC("Sent Temperature Data\n", COMPUTER_PORT, 22);
#endif
        break;
    }

    // Read PPM OFFSET data set into meter
    if (meter_comm_getInstantParam(PPM_OFFSET, (uint8_t *)&storedData, sizeof(float),NORM_RESP) != METER_RESPONSE_OK)
        return RTC_READ_ERROR;
    // Copy data
    tempData = (uint8_t *)&storedData;
    *(data + (*dataLen)++) = *tempData;
    *(data + (*dataLen)++) = *(tempData + 1);
    *(data + (*dataLen)++) = *(tempData + 2);
    *(data + (*dataLen)++) = *(tempData + 3);

#ifdef DEBUG_MSG
    sprintf(tempStr, "PPM offset: %d\n", (int)(storedData * PPM_DIVISIOR));
    rtc_prod_sendDataToPC(tempStr, COMPUTER_PORT, strlen(tempStr));
#endif

    // Read Temperature OFFSET data set into meter
    if (meter_comm_getInstantParam(TEMPERATURE_OFFSET, (uint8_t *)&storedData, sizeof(float),NORM_RESP) != METER_RESPONSE_OK)
        return RTC_READ_ERROR;
    tempData = (uint8_t *)&storedData;
    *(data + (*dataLen)++) = *tempData;
    *(data + (*dataLen)++) = *(tempData + 1);
    *(data + (*dataLen)++) = *(tempData + 2);
    *(data + (*dataLen)++) = *(tempData + 3);

#ifdef DEBUG_MSG
    // sprintf(tempStr, "Temperature Offset: %0.2f\n", storedData);
    sprintf(tempStr, "Temperature Offset: %d\n", (int)(storedData * TEMP_DIVISIOR));
    rtc_prod_sendDataToPC(tempStr, COMPUTER_PORT, strlen(tempStr));
#endif

    return RTC_CALIB_OK;
}

/**
 * This function gets the PPM Offset data
 */
e_jigErrorCodes rtc_prod_getPppmData(char *data, uint8_t *dataLen)
{
    float storedData = 0;
    uint8_t *tempData;
    // Read PPM OFFSET data set into meter
    if (meter_comm_getInstantParam(PPM_OFFSET, (uint8_t *)&storedData, sizeof(float),NORM_RESP) == METER_RESPONSE_TIMEOUT)
        return RTC_READ_ERROR;
    // Copy data
    tempData = (uint8_t *)&storedData;
    *(data + (*dataLen)++) = *tempData;
    *(data + (*dataLen)++) = *(tempData + 1);
    *(data + (*dataLen)++) = *(tempData + 2);
    *(data + (*dataLen)++) = *(tempData + 3);
    return RTC_READ_OK;
}

/**
 * This function gets the Temperature Offset data
 */
e_jigErrorCodes rtc_prod_getTempData(char *data, uint8_t *dataLen)
{
    float storedData = 0;
    uint8_t *tempData;
    // Read PPM OFFSET data set into meter
    if (meter_comm_getInstantParam(TEMPERATURE_OFFSET, (uint8_t *)&storedData, sizeof(float),NORM_RESP) != METER_RESPONSE_OK)
        return RTC_READ_ERROR;
    // Copy data
    tempData = (uint8_t *)&storedData;
    *(data + (*dataLen)++) = *tempData;
    *(data + (*dataLen)++) = *(tempData + 1);
    *(data + (*dataLen)++) = *(tempData + 2);
    *(data + (*dataLen)++) = *(tempData + 3);
    return RTC_READ_OK;
}

/**
 * e_jigErrorCodes rtc_prod_getRtcParameters(char *data, uint8_t *dataLen);
 * This function does the following:
 * 1. send command to start 1-Hz signal when meter is doing compensation.
 * 2. Get effective compensated PPM data from RTC board.
 * 3. Read back 5 different parameters from meter under test.
 */
e_jigErrorCodes rtc_prod_getAvgPPM(char *data, uint8_t *dataLen)
{
    float storedData = 0xffffffff;
    float rtcCalibParam[RTC_CALIB_DATA_LEN];
    uint8_t *tempData, loopCounter = 0;
    int wTimeout1 = 0;
    uint8_t tempDataLen = 0;
    if (0 == SW_IN_12_9600_BAUD)
    {
        R_UART2_Stop();
        R_UART3_Stop();
        R_SAU1_Create(9600);
        R_UART3_Start();
    }
    // Read back average PPM
    while (1)
    {
        rtc_prod_setPpmTempData(RTC_PPM_TEMP_PORT, 25);
        // check data validity
        if ((g_avgPPM == -10000) || (g_avgPPM == 0))
        {
#ifdef DEBUG_MSG
            rtc_prod_sendDataToPC("Checking Data Again...\n", COMPUTER_PORT, 20);
#endif
            wTimeout1++;
            if (wTimeout1 > 3)
            {
                storedData = ((float)100);
                tempData = (uint8_t *)&storedData;
                *(data + (*dataLen)++) = *tempData;
                *(data + (*dataLen)++) = *(tempData + 1);
                *(data + (*dataLen)++) = *(tempData + 2);
                *(data + (*dataLen)++) = *(tempData + 3);
                if (0 == SW_IN_12_9600_BAUD)
                {
                    R_UART2_Stop();
                    R_UART3_Stop();
                    R_SAU1_Create(19200);
                    R_UART2_Start();
                    R_UART3_Start();
                }
                return RTC_GET_DATA_ERROR; // Check for 5 sec for correct data
            }
            delay_ms(1000);
            continue;
        }

        break;
    }

    // Copy Avg PPM data
    storedData = ((float)g_avgPPM) / PPM_DIVISIOR;
    tempData = (uint8_t *)&storedData;
    *(data + (*dataLen)++) = *tempData;
    *(data + (*dataLen)++) = *(tempData + 1);
    *(data + (*dataLen)++) = *(tempData + 2);
    *(data + (*dataLen)++) = *(tempData + 3);
    if (0 == SW_IN_12_9600_BAUD)
    {
        R_UART2_Stop();
        R_UART3_Stop();
        R_SAU1_Create(19200);
        R_UART2_Start();
        R_UART3_Start();
    }
    return RTC_READ_OK;
}

e_jigErrorCodes rtc_prod_getRtcParameters(char *data, uint8_t *dataLen)
{
    float storedData = 0;
    float rtcCalibParam[RTC_CALIB_DATA_LEN];
    uint8_t *tempData, loopCounter = 0;
    int wTimeout1 = 0;
    uint8_t tempDataLen = 0;

    // Read back average PPM
    while (1)
    {
        rtc_prod_setPpmTempData(RTC_PPM_TEMP_PORT, 25);
        // check data validity
        if ((g_avgPPM == -10000) || (g_avgPPM == 0))
        {
#ifdef DEBUG_MSG
            rtc_prod_sendDataToPC("Checking Data Again...\n", COMPUTER_PORT, 20);
#endif
            wTimeout1++;
            if (wTimeout1 > 5)
                return RTC_GET_DATA_ERROR; // Check for 5 sec for correct data
            delay_ms(1000);
            continue;
        }

        break;
    }

    // Copy Avg PPM data
    storedData = ((float)g_avgPPM) / PPM_DIVISIOR;
    tempData = (uint8_t *)&storedData;
    *(data + (*dataLen)++) = *tempData;
    *(data + (*dataLen)++) = *(tempData + 1);
    *(data + (*dataLen)++) = *(tempData + 2);
    *(data + (*dataLen)++) = *(tempData + 3);

    // Read back average PPM
    wTimeout1 = 0;
    tempDataLen = *dataLen;
    while (1)
    {
        // Read all RTC Calibration Parameters
        if (meter_comm_getInstantParam(GET_RTC_N_TEMP_COEFF, (uint8_t *)rtcCalibParam, RTC_CALIB_DATA_LEN * sizeof(float),NORM_RESP) != METER_RESPONSE_OK)
            return RTC_READ_ERROR;
        tempData = (uint8_t *)rtcCalibParam;
        for (loopCounter = 0; loopCounter < RTC_CALIB_DATA_LEN * sizeof(float); loopCounter++)
        {
            *(data + (*dataLen)++) = *(tempData + loopCounter);
        }
        // check of computed PPM and meter temperature
        if (((int)rtcCalibParam[3] == 0) || ((int)rtcCalibParam[4] == 0))
        {
            wTimeout1++;
            *dataLen = tempDataLen;
            if (wTimeout1 > 5)
                return RTC_READ_ERROR; // Check for 5 sec for correct data
            delay_ms(1000);
            continue;
        }
        break;
    }
    return RTC_READ_OK;
}

void rtc_prod_sendDataToPC(char *str, int portNo, uint8_t len)
{
    if (portNo == COMPUTER_PORT)
    {
        // This is demo function to send the data to PC
        uint8_t byLength = len;
        memcpy(&stComm[portNo].byWrBuf[0], str, byLength);
        stComm[portNo].bySndLen = byLength;
        stComm[portNo].bySndPtr = 0;
        // stComm[1].byWrPtr = 0;

        Port1TxIntEnable();

        while ((stComm[portNo].bySndPtr < stComm[portNo].bySndLen))
        {
            delay_ms(100);
        }
    }
}
