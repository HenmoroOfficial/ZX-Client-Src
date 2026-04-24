// Filename	: DlgNPC.cpp
// Creator	: Tom Zhou
// Date		: October 11, 2005

#include "AFI.h"
#include "AUI\\AUISubDialog.h"
#include "AUI\\AUIImagePicture.h"
#include "aui\\CSplit.h"
#include "DlgNPC.h"
#include "DlgBattleInfo.h"
#include "DlgTaskList.h"
#include "DlgTask.h"
#include "DlgHost.h"
#include "DlgShop.h"
#include "DlgShopReputation.h"
#include "DlgProduce.h"
#include "DlgInstall.h"
#include "DlgInventory.h"
#include "DlgWorldMap.h"
#include "DlgGuildCreate.h"
#include "DlgGuildMan.h"
#include "DlgFaceLift.h"
#include "DlgMailList.h"
#include "DlgDoubleExp.h"
#include "DlgAuctionBuyList.h"
#include "DlgGuildMap.h"
#include "DlgGuildMapStatus.h"
#include "DlgGoldTrade.h"
#include "DlgStoragePW.h"
#include "DlgTalismanService.h"
#include "DlgSiege.h"
#include "DlgCityBuilding.h"
#include "DlgHorseTame.h"
#include "DlgShopExchange.h"
#include "DlgAward.h"
#include "DlgMend.h"
#include "DlgQShop.h"
#include "DlgSpread.h"
#include "DlgTerrWarMap.h"
#include "DlgBattleFactionWarApply.h"
#include "DlgBattleFactionWarControl.h"
#include "DlgBattleFactionWarWatch.h"
#include "DlgBattleFlow.h"

#include "EC_BattleDungeonTower.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameSession.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_TaskInterface.h"
#include "EC_Skill.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrConsume.h"
#include "EC_Faction.h"
#include "EC_World.h"
#include "EC_ManNPC.h"
#include "EC_NPC.h"
#include "EL_Precinct.h"
#include "EC_HelpGraphGuide.h"
#include "EC_NewHelpGuide.h"
#include "TaskTemplMan.h"
#include "Network\\ids.hxx"
#include "elementdataman.h"
#include "globaldataman.h"
#include "AUI\\AUICTranslate.h"
#include "A2DSprite.h"
#include "LuaEvent.h"

const int g_nProsperityRequire[] =
{
	15000,
	167000,
	738000,
	2216000,
	11551000,
	22736000,
	43927000,
	85119000,
	166469000,	
	INT_MAX
};


#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgNPC, CDlgBase)

AUI_ON_COMMAND("back",			OnCommand_back)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_CANCEL)
AUI_ON_COMMAND("Btn_Next",		OnCommand_NextPage)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgNPC, CDlgBase)

AUI_ON_EVENT("Lst_Main",		WM_LBUTTONUP,		OnEventLButtonUp_Lst_Main)

AUI_END_EVENT_MAP()

enum
{
	CDLGNPC_FORGET			= 0xFFFFF01,
	CDLGNPC_FACTION_CREATE,
	CDLGNPC_FACTION_UPGRADE,
	CDLGNPC_FACTION_DISMISS,
	CDLGNPC_FACE,
	CDLGNPC_MAIL,
	CDLGNPC_AUCTION,
	CDLGNPC_DOUBLEEXP,
	CDLGNPC_BATTLEINFO,
	CDLGNPC_BATTLEINFOREBORN,
	CDLGNPC_BATTLELEAVE,
	CDLGNPC_GOLDTRADE,
	CDLGNPC_GUILDSTORAGE,
	CDLGNPC_INSTALL,
	CDLGNPC_UNINSTALL1,
	CDLGNPC_UNINSTALL2,
	CDLGNPC_FAMILY_CREATE,
	CDLGNPC_FAMILY_DISMISS,
	CDLGNPC_TALISMAN_CLEAN,
	CDLGNPC_TALISMAN_COMBINE,
	CDLGNPC_TALISMAN_REFINE,
	CDLGNPC_TALISMAN_UPGRADE,
	CDLGNPC_TALISMAN_ENCHANT,
	CDLGNPC_SIEGE_INFO,
	CDLGNPC_SIEGE_ENTER,
	CDLGNPC_SIEGE_CONSTRUCTION,
	CDLGNPC_PETADOPT,
	CDLGNPC_PETFREE,
	CDLGNPC_PETCOMBINE,
	CDLGNPC_PETRENAME,
	CDLGNPC_PETEQUIP_ENHANCE,
	CDLGNPC_HORSETAME,
	CDLGNPC_INTERFIGHT1,
	CDLGNPC_LOCK,
	CDLGNPC_UNLOCK,
	CDLGNPC_MEND,
	CDLGNPC_SOUL_DEPART,
	CDLGNPC_SOUL_ENCHASE,
	CDLGNPC_SOUL_OWN,
	CDLGNPC_SOUL_RESUME,
	CDLGNPC_SOUL_SPILT,
	CDLGNPC_SJBATTLE,
	CDLGNPC_ARENA,
	CDLGNPC_ONLINE_SPREAD_GETID,
	CDLGNPC_ONLINE_SPREAD_GETLINK,
	CDLGNPC_ONLINE_SPREAD,
	CDLGNPC_FACEOFF,
	CDLGNPC_INTERFIGHT2,
	CDLGNPC_INTERFIGHTWATCH,
	CDLGNPC_SCENEBATTLE,
	CDLGNPC_BODYCHANGE_ENCHASE0,
	CDLGNPC_BODYCHANGE_ENCHASE1,
	CDLGNPC_BODYCHANGE_ENCHASE2,
	CDLGNPC_BODYCHANGE_RECOVER,
	CDLGNPC_TERRWAR_ANNO,
	CDLGNPC_TERRWAR_ENTER,
	CDLGNPC_TERRWAR_AWARD,
	CDLGNPC_TERRWAR_AWARD_SHENQI,
	CDLGNPC_MARKASTRO,
	CDLGNPC_MENDMONEY,
	CDLGNPC_EQUIP_LVUP,
	CDLGNPC_TRANSPORT_CROSS,
	CDLGNPC_TRANSPORT_ORIG,
	CDLGNPC_BATTLE_CHALLENGE,
	CDLGNPC_JACK_DRILL,
	CDLGNPC_JACK_REDRILL,
	CDLGNPC_JACK_CUSTOM,
	CDLGNPC_BIJOU_ATTACH,
	CDLGNPC_BIJOU_DETACH,
	CDLGNPC_BIJOU_UPGRADE,
	CDLGNPC_BIJOU_REFINE,
	CDLGNPC_BIJOU_EXTRACT,
	CDLGNPC_BIJOU_TRANSFER,
	CDLGNPC_KF2011_SIDERED,
	CDLGNPC_KF2011_SIDEBLUE,
	CDLGNPC_KF2011_WATCH,
	CDLGNPC_TRANSCRIPTION,	// 普通副本
	CDLGNPC_TRANSCRIPTION_TOWER,	// 爬塔副本
	CDLGNPC_TOWER_INFO,		// 爬塔副本相关个人信息
	CDLGNPC_CHANGE_NAME,	// 改名
	CDLGNPC_NAME_SEARCH,	// 改名查询
	CDLGNPC_CHANGE_NAME_FAMILY,	// 家族改名
	CDLGNPC_CHANGE_NAME_GUILD,	// 帮派改名
	CDLGNPC_TALISMAN_LEVELUP,	// 法宝飞升
	CDLGNPC_TALISMAN_SKILL_REFINE,	// 法宝技能融合
	CDLGNPC_TALISMAN_SKILL_EMBED,	// 法宝技能镶嵌
	CDLGNPC_FACTIONPKRAID_APPLY,   // 帮战申请
	CDLGNPC_FACTIONPKRAID_MAIN,    // 帮战报名
	CDLGNPC_FACTIONPKRAID_WATCH,   //  帮战观看
	CDLGNPC_FACTION_TRANSMIT_SRTVE, // 传送服务
	CDLGNPC_EQUIP_SOLT,			// 装备打孔
	CDLGNPC_ASTROLOGY_INSTALL,	// 镶嵌星座
	CDLGNPC_ASTROLOGY_UNINSTALL,// 摘除星座
	CDLGNPC_FACTION_BATTLE_LEAVE_TRANSMIT_SRTVE, // 2012-9-3 add by zy 从战场离开服务
	CDLGNPC_KINGDOM_WAR_MAIN,  //国战地图1
	CDLGNPC_KINGDOM_WAR_VICE, //国战地图2
	CDLGNPC_KINGDOM_WAR_BATH, //泡澡地图
    CDLGNPC_JEWELRY_LVUP,//首饰升级
	CDLGNPC_PRODUCE_JINFASHEN, // 快速制作金身、法身
	CDLGNPC_SKILL_FASTPRO, //生产技能快速生产
	CDLGNPC_FLOW_BATTLE, //流水席
	CDLGNPC_RESET_SKILL_PART, //技能洗点(当前)
	CDLGNPC_RESET_SKILL_ALL, //技能洗点(全部)
	CDLGNPC_RESET_TALENT, //天书洗点
	CDLGNPC_LOOK_BATTLE_INFO, //请求跨服战场信息
	CDLGNPC_ENTER_BATTLE1,	//请求进入跨服战场team1
	CDLGNPC_ENTER_BATTLE2,	//请求进入跨服战场team2
	CDLGNPC_CREATE_BUILD,		// 请求创建帮派基地
	CDLGNPC_ZAIBIAN,	//灾变副本
	CDLGNPC_GUILD_QSHOP,			// 帮派元宝商城
};

CDlgNPC::CDlgNPC()
{
}

CDlgNPC::~CDlgNPC()
{
}

bool CDlgNPC::OnInitDialog()
{
	m_pTxt_npc = (PAUILABEL)GetDlgItem("Txt_npc");
	GetGameUIMan()->AttachTaskEmotion(m_pTxt_npc);
	m_pLst_Main = (PAUILISTBOX)GetDlgItem("Lst_Main");
	m_pTxt_Content = (PAUITEXTAREA)GetDlgItem("Txt_Content");
	if(m_pTxt_Content)
		m_pTxt_Content->SetImageList(&m_vecImageList);
	m_pBtn_Back = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Back");
	m_pLst_Main->SetAutoWrap(true);
	
	m_nLastFinishId = 0;
	m_dwFinishTime = 0;
	//点到npc直接弹出对应任务m_bAutoGoto = false;
	//点到npc直接弹出对应任务m_nTaskGoto = 0;
	
    if(NULL != GetDlgItem("Shape_1"))
	   GetDlgItem("Shape_1")->Show(false);
	if(NULL != GetDlgItem("Img_1"))
	   GetDlgItem("Img_1")->Show(false);
	if(NULL != GetDlgItem("Txt_1"))
	   GetDlgItem("Txt_1")->Show(false);
	if(NULL != GetDlgItem("Txt_2"))
	   GetDlgItem("Txt_2")->Show(false);

	m_iCurPage     = 0;
	m_iWindowIndex = 0;
	m_pA2DSpriteNpcHead = NULL;
	m_pA2DSpriteNpc =  NULL;
	m_pImg_NpcHead =  (PAUIIMAGEPICTURE)GetDlgItem("Img_NpcHead");
	m_pImg_Npc =  (PAUIIMAGEPICTURE)GetDlgItem("Img_Npc");

	if( !m_pA2DSpriteNpcHead )
	{
		m_pA2DSpriteNpcHead = new A2DSprite;
		if( !m_pA2DSpriteNpcHead ) return AUI_ReportError(__LINE__, __FILE__);
	}
	if( !m_pA2DSpriteNpc )
	{
		m_pA2DSpriteNpc = new A2DSprite;
		if( !m_pA2DSpriteNpc ) return AUI_ReportError(__LINE__, __FILE__);
	}

	return true;
}

void CDlgNPC::OnShowDialog()
{
	m_bPopFinishTask = false;
	SetNpcHeadPic();
}
bool  CDlgNPC::Release()
{
	A3DRELEASE(m_pA2DSpriteNpc)
    A3DRELEASE(m_pA2DSpriteNpcHead)
	return CDlgTaskBase::Release();
}
void CDlgNPC::OnTick()
{
// 	if (m_dwFinishTime > 0 && m_nLastFinishId > 0)
// 	{
// 		if (GetTickCount() > m_dwFinishTime)
// 		{
// 			m_nLastFinishId = 0;
// 			m_dwFinishTime = 0;
// 			GetGameUIMan()->EndNPCService();
// 			Show(false);
// 		}
// 
// 	}
}
/*点到npc直接弹出对应任务
int CDlgNPC::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="goto_task")
	{
		m_bAutoGoto = wParam>0;
	}
	else if (strMsg=="set_task")
	{
		m_nTaskGoto = wParam;
	}
	return 0;
}
*/
void CDlgNPC::OnCommand_back(const char * szCommand)
{
	NPC_ESSENCE *pCurNPCEssence = GetGameUIMan()->m_pCurNPCEssence;
	if( !pCurNPCEssence )
		return;

	if( GetData() == NPC_DIALOG_TALK ||
		GetData() == NPC_DIALOG_TASK_TALK )
	{
		int i, j, nIndex;
		ACString strText;
		talk_proc *pTalk = (talk_proc *)GetDataPtr("ptr_talk_proc");
		unsigned int id = (unsigned int)m_pTxt_Content->GetData();

		if( 0xFFFFFFFF == id )		// No any more parent window.
		{
			if( GetData() == NPC_DIALOG_TALK )
				PopupNPCDialog(pCurNPCEssence);
			else if( pCurNPCEssence )
			{
				if(stricmp(GetName(),"Win_NPC_Main") == 0)
				{
					this->Show(false);
				}

				{
				   GetGameUIMan()->m_pDlgNPC->PopupTaskDialog();
				   GetGameUIMan()->m_pDlgNPC->m_pLst_Main->AddString(GetStringFromTable(503));
				   if(!GetGameUIMan()->m_pDlgNPC->IsShow())
				       GetGameUIMan()->m_pDlgNPC->Show(true);
				}
			}
		}
		else
		{
			for( i = 0; i < pTalk->num_window; i++ )
			{
				if( id != pTalk->windows[i].id ) continue;

				m_pBtn_Back->SetData(id);

				if(stricmp(GetName(),"Win_NPC_Main") == 0)
				{
					m_iCurPage = 0;
					m_iWindowIndex = i;
					RefreshTalkContent();
					m_pTxt_Content->SetData(pTalk->windows[i].id_parent);
				}
				else
				{
				 
					ACString strTaskDesc;
					strTaskDesc = FormatTaskText(pTalk->windows[i].talk_text);
					m_pTxt_Content->SetText(strTaskDesc);
					m_pTxt_Content->SetData(pTalk->windows[i].id_parent);

					m_pLst_Main->ResetContent();
					for( j = 0; j < pTalk->windows[i].num_option; j++ )
					{
						strText = GetStringFromTable(249);
						strText += FormatTaskText(pTalk->windows[i].options[j].text, false);
						m_pLst_Main->AddString(strText);
						nIndex = m_pLst_Main->GetCount() - 1;
						m_pLst_Main->SetItemData(nIndex, pTalk->windows[i].options[j].id);
						m_pLst_Main->SetItemDataPtr(nIndex, &pTalk->windows[i].options[j]);
					}

				}

				break;
			}
		}
	}
	else if( GetData() == NPC_DIALOG_TASK_LIST )
	{
		PopupNPCDialog(pCurNPCEssence);
		if( GetData() == NPC_DIALOG_TASK_LIST )
		{
			Show(false);
			GetGameUIMan()->EndNPCService();
		}
	}
	else if( GetData() == NPC_DIALOG_TRANSMIT_LIST)
	{
		PopupNPCDialog(pCurNPCEssence);
	}
	else if( GetData() == NPC_DIALOG_ESSENCE )
	{
		Show(false);
		GetGameUIMan()->EndNPCService();
	}
}

void CDlgNPC::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
	GetGameUIMan()->EndNPCService();

	int idCurFinishTask = GetGameUIMan()->m_idCurFinishTask;
	if( GetData() == NPC_DIALOG_TASK_TALK && idCurFinishTask > 0 )
	{
		GetHostPlayer()->GetTaskInterface()->OnUIDialogEnd(idCurFinishTask);
		GetGameUIMan()->m_idCurFinishTask = -1;
	}
}
void CDlgNPC::OnCommand_NextPage(const char* szCommand)
{
     if(stricmp(GetName(),"Win_NPC_Main") == 0)
	 {
	     ++m_iCurPage;
		 RefreshTalkContent();
	 }
}
void CDlgNPC::PopupNPCDialog(const NPC_ESSENCE *pEssence)
{
	int i;
	const void *pData;
	DATA_TYPE DataType;
	bool bTask = false;
	ACString strText = GetStringFromTable(249);
	elementdataman *pDataMan = g_pGame->GetElementDataMan();
	unsigned int a_uiService[] =
	{
		pEssence->id_talk_service,			// 交谈的服务ID
		pEssence->id_sell_service,			// 出售商品的服务ID
		pEssence->id_buy_service,			// 收购品的服务ID
		0,									// 安装镶嵌品的服务ID (已删除)
		0,									// 拆除镶嵌品的服务ID (已删除)
		0,									// 教授技能的服务ID (已删除)
		pEssence->id_heal_service,			// 治疗的服务ID
		pEssence->id_transmit_service,		// 传送的服务ID
		0,									// 运输的服务ID (已删除)
		pEssence->id_proxy_service,			// 代售的服务ID
		pEssence->id_storage_service,		// 仓库的服务ID
		0,									// 生产的服务ID (已删除)
		0,									// 分解的服务ID (已删除)
		0,									// 鉴定的服务ID (已删除)
		pEssence->id_war_towerbuild_service,// 建造箭塔的服务ID
		pEssence->id_resetprop_service,		// 洗点的服务ID
		pEssence->id_equipbind_service,		// 装备绑定服务
		pEssence->id_equipdestroy_service,	// 装备销毁服务
		pEssence->id_equipundestroy_service,// 装备解除销毁服务
		pEssence->id_war_archer_service,	// 城战购买弓箭手服务
		pEssence->id_item_trade_service[0],	// 物物交易
		pEssence->id_item_trade_service[1],	// 物物交易
		pEssence->id_item_trade_service[2],	// 物物交易
		pEssence->id_item_trade_service[3],	// 物物交易
		pEssence->id_equip_soul_service,	// 器魄融合服务ID：0表示不开启此服务
		pEssence->id_consign_service,		// 商品寄售服务
		pEssence->id_reputation_shop_service, // 声望商店服务
		pEssence->id_task_out_service,		// 任务相关的服务ID: 发放任务服务
		pEssence->id_task_in_service,		// 任务相关的服务ID: 验证完成任务服务
		pEssence->id_task_matter_service,	// 任务相关的服务ID: 发放任务物品服务	
	};
	
	GetGameUIMan()->m_pCurNPCEssence = (NPC_ESSENCE *)pEssence;

	SetData(NPC_DIALOG_ESSENCE);
	m_pTxt_npc->SetText(pEssence->name);
	m_pTxt_npc->SetAlign(AUIFRAME_ALIGN_CENTER);
	ACString strTaskDesc;
	strTaskDesc = FormatTaskText(pEssence->hello_msg);
	m_pTxt_Content->SetImageList(&m_vecImageList);
	m_pTxt_Content->SetText(strTaskDesc);
	
	m_pLst_Main->SetHOver(true);
	m_pLst_Main->ResetContent();

	// 传送功能强制放在最前面显示，10.6.10日更新星盘充能后面玩家觉得不舒服
	for( i = 0; i < sizeof(a_uiService) / sizeof(unsigned int); i++ )
	{
		if( !a_uiService[i] ) continue;
		
		pData = pDataMan->get_data_ptr(a_uiService[i], ID_SPACE_ESSENCE, DataType);
		if( DataType == DT_NPC_TRANSMIT_SERVICE )
		{
			NPC_TRANSMIT_SERVICE *pService = (NPC_TRANSMIT_SERVICE *)pData;
			m_pLst_Main->AddString(strText + pService->name);
			
			m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, a_uiService[i]);
			m_pLst_Main->SetItemDataPtr(m_pLst_Main->GetCount() - 1, (void *)pData);
		}
	}
	// 副本服务单独处理
	if (pEssence->id_transcription>0)
	{
		pData = pDataMan->get_data_ptr(pEssence->id_transcription, ID_SPACE_CONFIG, DataType);
		if( DataType == DT_TRANSCRIPTION_CONFIG )
		{
			TRANSCRIPTION_CONFIG *pService = (TRANSCRIPTION_CONFIG *)pData;
			//帮战副本
			if (pEssence->id_transcription == CDlgBattleFactionWarControl::GetFactionPKRaidTemplateId())
			{
				m_pLst_Main->AddString(strText + GetStringFromTable(15500));
				m_pLst_Main->SetItemData(m_pLst_Main->GetCount()-1,CDLGNPC_FACTIONPKRAID_APPLY);
				m_pLst_Main->AddString(strText + GetStringFromTable(15501));
				m_pLst_Main->SetItemData(m_pLst_Main->GetCount()-1,CDLGNPC_FACTIONPKRAID_MAIN);
				m_pLst_Main->AddString(strText + GetStringFromTable(15502));
				m_pLst_Main->SetItemData(m_pLst_Main->GetCount()-1,CDLGNPC_FACTIONPKRAID_WATCH);

			}
			else
			{
		     	m_pLst_Main->AddString(strText + pService->name);
			    m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_TRANSCRIPTION);
			    m_pLst_Main->SetItemDataPtr(m_pLst_Main->GetCount() - 1, (void *)pEssence->id_transcription);
			}
		}
		else if ( DataType == DT_TOWER_TRANSCRIPTION_CONFIG )	// 爬塔副本
		{
			TOWER_TRANSCRIPTION_CONFIG *pService = (TOWER_TRANSCRIPTION_CONFIG *)pData;
			m_pLst_Main->AddString(strText + pService->name);
			m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_TRANSCRIPTION_TOWER);
			m_pLst_Main->SetItemDataPtr(m_pLst_Main->GetCount() - 1, (void *)pEssence->id_transcription);
		}
	}
	int idInstance = GetWorld()->GetInstanceID();
	if(idInstance == 317 || idInstance == 318)
	{
		if(pEssence->id == 58382 || pEssence->id == 58381)
		{
			m_pLst_Main->AddString(strText + GetStringFromTable(19951));
			m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, pEssence->id == 58382 ? CDLGNPC_ENTER_BATTLE1:CDLGNPC_ENTER_BATTLE2);
			m_pLst_Main->AddString(strText + GetStringFromTable(19952));
			m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_LOOK_BATTLE_INFO);
		}
		else if(pEssence->id == 40473 || pEssence->id == 40474)
		{
			m_pLst_Main->AddString(strText + GetStringFromTable(19952));
			m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_LOOK_BATTLE_INFO);
		}
	}

	if( pEssence->combined_services & 0x1 )
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(983));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_HORSETAME);
	}

	if( pEssence->combined_services & 0x2 )
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(546));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_INTERFIGHT1);
	}

	if( pEssence->combined_services & 0x4 )
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(7030));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_LOCK);
		m_pLst_Main->AddString(strText + GetStringFromTable(7031));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_UNLOCK);
	}
	
	if( pEssence->combined_services & 0x10 )	// Add "faction" service.
	{
		if( GetHostPlayer()->GetFamilyID() <= 0 )
		{
			m_pLst_Main->AddString(strText + GetStringFromTable(853));
			m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_FAMILY_CREATE);
		}
		if( GetHostPlayer()->GetFactionID() <= 0 )
		{
			m_pLst_Main->AddString(strText + GetStringFromTable(592));
			m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_FACTION_CREATE);
		}
		else if( GetHostPlayer()->GetFRoleID() == GNET::_R_MASTER )
		{
			int idFaction = GetHostPlayer()->GetFactionID();
			CECFactionMan *pFMan = GetGame()->GetFactionMan();
			Faction_Info *pInfo = pFMan->GetFaction(idFaction);

			if( pInfo && pInfo->GetLevel() < 7 )
			{
				m_pLst_Main->AddString(strText + GetStringFromTable(593));
				m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_FACTION_UPGRADE);
			}

			if( pFMan->GetDeleteTime() == 0 )
			{
				m_pLst_Main->AddString(strText + GetStringFromTable(594));
				m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_FACTION_DISMISS);
			}
		}
		if( GetHostPlayer()->GetFRoleID() == GNET::_R_HEADER )
		{
			m_pLst_Main->AddString(strText + GetStringFromTable(859));
			m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_FAMILY_DISMISS);
		}
	}
	if( pEssence->combined_services & 0x20 )
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(7038));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_MEND);
	}

	if( pEssence->combined_services & 0x40 )
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(634));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_MAIL);
	}

	if( pEssence->combined_services & 0x80 )
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(644));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_AUCTION);
	}
	
	if( pEssence->combined_services & 0x100 )
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(683));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_DOUBLEEXP);
	}

	if( pEssence->combined_services & 0x800 )
	{
		// 普通战场
		int iRebornCount = GetHostPlayer()->GetRebornInfo().size();
		if (0 == iRebornCount)
 		{
			m_pLst_Main->AddString(strText + GetStringFromTable(699));
			m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_BATTLEINFO);
 		}
	}

	if( pEssence->combined_services & 0x1000 )
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(724));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_BATTLELEAVE);
	}

	if( pEssence->combined_services & 0x2000 )
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(734));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_GOLDTRADE);
	}

	if( pEssence->combined_services & 0x4000 )
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(855));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_GUILDSTORAGE);
	}

	if( pEssence->combined_services & 0x8000 )
	{
// 		m_pLst_Main->AddString(strText + GetStringFromTable(877));
// 		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_TALISMAN_UPGRADE);
		m_pLst_Main->AddString(strText + GetStringFromTable(876));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_TALISMAN_REFINE);
		m_pLst_Main->AddString(strText + GetStringFromTable(875));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_TALISMAN_COMBINE);
		m_pLst_Main->AddString(strText + GetStringFromTable(924));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_TALISMAN_ENCHANT);
		m_pLst_Main->AddString(strText + GetStringFromTable(874));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_TALISMAN_CLEAN);
	}

	if( pEssence->combined_services & 0x10000 )
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(940));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_SIEGE_INFO);
	}
	if( pEssence->combined_services & 0x20000 )
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(941));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_SIEGE_ENTER);
	}
	if( pEssence->combined_services & 0x40000 )
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(942));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_SIEGE_CONSTRUCTION);
	}
	if( pEssence->combined_services & 0x80000 )
	{
		GetGameUIMan()->GetDialog("Win_Pet_Main")->Show(true);
		NewHelpGuide::StartGuide_Step_2(GUIDE_HELP_PET_11);
		return;
		//m_pLst_Main->AddString(strText + GetStringFromTable(978));
		//m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_PETADOPT);
	}
	if( pEssence->combined_services & 0x100000 )
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(979));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_PETFREE);
	}
	if( pEssence->combined_services & 0x200000 )
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(980));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_PETCOMBINE);
	}
	if( pEssence->combined_services & 0x400000 )
	{
		
		GetGameUIMan()->GetDialog("Win_Pet_Main")->Show(true);
		NewHelpGuide::StartGuide_Step_2(GUIDE_HELP_PET_11);
		return;
		m_pLst_Main->AddString(strText + GetStringFromTable(981));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_PETRENAME);
	}

	if( pEssence->combined_services & 0x800000 )
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(7060));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_SOUL_OWN);
	}
	if( pEssence->combined_services & 0x1000000 )
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(7061));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_SOUL_ENCHASE);
	}
	if( pEssence->combined_services & 0x2000000 )
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(7062));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_SOUL_SPILT);
	}
	if( pEssence->combined_services & 0x4000000 )
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(7063));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_SOUL_RESUME);
	}
	if( pEssence->combined_services & 0x8000000 )
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(7064));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_SOUL_DEPART);
	}
	if( pEssence->combined_services & 0x10000000)
	{

		// 飞升战场
		int iRebornCount = GetHostPlayer()->GetRebornInfo().size();
		if (iRebornCount > 0)
 		{
			m_pLst_Main->AddString(strText + GetStringFromTable(699));
			m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_BATTLEINFOREBORN);
 		}		
	}
	if( pEssence->combined_services & 0x20000000)
	{

		// 宋金战场
		m_pLst_Main->AddString(strText + GetStringFromTable(7370));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_SJBATTLE);
	}
	if( pEssence->combined_services & 0x40000000)
	{
		// 竞技场
		int nHostLevel = GetHostPlayer()->GetBasicProps().iLevel;
		if (nHostLevel>=90)
		{
			m_pLst_Main->AddString(strText + GetStringFromTable(7380));
			m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_ARENA);
		}
	}
	if( pEssence->combined_services & 0x80000000)
	{

		// 线上推广
		const ACHAR *szRefCode = GetHostPlayer()->GetSelfReferenceCode();
		m_pLst_Main->AddString(strText + GetStringFromTable(7360));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_ONLINE_SPREAD_GETID);
		m_pLst_Main->AddString(strText + GetStringFromTable(7361));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_ONLINE_SPREAD_GETLINK);
		m_pLst_Main->AddString(strText + GetStringFromTable(7362));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_ONLINE_SPREAD);
	}

	if( pEssence->combined_services2 & 0x1)
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(8400));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_FACEOFF);
	}
	if( pEssence->combined_services2 & 0x2)
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(8399));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_PETEQUIP_ENHANCE);
	}
	if( pEssence->combined_services2 & 0x4)
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(8471));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_INTERFIGHT2);
	}
	if( pEssence->combined_services2 & 0x8)
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(8472));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_INTERFIGHTWATCH);
	}
	if( pEssence->combined_services2 & 0x10)
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(8396));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_SCENEBATTLE);
	}
	if( pEssence->combined_services2 & 0x20)
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(9050));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_BODYCHANGE_ENCHASE0);
		m_pLst_Main->AddString(strText + GetStringFromTable(9051));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_BODYCHANGE_ENCHASE1);
		m_pLst_Main->AddString(strText + GetStringFromTable(9052));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_BODYCHANGE_ENCHASE2);
	}
	if (pEssence->combined_services2 & 0x40)
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(9054));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_BODYCHANGE_RECOVER);
	}
	bool bGetTerrMap = false;
	if (pEssence->combined_services2 & 0x80)
	{
		m_pLst_Main->AddString(_AL("^FF7F00") + strText + GetStringFromTable(9323));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_TERRWAR_ANNO);
		GetGameSession()->territory_get_map();
		bGetTerrMap = true;
	}
	if (pEssence->combined_services2 & 0x100)
	{
		ACString strTemp;
		vector<DOMAIN_INFO> *domainInfo = globaldata_getdomaininfos2();
		for (UINT ii = 0; ii<(*domainInfo).size(); ii++)
		{
			if (pEssence->id_territory == (*domainInfo)[ii].id)
			{
				strTemp.Format(GetStringFromTable(9324), (*domainInfo)[ii].name);
			}
		}
		m_pLst_Main->AddString(strText + strTemp);
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_TERRWAR_ENTER);
		if (!bGetTerrMap)
		{
			GetGameSession()->territory_get_map();
			bGetTerrMap = true;
		}
	}
	if (pEssence->combined_services2 & 0x200)
	{
		int idFRole = GetHostPlayer()->GetFRoleID();
		if (idFRole == GNET::_R_MASTER && !bGetTerrMap)
		{
			GetGameSession()->territory_get_map();
		}
	}
	if (pEssence->combined_services2 & 0x400)
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(9460));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_MARKASTRO);
	}
	if (pEssence->combined_services2 & 0x800)
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(7039));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_MENDMONEY);
	}
	if (pEssence->combined_services2 & 0x1000)
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(3670));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_EQUIP_LVUP);
	}
	if (pEssence->combined_services2 & 0x2000)
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(3671));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_TRANSPORT_CROSS);
	}
	if (pEssence->combined_services2 & 0x4000)
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(3672));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_TRANSPORT_ORIG);
	}
	if (pEssence->combined_services2 & 0x8000)
	{
		if (GetHostPlayer()->GetRebornInfo().size()>0)
		{
			m_pLst_Main->AddString(strText + GetStringFromTable(10500));
			m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_BATTLE_CHALLENGE);
		}
	}
	if (pEssence->combined_services2 & 0x10000)
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(1513));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_JACK_DRILL);
	}
	if (pEssence->combined_services2 & 0x20000)
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(1514));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_JACK_REDRILL);
	}
	if (pEssence->combined_services2 & 0x40000)
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(1515));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_JACK_CUSTOM);
	}
	if (pEssence->combined_services2 & 0x80000)
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(1516));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_BIJOU_ATTACH);
	}
	if (pEssence->combined_services2 & 0x100000)
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(1517));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_BIJOU_DETACH);
	}
	if (pEssence->combined_services2 & 0x200000)
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(1518));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_BIJOU_UPGRADE);
	}
	if (pEssence->combined_services2 & 0x400000)
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(1519));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_BIJOU_REFINE);
	}
	if (pEssence->combined_services2 & 0x800000)
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(1520));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_BIJOU_EXTRACT);
	}
	if (pEssence->combined_services2 & 0x1000000)
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(1521));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_BIJOU_TRANSFER);
	}
	if (pEssence->combined_services2 & 0x2000000)
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(10601));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_KF2011_SIDERED);
	}
	if (pEssence->combined_services2 & 0x4000000)
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(10602));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_KF2011_SIDEBLUE);
	}
	if (pEssence->combined_services2 & 0x8000000)
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(10603));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_KF2011_WATCH);
	}
	if (pEssence->combined_services2 & 0x20000000)
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(13404));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_TOWER_INFO);
	}
	if (pEssence->combined_services2 & 0x40000000)
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(15021));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_CHANGE_NAME);
	}
	if (pEssence->combined_services2 & 0x40000000)
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(15022));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_NAME_SEARCH);
	}
	if (pEssence->combined_services2 & 0x80000000)
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(15026));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_CHANGE_NAME_FAMILY);
	}

	if (pEssence->combined_services3 & 0x1)
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(15027));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_CHANGE_NAME_GUILD);
	}
	if (pEssence->combined_services3 & 0x2)
	{
		if (GetHostPlayer()->GetRebornInfo().size()!=0)
		{			
			m_pLst_Main->AddString(strText + GetStringFromTable(15028));
			m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_TALISMAN_LEVELUP);
		}
	}
	if (pEssence->combined_services3 & 0x4)
	{
		if (GetHostPlayer()->GetRebornInfo().size()!=0)
		{			
			m_pLst_Main->AddString(strText + GetStringFromTable(15029));
			m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_TALISMAN_SKILL_REFINE);
		}
	}
	if (pEssence->combined_services3 & 0x8)
	{
		if (GetHostPlayer()->GetRebornInfo().size()!=0)
		{			
			m_pLst_Main->AddString(strText + GetStringFromTable(15030));
			m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_TALISMAN_SKILL_EMBED);
		}
	}
	if (pEssence->combined_services3 & 0x20)
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(16011));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_EQUIP_SOLT);
	}
	if (pEssence->combined_services3 & 0x40)
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(16012));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_ASTROLOGY_INSTALL);
	}
	if (pEssence->combined_services3 & 0x80)
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(16013));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_ASTROLOGY_UNINSTALL);
	}
	if(pEssence->combined_services3 & 0x400) // 2012-9-3 add by zy 从战场离开服务
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(3672));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_FACTION_BATTLE_LEAVE_TRANSMIT_SRTVE);
	}
	if (pEssence->combined_services3 & 0x800)
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(3683));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_FACTION_TRANSMIT_SRTVE);
	}
    if (pEssence->combined_services3 & 0x1000)
	{
	   	m_pLst_Main->AddString(strText + GetStringFromTable(15550));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_KINGDOM_WAR_MAIN);
		m_pLst_Main->AddString(strText + GetStringFromTable(15551));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_KINGDOM_WAR_VICE);
		m_pLst_Main->AddString(strText + GetStringFromTable(15660));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_KINGDOM_WAR_BATH);
	}
	if( pEssence->combined_services3 & 0x2000 )
	{
		GetGameSession()->get_global_value(10052);		// 飞升进度相关的全局变量
		GetGameUIMan()->GetDialog("Win_Fly_Prgs")->Show(true);
		return;
	}
	if(pEssence->combined_services3 & 0x4000)
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(3673));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_JEWELRY_LVUP);
	}
	if(pEssence->combined_services3 & 0x8000)
	{
		if(GetHostPlayer()->GetProduceSkillLevel() > 0)
		{
			m_pLst_Main->AddString(strText + GetStringFromTable(17751));
			m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1,CDLGNPC_SKILL_FASTPRO);
		}
	}
	if (pEssence->combined_services3 & 0x10000)
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(17720));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_PRODUCE_JINFASHEN);
	}
	if (pEssence->combined_services3 & 0x20000)
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(18000));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_FLOW_BATTLE);
	}
	if(pEssence->combined_services3 & 0x40000)
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(20149));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_CREATE_BUILD);	
	}
	if(pEssence->combined_services3 & 0x100000)
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(21500));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_ZAIBIAN);
	}
	if(pEssence->combined_services3 & 0x200000)
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(20220));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_GUILD_QSHOP);
	}
	if( pEssence->service_install )
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(765));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_INSTALL);
	}

	if( pEssence->service_install )
	{
		m_pLst_Main->AddString(strText + GetStringFromTable(766));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_UNINSTALL1);
//		m_pLst_Main->AddString(strText + GetStringFromTable(767));
//		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_UNINSTALL2);
	}

	//拆分洗点服务
	if ( a_uiService[15] != 0 )
	{
		pData = pDataMan->get_data_ptr(a_uiService[15], ID_SPACE_ESSENCE, DataType);

		if( DataType == DT_NPC_RESETPROP_SERVICE )
		{
			m_pLst_Main->AddString(strText + GetStringFromTable(19900));
			m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_RESET_SKILL_PART);

			m_pLst_Main->AddString(strText + GetStringFromTable(19901));
			m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_RESET_SKILL_ALL);

			m_pLst_Main->AddString(strText + GetStringFromTable(19902));
			m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, CDLGNPC_RESET_TALENT);
		}
	}

	for( i = 0; i < sizeof(a_uiService) / sizeof(unsigned int); i++ )
	{
		if( !a_uiService[i] ) continue;

		pData = pDataMan->get_data_ptr(a_uiService[i], ID_SPACE_ESSENCE, DataType);

		if( DataType == DT_NPC_TASK_IN_SERVICE ||
			DataType == DT_NPC_TASK_OUT_SERVICE ||
			DataType == DT_NPC_TASK_MATTER_SERVICE )
		{
			if( bTask )
				continue;
			else
			{
				if( m_pLst_Main->GetCount() > 0 )
				{
/* 寻径结束自动打开相应任务					if(m_bAutoGoto)	// 寻径结束自动打开相应任务
					{
						PopupTaskDialog(true);
						Show(true);
						GetGameUIMan()->m_bShowAllPanels = true;
						return;
					}
					else*/
					{
						m_pLst_Main->AddString(strText + GetStringFromTable(244));
						bTask = true;
					}
				}
				else
				{
					PopupTaskDialog(true);
					break;
				}
			}
		}
		else if( DataType == DT_NPC_TALK_SERVICE )
		{
			NPC_TALK_SERVICE *pService = (NPC_TALK_SERVICE *)pData;
			m_pLst_Main->AddString(strText + pService->name);
		}
		else if( DataType == DT_NPC_SELL_SERVICE )
		{
			NPC_SELL_SERVICE *pService = (NPC_SELL_SERVICE *)pData;
			m_pLst_Main->AddString(strText + pService->name);
		}
		else if( DataType == DT_NPC_BUY_SERVICE )
		{
			continue;
		}
		else if( DataType == DT_ITEM_TRADE_CONFIG )
		{
			ITEM_TRADE_CONFIG *pService = (ITEM_TRADE_CONFIG *)pData;
			m_pLst_Main->AddString(strText + pService->name);
		}
		else if( DataType == DT_NPC_HEAL_SERVICE )
		{
			NPC_HEAL_SERVICE *pService = (NPC_HEAL_SERVICE *)pData;
			m_pLst_Main->AddString(strText + pService->name);
		}
		else if( DataType == DT_NPC_PROXY_SERVICE )
		{
			NPC_PROXY_SERVICE *pService = (NPC_PROXY_SERVICE *)pData;
			m_pLst_Main->AddString(strText + pService->name);
		}
		else if( DataType == DT_NPC_STORAGE_SERVICE )
		{
			NPC_STORAGE_SERVICE *pService = (NPC_STORAGE_SERVICE *)pData;
			m_pLst_Main->AddString(strText + pService->name);
			m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, a_uiService[i]);
			m_pLst_Main->AddString(strText + GetStringFromTable(253));
		}
		else if( DataType == DT_NPC_WAR_TOWERBUILD_SERVICE )
		{
			NPC_WAR_TOWERBUILD_SERVICE *pService = (NPC_WAR_TOWERBUILD_SERVICE *)pData;
			m_pLst_Main->AddString(strText + pService->name);
		}
		else if( DataType == DT_NPC_EQUIPBIND_SERVICE )
		{
			NPC_EQUIPBIND_SERVICE *pService = (NPC_EQUIPBIND_SERVICE *)pData;
			m_pLst_Main->AddString(strText + pService->name);
		}
		else if( DataType == DT_NPC_EQUIPDESTROY_SERVICE )
		{
			NPC_EQUIPDESTROY_SERVICE *pService = (NPC_EQUIPDESTROY_SERVICE *)pData;
			m_pLst_Main->AddString(strText + pService->name);
		}
		else if( DataType == DT_NPC_EQUIPUNDESTROY_SERVICE )
		{
			NPC_EQUIPUNDESTROY_SERVICE *pService = (NPC_EQUIPUNDESTROY_SERVICE *)pData;
			m_pLst_Main->AddString(strText + pService->name);
		}
		else if( DataType == DT_NPC_WAR_BUY_ARCHER_SERVICE )
		{
			NPC_WAR_BUY_ARCHER_SERVICE *pService = (NPC_WAR_BUY_ARCHER_SERVICE *)pData;
			m_pLst_Main->AddString(strText + pService->name);
		}
		else if( DataType == DT_EQUIP_SOUL_MELD_SERVICE )
		{
			EQUIP_SOUL_MELD_SERVICE *pService = (EQUIP_SOUL_MELD_SERVICE *)pData;
			m_pLst_Main->AddString(strText + pService->name);
			ACString strName(pService->name);
			m_pAUIManager->GetDialog("Win_SoulCombine")->GetDlgItem("Title")->SetText(strName);
		}
		else if( DataType == DT_NPC_CONSIGN_SERVICE )
		{
			NPC_CONSIGN_SERVICE *pService = (NPC_CONSIGN_SERVICE *)pData;
			m_pLst_Main->AddString(strText + pService->name);
		}
		else if(DataType == DT_NPC_REPUTATION_SHOP_SERVICE)
		{
			NPC_REPUTATION_SHOP_SERVICE* pService = (NPC_REPUTATION_SHOP_SERVICE*)pData;
			m_pLst_Main->AddString(strText + pService->name);
		}
		else
			continue;

		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, a_uiService[i]);
		m_pLst_Main->SetItemDataPtr(m_pLst_Main->GetCount() - 1, (void *)pData);
	}

	if( pEssence->domain_related == 1 )
	{
		if( GetHostPlayer()->GetFRoleID() == GNET::_R_UNMEMBER )
			m_pLst_Main->ResetContent();
		else
		{
			CECNPCMan *pNPCMan = GetWorld()->GetNPCMan();
			CECNPC *pNPC = pNPCMan->GetNPC(GetHostPlayer()->GetCurServiceNPC());
			if( pNPC )
			{
				A3DVECTOR3 pos = pNPC->GetServerPos();
				CECWorld *pWorld = GetWorld();
				CELPrecinct* pPrecinct = pWorld->GetPrecinctSet()->IsPointIn(
					pos.x, pos.z, pWorld->GetInstanceID());
				int idFaction = -1;
				if( pPrecinct )
				{
					vector<DOMAIN_INFO> *info = globaldata_getdomaininfos();
					int idDomain = pPrecinct->GetDomainID();
					DWORD j;
					for(j = 0; j < info->size(); j++ )
					{
						if( (*info)[j].id == idDomain )
						{
							idFaction = (*info)[j].id_owner;
							break;
						}
					}
				}
				if( GetHostPlayer()->GetFactionID() != idFaction )
					m_pLst_Main->ResetContent();
			}
		}
	}
	
	// Add "EXIT" choice.
	m_pLst_Main->AddString(GetStringFromTable(503));
	
	// 必须在最后再显示，因为有些功能可能不需要显示该界面，这个函数前面返回
	Show(true);
	GetGameUIMan()->m_bShowAllPanels = true;
	NewHelpGuide::TriggerOpeningNpcTalk(pEssence->id);
}
void CDlgNPC::SetNpcHeadPic()
{
	NPC_ESSENCE *pEssence = GetGameUIMan()->m_pCurNPCEssence;
	if( NULL == pEssence )
		return;
	AString strFile1, strFile;
	if (NULL != m_pImg_NpcHead)
	{
    	af_GetFileTitle(g_pGame->GetDataPath(pEssence->head_icon), strFile1);
		strFile.Format("npcimg\\%s",strFile1);
		strFile.MakeLower();
		if(m_pA2DSpriteNpcHead)
			m_pA2DSpriteNpcHead->Release();
		bool bVal = m_pA2DSpriteNpcHead->Init(m_pA3DDevice, strFile, 0 );
		if (!bVal)
		{
			m_pImg_NpcHead->SetCover( NULL, 0);
		}
		else
		{
		    m_pImg_NpcHead->SetCover( m_pA2DSpriteNpcHead, 0);
		}

	}

	//待修改
	if(NULL != m_pImg_Npc)
	{
		af_GetFileTitle(g_pGame->GetDataPath(pEssence->head_icon), strFile1);
		strFile.Format("npcimg\\%s",strFile1);
		strFile.MakeLower();
		if(m_pA2DSpriteNpc)
			m_pA2DSpriteNpc->Release();
		bool bVal = m_pA2DSpriteNpc->Init(m_pA3DDevice, strFile, 0 );
		if (!bVal)
		{
			m_pImg_Npc->SetCover( NULL, 0);
		}
		else
		{
		    m_pImg_Npc->SetCover( m_pA2DSpriteNpc, 0);
		}
	}
}
void CDlgNPC::RefreshTalkContent()
{
    if(stricmp(GetName(),"Win_NPC_Main") == 0)
	{
	    talk_proc *pTalk = (talk_proc *)GetDataPtr("ptr_talk_proc");
		if(NULL != pTalk && m_iWindowIndex >= 0 && m_iWindowIndex < pTalk->num_window)
		{
			int i, nIndex;
			ACString strText;

			ACString strTaskDesc;
			ACString strCurDesc;
			strTaskDesc = pTalk->windows[m_iWindowIndex].talk_text;
			CSplit s = AC2WC(strTaskDesc);
			CSplit::VectorAWString vec = s.Split(L"\\f");

			
			
			if(m_iCurPage >=0 && m_iCurPage <(int)vec.size())
			{
			     strCurDesc = WC2AC(vec[m_iCurPage]);
				 strCurDesc = FormatTaskText(strCurDesc);
			}

			if(NULL != GetDlgItem("Btn_Next"))
			   GetDlgItem("Btn_Next")->Show(m_iCurPage+1 < (int)vec.size());
			

			m_pTxt_Content->SetImageList(&m_vecImageList);
			m_pTxt_Content->SetText(strCurDesc);
			m_pTxt_npc->SetText(_AL(""));
			m_pLst_Main->ResetContent();

			if(m_iCurPage == vec.size()-1)
			{
				for( i = 0; i < pTalk->windows[m_iWindowIndex].num_option; i++ )
				{
					strText = GetStringFromTable(249);
					strText += FormatTaskText(pTalk->windows[m_iWindowIndex].options[i].text, false);
					m_pLst_Main->AddString(strText);
					nIndex = m_pLst_Main->GetCount() - 1;
					m_pLst_Main->SetItemData(nIndex, pTalk->windows[m_iWindowIndex].options[i].id);
					m_pLst_Main->SetItemDataPtr(nIndex, &pTalk->windows[m_iWindowIndex].options[i]);
				}
			}
		}

	}
}
void CDlgNPC::PopupNPCDialog(talk_proc *pTalk)
{
	if( pTalk->num_window == 0 ) return;

	SetDataPtr(pTalk,"ptr_talk_proc");
	m_pBtn_Back->SetData(pTalk->windows[0].id);

	if(stricmp(GetName(),"Win_NPC_Main") == 0)
	{
	      m_iCurPage = 0;
		  m_iWindowIndex = 0;
		  RefreshTalkContent();
	}
	else
	{
		int i, nIndex;
		ACString strText;
		ACString strTaskDesc;
		strTaskDesc = FormatTaskText(pTalk->windows[0].talk_text);
		m_pTxt_Content->SetImageList(&m_vecImageList);
		m_pTxt_Content->SetText(strTaskDesc);
		m_pTxt_npc->SetText(_AL(""));

		m_pLst_Main->ResetContent();
		for( i = 0; i < pTalk->windows[0].num_option; i++ )
		{
			strText = GetStringFromTable(249);
			strText += FormatTaskText(pTalk->windows[0].options[i].text, false);
			m_pLst_Main->AddString(strText);
			nIndex = m_pLst_Main->GetCount() - 1;
			m_pLst_Main->SetItemData(nIndex, pTalk->windows[0].options[i].id);
			m_pLst_Main->SetItemDataPtr(nIndex, &pTalk->windows[0].options[i]);
		}
	}
	
	if( !IsShow() ) Show(true);
}

void CDlgNPC::PopupTaskDialog(bool bTaskNPC)
{
	NPC_ESSENCE *pCurNPCEssence = GetGameUIMan()->m_pCurNPCEssence;
	NPC_ESSENCE *pEssence = pCurNPCEssence;
	if( !pEssence ) return;

	int i, j, idTask;
	const void *pData;
	DATA_TYPE DataType;
	unsigned int a_idTask[512];
	int task_count;
	unsigned int a_uiService[] =
	{
		pEssence->id_task_out_service,		// 任务相关的服务ID: 发放任务服务
		pEssence->id_task_in_service,		// 任务相关的服务ID: 验证完成任务服务
		pEssence->id_task_matter_service	// 任务相关的服务ID: 发放任务物品服务
	};
	int idLastTask = 0, nLastTaskTime = 0, nFinishTime;
	elementdataman *pDataMan = g_pGame->GetElementDataMan();
	abase::hash_map<int, int> mapID, mapIn, mapOut, mapMatter;
	CECTaskInterface *pTask = GetHostPlayer()->GetTaskInterface();

	bool b1 = false;
	for( i = 0; i < sizeof(a_uiService) / sizeof(unsigned int); i++ )
	{
		pData = pDataMan->get_data_ptr(a_uiService[i], ID_SPACE_ESSENCE, DataType);

		if( DataType == DT_NPC_TASK_IN_SERVICE ||
			DataType == DT_NPC_TASK_OUT_SERVICE )
		{
			if( DataType == DT_NPC_TASK_IN_SERVICE )
			{
				NPC_TASK_IN_SERVICE *pService = (NPC_TASK_IN_SERVICE *)pData;
				memcpy(a_idTask, pService->id_tasks, sizeof(pService->id_tasks));
				task_count = SIZE_OF_ARRAY(pService->id_tasks);
			}
			else
			{
				NPC_TASK_OUT_SERVICE *pService = (NPC_TASK_OUT_SERVICE *)pData;

				if (pService->id_task_set == 0)
				{
					memcpy(a_idTask, pService->id_tasks, sizeof(pService->id_tasks));
					task_count = SIZE_OF_ARRAY(pService->id_tasks);
				}
				else // 任务库
				{
					const unsigned short* pStorage = pTask->GetStorageTasks(pService->id_task_set);
					task_count = 0;

					if (pStorage)
					{
						for (int i = 0; i < SIZE_OF_ARRAY(pService->id_tasks); i++)
						{
							int id = pService->id_tasks[i];

							if (id == 0)
								continue;

							for (int j = 0; j < TASK_STORAGE_LEN; j++)
							{
								if (pStorage[j] == id)
								{
									a_idTask[task_count] = id;
									task_count++;
								}
							}
						}
					}

					b1 = true;
				}
			}

			for( j = 0; j < task_count; j++ )
			{
				idTask = a_idTask[j];
				if( idTask <= 0 ) continue;

				mapID[idTask] = a_uiService[i];
				if( DataType == DT_NPC_TASK_IN_SERVICE )
					mapIn[idTask] = a_uiService[i];
				else
					mapOut[idTask] = a_uiService[i];

				nFinishTime = (int)pTask->GetTaskFinishedTime(idTask);
				if( nFinishTime > nLastTaskTime )
				{
					idLastTask = idTask;
					nLastTaskTime = nFinishTime;
				}
			}
		}
		else if( DataType == DT_NPC_TASK_MATTER_SERVICE )
		{
			NPC_TASK_MATTER_SERVICE *pService = (NPC_TASK_MATTER_SERVICE *)pData;

			for( j = 0; j < 16; j++ )
			{
				idTask = pService->tasks[j].id_task;
				if( idTask > 0 )
				{
					mapID[idTask] = a_uiService[i];
					mapMatter[idTask] = a_uiService[i];
				}
			}
		}
	}

	A3DCOLOR color;
	ACString strText;
	const talk_proc *pTalk;
	ATaskTempl *pTemp, *pTempRoot;
	int nIndex, nNumTasks = 0;
	abase::hash_map<int, int>::iterator it;
	int nHostLevel = GetHostPlayer()->GetBasicProps().iLevel;
	ATaskTemplMan *pTempMan = g_pGame->GetTaskTemplateMan();

	m_pLst_Main->ResetContent();
	if( b1 )
	{
		m_pLst_Main->AddString(GetStringFromTable(229));
		m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, (DWORD)-1);
	}
	for( it = mapID.begin(); it != mapID.end(); ++it )
	{
		idTask = it->first;

		color = 0;
		pTemp = NULL;
		pTalk = NULL;
		color = m_pLst_Main->GetColor();
		if( mapOut[idTask] )
		{
			if( pTask->HasTask(idTask) )
			{
				if( !pTask->CanFinishTask(idTask) )
				{
					pTemp = pTempMan->GetTaskTemplByID(idTask);
					if ( pTemp )
					{
						pTalk = pTemp->GetUnfinishedTalk();
					}
				}
			}
			else if( !b1 && pTask->CanShowTask(idTask) )
			{
				pTemp = pTempMan->GetTaskTemplByID(idTask);
				
				if ( pTemp )
				{
					if( 0 == pTask->CanDeliverTask(idTask) )
						pTalk = pTemp->GetDeliverTaskTalk();
					else
					{
						pTalk = pTemp->GetUnqualifiedTalk();
						color = A3DCOLORRGB(128, 128, 128);
					}
				}
			}
		}
		if( mapIn[idTask] && pTask->HasTask(idTask)
			&& pTask->CanFinishTask(idTask) && !pTalk )
		{
			pTemp = pTempMan->GetTaskTemplByID(idTask);
			if ( pTemp )
			{
				pTalk = pTemp->GetAwardTalk();
			}
		}

		if( pTemp && pTalk && (pTalk->num_window > 1 || pTalk->num_window == 1 && wcslen(pTalk->windows[0].talk_text)))
		{
			strText = GetStringFromTable(249);
			pTempRoot = (ATaskTempl *)pTemp->GetTopTask();
			if( pTemp != pTempRoot )
			{
				strText += pTempRoot->GetName();
				strText += _AL(" - ");
			}
			strText += pTemp->GetName();
			if ((pTemp->GetID()==23475) 
				&& m_pLst_Main->GetCount()==0)
			{
				GetGameRun()->GetHelpGraphGuide()->FinishHelp(CECHelpGraphGuide::HELP2012_3_1_TRACK);
			}
			m_pLst_Main->AddString(strText);
			nIndex = m_pLst_Main->GetCount() - 1;
			m_pLst_Main->SetItemData(nIndex, it->second);			// Service ID.
			m_pLst_Main->SetItemDataPtr(nIndex, (void *)pTalk);	// Talk data.
			m_pLst_Main->SetItemData(nIndex,idTask,1);

			int nDisplayType = 0;
// 			if( !pTemp->IsKeyTask() )
// 				nType = pTemp->GetType() + 1;
			const ATaskTempl* pTopTask = pTemp->GetTopTask();
			nDisplayType = pTopTask->GetDisplayType();
			m_pLst_Main->SetItemTextColor(nIndex, TaskNewColor[nDisplayType]);
/* 寻径结束自动打开相应任务
			if (m_bAutoGoto && m_nTaskGoto==idTask)
			{
				GetGameUIMan()->PopupNPCDialog((talk_proc*)pTalk);
				m_pTxt_Content->SetData(0xFFFFFFFF);
				SetData(NPC_DIALOG_TASK_TALK);
				m_bAutoGoto = false;
				m_nTaskGoto = 0;
				return;
			}*/
			nNumTasks++;
		}
	}

	if( idLastTask > 0 )
	{
		if( nNumTasks > 0 || b1 )
			m_pTxt_Content->SetText(GetStringFromTable(502));
		else
			m_pTxt_Content->SetText(GetStringFromTable(501));
	}
	else if( bTaskNPC )
	{
		ACString strTaskDesc;
		strTaskDesc = FormatTaskText(pCurNPCEssence->hello_msg);
		m_pTxt_Content->SetImageList(&m_vecImageList);
		m_pTxt_Content->SetText(strTaskDesc);
	}
	else
	{
		if( nNumTasks > 0 || b1 )
			m_pTxt_Content->SetText(GetStringFromTable(502));
		else
			m_pTxt_Content->SetText(GetStringFromTable(501));
	}

	SetData(NPC_DIALOG_TASK_LIST);
}

void CDlgNPC::PopupTaskFinishDialog(int idTask, talk_proc *pTalk)
{
	bool bOnlyFinish = true;
	if (IsShow())
		bOnlyFinish = false;

	PopupNPCDialog(pTalk);
	SetData(NPC_DIALOG_TASK_TALK);
	GetGameUIMan()->m_idCurFinishTask = idTask;
	if(bOnlyFinish)
		m_bPopFinishTask = true;
}

void CDlgNPC::PopupTransmitDialog(const NPC_TRANSMIT_SERVICE *pService)
{
	DATA_TYPE DataType;
	talk_proc *pTalk = (talk_proc *)g_pGame->GetElementDataMan()
		->get_data_ptr(pService->id_dialog, ID_SPACE_TALK, DataType);
	if( pTalk )
	{
		ACString strTaskDesc;
		strTaskDesc = FormatTaskText(pTalk->windows[0].talk_text);
		m_pTxt_Content->SetImageList(&m_vecImageList);
		m_pTxt_Content->SetText(strTaskDesc);
	}
	else
		m_pTxt_Content->SetText(_AL(""));
	m_pLst_Main->ResetContent();
	int i;
	for(i = 0; i < 16; i++ )
	{
		if( pService->targets[i].id_world <= 0 )
			break;

		m_pLst_Main->AddString(pService->targets[i].name);
		if( pService->targets[i].required_level > GetHostPlayer()->GetBasicProps().iLevel )
		{
			m_pLst_Main->SetItemTextColor(m_pLst_Main->GetCount() - 1, A3DCOLORRGB(255, 0, 0));
			m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, 1);
		}
		else if( pService->targets[i].fee > GetHostPlayer()->GetMoneyAmount() )
		{
			m_pLst_Main->SetItemTextColor(m_pLst_Main->GetCount() - 1, A3DCOLORRGB(255, 0, 0));
			m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, 2);
		}
		else
		{
			m_pLst_Main->SetItemTextColor(m_pLst_Main->GetCount() - 1, A3DCOLORRGB(255, 255, 255));
			m_pLst_Main->SetItemData(m_pLst_Main->GetCount() - 1, 0);
		}
	}
	SetData(NPC_DIALOG_TRANSMIT_LIST);
}

bool CDlgNPC::PopupCorrespondingServiceDialog(
	int idFunction, int iService, void *pData)
{
	PAUIDIALOG pShow1 = NULL, pShow2 = NULL;
	bool bActive1 = false;
	NPC_ESSENCE *pCurNPCEssence = GetGameUIMan()->m_pCurNPCEssence;

	if( idFunction == NPC_SELL || idFunction == NPC_BUY )
	{
		// PAUISTILLIMAGEBUTTON pButton;

		pShow1 = m_pAUIManager->GetDialog("Win_Shop");
		pShow2 = m_pAUIManager->GetDialog("Win_Inventory");

		GetHostPlayer()->PrepareNPCService(iService);
		GetGameUIMan()->m_pDlgShop->UpdateShop(1);

//		if( pCurNPCEssence && pCurNPCEssence->id_repair_service )
//		{
//			pShow1->GetDlgItem("Btn_Repair")->Show(true);
//			pShow1->GetDlgItem("Btn_RepairAll")->Show(true);
//		}
//		else
//		{
//			pShow1->GetDlgItem("Btn_Repair")->Show(false);
//			pShow1->GetDlgItem("Btn_RepairAll")->Show(false);
//		}
		
		pShow2->CheckRadioButton(2, 1);
#ifdef LUA_UI
#else
		GetGameUIMan()->m_pDlgInventory->SetShowItem(CDlgInventory::INVENTORY_ITEM_NORMAL);
#endif	
	}
	else if( idFunction == ITEM_TRADE_SERVICE )
	{
		for(int j = 0; j < 4; j++)
			if(pCurNPCEssence->id_item_trade_service[j] == iService)
			{
				CDlgShopExchange::SetShopIndex(j);
				pShow1 = m_pAUIManager->GetDialog("Win_ShopExchange");
				((CDlgShopExchange*)pShow1)->OnCommand_set("set1");
				pShow2 = m_pAUIManager->GetDialog("Win_Inventory");
				break;
			}
	}
	else if( idFunction == EQUIP_SOUL_MELD )
	{
		NPC_ESSENCE *pEssence = GetGameUIMan()->m_pCurNPCEssence;
		if (pEssence->id_territory==0)
			pShow1 = m_pAUIManager->GetDialog("Win_SoulCombine");
		else
		{
			vector<DOMAIN_INFO> *info = globaldata_getdomaininfos2();
			for(UINT i = 0; i < info->size(); i++ )
			{
				DOMAIN_INFO di = (*info)[i];
				if (di.id == pEssence->id_territory)
				{
					if (di.id_owner>0 && di.id_owner==GetHostPlayer()->GetFactionID())
					{
						pShow1 = m_pAUIManager->GetDialog("Win_SoulCombine");
					}
					else
					{
						ACString strText;
						strText.Format(GetGameUIMan()->GetStringFromTable(9360), di.name);
						GetGameUIMan()->MessageBox("", strText);			
						GetGameUIMan()->EndNPCService();	
					}
					break;
				}
			}

		}
	}
	else if( idFunction == NPC_CONSIGN )
	{
		if (GetHostPlayer()->GetSrcServerID()!=0)
		{
			GetGameUIMan()->EndNPCService();
		}
		else
		{
			pShow1 = m_pAUIManager->GetDialog("Win_ShopConsign");
			pShow2 = m_pAUIManager->GetDialog("Win_Inventory");
		}
	}
	else if( idFunction == NPC_TRANSCRIPTION)
	{
		pShow1 = m_pAUIManager->GetDialog("Win_Battle_DungeonApply");
		if (int(((const TRANSCRIPTION_CONFIG *)pData)->id)==CECBattleDungeonTower::TOWER_TEMP_ID)
		{
			pShow1 = GetGameUIMan()->GetDialog("Win_Battle_Dungeon_Tower");
		}
		pShow1->SetData(((const TRANSCRIPTION_CONFIG *)pData)->id, "id");
	}
	else if( idFunction == NPC_HEAL )
	{
		GetGameSession()->c2s_CmdNPCSevHeal();
		GetGameUIMan()->EndNPCService();
	}
	else if( idFunction == NPC_TRANSMIT )
	{
		PopupTransmitDialog((const NPC_TRANSMIT_SERVICE *)pData);
		return true;
	}
// 	else if( idFunction == NPC_SKILL )
// 	{
// 		pShow1 = m_pAUIManager->GetDialog("Win_Teach");
// 		GetHostPlayer()->PrepareNPCService(iService);
// 		GetGameUIMan()->UpdateTeach(0);
// 	}
// 	else if( idFunction == NPC_WAR_TOWERBUILD )
// 	{
// 		pShow1 = m_pAUIManager->GetDialog("Win_WarTower");
// 	}
	else if( idFunction == NPC_RESETPROP )
	{
		pShow1 = m_pAUIManager->GetDialog("Win_ResetProp");
		pShow2 = m_pAUIManager->GetDialog("Win_Inventory");
	}
	else if( idFunction == NPC_GIVE_TASK )
	{
		NPC_ESSENCE* pEssence = GetGameUIMan()->m_pCurNPCEssence;

		if (pEssence)
		{
			DATA_TYPE DataType;
			const void* pSvr = g_pGame->GetElementDataMan()->get_data_ptr(pEssence->id_task_out_service, ID_SPACE_ESSENCE, DataType);

			if (pSvr && DataType == DT_NPC_TASK_OUT_SERVICE)
			{
				talk_proc::option *opt = (talk_proc::option *)pData;
				GetGameSession()->c2s_CmdNPCSevAcceptTask(opt->param, static_cast<const NPC_TASK_OUT_SERVICE*>(pSvr)->id_task_set);
			}
		}

		GetGameUIMan()->EndNPCService();	
	}
	else if( idFunction == NPC_COMPLETE_TASK )
	{
		AWARD_DATA ad;
		talk_proc::option *opt = (talk_proc::option *)pData;
		CECTaskInterface *pTask = GetHostPlayer()->GetTaskInterface();

		pTask->GetAwardCandidates(opt->param, &ad);
		if( ad.m_ulCandItems > 1 )
		{
			pShow1 = m_pAUIManager->GetDialog("Win_Award");
			pShow1->SetDataPtr(pData,"ptr_talk_proc");
			pShow1->CheckRadioButton(1, 0);
			pShow1->GetDlgItem("Btn_Confirm")->Enable(false);

			CDlgAward* pAward = (CDlgAward*)pShow1;

			pAward->ChooseAward(ad);
			
		}
		else
		{
			GetGameSession()->c2s_CmdNPCSevReturnTask(opt->param, 0);
			GetGameUIMan()->EndNPCService();
			
			if (opt->param==23475)
			{
				GetGameRun()->GetHelpGraphGuide()->FinishHelp(CECHelpGraphGuide::HELP2012_3_3_TASK_ACCEPT);
			}
			NewHelpGuide::FinishGuide(GUIDE_HELP_NPCCHAT_2);
			m_nLastFinishId = opt->param;
			m_dwFinishTime = GetTickCount() + 1000;
			Show(false);
			return true;
		}
	}
	else if( idFunction == NPC_EQUIPBIND )
	{
		pShow1 = m_pAUIManager->GetDialog("Win_EquipBind");
		pShow2 = m_pAUIManager->GetDialog("Win_Inventory");
	}
	else if( idFunction == NPC_EQUIPDESTROY )
	{
		pShow1 = m_pAUIManager->GetDialog("Win_EquipDestroy");
		pShow2 = m_pAUIManager->GetDialog("Win_Inventory");
	}
	else if( idFunction == NPC_EQUIPUNDESTROY )
	{
		pShow1 = m_pAUIManager->GetDialog("Win_EquipUndestroy");
		pShow2 = m_pAUIManager->GetDialog("Win_Inventory");
	}
	else if( idFunction == NPC_WAR_BUY_ARCHER )
	{
		((CDlgCityBuilding*)m_pAUIManager->GetDialog("Win_CityBuilding"))->OnBuyArcherService((NPC_WAR_BUY_ARCHER_SERVICE*)pData);
		GetGameUIMan()->EndNPCService();
	}
	else if( idFunction == NPC_GIVE_TASK_MATTER )
	{
		talk_proc::option *opt = (talk_proc::option *)pData;
		GetGameSession()->c2s_CmdNPCSevTaskMatter(opt->param);
		GetGameUIMan()->EndNPCService();
	}
	else if( idFunction == NPC_STORAGE )
	{
		if( GetHostPlayer()->TrashBoxHasPsw() )
		{
			pShow1 = m_pAUIManager->GetDialog("Win_InputString");
			bActive1 = false;
			pShow1->GetDlgItem("DEFAULT_Txt_Input")->SetText(_AL(""));
			pShow1->SetData(0);
			((CDlgStoragePW*)pShow1)->SetFaction(false);
		}
		else
			g_pGame->GetGameSession()->c2s_CmdNPCSevOpenTrash("");
	}
	else if( idFunction == NPC_STORAGE_PASSWORD )
	{
		pShow1 = m_pAUIManager->GetDialog("Win_InputString3");
		bActive1 = false;
	}
	else if( idFunction == TALK_RETURN )
	{
		OnCommand_back("back");
		return true;		// To avoid to close NPC dialog.
	}
	else if( idFunction == TALK_EXIT )
	{
		if (!m_bPopFinishTask)
			GetGameUIMan()->EndNPCService();

		int idCurFinishTask = GetGameUIMan()->m_idCurFinishTask;
		if( GetData() == CDlgNPC::NPC_DIALOG_TASK_TALK && idCurFinishTask > 0 )
		{
			GetHostPlayer()->GetTaskInterface()->OnUIDialogEnd(idCurFinishTask);
			GetGameUIMan()->m_idCurFinishTask = -1;
		}
	}
	else if( idFunction == TALK_GIVEUP_TASK)
	{
		talk_proc::option *opt = (talk_proc::option *)pData;
		GetHostPlayer()->GetTaskInterface()->GiveUpTask(opt->param);
		GetGameUIMan()->EndNPCService();
	}
	else if(idFunction == 500)
	{
		pShow1 = m_pAUIManager->GetDialog("Win_Shop_Credit");
		((CDlgShopReputation*)pShow1)->OnCommand_set("set1");
		pShow2 = m_pAUIManager->GetDialog("Win_Inventory");

		GetHostPlayer()->PrepareNPCService(iService);
		GetGameUIMan()->m_pDlgShopReputation->OnCommand_set("set1");
		GetGameUIMan()->m_pDlgShopReputation->UpdateShop();
	}
	else
	{
		GetGameUIMan()->UnfreezeAllItems();
		GetGameUIMan()->EndNPCService();
	}

	if( pShow1 )
	{
		DATA_TYPE DataType;
		elementdataman *pDataMan = g_pGame->GetElementDataMan();

		pDataMan->get_data_ptr(iService, ID_SPACE_ESSENCE, DataType);

		pShow1->Show(true, bActive1);
		pShow1->SetData(DataType);

		if( pShow2 )
		{
			POINT ptPos = pShow1->GetPos();
			if( ptPos.x == 0 && ptPos.y == 0 )
			{
				SIZE s1 = pShow1->GetSize();
				SIZE s2 = pShow2->GetSize();
				A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
				int x, y = (p->Height - max(s1.cy, s2.cy)) / 2;

				x = (p->Width - s1.cx - s2.cx) / 2;
				
				// old : pShow1->SetPos(x, y);
				pShow1->SetPosEx(x, y);

				x += s1.cx;
				
				// old : pShow2->SetPos(x, y);
				pShow2->SetPosEx(x, y);
			}

			pShow2->Show(true);
		}
	}
	Show(false);

	return true;
}

//完成任务继续打开hello界面，2010/5/28取消, fuchonggang
void CDlgNPC::AutoPopup()
{
	return;
	if (m_nLastFinishId>0)
	{
		bool bFinish = true;
		CECTaskInterface *pTask = GetHostPlayer()->GetTaskInterface();
		int iSize = pTask->GetTaskCount();
		for( int i = 0; i < iSize; i++ )
		{
			if (m_nLastFinishId == pTask->GetTaskId(i))
				bFinish = false;
		}
		if (bFinish && iSize>0)
		{
			GetGameSession()->c2s_CmdNPCSevHello(GetHostPlayer()->GetSelectedTarget());
			m_nLastFinishId = 0;
			m_dwFinishTime = 0;
//			GetGameUIMan()->EndNPCService();
//			Show(false);
		} 
	}

}

void CDlgNPC::OnEventLButtonUp_Lst_Main(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	POINT ptPos = pObj->GetPos();
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - ptPos.x - p->X;
	int y = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;
	if( m_pLst_Main->GetHitArea(x, y) != AUILISTBOX_RECT_TEXT )
		return;

	int nCurSel = m_pLst_Main->GetCurSel();
	if( nCurSel < 0 || nCurSel >= m_pLst_Main->GetCount() )
		return;

	int i, j, nIndex;
	ACString strText;
	DATA_TYPE DataType;
	CECTaskInterface *pTask = GetHostPlayer()->GetTaskInterface();
	unsigned int id = m_pLst_Main->GetItemData(nCurSel);
	if( GetData() == NPC_DIALOG_TALK ||
		GetData() == NPC_DIALOG_TASK_TALK )
	{
		unsigned int id = m_pLst_Main->GetItemData(nCurSel);
		talk_proc *pTalk = (talk_proc *)GetDataPtr("ptr_talk_proc");

		if( TALKPROC_IS_TERMINAL(id) )			// Ternimal.
		{
			for( i = 0; i < pTalk->num_window; i++ )
			{
				if( id != pTalk->windows[i].id ) continue;

				GetGameUIMan()->MessageBox("", FormatTaskText(pTalk->windows[i].talk_text),
					MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
				GetGameUIMan()->EndNPCService();
				Show(false);

				break;
			}

		}
		else if( TALKPROC_IS_FUNCTION(id) )		// Function.
		{
			if( GetGameUIMan()->m_pCurNPCEssence )
			{
				NPC_ESSENCE *pEssence = GetGameUIMan()->m_pCurNPCEssence;
				int nIndex = TALKPROC_GET_FUNCTION_ID(id);
				unsigned int a_uiService[] =
				{
					pEssence->id_talk_service,			// 交谈的服务ID
					pEssence->id_sell_service,			// 出售商品的服务ID
					pEssence->id_buy_service,			// 收购品的服务ID
					pEssence->id_item_trade_service[0],	// 物物交易
					pEssence->id_item_trade_service[1],	// 物物交易
					pEssence->id_item_trade_service[2],	// 物物交易
					pEssence->id_item_trade_service[3],	// 物物交易
					pEssence->id_consign_service,		// 角色寄售
					0,									// 修理商品的服务ID
					0,									// 安装镶嵌品的服务ID
					0,									// 拆除镶嵌品的服务ID
					pEssence->id_task_out_service,		// 任务相关的服务ID: 发放任务服务
					pEssence->id_task_in_service,		// 任务相关的服务ID: 验证完成任务服务
					pEssence->id_task_matter_service,	// 任务相关的服务ID: 发放任务物品服务
					0,									// 教授技能的服务ID
					pEssence->id_heal_service,			// 治疗的服务ID
					pEssence->id_transmit_service,		// 传送的服务ID
					0,									// 运输的服务ID
					pEssence->id_proxy_service,			// 代售的服务ID
					pEssence->id_storage_service,		// 仓库的服务ID
					0,									// 生产的服务ID
					0,									// 分解的服务ID
					0,									// 鉴定的服务ID
					0,									// Talk return.
					0,									// Talk exit.
					0									// Storage password.
				};

				PopupCorrespondingServiceDialog(id,
					a_uiService[nIndex], m_pLst_Main->GetItemDataPtr(nCurSel));
			}
			else
			{
				PopupCorrespondingServiceDialog(
					id, 0, m_pLst_Main->GetItemDataPtr(nCurSel));
			}
		}
		else					// Window.
		{
			for( i = 0; i < pTalk->num_window; i++ )
			{
				if( id != pTalk->windows[i].id ) continue;

                m_pBtn_Back->SetData(id);
				if(stricmp(GetName(),"Win_NPC_Main") == 0)
				{
				    m_iCurPage = 0;
					m_iWindowIndex = i;
					RefreshTalkContent();
					m_pTxt_Content->SetData(pTalk->windows[i].id_parent);
				}
				else
				{
					ACString strTaskDesc;
					strTaskDesc = FormatTaskText(pTalk->windows[i].talk_text);
					m_pTxt_Content->SetImageList(&m_vecImageList);
					m_pTxt_Content->SetText(strTaskDesc);
					m_pTxt_Content->SetData(pTalk->windows[i].id_parent);
					m_pLst_Main->ResetContent();
					for( j = 0; j < pTalk->windows[i].num_option; j++ )
					{
						strText = GetStringFromTable(249);
						strText += FormatTaskText(pTalk->windows[i].options[j].text, false);
						m_pLst_Main->AddString(strText);
						nIndex = m_pLst_Main->GetCount() - 1;
						m_pLst_Main->SetItemData(nIndex, pTalk->windows[i].options[j].id);
						m_pLst_Main->SetItemDataPtr(nIndex, &pTalk->windows[i].options[j]);
					}

				}

				break;
			}
		}
	}
	else if(id == CDLGNPC_LOOK_BATTLE_INFO)
	{
		c2s_SendLookBattleInfo(58);
		GetGameUIMan()->EndNPCService();
		Show(false);
		return;	
	}
	else if( id == CDLGNPC_ENTER_BATTLE1)
	{
		GetGameSession()->battle_join(GetGameRun()->GetCurLine(), 58, 1, 1);
		GetGameUIMan()->EndNPCService();
		Show(false);
		((CDlgBattleInfo*)GetGameUIMan()->GetDialog("Win_BattleList"))->SetJoinType(1);
		return;
	}
	else if( id == CDLGNPC_ENTER_BATTLE2)
	{
		GetGameSession()->battle_join(GetGameRun()->GetCurLine(), 58, 2, 1);
		GetGameUIMan()->EndNPCService();
		((CDlgBattleInfo*)GetGameUIMan()->GetDialog("Win_BattleList"))->SetJoinType(2);
		Show(false);
		return;
	}
	else if( GetData() == NPC_DIALOG_TRANSMIT_LIST )
	{
		if( m_pLst_Main->GetItemData(nCurSel) == 0 )
		{
			GetGameSession()->c2s_CmdNPCSevTransmit(nCurSel);
			OnCommand_CANCEL("");
			LuaEvent::FirstTransmit();
		}
		else if( m_pLst_Main->GetItemData(nCurSel) == 1 )
			GetGameUIMan()->MessageBox("", GetStringFromTable(806),
				MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		else if( m_pLst_Main->GetItemData(nCurSel) == 2 )
			GetGameUIMan()->MessageBox("", GetStringFromTable(807),
				MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	}
	else if( GetData() == NPC_DIALOG_TASK_LIST )
	{
		if( m_pLst_Main->GetItemData(nCurSel) == (DWORD)-1 )
		{
			CDlgTaskList *pTaskList = (CDlgTaskList*)GetGameUIMan()->GetDialog("Win_QuestList");
			pTaskList->Show(true);
			Show(false);
			return;
		}
		else if( nCurSel == m_pLst_Main->GetCount() - 1 )
		{
			GetGameUIMan()->EndNPCService();
			Show(false);
		}
		else
		{
			talk_proc *pTalk = (talk_proc *)m_pLst_Main->GetItemDataPtr(nCurSel);
			if (nCurSel==0)
			{
				GetGameRun()->GetHelpGraphGuide()->FinishHelp(CECHelpGraphGuide::HELP2012_3_2_FIRSTTASK);

			}
			
			ATaskTempl *pTemp = NULL;
			int idTask = m_pLst_Main->GetItemData(nCurSel,1);
			ATaskTemplMan *pTempMan = g_pGame->GetTaskTemplateMan();
			if(idTask > 0)
			{
			   pTemp = pTempMan->GetTaskTemplByID(idTask);
			}
            //主线任务
			if(pTemp && pTemp->IsKeyTask())
			{
				this->Show(false);
			    GetGameUIMan()->PopupNpcMainDialog(pTalk);
			    GetGameUIMan()->m_pDlgNpcMain->m_pTxt_Content->SetData(0xFFFFFFFF);
				GetGameUIMan()->m_pDlgNpcMain->SetData(NPC_DIALOG_TASK_TALK);
			}
			else
			{
				GetGameUIMan()->PopupNPCDialog(pTalk);
				m_pTxt_Content->SetData(0xFFFFFFFF);
				SetData(NPC_DIALOG_TASK_TALK);
			}
			NewHelpGuide::TriggerClickNpcListItem(nCurSel);
		}
	}
	else if( GetData() == NPC_DIALOG_ESSENCE )
	{
		if( nCurSel == m_pLst_Main->GetCount() - 1 )
		{
			GetGameUIMan()->EndNPCService();
			Show(false);
			return;
		}

		int iService = (int)m_pLst_Main->GetItemData(nCurSel);
		NPC_ESSENCE *pEssence = GetGameUIMan()->m_pCurNPCEssence;
		if( pEssence )
		{
			if (iService == CDLGNPC_TRANSCRIPTION && pEssence->id_transcription>0)
			{
				PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Battle_DungeonApply");
				pDlg->SetData(pEssence->id_transcription, "id");
				pDlg->Show(true);
				Show(false);
				return;
			}
			if (iService == CDLGNPC_TRANSCRIPTION_TOWER && pEssence->id_transcription>0)
			{
				PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Battle_Dungeon_Tower");
				pDlg->SetData(pEssence->id_transcription, "id");
				pDlg->Show(true);
				Show(false);
				return;
			}
			if(iService == CDLGNPC_FACTIONPKRAID_APPLY && pEssence->id_transcription == CDlgBattleFactionWarControl::GetFactionPKRaidTemplateId())
			{
				CDlgBattleFactionWarApply* pDlgBFWApply = (CDlgBattleFactionWarApply*)(GetGameUIMan()->GetDialog("Win_Battle_GuildWar_Apply"));
				if(pDlgBFWApply->IsShow()) return;
				if(!(pDlgBFWApply->CheckApplyCondition())) return;
				pDlgBFWApply->SetData(1,"openstyle");
				pDlgBFWApply->Show(true);
				Show(false);
			    return;  
			}
			if(iService == CDLGNPC_FACTIONPKRAID_MAIN && pEssence->id_transcription == CDlgBattleFactionWarControl::GetFactionPKRaidTemplateId())
			{
				CDlgBattleFactionWarControl* pDlgBFWControl = (CDlgBattleFactionWarControl*)(GetGameUIMan()->GetDialog("Win_Battle_GuildWar_Applylist"));
				if(pDlgBFWControl->IsShow()) return;
				if(!(pDlgBFWControl->CheckFactionWarState())) return;
				pDlgBFWControl->GetFactionWarInfo();
				pDlgBFWControl->SetData(1,"openstyle");
				Show(false);
			    return;
			}
			if(iService == CDLGNPC_FACTIONPKRAID_WATCH && pEssence->id_transcription == CDlgBattleFactionWarControl::GetFactionPKRaidTemplateId())
			{
				CDlgBattleFactionWarWatch* pDlgBFWatch = (CDlgBattleFactionWarWatch*)(GetGameUIMan()->GetDialog("Win_Battle_GuildWarList"));
				if(pDlgBFWatch->IsShow()) return;
				pDlgBFWatch->GetFactionWarList();
				Show(false);
			    return;
			}

			if( iService == CDLGNPC_HORSETAME && pEssence->combined_services & 0x1 )
			{
				GetGameUIMan()->m_pDlgInventory->Show(true);
				GetGameUIMan()->m_pDlgHorseTame->Show(true);
				GetGameUIMan()->EndNPCService();
				Show(false);
				return;
			}
			else if( iService == CDLGNPC_INTERFIGHT1 && pEssence->combined_services & 0x2 )
			{
				Show(false);
				((CDlgBattleInfo*)GetGameUIMan()->GetDialog("Win_BattleList"))->SetJoinType(1);
				GetGameSession()->battle_getlist(BATTLE_TYPE_NAMED_CRSSVR);
				GetGameUIMan()->EndNPCService();
				return;
			}
			else if( iService == CDLGNPC_LOCK && pEssence->combined_services & 0x4 )
			{
				Show(false);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				GetGameUIMan()->GetDialog("Win_ItemLock")->Show(true);
				return;
			}
			else if( iService == CDLGNPC_UNLOCK && pEssence->combined_services & 0x4 )
			{
				Show(false);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				GetGameUIMan()->GetDialog("Win_ItemUnlock")->Show(true);
				return;
			}

			else if( pEssence->combined_services & 0x10
				&& (iService == CDLGNPC_FACTION_CREATE
				|| iService == CDLGNPC_FAMILY_CREATE
				|| iService == CDLGNPC_FACTION_UPGRADE
				|| iService == CDLGNPC_FACTION_DISMISS
				|| iService == CDLGNPC_FAMILY_DISMISS) )
			{
				if( iService == CDLGNPC_FAMILY_CREATE )
				{
					if( GetHostPlayer()->GetBasicProps().iLevel < 25 )
					{
						GetGameUIMan()->MessageBox("", GetStringFromTable(854),
							MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
					}
					else if( GetHostPlayer()->GetMoneyAmount() < 50000 )
					{
						GetGameUIMan()->MessageBox("", GetStringFromTable(863),
							MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
					}
					else
					{
						GetGameUIMan()->m_pDlgFamilyCreate->Show(true);
						LuaEvent::FirstCreateFamily();
					}
				}
				else if( iService == CDLGNPC_FACTION_CREATE )
				{
					if( GetHostPlayer()->GetFRoleID() != GNET::_R_HEADER ||
						GetHostPlayer()->GetFactionID() != 0 ||
						GetHostPlayer()->GetBasicProps().iLevel < 35)
					{
						GetGameUIMan()->MessageBox("", GetStringFromTable(607),
							MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
					}
					else if( GetHostPlayer()->GetMoneyAmount() < 300000 )
					{
						GetGameUIMan()->MessageBox("", GetStringFromTable(864),
							MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
					}
					else
					{
						GetGameUIMan()->m_pDlgGuildCreate->Show(true);
						LuaEvent::FirstCreateFaction();
					}
				}
				else if( iService == CDLGNPC_FACTION_UPGRADE )
				{
					int idFaction = GetHostPlayer()->GetFactionID();
					CECFactionMan *pFMan = GetGame()->GetFactionMan();
					Faction_Info *pInfo = pFMan->GetFaction(idFaction);

					if( pInfo )
					{
						ACString strText;
						if( pInfo->GetProsperity() >= g_nProsperityRequire[pInfo->GetLevel()] )
						{
							strText.Format(GetStringFromTable(608), pInfo->GetLevel() + 2,
								pInfo->GetLevel() * 2 + 7,
								pInfo->GetLevel() + 2, g_nProsperityRequire[pInfo->GetLevel()]);
							GetGameUIMan()->MessageBox(
								"Faction_Upgrade", strText,
								MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160));
						}
						else
						{
							strText.Format(GetStringFromTable(609), pInfo->GetLevel() + 2,
								pInfo->GetLevel() * 2 + 7,
								pInfo->GetLevel() + 2, g_nProsperityRequire[pInfo->GetLevel()]);
							GetGameUIMan()->MessageBox("", strText,
								MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
						}
					}
				}
				else if( iService == CDLGNPC_FACTION_DISMISS )
				{
					GetGameUIMan()->MessageBox("Faction_Dismiss", GetStringFromTable(596),
						MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160));
				}
				else if( iService == CDLGNPC_FAMILY_DISMISS )
				{
					GetGameUIMan()->MessageBox("Family_Dismiss", GetStringFromTable(860),
						MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160));
				}
				GetGameUIMan()->EndNPCService();
				Show(false);
				return;
			}
			else if( iService == CDLGNPC_MEND && pEssence->combined_services & 0x20 )
			{
				Show(false);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				GetGameUIMan()->m_pDlgMend->Show(true);
				return;
			}
			else if( iService == CDLGNPC_MAIL && pEssence->combined_services & 0x40 )
			{
				Show(false);
				GetGameUIMan()->m_pDlgMailList->Show(true);
				return;
			}
// 			else if( iService == CDLGNPC_AUCTION && pEssence->combined_services & 0x80)
// 			{
// 				Show(false);
// 				GetGameSession()->auction_AttendList();
// 				GetGameUIMan()->m_pDlgAuctionBuyList->Show(true);
// 				GetGameUIMan()->m_pDlgAuctionBuyList->m_nIDNext = 0;
// 				GetGameUIMan()->m_pDlgAuctionBuyList->m_nPageNext = 0;
// 				ACString szText;
// 				szText.Format(GetStringFromTable(3501), GetStringFromTable(3509));
// 				GetGameUIMan()->m_pDlgAuctionBuyList->m_pTxt_RefreshStatus->SetText(szText);
// 				GetGameUIMan()->m_pDlgAuctionBuyList->m_pTxt_Search->
// 					SetText(GetStringFromTable(3509));
// 				return;
// 			}
			else if( iService == CDLGNPC_DOUBLEEXP && pEssence->combined_services & 0x100)
			{
				Show(false);
				GetGameUIMan()->m_pDlgDoubleExp->Show(true);
				return;
			}
			else if( iService == CDLGNPC_BATTLEINFO && pEssence->combined_services & 0x800 )
			{
				Show(false);
				if (GetHostPlayer()->GetRebornInfo().size()==0)
				{
					GetGameSession()->battle_getlist(BATTLE_TYPE_NORMAL);
					GetGameUIMan()->GetDialog("Win_BattleList")->SetData(6);
				}
				return;
			}
			else if( iService == CDLGNPC_BATTLELEAVE && pEssence->combined_services & 0x1000 )
			{
				Show(false);
				GetGameUIMan()->MessageBox("Game_BattleLeave", GetStringFromTable(725),
					MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160));
				return;
			}
			else if( iService == CDLGNPC_GOLDTRADE && pEssence->combined_services & 0x2000 )
			{
				Show(false);
				GetGameUIMan()->m_pDlgGoldTrade->Show(true);
				return;
			}
			else if( iService == CDLGNPC_GUILDSTORAGE && pEssence->combined_services & 0x4000 )
			{
				if( GetHostPlayer()->TrashBoxHasPsw() )
				{
					PAUIDIALOG pShow1 = m_pAUIManager->GetDialog("Win_InputString");
					pShow1->GetDlgItem("DEFAULT_Txt_Input")->SetText(_AL(""));
					pShow1->SetData(0);
					pShow1->Show(true, false);
					((CDlgStoragePW*)pShow1)->SetFaction(true);
				}
				else
				{
					g_pGame->GetGameSession()->c2s_CmdNPCSevOpenTrash("", true);
					GetGameUIMan()->EndNPCService();
				}
				Show(false);
				return;
			}
			else if( iService == CDLGNPC_TALISMAN_CLEAN && pEssence->combined_services & 0x8000 )
			{
				Show(false);
				GetGameUIMan()->m_pDlgTalismanClean->Show(true);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				return;
			}
			else if( iService == CDLGNPC_TALISMAN_COMBINE && pEssence->combined_services & 0x8000 )
			{
				Show(false);
				GetGameUIMan()->m_pDlgTalismanCombine->Show(true);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				return;
			}
			else if( iService == CDLGNPC_TALISMAN_REFINE && pEssence->combined_services & 0x8000 )
			{
				Show(false);
				GetGameUIMan()->m_pDlgTalismanRefine->Show(true);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				return;
			}
			else if( iService == CDLGNPC_TALISMAN_UPGRADE && pEssence->combined_services & 0x8000 )
			{
				Show(false);
				GetGameUIMan()->m_pDlgTalismanUpgrade->Show(true);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				return;
			}
			else if( iService == CDLGNPC_TALISMAN_ENCHANT && pEssence->combined_services & 0x8000 )
			{
				Show(false);
				GetGameUIMan()->m_pDlgTalismanEnchanting->Show(true);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				return;
			}
			else if( iService == CDLGNPC_SIEGE_INFO && pEssence->combined_services & 0x10000 )
			{
				Show(false);
				((CDlgSiege*)m_pAUIManager->GetDialog("Win_SiegeInfo"))->OnWarInfoGetService();
				GetGameUIMan()->EndNPCService();
				return;
			}
			else if( iService == CDLGNPC_SIEGE_ENTER && pEssence->combined_services & 0x20000 )
			{
				Show(false);
				((CDlgSiege*)m_pAUIManager->GetDialog("Win_SiegeInfo"))->OnWarEnterService();				
				GetGameUIMan()->EndNPCService();
				return;
			}
			else if( iService == CDLGNPC_SIEGE_CONSTRUCTION && pEssence->combined_services & 0x40000 )
			{
				Show(false);
				((CDlgCityBuilding*)m_pAUIManager->GetDialog("Win_CityBuilding"))->OnCityBuildService();
				GetGameUIMan()->EndNPCService();
				return;
			}
			else if( iService == CDLGNPC_PETADOPT && pEssence->combined_services & 0x80000 )
			{
				Show(false);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				GetGameUIMan()->GetDialog("Win_PetRaise")->Show(true);
				return;
			}
			else if( iService == CDLGNPC_PETFREE && pEssence->combined_services & 0x100000 )
			{
				Show(false);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				GetGameUIMan()->GetDialog("Win_PetRelease")->Show(true);
				return;
			}
			else if( iService == CDLGNPC_PETCOMBINE && pEssence->combined_services & 0x200000 )
			{
				Show(false);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				GetGameUIMan()->GetDialog("Win_PetCombine")->Show(true);
				return;
			}
			else if( iService == CDLGNPC_PETRENAME && pEssence->combined_services & 0x400000 )
			{
				Show(false);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				GetGameUIMan()->GetDialog("Win_PetChangeName")->Show(true);
				LuaEvent::FirstChangePetName();
				return;
			}
			else if( iService == CDLGNPC_SOUL_OWN && pEssence->combined_services & 0x800000 )
			{
				Show(false);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				GetGameUIMan()->GetDialog("Win_EquipOwn")->Show(true);
				return;
			}
			else if( iService == CDLGNPC_SOUL_ENCHASE && pEssence->combined_services & 0x1000000 )
			{
				Show(false);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				GetGameUIMan()->GetDialog("Win_EquipEnchase")->Show(true);
				return;
			}
			else if( iService == CDLGNPC_SOUL_SPILT && pEssence->combined_services & 0x2000000 )
			{
				Show(false);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				GetGameUIMan()->GetDialog("Win_Spilt")->Show(true);
				return;
			}
			else if( iService == CDLGNPC_SOUL_RESUME && pEssence->combined_services & 0x4000000 )
			{
				Show(false);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				GetGameUIMan()->GetDialog("Win_EquipResume")->Show(true);
				return;
			}
			else if( iService == CDLGNPC_SOUL_DEPART && pEssence->combined_services & 0x8000000 )
			{
				Show(false);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				GetGameUIMan()->GetDialog("Win_EquipDepart")->Show(true);
				return;
			}
			else if( iService == CDLGNPC_BATTLEINFOREBORN && pEssence->combined_services & 0x10000000 )
			{
				Show(false);
				if (GetHostPlayer()->GetRebornInfo().size()>0)
				{
					GetGameSession()->battle_getlist(BATTLE_TYPE_REBORN);
					GetGameUIMan()->GetDialog("Win_BattleList2")->SetData(12);
				}
				return;
			}
			else if( iService == CDLGNPC_SJBATTLE && pEssence->combined_services & 0x20000000 )
			{
				Show(false);
				if (GetHostPlayer()->GetRebornInfo().size()==0)
					GetGameSession()->battle_getlist(BATTLE_TYPE_SONJIN);
				else
					GetGameSession()->battle_getlist(BATTLE_TYPE_SONJINREBORN);
				GetGameUIMan()->GetDialog("Win_BattleSong")->Show(true);
				
				return;
			}
			else if( iService == CDLGNPC_ARENA && pEssence->combined_services & 0x40000000 )
			{
				Show(false);
				if (GetHostPlayer()->GetRebornInfo().size()==0)
					GetGameSession()->battle_getlist(BATTLE_TYPE_ARENA);
				else
					GetGameSession()->battle_getlist(BATTLE_TYPE_ARENAREBORN);
				GetGameUIMan()->GetDialog("Win_BattleListPK")->Show(true);
				
				return;
			}
			else if( iService == CDLGNPC_ONLINE_SPREAD_GETID && pEssence->combined_services & 0x80000000 )
			{
				Show(false);
				if (GetHostPlayer()->GetRebornInfo().size()==0 && GetHostPlayer()->GetBasicProps().iLevel<60)
				{
					GetGameUIMan()->MessageBox("", GetStringFromTable(7609),
					MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
					GetGameUIMan()->EndNPCService();
				}
				else
				{
					CDlgSpread *pSpreadId = (CDlgSpread*)GetGameUIMan()->GetDialog("Win_PopIdCheck");
					pSpreadId->Show(true);
				}
				
				return;
			}
			else if( iService == CDLGNPC_ONLINE_SPREAD_GETLINK && pEssence->combined_services & 0x80000000 )
			{
				Show(false);
				char szText[1024];
				const ACHAR *szRefCode = GetHostPlayer()->GetSelfReferenceCode();
				if (szRefCode)
				{
					sprintf(szText, AC2AS(GetStringFromTable(9172)),
						AC2AS(szRefCode),
						GetGameUIMan()->URLEncode(GetHostPlayer()->GetName()),
						GetGameSession()->GetZoneID());
					g_pGame->OpenURL(szText); 
				}
				GetGameUIMan()->EndNPCService();
				
				return;
			}
			else if( iService == CDLGNPC_ONLINE_SPREAD && pEssence->combined_services & 0x80000000 )
			{
				Show(false);
				GetGameUIMan()->GetDialog("Win_PopProfit")->Show(true);
				
				return;
			}
			else if( iService == CDLGNPC_FACEOFF && pEssence->combined_services2 & 0x1 )
			{
				Show(false);
				GetGameUIMan()->GetDialog("Win_ImageChange")->Show(true);
				
				return;
			}
			else if( iService == CDLGNPC_PETEQUIP_ENHANCE && pEssence->combined_services2 & 0x2 )
			{
				Show(false);
				GetGameUIMan()->GetDialog("Win_PetEquipEnchanse")->Show(true);
				
				return;
			}
			else if( iService == CDLGNPC_INTERFIGHT2 && pEssence->combined_services2 & 0x4 )
			{
				Show(false);
				((CDlgBattleInfo*)GetGameUIMan()->GetDialog("Win_BattleList"))->SetJoinType(2);
				GetGameSession()->battle_getlist(BATTLE_TYPE_NAMED_CRSSVR);
				//GetGameSession()->battle_join(GetGameRun()->GetCurLine(), 20, 2);
				GetGameUIMan()->EndNPCService();
				
				return;
			}
			else if( iService == CDLGNPC_INTERFIGHTWATCH && pEssence->combined_services2 & 0x8 )
			{
				Show(false);
				((CDlgBattleInfo*)GetGameUIMan()->GetDialog("Win_BattleList"))->SetJoinType(3);
				GetGameSession()->battle_getlist(BATTLE_TYPE_NAMED_CRSSVR);
				//GetGameSession()->battle_join(GetGameRun()->GetCurLine(), 20, 3);
				GetGameUIMan()->EndNPCService();
				
				return;
			}
			else if( iService == CDLGNPC_SCENEBATTLE && pEssence->combined_services2 & 0x10 )
			{
				Show(false);
				GetGameSession()->instancing_availablelist();
				
				return;
			}
			else if( (iService >= CDLGNPC_BODYCHANGE_ENCHASE0)	&&
					(iService <= CDLGNPC_BODYCHANGE_ENCHASE2)	&&
					pEssence->combined_services2 & 0x20 )
			{
				Show(false);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				GetGameUIMan()->GetDialog("Win_BodyChange_Enchanting")->SetData(iService-CDLGNPC_BODYCHANGE_ENCHASE0);
				GetGameUIMan()->GetDialog("Win_BodyChange_Enchanting")->Show(true);
				
				return;
			}
			else if( iService == CDLGNPC_BODYCHANGE_RECOVER && pEssence->combined_services2 & 0x40 )
			{
				Show(false);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				GetGameUIMan()->GetDialog("Win_BodyChange_Recover")->Show(true);
				
				return;
			}
			else if( iService == CDLGNPC_TERRWAR_ANNO && pEssence->combined_services2 & 0x80 )
			{
				Show(false);
				GetGameUIMan()->GetDialog("Win_WorldMap")->Show(true);
				PAUISUBDIALOG pSub = (PAUISUBDIALOG)GetGameUIMan()->GetDialog("Win_WorldMap")->GetDlgItem("Sub_Worldmap");
				pSub->SetDialog(GetGameUIMan()->GetDialog("WorldMap_TerriSub"));
				CDlgTerrWarMap::SetMOpen(false);
				GetGameUIMan()->EndNPCService();
				
				return;
			}
			else if( iService == CDLGNPC_TERRWAR_ENTER && pEssence->combined_services2 & 0x100 )
			{
				Show(false);
				CDlgTerrWarMap* pDlgTerrMap = (CDlgTerrWarMap*)GetGameUIMan()->GetDialog("WorldMap_TerriSub");
				int iStatus = pDlgTerrMap->GetStatus();

				vector<DOMAIN_INFO> *info = globaldata_getdomaininfos2();
				for(UINT i = 0; i < info->size(); i++ )
				{
					DOMAIN_INFO di = (*info)[i];
					if (di.id == pEssence->id_territory)
					{
						if (di.flag!=0)
						{
							GetGameUIMan()->EndNPCService();
							GetGameUIMan()->MessageBox(9389);
							return;
						}
						int idFaction = GetHostPlayer()->GetFactionID();
						if (idFaction>0 && (idFaction==di.id_attacker||idFaction==di.id_defender))
						{
							ACString strText;
							strText.Format(GetGameUIMan()->GetStringFromTable(9340), GetGameUIMan()->GetStringFromTable(9333+di.type), di.name);
							PAUIDIALOG pDlg;
							GetGameUIMan()->MessageBox("Game_BattleTerrEnter", strText, MB_YESNO, A3DCOLORRGBA(255,255,255,166), &pDlg);
							pDlg->SetData(GetHostPlayer()->GetFactionID());
							pDlg->SetDataPtr((void*)di.id);
							GetGameUIMan()->EndNPCService();
							return;
						}
						else
						{
							int factId1 = 0;
							int factId2 = 0;
							CECInventory *pInv = GetHostPlayer()->GetPack();
							for (int i=0; i<pInv->GetSize(); i++)
							{
								CECIvtrItem* pItem = pInv->GetItem(i);
								if (pItem && pItem->GetClassID()==CECIvtrItem::ICID_MERCENARY_CREDENTIAL)
								{
									CECIvtrMercenaryCredential* pMc = (CECIvtrMercenaryCredential*)pItem;
									if (pMc->GetEssence().territory_id == di.id || pMc->GetEssence().territory_id == 0)
									{
										if (pMc->GetEssence().faction_id == di.id_defender)
											factId1 = di.id_defender;
										else if (pMc->GetEssence().faction_id == di.id_attacker)
											factId2 = di.id_attacker;
									}
								}
							}
							if (factId1>0 && factId2>0)
							{
								CECFactionMan *pFMan = GetGame()->GetFactionMan();
								Faction_Info* pFInfo1 = pFMan->GetFaction(factId1, true);
								Faction_Info* pFInfo2 = pFMan->GetFaction(factId2, true);
								GetGameUIMan()->GetDialog("Win_Battle_TerriHireChoice")->GetDlgItem("Btn_Guild1")->SetData(factId1);
								GetGameUIMan()->GetDialog("Win_Battle_TerriHireChoice")->GetDlgItem("Btn_Guild1")->SetText(pFInfo1 ? pFInfo1->GetName() : _AL("Faction1"));
								GetGameUIMan()->GetDialog("Win_Battle_TerriHireChoice")->GetDlgItem("Btn_Guild2")->SetData(factId2);
								GetGameUIMan()->GetDialog("Win_Battle_TerriHireChoice")->GetDlgItem("Btn_Guild2")->SetText(pFInfo2 ? pFInfo2->GetName() : _AL("Faction2"));
								GetGameUIMan()->GetDialog("Win_Battle_TerriHireChoice")->SetData(di.id);
								GetGameUIMan()->GetDialog("Win_Battle_TerriHireChoice")->Show(true);
								GetGameUIMan()->EndNPCService();
								return;
							}
							else if (factId1>0 || factId2>0)
							{
								int idFact = factId1>0 ? factId1 : factId2;
								CECFactionMan *pFMan = GetGame()->GetFactionMan();
								Faction_Info *pInfo = pFMan->GetFaction(idFact, true);
								ACString strText;
								strText.Format(GetGameUIMan()->GetStringFromTable(9348), pInfo ? pInfo->GetName() : _AL("?"), di.name);
								PAUIDIALOG pDlg;
								GetGameUIMan()->MessageBox("Game_BattleTerrEnter", strText, MB_YESNO, A3DCOLORRGBA(255,255,255,166), &pDlg);
								pDlg->SetData(idFact);
								pDlg->SetDataPtr((void*)(pEssence->id_territory|0x80000000));
								GetGameUIMan()->EndNPCService();
								return;
							}
						}
					}
				}
				GetGameUIMan()->AddChatMessage(GetGameUIMan()->GetStringFromTable(729), GP_CHAT_MISC);
				GetGameUIMan()->EndNPCService();
				return;
			}
			else if( iService == CDLGNPC_TERRWAR_AWARD && pEssence->combined_services2 & 0x200 )
			{
				Show(false);
				CDlgTerrWarMap* pDlgTerrMap = (CDlgTerrWarMap*)GetGameUIMan()->GetDialog("WorldMap_TerriSub");
				pDlgTerrMap->GetAward(pEssence->id_territory);
				GetGameUIMan()->EndNPCService();
				return;
			}
			else if( iService == CDLGNPC_TERRWAR_AWARD_SHENQI && pEssence->combined_services2 & 0x200 )
			{
				Show(false);
				int idFaction = GetHostPlayer()->GetFactionID();
				vector<DOMAIN_INFO> *info = globaldata_getdomaininfos2();
				for(UINT i = 0; i < info->size(); i++ )
				{
					DOMAIN_INFO di = (*info)[i];
					if (di.id_owner == idFaction && di.random_award > 0)
					{
						GetGameSession()->c2s_CmdNPCSevTerritoryAward(idFaction, di.id, 4, di.random_award, di.random_award_num, 0);
						break;
					}
				}
				GetGameUIMan()->EndNPCService();
				return;
			}
			else if( iService == CDLGNPC_MARKASTRO && pEssence->combined_services2 & 0x400)
			{
				Show(false);
				GetGameUIMan()->GetDialog("Win_MarkAstro")->Show(true);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				return;
			}
			else if( iService == CDLGNPC_MENDMONEY && pEssence->combined_services2 & 0x800)
			{
				Show(false);
				GetGameUIMan()->GetDialog("Win_MendMoney")->Show(true);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				return;
			}
			else if( iService == CDLGNPC_EQUIP_LVUP && pEssence->combined_services2 & 0x1000)
			{
				Show(false);
				GetGameUIMan()->GetDialog("Win_EquipLvup")->Show(true);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				return;
			}
			else if( iService == CDLGNPC_TRANSPORT_CROSS && pEssence->combined_services2 & 0x2000)
			{
				const ROLEBASICPROP &rbp = GetHostPlayer()->GetBasicProps();
				if (GetHostPlayer()->IsGM()||rbp.iDeityLevel>0)
				{
					GetGameRun()->SaveConfigsToServer();
					c2s_SendCmdNPCSevTransmitToCross();
					//GetGameUIMan()->ChangeGs(-12345);
				}
				else
					GetGameUIMan()->MessageBox(1058);

				GetGameUIMan()->EndNPCService();
				Show(false);
				return;
			}
			else if( iService == CDLGNPC_TRANSPORT_ORIG && pEssence->combined_services2 & 0x4000)
			{
				GetGameRun()->SaveConfigsToServer();
				c2s_SendCmdNPCSevTransmitToNormal();
				//GetGameUIMan()->ChangeGs(-12346);
				GetGameUIMan()->EndNPCService();
				Show(false);
				return;
			}
			else if( iService == CDLGNPC_BATTLE_CHALLENGE && pEssence->combined_services2 & 0x8000)
			{
				Show(false);
				if (GetHostPlayer()->GetRebornInfo().size()>0)
				{
					GetGameSession()->battle_getlist(BATTLE_TYPE_CHALLENGE);
					GetGameUIMan()->GetDialog("Win_BattleList2")->SetData(6);
				}
				else
					GetGameUIMan()->EndNPCService();
				return;
			}
			else if( iService == CDLGNPC_JACK_DRILL && pEssence->combined_services2 & 0x10000)
			{
				Show(false);
				GetGameUIMan()->GetDialog("Win_Accessory_Drill")->Show(true);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				return;
			}
			else if( iService == CDLGNPC_JACK_REDRILL && pEssence->combined_services2 & 0x20000)
			{
				Show(false);
				GetGameUIMan()->GetDialog("Win_Accessory_ReDrill")->Show(true);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				return;
			}
			else if( iService == CDLGNPC_JACK_CUSTOM && pEssence->combined_services2 & 0x40000)
			{
				Show(false);
				GetGameUIMan()->GetDialog("Win_Accessory_Custom")->Show(true);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				return;
			}
			else if( iService == CDLGNPC_BIJOU_ATTACH && pEssence->combined_services2 & 0x80000)
			{
				Show(false);
				GetGameUIMan()->GetDialog("Win_Accessory_Attach")->Show(true);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				return;
			}
			else if( iService == CDLGNPC_BIJOU_DETACH && pEssence->combined_services2 & 0x100000)
			{
				Show(false);
				GetGameUIMan()->GetDialog("Win_Accessory_Detach")->Show(true);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				return;
			}
			else if( iService == CDLGNPC_BIJOU_UPGRADE && pEssence->combined_services2 & 0x200000)
			{
				Show(false);
				GetGameUIMan()->GetDialog("Win_Accessory_Upgrade")->Show(true);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				return;
			}
			else if( iService == CDLGNPC_BIJOU_REFINE && pEssence->combined_services2 & 0x400000)
			{
				Show(false);
				GetGameUIMan()->GetDialog("Win_Accessory_Refine")->Show(true);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				return;
			}
			else if( iService == CDLGNPC_BIJOU_EXTRACT && pEssence->combined_services2 & 0x800000)
			{
				Show(false);
				GetGameUIMan()->GetDialog("Win_Accessory_Spilt")->Show(true);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				return;
			}
			else if( iService == CDLGNPC_BIJOU_TRANSFER && pEssence->combined_services2 & 0x1000000)
			{
				Show(false);
				GetGameUIMan()->GetDialog("Win_Accessory_Move")->Show(true);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				return;
			}
			else if( iService == CDLGNPC_KF2011_SIDERED && pEssence->combined_services2 & 0x2000000)
			{
				Show(false);
				((CDlgBattleInfo*)GetGameUIMan()->GetDialog("Win_BattleList"))->SetJoinType(1);
				GetGameSession()->battle_getlist(BATTLE_TYPE_CRSSVRTEAM);
				GetGameUIMan()->EndNPCService();
				return;
			}
			else if( iService == CDLGNPC_KF2011_SIDEBLUE && pEssence->combined_services2 & 0x4000000)
			{
				Show(false);
				((CDlgBattleInfo*)GetGameUIMan()->GetDialog("Win_BattleList"))->SetJoinType(2);
				GetGameSession()->battle_getlist(BATTLE_TYPE_CRSSVRTEAM);
				GetGameUIMan()->EndNPCService();
				return;
			}
			else if( iService == CDLGNPC_KF2011_WATCH && pEssence->combined_services2 & 0x8000000)
			{
				Show(false);
				((CDlgBattleInfo*)GetGameUIMan()->GetDialog("Win_BattleList"))->SetJoinType(3);
				GetGameSession()->battle_getlist(BATTLE_TYPE_CRSSVRTEAM);
				GetGameUIMan()->EndNPCService();
				return;
			}
			else if( iService == CDLGNPC_TOWER_INFO && pEssence->combined_services2 & 0x20000000)
			{
				GetGameUIMan()->GetDialog("Win_CharDungeon")->Show(true);
				Show(false);
				GetGameUIMan()->EndNPCService();
				return;
			}
			else if( iService == CDLGNPC_CHANGE_NAME && pEssence->combined_services2 & 0x40000000)
			{
				GetGameUIMan()->GetDialog("Win_Char_ChangeName")->Show(true);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				Show(false);
				return;
			}
			else if( iService == CDLGNPC_NAME_SEARCH && pEssence->combined_services2 & 0x40000000)
			{
				GetGameUIMan()->GetDialog("Win_Char_ChangeName_History")->Show(true);
				Show(false);
				GetGameUIMan()->EndNPCService();
				return;
			}
			else if( iService == CDLGNPC_CHANGE_NAME_FAMILY && pEssence->combined_services2 & 0x80000000)
			{
				GetGameUIMan()->GetDialog("Win_Guild_ChangeName")->SetData(1, "is_family");
				GetGameUIMan()->GetDialog("Win_Guild_ChangeName")->Show(true);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				Show(false);
				return;
			}
			else if( iService == CDLGNPC_CHANGE_NAME_GUILD && pEssence->combined_services3 & 0x1)
			{
				GetGameUIMan()->GetDialog("Win_Guild_ChangeName")->SetData(0, "is_family");
				GetGameUIMan()->GetDialog("Win_Guild_ChangeName")->Show(true);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				Show(false);
				return;
			}
			else if( iService == CDLGNPC_TALISMAN_LEVELUP && pEssence->combined_services3 & 0x2)
			{
				Show(false);
				GetGameUIMan()->GetDialog("Win_TalismanLevelUp")->Show(true);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				return;
			}
			else if( iService == CDLGNPC_TALISMAN_SKILL_REFINE && pEssence->combined_services3 & 0x4)
			{
				Show(false);
				GetGameUIMan()->GetDialog("Win_TalismanCombineSkill")->Show(true);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				return;
			}
			else if( iService == CDLGNPC_TALISMAN_SKILL_EMBED && pEssence->combined_services3 & 0x8)
			{
				Show(false);
				GetGameUIMan()->GetDialog("Win_TalismanEnchaseSkill")->Show(true);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				return;
			}
			else if( iService == CDLGNPC_EQUIP_SOLT && pEssence->combined_services3 & 0x20)
			{
				Show(false);
				GetGameUIMan()->GetDialog("Win_Equip_Fengshen_Drill")->Show(true);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				return;
			}
			else if( iService == CDLGNPC_ASTROLOGY_INSTALL && pEssence->combined_services3 & 0x40)
			{
				Show(false);
				GetGameUIMan()->GetDialog("Win_Equip_Fengshen_Attach")->Show(true);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				return;
			}
			else if( iService == CDLGNPC_ASTROLOGY_UNINSTALL && pEssence->combined_services3 & 0x80)
			{
				Show(false);
				GetGameUIMan()->GetDialog("Win_Equip_Fengshen_Erase")->Show(true);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				return;
			}
			else if( iService == CDLGNPC_FACTION_TRANSMIT_SRTVE && pEssence->combined_services3 & 0x800)
			{
				Show(false);
				GetGameRun()->SaveConfigsToServer();
				c2s_SendCmdNPCSevTransmitServe();
				GetGameUIMan()->EndNPCService();
				return;
			}
			else if( iService == CDLGNPC_FACTION_BATTLE_LEAVE_TRANSMIT_SRTVE && pEssence->combined_services3 & 0x400)// 2012-9-3 add by zy 从战场离开服务
			{
				Show(false);
				GetGameRun()->SaveConfigsToServer();
				c2s_SendCmdNPCSevBattleLeaveTransmitServe();
				GetGameUIMan()->EndNPCService();
				return;
			}
			else if(iService == CDLGNPC_KINGDOM_WAR_MAIN && pEssence->combined_services3 & 0x1000)
			{
                 Show(false);
				 GetGameSession()->c2s_CmdNPCSevKingdomEnter(1);
				 GetGameUIMan()->EndNPCService();
				 return;
			}
		    else if(iService == CDLGNPC_KINGDOM_WAR_VICE && pEssence->combined_services3 & 0x1000)
			{
				Show(false);
				GetGameSession()->c2s_CmdNPCSevKingdomEnter(2);
				GetGameUIMan()->EndNPCService();
				return;
			}
			else if(iService == CDLGNPC_KINGDOM_WAR_BATH && pEssence->combined_services3 & 0x1000)
			{
				Show(false);
				//客户端不对金币做判断
				/*if(g_pGame->GetGameRun()->GetHostPlayer()->GetGender() == GENDER_MALE &&
					g_pGame->GetGameRun()->GetHostPlayer()->GetMoneyAmount() < 1000000)
				{
					 GetGameUIMan()->MessageBox("", GetStringFromTable(15662),
								MB_OK, A3DCOLORRGBA(255, 255, 255, 160));

					 GetGameUIMan()->EndNPCService();
				     return;
				}*/
				
			    GetGameSession()->c2s_CmdNPCSevKingdomEnter(3);
				
				GetGameUIMan()->EndNPCService();
				return;
			}
			else if(iService == CDLGNPC_JEWELRY_LVUP && pEssence->combined_services3 & 0x4000)
			{
				Show(false);
				GetGameUIMan()->GetDialog("Win_Equip_JewelUp")->Show(true);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				return;
			}
			else if(iService == CDLGNPC_SKILL_FASTPRO && pEssence->combined_services3 & 0x8000)
			{
				Show(false);
				CDlgProduce* pDlg = GetGameUIMan()->m_pDlgProduce;
				if( pDlg)
				{
					if( pDlg->IsShow() )
						pDlg->OnCommand_CANCEL("");
					else if (!GetHostPlayer()->IsPlayerMoving() )
					{
						pDlg->SetCanAutoSell(true);
						pDlg->ShowAutoSell(true);
						pDlg->Show(true);
					}					
				}
			}
			else if (iService == CDLGNPC_PRODUCE_JINFASHEN && pEssence->combined_services3 & 0x10000)
			{
				Show(false);
				GetGameUIMan()->GetDialog("Win_QProduce_Tool")->Show(true);
				return;
			}
			else if( CDLGNPC_FLOW_BATTLE == iService && pEssence->combined_services3 & 0x20000)
			{
				Show(false);
				/// 发送请求加入流水席消息
				((DlgBattleFlowInfoApply*)GetGameUIMan()->GetDialog("Win_Battle_SeniorPK_Apply"))->SendFolwInfoApply();

				GetGameUIMan()->EndNPCService();
				return;
			}
			else if(CDLGNPC_CREATE_BUILD == iService && pEssence->combined_services3 & 0x40000)
			{
				Show(false);
				GetGameUIMan()->GetDialog("Win_Guild_LandCreate")->Show(true);
				GetGameUIMan()->EndNPCService();
				return;
			}
			else if(CDLGNPC_ZAIBIAN == iService && pEssence->combined_services3 & 0x100000)
			{
				Show(false);
				GetGameSession()->c2s_CmdZaibian();
			}
			else if(CDLGNPC_GUILD_QSHOP == iService && pEssence->combined_services3 & 0x200000)
			{
				GetGameUIMan()->GetDialog("Win_Guild_Qshop")->Show(true);
				Show(false);
			}
			else if( iService == CDLGNPC_INSTALL && pEssence->service_install )
			{
				Show(false);
				GetGameUIMan()->m_pDlgInstall->Show(true);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				NewHelpGuide::TriggerClickNpcListItem(nCurSel);
				return;
			}
			else if( iService == CDLGNPC_UNINSTALL1 && pEssence->service_uninstall )
			{
				Show(false);
				GetGameUIMan()->m_pDlgUninstall1->Show(true);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				return;
			}
			else if( iService == CDLGNPC_UNINSTALL2 && pEssence->service_uninstall )
			{
				Show(false);
				GetGameUIMan()->m_pDlgUninstall2->Show(true);
				GetGameUIMan()->m_pDlgInventory->Show(true);
				return;
			}
			else if ( iService == CDLGNPC_RESET_SKILL_PART )
			{
				Show(false);
				PAUIDIALOG dlg = m_pAUIManager->GetDialog("Win_ResetSkillPart");
				dlg->Show(true);

				// set resetprop_type to Data.
				dlg->SetData(0);

				m_pAUIManager->GetDialog("Win_Inventory")->Show(true);
				return;
			}
			else if ( iService == CDLGNPC_RESET_SKILL_ALL )
			{
				Show(false);
				m_pAUIManager->GetDialog("Win_ResetSkillAll")->Show(true);
				PAUIDIALOG dlg = m_pAUIManager->GetDialog("Win_ResetSkillAll");
				dlg->Show(true);

				// set resetprop_type to Data.
				dlg->SetData(1);

				m_pAUIManager->GetDialog("Win_Inventory")->Show(true);
				return;
			}
			else if ( iService == CDLGNPC_RESET_TALENT )
			{
				Show(false);
				PAUIDIALOG dlg = m_pAUIManager->GetDialog("Win_ResetTalent");
				dlg->Show(true);

				// set resetprop_type to Data.
				dlg->SetData(2);

				m_pAUIManager->GetDialog("Win_Inventory")->Show(true);
				return;
			}
		}

		int idFunction, id_dialog = 0;
		void *pData = m_pLst_Main->GetItemDataPtr(nCurSel);

		g_pGame->GetElementDataMan()->get_data_ptr(iService, ID_SPACE_ESSENCE, DataType);

		if( DataType == DT_NPC_TASK_IN_SERVICE ||
			DataType == DT_NPC_TASK_OUT_SERVICE ||
			DataType == DT_NPC_TASK_MATTER_SERVICE )
		{
			PopupTaskDialog();
			m_pLst_Main->AddString(GetStringFromTable(503));
			return;
		}
		else if( DataType == DT_NPC_TALK_SERVICE )
		{
			NPC_TALK_SERVICE *pService = (NPC_TALK_SERVICE *)pData;
			id_dialog = pService->id_dialog;
			idFunction = NPC_TALK;
		}
		else if( DataType == DT_NPC_SELL_SERVICE )
		{
			NPC_SELL_SERVICE *pService = (NPC_SELL_SERVICE *)pData;
			id_dialog = pService->id_dialog;
			idFunction = NPC_SELL;
		}
		else if( DataType == DT_NPC_BUY_SERVICE )
		{
			NPC_BUY_SERVICE *pService = (NPC_BUY_SERVICE *)pData;
			id_dialog = pService->id_dialog;
			idFunction = NPC_BUY;
		}
		else if( DataType == DT_ITEM_TRADE_CONFIG )
		{
			ITEM_TRADE_CONFIG *pService = (ITEM_TRADE_CONFIG *)pData;
			id_dialog = pService->id_dialog;
			idFunction = ITEM_TRADE_SERVICE;
		}
		else if( DataType == DT_NPC_HEAL_SERVICE )
		{
			NPC_HEAL_SERVICE *pService = (NPC_HEAL_SERVICE *)pData;
			id_dialog = pService->id_dialog;
			idFunction = NPC_HEAL;
		}
		else if( DataType == DT_NPC_TRANSMIT_SERVICE )
		{
			NPC_TRANSMIT_SERVICE *pService = (NPC_TRANSMIT_SERVICE *)pData;
			idFunction = NPC_TRANSMIT;
		}
		else if( DataType == DT_NPC_PROXY_SERVICE )
		{
			NPC_PROXY_SERVICE *pService = (NPC_PROXY_SERVICE *)pData;
			id_dialog = pService->id_dialog;
			idFunction = NPC_PROXY;
		}
		else if( DataType == DT_NPC_STORAGE_SERVICE )
		{
			NPC_STORAGE_SERVICE *pService = (NPC_STORAGE_SERVICE *)pData;
			ACString strText = m_pLst_Main->GetText(nCurSel);
			ACString strComp = ACString(GetStringFromTable(249)) + ACString(GetStringFromTable(253));

			if( 0 == a_stricmp(strText, strComp) )
				idFunction = NPC_STORAGE_PASSWORD;
			else
				idFunction = NPC_STORAGE;
		}
		else if( DataType == DT_NPC_WAR_TOWERBUILD_SERVICE)
		{
			NPC_WAR_TOWERBUILD_SERVICE *pService = (NPC_WAR_TOWERBUILD_SERVICE *)pData;
			idFunction = NPC_WAR_TOWERBUILD;
		}
		else if( DataType == DT_NPC_RESETPROP_SERVICE)
		{
			NPC_RESETPROP_SERVICE *pService = (NPC_RESETPROP_SERVICE *)pData;
			idFunction = NPC_RESETPROP;
		}
		else if( DataType == DT_NPC_EQUIPBIND_SERVICE )
		{
			NPC_EQUIPBIND_SERVICE *pService = (NPC_EQUIPBIND_SERVICE *)pData;
			idFunction = NPC_EQUIPBIND;
		}
		else if( DataType == DT_NPC_EQUIPDESTROY_SERVICE )
		{
			NPC_EQUIPDESTROY_SERVICE *pService = (NPC_EQUIPDESTROY_SERVICE *)pData;
			idFunction = NPC_EQUIPDESTROY;
		}
		else if( DataType == DT_NPC_EQUIPUNDESTROY_SERVICE )
		{
			NPC_EQUIPUNDESTROY_SERVICE *pService = (NPC_EQUIPUNDESTROY_SERVICE *)pData;
			idFunction = NPC_EQUIPUNDESTROY;
		}
		else if( DataType == DT_NPC_WAR_BUY_ARCHER_SERVICE )
		{
			NPC_WAR_BUY_ARCHER_SERVICE *pService = (NPC_WAR_BUY_ARCHER_SERVICE *)pData;
			idFunction = NPC_WAR_BUY_ARCHER;
		}
		else if( DataType == DT_EQUIP_SOUL_MELD_SERVICE )
		{
			EQUIP_SOUL_MELD_SERVICE *pService = (EQUIP_SOUL_MELD_SERVICE *)pData;
			idFunction = EQUIP_SOUL_MELD;
		}
		else if( DataType == DT_NPC_CONSIGN_SERVICE )
		{
			NPC_CONSIGN_SERVICE *pService = (NPC_CONSIGN_SERVICE *)pData;
			idFunction = NPC_CONSIGN;
		}
		else if( DataType == DT_TRANSCRIPTION_CONFIG )
		{
			TRANSCRIPTION_CONFIG *pService = (TRANSCRIPTION_CONFIG *)pData;
			idFunction = NPC_TRANSCRIPTION;
		}
		else if( DataType == DT_TOWER_TRANSCRIPTION_CONFIG )
		{
			TOWER_TRANSCRIPTION_CONFIG *pService = (TOWER_TRANSCRIPTION_CONFIG *)pData;
			idFunction = NPC_TRANSCRIPTION;
		}
		else if(DataType == DT_NPC_REPUTATION_SHOP_SERVICE)
		{
			NPC_REPUTATION_SHOP_SERVICE* pService = (NPC_REPUTATION_SHOP_SERVICE*)pData;			
			idFunction = 500; 
		}

		if( id_dialog > 0 )
		{
			talk_proc *pTalk = (talk_proc *)g_pGame->GetElementDataMan()
				->get_data_ptr(id_dialog, ID_SPACE_TALK, DataType);

			GetGameUIMan()->PopupNPCDialog(pTalk);
			m_pTxt_Content->SetData(0xFFFFFFFF);
			SetData(NPC_DIALOG_TALK);
		}
		else
		{
			void *pData = m_pLst_Main->GetItemDataPtr(nCurSel);
			PopupCorrespondingServiceDialog(idFunction, iService, pData);
		}
	}
}
bool CDlgNPC::DealWindowsMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, bool bKeyboard)
{
	if(stricmp(GetName(),"Win_NPC_Main") == 0)
	{
		TCHAR ch = g_pGame->GetMappingKeyCode((TCHAR)wParam);	

		if( uMsg == WM_KEYDOWN)
		{
			if(ch == VK_RETURN)
			{
				if(NULL != GetDlgItem("Btn_Next")  &&  GetDlgItem("Btn_Next")->IsShow())
				{
					OnCommand_NextPage("");
					return true;
				}
				else if(m_pLst_Main && m_pLst_Main->GetCount() > 0)
				{
					int nCurSel = m_pLst_Main->GetCurSel();
					if( nCurSel < 0 || nCurSel >= m_pLst_Main->GetCount() )
					{
						m_pLst_Main->SetCurSel(0);
					}
					else
					{
						POINT ptPos = m_pLst_Main->GetPos();
						A3DRECT rect = m_pLst_Main->GetRect();
						int x = ptPos.x + rect.Width()/2;
						int y = ptPos.y + rect.Height()/2;
						OnEventLButtonUp_Lst_Main(0,MAKELPARAM(x,y),m_pLst_Main);
					}
					return true;
				}

			}
			else
			{
				if(m_pLst_Main && m_pLst_Main->GetCount() > 0)
				{
					int nCurSel = m_pLst_Main->GetCurSel();
					if( !(nCurSel < 0 || nCurSel >= m_pLst_Main->GetCount()) )
					{
						if(ch == VK_UP)
						{
							if(nCurSel > 0)
							{
								--nCurSel;
								m_pLst_Main->SetCurSel(nCurSel);
								return true;
							}
						}
						else if(ch == VK_DOWN)
						{
							if(nCurSel < m_pLst_Main->GetCount()-1)
							{
								++nCurSel;
								m_pLst_Main->SetCurSel(nCurSel);
								return true;
							}
						}
					}

				}
			}
			
		}
		
	}
	return CDlgTaskBase::DealWindowsMessage(uMsg,wParam,lParam,bKeyboard);
}