#include "r_cg_macrodriver.h"
#include "r_cg_port.h"
#include <string.h>
#include <stdio.h>
#include "r_cg_userdefine.h"
#include "leds_operations.h"

BYTE byLedDisplayResult[MAX_LED] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};

/**
 * This function update the LEDs status based on the test result.
 */
void leds_operations_updateLedStatus(e_ledNames ledNumber, e_ledStatus ledStatus)
{
    if (ledStatus == TEST_PASS)
    {
        byLedDisplayResult[ledNumber] = TEST_PASS;
    }
    else
    {
        byLedDisplayResult[ledNumber] = TEST_FAIL;
    }
}

/**
 * This functions sets the value of LEDs based on the underlying test results
 */
void SetLED(BYTE byNumber, BYTE byStatus)
{
    switch (byNumber)
    {
    case 0:
        if (byStatus == 0)
        {
            LED_1 = 0;
        }
        else if (byStatus == 1)
        {
            LED_1 ^= 1;
        }
        else
        {
            LED_1 = 1;
        }
        break;
    case 1:
        if (byStatus == 0)
        {
            LED_2 = 0;
        }
        else if (byStatus == 1)
        {
            LED_2 ^= 1;
        }
        else
        {
            LED_2 = 1;
        }
        break;
    case 2:
        if (byStatus == 0)
        {
            LED_3 = 0;
        }
        else if (byStatus == 1)
        {
            LED_3 ^= 1;
        }
        else
        {
            LED_3 = 1;
        }
        break;
    case 3:
        if (byStatus == 0)
        {
            LED_4 = 0;
        }
        else if (byStatus == 1)
        {
            LED_4 ^= 1;
        }
        else
        {
            LED_4 = 1;
        }
        break;
    case 4:
        if (byStatus == 0)
        {
            LED_5 = 0;
        }
        else if (byStatus == 1)
        {
            LED_5 ^= 1;
        }
        else
        {
            LED_5 = 1;
        }
        break;
    case 5:
        if (byStatus == 0)
        {
            LED_6 = 0;
        }
        else if (byStatus == 1)
        {
            LED_6 ^= 1;
        }
        else
        {
            LED_6 = 1;
        }
        break;
    case 6:
        if (byStatus == 0)
        {
            LED_7 = 0;
        }
        else if (byStatus == 1)
        {
            LED_7 ^= 1;
        }
        else
        {
            LED_7 = 1;
        }
        break;
    case 7:
        if (byStatus == 0)
        {
            LED_8 = 0;
        }
        else if (byStatus == 1)
        {
            LED_8 ^= 1;
        }
        else
        {
            LED_8 = 1;
        }
        break;
    case 8:
        if (byStatus == 0)
        {
            LED_9 = 0;
        }
        else if (byStatus == 1)
        {
            LED_9 ^= 1;
        }
        else
        {
            LED_9 = 1;
        }
        break;
    case 9:
        if (byStatus == 0)
        {
            LED_10 = 0;
        }
        else if (byStatus == 1)
        {
            LED_10 ^= 1;
        }
        else
        {
            LED_10 = 1;
        }
        break;
    case 10:
        if (byStatus == 0)
        {
            LED_11 = 0;
        }
        else if (byStatus == 1)
        {
            LED_11 ^= 1;
        }
        else
        {
            LED_11 = 1;
        }
        break;
    case 11:
        if (byStatus == 0)
        {
            LED_12 = 0;
        }
        else if (byStatus == 1)
        {
            LED_12 ^= 1;
        }
        else
        {
            LED_12 = 1;
        }
        break;
    case 12:
        if (byStatus == 0)
        {
            LED_13 = 0;
        }
        else if (byStatus == 1)
        {
            LED_13 ^= 1;
        }
        else
        {
            LED_13 = 1;
        }
        break;
    case 13:
        if (byStatus == 0)
        {
            LED_14 = 0;
        }
        else if (byStatus == 1)
        {
            LED_14 ^= 1;
        }
        else
        {
            LED_14 = 1;
        }
        break;
    case 14:
        if (byStatus == 0)
        {
            LED_15 = 0;
        }
        else if (byStatus == 1)
        {
            LED_15 ^= 1;
        }
        else
        {
            LED_15 = 1;
        }
        break;
    case 15:
        if (byStatus == 0)
        {
            LED_16 = 0;
        }
        else if (byStatus == 1)
        {
            LED_16 ^= 1;
        }
        else
        {
            LED_16 = 1;
        }
        break;

    default:
        break;
    }
}