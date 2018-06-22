#ifndef __APP_XTASKWIFI_H
#define	__APP_XTASKWIFI_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "MUtility.h"

class CXTaskComWifi
{
public:
	CXTaskComWifi(void);
	~CXTaskComWifi(void);

public:
	void InitTask(void);                           // task initialization
	void DetectFactory(void);                      // detect factory setting
	void DoReal(void);                             // task loop
	void ParseConf(U8 *command, U8 size);          // command routine for wifi module
	void ResetFactory(void);                       // Reset Factory
private:
	U8 m_linkState;                                   // enable to receive and transmit props
	U8 m_smartConfig;                                  // start wifi module to ap mode
};

#ifdef __cplusplus
}
#endif

#endif /* __APP_XTASKWIFI_H */
