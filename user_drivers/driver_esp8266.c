#include "driver_esp8266.h"
#include "driver_common.h"
#include "driver_delay.h"



static void                   ESP8266_GPIO_Config                ( void );
static void                   ESP8266_USART_Config               ( void );
//static void                   ESP8266_USART_NVIC_Configuration   ( void );
volatile uint8_t ucTcpClosedFlag = 0;
UART_HandleTypeDef huart3;
/*********************************************** ESP8266 函数宏定义 *******************************************/

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
  * @brief  ESP8266初始化函数
  * @param  无
  * @retval 无
  */
void ESP8266_Init( void )
{
	ESP8266_GPIO_Config(); 
	ESP8266_USART_Config();
	
	macESP8266_RST_HIGH_LEVEL();
	macESP8266_CH_DISABLE();
}


/**
  * @brief  初始化ESP8266用到的GPIO引脚
  * @param  无
  * @retval 无
  */
static void ESP8266_GPIO_Config( void )
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;


	/* 配置 CH_PD 引脚*/
	// Configure the GPIO_LED pin
	GPIO_InitStructure.Pin   = macESP8266_CH_PD_PIN;
	GPIO_InitStructure.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull  = GPIO_NOPULL;
	GPIO_InitStructure.Speed  = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(macESP8266_CH_PD_PORT, &GPIO_InitStructure);
	
	/* 配置 RST 引脚*/											   
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
//		if( strEsp8266_Fram_Record.InfBit.FramLength > ( RX_BUF_MAX_LEN - 1 ) )                      //预留1个字节写结束符
//			strEsp8266_Fram_Record.InfBit.FramLength = 0;
//		strEsp8266_Fram_Record.Data_RX_BUF[strEsp8266_Fram_Record.InfBit.FramLength++]  = ucCh;
//	}
//	 	 
//	if( __HAL_UART_GET_IT_SOURCE(&huart3, UART_IT_IDLE) & USART_CR1_IDLEIE)                                         //数据帧接收完毕
//	{
//    strEsp8266_Fram_Record.InfBit.FramFinishFlag = 1;
//		
//		ucCh = (uint16_t)(huart3.Instance->DR & (uint16_t)0x01FF);                                                           //由软件序列清除中断标志位(先读USART_SR，然后读USART_DR)
//	
//		ucTcpClosedFlag = strstr((char*)strEsp8266_Fram_Record.Data_RX_BUF, "CLOSED\r\n" ) ? 1 : 0;
//		
//  }	
	
	// Received completed interrupt(RXNE) if RXNEIE=1 in the USARTx_CR1 register
	if((huart3.Instance->SR & USART_SR_RXNE) && (huart3.Instance->CR1&USART_CR1_RXNEIE ))
	{
		ucCh  = (uint8_t)(huart3.Instance->DR &(uint8_t)0x00FF);
		
		if( strEsp8266_Fram_Record.InfBit.FramLength > ( RX_BUF_MAX_LEN - 1 ) )                      //预留1个字节写结束符
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
 * 函数名：ESP8266_Rst
 * 描述  ：重启WF-ESP8266模块
 * 输入  ：无
 * 返回  : 无
 * 调用  ：被 ESP8266_AT_Test 调用
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
 * 函数名：ESP8266_Cmd
 * 描述  ：对WF-ESP8266模块发送AT指令
 * 输入  ：cmd，待发送的指令
 *         reply1，reply2，期待的响应，为NULL表不需响应，两者为或逻辑关系
 *         waittime，等待响应的时间
 * 返回  : 1，指令发送成功
 *         0，指令发送失败
 * 调用  ：被外部调用
 */
uint8_t ESP8266_Cmd( char * cmd, char * reply1, char * reply2, uint32_t waittime )
{ 
	uint8_t result = 1;
	uint32_t check_cnt,check_time = 50;
	uint32_t length_bak = 0;
	
	ESP8266_RxBufClear();
//	strEsp8266_Fram_Record.InfBit.FramLength = 0;               //从新开始接收新的数据包
//	memset(strEsp8266_Fram_Record.Data_RX_BUF, 0, sizeof(strEsp8266_Fram_Record.Data_RX_BUF));
	macESP8266_Usart( "%s\r\n", cmd );
	macPC_Usart("%s\r\n", cmd);

	if(( reply1 == 0 ) && ( reply2 == 0 ) )                      //不需要接收数据
		result = 0;
	else
	{
		check_cnt = waittime/check_time;
		while(check_cnt--)
		{
			Delay_ms( check_time );                 //延时
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
 * 函数名：ESP8266_AT_Test
 * 描述  ：对WF-ESP8266模块进行AT测试启动
 * 输入  ：无
 * 返回  : 无
 * 调用  ：被外部调用
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
 * 函数名：ESP8266_Net_Mode_Choose
 * 描述  ：选择WF-ESP8266模块的工作模式
 * 输入  ：enumMode，工作模式
 * 返回  : 1，选择成功
 *         0，选择失败
 * 调用  ：被外部调用
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
 * 函数名：ESP8266_Net_Mode_Choose
 * 描述  ：选择WF-ESP8266模块的工作模式
 * 输入  ：enumMode，工作模式
 * 返回  : 1，选择成功
 *         0，选择失败
 * 调用  ：被外部调用
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
 * 函数名：ESP8266_JoinAP
 * 描述  ：WF-ESP8266模块连接外部WiFi
 * 输入  ：pSSID，WiFi名称字符串
 *       ：pPassWord，WiFi密码字符串
 * 返回  : 1，连接成功
 *         0，连接失败
 * 调用  ：被外部调用
 */
uint8_t ESP8266_JoinAP( char * pSSID, char * pPassWord )
{
	char cCmd [120];

	sprintf( cCmd, "AT+CWJAP=\"%s\",\"%s\"", pSSID, pPassWord );
	
	return ESP8266_Cmd( cCmd, "OK", NULL, 5000 );
	
}


/*
 * 函数名：ESP8266_BuildAP
 * 描述  ：WF-ESP8266模块创建WiFi热点
 * 输入  ：pSSID，WiFi名称字符串
 *       ：pPassWord，WiFi密码字符串
 *       ：enunPsdMode，WiFi加密方式代号字符串
 * 返回  : 1，创建成功
 *         0，创建失败
 * 调用  ：被外部调用
 */
uint8_t ESP8266_BuildAP( char * pSSID, char * pPassWord, ENUM_AP_PsdMode_TypeDef enunPsdMode )
{
	char cCmd [120];

	sprintf( cCmd, "AT+CWSAP=\"%s\",\"%s\",1,%d", pSSID, pPassWord, enunPsdMode );
	
	return ESP8266_Cmd( cCmd, "OK", 0, 1000 );
	
}


/*
 * 函数名：ESP8266_Enable_MultipleId
 * 描述  ：WF-ESP8266模块启动多连接
 * 输入  ：enumEnUnvarnishTx，配置是否多连接
 * 返回  : 0，配置成功
 *         1，配置失败
 * 调用  ：被外部调用
 */
uint8_t ESP8266_Enable_MultipleId( FunctionalState enumEnUnvarnishTx )
{
	char cStr [20];
	
	sprintf( cStr, "AT+CIPMUX=%d",( enumEnUnvarnishTx ? 1 : 0 ) );
	
	return ESP8266_Cmd( cStr, "OK", 0, 500 );
	
}


/*
 * 函数名：ESP8266_Link_Server
 * 描述  ：WF-ESP8266模块连接外部服务器
 * 输入  ：enumE，网络协议
 *       ：ip，服务器IP字符串
 *       ：ComNum，服务器端口字符串
 *       ：id，模块连接服务器的ID
 * 返回  : 1，连接成功
 *         0，连接失败
 * 调用  ：被外部调用
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
 * 函数名：ESP8266_StartOrShutServer
 * 描述  ：WF-ESP8266模块开启或关闭服务器模式
 * 输入  ：enumMode，开启/关闭
 *       ：pPortNum，服务器端口号字符串
 *       ：pTimeOver，服务器超时时间字符串，单位：秒
 * 返回  : 1，操作成功
 *         0，操作失败
 * 调用  ：被外部调用
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
 * 函数名：ESP8266_Get_LinkStatus
 * 描述  ：获取 WF-ESP8266 的连接状态，较适合单端口时使用
 * 输入  ：无
 * 返回  : 2，获得ip
 *         3，建立连接
 *         3，失去连接
 *         0，获取状态失败
 * 调用  ：被外部调用
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
 * 函数名：ESP8266_Get_IdLinkStatus
 * 描述  ：获取 WF-ESP8266 的端口（Id）连接状态，较适合多端口时使用
 * 输入  ：无
 * 返回  : 端口（Id）的连接状态，低5位为有效位，分别对应Id5~0，某位若置1表该Id建立了连接，若被清0表该Id未建立连接
 * 调用  ：被外部调用
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
 * 函数名：ESP8266_Inquire_ApIp
 * 描述  ：获取 F-ESP8266 的 AP IP
 * 输入  ：pApIp，存放 AP IP 的数组的首地址
 *         ucArrayLength，存放 AP IP 的数组的长度
 * 返回  : 1，获取失败
 *         0，获取成功
 * 调用  ：被外部调用
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
 * 函数名：ESP8266_UnvarnishSend
 * 描述  ：配置WF-ESP8266模块进入透传发送
 * 输入  ：无
 * 返回  : 0，配置成功
 *         1，配置失败
 * 调用  ：被外部调用
 */
uint8_t ESP8266_UnvarnishSend( void )
{
	if(ESP8266_Cmd( "AT+CIPMODE=1", "OK", 0, 500 ) )
		return 1;	
	return ESP8266_Cmd( "AT+CIPSEND", "OK", ">", 500 );	
}


/*
 * 函数名：ESP8266_ExitUnvarnishSend
 * 描述  ：配置WF-ESP8266模块退出透传模式
 * 输入  ：无
 * 返回  : 无
 * 调用  ：被外部调用
 */
void ESP8266_ExitUnvarnishSend( void )
{
	Delay_ms( 1000 );
	
	macESP8266_Usart( "+++" );
	
	Delay_ms( 500 ); 
	
}


/*
 * 函数名：ESP8266_SendString
 * 描述  ：WF-ESP8266模块发送字符串
 * 输入  ：enumEnUnvarnishTx，声明是否已使能了透传模式
 *       ：pStr，要发送的字符串
 *       ：ulStrLength，要发送的字符串的字节数
 *       ：ucId，哪个ID发送的字符串
 * 返回  : 0，发送成功
 *         1，发送失败
 * 调用  ：被外部调用
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
 * 函数名：ESP8266_ReceiveString
 * 描述  ：WF-ESP8266模块接收字符串
 * 输入  ：enumEnUnvarnishTx，声明是否已使能了透传模式
 * 返回  : 接收到的字符串首地址
 * 调用  ：被外部调用
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
		
		//处理透传协议数据
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
	Delay_ms( timeout );                 //延时
	for(i=0; i<sizeof(strEsp8266_Fram_Record.Data_RX_BUF)-3;i++)
	{
		if(strEsp8266_Fram_Record.Data_RX_BUF[i] == 0x00)
			strEsp8266_Fram_Record.Data_RX_BUF[i] = 0x01;
	}
//	while(check_cnt--)
//	{
//		Delay_ms( check_time );                 //延时
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

