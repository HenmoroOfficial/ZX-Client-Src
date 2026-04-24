/********************************************************************
	created:	2009/03/04
	file base:	DlgSystem4
	file ext:	h
	author:		Fu Chonggang
	
	purpose:	
*********************************************************************/


#pragma once

#include "DlgBase.h"

class CDlgSystem4 : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
public:

	CDlgSystem4();
	virtual ~CDlgSystem4();

	void OnCommandTeam(const char * szCommand);
	void OnCommandFaction(const char * szCommand);
	void OnCommandTeacher(const char * szCommand);
	void OnCommandFriendSeek(const char * szCommand);
	void OnCommandFriend(const char * szCommand);
	void OnCommandGameCasual(const char * szCommand);
	void OnCommandGarden(const char * szCommand);
	void OnCommandKnows(const char * szCommand);
};
