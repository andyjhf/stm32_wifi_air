/*******************************************************************************
 * @file    driver_USART1.c
 * @version V1.0.0
 * @date    2016-01-26
 * @brief   This file provides all rs485 functions.
 *******************************************************************************/
 
#include "driver_uart1.h"

USART_DATA	g_usart3;

void USART3_IRQHandler(void)
{
	// Received completed interrupt(RXNE) if RXNEIE=1 in the USARTx_CR1 register
	if((USART3->SR & USART_SR_RXNE) && (USART3->CR1&USART_CR1_RXNEIE ))
	{
		// Auto cleared by a read to the DR
		g_usart3.rxBuff[g_usart3.rxEnd++] = USART3->DR;
	}
	// Transmit completed interrupt(TC) if TCIE=1 in the USARTx_CR1 register
	else if((USART3->SR & USART_SR_TC) && (USART3->CR1 & USART_CR1_TCIE))
	{
		if(g_usart3.txStart!=g_usart3.txEnd)
		{
			// Auto cleared by a write to the DR
			USART3->DR= g_usart3.txBuff[g_usart3.txStart++];
		}
		else
		{
			USART3_PreReceive();
		}
	}
	else
	{
		// Note: STM32F411 can be cleared by a read to the DR
		g_usart3.rxBuff[g_usart3.rxEnd++] = USART3->DR;
	}
}

void USART3_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	UART_HandleTypeDef huart3;

	/* Peripheral clock enable */
	__HAL_RCC_USART3_CLK_ENABLE();

	/**USART3 GPIO Configuration    
	PB10     ------> USART3_TX
	PB11     ------> USART3_RX 
	*/
	GPIO_InitStruct.Pin = GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }


	// Enable USART3_IRQn
	HAL_NVIC_SetPriority(USART3_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(USART3_IRQn);

	// Set USART3 receive state
	__HAL_UART_ENABLE_IT(&huart3,UART_IT_RXNE);
	__HAL_UART_ENABLE_IT(&huart3,UART_IT_TC);
//	USART3_PreTransmit();

	// Enable USART3
	__HAL_UART_ENABLE(&huart3);
}

void USART3_OffReceive(void)
{
	USART3->CR1 &= ~USART_CR1_RE;                  // Receiver disable
}

void USART3_PreReceive(void)
{
	// note: STM32F411 can be cleared by writing a '0'
	USART3->SR &=  ~USART_SR_TC;                   // clear TC interrupt flag
//	USART3->SR &=  ~USART_SR_RXNE;                 // clear RXNE interrupt flag
//	USART3->CR1 &= ~USART_CR1_TCIE;                // disable TC interrupt
//	USART3->CR1 |= USART_CR1_RXNEIE;               // enable RXNE interrupt
//	USART3->CR1 |= USART_CR1_RE;                   // Receiver Enable

	g_usart3.rxStart= 0;                           // clear buffer and set receive state
	g_usart3.rxEnd  = 0;
//	RS485_DE_PORT->BSRRH = RS485_DE_PIN;           // reset receive status(0) of RS485 chip
	g_usart3.status = USART_RX;                    // set usart state for receving
}

void USART3_PreTransmit(void)
{
//	USART3->SR &=  ~USART_SR_TC;                   // clear TC interrupt flag
	USART3->SR &=  ~USART_SR_RXNE;                 // clear RXNE interrupt flag
//	USART3->CR1 &= ~USART_CR1_RXNEIE;              // disable RXNE interrupt
//	USART3->CR1 |= USART_CR1_TCIE;                 // enable TC interrupt
//	USART3->CR1 &= ~USART_CR1_RE;                  // Receiver disable

//	RS485_DE_PORT->BSRRL = RS485_DE_PIN;           // set transmit status(1) of RS485 chip
	g_usart3.txStart  = 0;                         // clear buffer and set transmit state
	g_usart3.txEnd    = 0;                         // note: do not clear end 
	g_usart3.status = USART_TX;                    // set usart state for transmitting
}

void USART3_StartSend(void)
{
	// Transmit first data to DR
	USART3->DR = g_usart3.txBuff[g_usart3.txStart++];
}

uint8_t USART3_GetByte(uint8_t* pData)
{
	uint8_t size=0;
	if(g_usart3.rxStart != g_usart3.rxEnd)
	{
		pData[size++] = g_usart3.rxBuff[g_usart3.rxStart++];
	}
	return size;
}

uint8_t USART3_Write(uint8_t* pData, uint8_t len)
{
	while(len--)
	{
		g_usart3.txBuff[g_usart3.txEnd++] = *pData++;
	}
	return g_usart3.txEnd;
}

uint8_t USART3_GetStatus(void)
{
	return g_usart3.status;
}
