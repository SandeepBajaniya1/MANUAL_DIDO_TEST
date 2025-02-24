#ifndef __LEDS_OPERATIONS_H__
#define __LEDS_OPERATIONS_H__


// #include "r_cg_macrodriver.h"
// #include "r_cg_port.h"

/*-------MACROS--------*/


/*---------Structures and Enums----------*/
typedef enum
{
    OPTICAL_STATUS,
    I2C_CHECK,
    DATE_TIME,
    CLEAR_STATUS,
    RESET_STATUS,
    RELAY_STATUS,
    CUR_VTG_CHECK,
    KWH_CHECK,
    RF_STATUS,
    SW_STATUS,
    BOARD_VTG_STATUS,
    
    ALL_PASS = 12,
    CAP_ALL_PASS=13,

}e_ledNames;

typedef enum
{
    TEST_PASS,
    TEST_FAIL,
    NO_STATUS,
}e_ledStatus;

/*----------Global Variables--------------*/
extern BYTE byLedDisplayResult[MAX_LED];

/*-------Functions-------*/
void leds_operations_updateLedStatus(e_ledNames ledNumber, e_ledStatus ledStatus);
void SetLED(BYTE byNumber, BYTE byStatus);

#endif