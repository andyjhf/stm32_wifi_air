#ifndef __DRIVER_UART6_H
#define __DRIVER_UART6_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "driver_utility.h"

#define BUF_SIZE 255

// RS-485 half-duplex
typedef enum 
{
	USART_RX = 0,
	USART_TX = 1
} USART_STATUES;

typedef struct
{
	uint8_t       txBuff[BUF_SIZE+1];              // receive and transmit buffer
	uint8_t       txStart;                         // receive cursor 0-255
	uint8_t       txEnd;                           // transmit cursor 0-255
	uint8_t       rxBuff[BUF_SIZE+1];              // receive and transmit buffer
	uint8_t       rxStart;                         // receive cursor 0-255
	uint8_t       rxEnd;                           // transmit cursor 0-255
	USART_STATUES status;                          // uart status of receive or transmit
}USART_DATA;

void USART1_OffReceive(void);                      // turn off receiver
void USART1_PreReceive(void);                      // prepare for receiving
void USART1_PreTransmit(void);                     // prepare for transmitting
void USART1_StartSend(void);                       // start to transmit
void USART1_Init(void);                            // initialize USART1
uint8_t USART1_GetByte(uint8_t* pData);            // get a byte from USAR6T-buffer
uint8_t USART1_Write(uint8_t* pData, uint8_t len); // write some bytes to USART1-buffer
uint8_t USART1_GetStatus(void);                    // get USART1 state

#ifdef __cplusplus
}
#endif

#endif /*__DRIVER_UART6_H */

