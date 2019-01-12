#ifndef   _AM2302_H
#define   _AM2302_H


#include "driver_utility.h"
#include "stm32f1xx_hal.h"

//AM2302  IO  define
#define  AM2302_GPIO_PORT  GPIOB
#define  AM2302_PIN        GPIO_PIN_1		 
		 
//
#define  HIGH    1
#define  LOW     0
 
//???,???????????,?????????  
#define  AM2302_DATA_OUT(a) if (a)  \
                    HAL_GPIO_WritePin(AM2302_GPIO_PORT, AM2302_PIN, GPIO_PIN_SET);\
                    else        \
                    HAL_GPIO_WritePin(AM2302_GPIO_PORT, AM2302_PIN, GPIO_PIN_RESET)
 //???????  
#define  AM2302_DATA_IN()      HAL_GPIO_ReadPin(AM2302_GPIO_PORT, AM2302_PIN)	 
		 
void AM2302_GPIO_Config(void);  
static void AM2302_Mode_IPU(void);  
static void AM2302_Mode_Out_PP(void);  
uint8_t Read_AM2302(uint16_t *temp, uint16_t *humi); 
static uint8_t Read_Byte(void);  
										
#endif
