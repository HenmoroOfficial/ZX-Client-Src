/**
* @File EC_ThreadPing.h
* @Brief gethostbyname会阻塞线程，通过线程池进行调用
* @Author Fu Chonggang
* @Date 2011-7-14 10:53:55
* @Copyright (c) 2011 ZhuXian Studio, All Rights Reserved. 
*/

///////////////////////////////////////////////////////////////////////////////
#include "DlgLoginServerList.h"
#include <vector>

typedef CDlgLoginServerList::SERVERENTRY* ELEMENT;

class CECPool;

class CECWorkThread
{
public:
	CECWorkThread(CECPool* man);
	~CECWorkThread();
	static DWORD WINAPI Run(void * ptr);

	void stop();

public:
	HANDLE		m_Handle;
	CECPool*	m_pManager;
	bool		m_bRun;
};


class CECPool
{
public:
	CECPool(int threadNum);
	~CECPool();
	void AddTask(ELEMENT t, DWORD dwTimeout = 500);
protected:
public:
	
	std::vector<ELEMENT>	m_pElements;        // Array of elements to be processed
	int      m_nMaxElements;     // Maximum # of elements in the array

	std::vector<CECWorkThread*>	m_vecThreads;
};