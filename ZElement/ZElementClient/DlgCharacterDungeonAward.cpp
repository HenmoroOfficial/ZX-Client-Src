// Filename	: DlgCharacterDungeonAward.cpp
// Creator	: Fu Chonggang
// Date		: 2012.4.19

#include "DlgCharacterDungeonAward.h"
#include "AFI.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUIStillImageButton.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_BattleDungeonTower.h"
#include "EC_IvtrItem.h"
#include "EC_Game.h"
#include "EC_GameSession.h"
#include "ExpTypes.h"
#include "elementdataman.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgCharacterDungeonAward, CDlgBase)

AUI_ON_COMMAND("last",			OnCommandLast)
AUI_ON_COMMAND("next",			OnCommandNext)
AUI_ON_COMMAND("head",			OnCommandHead)
AUI_ON_COMMAND("tail",			OnCommandTail)
AUI_ON_COMMAND("Btn_Get",		OnCommandAward)
AUI_ON_COMMAND("Rdo_*",			OnCommandAwardType)

AUI_END_COMMAND_MAP()

const int CDlgCharacterDungeonAward::ms_iNumPerPage = 10;
const int CDlgCharacterDungeonAward::ms_iNumTotal = 100;

CDlgCharacterDungeonAward::CDlgCharacterDungeonAward()
{
	m_iCurPage = 0;
}

CDlgCharacterDungeonAward::~CDlgCharacterDungeonAward()
{
}

bool CDlgCharacterDungeonAward::OnInitDialog()
{
	CheckRadioButton(1, 1);
	m_pLstAward = (PAUILISTBOX)GetDlgItem("Lst_All");
	m_pBtnGet = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Get");
	return true;
}

void CDlgCharacterDungeonAward::OnShowDialog()
{
	UpdateAward();
}

void CDlgCharacterDungeonAward::OnTick()
{
	int iLv = m_iCurPage*ms_iNumPerPage + m_pLstAward->GetCurSel();
	m_pBtnGet->Enable(m_pLstAward->GetCurSel()>=0 && 
		GetHostPlayer()->GetBattleDungeonTower()->AwardCanDel(iLv, GetCheckedRadioButton(1)==2));
}

void CDlgCharacterDungeonAward::OnCommandLast(const char * szCommand)
{
	if (m_iCurPage==0)
		return;
	
	m_iCurPage--;
	UpdateAward();
}

void CDlgCharacterDungeonAward::OnCommandNext(const char * szCommand)
{
	if (m_iCurPage>=ms_iNumTotal/ms_iNumPerPage)
		return;
	
	m_iCurPage++;
	UpdateAward();
}

void CDlgCharacterDungeonAward::OnCommandHead(const char * szCommand)
{
	if (m_iCurPage==0)
		return;
	
	m_iCurPage = 0;
	UpdateAward();
}

void CDlgCharacterDungeonAward::OnCommandTail(const char * szCommand)
{
	if (m_iCurPage==ms_iNumTotal/ms_iNumPerPage)
		return;
	
	m_iCurPage = ms_iNumTotal/ms_iNumPerPage;
	UpdateAward();
}

void CDlgCharacterDungeonAward::OnCommandAward(const char * szCommand)
{
	GetGame()->GetGameSession()->c2s_CmdTowerReward(GetHostPlayer()->GetBattleDungeonTower()->TOWER_TEMP_ID, 
		m_iCurPage*ms_iNumPerPage+m_pLstAward->GetCurSel(), GetCheckedRadioButton(1)==1?1:0);
}

void CDlgCharacterDungeonAward::OnCommandAwardType(const char * szCommand)
{
	UpdateAward();
}

void CDlgCharacterDungeonAward::UpdateAward()
{
	const TOWER_TRANSCRIPTION_CONFIG* pConfig = GetHostPlayer()->GetBattleDungeonTower()->GetConfig();

	int iStart = m_iCurPage*ms_iNumPerPage;
	int iEnd = (m_iCurPage+1)*ms_iNumPerPage-1;
	if (iEnd>99)
		iEnd = 99;

	m_pLstAward->ResetContent();
	elementdataman* pDB = GetGame()->GetElementDataMan();
	DATA_TYPE DataType = DT_INVALID;
	int i;
	ACString strLevel, strAward;
	for (i=iStart; i<=iEnd; ++i)
	{
		if (pConfig->property_config_id[i]==0)
		{
			break;
		}
		strAward.Empty();
		TOWER_TRANSCRIPTION_PROPERTY_CONFIG *pLevelConfig = 
			(TOWER_TRANSCRIPTION_PROPERTY_CONFIG*)pDB->get_data_ptr(pConfig->property_config_id[i], ID_SPACE_CONFIG, DataType);
		assert(DataType==DT_TOWER_TRANSCRIPTION_PROPERTY_CONFIG);
		int j;
		if (GetCheckedRadioButton(1)==1)
		{
			for (j=0; j<5; ++j)
			{
				if(pLevelConfig->life_time_award[j].item_id==0)
					continue;
				CECIvtrItem *pItem = CECIvtrItem::CreateItem(pLevelConfig->life_time_award[j].item_id, 0, 1);
				if (pItem)
				{
					ACString strNum;
					strNum.Format(_AL("x%d "), pLevelConfig->life_time_award[j].item_count);
					strAward += pItem->GetName();
					strAward += strNum;
					delete pItem;
				}
			}
		}
		else
		{
			for (j=0; j<5; ++j)
			{
				if(pLevelConfig->single_time_award[j].item_id==0)
					continue;
				CECIvtrItem *pItem = CECIvtrItem::CreateItem(pLevelConfig->single_time_award[j].item_id, 0, 1);
				if (pItem)
				{
					ACString strNum;
					strNum.Format(_AL("x%d "), pLevelConfig->single_time_award[j].item_count);
					strAward += pItem->GetName();
					strAward += strNum;
					delete pItem;
				}
			}
		}
		strLevel.Format(GetGameUIMan()->GetStringFromTable(13403), i+1);
		m_pLstAward->AddString(strLevel + _AL("\t") + strAward);
	}
}