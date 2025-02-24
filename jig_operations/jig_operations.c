#include "r_cg_macrodriver.h"
#include "r_cg_serial.h"
#include "r_cg_userdefine.h"
#include "rtc_prod.h"
#include "pc_comm.h"
#include "error_codes.h"
#include "jig_operations.h"
#include "meter_data.h"
#include "pcb_tests.h"
#include "functional_tests.h"

char g_data[PC_DATA_MAX_LEN];
uint8_t g_dataLen = 0;
e_jigErrorCodes g_ret = RTC_CALIB_ERROR;

static float UnUsed_value = 4.4; // TEMPORAY DATA FOR TESTING

/**
 * This functions excetes the received command
 */
e_jigErrorCodes jig_operations_executeCommand(t_rxFrameInfo *rxPcFrame, t_txFrameInfo *txPcFrame)
{
    e_jigErrorCodes ret = JIG_COMMNAD_OK;
    char data[PC_DATA_MAX_LEN];
    char data1[] = {0x01, 0x02, 0x03, 0x04}; // TEMPORAY DATA FOR TESTING
    uint8_t dataLen = 0;
    uint8_t *dataPtr;
    char tempStr[30];
    char meterResponseBuf[METER_RESPONSE_SIZE];

    memcpy(data, 0, sizeof(data));

    switch (rxPcFrame->rxCommand)
    {
    case IS_JIG_READY:
        dataLen = 0;
        pc_comm_createResponseFrame(data, dataLen, ret, txPcFrame); // Update response structure
        ret = pc_comm_sendPcResponse(COMPUTER_PORT, txPcFrame);     // Send response to PC
        break;
    case GET_SWITCH_STATUS:
        dataPtr = (uint8_t *)&g_swStatus;
        data[0] = *dataPtr;
        data[1] = *(dataPtr + 1);
        dataLen = 2;
        pc_comm_createResponseFrame(data, dataLen, ret, txPcFrame); // Update response structure
        ret = pc_comm_sendPcResponse(COMPUTER_PORT, txPcFrame);     // Send response to PC
        break;
    case GET_3P_JIG_FIRM_VER:
        dataLen = (uint8_t)strlen(JIG_FIRM_VERSION);
        memcpy(data, JIG_FIRM_VERSION, dataLen);
        pc_comm_createResponseFrame(data, dataLen, ret, txPcFrame); // Update response structure
        ret = pc_comm_sendPcResponse(COMPUTER_PORT, txPcFrame);     // Send response to PC
        break;

    case CHECK_METER_COMM:
        RELAY_POWER_ON();
        delay_ms(8000);
        ret = meter_comm_sendAuth(OPTICAL_PORT);
        if (ret == METER_RESPONSE_OK)
        {
            ret = JIG_COMMNAD_OK;
        }
        pc_comm_createResponseFrame(data, dataLen, ret, txPcFrame); // Update response structure
        ret = pc_comm_sendPcResponse(COMPUTER_PORT, txPcFrame);     // Send response to PC
        break;
    case SET_JIG_SERIAL_NUMBER:
        ret = write_data_flash(data1, &dataLen, (uint16_t)JIG_MANUFACTURING_DATE_ADDRESS, (uint16_t)JIG_SERIAL_NUMBER_LENGTH);
        pc_comm_createResponseFrame(data, dataLen, ret, txPcFrame); // Update response structure
        ret = pc_comm_sendPcResponse(COMPUTER_PORT, txPcFrame);
        break;
    case GET_JIG_SERIAL_NUMBER:
        ret = read_data_flash(data, &dataLen, (uint16_t)JIG_SERIAL_NUMBER_ADDRESS, (uint16_t)JIG_SERIAL_NUMBER_LENGTH);
        pc_comm_createResponseFrame(data, dataLen, ret, txPcFrame); // Update response structure
        ret = pc_comm_sendPcResponse(COMPUTER_PORT, txPcFrame);
        break;
    case GET_JIG_MANUFACTURING_DATE:
        ret = read_data_flash(data, &dataLen, (uint16_t)JIG_MANUFACTURING_DATE_ADDRESS, (uint16_t)JIG_MANUFACTURING_DATE_LENGTH);
        ret = read_data_flash(data, &dataLen, (uint16_t)&UnUsed_value, (uint16_t)JIG_SERIAL_NUMBER_LENGTH);
        pc_comm_createResponseFrame(data, dataLen, ret, txPcFrame); // Update response structure
        ret = pc_comm_sendPcResponse(COMPUTER_PORT, txPcFrame);
        break;

    case GET_METER_INT_FIRM_VERSION:
        ret = meter_data_getMeterParam(data, &dataLen, GET_METER_INT_FIRM_VERSION);
        pc_comm_createResponseFrame(data, dataLen, ret, txPcFrame); // Update response structure
        ret = pc_comm_sendPcResponse(COMPUTER_PORT, txPcFrame);     // Send response to PC
        break;
    case GET_METER_BOARD_NUMBER:
        ret = meter_data_getMeterParam(data, &dataLen, GET_METER_BOARD_NUMBER);
        pc_comm_createResponseFrame(data, dataLen, ret, txPcFrame); // Update response structure
        ret = pc_comm_sendPcResponse(COMPUTER_PORT, txPcFrame);     // Send response to PC
        break;

    case GET_METER_PCB_BAT_VTG:
        ret = pcb_tests_getBatVoltage(data, &dataLen);
        pc_comm_createResponseFrame(data, dataLen, ret, txPcFrame); // Update response structure
        ret = pc_comm_sendPcResponse(COMPUTER_PORT, txPcFrame);     // Send response to PC
        break;
    case GET_MAIN_BAT_SLEEP_CURRENT:
// This test takes more time, that command has been received
#ifdef WCM_3P
        ret = pcb_tests_getSleepcurrent_LTCT_3p(data, &dataLen);
#elif defined(LTCT_3P)
        ret = pcb_tests_getSleepcurrent_LTCT_3p(data, &dataLen);
#elif defined(HTCT_3P)
        ret = pcb_tests_getSleepcurrent_LTCT_3p(data, &dataLen);
#endif
        pc_comm_createResponseFrame(data, dataLen, ret, txPcFrame); // Update response structure
        ret = pc_comm_sendPcResponse(COMPUTER_PORT, txPcFrame);     // Send response to PC
        break;
    case GET_METER_PCB_MAINS_DC_VTG:
        ret = pcb_tests_mainsDcVtg(data, &dataLen);
        pc_comm_createResponseFrame(data, dataLen, ret, txPcFrame); // Update response structure
        ret = pc_comm_sendPcResponse(COMPUTER_PORT, txPcFrame);     // Send response to PC
        break;
    case GET_METER_PCB_SUPERCAP_VTG:
        ret = pcb_tests_superCapVtg(data, &dataLen);
        pc_comm_createResponseFrame(data, dataLen, ret, txPcFrame); // Update response structure
        ret = pc_comm_sendPcResponse(COMPUTER_PORT, txPcFrame);     // Send response to PC
        break;

    case GET_STATUS_METER_I2C_CHECK: // MRY test
        ret = funcTest_I2cCheck();
        pc_comm_createResponseFrame(data, dataLen, ret, txPcFrame);
        ret = pc_comm_sendPcResponse(COMPUTER_PORT, txPcFrame);
        break;
    case SET_METER_DATE_TIME:
        memcpy(data, rxPcFrame->rxData, rxPcFrame->rxDataLen);
        dataLen = rxPcFrame->rxDataLen;
        ret = meter_data_setDateTime(data, &dataLen);
        pc_comm_createResponseFrame(data, dataLen, ret, txPcFrame); // Update response structure
        ret = pc_comm_sendPcResponse(COMPUTER_PORT, txPcFrame);     // Send response to PC
        break;

    case GET_STATUS_METER_CLEAR1:
        ret = funcTest_clear1();
        pc_comm_createResponseFrame(data, dataLen, ret, txPcFrame);
        ret = pc_comm_sendPcResponse(COMPUTER_PORT, txPcFrame);
        break;
    case GET_STATUS_METER_CLEAR2:
        ret = funcTest_clear2();
        pc_comm_createResponseFrame(data, dataLen, ret, txPcFrame);
        ret = pc_comm_sendPcResponse(COMPUTER_PORT, txPcFrame);
        break;
    case GET_STATUS_METER_RESET:
        ret = pcb_tests_meter_reset(data, &dataLen);
        pc_comm_createResponseFrame(data, dataLen, ret, txPcFrame);
        ret = pc_comm_sendPcResponse(COMPUTER_PORT, txPcFrame);
        break;
    case GET_STATUS_RELAY_TEST:
#ifdef WCM_3P
        ret = funcTest_relay();
#elif defined(LTCT_3P)
        ret = funcTest_Digital_IO(data, &dataLen);
#elif defined(HTCT_3P)
        ret = funcTest_Digital_IO(data, &dataLen);
#endif
        pc_comm_createResponseFrame(data, dataLen, ret, txPcFrame);
        ret = pc_comm_sendPcResponse(COMPUTER_PORT, txPcFrame);
        break;
    case GET_STATUS_VTG_AND_CURRENT:
        ret = funcTest_vtgAndCurrent_LTCT_3p(data, &dataLen);
        pc_comm_createResponseFrame(data, dataLen, ret, txPcFrame);
        ret = pc_comm_sendPcResponse(COMPUTER_PORT, txPcFrame);
        break;
    case GET_STATUS_KWH:
        ret = funcTest_kwh();
        pc_comm_createResponseFrame(data, dataLen, ret, txPcFrame);
        ret = pc_comm_sendPcResponse(COMPUTER_PORT, txPcFrame);
        break;
    case GET_STATUS_KVARH:
        ret = funcTest_kVarh();
        pc_comm_createResponseFrame(data, dataLen, ret, txPcFrame);
        ret = pc_comm_sendPcResponse(COMPUTER_PORT, txPcFrame);
        break;
    case GET_STATUS_RF:
        ret = funcTest_rf();
        pc_comm_createResponseFrame(data, dataLen, ret, txPcFrame);
        ret = pc_comm_sendPcResponse(COMPUTER_PORT, txPcFrame);
        break;
    case GET_STATUS_METER_SWITCHES:
        ret = funcTest_meter_switch(data, &dataLen);
        pc_comm_createResponseFrame(data, dataLen, ret, txPcFrame);
        ret = pc_comm_sendPcResponse(COMPUTER_PORT, txPcFrame);
        break;
    case GET_STATUS_DIGITAL_IO:
#ifdef WCM_3P
        ret = funcTest_relay();
#elif defined(LTCT_3P)
        ret = funcTest_Digital_IO(data, &dataLen);
#elif defined(HTCT_3P)
        ret = funcTest_Digital_IO(data, &dataLen);
#endif
        pc_comm_createResponseFrame(data, dataLen, ret, txPcFrame);
        ret = pc_comm_sendPcResponse(COMPUTER_PORT, txPcFrame);
        break;
    case SET_STATUS_ALL_PASS_LED:
        memcpy(data, rxPcFrame->rxData, rxPcFrame->rxDataLen);
        dataLen = rxPcFrame->rxDataLen;
        ret = funcTest_allPassLed(data, &dataLen);
        pc_comm_createResponseFrame(data, dataLen, ret, txPcFrame);
        ret = pc_comm_sendPcResponse(COMPUTER_PORT, txPcFrame);
        break;
    case START_RTC_CALIB:
        if (RELAY_POWER == 0)
        {
            RELAY_POWER_ON();
            delay_ms(8000);
        }
#ifdef DEBUG_MSG
        sprintf(tempStr, "\n GOT computer Data: %s\n", rxPcFrame->rxData);
        // rtc_prod_sendDataToPC("\nGot Computer Data\n", COMPUTER_PORT, 19);
        rtc_prod_sendDataToPC(tempStr, COMPUTER_PORT, strlen(tempStr));
#endif
        // this is needed when communicating with RTC board.
        if (0 == SW_IN_12_9600_BAUD)
        {
            R_UART2_Stop();
            R_UART3_Stop();
            R_SAU1_Create(9600);
            R_UART3_Start();
        }
        ret = rtc_prod_initiateRTCCalibration(data, &dataLen); // Initialize the RTC Process
        gRTC_ret = ret;
        pc_comm_createResponseFrame(data, dataLen, ret, txPcFrame); // Update response structure
        ret = pc_comm_sendPcResponse(COMPUTER_PORT, txPcFrame);     // Send response to PC
        if (0 == SW_IN_12_9600_BAUD)
        {
            R_UART2_Stop();
            R_UART3_Stop();
            R_SAU1_Create(19200);
            R_UART2_Start();
            R_UART3_Start();
        }
        break;

    case GET_PPM_OFFSET:
#ifdef DEBUB_MSG
        sprintf(tempStr, "\n Got PPM Read Command: %s\n", rxPcFrame->rxData);
        // rtc_prod_sendDataToPC("\nGot Computer Data\n", COMPUTER_PORT, 19);
        rtc_prod_sendDataToPC(tempStr, COMPUTER_PORT, strlen(tempStr));
#endif
        ret = rtc_prod_getPppmData(data, &dataLen);
        pc_comm_createResponseFrame(data, dataLen, ret, txPcFrame); // Update response structure
        ret = pc_comm_sendPcResponse(COMPUTER_PORT, txPcFrame);     // Send response to PC
        break;

    case GET_TEMP_OFFSET:
#ifdef DEBUB_MSG
        sprintf(tempStr, "\n Got Temperature Offset Read Command: %s\n", rxPcFrame->rxData);
        // rtc_prod_sendDataToPC("\nGot Computer Data\n", COMPUTER_PORT, 19);
        rtc_prod_sendDataToPC(tempStr, COMPUTER_PORT, strlen(tempStr));
#endif
        ret = rtc_prod_getTempData(data, &dataLen);
        pc_comm_createResponseFrame(data, dataLen, ret, txPcFrame); // Update response structure
        ret = pc_comm_sendPcResponse(COMPUTER_PORT, txPcFrame);     // Send response to PC
        break;

    case GET_RTC_PARAMETERS:
        ret = funcTest_getRTC_parameters(data, &dataLen);
        pc_comm_createResponseFrame(data, dataLen, ret, txPcFrame); // Update response structure
        ret = pc_comm_sendPcResponse(COMPUTER_PORT, txPcFrame);     // Send response to PC
        break;
    case START_1HZ:
        ret = meter_data_getMeterParam(data, &dataLen, START_1HZ);
        dataLen = 0;
        pc_comm_createResponseFrame(data, dataLen, ret, txPcFrame); // Update response structure
        ret = pc_comm_sendPcResponse(COMPUTER_PORT, txPcFrame);     // Send response to PC
        break;
    case STOP_1HZ:
        ret = meter_data_getMeterParam(data, &dataLen, STOP_1HZ);
        dataLen = 0;
        pc_comm_createResponseFrame(data, dataLen, ret, txPcFrame); // Update response structure
        ret = pc_comm_sendPcResponse(COMPUTER_PORT, txPcFrame);     // Send response to PC
        break;
    case GET_MAGNET_SENSOR_DATA:
        ret = meter_data_getMeterParam(data, &dataLen, GET_MAGNET_SENSOR_DATA);
        pc_comm_createResponseFrame(data, dataLen, ret, txPcFrame); // Update response structure
        ret = pc_comm_sendPcResponse(COMPUTER_PORT, txPcFrame);     // Send response to PC
        break;

    default:
        ret = JIG_WRONG_COMMAND;
    }

    return ret;
}

//******************************************************************************************************** */

e_jigErrorCodes read_data_flash(char *readData, uint8_t *dataLength, uint16_t get_dataStarting_Address, uint16_t get_dataAddress_length)
{
    // Pointer to the starting address in flash memory
    uint8_t *flash_ptr;
    unsigned char temp_data = 0xff;
    uint8_t i;

    // Set flash_ptr to the starting address in flash memory
    flash_ptr = (uint8_t *)(get_dataStarting_Address);

    for (i = 0; i < get_dataAddress_length; i++)
    {
        // Read data from the flash memory
        temp_data = *(flash_ptr + i);          // Increment flash_ptr by i
        readData[(*dataLength)++] = temp_data; // Store data in readData
    }

    return JIG_COMMNAD_OK;
}

// Function to write data to flash memory
e_jigErrorCodes write_data_flash(char *writedata, uint8_t *dataLength, uint16_t set_dataStarting_Address, uint16_t set_dataAddress_length)
{
    uint8_t i = 0;
    uint8_t *flash_ptr;

    //  disable_flash_write_protection
    EPR_WRITE_PROTECT_DISABLE_STATEMENT; /* Disable write protect */

    flash_ptr = (uint8_t *)set_dataStarting_Address;

    for (i = 0; i < set_dataAddress_length; i++)
    {
        // Check if the address is within the allowed flash range
        // if ((set_dataStarting_Address + i) >= DATA_FLASH_START_ADDRESS &&
        //    (set_dataStarting_Address + i) <= DATA_FLASH_END_ADDRESS) {
        delay_ms(10);
        // Flash memory requires erasing before writing
        *(flash_ptr + i) = writedata[*dataLength++]; // Erase the sector where the write will occur

        // }
        // else {
        //
        //         return FLASH_WRITE_ERROR;
        //     }
    }

    //  enable_flash_write_protection
    EPR_WRITE_PROTECT_ENABLE_STATEMENT; /* Enable write protect */
    return FLASH_WRITE_SUCCESS;
}
