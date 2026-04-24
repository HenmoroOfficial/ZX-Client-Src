// Filename	: EC_PageDownloaderMan.cpp
// Creator	: Xiao Zhou
// Date		: 2008/4/10

#include "AFI.h"
#include "EC_PageDownloaderMan.h"
#include "EC_Utility.h"
#include "EC_Global.h"
#include "EC_GameRun.h"

#define CECDOWNLOADMAN_MAX_RETRY_TIMES 3
#define CECDOWNLOADMAN_RETRY_INVTERVAL 5

const char* szTempFile = "cache\\playimage.tmp";

HANDLE g_hReleaseDownloader= NULL;

static DWORD WINAPI PageDownloaderThreadFunc(LPVOID lpParam)
{
	CECPageDownloaderMan* pDownloaderMan = (CECPageDownloaderMan*)lpParam;
	int iRet = -1;
	__try
	{
		pDownloaderMan->Run();
	}
	__except (glb_HandleException(GetExceptionInformation()))
	{
		::OutputDebugString(_AL("Exception occurred...\n"));
		a_LogOutput(1, "Exception occurred in page downloader thread!");
	}

	return iRet;
}



CECPageDownloaderMan::CECPageDownloaderMan()
{
	m_hPageDownloaderThread = NULL;
	InitializeCriticalSection(&m_csTask);
}

CECPageDownloaderMan::~CECPageDownloaderMan()
{
	DeleteCriticalSection(&m_csTask);
}

bool CECPageDownloaderMan::Init(LPFDOWNLOADCALLBACK pCallback)
{
	m_vecTask.clear();
	m_pDownloadCallback = pCallback;
	DWORD dwThreadID;
	m_hPageDownloaderThread = CreateThread(NULL, 0, PageDownloaderThreadFunc, this, CREATE_SUSPENDED, &dwThreadID);

	return true;
}

bool CECPageDownloaderMan::AddNewTask(const char* szUrl, const char* szLocalFile, int id)
{
	PAGE_DOWNLOADER_TASK task;
	task.url = szUrl;
	task.localfile = szLocalFile;
	task.id = id;
	task.count = 1;
	task.starttime = time(NULL);
	InsertTask(task);
	ResumeThread(m_hPageDownloaderThread);

	return true;
}

void CECPageDownloaderMan::InsertTask(const PAGE_DOWNLOADER_TASK &task)
{
	EnterCriticalSection(&m_csTask);
	DWORD i = 0;
	while(i < m_vecTask.size() && m_vecTask[i].starttime <= task.starttime )
		i++;
	m_vecTask.insert(m_vecTask.begin() + i, task);
	LeaveCriticalSection(&m_csTask);
}

void CECPageDownloaderMan::Run()
{
	while(1)
	{
		time_t t = time(NULL);
		EnterCriticalSection(&m_csTask);
		if( m_vecTask.size() == 0 )
		{
			LeaveCriticalSection(&m_csTask);
			SuspendThread(GetCurrentThread());
			continue;
		}
		PAGE_DOWNLOADER_TASK task = *m_vecTask.begin();
		if( task.starttime > t )
		{
			LeaveCriticalSection(&m_csTask);
			Sleep((task.starttime - t) * 1000);
		}
		else
		{
			m_vecTask.erase(m_vecTask.begin());
			LeaveCriticalSection(&m_csTask);
			DeleteFileA(szTempFile);
			if( !m_FileDownloader.DownloadByUrl(task.url, szTempFile) )
			{
				if( task.count < CECDOWNLOADMAN_MAX_RETRY_TIMES )
				{
					task.count++;
					task.starttime = time(NULL) + CECDOWNLOADMAN_RETRY_INVTERVAL;
					InsertTask(task);
				}
				else
					m_pDownloadCallback(task.id, task.localfile, false);
			}
			else
			{
				if( CopyFileA(szTempFile, task.localfile, FALSE) )
					m_pDownloadCallback(task.id, task.localfile, true);
			}
		}
	}
}

void CECPageDownloaderMan::Release()
{
	if (m_hPageDownloaderThread)
	{
		TerminateThread(m_hPageDownloaderThread, 0);
		CloseHandle(m_hPageDownloaderThread);
		m_hPageDownloaderThread = NULL;
	}
	DeleteFileA(szTempFile);
}