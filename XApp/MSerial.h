#ifndef __MIDWARE_MSERIAL_H
#define	__MIDWARE_MSERIAL_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "MUtility.h"

extern U8  m_rxBuf[256];                              // received data buffer
extern U8  m_rxLen;                                   // received data size
extern U8  m_txBuf[256];                              // transmit data buffer
extern U8  m_txLen;                                   // transmit data size

//void USART1_OffReceive(void);                      // turn off receiver
//void USART1_PreReceive(void);                      // prepare for receiving
//void USART1_PreTransmit(void);                     // prepare for transmitting
//void USART1_StartSend(void);                       // start to transmit
//U8   USART1_GetByte(U8* pData);                    // get a byte from USAR6T-buffer
//U8   USART1_Write(U8* pData, U8 len);              // write some bytes to USART6-buffer
//U8   USART1_GetStatus(void);                       // get usart6 state
		
U16 OnNewSend(void);                               // FALSE:no data to send
U16 OnNewRecv(void);                               // FALSE:do not deal the received data
		
void MSerial_DoLoop(U16 tmOnce);

#ifdef __cplusplus
}
#endif

#endif /* __MIDWARE_MSERIAL_H */
