#ifndef __METER_COMM_H__
#define __METER_COMM_H__

#include "string.h"
#include <stdio.h>
#include "r_main.h"
#include "r_cg_macrodriver.h"
#include "r_cg_port.h"
#include "r_cg_serial.h"

#include "SerialData.h"

#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_serial.h"
#include "r_cg_adc.h"
#include "r_cg_it.h"
#include "r_cg_userdefine.h"
#include "error_codes.h"
#include "leds_operations.h"

/*-------MACROS--------*/
#define AUTH_SIZE 39
#define AUTH_RESP_TIME 50 // This number will be multiplied by 100
#define CMD_RESP_TIME 50

/*---------ENUM-----------*/

/**
 * This contains the same addresses as in the meter
 */
typedef enum
{
    RMS_Volt_Current = 1,
    SET_GET_DATE_TIME = 3,
    Board_Number = 11,
    Digital_IO = 17,
    HW_Switch_Connect_Chk = 19,
    HW_Switch_Release_Chk = 20,
    
    RTC_Comp_Pulse_start = 33,
    RTC_Comp_Pulse_stop,
    METER_GET_INT_FIRM_VERSION = 38,

    RTC_CALIBRATE = 100,
    TEMPERATURE_OFFSET,
    PPM_OFFSET,
    GET_RTC_N_TEMP_COEFF,
    GET_CURRENT_PPM,
    MAGNET_XYZ_DATA,
} e_meterSetGetInstantParameters;

/*This code use in meter code to process all type CMDs
 */
typedef enum
{
    METER_CMD_CODE_AUTHENTICATION = 0,
    METER_CMD_CODE_CALIBRATION,
    METER_CMD_CODE_MEMORY_FORMAT = 0x03,
    METER_CMD_CODE_SIGLOG,
    METER_CMD_CODE_OCMEM_ACCESS,
    METER_CMD_CODE_PLATFORM_ACCESS,
    METER_CMD_CODE_UNLOCK_ACCESS,
} e_MeterCommandCode;

/**
 * This enum is for meter memory access type
 */

typedef enum
{
    METER_CMD_MEMORY_ACCESS_TYPE_ONCHIP = 0x01,
    METER_CMD_MEMORY_ACCESS_TYPE_EEPROM = 0x02,
    METER_CMD_MEMORY_ACCESS_TYPE_DATAFLASH = 0x03,
    METER_CMD_MEMORY_ACCESS_TYPE_SERIALFLASH = 0x04,
    METER_CMD_MEMORY_ACCESS_TYPE_INSTANTPARAM = 0x05,
} e_memoryAccessType;
/**
 * This is taken from meter code. Function: r_meter_cmd_format
 */
typedef enum
{
    METER_CMD_MEM_TYPE_EEPROM = 0x01,    // clear2
    METER_CMD_MEM_TYPE_DATAFLASH = 0x02, // clear1
    METER_CMD_MEM_TYPE_INVOKING_ACTION = 0X08,
} e_meterCommandMemoryType;

/**
 * This is taken from meter code. Function: r_meter_cmd_format.c
 */
typedef enum
{
    METER_CMD_FORMAT_ACTION_IMAGE_BACKUP = 0x01,
    METER_CMD_FORMAT_ACTION_MEM_TEST_I2C = 0x05,
    METER_CMD_FORMAT_ACTION_MEM_TEST_SPI = 0x06,
    METER_CMD_FORMAT_ACTION_RELAY_CONNECT = 0x07,
    METER_CMD_FORMAT_ACTION_RELAY_DISCONNECT = 0x08,
    METER_CMD_FORMAT_ACTION_RF_IO_LOW_CHECK = 0x09,
    METER_CMD_FORMAT_ACTION_RF_IO_HIGH_CHECK = 0x0A,
    METER_CMD_FORMAT_ACTION_RF_IO_LOW_CHECK_ONBOARD = 0x0B,
    METER_CMD_FORMAT_ACTION_RF_IO_HIGH_CHECK_ONBOARD = 0x0C,
    METER_CMD_MEM_CLEAR_AREA = 0xFF,
} e_meterCommandInvokeActions;

typedef enum
{
    AUTH_RESP = 0x00,
    NORM_RESP,
    CMD_RESP_OK,
    CMD_SET_INSTANT_PARAM_RESP_OK,
} e_meterResponse;

typedef enum
{
    GET_DATA = 0x00,
    SET_DATA = 0x01,
} e_meterDataChoice;

/*---------Global Variables--------*/
extern BYTE byAuthCmd[AUTH_SIZE];
extern BYTE byCmdConnect[];
extern BYTE byCmdDisConnect[];
extern BYTE byCmdDfFrmt[];
extern BYTE byCmdI2CChk[];
extern BYTE byCmdSPIChk[];
extern BYTE byCmdRFIOLow[];
extern BYTE byCmdRFIOHigh[];
extern struct t_rxFrameInfo;

/*---------Functions---------------*/
void loadAuthCommand(void);
e_jigErrorCodes meter_comm_sendAuth(uint8_t portNo);
e_jigErrorCodes meter_comm_checkMeterResponse(uint8_t portNo, int wRespWaitTime, e_meterResponse);
e_jigErrorCodes meter_comm_setInstantParam(uint8_t meterParam, uint8_t *data, uint8_t dataLen, e_meterResponse respType);
e_jigErrorCodes meter_comm_getInstantParam(uint8_t rtcParam, uint8_t *data, uint8_t dataLen, e_meterResponse respType);
e_jigErrorCodes meter_comm_invokeCommand(uint8_t meterParam, uint8_t byport);
e_jigErrorCodes verify_meterResponse(uint8_t port);

uint8_t checkCommandResp(uint8_t byPort, uint16_t wRespWaitTime);
uint8_t getResposnseFromMeter(uint8_t byPort, uint16_t wRespWaitTime, uint8_t byItem, uint8_t *byRspBuf);
// e_jigErrorCodes meter_comm_getMeterData(char *data, uint8_t *dataLen, e_jigOperationCommands meterOperation);

#endif