

#include "globalVariable.h"
#include "fuc_other.h"


void SPI_WriteByte(uint8_t lu8_Byte)
{
    int8_t i;
	
	  gu8A_SPI_WriteBit[7] = ((lu8_Byte & 0x80) >> 7);
	  gu8A_SPI_WriteBit[6] = ((lu8_Byte & 0x40) >> 6);
	  gu8A_SPI_WriteBit[5] = ((lu8_Byte & 0x20) >> 5);
	  gu8A_SPI_WriteBit[4] = ((lu8_Byte & 0x10) >> 4);
	
	  gu8A_SPI_WriteBit[3] = ((lu8_Byte & 0x08) >> 3);
	  gu8A_SPI_WriteBit[2] = ((lu8_Byte & 0x04) >> 2);
	  gu8A_SPI_WriteBit[1] = ((lu8_Byte & 0x02) >> 1);
	  gu8A_SPI_WriteBit[0] = ((lu8_Byte & 0x01) >> 0);
	  
	  for(i=7 ; i>=0 ; i--)
	  {
		    SPI_Master_SCK = 0;//SCK
		
		    SPI_Master_MOSI = gu8A_SPI_WriteBit[i];//MOSI
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
			
			  gu8_SPI_ReadHex = (gu8_SPI_ReadHex << 1) | SPI_Master_MISO;
			
		    delayTime(10);
		    SPI_Master_SCK = 0;
		    delayTime(10);
		}
	  return gu8_SPI_ReadHex;
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
