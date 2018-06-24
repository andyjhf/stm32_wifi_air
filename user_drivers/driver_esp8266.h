#ifndef  __BSP_ESP8266_H
#define	 __BSP_ESP8266_H



#include "driver_common.h"
#include <stdio.h>
#include <stdbool.h>

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif


/******************************* ESP8266 数据类型定义 ***************************/
typedef enum{
	STA = 1,
  AP,
  STA_AP  
} ENUM_Net_ModeTypeDef;


typedef enum{
	 enumTCP,
	 enumUDP,
} ENUM_NetPro_TypeDef;
	

typedef enum{
	Multiple_ID_0 = 0,
	Multiple_ID_1 = 1,
	Multiple_ID_2 = 2,
	Multiple_ID_3 = 3,
	Multiple_ID_4 = 4,
	Single_ID_0 = 5,
} ENUM_ID_NO_TypeDef;
	

typedef enum{
	OPEN = 0,
	WEP = 1,
	WPA_PSK = 2,
	WPA2_PSK = 3,
	WPA_WPA2_PSK = 4,
} ENUM_AP_PsdMode_TypeDef;



/******************************* ESP8266 外部全局变量声明 ***************************/
#define RX_BUF_MAX_LEN     1024                                     //最大接收缓存字节数

extern struct  STRUCT_USARTx_Fram                                  //串口数据帧的处理结构体
{
	uint8_t  Data_RX_BUF [ RX_BUF_MAX_LEN ];
	
  union {
    __IO uint16_t InfAll;
    struct {
		  __IO uint16_t FramLength       :15;                               // 14:0 
		  __IO uint16_t FramFinishFlag   :1;                                // 15 
	  } InfBit;
  }; 
	
} strEsp8266_Fram_Record;


/****************************************** ESP8266 函数声明 ***********************************************/
void       ESP8266_Init                        ( void );
void       ESP8266_Rst                         ( void );
uint8_t    ESP8266_Cmd                         ( char * cmd, char * reply1, char * reply2, uint32_t waittime );
void       ESP8266_AT_Test                     ( void );
uint8_t    ESP8266_Net_Mode_Choose             ( ENUM_Net_ModeTypeDef enumMode );
uint8_t    ESP8266_JoinAP                      ( char * pSSID, char * pPassWord );
uint8_t    ESP8266_BuildAP                     ( char * pSSID, char * pPassWord, ENUM_AP_PsdMode_TypeDef enunPsdMode );
uint8_t    ESP8266_Enable_MultipleId           ( FunctionalState enumEnUnvarnishTx );
uint8_t    ESP8266_Link_Server                 ( ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id);
uint8_t    ESP8266_StartOrShutServer           ( FunctionalState enumMode, char * pPortNum, char * pTimeOver );
uint8_t    ESP8266_Get_LinkStatus              ( void );
uint8_t    ESP8266_Get_IdLinkStatus            ( void );
uint8_t    ESP8266_Inquire_ApIp                ( char * pApIp, uint8_t ucArrayLength );
uint8_t    ESP8266_UnvarnishSend               ( void );
void       ESP8266_ExitUnvarnishSend           ( void );
uint8_t    ESP8266_SendString                  ( FunctionalState enumEnUnvarnishTx, char * pStr, uint32_t ulStrLength, ENUM_ID_NO_TypeDef ucId );
uint8_t*   ESP8266_ReceiveString               ( FunctionalState enumEnUnvarnishTx );
uint8_t    ESP8266_SmartConfig(void);
void       ESP8266_UnvarnishReceive(void);
uint8_t    ESP8266_ReceiveStatusString(char * string1, char * string2, uint32_t timeout);
void       ESP8266_RxBufClear(void);
uint8_t    ESP8266_Net_Auto_Connect(void);

#endif
