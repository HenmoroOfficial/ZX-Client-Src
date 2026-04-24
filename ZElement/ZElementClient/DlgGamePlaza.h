// Filename	: DlgGamePlaza.h
// Creator	: Chen Zhixin
// Date		: 2009/03/30

#pragma once

#include "DlgBase.h"

class AUIScroll;
class AUIImagePicture;
class AUILabel;
class AUIStillImageButton;
class AUIListBox;
class AUITextArea;

#define TABLE_PER_PAGE 9
#define PERSONS_PER_TABLE 3
#define TOTAL_TABLES 200

const INT64 ddzlevel[] =
{
	-500,
	-200,
	-100,
	-50,
	0,
	50,
	150,
	300,
	500,
	750,
	1500,
	3000,
	5000,
	8000,
	12000,
	20000,
	30000,
	45000,
	70000
};

struct FG_PLAYERINFO;

class CDlgGamePlaza : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
public:
	CDlgGamePlaza();
	virtual ~CDlgGamePlaza();
	
	void OnCommand_Confirm(const char * szCommand);
	void OnCommand_Cancel(const char * szCommand);
	void OnCommand_Guest(const char * szCommand);
	void OnCommand_Join(const char * szCommand);
	void OnCommand_Minimize(const char * szCommand);
	void OnCommand_ReturnGame(const char * szCommand);
	void OnCommand_Help(const char * szCommand);

	void OnEventLButtonDownTable(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	
	void UpdatePlaza(void);
	void PlayerEnterHall(FG_PLAYERINFO *pData);
	void PlayerLeaveHall(int playerId);
	void UpdateHallMember();
	ACString GetTitle(INT64 iScore);
protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();

	AUIImagePicture		*m_pImg_Icon[TABLE_PER_PAGE * PERSONS_PER_TABLE];
	AUIImagePicture		*m_pImg_IconM[TABLE_PER_PAGE * PERSONS_PER_TABLE];
	AUIImagePicture		*m_pImg_Ready[TABLE_PER_PAGE * PERSONS_PER_TABLE];
	AUIImagePicture		*m_pImg_HeadIcon;
	AUILabel			*m_pLab_PlayerName[TABLE_PER_PAGE * PERSONS_PER_TABLE];
	AUILabel			*m_pLab_Num[TABLE_PER_PAGE];
	AUIImagePicture		*m_pImgTable[TABLE_PER_PAGE];
	AUIScroll			*m_pScl_ViewTable;
	AUIListBox			*m_pLst_Player;
	AUILabel			*m_pTxt_Name;
	AUILabel			*m_pTxt_Title;
	AUILabel			*m_pTxt_WinPercent;
	AUILabel			*m_pTxt_Score;
	AUIStillImageButton *m_pBtn_Join[TABLE_PER_PAGE][PERSONS_PER_TABLE];

	int					m_iCurrentPage;
	int					m_LastSel;
};

