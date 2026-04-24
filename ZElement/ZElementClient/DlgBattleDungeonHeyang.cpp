// Filename	: DlgBattleDungeonHeyang.h
// Creator	: Fu Chonggang
// Date		: 2012.4.19

#include "DlgBattleDungeonHeyang.h"
#include "AWScriptFile.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUICheckBox.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUITextArea.h"
#include "EC_GameUIMan.h"
#include "EC_Game.h"
#include "EC_Configs.h"
#include "EC_GPDataType.h"
#include "EC_GameUIMisc.h"
#include "EC_BattleDungeon.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgBattleDungeonHeyang, CDlgBase)

AUI_ON_COMMAND("Chk_Pop",		OnCommandSet)
AUI_ON_COMMAND("Btn_Quit",		OnCommandQuit)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgBattleDungeonHeyang, CDlgBase)

AUI_ON_EVENT("Lst_Type",		WM_LBUTTONDOWN,		OnEventLButtonDownStage)
AUI_ON_EVENT("Lst_Type",		WM_LBUTTONDBLCLK,	OnEventLButtonDownStage)

AUI_END_EVENT_MAP()

CDlgBattleDungeonHeyang::CDlgBattleDungeonHeyang()
{
}

CDlgBattleDungeonHeyang::~CDlgBattleDungeonHeyang()
{
}

bool CDlgBattleDungeonHeyang::OnInitDialog()
{
	m_vecConfig.clear();

	AWScriptFile s;
	if( !s.Open("Configs\\dreamheyang.txt") )
		AUI_ReportError(__LINE__, __FILE__);	
	while( !s.IsEnd() )
	{
		if( !s.GetNextToken(true) )
			break;
		
		HeYangConfig hyConfig;
		
		BEGIN_FAKE_WHILE2
		hyConfig.iTaskStartId1 = a_atoi(s.m_szToken);
				
		CHECK_BREAK2(s.GetNextToken(true));
		hyConfig.iTaskStartId2 = a_atoi(s.m_szToken);
		
		CHECK_BREAK2(s.GetNextToken(true));
		hyConfig.iTaskEndId = a_atoi(s.m_szToken);
		
		CHECK_BREAK2(s.GetNextToken(true));
		hyConfig.id = a_atoi(s.m_szToken);

		CHECK_BREAK2(s.GetNextToken(true));
		hyConfig.strNameStage = s.m_szToken;
		CHECK_BREAK2(s.GetNextToken(true));
		hyConfig.strNameMonster = s.m_szToken;
		
		CHECK_BREAK2(s.GetNextToken(true));
		hyConfig.iMonsterNum = a_atoi(s.m_szToken);
		CHECK_BREAK2(s.GetNextToken(true));
		hyConfig.strIcon = AC2AS(s.m_szToken);
		CHECK_BREAK2(s.GetNextToken(true));
		hyConfig.strDesc = s.m_szToken;
		m_vecConfig.push_back(hyConfig);		
		
		END_FAKE_WHILE2;
	}
	s.Close();
	
	m_pLabTimeLeft = (PAUILABEL)GetDlgItem("Txt_Time");
	m_pLstStage = (PAUILISTBOX)GetDlgItem("Lst_Type");
	m_pLstStage->ResetContent();
	int i;
	for (i=0; i<(int)m_vecConfig.size(); ++i)
	{
		m_pLstStage->AddString(m_vecConfig[i].strNameStage);
	}
	OnEventLButtonDownStage(0, 0, m_pLstStage);
	m_iIndex	= -1;
	return true;
}

void CDlgBattleDungeonHeyang::OnShowDialog()
{
	PAUICHECKBOX pChk = (PAUICHECKBOX)GetDlgItem("Chk_Pop");
	EC_LOCAL_SETTING setting = GetGame()->GetConfigs()->GetLocalSettings();
	pChk->Check(setting.bStopPopDreamHeyangHint);
}

void CDlgBattleDungeonHeyang::OnTick()
{
	ACString strTime = GetTimeText(CECBattleDungeonMan::getSingletonPtr()->m_iEndTime-GetGame()->GetServerGMTTime());
	m_pLabTimeLeft->SetText(strTime);
}

void CDlgBattleDungeonHeyang::OnCommandSet(const char * szCommand)
{
	PAUICHECKBOX pChk = (PAUICHECKBOX)GetDlgItem("Chk_Pop");
	EC_LOCAL_SETTING setting = GetGame()->GetConfigs()->GetLocalSettings();
	setting.bStopPopDreamHeyangHint = pChk->IsChecked();
	GetGame()->GetConfigs()->SetLocalSettings(setting);
}

void CDlgBattleDungeonHeyang::OnCommandQuit(const char * szCommand)
{
	GetGameUIMan()->MessageBox("Game_DungeonLeave", GetGameUIMan()->GetStringFromTable(12263), MB_YESNO);
}

void CDlgBattleDungeonHeyang::OnEventLButtonDownStage(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	int iIndex = m_pLstStage->GetCurSel();
	if (iIndex<0 || iIndex>=m_pLstStage->GetCount())
	{
		return;
	}
	
	PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)GetDlgItem("Img_Type");;
	AUIOBJECT_SETPROPERTY p;
	sprintf(p.fn, m_vecConfig[iIndex].strIcon);
	pImg->SetProperty("Image File", &p);
	PAUITEXTAREA pTxt = (PAUITEXTAREA)GetDlgItem("Txt_Intro");
	pTxt->SetText(m_vecConfig[iIndex].strDesc);
}

int CDlgBattleDungeonHeyang::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="show")
	{
		int i;
		for (i=0; i<(int)m_vecConfig.size(); ++i)
		{
			if ((int)wParam==m_vecConfig[i].iTaskStartId1 ||(int)wParam==m_vecConfig[i].iTaskStartId2)
			{
				m_iIndex = i;
				break;
			}
		}
		if (i==(int)m_vecConfig.size())
		{
			return 0;
		}
		
		m_pLstStage->SetCurSel(m_iIndex);

		// 选择当前关卡
		OnEventLButtonDownStage(0, 0, m_pLstStage);

		// 显示比分界面
		PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Battle_Dungeon_Heyang_Score");
		if (!pDlg->IsShow())
		{
			// 第一次弹出界面
			pDlg->Show(true);
		}
		PAUILABEL pLabName = (PAUILABEL)pDlg->GetDlgItem("Lab_Name");
		pLabName->SetText(m_vecConfig[m_iIndex].strNameMonster);
		PAUILABEL pLabNum = (PAUILABEL)pDlg->GetDlgItem("Txt_Num");
		ACString strText;
		strText.Format(_AL("0/%d"), m_vecConfig[m_iIndex].iMonsterNum);
		pLabNum->SetText(strText);

		// 根据需要弹出提示
		EC_LOCAL_SETTING setting = GetGame()->GetConfigs()->GetLocalSettings();
		if (!setting.bStopPopDreamHeyangHint)
		{
			PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Battle_Dungeon_Heyang_Intro");
			pDlg->SetData(m_vecConfig[m_iIndex].id, "id");
			pDlg->Show(true);
		}
	}
	else if (strMsg=="hide")
	{
		int i;
		for (i=0; i<(int)m_vecConfig.size(); ++i)
		{
			if ((int)wParam==m_vecConfig[i].iTaskEndId)
			{
				GetGameUIMan()->GetDialog("Win_Battle_Dungeon_Heyang_Score")->Show(false);
				return 0;
			}
		}
	}
	else if(strMsg=="RAID_GLOBAL_VARIABLE")
	{
		if (m_iIndex<0)
		{
			return 0;
		}
		S2C::raid_global_variable *p = (S2C::raid_global_variable*)wParam;
		PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Battle_Dungeon_Heyang_Score");
		PAUILABEL pLabName = (PAUILABEL)pDlg->GetDlgItem("Lab_Name");
		pLabName->SetText(m_vecConfig[m_iIndex].strNameMonster);
		PAUILABEL pLabNum = (PAUILABEL)pDlg->GetDlgItem("Txt_Num");
		int i;
		ACString strText;
		for (i=0; i<p->count; ++i)
		{
			if (p->var_list[i].key==m_vecConfig[m_iIndex].id)
			{
				strText.Format(_AL("%d/%d"), p->var_list[i].value, m_vecConfig[m_iIndex].iMonsterNum);
			}
		}
		pLabNum->SetText(strText);
	}

	return 0;
}

void CDlgBattleDungeonHeyang::GetInfo(int id, AString &strIcon, ACString &strDesc)
{
	strIcon.Empty();
	strDesc.Empty();
	int i;
	for (i=0; i<(int)m_vecConfig.size(); ++i)
	{
		if (id==m_vecConfig[i].id)
		{
			strIcon = m_vecConfig[i].strIcon;
			strDesc = m_vecConfig[i].strDesc;
			return;
		}
	}
}



AUI_BEGIN_COMMAND_MAP(CDlgBattleDungeonHeyangIntro, CDlgBase)

AUI_ON_COMMAND("Chk_Close",		OnCommandSet)

AUI_END_COMMAND_MAP()

void CDlgBattleDungeonHeyangIntro::OnShowDialog()
{
	int id = GetData("id");
	AString strIcon;
	ACString strDesc;

	CDlgBattleDungeonHeyang* pDlgHeyang = (CDlgBattleDungeonHeyang*)GetGameUIMan()->GetDialog("Win_Battle_Dungeon_Heyang");
	pDlgHeyang->GetInfo(id, strIcon, strDesc);
	PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)GetDlgItem("Img_Type");;
	AUIOBJECT_SETPROPERTY p;
	sprintf(p.fn, strIcon);
	pImg->SetProperty("Image File", &p);
	PAUITEXTAREA pTxt = (PAUITEXTAREA)GetDlgItem("Txt_Intro");
	pTxt->SetText(strDesc);
	
	PAUICHECKBOX pChk = (PAUICHECKBOX)GetDlgItem("Chk_Close");
	EC_LOCAL_SETTING setting = GetGame()->GetConfigs()->GetLocalSettings();
	pChk->Check(setting.bStopPopDreamHeyangHint);
}

void CDlgBattleDungeonHeyangIntro::OnCommandSet(const char * szCommand)
{
	PAUICHECKBOX pChk = (PAUICHECKBOX)GetDlgItem("Chk_Close");
	EC_LOCAL_SETTING setting = GetGame()->GetConfigs()->GetLocalSettings();
	setting.bStopPopDreamHeyangHint = pChk->IsChecked();
	GetGame()->GetConfigs()->SetLocalSettings(setting);
	
}