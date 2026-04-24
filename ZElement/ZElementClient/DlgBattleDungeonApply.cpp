
#include "A2DSprite.h"
#include "AUI\\CSplit.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUITextArea.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "DlgBattleDungeonApply.h"
#include "EC_Game.h"
#include "EC_GameUIMan.h"
#include "EC_GameUIMisc.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_BattleDungeon.h"
#include "raidgetlist_re.hpp"
#include "ExpTypes.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgBattleDungeonApply, CDlgBase)

AUI_ON_COMMAND("Btn_Book",		OnCommandActive)
AUI_ON_COMMAND("Btn_Apply",		OnCommandApply)
AUI_ON_COMMAND("Btn_Refresh",	OnCommandRefresh)
AUI_ON_COMMAND("IDCANCEL",		OnCommandClose)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgBattleDungeonApply, CDlgBase)

AUI_ON_EVENT("Lst_Info",			WM_LBUTTONDBLCLK,	OnLButtonDblClkList)

AUI_END_EVENT_MAP()

abase::hash_map<int, int> CDlgBattleDungeonApply::ms_tid2desc;

CDlgBattleDungeonApply::CDlgBattleDungeonApply()
{
}

CDlgBattleDungeonApply::~CDlgBattleDungeonApply()
{
}

bool CDlgBattleDungeonApply::OnInitDialog()
{
	m_pLstRoom = (PAUILISTBOX)GetDlgItem("Lst_Info");
	m_pBtnActive = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Book");
	m_pBtnJoin = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Apply");

	ms_tid2desc.clear();
	ms_tid2desc[484] = 11150;		// 焚香副本
	ms_tid2desc[505] = 11152;		// 2012元旦
	ms_tid2desc[520] = 11154;		// 古卷第三阶段单boss副本
	ms_tid2desc[534] = 11156;		// 梦回河阳
	ms_tid2desc[642] = 11160;		// 第一个6v6小游戏
	ms_tid2desc[690] = 11162;		// PVE副本
	ms_tid2desc[722] = 11164;       // 上古挑战副本
	ms_tid2desc[723] = 11182;		// 凌霄城副本
	ms_tid2desc[770] = 13500;		// 2013七夕副本
	ms_tid2desc[788] = 13502;		// 童子副本
	ms_tid2desc[789] = 13504;		// 财神副本
	ms_tid2desc[821] = 13510;		// 河阳灾变副本
	ms_tid2desc[837] = 13506;		// 150-154十神宝窟副本
	ms_tid2desc[838] = 13508;		// 155-160十神宝窟副本
	CECBattleDungeonMan::getSingletonPtr()->m_iEndTime = 0;
	CECBattleDungeonMan::getSingletonPtr()->JoinBattle(-1);
	
	m_vecImageList.clear();
	A2DSprite* pSprite = new A2DSprite;
	if( !pSprite ) return AUI_ReportError(__LINE__, __FILE__);
	
	bool bval = pSprite->Init(m_pA3DDevice, "icon\\Battle_Dungeon_Leader.tga", 0);
	if( !bval ) return AUI_ReportError(__LINE__, __FILE__);
	
	m_vecImageList.push_back(pSprite);
	m_pLstRoom->SetImageList(&m_vecImageList);
	m_pLstRoom->SetItemMask(0xffffffff);

	return true;
}

int CDlgBattleDungeonApply::GetIndexFromTmplId(int id)
{
	if (ms_tid2desc.find(id)!=ms_tid2desc.end())
	{
		return ms_tid2desc[id];
	}

	return 0;
}

bool CDlgBattleDungeonApply::Release()
{
	abase::vector<A2DSprite*>::iterator it;
	for(it=m_vecImageList.begin(); it!=m_vecImageList.end(); ++it)
	{
		A3DRELEASE(*it);
	}
	return CDlgBase::Release();
}

void CDlgBattleDungeonApply::OnShowDialog()
{
	CECBattleDungeonMan::getSingletonPtr()->Init(GetData("id"));

	if (CECBattleDungeonMan::getSingletonPtr()->GetCurBattle())
	{
		// 名字
		PAUILABEL pLab = (PAUILABEL)GetDlgItem("Title");
		pLab->SetText(CECBattleDungeonMan::getSingletonPtr()->GetCurBattle()->m_pConfigData->name);
		// 图片描述
		PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)GetDlgItem("Img_Map");
		AUIOBJECT_SETPROPERTY p;
		sprintf(p.fn, AC2AS(GetGameUIMan()->GetStringFromTable(ms_tid2desc[GetData("id")]+1)));
		pImg->SetProperty("Image File", &p);
		// 文字描述
		PAUITEXTAREA pTxt = (PAUITEXTAREA)GetDlgItem("Txt_Intro");
		pTxt->SetText(GetGameUIMan()->GetStringFromTable(ms_tid2desc[GetData("id")]));
		// 今天进入次数
		PAUILABEL pLabTimes = (PAUILABEL)GetDlgItem("Txt_Time");
		ACString strText;
		strText.Format(_AL("%d/%d"), CECBattleDungeonMan::getSingletonPtr()->m_mapDungeonEnterInfo[CECBattleDungeonMan::getSingletonPtr()->GetCurBattle()->m_iMapId],
			CECBattleDungeonMan::getSingletonPtr()->GetCurBattle()->m_pConfigData->max_finish_count);
		pLabTimes->SetText(strText);

		GetGameSession()->raid_getlist(CECBattleDungeonMan::getSingletonPtr()->GetCurBattle()->m_iMapId);
		GetGameSession()->c2s_CmdQueryRaidEnterCount(CECBattleDungeonMan::getSingletonPtr()->GetCurBattle()->m_iMapId);
	}
}

void CDlgBattleDungeonApply::OnTick()
{
	int i;
	ACString strText;
	for (i=0; i<m_pLstRoom->GetCount(); ++i)
	{
		CSplit s(m_pLstRoom->GetText(i));
		CSplit::VectorAWString vec = s.Split(_AL("\t"));

		if (vec.size()==7)
		{
			int iTime = (int)m_pLstRoom->GetItemData(i, 2) - GetGame()->GetServerGMTTime();
			if (m_pLstRoom->GetItemData(i, 3)!=0)
			{
				EditBoxItemBase itemImage(enumEIImage);
				itemImage.SetImageIndex(0);
				itemImage.SetImageFrame(0);
				vec[0] = (ACHAR)AUICOMMON_ITEM_CODE_START + itemImage.Serialize();
			}
			vec[5] = GetGameUIMan()->GetStringFromTable(13141+m_pLstRoom->GetItemData(i, 4));
			strText = vec[0]+_AL("\t")+vec[1]+_AL("\t")+vec[2]+_AL("\t")+GetTimeText(iTime)+_AL("\t")+vec[4]+_AL("\t")+vec[5]+_AL("\t")+vec[6];
			m_pLstRoom->SetText(i, strText);
			if(iTime<0)
			{
				m_pLstRoom->DeleteString(i--);
			}
		}
	}
	bool bInTeam = CECBattleDungeonMan::getSingletonPtr()->GetInTeamBattle() &&
		CECBattleDungeonMan::getSingletonPtr()->GetCurBattle() != 
		CECBattleDungeonMan::getSingletonPtr()->GetInTeamBattle();

	m_pBtnActive->Enable(!bInTeam);
	m_pBtnJoin->Enable(!bInTeam && m_pLstRoom->GetCurSel()>=0 && m_pLstRoom->GetCurSel()<m_pLstRoom->GetCount());
}

void CDlgBattleDungeonApply::OnCommandActive(const char * szCommand)
{
	if(!CECBattleDungeonMan::getSingletonPtr()->GetCurBattle())
		return;

	if (CECBattleDungeonMan::getSingletonPtr()->GetInTeamBattle())
	{
		GetGameSession()->raid_getroom(CECBattleDungeonMan::getSingletonPtr()->m_iRoomId);
		return;
	}
	TRANSCRIPTION_CONFIG*	pData = CECBattleDungeonMan::getSingletonPtr()->GetCurBattle()->m_pConfigData;
	if (!pData)
		return;

	if (pData->open_room_item_id>0 && GetHostPlayer()->GetPack()->GetItemTotalNum(pData->open_room_item_id)<pData->open_room_item_count)
	{
		CECBattleDungeonMan::getSingletonPtr()->ReportError(ERR_RAID_ITEMLIMIT);
		return;
	}
	if (!CheckCondiction())
	{
		return;
	}
	
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Battle_DungeonRoomNameInput");
	if(pData->level_controller_id[0]>0)
	{
		pDlg = GetGameUIMan()->GetDialog("Win_Battle_Dungeon_SiX_Room");
		pDlg->CheckRadioButton(1, 1);
	}
	pDlg->Show(true);
}

void CDlgBattleDungeonApply::OnCommandApply(const char * szCommand)
{
	if(CECBattleDungeonMan::getSingletonPtr()->GetInTeamBattle())
	{
		GetGameSession()->raid_getroom(CECBattleDungeonMan::getSingletonPtr()->m_iRoomId);
		return;
	}

	if (!CheckCondiction())
	{
		return;
	}

	if (m_pLstRoom->GetCurSel()>=0)
	{
		GetGameSession()->c2s_CmdNPCSevJoinRaidRoom(CECBattleDungeonMan::getSingletonPtr()->GetCurBattle()->m_iMapId, 
			CECBattleDungeonMan::getSingletonPtr()->GetCurBattle()->m_iId, m_pLstRoom->GetItemData(m_pLstRoom->GetCurSel()),RF_NONE);
	}
}

void CDlgBattleDungeonApply::OnCommandRefresh(const char * szCommand)
{
	OnShowDialog();
}

bool CDlgBattleDungeonApply::CheckCondiction()
{
	TRANSCRIPTION_CONFIG*	pData = CECBattleDungeonMan::getSingletonPtr()->GetCurBattle()->m_pConfigData;
	if (GetHostPlayer()->GetMoneyAmount()<pData->required_money)
	{
		CECBattleDungeonMan::getSingletonPtr()->ReportError(ERR_RAID_CLIENT_LACK_MONEY);
		return false;
	}
	if (CECBattleDungeonMan::getSingletonPtr()->m_mapDungeonEnterInfo[CECBattleDungeonMan::getSingletonPtr()->GetCurBattle()->m_iMapId] >=
		pData->max_finish_count)
	{
		CECBattleDungeonMan::getSingletonPtr()->ReportError(ERR_RAID_CLIENT_ENTER_ENOUGHTIMES);
		return false;
	}
	if (GetHostPlayer()->GetBasicProps().iLevel<pData->player_min_level || 
		GetHostPlayer()->GetBasicProps().iLevel>pData->player_max_level)
	{
		CECBattleDungeonMan::getSingletonPtr()->ReportError(ERR_RAID_CLIENT_PLAYER_LEVEL);
		return false;
	}
	if (GetHostPlayer()->GetRebornCount()<pData->renascence_count)
	{
		CECBattleDungeonMan::getSingletonPtr()->ReportError(ERR_RAID_RENASCENCE_COUNT);
		return false;
	}

	for (int i=0; i<4; ++i)
	{
		if(pData->required_reputation[i].reputation_type>0)
		{
			if (GetHostPlayer()->GetRegionReputation(pData->required_reputation[i].reputation_type)<pData->required_reputation[i].reputation_value)
			{
				CECBattleDungeonMan::getSingletonPtr()->ReportError(ERR_RAID_CLIENT_REPUTATION_LIIT);
				return false;
			}
		}
	}
	if (pData->required_item_id>0 && GetHostPlayer()->GetPack()->GetItemTotalNum(pData->required_item_id)<pData->required_item_count)
	{
		CECBattleDungeonMan::getSingletonPtr()->ReportError(ERR_RAID_CLIENT_ENTER_ITEM_LIMIT);
		return false;
	}
	return true;
}

void CDlgBattleDungeonApply::OnCommandClose(const char * szCommand)
{
	Show(false);
	GetGameUIMan()->EndNPCService();
}

void CDlgBattleDungeonApply::OnLButtonDblClkList(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if(m_pLstRoom->GetCurSel()>=0&&m_pLstRoom->GetCurSel()<m_pLstRoom->GetCount())
	{		
		if ((int)m_pLstRoom->GetItemData(m_pLstRoom->GetCurSel(), 1)==GetHostPlayer()->GetCharacterID())
		{			
			GetGameSession()->raid_getroom(m_pLstRoom->GetItemData(m_pLstRoom->GetCurSel()));
		}
	}
}

int CDlgBattleDungeonApply::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	using namespace GNET;
	if (strMsg=="getlist_re")
	{
		RaidGetList_Re* p = (RaidGetList_Re*)wParam;
		m_pLstRoom->ResetContent();
		CECBattleDungeonMan::getSingletonPtr()->GetCurBattle()->m_vecRoomInfo.clear();
		ACString strText;
		ACString strName;
		UINT i;
		for (i=0; i<p->fields.size(); ++i)
		{
			GRaidRoomInfo& info = p->fields[i];
			CECBattleDungeonMan::getSingletonPtr()->GetCurBattle()->m_vecRoomInfo.push_back(info);
			ACString strName((const ACHAR*)info.roomname.begin(), info.roomname.size()/sizeof(ACHAR));
			ACString strLeader((const ACHAR*)info.leadername.begin(), info.leadername.size()/sizeof(ACHAR));
			ACString strVote = _AL(" ");
			if (info.can_vote!=0)
			{
				EditBoxItemBase itemImage(enumEIImage);
				itemImage.SetImageIndex(0);
				itemImage.SetImageFrame(0);
				strVote = (ACHAR)AUICOMMON_ITEM_CODE_START + itemImage.Serialize();
			}
			strText.Format(_AL("%s\t%s\t%d/%d\t%s\t%s\t%s\t%s"), strVote, strName, info.player_number, info.player_limit, 
				GetTimeText(info.apply_endtime), strLeader, GetGameUIMan()->GetStringFromTable(13141+info.difficulty), GetGameUIMan()->GetLine(info.leader_line, true));
			m_pLstRoom->AddString(strText);
			m_pLstRoom->SetItemData(i, info.apply_endtime, 2);
			m_pLstRoom->SetItemData(i, info.leaderid, 1);
			m_pLstRoom->SetItemData(i, info.raidroom_id);
			m_pLstRoom->SetItemData(i, info.can_vote, 3);
			m_pLstRoom->SetItemHint(i, info.can_vote!=0 ? GetGameUIMan()->GetStringFromTable(15016):_AL(""));
			m_pLstRoom->SetItemData(i, info.difficulty, 4);
		}
	}
	else if (strMsg=="enter_count")
	{
		if (CECBattleDungeonMan::getSingletonPtr()->GetCurBattle() && 
			(int)wParam==CECBattleDungeonMan::getSingletonPtr()->GetCurBattle()->m_iMapId)
		{
			PAUILABEL pLabTimes = (PAUILABEL)GetDlgItem("Txt_Time");
			ACString strText;
			strText.Format(_AL("%d/%d"), CECBattleDungeonMan::getSingletonPtr()->m_mapDungeonEnterInfo[CECBattleDungeonMan::getSingletonPtr()->GetCurBattle()->m_iMapId],
				CECBattleDungeonMan::getSingletonPtr()->GetCurBattle()->m_pConfigData->max_finish_count);
			pLabTimes->SetText(strText);

			return 1;
		}
	}
	return 0;
}