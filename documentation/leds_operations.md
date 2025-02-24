# LED Operations

## LED Assignment
```
LED0: Optical
LED1. MRY or i2c_check
LED2. Date and time
LED3. Clear
LED4. Reset
LED5. Relay
LED6. Current and Voltage
LED7. KWH
LED8. RF
LED9. Switch
LED10. All Voltage (Board Voltages)
LED11. <Not used>
LED12. All Pass
LED13. <Not used>
LED14. <Not used>
LED15. <Not used>
```


byLedDisplayResult switch (ledNumber)
    {
    case OPTICAL_STATUS:
        if (ledStatus == TEST_PASS)
        {
            byLedDisplayResult[OPTICAL_STATUS] = TEST_PASS;
        }
        else
        {
            byLedDisplayResult[OPTICAL_STATUS] = TEST_FAIL;
        }
        break;

    case I2C_CHECK:
        if (ledStatus == TEST_PASS)
        {
            byLedDisplayResult[I2C_CHECK] = TEST_PASS;
        }
        else
        {
            byLedDisplayResult[I2C_CHECK] = TEST_FAIL;
        }
        break;

    case DATE_TIME:
        if (ledStatus == TEST_PASS)
        {
            byLedDisplayResult[DATE_TIME] = TEST_PASS;
        }
        else
        {
            byLedDisplayResult[DATE_TIME] = TEST_FAIL;
        }
        break;

    case CLEAR_STATUS:
        if (ledStatus == TEST_PASS)
        {
            byLedDisplayResult[CLEAR_STATUS] = TEST_PASS;
        }
        else
        {
            byLedDisplayResult[CLEAR_STATUS] = TEST_FAIL;
        }
        break;

    case RESET_STATUS:
        if (ledStatus == TEST_PASS)
        {
            byLedDisplayResult[RESET_STATUS] = TEST_PASS;
        }
        else
        {
            byLedDisplayResult[RESET_STATUS] = TEST_FAIL;
        }
        break;

    case RELAY_STATUS:
        if (ledStatus == TEST_PASS)
        {
            byLedDisplayResult[RELAY_STATUS] = TEST_PASS;
        }
        else
        {
            byLedDisplayResult[RELAY_STATUS] = TEST_FAIL;
        }
        break;

    case CUR_VTG_CHECK:
        if (ledStatus == TEST_PASS)
        {
            byLedDisplayResult[CUR_VTG_CHECK] = TEST_PASS;
        }
        else
        {
            byLedDisplayResult[CUR_VTG_CHECK] = TEST_FAIL;
        }
        break;

    case KWH_CHECK:
        if (ledStatus == TEST_PASS)
        {
            byLedDisplayResult[KWH_CHECK] = TEST_PASS;
        }
        else
        {
            byLedDisplayResult[KWH_CHECK] = TEST_FAIL;
        }
        break;

    case OPTICAL_STATUS:
        if (ledStatus == TEST_PASS)
        {
            byLedDisplayResult[OPTICAL_STATUS] = TEST_PASS;
        }
        else
        {
            byLedDisplayResult[OPTICAL_STATUS] = TEST_FAIL;
        }
        break;

    case OPTICAL_STATUS:
        if (ledStatus == TEST_PASS)
        {
            byLedDisplayResult[OPTICAL_STATUS] = TEST_PASS;
        }
        else
        {
            byLedDisplayResult[OPTICAL_STATUS] = TEST_FAIL;
        }
        break;

    case OPTICAL_STATUS:
        if (ledStatus == TEST_PASS)
        {
            byLedDisplayResult[OPTICAL_STATUS] = TEST_PASS;
        }
        else
        {
            byLedDisplayResult[OPTICAL_STATUS] = TEST_FAIL;
        }
        break;

    case OPTICAL_STATUS:
        if (ledStatus == TEST_PASS)
        {
            byLedDisplayResult[OPTICAL_STATUS] = TEST_PASS;
        }
        else
        {
            byLedDisplayResult[OPTICAL_STATUS] = TEST_FAIL;
        }
        break;

    case OPTICAL_STATUS:
        if (ledStatus == TEST_PASS)
        {
            byLedDisplayResult[OPTICAL_STATUS] = TEST_PASS;
        }
        else
        {
            byLedDisplayResult[OPTICAL_STATUS] = TEST_FAIL;
        }
        break;

    default:
        break;
    }