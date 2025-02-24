#ifndef __R_MAIN_H__
#define __R_MAIN_H__

#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "leds_operations.h"
#include "without_pc.h"

// #define DEBUG_MSG
extern BYTE byTestResult;
extern uint8_t byTestByte;
extern uint16_t wTimeout;
extern BYTE *byCmdPtr, *byRspPtr;
extern BYTE byRespBuf[64];
extern uint32_t dMtrSerial;
extern UN_VOLTAGE_FAIL_STEP unVFS;
extern UN_BOARD_TEST_VALUES unBoardValues;
extern ST_RELAY_TEST stRlyChk[MAX_RELAY_COUNT];
extern BYTE by10MsTick, by10MsTick2;



void old_arch(void);
#endif