/*******************************************************************************
 * @file    driver_USART1.c
 * @author  jianchuan.zhao
 * @version V1.0.0
 * @date    2016-01-26
 * @brief   This file provides all rs485 functions.
 *******************************************************************************/
 
#include "driver_uart1.h"

USART_DATA	g_usart1;

void USART1_IRQHandler(void)
{
	// Received completed interrupt(RXNE) if RXNEIE=1 in the USARTx_CR1 register
	if((USART1->SR & USART_SR_RXNE) && (USART1->CR1&USART_CR1_RXNEIE ))
	{
		// Auto cleared by a read to the DR
		g_usart1.rxBuff[g_usart1.rxEnd++] = USART1->DR;
	}
	// Transmit completed interrupt(TC) if TCIE=1 in the USARTx_CR1 register
	else if((USART1->SR & USART_SR_TC) && (USART1->CR1 & USART_CR1_TCIE))
	{
		if(g_usart1.txStart!=g_usart1.txEnd)
		{
			// Auto cleared by a write to the DR
			USART1->DR= g_usart1.txBuff[g_usart1.txStart++];
		}
		else
		{
			USART1_PreReceive();
		}
	}
	else
	{
		// Note: STM32F411 can be cleared by a read to the DR
		g_usart1.rxBuff[g_usart1.rxEnd++] = USART1->DR;
	}
}

void USART1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	UART_HandleTypeDef huart1;

	/* Peripheral clock enable */
	__HAL_RCC_USART1_CLK_ENABLE();

	/**USART1 GPIO Configuration    
	PA9     ------> USART1_TX
	PA10     ------> USART1_RX 
	*/
	GPIO_InitStruct.Pin = GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }


	// Enable USART1_IRQn
	HAL_NVIC_SetPriority(USART1_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(USART1_IRQn);

	// Set USART1 receive state
	USART1_PreReceive();

	// Enable USART1
	__HAL_UART_ENABLE(&huart1);
}

void USART1_OffReceive(void)
{
	USART1->CR1 &= ~USART_CR1_RE;                  // Receiver disable
}

void USART1_PreReceive(void)
{
	// note: STM32F411 can be cleared by writing a '0'
	USART1->SR &=  ~USART_SR_TC;                   // clear TC interrupt flag
	USART1->SR &=  ~USART_SR_RXNE;                 // clear RXNE interrupt flag
	USART1->CR1 &= ~USART_CR1_TCIE;                // disable TC interrupt
	USART1->CR1 |= USART_CR1_RXNEIE;               // enable RXNE interrupt
	USART1->CR1 |= USART_CR1_RE;                   // Receiver Enable

//	RS485_DE_PORT->BSRRH = RS485_DE_PIN;           // reset receive status(0) of RS485 chip

	g_usart1.rxStart= 0;                           // clear buffer and set receive state
	g_usart1.rxEnd  = 0;
	g_usart1.status = USART_RX;                    // set usart state for receving
}

void USART1_PreTransmit(void)
{
	USART1->SR &=  ~USART_SR_TC;                   // clear TC interrupt flag
	USART1->SR &=  ~USART_SR_RXNE;                 // clear RXNE interrupt flag
	USART1->CR1 &= ~USART_CR1_RXNEIE;              // disable RXNE interrupt
	USART1->CR1 |= USART_CR1_TCIE;                 // enable TC interrupt
	USART1->CR1 &= ~USART_CR1_RE;                  // Receiver disable

//	RS485_DE_PORT->BSRRL = RS485_DE_PIN;           // set transmit status(1) of RS485 chip
	g_usart1.txStart  = 0;                         // clear buffer and set transmit state
	g_usart1.txEnd    = 0;                         // note: do not clear end 
	g_usart1.status = USART_TX;                    // set usart state for transmitting
}

void USART1_StartSend(void)
{
	// Transmit first data to DR
	USART1->DR = g_usart1.txBuff[g_usart1.txStart++];
}

uint8_t USART1_GetByte(uint8_t* pData)
{
	uint8_t size=0;
	if(g_usart1.rxStart != g_usart1.rxEnd)
	{
		pData[size++] = g_usart1.rxBuff[g_usart1.rxStart++];
	}
	return size;
}

uint8_t USART1_Write(uint8_t* pData, uint8_t len)
{
	while(len--)
	{
		g_usart1.txBuff[g_usart1.txEnd++] = *pData++;
	}
	return g_usart1.txEnd;
}

uint8_t USART1_GetStatus(void)
{
	return g_usart1.status;
}
