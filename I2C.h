#include "r_cg_userdefine.h"

#define RTC_SCL_PIN			P6.0
#define RTC_SDA_PIN			P6.1
#define RTC_SCL_PIN_IN		P6.0
#define RTC_SDA_PIN_IN		P6.1


#define RTC_SDA_PIN_DIR		PM6.1

#define sbi(pin)			(pin = 1)
#define cbi(pin)			(pin = 0)

#define RTC_CHIP			3
#define EEPROM_CHIP			4
/////////////////////////////
#define DCU_EVENT_LOC         128
#define MAINS_STAT_FLAG_LOC   130
#define IMAGE_UPDATE_INFO_LOC 256
#define DCU_INSTALLED_LOC     384
#define EEPROM_CONFIG_ADDRESS 1024


/////////////////////////////
void rtc_read_date(void);
void rtc_read_time(void);
void set_rtc_time(unsigned char chour,unsigned char cminute, unsigned char csecond);
void set_rtc_date(unsigned char cday,unsigned char cmonth, unsigned char cyear);
unsigned char convt_byte_to_bcd(unsigned char byte_data);
void write_eeprom(unsigned long int address,unsigned char cData);
unsigned char read_eeprom(unsigned long int address);
void rtc_write_byte(unsigned char addr, unsigned char data);
unsigned char rtc_read_byte(unsigned char addr);
void to_eeprom(unsigned long int nAddr, unsigned long int ldata, unsigned char size);
unsigned long int from_eeprom(unsigned long int nAddr,unsigned char size);
void read_page_eeprom(unsigned long int address, unsigned char *byDstPtr);
void write_page_eeprom(unsigned long int address,unsigned char *cData);
void RTC_Init(void);