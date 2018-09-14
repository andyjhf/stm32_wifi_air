/*******************************************************************************
 * @file    MSerial.cpp
 * @author  jianchuan.zhao
 * @version V1.0.0
 * @date    12/12/2014
 * @brief   This file provides all ... functions.
 *******************************************************************************/
 
#include "MSerial.h"

// weak functions
__weak void USART3_OffReceive(void) {}                   // turn off receiver
__weak void USART3_PreReceive(void) {}                   // prepare for receiving
__weak void USART3_PreTransmit(void){}                   // prepare for transmitting
__weak void USART3_StartSend(void)  {}                   // start to transmit
__weak U8   USART3_GetByte(U8* pData)       { return 0;} // get a byte from USAR6T-buffer
__weak U8   USART3_Write(U8* pData, U8 len) { return 0;} // write some bytes to USART1-buffer
__weak U8   USART3_GetStatus(void)          { return 0;} // get USART1 state

__weak U16 OnNewSend()  {return 0;}           // FALSE:no data to send
__weak U16 OnNewRecv()  {return 0;}           // FALSE:do not deal the received data

static void OnSend(void);                                 // handle the data should be sent
static void Waiting(void);                                // wait a lillte interval between receive completion and send starting, data line output mode
static void Sending(void);                                // send one by one and check whether the end of transmitting 
static void Receiving(U16 tmOnce);                    // receive one by one and check whether the completion of receiving
static void OnReceive(void);                              // handle the received data

#define S_ONSEND     0                             // OnSend state
#define S_WAITING    1                             // waiting state
#define S_SENDING    2                             // sending state
#define S_RECEIVE    3                             // receing state
#define S_ONRECV     4                             // receive over state

static U8 m_state   = S_ONSEND;                         // state
static U16 m_stateCnt= 0;                                 // time count of one state

U8  m_rxBuf[256];                              // received data buffer
U8  m_rxLen = 0;                                   // received data size

U8  m_txBuf[256];                              // transmit data buffer
U8  m_txLen = 0;                                   // transmit data size

static U16 m_idleCnt = 0;                                 // timer count of interval between one char and one char
static U16 m_onesendCnt = 15000;															//timer count of send frequency

static U16 m_tmRxOver = 15;                                // configured interval time
static U16 m_tmNoAck = 2000;                                 // configured no acknowledge time
static U16 m_tmWait = 15;                                  // configured waiting time


void MSerial_DoLoop(U16 tmOnce)
{
	U8 state=m_state;                              // hold current state
	switch(state)                                  // switch case
	{
		case S_ONSEND:                             // handle to send state
			OnSend();                              // handle to format what data should be sent or not
			break;
		case S_WAITING:                            // waiting state
			Waiting();                             // wait fot a little while then start to send(rs485 send state)
			break;
		case S_SENDING:                            // sending state
			Sending();                             // check whether the sending is completed
			break;
		case S_RECEIVE:                            // receiving state
			Receiving(tmOnce);                     // copy uart rx data to receiver buffer
			break;
		case S_ONRECV:                             // receive over state
			OnReceive();                           // handle to received data
			break;
	}
	if(state!=m_state)                             // state change
	{
		m_stateCnt=0;                              // clear state counter
	}
	m_stateCnt += tmOnce;                          // count-up state counter
}

static void OnSend(void)
{
	m_txLen = 0;                                   // clear tranimit buffer
	if(m_stateCnt > m_onesendCnt)
	{
		if(OnNewSend())                                // handle whether there is data to send or not
		{
			m_state = S_WAITING;                       // if need to send data this time then turn to [waiting] state
			USART3_PreTransmit();                      // diable RXNEIE,enable TCIE and RS485-DE,clear buffer
			USART3_Write(m_txBuf, m_txLen);            // copy send data to uart tx buffer
		}
//		else
//		{
//			m_state = S_RECEIVE;                       // if there is no data to send, turn to [receive] state
//			m_rxLen = 0;                               // clear receiver buffer
//			USART1_PreReceive();                       // set uart to rx state
//		}
	}
}

static void Waiting(void)
{
	if(m_stateCnt>=m_tmWait)                       // waiting time is over
	{
		m_state = S_SENDING;                       // turn to [sending] state
		USART3_StartSend();                        // starting to transmit
	}
}

static void Sending(void)
{
	if(0==USART3_GetStatus())                      // send over
	{
		m_state = S_RECEIVE;                       // change to receive state
		m_rxLen = 0;                               // clear receiver buffer
//		USART1_PreReceive();                       // set uart to rx state
	}
}

static void Receiving(U16 tmOnce)
{
	while(USART3_GetByte(&m_rxBuf[m_rxLen]))       // get uart rx data to receiver buffer one by one
	{
		m_rxLen++;                                 // receiver length plus one
		m_idleCnt = 0;                             // clear idle counter
		if(m_rxLen==0xff)                          // out of buffer(maybe dirty data or bus noise of long time)
			break;
	}
	m_idleCnt += tmOnce;                           // count-up idle counter
	if((m_rxLen>0 && m_idleCnt>=m_tmRxOver) ||     // frame end, 
		m_idleCnt>m_tmNoAck || m_rxLen>=255)       // timeout, out of buffer
	{
//		USART1_OffReceive();                       // close uart rx function
		m_state = S_ONRECV;                        // change to OnReceive state
	}
}

static void OnReceive(void)
{
//	if(OnNewRecv())                                // handle to received data and cheak whether need to be sent
//	{
//		m_state = S_ONSEND;                        // turn to OnSend state
//		m_txLen = 0;                               // clear send buffer
//	}
//	else                                           // do not need to send data
//	{
//		m_state = S_RECEIVE;                       // turn to receive state
//		m_rxLen=0;                                 // clear receiver buffer
//		USART1_PreReceive();                       // set uart to rx state
//	}
	OnNewRecv();
	m_state = S_ONSEND;                        // turn to OnSend state
	m_txLen = 0;                               // clear send buffer
	
	m_idleCnt=0;                                   // clear idle counter
	m_stateCnt=0;                                  // clear state counter
}
