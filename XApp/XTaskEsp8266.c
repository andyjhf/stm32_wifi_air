#include "XTaskEsp8266.h"
#include "XApp.h"
#include "http.h"

osThreadId XTaskEsp8266Handle;

json_param m_data;
uint8_t json_buf[128];
static uint8_t m_state = 0;
static uint8_t m_linked_ap = 0;
static uint8_t m_linked_server = 0;
static uint8_t m_disconnect_server = 0;
static uint8_t m_send_togle = 1;

static uint8_t m_send_get = 1;
static uint8_t m_send_time = 0;
static uint8_t m_error_cnt = 0;

static uint8_t m_Dis_Link_cnt = 0;

static char* pHttpStr;

#define KEY    "xUrvOCDB=iRuS5noq9FsKrvoW=s="
#define POST_URL    "devices/5835707/datapoints"
#define GET_URL    "devices/5835707"
char send_buf[2048];
void StartXTaskEsp8266(void const * argument)
{

  uint8_t result = 1;
	uint8_t status;
	uint8_t LinkStatus;
	char* pJson;
	ESP8266_Rst();
	osDelay(5000);
  /* Infinite loop */
  for(;;)
  {
		
		LinkStatus = ESP8266_Get_IdLinkStatus();
		printf("status = %d\r\n",LinkStatus);
		switch(LinkStatus)
		{
			case AP_Connect:
				if(m_send_get)
					m_linked_ap = 1;
				m_linked_server = 0;
				m_state = 1;
				m_error_cnt = 0;
				m_Dis_Link_cnt = 0;
				osDelay(1000);
			break;
			case TCP_UDP_Connect:
				m_linked_server = 0;
				m_disconnect_server = 1;
				m_linked_ap = 0;
				m_state = 1;
				m_error_cnt = 0;
				m_Dis_Link_cnt = 0;
				osDelay(1000);
			break;
			case TCP_UDP_Disconnect:
				m_linked_server = 0;
				if(m_send_get)
					m_linked_ap = 1;
				m_state = 1;
				m_error_cnt = 0;
				m_Dis_Link_cnt = 0;
				osDelay(1000);
			break;
			case AP_Disconnect:
				m_linked_ap = 0;
				m_linked_server = 0;
				
				m_Dis_Link_cnt++;
				if(m_Dis_Link_cnt > 10)
				{
					m_Dis_Link_cnt = 0;
					printf("=====reset wifi module\r\n");
					ESP8266_Rst();
					osDelay(15000);
				}
				m_state = 1;
				m_error_cnt = 0;
				osDelay(1000);
			break;
			default:
				m_linked_ap = 0;
				m_linked_server = 0;
				m_state = 0;
				m_error_cnt++;
				if(m_error_cnt>5)
				{
					m_error_cnt = 0;
					printf("=====reset wifi module\r\n");
					ESP8266_Rst();
					osDelay(15000);
				}
			break;
		
		}
		if(m_state)
		{
			if(g_SmartConfig)
			{
				ESP8266_Net_Mode_Choose(STA);
				ESP8266_Net_Auto_Connect();
				result = ESP8266_SmartConfig();
				if(result == 0)
				{
					printf("link to AP\r\n");
					g_SmartConfig = 0;
				}
				else
				{
					printf("fail to link to AP\r\n");
				}
			}
			else
			{
//				if(m_send_get)
//				{
//					if(!ESP8266_Enable_MultipleId(DISABLE)
//						&& !ESP8266_Link_Server(enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0))
//					{
//						memset(send_buf,0,sizeof(send_buf));
//						HTTP_GetPkt(send_buf,GET_URL,KEY,macUser_ESP8266_TcpServer_IP,macUser_ESP8266_TcpServer_Port);
//						printf("%s\r\n",send_buf);
//						if(!ESP8266_SendString (DISABLE, send_buf, strlen(send_buf), Single_ID_0))
//							ESP8266_Receive(DISABLE,10000);
//						m_send_get = 0;
//					}
//				}
//				
//				if(m_linked_ap)
				
				if(m_disconnect_server)
				{
					m_disconnect_server = 0;
					ESP8266_Disconnet_Link_Server();
				}

				if(m_linked_ap)
				{
					
					m_linked_ap =0;
					m_send_get = 0;
					if(!ESP8266_Enable_MultipleId(DISABLE)
						&& !ESP8266_Link_Server(enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0)
						&& !ESP8266_UnvarnishSend())
					{
						m_linked_server = 1;
						printf("link to server\r\n");
					}
					else
					{
						printf("fail to link to server\r\n");
					}
				}
				if(m_linked_server)
				{
					m_linked_server = 0;
					
					if(m_send_togle)
					{
						m_send_togle = 0;
						memset(send_buf,0,sizeof(send_buf));
						HTTP_GetPkt(send_buf,GET_URL,KEY,macUser_ESP8266_TcpServer_IP,macUser_ESP8266_TcpServer_Port);
						printf("%s\r\n",send_buf);
						if(!ESP8266_SendString (ENABLE, send_buf, strlen(send_buf), Single_ID_0))
						{
							pHttpStr = ESP8266_Receive(ENABLE,10000);
							if(pHttpStr != NULL)
							{
								pJson = strstr(pHttpStr,"{");
								if(pJson != NULL)
								{
									Json_to_data(pJson,&m_data);
									printf("protocol :%s\r\n",m_data.data.protocol);
									printf("auth_info :%s\r\n",m_data.data.auth_info);
									printf("title: %s\r\n",m_data.data.title);						
								}
							}
							
						}
						ESP8266_ExitUnvarnishSend();
						osDelay(300);
						ESP8266_Disconnet_Link_Server();
						printf("====protocol :%s\r\n",m_data.data.protocol);
					}
					else
					{
						m_send_togle = 1;
						memset(json_buf,0,sizeof(json_buf));
						data_to_Json((U8 *)json_buf);
						memset(send_buf,0,sizeof(send_buf));
						HTTP_PostPkt(send_buf,POST_URL,KEY,macUser_ESP8266_TcpServer_IP,macUser_ESP8266_TcpServer_Port,(char *)json_buf);
						printf("%s\r\n",send_buf);
						if(!ESP8266_SendString (ENABLE, send_buf, strlen(send_buf), Single_ID_0))
						{
							pHttpStr = ESP8266_Receive(ENABLE,10000);
						}
						ESP8266_ExitUnvarnishSend();
						osDelay(300);
						ESP8266_Disconnet_Link_Server();
					}
				}
			}
		}
		m_send_time++;
		if(m_send_time >= 7)
		{
			m_send_time = 0;
			m_send_get = 1;
		}
    osDelay(3000);
  }
  /* USER CODE END 5 */ 
}

void XTaskEsp8266_Init(void)
{
	ESP8266_Init();
	osThreadDef(XTaskEsp8266, StartXTaskEsp8266, osPriorityNormal, 0, 1024);
  XTaskEsp8266Handle = osThreadCreate(osThread(XTaskEsp8266), NULL);

}
