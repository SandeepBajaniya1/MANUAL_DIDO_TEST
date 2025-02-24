#ifndef __RTC_PROD_H__
#define __RTC_PROD_H__

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
#include "meter_comm.h"

/*---------MACROS------------*/
#define PPM_MAX 1000
#define PPM_MIN -1000
#define AVG_PPM_MAX 100
#define AVG_PPM_MIN -100
#define TEMPERATURE_MAX 8500
#define TEMPERATURE_MIN -4000
#define PPM_DIVISIOR 10.00
#define TEMP_DIVISIOR 100.00
#define RTC_PPM_TEMP_PORT 3
#define METER_1_HZ_DELAY 5000
#define PPPM_TEMP_DATA_BUF 40
#define RTC_CALIB_DATA_LEN 5

/*---------Global Variables--------*/
extern int g_temperature;
extern int g_PPM;

void rtc_prod_sendDataToPC(char *, int, uint8_t);
void rtc_prod_rtcCalibrate(uint8_t, float);
e_jigErrorCodes rtc_prod_initiateRTCCalibration(char *, uint8_t *);
e_jigErrorCodes rtc_prod_getPppmData(char *, uint8_t *);
e_jigErrorCodes rtc_prod_getTempData(char *, uint8_t *);
e_jigErrorCodes rtc_prod_getRtcParameters(char *, uint8_t *);
e_jigErrorCodes rtc_prod_getAvgPPM(char *, uint8_t *);
#endif