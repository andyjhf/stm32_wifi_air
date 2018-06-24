#include "driver_esp8266.h"
#include "driver_common.h"
#include "driver_delay.h"



static void                   ESP8266_GPIO_Config                ( void );
static void                   ESP8266_USART_Config               ( void );
//static void                   ESP8266_USART_NVIC_Configuration   ( void );
volatile uint8_t ucTcpClosedFlag = 0;
UART_HandleTypeDef huart3;
/*********************************************** ESP8266 �����궨�� *******************************************/

//#define     macPC_Usart( fmt, ... )                

#define     macESP8266_CH_ENABLE()                 HAL_GPIO_WritePin(macESP8266_CH_PD_PORT, macESP8266_CH_PD_PIN, GPIO_PIN_SET)
#define     macESP8266_CH_DISABLE()                HAL_GPIO_WritePin(macESP8266_CH_PD_PORT, macESP8266_CH_PD_PIN, GPIO_PIN_RESET)

#define     macESP8266_RST_HIGH_LEVEL()            HAL_GPIO_WritePin(macESP8266_RST_PORT, macESP8266_RST_PIN, GPIO_PIN_SET)
#define     macESP8266_RST_LOW_LEVEL()             HAL_GPIO_WritePin(macESP8266_RST_PORT, macESP8266_RST_PIN, GPIO_PIN_RESET)

#define     macESP8266_Usart( fmt, ... )           USART_printf(&huart3, fmt, ##__VA_ARGS__ ) 
#define     macPC_Usart( fmt, ... )                printf( fmt, ##__VA_ARGS__ )

#define     macESP8266_USART_INT_FUN              USART3_IRQHandler

struct  STRUCT_USARTx_Fram strEsp8266_Fram_Record = { 0 };



/**
  * @brief  ESP8266��ʼ������
  * @param  ��
  * @retval ��
  */
void ESP8266_Init( void )
{
	ESP8266_GPIO_Config(); 
	ESP8266_USART_Config();
	
	macESP8266_RST_HIGH_LEVEL();
	macESP8266_CH_DISABLE();
}


/**
  * @brief  ��ʼ��ESP8266�õ���GPIO����
  * @param  ��
  * @retval ��
  */
static void ESP8266_GPIO_Config( void )
{
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;


	/* ���� CH_PD ����*/
	// Configure the GPIO_LED pin
	GPIO_InitStructure.Pin   = macESP8266_CH_PD_PIN;
	GPIO_InitStructure.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull  = GPIO_NOPULL;
	GPIO_InitStructure.Speed  = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(macESP8266_CH_PD_PORT, &GPIO_InitStructure);
	
	/* ���� RST ����*/											   
	GPIO_InitStructure.Pin = macESP8266_RST_PIN;	
	HAL_GPIO_Init(macESP8266_RST_PORT, &GPIO_InitStructure);
}

static void ESP8266_USART_Config( void )
{
	GPIO_InitTypeDef GPIO_InitStruct;
	

	/* Peripheral clock enable */
	__HAL_RCC_USART3_CLK_ENABLE();

	/**USART1 GPIO Configuration    
	PA9     ------> USART1_TX
	PA10     ------> USART1_RX 
	*/
	GPIO_InitStruct.Pin = macESP8266_USART_TX_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(macESP8266_USART_TX_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = macESP8266_USART_RX_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(macESP8266_USART_RX_PORT, &GPIO_InitStruct);
	
	huart3.Instance = macESP8266_USARTx;
  huart3.Init.BaudRate = macESP8266_USART_BAUD_RATE;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if(HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }


	// Enable USART3_IRQn
	HAL_NVIC_SetPriority(macESP8266_USART_IRQ, 0, 0);
	HAL_NVIC_EnableIRQ(macESP8266_USART_IRQ);

	__HAL_UART_ENABLE_IT(&huart3,UART_IT_RXNE);
	__HAL_UART_ENABLE_IT(&huart3,UART_IT_IDLE);
	__HAL_UART_ENABLE(&huart3);
}

void macESP8266_USART_INT_FUN( void )
{	
	uint8_t ucCh;
//	
//	if( __HAL_UART_GET_IT_SOURCE(&huart3, UART_IT_RXNE) & USART_CR1_RXNEIE)
//	{
//		ucCh  = (uint8_t)(huart3.Instance->DR &(uint8_t)0x00FF);
//		
//		if( strEsp8266_Fram_Record.InfBit.FramLength > ( RX_BUF_MAX_LEN - 1 ) )                      //Ԥ��1���ֽ�д������
//			strEsp8266_Fram_Record.InfBit.FramLength = 0;
//		strEsp8266_Fram_Record.Data_RX_BUF[strEsp8266_Fram_Record.InfBit.FramLength++]  = ucCh;
//	}
//	 	 
//	if( __HAL_UART_GET_IT_SOURCE(&huart3, UART_IT_IDLE) & USART_CR1_IDLEIE)                                         //����֡�������
//	{
//    strEsp8266_Fram_Record.InfBit.FramFinishFlag = 1;
//		
//		ucCh = (uint16_t)(huart3.Instance->DR & (uint16_t)0x01FF);                                                           //�������������жϱ�־λ(�ȶ�USART_SR��Ȼ���USART_DR)
//	
//		ucTcpClosedFlag = strstr((char*)strEsp8266_Fram_Record.Data_RX_BUF, "CLOSED\r\n" ) ? 1 : 0;
//		
//  }	
	
	// Received completed interrupt(RXNE) if RXNEIE=1 in the USARTx_CR1 register
	if((huart3.Instance->SR & USART_SR_RXNE) && (huart3.Instance->CR1&USART_CR1_RXNEIE ))
	{
		ucCh  = (uint8_t)(huart3.Instance->DR &(uint8_t)0x00FF);
		
		if( strEsp8266_Fram_Record.InfBit.FramLength > ( RX_BUF_MAX_LEN - 1 ) )                      //Ԥ��1���ֽ�д������
			strEsp8266_Fram_Record.InfBit.FramLength = 0;
		strEsp8266_Fram_Record.Data_RX_BUF[strEsp8266_Fram_Record.InfBit.FramLength++]  = ucCh;
	}
//	else if((huart3.Instance->SR & USART_SR_IDLE) && (huart3.Instance->CR1&USART_CR1_IDLEIE ))
//	{
		
//	}
	// Transmit completed interrupt(TC) if TCIE=1 in the USARTx_CR1 register
	else
	{
		ucCh  = (uint8_t)(huart3.Instance->DR &(uint8_t)0x00FF);
	}
	
	
//	uint32_t isrflags   = READ_REG(huart3.Instance->SR);
//	uint32_t cr1its     = READ_REG(huart3.Instance->CR1);
//	uint32_t cr3its     = READ_REG(huart3.Instance->CR3);
//	uint32_t errorflags = 0x00U;
////	uint32_t dmarequest = 0x00U;

//  /* If no error occurs */
//  errorflags = (isrflags & (uint32_t)(USART_SR_PE | USART_SR_FE | USART_SR_ORE | USART_SR_NE));
//  if(errorflags == RESET)
//  {
//    /* UART in mode Receiver -------------------------------------------------*/
//    if(((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET))
//    {
//			if(strEsp8266_Fram_Record.InfBit.FramLength >= RX_BUF_MAX_LEN)
//				strEsp8266_Fram_Record.InfBit.FramLength = 0;
//      strEsp8266_Fram_Record.Data_RX_BUF[strEsp8266_Fram_Record.InfBit.FramLength++] = (uint8_t)(huart3.Instance->DR & (uint8_t)0x00FF);
//      return;
//    }
//  }

}

void ESP8266_RxBufClear(void)
{
	strEsp8266_Fram_Record.InfBit.FramLength = 0;
	memset(strEsp8266_Fram_Record.Data_RX_BUF, 0, sizeof(strEsp8266_Fram_Record.Data_RX_BUF));
}

/*
 * ��������ESP8266_Rst
 * ����  ������WF-ESP8266ģ��
 * ����  ����
 * ����  : ��
 * ����  ���� ESP8266_AT_Test ����
 */
void ESP8266_Rst( void )
{
	#if 0
	 ESP8266_Cmd( "AT+RST", "OK", "ready", 2500 );   	
	
	#else
	 macESP8266_RST_LOW_LEVEL();
	 Delay_ms( 500 ); 
	 macESP8266_RST_HIGH_LEVEL();
	#endif

}


/*
 * ��������ESP8266_Cmd
 * ����  ����WF-ESP8266ģ�鷢��ATָ��
 * ����  ��cmd�������͵�ָ��
 *         reply1��reply2���ڴ�����Ӧ��ΪNULL������Ӧ������Ϊ���߼���ϵ
 *         waittime���ȴ���Ӧ��ʱ��
 * ����  : 1��ָ��ͳɹ�
 *         0��ָ���ʧ��
 * ����  �����ⲿ����
 */
uint8_t ESP8266_Cmd( char * cmd, char * reply1, char * reply2, uint32_t waittime )
{ 
	uint8_t result = 1;
	uint32_t check_cnt,check_time = 50;
	uint32_t length_bak = 0;
	
	ESP8266_RxBufClear();
//	strEsp8266_Fram_Record.InfBit.FramLength = 0;               //���¿�ʼ�����µ����ݰ�
//	memset(strEsp8266_Fram_Record.Data_RX_BUF, 0, sizeof(strEsp8266_Fram_Record.Data_RX_BUF));
	macESP8266_Usart( "%s\r\n", cmd );
	macPC_Usart("%s\r\n", cmd);

	if(( reply1 == 0 ) && ( reply2 == 0 ) )                      //����Ҫ��������
		result = 0;
	else
	{
		check_cnt = waittime/check_time;
		while(check_cnt--)
		{
			Delay_ms( check_time );                 //��ʱ
			if((strEsp8266_Fram_Record.InfBit.FramLength != 0)&&((length_bak == 0)||(strEsp8266_Fram_Record.InfBit.FramLength != length_bak)))
			{
				length_bak = strEsp8266_Fram_Record.InfBit.FramLength;
				continue;
			}
			else if(strEsp8266_Fram_Record.InfBit.FramLength == 0)
				continue;
			
	//		if(strstr((char*)strEsp8266_Fram_Record.Data_RX_BUF, "ERROR"))
	//		{
	//			if((reply3 != 0)&&strstr((char*)strEsp8266_Fram_Record.Data_RX_BUF, reply3))
	//				return 0;
	//			else
	//				return 1;
	//		}
			if((reply1 != 0) && ( reply2 != 0))
				if(strstr((char*)strEsp8266_Fram_Record.Data_RX_BUF, reply1) 
					&& strstr((char*)strEsp8266_Fram_Record.Data_RX_BUF, reply2))
					result = 0;
				else
					result = 1;
			else if((reply1 != 0))
				if(strstr((char*)strEsp8266_Fram_Record.Data_RX_BUF, reply1))
					result = 0;
				else
					result = 1;
			else 
				if(strstr((char*)strEsp8266_Fram_Record.Data_RX_BUF, reply2))
					result = 0;
				else
					result = 1;
		}
	}
	macPC_Usart( "%s", strEsp8266_Fram_Record.Data_RX_BUF );
	ESP8266_RxBufClear();
	return result;
}

/*
 * ��������ESP8266_AT_Test
 * ����  ����WF-ESP8266ģ�����AT��������
 * ����  ����
 * ����  : ��
 * ����  �����ⲿ����
 */
//void ESP8266_AT_Test( void )
//{
//	macESP8266_RST_HIGH_LEVEL();
//	
//	Delay_ms( 1000 ); 
//	
//	while( ! ESP8266_Cmd( "AT", "OK", NULL, 500 ) ) ESP8266_Rst();  	

//}
void ESP8266_AT_Test( void )
{
	char count=0;
	
	macESP8266_RST_HIGH_LEVEL();	
	Delay_ms( 1000 );
	while( count < 10 )
	{
		if(!ESP8266_Cmd( "AT", "OK", NULL, 500)) return;
		ESP8266_Rst();
		++ count;
	}
}


/*
 * ��������ESP8266_Net_Mode_Choose
 * ����  ��ѡ��WF-ESP8266ģ��Ĺ���ģʽ
 * ����  ��enumMode������ģʽ
 * ����  : 1��ѡ��ɹ�
 *         0��ѡ��ʧ��
 * ����  �����ⲿ����
 */
uint8_t ESP8266_Net_Mode_Choose( ENUM_Net_ModeTypeDef enumMode )
{
	char resp[20];
	sprintf( resp, "+CWMODE_CUR:%d", enumMode);
	if(!ESP8266_Cmd( "AT+CWMODE_CUR?", "OK", resp, 1000 ))
		return 0;
	switch( enumMode )
	{
		case STA:
			return ESP8266_Cmd( "AT+CWMODE_DEF=1", "OK", 0, 2500 ); 
		
	  case AP:
		  return ESP8266_Cmd( "AT+CWMODE_DEF=2", "OK", 0, 2500 ); 
		
		case STA_AP:
		  return ESP8266_Cmd( "AT+CWMODE_DEF=3", "OK", 0, 2500 ); 
		
	  default:
		  return 1;
  }
	
}

/*
 * ��������ESP8266_Net_Mode_Choose
 * ����  ��ѡ��WF-ESP8266ģ��Ĺ���ģʽ
 * ����  ��enumMode������ģʽ
 * ����  : 1��ѡ��ɹ�
 *         0��ѡ��ʧ��
 * ����  �����ⲿ����
 */
uint8_t ESP8266_Net_Auto_Connect(void)
{
	char resp[20];
	sprintf( resp, "+CWAUTOCONN:%d", 1);
	if(!ESP8266_Cmd( "AT+CWAUTOCONN?", "OK", resp, 1000 ))
		return 0;
	return ESP8266_Cmd( "AT+CWAUTOCONN=1", "OK", 0, 1000 ); 
}

/*
 * ��������ESP8266_JoinAP
 * ����  ��WF-ESP8266ģ�������ⲿWiFi
 * ����  ��pSSID��WiFi�����ַ���
 *       ��pPassWord��WiFi�����ַ���
 * ����  : 1�����ӳɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
uint8_t ESP8266_JoinAP( char * pSSID, char * pPassWord )
{
	char cCmd [120];

	sprintf( cCmd, "AT+CWJAP=\"%s\",\"%s\"", pSSID, pPassWord );
	
	return ESP8266_Cmd( cCmd, "OK", NULL, 5000 );
	
}


/*
 * ��������ESP8266_BuildAP
 * ����  ��WF-ESP8266ģ�鴴��WiFi�ȵ�
 * ����  ��pSSID��WiFi�����ַ���
 *       ��pPassWord��WiFi�����ַ���
 *       ��enunPsdMode��WiFi���ܷ�ʽ�����ַ���
 * ����  : 1�������ɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
uint8_t ESP8266_BuildAP( char * pSSID, char * pPassWord, ENUM_AP_PsdMode_TypeDef enunPsdMode )
{
	char cCmd [120];

	sprintf( cCmd, "AT+CWSAP=\"%s\",\"%s\",1,%d", pSSID, pPassWord, enunPsdMode );
	
	return ESP8266_Cmd( cCmd, "OK", 0, 1000 );
	
}


/*
 * ��������ESP8266_Enable_MultipleId
 * ����  ��WF-ESP8266ģ������������
 * ����  ��enumEnUnvarnishTx�������Ƿ������
 * ����  : 0�����óɹ�
 *         1������ʧ��
 * ����  �����ⲿ����
 */
uint8_t ESP8266_Enable_MultipleId( FunctionalState enumEnUnvarnishTx )
{
	char cStr [20];
	
	sprintf( cStr, "AT+CIPMUX=%d",( enumEnUnvarnishTx ? 1 : 0 ) );
	
	return ESP8266_Cmd( cStr, "OK", 0, 500 );
	
}


/*
 * ��������ESP8266_Link_Server
 * ����  ��WF-ESP8266ģ�������ⲿ������
 * ����  ��enumE������Э��
 *       ��ip��������IP�ַ���
 *       ��ComNum���������˿��ַ���
 *       ��id��ģ�����ӷ�������ID
 * ����  : 1�����ӳɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
uint8_t ESP8266_Link_Server( ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id)
{
	char cStr [100] = { 0 }, cCmd [120];

  switch(  enumE )
  {
		case enumTCP:
		  sprintf( cStr, "\"%s\",\"%s\",%s", "TCP", ip, ComNum );
		  break;
		
		case enumUDP:
		  sprintf( cStr, "\"%s\",\"%s\",%s", "UDP", ip, ComNum );
		  break;
		
		default:
			break;
  }

  if( id < 5 )
    sprintf( cCmd, "AT+CIPSTART=%d,%s", id, cStr);

  else
	  sprintf( cCmd, "AT+CIPSTART=%s", cStr );

	return ESP8266_Cmd( cCmd, "OK", 0, 10000 );
	
}


/*
 * ��������ESP8266_StartOrShutServer
 * ����  ��WF-ESP8266ģ�鿪����رշ�����ģʽ
 * ����  ��enumMode������/�ر�
 *       ��pPortNum���������˿ں��ַ���
 *       ��pTimeOver����������ʱʱ���ַ�������λ����
 * ����  : 1�������ɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
uint8_t ESP8266_StartOrShutServer( FunctionalState enumMode, char * pPortNum, char * pTimeOver )
{
	char cCmd1 [120], cCmd2 [120];

	if( enumMode )
	{
		sprintf( cCmd1, "AT+CIPSERVER=%d,%s", 1, pPortNum );
		
		sprintf( cCmd2, "AT+CIPSTO=%s", pTimeOver );
		if(!ESP8266_Cmd( cCmd1, "OK", 0, 500 ))
			return ESP8266_Cmd( cCmd2, "OK", 0, 500 );
		else
			return 1;
	}
	
	else
	{
		sprintf( cCmd1, "AT+CIPSERVER=%d,%s", 0, pPortNum );

		return ESP8266_Cmd( cCmd1, "OK", 0, 500 );
	}
	
}


/*
 * ��������ESP8266_Get_LinkStatus
 * ����  ����ȡ WF-ESP8266 ������״̬�����ʺϵ��˿�ʱʹ��
 * ����  ����
 * ����  : 2�����ip
 *         3����������
 *         3��ʧȥ����
 *         0����ȡ״̬ʧ��
 * ����  �����ⲿ����
 */
uint8_t ESP8266_Get_LinkStatus( void )
{
	if( !ESP8266_Cmd( "AT+CIPSTATUS", "OK", 0, 500 ) )
	{
		if( strstr((char*)strEsp8266_Fram_Record.Data_RX_BUF, "STATUS:2\r\n" ) )
			return 2;
		
		else if( strstr((char*)strEsp8266_Fram_Record.Data_RX_BUF, "STATUS:3\r\n" ) )
			return 3;
		
		else if( strstr((char*)strEsp8266_Fram_Record.Data_RX_BUF, "STATUS:4\r\n" ) )
			return 4;		

	}
	
	return 0;
	
}


/*
 * ��������ESP8266_Get_IdLinkStatus
 * ����  ����ȡ WF-ESP8266 �Ķ˿ڣ�Id������״̬�����ʺ϶�˿�ʱʹ��
 * ����  ����
 * ����  : �˿ڣ�Id��������״̬����5λΪ��Чλ���ֱ��ӦId5~0��ĳλ����1���Id���������ӣ�������0���Idδ��������
 * ����  �����ⲿ����
 */
uint8_t ESP8266_Get_IdLinkStatus( void )
{
	uint8_t ucIdLinkStatus = 0x00;
	
	
	if( !ESP8266_Cmd( "AT+CIPSTATUS", "OK", 0, 500 ) )
	{
		if( strstr((char*)strEsp8266_Fram_Record.Data_RX_BUF, "+CIPSTATUS:0," ) )
			ucIdLinkStatus |= 0x01;
		else 
			ucIdLinkStatus &= ~ 0x01;
		
		if( strstr((char*)strEsp8266_Fram_Record.Data_RX_BUF, "+CIPSTATUS:1," ) )
			ucIdLinkStatus |= 0x02;
		else 
			ucIdLinkStatus &= ~ 0x02;
		
		if( strstr((char*)strEsp8266_Fram_Record.Data_RX_BUF, "+CIPSTATUS:2," ) )
			ucIdLinkStatus |= 0x04;
		else 
			ucIdLinkStatus &= ~ 0x04;
		
		if( strstr((char*)strEsp8266_Fram_Record.Data_RX_BUF, "+CIPSTATUS:3," ) )
			ucIdLinkStatus |= 0x08;
		else 
			ucIdLinkStatus &= ~ 0x08;
		
		if( strstr((char*)strEsp8266_Fram_Record.Data_RX_BUF, "+CIPSTATUS:4," ) )
			ucIdLinkStatus |= 0x10;
		else 
			ucIdLinkStatus &= ~ 0x10;	

	}
	
	return ucIdLinkStatus;
	
}


/*
 * ��������ESP8266_Inquire_ApIp
 * ����  ����ȡ F-ESP8266 �� AP IP
 * ����  ��pApIp����� AP IP ��������׵�ַ
 *         ucArrayLength����� AP IP ������ĳ���
 * ����  : 1����ȡʧ��
 *         0����ȡ�ɹ�
 * ����  �����ⲿ����
 */
uint8_t ESP8266_Inquire_ApIp( char * pApIp, uint8_t ucArrayLength )
{
	char uc;
	
	char * pCh;
	
	
  ESP8266_Cmd( "AT+CIFSR", "OK", 0, 500 );
	
	pCh = strstr((char*)strEsp8266_Fram_Record.Data_RX_BUF, "APIP,\"" );
	
	if( pCh )
		pCh += 6;
	
	else
		return 1;
	
	for( uc = 0; uc < ucArrayLength; uc ++ )
	{
		pApIp [ uc ] = *( pCh + uc);
		
		if( pApIp [ uc ] == '\"' )
		{
			pApIp [ uc ] = '\0';
			break;
		}
		
	}
	
	return 0;
	
}


/*
 * ��������ESP8266_UnvarnishSend
 * ����  ������WF-ESP8266ģ�����͸������
 * ����  ����
 * ����  : 0�����óɹ�
 *         1������ʧ��
 * ����  �����ⲿ����
 */
uint8_t ESP8266_UnvarnishSend( void )
{
	if(ESP8266_Cmd( "AT+CIPMODE=1", "OK", 0, 500 ) )
		return 1;	
	return ESP8266_Cmd( "AT+CIPSEND", "OK", ">", 500 );	
}


/*
 * ��������ESP8266_ExitUnvarnishSend
 * ����  ������WF-ESP8266ģ���˳�͸��ģʽ
 * ����  ����
 * ����  : ��
 * ����  �����ⲿ����
 */
void ESP8266_ExitUnvarnishSend( void )
{
	Delay_ms( 1000 );
	
	macESP8266_Usart( "+++" );
	
	Delay_ms( 500 ); 
	
}


/*
 * ��������ESP8266_SendString
 * ����  ��WF-ESP8266ģ�鷢���ַ���
 * ����  ��enumEnUnvarnishTx�������Ƿ���ʹ����͸��ģʽ
 *       ��pStr��Ҫ���͵��ַ���
 *       ��ulStrLength��Ҫ���͵��ַ������ֽ���
 *       ��ucId���ĸ�ID���͵��ַ���
 * ����  : 0�����ͳɹ�
 *         1������ʧ��
 * ����  �����ⲿ����
 */
uint8_t ESP8266_SendString( FunctionalState enumEnUnvarnishTx, char * pStr, uint32_t ulStrLength, ENUM_ID_NO_TypeDef ucId )
{
	char cStr [20];
	uint8_t bRet = 1;
	
		
	if( enumEnUnvarnishTx )
	{
		macESP8266_Usart( "%s", pStr );
		
		bRet = 0;
		
	}

	else
	{
		if( ucId < 5 )
			sprintf( cStr, "AT+CIPSEND=%d,%d", ucId, ulStrLength + 2 );

		else
			sprintf( cStr, "AT+CIPSEND=%d", ulStrLength + 2 );
		
		ESP8266_Cmd( cStr, "> ", 0, 1000 );

		bRet = ESP8266_Cmd( pStr, "SEND OK", 0, 1000 );
  }
	
	return bRet;

}


/*
 * ��������ESP8266_ReceiveString
 * ����  ��WF-ESP8266ģ������ַ���
 * ����  ��enumEnUnvarnishTx�������Ƿ���ʹ����͸��ģʽ
 * ����  : ���յ����ַ����׵�ַ
 * ����  �����ⲿ����
 */
uint8_t* ESP8266_ReceiveString( FunctionalState enumEnUnvarnishTx )
{
	uint8_t * pRecStr = 0;
	
	
	strEsp8266_Fram_Record.InfBit.FramLength = 0;
	strEsp8266_Fram_Record.InfBit.FramFinishFlag = 0;
	
	while( ! strEsp8266_Fram_Record.InfBit.FramFinishFlag );
	strEsp8266_Fram_Record.Data_RX_BUF [ strEsp8266_Fram_Record.InfBit.FramLength ] = '\0';
	
	if( enumEnUnvarnishTx )
		pRecStr = strEsp8266_Fram_Record.Data_RX_BUF;
	
	else 
	{
		if( strstr((char*)strEsp8266_Fram_Record.Data_RX_BUF, "+IPD" ) )
			pRecStr = strEsp8266_Fram_Record.Data_RX_BUF;

	}

	return pRecStr;
	
}

uint8_t ESP8266_SmartConfig(void)
{
	uint8_t result = 1;
	if(!ESP8266_Cmd( "AT+CWSTARTSMART=3", "OK", 0, 2000 ))
	{
		ESP8266_RxBufClear();
		if(ESP8266_ReceiveStatusString("smartconfig connected wifi",0,60000) == 0)
			result = 0;
		else
			result = 2;
		
	}
	ESP8266_Cmd( "AT+CWSTOPSMART", "OK", 0, 1000 );
	return result;	
}

void ESP8266_UnvarnishReceive(void)
{
	uint32_t length;
	if(strEsp8266_Fram_Record.InfBit.FramLength == 0)
		return;
	else
	{
		length = strEsp8266_Fram_Record.InfBit.FramLength;
		Delay_ms( 50 );
		while(length != strEsp8266_Fram_Record.InfBit.FramLength)
		{
			Delay_ms( 50 );
		}
		
		//����͸��Э������
		macPC_Usart("%s", strEsp8266_Fram_Record.Data_RX_BUF);
		ESP8266_RxBufClear();
	}

}

/*
*/

uint8_t ESP8266_ReceiveStatusString(char * string1, char * string2, uint32_t timeout)
{
	uint32_t i;
	uint8_t result = 2;
//	uint32_t check_cnt,check_time = 50;
	
//	check_cnt = timeout/check_time;
	Delay_ms( timeout );                 //��ʱ
	for(i=0; i<sizeof(strEsp8266_Fram_Record.Data_RX_BUF)-3;i++)
	{
		if(strEsp8266_Fram_Record.Data_RX_BUF[i] == 0x00)
			strEsp8266_Fram_Record.Data_RX_BUF[i] = 0x01;
	}
//	while(check_cnt--)
//	{
//		Delay_ms( check_time );                 //��ʱ
		if((string1 != 0) && ( string2 != 0))
		{
			if(strstr((char*)strEsp8266_Fram_Record.Data_RX_BUF, string1)) 
				result = 0;
			else if(strstr((char*)strEsp8266_Fram_Record.Data_RX_BUF, string2))
				result = 1;
			else
				result = 2;
		}
		else if((string1 != 0))
			if(strstr((char*)strEsp8266_Fram_Record.Data_RX_BUF, string1))
				result = 0;
			else
				result = 2;
		else 
			if(strstr((char*)strEsp8266_Fram_Record.Data_RX_BUF, string2))
				result = 0;
			else
				result = 2;
//	}
	macPC_Usart("%s", strEsp8266_Fram_Record.Data_RX_BUF);
	ESP8266_RxBufClear();
	return result;
}

