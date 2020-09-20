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

#include "globalVariable.h"
#include "fuc_MAX31856.h"
#include "fuc_SPI_IO.h"
#include "fuc_7segm_dspy.h"
#include "fuc_other.h"
#include "fuc_RXD1.h"


#define PLL_CLOCK   50000000


//===


uint8_t gu8A_SPI_WriteBit[8] = {0};
uint8_t gu8A_SPI_WriteData[10] = {0};

uint8_t gu8_SPI_ReadHex = 0;
uint8_t gu8A_SPI_ReadData[10] = {0};

//---

uint8_t gu8_Rx0ReceiveEnable = 1;//need set 1
uint8_t gu8A_Rx0ReceiveData[2048] = {0};
uint16_t gu16_Rx0ReceiveIndex = 0;
uint8_t gu8_Rx0ReceiveCorrect = 0;


uint8_t gu8_Tx0TransmitEnable = 0;
uint8_t gu8A_Tx0TransmitData[100] = {0};
uint8_t gu8_Tx0TransmitIndex = 0;
uint8_t gu8_Tx0TransmitEnd = 0;
uint8_t gu8_Tx0ContentSelect = 0;

//===
stru_RXD   RXD0;
stru_RXD   RXD1;
//uint8_t gu8_Rx1ReceiveEnable = 1;//need set 1
//uint8_t gu8A_Rx1ReceiveData[2048] = {0};
//uint16_t gu16_Rx1ReceiveIndex = 0;
//uint8_t gu8_Rx1ReceiveCorrect = 0;
//uint8_t gu8A_Rx1ModBusCRC16Content[10] = {0};
//uint16_t gu16_Rx1ModBusCRC16Value = 0;


uint8_t gu8_Tx1TransmitEnable = 0;
uint8_t gu8A_Tx1TransmitData[500] = {0};
uint16_t gu16_Tx1TransmitIndex = 0;
uint16_t gu16_Tx1TransmitEnd = 0;
uint8_t gu8_Tx1ContentSelect = 0;
uint8_t gu8A_Tx1ModBusCRC16Content[100] = {0};
uint16_t gu16_Tx1ModBusCRC16Value = 0;

//===


uint32_t gu32_CJT_buf = 0;

//===

stru_Timer0   timer0;
stru_Timer0   timer0Run;
stru_Timer0   timeOut;

calibrate_HV   calib_HV;
_flash         flash;

stru_MAX31856register                             MAX31856register;
union uni_MAX31856_cold_junction_temprature       MAX31856CJT;
uni_MAX31856_linearized_thermocouple_temprature   MAX31856LTC;

stru_MAX31856LTC_calculate   LTC_calcu;   
stru_7segmDspyDIG            _7segmDspyDIG;
stru_4DIG_7DISPLAY           _4DIG_7dspy;

/*==========================================================*/

void GPCDEF_IRQHandler(void);
void GPIO_Init(void);
void UART0_Init(void);
void UART1_Init(void);
void Timer_Init(void);
void WatchDog_Init(void);


void UART0_Handle(void);
void uart1_handle(void);
void Timer0_Handle(void);


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
		
//		Flash_XY_Read();
		
		timer0Run.gfg_startUp_7segmDspy = 1;
		_7segmDspyDIG.gu8_startUpPoll = _7segmDspy_9xxx_1stON;
		
		
		MAX31856_CR0_registerWrite();
		MAX31856_CR1_registerWrite();
		
		for(;;)
		{
		    WDT_RESET_COUNTER();
			  
			  if(timeOut.gfg_STWD100NYN_WDI == 1)//WatchDog
				{
					  timeOut.gfg_STWD100NYN_WDI = 0;
					  WDI = !WDI;
				}
				
				MAX31856_CR0_registerRead();
				MAX31856_CR1_registerRead();
				
				MAX31856_CJTH_and_CJTL_registerRead();
				MAX31856_LTCBH_LTCBM_and_LTCBL_registerRead();
				
				
				LTC_calcu.gu64_sumBuf += MAX31856LTC.gu32_LTC_value;
				LTC_calcu.gu32_count++;
				
				if(LTC_calcu.gu32_count > 2999)
				{   
				    LTC_calcu.gu32_avgBuf =  LTC_calcu.gu64_sumBuf / 3000;
					  LTC_calcu.gu32_avg = (LTC_calcu.gu32_avgBuf * 100) / LTC_resolution;
					  
					  //===pos or neg===
					  if((MAX31856LTC.gu32_LTC_value & 0x00800000) == 0)//positive temp     
            { 
						    MAX31856_LTC_realValue(LTC_calcu.gu32_avg); 
						}
						else if((MAX31856LTC.gu32_LTC_value & 0x00800000) == 1)//negtive temp
            { 
							  LTC_calcu.gu32_avg = (~LTC_calcu.gu32_avg) + 1;
						    MAX31856_LTC_realValue(LTC_calcu.gu32_avg); 
						}
						//===     
						
						MAX31856_LTC_realValue(LTC_calcu.gu32_avg);
							  
						LTC_calcu.gu32_count = 0;
						LTC_calcu.gu64_sumBuf = 0;
				}
				
						
				//===7display show temprature===
				if(timer0Run.gfg_startUp_7segmDspy == 0)
				{
				    _7segmDspy_showMessage();
					  poll4DIG();
				}
				//===
				
				//=====initial 7display test=====
				if((timer0Run.gfg_startUp_7segmDspy == 1) && (timeOut.gfg_startUp_7segmDspy == 1))
				{
					  timeOut.gfg_startUp_7segmDspy = 0;
					
				    switch (_7segmDspyDIG.gu8_startUpPoll)
						{
						    case _7segmDspy_9xxx_1stON:
						       DIG1 = 0;
								   DIG2 = 1;
								   DIG3 = 1;
								   DIG4 = 1;
			             _7segmDspy_8();
							     DP = 1;
								   _7segmDspyDIG.gu8_startUpPoll = _7segmDspy_9xxx_1stOFF;
						    break;
								
								
								case _7segmDspy_9xxx_1stOFF:
									 DIG1 = 1;
								   DIG2 = 1;
								   DIG3 = 1;
								   DIG4 = 1;
			             _7segmDspy_ALLOFF();
							     DP = 0;
								   _7segmDspyDIG.gu8_startUpPoll = _7segmDspy_x9xx_2ndON;
								break;
								
								
								case _7segmDspy_x9xx_2ndON:
								   DIG1 = 1;
								   DIG2 = 0;
								   DIG3 = 1;
								   DIG4 = 1;
			             _7segmDspy_8();
							     DP = 1;
								   _7segmDspyDIG.gu8_startUpPoll = _7segmDspy_x9xx_2ndOFF;
								break;
								
								
								case _7segmDspy_x9xx_2ndOFF:
									 DIG1 = 1;
								   DIG2 = 1;
								   DIG3 = 1;
								   DIG4 = 1;
			             _7segmDspy_ALLOFF();
							     DP = 0;
								   _7segmDspyDIG.gu8_startUpPoll = _7segmDspy_xx9x_3rdON;
								break;
								
								
								case _7segmDspy_xx9x_3rdON:
									 DIG1 = 1;
								   DIG2 = 1;
									 DIG3 = 0;
								   DIG4 = 1;
			             _7segmDspy_8();
							     DP = 1;
								   _7segmDspyDIG.gu8_startUpPoll = _7segmDspy_xx9x_3rdOFF;
								break;
								
								
								case _7segmDspy_xx9x_3rdOFF:
									 DIG1 = 1;
								   DIG2 = 1;
								   DIG3 = 1;
								   DIG4 = 1;
			             _7segmDspy_ALLOFF();
							     DP = 0;
								   _7segmDspyDIG.gu8_startUpPoll = _7segmDspy_xxx9_4thON;
								break;
								
								
								case _7segmDspy_xxx9_4thON:
									 DIG1 = 1;
								   DIG2 = 1;
								   DIG3 = 1;
								   DIG4 = 0;
			             _7segmDspy_8();
							     DP = 1;
								   _7segmDspyDIG.gu8_startUpPoll = _7segmDspy_xxx9_4thOFF;
								break;
								
								
								case _7segmDspy_xxx9_4thOFF:
									 DIG1 = 1;
								   DIG2 = 1;
								   DIG3 = 1;
									 DIG4 = 1;
			             _7segmDspy_ALLOFF();
							     DP = 0;
								   _7segmDspyDIG.gu8_startUpPoll = _7segmDspy_4DIG_ALLON;
								break;
								
								
								case _7segmDspy_4DIG_ALLON:
									 DIG1 = 0;
								   DIG2 = 0;
								   DIG3 = 0;
								   DIG4 = 0;
								   _7segmDspy_8();
								   DP = 1;
								   _7segmDspyDIG.gu8_startUpPoll = _7segmDspy_4DIG_ALLOFF;
								break;
								   
								
								case _7segmDspy_4DIG_ALLOFF:
									 DIG1 = 1;
								   DIG2 = 1;
								   DIG3 = 1;
								   DIG4 = 1;
								   _7segmDspy_ALLOFF();
								   DP = 0;
                   _7segmDspyDIG.gu8_startUpPoll = _7segmDspy_testEND;
								break;
								
								default:
						       timer0Run.gfg_startUp_7segmDspy = 0;
						}
						
				}
				//===END===
				
				
        //=====Tx0 fill in content=====
        if(gu8_Rx0ReceiveCorrect == 1)
		    {
		        gu8_Rx0ReceiveCorrect = 0;
		        gu8_Rx0ReceiveEnable = 0;//Rx OFF
			      
					  switch(gu8_Tx0ContentSelect)
						{
						    case AllValue:
									gu8A_Tx0TransmitData[0] = 0x02;
								  
									gu8A_Tx0TransmitData[40] = 0x03;
									
									gu8_Tx0TransmitIndex = 0;//start transmit from 0
			            gu8_Tx0TransmitEnd = 41;//need transmit length 41 byte
			            gu8_Tx0TransmitEnable = 1;//Tx ON
								break;		
								
                case AllGain:
								  gu8A_Tx0TransmitData[0] = 0x02;
		              
					        gu8A_Tx0TransmitData[82] = 0x03;
			  
			            gu8_Tx0TransmitIndex = 0;//start transmit from 0
			            gu8_Tx0TransmitEnd = 83;//need transmit length 83 byte
			            gu8_Tx0TransmitEnable = 1;//Tx ON
								break;

						}   
				}   
				//===END===
				
				//===Tx0 Transmit===
				if(gu8_Tx0TransmitEnable == 1)
        {
		        if(gu8_Tx0TransmitIndex < gu8_Tx0TransmitEnd)
				    {
				        UART_WRITE(UART0, gu8A_Tx0TransmitData[gu8_Tx0TransmitIndex]);
							  while(UART_IS_TX_FULL(UART0)){}
				        gu8_Tx0TransmitIndex ++;
				    
				        if(gu8_Tx0TransmitIndex >= gu8_Tx0TransmitEnd)
				        {
					          gu8_Tx0TransmitEnable = 0;//Tx OFF
									
									  while(UART_IS_TX_EMPTY(UART0) == 0){}
					          gu8_Rx0ReceiveEnable = 1;//Rx ON 
					  
					          gu8_Tx0TransmitIndex = 0;//clear array index 
					          gu8_Tx0TransmitEnd = 0;//clear transmit length
					  
					          memset(gu8A_Tx0TransmitData , 0x00 , sizeof(gu8A_Tx0TransmitData));
								}
						}
				}
				//===END===
				
        //=====Tx1 fill in content=====
				if(RXD1.gu8_recivVerifyOK == 1)
				{
				    RXD1.gu8_recivVerifyOK = 0;
		        RXD1.gu8_recivEnable = 0;//rx1 off
					
					  
				}   
				//=====End=====
				
				//=====Tx1 Transmit=====
				if(gu8_Tx1TransmitEnable == 1)
				{
		        if(gu16_Tx1TransmitIndex < gu16_Tx1TransmitEnd)
				    {	
				        UART_WRITE(UART1, gu8A_Tx1TransmitData[gu16_Tx1TransmitIndex]);
							  while (UART_IS_TX_FULL(UART1)){}
				        gu16_Tx1TransmitIndex ++;
				    
				        if(gu16_Tx1TransmitIndex >= gu16_Tx1TransmitEnd)
				        {
					          gu8_Tx1TransmitEnable = 0;//Tx OFF
									  
									  while(UART_IS_TX_EMPTY(UART1) == 0){}
										
					          RXD1.gu8_recivEnable = 1;//Rx ON	
								
					          gu16_Tx1TransmitIndex = 0;//clear array index 
					          gu16_Tx1TransmitEnd = 0;//clear transmit length
					  
					          memset(gu8A_Tx1TransmitData , 0x00 , sizeof(gu8A_Tx1TransmitData));
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
			
//		    Gu8_ADS1220Adc.CanRead = 1;	
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
	  if(timer0.gu16_STWD100NYN_WDI_ms > 5000)
		{
		    timeOut.gfg_STWD100NYN_WDI = 1;//time out
			  timer0.gu16_STWD100NYN_WDI_ms = 0;//clear
		}
		else 
		{
		    timer0.gu16_STWD100NYN_WDI_ms ++;
		}
		
		//=====
		
		if(timer0Run.gfg_startUp_7segmDspy == 1)
		{
		    if(timer0.gu16_startUp_7segmDspy_ms > 250)
				{
					  timeOut.gfg_startUp_7segmDspy = 1;//time out
					  timer0.gu16_startUp_7segmDspy_ms = 0;//clear
				}
				else 
				{
					  timer0.gu16_startUp_7segmDspy_ms ++;
				}
		}
		
		//=====
		
		if(timer0.gu16_DIGwait_ms > 1)// 1/16 = 0.0625s = 62.5ms  need <62.5ms
		{
		    timeOut.gfg_DIGwait = 1;//time out
				timer0.gu16_DIGwait_ms = 0;//clear
		}
		else 
		{
		    timer0.gu16_DIGwait_ms ++;
		}
		
		//=====
		
		if(timer0.gu16_pollTEMP_ms > 100)
		{
		    timeOut.gfg_pollTEMP = 1;//time out
			  timer0.gu16_pollTEMP_ms = 0;//clear
		}
		else
		{
		    timer0.gu16_pollTEMP_ms ++;
		}
			
}



void UART02_IRQHandler(void)
{
    UART0_Handle();
}



void UART0_Handle()
{
 	  uint32_t u32IntSts = UART0->ISR;
     
    if((u32IntSts & UART_ISR_RDA_INT_Msk) && (gu8_Rx0ReceiveEnable == 1))
    {
		 	  while(UART_IS_RX_READY(UART0))
			  {
				    gu8A_Rx0ReceiveData[gu16_Rx0ReceiveIndex] = UART_READ(UART0);
				    
				    
						
						
						if(gu16_Rx0ReceiveIndex < 2047)
						{
						    gu16_Rx0ReceiveIndex = (gu16_Rx0ReceiveIndex == 2046) ? 0 : (gu16_Rx0ReceiveIndex + 1);
						}
				}
		}
}



void UART1_IRQHandler(void)
{
    uart1_handle();
}



void uart1_handle(void)
{
    uint32_t u32IntSts = UART1->ISR;
	
	  if((u32IntSts & UART_ISR_RDA_INT_Msk) && (RXD1.gu8_recivEnable == 1))
		{
		    while(UART_IS_RX_READY(UART1))
				{
				    RXD1.gu8A_recivData[RXD1.gu16_recivIndex] = UART_READ(UART1);
					
					  r1_receive();
					  
			      //---
						
//						if(RXD1.gu16_recivIndex < 2047)
//						{                                          
//							  if(RXD1.gu16_recivIndex == 2046)
//								{
//								    memset(RXD1.gu8A_recivData , 0x00 , sizeof(RXD1.gu8A_recivData));  //Gu8_Rx1ReceiveIndex = (Gu8_Rx1ReceiveIndex == 17) ? 0 : (Gu8_Rx1ReceiveIndex + 1);
//								    RXD1.gu16_recivIndex = 0;   									
//								}
//								else if(RXD1.gu16_recivIndex < 2046)
//								{
//								    RXD1.gu16_recivIndex ++;
//								}
//						}
				}
		}
}



//void Rx1ModBusCRC16_Fillin_Buffer(void)
//{
//    gu8A_Rx1ModBusCRC16Content[0] = gu8A_Rx1ReceiveData[gu16_Rx1ReceiveIndex - 4];
//		gu8A_Rx1ModBusCRC16Content[1] = gu8A_Rx1ReceiveData[gu16_Rx1ReceiveIndex - 3];
//							
//		gu16_Rx1ModBusCRC16Value = gu8A_Rx1ReceiveData[gu16_Rx1ReceiveIndex - 2];
//		gu16_Rx1ModBusCRC16Value = (gu16_Rx1ModBusCRC16Value << 8) + gu8A_Rx1ReceiveData[gu16_Rx1ReceiveIndex - 1];
//}


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




/*** (C) COPYRIGHT 2013 Nuvoton Technology Corp. ***/
