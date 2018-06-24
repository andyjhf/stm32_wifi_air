#include "XTaskComERV.h"
#include "XApp.h"

// note:
//#define	ID_IDLE_CNT		(20/CYCLE_ID)
//#define ID_NOACK_CNT	(80/CYCLE_ID)
//#define	ID_PRESEND_CNT	(10/CYCLE_ID)
static U8 m_recvCmd;
static U8 m_recvParaCnt;
static U8 m_recvSrc;

static XERVCtrlCmd   m_ervctrlCmd;
static CMQueue *m_queue;

static U16 m_errCnt;
static U8	m_sendSlaveSddr;

void CXTaskComERV_InitTask(void)
{
	m_sendSlaveSddr = 0;
	
	m_recvCmd = 0;
	m_recvSrc = 0;

	//m_ctrlCmd
	m_queue = CMQueue_Init(sizeof(XERVCtrlCmd),20);

	m_errCnt  = 0;
}

U16 OnNewSend(void)
{
	if(CMQueue_Pop(m_queue,&m_ervctrlCmd))
	{
		
	}
	else{
		
	}
	return 1;
}

U16 OnNewRecv(void)
{

	return 0;
}

void CXTaskComERV_ParseRemote(U8 *remote, U8 size)
{
	
}

void CXTaskComERV_SetReg(U8 group,U16 addr, U16 value)
{
	

}

static U8 checkFrame(void)
{
	
	return 0;
}

static void onError(void)
{
	
}


