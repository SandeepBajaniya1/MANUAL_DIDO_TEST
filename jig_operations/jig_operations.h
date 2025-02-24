#ifndef __JIG_OPERATIONS_H__
#define __JIG_OPERATIONS_H__

#include "r_cg_macrodriver.h"
#include "SerialData.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_serial.h"
#include "r_cg_adc.h"
#include "r_cg_it.h"
#include <string.h>
#include <stdio.h>
#include "r_cg_userdefine.h"

#include "rtc_prod.h"

#include "error_codes.h"

// #include "wrp_user_hardware.h"

/**-----------MACROS-----------*/
#define METER_RESPONSE_SIZE 100

// for data mem write and read
#define EPR_WRITE_PROTECT_DISABLE_STATEMENT \
    {                                       \
        ;                                   \
    } //{SET_BIT(P6,2,1);}      /* Command or interface to disable write protection, IICA board on EVB not have WP */
#define EPR_WRITE_PROTECT_ENABLE_STATEMENT \
    {                                      \
        ;                                  \
    } //{SET_BIT(P6,2,0);}      /* Command or interface to enable write protection, IICA board on EVB not have WP */

#define DATA_FLASH_START_ADDRESS 0x000F1000
#define DATA_FLASH_END_ADDRESS 0x000F2000

#define JIG_SERIAL_NUMBER_ADDRESS DATA_FLASH_START_ADDRESS
#define JIG_SERIAL_NUMBER_LENGTH 4
#define JIG_MANUFACTURING_DATE_ADDRESS JIG_SERIAL_NUMBER_ADDRESS + JIG_SERIAL_NUMBER_LENGTH
#define JIG_MANUFACTURING_DATE_LENGTH 8

/**------------ENUMS------------*/
typedef enum
{
    NO_COMMAND = 0x00,
    IS_JIG_READY = 0x01,
    GET_SWITCH_STATUS = 0x02,
    GET_3P_JIG_FIRM_VER = 0x03,
    CHECK_METER_COMM = 0x04, // To check meter optical communication

    SET_JIG_SERIAL_NUMBER = 0x05,
    GET_JIG_SERIAL_NUMBER = 0x06,
    GET_JIG_MANUFACTURING_DATE = 0X07,

    GET_METER_INT_FIRM_VERSION = 0x10,
    GET_METER_BOARD_NUMBER = 0x11,

    GET_METER_PCB_BAT_VTG = 0x20,
    GET_MAIN_BAT_SLEEP_CURRENT = 0x21,
    GET_METER_PCB_MAINS_DC_VTG = 0x22,
    GET_METER_PCB_SUPERCAP_VTG = 0x23,

    GET_STATUS_METER_I2C_CHECK = 0x30, // MRY test
    SET_METER_DATE_TIME = 0x31,
    GET_STATUS_METER_CLEAR1 = 0x32,
    GET_STATUS_METER_RESET = 0x33,
    GET_STATUS_RELAY_TEST = 0x34, // Not use din LTCT
    GET_STATUS_VTG_AND_CURRENT = 0x35,
    GET_STATUS_KWH = 0x36,
    GET_STATUS_KVARH = 0x37,
    GET_STATUS_RF = 0x38,
    GET_STATUS_METER_SWITCHES = 0x39,
    GET_STATUS_DIGITAL_IO=0x40,
    GET_RTC_PARAMETERS =0x41,
    GET_MAGNET_SENSOR_DATA =0x42,
    SET_STATUS_ALL_PASS_LED= 0x43,
    
    START_RTC_CALIB = 0x52,
    GET_PPM_OFFSET,
    GET_TEMP_OFFSET,
    START_1HZ,
    STOP_1HZ,
    
    GET_STATUS_METER_CLEAR2 = 0x60,
    GET_STATUS_METER_SPI_CHECK,
    GET_STATUS_METER_RELAY_CONNECT,
    GET_STATUS_METER_RELAY_DISCONNECT,
    GET_STATUS_METER_RFIO_LOW,
    GET_STATUS_METER_RFIO_HIGH,

} e_jigOperationCommands;

/**------Global Variables-------*/
extern char g_data[];
extern uint8_t g_dataLen;
extern e_jigErrorCodes g_ret;
extern e_jigErrorCodes gRTC_ret;
extern uint8_t g_pcb_bat_status;
extern uint8_t g_pcb_mainDcVtg_status;
extern uint8_t g_pcb_sleepCurrent_status;
extern uint8_t g_pcb_superCapVtg_status;
extern uint8_t g_KWH_status;
extern uint8_t g_kVarh_status;

/**-----------Functions----------*/
// e_jigErrorCodes jig_operations_executeCommand(t_rxFrameInfo *rxPcFrame, t_txFrameInfo *txPcFrame);

// void jig_operations_executeCommand(t_frameInfo *rxPcFrame, t_txFrameInfo *txPcFrame);
// void jig_operations_executeCommand(t_frameInfo *rxPcFrame, t_txFrameInfo *txPcFrame);
// void jig_operations_executeCommand(void);

e_jigErrorCodes read_data_flash(char *readData, uint8_t *dataLength, uint16_t get_dataStarting_Address, uint16_t get_dataAddress_length);
e_jigErrorCodes write_data_flash(char *writedata, uint8_t *dataLength, uint16_t set_dataStarting_Address, uint16_t set_dataAddress_length);

#endif