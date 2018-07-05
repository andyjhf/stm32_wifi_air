#include "XTaskComERV.h"
#include "XApp.h"

// note:
//#define	ID_IDLE_CNT		(20/CYCLE_ID)
//#define ID_NOACK_CNT	(80/CYCLE_ID)
//#define	ID_PRESEND_CNT	(10/CYCLE_ID)
#define CHARACTER_BYTE0							0x42
#define CHARACTER_BYTE1							0x4D

#define TIMEOUT_LOOP								5

#define CMD_AIR_READ_GENERAL_DATA				0xab
#define CMD_AIR_READ_ALL_DATA						0xac
#define CMD_AIR_MODULE_SLEEP_CTRL				0xe4

osThreadId XTaskAirModuleHandle;
static U8 m_sendCmd;

//static CMQueue *m_queue;

static U16 m_errCnt;

static uint16_t checkSum(uint8_t* buf, uint16_t length);
static U8 checkFrame(void);
static void onError(void);


void StartXTaskAirModule(void const * argument)
{
//	osDelay(30000);     //等待空气检测模块稳定
	for(;;)
  {
		MSerial_DoLoop(TIMEOUT_LOOP);
		osDelay(TIMEOUT_LOOP);
		IWDG_Feed();
	}

}

void CXTaskComERV_InitTask(void)
{

	//m_ctrlCmd
//	m_queue = CMQueue_Init(sizeof(XERVCtrlCmd),20);

	m_errCnt  = 0;

	osThreadDef(XTaskAirModule, StartXTaskAirModule, osPriorityAboveNormal, 0, 128);
  XTaskAirModuleHandle = osThreadCreate(osThread(XTaskAirModule), NULL);
}

U16 OnNewSend(void)
{
	m_txBuf[0] = CHARACTER_BYTE0;																				//slave address
	m_txBuf[1] = CHARACTER_BYTE1;																	//Fuctions
	m_txBuf[2] = CMD_AIR_READ_ALL_DATA;																		//Starting address HI
	m_txBuf[3] = 0x00;																		//Starting address LO
	m_txBuf[4] = 0x00;																		//Num of Registers Hi

	U16 usSUM16=checkSum(&m_txBuf[0], 0x05);
	m_txBuf[5] = HIBYTE(usSUM16);													//CRC LO
	m_txBuf[6] = LOBYTE(usSUM16);													//CRC HI

	m_txLen = 7;
	
	m_sendCmd = m_txBuf[2];
//	printf("send\r\n");
	return 1;
}

U16 OnNewRecv(void)
{
	uint8_t* p = m_rxBuf;
	if (1==checkFrame())                           // 1.check frame(length,header/tail,CRC)
	{
		g_airmoduleState = 0;
		onError();
		return 0;
	}
	m_errCnt = *p;
	m_errCnt = 0;
	g_airmoduleState = 1;
	if(m_sendCmd == CMD_AIR_READ_ALL_DATA)
	{
		g_AirPM1_0    = (m_rxBuf[INDEX_AIR_PM1_0]<<8) | m_rxBuf[INDEX_AIR_PM1_0+1];
		g_AirPM2_5    = (m_rxBuf[INDEX_AIR_PM2_5]<<8) | m_rxBuf[INDEX_AIR_PM2_5+1];
		g_AirPM10     = (m_rxBuf[INDEX_AIR_PM10]<<8) | m_rxBuf[INDEX_AIR_PM10+1];
		g_TVOC        = (m_rxBuf[INDEX_TVOC]<<8) | m_rxBuf[INDEX_TVOC+1];
		g_HCHO        = (m_rxBuf[INDEX_HCHO]<<8) | m_rxBuf[INDEX_HCHO+1];
		g_CO2         = (m_rxBuf[INDEX_CO2]<<8) | m_rxBuf[INDEX_CO2+1];
		g_Temperature = (m_rxBuf[INDEX_TEMP]<<8) | m_rxBuf[INDEX_TEMP+1];
		g_Humidity    = (m_rxBuf[INDEX_HUMI]<<8) | m_rxBuf[INDEX_HUMI+1];
//		printf("temp = %d.%d\r\n",g_Temperature/10,g_Temperature%10);
//		printf("g_AirPM2_5 = %d\r\n",g_AirPM2_5);
	}
	else if(m_sendCmd == CMD_AIR_READ_GENERAL_DATA)
	{
		
	}
	else if(m_sendCmd == CMD_AIR_MODULE_SLEEP_CTRL)
	{
	
	
	}
		
	return 1;
}

/*
void CXTaskComERV_ParseRemote(U8 *remote, U8 size)
{
	
}

void CXTaskComERV_SetReg(U8 group,U16 addr, U16 value)
{
	

}
*/
static U8 checkFrame(void)
{
	uint32_t i;
	// check frame length
	if(m_rxLen<8)
	{
		printf("timeout\r\n");
		return 1;
	}
	if((CHARACTER_BYTE0 != m_rxBuf[0])&&(CHARACTER_BYTE1 != m_rxBuf[1]))
	{
		printf("header error\r\n");
		return 1;
	}

	// check frame CRC (TODO:do not check CRC, reduce the execution time)
	if(checkSum(&m_rxBuf[0], m_rxLen-2)!= ((m_rxBuf[m_rxLen-2]<<8)|m_rxBuf[m_rxLen-1]))
	{
		printf("sum error\r\n");
		return 1;
	}

	return 0;
}

static void onError(void)
{
	if(m_errCnt<0xffff)
		m_errCnt++;

	if(0==m_rxLen)
	{
	//	g_rs485 = 1;
	}
	else
	{
//		g_rs485 = 0;
	}
	
	// determine the error kind after 30 secs(A/B wires reversed or A/B wires are open)
	if(m_errCnt>=300)
	{
		//led 显示异常
	}
}

static uint16_t checkSum(uint8_t* buf, uint16_t length)
{
	uint16_t i;
	uint16_t sum = 0;
	for(i=0; i<length; i++)
		sum += buf[i];
	return sum;
}

