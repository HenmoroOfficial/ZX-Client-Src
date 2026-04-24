// Filename	: DlgExplorer.h
// Creator	: Xiao Zhou
// Date		: 2009/6/10

#pragma once

#include "PWBrowser.h"
#include "DlgBase.h"
#include "AUI\\AUIWindowPicture.h"
#include "AUI\\WndThreadManager.h"
#include "aui\\AUIListBox.h"
#include "vector.h"
#include "hashmap.h"

#define USE_GLOBAL_BROWSER 1 //0采用独立BROWSER，1采用全局BROWSER
class CDlgExplorer;
class CPWBrowser;
struct DOWNLOADSTATUS
{
	ULONG ulProgress;
	ULONG ulProgressMax;
	ULONG ulStatusCode;
	//LPCWSTR szStatusText;
};

struct DOWNLOADER_TASK
{
	AWString url;
	AWString localfile;
	int id;
	//char count;
	//time_t starttime;
};

struct DOWNLOADING_TASK
{
	AWString url;
	AWString localfile;
	DOWNLOADSTATUS status;
};
BOOL InitBrowser();
BOOL FreeBrowser();
#define STATUS_TXT_LEN 256
class CExplorerMan: public CPWBrowserMan
{
public:
	CExplorerMan();
	virtual ~CExplorerMan();
	virtual bool OnInitDialog();
	BOOL CreateBrowser(AString strName);
	void Stop();
	void HideBrowser();
	void ShowBrowser();
	BOOL AddDownloadUrl(WCHAR *FileUrl);
	void ThreadRun();
	BOOL InitTaskList();
	void NavigateUrl(WCHAR *URL);
	void GoForward();
	void GoBackward();
	void Refresh();
	bool GetDownloadState(abase::vector<AWString>& vecTaskDone, abase::vector<AWString>& vecTaskToDo, AWString& curDownFile, int &curDownProgress);
	double GetDownloadRate();
	bool IsDownloading();
	void OnWinTimer();
	void OnTick();
	void SetDownloadStatus(ULONG ulProgress,ULONG ulProgressMax,LONG ulStatusCode,LPCWSTR szStatusText);
	BOOL GetDownloadingUrlFileSize(UINT* plength);   

	typedef abase::hash_map<AString, CPWBrowser*>	BrowserTable;
protected:
	bool              m_bUpdateDownLoadState;
	bool              m_bDownLoading;
	bool              m_bUpdateDownData;
	bool              m_bInTimer;
	WCHAR             m_szDownStatusText[STATUS_TXT_LEN];	
	CPWBrowser        *m_pBrowser;	
	HANDLE            m_hDownloadThread;
	DWORD             m_dwDownloadThreadId;
	abase::vector<DOWNLOADER_TASK>	m_vecTaskDone;
	abase::vector<DOWNLOADER_TASK>	m_vecTaskToDo;
	DOWNLOADING_TASK                m_currentTask;
	HANDLE           				m_mutex;
	HANDLE                          m_event;
	WCHAR                           m_wsPath[MAX_PATH];
	
	BrowserTable m_mapBrowsers;

	static unsigned long WINAPI  DownloadThread(LPVOID pDlgExplorer); 
private:
	CDlgExplorer*                   m_pDlg;
public:
	CPWBrowser* GetBrowser(){return m_pBrowser;};
	CDlgExplorer* GetDlgExplorer() { return m_pDlg; }
	BOOL BindUI(CDlgExplorer *pDlg);
	BOOL ReleaseUI();
};
extern CExplorerMan *g_pExplorerMan;
extern CPWBrowser    *g_pBrowser;

class CBSCallbackImpl : public IBindStatusCallback
{
public:
	CBSCallbackImpl(CExplorerMan* pDlgExplorer, HANDLE hEventStop);

	// IUnknown methods
	STDMETHOD(QueryInterface)(REFIID riid, void **ppvObject);
	STDMETHOD_(ULONG, AddRef)();
	STDMETHOD_(ULONG, Release)();

	// IBindStatusCallback methods
	STDMETHOD(OnStartBinding)(DWORD, IBinding *);
	STDMETHOD(GetPriority)(LONG *);
	STDMETHOD(OnLowResource)(DWORD);
	STDMETHOD(OnProgress)(ULONG ulProgress,
						  ULONG ulProgressMax,
						  ULONG ulStatusCode,
						  LPCWSTR szStatusText);
	STDMETHOD(OnStopBinding)(HRESULT, LPCWSTR);
	STDMETHOD(GetBindInfo)(DWORD *, BINDINFO *);
	STDMETHOD(OnDataAvailable)(DWORD, DWORD, FORMATETC *, STGMEDIUM *);
	STDMETHOD(OnObjectAvailable)(REFIID, IUnknown *);

protected:
	ULONG m_ulObjRefCount;
	UINT  m_filesize;
private:
	CExplorerMan *m_pDlgExplorer;
	HANDLE m_hEventStop;

};

enum
{
	UF_BINDSTATUS_FIRST = BINDSTATUS_FINDINGRESOURCE,
	UF_BINDSTATUS_LAST = BINDSTATUS_ACCEPTRANGES
};

class CDlgExplorer : public CDlgBase, public CPWMessageTranslate  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
		
public:
	CDlgExplorer();
	virtual ~CDlgExplorer();
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	void OnTick();	
	bool MessageTranslate(AString Control, UINT msg, WPARAM wParam, LPARAM lParam);
	BOOL PeekMessage_Filter(LPMSG lpMsg,HWND hWnd);
	BOOL AddDownloadUrl(WCHAR *FileUrl);
	BOOL InitTaskList();
	void NavigateUrl(const WCHAR *URL);
	void NavigateUrl(const CHAR *Url);
	void OnWinTimer();
	BOOL FreeExplorerMan();
	const CExplorerMan* GetExplorerMan(){return m_pExploreMan;};
	const PAUIWINDOWPICTURE   GetWPic() {return m_pWPic;};
	void OnCommandCancel(const char *szCommand);
	void GetCurrentUrl();
	void GoForward();
	void GoBackward();
	void Refresh();
	void OnHideDialog();
	bool GetDownloadState(abase::vector<AWString>& vecTaskDone, abase::vector<AWString>& vecTaskToDo, AWString& curDownFile, int &curDownProgress);	
	double GetDownloadRate();
	bool IsDownloading();
protected:
	bool              m_bInTimer;	
	PAUIWINDOWPICTURE m_pWPic;
	//PAUILISTBOX     m_pFileList;
	//CPWBrowser      *m_pBrowser;
	CExplorerMan      *m_pExploreMan;
	void KeyEvent(UINT uMsg,WPARAM wParam,LPARAM lParam);
	void KeyEvent(HWND hwnd, UINT uMsg,WPARAM wParam,LPARAM lParam);
	void MouseEvent(HWND hwnd, UINT uMsg,WPARAM wParam,LPARAM lParam);
	void MouseEventX(HWND hwnd, UINT uMsg,WPARAM wParam,LPARAM lParam);
	void MouseEvent(UINT uMsg,WPARAM wParam,LPARAM lParam);
	HWND GetCaretWindowPos(LPRECT lprect);
	CWndThreadManager m_WndThreadManager;
	/*
	abase::vector<DOWNLOADER_TASK>	*m_pVecTaskDone;
	abase::vector<DOWNLOADER_TASK>	*m_pVecTaskToDo;
	DOWNLOADING_TASK                *m_pCurrentTask;
	*/
};

//extern CDlgExplorer dlgExplorer;
