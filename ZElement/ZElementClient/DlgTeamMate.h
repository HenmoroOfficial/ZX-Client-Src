// File		: DlgTeamMate.h
// Creator	: Xiao Zhou
// Date		: 2005/8/17

#pragma once

#include "DlgBase.h"

#define CDLGTEAMMATE_ST_MAX			3

class AUIImagePicture;
class AUILabel;
class AUIProgress;

class CDlgTeamMate : public CDlgBase  
{
	friend class CDlgTeamMain;

	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
public:
	void OnCommandCANCAL(const char *szCommand);

	void OnEventLButtonUp(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventLButtonClick(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventRButtonUp(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	
	CDlgTeamMate();
	virtual ~CDlgTeamMate();

protected:
	virtual void DoDataExchange(bool bSave);
	
	AUIImagePicture *	m_pImgProf;
	AUIImagePicture *	m_pImgCombatMask;
	AUIImagePicture *	m_pImgLeader;
	AUILabel *			m_pTxtCharName;
	AUILabel *			m_pTxtLV;
	AUIProgress *		m_pPrgsHP;
	AUIProgress *		m_pPrgsMP;
	AUIProgress *		m_pPrgsDP;
	AUIImagePicture *	m_pImgSt[CDLGTEAMMATE_ST_MAX];
	
	int					m_nDeityLv;
};
