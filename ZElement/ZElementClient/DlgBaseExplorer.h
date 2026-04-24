/*
 * FILE: DlgBaseExplorer.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Wangrui, 2010/11/10
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "DlgBase.h"
#include "AUI\\AUIWindowPicture.h"

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////

class CExplorer;

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	
//	Class CDlgBaseExplorer
//	
///////////////////////////////////////////////////////////////////////////

class CDlgBaseExplorer : public CDlgBase, public CPWMessageTranslate 
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgBaseExplorer();
	virtual ~CDlgBaseExplorer();
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnHideDialog();
	virtual void OnTick();

	virtual bool MessageTranslate(AString Control, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual bool PeekMessage_Filter(LPMSG lpMsg, HWND hWnd);
	BOOL AddDownloadUrl(WCHAR *FileUrl);
	BOOL InitTaskList();

	virtual bool HasOuterScroll() { return m_bHasOuterScroll; }
	virtual void InitScroll(int nScrollHeight, int nWndHeight) {}
	virtual int  GetScrollTop() { return 0; }
	virtual void UpdateUIScroll(int nScrollPos) {}

	
	virtual void OnDocBodyInnerText(const AWString& strText) {}

	void CreateBrowser();	
	bool FreeExplorer();
	virtual void OnWinTimer();
	

	void NavigateUrl(const WCHAR *URL);
	void NavigateUrl(const CHAR *Url);
	void GetCurrentUrl();
	void GoForward();
	void GoBackward();
	void Refresh();

	bool GetDownloadState(abase::vector<AWString>& vecTaskDone, abase::vector<AWString>& vecTaskToDo, AWString& curDownFile, int &curDownProgress);	
	double GetDownloadRate();
	bool IsDownloading();
	void CancelCurDownLoad();

	bool IsNoDocScroll() { return m_bNoDocScroll; }
	const PAUIWINDOWPICTURE GetWPic() { return m_pWPic; }
	
	static bool SendHTTPByIE(LPCSTR   URL,LPCSTR   headers,const   BYTE   *Post,   DWORD   postLength);
	static DWORD WINAPI OpenURL(void *url);


	// event callback
	virtual void OnBeforeNavigate2(IDispatch** pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, VARIANT_BOOL* Cancel){}
	virtual void OnNavigateComplete2(IDispatch** pDisp, VARIANT* URL){}
	virtual void OnNewWindow2(IDispatch** ppDisp, VARIANT_BOOL* Cancel){}
	virtual void OnNewWindow3(IDispatch** ppDisp, VARIANT_BOOL* Cancel, unsigned long dwFlags, BSTR bstrUrlContext, BSTR bstrUrl){}
	virtual void OnWindowClosing(VARIANT_BOOL IsChildWindow, VARIANT_BOOL* Cancel){}
	virtual void OnQuit(){}
	virtual void DocumentComplete(IDispatch* pDisp,VARIANT* URL){}
	virtual void OnNavigateError(IDispatch* pDisp,VARIANT* URL,VARIANT* Frame,VARIANT* StatusCode,VARIANT_BOOL* Cancel){}
	virtual void OnFileDownLoad(VARIANT_BOOL ActiveDocument, VARIANT_BOOL* Cancel){}


protected:
	virtual HWND GetCaretWindowPos(LPRECT lprect);
	void KeyEvent(UINT uMsg,WPARAM wParam,LPARAM lParam){}
	void KeyEvent(HWND hwnd, UINT uMsg,WPARAM wParam,LPARAM lParam){}
	void MouseEvent(UINT uMsg,WPARAM wParam,LPARAM lParam){}
	virtual void MouseEvent(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void MouseEventX(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


	void OnCommand_Prev(const char *szCommand);
	void OnCommand_Next(const char *szCommand);
	void OnCommand_Refresh(const char *szCommand);
	
protected:
	bool				m_bInTimer;	
	PAUIWINDOWPICTURE	m_pWPic;
	bool				m_bHasOuterScroll;
	int					m_iExplorerIdx;
	CExplorer*			m_pExplorer;
	bool				m_bNoDocScroll;

	int					m_iRefreshTime;
};


/*
诛仙知道
*/
class CDlgZhuxianKnows : public CDlgBaseExplorer
{
public:
	CDlgZhuxianKnows() {}
	virtual ~CDlgZhuxianKnows() {}

	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);

private:
	int	m_iTime;
	AString m_strToken;
	static const int ms_TimeCircle;
};


/*
多平台接入
注意：这个类是在login界面用的，所以不能调用dlgbase::GetGameUIMan()
*/
class CDlgLoginOtherPlatform : public CDlgBaseExplorer
{
	AUI_DECLARE_COMMAND_MAP()
		
public:
	CDlgLoginOtherPlatform() {}
	virtual ~CDlgLoginOtherPlatform() {}

	virtual void OnShowDialog();
//	virtual void OnHideDialog();
	
	void OnCommand_Refresh(const char * szCommand);
	void OnCommand_IDCANCEL(const char * szCommand);
	
	virtual void OnTick();
private:
};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////