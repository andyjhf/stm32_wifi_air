#ifndef __APP_XTASKHOST_H
#define	__APP_XTASKHOST_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "MUtility.h"

void CXTaskHost_InitTask(void);                           // task initialization
//void CXTaskHost_DoLoop(U16 tmOnce);                       // task loop
//void ParseCommand(U8 *command, U8 size);       // command routine for host reset/wifi module reset


#ifdef __cplusplus
}
#endif

#endif /* __APP_XTASKHOST_H */
