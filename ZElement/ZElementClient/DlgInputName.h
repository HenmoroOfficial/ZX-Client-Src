// Filename	: DlgInputName.h
// Creator	: Tom Zhou
// Date		: October 13, 2005

#pragma once

#include "DlgBase.h"

class AUIEditBox;

class CDlgInputName : public CDlgBase  
{
	friend class CDlgGuildMan;
	friend class CDlgMidMap;
	friend class CDlgMiniMap;
	friend class CDlgWorldMap;
	friend class CDlgFriendList;
	friend class CDlgFriendOptionGroup;
	friend class CDlgFriendOptionNormal;
	friend class CDlgChannelChat;
	friend class CDlgReplayEdit;
	friend class CDlgTeacherMan;
	friend class DlgCrssvrTeamsGroupInfo;
	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgInputName();
	virtual ~CDlgInputName();

	void OnCommand_confirm(const char * szCommand);
	void OnCommand_CANCEL(const char * szCommand);

	enum
	{
		INPUTNAME_NULL = 0,
		INPUTNAME_MAKE_MARK,
		INPUTNAME_RENAME_MARK,
		INPUTNAME_ADD_FRIEND,
		INPUTNAME_ADD_GROUP,
		INPUTNAME_ADD_GUILD,
		INPUTNAME_ADD_FAMILY,
		INPUTNAME_ADD_SECT,
		INPUTNAME_RENAME_GROUP,
		INPUTNAME_FACTION_TITLE,
		INPUTNAME_CHANNEL_INVITE,
		INPUTNAME_ADD_BLACKLIST,
		INPUTNAME_EDIT_RECORD,
		INPUTNAME_CRSSVRTEAMES,
		INPUTNAME_ADD_FRIEND_REMARK,
	};
	
protected:
	virtual void DoDataExchange(bool bSave);
	
	AUIEditBox *	m_pTxtInput;
};
