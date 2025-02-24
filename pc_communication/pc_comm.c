#include "r_cg_macrodriver.h"
#include "rtc_prod.h"
#include "pc_comm.h"
#include "jig_operations.h"
#include "error_codes.h"
#include "r_main.h"

int g_enableDebug = 0;

/**
 * This function starts the PC operations and waits for different PC related commands.
 * Note: currently, It is waiting for one operation only as code was integrated into the Harjeet's code.
 */
void pc_comm_startPcOperation(void)
{
    e_jigOperationCommands rxPcCommand;
    t_rxFrameInfo rxPcFrame;
    t_txFrameInfo txPcFrame;

    rxPcCommand = pc_comm_getPcCommand(COMPUTER_PORT, &rxPcFrame);
    if (rxPcCommand != NO_COMMAND)
    {
        txPcFrame.rxCommand = rxPcFrame.rxCommand;
        jig_operations_executeCommand(&rxPcFrame, &txPcFrame);
    }
}

/**
 * This function keeps checking for received command from computer and returns the same as soon as it gets one.
 */
e_jigOperationCommands pc_comm_waitForPcCommand(t_rxFrameInfo *p_rxPcFrame)
{
    e_jigOperationCommands rxPcCommand;

    while ((rxPcCommand = pc_comm_getPcCommand(COMPUTER_PORT, p_rxPcFrame)) == NO_COMMAND)
    {
#ifdef DEBUG_MSG
        rtc_prod_sendDataToPC("\nWaiting for Data\n", COMPUTER_PORT, 18);
#endif
        delay_ms(1000);
    }

    return rxPcCommand;
}

/**
 * This function returns the command sent by PC application if there is any.
 * It also parse the meter number and data sent along with frame packet.
 */
e_jigOperationCommands pc_comm_getPcCommand(e_commPorts pcPort, t_rxFrameInfo *rxPcFrame)
{
    uint8_t frameLen, rxData[RX_TX_BUF];
    static char rdBuf[50];
    if ((stComm[pcPort].byWrPtr > 5) && (stComm[pcPort].wRcvTimeOut > 99))
    {
#ifdef DEBUG_MSG
        memcpy(rdBuf, &stComm[pcPort].byRdBuf[0], 50);
        rtc_prod_sendDataToPC(rdBuf, COMPUTER_PORT, sizeof(rdBuf));
#endif
        // Some data received
        stComm[pcPort].byWrPtr = 0; // to enable it to receive new data
        memcpy(rxData, &stComm[pcPort].byRdBuf[0], RX_TX_BUF);
        // check for header
        if (0 == memcmp(rxData, PC_COMM_HEADER, PC_HEADER_LEN))
        {
            frameLen = rxData[PC_HEADER_LEN]; // or we can compute the frame length by finding '#'
            // check for frame end at correct location
            if ('#' == rxData[frameLen - 1])
            {
                memcpy(&rxPcFrame->rxCommand, &rxData[PC_COMMAND_LOC], PC_COMMAND_LEN);
                rxPcFrame->rxDataLen = frameLen - (OVERHEAD);
                memcpy(rxPcFrame->rxData, &rxData[PC_DATA_LOC], rxPcFrame->rxDataLen);
                
                return rxData[PC_COMMAND_LOC];
            }
        }
    }
    return NO_COMMAND;
}

/**
 * This function sends the frame with command response to PC. we can send normal data as well embedded into the frame.
 * Same frame format is used.
 */
e_jigErrorCodes pc_comm_sendPcResponse(e_commPorts pcPort, t_txFrameInfo *txPcFrame)
{
    e_jigErrorCodes ret = PC_SEND_OK;
    uint8_t tempStr[20];
    uint8_t byLength = PC_HEADER_LEN, bySPCDIndex, *pbyTemp, i;
    memcpy(&stComm[pcPort].byWrBuf[0], PC_COMM_HEADER, PC_HEADER_LEN);
    byLength++;
    stComm[pcPort].byWrBuf[byLength++] = txPcFrame->rxCommand;
    stComm[pcPort].byWrBuf[byLength++] = txPcFrame->errorCode;

    for (i = 0; i < txPcFrame->txDataLen; i++)
    {
        stComm[pcPort].byWrBuf[byLength++] = txPcFrame->txData[i];
    }

    stComm[1].byWrBuf[byLength++] = '#';
    stComm[1].byWrBuf[PC_HEADER_LEN] = byLength;

    stComm[1].bySndLen = byLength;
    stComm[1].bySndPtr = 0;
    stComm[1].byWrPtr = 0;
    Port1TxIntEnable();
    bySPCDIndex = 0;
    while ((stComm[1].bySndPtr < stComm[1].bySndLen) && (bySPCDIndex++ < PC_SEND_TIMEOUT))
    {
        delay_ms(100);
    }

    if (bySPCDIndex >= PC_SEND_TIMEOUT)
    {
        ret = PC_SEND_ERROR;
    }

    return ret;
}

/**
 * It creates the send frame data
 */
void pc_comm_createResponseFrame(char *data, uint8_t dataLen, e_jigErrorCodes ret, t_txFrameInfo *txPcFrame)
{
    uint8_t i;

    // copy error code
    txPcFrame->errorCode = ret;

    // Populate the data field
    for (i = 0; i < dataLen; i++)
    {
        txPcFrame->txData[i] = *(data + i);
    }
    txPcFrame->txDataLen = dataLen;
}

/**
 * This function send any string or data to PC
 */
void pc_comm_sendDataToPC(char *str)
{
    // This is demo function to send the data to PC
    uint8_t byLength = 0;
    byLength = strlen(str);
    memcpy(&stComm[1].byWrBuf[0], str, byLength);
    stComm[1].bySndLen = byLength;
    stComm[1].bySndPtr = 0;
    // stComm[1].byWrPtr = 0;

    Port1TxIntEnable();

    while ((stComm[1].bySndPtr < stComm[1].bySndLen))
    {
        delay_ms(100);
    }
}

/**
 * Older Implementation
 */
uint8_t checkPCComm(void)
{
    char data[PC_DATA_MAX_LEN];
    uint8_t dataLen = 0;
    e_jigErrorCodes ret;
    t_txFrameInfo txPcFrame;

    uint8_t byLength = 6, byCPCCIndex, *pbyTemp, byPktID = 0xFF;
    if ((stComm[1].byWrPtr > 5) && (stComm[1].wRcvTimeOut > 99))
    {
        // Some data received
        stComm[1].byWrPtr = 0;
        // stComm[1].byPktRcvd = 1;
        pbyTemp = &stComm[1].byRdBuf[0];
        if (0 == memcmp(pbyTemp, "$JIG02", 6))
        {
            byLength = stComm[1].byRdBuf[6];
            if (byLength >= stComm[1].byWrPtr)
            { // Complete packet received
                pbyTemp = (uint8_t *)&dMtrSerial;
                if ('#' == stComm[1].byRdBuf[byLength - 1])
                {
                    // for ( byCPCCIndex = 0; byCPCCIndex < 4; byCPCCIndex++)
                    //{
                    //         *pbyTemp++ = stComm[1].byRdBuf[7 + byCPCCIndex];
                    // }
                    // Write to eeprom
                    byPktID = stComm[1].byRdBuf[11];
                    switch (byPktID)
                    {
                    case CURRENT_PKT:
                        getResposnseFromMeter(0, 150, CURRENT_PKT, &byRespBuf[0]);
                        sendPCData(CURRENT_PKT);
                        break;
                    case VERSION_PKT:
                    case GET_VERSION_PKT:
                        getResposnseFromMeter(0, 150, VERSION_PKT, &byRespBuf[0]);
                        sendPCData(VERSION_PKT);
                        break;
                    case GET_INT_FIRM_VERSION:
                        getResposnseFromMeter(0, 150, GET_INT_FIRM_VERSION, &byRespBuf[0]);
                        if (byRespBuf[6] == 0 && byRespBuf[7] == 0 && byRespBuf[8] == 0)
                        {
                            getResposnseFromMeter(0, 150, GET_INT_FIRM_VERSION, &byRespBuf[0]);
                            if (byRespBuf[6] == 0 && byRespBuf[7] == 0 && byRespBuf[8] == 0)
                            {
                                getResposnseFromMeter(0, 150, GET_INT_FIRM_VERSION, &byRespBuf[0]);
                            }
                        }
                        sendPCData(GET_INT_FIRM_VERSION);
                        break;
                    case DATE_TIME_PKT:;
                    case GET_DATE_TIME_PKT:
                        getResposnseFromMeter(0, 150, DATE_TIME_PKT, &byRespBuf[0]);
                        sendPCData(DATE_TIME_PKT);
                        break;
                    case SERIAL_ID_PKT:
                        getResposnseFromMeter(0, 150, SERIAL_ID_PKT, &byRespBuf[0]);
                        sendPCData(SERIAL_ID_PKT);
                        break;
                    case SET_SERIAL_ID_PKT:
                        for (byLength = 0; byLength < 4; byLength++)
                        {
                            byRespBuf[byLength] = stComm[1].byRdBuf[12 + byLength];
                        }
                        getResposnseFromMeter(0, 150, SET_SERIAL_ID_PKT, &byRespBuf[0]);
                        getResposnseFromMeter(0, 150, SERIAL_ID_PKT, &byRespBuf[0]);
                        sendPCData(SERIAL_ID_PKT);
                        break;
                    case BOARD_ID_PKT:
                        getResposnseFromMeter(0, 150, BOARD_ID_PKT, &byRespBuf[0]);
                        sendPCData(BOARD_ID_PKT);
                        break;
                    case SET_BOARD_ID_PKT:
                        for (byLength = 0; byLength < 8; byLength++)
                        {
                            byRespBuf[byLength] = stComm[1].byRdBuf[12 + byLength];
                        }
                        getResposnseFromMeter(0, 150, SET_BOARD_ID_PKT, &byRespBuf[0]);
                        getResposnseFromMeter(0, 150, BOARD_ID_PKT, &byRespBuf[0]);
                        sendPCData(BOARD_ID_PKT);
                        break;
                    case SET_DATE_TIME_PKT:
                        for (byLength = 0; byLength < 8; byLength++)
                        {
                            byRespBuf[byLength] = stComm[1].byRdBuf[12 + byLength];
                        }
                        getResposnseFromMeter(0, 150, SET_DATE_TIME_PKT, &byRespBuf[0]);
                        getResposnseFromMeter(0, 150, DATE_TIME_PKT, &byRespBuf[0]);
                        sendPCData(DATE_TIME_PKT);
                        break;
                    case START_RTC_CALIB:
                        // this is needed when communicating with RTC board.
                        R_UART2_Stop();
                        R_UART3_Stop();
                        R_SAU1_Create(9600);
                        R_UART3_Start();
                        txPcFrame.rxCommand = START_RTC_CALIB;
                        ret = rtc_prod_initiateRTCCalibration(data, &dataLen); // Initialize the RTC Process
                        g_ret = ret;
                        pc_comm_createResponseFrame(data, dataLen, ret, &txPcFrame); // Update response structure
                        ret = pc_comm_sendPcResponse(COMPUTER_PORT, &txPcFrame);     // Send response to PC
                        R_UART2_Stop();
                        R_UART3_Stop();
                        R_SAU1_Create(19200);
                        R_UART2_Start();
                        R_UART3_Start();
                        break;
                    case RESET_JIG:
                        // Reset board here
                        break;
                    case GET_JIG_FIRM_VER:
                        sendPCData(GET_JIG_FIRM_VER);
                        break;
                    case ENABLE_DEBUG:
                        g_enableDebug = 1;
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }
    return byPktID;
}

/**
 * Older Implementation
 */
void sendPCData(uint8_t bySPCDTag)
{
    uint8_t datalen = 12;
    uint8_t byLength = 6, bySPCDIndex, *pbyTemp;
    memcpy(&stComm[1].byWrBuf[0], "$JIG02", 6);
    byLength++;
    pbyTemp = (uint8_t *)&dMtrSerial;
    for (bySPCDIndex = 0; bySPCDIndex < 4; bySPCDIndex++)
    {
        stComm[1].byWrBuf[byLength++] = *pbyTemp++;
    }
    stComm[1].byWrBuf[byLength++] = bySPCDTag;
    switch (bySPCDTag)
    {
    case SERIAL_ID_PKT: // version number
        pbyTemp = (uint8_t *)&dMtrSerial;
        for (bySPCDIndex = 0; bySPCDIndex < 4; bySPCDIndex++)
        {
            stComm[1].byWrBuf[byLength++] = byRespBuf[6 + bySPCDIndex]; //*pbyTemp++;
            *pbyTemp++ = byRespBuf[6 + bySPCDIndex];
        }
        break;
    case BOARD_ID_PKT: // version number
        // pbyTemp = (uint8_t*)&dMtrSerial;
        for (bySPCDIndex = 0; bySPCDIndex < 8; bySPCDIndex++)
        {
            stComm[1].byWrBuf[byLength++] = byRespBuf[6 + bySPCDIndex]; //*pbyTemp++;
            *pbyTemp++ = byRespBuf[6 + bySPCDIndex];
        }
        break;
    case SWITCH_CONNECT_CHK_PKT:
    case SWITCH_RELEASE_CHK_PKT:
        stComm[1].byWrBuf[byLength++] = byRespBuf[6];
        break;
    case VERSION_PKT: // version number
        pbyTemp = &byRespBuf[6];
        for (bySPCDIndex = 0; bySPCDIndex < 10; bySPCDIndex++)
        {
            stComm[1].byWrBuf[byLength++] = *pbyTemp++;
        }
        break;
    case GET_INT_FIRM_VERSION: // version number
        pbyTemp = &byRespBuf[6];
        for (bySPCDIndex = 0; bySPCDIndex < 11; bySPCDIndex++)
        {
            stComm[1].byWrBuf[byLength++] = *pbyTemp++;
        }
        break;
    case DATE_TIME_PKT: // Date Time + Cover Open Tamper
        pbyTemp = &byRespBuf[6];
        for (bySPCDIndex = 0; bySPCDIndex < 9; bySPCDIndex++)
        {
            stComm[1].byWrBuf[byLength++] = *pbyTemp++;
        }
        break;
    case CURRENT_PKT: // Voltage + Phase Current + Neutral Current
        pbyTemp = &byRespBuf[6];

        if (0 == SW_IN_11_1PH_3PH)
        {
            datalen = 28;
        }
        for (bySPCDIndex = 0; bySPCDIndex < datalen; bySPCDIndex++)
        {
            stComm[1].byWrBuf[byLength++] = *pbyTemp++;
        }
        break;
    case VOLTAGE_FAIL_PKT:
        pbyTemp = &unVFS.byVFBuf[0];
        for (bySPCDIndex = 0; bySPCDIndex < 8; bySPCDIndex++)
        {
            stComm[1].byWrBuf[byLength++] = *pbyTemp++;
        }
        break;
    case FINAL_PKT: // Voltage + Phase Current + Neutral Current
        pbyTemp = &byLedDisplayResult[0];
        for (bySPCDIndex = 0; bySPCDIndex < MAX_LED; bySPCDIndex++)
        {
            stComm[1].byWrBuf[byLength++] = *pbyTemp++;
        }
        break;
    case BOARD_VOLTAGES_DATA:
        pbyTemp = &unBoardValues.byVFBuf[0];
        for (bySPCDIndex = 0; bySPCDIndex < sizeof(unBoardValues); bySPCDIndex++)
        {
            stComm[1].byWrBuf[byLength++] = *pbyTemp++;
        }
        break;
    case GET_JIG_FIRM_VER:
        pbyTemp = &JIG_FIRM_VERSION[0];
        for (bySPCDIndex = 0; bySPCDIndex < (sizeof(JIG_FIRM_VERSION)) - 1; bySPCDIndex++)
        {
            stComm[1].byWrBuf[byLength++] = *pbyTemp++;
        }
        break;

    default:
        break;
    }
    stComm[1].byWrBuf[byLength++] = '#';
    stComm[1].byWrBuf[6] = byLength;

    stComm[1].bySndLen = byLength;
    stComm[1].bySndPtr = 0;
    stComm[1].byWrPtr = 0;
    Port1TxIntEnable();
    bySPCDIndex = 0;
    while ((stComm[1].bySndPtr < stComm[1].bySndLen) && (bySPCDIndex++ < 100))
    {
        delay_ms(100);
    }
    delay_ms(250);
}
