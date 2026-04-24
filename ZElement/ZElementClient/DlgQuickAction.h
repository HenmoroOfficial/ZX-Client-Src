// Filename	: DlgQuickAction.h
// Creator	: Tom Zhou
// Date		: October 10, 2005

#pragma once

#include "DlgBase.h"
#include "AUI\\AUIStillImageButton.h"

class CDlgQuickAction : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgQuickAction();
	virtual ~CDlgQuickAction();

	void OnCommand_SecretMsg(const char * szCommand);
	void OnCommand_Report(const char * szCommand);
	void OnCommand_Trade(const char * szCommand);
	void OnCommand_Whisper(const char * szCommand);
	void OnCommand_AddFriend(const char * szCommand);
	void OnCommand_InviteGroup(const char * szCommand);
	void OnCommand_InviteGuild(const char * szCommand);
	void OnCommand_InviteFamily(const char * szCommand);
	void OnCommand_Follow(const char * szCommand);
	void OnCommand_JoinChannel(const char * szCommand);
	void OnCommand_InviteChannel(const char * szCommand);
	void OnCommand_Bind(const char * szCommand);
	void OnCommand_Duel(const char * szCommand);
	void OnCommand_EPEquip(const char * szCommand);
	void OnCommand_CopyName(const char * szCommand);
	void OnCommand_Teacher(const char * szCommand);
	void OnCommand_Shield(const char * szCommand);
	void OnCommand_RideTogether(const char * szCommand);
	void OnCommand_Achievement(const char * szCommand);
	void OnCommand_CANCEL(const char * szCommand);
	void OnCommand_Rose(const char * szCommand);
	void OnCommand_QilinInvite(const char * szCommand);
protected:
	PAUISTILLIMAGEBUTTON m_pInviteGuild;
	PAUISTILLIMAGEBUTTON m_pInviteFamily;
	PAUISTILLIMAGEBUTTON m_pInviteChannel;
	PAUISTILLIMAGEBUTTON m_pInviteGroup;
	PAUISTILLIMAGEBUTTON m_pTrade;
	PAUISTILLIMAGEBUTTON m_pTeacher;
	PAUISTILLIMAGEBUTTON m_pDuel;
	PAUISTILLIMAGEBUTTON m_pFollow;
	PAUISTILLIMAGEBUTTON m_pBind;
	PAUISTILLIMAGEBUTTON m_pQilinBind;
	PAUISTILLIMAGEBUTTON m_pRideTogether;
	PAUISTILLIMAGEBUTTON m_pRose;
	virtual void OnTick();
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
};
