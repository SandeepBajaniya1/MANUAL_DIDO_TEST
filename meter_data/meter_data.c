#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_serial.h"
#include "r_cg_adc.h"
#include "r_cg_it.h"
#include <string.h>
#include <stdio.h>
#include "r_cg_userdefine.h"
#include "meter_data.h"

/**
 * Name: e_jigErrorCodes meter_data_getMagnetSensorData(char *, uint8_t *)
 *
 */
// e_jigErrorCodes meter_data_getMagnetSensorData(char *data, uint8_t *dataLen)
//{
//     float magnetSensorData[MAGNET_SENSOR_DATA_LEN];
//     uint8_t *tempData, loopCounter = 0;
//
//     // Read Magnet Sensor Data from Meter
//     if (meter_comm_getInstantParam(MAGNET_XYZ_DATA, (uint8_t *)magnetSensorData, MAGNET_SENSOR_DATA_LEN * sizeof(float),NORM_RESP) == METER_RESPONSE_TIMEOUT)
//         return MAGNET_READ_ERROR;
//     tempData = (uint8_t *)magnetSensorData;
//     for (loopCounter = 0; loopCounter < MAGNET_SENSOR_DATA_LEN * sizeof(float); loopCounter++)
//     {
//         *(data + (*dataLen)++) = *(tempData + loopCounter);
//     }
//
//     return JIG_OPER_OK;
// }

// e_jigErrorCodes meter_data_getRtcParam(char *data, uint8_t *dataLen)
// {
//     float rtcParameters[RTC_PARAM_LEN];
//     uint8_t *tempData, loopCounter = 0;

//     // Read RTC parameters from Meter
//     if (meter_comm_getInstantParam(GET_RTC_N_TEMP_COEFF, (uint8_t *)rtcParameters, RTC_PARAM_LEN * sizeof(float)) == METER_RESPONSE_TIMEOUT)
//         return RTC_READ_ERROR;
//     tempData = (uint8_t *)rtcParameters;
//     for (loopCounter = 0; loopCounter < RTC_PARAM_LEN * sizeof(float); loopCounter++)
//     {
//         *(data + (*dataLen)++) = *(tempData + loopCounter);
//     }

//     return RTC_READ_OK;
// }

/**
 * This function sets the date and time in the meter and then reads back the written date and time.
 */
e_jigErrorCodes meter_data_setDateTime(char *data, uint8_t *dataLen)
{
    e_jigErrorCodes ret;
    if (0 != SW_IN_1)
    {
        return REQ_SW_NOT_ON;
    }
    ret = meter_comm_setInstantParam(SET_GET_DATE_TIME, (uint8_t *)data, *dataLen, CMD_SET_INSTANT_PARAM_RESP_OK);
    if (ret != METER_RESPONSE_OK)
    {
        *dataLen = 0;
        leds_operations_updateLedStatus(DATE_TIME, TEST_FAIL);
        return ret;
    }

    delay_ms(1000);
    *dataLen = 0x08;
    memset(data, 0x00, *dataLen);
    ret = meter_comm_getInstantParam(SET_GET_DATE_TIME, (uint8_t *)data, *dataLen, NORM_RESP);
    if (ret != METER_RESPONSE_OK)
    {
        *dataLen = 0;
        leds_operations_updateLedStatus(DATE_TIME, TEST_FAIL);
        return ret;
    }

    leds_operations_updateLedStatus(DATE_TIME, TEST_PASS);
    return JIG_COMMNAD_OK;
}

/**
 * This function returns the meter data based on the condition
 */
e_jigErrorCodes meter_data_getMeterParam(char *data, uint8_t *dataLen, e_jigOperationCommands oper)
{
    e_jigErrorCodes ret = JIG_COMMNAD_OK;
    *dataLen = 0;

    switch (oper)
    {
    case GET_METER_INT_FIRM_VERSION:
        *dataLen = 0x0B;
        ret = meter_comm_getInstantParam(METER_GET_INT_FIRM_VERSION, (uint8_t *)data, *dataLen, NORM_RESP);
        if (ret != METER_RESPONSE_OK)
        {
            *dataLen = 0;
            return ret;
        }
        break;
    case GET_METER_BOARD_NUMBER:
        *dataLen = 0x08;
        if ((ret = meter_comm_getInstantParam(Board_Number, (uint8_t *)data, *dataLen, NORM_RESP)) != METER_RESPONSE_OK)
        {
            *dataLen = 0;
            return ret;
        }
        break;
    case HW_Switch_Connect_Chk:
        *dataLen = 0x01;
        if ((ret = meter_comm_getInstantParam(HW_Switch_Connect_Chk, (uint8_t *)data, *dataLen, NORM_RESP)) != METER_RESPONSE_OK)
        {
            *dataLen = 0;
            return ret;
        }
        break;
    case HW_Switch_Release_Chk:
        *dataLen = 0x01;
        if ((ret = meter_comm_getInstantParam(HW_Switch_Release_Chk, (uint8_t *)data, *dataLen, NORM_RESP)) != METER_RESPONSE_OK)
        {
            *dataLen = 0;
            return ret;
        }
        break;
    case GET_STATUS_VTG_AND_CURRENT:
        *dataLen = 0x1C;
        if ((ret = meter_comm_getInstantParam(RMS_Volt_Current, (uint8_t *)data, *dataLen, NORM_RESP)) != METER_RESPONSE_OK)
        {
            *dataLen = 0;
            return ret;
        }
        break;

    case GET_STATUS_METER_CLEAR1:
        if ((ret = meter_comm_invokeCommand(METER_CMD_MEM_TYPE_DATAFLASH,OPTICAL_PORT)) != METER_RESPONSE_OK)
        {
            return ret;
        }
        break;
    case GET_STATUS_METER_CLEAR2:
        if ((ret = meter_comm_invokeCommand(METER_CMD_MEM_TYPE_EEPROM,OPTICAL_PORT)) != METER_RESPONSE_OK)
        {
            return ret;
        }
        break;
    case GET_STATUS_METER_I2C_CHECK:
        if ((ret = meter_comm_invokeCommand(METER_CMD_FORMAT_ACTION_MEM_TEST_I2C,OPTICAL_PORT)) != METER_RESPONSE_OK)
        {
            return ret;
        }
        break;
    case GET_STATUS_METER_SPI_CHECK:
        if ((ret = meter_comm_invokeCommand(METER_CMD_FORMAT_ACTION_MEM_TEST_SPI,OPTICAL_PORT)) != METER_RESPONSE_OK)
        {
            return ret;
        }
        break;
    case GET_STATUS_METER_RELAY_CONNECT:
        if ((ret = meter_comm_invokeCommand(METER_CMD_FORMAT_ACTION_RELAY_CONNECT,OPTICAL_PORT)) != METER_RESPONSE_OK)
        {
            return ret;
        }
        break;
    case GET_STATUS_METER_RELAY_DISCONNECT:
        if ((ret = meter_comm_invokeCommand(METER_CMD_FORMAT_ACTION_RELAY_DISCONNECT,OPTICAL_PORT)) != METER_RESPONSE_OK)
        {
            return ret;
        }
        break;
    case GET_STATUS_METER_RFIO_LOW:
        if ((ret = meter_comm_invokeCommand(METER_CMD_FORMAT_ACTION_RF_IO_LOW_CHECK,OPTICAL_PORT)) != METER_RESPONSE_OK)
        {
            return ret;
        }
        break;
    case GET_STATUS_METER_RFIO_HIGH:
        if ((ret = meter_comm_invokeCommand(METER_CMD_FORMAT_ACTION_RF_IO_HIGH_CHECK,OPTICAL_PORT)) != METER_RESPONSE_OK)
        {
            return ret;
        }
        break;
    case GET_RTC_PARAMETERS:
        *dataLen = 0x14;
        if ((ret = meter_comm_getInstantParam(GET_RTC_N_TEMP_COEFF, (uint8_t *)data, *dataLen, NORM_RESP)) != METER_RESPONSE_OK)
        {
            *dataLen = 0;
            return ret;
        }
        break;
    case START_1HZ: // set function but used only statrting 1hz.
        *dataLen = 0x01;
        if ((ret = meter_comm_setInstantParam(RTC_Comp_Pulse_start, (uint8_t *)data, *dataLen, CMD_SET_INSTANT_PARAM_RESP_OK)) != METER_RESPONSE_OK)
        {
            *dataLen = 0;
            return ret;
        }
        break;
    case STOP_1HZ:
        *dataLen = 0x01;
        if ((ret = meter_comm_setInstantParam(RTC_Comp_Pulse_stop, (uint8_t *)data, *dataLen, CMD_SET_INSTANT_PARAM_RESP_OK)) != METER_RESPONSE_OK)
        {
            *dataLen = 0;
            return ret;
        }
        break;
    case GET_MAGNET_SENSOR_DATA:
        *dataLen = 0x0c;
        if ((ret = meter_comm_getInstantParam(MAGNET_XYZ_DATA, (uint8_t *)data, *dataLen, NORM_RESP)) != METER_RESPONSE_OK)
        {
            *dataLen = 0;
            return ret;
        }
        break;
    default:
        break;
    }
    return JIG_COMMNAD_OK;
}
