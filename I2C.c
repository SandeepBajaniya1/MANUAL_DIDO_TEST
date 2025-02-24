
//#pragma interrupt INTST3 r_uart3_interrupt_send
//#pragma interrupt INTSR3 r_uart3_interrupt_receive

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
//#include "r_cg_cgc.h"
#include "r_cg_port.h"
//#include "r_cg_intc.h"
//#include "r_cg_serial.h"
//#include "r_cg_timer.h"
//#include "GSM_GPRS.h"
#include "r_cg_userdefine.h"
//#include "Meter.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "I2C.h"

//#define sbi(port,bit)  (port |= (1<<bit))   //set bit in port
//#define cbi(port,bit)  (port &= ~(1<<bit))  //clear bit in port

unsigned char t_hr, t_min, t_sec, prev_sec, d_day, d_mnth, d_yr; 
DWORD real_date, real_time;

void delay10us(WORD wDelay)
{
	WORD wDelTemp;
	while (wDelay)
	{
		--wDelay;
		for (wDelTemp = 0; wDelTemp < 48; )
			wDelTemp++;
	}
}

void delay1ms(WORD ms)
{
	while (ms)
	{
		--ms;
		delay10us(30);
	}
}


void bstart(unsigned char device)
{
	//if(device==RTC_CHIP)
	{
		cbi(RTC_SCL_PIN);
		delay10us(2);
		sbi(RTC_SDA_PIN);//rakesh add
		delay10us(2);
		sbi(RTC_SCL_PIN);
		delay10us(2);
		cbi(RTC_SDA_PIN);
		delay10us(2);
		cbi(RTC_SCL_PIN);
	}
	//else
	//{
	//	cbi(EEPROM_SCL_PIN);
	//	delay10us(2);
	//	sbi(SDA_PIN);//rakesh add
	//	delay10us(2);
	//	sbi(EEPROM_SCL_PIN);
	//	delay10us(2);
	//	cbi(SDA_PIN);
	//	delay10us(2);
	//	cbi(EEPROM_SCL_PIN);
	//}

	return ;
}
//rakesh en

//rakesh st
void bstop(unsigned char device)
{
	//if(device==RTC_CHIP)
	{
		cbi(RTC_SCL_PIN);
		delay10us(2);
		cbi(RTC_SDA_PIN);
		delay10us(2);
		sbi(RTC_SCL_PIN);
		delay10us(2);
		sbi(RTC_SDA_PIN);
		delay10us(2);
		cbi(RTC_SCL_PIN);
	}
	//else
	//{
	//	cbi(EEPROM_SCL_PIN);
	//	delay10us(2);
	//	cbi(SDA_PIN);
	//	delay10us(2);
	//	sbi(EEPROM_SCL_PIN);
	//	delay10us(2);
	//	sbi(SDA_PIN);
	//	delay10us(2);
	//	cbi(EEPROM_SCL_PIN);
	//}
	return ;
}
//rakesh en

//rakesh st
void tx_data(unsigned char data, unsigned char device)
{   
	unsigned char j;
	//if(device==RTC_CHIP)
		cbi(RTC_SCL_PIN);
	//else
	//	cbi(EEPROM_SCL_PIN);

	delay10us(2);
	//if(device==RTC_CHIP)
	{
		for(j=0;j<8;j++)
		{
			if((data & 0x80) == 0x80)
				sbi(RTC_SDA_PIN);
			else
				cbi(RTC_SDA_PIN);

			delay10us(2);
			sbi(RTC_SCL_PIN);
			delay10us(2);
			cbi(RTC_SCL_PIN);
			delay10us(2);
			data = (data << 1);		
		}
	}
	//else
	//{
	//	for(j=0;j<8;j++)
	//	{
	//		if((data & 0x80) == 0x80)
	//			sbi(SDA_PIN);
	//		else
	//			cbi(SDA_PIN);
	//
	//		delay10us(2);
	//		sbi(EEPROM_SCL_PIN);
	//		delay10us(2);
	//		cbi(EEPROM_SCL_PIN);
	//		delay10us(2);
	//		data = (data << 1);		
	//	}
	//}

	//if(device==RTC_CHIP)
	{
		sbi(RTC_SDA_PIN);
		sbi(RTC_SDA_PIN_DIR);  //input
	}
	//else
	//{
	//	sbi(SDA_PIN);
	//	cbi(SDA_PIN_DIR);  //input
	//}
	delay10us(2);
	//
	//if(device==RTC_CHIP)
	{
		sbi(RTC_SCL_PIN);
		delay10us(2);
		//I2C_DATA
		cbi(RTC_SCL_PIN);
		cbi(RTC_SDA_PIN_DIR);  //output
	}
	//else
	//{
	//	sbi(EEPROM_SCL_PIN);
	//	delay10us(2);
	//	//I2C_DATA
	//	cbi(EEPROM_SCL_PIN);
	//	sbi(SDA_PIN_DIR);  //output
	//}
	//
	delay10us(2);
	return;
}
//rakesh en

//rakesh st
unsigned char rx_data(unsigned char ack, unsigned char device)
{
	unsigned char j;
	unsigned char read_data;
	unsigned short cnt = 100;
	read_data = 0;
	//if(device==RTC_CHIP)
	{
		sbi(RTC_SDA_PIN_DIR); //input
		cbi(RTC_SCL_PIN);
	}
	//else
	//{
	//	cbi(SDA_PIN_DIR); //input
	//	cbi(EEPROM_SCL_PIN);
	//}


	//cbi(EEPROM_CLOCK);
	delay10us(2);//increased delay from 4 to 12

	for(j=0;j<8;j++)
	{
		cnt = 100;
		while((cnt--) != 0)
		{
			//if(device==RTC_CHIP)
			{
				sbi(RTC_SCL_PIN);
				delay10us(4);    
				//if((P5OUT& 0x80)==0x80)
				if(0 != RTC_SCL_PIN_IN)
					break;
			}
			//else
			//{
			//	sbi(EEPROM_SCL_PIN);
			//	delay10us(4);    
			//	//if((P3OUT& 0x02)==0x02)
			//	if(0 != EEPROM_SCL_PIN_IN)
			//		break;
			//}
		}
		delay10us(4);
		read_data = (read_data << 1);
		//if(device==RTC_CHIP)
		{
			//if((P5IN & 0x01)==0x01)
			if(0 != RTC_SDA_PIN_IN)
				read_data = (read_data | 0x01);
			cbi(RTC_SCL_PIN);
		}
		//else
		//{
		//	//if((P3IN & 0x04)==0x04)
		//	if(0 != SDA_PIN_IN)
		//		read_data = (read_data | 0x01);
		//	cbi(EEPROM_SCL_PIN);
		//}              
		delay10us(4);
	}
	//if(device==RTC_CHIP)
	{
		cbi(RTC_SDA_PIN_DIR);	
	}
	//else
	//{
	//	sbi(SDA_PIN_DIR);
	//}
	delay10us(4);
	//if(device==RTC_CHIP)
	{
		if(ack)
			sbi(RTC_SDA_PIN);
		else
			cbi(RTC_SDA_PIN);
	}
	//else
	//
	//	if(ack)
	//		sbi(SDA_PIN);
	//	else
	//		cbi(SDA_PIN);
	//

	//if(device==RTC_CHIP)
		sbi(RTC_SCL_PIN);
	//else
	//	sbi(EEPROM_SCL_PIN);
	delay10us(4);
	cbi(RTC_SCL_PIN);
	//cbi(EEPROM_SCL_PIN);
	return read_data;	
}
//rakesh en


void set_rtc_time(unsigned char chour,unsigned char cminute, unsigned char csecond)
{
	//chour = convt_byte_to_bcd(chour);
	//cminute = convt_byte_to_bcd(cminute);
	//csecond = convt_byte_to_bcd(csecond);
	bstart(RTC_CHIP);
	tx_data(0x64,RTC_CHIP);
	tx_data(0,RTC_CHIP);			// to be verified

	tx_data(csecond,RTC_CHIP);
	tx_data(cminute,RTC_CHIP);
	tx_data(chour,RTC_CHIP);

	bstop(RTC_CHIP);
	delay1ms(250);
}

void set_rtc_date(unsigned char cday,unsigned char cmonth, unsigned char cyear)
{
	//cday = convt_byte_to_bcd(cday);
	//cmonth = convt_byte_to_bcd(cmonth);
	//cyear = convt_byte_to_bcd(cyear);

	bstart(RTC_CHIP);
	tx_data(0x64,RTC_CHIP);
	tx_data(4,RTC_CHIP);

	tx_data(cday,RTC_CHIP);
	tx_data(cmonth,RTC_CHIP);
	tx_data(cyear,RTC_CHIP);
	bstop(RTC_CHIP);
	delay1ms(250);
}

void rtc_write_byte(unsigned char addr, unsigned char data)
{
	bstart(RTC_CHIP);
	tx_data(0x64,RTC_CHIP);
	tx_data(addr,RTC_CHIP);

	tx_data(data,RTC_CHIP);
	bstop(RTC_CHIP);
	delay1ms(250);
}
unsigned char rtc_read_byte(unsigned char addr)
{
	unsigned char curr_byte;

	bstart(RTC_CHIP);
	tx_data(0x64,RTC_CHIP);  
	tx_data(addr,RTC_CHIP);  
	bstart(RTC_CHIP);
	tx_data(0x65,RTC_CHIP);  
	curr_byte= rx_data(1,RTC_CHIP); //read byte
	bstop(RTC_CHIP);
	delay1ms(1);

	return curr_byte;
}

void rtc_read_time(void)
{
	unsigned char tmp_byte;

	tmp_byte=rtc_read_byte(0);
	//tmp_byte=tmp_byte&0x7f;
	//t_sec=(tmp_byte & 0xf0)>>4;
	//t_sec=(t_sec*10)+(tmp_byte & 0x0f);
	//Harjeet 17072016... updated for keeping seconds even in all cases.. 
	//Just to match the communication structure of time in 4 bytes
	//t_sec_disp = t_sec;
	//if ((t_sec % 2) != 0)
	//{
	//	t_sec = t_sec - 1;
	//}
	t_sec=tmp_byte&0x7f;

	tmp_byte=rtc_read_byte(1);
	//tmp_byte=tmp_byte&0x7f;
	//t_min=(tmp_byte & 0xf0)>>4;
	//t_min=(t_min*10)+(tmp_byte & 0x0f);
	t_min=tmp_byte&0x7f;

	tmp_byte=rtc_read_byte(2);
	//tmp_byte=tmp_byte&0x3f;
	//t_hr=(tmp_byte & 0xf0)>>4;
	//t_hr=(t_hr*10)+(tmp_byte & 0x0f);
	//if(t_hr>=99)
	//	t_hr=99;
	t_hr=tmp_byte&0x3f;

	//real_time=t_hr;
	//real_time=(real_time*100)+t_min;
	//real_time=(real_time*100)+t_sec;
	return;
}

void rtc_read_date(void)
{
	unsigned char tmp_byte;

	tmp_byte=rtc_read_byte(4);
	//tmp_byte=tmp_byte&0x3f;
	//d_day=(tmp_byte & 0xf0)>>4;
	//d_day=(d_day*10)+(tmp_byte & 0x0f);
	d_day=tmp_byte&0x3f;

	tmp_byte=rtc_read_byte(5);
	//tmp_byte=tmp_byte&0x1f;
	//d_mnth=(tmp_byte & 0xf0)>>4;
	//d_mnth=(d_mnth*10)+(tmp_byte & 0x0f);
	d_mnth=tmp_byte&0x1f;

	tmp_byte=rtc_read_byte(6);
	//tmp_byte=tmp_byte&0x7f;
	//d_yr=(tmp_byte & 0xf0)>>4;
	//d_yr=(d_yr*10)+(tmp_byte & 0x0f);
	//if(d_yr>99)
	//	d_yr=99;
	d_yr=tmp_byte&0x7f;

	//real_date=d_yr;
	//real_date=(real_date*100)+d_mnth;
	//real_date=(real_date*100)+d_day;
	return;
}

unsigned char convt_byte_to_bcd(unsigned char byte_data)
{
	unsigned char bcd_data;
	bcd_data=byte_data/10;
	bcd_data=(bcd_data<<4)+(byte_data % 10);
	return bcd_data;
}

void RTC_Init(void)
{
	BYTE S;
	S = rtc_read_byte(0x0E);
	while ((0x02 & S) != 0)
	{ 
		rtc_write_byte(0x0E,0);
		S = rtc_read_byte(0x0E);
	}
	rtc_write_byte(0x0D,0);
	//rtc_write_byte(0x18,0x0C);

	//set_rtc_date(26,5,18);
	//set_rtc_time(17,1,0);

	rtc_read_time();
	rtc_read_date();
}