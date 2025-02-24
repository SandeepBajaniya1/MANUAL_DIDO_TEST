#ifndef __METER_DATA_H__
#define __METER_DATA_H__

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
#include "jig_operations.h"
#include "meter_comm.h"

/*--------MACROS------------*/
#define MAGNET_SENSOR_DATA_LEN 3
#define RTC_PARAM_LEN 5
#define METER_DATA_LEN  100

/*---------Functions------------*/
e_jigErrorCodes meter_data_getMagnetSensorData(char *, uint8_t *);
e_jigErrorCodes meter_data_getRtcParam(char *, uint8_t *);
e_jigErrorCodes meter_data_setDateTime(char *data, uint8_t *dataLen);
e_jigErrorCodes meter_data_getMeterParam(char *data, uint8_t *dataLen, e_jigOperationCommands oper);

#endif