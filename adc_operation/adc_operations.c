#include "adc_operations.h"

/**
 * 
*/
float getAdcValue(uint8_t byChannel)
{
    uint16_t wADCValue; //, wADCValueBuf[32];
    uint8_t byADCIndex;
    float fADCVal = 0;
    // by10MsTick = 0;
    R_ADC_Stop();
    ADS = byChannel;
    for (byADCIndex = 0; byADCIndex < 32; byADCIndex++)
    {
        R_ADC_Start();
        delay_ms(2);
        R_ADC_Stop();
        R_ADC_Get_Result(&wADCValue);
        fADCVal += wADCValue;
    }
    fADCVal = fADCVal / 32.0;
    return (fADCVal / 204.80);
}

/**
 * 
*/
float getAdcValue2(uint8_t byChannel)
{
    uint16_t wADCValue; //, wADCValueBuf[32];
    uint8_t byADCIndex;
    float fADCVal = 0;
    // by10MsTick = 0;
    R_ADC_Stop();
    ADS = byChannel;
    for (byADCIndex = 0; byADCIndex < 3; byADCIndex++)
    {
        R_ADC_Start();
        delay_ms(2);
        R_ADC_Stop();
        R_ADC_Get_Result(&wADCValue);
        fADCVal += wADCValue;
    }
    fADCVal = fADCVal / 3.0;
    return (fADCVal / 204.80);
}