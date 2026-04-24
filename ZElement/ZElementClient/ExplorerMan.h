/*
 * FILE: ExplorerMan.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Wangrui, 2010/11/9
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "PWBrowser.h"

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

#define USE_GLOBAL_BROWSER 1 //0采用独立BROWSER，1采用全局BROWSER

///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////

class CExplorer;
class CDlgBaseExplorer;

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

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////

extern const AWString g_strBlankUrl;

///////////////////////////////////////////////////////////////////////////
//	
//	Class CBSCallbackImpl
//	
///////////////////////////////////////////////////////////////////////////

class CBSCallbackImpl : public IBindStatusCallback
{
public:		//	Types
	enum
	{
		UF_BINDSTATUS_FIRST = BINDSTATUS_FINDINGRESOURCE,
		UF_BINDSTATUS_LAST = BINDSTATUS_ACCEPTRANGES
	};

public:
	CBSCallbackImpl(CExplorer* pExplorer, HANDLE hEventStop);

	// IUnknown methods
	STDMETHOD(QueryInterface)(REFIID riid, void **ppvObject);
	STDMETHOD_(ULONG, AddRef)();
	STDMETHOD_(ULONG, Release)();

	// IBindStatusCallback methods
	STDMETHOD(OnStartBinding)(DWORD, IBinding *);
	STDMETHOD(GetPriority)(LONG *);
	STDMETHOD(OnLowResource)(DWORD);
	STDMETHOD(OnProgress)(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, LPCWSTR szStatusText);
	STDMETHOD(OnStopBinding)(HRESULT, LPCWSTR);
	STDMETHOD(GetBindInfo)(DWORD *, BINDINFO *);
	STDMETHOD(OnDataAvailable)(DWORD, DWORD, FORMATETC *, STGMEDIUM *);
	STDMETHOD(OnObjectAvailable)(REFIID, IUnknown *);

protected:
	ULONG m_ulObjRefCount;
	UINT  m_filesize;
private:
	CExplorer *m_pExplorer;
	HANDLE m_hEventStop;
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CExplorerMan
//	
///////////////////////////////////////////////////////////////////////////

class CExplorerMan
{
#ifdef BACK_VERSION
	DECLARE_SINGLETON(CExplorerMan);
#endif

public:		//	Types

	enum
	{
		EXPLORER_ERROR = -1,
		EXPLORER_DEFAULT,
		EXPLORER_XIANFU,
		EXPLORER_MUSIC_RADIO,
		EXPLORER_KNOWS,		//诛仙知道
		EXPLORER_LOGIN_WND,	//多平台登陆
		EXPLORER_WEBHOT,
		EXPLORER_GUILD_BBS,
		EXPLORER_NUM,
	};


public:
	virtual ~CExplorerMan();
	CExplorer* GetExplorer(int iIdx) { return (iIdx>EXPLORER_ERROR && iIdx<EXPLORER_NUM) ? m_aExplorers[iIdx] : NULL; }
	bool CreateExplorer(int iIdx, CDlgBaseExplorer* pDlg);
	bool PreCreateExplorer( HWND hParentWnd );
	void Release();
	static CExplorerMan& GetInstance() { return s_instance; }
	bool RemoveExplorer( int nIndex );
	bool PreTranslateMessage( const MSG& msg );

protected:
	CExplorer*	m_aExplorers[EXPLORER_NUM];

	static CExplorerMan s_instance;
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CExplorer
//	
///////////////////////////////////////////////////////////////////////////

class CExplorer
{
public:		//	Types
	enum
	{
		STATUS_TXT_LEN = 256,
	};

public:
	CExplorer(HWND hWnd);
	virtual ~CExplorer();
    virtual bool OnInitDialog();
	void SetUIScrollHeight(int nHeight);
	void HideBrowser();
	void ShowBrowser();
	void NavigateUrl(const CHAR* Url);
	void NavigateUrl(const WCHAR *URL);
	void GoForward();
	void GoBackward();
	void Refresh();
	BOOL AddDownloadUrl(WCHAR *FileUrl);
	void ThreadRun();
	BOOL InitTaskList();
	bool GetDownloadState(abase::vector<AWString>& vecTaskDone, abase::vector<AWString>& vecTaskToDo, AWString& curDownFile, int &curDownProgress);
	double GetDownloadRate();
	bool IsDownloading();
	void CancelCurDownLoad();
	void SetDownloadStatus(ULONG ulProgress,ULONG ulProgressMax,LONG ulStatusCode,LPCWSTR szStatusText);
	BOOL GetDownloadingUrlFileSize(UINT* plength); 
	void OnWinTimer();
	void OnTick();
	void OnDocBodyInnerText(const AWString& strText);
	void SetDrawOutside(bool bOutside);
	void SetNoDocScroll(bool bNoScroll);
	void SetBrwoserTrans(BYTE byTrans);

	CPWBrowser* GetBrowser() { return m_pBrowser; }
	bool Create();
	bool PreCreate();
	bool BindUI(CDlgBaseExplorer* pDlg);
	bool ReleaseUI();
	void SetWebBrowserSilent( bool bSilent = true );

	CDlgBaseExplorer * GetUI(){ return m_pDlgBind; }

protected:
	void _MyRelease();
	bool _Create();
	void _SyncOuterScrollBar();

protected:
	bool				m_bInTimer;
	WCHAR				m_szDownStatusText[STATUS_TXT_LEN];	
	CPWBrowser*			m_pBrowser;	
	UINT				m_nWndHeight;
	HWND				m_hGameInitWnd;
	A3DRECT				m_RectDisplay;
	bool				m_bDrawOutside;
	bool				m_bNoDocScroll;


	bool              m_bUpdateDownLoadState;
	bool              m_bDownLoading;
	bool              m_bUpdateDownData;
	//bool              m_bInTimer;
	bool              m_bCancelCurDownLoad;
	//WCHAR             m_szDownStatusText[STATUS_TXT_LEN];	
	//CPWBrowser        *m_pBrowser;	
	HANDLE            m_hDownloadThread;
	DWORD             m_dwDownloadThreadId;
	abase::vector<DOWNLOADER_TASK>	m_vecTaskDone;
	abase::vector<DOWNLOADER_TASK>	m_vecTaskToDo;
	DOWNLOADING_TASK                m_currentTask;
	HANDLE           				m_mutex;
	HANDLE                          m_event;
	WCHAR                           m_wsPath[MAX_PATH];
	static unsigned long WINAPI  DownloadThread(LPVOID pDlgExplorer); 

private:

	CDlgBaseExplorer*	m_pDlgBind;
};

//-----------------------------------------------------------------------------
/**
@usage: 解析浏览器中内容，并返回固定格式的用户名和密码信息，暂仅用于多平台接入需求
		考虑到此接口的特殊性，因此单独实现一个函数，而不归属于任何类中
@date:	Added 2012-05-09
@param:	pExplorer: 浏览器实例化对象；strName：如果解析内容成功，返回用户名，
		strPassword: 如果解析成功，返回密码
@return: true: 解析内容成功，false：无任何解析数据，或者解析数据失败
@warning:一般在浏览器收到返回数据时调用，也可以在OnTick中不断监听？
*/
bool CExplorerInfoParser( CExplorer* pExplorer, ACString& strName, ACString& strPassword );
//-----------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////