// Filename	: DlgBattleDungeonTowerIngame.cpp
// Creator	: Fu Chonggang
// Date		: 2012.4.19

#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagePicture.h"
#include "DlgBattleDungeonTowerIngame.h"
#include "DlgSkill.h"
#include "DlgCharacterDungeonBook.h"
#include "AFI.h"
#include "EC_GameUIMan.h"
#include "EC_Game.h"
#include "EC_Skill.h"
#include "EC_SendC2SCmds.h"
#include "EC_HostPlayer.h"
#include "EC_GPDataType.h"
#include "EC_BattleDungeonTower.h"
#include "EC_BattleDungeon.h"
#include "EC_GameUIMisc.h"
#include "elementdataman.h"
#include "exptypes.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgBattleDungeonTowerRandom, CDlgBase)

AUI_ON_COMMAND("Btn_Start",		OnCommand_Start)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_IDCANCEL)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgBattleDungeonTowerRandom, CDlgBase)

AUI_ON_EVENT("Img_*",				WM_LBUTTONDOWN,		OnEventLButtonDownItem)
AUI_ON_EVENT("Img_*",				WM_LBUTTONDBLCLK,	OnEventLButtonDownItem)

AUI_END_EVENT_MAP()


const int CDlgBattleDungeonTowerRandom::ms_SelectTime = 20000;
const int CDlgBattleDungeonTowerRandom::ms_DisOrderTime = 500;

bool CDlgBattleDungeonTowerRandom::OnInitDialog()
{
	AString strName;
	int i;
	for (i=0; i<12; ++i)
	{
		strName.Format("Img_%d", i+1);
		m_pImgPoker[i] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
	}
	
	for (i=0; i<4; ++i)
	{
		strName.Format("Img_Done%d", i+1);
		m_pImgSelected[i] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		m_iPickIndex[i] = 0;
		m_iResult[i] = 0;
	}

	m_pLabTime = (PAUILABEL)GetDlgItem("Txt_Time");
	m_pLabTimes = (PAUILABEL)GetDlgItem("Txt_Frequency");

	m_iTime = 0;
	m_iPickCount = 0;
	m_iState = -1;
	m_iCurHintIndex = -1;
	return true;
}

void CDlgBattleDungeonTowerRandom::OnShowDialog()
{
	GetDlgItem("Btn_Start")->Show(true);
	GetDlgItem("IDCANCEL")->Show(false);	
	//GetDlgItem("Gfx_1")->Show(false);
	SetCanEscClose(false);
	m_iTime = ms_SelectTime;
	m_iPickCount = 0;

	m_iState = 0;
	CECBattleDungeonTower*  pDungeonTower = GetHostPlayer()->GetBattleDungeonTower();
	pDungeonTower->SetCurLevel(GetData("level"));		// random_tower_monster中设置
	elementdataman* pDB = GetGame()->GetElementDataMan();
	const TOWER_TRANSCRIPTION_CONFIG* pConfig = GetHostPlayer()->GetBattleDungeonTower()->GetConfig();
	int iLv = GetHostPlayer()->GetBattleDungeonTower()->GetCurLevel();
	DATA_TYPE DataType = DT_INVALID;
	TOWER_TRANSCRIPTION_PROPERTY_CONFIG *pLevelConfig = 
		(TOWER_TRANSCRIPTION_PROPERTY_CONFIG*)pDB->get_data_ptr(pConfig->property_config_id[iLv], ID_SPACE_CONFIG, DataType);
	CDlgCharacterDungeonBook *pDlg = (CDlgCharacterDungeonBook*)GetGameUIMan()->GetDialog("Win_Character_Dungeon_Book");
	int i;
	for (i=0; i<12; ++i)
	{
		pDlg->SetMonsterIcon(m_pImgPoker[i], pLevelConfig->monster_id_list[i]);
	}
	for (i=0; i<4; ++i)
	{
		AUIOBJECT_SETPROPERTY p;
		sprintf(p.fn, "icon\\Dungeon_Tower_Bg.tga");
		m_pImgSelected[i]->SetProperty("Image File", &p);
		m_pImgSelected[i]->SetHint(_AL(""));
		m_iPickIndex[i] = 0;
		m_iResult[i] = 0;
		m_pImgSelected[i]->SetData(-1, "book_id");
	}
	m_iCurHintIndex = -1;
	m_bSend = false;
}

void CDlgBattleDungeonTowerRandom::OnTick()
{
	ACString strText;
	strText.Format(_AL("0:%02d"), m_iTime/1000);
	m_pLabTime->SetText(strText);
	strText.Format(_AL("%d/4"), m_iPickCount);
	m_pLabTimes->SetText(strText);
	if (m_iState==2)
	{
		m_iTime -= GetGame()->GetTickTime();
		if (m_iTime<0)
		{
			RandomOneMonster();
		}
	}
	else if (m_iState==1)
	{
		TickDisOrderCard();
	}

	
	PAUIDIALOG pDlg;
	PAUIOBJECT pObj;
	
	GetGameUIMan()->GetMouseOn(&pDlg, &pObj);
	int i;
	for (i=0; i<4; ++i)
	{
		if (m_pImgSelected[i]==pObj && i!=m_iCurHintIndex)
		{
			CDlgCharacterDungeonBook *pDlgBook = (CDlgCharacterDungeonBook*)GetGameUIMan()->GetDialog("Win_Character_Dungeon_Book");
			int id = m_pImgSelected[i]->GetData("book_id");
			if (id<=0)
				break;

			m_iCurHintIndex = i;
			PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Character_Dungeon_BookHover");
			PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)pDlg->GetDlgItem("Img_Pic");
			PAUITEXTAREA	pTxt  = (PAUITEXTAREA)pDlg->GetDlgItem("Txt_Intro");
			CECBattleDungeonTower *pDungeonTower = GetHostPlayer()->GetBattleDungeonTower();
			if(pDungeonTower->GetMonsterDesc(id))
				pTxt->SetText(pDungeonTower->GetMonsterDesc(id));
			pDlgBook->SetBookIcon(pImg, id);
		}
	}
}

void CDlgBattleDungeonTowerRandom::OnCommand_Start(const char * szCommand)
{
	m_iState = 1;
	m_iDisOrderTime = ms_DisOrderTime;
	m_iTime = ms_SelectTime;
	GetDlgItem("Btn_Start")->Show(false);
	//GetDlgItem("Gfx_1")->Show(true);
	GetDlgItem("IDCANCEL")->Show(true);
}

void CDlgBattleDungeonTowerRandom::OnCommand_IDCANCEL(const char * szCommand)
{
	if(IsCanEscClose())
		OnCommand_Fade("");
}

void CDlgBattleDungeonTowerRandom::TickDisOrderCard()
{
	m_iDisOrderTime -= GetGame()->GetTickTime();
	int i;
	if (m_iDisOrderTime<0)
	{
		m_iState = 2;
		for (i=0; i<12; ++i)
		{
			AUIOBJECT_SETPROPERTY p;
			sprintf(p.fn, "icon\\Dungeon_Tower_Bg.tga");
			m_pImgPoker[i]->SetProperty("Image File", &p);
		}
		for (i=0; i<m_iPickCount; ++i)
		{
			PickOneCard(i);
		}
		return;
	}

	elementdataman* pDB = GetGame()->GetElementDataMan();
	const TOWER_TRANSCRIPTION_CONFIG* pConfig = GetHostPlayer()->GetBattleDungeonTower()->GetConfig();
	int iLv = GetHostPlayer()->GetBattleDungeonTower()->GetCurLevel();
	DATA_TYPE DataType = DT_INVALID;
	TOWER_TRANSCRIPTION_PROPERTY_CONFIG *pLevelConfig = 
		(TOWER_TRANSCRIPTION_PROPERTY_CONFIG*)pDB->get_data_ptr(pConfig->property_config_id[iLv], ID_SPACE_CONFIG, DataType);
	int iTotalMonster = 0;
	for (i=0; i<12; ++i)
	{
		if (pLevelConfig->monster_id_list[i]==0)
		{
			break;
		}
		iTotalMonster++;
	}
	for (i=0; i<12; ++i)
	{
		int index = a_Random(0, iTotalMonster);
		CDlgCharacterDungeonBook *pDlg = (CDlgCharacterDungeonBook*)GetGameUIMan()->GetDialog("Win_Character_Dungeon_Book");
		if (index==iTotalMonster)
		{
			pDlg->SetMonsterIcon(m_pImgPoker[i], 0);
		}
		else
			pDlg->SetMonsterIcon(m_pImgPoker[i], pLevelConfig->monster_id_list[index]);
	}
}

void CDlgBattleDungeonTowerRandom::OnEventLButtonDownItem(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if (strstr(pObj->GetName(), "Img_Done"))
	{
		return;
	}
	if (m_iState!=2)
	{
		return;
	}
	if (m_bSend)
	{
		return;
	}

	int index = atoi(pObj->GetName() + strlen("Img_"));
	for (int i=0; i<m_iPickCount; i++)
	{
		if (m_iPickIndex[i]==index)
		{
			return;
		}
	}

	c2s_SendCmdStartRandomTowerMonster(index);
	m_bSend = true;
}

int CDlgBattleDungeonTowerRandom::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="random_result")
	{
		if (!IsShow())
		{
			Show(true);
		}
		m_bSend = false;
		S2C::random_tower_monster_result *p = (S2C::random_tower_monster_result*)wParam;
		m_iResult[m_iPickCount] = p->monster_id;
		m_iPickIndex[m_iPickCount] = p->click_index==0 ? 1 : p->click_index;
		m_iPickCount++;
		PickOneCard(m_iPickCount-1);
		m_iTime = ms_SelectTime;
		if (m_iPickCount==4)
		{
			m_iState = 3;
			SetCanEscClose(true);
		}
		CECBattleDungeonMan::getSingletonPtr()->SetInTower(true);
	}
	return 0;
}

void CDlgBattleDungeonTowerRandom::RandomOneMonster()
{
	m_iTime = ms_SelectTime;
	int randIndex;
	while(1)
	{
		randIndex = a_Random(1, 12);
		int i;
		for (i=0; i<m_iPickCount; i++)
		{
			if (m_iPickIndex[i]==randIndex)
			{
				break;
			}
		}
		if(i==m_iPickCount)
		{
			OnEventLButtonDownItem(0, 0, m_pImgPoker[randIndex-1]);
			break;
		}
	}
}

void CDlgBattleDungeonTowerRandom::PickOneCard(int index)
{
	CDlgCharacterDungeonBook *pDlg = (CDlgCharacterDungeonBook*)GetGameUIMan()->GetDialog("Win_Character_Dungeon_Book"); 
	pDlg->SetMonsterIcon(m_pImgPoker[m_iPickIndex[index]-1], m_iResult[index]);
	pDlg->SetMonsterIcon(m_pImgSelected[index], m_iResult[index]);
	
	elementdataman* pElementDataMan = GetGame()->GetElementDataMan();
	DATA_TYPE dt;
	MONSTER_ESSENCE *pEss = (MONSTER_ESSENCE*)pElementDataMan->get_data_ptr(
		m_iResult[index], ID_SPACE_ESSENCE,dt); 
	if (dt != DT_MONSTER_ESSENCE)
	{
		m_pImgSelected[index]->SetData(-1, "book_id");
	}
	else
		m_pImgSelected[index]->SetData(pEss->big_icon, "book_id");

	if (m_iResult[index]!=0)
	{
		m_pImgSelected[index]->SetHint(_AL("^DIALOG:Win_Character_Dungeon_BookHover"));
	}
	else
	{
		m_pImgSelected[index]->SetHint(_AL(""));
	}
}


//////////////////////////////////////////////////////////////////////////
// CDlgBattleDungeonTowerRandom		战场基本信息：倒计时、当前关卡
//////////////////////////////////////////////////////////////////////////

int CDlgBattleDungeonTowerStage::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

bool CDlgBattleDungeonTowerStage::OnInitDialog()
{
	m_pLabTime = (PAUILABEL)GetDlgItem("Txt_TimeLeft");
	m_pLabLevel = (PAUILABEL)GetDlgItem("Txt_Now");
	m_pLabLevelCur = (PAUILABEL)GetDlgItem("Txt_Today");

	return true;
}

void CDlgBattleDungeonTowerStage::OnShowDialog()
{
}

void CDlgBattleDungeonTowerStage::OnTick()
{
	ACString strText;
	strText = GetTimeText(CECBattleDungeonMan::getSingletonPtr()->m_iEndTime-GetGame()->GetServerGMTTime());
	m_pLabTime->SetText(strText);

	strText.Format(_AL("%d"), GetHostPlayer()->GetBattleDungeonTower()->GetCurLevel()+1);
	m_pLabLevel->SetText(strText);

	strText.Format(_AL("%d"), GetHostPlayer()->GetBattleDungeonTower()->GetTodayStart()+1);
	m_pLabLevelCur->SetText(strText);

	AlignTo(GetGameUIMan()->GetDialog("Win_CharHead"),
		AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT,
		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
}


AUI_BEGIN_COMMAND_MAP(CDlgBattleDungeonTowerLuckySkill, CDlgBase)

AUI_ON_COMMAND("Btn_Max",		OnCommandOpen)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgBattleDungeonTowerLuckySkill, CDlgBase)

AUI_ON_EVENT("Item_*",			WM_LBUTTONDOWN,		OnEventLButtonDown)

AUI_END_EVENT_MAP()

CDlgBattleDungeonTowerLuckySkill *CDlgBattleDungeonTowerLuckySkill::m_pDlgLuckySkill = NULL;
CDlgBattleDungeonTowerLuckySkill *CDlgBattleDungeonTowerLuckySkill::m_pDlgLuckySkillMin = NULL;
abase::vector<CDlgBattleDungeonTowerLuckySkill::SkillState> CDlgBattleDungeonTowerLuckySkill::m_vecSkill;
int CDlgBattleDungeonTowerLuckySkill::m_iUsedCount = 0;

CDlgBattleDungeonTowerLuckySkill::CDlgBattleDungeonTowerLuckySkill()
{
}

CDlgBattleDungeonTowerLuckySkill::~CDlgBattleDungeonTowerLuckySkill()
{
}

bool CDlgBattleDungeonTowerLuckySkill::OnInitDialog()
{
	if (m_szName=="Win_Battle_Dungeon_Tower_LuckySkill")
	{
		m_pDlgLuckySkill = this;
	}
	else if (m_szName=="Win_Battle_Dungeon_Tower_Lucky")
	{
		m_pDlgLuckySkillMin = this;
	}
	return true;
}

void CDlgBattleDungeonTowerLuckySkill::OnShowDialog()
{
	
}

void CDlgBattleDungeonTowerLuckySkill::OnTick()
{

}

int CDlgBattleDungeonTowerLuckySkill::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="PLAYER_TOWER_GIVE_SKILLS")
	{
		S2C::player_tower_give_skills *p = (S2C::player_tower_give_skills*)wParam;
		int i;
		for (i=0; i<(int)m_vecSkill.size(); ++i)
		{
			delete m_vecSkill[i].pSkill;
		}
		m_vecSkill.clear();
		m_iUsedCount = 0;
		for (i=0; i<p->skill_count; ++i)
		{
			SkillState pState;
			pState.skill_id = p->skills[i].skill_id;
			pState.skill_level = p->skills[i].skill_level;
			pState.used = p->skills[i].used;
			pState.pSkill = new CECSkill(pState.skill_id, pState.skill_level);
			m_vecSkill.push_back(pState);
			if (p->skills[i].used)
			{
				m_iUsedCount++;
			}
		}
	}
	else if (strMsg=="SET_TOWER_GIVE_SKILL")
	{
		S2C::set_tower_give_skill *p = (S2C::set_tower_give_skill*)wParam;
		int i;
		for (i=0; i<(int)m_vecSkill.size(); ++i)
		{
			if (m_vecSkill[i].skill_id==p->skill_id)
			{
				m_vecSkill[i].used = p->used;
				m_iUsedCount++;
				break;
			}
		}
	}

	// 最小化按钮显示更新
	ACString strText;
	strText.Format(_AL("%d/3"), m_iUsedCount);
	m_pDlgLuckySkillMin->GetDlgItem("Lab_Frequency")->SetText(strText);
	if (!m_pDlgLuckySkillMin->IsShow())
	{
		m_pDlgLuckySkillMin->Show(true);
	}

	// 技能显示更新
	int i;
	AString strName;
	for (i=0; i<10; ++i)
	{
		strName.Format("Item_%d", i+1);
		PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)m_pDlgLuckySkill->GetDlgItem(strName);
		CDlgSkill *pDlgSkill = (CDlgSkill*)GetGameUIMan()->GetDialog("Win_Skill");
		if (i<(int)m_vecSkill.size())
		{
			pDlgSkill->SetImage(pImg, m_vecSkill[i].pSkill);
			
			if(m_vecSkill[i].used)
			{
				pImg->SetColor(A3DCOLORRGB(128, 128, 128));
				pImg->SetDataPtr(NULL, "ptr_CECSkill");
			}
			else
			{
				pImg->SetColor(A3DCOLORRGB(255, 255, 255));
				pImg->SetDataPtr(m_vecSkill[i].pSkill, "ptr_CECSkill");
			}
		}
		else
		{
			pDlgSkill->SetImage(pImg, NULL);
			pImg->SetColor(A3DCOLORRGB(255, 255, 255));
			pImg->SetDataPtr(NULL, "ptr_CECSkill");
		}
	}

	return 0;
}

void CDlgBattleDungeonTowerLuckySkill::OnCommandOpen(const char * szCommand)
{
	m_pDlgLuckySkill->Show(!m_pDlgLuckySkill->IsShow());
	m_pDlgLuckySkill->AlignTo(m_pDlgLuckySkillMin,
		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_LEFT,
		AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_BOTTOM);
}

void CDlgBattleDungeonTowerLuckySkill::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if (pObj->GetType()!=AUIOBJECT_TYPE_IMAGEPICTURE)
		return;

	if(pObj->GetDataPtr("ptr_CECSkill"))
	{
		CECSkill *pSkill = (CECSkill *)pObj->GetDataPtr("ptr_CECSkill");
		if( pSkill )
			GetHostPlayer()->ApplySkillShortcut(pSkill->GetSkillID(), false, 0, -1, pSkill);
	}
}

