// Filename	: DlgTarget.cpp
// Creator	: Tom Zhou
// Date		: October 12, 2005

#include "DlgTarget.h"
#include "DlgQuickAction.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_ElsePlayer.h"
#include "EC_ManPlayer.h"
#include "EC_ManNPC.h"
#include "EC_Monster.h"
#include "EC_NPC.h"
#include "EC_Team.h"
#include "EC_World.h"
#include "EC_Faction.h"
#include "EC_Skill.h"
#include "EC_NPCServer.h"
#include "EC_Pet.h"
#include "EC_Global.h"
#include "AFI.h"
#include "A2DSprite.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUICommon.h"
#include "AUI\\AUICheckBox.h"
#include "AUI\\AUIProgress.h"
#include "AUI\\AUIImagePicture.h"
#include "Network\\ids.hxx"
#include "elementdataman.h"
#include "DlgDragDrop.h"
#include "DlgBattleFactionWarApply.h"
#include "DlgBattleFactionWarControl.h"

#include "EC_IvtrConsume.h"
#include "EC_Inventory.h"
#include "LuaEvent.h"

#include "AUI\\AUICTranslate.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgTarget, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",		OnCommand_CANCEL)
AUI_ON_COMMAND("InviteGroup",	OnCommand_Speacial)
AUI_ON_COMMAND("Trade",			OnCommand_Speacial)
AUI_ON_COMMAND("EPEquip",		OnCommand_Speacial)
AUI_ON_COMMAND("Follow",		OnCommand_Speacial)
AUI_ON_COMMAND("GuildBattle",	OnCommand_GuildBattle)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgTarget, CDlgBase)

AUI_ON_EVENT("Pic_HeadFrame",		WM_MOUSEMOVE,		OnEventMouseMove)
AUI_ON_EVENT("Pic_HeadFrame",		WM_LBUTTONDOWN,		OnEventLButtonDown_Size)
AUI_ON_EVENT("Pic_HeadFrame",		WM_LBUTTONUP,		OnEventLButtonUp_Size)
AUI_ON_EVENT("Pic_HeadFrame",		WM_RBUTTONUP,		OnEventRButtonUp)

AUI_ON_EVENT("Pic_Prof",			WM_LBUTTONDOWN,		OnEventLButtonDown_Prof)
AUI_ON_EVENT("Img_Race",			WM_LBUTTONDOWN,		OnEventLButtonDown_Monster)

AUI_END_EVENT_MAP()

A2DSprite* CDlgTarget::m_pA2DSpriteTargetHead = NULL;


char* szHp_Layer[] = {"Progress\\prgs_monsterhp01.tga",
					"Progress\\prgs_monsterhp02.tga",
					"Progress\\prgs_monsterhp03.tga",
					"Progress\\prgs_monsterhp04.tga",
					"Progress\\prgs_monsterhp05.tga",
					"Progress\\prgs_monsterhp06.tga",
					"Progress\\prgs_monsterhp07.tga",
					"Progress\\prgs_monsterhp08.tga",
					"Progress\\prgs_monsterhp09.tga",
					"Progress\\prgs_monsterhp10.tga", 
					"Progress\\prgs_monsterhp10.tga"};

CDlgTarget::CDlgTarget()
{
	m_nMouseOffset		= 0;
	m_nMouseOffsetThis	= 0;
	m_nShowLevel		= -1;
	m_iCultivation		= -1;
	m_bFengsheng		= false;
	m_bHighLevel        = false;
	m_nMonsterID		= -1;
}

CDlgTarget::~CDlgTarget()
{
}

bool CDlgTarget::OnInitDialog()
{
	if (m_szName == "Win_TargetNPC2")
	{
		m_pImg_Head = (PAUIIMAGEPICTURE)GetDlgItem("Pic_HeadFrameNpc");
	}
	else if(m_szName == "Win_TargetMonster")
	{
		GetDlgItem("Txt_Layer")->Show(false);
		m_pImg_Head = (PAUIIMAGEPICTURE)GetDlgItem("Pic_HeadFrame");
	}
	else
		m_pImg_Head = (PAUIIMAGEPICTURE)GetDlgItem("Pic_HeadFrame");

	m_pTxt_Name = (PAUILABEL)GetDlgItem("Txt_Name");
	m_pTxt_Lv = (PAUILABEL)GetDlgItem("Txt_Lv");
	m_pPrg_HP = (PAUIPROGRESS)GetDlgItem("Prg_HP");
	m_pPrg_MP = (PAUIPROGRESS)GetDlgItem("Prg_MP");
	m_pPrg_DP = (PAUIPROGRESS)GetDlgItem("Prg_DP");
	m_pPrg_HP_TopLayer  = (PAUIPROGRESS)GetDlgItem("Prgs_HpBoss");
	m_pTxt_Name->SetAlign(AUIFRAME_ALIGN_CENTER);
	if( m_pImg_Head )
		m_pImg_Head->SetAcceptMouseMessage(true);

	if( GetDlgItem("Btn_GuildEnemies") )
		GetDlgItem("Btn_GuildEnemies")->Show(false);
	if( !m_pA2DSpriteTargetHead )
	{
		m_pA2DSpriteTargetHead = new A2DSprite;
		if( !m_pA2DSpriteTargetHead ) return AUI_ReportError(__LINE__, __FILE__);
	}

	return true;
}

bool CDlgTarget::Release()
{
	A3DRELEASE(m_pA2DSpriteTargetHead)
	return CDlgBase::Release();
}


void CDlgTarget::OnCommand_CANCEL(const char * szCommand)
{
	m_pAUIManager->RespawnMessage();
}

void CDlgTarget::OnCommand_Speacial(const char * szCommand)
{
	int idSelObj = GetHostPlayer()->GetSelectedTarget();
	if( idSelObj != GetHostPlayer()->GetCharacterID() )
	{
		GetGameUIMan()->m_pDlgQuickAction->SetData(idSelObj);
		GetGameUIMan()->m_pDlgQuickAction->OnCommand(szCommand);
	}
}

void CDlgTarget::OnCommand_GuildBattle(const char * szCommand)
{
	CECFactionMan *pFMan = GetGame()->GetFactionMan();
	Faction_Info *pInfo = pFMan->GetFaction(GetHostPlayer()->GetFactionID());
	if( pInfo && pInfo->GetLevel() < 1 )
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(888), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}

	int idSelObj = GetHostPlayer()->GetSelectedTarget();
	CECPlayerMan *pPlayerMan = GetWorld()->GetPlayerMan();
	CECPlayer *pPlayer = (CECPlayer *)pPlayerMan->GetPlayer(idSelObj);
	if( !pPlayer ) return;

	pInfo = pFMan->GetFaction(pPlayer->GetFactionID());
	if(pInfo && pInfo->GetLevel() < 1)
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(889), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}

	if( 0 == CDlgBattleFactionWarControl::GetCombatFaction() &&
		GetHostPlayer()->GetFactionID()!=0 &&
		pPlayer->GetFactionID()!=0 &&
		pPlayer->GetFactionID() != GetHostPlayer()->GetFactionID() &&
		(GetHostPlayer()->GetFRoleID() == GNET::_R_MASTER || 
		GetHostPlayer()->GetFRoleID() == GNET::_R_VICEMASTER) )
	{
	  CDlgBattleFactionWarApply *pDlgFactionWar = (CDlgBattleFactionWarApply*)GetGameUIMan()->GetDialog("Win_Battle_GuildWar_Apply");
	  pDlgFactionWar->ApplyFactionWarByTargetHead(pPlayer->GetName());
	}
}

void CDlgTarget::OnTick()
{
	int idSelObj = GetHostPlayer()->GetSelectedTarget();
	int iCurHP = 0, iMaxHP = 0, iCurMP = 0, iMaxMP = 0, iCurDP = 0, iMaxDP = 0, iRage = 0;

	if( ISPLAYERID(idSelObj) )
	{
		CECPlayerMan *pPlayerMan = GetWorld()->GetPlayerMan();
		CECPlayer *pPlayer = (CECPlayer *)pPlayerMan->GetPlayer(idSelObj);
		PAUIDIALOG pDlg = this;
		if( !pPlayer )
		{
			Show(false);
			return;
		}
		
		const ROLEBASICPROP& bp = pPlayer->GetBasicProps();
		const ROLEEXTPROP &ep = pPlayer->GetExtendProps();

		CECPlayer::SECT_LEVEL2_INFO * pInfo = CECPlayer::GetSectInfoByProf(bp.iProfession);
		bool bHightLevel = (bp.iLevel > 150) && (pPlayer->GetRebornCount() > 0);

		if (pPlayer->GetCultivation() != m_iCultivation || m_bFengsheng!=bp.iDeityLevel>0 || m_bHighLevel != bHightLevel)
		{
			AUIOBJECT_SETPROPERTY p;
			m_iCultivation = pPlayer->GetCultivation();
			m_bFengsheng = bp.iDeityLevel>0;
			m_bHighLevel = bHightLevel;
			m_pPrg_DP->Show(m_bFengsheng);
			if(m_bHighLevel)
			{
				if(m_bFengsheng)
				{
					if ( 0 == m_iCultivation )
					{
						sprintf(p.fn, "Window\\CharHead03.tga");
					}
					else if ( 1 == m_iCultivation )
					{
						sprintf(p.fn, "Window\\CharHead_FengshenTaoism03.tga");
					}
					else if ( 2 == m_iCultivation )
					{
						sprintf(p.fn, "Window\\CharHead_FengshenEvil03.tga");
					}
					else if ( 4 == m_iCultivation )
					{
						sprintf(p.fn, "Window\\CharHead_FengshenBuddhism03.tga");
					}
				}
				else
				{
					if ( 0 == m_iCultivation )
					{
						sprintf(p.fn, "Window\\CharHead03.tga");
					}
					else if ( 1 == m_iCultivation )
					{
						sprintf(p.fn, "Window\\CharHead_Taoism03.tga");
					}
					else if ( 2 == m_iCultivation )
					{
						sprintf(p.fn, "Window\\CharHead_Evil03.tga");
					}
					else if ( 4 == m_iCultivation )
					{
						sprintf(p.fn, "Window\\CharHead_Buddhism03.tga");
					}
				}
			}			
			else if (m_bFengsheng)
			{
				if ( 0 == m_iCultivation )
				{
					sprintf(p.fn, "Window\\CharHead1.tga");
				}
				else if ( 1 == m_iCultivation )
				{
					sprintf(p.fn, "Window\\CharHead_FengshenTaoism.tga");
				}
				else if ( 2 == m_iCultivation )
				{
					sprintf(p.fn, "Window\\CharHead_FengshenEvil.tga");
				}
				else if ( 4 == m_iCultivation )
				{
					sprintf(p.fn, "Window\\CharHead_FengshenBuddhism.tga");
				}
			}
			else
			{
				if ( 0 == m_iCultivation )
				{
					sprintf(p.fn, "Window\\CharHead1.tga");
				}
				else if ( 1 == m_iCultivation )
				{
					sprintf(p.fn, "Window\\CharHead_Taoism.tga");
				}
				else if ( 2 == m_iCultivation )
				{
					sprintf(p.fn, "Window\\CharHead_Evil.tga");
				}
				else if ( 4 == m_iCultivation )
				{
					sprintf(p.fn, "Window\\CharHead_Buddhism.tga");
				}
			}
			PAUIIMAGEPICTURE pImgBack = (PAUIIMAGEPICTURE)GetDlgItem("Img_Bg");
			pImgBack->SetProperty("Image File", &p);
		}

		CECElsePlayer *pElsePlayer = GetWorld()->GetPlayerMan()->GetElsePlayer(idSelObj);
		if( (GetHostPlayer()->GetTeam() && 
			(GetHostPlayer()->GetTeam()->GetLeaderID() != GetHostPlayer()->GetCharacterID() ||
			pElsePlayer && pElsePlayer->GetInTeamState() != 0)) ||
			(pElsePlayer && pElsePlayer->GetSrcServerID()!=GetHostPlayer()->GetSrcServerID()))
			GetDlgItem("Btn_Team")->Enable(false);
		else
			GetDlgItem("Btn_Team")->Enable(true);

		if(GetHostPlayer()->GetBattleType() != -1 && pElsePlayer != NULL)	/// 2012-9-14 add by zy 添加跨服战场组队判断
		{
			int nHint = 0;
			if(pElsePlayer->GetSrcServerID()!=GetHostPlayer()->GetSrcServerID())
			{
				if(GetHostPlayer()->GetBattleCamp() != pElsePlayer->GetBattleCamp())
				{
					GetDlgItem("Btn_Team")->Enable(false);
					nHint = 5310;
				}
				else
				{
					GetDlgItem("Btn_Team")->Enable(true);
					nHint = 5313;
				}
			}
			else
			{
				if(GetHostPlayer()->GetBattleCamp() != pElsePlayer->GetBattleCamp())
				{
					GetDlgItem("Btn_Team")->Enable(false);
					nHint = 5310;
				}
				else
				{
					GetDlgItem("Btn_Team")->Enable(true);
					nHint = 5313;
				
				}
			}
			GetDlgItem("Btn_Team")->SetHint(GetGameUIMan()->GetStringFromTable(nHint));
		}

		int iFrameIndex = -1;
		ACString strText;
		bHightLevel = (bp.iLevel > 150) && (pPlayer->GetRebornCount() > 0) && (pInfo && pInfo->level2 == 5);
		if(bHightLevel && pInfo)
		{
			if((pInfo->mask_sect & (1<<SECT_2)))
			{
				iFrameIndex = 15;
				strText = GetStringFromTable(18750);
			}
			else if(pInfo->mask_sect & (1<<SECT_3))
			{
				iFrameIndex = 18;
				strText = GetStringFromTable(18751);
			}
			else if(pInfo->mask_sect & (1<<SECT_4))
			{
				iFrameIndex = 21;
				strText = GetStringFromTable(18752);
			}
			else if(pInfo->mask_sect & (1<<SECT_5))
			{
				iFrameIndex = 24;
				strText = GetStringFromTable(18753);
			}
			else if(pInfo->mask_sect & (1<<SECT_6))
			{
				iFrameIndex = 30;
				strText = GetStringFromTable(18754);
			}
			else if(pInfo->mask_sect & (1<<SECT_7))
			{
				iFrameIndex = 70;
				strText = GetStringFromTable(18755);
			}
			else if(pInfo->mask_sect & (1<<SECT_10))
			{
				iFrameIndex = 38;
				strText = GetStringFromTable(18756);
			}
			else if(pInfo->mask_sect & (1<<SECT_11))
			{
				iFrameIndex = 44;
				strText = GetStringFromTable(18757);
			}
			else if(pInfo->mask_sect & (1<<SECT_12))
			{
				iFrameIndex = 101;
				strText = GetStringFromTable(18758);
			}
			else if(pInfo->mask_sect & (1<<SECT_13))
			{
				iFrameIndex = 50;
				strText = GetStringFromTable(18759);
			}
			else if(pInfo->mask_sect & (1<<SECT_14))
			{
				iFrameIndex = 62;
				strText = GetStringFromTable(18760);
			}
			else if(pInfo->mask_sect & (1<<SECT_15))
			{
				iFrameIndex = 61;
				strText = GetStringFromTable(18761);
			}
			else if(pInfo->mask_sect & (1<<SECT_16))
			{
				iFrameIndex = 108;
				strText = GetStringFromTable(18762);
			}
			else if(pInfo->mask_sect & (1<<SECT_17))
			{
				iFrameIndex = 114;
				strText = GetStringFromTable(18763);
			}
		}
		else
		{
			iFrameIndex = pPlayer->GetProfession();
			strText = GetStringFromTable(PROFESSION_START + pPlayer->GetProfession());
		}
		((PAUIIMAGEPICTURE)GetDlgItem("Pic_Prof"))->FixFrame(iFrameIndex);
		((PAUIIMAGEPICTURE)GetDlgItem("Pic_Prof"))->SetHint(strText);

		if( 0 != CDlgBattleFactionWarControl::GetCombatFaction() || 
			GetHostPlayer()->GetFactionID() == 0 ||
			pPlayer->GetFactionID() == 0 ||
			pPlayer->GetFactionID() == GetHostPlayer()->GetFactionID() ||
			(GetHostPlayer()->GetFRoleID() != GNET::_R_MASTER && 
			GetHostPlayer()->GetFRoleID() != GNET::_R_VICEMASTER) )
		{
			GetDlgItem("Btn_GuildBattle")->Show(false);
		}
		else
		{
			GetDlgItem("Btn_GuildBattle")->Show(true);
		}
		
		if( GetHostPlayer()->GetCombatFaction() != 0 && 
			pPlayer->GetFactionID() == GetHostPlayer()->GetCombatFaction() )
			GetDlgItem("Img_GuildCombat")->Show(true);
		else
			GetDlgItem("Img_GuildCombat")->Show(false);

		iCurHP = bp.iCurHP;
		iMaxHP = max(ep.max_hp, 1);
		iCurMP = bp.iCurMP;
		iMaxMP = max(ep.max_mp, 1);
		iRage  = bp.irage;
		iCurDP = bp.iCurDP;
		iMaxDP = max(ep.max_dp, 1);
		m_pPrg_HP->SetProgress(AUIPROGRESS_MAX * iCurHP / iMaxHP);

		if(pInfo && pInfo->mask_sect & (1 << SECT_17))
		{
			AUIOBJECT_SETPROPERTY p;
			sprintf(p.fn, "zhuxian3\\Progress\\Prgs_Ap.tga");
			m_pPrg_MP->SetProperty("Fill Image File", &p);
			m_pPrg_MP->SetProgress(int(double(iRage) * AUIPROGRESS_MAX / iMaxHP));
			m_pPrg_MP->SetHint(GetStringFromTable(21702));
		}
		else
		{
			AUIOBJECT_SETPROPERTY p;
			sprintf(p.fn, "zhuxian3\\Progress\\Prgs_Mp.tga");
			m_pPrg_MP->SetProperty("Fill Image File", &p);
			m_pPrg_MP->SetProgress(int(double(iCurMP) * AUIPROGRESS_MAX / iMaxMP));
			m_pPrg_MP->SetHint(GetStringFromTable(21701));
		}		

		if(m_pPrg_DP) 
		{
			m_pPrg_DP->Show(m_bFengsheng);
			m_pPrg_DP->SetProgress(AUIPROGRESS_MAX * iCurDP / iMaxDP);
		}
	
		ACHAR szText[80];
		m_pTxt_Name->SetAlign(AUIFRAME_ALIGN_CENTER);
		AUI_ConvertChatString(GetGameRun()->GetPlayerName(idSelObj, true), szText);
		m_pTxt_Name->SetText(szText);
		m_pTxt_Name->SetColor(pPlayer->GetNameColor());
		m_pTxt_Lv->SetAlign(AUIFRAME_ALIGN_CENTER);
		a_sprintf(szText, _AL("%d"), pPlayer->GetBasicProps().iLevel);
		m_pTxt_Lv->SetText(szText);
		int battleType = GetHostPlayer()->GetBattleType();
		if (battleType==BATTLE_TYPE_ARENA || battleType==BATTLE_TYPE_ARENAREBORN || battleType==BATTLE_TYPE_CRSSVR || GetHostPlayer()->IsInAnonymousMap())
		{
			m_pTxt_Name->SetText(_AL(""));
			m_pTxt_Lv->SetText(_AL(""));
		}
		
		int j;
		PAUIIMAGEPICTURE pImage;
		AString strFile;
		for( j = 0; j < CDLGTARGET_ST_MAX ; j++ )
		{
			char szItem[20];
			sprintf(szItem, "st_%d", j + 1);
			pImage = (PAUIIMAGEPICTURE)GetDlgItem(szItem);
			if( !pImage ) break;

			if( j < pPlayer->GetIconStateNum() )
			{
				const GNET::TeamState *pTS = GNET::TeamState::Query(pPlayer->GetIconState(j).id);
				if( pTS )
					af_GetFileTitle(pTS->GetIcon(), strFile);
				else
					strFile = "";
				
				strFile.MakeLower();
				pImage->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_STATE],
					GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_STATE][strFile]);
				if( !pTS )
					pImage->SetHint(_AL(""));
				else
				{
					const ACHAR * szDesc = GetGame()->GetBuffDesc(pTS->GetID());
					AUICTranslate trans;
					if( szDesc )
						pImage->SetHint(trans.Translate(szDesc));
					else
						pImage->SetHint(trans.Translate(pTS->GetName()));
				}

				if(pPlayer->GetIconState(j).iOverlayCnt > 1)
				{
					ACString strText;
					strText.Format(_AL("%d"), pPlayer->GetIconState(j).iOverlayCnt);
					pImage->SetText(strText);
				}
				else
				{
					pImage->SetText(_AL(""));
				}
			}
			else
			{
				pImage->ClearCover();
				pImage->SetHint(_AL(""));
				pImage->SetText(_AL(""));
			}
		}
		if( pPlayer->IsAllResReady() )
		{
			m_pImg_Head->SetRenderCallback(PlayerRenderPortrait,
				(DWORD)pPlayer, m_nMouseOffset + m_nMouseOffsetThis);
		}
		else
			m_pImg_Head->SetRenderCallback(NULL, 0);
	}
	else if( ISNPCID(idSelObj) )
	{
//		m_pImg_Head = (PAUIIMAGEPICTURE)GetDlgItem("Pic_HeadFrame");
//		m_pTxt_Name = (PAUILABEL)GetDlgItem("Txt_Name");
//		m_pTxt_Lv = (PAUILABEL)GetDlgItem("Txt_Lv");
//		m_pPrg_HP = (PAUIPROGRESS)GetDlgItem("Prg_HP");
//		m_pPrg_MP = (PAUIPROGRESS)GetDlgItem("Prg_MP");
		
		CECNPC *pNPC = GetWorld()->GetNPCMan()->GetNPC(idSelObj);
		if( !pNPC )
		{
			Show(false);
			return;
		}
		AUIOBJECT_SETPROPERTY p;
		const ROLEBASICPROP& bp = pNPC->GetBasicProps();
		const ROLEEXTPROP& ep = pNPC->GetExtendProps();
		iCurHP = bp.iCurHP;
		iMaxHP = max(ep.max_hp, 1);
		int nShowLevel = 0;
		if (m_szName == "Win_TargetMonster")
		{
			GetDlgItem("Img_Catch")->Show(false);
			((PAUIIMAGEPICTURE)GetDlgItem("Img_Race"))->FixFrame(0);
			((PAUIIMAGEPICTURE)GetDlgItem("Img_Race"))->SetHint(_AL(""));
		}
		if( pNPC->IsMonsterNPC() )
		{
			elementdataman* pElementDataMan = GetGame()->GetElementDataMan();
			DATA_TYPE dt;
			MONSTER_ESSENCE* pEssence = (MONSTER_ESSENCE*)pElementDataMan->get_data_ptr(
				pNPC->GetTemplateID(), ID_SPACE_ESSENCE,dt); 
			if (pEssence && dt == DT_MONSTER_ESSENCE )
				nShowLevel = pEssence->show_level;
			if (m_szName == "Win_TargetMonster")
			{
				int iFraction = pEssence->monster_faction;
				int iF=0;
				while (iFraction>0)
				{
					if (iFraction&1)
					{
						if (iFraction>1)
						{
							iFraction = 15;
						}
						break;
					}
					iFraction>>=1;
					iF++;
				}
				((PAUIIMAGEPICTURE)GetDlgItem("Img_Race"))->FixFrame(iF);
				((PAUIIMAGEPICTURE)GetDlgItem("Img_Race"))->SetHint(GetStringFromTable(8420+iF));
				if (pEssence->can_catch && iCurHP<iMaxHP/2)
				{
					LuaEvent::FirstCatchPet();
					GetDlgItem("Img_Catch")->Show(true);
				}
				else
					GetDlgItem("Img_Catch")->Show(false);
				

				int	nEveryMaxHP =  iMaxHP / pEssence->hp_disp_lv;	// 每层最大血值
				if(nEveryMaxHP == 0)
				{
					nEveryMaxHP = 1;
				}
				int nCurLayer = iCurHP / nEveryMaxHP ;	// 当前在第几层
				int nCurHp = iCurHP %  nEveryMaxHP;		// 当前层余数
				m_pPrg_HP_TopLayer->Show(false);
				GetDlgItem("Txt_Layer")->Show(false);
				ACString strText;
				if(nCurLayer >= 1)
				{		
					m_pPrg_HP_TopLayer->Show(true);

					sprintf(p.fn, szHp_Layer[nCurLayer]);
					m_pPrg_HP_TopLayer->SetProperty("Fill Image File",&p);
					m_pPrg_HP_TopLayer->SetProgress(int(double(AUIPROGRESS_MAX) * nCurHp / nEveryMaxHP));
					sprintf(p.fn, szHp_Layer[nCurLayer - 1]);
					m_pPrg_HP->SetProperty("Fill Image File",&p);
					m_pPrg_HP->SetProgress(int(double(AUIPROGRESS_MAX) * nEveryMaxHP / nEveryMaxHP));
					if((nCurLayer + 1) > 10)
					{
						strText.Format(_AL("x%d"), nCurLayer);
					}
					else if(nCurHp == 0 && nCurLayer == 1)
					{
						strText.Format(_AL(""));
					}
					else if(nCurHp == 0)
					{
						strText.Format(_AL("x%d"), nCurLayer);
					}
					else 
					{
						strText.Format(_AL("x%d"), nCurLayer + 1);
					}
					GetDlgItem("Txt_Layer")->SetText(strText);
					GetDlgItem("Txt_Layer")->Show(true);
				}
				else
				{
					sprintf(p.fn, szHp_Layer[0]);
					m_pPrg_HP->SetProperty("Fill Image File",&p);
					m_pPrg_HP->SetProgress(int(double(AUIPROGRESS_MAX) *  iCurHP / nEveryMaxHP));
					//strText.Format(_AL("x1"));
				}
				
			}		
		}
		else
		{
			sprintf(p.fn, szHp_Layer[0]);
			m_pPrg_HP->SetProperty("Frame Image",&p);
			m_pPrg_HP->SetProgress(int(double(AUIPROGRESS_MAX) * iCurHP / iMaxHP));	
		}

		ACHAR szText[30];
		a_sprintf(szText, _AL("%d%%"), int(double(iCurHP) * 100.0 / iMaxHP));
		if(m_pPrg_HP_TopLayer && m_pPrg_HP_TopLayer->IsShow())
		{
			m_pPrg_HP_TopLayer->SetHint(szText);
		}
		else
		{
			m_pPrg_HP->SetHint(szText);
		}
//		if( pNPC->IsMonsterNPC() )
//		{
//			iCurMP = bp.iCurMP;
//			iMaxMP = max(ep.max_mp, 1);
//			m_pPrg_MP->SetProgress(AUIPROGRESS_MAX * iCurMP / iMaxMP);
//		}
//		else
//			m_pPrg_MP->SetProgress(AUIPROGRESS_MAX);
		
		ACHAR szName[40];
		const ACHAR *pszName = pNPC->GetName();
		int battleType = GetHostPlayer()->GetBattleType();
		if ( (battleType==BATTLE_TYPE_ARENA || battleType==BATTLE_TYPE_ARENAREBORN || battleType==BATTLE_TYPE_CRSSVR || GetHostPlayer()->IsInAnonymousMap()) && pNPC->IsPetNPC())
		{
			m_pTxt_Name->SetText(_AL(""));
		}
		else
		{
			AUI_ConvertChatString(pszName, szName);
			m_pTxt_Name->SetText(szName);
			m_pTxt_Name->SetColor(pNPC->GetNameColor());
		}
		int j;
		PAUIIMAGEPICTURE pImage;
		AString strFile;
		for( j = 0; j < CDLGTARGET_ST_MAX ; j++ )
		{
			char szItem[20];
			sprintf(szItem, "st_%d", j + 1);
			pImage = (PAUIIMAGEPICTURE)GetDlgItem(szItem);
			if( !pImage ) break;

			if( j < pNPC->GetIconStateNum() )
			{
				const GNET::TeamState *pTS = GNET::TeamState::Query(pNPC->GetIconState(j).id);
				if( pTS )
					af_GetFileTitle(pTS->GetIcon(), strFile);
				else
					strFile = "";
				
				strFile.MakeLower();
				pImage->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_STATE],
					GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_STATE][strFile]);
				if( !pTS )
					pImage->SetHint(_AL(""));
				else
				{
					const ACHAR * szDesc = GetGame()->GetBuffDesc(pTS->GetID());
					AUICTranslate trans;
					if( szDesc )
						pImage->SetHint(trans.Translate(szDesc));
					else
						pImage->SetHint(trans.Translate(pTS->GetName()));
				}
			}
			else
			{
				pImage->ClearCover();
				pImage->SetHint(_AL(""));
			}
		}

		if (m_szName == "Win_TargetNPC2" && m_nMonsterID != idSelObj )
		{
			m_nMonsterID = idSelObj;

			if (pNPC->IsServerNPC())
			{
				CECNPCServer* pNPCServer = (CECNPCServer*)pNPC;
				const NPC_ESSENCE *pNPCEssence = pNPCServer->GetDBEssence();
				if (!m_pImg_Head)
				{
					return;
				}
				AString strFile1, strFile;
				af_GetFileTitle(g_pGame->GetDataPath(pNPCEssence->head_icon), strFile1);
				strFile.Format("npcimg\\%s",strFile1);
				strFile.MakeLower();
				if(m_pA2DSpriteTargetHead)
					m_pA2DSpriteTargetHead->Release();
				bool bVal = m_pA2DSpriteTargetHead->Init(m_pA3DDevice, strFile, 0 );
				if (!bVal)
				{
					m_pImg_Head->SetCover( NULL, 0);
					return;
				}
				m_pImg_Head->SetCover( m_pA2DSpriteTargetHead, 0);
			}
			else if (pNPC->IsPetNPC())
			{
				CECPet *pNPCPet = (CECPet*)pNPC;
				const PET_BEDGE_ESSENCE *pPetEssence = pNPCPet->GetDBEssence();

				AString strFile;
				af_GetFileTitle(GetGame()->GetDataPath(pPetEssence->file_head_icon[pNPCPet->GetFace()]), strFile);
				strFile.MakeLower();
				m_pImg_Head->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_PET],
					GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_PET][strFile]);
				int battleType = GetHostPlayer()->GetBattleType();
				if (battleType==BATTLE_TYPE_ARENA || battleType==BATTLE_TYPE_ARENAREBORN || battleType==BATTLE_TYPE_CRSSVR || GetHostPlayer()->IsInAnonymousMap())
				{
					m_pTxt_Name->SetText(_AL(""));
				}
				
			}
		}
		else if( nShowLevel != m_nShowLevel && m_szName != "Win_TargetNPC2")
		{
			AUIOBJECT_SETPROPERTY p;
			sprintf(p.fn, "Window\\Target2%d.tga", nShowLevel);
			SetProperty("Frame Image", &p);
			m_nShowLevel = nShowLevel;
		}
	}
	else
		return;
}

void CDlgTarget::OnEventRButtonUp(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	// 匿名地图中
	if (GetHostPlayer()->IsInAnonymousMap())
	{
		int idSelObj = GetHostPlayer()->GetSelectedTarget();
		if( ISPLAYERID (idSelObj))
		{
			CECPlayer* pPlayer = GetWorld()->GetPlayerMan()->GetPlayer(idSelObj);
			if (pPlayer &&  (GetHostPlayer()->GetRace() != pPlayer->GetRace()) )
				return;
		}		
	}

	int battleType = GetHostPlayer()->GetBattleType();
	if ( battleType!=BATTLE_TYPE_ARENA && battleType!=BATTLE_TYPE_ARENAREBORN && battleType!=BATTLE_TYPE_CRSSVR )
	{
		int idSelObj = GetHostPlayer()->GetSelectedTarget();
		A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
		int x = GET_X_LPARAM(lParam) - p->X;
		int y = GET_Y_LPARAM(lParam) - p->Y;
		
		GetGameUIMan()->PopupPlayerContextMenu(idSelObj, x, y);
	}
}


void CDlgTarget::OnEventMouseMove(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if( (wParam & MK_LBUTTON) )
	{
		m_nMouseOffsetThis = GET_X_LPARAM(lParam) - m_nMouseLastX;
		if( m_nMouseOffset + m_nMouseOffsetThis > 70 )
			m_nMouseOffsetThis = 70 - m_nMouseOffset;
		else if( m_nMouseOffset + m_nMouseOffsetThis < -70 )
			m_nMouseOffsetThis = -70 - m_nMouseOffset;
	}
}

void CDlgTarget::OnEventLButtonDown_Size(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
 	SetCaptureObject(pObj);
	m_nMouseLastX = GET_X_LPARAM(lParam);
	m_nMouseOffset += m_nMouseOffsetThis;
	m_nMouseOffsetThis = 0;
}

void CDlgTarget::OnEventLButtonUp_Size(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	SetCaptureObject(NULL);
}

void CDlgTarget::OnShowDialog()
{
	if (m_szName=="Win_TargetPlayer")
	{
		int idSelObj = GetHostPlayer()->GetSelectedTarget();
		PAUIOBJECT pObjTeam = GetDlgItem("Btn_Team");
		PAUIOBJECT pObjTrade = GetDlgItem("Btn_Trade");
		pObjTeam->Enable(true);
		pObjTeam->SetHint(GetGameUIMan()->GetStringFromTable(5313));
		pObjTrade->Enable(true);
		pObjTrade->SetHint(GetGameUIMan()->GetStringFromTable(5312));
		if( ISPLAYERID(idSelObj) )
		{
			CECPlayerMan *pPlayerMan = GetWorld()->GetPlayerMan();
			CECPlayer *pPlayer = (CECPlayer *)pPlayerMan->GetPlayer(idSelObj);
			PAUIDIALOG pDlg = this;
			if( pPlayer && pPlayer->GetSrcServerID()!=GetHostPlayer()->GetSrcServerID())
			{
				pObjTeam->Enable(false);
				pObjTeam->SetHint(GetGameUIMan()->GetStringFromTable(5310));
				pObjTrade->Enable(false);
				pObjTrade->SetHint(GetGameUIMan()->GetStringFromTable(5311));
			}
		}
	}
}
void CDlgTarget::OnEventLButtonDown_Prof(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();

	GetGameUIMan()->m_ptLButtonDown.x = GET_X_LPARAM(lParam) - p->X;
	GetGameUIMan()->m_ptLButtonDown.y = GET_Y_LPARAM(lParam) - p->Y;


	CECPlayer* player = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetPlayer(g_pGame->GetGameRun()->GetHostPlayer()->GetSelectedTarget());
	int idProf = player->GetBasicProps().iProfession;

	((PAUIIMAGEPICTURE)pObj)->SetCover(GetGameUIMan()->m_pA2DSpriteProfs[idProf],0,0);
	GetGameUIMan()->InvokeDragDrop(this, pObj, GetGameUIMan()->m_ptLButtonDown);
	((PAUIIMAGEPICTURE)pObj)->SetCover(NULL,1,0);
//	GetGameUIMan()->PlayItemSound((CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem"), false);
//	GetGameUIMan()->m_pDlgDragDrop->SetData(1234);
}


void CDlgTarget::OnEventLButtonDown_Monster(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();

	GetGameUIMan()->m_ptLButtonDown.x = GET_X_LPARAM(lParam) - p->X;
	GetGameUIMan()->m_ptLButtonDown.y = GET_Y_LPARAM(lParam) - p->Y;

	CECMonster* pMonster = (CECMonster*)g_pGame->GetGameRun()->GetWorld()->GetNPCMan()->GetNPC(g_pGame->GetGameRun()->GetHostPlayer()->GetSelectedTarget());
	if(!pMonster->IsMonsterNPC())	// server npc or pet not allow to drag/drop.
		return;

	//	Get database data
	const MONSTER_ESSENCE* pMonEssence = pMonster->GetDBEssence();
	if(!pMonEssence)
		return;

	int iFraction = pMonEssence->monster_faction;
	int iF=0;
	while (iFraction>0)
	{
		if (iFraction&1)
		{
			if (iFraction>1)
			{
				iFraction = 15;
			}
			break;
		}
		iFraction>>=1;
		iF++;
	}
	((PAUIIMAGEPICTURE)pObj)->SetCover(GetGameUIMan()->m_pA2DSpriteMonsterProfs[iF],0,0);

	GetGameUIMan()->InvokeDragDrop(this, pObj, GetGameUIMan()->m_ptLButtonDown);
//	GetGameUIMan()->PlayItemSound((CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem"), false);
	((PAUIIMAGEPICTURE)pObj)->SetCover(NULL,1,0);
//	GetGameUIMan()->m_pDlgDragDrop->SetData(4567);
}




bool CDlgTargetHP::OnInitDialog()
{
	m_pTxt_Blood = (PAUILABEL)GetDlgItem("Txt_Hp");
	m_pPrgs_Blood = (PAUIPROGRESS)GetDlgItem("Prgs_Hp");
	return true;
}

void CDlgTargetHP::OnTick()
{
	int idSelObj = GetHostPlayer()->GetSelectedTarget();
	if( ISPLAYERID(idSelObj))
	{
		CECPlayerMan *pPlayerMan = GetWorld()->GetPlayerMan();
		CECPlayer *pPlayer = (CECPlayer *)pPlayerMan->GetPlayer(idSelObj);
		if(pPlayer && pPlayer->GetBloodPoolState())
		{
			ACString strText;
			int  iCurBloodPool = pPlayer->GetCurBloodPool();
			int  iMaxBloodPool = max(pPlayer->GetMaxBloodPool(), 1);
			m_pPrgs_Blood->SetProgress(AUIPROGRESS_MAX * iCurBloodPool / iMaxBloodPool);
			strText.Format(_AL("%d/%d"), iCurBloodPool, iMaxBloodPool);
			m_pTxt_Blood->SetText(strText);
		}
		else
			Show(false);
	}
	else
		Show(false);
}