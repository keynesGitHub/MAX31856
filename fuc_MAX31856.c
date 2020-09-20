


#include "globalVariable.h"
#include "fuc_SPI_IO.h"
#include "fuc_other.h"


void MAX31856_CR0_registerWrite(void)
{
    gu8A_SPI_WriteData[0] = 0x80;//CR0
    gu8A_SPI_WriteData[1] = 0x80;//Automatic Conversion mode
	  
    SPI_Master_CS = 0;//SPI_CS Low
    SPI_WriteBlock(gu8A_SPI_WriteData, 2);
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
    gu8A_SPI_WriteData[0] = 0x81;//CR1
    gu8A_SPI_WriteData[1] = 0x43;//16 samples averaged, K
	  
    SPI_Master_CS = 0;//SPI_CS Low
    SPI_WriteBlock(gu8A_SPI_WriteData, 2);
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
    gu8A_SPI_WriteData[0] = 0x8A;
    gu8A_SPI_WriteData[1] = 0x00;
    gu8A_SPI_WriteData[2] = 0x00;
	  
    SPI_Master_CS = 0;//SPI_CS Low
    SPI_WriteBlock(gu8A_SPI_WriteData, 3);
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

void MAX31856_LTC_realValue(uint32_t lu32_LTC_buf)
{
    if(lu32_LTC_buf >= 10000)//999.9
		{
		    _7segmDspyDIG.gu8_xxx9_4th = (lu32_LTC_buf / 10) % 10;    //xxx.9
			  _7segmDspyDIG.gu8_xx9x_3rd  = (lu32_LTC_buf / 100) % 10;   //xx9.x
			  _7segmDspyDIG.gu8_x9xx_2nd = (lu32_LTC_buf / 1000) % 10;  //x9x.x
			  _7segmDspyDIG.gu8_9xxx_1st  = lu32_LTC_buf / 10000;        //9xx.x
		    _4DIG_7dspy.gu8_decimalPoint = _1DIG;
		}
   	else if(lu32_LTC_buf < 10000)//99.99
    {
		    _7segmDspyDIG.gu8_xxx9_4th = lu32_LTC_buf % 10;          //xx.x9
			  _7segmDspyDIG.gu8_xx9x_3rd  = (lu32_LTC_buf / 10) % 10;   //xx.9x
			  _7segmDspyDIG.gu8_x9xx_2nd = (lu32_LTC_buf / 100) % 10;  //x9.xx
			  _7segmDspyDIG.gu8_9xxx_1st  = lu32_LTC_buf / 1000;        //9x.xx
			  _4DIG_7dspy.gu8_decimalPoint = _2DIG;
		}
}
