#include "XTaskHost.h"
#include "XApp.h"
#include "hcho.h"
#include "dht11.h"
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
static U16 m_tmSample = 5000;
static U16 m_humi;
static U16 m_temp;
static U16 m_humibuf[10];
static U16 m_tempbuf[10];
static U8 m_tempSampleCnt = 0;

osThreadId HostTaskHandle;
static void StartHostTask(void const * argument);
U16 average(char type);
void CXTaskHost_InitTask(void)
{
	U16 i;
	U8 j,result;
	m_tmBlink1   = 100;
	m_tmBlink2   = 2000;

	m_PowerCnt    = 0;
	m_blinkCnt1   = 0;
	m_blinkCnt2   = 0;

	m_wifiReady = 0;                               // 1: wifi module ready(started)
	m_wifiLink  = 0;                               // 1: wifi module linked to ayla cloud
	m_airModuleState  = 0;
	
	memset((U8*)m_humibuf,0,sizeof(m_humibuf));
	memset((U8*)m_tempbuf,0,sizeof(m_tempbuf));
	for(i=0;i<10000;i++)
	{
		taskDISABLE_INTERRUPTS();
		result = Read_AM2302(&m_temp, &m_humi);
		taskENABLE_INTERRUPTS();
		if(result == SUCCESS)
		{
			for(j=0;j<sizeof(m_humibuf)/sizeof(U16);j++)
			{
				m_humibuf[j] = m_humi;  
				m_tempbuf[j] = m_temp;
			}
			break;
		}
	}
	
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
		result = Read_AM2302(&m_temp, &m_humi);
		taskENABLE_INTERRUPTS();
		if(result == SUCCESS)
		{
			m_humibuf[m_tempSampleCnt++] = m_humi;
			m_tempbuf[m_tempSampleCnt++] = m_temp;
			if(m_tempSampleCnt == sizeof(m_tempbuf)/sizeof(U16))
				m_tempSampleCnt = 0;
			g_Humidity = average(1);  
			g_Temperature = average(2);
			if(g_Temperature & 0x8000)
				printf("temp = -%d.%d \r\n",(g_Temperature&0x7F)/10,(g_Temperature&0x7F)%10);
			else
				printf("temp = %d.%d \r\n",g_Temperature/10,g_Temperature%10);
			printf("humi = %d.%d \r\n",g_Humidity/10,g_Humidity%10);
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
	AM2302_GPIO_Config();
  for(;;)
  {
		CXTaskHost_DoLoop(10);
		osDelay(10);
  }

}

U16 average(char type)
{
	U16 i;
	S32 sum = 0;
	S16 temp;
	U8 flag = 0;
	for(i=0;i<sizeof(m_tempbuf)/sizeof(U16);i++)
	{
		if(type == 1)
			sum += m_humibuf[i];
		else
		{
			if(m_tempbuf[i] & 0x8000)
			{
				sum -= (m_tempbuf[i] & 0x7fff);
			}
			else
			{
				sum += m_tempbuf[i];
			}
		}
	}
	temp = sum/(sizeof(m_tempbuf)/sizeof(U16));
	if(temp < 0)
	{
		temp = (~temp)+1;
		temp = temp | 0x8000;
	}
	return temp;
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
