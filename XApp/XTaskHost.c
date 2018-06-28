#include "XTaskHost.h"
#include "XApp.h"

static U16 m_tmSampling = 5;
static U16 m_tmBlink1   = 250;
static U16 m_tmBlink2   = 2000;

static U16 m_PowerCnt    = 0;
static U16 m_samplingCnt = 0;
static U16 m_blinkCnt1   = 0;
static U16 m_blinkCnt2   = 0;

static U8 m_swiBit = 0;
static U8 m_readyBit  = 0;
static U8 m_linkBit   = 0;

static U8 m_wifiReady = 0;                               // 1: wifi module ready(started)
static U8 m_wifiLink  = 0;                               // 1: wifi module linked to ayla cloud
static U8 m_ervReady  = 0;                               // 1: erv comm ready
static U8 m_ervLink   = 0;                               // 1: erv outdoor broadcast it's data

static U8 m_hostReset = 0;
static U16 m_PowerCnt; 

static void CXTaskHost_sampling(void);

void CXTaskHost_InitTask(void)
{
	m_tmSampling = 5;
	m_tmBlink1   = 250;
	m_tmBlink2   = 2000;

	m_PowerCnt    = 0;
	m_samplingCnt = 0;
	m_blinkCnt1   = 0;
	m_blinkCnt2   = 0;

	m_swiBit = 0;
	m_readyBit  = 0;
	m_linkBit   = 0;

	m_wifiReady = 0;                               // 1: wifi module ready(started)
	m_wifiLink  = 0;                               // 1: wifi module linked to ayla cloud
	m_ervReady  = 0;                               // 1: erv comm ready
	m_ervLink   = 0;                               // 1: erv outdoor broadcast it's data
	
	m_hostReset = 0;
}

void CXTaskHost_DoLoop(U16 tmOnce)
{
	U16 i;
	// sampling
	if(m_samplingCnt>=m_tmSampling)
	{
		m_samplingCnt = 0;                         // clear sampling counter(5ms)
		CXTaskHost_sampling();                                // take a sample
	}

	// led indicator(blink quickly)
	if(m_blinkCnt1>=m_tmBlink1)
	{
		m_ervReady = g_rs485;//(g_ervinfo[ERVINFO_INDEX_BASE_STATE] & OD_DAT_READY)? 1:0;       // erv ready state
		m_ervLink  = (g_ervinfo[ERVINFO_INDEX_BASE_STATE] & OD_DAT_LINK)? 1:0;       // erv link state
		m_blinkCnt1 = 0;                           // clear quick blinking counter
		if(0xff==g_led2)                           // LED2 is in auto mode
		{
			if(0==m_wifiReady)                     // wifi ready state is not OK
			{
				LED_Toggle(LED2);                  // keep led2 blink by the interval of 250ms
			}
		}
		else                                       // LED2 is in manual mode
		{
			LED_Set(LED2,g_led2);
		}
		if(0xff==g_led3)                           // LED3 is in auto mode
		{
			if(0==m_ervReady)                      // ERV Comm ready state is not OK
			{
				LED_Toggle(LED1);                  // keep led3 blink by the interval of 250ms
			}
		}
		else                                       // LED3 is in manual mode
		{
			LED_Set(LED1,g_led3);
		}
	}

	// led indicator(blink slowly)
	if(m_blinkCnt2>=m_tmBlink2)
	{
//		update_wifi_strength_enable = 1;           //WiFi strength enable
//		WiFi_rank();                               //WiFi strength into grade
		
		m_ervReady = g_rs485;//(g_ervinfo[ERVINFO_INDEX_BASE_STATE] & OD_DAT_READY)? 1:0;       // erv ready state
		m_ervLink  = (g_ervinfo[ERVINFO_INDEX_BASE_STATE] & OD_DAT_LINK)? 1:0;       // erv link state
		m_blinkCnt2=0;                             // clear slow blinking counter
		if(1==m_wifiReady && 0xff==g_led2)         // wifi ready state is OK and led2 is in auto mode
		{
			if(1==m_wifiLink)                      // wifi has been connectted to ADS
			{
				LED_Set(LED2, LED_ON);             // keep led2 on
			}
			else
			{
				LED_Toggle(LED2);                  // toggle led2(blink)
			}
		}
		if(1==m_ervReady && 0xff==g_led3)
		{
			if(1==m_ervLink)
			{
				LED_Set(LED1, LED_ON);             // keep led3 on
			}
			else
			{
				LED_Toggle(LED1);                  // toggle led3(blink)
			}
		}
//		// TODO: TBD
//		static U16 readyCnt=0;
//		if(0==m_wifiReady)
//		{
//			readyCnt += m_tmBlink2;
//			if(readyCnt>=50000)
//			{
//				readyCnt = 0;
//				m_wifiReset=1;
//			}
//		}
//		else
//		{
//			readyCnt=0;
//		}
	}	

	if(g_wifiReset)                                // reset wifi module flag
	{
		g_wifiReset= 0;
//		WIFI_Reset(1);                             // set wifi module reset pin low level
		for (i=0; i<2000; i++);                // keep a short time(167.5us, wifi module reset OK)
//		WIFI_Reset(0);                             // return to high level
	}
	else
	{
//		WIFI_Reset(0);                             // keep wifi module reset pin high level
	}
/*
	if(0==g_hostReset && ayla_tick()<AYLA_YEAR_MAX)// reset host flag (watch dog)
	{
		if(m_hostReset == 0)
			IWDG_Feed();
	}
	*/
//	if(1==g_hostReset || ayla_tick()>=AYLA_YEAR_MAX)
	{
		g_hostReset = 0;                           // do it once
		//IWDG_InitEx();                             // initialize independent watchdog but not feed it
		m_hostReset = 1;
	}

	if(m_PowerCnt<6000)	                           // 6000ms counter waiting for WiFi's initilization after power on
	{
		m_PowerCnt += tmOnce;
	}
	m_samplingCnt  += tmOnce;                      // 5ms counter for sampling
	m_blinkCnt1    += tmOnce;                      // 250ms counter for slow blinking 
	m_blinkCnt2    += tmOnce;                      // 2000ms counter for quick blinking
}

static void CXTaskHost_sampling(void)
{
	// dip switch(dip switch's No. same as SWI number) 
	m_swiBit <<= 1;

	m_swiBit |= SWI_GetState(SW1_1);

	// high level 4 times continuously
	if(0x0f == (m_swiBit&0x0f))
	{
//		g_platform[PLATFORM_INDEX_SW_VAL] |= (1<<(5-i));
	}
	// low level 4 times continuously
	else if(0==(m_swiBit&0x0f))
	{
//		g_platform[PLATFORM_INDEX_SW_VAL] &= ~(1<<(5-i));
	}

//	// led2(red) status
//	if(LED_Get(LED2))
//		g_platform[PLATFORM_INDEX_LED_STATE] |= 0x01;                         // led2 on
//	else
//		g_platform[PLATFORM_INDEX_LED_STATE] &= ~0x01;                        // led2 off

//	// led3(green) status
//	if(LED_Get(LED1))
//		g_platform[PLATFORM_INDEX_LED_STATE] |= 0x02;                         // led3 on
//	else
//		g_platform[PLATFORM_INDEX_LED_STATE] &= ~0x02;                        // led3 off
}
