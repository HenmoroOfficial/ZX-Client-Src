/********************************************************************
	created:	2011/6/03
	file name:	CDlgGMCallMultiPlayer.h
	author:		Fu Chonggang
*********************************************************************/

#pragma once

#include "DlgBase.h"
#include "hashmap.h"
#include "vector.h"

class AUIComboBox;
class AUIListBox;

class CDlgGMCallMultiPlayer : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgGMCallMultiPlayer();
	~CDlgGMCallMultiPlayer();

	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	
	void OnCommandChangeRefresh(const char * szCommand);
	void OnCommandChangeGroup(const char * szCommand);
	void OnCommandCallSingle(const char * szCommand);
	void OnCommandCallMulti(const char * szCommand);

	typedef abase::vector< abase::pair<ACString, int> > PlayerInfo;
	typedef abase::hash_map<ACString, PlayerInfo> GroupInfo;

private:
	void UpdateFromFile();
	void UpdateDisplay();

private:
	AUIComboBox* m_pComboGroup;
	AUIListBox*  m_pLstNames;

	GroupInfo	m_pGroupInfo;
	
	static const AString CONFIG_FILENAME;
};