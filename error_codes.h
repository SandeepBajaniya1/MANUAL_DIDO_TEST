/**
 * This file defines the error codes used in the JIG Controller
*/

#ifndef __ERROR_CODES_H__
#define __ERROR_CODES_H__

typedef enum
{
    JIG_OPER_OK = 0x00, // This should be sent as a command response error code if everything goes OK
    JIG_WRONG_COMMAND,
    PC_SEND_OK,
    PC_SEND_ERROR,  // This will come if time out occur while sending the data

    // Following JIG errors are for PC
    JIG_COMMNAD_OK = 0x64,
	JIG_OPER_TIMEOUT,
    METER_AUTH_FAILED,
    METER_RESPONSE_OK,     // meter response perfect
    METER_RESPONSE_ERROR,  // meter response not ok after comparison
    METER_RESPONSE_TIMEOUT,  
    METER_TEST_FAILED,
    METER_KWH_TEST_FAILED,
    METER_KVARH_TEST_FAILED,
    METER_PIN_TEST_FAILED,   //meterpin testing
    FREQUENCY_GENERATION_SUCCESS,
    FREQUENCY_NOT_GENERATED,
    REQ_SW_NOT_ON,  // Required Switch is not connected
    

    FLASH_WRITE_SUCCESS,
    FLASH_WRITE_ERROR,
    
    
    RTC_CALIB_OK = 0xB0, // RTC Calibration related error codes
    RTC_CALIB_ERROR,
    RTC_GET_DATA_ERROR,
    RTC_PPM_ERROR,
    RTC_TEMP_ERROR,
    METER_ACK_OK,
    METER_ACK_ERROR,
    RTC_READ_OK,
    RTC_READ_ERROR,
    MAGNET_READ_ERROR,
}e_jigErrorCodes;

#endif