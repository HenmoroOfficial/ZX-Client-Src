// Filename	: DlgMiniMap.cpp
// Creator	: Tom Zhou
// Date		: October 15, 2005

#include "A3DFuncs.h"
#include "A2DSprite.h"
#include "A3DStream.h"
#include "A3DCamera.h"
#include "A3DDevice.h"
#include "A3DTexture.h"
#include "A3DFlatCollector.h"
#include "AIniFile.h"
#include "AWIniFile.h"
#include "AUI\\AUIDef.h"
#include "AUI\\CSplit.h"
#include "AUI\\AUIEditBox.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUICheckBox.h"
#include "AUI\\AUIImagePicture.h"
#include "DlgChat.h"
#include "DlgMiniMap.h"
#include "DlgFriendList.h"
#include "DlgSetting.h"
#include "DlgInputName.h"
#include "DlgMidMap.h"
#include "DlgWorldMap.h"
#include "DlgPKSetting.h"
#include "DlgTask.h"
#include "DlgBroadCastInGame.h"
#include "DlgCommonDlg.h"
#include "DlgDigTreas.h"
#include "DlgDynamicSystem.h"
#include "DlgBattleTeam6v6.h"
#include "DlgGuildLandBuild.h"
#include "EC_FixedMsg.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameSession.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EL_Precinct.h"
#include "EC_NPCServer.h"
#include "EC_ManNPC.h"
#include "EC_Friend.h"
#include "EC_ElsePlayer.h"
#include "EC_ManPlayer.h"
#include "EC_Team.h"
#include "EC_Instance.h"
#include "EC_World.h"
#ifdef BACK_VERSION
#include "EC_Viewport.h"
#include "EC_SunMoon.h"
#else
#include "ECViewport.h"
#include "A3DSunMoon.h"
#endif
#include "EC_Configs.h"
#include "EC_Utility.h"
#include "EC_Model.h"
#include "EC_AutoMove.h"
#include "EC_Secretary.h"
#include "EC_HelpGraphGuide.h"
#include "globaldataman.h"
#include "elementdataman.h"
#include "LuaEvent.h"
#include "DlgKingWar.h"
#include "DlgCrssvrTeams.h"
#include "EC_BattleDungeon.h"

#define new A_DEBUG_NEW
#define SQUARE(x)	((x) * (x))
#define DEFAULT_RADAR_DIAMETER 160

#define POINTTOSCREEN_X(x) int(int(p->X) + (rcMap.left + rcMap.Width() / 2 - rcMap.Height() / 2) + (x - rcMiniMap.left) * rcMap.Height() / 1024)
#define POINTTOSCREEN_Y(y) int(int(p->Y) + rcMap.top + (1024 - (y - rcMiniMap.top)) * rcMap.Height() / 1024)
#define SCREENTOPOINT_X(x) float((x - int(p->X) - (rcMap.left + rcMap.Width() / 2 - rcMap.Height() / 2)) * 1024 / rcMap.Height() + rcMiniMap.left)
#define SCREENTOPOINT_Y(y) float(1024 - (y - int(p->Y) - rcMap.top) * 1024 / rcMap.Height() + rcMiniMap.top)

AUI_BEGIN_COMMAND_MAP(CDlgMiniMap, CDlgBase)

AUI_ON_COMMAND("close",			OnCommand_close)
AUI_ON_COMMAND("openmarkwin",	OnCommand_openmarkwin)
AUI_ON_COMMAND("bigmap",		OnCommand_bigmap)
AUI_ON_COMMAND("midmap",		OnCommand_midmap)
AUI_ON_COMMAND("roomout",		OnCommand_roomout)
AUI_ON_COMMAND("roomin",		OnCommand_roomin)
AUI_ON_COMMAND("mode",			OnCommand_mode)
AUI_ON_COMMAND("makemark",		OnCommand_makemark)
AUI_ON_COMMAND("listregion",	OnCommand_listregion)
AUI_ON_COMMAND("edit",			OnCommand_edit)
AUI_ON_COMMAND("arrow",			OnCommand_arrow)
AUI_ON_COMMAND("automove",		OnCommand_automove)
AUI_ON_COMMAND("delete",		OnCommand_delete)
AUI_ON_COMMAND("deleteall",		OnCommand_deleteall)
AUI_ON_COMMAND("Btn_Help",		OnCommand_NewHelp)
AUI_ON_COMMAND("Btn_NewerHelp",	OnCommand_NewHelp)
AUI_ON_COMMAND("Btn_RadarIntro",OnCommand_RadarHelp)
AUI_ON_COMMAND("Btn_Menology",	OnCommand_daily)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_CANCEL)
AUI_ON_COMMAND("onexp",			OnCommand_onexp)
AUI_ON_COMMAND("Btn_Auction",	OnCommand_Auction)
AUI_ON_COMMAND("Btn_Radio",		OnCommandBroadcast)
AUI_ON_COMMAND("Btn_Vip",		OnCommand_Vip)
AUI_ON_COMMAND("Btn_Gmbbs",		OnCommand_GMBbs)
AUI_ON_COMMAND("Btn_Battle_Dungeon_Tower",	OnCommand_Tower)
AUI_ON_COMMAND("Btn_UpdateDiary",	OnCommand_UpdateLog)
AUI_ON_COMMAND("Btn_AttiUp",	OnCommand_AttiUp)
AUI_ON_COMMAND("Btn_Recommend",		OnCommand_Recommand)

AUI_ON_COMMAND("Btn_ZhuxianKnows_Min",		OnCommand_ZhuxianKnows)
AUI_ON_COMMAND("Btn_Activity_Min",		OnCommand_Activity)
AUI_ON_COMMAND("Battle_Dungeon_6V6_Min",		OnCommand_Dungeon_6V6)
AUI_ON_COMMAND("Btn_Group", OnCommand_6V6)
AUI_ON_COMMAND("Btn_6V6", OnCommand_Group)
AUI_ON_COMMAND("Btn_Explorer_Popu_Min",		OnCommand_Hot)
AUI_ON_COMMAND("Btn_King_Min",		OnCommand_King)
AUI_ON_COMMAND("Btn_Radio_Min",		OnCommand_RedRadio)
AUI_ON_COMMAND("Btn_ZhuxianExplore_Min",		OnCommand_ZhuxianExplore)
AUI_ON_COMMAND("Btn_ThemeLink",		OnCommand_ThemeLink)
AUI_ON_COMMAND("Btn_Ancient",		OnCommand_Ancient)
AUI_ON_COMMAND("Btn_QuestKf",		OnCommand_QuestKf)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgMiniMap, CDlgBase)

AUI_ON_EVENT("Hide_2",		WM_MOUSEMOVE,		OnEventMouseMove_Hide_2)
AUI_ON_EVENT("Hide_2",		WM_LBUTTONUP,		OnEventLButtonUp_Hide_2)
AUI_ON_EVENT("List_Choose",	WM_LBUTTONDOWN,		OnEventLButtonDown_List_Choose)
AUI_ON_EVENT("List_Choose",	WM_RBUTTONUP,		OnEventRButtonDown_List)
AUI_ON_EVENT("List_Region",	WM_RBUTTONUP,		OnEventRButtonDown_List)

AUI_END_EVENT_MAP()

const char *szMapMarkFile[] =
{
	"InGame\\MiniMapMark\\Player.tga",
	"InGame\\MiniMapMark\\Monster.tga",
	"InGame\\MiniMapMark\\MonsterBoss.tga",
	"InGame\\MiniMapMark\\MonsterTask.tga",
	"InGame\\MiniMapMark\\Guild.tga",
	"InGame\\MiniMapMark\\Family.tga",
	"InGame\\MiniMapMark\\Friend.tga",
	"InGame\\MiniMapMark\\NPC.tga",
	"InGame\\MiniMapMark\\NPCAcceptTask.tga",
	"InGame\\MiniMapMark\\NPCFinishTask.tga",
	"InGame\\MiniMapMark\\TeamMate.tga",
	"InGame\\MiniMapMark\\Captain.tga",
	"InGame\\MiniMapMark\\Enemy.tga",
	"InGame\\MiniMapMark\\Host.tga",
	"InGame\\MiniMapMark\\Enemy.tga",
	"InGame\\MiniMapMark\\Tag0.tga",
	"InGame\\MiniMapMark\\Tag1.tga",
	"InGame\\MiniMapMark\\Tag2.tga",
	"InGame\\MiniMapMark\\Tag3.tga",
	"InGame\\MiniMapMark\\Tag4.tga",
	"InGame\\MiniMapMark\\Tag5.tga",
	"InGame\\MiniMapMark\\Tag6.tga",
	"InGame\\MiniMapMark\\Tag7.tga",
	"InGame\\MiniMapMark\\Tag8.tga",
	"InGame\\MiniMapMark\\Tag9.tga",
	"InGame\\MiniMapMark\\AutoMove.tga",
	"InGame\\MiniMapMark\\TagClick.tga",
	"InGame\\MiniMapMark\\Target.tga",
	"InGame\\MiniMapMark\\BuildingA.tga",
	"InGame\\MiniMapMark\\BuildingB.tga",
	"InGame\\MiniMapMark\\WarAttackerCenter.tga",
	"InGame\\MiniMapMark\\WarDefenderCenter.tga",
	"InGame\\MiniMapMark\\WarAttackerZone.tga",
	"InGame\\MiniMapMark\\WarDefenderZone.tga",
	"InGame\\MiniMapMark\\WarDefenderTower.tga",
	"InGame\\MiniMapMark\\WarDefenderTowerSmall.tga",
	"InGame\\MiniMapMark\\WarDefenderTowerAssist.tga",
	"InGame\\MiniMapMark\\WarDefenderWall.tga",
	"InGame\\MiniMapMark\\WarAttackerCenter1.tga",
	"InGame\\MiniMapMark\\WarDefenderCenter1.tga",
	"InGame\\MiniMapMark\\WarAttackerZone1.tga",
	"InGame\\MiniMapMark\\WarDefenderZone1.tga",
	"InGame\\MiniMapMark\\WarDefenderTower1.tga",
	"InGame\\MiniMapMark\\WarDefenderTowerSmall1.tga",
	"InGame\\MiniMapMark\\WarDefenderTowerAssist1.tga",
	"InGame\\MiniMapMark\\WarDefenderWall1.tga",
	"InGame\\MiniMapMark\\MidmapMoney.tga",
};

static const int g_nSide = 256;
static const float g_fCell = 1024.0f;
static const float g_fSide = 256.f;

int CDlgMiniMap::m_nMode;
float CDlgMiniMap::m_fZoom;
bool CDlgMiniMap::m_bShowTargetArrow;
bool CDlgMiniMap::m_bVisionLock;
A3DTexture * CDlgMiniMap::m_pA3DTexMask = NULL;
abase::vector<CDlgMiniMap::MARK> CDlgMiniMap::m_vecMark;
abase::vector<CDlgMiniMap::MAPMARK> CDlgMiniMap::m_vecMiniMapMark;
CDlgMiniMap::MAPTLVERTEX CDlgMiniMap::m_vertexMiniMap[4];
A3DStream * CDlgMiniMap::m_pMiniMapStream;
A3DTexture * CDlgMiniMap::m_pTexRadar = NULL;
AString CDlgMiniMap::m_szTexRadar = "";
A2DSprite * CDlgMiniMap::m_pA2DSpriteCompass = NULL;
A2DSprite ** CDlgMiniMap::m_pA2DMapMark = NULL;
A2DSprite * CDlgMiniMap::m_pA2DRadarBack = NULL;
DWORD CDlgMiniMap::m_dwTempTargetTime;
A3DVECTOR3 CDlgMiniMap::m_vecTempTarget;
int CDlgMiniMap::m_idTempTargetInstance = 0;
A3DVECTOR3 CDlgMiniMap::m_vecNowTarget;
CDlgMiniMap::BUILDING_STATUS CDlgMiniMap::m_WarBuildingStatus[20];
bool CDlgMiniMap::m_bOpendAuction = false;
bool CDlgMiniMap::m_bShouldOpenAuction = false;
bool CDlgMiniMap::m_bGuajiCrossDay = false;

CDlgMiniMap::CDlgMiniMap()
{
	int j;

	m_fZoom = 1.0f;
	m_nMode = MODE_B;
	m_bShowTargetArrow = true;
	m_bVisionLock	= false;
	m_dwTempTargetTime = 0;
	m_vecTempTarget = A3DVECTOR3(0.0f);
	m_vecNowTarget = A3DVECTOR3(0.0f);

	for( j = 0; j < 4; j++ )
	{
		m_vertexMiniMap[j].z = 0.0f;
		m_vertexMiniMap[j].rhw = 1.0f;
		m_vertexMiniMap[j].diffuse = 0xFFFFFFFF;
		m_vertexMiniMap[j].specular = 0xFF000000;
	}

	m_vecMark.clear();
	m_vecMiniMapMark.clear();
}

CDlgMiniMap::~CDlgMiniMap()
{
	if( m_pMiniMapStream )
	{
		m_pMiniMapStream->Release();
		delete m_pMiniMapStream;
		m_pMiniMapStream = NULL;
	}

	m_vecMark.clear();
	m_vecMiniMapMark.clear();

	A3DRELEASE(m_pA2DSpriteCompass);
	A3DRELEASE(m_pA3DTexMask);
	A3DRELEASE(m_pA2DRadarBack);
	A3DRELEASE(m_pTexRadar);
	m_szTexRadar = "";
	if( m_pA2DMapMark )
	{
		for(int i = 0; i < MAPMARK_NUM; i++ )
			A3DRELEASE(m_pA2DMapMark[i]);
		delete m_pA2DMapMark;
		m_pA2DMapMark = NULL;
	}
}

void CDlgMiniMap::OnTick()
{
	if( stricmp(GetName(), "Win_MiniMap") == 0 ||
		stricmp(GetName(),"Win_Minimap_Recommend") == 0)
	{
		ACString strLineAndTime;
		ACHAR szText[200];
		float fDNFactor = GetWorld()->GetSunMoon()->GetDNFactor();
		float fDNFactorDest = GetWorld()->GetSunMoon()->GetDNFactorDest();
		int nTimeIndex = int(48.0f * GetWorld()->GetSunMoon()->GetTimeOfTheDay() + 2) % 48;
		struct tm			serverLocal = g_pGame->GetServerLocalTime();

        if(NULL != m_pImg_ServerTime)
		    m_pImg_ServerTime->FixFrame(nTimeIndex / 4);
		CECStringTab *pDescTab = GetGame()->GetItemDesc();
		a_sprintf(szText, GetStringFromTable(604),
				pDescTab->GetWideString(ITEMDESC_TALISMAN_HOUR0 + nTimeIndex / 4),
				pDescTab->GetWideString(ITEMDESC_TALISMAN_MINUTE0 + nTimeIndex % 4),
				serverLocal.tm_hour, serverLocal.tm_min);
		if(NULL != m_pImg_ServerTime)
		   m_pImg_ServerTime->SetHint(szText);
		strLineAndTime = szText;
		a_sprintf(szText, _AL("%d"), serverLocal.tm_mday);
		if(NULL != m_pLab_Date)
		  m_pLab_Date->SetText(szText);

		if(stricmp(GetName(), "Win_MiniMap") == 0)
		{
				A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
				int nCurLine = GetGame()->GetGameRun()->GetCurLine();
				ACString strText;
				CECGameRun::LineInfoVec& v = GetGame()->GetGameRun()->GetLinesInfo();
				for (size_t n = 0; n < v.size(); n++)
				{
					CECGameRun::LineInfo& info = v[n];

					if (info.id == nCurLine)
					{
						strText.Format(GetStringFromTable(801), info.name);
						//GetDlgItem("Img_ServerLine")->SetHint(strText);

						strLineAndTime = strText + _AL("\r") + strLineAndTime;
						GetDlgItem("Img_ServerLine")->SetHint(strLineAndTime); 
						break;
					}
				}
				DWORD dwTick = GetTickCount();
				if( !m_pChk_Mail->IsChecked() )
					if( (int)m_pChk_Mail->GetData() == -1 ||
						m_pChk_Mail->GetData() != 0 && 
						GetGame()->GetServerGMTTime() - (int)m_pChk_Mail->GetData() > 0 )
					{
						m_pChk_Mail->SetData(0);
						GetGameSession()->mail_CheckNew();
					}
				CECHostPlayer::BATTLEINFO binfo = GetHostPlayer()->GetBattleInfo();
				PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_WarStat");
				if( pDlg )
				{
					if( binfo.idBattle )
					{
						pDlg->SetEnable(false);
						if( !pDlg->IsShow() )
							pDlg->Show(true);
						pDlg->SetCanMove(false);
						int nRestTime = binfo.iEndTime - GetGame()->GetServerGMTTime();
						if( nRestTime < 0 )
							nRestTime = 0;
						if( nRestTime >= 60 * 120 )
						{
							pDlg->GetDlgItem("Lab_GameOver")->Show(false);
							pDlg->GetDlgItem("Lab_GateOpen")->Show(true);
							a_sprintf(szText, GetStringFromTable(1602), nRestTime / 60 - 120);
						}
						else
						{
							pDlg->GetDlgItem("Lab_GameOver")->Show(true);
							pDlg->GetDlgItem("Lab_GateOpen")->Show(false);
							if( nRestTime >= 60 )
								a_sprintf(szText, GetStringFromTable(1602), nRestTime / 60);
							else
								a_sprintf(szText, GetStringFromTable(1601), nRestTime);
						}
							pDlg->GetDlgItem("Lab_Time")->SetText(szText);
						a_sprintf(szText, _AL("%d"), binfo.iFlagAttacker);
						pDlg->GetDlgItem("Lab_11")->SetText(szText);
						a_sprintf(szText, _AL("%d"), binfo.iFlagDefender);
						pDlg->GetDlgItem("Lab_12")->SetText(szText);
						a_sprintf(szText, _AL("%d"), binfo.iResAttacker);
						pDlg->GetDlgItem("Lab_13")->SetText(szText);
						a_sprintf(szText, _AL("%d"), binfo.iResDefender);
						pDlg->GetDlgItem("Lab_14")->SetText(szText);
						PAUIIMAGEPICTURE pImage1 = (PAUIIMAGEPICTURE)pDlg->GetDlgItem("Img_GuildSign1");
						int nIndex1 = 0;
						A2DSprite *sprite1 = GetGameUIMan()->GetFactionIcon(GetGame()->GetGameInit().iServerID, binfo.iAttackerFaction, &nIndex1);
						pImage1->SetCover(sprite1, nIndex1);
						PAUIIMAGEPICTURE pImage2 = (PAUIIMAGEPICTURE)pDlg->GetDlgItem("Img_GuildSign2");
						int nIndex2 = 0;
						A2DSprite *sprite2 = GetGameUIMan()->GetFactionIcon(GetGame()->GetGameInit().iServerID, binfo.iDefenderFaction, &nIndex2);
						pImage1->SetCover(sprite2, nIndex2);
					}
					else
						pDlg->Show(false);
				}

		}
// 		PAUICHECKBOX virsionLock;
// 		virsionLock = (PAUICHECKBOX)GetDlgItem("Chk_VisionLock");
// 		EC_SYSTEM_SETTING setting;
// 		setting = GetGame()->GetConfigs()->GetSystemSettings();
// 		m_bVisionLock = setting.bLockCamera;
// 		virsionLock->Check(!m_bVisionLock);
		PAUICHECKBOX onExp = (PAUICHECKBOX)GetDlgItem("Chk_OnExp");
		if(NULL != onExp)
		{
			onExp->Check(GetHostPlayer()->IsSitting());
		}

		m_bShouldOpenAuction = false;
		tm tmNow = GetGame()->GetServerLocalTime();
		if (!m_bOpendAuction)
		{
			if (tmNow.tm_hour==20)//客户端写死，晚上8:00-9:00按钮闪，直到点击
			{
				m_bShouldOpenAuction = true;
			}
		}

		if(m_pBtn_Auction)
		   m_pBtn_Auction->SetFlash(m_bShouldOpenAuction);

		if(stricmp(GetName(), "Win_MiniMap") == 0)
		{
			if (tmNow.tm_hour==0 && tmNow.tm_min==0)	// 跨天挂机取vip奖励信息
			{
				if (tmNow.tm_sec<10)
				{
					m_bGuajiCrossDay = true;
				}
				else if (tmNow.tm_sec<20 && m_bGuajiCrossDay)
				{
					m_bGuajiCrossDay = false;
					GetGameSession()->c2s_CmdGetVipAwardInfo();
				}
			}

		}

		if (RadioIsPlaying())
		{
			CECSecretary* pSecretary = GetGame()->GetGameRun()->GetSecretary();
			if(m_pBtn_Radio && pSecretary)
			   m_pBtn_Radio->SetFlash(pSecretary->CheckEventState(CECSecretary::FIRST_OPEN_RADIO));
		}
		else if (m_pBtn_Radio && m_pBtn_Radio->GetFlash())
		{
			m_pBtn_Radio->SetFlash(false);
		}

		if(NULL != m_pAncient)
		   m_pAncient->Enable(g_pGame->GetGameRun()->GetWorld()->GetInstanceID() == 44);
		if(NULL != m_pQuestKf)
		   m_pQuestKf->Enable(GetHostPlayer()->GetSrcServerID()!=0);

		if(stricmp(GetName(),"Win_Minimap_Recommend") == 0)
		{
			if (GetHostPlayer()->GetKingdomKingId() != 0)
			{
				GetDlgItem("Btn_King_Min")->Enable(true);
				GetDlgItem("Btn_King_Min")->SetHint(GetStringFromTable(15647));
			}
			else
			{
				GetDlgItem("Btn_King_Min")->Enable(false);
				GetDlgItem("Btn_King_Min")->SetHint(GetStringFromTable(15648));
			}

		}
		   

	}
}

void CDlgMiniMap::Resize(A3DRECT rcOld, A3DRECT rcNew)
{
	CDlgBase::Resize(rcOld, rcNew);
	float fWindowScale = GetGameUIMan()->GetWindowScale();
	m_pA2DSpriteCompass->SetScaleX(fWindowScale);
	m_pA2DSpriteCompass->SetScaleY(fWindowScale);
	m_pA2DRadarBack->SetScaleX(fWindowScale);
	m_pA2DRadarBack->SetScaleY(fWindowScale);
	for(int i = 0; i < MAPMARK_NUM; i++ )
	{
		m_pA2DMapMark[i]->SetScaleX(fWindowScale);
		m_pA2DMapMark[i]->SetScaleY(fWindowScale);
	}
}

bool CDlgMiniMap::OnInitDialog()
{
	bool bval;

	m_pImg_ServerTime = (PAUIIMAGEPICTURE)GetDlgItem("Img_ServerTime");
	if( stricmp(GetName(), "Win_MiniMap") == 0 ||
		stricmp(GetName(), "Win_MiniMapB") == 0 ||
		stricmp(GetName(), "Win_Minimap_Recommend") == 0)

	{
		m_pLab_Date = (PAUILABEL)GetDlgItem("Lab_Day");
		m_pBtn_Auction = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Auction");
		m_pBtn_Radio = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Radio");
		m_pChk_Mail = (PAUICHECKBOX)GetDlgItem("Chk_Mail");
		if(NULL != m_pChk_Mail)
		{
			m_pChk_Mail->SetAcceptMouseMessage(false);
			m_pChk_Mail->Check(false);
			m_pChk_Mail->SetHint(_AL(""));
			m_pChk_Mail->SetData(-1);
		}
		m_pLab_MapName = (PAUILABEL)GetDlgItem("Txt_Name");
		m_bOpendAuction = false;
		m_bShouldOpenAuction = false;
		
		//GetDlgItem("Shape_1")->Show(false);
		//GetDlgItem("Img_1")->Show(false);
		//GetDlgItem("Txt_1")->Show(false);
		m_pAncient = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Ancient");
		m_pQuestKf = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_QuestKf");
	}

	 if(stricmp(GetName(), "Win_Minimap_Recommend") == 0)
	 {
		 GetDlgItem("Btn_Group")->Show(false);
		 GetDlgItem("Btn_6V6")->Show(false);
	 }

	// Mini map dir.
	if( !m_pA2DSpriteCompass )
	{
		m_pA2DSpriteCompass = new A2DSprite;
		if( !m_pA2DSpriteCompass ) return AUI_ReportError(__LINE__, __FILE__);

		bval = m_pA2DSpriteCompass->Init(m_pA3DDevice, "InGame\\MiniMapDir.tga", 0);
		if( !bval ) return AUI_ReportError(__LINE__, __FILE__);
		m_pA2DSpriteCompass->SetLocalCenterPos(
			m_pA2DSpriteCompass->GetWidth() / 2,
			m_pA2DSpriteCompass->GetHeight() / 2);
		m_pA2DSpriteCompass->SetLinearFilter(true);
	}

	// Mini map mask.
	if( !m_pA3DTexMask )
	{
		m_pA3DTexMask = new A3DTexture;
		AString strFile = "Surfaces\\InGame\\MiniMapMask.tga";
		bval = m_pA3DTexMask->LoadFromFile(m_pA3DDevice, strFile, strFile);
		if( !bval ) return AUI_ReportError(__LINE__, __FILE__);
	}

	// Map marks.
	if( !m_pA2DMapMark )
	{
		m_pA2DMapMark = new A2DSprite*[MAPMARK_NUM];
		for(int k = 0; k < MAPMARK_NUM; k++ )
		{
			m_pA2DMapMark[k] = new A2DSprite;
			if( !m_pA2DMapMark[k] ) return AUI_ReportError(__LINE__, __FILE__);

			bval = m_pA2DMapMark[k]->Init(m_pA3DDevice, szMapMarkFile[k], AUI_COLORKEY);
			if( !bval ) return AUI_ReportError(__LINE__, __FILE__);

			m_pA2DMapMark[k]->SetLocalCenterPos(m_pA2DMapMark[k]->GetWidth() / 2, 
				m_pA2DMapMark[k]->GetHeight() / 2);
		}
	}

	if( !m_pA2DRadarBack )
	{
		m_pA2DRadarBack = new A2DSprite;
		if( !m_pA2DRadarBack ) return AUI_ReportError(__LINE__, __FILE__);

		bval = m_pA2DRadarBack->Init(m_pA3DDevice, "Window\\对话框_标准边框4_底.tga", AUI_COLORKEY);
		if( !bval ) return AUI_ReportError(__LINE__, __FILE__);
	}

	// now create a dynamic stream for map render
	if( !m_pMiniMapStream )
	{
		m_pMiniMapStream = new A3DStream();
		if( !m_pMiniMapStream->Init(m_pA3DDevice, sizeof(MAPTLVERTEX),
			FVF_MAPTLVERTEX, 6, 0, A3DSTRM_REFERENCEPTR, 0) )
		{
			return AUI_ReportError(__LINE__, __FILE__);
		}
	}

	return true;
}

void CDlgMiniMap::OnCommand_close(const char * szCommand)
{
	if( m_szName == "Win_MiniMapClose" )
	{
		((PAUIDIALOG)GetDataPtr())->Show(true);
		Show(false);
	}
	else
	{
		GetGameUIMan()->GetDialog("Win_MiniMapClose")->Show(true);
		GetGameUIMan()->GetDialog("Win_MiniMapClose")->SetDataPtr(this);
		Show(false);
	}
}

void CDlgMiniMap::OnCommand_openmarkwin(const char * szCommand)
{
	POINT ptPos = GetPos();
	PAUIDIALOG pShow = GetGameUIMan()->m_pDlgMiniMapMark;
	
	// old : pShow->SetPos(ptPos.x, ptPos.y + 46);
	pShow->SetPosEx(ptPos.x - pShow->GetSize().cx, ptPos.y);
	
	pShow->Show(!pShow->IsShow());

	ACHAR szText[40];
	A3DVECTOR3 vecPosHost = GetHostPlayer()->GetPos();

	a_sprintf(szText, _AL("%d %d"),
		int(vecPosHost.x), int(vecPosHost.z));
	pShow->GetDlgItem("Txt_Mark")->SetText(szText);
}

void CDlgMiniMap::OnCommand_midmap(const char * szCommand)
{
	if(g_pGame->GetGuildEnter())
	{
		DlgGuildLandBuild* pDlgLandBuild = (DlgGuildLandBuild*)GetGameUIMan()->GetDialog("Win_Guild_LandBuild");
		pDlgLandBuild->Show(!pDlgLandBuild->IsShow());
	}
	else
	{
		GetGameRun()->GetHelpGraphGuide()->FinishHelp(CECHelpGraphGuide::HELP2012_8_1_MAP_HOTKEY);

		if( GetGameUIMan()->m_pDlgMidMap->IsShow() )
			GetGameUIMan()->m_pDlgMidMap->Show(false);
		else
		{
			if( GetWorld()->GetInstanceID() == 15 && 
				GetHostPlayer()->GetPos().x > -473 && GetHostPlayer()->GetPos().x < -381 &&
				GetHostPlayer()->GetPos().z > -444 && GetHostPlayer()->GetPos().z < -194)
				return;

			GetGameUIMan()->m_pDlgMidMap->SetPosEx(0, 0, alignCenter, alignCenter);
			GetGameUIMan()->m_pDlgMidMap->Show(true);
			GetGameUIMan()->m_pDlgMidMap->SetCanMove(false);
		}
	}
}

void CDlgMiniMap::OnCommand_bigmap(const char * szCommand)
{
	if( GetWorld()->GetInstanceID() == 15 && 
		GetHostPlayer()->GetPos().x > -473 && GetHostPlayer()->GetPos().x < -381 &&
		GetHostPlayer()->GetPos().z > -444 && GetHostPlayer()->GetPos().z < -194)
		return;

	PAUIDIALOG pShow = GetGameUIMan()->m_pDlgWorldMap;

	bool bShow = !pShow->IsShow();
	pShow->Show(bShow);
}

void CDlgMiniMap::OnCommand_roomout(const char * szCommand)
{
	if( m_fZoom > 0.5f )
		m_fZoom /= 2.0f;
}

void CDlgMiniMap::OnCommand_roomin(const char * szCommand)
{
	if( m_fZoom < 2.0f )
		m_fZoom *= 2.0f;
}

void CDlgMiniMap::OnCommand_mode(const char * szCommand)
{
	if( m_nMode == MODE_A )
		m_nMode = MODE_B;
	else
		m_nMode = MODE_A;
}

void CDlgMiniMap::OnCommand_makemark(const char * szCommand)
{
	if( m_vecMark.size() >= CECGAMEUIMAN_MAX_MARKS )
	{
		GetGameUIMan()->AddChatMessage(GetStringFromTable(507), GP_CHAT_MISC);
		return;
	}

	ACString strText = GetDlgItem("Txt_Mark")->GetText();
	if( strText.GetLength() <= 0 ) return;

	int i;
	CSplit s(strText);
	CSPLIT_STRING_VECTOR vec;

	for( i = 266; i <= 269; i++ )
	{
		vec = s.Split(GetStringFromTable(i));
		if( vec.size() >= 2 ) break;
	}

	if( vec.size() < 2 )
	{
		GetGameUIMan()->AddChatMessage(GetStringFromTable(237), GP_CHAT_MISC);
		return;
	}

	GetGameUIMan()->m_vecTargetPos.x = (float)a_atoi(vec[0]);
	GetGameUIMan()->m_vecTargetPos.z = (float)a_atoi(vec[1]);
	GetGameUIMan()->m_vecTargetPos.y = g_pGame->GetGameRun()->GetWorld()->GetTerrainHeight(GetGameUIMan()->m_vecTargetPos);

	GetGameUIMan()->m_pDlgInputName->Show(true, true);
	GetGameUIMan()->m_pDlgInputName->m_pTxtInput->SetText(_AL(""));
	GetGameUIMan()->m_pDlgInputName->SetData(CDlgInputName::INPUTNAME_MAKE_MARK);
}

void CDlgMiniMap::OnCommand_listregion(const char * szCommand)
{
	if( m_vecMark.size() >= CECGAMEUIMAN_MAX_MARKS )
	{
		GetGameUIMan()->AddChatMessage(GetStringFromTable(507), GP_CHAT_MISC);
		return;
	}

	int i;
	PAUILISTBOX pListRegion = (PAUILISTBOX)GetDlgItem("List_Region");
	PAUILISTBOX pListChoose = (PAUILISTBOX)GetDlgItem("List_Choose");

	MARK Mark;
	ACHAR szText[40];
	CELPrecinct::MARK *pMark = GetGameUIMan()->m_pCurPrecinct->GetMark(pListRegion->GetCurSel());

	Mark.nNPC = pListRegion->GetCurSel();
	Mark.strName = pMark->strName;
	Mark.vecPos = pMark->vPos;
	Mark.idInstance = GetWorld()->GetInstanceID();
	a_sprintf(szText, _AL("%d,%d"),
		int(pMark->vPos.x), int(pMark->vPos.z));
	
	for( i = 0; i < (int)m_vecMark.size(); i++ )
	{
		if( m_vecMark[i].nNPC >= 0 && 
			m_vecMark[i].strName == pListRegion->GetText(pListRegion->GetCurSel()) )
		{
			m_vecMark.erase(m_vecMark.begin() + i);
			m_vecMark.insert(m_vecMark.begin() + i, Mark);
			pListChoose->SetItemHint(i, szText);
			return;
		}
	}

	m_vecMark.push_back(Mark);
	pListChoose->AddString(pMark->strName);
	pListChoose->SetItemHint(pListChoose->GetCount() - 1, szText);
}

void CDlgMiniMap::OnCommand_edit(const char * szCommand)
{
	PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("List_Choose");
	int nCurSel = pList->GetCurSel();
	if( nCurSel < 0 || nCurSel >= pList->GetCount() ) return;
	
	if( m_vecMark[nCurSel].nNPC >= 0 )
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(511),
			MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	}
	else
	{
		ACString strName = pList->GetText(nCurSel);
		PAUIDIALOG pDlgInuptString11 = GetGameUIMan()->GetDialog("Win_InputString11");
		PAUITEXTAREA pTxt = (PAUITEXTAREA)pDlgInuptString11->GetDlgItem("DEFAULT_Txt_Input");
		pTxt->SetText(strName);
		pTxt = (PAUITEXTAREA)pDlgInuptString11->GetDlgItem("Txt_NewLocation");
		strName.Format(_AL("%.0f %.0f"), m_vecMark[nCurSel].vecPos.x, m_vecMark[nCurSel].vecPos.z);
		pTxt->SetText(strName);

		pDlgInuptString11->Show(true, true);
		pDlgInuptString11->SetData(CDlgInputName::INPUTNAME_RENAME_MARK);
		pDlgInuptString11->SetDataPtr((void*)nCurSel);
	}
}

void CDlgMiniMap::OnCommand_arrow(const char * szCommand)
{
	m_bShowTargetArrow = !m_bShowTargetArrow;
}

void CDlgMiniMap::OnCommand_automove(const char * szCommand)
{
	PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("List_Choose");
	int nCurSel = pList->GetCurSel();

	if( nCurSel >= 0 && nCurSel < pList->GetCount() )
	{
		g_ECAutoMove.SearchPath(GetHostPlayer()->GetPos(), m_vecMark[nCurSel].vecPos, m_vecMark[nCurSel].idInstance);
	}
}

void CDlgMiniMap::OnCommand_delete(const char * szCommand)
{
	PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("List_Choose");

	if( pList->GetCurSel() >= 0 && pList->GetCurSel() < pList->GetCount() )
	{
		PAUIDIALOG pMsgBox = NULL;
		GetGameUIMan()->MessageBox("Game_DelMark", GetStringFromTable(509),
			MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		pMsgBox->SetData(pList->GetCurSel());
	}
}

void CDlgMiniMap::OnCommand_deleteall(const char * szCommand)
{
	PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("List_Choose");

	if( pList->GetCount() > 0 )
	{
		GetGameUIMan()->MessageBox("Game_DelAllMarks", GetStringFromTable(510),
			MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160));
	}
}

void CDlgMiniMap::OnCommand_RadarHelp(const char * szCommand)
{
	PAUIDIALOG pDlg;
	if (GetHostPlayer()->GetSrcServerID()==0)
		pDlg = GetGameUIMan()->GetDialog("Win_RadarHelp");
	else
		pDlg = GetGameUIMan()->GetDialog("Win_KfpkRadarHelp");
	pDlg->Show(!pDlg->IsShow());
	pDlg->AlignTo(this, AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_LEFT, AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
}

void CDlgMiniMap::OnCommand_daily(const char * szCommand)
{
	//PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Menology");
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_DailyPromp_Today"); //2012-12-5 add 修改显示界面 by zy
	
	pDlg->Show(!pDlg->IsShow());
}

void CDlgMiniMap::OnCommand_CANCEL(const char * szCommand)
{
	if( m_szName == "Win_Mark" || m_szName == "Win_Minimap_Recommend")
		Show(false);
	else
		m_pAUIManager->RespawnMessage();
}

void CDlgMiniMap::OnCommand_onexp(const char * szCommand)
{
	c2s_SendCmdStartOnlineAgent();
}

void CDlgMiniMap::OnCommand_Auction(const char * szCommand)
{
	if (GetGameUIMan()->GetDialog("Win_Auction")->IsShow())
	{
		GetGameUIMan()->GetDialog("Win_Auction")->Show(false);
	}
	else
		GetGameSession()->unique_bid_get_history();
	m_bOpendAuction = true;
}

void CDlgMiniMap::OnCommandBroadcast(const char * szCommand)
{
	ACString strText;
	AWIniFile iniFile;
	if(iniFile.Open("configs\\broadcast.ini"))
	{
		strText = iniFile.GetValueAsString(_AL("Channel"), _AL("ChannelURL0"), _AL(""));
		iniFile.Close();
		if( strText.GetLength() > 0 )
		{
			CDlgBroadCastInGame* pDlgRadioInGame = (CDlgBroadCastInGame*)GetGameUIMan()->GetDialog("Win_Radio");
			pDlgRadioInGame->Show(true);
			pDlgRadioInGame->NavigateUrl(strText);
		}
	}

	CECSecretary* pSecretary = GetGame()->GetGameRun()->GetSecretary();
	if (pSecretary->CheckEventState(CECSecretary::FIRST_OPEN_RADIO))
	{
		if(m_pBtn_Radio)
		   m_pBtn_Radio->SetFlash(false);
		pSecretary->SetEventState(CECSecretary::FIRST_OPEN_RADIO);
	}
}

void CDlgMiniMap::OnCommand_Vip(const char * szCommand)
{
	if (GetHostPlayer()->GetSrcServerID()==0)
	{
		PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_QShop_vip");
		if( pDlg )
		{
			pDlg->Show(!pDlg->IsShow());
		}
	}
	else
		GetGameUIMan()->MessageBox(12999);
}

void CDlgMiniMap::OnCommand_GMBbs(const char * szCommand)
{
	AString strText = "";
	AIniFile iniFile;
	if(iniFile.Open("configs\\url.ini"))
	{
		strText = iniFile.GetValueAsString("ingame", "gmbbs", "");
		iniFile.Close();
	}
	if( strText != "" )
		ShellExecuteA(NULL,"open", strText, NULL, NULL, SW_SHOWNORMAL);
}

void CDlgMiniMap::OnCommand_Tower(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_CharDungeon");
	pDlg->Show(!pDlg->IsShow());
}

void CDlgMiniMap::OnCommand_UpdateLog(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_UpdateDiary");
	pDlg->Show(!pDlg->IsShow());
}

void CDlgMiniMap::OnCommand_AttiUp(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_AttriUp");
	pDlg->Show(!pDlg->IsShow());
}

void CDlgMiniMap::OnCommand_NewHelp(const char * szCommand)
{
	//PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_NewerHelp2");
	//pDlg->Show(!pDlg->IsShow());
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Help_GameGuide");
	pDlg->Show(!pDlg->IsShow());
}

bool CDlgMiniMap::Render(void)
{
	if( m_szName == "Win_MiniMap" || m_szName == "Win_MiniMapB")
		UpdateMiniMap();

	return CDlgBase::Render();
}

bool CDlgMiniMap::RadioIsPlaying()
{
	tm tmNow = GetGame()->GetServerLocalTime();
	return (tmNow.tm_hour>=15&&tmNow.tm_hour<17) || (tmNow.tm_hour>=20&&tmNow.tm_hour<22);
}

bool CDlgMiniMap::UpdateMiniMap()
{
	PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)GetDlgItem("Hide_2");
	pImage->SetRenderCallback(NULL, (DWORD)this);

	if( GetGameUIMan()->m_pDlgWorldMap->IsShow() )
	{
		return true;
	}
#ifdef BACK_VERSION
	A3DCamera *pA3DCamera = g_pGame->GetViewport()->GetA3DCamera();
#else
	A3DCameraBase *pA3DCamera = g_pGame->GetViewport()->GetA3DCameraBase();
#endif
	A3DVECTOR3 vecDir = GetHostPlayer()->GetDir();
	int nDegree = 90 - (int)RAD2DEG((float)atan2(vecDir.z, vecDir.x));

	if( MODE_A == m_nMode )
	{
		m_pA2DSpriteCompass->SetDegree(270 - (int)pA3DCamera->GetDeg());
		m_pA2DMapMark[MAPMARK_HOST]->SetDegree(270 - (int)pA3DCamera->GetDeg() + nDegree);
	}
	else
	{
		m_pA2DSpriteCompass->SetDegree(0);
		m_pA2DMapMark[MAPMARK_HOST]->SetDegree(nDegree);
	}

	ACString strText;
	A3DVECTOR3 vecPosHost = GetHostPlayer()->GetPos();
	CELPrecinct *pPrecinct = GetWorld()->GetCurPrecinct();
	strText.Format(GetStringFromTable(211), pPrecinct ? pPrecinct->GetName() : _AL(""),
		int(vecPosHost.x), int(vecPosHost.z));
	m_pLab_MapName->SetText(strText);

	int idInstance = GetWorld()->GetInstanceID();
	CECInstance *pInstance = GetGameRun()->GetInstance(idInstance);

	POINT pt = pImage->GetPos();
	m_pA2DRadarBack->DrawToBack(pt.x, pt.y);

	A3DRECT rcMiniMap = pImage->GetRect();
	if( rcMiniMap.Width() < rcMiniMap.Height())
		rcMiniMap.bottom = rcMiniMap.top + rcMiniMap.Width();
	else if( rcMiniMap.Width() > rcMiniMap.Height())
		rcMiniMap.right= rcMiniMap.left + rcMiniMap.Height();

	if( rcMiniMap.Width() % 2 != 0 )
		rcMiniMap.right -= 1;
	if( rcMiniMap.Height() % 2 != 0  )
		rcMiniMap.bottom -= 1;
	int W = rcMiniMap.Width();
	int H = rcMiniMap.Height();
	A3DPOINT2 C = rcMiniMap.CenterPoint();
	
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int nRow = pInstance->GetRowNum();
	int nCol = pInstance->GetColNum();
	AString strSubPath = pInstance->GetPath();

	A3DPOINT2 ptHost;
	ARectI rcHost;
	ARectF rcRenderArea;

	float fOffsetX = nCol * g_fCell;
	float fOffsetY = nRow * g_fCell;

	float fZoom = float(DEFAULT_RADAR_DIAMETER) / W * m_fZoom;
	float fRatio = 512.0f / 1024.0f / fZoom;
	float fNormal = fRatio;
	if( m_fZoom < 1.0f )
		fNormal = fRatio * m_fZoom;
	vecPosHost.x = float(floor(vecPosHost.x * fNormal) / fNormal);
	vecPosHost.z = float(floor(vecPosHost.z * fNormal) / fNormal);
	ptHost.x = g_nSide + int((vecPosHost.x + fOffsetX/2) * 512.f / 1024.0f);
	ptHost.y = g_nSide + int((fOffsetY/2 - vecPosHost.z) * 512.f / 1024.0f);
	
	rcHost.left = ptHost.x - int(DEFAULT_RADAR_DIAMETER * m_fZoom / 2);
	rcHost.top = ptHost.y - int(DEFAULT_RADAR_DIAMETER * m_fZoom / 2);
	rcHost.right = rcHost.left + int(DEFAULT_RADAR_DIAMETER * m_fZoom);
	rcHost.bottom = rcHost.top + int(DEFAULT_RADAR_DIAMETER * m_fZoom);

	rcRenderArea.SetRect(-W / 2.0f, -H / 2.0f, W / 2.0f, H / 2.0f);
	if( rcHost.left < 0 )
		rcRenderArea.left -= rcHost.left / fZoom;
	if( rcHost.right > nCol*512 + 512 )
		rcRenderArea.right -= (rcHost.right - 1024) / fZoom;
	if( rcHost.top < 0 )
		rcRenderArea.top -= rcHost.top / fZoom;
	if( rcHost.bottom > nRow*512 + 512 )
		rcRenderArea.bottom -= (rcHost.bottom - 1024) / fZoom;
	rcHost &= A3DRECT(0, 0, nCol*512 + 512, nRow*512 + 512);

	int j;
	bool bval;
	AString strIndex;

	strSubPath += pInstance->GetSubPath(GetHostPlayer()->GetPos().x, GetHostPlayer()->GetPos().z);
	AString strFile = AString("Surfaces\\MiniMaps\\") + strSubPath + ".dds";
	if( strFile != m_szTexRadar )
	{
		if( !m_pTexRadar )
			m_pTexRadar = new A3DTexture;
		else
			m_pTexRadar->Release();
		m_pTexRadar->SetNoDownSample(true);
		m_pTexRadar->LoadFromFile(m_pA3DDevice, strFile, strFile);
		m_szTexRadar = strFile;
	}

	m_vertexMiniMap[0].x = rcRenderArea.left;
	m_vertexMiniMap[0].y = rcRenderArea.top;
	m_vertexMiniMap[1].x = rcRenderArea.right;
	m_vertexMiniMap[1].y = rcRenderArea.top;
	m_vertexMiniMap[2].x = rcRenderArea.left;
	m_vertexMiniMap[2].y = rcRenderArea.bottom;
	m_vertexMiniMap[3].x = rcRenderArea.right;
	m_vertexMiniMap[3].y = rcRenderArea.bottom;

	A3DMATRIX4 m;
	if( MODE_A == m_nMode )
	{
		A3DVECTOR3 vecDir = pA3DCamera->GetDirH();
		m = TransformMatrix(g_vAxisZ,
			A3DVECTOR3(vecDir.x, vecDir.z, 0.0f),
			A3DVECTOR3(float(C.x), float(C.y), 0.0f));
	}

	m_pA3DDevice->SetLightingEnable(false);
#ifdef BACK_VERSION
	m_pA3DDevice->SetDeviceVertexShader(FVF_MAPTLVERTEX);
#else
	m_pA3DDevice->SetFVF(FVF_MAPTLVERTEX);
#endif
	m_pA3DDevice->ClearPixelShader();
	m_pA3DDevice->ClearTexture(0);
	m_pA3DTexMask->Appear(1);

	m_pA3DDevice->SetTextureAddress(0, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	m_pA3DDevice->SetTextureColorArgs(0, A3DTA_TEXTURE, A3DTA_CURRENT);
	m_pA3DDevice->SetTextureColorOP(0, A3DTOP_MODULATE);
	m_pA3DDevice->SetTextureAlphaArgs(0, A3DTA_TEXTURE, A3DTA_CURRENT);
	m_pA3DDevice->SetTextureAlphaOP(0, A3DTOP_MODULATE);
	m_pA3DDevice->SetTextureColorArgs(1, A3DTA_TEXTURE, A3DTA_CURRENT);
	m_pA3DDevice->SetTextureColorOP(1, A3DTOP_SELECTARG2);
	m_pA3DDevice->SetTextureAlphaArgs(1, A3DTA_TEXTURE, A3DTA_CURRENT);
	m_pA3DDevice->SetTextureAlphaOP(1, A3DTOP_SELECTARG1);
	m_pA3DDevice->SetTextureTransformFlags(1, A3DTTFF_DISABLE);

	static WORD a_wIndex[6] = { 0, 1, 2, 2, 1, 3 };
	A3DVECTOR3 vecPos = A3DVECTOR3(0.0f, 0.0f, 0.0f);
	m_pTexRadar->Appear(0);

//	rcHost &= A3DRECT(0, 0, 512, 512);
	m_vertexMiniMap[0].tu = rcHost.left		/ g_fSide / (2+2*nCol);
	m_vertexMiniMap[0].tv = rcHost.top		/ g_fSide / (2+2*nRow);
	m_vertexMiniMap[1].tu = rcHost.right	/ g_fSide / (2+2*nCol);
	m_vertexMiniMap[1].tv = rcHost.top		/ g_fSide / (2+2*nRow);
	m_vertexMiniMap[2].tu = rcHost.left		/ g_fSide / (2+2*nCol);
	m_vertexMiniMap[2].tv = rcHost.bottom	/ g_fSide / (2+2*nRow);
	m_vertexMiniMap[3].tu = rcHost.right	/ g_fSide / (2+2*nCol);
	m_vertexMiniMap[3].tv = rcHost.bottom	/ g_fSide / (2+2*nRow);

	for( j = 0; j < 4; j++ )	
	{
		m_vertexMiniMap[j].tu2 = float(m_vertexMiniMap[j].x
			+ C.x - rcMiniMap.left) / float(W);
		m_vertexMiniMap[j].tv2 = float(m_vertexMiniMap[j].y
			+ C.y - rcMiniMap.top) / float(H);

		if( MODE_A == m_nMode )
		{
			vecPos.x = m_vertexMiniMap[j].x;
			vecPos.y = m_vertexMiniMap[j].y;
			vecPos = vecPos * m;
			m_vertexMiniMap[j].x = vecPos.x + p->X;
			m_vertexMiniMap[j].y = vecPos.y + p->Y;
		}
		else
		{
			m_vertexMiniMap[j].x += C.x + p->X;
			m_vertexMiniMap[j].y += C.y + p->Y;
		}
		m_vertexMiniMap[j].x -= 0.5f;
		m_vertexMiniMap[j].y -= 0.5f;
	}

	MAPTLVERTEX * pVerts;
	if( !m_pMiniMapStream->LockVertexBuffer(0, 0, (BYTE**) &pVerts, 0) )
		return false;

	pVerts[0] = m_vertexMiniMap[0];
	pVerts[1] = m_vertexMiniMap[1];
	pVerts[2] = m_vertexMiniMap[2];

	pVerts[3] = m_vertexMiniMap[2];
	pVerts[4] = m_vertexMiniMap[1];
	pVerts[5] = m_vertexMiniMap[3];

	m_pMiniMapStream->UnlockVertexBuffer();
	m_pMiniMapStream->Appear();

	bval = m_pA3DDevice->DrawPrimitive(A3DPT_TRIANGLELIST, 0, 2);
	if( !bval ) return AUI_ReportError(__LINE__, __FILE__);

	m_pTexRadar->Disappear(0);
	
	m_pA3DDevice->SetTextureColorOP(1, A3DTOP_DISABLE);
	m_pA3DDevice->SetTextureAlphaOP(1, A3DTOP_DISABLE);
	m_pA3DDevice->SetTextureAddress(0, A3DTADDR_WRAP, A3DTADDR_WRAP);
	
	m_pA3DTexMask->Disappear(1);
	m_pA3DDevice->SetLightingEnable(true);

	bval = m_pA2DSpriteCompass->DrawToInternalBuffer((float)(p->X + C.x - 1), (float)(p->Y + C.y - 1));
	if( !bval ) return AUI_ReportError(__LINE__, __FILE__);

	// Set render callback.
	pImage->SetRenderCallback(MiniMapRenderCallback, (DWORD)this);
	
	return true;
}

int compare( const void *arg1, const void *arg2 )
{
	return ((CDlgMiniMap::RadarPoint*)arg1)->type - ((CDlgMiniMap::RadarPoint*)arg2)->type;
}

void CDlgMiniMap::MiniMapRenderCallback(const A3DRECT &rc, DWORD param1, DWORD param2, DWORD param3)
{
	CDlgMiniMap *pDlgMap = (CDlgMiniMap *)param1;
	PAUIOBJECT pObjMiniMap = pDlgMap->GetDlgItem("Hide_2");
	if( !pObjMiniMap->IsShow() ) return;	// Mini map is minimized.

	float fWindowScale = pDlgMap->GetAUIManager()->GetWindowScale();
	float fSize = fWindowScale / 2;
	A3DRECT rcMiniMap = pObjMiniMap->GetRect();
	if( rcMiniMap.Width() < rcMiniMap.Height())
		rcMiniMap.bottom = rcMiniMap.top + rcMiniMap.Width();
	else if( rcMiniMap.Width() > rcMiniMap.Height())
		rcMiniMap.right= rcMiniMap.left + rcMiniMap.Height();

	if( rcMiniMap.Width() % 2 != 0 )
		rcMiniMap.right -= 1;
	if( rcMiniMap.Height() % 2 != 0  )
		rcMiniMap.bottom -= 1;
	int W = rcMiniMap.Width();
	int H = rcMiniMap.Height();
	A3DPOINT2 C = rcMiniMap.CenterPoint();

	int i;
	A3DPOINT2 ptPos;
	A3DVECTOR3 vecPos;
	CECHostPlayer *pHost = pDlgMap->GetHostPlayer();
	A3DVECTOR3 vecPosHost = pHost->GetPos();
	int idTarget = pHost->GetSelectedTarget();
	float fZoom = float(DEFAULT_RADAR_DIAMETER) / W * m_fZoom;
	float fRatio = 512.0f / 1024.0f / fZoom;
	float fNormal = fRatio;
	if( m_fZoom < 1.0f )
		fNormal = fRatio * m_fZoom;
	vecPosHost.x = float(floor(vecPosHost.x * fNormal) / fNormal);
	vecPosHost.z = float(floor(vecPosHost.z * fNormal) / fNormal);
	float fRadius = (float)SQUARE(min(W, H) / 2 - 2);
#ifdef BACK_VERSION
	A3DCamera *pA3DCamera = g_pGame->GetViewport()->GetA3DCamera();
#else
	A3DCameraBase *pA3DCamera = g_pGame->GetViewport()->GetA3DCameraBase();
#endif
	A3DMATRIX4 mr = RotateY(DEG2RAD(-(int)pA3DCamera->GetDeg() - 90));
	A3DVIEWPORTPARAM *p = pDlgMap->m_pA3DEngine->GetActiveViewport()->GetParam();

	MAPMARK Mark;
	CECNPCServer *pServer;
	CECNPCMan *pNPCMan = pDlgMap->GetWorld()->GetNPCMan();
	const APtrArray<CECNPC *> &NPCs = pNPCMan->GetNPCsInMiniMap();
	EC_VIDEO_SETTING vs = g_pGame->GetConfigs()->GetVideoSettings();

	CECFriendMan::GROUP *pGroup;
	CECFriendMan::FRIEND *pFriend;
	CECTeam *pTeam = pHost->GetTeam();
	CECFriendMan *pMan = pHost->GetFriendMan();
	CECPlayerMan *pPlayerMan = pDlgMap->GetWorld()->GetPlayerMan();
	const APtrArray<CECElsePlayer *> &Players = pPlayerMan->GetPlayersInMiniMap();

	int iTeamMenber = pHost->GetTeam() ? pHost->GetTeam()->GetMemberNum() : 0;
	CDlgMiniMap::RadarPoint *rp = new CDlgMiniMap::RadarPoint
		[iTeamMenber + Players.GetSize() + NPCs.GetSize() + m_vecMark.size() + 3];
	int n = 0;

	pDlgMap->m_vecMiniMapMark.clear();

	// Player
	if( vs.bMapPlayer )
	{
		for( i = 0; i < Players.GetSize(); i++ )
		{
			if( pTeam && pTeam->GetMemberByID(Players[i]->GetCharacterID()) )
				continue;

			vecPos = (Players[i]->GetPos() - vecPosHost);
			if( SQUARE(vecPos.x * fRatio) + SQUARE(vecPos.z * fRatio) >= fRadius )
				continue;

			if( MODE_A == pDlgMap->m_nMode )
				vecPos = vecPos * mr;
			ptPos.x = int(p->X + C.x + vecPos.x * fRatio);
			ptPos.y = int(p->Y + C.y - vecPos.z * fRatio);

			rp[n].pos = ptPos;
			
			if( Players[i]->GetCharacterID() == idTarget )
			{
				rp[n].type = MAPMARK_TARGET;
				n++;
				rp[n].pos = ptPos;
			}
			int battleType = pDlgMap->GetHostPlayer()->GetBattleType();
			if ( battleType==BATTLE_TYPE_ARENA || battleType==BATTLE_TYPE_ARENAREBORN || battleType==BATTLE_TYPE_CRSSVR )
			{
				rp[n].type = CDlgMiniMap::MAPMARK_PLAYER;
			}
			else if (pHost->GetSrcServerID()!=Players[i]->GetSrcServerID())
			{
				if( battleType==BATTLE_TYPE_FLPW_CRSSVR
					|| (CECBattleDungeonMan::getSingletonPtr()->GetCurBattle() &&
					CECBattleDungeonMan::getSingletonPtr()->GetCurBattle()->m_iId==795)
					|| (CECBattleDungeonMan::getSingletonPtr()->GetCurBattle() &&CECBattleDungeonMan::getSingletonPtr()->GetCurBattle()->m_iId==642))
				{
					if( Players[i]->GetBattleCamp() == pHost->GetBattleCamp() )
					{
						rp[n].type = MAPMARK_TEAMMATE;
					}
					else
					{
							rp[n].type = MAPMARK_ENEMY;
					}
				}
				else
				{
					rp[n].type = MAPMARK_ENEMY;
				}
				
			}
			else if( pHost->IsInBattle() )
			{
				if( Players[i]->GetBattleCamp() == pHost->GetBattleCamp() )
					rp[n].type = MAPMARK_TEAMMATE;
				else if( Players[i]->GetBattleCamp() == GP_BATTLE_CAMP_OBSERVER ) // 不显示观战方
					continue;
				else
					rp[n].type = MAPMARK_ENEMY;
			}
			else if( pHost->GetCombatFaction() != 0 && Players[i]->GetFactionID() == pHost->GetCombatFaction() )
			{
				rp[n].type = MAPMARK_ENEMYGUILD;
			}
			else if( pDlgMap->GetGameUIMan()->m_pDlgFriendList->IsEnemy(Players[i]->GetCharacterID()) )
			{
				rp[n].type = MAPMARK_ENEMY;
				int W = int(m_pA2DMapMark[MAPMARK_ENEMY]->GetWidth() * fSize);
				int H = int(m_pA2DMapMark[MAPMARK_ENEMY]->GetHeight() * fSize);
				Mark.strHint = Players[i]->GetName();
				Mark.vecPos = Players[i]->GetPos();
				Mark.rect.SetRect(ptPos.x - W, ptPos.y - H,
						ptPos.x + W, ptPos.y + H);
				m_vecMiniMapMark.push_back(Mark);
			}
			else if( pFriend = pMan->GetFriendByID(Players[i]->GetCharacterID()) )
			{
				rp[n].type = MAPMARK_FRIEND;
				pGroup = pMan->GetGroupByID(pFriend->iGroup);
				if( pGroup )
				{
					int W = int(m_pA2DMapMark[MAPMARK_FRIEND]->GetWidth() * fSize);
					int H = int(m_pA2DMapMark[MAPMARK_FRIEND]->GetHeight() * fSize);
					Mark.strHint = Players[i]->GetName();
					Mark.vecPos = Players[i]->GetPos();
					Mark.rect.SetRect(ptPos.x - W, ptPos.y - H,
							ptPos.x + W, ptPos.y + H);
					m_vecMiniMapMark.push_back(Mark);
				}
			}
			else if( pHost->GetFactionID() != 0 && Players[i]->GetFactionID() == pHost->GetFactionID() )
			{
				rp[n].type = MAPMARK_GUILD;
				int W = int(m_pA2DMapMark[MAPMARK_GUILD]->GetWidth() * fSize);
				int H = int(m_pA2DMapMark[MAPMARK_GUILD]->GetHeight() * fSize);
				Mark.strHint = Players[i]->GetName();
				Mark.vecPos = Players[i]->GetPos();
				Mark.rect.SetRect(ptPos.x - W, ptPos.y - H,
						ptPos.x + W, ptPos.y + H);
				m_vecMiniMapMark.push_back(Mark);
			}
			else if( pHost->GetFamilyID() != 0 && Players[i]->GetFamilyID() == pHost->GetFamilyID() )
			{
				rp[n].type = MAPMARK_FAMILY;
				int W = int(m_pA2DMapMark[MAPMARK_FAMILY]->GetWidth() * fSize);
				int H = int(m_pA2DMapMark[MAPMARK_FAMILY]->GetHeight() * fSize);
				Mark.strHint = Players[i]->GetName();
				Mark.vecPos = Players[i]->GetPos();
				Mark.rect.SetRect(ptPos.x - W, ptPos.y - H,
						ptPos.x + W, ptPos.y + H);
				m_vecMiniMapMark.push_back(Mark);
			}
			else
				rp[n].type = MAPMARK_PLAYER;

			n++;
		}
	}

	// NPC
	for( i = 0; i < NPCs.GetSize(); i++ )
	{
		vecPos = (NPCs[i]->GetPos() - vecPosHost);
		if( SQUARE(vecPos.x * fRatio) + SQUARE(vecPos.z * fRatio) < fRadius && NPCs[i]->IsPhaseShow())
//		if( fabs(vecPos.x * fRatio) < fRadius && fabs(vecPos.z * fRatio) < fRadius )
		{
			if( MODE_A == pDlgMap->m_nMode )
				vecPos = vecPos * mr;
			ptPos.Set(int(p->X + C.x + vecPos.x * fRatio),
				int(p->Y + C.y - vecPos.z * fRatio));

			if( NPCs[i]->GetNPCID() == idTarget )
			{
				rp[n].pos = ptPos;
				rp[n].type = MAPMARK_TARGET;
				n++;
			}
			
			if( pHost->IsInBattle() )
			{
				elementdataman* pElementDataMan = g_pGame->GetElementDataMan();
				DATA_TYPE dt;
				NPC_ESSENCE* pNPCEssence = (NPC_ESSENCE*)pElementDataMan->get_data_ptr(
					NPCs[i]->GetTemplateID(), ID_SPACE_ESSENCE,dt);

				if( pNPCEssence && pNPCEssence->id_src_monster )
				{
					MONSTER_ESSENCE* pEssence = (MONSTER_ESSENCE*)pElementDataMan->get_data_ptr(
						pNPCEssence->id_src_monster, ID_SPACE_ESSENCE,dt); 
					if( pEssence )
					{
						if( pEssence->faction & 16 )
						{
							rp[n].type = MAPMARK_BUILDINGB;
							rp[n].pos = ptPos;
							n++;
							continue;
						}
						else if( pEssence->faction & 32 )
						{
							rp[n].type = MAPMARK_BUILDINGA;
							rp[n].pos = ptPos;
							n++;
							continue;
						}
					}
				}
			}

			if( (NPCs[i]->IsServerNPC() && !vs.bMapNPC) ||
				(!NPCs[i]->IsServerNPC() && !vs.bMapMonster) )
			{
				continue;
			}

			rp[n].pos = ptPos;

			if( NPCs[i]->IsServerNPC() )
			{
				pServer = (CECNPCServer *)NPCs[i];
				int nDraw;
				if( pServer->GetCurQuestIcon() == CECNPCServer::QI_IN ||
					pServer->GetCurQuestIcon() == CECNPCServer::QI_IN_K )
				{
					nDraw = MAPMARK_NPCFINISHTASK;
				}
				else if( pServer->GetCurQuestIcon() == CECNPCServer::QI_OUT ||
					pServer->GetCurQuestIcon() == CECNPCServer::QI_OUT_K )
					nDraw = MAPMARK_NPCACCEPTTASK;
				else
					nDraw = MAPMARK_NPC;

				rp[n].type = nDraw;
				int W = int(m_pA2DMapMark[nDraw]->GetWidth() * fSize);
				int H = int(m_pA2DMapMark[nDraw]->GetHeight() * fSize);
				Mark.strHint = NPCs[i]->GetName();
				Mark.vecPos = NPCs[i]->GetPos();
				Mark.rect.SetRect(ptPos.x - W, ptPos.y - H,
						ptPos.x + W, ptPos.y + H);
				m_vecMiniMapMark.push_back(Mark);
			}
			else
			{
				rp[n].type = MAPMARK_MONSTER;
				if( NPCs[i]->IsMonsterNPC() )
				{
					if( CDlgTask::m_mapTaskMonster.find(NPCs[i]->GetTemplateID()) != CDlgTask::m_mapTaskMonster.end() )
						rp[n].type = MAPMARK_MONSTERTASK;
					else
					{
						int nShowLevel = 0;
						elementdataman* pElementDataMan = g_pGame->GetElementDataMan();
						DATA_TYPE dt;
						MONSTER_ESSENCE* pEssence = (MONSTER_ESSENCE*)pElementDataMan->get_data_ptr(
							NPCs[i]->GetTemplateID(), ID_SPACE_ESSENCE,dt); 
						if( pEssence && dt == DT_MONSTER_ESSENCE )
							nShowLevel = pEssence->show_level;
						if( nShowLevel == 4 )
							rp[n].type = MAPMARK_MONSTERBOSS;
					}
				}
			}

			n++;
		}
	}

	// Team member
	if( vs.bMapPlayer )
	{
		CECTeamMember *pMember;
		CECTeam *pTeam = pHost->GetTeam();
		CECFriendMan *pMan = pHost->GetFriendMan();
		for( i = 0; pTeam && i < pTeam->GetMemberNum(); i++ )
		{
			pMember = pTeam->GetMemberByIndex(i);
			if( !POSISVALID(pMember->GetPos().x) ||
				pMember->GetCharacterID() == pHost->GetCharacterID() ||
				pMember->GetInstanceID() != pDlgMap->GetWorld()->GetInstanceID() )
			{
				continue;
			}

			vecPos = (pMember->GetPos() - vecPosHost);

			if( SQUARE(vecPos.x * fRatio) + SQUARE(vecPos.z * fRatio) >= fRadius )
			{
//				vecPos.Normalize();
//				vecPos *= (float)sqrt(fRadius) / fRatio;
				if( fabs(vecPos.x) > fabs(vecPos.z) )
					vecPos *= fRadius / (fRatio * (float)fabs(vecPos.x));
				else
					vecPos *= fRadius / (fRatio * (float)fabs(vecPos.z));
			}

			if( MODE_A == pDlgMap->m_nMode )
				vecPos = vecPos * mr;
			ptPos.x = int(p->X + C.x + vecPos.x * fRatio);
			ptPos.y = int(p->Y + C.y - vecPos.z * fRatio);
			rp[n].pos = ptPos;

			int nDraw;
			if( pMember->GetCharacterID() == pTeam->GetLeaderID() )
				nDraw = MAPMARK_CAPTAIN;
			else
				nDraw = MAPMARK_TEAMMATE;
			rp[n].type = nDraw;

			int W = int(m_pA2DMapMark[nDraw]->GetWidth() * fSize);
			int H = int(m_pA2DMapMark[nDraw]->GetHeight() * fSize);

			Mark.strHint = pMember->GetName();
			Mark.vecPos = pMember->GetPos();
			Mark.rect.SetRect(ptPos.x - W, ptPos.y - H,
					ptPos.x + W, ptPos.y + H);
			m_vecMiniMapMark.push_back(Mark);

			n++;
		}
	}

	// customize marks
	for( i = 0; i < (int)m_vecMark.size(); i++ )
		if( m_vecMark[i].idInstance == g_pGame->GetGameRun()->GetWorld()->GetInstanceID() )
		{
			vecPos = (m_vecMark[i].vecPos - vecPosHost);
			vecPos.y = 0.0f;
			if( SQUARE(vecPos.x * fRatio) + SQUARE(vecPos.z * fRatio) >= fRadius )
			{
				vecPos.Normalize();
				vecPos *= (float)sqrt(fRadius) / fRatio;
//				if( fabs(vecPos.x) > fabs(vecPos.z) )
//					vecPos *= fRadius / (fRatio * (float)fabs(vecPos.x));
//				else
//					vecPos *= fRadius / (fRatio * (float)fabs(vecPos.z));
			}

			if( MODE_A == m_nMode ) vecPos = vecPos * mr;
			ptPos.x = int(p->X + C.x + vecPos.x * fRatio);
			ptPos.y = int(p->Y + C.y - vecPos.z * fRatio);
			rp[n].pos = ptPos;
			rp[n].type = i + MAPMARK_TAG0;

			int W = int(m_pA2DMapMark[i + MAPMARK_TAG0]->GetWidth() * fSize);
			int H = int(m_pA2DMapMark[i + MAPMARK_TAG0]->GetHeight() * fSize);
			if( m_vecMark[i].nNPC >= 0 )
				Mark.strHint = _AL("^FFFF00") + m_vecMark[i].strName;
			else
				Mark.strHint = m_vecMark[i].strName;
			Mark.vecPos = m_vecMark[i].vecPos;
			Mark.rect.SetRect(ptPos.x - W, ptPos.y - H,
					ptPos.x + W, ptPos.y + H);
			m_vecMiniMapMark.push_back(Mark);

			n++;
		}

	// mouse click target
	DWORD dwTimePassed = GetTickCount() - m_dwTempTargetTime;
	if( pDlgMap->m_idTempTargetInstance == g_pGame->GetGameRun()->GetWorld()->GetInstanceID() &&
		m_dwTempTargetTime > 0 && dwTimePassed <= 3000 && (dwTimePassed / 500) % 2 == 0 )
	{
		vecPos = (m_vecTempTarget - vecPosHost);
		vecPos.y = 0.0f;
		if( SQUARE(vecPos.x * fRatio) + SQUARE(vecPos.z * fRatio) >= fRadius )
		{
			vecPos.Normalize();
			vecPos *= (float)sqrt(fRadius) / fRatio;
//			if( fabs(vecPos.x) > fabs(vecPos.z) )
//				vecPos *= fRadius / (fRatio * (float)fabs(vecPos.x));
//			else
//				vecPos *= fRadius / (fRatio * (float)fabs(vecPos.z));
		}

		if( MODE_A == m_nMode ) vecPos = vecPos * mr;
		ptPos.x = int(C.x + vecPos.x * fRatio);
		ptPos.y = int(C.y - vecPos.z * fRatio);

		rp[n].pos = ptPos;
		rp[n].type = MAPMARK_TAGCLICK;
		n++;
	}

	rp[n].pos.x = p->X + C.x - 1;
	rp[n].pos.y = p->Y + C.y - 1;
	rp[n].type = MAPMARK_HOST;
	n++;
	
	qsort((void*)rp, n, sizeof(RadarPoint), compare);

	for(i = 0; i < n; i++)
	{
		m_pA2DMapMark[rp[i].type]->DrawToInternalBuffer(rp[i].pos.x, rp[i].pos.y);
	}

	delete []rp;

	if( g_ECAutoMove.GetState() == CECAutoMove::enumStateSuccess )
	{
		for(int i = g_ECAutoMove.GetWayPointCount() - 1; i >= 0; i -= AUTOMOVE_POINTDEC)
		{
			vecPos = g_ECAutoMove.GetPosByIndex(i) - vecPosHost;
			if( SQUARE(vecPos.x * fRatio) + SQUARE(vecPos.z * fRatio) <= fRadius )
			{
				if( MODE_A == pDlgMap->m_nMode )
					vecPos = vecPos * mr;
				ptPos.Set(int(p->X + C.x + vecPos.x * fRatio),
					int(p->Y + C.y - vecPos.z * fRatio));
				m_pA2DMapMark[MAPMARK_AUTOMOVE]->DrawToInternalBuffer(ptPos.x, ptPos.y);
			}
		}
	}
}

void CDlgMiniMap::OnEventMouseMove_Hide_2(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	int i;
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);

	A3DRECT rcMiniMap = pObj->GetRect();
	A3DPOINT2 C = rcMiniMap.CenterPoint();
	float fZoom = float(DEFAULT_RADAR_DIAMETER) / rcMiniMap.Width() * m_fZoom;
	float fRatio = 512.0f / 1024.0f / fZoom;
#ifdef BACK_VERSION
	A3DCamera *pA3DCamera = g_pGame->GetViewport()->GetA3DCamera();
#else
	A3DCameraBase *pA3DCamera = g_pGame->GetViewport()->GetA3DCameraBase();
#endif
	A3DMATRIX4 mr = InverseTM(RotateY(DEG2RAD(-(int)pA3DCamera->GetDeg() - 90)));
	A3DVECTOR3 vecHost = GetHostPlayer()->GetPos();
	
	m_vecNowTarget.x = (x - C.x) / fRatio;
	m_vecNowTarget.y = 0;
	m_vecNowTarget.z = (C.y - y) / fRatio;
	if( CDlgMiniMap::MODE_A == m_nMode )
		m_vecNowTarget = m_vecNowTarget * mr;
	m_vecNowTarget += vecHost;
	m_vecNowTarget.y = g_pGame->GetGameRun()->GetWorld()->GetTerrainHeight(m_vecNowTarget);
	pObj->SetHint(_AL(""));
	
	for( i = 0; i < (int)m_vecMiniMapMark.size(); i++ )
	{
		if( m_vecMiniMapMark[i].rect.PtInRect(x, y) )
		{
			pObj->SetHint(m_vecMiniMapMark[i].strHint);
			m_vecNowTarget = m_vecMiniMapMark[i].vecPos + A3DVECTOR3(0.0f, 1.8f, 0.0f);
			return;
		}
	}
}

void CDlgMiniMap::OnEventLButtonUp_Hide_2(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - p->X;
	int y = GET_Y_LPARAM(lParam) - p->Y;
	
	A3DRECT rcMiniMap = pObj->GetRect();
	A3DPOINT2 C = rcMiniMap.CenterPoint();
	float fZoom = float(DEFAULT_RADAR_DIAMETER) / rcMiniMap.Width() * m_fZoom;
	float fRatio = 512.0f / 1024.0f / fZoom;
#ifdef BACK_VERSION
	A3DCamera *pA3DCamera = g_pGame->GetViewport()->GetA3DCamera();
#else
	A3DCameraBase *pA3DCamera = g_pGame->GetViewport()->GetA3DCameraBase();
#endif
	A3DMATRIX4 mr = InverseTM(RotateY(DEG2RAD(-(int)pA3DCamera->GetDeg() - 90)));
	A3DVECTOR3 vecHost = GetHostPlayer()->GetPos();
	
	m_dwTempTargetTime = GetTickCount();
	m_vecTempTarget.x = (x - C.x) / fRatio;
	m_vecTempTarget.z = (C.y - y) / fRatio;
	if( CDlgMiniMap::MODE_A == m_nMode )
		m_vecTempTarget = m_vecTempTarget * mr;
	m_vecTempTarget += vecHost;
	m_vecTempTarget.y = g_pGame->GetGameRun()->GetWorld()->GetTerrainHeight(m_vecTempTarget); 
	
	m_idTempTargetInstance = GetWorld()->GetInstanceID();
	
	g_ECAutoMove.SearchPath(g_pGame->GetGameRun()->GetWorld()->GetHostPlayer()->GetPos(), 
							m_vecTempTarget,
							g_pGame->GetGameRun()->GetWorld()->GetInstanceID(),false);
	
	if( GetHostPlayer()->GetModelTempArrow() )
		GetHostPlayer()->GetModelTempArrow()->PlayActionByName("动画", 1.0f, true);
}

void CDlgMiniMap::OnEventLButtonDown_List_Choose(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	/*
	PAUILISTBOX pListChoose = (PAUILISTBOX)GetDlgItem("List_Choose");
	int nCurSel = pListChoose->GetCurSel();
	if( nCurSel >= 0 && nCurSel < pListChoose->GetCount() && AUI_PRESS(VK_CONTROL) )
	{
		CDlgChat* pChat = GetGameUIMan()->m_pDlgChat;
		PAUIEDITBOX pEdit = (PAUIEDITBOX)pChat->GetDlgItem("DEFAULT_Txt_Speech");
		if( pEdit->GetItemCount(enumEICoord) < 1 )
		{
			ACHAR szText[100];
			a_sprintf(szText, _AL("%.1f %.1f %.1f %d"), 
				m_vecMark[nCurSel].vecPos.x, 
				m_vecMark[nCurSel].vecPos.y,
				m_vecMark[nCurSel].vecPos.z,
				m_vecMark[nCurSel].idInstance
				);
			pEdit->AppendItem(enumEICoord, m_vecMark[nCurSel].strName, szText);
		}
		GetGameUIMan()->BringWindowToTop(pChat);
		pChat->ChangeFocus(pEdit);
	}
	*/
}

void CDlgMiniMap::OnEventRButtonDown_List(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	PAUILISTBOX	pList = (PAUILISTBOX)pObj;
	int nSel = pList->GetCurSel();
	if( nSel >= 0 && nSel < pList->GetCount() )
	{
		if( stricmp(pList->GetName(), "List_Choose") == 0 )
			g_ECAutoMove.SearchPath(GetHostPlayer()->GetPos(), m_vecMark[nSel].vecPos, m_vecMark[nSel].idInstance);
		else if( stricmp(pList->GetName(), "List_Region") == 0 )
		{
			CELPrecinct::MARK *pMark = GetGameUIMan()->m_pCurPrecinct->GetMark(nSel);
			g_ECAutoMove.SearchPath(GetHostPlayer()->GetPos(), pMark->vPos, g_pGame->GetGameRun()->GetWorld()->GetInstanceID());
		}
	}
}

void CDlgMiniMap::ChangeWorldInstance(int idInstance)
{
	if( GetGameUIMan()->m_pDlgWorldMap->IsShow() )
		GetGameUIMan()->m_pDlgWorldMap->OnCommand_CANCEL("IDCANCEL");
}

void CDlgMiniMap::SetNewMail(int nRemainTime)
{
	if(NULL != m_pChk_Mail)
	{
		if( nRemainTime == 0 )
		{
			m_pChk_Mail->SetData(0);
			m_pChk_Mail->Check(true);
			m_pChk_Mail->SetHint(GetStringFromTable(723));
			m_pChk_Mail->SetFlash(true);
		}
		else if( nRemainTime > 0 )
		{
			m_pChk_Mail->SetData(nRemainTime + GetGame()->GetServerGMTTime() + 10);
			m_pChk_Mail->Check(false);
			m_pChk_Mail->SetHint(_AL(""));
		}
		else
		{
			m_pChk_Mail->SetData(0);
			m_pChk_Mail->Check(false);
			m_pChk_Mail->SetHint(_AL(""));
		}

	}
}

void CDlgMiniMap::SetWarBuildStatus(int key, int status, A3DVECTOR3 pos, float hp)
{
	m_WarBuildingStatus[key].status = status;
	m_WarBuildingStatus[key].pos = pos;
	m_WarBuildingStatus[key].hp = hp;
}

void CDlgMiniMap::OnCommand_Recommand(const char* szCommand)
{
    PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Minimap_Recommend");
	if(pDlg)
	{
	   pDlg->Show(!pDlg->IsShow());
	}
}
void CDlgMiniMap::OnCommand_ZhuxianKnows(const char* szCommand)
{
    CDlgKnowsMin* pDlgZhuxianKnows = (CDlgKnowsMin*)(GetGameUIMan()->GetDialog("Win_ZhuxianKnows_Min"));
	if(pDlgZhuxianKnows)
	{
	   pDlgZhuxianKnows->OnCommandOpen("");
	}
}
void CDlgMiniMap::OnCommand_Activity(const char* szCommand)
{
	CDlgDynamicMin* pDynamicMin = (CDlgDynamicMin*)(GetGameUIMan()->GetDialog("Win_Activity_Min"));
	if(pDynamicMin)
	{
	   pDynamicMin->OnCommandOpen("");
	}
}
void CDlgMiniMap::OnCommand_Dungeon_6V6(const char* szCommand)
{
    //CDlgBattleTeam6v6ApplyMin* p6V6Min = (CDlgBattleTeam6v6ApplyMin*)(GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_Min"));
	//if(p6V6Min)
	//{
	 // p6V6Min->OnCommandOpen("");
	//}
    if(m_szName == "Win_Minimap_Recommend")
	{
	   GetDlgItem("Btn_Group")->Show(!GetDlgItem("Btn_Group")->IsShow());
	   GetDlgItem("Btn_6V6")->Show(!GetDlgItem("Btn_6V6")->IsShow());
	}
}
void CDlgMiniMap::OnCommand_Group(const char * szCommand)
{
    DlgCrssvrTeamsMin* p6V6MinExpand = (DlgCrssvrTeamsMin*)(GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_Min_Expand"));
	if(p6V6MinExpand)
	{
	   p6V6MinExpand->OnCommand_Group(szCommand);
	}
}
void CDlgMiniMap::OnCommand_6V6(const char * szCommand)
{
	DlgCrssvrTeamsMin* p6V6MinExpand = (DlgCrssvrTeamsMin*)(GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_Min_Expand"));
	if(p6V6MinExpand)
	{
		p6V6MinExpand->OnCommand_6V6(szCommand);
	}
}
void CDlgMiniMap::OnCommand_Hot(const char* szCommand)
{
	CDlgWebHot* pWebHot = (CDlgWebHot*)(GetGameUIMan()->GetDialog("Win_Explorer_Popu_Min"));
	if(pWebHot)
	{
	  pWebHot->OnCommandOpen("");
	}
}
void CDlgMiniMap::OnCommand_King(const char* szCommand)
{
	CDlgKingMin* pKingMin = (CDlgKingMin*)(GetGameUIMan()->GetDialog("Win_King_Min"));
	if(pKingMin)
	{
	   pKingMin->OnCommand_Open("");
	}
}
void CDlgMiniMap::OnCommand_RedRadio(const char* szCommand)
{
    CDlgCommonDlg* pDlg = (CDlgCommonDlg*)(GetGameUIMan()->GetDialog("Win_Radio_Min"));
	if(pDlg)
	{
	    pDlg->OnCommandRadio("");
	}

	if(GetDlgItem("Gfx_Radio_Min") && GetDlgItem("Gfx_Radio_Min")->IsShow()) 
	    GetDlgItem("Gfx_Radio_Min")->Show(false);
}
void CDlgMiniMap::OnCommand_ZhuxianExplore(const char* szCommand)
{
	CDlgDlgTreasMin* pTreasMin = (CDlgDlgTreasMin*)(GetGameUIMan()->GetDialog("Win_ZhuxianExplore_Min"));
	if(pTreasMin)
	{
	   pTreasMin->OnCommand_Active("");
	}
}
void CDlgMiniMap::OnCommand_ThemeLink(const char* szCommand)
{
	GetGame()->GetGameSession()->get_kdctoken();
	CDlgBaseExplorer* pDlg = (CDlgBaseExplorer*)GetGameUIMan()->GetDialog("win_explorer");
	PAUILABEL pLab = (PAUILABEL)pDlg->GetDlgItem("Title");
	pLab->SetText(GetGameUIMan()->GetStringFromTable(17880));
	pDlg->Show(true);
}
void CDlgMiniMap::OnCommand_QuestKf(const char* szCommand)
{
	if(GetGameUIMan()->GetDialog("Win_QuestKf")->IsShow())
		GetGameUIMan()->GetDialog("Win_QuestKf")->Show(false);
	else 
		GetGameUIMan()->GetDialog("Win_QuestKf")->Show(true);
}
void CDlgMiniMap::OnCommand_Ancient(const char* szCommand)
{
	if(GetGameUIMan()->GetDialog("Win_Quest_Wanted")->IsShow())
		GetGameUIMan()->GetDialog("Win_Quest_Wanted")->Show(false);
	else 
		GetGameUIMan()->GetDialog("Win_Quest_Wanted")->Show(true);
}
