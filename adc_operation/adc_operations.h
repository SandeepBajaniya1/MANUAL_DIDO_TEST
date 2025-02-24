#ifndef __ADC_OPERATIONS_H__
#define __ADC_OPERATIONS_H__

#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_serial.h"
#include "r_cg_adc.h"
#include "r_cg_it.h"
#include "r_main.h"

/*--------Functions-------------*/
float getAdcValue(uint8_t);
float getAdcValue2(uint8_t byChannel);

#endif