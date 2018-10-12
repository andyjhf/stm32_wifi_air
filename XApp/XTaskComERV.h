#ifndef __APP_XTASKCOMVRF_H
#define	__APP_XTASKCOMVRF_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "MUtility.h"
#include "MSerial.h"

#define INDEX_AIR_PM1_0						4
#define INDEX_AIR_PM2_5						6
#define INDEX_AIR_PM10						8
#define INDEX_AIR_M0_3						16
#define INDEX_AIR_M0_5						18
#define INDEX_AIR_M1_0						20
#define INDEX_AIR_M2_5						22
#define INDEX_AIR_M5_0						24
#define INDEX_AIR_M10_0						26
#define INDEX_TVOC								28
#define INDEX_HCHO								31
#define INDEX_CO2									34
#define INDEX_TEMP								36
#define INDEX_HUMI								38

void CXTaskComERV_InitTask(void);
//void CXTaskComERV_ParseRemote(U8 *szRemote, U8 size);
//void CXTaskComERV_SetReg(U8 group,U16 addr, U16 value);

U16 OnNewSend(void);
U16 OnNewRecv(void);


#ifdef __cplusplus
}
#endif
#endif /* __APP_XTASKCOMVRF_H */
