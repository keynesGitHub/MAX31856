

#include <string.h>
#include "globalVariable.h"




void r1_receive(void)
{
	  if(RXD1.gu16_recivIndex == 7)//data length 8
		{
		    if((RXD1.gu8A_recivData[RXD1.gu16_recivIndex - 7] == 0x01) && (RXD1.gu8A_recivData[RXD1.gu16_recivIndex - 6] == 0x03) && (RXD1.gu8A_recivData[RXD1.gu16_recivIndex - 5] == 0x00))
        { 
		        RXD1.gu16_startPoint = RXD1.gu16_recivIndex; 
		    }
		}
    
		
		
		
		
    
		if(RXD1.gu16_recivIndex < 2047)						
	  {                                          					  
	      if(RXD1.gu16_recivIndex == 2046)
		    {
      	    memset(RXD1.gu8A_recivData , 0x00 , sizeof(RXD1.gu8A_recivData));  //Gu8_Rx1ReceiveIndex = (Gu8_Rx1ReceiveIndex == 17) ? 0 : (Gu8_Rx1ReceiveIndex + 1);
		        RXD1.gu16_recivIndex = 0;   									
				}
		    else if(RXD1.gu16_recivIndex < 2046)
				{
		        RXD1.gu16_recivIndex ++;
				}
	}
}
