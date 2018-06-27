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


void CXTaskHost_InitTask(void);                           // task initialization
void CXTaskHost_DoLoop(U16 tmOnce);                       // task loop
//void ParseCommand(U8 *command, U8 size);       // command routine for host reset/wifi module reset


#ifdef __cplusplus
}
#endif

#endif /* __APP_XTASKHOST_H */
