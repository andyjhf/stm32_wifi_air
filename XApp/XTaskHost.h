#ifndef __APP_XTASKHOST_H
#define	__APP_XTASKHOST_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "MUtility.h"

#define WIFI_SIGNAL_STRENGTH_5   5		
#define WIFI_SIGNAL_STRENGTH_4   4     //WiFi signal grade is 4
#define WIFI_SIGNAL_STRENGTH_3   3    
#define WIFI_SIGNAL_STRENGTH_2   2
#define WIFI_SIGNAL_STRENGTH_1   1
#define WIFI_SIGNAL_STRENGTH_0   0
		
void WiFi_rank();            //WiFi strength into grade

class CXTaskHost
{
public:
	CXTaskHost(void);
	~CXTaskHost(void);

public:
	void InitTask(void);                           // task initialization
	void DoLoop(U16 tmOnce);                       // task loop
	void ParseCommand(U8 *command, U8 size);       // command routine for host reset/wifi module reset

private:
	void sampling(void);                           // sampling routine

private:
	U16 m_tmSampling;                              // sampling period(5ms)
	U16 m_tmBlink1;                                // quick blinking period(250ms)
	U16 m_tmBlink2;                                // slow blinking period(2000ms)

	U16 m_samplingCnt;                             // sampling up-counter
	U16 m_blinkCnt1;                               // quick blinking up-counter
	U16 m_blinkCnt2;                               // slow blinking up-counter

	U8 m_swiBit[6];                                // dip switch sampling buffer
	U8 m_readyBit;                                 // wifi ready pin sampling buffer
	U8 m_linkBit;                                  // wifi link pin sampling buffer

	U8 m_wifiReady;                                // 1: wifi module ready(started)
	U8 m_wifiLink;                                 // 1: wifi module linked to ayla cloud
	U8 m_ervReady;                                 // 1: erv communication frame check state(timeout/crc error)
	U8 m_ervLink;                                  // 1: erv outdoor online flag

	U8 m_hostReset;

public:
	U16 m_PowerCnt;                                // 5 seconds counter after mcu reset
};

#ifdef __cplusplus
}
#endif

#endif /* __APP_XTASKHOST_H */
