

#include <string.h>
#include "globalVariable.h"
#include "fuc_modbusCRC16.h"




void r1_receive(void)
{
    uint16_t i = 0;
		
		if((RXD1.gu8A_recivData[RXD1.gu16_recivIndex - 1] == 0x01) && (RXD1.gu8A_recivData[RXD1.gu16_recivIndex] == 0x03) && (RXD1.gu16_recivSX == 0))
		{
		    RXD1.gu16_recivSX = RXD1.gu16_recivIndex;
		}
		else if((RXD1.gu16_recivSX != 0) &&
			      (RXD1.gu8A_recivData[RXD1.gu16_recivSX - 1] == 0x01) && (RXD1.gu8A_recivData[RXD1.gu16_recivSX] == 0x03) &&
			      (RXD1.gu8A_recivData[RXD1.gu16_recivSX + 1] == 0x00) && (RXD1.gu8A_recivData[RXD1.gu16_recivSX + 2] <= 0x01) && //startAddrs
		        (RXD1.gu8A_recivData[RXD1.gu16_recivSX + 3] == 0x00) && (RXD1.gu8A_recivData[RXD1.gu16_recivSX + 4] >= 0x01) && (RXD1.gu8A_recivData[RXD1.gu16_recivSX + 4] <= 0x02))//quantity
		{ 
		    for(i=0; i<(MB_RTU_Query_length - 2); i++)
			  {
				    RXD1.gu8A_MB_CRC16Content[i] = RXD1.gu8A_recivData[RXD1.gu16_recivSX + i - 1]; 
				}
				RXD1.gu16_MB_CRC16Value = ModBus_CRC16(RXD1.gu8A_MB_CRC16Content, (MB_RTU_Query_length - 2));
				
				RXD1.gu16_CRC16_MSB = ((RXD1.gu16_MB_CRC16Value & 0xFF00) >> 8);
			  RXD1.gu16_CRC16_LSB = RXD1.gu16_MB_CRC16Value & 0x00FF;
				
				if((RXD1.gu8A_recivData[RXD1.gu16_recivSX + 5] == RXD1.gu16_CRC16_MSB) && (RXD1.gu8A_recivData[RXD1.gu16_recivSX + 6] == RXD1.gu16_CRC16_LSB))//CRC16 check
				{
						MB_procl.gu8_respn_deviceID = RXD1.gu8A_recivData[RXD1.gu16_recivSX - 1];
					  MB_procl.gu8_respn_fc = RXD1.gu8A_recivData[RXD1.gu16_recivSX];
							
						MB_procl.gu16_query_startAddrs = RXD1.gu8A_recivData[RXD1.gu16_recivIndex + 1];
						MB_procl.gu16_query_startAddrs = ((MB_procl.gu16_query_startAddrs & 0x00FF) << 8) + RXD1.gu8A_recivData[RXD1.gu16_recivSX + 2];
							
						MB_procl.gu16_respn_nbr = RXD1.gu8A_recivData[RXD1.gu16_recivIndex + 3];
						MB_procl.gu16_respn_nbr = ((MB_procl.gu16_respn_nbr & 0x00FF) << 8) + RXD1.gu8A_recivData[RXD1.gu16_recivSX + 4];
					
					  RXD1.gu8fg_verifyOK = 1;//data correct
						RXD1.gu8fg_enable = 0;//rx1 off
					  RXD1.gu16_recivSX = 0;
				}
		}
		
    //===
		
    
		if(RXD1.gu16_recivIndex < 127)						
	  {                                          					  
	      if(RXD1.gu16_recivIndex >= 126)
		    {
      	    memset(RXD1.gu8A_recivData , 0x00 , sizeof(RXD1.gu8A_recivData));  //Gu8_Rx1ReceiveIndex = (Gu8_Rx1ReceiveIndex == 17) ? 0 : (Gu8_Rx1ReceiveIndex + 1);
		        RXD1.gu16_recivIndex = 0;  
            RXD1.gu16_recivSX = 0;					
				}
		    else if(RXD1.gu16_recivIndex < 126)
				{
		        RXD1.gu16_recivIndex ++;
				}
	}
}
