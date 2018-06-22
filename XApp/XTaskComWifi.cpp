#include "XTaskComWifi.h"
#include "XApp.h"

CXTaskComWifi::CXTaskComWifi(void)
{
	ESP8266_Init();

	m_smartConfig  = 0;                                 // enable to serial polling between host and wifi module
	m_linkState = 0;                                 // factory reset flag(by dip)
}

CXTaskComWifi::~CXTaskComWifi(void)
{
}

void CXTaskComWifi::InitTask(void)
{
	ESP8266_Init();
	m_smartConfig  = 0;                                 // enable to serial polling between host and wifi module
	m_linkState = 0;                                 // factory reset flag(by dip)
}

void CXTaskComWifi::DetectFactory(void)
{
	if(HOST_DAT_SW1_ON==(g_platform[PLATFORM_INDEX_SW_VAL]&0x3f))          // 1-0-0-0-0-0
	{
		m_smartConfig = 1;                             // generate a factory reset flag
	}
}




void CXTaskComWifi::DoReal(void)
{
	if(m_smartConfig)
	{
		switch(step)
		{
			case 0:
				if(send == 0)
					ESP8266_send ("AT+CWMODE_CUR?");
			break;
			case 1:
				
			break;
			case 2:
				
			break;
			case 3:
				
			break;
			case 4:
				
			break;
			case 5:
				
			break;
		}
		
		switch(step)
		{
			case 0:
				
			break;
			case 0:
				
			break;
			case 0:
				
			break;
			case 0:
				
			break;
			case 0:
				
			break;
			case 0:
				
			break;
		}
		
	}
	else
	{
		
	}
	struct prop *prop;
	if (1 == WIFI_Ready())	                       // wifi ready pin is OK
	{
		// factory reset(setup mode is different from user mode)
		if ((m_factory || m_factory2) && !serial_tx_cmd(ACMD_LOAD_FACTORY, NULL, 0)) 
		{
			m_enable  = 0;
			if(m_factory2)                       // it should be reset MCU if it's from cloud 2016.08.19 
			{
				g_hostReset = 1;                 // reset MCU(need to listen enable)
			}
			m_factory = 0;                       // factory reset flag(by dip)
			m_factory2 = 0;                      // factory reset flag2(by cmd)
		}

		// enable prop processing
		if(m_enable)
		{
			conf_poll();                           // config poll
			conf_wifi_poll(30);                    // wifi poll
			prop_poll();                           // prop poll

			if (sched_next_event_tick &&           // schedule
				(ayla_tick() == sched_next_event_tick || cmp_gt(ayla_tick(), sched_next_event_tick))) 
			{
				sched_run_all(&sched_next_event_tick);
			}
			
			if(m_startAp && !conf_wifi_start_ap()) // start ap mode
			{
				m_startAp=0;
			}
		}

		// serial rx/tx
		serial_poll();

		prop = prop_lookup_error();                // clear prop error
		if (prop != NULL)
		{
			prop->send_err = 0;
		}
	}
	else
	{
		// TODO: risk 
		m_enable=1;
	}
}

void CXTaskComWifi::ParseConf(U8 *command, U8 size)
{
	if(size>=1)
	{
		switch (command[0])
		{
			case 1:
				m_startAp = 1;                     // 0x01 start ap mode
				break;
			case 2:
				// m_factory = 1;                  // 0x02 factory2 reset from cloud
				m_factory2 = 1;
				break;
			default:
				break;
		}
	}
}

void CXTaskComWifi::ResetFactory(void)
{
	m_factory2 = 1;
}
