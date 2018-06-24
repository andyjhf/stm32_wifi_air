#include "XTaskEsp8266.h"
#include "XApp.h"

osThreadId XTaskEsp8266Handle;
static uint8_t g_SmartConfig = 0;
static uint8_t m_linked_ap = 0;
static uint8_t m_linked_server = 0;

#define      macUser_ESP8266_TcpServer_IP                 "192.168.3.17"      //要连接的服务器的 IP
#define      macUser_ESP8266_TcpServer_Port               "3456"               //要连接的服务器的端口

void StartXTaskEsp8266(void const * argument)
{

  uint8_t result = 1;
	uint8_t status;
	ESP8266_RxBufClear();
	ESP8266_Rst();
	status = ESP8266_ReceiveStatusString("WIFI GOT IP", "WIFI DISCONNECT", 20000);
	if(status == 0)
		m_linked_ap = 1;
	else if(status == 1)
	{
	//LED config
	}
	else
		g_SmartConfig = 1;
  /* Infinite loop */
  for(;;)
  {
		if(g_SmartConfig)
		{
//			osDelay(5000);
			ESP8266_Net_Mode_Choose(STA);
			ESP8266_Net_Auto_Connect();
			result = ESP8266_SmartConfig();
			if(result == 0)
			{
				printf("link to AP\r\n");
				m_linked_ap = 1;
				g_SmartConfig = 0;
			}
			else
			{
				printf("fail to link to AP\r\n");
				m_linked_ap = 0;
				m_linked_server = 0;
			}
		}
		if(m_linked_ap)
		{
			if(!ESP8266_Enable_MultipleId(DISABLE)
				&& !ESP8266_Link_Server(enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0)
				&& !ESP8266_UnvarnishSend())
			{
				printf("link to server\r\n");
				m_linked_ap = 0;
				m_linked_server = 1;
			}
			else
				printf("fail to link to server\r\n");
		}
		if(m_linked_server)
		{

//			ESP8266_SendString (ENABLE, "123456789", 0, Single_ID_0); 
			ESP8266_UnvarnishReceive();
		}
		
    osDelay(1000);
  }
  /* USER CODE END 5 */ 
}

void XTaskEsp8266_Init(void)
{
	ESP8266_Init();
	osThreadDef(XTaskEsp8266, StartXTaskEsp8266, osPriorityNormal, 0, 128);
  XTaskEsp8266Handle = osThreadCreate(osThread(XTaskEsp8266), NULL);

}
