// Filename	: DlgBattleDungeonTowerScore.cpp
// Creator	: Fu Chonggang
// Date		: 2012.4.19

#include "DlgBattleDungeonTowerScore.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_Game.h"
#include "EC_GameSession.h"
#include "EC_BattleDungeonTower.h"
#include "EC_BattleDungeon.h"
#include "elementdataman.h"
#include "ExpTypes.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgBattleDungeonTowerScore, CDlgBase)

AUI_ON_COMMAND("Rdo_Main*",		OnCommandMainOnce)
AUI_ON_COMMAND("Rdo_Once_*",	OnCommandOnce)
AUI_ON_COMMAND("Btn_Prev",		OnCommandPrefOnce)
AUI_ON_COMMAND("Btn_Next",		OnCommandNextOnce)
AUI_ON_COMMAND("Btn_OnceGet",	OnCommandGetOnce)

AUI_ON_COMMAND("Rdo_DailyMain*",OnCommandMainDaily)
AUI_ON_COMMAND("Rdo_Daily_*",	OnCommandDaily)
AUI_ON_COMMAND("Btn_DailyPrev",	OnCommandPrefDaily)
AUI_ON_COMMAND("Btn_DailyNext",	OnCommandNextDaily)
AUI_ON_COMMAND("Btn_DailyGet",	OnCommandGetDaily)

AUI_END_COMMAND_MAP()

const int CDlgBattleDungeonTowerScore::ms_iMaxLevel = 100;

CDlgBattleDungeonTowerScore::CDlgBattleDungeonTowerScore()
{
}

CDlgBattleDungeonTowerScore::~CDlgBattleDungeonTowerScore()
{
}

bool CDlgBattleDungeonTowerScore::OnInitDialog()
{
	m_pBtnPrevOnce = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Prev");
	m_pBtnNextOnce = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Next");
	m_pBtnPrevDaily = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_DailyPrev");
	m_pBtnNextDaily = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_DailyNext");
	CheckRadioButton(1, 1);
	CheckRadioButton(2, 1);
	CheckRadioButton(3, 1);
	CheckRadioButton(4, 1);
	m_iStartLvOnceIndex = 0;
	m_iStartLvDailyIndex = 0;
	m_iStartLvDailyToday = 0;

	return true;
}

void CDlgBattleDungeonTowerScore::OnShowDialog()
{
	UpdateDisplay();
}

void CDlgBattleDungeonTowerScore::OnTick()
{
	static int lastAction = 0;
	if (m_pBtnPrevOnce->GetState()==AUISTILLIMAGEBUTTON_STATE_CLICK)
	{
		lastAction += GetGame()->GetTickTime();
		if (lastAction>150)
		{
			OnCommandPrefOnce("");
			lastAction = 0;
		}
	}
	else if (m_pBtnNextOnce->GetState()==AUISTILLIMAGEBUTTON_STATE_CLICK)
	{
		lastAction += GetGame()->GetTickTime();
		if (lastAction>150)
		{
			OnCommandNextOnce("");
			lastAction = 0;
		}
	}
	else if (m_pBtnPrevDaily->GetState()==AUISTILLIMAGEBUTTON_STATE_CLICK)
	{
		lastAction += GetGame()->GetTickTime();
		if (lastAction>150)
		{
			OnCommandPrefDaily("");
			lastAction = 0;
		}
	}
	else if (m_pBtnNextDaily->GetState()==AUISTILLIMAGEBUTTON_STATE_CLICK)
	{
		lastAction += GetGame()->GetTickTime();
		if (lastAction>150)
		{
			OnCommandNextDaily("");
			lastAction = 0;
		}
	}
	else
		lastAction = 0;
	
	GetDlgItem("Btn_OnceGet")->Enable(m_iLevelOnce>0 && GetHostPlayer()->GetBattleDungeonTower()->AwardCanDel(m_iLevelOnce-1, false));	
	GetDlgItem("Btn_DailyGet")->Enable(m_iLevelDaily>0 && GetHostPlayer()->GetBattleDungeonTower()->AwardCanDel(m_iLevelDaily-1, true));
	GetDlgItem("Btn_OnceGet")->Show(!CECBattleDungeonMan::getSingletonPtr()->IsInTower());
	GetDlgItem("Btn_DailyGet")->Show(!CECBattleDungeonMan::getSingletonPtr()->IsInTower());
	GetDlgItem("Lab_Hint")->Show(CECBattleDungeonMan::getSingletonPtr()->IsInTower());
}

void CDlgBattleDungeonTowerScore::OnCommandMainOnce(const char * szCommand)
{	
	int iLvStart = GetLvBaseOnce();
	PAUIOBJECT pRdoLv;
	AString strName;
	ACString strText;
	int i;
	for (i=0; i<10; ++i)
	{
		strName.Format("Rdo_Once_%02d", i+1);
		pRdoLv = GetDlgItem(strName);
		if (iLvStart+i<1 || iLvStart+i>100)
		{
			strText = _AL("--");
		}
		else
			strText.Format(_AL("%d"), iLvStart+i);
		pRdoLv->SetText(strText);
	}

	OnCommandOnce("");
}

void CDlgBattleDungeonTowerScore::OnCommandOnce(const char * szCommand)
{
	elementdataman* pDB = GetGame()->GetElementDataMan();
	DATA_TYPE DataType = DT_INVALID;
	const TOWER_TRANSCRIPTION_CONFIG* pConfig = GetHostPlayer()->GetBattleDungeonTower()->GetConfig();
	int iLvStart = GetLvBaseOnce();
	int iLv = iLvStart + GetCheckedRadioButton(1) -1;
	m_iLevelOnce	= iLv;
	int i;
	PAUIIMAGEPICTURE pImg;
	AString strName;
	if (pConfig->property_config_id[iLv-1]==0)
	{
		for (i=0; i<5; ++i)
		{
			strName.Format("Img_OnceItem_%02d", i+1);
			pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
			pImg->Show(false);
		}
		return;
	}
	TOWER_TRANSCRIPTION_PROPERTY_CONFIG *pLevelConfig = 
		(TOWER_TRANSCRIPTION_PROPERTY_CONFIG*)pDB->get_data_ptr(pConfig->property_config_id[iLv-1], ID_SPACE_CONFIG, DataType);
	assert(DataType==DT_TOWER_TRANSCRIPTION_PROPERTY_CONFIG);
	for (i=0; i<5; ++i)
	{
		strName.Format("Img_OnceItem_%02d", i+1);
		pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		strName.Format("Lab_OnceItem_%02d", i+1);
		if(pLevelConfig->life_time_award[i].item_id==0)
		{
			pImg->Show(false);
		}
		else
		{
			pImg->Show(true);
			GetGameUIMan()->SetIvtrInfo(pImg, pLevelConfig->life_time_award[i].item_id, pLevelConfig->life_time_award[i].item_count, true);
		}
	}
}

void CDlgBattleDungeonTowerScore::OnCommandMainDaily(const char * szCommand)
{	
	int iLvStart = GetLvBaseDaily();
	PAUIOBJECT pRdoLv;
	AString strName;
	ACString strText;
	int i;
	for (i=0; i<10; ++i)
	{
		strName.Format("Rdo_Daily_%02d", i+1);
		pRdoLv = GetDlgItem(strName);
		if (iLvStart+i<1 || iLvStart+i>100)
		{
			strText = _AL("--");
		}
		else
			strText.Format(_AL("%d"), iLvStart+i);
		pRdoLv->SetText(strText);
	}
	
	OnCommandDaily("");
}

void CDlgBattleDungeonTowerScore::OnCommandDaily(const char * szCommand)
{
	elementdataman* pDB = GetGame()->GetElementDataMan();
	DATA_TYPE DataType = DT_INVALID;
	const TOWER_TRANSCRIPTION_CONFIG* pConfig = GetHostPlayer()->GetBattleDungeonTower()->GetConfig();
	int iLvStart = GetLvBaseDaily();
	int iLv = iLvStart + GetCheckedRadioButton(2) - 1;
	m_iLevelDaily	= iLv;
	int i;
	PAUIIMAGEPICTURE pImg;
	AString strName;
	if (iLv<=0 || iLv>ms_iMaxLevel || pConfig->property_config_id[iLv-1]==0)
	{
		for (i=0; i<5; ++i)
		{
			strName.Format("Img_DailyItem_%02d", i+1);
			pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
			pImg->Show(false);
			GetGameUIMan()->SetIvtrInfo(pImg, NULL);
		}
		return;
	}
	TOWER_TRANSCRIPTION_PROPERTY_CONFIG *pLevelConfig = 
		(TOWER_TRANSCRIPTION_PROPERTY_CONFIG*)pDB->get_data_ptr(pConfig->property_config_id[iLv-1], ID_SPACE_CONFIG, DataType);
	assert(DataType==DT_TOWER_TRANSCRIPTION_PROPERTY_CONFIG);
	for (i=0; i<5; ++i)
	{
		strName.Format("Img_DailyItem_%02d", i+1);
		pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		strName.Format("Lab_DailyItem_%02d", i+1);
		if(pLevelConfig->single_time_award[i].item_id==0)
		{
			pImg->Show(false);
		}
		else
		{
			pImg->Show(true);
			GetGameUIMan()->SetIvtrInfo(pImg, pLevelConfig->single_time_award[i].item_id, pLevelConfig->single_time_award[i].item_count, true);
		}
	}
}

void CDlgBattleDungeonTowerScore::OnCommandGetOnce(const char * szCommand)
{
	GetGame()->GetGameSession()->c2s_CmdTowerReward(GetHostPlayer()->GetBattleDungeonTower()->TOWER_TEMP_ID, m_iLevelOnce-1, 1);
}

void CDlgBattleDungeonTowerScore::OnCommandGetDaily(const char * szCommand)
{
	GetGame()->GetGameSession()->c2s_CmdTowerReward(GetHostPlayer()->GetBattleDungeonTower()->TOWER_TEMP_ID, m_iLevelDaily-1, 0);
}

void CDlgBattleDungeonTowerScore::OnCommandPrefOnce(const char * szCommand)
{
	if (m_iStartLvOnceIndex>0)
	{
		m_iStartLvOnceIndex--;
		UpdateBaseDisplayOnce();
		if (GetCheckedRadioButton(3)<8)
		{
			CheckRadioButton(3, GetCheckedRadioButton(3)+1);
		}
		else
		{
			OnCommandMainOnce("");
		}		
	}
}

void CDlgBattleDungeonTowerScore::OnCommandNextOnce(const char * szCommand)
{
	if (m_iStartLvOnceIndex<2)
	{
		m_iStartLvOnceIndex++;
		UpdateBaseDisplayOnce();
		if (GetCheckedRadioButton(3)>1)
		{
			CheckRadioButton(3, GetCheckedRadioButton(3)-1);
		}
		else
		{
			OnCommandMainOnce("");
		}
	}
}

void CDlgBattleDungeonTowerScore::OnCommandPrefDaily(const char * szCommand)
{
	if (m_iStartLvDailyIndex>0)
	{
		m_iStartLvDailyIndex--;
		UpdateBaseDisplayDaily();
		if (GetCheckedRadioButton(4)<8)
		{
			CheckRadioButton(4, GetCheckedRadioButton(4)+1);
		}
		else
		{
			OnCommandMainDaily("");
		}		
	}
}

void CDlgBattleDungeonTowerScore::OnCommandNextDaily(const char * szCommand)
{
	if (m_iStartLvDailyIndex<2 || (m_iStartLvDailyIndex==2&&m_iStartLvDailyToday!=0))
	{
		m_iStartLvDailyIndex++;
		UpdateBaseDisplayDaily();
		if (GetCheckedRadioButton(4)>1)
		{
			CheckRadioButton(4, GetCheckedRadioButton(4)-1);
		}
		else
		{
			OnCommandMainDaily("");
		}
	}
}

int CDlgBattleDungeonTowerScore::GetLvBaseOnce()
{
	int iRet = (GetCheckedRadioButton(3)-1+m_iStartLvOnceIndex)*10+1;
	if (iRet<=0)
	{
		return 1;
	}
	else
		return iRet;
}

int CDlgBattleDungeonTowerScore::GetLvBaseDaily()
{
	int iRet = (GetCheckedRadioButton(4)-1+m_iStartLvDailyIndex)*10+1+m_iStartLvDailyToday -
		(m_iStartLvDailyToday>0 ? 10 : 0);
	return iRet;
}

void CDlgBattleDungeonTowerScore::UpdateBaseDisplayOnce()
{
	AString strName;
	ACString strText;
	PAUIOBJECT pObj;
	int i;
	for (i=0; i<8; ++i)
	{
		strName.Format("Rdo_Main%d", i);
		pObj = GetDlgItem(strName);
		strText.Format(_AL("%d-%d"), 10*(m_iStartLvOnceIndex+i)+1, 10*(m_iStartLvOnceIndex+i+1));
		pObj->SetText(strText);
	}
}

void CDlgBattleDungeonTowerScore::UpdateBaseDisplayDaily()
{
	AString strName;
	ACString strText;
	PAUIOBJECT pObj;
	int i;
	if (m_iStartLvDailyToday==0)
	{
		for (i=0; i<8; ++i)
		{
			strName.Format("Rdo_DailyMain%d", i);
			pObj = GetDlgItem(strName);
			strText.Format(_AL("%d-%d"), 10*(m_iStartLvDailyIndex+i)+1, 10*(m_iStartLvDailyIndex+i+1));
			pObj->SetText(strText);
		}
	}
	else
	{
		for (i=0; i<8; ++i)
		{
			strName.Format("Rdo_DailyMain%d", i);
			pObj = GetDlgItem(strName);
			strText.Format(_AL("%d-%d"), 10*(m_iStartLvDailyIndex+i-1)+1+m_iStartLvDailyToday, 10*(m_iStartLvDailyIndex+i)+m_iStartLvDailyToday);
			pObj->SetText(strText);
		}
		if (m_iStartLvDailyIndex==0)
		{
			pObj = GetDlgItem("Rdo_DailyMain0");
			strText.Format(_AL("1-%d"), m_iStartLvDailyToday);
			pObj->SetText(strText);
		}
		if (10*(m_iStartLvDailyIndex+7)+m_iStartLvDailyToday>100)
		{
			pObj = GetDlgItem("Rdo_DailyMain7");
			strText.Format(_AL("%d-100"), 10*(m_iStartLvDailyIndex+7-1)+1+m_iStartLvDailyToday);
			pObj->SetText(strText);
		}
	}
}

void CDlgBattleDungeonTowerScore::UpdateDisplay()
{
	CECBattleDungeonTower* pDungeonTower = GetHostPlayer()->GetBattleDungeonTower();
	ACString strText;
	PAUILABEL pLab = (PAUILABEL)GetDlgItem("Txt_Today");
	strText.Format(_AL("%d"), pDungeonTower->GetTodayStart()+1);
	pLab->SetText(strText);
	pLab = (PAUILABEL)GetDlgItem("Txt_Top");
	strText.Format(_AL("%d"), pDungeonTower->GetTodayDest()+1);
	pLab->SetText(strText);
	pLab = (PAUILABEL)GetDlgItem("Txt_LastTop");
	strText.Format(_AL("%d"), pDungeonTower->GetTowerLevel()+1);
	pLab->SetText(strText);

	UpdateBaseDisplayOnce();
	// ÖÕÉú
	m_iStartLvOnceIndex = 0;
	if (pDungeonTower->GetTodayStart()/10>7)
	{
		int i;
		for (i=0; i<pDungeonTower->GetTodayStart()/10-7; ++i)
		{
			OnCommandNextOnce("");
		}
	}
	CheckRadioButton(3, pDungeonTower->GetTodayStart()/10 - m_iStartLvOnceIndex + 1);
	OnCommandMainOnce("");


	// Ã¿ÈÕ
	m_iStartLvDailyIndex = 0;
	m_iStartLvDailyToday = (pDungeonTower->GetTodayDest() - pDungeonTower->GetTodayStart())>=10 ?
		pDungeonTower->GetTodayDest()-9 : pDungeonTower->GetTodayStart();
	if (m_iStartLvDailyToday<0)
	{
		m_iStartLvDailyToday = 0;
	}
	m_iStartLvDailyToday %= 10;
	
	UpdateBaseDisplayDaily();
	int iIndex = pDungeonTower->GetTodayStart()/10 - m_iStartLvDailyIndex + 1 + (m_iStartLvDailyToday>0 ? 1:0);
	if (iIndex>8)
	{
		int i;
		for (i=0; i<iIndex-8; ++i)
		{
			OnCommandNextDaily("");
		}
	}
	CheckRadioButton(4, iIndex-m_iStartLvDailyIndex);
	OnCommandMainDaily("");
}