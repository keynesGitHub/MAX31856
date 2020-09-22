
#ifndef __globalVariable__
#define __globalVariable__

#include "NUC100Series.h"

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

#define _7segmDspy_9xxx_1stON     1
#define _7segmDspy_9xxx_1stOFF    2

#define _7segmDspy_x9xx_2ndON     3
#define _7segmDspy_x9xx_2ndOFF    4

#define _7segmDspy_xx9x_3rdON     5
#define _7segmDspy_xx9x_3rdOFF    6

#define _7segmDspy_xxx9_4thON     7
#define _7segmDspy_xxx9_4thOFF    8

#define _7segmDspy_4DIG_ALLON     9
#define _7segmDspy_4DIG_ALLOFF    10

#define _7segmDspy_testEND        11

#define WDI       PB15//pin43

#define AllValue   1
#define AllGain    2

#define CJT_resolution 256
#define LTC_resolution 4096

#define _1DIG  1
#define _2DIG  2

#define MB_RTU_Query_length   8
#define MB_RTU_03H            0x03
#define MB_RTU_06H            0x06


//===

extern uint8_t gu8A_SPI_WriteBit[8];
extern uint8_t gu8A_SPI_WriteData[10];


extern uint8_t gu8_SPI_ReadHex;
extern uint8_t gu8A_SPI_ReadData[10];

//---

//extern uint8_t gu8_Rx0ReceiveEnable;//need set 1
//extern uint8_t gu8A_Rx0ReceiveData[2048];
//extern uint16_t gu16_Rx0ReceiveIndex;
//extern uint8_t gu8_Rx0ReceiveCorrect;


//extern uint8_t gu8_Tx0TransmitEnable;
//extern uint8_t gu8A_Tx0TransmitData[100];
//extern uint8_t gu8_Tx0TransmitIndex;
//extern uint8_t gu8_Tx0TransmitEnd;
//extern uint8_t gu8_Tx0ContentSelect;

//===
typedef struct
{ 
    uint8_t gu8A_recivData[128];
    uint16_t gu16_recivIndex;
	  uint16_t gu16_recivSX;
	  uint16_t gu16_recivEX;
	  uint8_t gu8A_MB_CRC16Content[16];
	  uint16_t gu16_MB_CRC16Value;
	  uint16_t gu16_CRC16_MSB;
    uint16_t gu16_CRC16_LSB;  
	  
	  uint8_t gu8fg_enable:1;//need set 1   
	  uint8_t gu8fg_verifyOK:1;
	
}stru_RXD;
extern stru_RXD   RXD1;
//extern uint8_t gu8_Rx1ReceiveEnable;//need set 1
//extern uint8_t gu8A_Rx1ReceiveData[2048];
//extern uint16_t gu16_Rx1ReceiveIndex;
//extern uint8_t gu8_Rx1ReceiveCorrect;
//extern uint8_t gu8A_Rx1ModBusCRC16Content[10];
//extern uint16_t gu16_Rx1ModBusCRC16Value;

typedef struct
{
    uint8_t gu8A_tramtData[128];
    uint16_t gu16_tramtIndex;
    uint16_t gu16_tramtEnd;
    uint8_t gu8_contentSelect;	  
	  uint8_t gu8A_MB_CRC16Content[32];
    uint16_t gu16_MB_CRC16Value;	
	
	  uint8_t gu8fg_enable:1;
	  
}stru_TXD;
extern stru_TXD   TXD1;



//extern uint8_t gu8_Tx1TransmitEnable;
//extern uint8_t gu8A_Tx1TransmitData[500];
//extern uint16_t gu16_Tx1TransmitIndex;
//extern uint16_t gu16_Tx1TransmitEnd;
//extern uint8_t gu8_Tx1ContentSelect;
//extern uint8_t gu8A_Tx1ModBusCRC16Content[100];
//extern uint16_t gu16_Tx1ModBusCRC16Value;

//---



typedef struct
{
	  uint16_t gu16_holdregsr[32];
	   
	  uint16_t gu16_query_startAddrs;
	  
    uint8_t gu8_respn_deviceID;
	  uint8_t gu8_respn_fc;
	  uint16_t gu16_respn_nbr;
	  uint16_t gu16_respn_contn[16];
	  uint16_t gu16_respn_CRC16;
	
}stru_ModBus_protocol;
extern stru_ModBus_protocol   MB_procl;




extern uint32_t gu32_CJT_buf;

typedef struct
{
	  uint8_t gu8_startUpPoll;
	  uint8_t gu8_poll4DIG;
	
    uint8_t gu8_9xxx_1st;
    uint8_t gu8_x9xx_2nd;
    uint8_t gu8_xx9x_3rd;
    uint8_t gu8_xxx9_4th;
	
}stru_7segmDspyDIG;
extern stru_7segmDspyDIG   _7segmDspyDIG;




typedef struct
{
    uint16_t gu16_startUp_7segmDspy_ms;
	  uint16_t gu16_DIGwait_ms;
	  uint16_t gu16_STWD100NYN_WDI_ms;
    uint16_t gu16_pollTEMP_ms;
	
	  uint8_t gu8fg_startUp_7segmDspy:1;
	  uint8_t gu8fg_DIGwait:1;
    uint8_t gu8fg_STWD100NYN_WDI:1;
    uint8_t gu8fg_pollTEMP:1;
   
}stru_Timer0;

extern stru_Timer0   timeOut;



typedef struct
{
    uint32_t gu32_Xbase_xR25200;//base
    uint32_t gu32_Yrate_xR25200;//rate

    uint32_t gu32_Xbase_xR249k;//base
    uint32_t gu32_Yrate_xR249k;//rate
 
}calibrate_HV;
extern calibrate_HV   calib_HV;



typedef struct
{
    uint32_t gu32_Xbase_xR249k;
    uint32_t gu32_Yrate_xR249k;

    uint32_t gu32_Xbase_xR25200;	
	  uint32_t gu32_Yrate_xR25200;
	
}_flash;
extern _flash   flash;



typedef struct
{
    uint8_t gu8_CR0;
	  uint8_t gu8_CR1;
	
}stru_MAX31856register;
extern stru_MAX31856register   MAX31856register;



union uni_MAX31856_cold_junction_temprature
{
    uint32_t gu32_CJT_value;
    uint8_t gu8_CJT[2];
	
}extern MAX31856CJT;



typedef union
{
    uint32_t gu32_LTC_value;
	  uint8_t  gu8_LTC[3];
	
}uni_MAX31856_linearized_thermocouple_temprature;
extern uni_MAX31856_linearized_thermocouple_temprature   MAX31856LTC;



typedef struct
{
	  uint64_t gu64_sumBuf;
    uint32_t gu32_avg;
    uint32_t gu32_avgBuf;
    uint32_t gu32_count;
	
}stru_MAX31856LTC_calculate;




typedef struct
{
    uint8_t gu8_decimalPoint;
    
}stru_4DIG_7DISPLAY;
extern stru_4DIG_7DISPLAY   _4DIG_7dspy;




#endif

