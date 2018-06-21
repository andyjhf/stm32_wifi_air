#include "XTaskComERV.h"
#include "XApp.h"

// note:
//#define	ID_IDLE_CNT		(20/CYCLE_ID)
//#define ID_NOACK_CNT	(80/CYCLE_ID)
//#define	ID_PRESEND_CNT	(10/CYCLE_ID)

CXTaskComERV::CXTaskComERV(void):CMSerial()
{
	m_tmRxOver = 5;
	m_tmNoAck  = 2000;
	m_tmWait   = 5;
	m_sendSlaveSddr = 0;

	m_recvCmd = 0;
	m_recvSrc = 0;

	//m_ctrlCmd
	m_queue   = new CMQueue(sizeof(tagXERVCtrlCmd),128);
	m_errCnt  = 0;
#if DEBUG_LOG == 1
	m_debug = 0;
#endif
}

CXTaskComERV::~CXTaskComERV(void)
{
}

void CXTaskComERV::InitTask(void)
{
	m_tmRxOver = 5;
	m_tmNoAck  = 2000;
	m_tmWait   = 5;
	m_sendSlaveSddr = 0;
	
	m_recvCmd = 0;
	m_recvSrc = 0;

	//m_ctrlCmd
	//m_queue = new CMQueue(sizeof(tagXCtrlCmd),128);

	m_errCnt  = 0;
#if DEBUG_LOG == 1
	m_debug = 0;
#endif
}

U16 CXTaskComERV::OnNewSend()
{
	if(m_queue->Pop(&m_ervctrlCmd))
	{
		
	}
#if DEBUG_LOG == 1
	else if(m_debug == 1)
	{
		m_debug = 0;
		m_txBuf[0] = 0x02;
		memcpy(&m_txBuf[1],g_szErvinfo,30);
		m_txLen = 31;
	}
#endif
	else{
		
	}
	return 1;
}

U16 CXTaskComERV::OnNewRecv()
{

	
}

void CXTaskComERV::ParseRemote(U8 *remote, U8 size)
{
	
}

void CXTaskComERV::SetReg(U8 group,U16 addr, U16 value)
{
	

}

U8 CXTaskComERV::checkFrame(void)
{
	
}

void CXTaskComERV::onError(void)
{
	
}
#if DEBUG_LOG == 1
void CXTaskComERV::debug(void)
{
	m_debug = 1;
}
#endif

