#ifndef __WITHOUT_PC_H__
#define __WITHOUT_PC_H__

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



void without_PC_operation(void);

void without_pc_rtcCalibration(void);
void glow_allPassLed(void);

#endif