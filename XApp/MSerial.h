#ifndef __MIDWARE_MSERIAL_H
#define	__MIDWARE_MSERIAL_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "MUtility.h"

void USART6_OffReceive(void);                      // turn off receiver
void USART6_PreReceive(void);                      // prepare for receiving
void USART6_PreTransmit(void);                     // prepare for transmitting
void USART6_StartSend(void);                       // start to transmit
U8   USART6_GetByte(U8* pData);                    // get a byte from USAR6T-buffer
U8   USART6_Write(U8* pData, U8 len);              // write some bytes to USART6-buffer
U8   USART6_GetStatus(void);                       // get usart6 state
		
U16 OnNewSend(void);                               // FALSE:no data to send
U16 OnNewRecv(void);                               // FALSE:do not deal the received data
		
void MSerial_DoLoop(U16 tmOnce);

#ifdef __cplusplus
}
#endif

#endif /* __MIDWARE_MSERIAL_H */
