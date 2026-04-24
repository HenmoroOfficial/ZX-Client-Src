#include "EC_ThreadPing.h"


HANDLE   m_h[2] = {0, 0};             // Mutex & semaphore handles
HANDLE   &m_PingMutex = m_h[0];           // Reference to m_h[0]
HANDLE   &m_PingSemaphore = m_h[1]; // Reference to m_h[1]


CECWorkThread::CECWorkThread(CECPool* man)
{
	m_bRun = true;
	m_pManager = man;
	DWORD dwThreadID;
	m_Handle = CreateThread(NULL, 0, Run, this, 0, &dwThreadID);
}

CECWorkThread::~CECWorkThread()
{
}

DWORD CECWorkThread::Run(void * ptr)
{
	CECWorkThread* pWork = (CECWorkThread*)ptr;
	CECPool*	pMan = pWork->m_pManager;
	while (pWork->m_bRun)
	{
		WaitForMultipleObjects(2, m_h, TRUE, INFINITE);
		if (pMan->m_pElements.size()>0)
		{
			ELEMENT serverEntry = pMan->m_pElements.back();
			pMan->m_pElements.pop_back();
			ReleaseMutex(m_PingMutex);
			struct hostent * addr = gethostbyname(serverEntry->info.vecAddress[0]);
			if (!addr)
			{
				continue;
			}
			
			char ** p;
			int n = 0;
			for(p=addr->h_addr_list;*p!=NULL;p++) 
				n++;
			if(n>0)
			{
				n = rand() % n;
				serverEntry->addr = *(struct in_addr *)(addr->h_addr_list[n]);
				serverEntry->getnameThread = NULL;
				serverEntry->addr_valid = true;
			}
		}
	}
	return 0;
}

void CECWorkThread::stop() 
{
	TerminateThread(m_Handle, 0);
	m_bRun=false; 
}

CECPool::CECPool(int threadNum)
{	
	m_nMaxElements = threadNum;
	m_PingMutex = CreateMutex(NULL, FALSE, NULL);
	m_PingSemaphore = CreateSemaphore(NULL, 0, 64, NULL);

	for(int i=0; i<threadNum; ++i)
		m_vecThreads.push_back(new CECWorkThread(this));
}

CECPool::~CECPool()
{
	for(UINT i=0; i<m_vecThreads.size(); ++i)
	{
		m_vecThreads[i]->stop();
		delete m_vecThreads[i];
	}

	CloseHandle(m_PingMutex);
	CloseHandle(m_PingSemaphore);

}

void CECPool::AddTask(ELEMENT t, DWORD dwTimeout)
{	
	BOOL fOk = FALSE;
	DWORD dw = WaitForSingleObject(m_PingMutex, dwTimeout);
	
	if (dw == WAIT_OBJECT_0) {
		m_pElements.push_back(t);
		ReleaseMutex(m_PingMutex);
		LONG lPrevCount;
		fOk = ReleaseSemaphore(m_PingSemaphore, 1, &lPrevCount);
		
	} else {
		// Timeout, set error code and return failure
		SetLastError(ERROR_TIMEOUT);
	}
}