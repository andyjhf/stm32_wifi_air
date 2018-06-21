/**
  ******************************************************************************
  * @file    bsp_uart2.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ����c��printf������usart�˿�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "driver_uart2.h"

UART_HandleTypeDef huart2;
void USART2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	

	/* Peripheral clock enable */
	__HAL_RCC_USART2_CLK_ENABLE();

	/**USART1 GPIO Configuration    
	PA9     ------> USART1_TX
	PA10     ------> USART1_RX 
	*/
	GPIO_InitStruct.Pin = macUSART_TX_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = macUSART_RX_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	huart2.Instance = macUSARTx;
  huart2.Init.BaudRate = macUSART_BAUD_RATE;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }


	// Enable USART1_IRQn
	HAL_NVIC_SetPriority(USART2_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(USART2_IRQn);

	// Enable USART1
	__HAL_UART_ENABLE(&huart2);
	__HAL_UART_ENABLE_IT(&huart2,UART_IT_RXNE);
}

/// �ض���c�⺯��printf��USART1
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ�USART1 */
		huart2.Instance->DR = ch;
		
		/* �ȴ�������� */
		while (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TXE) == false);		
	
		return (ch);
}

/// �ض���c�⺯��scanf��USART1
int fgetc(FILE *f)
{
		/* �ȴ�����1�������� */
		while (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_RXNE) == false);

		return (int)__HAL_UART_FLUSH_DRREGISTER(&huart2);
}
/*********************************************END OF FILE**********************/
