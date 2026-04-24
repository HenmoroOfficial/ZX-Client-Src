// Filename	: DlgBroadCastInGame.h
// Creator	: Fu Chonggang
// Date		: 2011/3/31

#pragma once

#include "DlgBaseExplorer.h"

class AUICheckBox;

class CDlgBroadCastInGame : public CDlgBaseExplorer  
{
	AUI_DECLARE_EVENT_MAP();
	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgBroadCastInGame();
	virtual ~CDlgBroadCastInGame();

	void OnCommand_Minimize(const char * szCommand);
	void OnCommand_Sound(const char * szCommand);
	void OnCommand_CANCEL(const char * szCommand);
	
protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnHideDialog();
	virtual void OnTick();
	virtual bool Release();

	AUICheckBox*	m_pChkSound;

	bool	m_bDisableSound;
	bool	m_bMinimized;
	int		m_iMusicVol;
	int		m_iSoundVol;
};
