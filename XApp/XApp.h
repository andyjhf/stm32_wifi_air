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


// global variables
extern U8 g_wifiReset;                             // 1: request to reset wifi module
extern U8 g_hostReset;                             // 1: request to reset self-host
extern U8 g_led2;                                  // set led2 on/off manual or automatic mode
extern U8 g_led3;                                  // set led3 on/off manual or automatic mode
extern U8 g_rs485;
extern U8 g_resetfactory;                          // Reset AP mode

// global protocol data
extern U8  g_platform[PLATFORM_SIZE]; 
extern U8  g_module[MODULE_SIZE];
extern U8  g_ervinfo[ERVINFO_SIZE]; 
extern U8  g_ervcommand[ERVCMD_SIZE];
extern U8  g_modulecommand[MODULECMD_SIZE];
		
// global protocol prop
extern U8  g_szOemVer[SZ_OEM_VER_SIZE+1];          // oem host template version prop
extern U8  g_szPlatform[SZ_PLATFORM_SIZE+1]; 
extern U8  g_szModule[SZ_MODULE_SIZE+1];
extern U8  g_szErvinfo[SZ_ERVINFO_SIZE+1];
extern U8  g_szErvcommand[SZ_ERVCMD_SIZE+1];
extern U8  g_szModulecommand[SZ_MODULECMD_SIZE+1];

// application
void InitProp(void);                               // initilize props data
void TransProp(void);                              // hex to string for props
void ParseProp(U8 *command, U8 size);              // parse the WiFi-host command

void XApp_Init(void);                              // initialize mcu application
void XApp_Run(void);                               // run the mcu application 

#ifdef __cplusplus
}
#endif

#endif /* __APP_XAPP_H */
