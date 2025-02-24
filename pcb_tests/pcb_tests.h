#ifndef __PCB_TESTS_H__
#define __PCB_TESTS_H__

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


/*--------MACROS------*/

/*-------Structures & ENUM---------*/


/*------Global Extern Variables---------*/


/*------Local variable------------------*/


/*------Functions----------------*/
e_jigErrorCodes pcb_tests_getBatVoltage(char *data, uint8_t *dataLen);
e_jigErrorCodes pcb_tests_getSleepcurrent_SPH(char *data, uint8_t *dataLen);
e_jigErrorCodes pcb_tests_getSleepcurrent_LTCT_3p(char *data, uint8_t *dataLen);
e_jigErrorCodes pcb_tests_mainsDcVtg(char *data, uint8_t *dataLen);
e_jigErrorCodes pcb_tests_superCapVtg(char *data, uint8_t *dataLen);
e_jigErrorCodes pcb_tests_meter_reset(char *data, uint8_t *dataLen);





#endif