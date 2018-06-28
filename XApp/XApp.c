#include "XApp.h"

// global variables
U8  g_SmartConfig = 0;														//ÖÇÄÜÅäÖÃwifi
U16 g_AirPM1_0;																		//PM1.0
U16 g_AirPM2_5;																		//PM2.5
U16 g_AirPM10;																		//PM10
U16 g_TVOC;
U16 g_HCHO;
U16 g_CO2;
U16 g_Temperature;
U16 g_Humidity;

U8 g_wifiReset;                                    // 1: request to reset wifi module
U8 g_hostReset;                                    // 1: request to reset self-host
U8 g_led2 = 0xff;                                  // set led2 on/off manual or automatic mode(0xff)
U8 g_led3 = 0xff;                                  // set led3 on/off manual or automatic mode(0xff)
U8 g_rs485 = 0;																		 // RS485 state  0: rs485 data check error 1: rs485 open or data ok
U8 g_resetfactory = 0;                             // Reset AP mode
                             
// global protocol data
U8  g_platform[PLATFORM_SIZE]; 
U8  g_module[MODULE_SIZE];
U8  g_ervinfo[ERVINFO_SIZE]; 
U8  g_ervcommand[ERVCMD_SIZE];
U8  g_modulecommand[MODULECMD_SIZE];

// global protocol prop
U8  g_szOemVer[SZ_OEM_VER_SIZE+1];                 // oem Host template version prop
U8  g_szPlatform[SZ_PLATFORM_SIZE+1]; 
U8  g_szModule[SZ_MODULE_SIZE+1];
U8  g_szErvinfo[SZ_ERVINFO_SIZE+1];
U8  g_szErvcommand[SZ_ERVCMD_SIZE+1];
U8  g_szModulecommand[SZ_MODULECMD_SIZE+1];


void InitProp(void)
{
	memset(g_szOemVer,         '\0', sizeof(g_szOemVer));
	memset(g_szPlatform,          '\0', sizeof(g_szPlatform));
	memset(g_szModule,           '\0', sizeof(g_szModule));
	memset(g_szErvinfo,          '\0', sizeof(g_szErvinfo));
	memset(g_szErvcommand,           '\0', sizeof(g_szErvcommand));
	memset(g_szModulecommand,          '\0', sizeof(g_szModulecommand));

}

void XApp_Init(void)
{
	// initialize global variables
	g_wifiReset = 1;                               // 1: request to reset wifi module
	g_hostReset = 0;                               // 1: request to reset self-host
	g_led2 = 0xff;	                               // led2 automatic mode
	g_led3 = 0xff;	                               // led3 automatic mode

	// initialize protocol data
	memset(g_platform,0,    sizeof(g_platform));        // Host data
	memset(g_module, 0,    sizeof(g_module));        // ERV system data
	memset(g_ervinfo, 0,     sizeof(g_ervinfo));        // ERV outdoor data
	memset(g_ervcommand, 0,     sizeof(g_ervcommand));        // ERV indoor data
	memset(g_modulecommand, 0, sizeof(g_modulecommand));        // ERV remote control data
	
	//g_module[0] = 0x00;
	//g_module[1] = 0x00;
	g_module[2] = 0x01;
	//g_module[3] = 0x00;
	//g_module[4] = 0x00;
	
	// initialize protocol prop
	InitProp();

	// construct all tasks
//	taskERV  = new CXTaskComERV;                   // erv task for usart communication with outdoor and indoor units
//	taskHost = new CXTaskHost;                     // host task for sampling dip setiing, and led blink/on/off
//	taskWifi = new CXTaskComWifi;                  // wifi task for communicating with wifi module

	// initialize all tasks
//	taskERV->InitTask();                           // initialize erv task
//	taskHost->InitTask();                          // initialize host task
//	taskWifi->InitTask();                          // initialize wifi task
}

void XApp_Run(void)
{
	static U32 curTick=0;                          // current tick time(ms)
	static U32 taskTick[3]={0,0,0};              // job rotation tick time(ms)
	static U8  slot=0;                             // job rotation No

#if DEBUG_LOG == 1
	static U32 debug_tick = 0;
	static U32 debug_cnt=0;
#endif
	for (;;)                                       // main loop
	{
		curTick=HAL_GetTick();                     // current tick time
		if(curTick != taskTick[slot])              // check current task's timetick
		{
			switch(slot)                           // which job No
			{
				case 0:                            // job 1
//					taskERV->DoLoop(1);            // do task of ERV communication once 1 millisecond
					break;
				case 1:                            // job 2
//					taskHost->DoLoop(1);           // do task of MCU host once 1 millisecond
					break;
				case 2:                            // job 3
//					TransProp();                   // transfer one data to a prop by sequence
					break;
			}
			taskTick[slot] = curTick;              // save job's tick time
		}
		slot++;                                    // next job
		slot %= 3;                                 // turn to head job
		
//		if(taskHost->m_PowerCnt>=5000)             // after 5000ms 
//		{
//			taskWifi->DoReal();                    // job4 usart communication with wifi module in realtime
//		}
//		else                                       // in 5000ms after MCU boot(scope:required 1800ms)
//		{
//			taskWifi->DetectFactory();             // detect factory reset 
//		}
//		
//		if(g_resetfactory == 1)
//		{
//			taskWifi->ResetFactory();			
//			g_resetfactory = 0;	
//		}
	
#if DEBUG_LOG == 1
		if(curTick != debug_tick)
		{	
			debug_cnt++;
			debug_tick = curTick;
		}
		if(debug_cnt > 10000)
		{
			debug_cnt = 0;
			taskERV->debug();
		}
#endif
	}
}
