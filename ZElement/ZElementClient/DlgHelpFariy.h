// Filename	: DlgGameFariy.h
// Creator	: Fu Chonggang
// Date		: 2009/08/19

#pragma once

#include "DlgBase.h"
#include "vector.h"

class AUIStillImageButton;
class AUILabel;
class AUIImagePicture;

class CDlgHelpFariy : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

	enum
	{
		ACTIONSTATE_MIN,
		ACTIONSTATE_SHOW	= ACTIONSTATE_MIN,
		ACTIONSTATE_FALLOW,
		ACTIONSTATE_HIDE,
	};
	
public:
	CDlgHelpFariy();
	virtual ~CDlgHelpFariy();
	void OnCommand_Help(const char * szCommand);
	void OnCommand_Search(const char * szCommand);
	void OnCommand_Memo(const char * szCommand);
	void OnCommand_Set(const char * szCommand);
	void OnCommand_Change(const char * szCommand);
	void OnCommand_Hide(const char * szCommand);
	void OnCommand_Close(const char * szCommand);
	void OnCommand_IDCANCEL(const char * szCommand);

	void OnEventLButtonDownAssist(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonUpAssist(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonDblClkDownAssist(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	
protected:
	virtual void OnTick();
	virtual bool OnInitDialog();
//	virtual bool Release();
	virtual void OnShowDialog();

	AUIStillImageButton *m_pBtn_Help;
	AUIStillImageButton *m_pBtn_Search;
	AUIStillImageButton *m_pBtn_Memo;
	AUIStillImageButton *m_pBtn_Set;
	AUIStillImageButton *m_pBtn_Change;
	AUIStillImageButton *m_pBtn_Hide;
	AUIStillImageButton *m_pBtn_Close;

	AUILabel			*m_pLab_Hint;
	AUIImagePicture		*m_pImgAssistant;

	int m_iState;
	int m_iNumFrames;
	int m_iCurFrame;
	int m_iX;
	int m_iY;
	
	static abase::vector<ACString>	m_HelpMsgs;		//	Fixed message table
	static UINT		m_iCurIndex;
	static DWORD	m_dwTimeRemind;
	static DWORD	m_dwTimeClose;
	static bool		m_bRemind;

};
