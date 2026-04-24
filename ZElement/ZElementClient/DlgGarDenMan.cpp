// Filename	: DlgGardenMan.cpp
// Creator	: Fu Chonggang
// Date		: 2009/8/12

#include "AUI\\AUILabel.h"
#include "AUI\\AUITextArea.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIScroll.h"
#include "AUI\\AUICheckBox.h"
#include "AUI\\A3DFTFont.h"
#include "AUI\\AUIDef.h"
#include "AUI\\AUICTranslate.h"
#include "AUI\\AUITreeView.h"
#include "A2DSprite.h"
#include "AFI.h"
#include "AFileImage.h"
#include "DlgGardenMan.h"
#include "DlgGardenQShop.h"
#include "DlgMinimizeBar.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_IvtrConsume.h"
#include "EC_IvtrHTItem.h"
#include "EC_HostPlayer.h"
#include "EC_GameUIMan.h"
#include "EC_Inventory.h"
#include "EC_Resource.h"
#include "EC_Friend.h"
#include "EC_HTPDataType.h"
#include "EC_HTC2SRawCmds.h"
#include "EC_Algorithm.h"
#include "EL_Precinct.h"
#include "EC_World.h"
#include "gnoctets.h"
#include "BaseFunc.h"

#include "hometowntemplate.h"
#include "time.h"


#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgGardenMan, CDlgBase)

// garden main
AUI_ON_COMMAND("oper*",				OnCommandGardenOperate)
AUI_ON_COMMAND("BtnDynanic_*",		OnCommandGardenStatusCheck)
AUI_ON_COMMAND("Btn_CheckLove*",	OnCommandGardenCheckLove)
AUI_ON_COMMAND("Btn_CheckMature*",	OnCommandGardenCheckMature)
AUI_ON_COMMAND("RdoMethod_*",		OnCommandGardenOperType)
AUI_ON_COMMAND("Btn_Barn",			OnCommandGardenOpenBarn)
AUI_ON_COMMAND("Btn_Prop",			OnCommandGardenOpenProp)
AUI_ON_COMMAND("Btn_CheckStatus",	OnCommandGardenCheckStatus)
AUI_ON_COMMAND("Btn_Minimize",		OnCommandGardenMinimize)
AUI_ON_COMMAND("close",				OnCommandClose)
AUI_ON_COMMAND("seedselect",		OnCommandGardenSeedSelect)
AUI_ON_COMMAND("Btn_Exchange",		OnCommandGardenExchange)
AUI_ON_COMMAND("Btn_Cool",			OnCommandGardenCool)
AUI_ON_COMMAND("Btn_LvUp",			OnCommandGardenLevelUp)
AUI_ON_COMMAND("Btn_Setting",		OnCommandGardenSetting)
AUI_ON_COMMAND("Btn_Friendlist",	OnCommandGardenFriendList)
AUI_ON_COMMAND("Btn_GardenEnter",	OnCommandGardenFriendGarden)
AUI_ON_COMMAND("IDCANCEL",			OnCommandCancel)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgGardenMan, CDlgBase)

AUI_ON_EVENT("Img_Plant_*",			WM_LBUTTONDOWN,		OnEventLButtonDownPlant)
AUI_ON_EVENT("Img_Plant_*",			WM_LBUTTONUP,		OnEventLButtonUpPlant)
AUI_ON_EVENT("Img_*",				WM_LBUTTONDOWN,		OnEventLButtonDownItem)
AUI_ON_EVENT("Tv_FriendList",		WM_LBUTTONDBLCLK,	OnEventLButtonDownFriendGarden)
AUI_ON_EVENT("*",					WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_ON_EVENT(NULL,					WM_MOUSEWHEEL,		OnEventMouseWheel)

AUI_END_EVENT_MAP()

abase::vector<int>								CDlgGardenMan::m_vecLoveGardenIds;
abase::vector<int>								CDlgGardenMan::m_vecMatureGardenIds;
abase::vector<ACString>							CDlgGardenMan::m_vecDynamicString[DYNAMICTYPE_NUM];
abase::vector<CDlgGardenMan::dlg_field_info>	CDlgGardenMan::m_vecFarmInfo[FARMTYPE_MAX];
abase::hash_map<int, int>						CDlgGardenMan::m_mapId2Index;
abase::vector<const HOMETOWN::CROP_ESSENCE*>	CDlgGardenMan::m_vecCropEss;
abase::vector<INT64>							CDlgGardenMan::m_vecLevelUpCharm;
int				CDlgGardenMan::m_iOwnerId			= 0;
INT64			CDlgGardenMan::m_iOwnerMoney		= 0;
INT64			CDlgGardenMan::m_iOwnerCharm		= 0;
int				CDlgGardenMan::m_iLevel				= 0;
bool			CDlgGardenMan::m_bOwn				= true;
int				CDlgGardenMan::m_iOperType			= OPERTYPE_NONE;
int				CDlgGardenMan::m_iActionType		= 1;
int				CDlgGardenMan::m_iSeedType			= -1;
int				CDlgGardenMan::m_iFarmState			= 0;
int				CDlgGardenMan::m_iFarmProtectPeriod	= 0;

using namespace HT_S2C;

using namespace HOMETOWN;

namespace GNET
{
	class Octets;
}

#define MSGBOX(str)			GetGameUIMan()->MessageBox("",str, MB_OK, A3DCOLORRGBA(255, 255, 255, 160))
#define GETSTRING(id)		GetGameUIMan()->GetStringFromTable(id)
#define ADDCHAT(str)		GetGameUIMan()->AddChatMessage(str, GP_CHAT_LOCAL)
#define FRIENDNAME(id)		GetHostPlayer()->GetFriendMan()->GetFriendByID(id)?GetHostPlayer()->GetFriendMan()->GetFriendByID(id)->strName : _AL("error")
#define EMOTIONSTRING(id)	GETSTRING(id+8609)

class CompareCDlgGardenManSeedOrderByLevel
{
public:
	bool operator ()(const HOMETOWN::CROP_ESSENCE * p1, const HOMETOWN::CROP_ESSENCE * p2)
	{
		return p1->required_level < p2->required_level;
	}
};

CDlgGardenMan::CDlgGardenMan()
{
	m_pLabTitle = NULL;
	m_pLabCharm = NULL;
	m_pLabLevel = NULL;
}

CDlgGardenMan::~CDlgGardenMan()
{
}


bool CDlgGardenMan::OnInitDialog()
{
	m_iBarLevel = 0;
	if (stricmp(m_szName,"Win_Garden") == 0)
	{
		DDX_Control("Title",		m_pLabTitle);
		m_pLabTitle->SetAlign(AUIFRAME_ALIGN_CENTER);
		DDX_Control("Txt_Charm",	m_pLabCharm);
		DDX_Control("Txt_Lv",		m_pLabLevel);

		int i;
		AString strName;
		for (i=0; i<HOMETOWNFARMPLANTNUM; i++)
		{
			strName.Format("Img_Plant_Crop%d", i+1);
			m_pImgPlants[i][0] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
			strName.Format("Img_Plant_Plot%d", i+1);
			m_pImgPlants[i][1] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
			strName.Format("Img_Plant_Water%d", i+1);
			m_pImgPlants[i][2] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
			strName.Format("Img_Plant_Drought%d", i+1);
			m_pImgPlants[i][3] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
			strName.Format("Img_Plant_Grass%d", i+1);
			m_pImgPlants[i][4] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
			strName.Format("Img_Plant_Worm%d", i+1);
			m_pImgPlants[i][5] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
			strName.Format("Img_Plant_LessMature%d", i+1);
			m_pImgPlants[i][6] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
			strName.Format("Img_Plant_Trap%d", i+1);
			m_pImgPlants[i][7] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
			strName.Format("Img_Plant_Gemmule%d", i+1);
			m_pImgPlants[i][8] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
			strName.Format("Img_Plant_Bigger%d", i+1);
			m_pImgPlants[i][9] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
			strName.Format("Img_Plant_Mature%d", i+1);
			m_pImgPlants[i][10] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
			strName.Format("Img_Plant_Die%d", i+1);
			m_pImgPlants[i][11] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
			strName.Format("Img_Plant_Harvest%d", i+1);
			m_pImgPlants[i][12] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
			for (int j=0; j<IMAGESTATE_NUM; j++)
				m_pImgPlants[i][j]->SetData(i);
		}
		for (i=0; i<HOMETOWNFARMPLANTLOVENUM; i++)
		{
			strName.Format("Img_Plant_Banner_Love%d", i+1);
			GetDlgItem(strName)->SetData(HOMETOWNFARMPLANTOWNNUM+i);
		}

		m_pImgGuardBeast = (PAUIIMAGEPICTURE)GetDlgItem("Img_Guard");
		//m_pImgGuardBeast->Show(false);
		for (i=0; i<BEE_NUM; i++)
		{
			strName.Format("Img_Bees%d", i+1);
			m_pImgBee[i] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
			m_pImgBee[i]->Show(false);
			strName.Format("Img_Butterfly%d", i+1);
			m_pImgButterfly[i] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
			m_pImgButterfly[i]->Show(false);
			m_iBeeFrameIndex[i] = 0;
			m_iBfFrameIndex[i] = 0;
		}
		for (i=BEE_NUM; i<6; i++)
		{
			strName.Format("Img_Bees%d", i+1);
			GetDlgItem(strName)->Show(false);
			strName.Format("Img_Butterfly%d", i+1);
			GetDlgItem(strName)->Show(false);
		}

		m_iOwnerMoney			= 0;
		m_iOwnerCharm			= 0;
		m_iLevel				= 0;
		m_iFarmState			= 0;
		m_iFarmProtectPeriod	= 0;
		m_iGuardFrameIndex		= 0;

		m_vecLoveGardenIds.clear();
		m_vecMatureGardenIds.clear();
		for (i=0;i<DYNAMICTYPE_NUM;i++)
		{
			m_vecDynamicString[i].clear();
		}
		ResetState();
	}
	else if (stricmp(m_szName,"Win_Garden_PlotMethod") == 0)
	{
	}
	else if (stricmp(m_szName,"Win_Garden_Seed") == 0)
	{
		UINT i;
		m_iFirstLine = 0;
		HOMETOWN::HometownTemplate* homeTemplate = GetGame()->GetHomeTownDataMan();
		const FARM_CONFIG *farmConfig = homeTemplate->get_farm_config();
		m_vecLevelUpCharm.clear();
		for (i=0; i<farmConfig->upgrade_config.size(); i++)
		{
			m_vecLevelUpCharm.push_back(farmConfig->upgrade_config[i]);
		}
		m_vecCropEss.clear();
		for (i=0; i<farmConfig->crop_list.size(); i++)
		{
			HOMETOWN::DATA_TYPE dt;
			const HOMETOWN::CROP_ESSENCE* toolEss;
			toolEss = homeTemplate->get_data_ptr(farmConfig->crop_list[i],HOMETOWN::ID_SPACE_ESSENCE, dt, toolEss);
			m_vecCropEss.push_back(toolEss);
		}
		BubbleSort(m_vecCropEss.begin(), m_vecCropEss.end(), CompareCDlgGardenManSeedOrderByLevel());
		PAUISCROLL pScl = (PAUISCROLL)GetDlgItem("Scl_Item");
		if (m_vecCropEss.size()>6)
		{
			pScl->SetScrollRange(0,(m_vecCropEss.size()+2)/3-2);
			pScl->SetBarLevel(0);
			pScl->SetScrollStep(1);
			pScl->Show(true);
		}
		else
			pScl->Show(false);
		UpdateSeed();
	}
	else if (stricmp(m_szName,"Win_CheckStatus") == 0)
	{
	}
	else if (stricmp(m_szName,"Win_Garden_Action") == 0)
	{
	}

	return true;
}

bool CDlgGardenMan::Release()
{
	if (stricmp(m_szName,"Win_Garden") == 0)
	{
		int i;
		for (i=0; i<FARMTYPE_MAX; i++)
		{
			m_vecFarmInfo[i].clear();
		}

		for (i=0;i<DYNAMICTYPE_NUM;i++)
		{
			m_vecDynamicString[i].clear();
		}
		m_vecLoveGardenIds.clear();
		m_vecMatureGardenIds.clear();
	}
	return CDlgBase::Release();
}

void CDlgGardenMan::OnTick()
{
	if (stricmp(m_szName,"Win_Garden") == 0)
	{
		ACString strText = m_pLabTitle->GetText();
		if (strText.IsEmpty())
		{
			if (m_iOwnerId == GetHostPlayer()->GetCharacterID())
				m_pLabTitle->SetText(GETSTRING(8599));
			else if (FRIENDNAME(m_iOwnerId))
			{
				strText.Format(GetGameUIMan()->GetStringFromTable(8600),FRIENDNAME(m_iOwnerId));
				m_pLabTitle->SetText(strText);
			}
		}
		PAUILABEL pLabCharm = (PAUILABEL)GetDlgItem("Txt_Charm");
		if (m_iOwnerCharm<0)
		{
			m_iOwnerCharm = 0;
		}
		GetGameUIMan()->Int64ToStr(m_iOwnerCharm, strText);
		pLabCharm->SetText(strText);

		if ((GetKeyState(VK_ESCAPE) & 0x8000))
		{
			m_iOperType = 0;
			GetGame()->ChangeCursor(RES_CUR_NORMAL);
			CheckRadioButton(1,0);
		}
		if (GetGame()->GetCurCursor()==RES_CUR_NORMAL && 
			!GetGameUIMan()->GetDialog("Win_Garden_PlotMethod")->IsShow() &&
			!GetGameUIMan()->GetDialog("Win_Garden_Seed")->IsShow())
		{
			m_iOperType = 0;
			CheckRadioButton(1,0);
		}
		GetDlgItem("Btn_LvUp")->Show(m_bOwn);
		if (m_bOwn)
		{
			GetDlgItem("Btn_LvUp")->Enable(m_iOwnerCharm>=m_vecLevelUpCharm[m_iLevel]);
		}
		GetDlgItem("Btn_Exchange")->Show(m_bOwn);
		GetDlgItem("Rdo_6")->Show(m_bOwn);
		GetDlgItem("Rdo_7")->Show(!m_bOwn);

		static DWORD dwTime = GetTickCount();
		DWORD dwTimeNow = GetTickCount();
		if (dwTimeNow > (dwTime+1000*120))
		{
			dwTime = dwTimeNow;
			htc2s_SendCmdViewFarm(m_iOwnerId);
		}

		TickState();
		UpdateHint();
	}
	else if (stricmp(m_szName,"Win_Garden_Friendlist") == 0)
	{
	}
	else
	{
		if (stricmp(m_szName,"Win_CheckStatusLove") == 0)
		{
			PAUISCROLL pScl = (PAUISCROLL)GetDlgItem("Scl_1");
			if (m_iBarLevel != pScl->GetBarLevel())
			{
				m_iBarLevel = pScl->GetBarLevel();
				AString strName;
				PAUISTILLIMAGEBUTTON pBtn;
				PAUISCROLL pScl = (PAUISCROLL)GetDlgItem("Scl_1");
				if (!pScl->IsShow()) return;
				abase::vector<ACString> vecDynamic = m_vecDynamicString[DYNAMICTYPE_LOVEGARDEN];
				if (UINT(10+m_iBarLevel) > vecDynamic.size() || UINT(10+m_iBarLevel) > m_vecLoveGardenIds.size())
				{
					assert(0);
					return;
				}
				
				for (int i=0; i<10; i++)
				{
					strName.Format("Btn_CheckLove%d", i+1);
					pBtn = (PAUISTILLIMAGEBUTTON)GetDlgItem(strName);
					pBtn->SetText(vecDynamic[i+m_iBarLevel]);
					pBtn->SetData(m_vecLoveGardenIds[i+m_iBarLevel]);
					pBtn->Show(true);
				}
			}
		}
		else if (stricmp(m_szName,"Win_CheckStatusMature") == 0)
		{
			PAUISCROLL pScl = (PAUISCROLL)GetDlgItem("Scl_1");
			if (m_iBarLevel != pScl->GetBarLevel())
			{
				m_iBarLevel = pScl->GetBarLevel();
				AString strName;
				PAUISTILLIMAGEBUTTON pBtn;
				PAUISCROLL pScl = (PAUISCROLL)GetDlgItem("Scl_1");
				if (!pScl->IsShow()) return;
				abase::vector<ACString> vecDynamic = m_vecDynamicString[DYNAMICTYPE_FRUITS];
				if (UINT(10+m_iBarLevel) > vecDynamic.size() || UINT(10+m_iBarLevel) > m_vecMatureGardenIds.size())
				{
					assert(0);
					return;
				}
				for (int i=0; i<10; i++)
				{
					strName.Format("Btn_CheckMature%d", i+1);
					pBtn = (PAUISTILLIMAGEBUTTON)GetDlgItem(strName);
					pBtn->SetText(vecDynamic[i+m_iBarLevel]);
					pBtn->SetData(m_vecMatureGardenIds[i+m_iBarLevel]);
					pBtn->Show(true);
				}
			}
		}
		if (GetGameUIMan()->GetActiveDialog() != this)
		{
			Show(false);
		}		
		if (stricmp(m_szName,"Win_Garden_Seed") == 0)
		{
			GetDlgItem("Btn_Confirm")->Enable(GetCheckedRadioButton(1)!=0);
			PAUISCROLL pScl = (PAUISCROLL)GetDlgItem("Scl_Item");
			if (m_iBarLevel != pScl->GetBarLevel())
			{
				m_iBarLevel = pScl->GetBarLevel();
				UpdateSeed();
			}
		}
	}
}

void CDlgGardenMan::OnShowDialog()
{
	if (stricmp(m_szName,"Win_Garden") == 0)
	{
		GetGameUIMan()->m_pDlgMinimizeBar[0]->RestoreDialog(this);
	}
	else if (stricmp(m_szName,"Win_Garden_PlotMethod") == 0)
	{
		PAUILABEL pLabTitle = (PAUILABEL)GetDlgItem("Title");
		pLabTitle->SetText(GETSTRING(8629+m_iOperType));
		CheckRadioButton(1,0);
	}
	else if (stricmp(m_szName,"Win_Garden_Seed") == 0)
	{
		PAUIIMAGEPICTURE pImg;
		AString strName;
		for (int i=0; i<GARDENSEEDITEM_NUM; i++)
		{
			strName.Format("Img_%d", i+1);
			pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
			pImg->SetCover(NULL,0,1);
		}
		m_iSeedType = -1;
		UpdateSeed();
	}
	else if (stricmp(m_szName,"Win_Garden_Friendlist") == 0)
	{
		int i, j, k;
		CECFriendMan::GROUP *pGroup;
		P_AUITREEVIEW_ITEM pRoot, pItem;
		ACString strText, strLine, strLevel, strArea;
		CECWorld *pWorld = GetWorld();
		CELPrecinctSet *pSet = pWorld->GetPrecinctSet();
		CECFriendMan *pMan = GetHostPlayer()->GetFriendMan();
		PAUITREEVIEW pTree = (PAUITREEVIEW)GetDlgItem("Tv_FriendList");
		pTree->DeleteAllItems();

		for( i = 0; i < pMan->GetGroupNum(); i++ )
		{
			pGroup = pMan->GetGroupByIndex(i);
			if( pGroup->strName.GetLength() > 0 )
				pRoot = pTree->InsertItem(pGroup->strName);
			else
				pRoot = pTree->InsertItem(GetStringFromTable(537));
			pTree->SetItemData(pRoot, pGroup->iGroup);
			if( pGroup->iGroup > 0 )
				pTree->SetItemTextColor(pRoot, pGroup->color);
			
			for( j = 0; j < pGroup->aFriends.GetSize(); j++ )
			{
				strText = pGroup->aFriends[j]->GetName();
				if( pGroup->aFriends[j]->iLine >= 0 )
				{
					pItem = pTree->InsertItem(strText, pRoot, AUITREEVIEW_FIRST);
					pTree->SetItemTextColor(pItem, A3DCOLORRGB(255, 203, 74));
				}
				else
				{
					pItem = pTree->InsertItem(strText, pRoot);
					pTree->SetItemTextColor(pItem, A3DCOLORRGB(128, 128, 128));
				}
				pTree->SetItemData(pItem, pGroup->aFriends[j]->id);
				
				if( pGroup->aFriends[j]->nLevel < 0 )
					strLevel = GetStringFromTable(574);
				else
					strLevel.Format(_AL("%d"), pGroup->aFriends[j]->nLevel);
				
				if( pGroup->aFriends[j]->idArea < 0 )
					strArea = GetStringFromTable(574);
				else
				{
					for( k = 0; k < pSet->GetPrecinctNum(); k++ )
					{
						if( (int)pSet->GetPrecinct(k)->GetID() != pGroup->aFriends[j]->idArea )
							continue;
						
						strArea = pSet->GetPrecinct(k)->GetName();
						break;
					}
				}
				
				strLine = GetGameUIMan()->GetLine(pGroup->aFriends[j]->iLine);
				strText.Format(GetStringFromTable(573), strLine, strLevel, strArea);
				pTree->SetItemHint(pItem, strText);
			}
			
			pTree->Expand(pRoot, AUITREEVIEW_EXPAND_EXPAND);
		}
	}
}

void CDlgGardenMan::OnCommandGardenOpenBarn(const char* szCommand)
{
	//庄园仓库
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Garden_Inventory");
	pDlg->Show(!pDlg->IsShow());
}
void CDlgGardenMan::OnCommandGardenOpenProp(const char* szCommand)
{
	//庄园道具
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Garden_Prop");
	pDlg->Show(!pDlg->IsShow());
}
void CDlgGardenMan::OnCommandGardenCheckStatus(const char* szCommand)
{
	//查看下拉菜单
	PAUIDIALOG pDlg;
	if (m_bOwn)
		pDlg = GetGameUIMan()->GetDialog("Win_CheckStatus");
	else
		pDlg = GetGameUIMan()->GetDialog("Win_CheckStatusOther");
	PAUIOBJECT pObj = GetDlgItem("Btn_CheckStatus");
	pDlg->SetPosEx(pObj->GetPos().x+pObj->GetSize().cx/2,pObj->GetPos().y+pObj->GetSize().cy/2);
	pDlg->Show(true);
}

void CDlgGardenMan::OnCommandGardenMinimize(const char* szCommand)
{
	GetGameUIMan()->m_pDlgMinimizeBar[0]->MinimizeDialog(this, 
		GetGameUIMan()->GetStringFromTable(8651), CDlgMinimizeBar::MINIMIZEBAR_TYPE_GARDEN);
}
void CDlgGardenMan::OnCommandGardenExchange(const char* szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Garden_ExchangeMain");
	pDlg->Show(!pDlg->IsShow());
}

void CDlgGardenMan::OnCommandGardenLevelUp(const char* szCommand)
{
	htc2s_SendCmdLevelUp();
}

void CDlgGardenMan::OnCommandGardenSetting(const char* szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Garden_Setting");
	pDlg->Show(!pDlg->IsShow());
}

void CDlgGardenMan::OnCommandGardenFriendList(const char* szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Garden_Friendlist");
	pDlg->Show(!pDlg->IsShow());
}

void CDlgGardenMan::OnCommandGardenFriendGarden(const char* szCommand)
{
	PAUITREEVIEW pTree = (PAUITREEVIEW)GetDlgItem("Tv_FriendList");
	P_AUITREEVIEW_ITEM pItem = pTree->GetSelectedItem();
	if (pItem && pTree->GetParentItem(pItem) != pTree->GetRootItem())
	{
		htc2s_SendCmdViewFarm(pTree->GetItemData(pTree->GetSelectedItem()));
	}
}

void CDlgGardenMan::OnCommandGardenOperate(const char* szCommand)
{
	GetGameUIMan()->m_nCursorState = CECGameUIMan::CURSOR_STATE_GARDENOPERATE;
	//庄园操作
	m_iOperType = atoi(szCommand + strlen("oper"));
	CDlgGardenMan *pGardenSetting = (CDlgGardenMan*)GetGameUIMan()->GetDialog("Win_Garden_Setting");
	PAUICHECKBOX pChk = (PAUICHECKBOX)pGardenSetting->GetDlgItem("Chk_HandCheck");
	switch (m_iOperType)
	{
	case OPERTYPE_PLOT:
	case OPERTYPE_WATER:
	case OPERTYPE_PEST:
	case OPERTYPE_WEED:
	case OPERTYPE_HARVEST:
	case OPERTYPE_STEAL:
		{
			if (!pChk->IsChecked())
				OnCommandGardenOperType("");
			else
				GetGameUIMan()->GetDialog("Win_Garden_PlotMethod")->Show(true);
			break;
		}
	case OPERTYPE_SEED:
		{
			GetGameUIMan()->GetDialog("Win_Garden_Seed")->Show(true);
			GetGame()->ChangeCursor(RES_CUR_NORMAL);
			break;
		}
	}
	if (pChk->IsChecked())
		GetGame()->ChangeCursor(RES_CUR_NORMAL);
}
void CDlgGardenMan::OnCommandGardenCool(const char* szCommand)
{
	if (stricmp(m_szName, "Win_Garden")==0)
	{
		htc2s_SendCmdViewFarm(GetHostPlayer()->GetCharacterID());
	}
	else
		CheckRadioButton(1,0);
}

void CDlgGardenMan::OnCommandGardenSeedSelect(const char* szCommand)
{
	if (m_iSeedType>0)
	{
		Show(false);
		GetGame()->ChangeCursor(RES_CUR_SEED1);
	}
	else
		MSGBOX(GETSTRING(8594));
}


void CDlgGardenMan::OnCommandGardenOperType(const char* szCommand)
{
	switch (m_iOperType)
	{
	case OPERTYPE_PLOT:
		{
			GetGame()->ChangeCursor(RES_CUR_PLOT1);
			break;
		}
	case OPERTYPE_SEED:
		{
			GetGame()->ChangeCursor(RES_CUR_SEED1);
			break;
		}
	case OPERTYPE_WATER:
		GetGame()->ChangeCursor(RES_CUR_WATER1);
		break;
	case OPERTYPE_PEST:
		GetGame()->ChangeCursor(RES_CUR_PEST1);
		break;
	case OPERTYPE_WEED:
		GetGame()->ChangeCursor(RES_CUR_WEED1);
		break;
	case OPERTYPE_HARVEST:
		GetGame()->ChangeCursor(RES_CUR_HARVEST1);
		break;
	case OPERTYPE_STEAL:
		GetGame()->ChangeCursor(RES_CUR_STEAL1);
		break;
	}
	if (0==stricmp(m_szName,"Win_Garden_PlotMethod"))
	{
		m_iActionType = atoi(szCommand+strlen("RdoMethod_"));
		Show(false);
	}
}

// status check
void CDlgGardenMan::OnCommandGardenStatusCheck(const char* szCommand)
{
	int iType = atoi(szCommand+strlen("BtnDynanic_")) - 1;
	abase::vector<ACString> vecDynamic;
	PAUIDIALOG	pDlg;
	if (iType<=DYNAMICTYPE_ACTION)
	{
		PAUILABEL	pLabTitle;
		PAUILABEL	pLabText;
		PAUISTILLIMAGEBUTTON pClose;
		if (DYNAMICTYPE_GARDEN == iType)
		{
			vecDynamic = m_vecDynamicString[DYNAMICTYPE_GARDEN];
			if(m_bOwn)
				pDlg = GetGameUIMan()->GetDialog("Win_Garden_Mine");
			else
				pDlg = GetGameUIMan()->GetDialog("Win_Garden_Others");
		}
		else if (DYNAMICTYPE_ACTION == iType)
		{
			
			vecDynamic = m_vecDynamicString[DYNAMICTYPE_ACTION];
			if(m_bOwn)
				pDlg = GetGameUIMan()->GetDialog("Win_Garden_Action");
			else
				pDlg = GetGameUIMan()->GetDialog("Win_Garden_ActionOther");
		}
		pLabTitle = (PAUILABEL)pDlg->GetDlgItem("Lab_Title");
		pLabText = (PAUILABEL)pDlg->GetDlgItem("Txt_Action");
		pClose= (PAUISTILLIMAGEBUTTON)pDlg->GetDlgItem("Btn_Close");

		ACString strText;
		strText.Empty();
		int iSize = vecDynamic.size();
		for (int i=0; i<iSize; i++)
		{
			strText += vecDynamic[i];
			if (i<(iSize-1))
				strText += _AL("\r");
		}
		int nWidth, nHeight, nLines;
		A3DFTFont *pFont = pLabText->GetFont();
		pFont->Resize(pLabText->GetFontHeight() * GetGameUIMan()->GetWindowScale());
		AUI_GetTextRect(pFont, strText, nWidth, nHeight, nLines, 3);
		SIZE s = pDlg->GetFrame()->GetSizeLimit();
		int W = max(nWidth, pLabTitle->GetSize().cx) + s.cx + 6;
		int H = pLabTitle->GetSize().cy + s.cy + nHeight + 2;
		
		pDlg->SetSize(W, H);
		pLabText->SetPos(9, pLabTitle->GetPos(true).y + pLabTitle->GetSize().cy + 3);
		pLabText->SetText(strText);
		pLabText->SetData(iSize);
		pClose->SetPos(W - pClose->GetSize().cx, 0);
		pDlg->AlignTo(this, AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_LEFT,
			AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
		pDlg->SetData(1);
		pDlg->Show(true);
		Show(false);
	}
	else
	{
		abase::vector<int> dynamicIds;
		AString strButtonName;
		if (DYNAMICTYPE_LOVEGARDEN == iType)
		{
			pDlg = GetGameUIMan()->GetDialog("Win_CheckStatusLove");
			vecDynamic = m_vecDynamicString[DYNAMICTYPE_LOVEGARDEN];
			dynamicIds = m_vecLoveGardenIds;
			strButtonName = "Btn_CheckLove";
		}
		else if (DYNAMICTYPE_FRUITS == iType)
		{
			pDlg = GetGameUIMan()->GetDialog("Win_CheckStatusMature");
			vecDynamic = m_vecDynamicString[DYNAMICTYPE_FRUITS];
			dynamicIds = m_vecMatureGardenIds;
			strButtonName = "Btn_CheckMature";
		}
		if (vecDynamic.size()!=dynamicIds.size())
		{
			return;
		}
		PAUISCROLL pScl = (PAUISCROLL)pDlg->GetDlgItem("Scl_1");
		AString strName;
		PAUISTILLIMAGEBUTTON pBtn;
		UINT i;
		
		if (vecDynamic.size()<=10)
		{
			pScl->Show(false);
			if (0==vecDynamic.size())
			{
				pBtn = (PAUISTILLIMAGEBUTTON)pDlg->GetDlgItem(strButtonName + "1");
				pBtn->SetText(GETSTRING(780));
				pBtn->SetData(0);
				for (i=1;i<10;i++)
				{
					strName.Format(strButtonName + "%d", i+1);
					pBtn = (PAUISTILLIMAGEBUTTON)pDlg->GetDlgItem(strName);
					pBtn->Show(false);
				}
			}
			else
			{
				for (i=0; i<vecDynamic.size(); i++)
				{
					strName.Format(strButtonName + "%d", i+1);
					pBtn = (PAUISTILLIMAGEBUTTON)pDlg->GetDlgItem(strName);
					pBtn->SetText(vecDynamic[i]);
					pBtn->SetData(dynamicIds[i]);
					pBtn->Show(true);
				}
				for (;i<10;i++)
				{
					strName.Format(strButtonName + "%d", i+1);
					pBtn = (PAUISTILLIMAGEBUTTON)pDlg->GetDlgItem(strName);
					pBtn->Show(false);
				}
			}
			pDlg->SetSize(pBtn->GetSize().cx, pBtn->GetSize().cy * (vecDynamic.size()>0 ? vecDynamic.size() : 1));
		}
		else
		{
			pScl->Show(true);
			pScl->SetScrollRange(0,vecDynamic.size() - 10);
			pScl->SetBarLength(-1);
			pScl->SetScrollStep(1);
			pScl->SetBarLevel(0);
			for (i=0; i<10; i++)
			{
				strName.Format(strButtonName + "%d", i+1);
				pBtn = (PAUISTILLIMAGEBUTTON)pDlg->GetDlgItem(strName);
				pBtn->SetText(vecDynamic[i]);
				pBtn->SetData(dynamicIds[i]);
				pBtn->Show(true);
			}
			pDlg->SetSize(pBtn->GetSize().cx, pBtn->GetSize().cy * 10);
		}
		Show(false);
		pDlg->Show(true);
		pDlg->AlignTo(this, AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_LEFT,
			AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
	}
}

void CDlgGardenMan::OnCommandGardenCheckLove(const char* szCommand)
{
	int index = atoi(szCommand + strlen("Btn_CheckLove"))-1;
	PAUIOBJECT pObj = GetDlgItem(szCommand);
	if (m_vecLoveGardenIds.size()>(UINT)(index+m_iBarLevel) && pObj->GetData()>0)
	{
		htc2s_SendCmdViewFarm(pObj->GetData());
	}
}

void CDlgGardenMan::OnCommandGardenCheckMature(const char* szCommand)
{
	int index = atoi(szCommand + strlen("Btn_CheckMature"))-1;
	PAUIOBJECT pObj = GetDlgItem(szCommand);
	if (m_vecMatureGardenIds.size()>(UINT)(index+m_iBarLevel) && pObj->GetData()>0)
	{
		htc2s_SendCmdViewFarm(pObj->GetData());
	}
}

// global
void CDlgGardenMan::OnCommandCancel(const char* szCommand)
{
	if (stricmp(m_szName,"Win_Garden") == 0)
	{
		m_pAUIManager->RespawnMessage();
	}
	else
		Show(false);
	if (stricmp(m_szName,"Win_Garden_PlotMethod") == 0)
		m_iOperType = 0;
	else if (stricmp(m_szName,"Win_Garden_Seed") == 0)
	{
		m_iOperType = 0;
		if (GetCheckedRadioButton(1)==0)
			GetGame()->ChangeCursor(RES_CUR_NORMAL);
	}
}

void CDlgGardenMan::OnCommandClose(const char* szCommand)
{
	Show(false);
	GetGameUIMan()->GetDialog("Win_Garden_Prop")->Show(false);
	GetGameUIMan()->GetDialog("Win_Garden_Inventory")->Show(false);
	GetGameUIMan()->m_nCursorState = CECGameUIMan::CURSOR_STATE_NORMAL;
}

void CDlgGardenMan::OnEventLButtonDownPlant(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	switch (m_iOperType)
	{
	case OPERTYPE_PLOT:
		{
			GetGame()->ChangeCursor(RES_CUR_PLOT2);
			break;
		}
	case OPERTYPE_SEED:
		{
			GetGame()->ChangeCursor(RES_CUR_SEED2);
			break;
		}
	case OPERTYPE_WATER:
		GetGame()->ChangeCursor(RES_CUR_WATER2);
		break;
	case OPERTYPE_PEST:
		GetGame()->ChangeCursor(RES_CUR_PEST2);
		break;
	case OPERTYPE_WEED:
		GetGame()->ChangeCursor(RES_CUR_WEED2);
		break;
	case OPERTYPE_HARVEST:
		GetGame()->ChangeCursor(RES_CUR_HARVEST2);
		break;
	case OPERTYPE_STEAL:
		GetGame()->ChangeCursor(RES_CUR_STEAL2);
		break;
	}
}
void CDlgGardenMan::OnEventLButtonUpPlant(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if (g_pGame->GetCurCursor() == RES_CUR_NORMAL)
		return;
	UINT id = pObj->GetData();
	dlg_field_info fi;
	if (id<HOMETOWNFARMPLANTOWNNUM)
	{
		if (id<m_vecFarmInfo[FARMTYPE_OWN].size())
			fi = m_vecFarmInfo[FARMTYPE_OWN][id];
		else
			return;
	}
	else
	{
		if ((id-HOMETOWNFARMPLANTOWNNUM)<m_vecFarmInfo[FARMTYPE_LOVE].size())
			fi = m_vecFarmInfo[FARMTYPE_LOVE][id-HOMETOWNFARMPLANTOWNNUM];
		else
			return;
	}
	ACString strText;
	switch (m_iOperType)
	{
	case OPERTYPE_PLOT:
		{
			if (!m_bOwn && id<HOMETOWNFARMPLANTOWNNUM)
			{
				ADDCHAT(GETSTRING(8672));
			}
			else if (m_bOwn && id>=HOMETOWNFARMPLANTOWNNUM)
			{
				ADDCHAT(GETSTRING(8671));
			}
			else if (fi.state&(1<<HTF_STATE_PLOW))
			{
				ADDCHAT(GETSTRING(8710));
			}
			else if (m_bOwn && id>=HOMETOWNFARMPLANTOWNNUM)
			{
				ADDCHAT(GETSTRING(8671));
			}
			else if (fi.state&(1<<HTF_STATE_PLANT) && (fi.crop_state&(1<<HTF_STATE_HARVEST))==0 && (fi.crop_state&(1<<HTF_STATE_WILT))==0)
			{
				PAUIDIALOG pMsgBox;
				GetGameUIMan()->MessageBox("Game_GardenPlow", GetGameUIMan()->GetStringFromTable(8660), MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
				pMsgBox->SetData(fi.field_id);
			}
			else if (m_bOwn && id<HOMETOWNFARMPLANTOWNNUM )
			{
				htc2s_SendCmdPlowField(m_iOwnerId, fi.field_id, m_iActionType);
				//ADDCHAT(strText);
			}
			else if (!m_bOwn && id>=HOMETOWNFARMPLANTOWNNUM)
			{
				htc2s_SendCmdPlowField(m_iOwnerId, fi.field_id, m_iActionType);
				strText.Format(GETSTRING(10005),EMOTIONSTRING(m_iActionType),FRIENDNAME(m_iOwnerId),GETSTRING(8593));
				ADDCHAT(strText);
			}
			else if (fi.state&(1<<HTF_STATE_INCULT))
			{
				ADDCHAT(GETSTRING(8663));
			}
			break;

		}
	case OPERTYPE_SEED:
		if (!m_bOwn && id<HOMETOWNFARMPLANTOWNNUM)
		{
			ADDCHAT(GETSTRING(8672));
		}
		else if (m_bOwn && id>=HOMETOWNFARMPLANTOWNNUM)
		{
			ADDCHAT(GETSTRING(8671));
		}
		else if (fi.state&(1<<HTF_STATE_INCULT))
		{
			ADDCHAT(GETSTRING(8681));
		}
		else if (fi.state&(1<<HTF_STATE_PLANT))
		{
			ADDCHAT(GETSTRING(8662));
		}
		else if (fi.state&(1<<HTF_STATE_PLOW))
			htc2s_SendCmdSowField(m_iOwnerId, fi.field_id, m_iSeedType);
		else
			ADDCHAT(GETSTRING(9061));
		
		break;
	case OPERTYPE_WATER:
		if (fi.state&(1<<HTF_STATE_INCULT))
		{
			ADDCHAT(GETSTRING(8681));
		}
		else if (fi.state&(1<<HTF_STATE_PLANT))
			htc2s_SendCmdWaterField(m_iOwnerId, fi.field_id, m_iActionType);
		else
			ADDCHAT(GETSTRING(8676));
		break;
	case OPERTYPE_PEST:
		if (fi.state&(1<<HTF_STATE_INCULT))
		{
			ADDCHAT(GETSTRING(8681));
		}
		else if (fi.state&(1<<HTF_STATE_PLANT))
			htc2s_SendCmdPestField(m_iOwnerId, fi.field_id, m_iActionType);
		else
			ADDCHAT(GETSTRING(8677));
		break;
	case OPERTYPE_WEED:
		if (fi.state&(1<<HTF_STATE_INCULT))
		{
			ADDCHAT(GETSTRING(8681));
		}
		else if (fi.state&(1<<HTF_STATE_PLANT))
			htc2s_SendCmdWeedField(m_iOwnerId, fi.field_id, m_iActionType);
		else
			ADDCHAT(GETSTRING(8678));
		break;
	case OPERTYPE_HARVEST:
		if (fi.crop_state&(1<<HTF_STATE_MATURE))
		{
			if (m_bOwn ||
				!m_bOwn && id>=HOMETOWNFARMPLANTOWNNUM && fi.friend_id == GetHostPlayer()->GetCharacterID())
				htc2s_SendCmdHarvestField(m_iOwnerId, fi.field_id, m_iActionType);
		}
		else
			ADDCHAT(GETSTRING(9109));
		break;
	case OPERTYPE_STEAL:
		if (fi.crop_state&(1<<HTF_STATE_MATURE) && !m_bOwn)
		{
			if (fi.friend_id == GetHostPlayer()->GetCharacterID())
				htc2s_SendCmdHarvestField(m_iOwnerId, fi.field_id, m_iActionType);
			else
				htc2s_SendCmdStealField(m_iOwnerId, fi.field_id, m_iActionType);
		}
		else if((fi.crop_state&(1<<HTF_STATE_MATURE)) == 0 && !m_bOwn &&
			((fi.crop_state&(1<<HTF_STATE_SPROUT)) || (fi.crop_state&(1<<HTF_STATE_IMMATURE))|| (fi.crop_state&(1<<HTF_STATE_PREMATURE))))
			ADDCHAT(GETSTRING(8682));
		else
			ADDCHAT(GETSTRING(8679));
		break;
	}
	
	switch (m_iOperType)
	{
	case OPERTYPE_PLOT:
		{
			GetGame()->ChangeCursor(RES_CUR_PLOT1);
			break;
		}
	case OPERTYPE_SEED:
		{
			GetGame()->ChangeCursor(RES_CUR_SEED1);
			break;
		}
	case OPERTYPE_WATER:
		GetGame()->ChangeCursor(RES_CUR_WATER1);
		break;
	case OPERTYPE_PEST:
		GetGame()->ChangeCursor(RES_CUR_PEST1);
		break;
	case OPERTYPE_WEED:
		GetGame()->ChangeCursor(RES_CUR_WEED1);
		break;
	case OPERTYPE_HARVEST:
		GetGame()->ChangeCursor(RES_CUR_HARVEST1);
		break;
	case OPERTYPE_STEAL:
		GetGame()->ChangeCursor(RES_CUR_STEAL1);
		break;
	}
}

void CDlgGardenMan::OnEventLButtonDownItem(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if (stricmp(m_szName,"Win_Garden_Seed") != 0)
		return;
	if( !strstr(pObj->GetName(), "Img_") )
		return;
	int index = atoi(pObj->GetName()+strlen("Img_"));
	m_iSeedType = m_vecCropEss[index+m_iFirstLine*3-1]->id;
	PAUIIMAGEPICTURE pImg;
	AString strName;
	for (int i=0; i<GARDENSEEDITEM_NUM; i++)
	{
		strName.Format("Img_%d", i+1);
		pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		if (pImg == pObj)
			pImg->SetCover(CDlgGardenQShop::m_pA2DSpriteItemSelect,0,1);
		else
			pImg->SetCover(NULL,0,1);
	}
}

void CDlgGardenMan::OnEventLButtonDownFriendGarden(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	OnCommandGardenFriendGarden("");
}

void CDlgGardenMan::OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);
	int zDelta = (short)HIWORD(wParam);
	if( zDelta > 0 )
		zDelta = 1;
	else
		zDelta = -1;
	if (stricmp(m_szName,"Win_CheckStatusLove") == 0)
	{
		PAUISCROLL pScl = (PAUISCROLL)GetDlgItem("Scl_1");
		if (!pScl->IsShow()) return;
		pScl->SetBarLevel(pScl->GetBarLevel() - zDelta);
	}
	else if (stricmp(m_szName,"Win_CheckStatusMature") == 0)
	{
		AString strName;
		PAUISCROLL pScl = (PAUISCROLL)GetDlgItem("Scl_1");
		if (!pScl->IsShow()) return;
		pScl->SetBarLevel(pScl->GetBarLevel() - zDelta);
	}
	else if (stricmp(m_szName,"Win_Garden_Seed") == 0)
	{
		PAUISCROLL pScl = (PAUISCROLL)GetDlgItem("Scl_Item");
		if (!pScl->IsShow()) return;
		pScl->SetBarLevel(pScl->GetBarLevel() - zDelta);
		UpdateSeed();
	}
}

void CDlgGardenMan::OnPrtc_ViewFarm(void *pData)
{
	if (!IsShow())
		Show(true);
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Option");
	if (pDlg->IsShow())
	{
		pDlg->Show(false);
	}
	htc2s_SendCmdGetPackage();
	CMD::farm_view* pFarm = (CMD::farm_view*)pData;
	farm_info &pFarmInfo = pFarm->farm;
	DealFarmInfo(&pFarmInfo);
}

void CDlgGardenMan::OnPrtc_ViewFarmField(void *pData)
{
	CMD::farm_view_field* pFarm = (CMD::farm_view_field*)pData;

	abase::hash_map<int, int>::iterator it = m_mapId2Index.find(pFarm->field_id);
	if (it==m_mapId2Index.end())
	{
		assert(0);
		return;
	}
	int index = m_mapId2Index[pFarm->field_id];
	if (index<HOMETOWNFARMPLANTOWNNUM)
	{
		if ((UINT)index>=m_vecFarmInfo[FARMTYPE_OWN].size())
		{
			assert(0);
			return;
		}
		
		m_vecFarmInfo[FARMTYPE_OWN][index].field_id = pFarm->field_id;
		m_vecFarmInfo[FARMTYPE_OWN][index].field_type = pFarm->field.field_type;
		m_vecFarmInfo[FARMTYPE_OWN][index].state = pFarm->field.state;
		m_vecFarmInfo[FARMTYPE_OWN][index].friend_id = pFarm->field.friend_id;
		m_vecFarmInfo[FARMTYPE_OWN][index].id = pFarm->field.crop.id;
		m_vecFarmInfo[FARMTYPE_OWN][index].seed_time = pFarm->field.crop.seed_time;
		m_vecFarmInfo[FARMTYPE_OWN][index].protect_period = pFarm->field.crop.protect_period;
		m_vecFarmInfo[FARMTYPE_OWN][index].ripe_time = pFarm->field.crop.ripe_time;
		m_vecFarmInfo[FARMTYPE_OWN][index].crop_state = pFarm->field.crop.state;
		m_vecFarmInfo[FARMTYPE_OWN][index].fruit_left = pFarm->field.crop.fruit_left;
		m_vecFarmInfo[FARMTYPE_OWN][index].fruit_total = pFarm->field.crop.fruit_total;
	}
	else
	{
		if (UINT(index-HOMETOWNFARMPLANTOWNNUM)>=m_vecFarmInfo[FARMTYPE_LOVE].size())
		{
			assert(0);
			return;
		}
		
		m_vecFarmInfo[FARMTYPE_LOVE][index-HOMETOWNFARMPLANTOWNNUM].field_id = pFarm->field_id;
		m_vecFarmInfo[FARMTYPE_LOVE][index-HOMETOWNFARMPLANTOWNNUM].field_type = pFarm->field.field_type;
		m_vecFarmInfo[FARMTYPE_LOVE][index-HOMETOWNFARMPLANTOWNNUM].state = pFarm->field.state;
		m_vecFarmInfo[FARMTYPE_LOVE][index-HOMETOWNFARMPLANTOWNNUM].friend_id = pFarm->field.friend_id;
		m_vecFarmInfo[FARMTYPE_LOVE][index-HOMETOWNFARMPLANTOWNNUM].id = pFarm->field.crop.id;
		m_vecFarmInfo[FARMTYPE_LOVE][index-HOMETOWNFARMPLANTOWNNUM].seed_time = pFarm->field.crop.seed_time;
		m_vecFarmInfo[FARMTYPE_LOVE][index-HOMETOWNFARMPLANTOWNNUM].protect_period = pFarm->field.crop.protect_period;
		m_vecFarmInfo[FARMTYPE_LOVE][index-HOMETOWNFARMPLANTOWNNUM].ripe_time = pFarm->field.crop.ripe_time;
		m_vecFarmInfo[FARMTYPE_LOVE][index-HOMETOWNFARMPLANTOWNNUM].crop_state = pFarm->field.crop.state;
		m_vecFarmInfo[FARMTYPE_LOVE][index-HOMETOWNFARMPLANTOWNNUM].fruit_left = pFarm->field.crop.fruit_left;
		m_vecFarmInfo[FARMTYPE_LOVE][index-HOMETOWNFARMPLANTOWNNUM].fruit_total = pFarm->field.crop.fruit_total;
	}
	UpdatePlantState(index);
}

void CDlgGardenMan::OnPrtc_LevelUp(void *pData)
{
	DealFarmInfo(pData);
}

void CDlgGardenMan::DealFarmInfo(void *pData)
{
	ResetState();
	farm_info& pFarmInfo = *(farm_info*)pData;

	m_iFarmState = pFarmInfo.state;
	m_iFarmProtectPeriod = pFarmInfo.protect_period;
 	m_pImgGuardBeast->Show((m_iFarmState&(1<<HTF_STATE_FARMPROTECTBEGIN)) > 0);
	UINT i,iSize;
	for (i=0; i<BEE_NUM; i++)
	{
		m_pImgBee[i]->Show((m_iFarmState&(1<<HTF_STATE_FARMSPEEDGROWBEGIN)) > 0);
		m_pImgButterfly[i]->Show((m_iFarmState&(1<<HTF_STATE_FARMSPEEDGROWBEGIN)) > 0);
	}

	m_bOwn = pFarmInfo.owner.id == GetHostPlayer()->GetCharacterID();
	if (m_bOwn)
	{
		m_iOwnerMoney = pFarmInfo.owner.money;
		htc2s_SendCmdListLoveFields();
		htc2s_SendCmdListMatureFields();
	}
	if (m_iOwnerId != pFarmInfo.owner.id)
	{
		if (!IsShow())
			Show(true);
		m_pLabTitle->SetText(_AL(""));
		m_iOwnerId = pFarmInfo.owner.id;
	}
	m_iOwnerCharm	= pFarmInfo.owner.charm;
	m_iLevel		= pFarmInfo.owner.level;

	//文字显示
	ACString strText;
	PAUILABEL pLabCharm = (PAUILABEL)GetDlgItem("Txt_Charm");
	GetGameUIMan()->Int64ToStr(pFarmInfo.owner.charm, strText);
	pLabCharm->SetText(strText);
	PAUILABEL pLabLevel = (PAUILABEL)GetDlgItem("Txt_Lv");
	strText.Format(_AL("%d"),pFarmInfo.owner.level);
	pLabLevel->SetText(strText);

	//土地信息
	int iNumOwn = 0;
	int iNumLove = 0;
	for ( i=0; i<pFarmInfo.fields.size(); i++)
	{
		field_info fi = pFarmInfo.fields[i];
		dlg_field_info fitemp;
		fitemp.field_id = fi.field_id;
		fitemp.field_type = fi.field_type;
		fitemp.state = fi.state;
		fitemp.friend_id = fi.friend_id;
		fitemp.strFriendName = ACString((ACHAR*)fi.friend_name, fi.friend_name_size/sizeof(ACHAR));
		fitemp.id = fi.crop.id;
		fitemp.seed_time = fi.crop.seed_time;
		fitemp.ripe_time = fi.crop.ripe_time;
		fitemp.protect_period = fi.crop.protect_period;
		fitemp.crop_state = fi.crop.state;
		fitemp.fruit_left = fi.crop.fruit_left;
		fitemp.fruit_total = fi.crop.fruit_total;
		m_vecFarmInfo[fi.field_type].push_back(fitemp);
		switch (fi.field_type)
		{
		case FARMTYPE_INCULT:
			{
				break;
			}
		case FARMTYPE_OWN:
			{
				UpdatePlantState(iNumOwn, fi.state, fi.crop.state);
				m_mapId2Index[fitemp.field_id] = iNumOwn;
				if (fi.state&(1<<HTF_STATE_PLANT))
				{
					AUIOBJECT_SETPROPERTY p;
					sprintf(p.fn, "garden\\%d-bigger.tga", fitemp.id);
					m_pImgPlants[iNumOwn][IMAGESTATE_BIGGER]->SetProperty("Image File", &p);
					sprintf(p.fn, "garden\\%d-pre.tga", fitemp.id);
					m_pImgPlants[iNumOwn][IMAGESTATE_LESSMATURE]->SetProperty("Image File", &p);
					sprintf(p.fn, "garden\\%d-mature.tga", fitemp.id);
					m_pImgPlants[iNumOwn][IMAGESTATE_MATURE]->SetProperty("Image File", &p);
				}
				iNumOwn++;
				break;
			}
		case FARMTYPE_LOVE:
			{
				UpdatePlantState(iNumLove+HOMETOWNFARMPLANTOWNNUM, fi.state, fi.crop.state);
				m_mapId2Index[fitemp.field_id] = iNumLove+HOMETOWNFARMPLANTOWNNUM;
				
				if (fi.state&(1<<HTF_STATE_PLANT))
				{
					AUIOBJECT_SETPROPERTY p;
					sprintf(p.fn, "garden\\%d-bigger.tga", fitemp.id);
					m_pImgPlants[iNumLove+HOMETOWNFARMPLANTOWNNUM][IMAGESTATE_BIGGER]->SetProperty("Image File", &p);
					sprintf(p.fn, "garden\\%d-pre.tga", fitemp.id);
					m_pImgPlants[iNumLove+HOMETOWNFARMPLANTOWNNUM][IMAGESTATE_LESSMATURE]->SetProperty("Image File", &p);
					sprintf(p.fn, "garden\\%d-mature.tga", fitemp.id);
					m_pImgPlants[iNumLove+HOMETOWNFARMPLANTOWNNUM][IMAGESTATE_MATURE]->SetProperty("Image File", &p);
				}
				iNumLove++;
				break;
			}
		default:
			assert(0);
		}
		if (iNumOwn>HOMETOWNFARMPLANTOWNNUM)
		{
			assert(0);
			break;
		}
		if (iNumLove>HOMETOWNFARMPLANTLOVENUM)
		{
			assert(0);
			break;
		}
	}

	//dynamic
	iSize = pFarmInfo.farm_dynamic.size();
	for (i=0; i<iSize; i++)
	{
		GNET::Octets octets = pFarmInfo.farm_dynamic[i];
		char *data = (char*)octets.begin();
		DYNAMIC::dynamic_header *pDynamicHeader = (DYNAMIC::dynamic_header*)data;
		ACString strTime;
		ACString strText;
		ACString strSponsorName;
		ACString strTargetName;
		const long t = pDynamicHeader->time;
		tm *gtime = localtime((const time_t*)&t);
		tm dTime = *gtime;
		tm tNow = g_pGame->GetServerLocalTime();
		if (tNow.tm_mday == dTime.tm_mday)
			strTime = GetGameUIMan()->GetStringFromTable(8606);
		else if (tNow.tm_mday == (dTime.tm_mday+1))
			strTime = GetGameUIMan()->GetStringFromTable(8607);
		else
			strTime.Format(GetGameUIMan()->GetStringFromTable(8608),dTime.tm_mon+1,dTime.tm_mday);
		data = data + sizeof(DYNAMIC::dynamic_header);
		switch (pDynamicHeader->action)
		{
// 		case EXCHG_MONEY:
// 			{
// 				DYNAMIC::exchg_money *pDat = (DYNAMIC::exchg_money*)data;
// 				ACString strMoney1, strMoney2;
// 				GetGameUIMan()->Int64ToStr(pDat->local_money, strMoney1);
// 				GetGameUIMan()->Int64ToStr(pDat->foreign_money, strMoney2);
// 				if (pDat->type == 0)
// 				{
// 					if (m_bOwn)
// 						strText.Format(GETSTRING(10000), strMoney1, strMoney2, strTime);
// 					else
// 						strText.Format(GETSTRING(10026), strMoney1, strMoney2, strTime);
// 				}
// 				else if (pDat->type == 1)
// 				{
// 					if (m_bOwn)
// 						strText.Format(GETSTRING(10001), strMoney1, strMoney2, strTime);
// 					else
// 						strText.Format(GETSTRING(10027), strMoney1, strMoney2, strTime);
// 				}
// 				m_vecDynamicString[DYNAMICTYPE_GARDEN].push_back(strText);
// 				break;
// 			}
// 		case LEVEL_UP:
// 			{
// 				DYNAMIC::level_up *pDat = (DYNAMIC::level_up*)data;
// 				if (m_bOwn)
// 					strText.Format(GETSTRING(10002), pDat->level, strTime);
// 				else
// 					strText.Format(GETSTRING(10170), pDat->level, strTime);
// 				m_vecDynamicString[DYNAMICTYPE_GARDEN].push_back(strText);
// 				break;
// 			}
// 		case BUY_ITEM:
// 			{
// 				DYNAMIC::buy_item *pDat = (DYNAMIC::buy_item*)data;
// 				if (m_bOwn)
// 				{
// 					strText.Format(GETSTRING(10013), _AL("名字"), strTime);
// 					m_vecDynamicString[DYNAMICTYPE_GARDEN].push_back(strText);
// 				}
// 				break;
// 			}
		case USE_ITEM:
			{
				DYNAMIC::use_item *pDat = (DYNAMIC::use_item*)data;
				strSponsorName = ACString((ACHAR*)pDat->sponsor_name, pDat->sponsor_name_size/sizeof(ACHAR));
				strTargetName = ACString((ACHAR*)pDat->target_name, pDat->target_name_size/sizeof(ACHAR));
				CECIvtrItem *pItem = CECIvtrItem::CreateHomeTownItem(pDat->itemid, 0, HOMETOWN::ID_SPACE_ESSENCE);
				if (pItem && (pItem->GetClassID()==CECIvtrItem::ICID_HOMETOWN_TOOL))
				{
					if (m_bOwn)
					{
						strText.Format(GETSTRING(10015),strSponsorName,pItem->GetName(), strTime);
						m_vecDynamicString[DYNAMICTYPE_GARDEN].push_back(strText);
					}
					else
					{
						strText.Format(GETSTRING(10038),strSponsorName, pItem->GetName(), strTime);
						m_vecDynamicString[DYNAMICTYPE_GARDEN].push_back(strText);
					}
					delete pItem;
				}
				break;
			}
// 		case SELL_ITEM:
// 			{
// 				DYNAMIC::sell_item *pDat = (DYNAMIC::sell_item*)data;
//				strSponsorName = ACString((ACHAR*)pDat->sponsor_name, pDat->sponsor_name_size/sizeof(ACHAR));
//				strTargetName = ACString((ACHAR*)pDat->target_name, pDat->target_name_size/sizeof(ACHAR));
// 				if (m_bOwn)
// 				{
// 					strText.Format(GETSTRING(10014), _AL("卖出物品名"), strTime);
// 					m_vecDynamicString[DYNAMICTYPE_GARDEN].push_back(strText);
// 				}
// 				break;
// 			}
		case FARM_PLOW_FIELD:
			{
				DYNAMIC::plow_field *pDat = (DYNAMIC::plow_field*)data;
				strSponsorName = ACString((ACHAR*)pDat->sponsor_name, pDat->sponsor_name_size/sizeof(ACHAR));
				strTargetName = ACString((ACHAR*)pDat->target_name, pDat->target_name_size/sizeof(ACHAR));
				if (m_bOwn)
				{
					strText.Format(GETSTRING(10016),strSponsorName, EMOTIONSTRING(pDat->emotion), strTime);
					m_vecDynamicString[DYNAMICTYPE_GARDEN].push_back(strText);
				}
				else
				{
					strText.Format(GETSTRING(10039), strSponsorName, EMOTIONSTRING(pDat->emotion), strTime);
					m_vecDynamicString[DYNAMICTYPE_GARDEN].push_back(strText);
				}
				break;
			}
		case FARM_SOW_FIELD:
			{
				DYNAMIC::sow_field *pDat = (DYNAMIC::sow_field*)data;
				strSponsorName = ACString((ACHAR*)pDat->sponsor_name, pDat->sponsor_name_size/sizeof(ACHAR));
				strTargetName = ACString((ACHAR*)pDat->target_name, pDat->target_name_size/sizeof(ACHAR));
				
				HometownTemplate* pDB = g_pGame->GetHomeTownDataMan();
				HOMETOWN::DATA_TYPE DataType;
				const HOMETOWN::CROP_ESSENCE* ess;
				ess = (HOMETOWN::CROP_ESSENCE*)pDB->get_data_ptr(pDat->seed_id, HOMETOWN::ID_SPACE_ESSENCE, DataType, ess);
				if (m_bOwn)
				{
					strText.Format(GETSTRING(10017),strSponsorName, ess->name, strTime);
					m_vecDynamicString[DYNAMICTYPE_GARDEN].push_back(strText);
				}
				else
				{
					strText.Format(GETSTRING(10040), strSponsorName, ess->name, strTime);
					m_vecDynamicString[DYNAMICTYPE_GARDEN].push_back(strText);
				}
				break;
			}
		case FARM_WATER_FIELD:
			{
				DYNAMIC::water_field *pDat = (DYNAMIC::water_field*)data;
				strSponsorName = ACString((ACHAR*)pDat->sponsor_name, pDat->sponsor_name_size/sizeof(ACHAR));
				strTargetName = ACString((ACHAR*)pDat->target_name, pDat->target_name_size/sizeof(ACHAR));
				if (m_bOwn)
				{
					strText.Format(GETSTRING(10018), strSponsorName, EMOTIONSTRING(pDat->emotion), strTime);
					m_vecDynamicString[DYNAMICTYPE_GARDEN].push_back(strText);
				}
				else
				{
					strText.Format(GETSTRING(10041), strSponsorName, EMOTIONSTRING(pDat->emotion), strTime);
					m_vecDynamicString[DYNAMICTYPE_GARDEN].push_back(strText);
				}
				break;
			}
		case FARM_PEST_FIELD:
			{
				DYNAMIC::pest_field *pDat = (DYNAMIC::pest_field*)data;
				strSponsorName = ACString((ACHAR*)pDat->sponsor_name, pDat->sponsor_name_size/sizeof(ACHAR));
				strTargetName = ACString((ACHAR*)pDat->target_name, pDat->target_name_size/sizeof(ACHAR));
				if (m_bOwn)
				{
					strText.Format(GETSTRING(10019), strSponsorName, EMOTIONSTRING(pDat->emotion), strTime);
					m_vecDynamicString[DYNAMICTYPE_GARDEN].push_back(strText);
				}
				else
				{
					strText.Format(GETSTRING(10042), strSponsorName, EMOTIONSTRING(pDat->emotion), strTime);
					m_vecDynamicString[DYNAMICTYPE_GARDEN].push_back(strText);
				}
				break;
			}
		case FARM_WEED_FIELD:
			{
				DYNAMIC::weed_field *pDat = (DYNAMIC::weed_field*)data;
				strSponsorName = ACString((ACHAR*)pDat->sponsor_name, pDat->sponsor_name_size/sizeof(ACHAR));
				strTargetName = ACString((ACHAR*)pDat->target_name, pDat->target_name_size/sizeof(ACHAR));
				if (m_bOwn)
				{
					strText.Format(GETSTRING(10020), strSponsorName, EMOTIONSTRING(pDat->emotion), strTime);
					m_vecDynamicString[DYNAMICTYPE_GARDEN].push_back(strText);
				}
				else
				{
					strText.Format(GETSTRING(10043),  strSponsorName, EMOTIONSTRING(pDat->emotion), strTime);
					m_vecDynamicString[DYNAMICTYPE_GARDEN].push_back(strText);
				}
				break;
			}
		case FARM_HARVEST_FIELD:
			{
				DYNAMIC::harvest_lovefield *pDat = (DYNAMIC::harvest_lovefield*)data;
				strSponsorName = ACString((ACHAR*)pDat->sponsor_name, pDat->sponsor_name_size/sizeof(ACHAR));
				strTargetName = ACString((ACHAR*)pDat->target_name, pDat->target_name_size/sizeof(ACHAR));
				CECIvtrItem *pItem = CECIvtrItem::CreateHomeTownItem(pDat->fruit_id, 0, HOMETOWN::ID_SPACE_ESSENCE);
				if (pItem && (pItem->GetClassID()==CECIvtrItem::ICID_HOMETOWN_FRUIT))
				{
					if (m_bOwn)
					{
						if (pDat->target == GetHostPlayer()->GetCharacterID())
							if (pDat->drop_count>0)
								strText.Format(GETSTRING(10022), strSponsorName, pDat->gain_count, pItem->GetName(), pDat->drop_count, strTime);
							else
								strText.Format(GETSTRING(10021), strSponsorName, pDat->gain_count, pItem->GetName(), strTime);
							else
								if (pDat->drop_count>0)
									strText.Format(GETSTRING(10024), strSponsorName, pDat->gain_count, pItem->GetName(), pDat->drop_count, strTime);
								else
									strText.Format(GETSTRING(10023), strSponsorName, pDat->gain_count, pItem->GetName(), strTime);
								m_vecDynamicString[DYNAMICTYPE_GARDEN].push_back(strText);
					}
					else
					{
						if (pDat->target == m_iOwnerId)
							if (pDat->drop_count>0)
								strText.Format(GETSTRING(10045), strSponsorName,pDat->gain_count, pItem->GetName(), pDat->drop_count, strTime);
							else
								strText.Format(GETSTRING(10044), strSponsorName,pDat->gain_count, pItem->GetName(), strTime);
							else //这儿处理包裹满的情况
								if (pDat->drop_count>0)
									strText.Format(GETSTRING(10047),strSponsorName, pDat->gain_count, pItem->GetName(), pDat->drop_count, strTime);
								else
									strText.Format(GETSTRING(10046),strSponsorName, pDat->gain_count, pItem->GetName(), strTime);
								m_vecDynamicString[DYNAMICTYPE_GARDEN].push_back(strText);
					}
					delete pItem;
				}
				break;
			}
		case FARM_STEAL_FIELD:
			{
				DYNAMIC::steal_field *pDat = (DYNAMIC::steal_field*)data;
				strSponsorName = ACString((ACHAR*)pDat->sponsor_name, pDat->sponsor_name_size/sizeof(ACHAR));
				strTargetName = ACString((ACHAR*)pDat->target_name, pDat->target_name_size/sizeof(ACHAR));
				CECIvtrItem *pItem = CECIvtrItem::CreateHomeTownItem(pDat->fruit_id, 0, HOMETOWN::ID_SPACE_ESSENCE);
				if (pItem && (pItem->GetClassID()==CECIvtrItem::ICID_HOMETOWN_FRUIT))
				{
					if (m_bOwn)
					{
						strText.Format(GETSTRING(10025),strSponsorName, EMOTIONSTRING(pDat->emotion), pDat->count, pItem->GetName(), strTime);
						m_vecDynamicString[DYNAMICTYPE_GARDEN].push_back(strText);
					}
					else
					{
						strText.Format(GETSTRING(10048), strSponsorName, EMOTIONSTRING(pDat->emotion), pDat->count, pItem->GetName(), strTime);
						m_vecDynamicString[DYNAMICTYPE_GARDEN].push_back(strText);
					}
					delete pItem;
				}
				break;
			}
		default:
			assert(0);
			break;
		}
	}
	iSize = pFarmInfo.action_dynamic.size();
	for (i=0; i<iSize; i++)
	{
		GNET::Octets octets = pFarmInfo.action_dynamic[i];
		char *data = (char*)octets.begin();
		DYNAMIC::dynamic_header *pDynamicHeader = (DYNAMIC::dynamic_header*)data;
		ACString strTime;
		ACString strText;
		ACString strSponsorName;
		ACString strTargetName;
		const long t = pDynamicHeader->time;
		tm *gtime = gmtime((const time_t*)&t);
		tm dTime = *gtime;
		tm tNow = g_pGame->GetServerLocalTime();
		if (tNow.tm_mday == dTime.tm_mday)
			strTime = GetGameUIMan()->GetStringFromTable(8606);
		else if (tNow.tm_mday == (dTime.tm_mday+1))
			strTime = GetGameUIMan()->GetStringFromTable(8607);
		else
			strTime.Format(GetGameUIMan()->GetStringFromTable(8608),dTime.tm_mon+1,dTime.tm_mday);
		data = data + sizeof(DYNAMIC::dynamic_header);
		switch (pDynamicHeader->action)
		{
		case EXCHG_MONEY:
			{
				DYNAMIC::exchg_money *pDat = (DYNAMIC::exchg_money*)data;
				ACString strMoney1, strMoney2;
				GetGameUIMan()->Int64ToStr(pDat->local_money, strMoney1);
				GetGameUIMan()->Int64ToStr(pDat->foreign_money, strMoney2);
				if (pDat->type == 0)
				{
					if (m_bOwn)
						strText.Format(GETSTRING(10000), strMoney1, strMoney2, strTime);
					else
						strText.Format(GETSTRING(10026), strMoney2, strMoney1, strTime);
				}
				else if (pDat->type == 1)
				{
					if (m_bOwn)
						strText.Format(GETSTRING(10001), strMoney1, strMoney2, strTime);
					else
						strText.Format(GETSTRING(10027), strMoney2, strMoney1, strTime);
				}
				m_vecDynamicString[DYNAMICTYPE_ACTION].push_back(strText);
				break;
			}
		case LEVEL_UP:
			{
				DYNAMIC::level_up *pDat = (DYNAMIC::level_up*)data;
				if (m_bOwn)
					strText.Format(GETSTRING(10002), pDat->level, strTime);
				else
					strText.Format(GETSTRING(10170), pDat->level, strTime);
				m_vecDynamicString[DYNAMICTYPE_ACTION].push_back(strText);
				break;
			}
		case BUY_ITEM:
			{
				DYNAMIC::buy_item *pDat = (DYNAMIC::buy_item*)data;
				CECIvtrItem *pItem = CECIvtrItem::CreateHomeTownItem(pDat->itemid, 0, HOMETOWN::ID_SPACE_ESSENCE);
				if (pItem && (pItem->GetClassID()==CECIvtrItem::ICID_HOMETOWN_TOOL))
				{
					if (m_bOwn)
					{
						strText.Format(GETSTRING(10013), pItem->GetName(), strTime);
						m_vecDynamicString[DYNAMICTYPE_ACTION].push_back(strText);
					}
					delete pItem;
				}
				break;
			}
		case USE_ITEM:
			{
				DYNAMIC::use_item *pDat = (DYNAMIC::use_item*)data;
				strSponsorName = ACString((ACHAR*)pDat->sponsor_name, pDat->sponsor_name_size/sizeof(ACHAR));
				strTargetName = ACString((ACHAR*)pDat->target_name, pDat->target_name_size/sizeof(ACHAR));
				CECIvtrItem *pItem = CECIvtrItem::CreateHomeTownItem(pDat->itemid, 0, HOMETOWN::ID_SPACE_ESSENCE);
				if (pItem && (pItem->GetClassID()==CECIvtrItem::ICID_HOMETOWN_TOOL))
				{
					if (m_bOwn)
					{
						if (pDat->target == GetHostPlayer()->GetCharacterID())
							strText.Format(GETSTRING(10003), pItem->GetName(), strTime);
						else
							strText.Format(GETSTRING(10004), strTargetName, pItem->GetName(), strTime);
						m_vecDynamicString[DYNAMICTYPE_ACTION].push_back(strText);
					}
					else
					{
						if (pDat->target == m_iOwnerId)
							strText.Format(GETSTRING(10028), pItem->GetName(), strTime);
						else
							strText.Format(GETSTRING(10029), strTargetName, pItem->GetName(), strTime);
						m_vecDynamicString[DYNAMICTYPE_ACTION].push_back(strText);
					}
					delete pItem;
				}
				break;
			}
		case SELL_ITEM:
			{
				DYNAMIC::sell_item *pDat = (DYNAMIC::sell_item*)data;
				CECIvtrItem *pItem = CECIvtrItem::CreateHomeTownItem(pDat->itemid, 0, HOMETOWN::ID_SPACE_ESSENCE);
				if (pItem && (pItem->GetClassID()==CECIvtrItem::ICID_HOMETOWN_TOOL))
				{
					if (m_bOwn)
					{
						strText.Format(GETSTRING(10014), pItem->GetName(), strTime);
						m_vecDynamicString[DYNAMICTYPE_ACTION].push_back(strText);
					}
					delete pItem;
				}
				break;
			}
		case FARM_PLOW_FIELD:
			{
				DYNAMIC::plow_field *pDat = (DYNAMIC::plow_field*)data;
				strSponsorName = ACString((ACHAR*)pDat->sponsor_name, pDat->sponsor_name_size/sizeof(ACHAR));
				strTargetName = ACString((ACHAR*)pDat->target_name, pDat->target_name_size/sizeof(ACHAR));
				if (m_bOwn)
				{
					strText.Format(GETSTRING(10005), EMOTIONSTRING(pDat->emotion),strTargetName, strTime);
					m_vecDynamicString[DYNAMICTYPE_ACTION].push_back(strText);
				}
				else
				{
					strText.Format(GETSTRING(10030), EMOTIONSTRING(pDat->emotion),strTargetName, strTime);
					m_vecDynamicString[DYNAMICTYPE_ACTION].push_back(strText);
				}
				break;
			}
		case FARM_SOW_FIELD:
			{
				DYNAMIC::sow_field *pDat = (DYNAMIC::sow_field*)data;
				strSponsorName = ACString((ACHAR*)pDat->sponsor_name, pDat->sponsor_name_size/sizeof(ACHAR));
				strTargetName = ACString((ACHAR*)pDat->target_name, pDat->target_name_size/sizeof(ACHAR));
				CECIvtrItem *pItem = CECIvtrItem::CreateHomeTownItem(pDat->seed_id, 0, HOMETOWN::ID_SPACE_ESSENCE);
				if (pItem)
				{
					if (m_bOwn)
					{
						strText.Format(GETSTRING(10006),strTargetName, pItem->GetName(), strTime);
						m_vecDynamicString[DYNAMICTYPE_ACTION].push_back(strText);
					}
					else
					{
						strText.Format(GETSTRING(10031), strTargetName, pItem->GetName(), strTime);
						m_vecDynamicString[DYNAMICTYPE_ACTION].push_back(strText);
					}
					break;
					delete pItem;
				}
			}
		case FARM_WATER_FIELD:
			{
				DYNAMIC::water_field *pDat = (DYNAMIC::water_field*)data;
				strSponsorName = ACString((ACHAR*)pDat->sponsor_name, pDat->sponsor_name_size/sizeof(ACHAR));
				strTargetName = ACString((ACHAR*)pDat->target_name, pDat->target_name_size/sizeof(ACHAR));
				if (m_bOwn)
				{
					strText.Format(GETSTRING(10007), EMOTIONSTRING(pDat->emotion),strTargetName, strTime);
					m_vecDynamicString[DYNAMICTYPE_ACTION].push_back(strText);
				}
				else
				{
					strText.Format(GETSTRING(10032), EMOTIONSTRING(pDat->emotion),strTargetName, strTime);
					m_vecDynamicString[DYNAMICTYPE_ACTION].push_back(strText);
				}
				break;
			}
		case FARM_PEST_FIELD:
			{
				DYNAMIC::pest_field *pDat = (DYNAMIC::pest_field*)data;
				strSponsorName = ACString((ACHAR*)pDat->sponsor_name, pDat->sponsor_name_size/sizeof(ACHAR));
				strTargetName = ACString((ACHAR*)pDat->target_name, pDat->target_name_size/sizeof(ACHAR));
				if (m_bOwn)
				{
					strText.Format(GETSTRING(10008), EMOTIONSTRING(pDat->emotion),strTargetName, strTime);
					m_vecDynamicString[DYNAMICTYPE_ACTION].push_back(strText);
				}
				else
				{
					strText.Format(GETSTRING(10033),  EMOTIONSTRING(pDat->emotion),strTargetName, strTime);
					m_vecDynamicString[DYNAMICTYPE_ACTION].push_back(strText);
				}
				break;
			}
		case FARM_WEED_FIELD:
			{
				DYNAMIC::weed_field *pDat = (DYNAMIC::weed_field*)data;
				strSponsorName = ACString((ACHAR*)pDat->sponsor_name, pDat->sponsor_name_size/sizeof(ACHAR));
				strTargetName = ACString((ACHAR*)pDat->target_name, pDat->target_name_size/sizeof(ACHAR));
				if (m_bOwn)
				{
					strText.Format(GETSTRING(10009), EMOTIONSTRING(pDat->emotion),strTargetName, strTime);
					m_vecDynamicString[DYNAMICTYPE_ACTION].push_back(strText);
				}
				else
				{
					strText.Format(GETSTRING(10034), EMOTIONSTRING(pDat->emotion), strTargetName, strTime);
					m_vecDynamicString[DYNAMICTYPE_ACTION].push_back(strText);
				}
				break;
			}
		case FARM_HARVEST_FIELD:
			{
				DYNAMIC::harvest_field *pDat = (DYNAMIC::harvest_field*)data;
				strSponsorName = ACString((ACHAR*)pDat->sponsor_name, pDat->sponsor_name_size/sizeof(ACHAR));
				strTargetName = ACString((ACHAR*)pDat->target_name, pDat->target_name_size/sizeof(ACHAR));
				CECIvtrItem *pItem = CECIvtrItem::CreateHomeTownItem(pDat->fruit_id, 0, HOMETOWN::ID_SPACE_ESSENCE);
				if (pItem && (pItem->GetClassID()==CECIvtrItem::ICID_HOMETOWN_FRUIT))
				{
					if (m_bOwn)
					{
						if (pDat->target == GetHostPlayer()->GetCharacterID())
							strText.Format(GETSTRING(10010), pDat->count, pItem->GetName(), strTime);
						else
							strText.Format(GETSTRING(10011),strTargetName, pDat->count, pItem->GetName(), strTime);
						m_vecDynamicString[DYNAMICTYPE_ACTION].push_back(strText);
					}
					else
					{
						if (pDat->target == m_iOwnerId)
							strText.Format(GETSTRING(10035), pDat->count, pItem->GetName(), strTime);
						else //这儿处理包裹满的情况
							strText.Format(GETSTRING(10036), strTargetName, pDat->count, pItem->GetName(), strTime);
						m_vecDynamicString[DYNAMICTYPE_ACTION].push_back(strText);
					}
					delete pItem;
				}
				break;
			}
		case FARM_STEAL_FIELD:
			{
				DYNAMIC::steal_field *pDat = (DYNAMIC::steal_field*)data;
				strSponsorName = ACString((ACHAR*)pDat->sponsor_name, pDat->sponsor_name_size/sizeof(ACHAR));
				strTargetName = ACString((ACHAR*)pDat->target_name, pDat->target_name_size/sizeof(ACHAR));
				CECIvtrItem *pItem = CECIvtrItem::CreateHomeTownItem(pDat->fruit_id, 0, HOMETOWN::ID_SPACE_ESSENCE);
				if (pItem && (pItem->GetClassID()==CECIvtrItem::ICID_HOMETOWN_FRUIT))
				{
					if (m_bOwn)
					{
						strText.Format(GETSTRING(10012),strTargetName, EMOTIONSTRING(pDat->emotion), pDat->count, pItem->GetName(), strTime);
						m_vecDynamicString[DYNAMICTYPE_ACTION].push_back(strText);
					}
					else
					{
						strText.Format(GETSTRING(10037), strTargetName, EMOTIONSTRING(pDat->emotion), pDat->count, pItem->GetName(), strTime);
						m_vecDynamicString[DYNAMICTYPE_ACTION].push_back(strText);
					}
					delete pItem;
				}
				break;
			}
		default:
			assert(0);
			break;
		}
	}
}

void CDlgGardenMan::OnPrtc_SowField(void *pData)
{
	CMD::farm_sow_field* pInfo = (CMD::farm_sow_field*)pData;
	abase::hash_map<int, int>::iterator it = m_mapId2Index.find(pInfo->field_id);
	if (it==m_mapId2Index.end())
	{
		assert(0);
		return;
	}
	int index = m_mapId2Index[pInfo->field_id];
	HometownTemplate* pDB = g_pGame->GetHomeTownDataMan();
	HOMETOWN::DATA_TYPE DataType;
	const HOMETOWN::CROP_ESSENCE* ess;
	ess = (HOMETOWN::CROP_ESSENCE*)pDB->get_data_ptr(pInfo->seed_id, HOMETOWN::ID_SPACE_ESSENCE, DataType, ess);
	if (ess==NULL)
	{
		return;
	}
	m_pImgPlants[index][IMAGESTATE_GEMMULE]->Show(true);
	if (index<HOMETOWNFARMPLANTOWNNUM)
	{
		m_vecFarmInfo[FARMTYPE_OWN][index].state = 1<<HTF_STATE_PLANT;
		m_vecFarmInfo[FARMTYPE_OWN][index].crop_state = 1<<HTF_STATE_SEED;
		m_vecFarmInfo[FARMTYPE_OWN][index].id = pInfo->seed_id;
		m_vecFarmInfo[FARMTYPE_OWN][index].ripe_time = ess->grow_period; 
		m_vecFarmInfo[FARMTYPE_OWN][index].protect_period = ess->protect_period; 
	}
	else
	{
		m_vecFarmInfo[FARMTYPE_LOVE][index-HOMETOWNFARMPLANTOWNNUM].state = 1<<HTF_STATE_PLANT;
		m_vecFarmInfo[FARMTYPE_LOVE][index-HOMETOWNFARMPLANTOWNNUM].crop_state = 1<<HTF_STATE_SEED;
		m_vecFarmInfo[FARMTYPE_LOVE][index-HOMETOWNFARMPLANTOWNNUM].id = pInfo->seed_id;
		m_vecFarmInfo[FARMTYPE_LOVE][index-HOMETOWNFARMPLANTOWNNUM].ripe_time = ess-> grow_period;
		m_vecFarmInfo[FARMTYPE_LOVE][index-HOMETOWNFARMPLANTOWNNUM].protect_period = ess->protect_period;
	}
	m_iOwnerMoney -= pInfo->cost;
}

void CDlgGardenMan::OnPrtc_PestField(void *pData)
{
	CMD::farm_pest_field* pInfo = (CMD::farm_pest_field*)pData;
	abase::hash_map<int, int>::iterator it = m_mapId2Index.find(pInfo->field_id);
	if (it==m_mapId2Index.end())
	{
		assert(0);
		return;
	}
	int index = m_mapId2Index[pInfo->field_id];
	m_pImgPlants[index][IMAGESTATE_WORM]->Show(false);
	if (m_bOwn)
		m_iOwnerCharm += pInfo->charm_change;
	if (index<HOMETOWNFARMPLANTOWNNUM)
		m_vecFarmInfo[FARMTYPE_OWN][index].crop_state &= ~(1<<HTF_STATE_PEST);
	else
		m_vecFarmInfo[FARMTYPE_LOVE][index-HOMETOWNFARMPLANTOWNNUM].crop_state &= ~(1<<HTF_STATE_PEST);
}

void CDlgGardenMan::OnPrtc_WaterField(void *pData)
{
 	CMD::farm_water_field* pInfo = (CMD::farm_water_field*)pData;
	abase::hash_map<int, int>::iterator it = m_mapId2Index.find(pInfo->field_id);
	if (it==m_mapId2Index.end())
	{
		assert(0);
		return;
	}
	int index = m_mapId2Index[pInfo->field_id];
	m_pImgPlants[index][IMAGESTATE_WATER]->Show(true);
	m_pImgPlants[index][IMAGESTATE_DROUGHT]->Show(false);
	if (m_bOwn)
		m_iOwnerCharm += pInfo->charm_change;
	if (index<HOMETOWNFARMPLANTOWNNUM)
		m_vecFarmInfo[FARMTYPE_OWN][index].crop_state &= ~(1<<HTF_STATE_DRY);
	else
		m_vecFarmInfo[FARMTYPE_LOVE][index-HOMETOWNFARMPLANTOWNNUM].crop_state &= ~(1<<HTF_STATE_DRY);
}

void CDlgGardenMan::OnPrtc_PlowField(void *pData)
{
	CMD::farm_plow_field* pInfo = (CMD::farm_plow_field*)pData;
	abase::hash_map<int, int>::iterator it = m_mapId2Index.find(pInfo->field_id);
	if (it==m_mapId2Index.end())
	{
		assert(0);
		return;
	}
	int index = m_mapId2Index[pInfo->field_id];
	ResetOneField(index);
	m_pImgPlants[index][IMAGESTATE_PLOT]->Show(true);
	if (index<HOMETOWNFARMPLANTOWNNUM)
		m_vecFarmInfo[FARMTYPE_OWN][index].state = 1<<HTF_STATE_PLOW;
	else
		m_vecFarmInfo[FARMTYPE_LOVE][index-HOMETOWNFARMPLANTOWNNUM].state = 1<<HTF_STATE_PLOW;
}

void CDlgGardenMan::OnPrtc_WeedField(void *pData)
{
	CMD::farm_weed_field* pInfo = (CMD::farm_weed_field*)pData;
	abase::hash_map<int, int>::iterator it = m_mapId2Index.find(pInfo->field_id);
	if (it==m_mapId2Index.end())
	{
		assert(0);
		return;
	}
	int index = m_mapId2Index[pInfo->field_id];
	m_pImgPlants[index][IMAGESTATE_GRASS]->Show(false);
	if (m_bOwn)
		m_iOwnerCharm += pInfo->charm_change;
	if (index<HOMETOWNFARMPLANTOWNNUM)
		m_vecFarmInfo[FARMTYPE_OWN][index].crop_state &= ~(1<<HTF_STATE_WEED);
	else
		m_vecFarmInfo[FARMTYPE_LOVE][index-HOMETOWNFARMPLANTOWNNUM].crop_state &= ~(1<<HTF_STATE_WEED);
}

void CDlgGardenMan::OnPrtc_HarvestField(void *pData)
{
	CMD::farm_harvest_field* pInfo = (CMD::farm_harvest_field*)pData;
	abase::hash_map<int, int>::iterator it = m_mapId2Index.find(pInfo->field_id);
	if (it==m_mapId2Index.end())
	{
		assert(0);
		return;
	}
	int index = m_mapId2Index[pInfo->field_id];
	ResetOneField(index);
	m_pImgPlants[index][IMAGESTATE_CROP]->Show(true);
	m_pImgPlants[index][IMAGESTATE_HARVEST]->Show(true);
	if (index<HOMETOWNFARMPLANTOWNNUM)
	{
		m_vecFarmInfo[FARMTYPE_OWN][index].crop_state = 1<<HTF_STATE_HARVEST;
	}
	else
	{
		m_vecFarmInfo[FARMTYPE_LOVE][index-HOMETOWNFARMPLANTOWNNUM].crop_state = 1<<HTF_STATE_HARVEST;
	}
	m_iOwnerCharm += pInfo->charm_add;
	CECIvtrItem *pItem = CECIvtrItem::CreateHomeTownItem(pInfo->fruit_id, 1, HOMETOWN::ID_SPACE_ESSENCE);
	if (pItem)
	{
		if (pInfo->fruit_count>0)
		{
			ACString strText;
			strText.Format(GETSTRING(8664), pInfo->fruit_count,pItem->GetName() );
			MSGBOX(strText);
			htc2s_SendCmdGetPackage();
		}
		delete pItem;
	}
}

void CDlgGardenMan::OnPrtc_StealField(void *pData)
{
	CMD::farm_steal_field* pCmd = (CMD::farm_steal_field*)pData;
	int iCharmChange = pCmd->charm_change;
	ACString strText;
	if (iCharmChange>0)
	{
		strText.Format(GetGameUIMan()->GetStringFromTable(10049+rand()%55),iCharmChange);
		GetGameUIMan()->MessageBox("",strText, MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	}
	else if (iCharmChange<0)
	{
		strText.Format(GetGameUIMan()->GetStringFromTable(10104+rand()%55),-iCharmChange);
		GetGameUIMan()->MessageBox("",strText, MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	}
	CECIvtrItem *pItem = CECIvtrItem::CreateHomeTownItem(pCmd->fruit_id, 0, HOMETOWN::ID_SPACE_ESSENCE);
	if (pItem)
	{
		CECIvtrHTFruit *pFruit = (CECIvtrHTFruit*)pItem;
		m_iOwnerCharm += pFruit->GetDBEssence()->price;
		delete pItem;
	}
	
	if (pCmd->fruit_count>0)
	{
		CECIvtrItem *pItem = CECIvtrItem::CreateHomeTownItem(pCmd->fruit_id, 1, HOMETOWN::ID_SPACE_ESSENCE);
		if (pItem)
		{
			strText.Format(GETSTRING(8664), pCmd->fruit_count,pItem->GetName() );
			ADDCHAT(strText);
			delete pItem;
		}

		int index = m_mapId2Index[pCmd->field_id];
		if (index<HOMETOWNFARMPLANTOWNNUM)
		{
			m_vecFarmInfo[FARMTYPE_OWN][index].fruit_left -= pCmd->fruit_count;
		}
		else
		{
			m_vecFarmInfo[FARMTYPE_LOVE][index-HOMETOWNFARMPLANTOWNNUM].fruit_left -= pCmd->fruit_count;
		}
	}
	htc2s_SendCmdGetPackage();
}

void CDlgGardenMan::OnPrtc_ListLoveFields(void *pData)
{
	CMD::farm_list_love_field* pCmd = (CMD::farm_list_love_field*)pData;
	m_vecDynamicString[DYNAMICTYPE_LOVEGARDEN].clear();
	m_vecLoveGardenIds.clear();
	for (int i=0; i<pCmd->count; i++)
	{
		ACString strName = FRIENDNAME(pCmd->friends[i]);
		if (strName.IsEmpty())
			strName.Format(_AL("friend-%d"),pCmd->friends[i]);
		m_vecDynamicString[DYNAMICTYPE_LOVEGARDEN].push_back(strName);
		m_vecLoveGardenIds.push_back(pCmd->friends[i]);
	}
}

void CDlgGardenMan::OnPrtc_ListMatureFields(void *pData)
{
	CMD::farm_list_mature_field* pCmd = (CMD::farm_list_mature_field*)pData;
	m_vecDynamicString[DYNAMICTYPE_FRUITS].clear();
	m_vecMatureGardenIds.clear();
	for (int i=0; i<pCmd->count; i++)
	{
		ACString strName = FRIENDNAME(pCmd->friends[i]);
		if (strName.IsEmpty())
			strName.Format(_AL("friend-%d"),pCmd->friends[i]);
		m_vecDynamicString[DYNAMICTYPE_FRUITS].push_back(strName);
		m_vecMatureGardenIds.push_back(pCmd->friends[i]);
	}
}

void CDlgGardenMan::ResetState()
{
	int i;
	for (i=0;i<=DYNAMICTYPE_ACTION;i++)
	{
		m_vecDynamicString[i].clear();
	}
	for (i=0; i<FARMTYPE_MAX; i++)
	{
		m_vecFarmInfo[i].clear();
	}
	m_mapId2Index.clear();
	m_iOwnerId = 0;
	m_bOwn = true;
	m_iActionType = 1;
	m_iSeedType = -1;
	GetGame()->ChangeCursor(RES_CUR_NORMAL);
	m_pLabTitle->SetText(_AL(""));
	m_pLabCharm->SetText(_AL("0"));
	m_pLabLevel->SetText(_AL("1"));

	for ( i=0; i<HOMETOWNFARMPLANTNUM; i++)
	{
		ResetOneField(i);
	}
}

void CDlgGardenMan::UpdatePlantState(int index)
{
	if (index<HOMETOWNFARMPLANTOWNNUM)
		UpdatePlantState(index, m_vecFarmInfo[FARMTYPE_OWN][index].state, m_vecFarmInfo[FARMTYPE_OWN][index].crop_state);
	else
		UpdatePlantState(index, m_vecFarmInfo[FARMTYPE_LOVE][index-HOMETOWNFARMPLANTOWNNUM].state, m_vecFarmInfo[FARMTYPE_LOVE][index-HOMETOWNFARMPLANTOWNNUM].crop_state);
}
void CDlgGardenMan::UpdatePlantState(int index, int fState, int cState)
{
	ResetOneField(index);
	if (fState&(1<<HTF_STATE_INCULT))
	{
		return;
	}
	else if (fState&(1<<HTF_STATE_CULT) )
	{
		m_pImgPlants[index][IMAGESTATE_CROP]->Show(true);
		return;
	}
	else if (fState&(1<<HTF_STATE_PLOW))
	{
		m_pImgPlants[index][IMAGESTATE_PLOT]->Show(true);
		return;
	}
	else if (fState&(1<<HTF_STATE_PLANT))
	{
		m_pImgPlants[index][IMAGESTATE_PLOT]->Show(true);
		if (cState&(1<<HTF_STATE_DRY))
		{
			m_pImgPlants[index][IMAGESTATE_DROUGHT]->Show(true);
		}
		else
			m_pImgPlants[index][IMAGESTATE_DROUGHT]->Show(false);

		if (cState&(1<<HTF_STATE_PEST))
		{
			m_pImgPlants[index][IMAGESTATE_WORM]->Show(true);
		}
		else
		{
			m_pImgPlants[index][IMAGESTATE_WORM]->Show(false);
		}

		if (cState&(1<<HTF_STATE_WEED))
		{
			m_pImgPlants[index][IMAGESTATE_GRASS]->Show(true);
		}
		else
		{
			m_pImgPlants[index][IMAGESTATE_GRASS]->Show(false);
		}
		
		if (cState&(1<<HTF_STATE_CROPPROTECTBEGIN))
		{
			m_pImgPlants[index][IMAGESTATE_TRAP]->Show(true);
		}
		else
		{
			m_pImgPlants[index][IMAGESTATE_TRAP]->Show(false);
		}

		if (cState&(1<<HTF_STATE_WILT))
			m_pImgPlants[index][IMAGESTATE_DIE]->Show(true);
		else if (cState&(1<<HTF_STATE_HARVEST))
			m_pImgPlants[index][IMAGESTATE_HARVEST]->Show(true);
		else if (cState&(1<<HTF_STATE_SEED))
			m_pImgPlants[index][IMAGESTATE_GEMMULE]->Show(true);
		else if (cState&(1<<HTF_STATE_SPROUT))
			m_pImgPlants[index][IMAGESTATE_GEMMULE]->Show(true);
		else if (cState&(1<<HTF_STATE_IMMATURE))
			m_pImgPlants[index][IMAGESTATE_BIGGER]->Show(true);
		else if (cState&(1<<HTF_STATE_PREMATURE))
			m_pImgPlants[index][IMAGESTATE_LESSMATURE]->Show(true);
		else if (cState&(1<<HTF_STATE_MATURE))
			m_pImgPlants[index][IMAGESTATE_MATURE]->Show(true);
		else if (cState&(1<<HTF_STATE_ALLSTOLEN))
			m_pImgPlants[index][IMAGESTATE_HARVEST]->Show(true);
		else if (cState&(1<<HTF_STATE_CROPPROTECTBEGIN))
			m_pImgPlants[index][IMAGESTATE_HARVEST]->Show(true);
	}
}

int CDlgGardenMan::GetFieldId(UINT index)
{
	if (index<HOMETOWNFARMPLANTOWNNUM)
	{
		if (index<m_vecFarmInfo[FARMTYPE_OWN].size())
			return m_vecFarmInfo[FARMTYPE_OWN][index].field_id;
	}
	else
	{
		if ((index-HOMETOWNFARMPLANTOWNNUM)<m_vecFarmInfo[FARMTYPE_LOVE].size())
			return m_vecFarmInfo[FARMTYPE_LOVE][index-HOMETOWNFARMPLANTOWNNUM].field_id;
	}
	return -1;
}

void CDlgGardenMan::TickState()
{
	//beast
	DWORD tNow = GetTickCount();
	static int	 BeastTime	= 0;	// 休闲状态倒计时
	static DWORD tLastBeast = GetTickCount();
	static DWORD tBeastDir  = GetTickCount();
	//bee
	static POINT_FLOAT BeeDir[BEE_NUM]	= {{ 0.05f, -0.05f},{ 0.05f, -0.05f},{ 0.05f, -0.05f}};//,{ 0.05f, -0.05f},{ 0.05f, -0.05f},{ 0.05f, -0.05f}};
	static DWORD BeeTime[BEE_NUM]	= {10000, 10000, 10000};//, 10000, 10000, 10000};
	static DWORD tLastBee[BEE_NUM];
	static POINT_FLOAT BfDir[BEE_NUM]	= {{ 0.05f, -0.05f},{ 0.05f, -0.05f},{ 0.05f, -0.05f}};//,{ 0.05f, -0.05f},{ 0.05f, -0.05f},{ 0.05f, -0.05f}};
	static DWORD BfTime[BEE_NUM]	= {10000, 10000, 10000};//, 10000, 10000, 10000};
	static DWORD tLastBf[BEE_NUM];
	//time record
	static DWORD tLast = tNow;
	DWORD t = tNow - tLast;
	tLast = tNow;
	A3DRECT  rc = GetRect();

	//move
	if (m_pImgGuardBeast->IsShow())
	{
		//best tick
		if (tNow-tLastBeast>200)
		{
			tLastBeast = tNow;
			if (m_iGuardFrameIndex >= 0 && m_iGuardFrameIndex <= 3)
				m_iGuardFrameIndex = (m_iGuardFrameIndex+1)%4;
			else if (m_iGuardFrameIndex >= 4 && m_iGuardFrameIndex <= 9)
				m_iGuardFrameIndex = 4 + (m_iGuardFrameIndex+1)%6;
			else if (m_iGuardFrameIndex >= 10 && m_iGuardFrameIndex <= 13)
				m_iGuardFrameIndex = 10 + (m_iGuardFrameIndex+1)%4;
			else
				m_iGuardFrameIndex = 0;
			m_pImgGuardBeast->FixFrame(m_iGuardFrameIndex);
		}
		int W = m_pImgGuardBeast->GetRect().Width();
		int H = m_pImgGuardBeast->GetRect().Height();
		int left  = 30;
		int up	  = GetDlgItem("Img_Plant_Banner_Love1")->GetPos(true).y + GetDlgItem("Img_Plant_Banner_Love1")->GetSize().cy/2;
		int right = GetDlgItem("Img_Plant_Banner_Love6")->GetPos(true).x - W;
		int down  = GetDlgItem("Img_Plant_Banner_Love6")->GetPos(true).y;
		POINT ptNow = m_pImgGuardBeast->GetPos(true);
		if (BeastState() == 0)
		{
			if (BeastTime>0)
			{
				BeastTime -= t;
			}
			else
			{
				if (ptNow.x<=(left+right)/2)
				{
					m_pImgGuardBeast->FixFrame(0);
					BeastTime = 0;
					m_iGuardFrameIndex = 0;
					tBeastDir = tNow;
				}
				else if (ptNow.x>=(left+right)/2)
				{
					m_pImgGuardBeast->FixFrame(10);
					BeastTime = 0;
					m_iGuardFrameIndex = 10;
					tBeastDir = tNow;
				}
				else
					assert(0);
			}
		}
		
		float BeastSpeedX = 0.06f;
		float BeastSpeedY = BeastSpeedX * (down - up) / (right - left);
		if (BeastState() == 1)
		{
			ptNow.x = long(left + BeastState() * BeastSpeedX * (tNow-tBeastDir));
			ptNow.y = long(up + BeastState() * BeastSpeedY * (tNow-tBeastDir));
		}
		else if (BeastState() == -1)
		{
			ptNow.x = long(right + BeastState() * BeastSpeedX * (tNow-tBeastDir));
			ptNow.y = long(down + BeastState() * BeastSpeedY * (tNow-tBeastDir));
		}
		if (BeastState()!=0)
		{
			if (ptNow.x<left)
			{
				ptNow.x = left;
				ptNow.y = up;
				m_pImgGuardBeast->FixFrame(4);
				m_iGuardFrameIndex = 4;
				tBeastDir = tNow;
				BeastTime = 5000;
			}
			if (ptNow.x>right)
			{
				ptNow.x = right;
				ptNow.y = down;
				m_pImgGuardBeast->FixFrame(4);
				m_iGuardFrameIndex = 4;
				tBeastDir = tNow;
				BeastTime = 5000;
			}
		}
		m_pImgGuardBeast->SetPos(ptNow.x, ptNow.y);
	}

	int left  = GetDlgItem("Img_Plant_Banner_Love1")->GetPos(true).x;
	int up	  = GetDlgItem("Img_Plant_Harvest1")->GetPos(true).y;
	int right = GetDlgItem("Img_Plant_Harvest6")->GetPos(true).x + GetDlgItem("Img_Plant_Harvest6")->GetSize().cx - m_pImgBee[0]->GetSize().cx;
	int down  = GetDlgItem("Img_Plant_Banner_Love6")->GetPos(true).y + GetDlgItem("Img_Plant_Banner_Love6")->GetSize().cy - m_pImgBee[0]->GetSize().cy;
	for (int i=0; i<BEE_NUM; i++)
	{
		//bee tick
		if (tNow-tLastBee[i]>30)
		{
			tLastBee[i] = tNow;
			if (m_iBeeFrameIndex[i] == 0)
				m_iBeeFrameIndex[i] = 1;
			else if (m_iBeeFrameIndex[i] == 1)
				m_iBeeFrameIndex[i] = 0;
			else if (m_iBeeFrameIndex[i] == 2)
				m_iBeeFrameIndex[i] = 3;
			else if (m_iBeeFrameIndex[i] == 3)
				m_iBeeFrameIndex[i] = 2;
			else
				m_iBeeFrameIndex[i] = 0;
			m_pImgBee[i]->FixFrame(m_iBeeFrameIndex[i]);
		}
		if (m_pImgBee[i]->IsShow())
		{
			int W = m_pImgBee[i]->GetRect().Width();
			int H = m_pImgBee[i]->GetRect().Height();
			POINT ptLast = m_pImgBee[i]->GetPos(true);
			POINT ptNow;
			ptNow.x = long(ptLast.x + BeeDir[i].x * t);
			ptNow.y = long(ptLast.y + BeeDir[i].y * t);
			if (ptNow.x<left)
			{
				ptNow.x = left;
				BeeTime[i] = 0;
			}
			if (ptNow.x>(right))
			{
				ptNow.x = right;
				BeeTime[i] = 0;
			}
			if (ptNow.y<up)
			{
				ptNow.y = up;
				BeeTime[i] = 0;
			}
			if (ptNow.y>down)
			{
				ptNow.y = down;
				BeeTime[i] = 0;
			}
			m_pImgBee[i]->SetPos(ptNow.x, ptNow.y);
		}
		if ((int)BeeTime[i]<=50)
		{
			BeeTime[i] = a_Random(2,4) * 1000;
			BeeDir[i].x = a_Random(25,30)/300.0f * (a_Random(0,1)>0?1:-1);
			BeeDir[i].y = a_Random(13,17)/300.0f * (a_Random(0,1)>0?1:-1);
			tLastBee[i] -= 200;
			if (BeeDir[i].x>0)
				m_iBeeFrameIndex[i] = 0;
			else
				m_iBeeFrameIndex[i] = 2;
		}
		BeeTime[i] -= t;
		
		//butterfly tick
		if (tNow-tLastBf[i]>200)
		{
			tLastBf[i] = tNow;
			if (m_iBfFrameIndex[i] >= 0 && m_iBfFrameIndex[i] <= 4)
				m_iBfFrameIndex[i] = (m_iBfFrameIndex[i]+1)%5;
			else if (m_iBfFrameIndex[i] >= 5 && m_iBfFrameIndex[i] <= 9)
				m_iBfFrameIndex[i] = 5+(m_iBfFrameIndex[i]+1)%5;
			else
				m_iBfFrameIndex[i] = 0;
			m_pImgButterfly[i]->FixFrame(m_iBfFrameIndex[i]);
		}
		if (m_pImgButterfly[i]->IsShow())
		{
			int W = m_pImgButterfly[i]->GetRect().Width();
			int H = m_pImgButterfly[i]->GetRect().Height();
			POINT ptLast = m_pImgButterfly[i]->GetPos(true);
			POINT ptNow;
			ptNow.x = long(ptLast.x + BfDir[i].x * t);
			ptNow.y = long(ptLast.y + BfDir[i].y * t);
			if (ptNow.x<left)
			{
				ptNow.x = left;
				BfTime[i] = 0;
			}
			if (ptNow.x>(right))
			{
				ptNow.x = right;
				BfTime[i] = 0;
			}
			if (ptNow.y<up)
			{
				ptNow.y = up;
				BfTime[i] = 0;
			}
			if (ptNow.y>down)
			{
				ptNow.y = down;
				BfTime[i] = 0;
			}
			m_pImgButterfly[i]->SetPos(ptNow.x, ptNow.y);
		}
		if ((int)BfTime[i]<=50)
		{
			BfTime[i] = a_Random(5,7) * 1000;
			BfDir[i].x = a_Random(17,20)/300.0f * (a_Random(0,1)>0?1:-1);
			BfDir[i].y = a_Random(8,10)/300.0f * (a_Random(0,1)>0?1:-1);
			tLastBf[i] -= 200;
			if (BfDir[i].x>0)
				m_iBfFrameIndex[i] = 0;
			else
				m_iBfFrameIndex[i] = 5;
		}
		BfTime[i] -= t;
	}
}

void CDlgGardenMan::UpdateHint()
{
	PAUIDIALOG pDlgMouseOn;
	PAUIOBJECT pObjMouseOn;
	GetGameUIMan()->GetMouseOn(&pDlgMouseOn, &pObjMouseOn);

	if( pDlgMouseOn && pDlgMouseOn == this &&
		pObjMouseOn && strncmp(pObjMouseOn->GetName(), "Img_Plant_", strlen("Img_Plant_") ) == 0 )
	{
		int index = pObjMouseOn->GetData();
		ACString strText;
		strText.Empty();
		dlg_field_info info;
		if (index<HOMETOWNFARMPLANTOWNNUM && (UINT)index<m_vecFarmInfo[FARMTYPE_OWN].size() )
			info = m_vecFarmInfo[FARMTYPE_OWN][index];
		else if (index>=HOMETOWNFARMPLANTOWNNUM && (UINT)(index-HOMETOWNFARMPLANTOWNNUM)<m_vecFarmInfo[FARMTYPE_LOVE].size())
			info = m_vecFarmInfo[FARMTYPE_LOVE][index-HOMETOWNFARMPLANTOWNNUM];
		else
		{
			info.crop_state = 0;
			info.state = 0;
			info.strFriendName.Empty();
		}
		
		if ((info.state&(1<<HTF_STATE_PLANT)) && 
			((info.crop_state&(1<<HTF_STATE_HARVEST)) == 0))
		{
			HometownTemplate* pDB = g_pGame->GetHomeTownDataMan();
			HOMETOWN::DATA_TYPE DataType;
			const HOMETOWN::CROP_ESSENCE* ess;
			ess = (HOMETOWN::CROP_ESSENCE*)pDB->get_data_ptr(info.id, HOMETOWN::ID_SPACE_ESSENCE, DataType, ess);
			if (ess==NULL)
			{
				return;
			}
			ACString strGrowTime = GetFormatTime(ess->grow_period);
			ACString strRipeTime = GetFormatTime(info.ripe_time);
			ACString strStolenProtectTime;
			ACString strFruit;

			strStolenProtectTime = GetFormatTime(info.protect_period);
			if (info.crop_state&(1<<HTF_STATE_WILT))
				strText.Format(GETSTRING(8668), ess->name);
			else if (!(info.crop_state&(1<<HTF_STATE_MATURE)))
			{
				strFruit.Format(_AL("%d-%d"),ess->fruit_min,ess->fruit_max);
				if ((info.crop_state&(1<<HTF_STATE_SEED)) || (info.crop_state&(1<<HTF_STATE_SPROUT)))
					strText.Format(GETSTRING(8665), ess->name,strGrowTime, strRipeTime, strStolenProtectTime,strFruit);
				else if (info.crop_state&(1<<HTF_STATE_IMMATURE))
					strText.Format(GETSTRING(8674), ess->name,strGrowTime, strRipeTime, strStolenProtectTime,strFruit);
				else if (info.crop_state&(1<<HTF_STATE_PREMATURE))
					strText.Format(GETSTRING(8675), ess->name,strGrowTime, strRipeTime, strStolenProtectTime,strFruit);
			}
			else
				strText.Format(GETSTRING(8666), ess->name,strGrowTime, strRipeTime, strStolenProtectTime,info.fruit_total,info.fruit_left);
			if (info.id >=1 && info.id <=16 || info.id >=42 && info.id <=57)
				strText = _AL("^6cfb4b") + strText;
			else if (info.id >=17 && info.id <=41 || info.id >=58 && info.id <=82)
				strText = _AL("^aa32ff") + strText;
			
			if (info.crop_state&(1<<HTF_STATE_DRY) || info.crop_state&(1<<HTF_STATE_PEST) || info.crop_state&(1<<HTF_STATE_WEED))
			{
				strText += _AL("\r");
				strText += GETSTRING(9150);
				if (info.crop_state&(1<<HTF_STATE_DRY))
				{
					strText += _AL(" ");
					strText += GETSTRING(9151);
				}
				if (info.crop_state&(1<<HTF_STATE_PEST))
				{
					strText += _AL(" ");
					strText += GETSTRING(9152);
				}
				if (info.crop_state&(1<<HTF_STATE_WEED))
				{
					strText += _AL(" ");
					strText += GETSTRING(9153);
				}
			}

			if (!info.strFriendName.IsEmpty())
			{
				strText += _AL("\r");
				strText += GETSTRING(9106);
				strText += info.strFriendName;
			}
			else if (m_bOwn&&index>=HOMETOWNFARMPLANTOWNNUM)
			{
				strText += _AL("\r");
				strText += GETSTRING(9106);
				strText += FRIENDNAME(info.friend_id);
			}
		}
		else if ((info.crop_state&(1<<HTF_STATE_HARVEST)))
			strText = GETSTRING(8680);
		pObjMouseOn->SetHint(strText);
	}
}

ACString CDlgGardenMan::GetFormatTime(int nTime)
{
	ACString szTime;
	if( nTime <= 0 )
		szTime.Format(_AL("0"));
	else if( nTime > 24*3600)
		szTime.Format(GetStringFromTable(1604), (nTime + 24*3600-1) / (24*3600));
	else if( nTime > 3600)
		szTime.Format(GetStringFromTable(1603), (nTime+3599) / 3600 );
	else if( nTime > 60 )
		szTime.Format(GetStringFromTable(1602), (nTime+59) / 60);
	else if( nTime > 0 )
		szTime.Format(GetStringFromTable(1601), nTime);
	else
		szTime.Format(GetStringFromTable(1601), 0);
	
	return szTime;
}

void CDlgGardenMan::UpdateSeed()
{
	if (stricmp(m_szName,"Win_Garden_Seed") != 0)
		return;

	PAUISCROLL pScl = (PAUISCROLL)GetDlgItem("Scl_Item");
	if (pScl->IsShow())
		m_iFirstLine = pScl->GetBarLevel();
	PAUIIMAGEPICTURE pImg;
	AString strName;
	UINT index;
	for (int i=0; i<GARDENSEEDITEM_NUM; i++)
	{
		strName.Format("Img_%d", i+1);
		pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		index = i+m_iFirstLine*3;
		if (index>=m_vecCropEss.size())
		{
			pImg->Show(false);
			continue;
		}
		else
			pImg->Show(true);
		const HOMETOWN::CROP_ESSENCE* ess = m_vecCropEss[index];
		const char *path = g_pGame->GetHomeTownIconPath()->GetANSIString(ess->icon_seed);
		AUIOBJECT_SETPROPERTY p;
		strcpy(p.fn, path);
		pImg->SetProperty("Image File", &p);

		if ((UINT)m_iSeedType == m_vecCropEss[index]->id)
			pImg->SetCover(CDlgGardenQShop::m_pA2DSpriteItemSelect,0,1);
		else
			pImg->SetCover(NULL,0,1);
		
		CECIvtrItem *pItem = CECIvtrItem::CreateHomeTownItem(m_vecCropEss[index]->id, 1, HOMETOWN::ID_SPACE_ESSENCE);
		if( pItem )
		{
			AUICTranslate trans;
			AWString strHint = trans.Translate(pItem->GetDesc());
			pImg->SetHint(strHint);
			delete pItem;
		}
	}
};

int CDlgGardenMan::BeastState()
{
	if(m_iGuardFrameIndex>=0 && m_iGuardFrameIndex<=3)
		return 1;
	else if (m_iGuardFrameIndex>=4 && m_iGuardFrameIndex<=9)
		return 0;
	else if (m_iGuardFrameIndex>=10 && m_iGuardFrameIndex<=13)
		return -1;
	assert(0);
	return -2;
}