#ifndef __SWITCHES_H__
#define __SWITCHES_H__

/*------Defines----------*/
#define RTC_BAT_VTG_THRESHOLD 2.70
#define MAIN_BAT_VTG_THRESHOLD 2.80
#define MAIN_BAT_SLEEP_CURRENT_MAX  0.10
#define DVCC_THRESHOLD 2.90
#define THRESHOLD_6_5   2.30
#define SUPERCAP_PWR_THRESHOLD 1.50
#define RESET_VTG 1.60

#define VRMS_MIN 150.0
#define IRMS_MIN  1.0
#define IRMS_N_MIN 1.0

/*---------Global Variables-----------*/
extern float fADCReading;
extern BYTE *pbyTmpPtr;

/*------Functions-------------*/
void sw_1_operation(void);
void sw_2_operation(void);
void sw_3_operation(void);
void sw_3_finalTask(void);
BYTE checkRelayStatus(BYTE byResulCompare);
void sw_4_operation(void);
BYTE checkBlinking(void);
void sw_5_operation(void);
BYTE checkRFPins(BYTE byResulCompare);
void sw_6_operation(void);
void sw_7_operationBatteryVoltage(void);
void sw_7_operationSuperCapacitor(void);
void sw_9_operation(void);


#endif