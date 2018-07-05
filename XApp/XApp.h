#ifndef __APP_XAPP_H
#define	__APP_XAPP_H

#include "XAppDef.h"

#ifdef __cplusplus
	extern "C" {
#endif

// global variables
extern U8  g_SmartConfig;														//÷«ƒ‹≈‰÷√wifi
extern U16 g_AirPM1_0;																		//PM1.0
extern U16 g_AirPM2_5;																		//PM2.5
extern U16 g_AirPM10;																		//PM10
extern U16 g_TVOC;
extern U16 g_HCHO;
extern U16 g_CO2;
extern U16 g_Temperature;
extern U16 g_Humidity;

extern U8 g_wifiReady;
extern U8 g_wifiLink;
extern U8 g_airmoduleState;

extern U8 g_led1;                                  // set led2 on/off manual or automatic mode
extern U8 g_led2;                                  // set led3 on/off manual or automatic mode

void XApp_Init(void);                              // initialize mcu application
void XApp_Run(void);                               // run the mcu application 

#ifdef __cplusplus
}
#endif

#endif /* __APP_XAPP_H */
