

#include "globalVariable.h"

void flash_Erase(void)
{
    FMC_Open();
	
	  FMC_Erase(0x0001F000);
	
	  FMC_Close();
}	



void flash_XY_Read(void)
{
	  FMC_Open();
	
    flash.gu32_Xbase_xR249k = FMC_Read(0x1F000);
	  flash.gu32_Yrate_xR249k = FMC_Read(0x1F004);
	
	  flash.gu32_Xbase_xR25200 = FMC_Read(0x1F008);
	  flash.gu32_Yrate_xR25200 = FMC_Read(0x1F010);
	
	  FMC_Close();
}



void flash_XY_Write(void)
{
	  FMC_Open();
	  
    FMC_Write(0x1F000 , calib_HV.gu32_Xbase_xR249k);
	  FMC_Write(0x1F004 , calib_HV.gu32_Yrate_xR249k);
	
	  FMC_Write(0x1F008 , calib_HV.gu32_Xbase_xR25200);
	  FMC_Write(0x1F010 , calib_HV.gu32_Yrate_xR25200);
	
	  FMC_Close();
}
