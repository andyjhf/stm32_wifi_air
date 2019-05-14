#include "XTaskHost.h"
#include "XApp.h"
#include "hcho.h"
#include "dht11.h"
#include "sht30.h"
#include "stdlib.h"
static U16 m_tmBlink1   = 100;
static U16 m_tmBlink2   = 2000;

static U16 m_PowerCnt    = 0;

static U16 m_blinkCnt1   = 0;
static U16 m_blinkCnt2   = 0;

static U8 m_wifiReady = 0;                               // 1: wifi module ready(started)
static U8 m_wifiLink  = 0;                               // 1: wifi module linked to ayla cloud
static U8 m_smartLink = 0;
static U8 m_airModuleState  = 0;                         

static U16 m_sampleCnt = 0;
static U16 m_tmSample = 10000;
static U16 m_humi;
static int m_temp;

osThreadId HostTaskHandle;
static void StartHostTask(void const * argument);
void CXTaskHost_InitTask(void)
{
	m_tmBlink1   = 100;
	m_tmBlink2   = 2000;

	m_PowerCnt    = 0;
	m_blinkCnt1   = 0;
	m_blinkCnt2   = 0;

	m_wifiReady = 0;                               // 1: wifi module ready(started)
	m_wifiLink  = 0;                               // 1: wifi module linked to ayla cloud
	m_airModuleState  = 0;
	
	osThreadDef(HostTask, StartHostTask, osPriorityRealtime, 0, 128);
  HostTaskHandle = osThreadCreate(osThread(HostTask), NULL);
}

static void CXTaskHost_DoLoop(U16 tmOnce)
{
	U8 result;
	// led indicator(blink quickly)
	if(m_blinkCnt1>=m_tmBlink1)
	{
		m_wifiReady = g_wifiReady;
		m_wifiLink = g_wifiLink;
		m_smartLink = g_SmartConfig;
		m_airModuleState = g_airmoduleState;
		m_blinkCnt1 = 0;                           // clear quick blinking counter
		if(0xff==g_led1)                           // LED2 is in auto mode
		{
			if(m_smartLink)
			{
				m_wifiReady = 0;
				LED_Set(LED1, LED_OFF);
				LED_Toggle(LED2);
			}
			else if(0==m_wifiReady)                     // wifi ready state is not OK
			{
				LED_Set(LED2, LED_OFF);
				LED_Toggle(LED1);
			}
		}
		else                                       // LED2 is in manual mode
		{
			LED_Set(LED1,g_led1);
		}
//		if(0xff==g_led2)                           // LED3 is in auto mode
//		{
//			if(0==m_airModuleState)                      
//			{
//				LED_Toggle(LED2);                  // keep led3 blink by the interval of 250ms
//			}
//		}
//		else                                       // LED3 is in manual mode
//		{
//			LED_Set(LED2,g_led2);
//		}
	}

	// led indicator(blink slowly)
	if(m_blinkCnt2>=m_tmBlink2)
	{
		m_wifiReady = g_wifiReady;
		m_wifiLink = g_wifiLink;
		m_airModuleState = g_airmoduleState;
		m_blinkCnt2=0;                             // clear slow blinking counter
		if(1==m_wifiReady && 0xff==g_led1)         // wifi ready state is OK and led2 is in auto mode
		{
			if(1==m_wifiLink)                      // wifi has been connectted to ADS
			{
				LED_Set(LED1, LED_ON);             // keep led2 on
			}
			else
			{
				LED_Toggle(LED1);                  // toggle led2(blink)
			}
		}
//		if(0xff==g_led2)
//		{
//			if(1==m_airModuleState)
//			{
//				LED_Set(LED2, LED_ON);             // keep led3 on
//			}
//			else
//			{
//				LED_Toggle(LED2);                  // toggle led3(blink)
//			}
//		}
	}	

	if(m_sampleCnt>=m_tmSample)
	{
		m_sampleCnt = 0;
		taskDISABLE_INTERRUPTS();
		result = SHT30_Read_Humiture(&m_temp, &m_humi);
		taskENABLE_INTERRUPTS();
		if(result == SUCCESS)
		{  
      g_Humidity = m_humi;  
      g_Temperature = (m_temp<0)?(abs(m_temp)|0x8000):m_temp;
			if(m_temp & 0x8000)
				printf("temp = -%d.%d \r\n",(m_temp&0x7FFF)/10,(m_temp&0x7FFF)%10);
			else
				printf("temp = %d.%d \r\n",m_temp/10,m_temp%10);
			printf("humi = %d.%d \r\n",m_humi/10,m_humi%10);
		}
	}

	if(m_PowerCnt<6000)	                           // 6000ms counter waiting for WiFi's initilization after power on
	{
		m_PowerCnt += tmOnce;
	}
	m_blinkCnt1    += tmOnce;                      // 250ms counter for slow blinking 
	m_blinkCnt2    += tmOnce;                      // 2000ms counter for quick blinking
	m_sampleCnt    += tmOnce;
}

static void StartHostTask(void const * argument)
{
//	AM2302_GPIO_Config();
  for(;;)
  {
		CXTaskHost_DoLoop(10);
		osDelay(10);
  }

}

//static void CXTaskHost_sampling(void)
//{
//	// dip switch(dip switch's No. same as SWI number) 
//	m_swiBit <<= 1;

//	m_swiBit |= SWI_GetState(SW1_1);

//	// high level 4 times continuously
//	if(0x0f == (m_swiBit&0x0f))
//	{
////		g_platform[PLATFORM_INDEX_SW_VAL] |= (1<<(5-i));
//	}
//	// low level 4 times continuously
//	else if(0==(m_swiBit&0x0f))
//	{
////		g_platform[PLATFORM_INDEX_SW_VAL] &= ~(1<<(5-i));
//	}

////	// led2(red) status
////	if(LED_Get(LED2))
////		g_platform[PLATFORM_INDEX_LED_STATE] |= 0x01;                         // led2 on
////	else
////		g_platform[PLATFORM_INDEX_LED_STATE] &= ~0x01;                        // led2 off

////	// led3(green) status
////	if(LED_Get(LED1))
////		g_platform[PLATFORM_INDEX_LED_STATE] |= 0x02;                         // led3 on
////	else
////		g_platform[PLATFORM_INDEX_LED_STATE] &= ~0x02;                        // led3 off
//}
