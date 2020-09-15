/**************************************************************************//**
 * @file     main.c
 * @version  V2.00
 * $Revision: 1 $
 * $Date: 14/12/08 11:49a $
 * @brief    Show the usage of GPIO interrupt function.
 * @note
 * Copyright (C) 2013 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include "NUC100Series.h"
#include <string.h>


#define PLL_CLOCK   50000000

/* Definition of GPIO Port Bits Used for Communication */

#define SPI_Master_CS     PC0
#define SPI_Master_SCK    PC1
#define SPI_Master_MISO   PC2
#define SPI_Master_MOSI   PC3

#define DIG1   PA0
#define DIG2   PA1
#define DIG3   PA2
#define DIG4   PA3

#define DIG1Fg   1
#define DIG2Fg   2
#define DIG3Fg   3
#define DIG4Fg   4

#define A   PA8
#define B   PA9
#define C   PA10
#define D   PA11
#define E   PA12
#define F   PA13
#define G   PA14
#define DP  PA15

#define number_0   0
#define number_1   1
#define number_2   2
#define number_3   3
#define number_4   4
#define number_5   5
#define number_6   6
#define number_7   7
#define number_8   8
#define number_9   9

#define _7Display_first_9xxx_ON      1
#define _7Display_first_9xxx_OFF     2

#define _7Display_second_x9xx_ON     3
#define _7Display_second_x9xx_OFF    4

#define _7Display_third_xx9x_ON      5
#define _7Display_third_xx9x_OFF     6

#define _7Display_fourth_xxx9_ON     7
#define _7Display_fourth_xxx9_OFF    8

#define _7Display_4DigitsON     9
#define _7Display_4DigitsOFF    10

#define WDI       PB15//pin43

#define AllValue   1
#define AllGain    2

#define CJT_resolution 256
#define LTC_resolution 4096

#define _1DIG  1
#define _2DIG  2


//============================================================


uint8_t GAu8_SPI_WriteBit[8] = {0};
uint8_t GAu8_SPI_WriteData[10] = {0};

uint8_t Gu8_SPI_ReadHex = 0;
uint8_t gu8A_SPI_ReadData[10] = {0};

//---

uint8_t Gu8_Rx0ReceiveEnable = 1;//need set 1
uint8_t GAu8_Rx0ReceiveData[2048] = {0};
uint16_t Gu16_Rx0ReceiveIndex = 0;
uint8_t Gu8_Rx0ReceiveCorrect = 0;


uint8_t Gu8_Tx0TransmitEnable = 0;
uint8_t GAu8_Tx0TransmitData[100] = {0};
uint8_t Gu8_Tx0TransmitIndex = 0;
uint8_t Gu8_Tx0TransmitEnd = 0;
uint8_t Gu8_Tx0ContentSelect = 0;

//---

uint8_t Gu8_Rx1ReceiveEnable = 1;//need set 1
uint8_t GAu8_Rx1ReceiveData[2048] = {0};
uint16_t Gu16_Rx1ReceiveIndex = 0;
uint8_t Gu8_Rx1ReceiveCorrect = 0;
uint8_t GAu8_Rx1ModBusCRC16Content[10] = {0};
uint16_t Gu16_Rx1ModBusCRC16Value = 0;


uint8_t Gu8_Tx1TransmitEnable = 0;
uint8_t GAu8_Tx1TransmitData[500] = {0};
uint16_t Gu16_Tx1TransmitIndex = 0;
uint16_t Gu16_Tx1TransmitEnd = 0;
uint8_t Gu8_Tx1ContentSelect = 0;
uint8_t GAu8_Tx1ModBusCRC16Content[100] = {0};
uint16_t Gu16_Tx1ModBusCRC16Value = 0;

//---
	
uint32_t Gu32_X_xR25200 = 0;//base
uint32_t Gu32_Y_xR25200 = 0;//rate
	  
	
uint32_t Gu32_X_xR249k = 0;//base
uint32_t Gu32_Y_xR249k = 0;//rate


//---

uint8_t gu8_startUpPoll_7Display = 0;
uint8_t gu8_poll4DIG = 0;
uint8_t gu8_first_9xxx = 0;
uint8_t gu8_second_x9xx = 0;
uint8_t gu8_third_xx9x = 0;
uint8_t gu8_fourth_xxx9 = 0;


uint32_t gu32_CJT_buffer = 0;
uint32_t gu32_LTC_buffer = 0;

typedef struct
{
	  uint8_t Data_MSB;
    uint8_t Data;
	  uint8_t Data_LSB;
	
	  uint8_t CanRead:1;
	  uint8_t ReadFinish:1;
	
}_ADS1220Adc;
_ADS1220Adc Gu8_ADS1220Adc;


uint32_t Gu32_ADS1220_24BitBuffer = 0;


typedef struct
{
    uint16_t startUp_7Display_ms;
	  uint16_t DIGwait_ms;
	  uint16_t STWD100NYN_WDI_ms;
    uint16_t pollTEMP_7display_ms;
	
	  uint8_t startUp_7Display:1;
	  uint8_t DIGwait:1;
    uint8_t STWD100NYN_WDI:1;
    uint8_t pollTEMP_7display:1;
   
}_Timer0;
_Timer0 gu8_timer0;
_Timer0 gu8_timer0Run;
_Timer0 gu8fg_timeOut;

typedef struct
{
    uint32_t X_xR249k;
    uint32_t Y_xR249k;

    uint32_t X_xR25200;	
	  uint32_t Y_xR25200;
	
}_Flash;
_Flash Gu32_Flash;

typedef struct
{
    uint8_t gu8_CR0;
	  uint8_t gu8_CR1;
	
}_MAX31856register;
_MAX31856register MAX31856register;

union MAX31856_cold_junction_temprature
{
    uint32_t gu32_CJT_value;
    uint8_t gu8_CJT[2];
	
}MAX31856CJT;

typedef union
{
    uint32_t gu32_LTC_value;
	  uint8_t  gu8_LTC[3];
	
}MAX31856_linearized_thermocouple_temprature;
MAX31856_linearized_thermocouple_temprature MAX31856LTC;

typedef struct
{
    uint8_t decimalPoint;
    
	   
}_4DIG_7DISPLAY;
_4DIG_7DISPLAY _4DIG_7display;

//=============================================================

void GPCDEF_IRQHandler(void);
void GPIO_Init(void);
void UART0_Init(void);
void UART1_Init(void);
void Timer_Init(void);
void WatchDog_Init(void);
void delayTime(uint32_t);

void SPI_WriteByte(uint8_t);
void SPI_WriteBlock(uint8_t * , uint16_t);

uint8_t SPI_ReadByte(void);
void SPI_ReadBlock(uint8_t * , uint16_t);

void SPI_Master_Init(void);

void UART0_Handle(void);
void UART1_Handle(void);
void Timer0_Handle(void);

void Rx1ModBusCRC16_Fillin_Buffer(void);

uint16_t ModBus_CRC16(uint8_t* , uint16_t);

void Flash_Erase(void);
void Flash_XY_Read(void);
void Flash_XY_Write(void);

void MAX31856_CR0_registerWrite(void);
void MAX31856_CR0_registerRead(void);
void MAX31856_CR1_registerWrite(void);
void MAX31856_CR1_registerRead(void);
void MAX31856_CJTH_and_CJTL_registerWrite(void);
void MAX31856_CJTH_and_CJTL_registerRead(void);
void MAX31856_LTCBH_LTCBM_and_LTCBL_registerRead(void);
void LTC_realValue(void);

void digitsDisplay(void);
void numberSelect(uint8_t);
void poll4DIG(void);

void _7Display_0(void);
void _7Display_1(void);
void _7Display_2(void);
void _7Display_3(void);
void _7Display_4(void);
void _7Display_5(void);
void _7Display_6(void);
void _7Display_7(void);
void _7Display_8(void);
void _7Display_9(void);
void _7Display_AllOFF(void);


unsigned char GAu8_ModBusCRCHi[]=  
{  
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,  
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,  
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,  
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,  
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,  
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,  
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,  
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,  
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,  
0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,  
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,  
0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,  
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,  
0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,  
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,  
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,  
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,  
0x40  
};

unsigned char GAu8_ModBusCRCLo[]=  
{  
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,  
0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,  
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,  
0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,  
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,  
0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,  
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,  
0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,  
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,  
0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,  
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,  
0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,  
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,  
0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,  
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,  
0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,  
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,  
0x40  
};


uint16_t ModBus_CRC16(uint8_t *UpData , uint16_t Length)  
{  
    uint8_t Lu8_CRCHi=0xff;  
    uint8_t Lu8_CRCLo=0xff;  
    uint8_t Lu8_Index;
  
    while(Length --)  
    {  
        Lu8_Index = Lu8_CRCHi ^ *UpData ++;  
        Lu8_CRCHi = Lu8_CRCLo ^ GAu8_ModBusCRCHi[Lu8_Index];  
        Lu8_CRCLo = GAu8_ModBusCRCLo[Lu8_Index];  
    }  
    return (Lu8_CRCHi << 8 | Lu8_CRCLo);  
}




void SYS_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/

	  /* Enable Internal RC 22.1184MHz clock */
    CLK_EnableXtalRC(CLK_PWRCON_OSC22M_EN_Msk);

    /* Waiting for Internal RC clock ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_OSC22M_STB_Msk);

    /* Switch HCLK clock source to Internal RC and HCLK source divide 1 */
    CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_HIRC, CLK_CLKDIV_HCLK(1));
	
	
    CLK_EnableXtalRC(CLK_PWRCON_XTL12M_EN_Msk);
			
	  CLK_WaitClockReady(CLK_CLKSTATUS_XTL12M_STB_Msk);
			
		CLK_SetCoreClock(PLL_CLOCK);
	
	  //---
			
		CLK_EnableModuleClock(UART0_MODULE);//UART0 clock on
			
		CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART_S_HIRC, CLK_CLKDIV_UART(1));//UART0 select external crystal
	
	  CLK_EnableModuleClock(UART1_MODULE);//UART1 clock on
	
	  CLK_SetModuleClock(UART1_MODULE, CLK_CLKSEL1_UART_S_HIRC, CLK_CLKDIV_UART(1));//UART1 select external crystal
	
	  //---
	
	  CLK_EnableModuleClock(TMR0_MODULE);//Timer clock on
	  
//	  CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0_S_HXT, NULL);//Timer0 select external crystal
		
		CLK_SetModuleClock(TMR2_MODULE, CLK_CLKSEL1_TMR0_S_HIRC, NULL);//Timer0 select internal 22.1184MHz
		
		//---
		
		CLK_EnableModuleClock(WDT_MODULE);//Enable WDT module clock    
        
    CLK_SetModuleClock(WDT_MODULE, CLK_CLKSEL1_WDT_S_LIRC, NULL);//Select WDT module clock source
		
		//---
}


/*---------------------------------------------------------------------------------------------------------*/
/* MAIN function                                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
int main(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Init System, peripheral clock and multi-function I/O */
    SYS_Init();
	
//	  SYS_LockReg();
	  
	  GPIO_Init();              //MCU GPIO pin define
	
	  UART0_Init();             //MCU UART0 pin define
	  UART1_Init();             //MCU UART1 pin define
	
	  Timer_Init();             //MCU Timer define
	  WatchDog_Init();          //MCU WatchDog define
	
	  SPI_Master_Init();        //MCU SPI pin define

//	  UART_EnableInt(UART0, (UART_IER_RDA_IEN_Msk | UART_IER_THRE_IEN_Msk | UART_IER_TOUT_IEN_Msk));
    UART_EnableInt(UART0, UART_IER_RDA_IEN_Msk);
		UART_EnableInt(UART1, UART_IER_RDA_IEN_Msk);
		
		//--------------------------------
		
		Flash_XY_Read();
		
		gu8_timer0Run.startUp_7Display = 1;
		gu8_startUpPoll_7Display = _7Display_first_9xxx_ON;
		
		
		MAX31856_CR0_registerWrite();
		MAX31856_CR1_registerWrite();
		
		for(;;)
		{
		    WDT_RESET_COUNTER();
			  
			  if(gu8fg_timeOut.STWD100NYN_WDI == 1)//WatchDog
				{
					  gu8fg_timeOut.STWD100NYN_WDI = 0;
					  WDI = !WDI;
				}
				
				MAX31856_CR0_registerRead();
				MAX31856_CR1_registerRead();
				
				MAX31856_CJTH_and_CJTL_registerRead();
				MAX31856_LTCBH_LTCBM_and_LTCBL_registerRead();
				
				gu32_LTC_buffer = (MAX31856LTC.gu32_LTC_value*100) / LTC_resolution;
				gu32_CJT_buffer = (MAX31856CJT.gu32_CJT_value*100) / CJT_resolution;
				
				//=====judge pos neg=====
				if((MAX31856LTC.gu32_LTC_value & 0x00080000) == 1)//negtive temp
				{
					  if(gu8fg_timeOut.pollTEMP_7display == 1)
						{
						    gu8fg_timeOut.pollTEMP_7display = 0;
						    gu32_LTC_buffer = (~gu32_LTC_buffer) + 1;
					      LTC_realValue();
						}
				}
				else if((MAX31856LTC.gu32_LTC_value & 0x00080000) == 0)//positive temp
				{
				    if(gu8fg_timeOut.pollTEMP_7display == 1)
						{
						    gu8fg_timeOut.pollTEMP_7display = 0;
							  LTC_realValue();
						}
				}
				//=====END=====
				
				//=====7display show temprature=====
				if(gu8_timer0Run.startUp_7Display == 0)
				{
				    digitsDisplay();
					  poll4DIG();
				}
				//=====END=====
				
				//=====initial 7display test=====
				if((gu8_timer0Run.startUp_7Display == 1) && (gu8fg_timeOut.startUp_7Display == 1))
				{
					  gu8fg_timeOut.startUp_7Display = 0;
					
				    switch (gu8_startUpPoll_7Display)
						{
						    case _7Display_first_9xxx_ON:
						       DIG1 = 0;
								   DIG2 = 1;
								   DIG3 = 1;
								   DIG4 = 1;
			             _7Display_8();
							     DP = 1;
								   gu8_startUpPoll_7Display = _7Display_first_9xxx_OFF;
						    break;
								
								
								case _7Display_first_9xxx_OFF:
									 DIG1 = 1;
								   DIG2 = 1;
								   DIG3 = 1;
								   DIG4 = 1;
			             _7Display_AllOFF();
							     DP = 0;
								   gu8_startUpPoll_7Display = _7Display_second_x9xx_ON;
								break;
								
								
								case _7Display_second_x9xx_ON:
								   DIG1 = 1;
								   DIG2 = 0;
								   DIG3 = 1;
								   DIG4 = 1;
			             _7Display_8();
							     DP = 1;
								   gu8_startUpPoll_7Display = _7Display_second_x9xx_OFF;
								break;
								
								
								case _7Display_second_x9xx_OFF:
									 DIG1 = 1;
								   DIG2 = 1;
								   DIG3 = 1;
								   DIG4 = 1;
			             _7Display_AllOFF();
							     DP = 0;
								   gu8_startUpPoll_7Display = _7Display_third_xx9x_ON;
								break;
								
								
								case _7Display_third_xx9x_ON:
									 DIG1 = 1;
								   DIG2 = 1;
									 DIG3 = 0;
								   DIG4 = 1;
			             _7Display_8();
							     DP = 1;
								   gu8_startUpPoll_7Display = _7Display_third_xx9x_OFF;
								break;
								
								
								case _7Display_third_xx9x_OFF:
									 DIG1 = 1;
								   DIG2 = 1;
								   DIG3 = 1;
								   DIG4 = 1;
			             _7Display_AllOFF();
							     DP = 0;
								   gu8_startUpPoll_7Display = _7Display_fourth_xxx9_ON;
								break;
								
								
								case _7Display_fourth_xxx9_ON:
									 DIG1 = 1;
								   DIG2 = 1;
								   DIG3 = 1;
								   DIG4 = 0;
			             _7Display_8();
							     DP = 1;
								   gu8_startUpPoll_7Display = _7Display_fourth_xxx9_OFF;
								break;
								
								
								case _7Display_fourth_xxx9_OFF:
									 DIG1 = 1;
								   DIG2 = 1;
								   DIG3 = 1;
									 DIG4 = 1;
			             _7Display_AllOFF();
							     DP = 0;
								   gu8_startUpPoll_7Display = _7Display_4DigitsON;
								break;
								
								
								case _7Display_4DigitsON:
									 DIG1 = 0;
								   DIG2 = 0;
								   DIG3 = 0;
								   DIG4 = 0;
								   _7Display_8();
								   DP = 1;
								   gu8_startUpPoll_7Display = _7Display_4DigitsOFF;
								break;
								   
								
								case _7Display_4DigitsOFF:
									 DIG1 = 1;
								   DIG2 = 1;
								   DIG3 = 1;
								   DIG4 = 1;
								   _7Display_AllOFF();
								   DP = 0;

								   gu8_timer0Run.startUp_7Display = 0;							   
								break;
						}
				}
				//=====END=====
				
				
        //=====Tx0 fill in content=====
        if(Gu8_Rx0ReceiveCorrect == 1)
		    {
		        Gu8_Rx0ReceiveCorrect = 0;
		        Gu8_Rx0ReceiveEnable = 0;//Rx OFF
			      
					  switch(Gu8_Tx0ContentSelect)
						{
						    case AllValue:
									GAu8_Tx0TransmitData[0] = 0x02;
								  
									GAu8_Tx0TransmitData[40] = 0x03;
									
									Gu8_Tx0TransmitIndex = 0;//start transmit from 0
			            Gu8_Tx0TransmitEnd = 41;//need transmit length 41 byte
			            Gu8_Tx0TransmitEnable = 1;//Tx ON
								break;		
								
                case AllGain:
								  GAu8_Tx0TransmitData[0] = 0x02;
		              
					        GAu8_Tx0TransmitData[82] = 0x03;
			  
			            Gu8_Tx0TransmitIndex = 0;//start transmit from 0
			            Gu8_Tx0TransmitEnd = 83;//need transmit length 83 byte
			            Gu8_Tx0TransmitEnable = 1;//Tx ON
								break;

						}   
				}   
				//=====End=====
				
				//=====Tx0 Transmit=====
				if(Gu8_Tx0TransmitEnable == 1)
        {
		        if(Gu8_Tx0TransmitIndex < Gu8_Tx0TransmitEnd)
				    {
				        UART_WRITE(UART0, GAu8_Tx0TransmitData[Gu8_Tx0TransmitIndex]);
							  while(UART_IS_TX_FULL(UART0)){}
				        Gu8_Tx0TransmitIndex ++;
				    
				        if(Gu8_Tx0TransmitIndex >= Gu8_Tx0TransmitEnd)
				        {
					          Gu8_Tx0TransmitEnable = 0;//Tx OFF
									
									  while(UART_IS_TX_EMPTY(UART0) == 0){}
					          Gu8_Rx0ReceiveEnable = 1;//Rx ON 
					  
					          Gu8_Tx0TransmitIndex = 0;//clear array index 
					          Gu8_Tx0TransmitEnd = 0;//clear transmit length
					  
					          memset(GAu8_Tx0TransmitData , 0x00 , sizeof(GAu8_Tx0TransmitData));
								}
						}
				}
				//=====End=====
				
        //=====Tx1 fill in content=====
				if(Gu8_Rx1ReceiveCorrect == 1)
				{
				    Gu8_Rx1ReceiveCorrect = 0;
		        Gu8_Rx1ReceiveEnable = 0;//Rx OFF
					
					  
				}   
				//=====End=====
				
				//=====Tx1 Transmit=====
				if(Gu8_Tx1TransmitEnable == 1)
				{
		        if(Gu16_Tx1TransmitIndex < Gu16_Tx1TransmitEnd)
				    {	
				        UART_WRITE(UART1, GAu8_Tx1TransmitData[Gu16_Tx1TransmitIndex]);
							  while (UART_IS_TX_FULL(UART1)){}
				        Gu16_Tx1TransmitIndex ++;
				    
				        if(Gu16_Tx1TransmitIndex >= Gu16_Tx1TransmitEnd)
				        {
					          Gu8_Tx1TransmitEnable = 0;//Tx OFF
									  
									  while(UART_IS_TX_EMPTY(UART1) == 0){}
										
					          Gu8_Rx1ReceiveEnable = 1;//Rx ON	
								
					          Gu16_Tx1TransmitIndex = 0;//clear array index 
					          Gu16_Tx1TransmitEnd = 0;//clear transmit length
					  
					          memset(GAu8_Tx1TransmitData , 0x00 , sizeof(GAu8_Tx1TransmitData));
								}
						}				  
				}
				//=====End=====
		}   
}

//=================================================MCU Interrupt==============================================
void GPCDEF_IRQHandler(void)
{
    /* To check if PE.5 interrupt occurred */
    if(GPIO_GET_INT_FLAG(PE, BIT5))//DRDY fall edge interrupt
    {   
        GPIO_CLR_INT_FLAG(PE, BIT5);
			
		    Gu8_ADS1220Adc.CanRead = 1;	
    }
    else
    {
        /* Un-expected interrupt. Just clear all PC, PD, PE and PF interrupts */
        PC->ISRC = PC->ISRC;
        PD->ISRC = PD->ISRC;
        PE->ISRC = PE->ISRC;//ADS1220_DRDY(PE5) fall edge interrupt
        PF->ISRC = PF->ISRC;
    }
}



void TMR0_IRQHandler(void)
{
    if(TIMER_GetIntFlag(TIMER0) == 1)
    {
        /* Clear Timer0 time-out interrupt flag */
        TIMER_ClearIntFlag(TIMER0);
			
        Timer0_Handle();
    }
}



void Timer0_Handle(void)
{
	  if(gu8_timer0.STWD100NYN_WDI_ms > 5000)
		{
		    gu8fg_timeOut.STWD100NYN_WDI = 1;//time out
			  gu8_timer0.STWD100NYN_WDI_ms = 0;//clear
		}
		else 
		{
		    gu8_timer0.STWD100NYN_WDI_ms ++;
		}
		
		//=====
		
		if(gu8_timer0Run.startUp_7Display == 1)
		{
		    if(gu8_timer0.startUp_7Display_ms > 100)
				{
					  gu8fg_timeOut.startUp_7Display = 1;//time out
					  gu8_timer0.startUp_7Display_ms = 0;//clear
				}
				else 
				{
					  gu8_timer0.startUp_7Display_ms ++;
				}
		}
		
		//=====
		
		if(gu8_timer0.DIGwait_ms > 1)// 1/16 = 0.0625s = 62.5ms  need <62.5ms
		{
		    gu8fg_timeOut.DIGwait = 1;//time out
				gu8_timer0.DIGwait_ms = 0;//clear
		}
		else 
		{
		    gu8_timer0.DIGwait_ms ++;
		}
		
		//=====
		
		if(gu8_timer0.pollTEMP_7display_ms > 100)
		{
		    gu8fg_timeOut.pollTEMP_7display = 1;//time out
			  gu8_timer0.pollTEMP_7display_ms = 0;//clear
		}
		else
		{
		    gu8_timer0.pollTEMP_7display_ms ++;
		}
			
}



void UART02_IRQHandler(void)
{
    UART0_Handle();
}



void UART0_Handle()
{
 	  uint32_t u32IntSts = UART0->ISR;
     
    if((u32IntSts & UART_ISR_RDA_INT_Msk) && (Gu8_Rx0ReceiveEnable == 1))
    {
		 	  while(UART_IS_RX_READY(UART0))
			  {
				    GAu8_Rx0ReceiveData[Gu16_Rx0ReceiveIndex] = UART_READ(UART0);
				    
				    
						
						
						if(Gu16_Rx0ReceiveIndex < 2047)
						{
						    Gu16_Rx0ReceiveIndex = (Gu16_Rx0ReceiveIndex == 2046) ? 0 : (Gu16_Rx0ReceiveIndex + 1);
						}
				}
		}
}



void UART1_IRQHandler(void)
{
    UART1_Handle();
}



void UART1_Handle(void)
{
    uint32_t u32IntSts = UART1->ISR;
	
	  if((u32IntSts & UART_ISR_RDA_INT_Msk) && (Gu8_Rx1ReceiveEnable == 1))
		{
		    while(UART_IS_RX_READY(UART1))
				{
				    GAu8_Rx1ReceiveData[Gu16_Rx1ReceiveIndex] = UART_READ(UART1);
					  
			      //---
						
						if(Gu16_Rx1ReceiveIndex < 2047)
						{                                          
							  if(Gu16_Rx1ReceiveIndex == 2046)
								{
								    memset(GAu8_Rx1ReceiveData , 0x00 , sizeof(GAu8_Rx1ReceiveData));  //Gu8_Rx1ReceiveIndex = (Gu8_Rx1ReceiveIndex == 17) ? 0 : (Gu8_Rx1ReceiveIndex + 1);
								    Gu16_Rx1ReceiveIndex = 0;   									
								}
								else if(Gu16_Rx1ReceiveIndex < 2046)
								{
								    Gu16_Rx1ReceiveIndex ++;
								}
						}
				}
		}
}



void Rx1ModBusCRC16_Fillin_Buffer(void)
{
    GAu8_Rx1ModBusCRC16Content[0] = GAu8_Rx1ReceiveData[Gu16_Rx1ReceiveIndex - 4];
		GAu8_Rx1ModBusCRC16Content[1] = GAu8_Rx1ReceiveData[Gu16_Rx1ReceiveIndex - 3];
							
		Gu16_Rx1ModBusCRC16Value = GAu8_Rx1ReceiveData[Gu16_Rx1ReceiveIndex - 2];
		Gu16_Rx1ModBusCRC16Value = (Gu16_Rx1ModBusCRC16Value << 8) + GAu8_Rx1ReceiveData[Gu16_Rx1ReceiveIndex - 1];
}


//===================================================End=====================================================================



//=====================================================MCU Init==============================================================
void GPIO_Init(void)
{
    GPIO_SetMode(PE, BIT5, GPIO_PMD_INPUT);//DRDY of ADS1220
	  GPIO_EnableInt(PE, 5, GPIO_INT_FALLING);//PE5 interrupt fall edge
    NVIC_EnableIRQ(GPCDEF_IRQn);//Enable GPIO interrupt
	  
	  
	  
    GPIO_SetMode(PC, BIT0, GPIO_PMD_OUTPUT);//SPI_CS
	  GPIO_SetMode(PC, BIT1, GPIO_PMD_OUTPUT);//SPI_SCK
	  GPIO_SetMode(PC, BIT2, GPIO_PMD_INPUT);//SPI_MISO
	  GPIO_SetMode(PC, BIT3, GPIO_PMD_OUTPUT);//SPI_MOSI
	  
	  
	  
	  /* Set GPB multi-function pins for UART0 RXD and TXD */
    SYS->GPB_MFP &= ~(SYS_GPB_MFP_PB0_Msk | SYS_GPB_MFP_PB1_Msk);
    SYS->GPB_MFP |= (SYS_GPB_MFP_PB0_UART0_RXD | SYS_GPB_MFP_PB1_UART0_TXD);
	  
	  
	  /* Set GPB multi-function pins for UART1 RXD and TXD */
	  SYS->GPB_MFP &= ~(SYS_GPB_MFP_PB4_Msk | SYS_GPB_MFP_PB5_Msk);
	  SYS->GPB_MFP |= (SYS_GPB_MFP_PB4_UART1_RXD | SYS_GPB_MFP_PB5_UART1_TXD);
	  
	  GPIO_SetMode(PA, BIT0, GPIO_PMD_OUTPUT);//DIG1
	  GPIO_SetMode(PA, BIT1, GPIO_PMD_OUTPUT);//DIG2
	  GPIO_SetMode(PA, BIT2, GPIO_PMD_OUTPUT);//DIG3
	  GPIO_SetMode(PA, BIT3, GPIO_PMD_OUTPUT);//DIG4
	
	  GPIO_SetMode(PA, BIT8, GPIO_PMD_OUTPUT); //A
		GPIO_SetMode(PA, BIT9, GPIO_PMD_OUTPUT); //B
		GPIO_SetMode(PA, BIT10, GPIO_PMD_OUTPUT);//C
		GPIO_SetMode(PA, BIT11, GPIO_PMD_OUTPUT);//D
		GPIO_SetMode(PA, BIT12, GPIO_PMD_OUTPUT);//E
		GPIO_SetMode(PA, BIT13, GPIO_PMD_OUTPUT);//F
		GPIO_SetMode(PA, BIT14, GPIO_PMD_OUTPUT);//G
		GPIO_SetMode(PA, BIT15, GPIO_PMD_OUTPUT);//DP
		
		GPIO_SetMode(PB, BIT15, GPIO_PMD_OUTPUT);//WDI	
}



void UART0_Init(void)
{
    SYS_ResetModule(UART0_RST);//Reset UART0

    UART_Open(UART0, 19200);//Configure UART0 and set UART0 Baudrate
}



void UART1_Init(void)
{
    SYS_ResetModule(UART1_RST);//Reset UART1
	
	  UART_Open(UART1, 19200);//Configure UART1 and set UART1 Baudrate
}	



void Timer_Init(void)
{
    TIMER_Open(TIMER0, TIMER_PERIODIC_MODE, 1000);//Timer0¡Aperiod mode¡A1000 times interrput per seconds¡C(period = 1ms)
    TIMER_EnableInt(TIMER0);
	
	  NVIC_EnableIRQ(TMR0_IRQn);//Enable Timer0 Interrupt
	  TIMER_Start(TIMER0);//start timer0
}



void WatchDog_Init(void)
{
    WDT_Open(WDT_TIMEOUT_2POW14, WDT_RESET_DELAY_1026CLK, TRUE, FALSE);
}
//=======================================================End======================================================================



//=====================================Common FUNC=================================================
void delayTime(uint32_t Lu32_Loop)
{
    uint32_t i = 0;
	  for(i=0 ; i<Lu32_Loop ; i++){}
}



void SPI_WriteByte(uint8_t lu8_Byte)
{
    int8_t i;
	
	  GAu8_SPI_WriteBit[7] = ((lu8_Byte & 0x80) >> 7);
	  GAu8_SPI_WriteBit[6] = ((lu8_Byte & 0x40) >> 6);
	  GAu8_SPI_WriteBit[5] = ((lu8_Byte & 0x20) >> 5);
	  GAu8_SPI_WriteBit[4] = ((lu8_Byte & 0x10) >> 4);
	
	  GAu8_SPI_WriteBit[3] = ((lu8_Byte & 0x08) >> 3);
	  GAu8_SPI_WriteBit[2] = ((lu8_Byte & 0x04) >> 2);
	  GAu8_SPI_WriteBit[1] = ((lu8_Byte & 0x02) >> 1);
	  GAu8_SPI_WriteBit[0] = ((lu8_Byte & 0x01) >> 0);
	  
	  for(i=7 ; i>=0 ; i--)
	  {
		    SPI_Master_SCK = 0;//SCK
		
		    SPI_Master_MOSI = GAu8_SPI_WriteBit[i];//MOSI
		    delayTime(10);
		    SPI_Master_SCK = 1;//SCK
		    delayTime(10);
			
		    SPI_Master_SCK = 0;//SCK;
		    delayTime(10);
		}
}



void SPI_WriteBlock(uint8_t *lpu8_Block , uint16_t lu16_Length)
{
 	  int16_t i;
	
	  for(i=0 ; i<lu16_Length ; i++)
	  {
		    SPI_WriteByte(lpu8_Block[i]);
			  delayTime(30);
		}		
}



uint8_t SPI_ReadByte(void)
{
    int16_t i;
	
	  for(i=0 ; i<8 ; i++)
	  {
		    SPI_Master_SCK = 0;
		    delayTime(10);
		    SPI_Master_SCK = 1;
			
			  Gu8_SPI_ReadHex = (Gu8_SPI_ReadHex << 1) | SPI_Master_MISO;
			
		    delayTime(10);
		    SPI_Master_SCK = 0;
		    delayTime(10);
		}
	  return Gu8_SPI_ReadHex;
}



void SPI_ReadBlock(uint8_t *lpu8_Block , uint16_t lu16_Length)
{
 	  int16_t i;
	  
	  for(i=0 ; i<lu16_Length ; i++)
	  {
		    lpu8_Block[i] = SPI_ReadByte();
		 	  delayTime(30);
		}
}



void SPI_Master_Init(void)
{
    SPI_Master_CS   = 1;//SPI_CS
	  SPI_Master_SCK  = 0;//SPI_SCK
	  SPI_Master_MISO = 0;//SPI_MISO
	  SPI_Master_MOSI = 0;//SPI_MOSI	
}



void ADS1220ReadAdcData(void)
{
    SPI_Master_CS = 0;//SPI_CS Low
	  SPI_ReadBlock(gu8A_SPI_ReadData, 3);
	  SPI_Master_CS = 1;//SPI_CS High
	
	  Gu8_ADS1220Adc.Data_MSB = gu8A_SPI_ReadData[0];
	  Gu8_ADS1220Adc.Data     = gu8A_SPI_ReadData[1];
	  Gu8_ADS1220Adc.Data_LSB = gu8A_SPI_ReadData[2];
	
		Gu32_ADS1220_24BitBuffer = Gu8_ADS1220Adc.Data_MSB;
		Gu32_ADS1220_24BitBuffer = (Gu32_ADS1220_24BitBuffer << 8) + Gu8_ADS1220Adc.Data;
		Gu32_ADS1220_24BitBuffer = (Gu32_ADS1220_24BitBuffer << 8) + Gu8_ADS1220Adc.Data_LSB;
}

//=====

void MAX31856_CR0_registerWrite(void)
{
    GAu8_SPI_WriteData[0] = 0x80;//CR0
    GAu8_SPI_WriteData[1] = 0x80;//Automatic Conversion mode
	  
    SPI_Master_CS = 0;//SPI_CS Low
    SPI_WriteBlock(GAu8_SPI_WriteData, 2);
 	  SPI_Master_CS = 1;//SPI_CS High 
}

void MAX31856_CR0_registerRead(void)
{
    SPI_Master_CS = 0;//SPI_CS Low
	  SPI_WriteByte(0x00);//CR0 read
	  delayTime(10);
	  SPI_ReadBlock(gu8A_SPI_ReadData, 1);
	  SPI_Master_CS = 1;//SPI_CS High
	  
	  MAX31856register.gu8_CR0 = gu8A_SPI_ReadData[0];
}

void MAX31856_CR1_registerWrite(void)
{
    GAu8_SPI_WriteData[0] = 0x81;//CR1
    GAu8_SPI_WriteData[1] = 0x43;//16 samples averaged, K
	  
    SPI_Master_CS = 0;//SPI_CS Low
    SPI_WriteBlock(GAu8_SPI_WriteData, 2);
 	  SPI_Master_CS = 1;//SPI_CS High 
}

void MAX31856_CR1_registerRead(void)
{
    SPI_Master_CS = 0;//SPI_CS Low
	  SPI_WriteByte(0x01);//CR1 read
	  delayTime(10);
	  SPI_ReadBlock(gu8A_SPI_ReadData, 1);
	  SPI_Master_CS = 1;//SPI_CS High
	  
	  MAX31856register.gu8_CR1 = gu8A_SPI_ReadData[0];
}

void MAX31856_CJTH_and_CJTL_registerWrite(void)
{
    GAu8_SPI_WriteData[0] = 0x8A;
    GAu8_SPI_WriteData[1] = 0x00;
    GAu8_SPI_WriteData[2] = 0x00;
	  
    SPI_Master_CS = 0;//SPI_CS Low
    SPI_WriteBlock(GAu8_SPI_WriteData, 3);
 	  SPI_Master_CS = 1;//SPI_CS High
}

void MAX31856_CJTH_and_CJTL_registerRead(void)
{
    SPI_Master_CS = 0;//SPI_CS Low
	  SPI_WriteByte(0x0A);
	  delayTime(10);
	  SPI_ReadBlock(gu8A_SPI_ReadData, 2);
	  SPI_Master_CS = 1;//SPI_CS High
	  
	  MAX31856CJT.gu8_CJT[1] = gu8A_SPI_ReadData[0];//CJTH
	  MAX31856CJT.gu8_CJT[0] = gu8A_SPI_ReadData[1];//CJTL
}

void MAX31856_LTCBH_LTCBM_and_LTCBL_registerRead(void)
{
    SPI_Master_CS = 0;//SPI_CS Low
	  SPI_WriteByte(0x0C);
	  delayTime(10);
	  SPI_ReadBlock(gu8A_SPI_ReadData, 3);
	  SPI_Master_CS = 1;//SPI_CS High
	  
	  MAX31856LTC.gu8_LTC[2] = gu8A_SPI_ReadData[0];//LTCBH
	  MAX31856LTC.gu8_LTC[1] = gu8A_SPI_ReadData[1];//LTCBM
	  MAX31856LTC.gu8_LTC[0] = gu8A_SPI_ReadData[2];//LTCBL
}

void LTC_realValue(void)
{
    if(gu32_LTC_buffer >= 10000)//999.9
		{
		    gu8_fourth_xxx9 = (gu32_LTC_buffer / 10) % 10;    //xxx.9
			  gu8_third_xx9x  = (gu32_LTC_buffer / 100) % 10;   //xx9.x
			  gu8_second_x9xx = (gu32_LTC_buffer / 1000) % 10;  //x9x.x
			  gu8_first_9xxx  = gu32_LTC_buffer / 10000;        //9xx.x
		    _4DIG_7display.decimalPoint = _1DIG;
		}
   	else if(gu32_LTC_buffer < 10000)//99.99
    {
		    gu8_fourth_xxx9 = gu32_LTC_buffer % 10;          //xx.x9
			  gu8_third_xx9x  = (gu32_LTC_buffer / 10) % 10;   //xx.9x
			  gu8_second_x9xx = (gu32_LTC_buffer / 100) % 10;  //x9.xx
			  gu8_first_9xxx  = gu32_LTC_buffer / 1000;        //9x.xx
			  _4DIG_7display.decimalPoint = _2DIG;
		}
}

void digitsDisplay(void)
{
    switch (gu8_poll4DIG)
	  {
		    case DIG1Fg :
		       DIG1 = 0;
					 DIG2 = 1;
					 DIG3 = 1;
					 DIG4 = 1;
				   numberSelect(gu8_first_9xxx);				
	         DP = 0;
	      break;
				
				
				case DIG2Fg :
					 DIG1 = 1;
           DIG2 = 0;
           DIG3 = 1;
				   DIG4 = 1;
           numberSelect(gu8_second_x9xx);
				   
				   if(_4DIG_7display.decimalPoint == _1DIG)     {DP = 0;}
           else if(_4DIG_7display.decimalPoint == _2DIG){DP = 1;}				
	      break;
				
				
				case DIG3Fg :
					 DIG1 = 1;
           DIG2 = 1;
           DIG3 = 0;
				   DIG4 = 1;
				   numberSelect(gu8_third_xx9x);
				   
				   if(_4DIG_7display.decimalPoint == _1DIG)     {DP = 1;}
           else if(_4DIG_7display.decimalPoint == _2DIG){DP = 0;}
				break;
				
				
				case DIG4Fg :
			     DIG1 = 1;
           DIG2 = 1;
           DIG3 = 1;
				   DIG4 = 0;
				   numberSelect(gu8_fourth_xxx9);
					 DP = 0;
				break;
		}   
}

void numberSelect(uint8_t lu8_numberSelect)
{
    switch (lu8_numberSelect)
		{
		    case number_0:
				   _7Display_0();
				break;
							 
				case number_1:
					 _7Display_1();
				break;
							 
				case number_2:
					 _7Display_2();
        break;
               
        case number_3:
           _7Display_3();
        break;
               
        case number_4:
           _7Display_4();
        break;

        case number_5:
           _7Display_5();
        break;
							 
				case number_6:
				   _7Display_6();
				break;
							 
				case number_7:
				   _7Display_7();
				break;
							 
				case number_8:
				   _7Display_8();
				break;
							 
				case number_9:
				   _7Display_9();
				break;
		}   
}

void _7Display_0(void)
{
    A = 1;
	  B = 1;
	  C	= 1;
	  D = 1;
	  E = 1;
	  F = 1;
	  G = 0;
}

void _7Display_1(void)
{
    A = 0;
	  B = 1;
	  C	= 1;
	  D = 0;
	  E = 0;
	  F = 0;
	  G = 0;
}

void _7Display_2(void)
{
    A = 1;
	  B = 1;
	  C	= 0;
	  D = 1;
	  E = 1;
	  F = 0;
	  G = 1;
}

void _7Display_3(void)
{
    A = 1;
	  B = 1;
	  C	= 1;
	  D = 1;
	  E = 0;
	  F = 0;
	  G = 1;
}

void _7Display_4(void)
{
    A = 0;
	  B = 1;
	  C	= 1;
	  D = 0;
	  E = 0;
	  F = 1;
	  G = 1;
}

void _7Display_5(void)
{
    A = 1;
	  B = 0;
	  C	= 1;
	  D = 1;
	  E = 0;
	  F = 1;
	  G = 1;
}

void _7Display_6(void)
{
    A = 0;
	  B = 0;
	  C	= 1;
	  D = 1;
	  E = 1;
	  F = 1;
	  G = 1;
}

void _7Display_7(void)
{
    A = 1;
	  B = 1;
	  C	= 1;
	  D = 0;
	  E = 0;
	  F = 0;
	  G = 0;
}

void _7Display_8(void)
{
    A = 1;
	  B = 1;
	  C	= 1;
	  D = 1;
	  E = 1;
	  F = 1;
	  G = 1;
}

void _7Display_9(void)
{
    A = 1;
	  B = 1;
	  C	= 1;
	  D = 0;
	  E = 0;
	  F = 1;
	  G = 1;
}

void _7Display_AllOFF(void)
{
    A = 0;
	  B = 0;
	  C	= 0;
	  D = 0;
	  E = 0;
	  F = 0;
	  G = 0;
	  DP = 0;
}

void poll4DIG(void)
{
	  if(gu8fg_timeOut.DIGwait == 1)
		{
		    gu8fg_timeOut.DIGwait = 0;
			
		    gu8_poll4DIG ++;
			  if(gu8_poll4DIG > DIG4Fg){gu8_poll4DIG = DIG1Fg;}
		}
}
//========================================

void Flash_Erase(void)
{
    FMC_Open();
	
	  FMC_Erase(0x0001F000);
	
	  FMC_Close();
}	



void Flash_XY_Read(void)
{
	  FMC_Open();
	
    Gu32_Flash.X_xR249k = FMC_Read(0x1F000);
	  Gu32_Flash.Y_xR249k = FMC_Read(0x1F004);
	
	  Gu32_Flash.X_xR25200 = FMC_Read(0x1F008);
	  Gu32_Flash.Y_xR25200 = FMC_Read(0x1F010);
	
	  FMC_Close();
}



void Flash_XY_Write(void)
{
	  FMC_Open();
	  
    FMC_Write(0x1F000 , Gu32_X_xR249k);
	  FMC_Write(0x1F004 , Gu32_Y_xR249k);
	
	  FMC_Write(0x1F008 , Gu32_X_xR25200);
	  FMC_Write(0x1F010 , Gu32_Y_xR25200);
	
	  FMC_Close();
}

/*** (C) COPYRIGHT 2013 Nuvoton Technology Corp. ***/
