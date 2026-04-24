// Filename	: EC_PageDownloaderMan.h
// Creator	: Xiao Zhou
// Date		: 2008/4/10

#ifndef EC_PAGEDOWNLOADER_H
#define EC_PAGEDOWNLOADER_H

#include "EC_BBSFileDownloader.h"
#include "vector.h"

class CECPageDownloaderMan
{
public:

	typedef void (* LPFDOWNLOADCALLBACK)(int taskid, AString localfn, bool success);

	struct PAGE_DOWNLOADER_TASK
	{
		AString url;
		AString localfile;
		int id;
		char count;
		time_t starttime;
	};

	CECPageDownloaderMan();
	~CECPageDownloaderMan();
	
	bool Init(LPFDOWNLOADCALLBACK pCallback);
	void Run();
	void Release();

	bool AddNewTask(const char* szUrl, const char* szLocalFile, int id);

private:
	void InsertTask(const PAGE_DOWNLOADER_TASK &task);

	CRITICAL_SECTION						m_csTask;
	abase::vector<PAGE_DOWNLOADER_TASK>		m_vecTask;
	HANDLE									m_hPageDownloaderThread;
	CECBBSFileDownloader					m_FileDownloader;
	LPFDOWNLOADCALLBACK						m_pDownloadCallback;
};

#endif