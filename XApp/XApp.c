#include "XApp.h"

// global variables
U8  g_SmartConfig = 0;														//÷«ƒ‹≈‰÷√wifi
U16 g_AirPM1_0;																		//PM1.0
U16 g_AirPM2_5;																		//PM2.5
U16 g_AirPM10;																		//PM10
U16 g_TVOC;
U16 g_HCHO;
U16 g_CO2;
U16 g_Temperature;
U16 g_Humidity;

U8 g_wifiReady = 0;
U8 g_wifiLink = 0;
U8 g_airmoduleState = 0;

U8 g_led1 = 0xff;                                  // set led2 on/off manual or automatic mode(0xff)
U8 g_led2 = 0xff;                                  // set led3 on/off manual or automatic mode(0xff)

void XApp_Init(void)
{

}

void XApp_Run(void)
{

}
