#ifndef __FUNCTIONAL_TESTS_H__
#define __FUNCTIONAL_TESTS_H__

#include "r_cg_macrodriver.h"
#include "SerialData.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_serial.h"
#include "r_cg_adc.h"
#include "r_cg_it.h"
#include <string.h>
#include <stdio.h>
#include "r_main.h"
#include "r_cg_userdefine.h"
#include "switches.h"
#include "error_codes.h"
#include "meter_comm.h"
#include "meter_data.h"
#include "jig_operations.h"

e_jigErrorCodes funcTest_I2cCheck();
e_jigErrorCodes funcTest_clear1();
e_jigErrorCodes funcTest_clear2();
e_jigErrorCodes funcTest_relay();
e_jigErrorCodes funcTest_vtgAndCurrent_LTCT_3p(char *data, uint8_t *dataLen);
e_jigErrorCodes funcTest_kwh();
e_jigErrorCodes funcTest_kVarh();
e_jigErrorCodes funcTest_rf();
e_jigErrorCodes funcTest_meter_switch(char *data,uint8_t *dataLen);
e_jigErrorCodes funcTest_allPassLed(char *data, uint8_t *dataLen);
e_jigErrorCodes funcTest_Digital_IO(char *data, uint8_t *dataLen);
e_jigErrorCodes funcTest_getRTC_parameters(char *data, uint8_t *dataLen);
e_jigErrorCodes manual_jig_funcTest_Digital_IO(void);
e_jigErrorCodes funcTest_vtgAndCurrent_SPH(char *data, uint8_t *dataLen);

uint8_t checkBlinking_Kwh(void);
int8_t checkBlinking_KVarh(void);
uint8_t checkRelayStatus_WCM_3p(BYTE event);
#endif