#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_serial.h"
#include "r_cg_adc.h"
#include "r_cg_it.h"
#include <string.h>
#include <stdio.h>
#include "aes.h"
#include "r_cg_userdefine.h"
#include "rtc_prod.h"

const BYTE byAuthKeyOld[] = "r_meter_access_r";
const BYTE byAuthKeyUMD[] = "k_meter_umd_auth";
// BYTE byAuthCmd[] = {0x02, 0x01, 0x24, 0x00, 0xAC, 0xC5, 0x06, 0x9B, 0x5D, 0xDA, 0x84, 0x32, 0x22, 0xD4, 0x08, 0xFF, 0x13, 0x81, 0xF2, 0xD5, 0xC9, 0xCA, 0x9A, 0xF7, 0x6D, 0x9D, 0xF0, 0x45, 0x66, 0xA8, 0x28, 0xEA, 0x37, 0xF0, 0x9C, 0x44, 0xC2, 0x43, 0x03};
BYTE byAuthCmd[] = {0x02, 0x01, 0x24, 0x00, 0x24, 0x95, 0x8B, 0x9C, 0xF4, 0x50, 0x56, 0x94, 0x5F, 0x71, 0xA8, 0x95, 0x60, 0xAD, 0x28, 0xCB, 0xB0, 0xD3, 0xE5, 0x77, 0x92, 0x49, 0x83, 0xB4, 0x6C, 0x7B, 0x6C, 0xAA, 0xEE, 0x71, 0x9C, 0xD1, 0x0F, 0x17, 0x03};

BYTE byAuthResp[] = {0x02, 0x03, 0x06, 0x00, 0x00, 0x01, 0x88, 0xA8, 0x03}; // Authentication Response
BYTE byRspOK[] = {0x02, 0x03, 0x06, 0x03, 0x00, 0x01, 0x67, 0xCC, 0x03};
BYTE bySetInstanceParam_resOK[] = {0x02, 0x03, 0x06, 0x05, 0x00, 0x01, 0xB1, 0x15, 0x03};
BYTE byCmdDfFrmt[] = {0x02, 0x01, 0x06, 0x03, 0x02, 0xFF, 0x5C, 0x05, 0x03};
BYTE byCmdI2CChk[] = {0x02, 0x01, 0x06, 0x03, 0x08, 0x05, 0xF9, 0xA0, 0x03};
BYTE byCmdSPIChk[] = {0x02, 0x01, 0x06, 0x03, 0x08, 0x06, 0xCB, 0x3B, 0x03};
BYTE byCmdConnect[] = {0x02, 0x01, 0x06, 0x03, 0x08, 0x07, 0xDA, 0xB2, 0x03};
BYTE byCmdDisConnect[] = {0x02, 0x01, 0x06, 0x03, 0x08, 0x08, 0x22, 0x45, 0x03};
BYTE byCmdRFIOLow[] = {0x02, 0x01, 0x06, 0x03, 0x08, 0x09, 0x33, 0xCC, 0x03};
BYTE byCmdRFIOHigh[] = {0x02, 0x01, 0x06, 0x03, 0x08, 0x0A, 0x01, 0x57, 0x03};

/**
 * function: void loadAuthCommand(void)
 */
void loadAuthCommand(void)
{
    BYTE byLength = 0;
    WORD wCRC;
    byAuthCmd[byLength++] = 0x02;
    byAuthCmd[byLength++] = 0x01;
    byAuthCmd[byLength++] = 0x24;
    byAuthCmd[byLength++] = 0x00;

    // if(0 != SW_IN_12_UMD_OLD)       //UMD
    //{
    AES128_ECB_encrypt((BYTE *)"abcdefgh.1234567", byAuthKeyUMD, &byAuthCmd[byLength]);
    byLength += 16;
    AES128_ECB_encrypt((BYTE *)"8.a1@$kimbal.com", byAuthKeyUMD, &byAuthCmd[byLength]);
    byLength += 16;
    //}
    // else
    //{
    //        AES128_ECB_encrypt((BYTE*)"abcdefgh.1234567", byAuthKeyOld, &byAuthCmd[byLength]);
    //        byLength += 16;
    //        AES128_ECB_encrypt((BYTE*)"8.a1@crypowr.com", byAuthKeyOld, &byAuthCmd[byLength]);
    //        byLength += 16;
    //}
    wCRC = Crc_fromBuffer(&byAuthCmd[1], byLength - 1);
    byAuthCmd[byLength++] = (BYTE)wCRC & 0xFF;
    byAuthCmd[byLength++] = (BYTE)(wCRC >> 8) & 0xFF;
    byAuthCmd[byLength++] = 0x03;
}

/**
 * After transmission command to meter, we will wait for meter response. Meter response will be different based on the command.
 * Check Acknowledgement from Meter. This can be used for command execution acknowledgement only. Not for any other specific (data read)
 * purpose.
 */
e_jigErrorCodes meter_comm_checkMeterResponse(uint8_t portNo, int respWaitTime, e_meterResponse respType)
{
    e_jigErrorCodes ret = METER_RESPONSE_OK;
    char rdBuf[250];
    // uint8_t byLength;
    int resTimeOut = 0;

    // wait for meter to respond
    while ((resTimeOut < respWaitTime) && (stComm[portNo].byWrPtr < 9))
    {
        delay_ms(100);
        resTimeOut++;
    }

    if (resTimeOut >= respWaitTime)
    {
        leds_operations_updateLedStatus(OPTICAL_STATUS, TEST_FAIL);
        return METER_RESPONSE_TIMEOUT;
    }

    // Check if proper response is returned or not
    switch (respType)
    {
    case AUTH_RESP:
        if ((0 != memcmp(byAuthResp, &stComm[portNo].byRdBuf[0], 9)))
        {
            leds_operations_updateLedStatus(OPTICAL_STATUS, TEST_FAIL);
            return METER_AUTH_FAILED;
        }
        break;
    case CMD_RESP_OK:
        if ((0 != memcmp(byRspOK, &stComm[portNo].byRdBuf[0], 9)))
        {
            // leds_operations_updateLedStatus(OPTICAL_STATUS, TEST_FAIL);
            return METER_RESPONSE_ERROR;
        }
        break;
    case CMD_SET_INSTANT_PARAM_RESP_OK:
        if ((0 != memcmp(bySetInstanceParam_resOK, &stComm[portNo].byRdBuf[0], 9)))
        {
            // leds_operations_updateLedStatus(OPTICAL_STATUS, TEST_FAIL);
            return METER_RESPONSE_ERROR;
        }
        break;

    case NORM_RESP:
        delay_ms(70);
        ret = verify_meterResponse(portNo);
        break;
    default:
        break;
    }

    leds_operations_updateLedStatus(OPTICAL_STATUS, TEST_PASS);
    return ret;
}

/**
 * This function sends the authentication command
 */
e_jigErrorCodes meter_comm_sendAuth(uint8_t portNo)
{
    // Create authentication command and send the same
    loadAuthCommand();
    memcpy(&stComm[portNo].byWrBuf[0], byAuthCmd, sizeof(byAuthCmd));
    stComm[portNo].bySndLen = sizeof(byAuthCmd);
    stComm[portNo].bySndPtr = 0;
    stComm[portNo].byWrPtr = 0;

    if (portNo == 0)
    {
        Port0TxIntEnable();
    }
    else if (portNo == RF_PORT)
    {
        Port2TxIntEnable();
    }

    return meter_comm_checkMeterResponse(portNo, AUTH_RESP_TIME, AUTH_RESP);
}

static void meter_comm_createMeterData(uint8_t meterParam, uint8_t *data, uint8_t dataLen, uint8_t byPort, uint8_t *length, e_meterDataChoice choice)
{
    static uint8_t loopCounter;

    stComm[byPort].byWrBuf[(*length)++] = 0x00; // Address
    stComm[byPort].byWrBuf[(*length)++] = 0x00;
    stComm[byPort].byWrBuf[(*length)++] = 0x00;
    stComm[byPort].byWrBuf[(*length)++] = meterParam; // to get various data
    stComm[byPort].byWrBuf[(*length)++] = dataLen;    // Data Length (total 20 Bytes of data)
    /*-----Make blank bytes-------*/
    switch (choice)
    {
    case GET_DATA:
        for (loopCounter = 0; loopCounter < dataLen; loopCounter++)
        {
            stComm[byPort].byWrBuf[(*length)++] = 0x00;
        }
        break;

    case SET_DATA:
        for (loopCounter = 0; loopCounter < dataLen; loopCounter++)
        {
            stComm[byPort].byWrBuf[(*length)++] = *(data + loopCounter);
        }
        break;

    default:
        break;
    }
}
static void meter_comm_createMeterInvokeCommandData(uint8_t meterParam, uint8_t byPort, uint8_t *length)
{
    if (meterParam == METER_CMD_MEM_TYPE_EEPROM)
    {
        stComm[byPort].byWrBuf[(*length)++] = meterParam;
        stComm[byPort].byWrBuf[(*length)++] = METER_CMD_MEM_CLEAR_AREA;
    }
    else if (meterParam == METER_CMD_MEM_TYPE_DATAFLASH)
    {
        stComm[byPort].byWrBuf[(*length)++] = meterParam;
        stComm[byPort].byWrBuf[(*length)++] = METER_CMD_MEM_CLEAR_AREA;
    }
    else
    {
        stComm[byPort].byWrBuf[(*length)++] = METER_CMD_MEM_TYPE_INVOKING_ACTION;
        stComm[byPort].byWrBuf[(*length)++] = meterParam;
    }
}

/**
 * Function Name: void meter_comm_setInstantParam(uint8_t meterParam, float data)
 * Description: This function send the data to Meter for setting the value through Instant Parameter function
 */
e_jigErrorCodes meter_comm_setInstantParam(uint8_t meterParam, uint8_t *data, uint8_t dataLen, e_meterResponse respType)
{
    e_jigErrorCodes ret;
    // This function will be sending the data to meter via optical PORT
    uint8_t u8retriesCount = 3;
    uint8_t byLength = 0, byGRindex, *pbyTemp;
    uint16_t wGRTemp;
    uint8_t byPort = OPTICAL_PORT;
    uint8_t *addPPM, *addTemperature;
    uint8_t loopCounter = 0;
    // Create authentication command and send the same

    ret = meter_comm_sendAuth(OPTICAL_PORT); // Send authentication command
    if (ret != METER_RESPONSE_OK)
    {
        return ret;
    }

    // Create Meter Frame
    u8retriesCount = 3;
    stComm[byPort].byWrBuf[byLength++] = 0x02;
    stComm[byPort].byWrBuf[byLength++] = 0x01;
    byLength++;
    stComm[byPort].byWrBuf[byLength++] = METER_CMD_MEMORY_ACCESS_TYPE_INSTANTPARAM;
    stComm[byPort].byWrBuf[byLength++] = 0x00; // Write
    stComm[byPort].byWrBuf[byLength++] = 0x05; // Access Area (for Instant)

    meter_comm_createMeterData(meterParam, data, dataLen, byPort, &byLength, SET_DATA);

    stComm[byPort].byWrBuf[2] = byLength; // frrame length info at 3rd byte of the packet

    // add CRC
    wGRTemp = Crc_fromBuffer(&stComm[byPort].byWrBuf[1], byLength - 1);
    stComm[byPort].byWrBuf[byLength++] = (BYTE)wGRTemp & 0xFF;
    stComm[byPort].byWrBuf[byLength++] = (BYTE)(wGRTemp >> 8) & 0xFF;
    stComm[byPort].byWrBuf[byLength++] = 0x03;

    stComm[byPort].bySndLen = byLength;
    stComm[byPort].bySndPtr = 0;
    stComm[byPort].byWrPtr = 0;
    Port0TxIntEnable(); // start sending data

    ret = meter_comm_checkMeterResponse(OPTICAL_PORT, CMD_RESP_TIME, respType);
    if (ret != METER_RESPONSE_OK)
    {
        return ret;
    }

    return ret;
}

/**
 * Function Name: e_jigErrorCodes meter_comm_getInstantParam(uint8_t meterParam, float *data, uint8_t dataLen)
 * This function gets (read) the stored temperature and PPM offset data
 * dataLen: expected return data length in bytes
 */

e_jigErrorCodes meter_comm_getInstantParam(uint8_t meterParam, uint8_t *data, uint8_t dataLen, e_meterResponse respType)
{
    // This function will be sending the data to meter via optical PORT
    e_jigErrorCodes ret;
    uint8_t u8retriesCount = 3;
    uint8_t byLength = 0, byGRindex, *pbyTemp;
    uint16_t wGRTemp;
    uint8_t byPort = 0, loopCounter = 0;
    uint8_t *buff;
    buff = (uint8_t *)data;

    ret = meter_comm_sendAuth(OPTICAL_PORT); // Send authentication command
    if (ret != METER_RESPONSE_OK)
    {
        return ret;
    }

    // Send Read Command
    u8retriesCount = 3;
    stComm[byPort].byWrBuf[byLength++] = 0x02;
    stComm[byPort].byWrBuf[byLength++] = 0x01;
    byLength++;
    stComm[byPort].byWrBuf[byLength++] = METER_CMD_MEMORY_ACCESS_TYPE_INSTANTPARAM;
    stComm[byPort].byWrBuf[byLength++] = 0x01; // Read
    stComm[byPort].byWrBuf[byLength++] = 0x05; // Access Area (for Instant)

    meter_comm_createMeterData(meterParam, data, dataLen, byPort, &byLength, GET_DATA);

    stComm[byPort].byWrBuf[2] = byLength; // frrame length info at 3rd byte of the packet

    // add CRC
    wGRTemp = Crc_fromBuffer(&stComm[byPort].byWrBuf[1], byLength - 1);
    stComm[byPort].byWrBuf[byLength++] = (BYTE)wGRTemp & 0xFF;
    stComm[byPort].byWrBuf[byLength++] = (BYTE)(wGRTemp >> 8) & 0xFF;
    stComm[byPort].byWrBuf[byLength++] = 0x03;

    stComm[byPort].bySndLen = byLength;
    stComm[byPort].bySndPtr = 0;
    stComm[byPort].byWrPtr = 0; // to capture the response we need to clear previous data
    Port0TxIntEnable();         // start sending data

    ret = meter_comm_checkMeterResponse(byPort, CMD_RESP_TIME, respType);
    if (ret != METER_RESPONSE_OK)
    {
        return ret;
    }
    else
    {
        for (loopCounter = 0; loopCounter < dataLen; loopCounter++)
        {
            *(buff + loopCounter) = stComm[byPort].byRdBuf[6 + loopCounter];
        }
    }

    return ret;
}

//**check the header,footer and crc of recieved data from meter */
e_jigErrorCodes verify_meterResponse(uint8_t port_no)
{
    e_jigErrorCodes ret = METER_RESPONSE_ERROR;
    uint8_t byRx_header = (BYTE)stComm[port_no].byRdBuf[0];
    uint8_t byRx_dataLen = (BYTE)stComm[port_no].byRdBuf[2];
    uint8_t byRx_error = (BYTE)stComm[port_no].byRdBuf[5];
    uint8_t byRx_footer = (BYTE)(stComm[port_no].byRdBuf[(byRx_dataLen+2)]);
    uint16_t wCrc_buff = Crc_fromBuffer(&stComm[port_no].byRdBuf[1], (byRx_dataLen-1));
    uint8_t byMSB_crc = (BYTE)wCrc_buff & 0xFF;
    uint8_t byLSB_crc = (BYTE)(wCrc_buff >> 8) & 0xFF;
    
    if((byRx_header == 0x02) && (byRx_footer == 0x03) && (byRx_error == 0x01) )
    {
      if((stComm[port_no].byRdBuf[byRx_dataLen]==byMSB_crc)  && (stComm[port_no].byRdBuf[(byRx_dataLen+1)]==byLSB_crc))
      {
        ret = METER_RESPONSE_OK;
      }

    }
    return ret;
}


/**
 * This function send the command to meter to perform and hardware action. This happens from the r_meter_cmd_format.c
 * using r_meter_cmd_invoke_action() function
 */
e_jigErrorCodes meter_comm_invokeCommand(uint8_t meterParam, uint8_t byPort)
{ // This function will be sending the data to meter via optical PORT
    e_jigErrorCodes ret;
    uint8_t byLength = 0;
    uint16_t wGRTemp;
    // uint8_t byPort = 0;

    ret = meter_comm_sendAuth(byPort); // Send authentication command
    if (ret != METER_RESPONSE_OK)
    {
        return ret;
    }
    stComm[byPort].byWrBuf[byLength++] = 0x02;
    stComm[byPort].byWrBuf[byLength++] = 0x01;
    byLength++;
    stComm[byPort].byWrBuf[byLength++] = METER_CMD_MEMORY_ACCESS_TYPE_DATAFLASH;

    meter_comm_createMeterInvokeCommandData(meterParam, byPort, &byLength);

    stComm[byPort].byWrBuf[2] = byLength;
    // add CRC
    wGRTemp = Crc_fromBuffer(&stComm[byPort].byWrBuf[1], byLength - 1);
    stComm[byPort].byWrBuf[byLength++] = (BYTE)wGRTemp & 0xFF;
    stComm[byPort].byWrBuf[byLength++] = (BYTE)(wGRTemp >> 8) & 0xFF;
    stComm[byPort].byWrBuf[byLength++] = 0x03;

    stComm[byPort].bySndLen = byLength;
    stComm[byPort].bySndPtr = 0;
    stComm[byPort].byWrPtr = 0; // to capture the response we need to clear previous data
    if (byPort == RF_PORT)
    {
        Port2TxIntEnable();
    }
    else if (byPort == OPTICAL_PORT)
    {
        Port0TxIntEnable(); // start sending data
    }

    if ((ret = meter_comm_checkMeterResponse(byPort, CMD_RESP_TIME, CMD_RESP_OK)) != METER_RESPONSE_OK)
    {
        return ret;
    }

    return METER_RESPONSE_OK;
}


/**
 * These are older function implementation. We may modify it
 *
 * This function sends command (not the instant parameter) to meter. It invokes 'r_meter_cmd_invoke_action' in meter
 */
uint8_t checkCommandResp(uint8_t byPort, uint16_t wRespWaitTime)
{
    uint8_t u8retriesCount = 2;
    loadAuthCommand();
    do
    {
        memcpy(&stComm[byPort].byWrBuf[0], byAuthCmd, sizeof(byAuthCmd));
        stComm[byPort].bySndLen = sizeof(byAuthCmd);
        stComm[byPort].bySndPtr = 0;
        stComm[byPort].byWrPtr = 0;
        if (0 == byPort)
        {
            Port0TxIntEnable();
        }
        else if (2 == byPort)
        {
            Port2TxIntEnable();
        }

        wTimeout = 0;
        while ((wTimeout < wRespWaitTime) && (stComm[byPort].byWrPtr < 9))
        {
            delay_ms(100);
            wTimeout++;
        }
        if ((stComm[byPort].byWrPtr >= 9))
        {
            if ((0 != memcmp(byAuthResp, &stComm[byPort].byRdBuf[0], 9)))
            {
                if (2 == byPort)
                {
                    byTestByte = 2;
                }
                if (0 != u8retriesCount)
                {
                    u8retriesCount--;
                    continue;
                }
                else
                {
                    return FALSE;
                }
            }
            else
            {
                byLedDisplayResult[0] = 0;
                break;
            }
        }
        else
        {
            if (2 == byPort)
            {
                byTestByte = 1;
            }
            if (0 != u8retriesCount)
            {
                u8retriesCount--;
                continue;
            }
            else
            {
                byLedDisplayResult[0] = 1;
                return FALSE;
            }
        }
    } while (u8retriesCount < 250);

    delay_ms(1500);
    u8retriesCount = 2;
    do
    {
        memcpy(&stComm[byPort].byWrBuf[0], byCmdPtr, 9);
        stComm[byPort].bySndLen = 9;
        stComm[byPort].bySndPtr = 0;
        stComm[byPort].byWrPtr = 0;
        if (0 == byPort)
        {
            Port0TxIntEnable();
        }
        else if (2 == byPort)
        {
            Port2TxIntEnable();
        }

        wTimeout = 0;
        while ((wTimeout < wRespWaitTime) && (stComm[byPort].byWrPtr < 9))
        {
            delay_ms(100);
            wTimeout++;
        }
        if ((stComm[byPort].byWrPtr >= 9))
        {
            if ((0 != memcmp(byRspOK, &stComm[byPort].byRdBuf[0], 9)))
            {
                if (2 == byPort)
                {
                    byTestByte = 2;
                }
                if (0 != u8retriesCount)
                {
                    u8retriesCount--;
                    continue;
                }
                else
                {
                    return FALSE;
                }
            }
            else
            {
                byLedDisplayResult[0] = 0;
                return TRUE;
            }
        }
        else
        {
            if (2 == byPort)
            {
                byTestByte = 1;
            }
            if (0 != u8retriesCount)
            {
                u8retriesCount--;
                continue;
            }
            else
            {
                byLedDisplayResult[0] = 1;
                return FALSE;
            }
        }
    } while (u8retriesCount < 250);
}

uint8_t getResposnseFromMeter(uint8_t byPort, uint16_t wRespWaitTime, uint8_t byItem, uint8_t *byRspBuf)
{
    uint8_t u8retriesCount = 2;
    uint8_t byLength = 0, byGRindex, *pbyTemp;
    uint16_t wGRTemp;
    loadAuthCommand();
    do
    {
        memcpy(&stComm[byPort].byWrBuf[0], byAuthCmd, sizeof(byAuthCmd));
        stComm[byPort].bySndLen = sizeof(byAuthCmd);
        stComm[byPort].bySndPtr = 0;
        stComm[byPort].byWrPtr = 0;
        if (0 == byPort)
        {
            Port0TxIntEnable();
        }
        else if (2 == byPort)
        {
            Port2TxIntEnable();
        }

        wTimeout = 0;
        while ((wTimeout < wRespWaitTime) && (stComm[byPort].byWrPtr < 9))
        {
            delay_ms(100);
            wTimeout++;
        }
        if ((stComm[byPort].byWrPtr >= 9))
        {
            if ((0 != memcmp(byAuthResp, &stComm[byPort].byRdBuf[0], 9)))
            {
                if (0 != u8retriesCount)
                {
                    u8retriesCount--;
                    continue;
                }
                else
                {
                    return FALSE;
                }
            }
            else
            {
                byLedDisplayResult[0] = 0;
                break;
            }
        }
        else
        {
            if (0 != u8retriesCount)
            {
                u8retriesCount--;
                continue;
            }
            else
            {
                byLedDisplayResult[0] = 1;
                return FALSE;
            }
        }
    } while (u8retriesCount < 250);

    delay_ms(1000);
    u8retriesCount = 2;
    stComm[byPort].byWrBuf[byLength++] = 0x02;
    stComm[byPort].byWrBuf[byLength++] = 0x01;
    byLength++;
    stComm[byPort].byWrBuf[byLength++] = 0x05;
    // memcpy(&stComm[byPort].byWrBuf[0],byCmdPtr,9);
    // stComm[byPort].bySndLen = 9;
    switch (byItem)
    {
    case CURRENT_PKT:
        stComm[byPort].byWrBuf[byLength++] = 0x01; // Read
        stComm[byPort].byWrBuf[byLength++] = 0x05; // Access Area
        stComm[byPort].byWrBuf[byLength++] = 0x00; // Address
        stComm[byPort].byWrBuf[byLength++] = 0x00;
        stComm[byPort].byWrBuf[byLength++] = 0x00;
        stComm[byPort].byWrBuf[byLength++] = 0x01;
        if (0 != SW_IN_11_1PH_3PH)
        {
            wGRTemp = 12;
        }
        else
        {
            wGRTemp = 28;
        }
        stComm[byPort].byWrBuf[byLength++] = (BYTE)wGRTemp;
        for (byGRindex = 0; byGRindex < wGRTemp; byGRindex++)
        {
            stComm[byPort].byWrBuf[byLength++] = 0x00;
        }
        break;

    case DATE_TIME_PKT:
        stComm[byPort].byWrBuf[byLength++] = 0x01; // Read
        stComm[byPort].byWrBuf[byLength++] = 0x05; // Access Area
        stComm[byPort].byWrBuf[byLength++] = 0x00; // Address MSB first
        stComm[byPort].byWrBuf[byLength++] = 0x00;
        stComm[byPort].byWrBuf[byLength++] = 0x00;
        stComm[byPort].byWrBuf[byLength++] = 0x03;
        stComm[byPort].byWrBuf[byLength++] = 0x09;
        for (byGRindex = 0; byGRindex < 9; byGRindex++)
        {
            stComm[byPort].byWrBuf[byLength++] = 0x00;
        }
        break;

    case SET_DATE_TIME_PKT:
        stComm[byPort].byWrBuf[byLength++] = 0x00; // Read
        stComm[byPort].byWrBuf[byLength++] = 0x05; // Access Area
        stComm[byPort].byWrBuf[byLength++] = 0x00; // Address MSB first
        stComm[byPort].byWrBuf[byLength++] = 0x00;
        stComm[byPort].byWrBuf[byLength++] = 0x00;
        stComm[byPort].byWrBuf[byLength++] = 0x03;
        stComm[byPort].byWrBuf[byLength++] = 0x08;
        for (byGRindex = 0; byGRindex < 8; byGRindex++)
        {
            stComm[byPort].byWrBuf[byLength++] = byRespBuf[byGRindex];
        }
        break;

    case SERIAL_ID_PKT:
        stComm[byPort].byWrBuf[byLength++] = 0x01; // Read
        stComm[byPort].byWrBuf[byLength++] = 0x05; // Access Area
        stComm[byPort].byWrBuf[byLength++] = 0x00; // Address
        stComm[byPort].byWrBuf[byLength++] = 0x00;
        stComm[byPort].byWrBuf[byLength++] = 0x00;
        stComm[byPort].byWrBuf[byLength++] = 0x00;
        stComm[byPort].byWrBuf[byLength++] = 0x04;
        for (byGRindex = 0; byGRindex < 4; byGRindex++)
        {
            stComm[byPort].byWrBuf[byLength++] = 0x00;
        }
        break;

    case SET_SERIAL_ID_PKT:
        stComm[byPort].byWrBuf[byLength++] = 0x00; // Write
        stComm[byPort].byWrBuf[byLength++] = 0x05; // Access Area
        stComm[byPort].byWrBuf[byLength++] = 0x00; // Address
        stComm[byPort].byWrBuf[byLength++] = 0x00;
        stComm[byPort].byWrBuf[byLength++] = 0x00;
        stComm[byPort].byWrBuf[byLength++] = 0x00;
        stComm[byPort].byWrBuf[byLength++] = 0x04;
        pbyTemp = (uint8_t *)&dMtrSerial;
        for (byGRindex = 0; byGRindex < 4; byGRindex++)
        {
            stComm[byPort].byWrBuf[byLength++] = byRespBuf[byGRindex]; // pbyTemp[3 - byGRindex];
            *pbyTemp++ = byRespBuf[byGRindex];
        }
        break;

    case BOARD_ID_PKT:
        stComm[byPort].byWrBuf[byLength++] = 0x01; // Read
        stComm[byPort].byWrBuf[byLength++] = 0x05; // Access Area
        stComm[byPort].byWrBuf[byLength++] = 0x00; // Address
        stComm[byPort].byWrBuf[byLength++] = 0x00;
        stComm[byPort].byWrBuf[byLength++] = 0x00;
        stComm[byPort].byWrBuf[byLength++] = 0x0B;
        stComm[byPort].byWrBuf[byLength++] = 0x08;
        for (byGRindex = 0; byGRindex < 8; byGRindex++)
        {
            stComm[byPort].byWrBuf[byLength++] = 0x00;
        }
        break;

    case SET_BOARD_ID_PKT:
        stComm[byPort].byWrBuf[byLength++] = 0x00; // Write
        stComm[byPort].byWrBuf[byLength++] = 0x05; // Access Area
        stComm[byPort].byWrBuf[byLength++] = 0x00; // Address
        stComm[byPort].byWrBuf[byLength++] = 0x00;
        stComm[byPort].byWrBuf[byLength++] = 0x00;
        stComm[byPort].byWrBuf[byLength++] = 0x0B;
        stComm[byPort].byWrBuf[byLength++] = 0x08;
        pbyTemp = (uint8_t *)&dMtrSerial;
        for (byGRindex = 0; byGRindex < 8; byGRindex++)
        {
            stComm[byPort].byWrBuf[byLength++] = byRespBuf[byGRindex]; // pbyTemp[3 - byGRindex];
            *pbyTemp++ = byRespBuf[byGRindex];
        }
        break;

    case SINGLE_WIRE_OFFSET_PKT:
        stComm[byPort].byWrBuf[byLength++] = 0x01; // Read
        stComm[byPort].byWrBuf[byLength++] = 0x05; // Access Area
        stComm[byPort].byWrBuf[byLength++] = 0x00; // Address
        stComm[byPort].byWrBuf[byLength++] = 0x00;
        stComm[byPort].byWrBuf[byLength++] = 0x00;
        stComm[byPort].byWrBuf[byLength++] = 0x1F;
        stComm[byPort].byWrBuf[byLength++] = 0x01;
        stComm[byPort].byWrBuf[byLength++] = 0x00;
        break;

    case SET_SINGLE_WIRE_OFFSET_PKT:
        stComm[byPort].byWrBuf[byLength++] = 0x00; // Write
        stComm[byPort].byWrBuf[byLength++] = 0x05; // Access Area
        stComm[byPort].byWrBuf[byLength++] = 0x00; // Address
        stComm[byPort].byWrBuf[byLength++] = 0x00;
        stComm[byPort].byWrBuf[byLength++] = 0x00;
        stComm[byPort].byWrBuf[byLength++] = 0x1F;
        stComm[byPort].byWrBuf[byLength++] = 0x01;
        stComm[byPort].byWrBuf[byLength++] = 0x00;
        break;

    case SWITCH_CONNECT_CHK_PKT:
        stComm[byPort].byWrBuf[byLength++] = 0x01; // Read
        stComm[byPort].byWrBuf[byLength++] = 0x05; // Access Area
        stComm[byPort].byWrBuf[byLength++] = 0x00; // Address
        stComm[byPort].byWrBuf[byLength++] = 0x00;
        stComm[byPort].byWrBuf[byLength++] = 0x00;
        stComm[byPort].byWrBuf[byLength++] = 0x13;
        stComm[byPort].byWrBuf[byLength++] = 0x01;
        stComm[byPort].byWrBuf[byLength++] = 0x00;
        break;

    case SWITCH_RELEASE_CHK_PKT:
        stComm[byPort].byWrBuf[byLength++] = 0x01; // Read
        stComm[byPort].byWrBuf[byLength++] = 0x05; // Access Area
        stComm[byPort].byWrBuf[byLength++] = 0x00; // Address
        stComm[byPort].byWrBuf[byLength++] = 0x00;
        stComm[byPort].byWrBuf[byLength++] = 0x00;
        stComm[byPort].byWrBuf[byLength++] = 0x14;
        stComm[byPort].byWrBuf[byLength++] = 0x01;
        stComm[byPort].byWrBuf[byLength++] = 0x00;
        break;

    case VERSION_PKT:
        stComm[byPort].byWrBuf[byLength++] = 0x01; // Read
        stComm[byPort].byWrBuf[byLength++] = 0x05; // Access Area
        stComm[byPort].byWrBuf[byLength++] = 0x00; // Address
        stComm[byPort].byWrBuf[byLength++] = 0x00;
        stComm[byPort].byWrBuf[byLength++] = 0x00;
        stComm[byPort].byWrBuf[byLength++] = 0x04;
        stComm[byPort].byWrBuf[byLength++] = 0x0A;
        for (byGRindex = 0; byGRindex < 10; byGRindex++)
        {
            stComm[byPort].byWrBuf[byLength++] = 0x00;
        }
        break;

    case GET_INT_FIRM_VERSION:
        stComm[byPort].byWrBuf[byLength++] = 0x01; // Read
        stComm[byPort].byWrBuf[byLength++] = 0x05; // Access Area
        stComm[byPort].byWrBuf[byLength++] = 0x00; // Address
        stComm[byPort].byWrBuf[byLength++] = 0x00;
        stComm[byPort].byWrBuf[byLength++] = 0x00;
        stComm[byPort].byWrBuf[byLength++] = 0x26;
        stComm[byPort].byWrBuf[byLength++] = 0x0B;
        for (byGRindex = 0; byGRindex < 11; byGRindex++)
        {
            stComm[byPort].byWrBuf[byLength++] = 0x00;
        }
        break;

    default:
        break;
    }

    stComm[byPort].byWrBuf[2] = byLength;
    wGRTemp = Crc_fromBuffer(&stComm[byPort].byWrBuf[1], byLength - 1);
    stComm[byPort].byWrBuf[byLength++] = (BYTE)wGRTemp & 0xFF;
    stComm[byPort].byWrBuf[byLength++] = (BYTE)(wGRTemp >> 8) & 0xFF;
    stComm[byPort].byWrBuf[byLength++] = 0x03;
    wGRTemp = byLength;

    do
    {
        byLength = (uint8_t)wGRTemp;
        stComm[byPort].bySndLen = byLength;
        stComm[byPort].bySndPtr = 0;
        stComm[byPort].byWrPtr = 0;
        // byLength -= 14;
        if (0 == byPort)
        {
            Port0TxIntEnable();
        }
        else if (2 == byPort)
        {
            Port2TxIntEnable();
        }

        byLength -= 5;
        if ((byItem == SET_SERIAL_ID_PKT) || (byItem == SET_DATE_TIME_PKT) || (byItem == SET_BOARD_ID_PKT))
        {
            byLength = 9;
        }
        wTimeout = 0;
        while ((wTimeout < wRespWaitTime) && (stComm[byPort].byWrPtr < byLength))
        {
            delay_ms(100);
            wTimeout++;
        }
        if ((stComm[byPort].byWrPtr >= byLength))
        {
            for (byGRindex = 0; byGRindex < byLength; byGRindex++)
            {
                byRspBuf[byGRindex] = stComm[byPort].byRdBuf[byGRindex];
            }
            if ((2 == stComm[byPort].byRdBuf[0]) && (3 == stComm[byPort].byRdBuf[1]) && (3 == stComm[byPort].byRdBuf[byLength - 1]))
            { // Valid response received
                return TRUE;
            }
            else
            {
                if (0 != u8retriesCount)
                {
                    u8retriesCount--;
                    continue;
                }
                else
                {
                    return FALSE;
                }
            }
        }
        else
        {
            if (0 != u8retriesCount)
            {
                u8retriesCount--;
                continue;
            }
            else
            {
                byLedDisplayResult[0] = 1;
                return FALSE;
            }
        }
    } while (u8retriesCount < 250);
    return TRUE;
}
