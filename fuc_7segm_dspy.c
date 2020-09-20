

#include "globalVariable.h"
#include "fuc_7segm_dspy.h"


void _7segmDspy_showMessage(void)
{
    switch (_7segmDspyDIG.gu8_poll4DIG)
	  {
		    case DIG1Fg :
		       DIG1 = 0;
					 DIG2 = 1;
					 DIG3 = 1;
					 DIG4 = 1;
				   numberSelect(_7segmDspyDIG.gu8_9xxx_1st);				
	         DP = 0;
	      break;
				
				
				case DIG2Fg :
					 DIG1 = 1;
           DIG2 = 0;
           DIG3 = 1;
				   DIG4 = 1;
           numberSelect(_7segmDspyDIG.gu8_x9xx_2nd);
				   
				   if(_4DIG_7dspy.gu8_decimalPoint == _1DIG)     {DP = 0;}
           else if(_4DIG_7dspy.gu8_decimalPoint == _2DIG){DP = 1;}				
	      break;
				
				
				case DIG3Fg :
					 DIG1 = 1;
           DIG2 = 1;
           DIG3 = 0;
				   DIG4 = 1;
				   numberSelect(_7segmDspyDIG.gu8_xx9x_3rd);
				   
				   if(_4DIG_7dspy.gu8_decimalPoint == _1DIG)     {DP = 1;}
           else if(_4DIG_7dspy.gu8_decimalPoint == _2DIG){DP = 0;}
				break;
				
				
				case DIG4Fg :
			     DIG1 = 1;
           DIG2 = 1;
           DIG3 = 1;
				   DIG4 = 0;
				   numberSelect(_7segmDspyDIG.gu8_xxx9_4th);
					 DP = 0;
				break;
		}   
}

void numberSelect(uint8_t lu8_numberSelect)
{
    switch (lu8_numberSelect)
		{
		    case number_0:
				   _7segmDspy_0();
				break;
							 
				case number_1:
					 _7segmDspy_1();
				break;
							 
				case number_2:
					 _7segmDspy_2();
        break;
               
        case number_3:
           _7segmDspy_3();
        break;
               
        case number_4:
           _7segmDspy_4();
        break;

        case number_5:
           _7segmDspy_5();
        break;
							 
				case number_6:
				   _7segmDspy_6();
				break;
							 
				case number_7:
				   _7segmDspy_7();
				break;
							 
				case number_8:
				   _7segmDspy_8();
				break;
							 
				case number_9:
				   _7segmDspy_9();
				break;
		}   
}

void _7segmDspy_0(void)
{
    A = 1;
	  B = 1;
	  C	= 1;
	  D = 1;
	  E = 1;
	  F = 1;
	  G = 0;
}

void _7segmDspy_1(void)
{
    A = 0;
	  B = 1;
	  C	= 1;
	  D = 0;
	  E = 0;
	  F = 0;
	  G = 0;
}

void _7segmDspy_2(void)
{
    A = 1;
	  B = 1;
	  C	= 0;
	  D = 1;
	  E = 1;
	  F = 0;
	  G = 1;
}

void _7segmDspy_3(void)
{
    A = 1;
	  B = 1;
	  C	= 1;
	  D = 1;
	  E = 0;
	  F = 0;
	  G = 1;
}

void _7segmDspy_4(void)
{
    A = 0;
	  B = 1;
	  C	= 1;
	  D = 0;
	  E = 0;
	  F = 1;
	  G = 1;
}

void _7segmDspy_5(void)
{
    A = 1;
	  B = 0;
	  C	= 1;
	  D = 1;
	  E = 0;
	  F = 1;
	  G = 1;
}

void _7segmDspy_6(void)
{
    A = 0;
	  B = 0;
	  C	= 1;
	  D = 1;
	  E = 1;
	  F = 1;
	  G = 1;
}

void _7segmDspy_7(void)
{
    A = 1;
	  B = 1;
	  C	= 1;
	  D = 0;
	  E = 0;
	  F = 0;
	  G = 0;
}

void _7segmDspy_8(void)
{
    A = 1;
	  B = 1;
	  C	= 1;
	  D = 1;
	  E = 1;
	  F = 1;
	  G = 1;
}

void _7segmDspy_9(void)
{
    A = 1;
	  B = 1;
	  C	= 1;
	  D = 0;
	  E = 0;
	  F = 1;
	  G = 1;
}

void _7segmDspy_ALLOFF(void)
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
	  if(timeOut.gfg_DIGwait == 1)
		{
		    timeOut.gfg_DIGwait = 0;
			
		    _7segmDspyDIG.gu8_poll4DIG ++;
			  if(_7segmDspyDIG.gu8_poll4DIG > DIG4Fg){_7segmDspyDIG.gu8_poll4DIG = DIG1Fg;}
		}
}
