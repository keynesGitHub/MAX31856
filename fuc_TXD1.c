


#include "NUC100Series.h"
#include "globalVariable.h"
#include "fuc_modbusCRC16.h"

void t1_fillIn_content(uint8_t lu8_devID,   uint8_t lu8_fc,   uint16_t lu16_sta_addr,   uint16_t lu16_nbr)
{
	  uint16_t i;
	  uint16_t pt = 3;//inital 3
	  uint16_t st;
	
    TXD1.gu8A_tramtData[0] = lu8_devID;
	  TXD1.gu8A_tramtData[1] = lu8_fc;
	  TXD1.gu8A_tramtData[2] = (uint8_t)(lu16_nbr * 2);
	
	  //===hold register fill in===
	  for(i=0; i<lu16_nbr; i++) 
	  {
		    TXD1.gu8A_tramtData[pt++] = (uint8_t)((MB_procl.gu16_holdReg[lu16_sta_addr] & 0xFF00) >> 8);
			  TXD1.gu8A_tramtData[pt++] = (uint8_t)(MB_procl.gu16_holdReg[lu16_sta_addr++] & 0x00FF);
		}
		//===
		
		//===CRC16===
		for(st=0; st<pt; st++)
		{
		    TXD1.gu8A_MB_CRC16Content[st] = TXD1.gu8A_tramtData[st];
		}
		TXD1.gu16_MB_CRC16Value = ModBus_CRC16(TXD1.gu8A_MB_CRC16Content, pt);
		//===
		
		TXD1.gu8A_tramtData[pt++] = (TXD1.gu16_MB_CRC16Value & 0xFF00) >> 8;
		TXD1.gu8A_tramtData[pt++] = TXD1.gu16_MB_CRC16Value & 0x00FF;
		
		TXD1.gu16_tramtEnd = pt;
		TXD1.gu16_tramtIndex = 0;
	  TXD1.gu8fg_enable = 1;//t1 ON
}


