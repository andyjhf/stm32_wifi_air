#ifndef __COMMON_H
#define __COMMON_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "driver_utility.h"



/******************************* 宏定义 ***************************/
#define            macNVIC_PriorityGroup_x                     NVIC_PriorityGroup_2



/********************************** 函数声明 ***************************************/
void USART_printf( UART_HandleTypeDef * USARTx, char * Data, ... );

#ifdef __cplusplus
}
#endif

#endif /* __COMMON_H */

