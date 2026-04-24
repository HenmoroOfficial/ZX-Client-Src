// File		: DlgRankList.h
// Creator	: Xiao Zhou
// Date		: 2006/8/14

#pragma once

#include "hashmap.h"
#include "DlgBase.h"
#include "LuaRankDisp.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIRadioButton.h"
#include "AUI\\AUILabel.h"

#define CDLGRANKLIST_MAINTYPE_MAX 4
#define CDLGRANKLIST_SUBTYPE_MAX 17

typedef	abase::hash_map<int, ACString>	PageDesc;

class CDlgRankList : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()

public:
	struct TableInfo
	{
		ACString	strTableName;
		ACString	strSelf;
		ACString	strRankDesc1;
		ACString	strRankDesc2;
		int			nSelfRank;
		int			nPageSize;
		int			nMaxPage;
	};

	CDlgRankList();
	virtual ~CDlgRankList();

	void RankListAction(void *pData);

	void OnCommandButtonA(const char * szCommand);
	void OnCommandButtonB(const char * szCommand);
	void OnCommandLast(const char * szCommand);
	void OnCommandNext(const char * szCommand);
	void OnCommandHelp(const char * szCommand);
	void OnCommandHead(const char * szCommand);
	void OnCommandTail(const char * szCommand);
	void OnCommandCancel(const char * szCommand);
	void OnCommandSelf(const char * szCommand);
	ACString GetCorresText(int nTable, int nPage);
	void ShowTowerRank();

protected:
	bool OnInitDialog();
	void OnShowDialog();
	void _LoadConfigInfo();
	
	void AddStringToList(const ACHAR* szText);

	void Update(int nTable, int nPage);

	PAUILISTBOX				m_pLst_All;
	PAUILABEL				m_pLab_Self;
	PAUILABEL				m_pTxt_Self;
	PAUILABEL				m_pTxt_Page;
	//PAUISTILLIMAGEBUTTON	m_pBtn_A[CDLGRANKLIST_MAINTYPE_MAX];
	PAUIRADIOBUTTON         m_pRdo_A[CDLGRANKLIST_MAINTYPE_MAX];
	PAUISTILLIMAGEBUTTON	m_pBtn_B[CDLGRANKLIST_SUBTYPE_MAX];
	PAUISTILLIMAGEBUTTON	m_pBtn_Desc1;
	PAUISTILLIMAGEBUTTON	m_pBtn_Desc2;
	PAUISTILLIMAGEBUTTON	m_pBtn_Self;
	abase::hash_map<int, PageDesc*> m_mapTableContent;
	abase::hash_map<int, TableInfo> m_mapTableInfo;
	CLuaRankDisp			m_RankDisp;

	int	m_idTable[CDLGRANKLIST_MAINTYPE_MAX][CDLGRANKLIST_SUBTYPE_MAX];
	int m_nPage;
	int	m_idCurTable;
};
