// DataManager.cpp: implementation of the CDataManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CSpreadSheet.h"
#include "ElementLocalize.h"
#include "DataManager.h"
#include "TemplateManager.h"
#include "TemplateBase.h"
#include "TaskTemplate.h"
#include "SelectTransDlg.h"
#include "../ZElementData/elementdataman.h"
#include "../ZElementData/DataPathMan.h"
#include "../ZElementData/EC_MD5Hash.h"
#include "../ZElementClient/task/TaskTemplMan.h"
#include "EC_StringTab.h"
#include "ElementInterfaceMgr.h"
#include "../ZelementData/Policy.h"
#include "PolicyTemplate.h"
#include "PrecinctTemplate.h"
#include "treasure.h"
#include "Jb.h"
#include "globaldataman.h"
#include "GlobalDataTemplate.h"
#include "Excel.h"
#include "TextFileMgr.h"
#include "DlgSelectItems.h"
#include "hometowntemplate.h"
#include "XmlLocalize.h"
#include "ElementDataView.h"
#include "MainFrm.h"

extern const TCHAR *g_szSheetTail;
extern const TCHAR *g_szSheetHeader;
extern const TCHAR *g_szHeader;
extern const TCHAR *g_szTail;
extern bool g_bModified;

int Aux_GetLineNum(CString str);
#define MAX_BUFFER_LEN 10240

#include <af.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern ALog g_Log;
extern char g_szWorkDir[];

struct TEMP_TEXT
{
	AWString strOrg;
	AWString strTrans;
};



CDataManager g_DataManager;
CTemplateBase * g_pElementTypes[DT_MAX];
CTemplateBase * g_pHomeTownTypes[HOMETOWN::DT_MAX];
#define	LOCALIZE_IDENTIFY		(('Q'<<24) | ('L'<<16) | ('P'<<8) | 'J')
#define LOCALIZE_VERSION 0x10000013 //0x10000000 Q版本

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
char szBufferC[1024];
WORD szBufferW[1024];
const char* w_to_c( const WORD * szSrc)
{
	int nLen = wcslen(szSrc);
	memset(szBufferC,0,1024);
	WideCharToMultiByte(CP_ACP,0,szSrc,nLen,szBufferC,1024,NULL,NULL);
	return szBufferC;
}

const WORD* c_to_w( const char * szSrc)
{
	int nLen = strlen(szSrc);
	memset(szBufferW,0,sizeof(WORD)*1024);
	MultiByteToWideChar(CP_ACP,0,szSrc,nLen,szBufferW,1024*sizeof(WORD));
	return szBufferW;
}

void GenCantImport(const abase::vector<TEMP_TEXT> vecNoReplace, const char* szPath);
void GenCantImportOrder(const abase::vector<TEMP_TEXT>* vecNoReplace, const char* szPath);
inline void GetCheckedItems(CElementDataView* pView, CDlgSelectItems* pDlg, vector<pair<HTREEITEM, BOOL> >& vItems)
{
	vItems.clear();
	if (!pView || !pDlg)
		return;

	const HTREEITEM* pItems = pView->GetItems();
	for (int i=CHUNK_ELEMENT_DATA; i<CHUNK_NUM; ++i)
	{
		vItems.push_back(pair<HTREEITEM, BOOL>(pItems[i], pDlg->GetDataSwitch(i)));
	}
}

int g_nTotalPer = 0;
CDataManager::~CDataManager()
{
	Release();
}

CDataManager::CDataManager()
{
	pTM_ElementData			= NULL;
	pTM_ElementTask			= NULL;
	pTM_ElementPolicy		= NULL;
	pTM_ElementInterfaceHint= NULL;
	pTM_ElementTextFile     = NULL;
	pTM_ElementInterfaceString= NULL;
	pTM_ElementItemDesc     = NULL;
	pTM_ElementInstaceText  = NULL;
	pTM_ElementItemExDesc   = NULL;
	pTM_ElementSkillDesc	= NULL;
	pTM_ElementFixMsg		= NULL;
	pTM_ElementServerErr	= NULL;
	pTM_ElementBuffDesc     = NULL;
	pTM_ElementTaskErr      = NULL;
	pTM_ElementLoadingHint = NULL;
	pTM_ElementPetTalk		= NULL;
	pTM_ElementAddonStr		= NULL;

	pTM_ElementPrecinct     = NULL;
	pTM_ElementHelpScript   = NULL;
	pTM_ElementHelpText     = NULL;
	//pTM_ElementTreasure     = NULL;

	//pTM_ElementTransTarget  = NULL;
	pTM_ElementDomainInfor  = NULL;
	pTM_ElementDomainInfor1  = NULL;
	pTM_ElementGShop        = NULL;
	pTM_ElementGShopTypes   = NULL;
	pTM_ElementGShop1        = NULL;
	pTM_ElementGShopTypes1   = NULL;
	pTM_ElementGShop2		= NULL;
	pTM_ElementGShopTypes2	= NULL;
	pTM_ElementHomeTownData = NULL;

	m_dwVersion = LOCALIZE_VERSION;

	m_pDlgSelectItems = 0;
}
void  CDataManager::InitElementDataTypes()
{
	memset(g_pElementTypes, 0, sizeof(g_pElementTypes));

	g_pElementTypes[DT_INVALID] =NULL;
	g_pElementTypes[DT_EQUIPMENT_ADDON] =new CTemplate_ElementName<EQUIPMENT_ADDON>(DT_EQUIPMENT_ADDON);
	g_pElementTypes[DT_EQUIPMENT_MAJOR_TYPE] = new CTemplate_ElementName<EQUIPMENT_MAJOR_TYPE>(DT_EQUIPMENT_MAJOR_TYPE);//new
	g_pElementTypes[DT_EQUIPMENT_SUB_TYPE] = new CTemplate_ElementName<EQUIPMENT_SUB_TYPE>(DT_EQUIPMENT_SUB_TYPE);//new
	g_pElementTypes[DT_EQUIPMENT_ESSENCE] = new CTemplate_ElementEquipment(DT_EQUIPMENT_ESSENCE);//new?
	g_pElementTypes[DT_REFINE_TICKET_ESSENCE] =new CTemplate_ElementName<REFINE_TICKET_ESSENCE>(DT_REFINE_TICKET_ESSENCE);
	g_pElementTypes[DT_MEDICINE_MAJOR_TYPE] =new CTemplate_ElementName<MEDICINE_MAJOR_TYPE>(DT_MEDICINE_MAJOR_TYPE);
	g_pElementTypes[DT_MEDICINE_SUB_TYPE] =new CTemplate_ElementName<MEDICINE_SUB_TYPE>(DT_MEDICINE_SUB_TYPE);
	g_pElementTypes[DT_MEDICINE_ESSENCE] =new CTemplate_ElementName<MEDICINE_ESSENCE>(DT_MEDICINE_ESSENCE);
	g_pElementTypes[DT_MATERIAL_MAJOR_TYPE] =new CTemplate_ElementName<MATERIAL_MAJOR_TYPE>(DT_MATERIAL_MAJOR_TYPE);
	g_pElementTypes[DT_MATERIAL_SUB_TYPE] =new CTemplate_ElementName<MATERIAL_SUB_TYPE>(DT_MATERIAL_SUB_TYPE);
	g_pElementTypes[DT_MATERIAL_ESSENCE] =new CTemplate_ElementName<MATERIAL_ESSENCE>(DT_MATERIAL_ESSENCE);
	g_pElementTypes[DT_SKILLTOME_SUB_TYPE] =new CTemplate_ElementName<SKILLTOME_SUB_TYPE>(DT_SKILLTOME_SUB_TYPE);
	g_pElementTypes[DT_SKILLTOME_ESSENCE] =new CTemplate_ElementName<SKILLTOME_ESSENCE>(DT_SKILLTOME_ESSENCE);
	g_pElementTypes[DT_TRANSMITROLL_ESSENCE] = new CTemplate_ElementName<TRANSMITROLL_ESSENCE>(DT_TRANSMITROLL_ESSENCE);//new
	g_pElementTypes[DT_LUCKYROLL_ESSENCE] = new CTemplate_ElementName<LUCKYROLL_ESSENCE>(DT_LUCKYROLL_ESSENCE);//new
	g_pElementTypes[DT_TOWNSCROLL_ESSENCE] =new CTemplate_ElementName<TOWNSCROLL_ESSENCE>(DT_TOWNSCROLL_ESSENCE);
	g_pElementTypes[DT_REVIVESCROLL_ESSENCE] =new CTemplate_ElementName<REVIVESCROLL_ESSENCE>(DT_REVIVESCROLL_ESSENCE);
	g_pElementTypes[DT_TASKMATTER_ESSENCE] =new CTemplate_ElementName<TASKMATTER_ESSENCE>(DT_TASKMATTER_ESSENCE);
	g_pElementTypes[DT_DROPTABLE_TYPE] = new CTemplate_ElementName<DROPTABLE_TYPE>(DT_DROPTABLE_TYPE);//new
	g_pElementTypes[DT_DROPTABLE_ESSENCE] = new CTemplate_ElementName<DROPTABLE_ESSENCE>(DT_DROPTABLE_ESSENCE);//new
	g_pElementTypes[DT_MONSTER_TYPE] =new CTemplate_ElementName<MONSTER_TYPE>(DT_MONSTER_TYPE);
	g_pElementTypes[DT_MONSTER_ESSENCE] =new CTemplate_ElementMonster(DT_MONSTER_ESSENCE);
	g_pElementTypes[DT_NPC_TALK_SERVICE] =new CTemplate_ElementName<NPC_TALK_SERVICE>(DT_NPC_TALK_SERVICE);
	g_pElementTypes[DT_NPC_SELL_SERVICE] =new CTemplate_ElementNpcSell(DT_NPC_SELL_SERVICE);
	g_pElementTypes[DT_NPC_BUY_SERVICE] =new CTemplate_ElementName<NPC_BUY_SERVICE>(DT_NPC_BUY_SERVICE);
	g_pElementTypes[DT_NPC_TASK_IN_SERVICE] =new CTemplate_ElementName<NPC_TASK_IN_SERVICE>(DT_NPC_TASK_IN_SERVICE);
	g_pElementTypes[DT_NPC_TASK_OUT_SERVICE] =new CTemplate_ElementName<NPC_TASK_OUT_SERVICE>(DT_NPC_TASK_OUT_SERVICE);
	g_pElementTypes[DT_NPC_TASK_MATTER_SERVICE] =new CTemplate_ElementName<NPC_TASK_MATTER_SERVICE>(DT_NPC_TASK_MATTER_SERVICE);
	g_pElementTypes[DT_NPC_HEAL_SERVICE] =new CTemplate_ElementName<NPC_HEAL_SERVICE>(DT_NPC_HEAL_SERVICE);
	g_pElementTypes[DT_NPC_TRANSMIT_SERVICE] =new CTemplate_ElementNpcTransmit(DT_NPC_TRANSMIT_SERVICE);
	g_pElementTypes[DT_NPC_PROXY_SERVICE] =new CTemplate_ElementName<NPC_PROXY_SERVICE>(DT_NPC_PROXY_SERVICE);
	g_pElementTypes[DT_NPC_STORAGE_SERVICE] =new CTemplate_ElementName<NPC_STORAGE_SERVICE>(DT_NPC_STORAGE_SERVICE);
	g_pElementTypes[DT_NPC_TYPE] =new CTemplate_ElementName<NPC_TYPE>(DT_NPC_TYPE);
	g_pElementTypes[DT_NPC_ESSENCE] =new CTemplate_ElementNpc(DT_NPC_ESSENCE);
	g_pElementTypes[DT_TALK_PROC] =new CTemplate_ElementTalk(DT_TALK_PROC);
	g_pElementTypes[DT_RECIPE_MAJOR_TYPE] =new CTemplate_ElementName<RECIPE_MAJOR_TYPE>(DT_RECIPE_MAJOR_TYPE);
	g_pElementTypes[DT_RECIPE_SUB_TYPE] =new CTemplate_ElementName<RECIPE_SUB_TYPE>(DT_RECIPE_SUB_TYPE);
	g_pElementTypes[DT_RECIPE_ESSENCE] =new CTemplate_ElementName<RECIPE_ESSENCE>(DT_RECIPE_ESSENCE);
	g_pElementTypes[DT_ENEMY_FACTION_CONFIG] =new CTemplate_ElementName<ENEMY_FACTION_CONFIG>(DT_ENEMY_FACTION_CONFIG);
	g_pElementTypes[DT_CHARACTER_CLASS_CONFIG] =new CTemplate_ElementName<CHARACTER_CLASS_CONFIG>(DT_CHARACTER_CLASS_CONFIG);
	g_pElementTypes[DT_PARAM_ADJUST_CONFIG] =new CTemplate_ElementName<PARAM_ADJUST_CONFIG>(DT_PARAM_ADJUST_CONFIG);
	g_pElementTypes[DT_TASKDICE_ESSENCE] =new CTemplate_ElementName<TASKDICE_ESSENCE>(DT_TASKDICE_ESSENCE);
	g_pElementTypes[DT_TASKNORMALMATTER_ESSENCE] =new CTemplate_ElementName<TASKNORMALMATTER_ESSENCE>(DT_TASKNORMALMATTER_ESSENCE);
	g_pElementTypes[DT_PLAYER_LEVELEXP_CONFIG] =new CTemplate_ElementName<PLAYER_LEVELEXP_CONFIG>(DT_PLAYER_LEVELEXP_CONFIG);
	g_pElementTypes[DT_MINE_TYPE] =new CTemplate_ElementName<MINE_TYPE>(DT_MINE_TYPE);
	g_pElementTypes[DT_MINE_ESSENCE] =new CTemplate_ElementName<MINE_ESSENCE>(DT_MINE_ESSENCE);
	g_pElementTypes[DT_GM_GENERATOR_TYPE] =new CTemplate_ElementName<GM_GENERATOR_TYPE>(DT_GM_GENERATOR_TYPE);
	g_pElementTypes[DT_GM_GENERATOR_ESSENCE] =new CTemplate_ElementName<GM_GENERATOR_ESSENCE>(DT_GM_GENERATOR_ESSENCE);
	g_pElementTypes[DT_FIREWORKS_ESSENCE] =new CTemplate_ElementName<FIREWORKS_ESSENCE>(DT_FIREWORKS_ESSENCE);
	g_pElementTypes[DT_NPC_WAR_TOWERBUILD_SERVICE] =new CTemplate_ElementName<NPC_WAR_TOWERBUILD_SERVICE>(DT_NPC_WAR_TOWERBUILD_SERVICE);
	g_pElementTypes[DT_PLAYER_SECONDLEVEL_CONFIG] =new CTemplate_ElementName<PLAYER_SECONDLEVEL_CONFIG>(DT_PLAYER_SECONDLEVEL_CONFIG);
	g_pElementTypes[DT_NPC_RESETPROP_SERVICE] =new CTemplate_ElementName<NPC_RESETPROP_SERVICE>(DT_NPC_RESETPROP_SERVICE);
	g_pElementTypes[DT_ESTONE_ESSENCE] = new CTemplate_ElementName<ESTONE_ESSENCE>(DT_ESTONE_ESSENCE);//new
	g_pElementTypes[DT_PSTONE_ESSENCE] = new CTemplate_ElementName<PSTONE_ESSENCE>(DT_PSTONE_ESSENCE);//new
	g_pElementTypes[DT_SSTONE_ESSENCE] = new CTemplate_ElementName<SSTONE_ESSENCE>(DT_SSTONE_ESSENCE);//new
	g_pElementTypes[DT_RECIPEROLL_MAJOR_TYPE] = new CTemplate_ElementName<RECIPEROLL_MAJOR_TYPE>(DT_RECIPEROLL_MAJOR_TYPE);//new
	g_pElementTypes[DT_RECIPEROLL_SUB_TYPE] = new CTemplate_ElementName<RECIPEROLL_SUB_TYPE>(DT_RECIPEROLL_SUB_TYPE);//new
	g_pElementTypes[DT_RECIPEROLL_ESSENCE] = new CTemplate_ElementName<RECIPEROLL_ESSENCE>(DT_RECIPEROLL_ESSENCE);//new
	g_pElementTypes[DT_SUITE_ESSENCE] =new CTemplate_ElementName<SUITE_ESSENCE>(DT_SUITE_ESSENCE);
	g_pElementTypes[DT_DOUBLE_EXP_ESSENCE] = new CTemplate_ElementName<DOUBLE_EXP_ESSENCE>(DT_DOUBLE_EXP_ESSENCE);//new
	g_pElementTypes[DT_DESTROYING_ESSENCE] =new CTemplate_ElementName<DESTROYING_ESSENCE>(DT_DESTROYING_ESSENCE);
	g_pElementTypes[DT_NPC_EQUIPBIND_SERVICE] =new CTemplate_ElementName<NPC_EQUIPBIND_SERVICE>(DT_NPC_EQUIPBIND_SERVICE);
	g_pElementTypes[DT_NPC_EQUIPDESTROY_SERVICE] =new CTemplate_ElementName<NPC_EQUIPDESTROY_SERVICE>(DT_NPC_EQUIPDESTROY_SERVICE);
	g_pElementTypes[DT_NPC_EQUIPUNDESTROY_SERVICE] =new CTemplate_ElementName<NPC_EQUIPUNDESTROY_SERVICE>(DT_NPC_EQUIPUNDESTROY_SERVICE);
	g_pElementTypes[DT_SKILLMATTER_ESSENCE] =new CTemplate_ElementName<SKILLMATTER_ESSENCE>(DT_SKILLMATTER_ESSENCE);
	g_pElementTypes[DT_VEHICLE_ESSENCE] = new CTemplate_ElementName<VEHICLE_ESSENCE>(DT_VEHICLE_ESSENCE);//new
	g_pElementTypes[DT_COUPLE_JUMPTO_ESSENCE] = new CTemplate_ElementName<COUPLE_JUMPTO_ESSENCE>(DT_COUPLE_JUMPTO_ESSENCE);//new
	g_pElementTypes[DT_LOTTERY_ESSENCE] = new CTemplate_ElementLottery(DT_LOTTERY_ESSENCE);//new?
	g_pElementTypes[DT_CAMRECORDER_ESSENCE] = new CTemplate_ElementName<CAMRECORDER_ESSENCE>(DT_CAMRECORDER_ESSENCE);//new
	g_pElementTypes[DT_TITLE_PROP_CONFIG] = new CTemplate_ElementName<TITLE_PROP_CONFIG>(DT_TITLE_PROP_CONFIG);//new
	g_pElementTypes[DT_SPECIAL_ID_CONFIG] = new CTemplate_ElementName<SPECIAL_ID_CONFIG>(DT_SPECIAL_ID_CONFIG);//new
	g_pElementTypes[DT_TEXT_FIREWORKS_ESSENCE] = new CTemplate_ElementName<TEXT_FIREWORKS_ESSENCE>(DT_TEXT_FIREWORKS_ESSENCE);//new?
	

	g_pElementTypes[DT_TALISMAN_MAINPART_ESSENCE] =new CTemplate_ElementTalisman(DT_TALISMAN_MAINPART_ESSENCE);
	g_pElementTypes[DT_TALISMAN_EXPFOOD_ESSENCE] = new CTemplate_ElementName<TALISMAN_EXPFOOD_ESSENCE>(DT_TALISMAN_EXPFOOD_ESSENCE);//new?
	g_pElementTypes[DT_TALISMAN_MERGEKATALYST_ESSENCE] = new CTemplate_ElementName<TALISMAN_MERGEKATALYST_ESSENCE>(DT_TALISMAN_MERGEKATALYST_ESSENCE);//new?
	g_pElementTypes[DT_TALISMAN_ENERGYFOOD_ESSENCE] = new CTemplate_ElementName<TALISMAN_ENERGYFOOD_ESSENCE>(DT_TALISMAN_ENERGYFOOD_ESSENCE);//new?
	g_pElementTypes[DT_SPEAKER_ESSENCE] = new CTemplate_ElementName<SPEAKER_ESSENCE>(DT_SPEAKER_ESSENCE);//new?
	g_pElementTypes[DT_PLAYER_TALENT_CONFIG] = new CTemplate_ElementName<PLAYER_TALENT_CONFIG>(DT_PLAYER_TALENT_CONFIG);//new?
	g_pElementTypes[DT_POTENTIAL_TOME_ESSENCE] = new CTemplate_ElementName<POTENTIAL_TOME_ESSENCE>(DT_POTENTIAL_TOME_ESSENCE);//new?

	g_pElementTypes[DT_WAR_ROLE_CONFIG] = new CTemplate_ElementName<WAR_ROLE_CONFIG>(DT_WAR_ROLE_CONFIG);//new?
	g_pElementTypes[DT_NPC_WAR_BUY_ARCHER_SERVICE] = new CTemplate_ElementName<NPC_WAR_BUY_ARCHER_SERVICE>(DT_NPC_WAR_BUY_ARCHER_SERVICE);//new?
	g_pElementTypes[DT_SIEGE_ARTILLERY_SCROLL_ESSENCE] = new CTemplate_ElementName<SIEGE_ARTILLERY_SCROLL_ESSENCE>(DT_SIEGE_ARTILLERY_SCROLL_ESSENCE);//new?
	g_pElementTypes[DT_PET_BEDGE_ESSENCE] = new CTemplate_ElementPetBedge(DT_PET_BEDGE_ESSENCE);//new?
	g_pElementTypes[DT_PET_FOOD_ESSENCE] = new CTemplate_ElementName<PET_FOOD_ESSENCE>(DT_PET_FOOD_ESSENCE);//new?
	g_pElementTypes[DT_PET_SKILL_ESSENCE] = new CTemplate_ElementName<PET_SKILL_ESSENCE>(DT_PET_SKILL_ESSENCE);//new?
	g_pElementTypes[DT_PET_ARMOR_ESSENCE] =new CTemplate_ElementPetArmor(DT_PET_ARMOR_ESSENCE);
	g_pElementTypes[DT_PET_AUTO_FOOD_ESSENCE] = new CTemplate_ElementName<PET_AUTO_FOOD_ESSENCE>(DT_PET_AUTO_FOOD_ESSENCE);//new?
	g_pElementTypes[DT_PET_REFINE_ESSENCE] = new CTemplate_ElementName<PET_REFINE_ESSENCE>(DT_PET_REFINE_ESSENCE);//new?
	g_pElementTypes[DT_PET_ASSIST_REFINE_ESSENCE] = new CTemplate_ElementName<PET_ASSIST_REFINE_ESSENCE>(DT_PET_ASSIST_REFINE_ESSENCE);//new?
	g_pElementTypes[DT_RENASCENCE_SKILL_CONFIG] = new CTemplate_ElementName<RENASCENCE_SKILL_CONFIG>(DT_RENASCENCE_SKILL_CONFIG);//new?
	g_pElementTypes[DT_RENASCENCE_PROP_CONFIG] = new CTemplate_ElementName<RENASCENCE_PROP_CONFIG>(DT_RENASCENCE_PROP_CONFIG);//new?
	g_pElementTypes[DT_AIRCRAFT_ESSENCE] = new CTemplate_ElementName<AIRCRAFT_ESSENCE>(DT_AIRCRAFT_ESSENCE);//new?
	g_pElementTypes[DT_FLY_ENERGYFOOD_ESSENCE] = new CTemplate_ElementName<FLY_ENERGYFOOD_ESSENCE>(DT_FLY_ENERGYFOOD_ESSENCE);//new?
	g_pElementTypes[DT_ITEM_TRADE_CONFIG] = new CTemplate_ElementItemTrade(DT_ITEM_TRADE_CONFIG);//new?
	g_pElementTypes[DT_BOOK_ESSENCE] = new CTemplate_ElementName<BOOK_ESSENCE>(DT_BOOK_ESSENCE);//new?
	g_pElementTypes[DT_PLAYER_SKILL_POINT_CONFIG] = new CTemplate_ElementName<PLAYER_SKILL_POINT_CONFIG>(DT_PLAYER_SKILL_POINT_CONFIG);//new?
	g_pElementTypes[DT_OFFLINE_TRUSTEE_ESSENCE] = new CTemplate_ElementName<OFFLINE_TRUSTEE_ESSENCE>(DT_OFFLINE_TRUSTEE_ESSENCE);//new?
	g_pElementTypes[DT_EQUIP_SOUL_ESSENCE] = new CTemplate_ElementName<EQUIP_SOUL_ESSENCE>(DT_EQUIP_SOUL_ESSENCE);//new?
	g_pElementTypes[DT_EQUIP_SOUL_MELD_SERVICE] = new CTemplate_ElementName<EQUIP_SOUL_MELD_SERVICE>(DT_EQUIP_SOUL_MELD_SERVICE);//new?
	g_pElementTypes[DT_PIE_LOVE_CONFIG] = new CTemplate_ElementName<PIE_LOVE_CONFIG>(DT_PIE_LOVE_CONFIG);//new?

	g_pElementTypes[DT_SPECIAL_NAME_ITEM_ESSENCE] = new CTemplate_ElementName<SPECIAL_NAME_ITEM_ESSENCE>(DT_SPECIAL_NAME_ITEM_ESSENCE);//new?
	g_pElementTypes[DT_RECYCLE_ITEM_CONFIG] = new CTemplate_ElementName<RECYCLE_ITEM_CONFIG>(DT_RECYCLE_ITEM_CONFIG);//new?
	g_pElementTypes[DT_SCORE_TO_RANK_CONFIG] = new CTemplate_ElementScoreToRank(DT_SCORE_TO_RANK_CONFIG);//new?
	g_pElementTypes[DT_BATTLE_DROP_CONFIG] = new CTemplate_ElementName<BATTLE_DROP_CONFIG>(DT_BATTLE_DROP_CONFIG);//new?
	g_pElementTypes[DT_BATTLE_DEPRIVE_CONFIG] = new CTemplate_ElementName<BATTLE_DEPRIVE_CONFIG>(DT_BATTLE_DEPRIVE_CONFIG);//new?
	g_pElementTypes[DT_BATTLE_SCORE_CONFIG] = new CTemplate_ElementName<BATTLE_SCORE_CONFIG>(DT_BATTLE_SCORE_CONFIG);//new?
	g_pElementTypes[DT_GIFT_BAG_ESSENCE] = new CTemplate_ElementName<GIFT_BAG_ESSENCE>(DT_GIFT_BAG_ESSENCE);//new?
	g_pElementTypes[DT_VIP_CARD_ESSENCE] = new CTemplate_ElementName<VIP_CARD_ESSENCE>(DT_VIP_CARD_ESSENCE);//new?	
	g_pElementTypes[DT_INSTANCING_BATTLE_CONFIG] = new CTemplate_ElementInstancingBattleConfig(DT_INSTANCING_BATTLE_CONFIG);
	g_pElementTypes[DT_CHANGE_SHAPE_CARD_ESSENCE] = new CTemplate_ElementChangeShapeCardEssence(DT_CHANGE_SHAPE_CARD_ESSENCE);
	g_pElementTypes[DT_CHANGE_SHAPE_STONE_ESSENCE] = new CTemplate_ElementName<CHANGE_SHAPE_STONE_ESSENCE>(DT_CHANGE_SHAPE_STONE_ESSENCE);
	g_pElementTypes[DT_CHANGE_SHAPE_PROP_CONFIG] = new CTemplate_ElementName<CHANGE_SHAPE_PROP_CONFIG>(DT_CHANGE_SHAPE_PROP_CONFIG);
	g_pElementTypes[DT_ORIGINAL_SHAPE_CONFIG] = new CTemplate_ElementName<ORIGINAL_SHAPE_CONFIG>(DT_ORIGINAL_SHAPE_CONFIG);
	g_pElementTypes[DT_LIFE_SKILL_CONFIG] = new CTemplate_ElementName<LIFE_SKILL_CONFIG>(DT_LIFE_SKILL_CONFIG);
	g_pElementTypes[DT_ARENA_DROPTABLE_CONFIG] = new CTemplate_ElementName<ARENA_DROPTABLE_CONFIG>(DT_ARENA_DROPTABLE_CONFIG);
	g_pElementTypes[DT_MERCENARY_CREDENTIAL_ESSENCE] = new CTemplate_ElementName<MERCENARY_CREDENTIAL_ESSENCE>(DT_MERCENARY_CREDENTIAL_ESSENCE);
	g_pElementTypes[DT_TELEPORTATION_ESSENCE] = new CTemplate_ElementName<TELEPORTATION_ESSENCE>(DT_TELEPORTATION_ESSENCE);
	g_pElementTypes[DT_TELEPORTATION_STONE_ESSENCE] = new CTemplate_ElementName<TELEPORTATION_STONE_ESSENCE>(DT_TELEPORTATION_STONE_ESSENCE);
	g_pElementTypes[DT_COMBINE_SKILL_EDIT_CONFIG] = new CTemplate_ElementName<COMBINE_SKILL_EDIT_CONFIG>(DT_COMBINE_SKILL_EDIT_CONFIG);
	g_pElementTypes[DT_UPGRADE_EQUIP_CONFIG] = new CTemplate_ElementName<UPGRADE_EQUIP_CONFIG>(DT_UPGRADE_EQUIP_CONFIG);
	g_pElementTypes[DT_UPGRADE_EQUIP_STONE_ESSENCE] = new CTemplate_ElementName<UPGRADE_EQUIP_STONE_ESSENCE>(DT_UPGRADE_EQUIP_STONE_ESSENCE);
	g_pElementTypes[DT_NPC_CONSIGN_SERVICE] = new CTemplate_ElementName<NPC_CONSIGN_SERVICE>(DT_NPC_CONSIGN_SERVICE);
	g_pElementTypes[DT_DEITY_CHARACTER_CONFIG] = new CTemplate_ElementName<DEITY_CHARACTER_CONFIG>(DT_DEITY_CHARACTER_CONFIG);	
	g_pElementTypes[DT_LOTTERY2_ESSENCE] = new CTemplate_ElementLottery2(DT_LOTTERY2_ESSENCE);
	g_pElementTypes[DT_GEM_CONFIG] = new CTemplate_ElementName<GEM_CONFIG>(DT_GEM_CONFIG);
	g_pElementTypes[DT_GEM_ESSENCE] = new CTemplate_ElementName<GEM_ESSENCE>(DT_GEM_ESSENCE);
	g_pElementTypes[DT_GEM_SEAL_ESSENCE] = new CTemplate_ElementName<GEM_SEAL_ESSENCE>(DT_GEM_SEAL_ESSENCE);
	g_pElementTypes[DT_GEM_DUST_ESSENCE] = new CTemplate_ElementName<GEM_DUST_ESSENCE>(DT_GEM_DUST_ESSENCE);
	g_pElementTypes[DT_GEM_EXTRACT_CONFIG] = new CTemplate_ElementName<GEM_EXTRACT_CONFIG>(DT_GEM_EXTRACT_CONFIG);
	g_pElementTypes[DT_GENERAL_ARTICLE_ESSENCE] = new CTemplate_ElementName<GENERAL_ARTICLE_ESSENCE>(DT_GENERAL_ARTICLE_ESSENCE);
	g_pElementTypes[DT_LOTTERY3_ESSENCE] = new CTemplate_ElementName<LOTTERY3_ESSENCE>(DT_LOTTERY3_ESSENCE);
	g_pElementTypes[DT_TRANSCRIPTION_CONFIG] = new CTemplate_ElementName<TRANSCRIPTION_CONFIG>(DT_TRANSCRIPTION_CONFIG);
	g_pElementTypes[DT_ONLINE_GIFT_BAG_CONFIG] = new CTemplate_ElementName<ONLINE_GIFT_BAG_CONFIG>(DT_ONLINE_GIFT_BAG_CONFIG);
	g_pElementTypes[DT_SMALL_ONLINE_GIFT_BAG_ESSENCE] = new CTemplate_ElementName<SMALL_ONLINE_GIFT_BAG_ESSENCE>(DT_SMALL_ONLINE_GIFT_BAG_ESSENCE);
	g_pElementTypes[DT_SCROLL_REGION_CONFIG] = new CTemplate_ElementScrollRegionConfig(DT_SCROLL_REGION_CONFIG);
	g_pElementTypes[DT_SCROLL_UNLOCK_ESSENCE] = new CTemplate_ElementName<SCROLL_UNLOCK_ESSENCE>(DT_SCROLL_UNLOCK_ESSENCE);
	g_pElementTypes[DT_SCROLL_DIG_COUNT_INC_ESSENCE] = new CTemplate_ElementName<SCROLL_DIG_COUNT_INC_ESSENCE>(DT_SCROLL_DIG_COUNT_INC_ESSENCE);
	g_pElementTypes[DT_TOWER_TRANSCRIPTION_CONFIG] = new CTemplate_ElementName<TOWER_TRANSCRIPTION_CONFIG>(DT_TOWER_TRANSCRIPTION_CONFIG);
	g_pElementTypes[DT_TOWER_TRANSCRIPTION_PROPERTY_CONFIG] = new CTemplate_ElementName<TOWER_TRANSCRIPTION_PROPERTY_CONFIG>(DT_TOWER_TRANSCRIPTION_PROPERTY_CONFIG);
	g_pElementTypes[DT_RUNE_DATA_CONFIG] = new CTemplate_ElementName<RUNE_DATA_CONFIG>(DT_RUNE_DATA_CONFIG);
	g_pElementTypes[DT_RUNE_COMB_PROPERTY] = new CTemplate_ElementName<RUNE_COMB_PROPERTY>(DT_RUNE_COMB_PROPERTY);
	g_pElementTypes[DT_RUNE_EQUIP_ESSENCE] = new CTemplate_ElementName<RUNE_EQUIP_ESSENCE>(DT_RUNE_EQUIP_ESSENCE);	
	g_pElementTypes[DT_LITTLE_PET_UPGRADE_CONFIG] = new CTemplate_ElementName<LITTLE_PET_UPGRADE_CONFIG>(DT_LITTLE_PET_UPGRADE_CONFIG);	
	g_pElementTypes[DT_RUNE_SLOT_ESSENCE] = new CTemplate_ElementName<RUNE_SLOT_ESSENCE>(DT_RUNE_SLOT_ESSENCE);	
	g_pElementTypes[DT_DROP_INTERNAL_CONFIG] = new CTemplate_ElementDropInternalConfig(DT_DROP_INTERNAL_CONFIG);
	g_pElementTypes[DT_PK2012_GUESS_CONFIG] = new CTemplate_ElementName<PK2012_GUESS_CONFIG> (DT_PK2012_GUESS_CONFIG);
	g_pElementTypes[DT_COLLISION_RAID_TRANSFIGURE_CONFIG] = new CTemplate_ElementCollRaidTransfigConfig (DT_COLLISION_RAID_TRANSFIGURE_CONFIG);
	g_pElementTypes[DT_BOOTH_FIGURE_ITEM_ESSENCE] = new CTemplate_ElementName<BOOTH_FIGURE_ITEM_ESSENCE> (DT_BOOTH_FIGURE_ITEM_ESSENCE);
	g_pElementTypes[DT_FLAG_BUFF_ITEM_ESSENCE] = new CTemplate_ElementName<FLAG_BUFF_ITEM_ESSENCE> (DT_FLAG_BUFF_ITEM_ESSENCE);
	g_pElementTypes[DT_NPC_REPUTATION_SHOP_SERVICE] = new CTemplate_ElementNpcRepShopSvc (DT_NPC_REPUTATION_SHOP_SERVICE);
	g_pElementTypes[DT_NPC_UI_TRANSFER_SERVICE] = new CTemplate_ElementName<NPC_UI_TRANSFER_SERVICE> (DT_NPC_UI_TRANSFER_SERVICE);
	g_pElementTypes[DT_UI_TRANSFER_CONFIG] = new CTemplate_ElementUITransferCfg (DT_UI_TRANSFER_CONFIG);
	g_pElementTypes[DT_XINGZUO_ITEM_ESSENCE] = new CTemplate_ElementName<XINGZUO_ITEM_ESSENCE> (DT_XINGZUO_ITEM_ESSENCE);
	g_pElementTypes[DT_COLLISION_RAID_AWARD_CONFIG] = new CTemplate_ElementName<COLLISION_RAID_AWARD_CONFIG> (DT_COLLISION_RAID_AWARD_CONFIG);
	g_pElementTypes[DT_XINGZUO_ENERGY_ITEM_ESSENCE] = new CTemplate_ElementName<XINGZUO_ENERGY_ITEM_ESSENCE> (DT_XINGZUO_ENERGY_ITEM_ESSENCE);

	g_pElementTypes[DT_COLLISION_RAID_AWARD_CONFIG] = new CTemplate_ElementName<COLLISION_RAID_AWARD_CONFIG> (DT_COLLISION_RAID_AWARD_CONFIG);
	g_pElementTypes[DT_CASH_MEDIC_MERGE_ITEM_ESSENCE] = new CTemplate_ElementName<CASH_MEDIC_MERGE_ITEM_ESSENCE> (DT_CASH_MEDIC_MERGE_ITEM_ESSENCE);
	g_pElementTypes[DT_BUFF_AREA_CONFIG] = new CTemplate_ElementName<BUFF_AREA_CONFIG> (DT_BUFF_AREA_CONFIG);
	g_pElementTypes[DT_LIVENESS_CONFIG] = new CTemplate_ElementLivenessCfg (DT_LIVENESS_CONFIG);
	g_pElementTypes[DT_CHALLENGE_2012_CONFIG] = new CTemplate_ElementName<CHALLENGE_2012_CONFIG> (DT_CHALLENGE_2012_CONFIG);
	g_pElementTypes[DT_SALE_PROMOTION_ITEM_ESSENCE] = new CTemplate_ElementSalePromotionItem (DT_SALE_PROMOTION_ITEM_ESSENCE);
	g_pElementTypes[DT_GIFT_BAG_LOTTERY_DELIVER_ESSENCE] = new CTemplate_ElementName<GIFT_BAG_LOTTERY_DELIVER_ESSENCE> (DT_GIFT_BAG_LOTTERY_DELIVER_ESSENCE);
	g_pElementTypes[DT_LOTTERY_TANGYUAN_ITEM_ESSENCE] = new CTemplate_ElementName<LOTTERY_TANGYUAN_ITEM_ESSENCE> (DT_LOTTERY_TANGYUAN_ITEM_ESSENCE);
	g_pElementTypes[DT_TASK_SPECIAL_AWARD_CONFIG] = new CTemplate_ElementTaskSpecialAwardCfg (DT_TASK_SPECIAL_AWARD_CONFIG);

	g_pElementTypes[DT_GIFT_PACK_ITEM_ESSENCE] = new CTemplate_ElementName<GIFT_PACK_ITEM_ESSENCE> (DT_GIFT_PACK_ITEM_ESSENCE);
	g_pElementTypes[DT_PROP_ADD_MATERIAL_ITEM_ESSENCE] = new CTemplate_ElementName<PROP_ADD_MATERIAL_ITEM_ESSENCE> (DT_PROP_ADD_MATERIAL_ITEM_ESSENCE);
	g_pElementTypes[DT_PROP_ADD_ITEM_ESSENCE] = new CTemplate_ElementName<PROP_ADD_ITEM_ESSENCE> (DT_PROP_ADD_ITEM_ESSENCE);
	g_pElementTypes[DT_PROP_ADD_CONFIG] = new CTemplate_ElementName<PROP_ADD_CONFIG> (DT_PROP_ADD_CONFIG);
	g_pElementTypes[DT_KING_WAR_CONFIG] = new CTemplate_ElementName<KING_WAR_CONFIG> (DT_KING_WAR_CONFIG);
	g_pElementTypes[DT_JINFASHEN_TO_MONEY_CONFIG] = new CTemplate_ElementName<JINFASHEN_TO_MONEY_CONFIG> (DT_JINFASHEN_TO_MONEY_CONFIG);

	g_pElementTypes[DT_BATTLE_201304_CONFIG] = new CTemplate_ElementName<BATTLE_201304_CONFIG> (DT_BATTLE_201304_CONFIG);
	g_pElementTypes[DT_RUNE2013_ITEM_ESSENCE] = new CTemplate_ElementName<RUNE2013_ITEM_ESSENCE> (DT_RUNE2013_ITEM_ESSENCE);
	g_pElementTypes[DT_RUNE2013_CONFIG] = new CTemplate_ElementName<RUNE2013_CONFIG> (DT_RUNE2013_CONFIG);
	g_pElementTypes[DT_BASHE_AWARD_CONFIG] = new CTemplate_ElementName<BASHE_AWARD_CONFIG> (DT_BASHE_AWARD_CONFIG);
}

void CDataManager::DestoryElementDataTypes()
{
	for(  int i = 0; i < DT_MAX; i++)
		if(g_pElementTypes[i]) delete g_pElementTypes[i];
}

void CDataManager::InitHomeTownDataTypes()
{
	g_pHomeTownTypes[HOMETOWN::DT_CROP_ESSENCE] = new CTemplate_ElementName<HOMETOWN::CROP_ESSENCE>(HOMETOWN::DT_CROP_ESSENCE);
	g_pHomeTownTypes[HOMETOWN::DT_FRUIT_ESSENCE] = new CTemplate_ElementName<HOMETOWN::FRUIT_ESSENCE>(HOMETOWN::DT_FRUIT_ESSENCE);
	g_pHomeTownTypes[HOMETOWN::DT_TOOL_FUNC] = new CTemplate_ElementName<HOMETOWN::TOOL_FUNC>(HOMETOWN::DT_TOOL_FUNC);
	g_pHomeTownTypes[HOMETOWN::DT_TOOL_ESSENCE] = new CTemplate_ElementName<HOMETOWN::TOOL_ESSENCE>(HOMETOWN::DT_TOOL_ESSENCE);
}

void CDataManager::DestoryHomeTownDataTypes()
{
	for(  int i = 0; i < HOMETOWN::DT_MAX; i++)
		if(g_pHomeTownTypes[i]) delete g_pHomeTownTypes[i];
}


void CDataManager::Release()
{
	if(pTM_ElementData)			{delete pTM_ElementData; pTM_ElementData = NULL; }
	if(pTM_ElementTask)			{delete pTM_ElementTask; pTM_ElementTask = NULL; }
	if(pTM_ElementPolicy)		{delete pTM_ElementPolicy; pTM_ElementPolicy = NULL; }
	if(pTM_ElementInterfaceHint){delete pTM_ElementInterfaceHint; pTM_ElementInterfaceHint = NULL; }
	if(pTM_ElementTextFile)		{ delete pTM_ElementTextFile; pTM_ElementTextFile = NULL; }
	if(pTM_ElementInterfaceString){delete pTM_ElementInterfaceString; pTM_ElementInterfaceString = NULL; }
	if(pTM_ElementItemDesc)     {delete pTM_ElementItemDesc; pTM_ElementItemDesc = NULL; }
	if(pTM_ElementInstaceText)  {delete pTM_ElementInstaceText; pTM_ElementInstaceText = NULL;}

	if(pTM_ElementItemExDesc)	{delete pTM_ElementItemExDesc; pTM_ElementItemExDesc = NULL; }
	if(pTM_ElementSkillDesc)	{delete pTM_ElementSkillDesc; pTM_ElementSkillDesc = NULL; }
	if(pTM_ElementFixMsg)	    {delete pTM_ElementFixMsg; pTM_ElementFixMsg = NULL; }
	if(pTM_ElementServerErr)	{delete pTM_ElementServerErr; pTM_ElementServerErr = NULL; }
	if(pTM_ElementBuffDesc)	    {delete pTM_ElementBuffDesc; pTM_ElementBuffDesc = NULL; }
	if(pTM_ElementTaskErr)      {delete pTM_ElementTaskErr; pTM_ElementTaskErr = NULL; }
	if(pTM_ElementLoadingHint)      {delete pTM_ElementLoadingHint; pTM_ElementLoadingHint = NULL; }
	if(pTM_ElementPetTalk)      {delete pTM_ElementPetTalk; pTM_ElementPetTalk = NULL; }
	if(pTM_ElementAddonStr)      {delete pTM_ElementAddonStr; pTM_ElementAddonStr = NULL; }
	

	if(pTM_ElementPrecinct)     {delete pTM_ElementPrecinct; pTM_ElementPrecinct = NULL; }
	if(pTM_ElementHelpScript)   {delete pTM_ElementHelpScript; pTM_ElementHelpScript = NULL; }
	if(pTM_ElementHelpText)     {delete pTM_ElementHelpText; pTM_ElementHelpText = NULL; }
	
	if(pTM_ElementDomainInfor)  {delete pTM_ElementDomainInfor; pTM_ElementDomainInfor = NULL; }
	if(pTM_ElementDomainInfor1)  {delete pTM_ElementDomainInfor1; pTM_ElementDomainInfor1 = NULL; }
	if(pTM_ElementGShop)        {delete pTM_ElementGShop; pTM_ElementGShop = NULL; }
	if(pTM_ElementGShopTypes)   {delete pTM_ElementGShopTypes; pTM_ElementGShopTypes = NULL; }
	if(pTM_ElementGShop1)        {delete pTM_ElementGShop1; pTM_ElementGShop1 = NULL; }
	if(pTM_ElementGShopTypes1)   {delete pTM_ElementGShopTypes1; pTM_ElementGShopTypes1 = NULL; }
	if(pTM_ElementGShop2)        {delete pTM_ElementGShop2; pTM_ElementGShop2 = NULL; }
	if(pTM_ElementGShopTypes2)   {delete pTM_ElementGShopTypes2; pTM_ElementGShopTypes2 = NULL; }
	if(pTM_ElementHomeTownData) { delete pTM_ElementHomeTownData; pTM_ElementHomeTownData = NULL; }
}

const TCHAR* szCellHead = _T("<Cell><Data ss:Type=\"String\">");
const TCHAR* szCellHead1 = _T("<Cell ss:StyleID=\"s21\"><Data ss:Type=\"String\">");
bool ReadLineW(FILE *pFile,TCHAR *szLine,int& len)
{
	WCHAR ch,ch2;
	int nLength = 0;
	while(true)
	{
		if(0==fread(&ch,sizeof(WCHAR),1,pFile)) 
		{
			szLine[nLength] = 0;
			len = nLength;
			return false;
		}
		if(ch=='\r') 
		{
			if(0==fread(&ch2,sizeof(WCHAR),1,pFile)) 
			{
				szLine[nLength] = 0;
				len = nLength;
				return false;
			}
			if(ch2=='\n')
			{
				szLine[nLength] = 0;
				len = nLength;
				return true;
			}
		}
		szLine[nLength++] = ch;
	}
}

bool ReadLineA(FILE *pFile,char *szLine,int& len)
{
	char ch,ch2;
	int nLength = 0;
	bool bStart = false;
	while(true)
	{
		if(0==fread(&ch,sizeof(char),1,pFile)) 
		{
			szLine[nLength] = 0;
			len = nLength;
			return false;
		}
		if(ch=='\r') 
		{
			if(0==fread(&ch2,sizeof(char),1,pFile)) 
			{
				szLine[nLength] = 0;
				len = nLength;
				return false;
			}
			if(ch2=='\n')
			{
				szLine[nLength] = 0;
				len = nLength;
				return true;
			}
			return true;
		}

		if( ch != ' ')
			bStart = true;
		if( ch == ' ' && !bStart) continue;
		
		szLine[nLength++] = ch;
	}
}

bool ReadLine(FILE *pFile,TCHAR *szLine,int& len,bool bUnicode)
{
	if(bUnicode) return ReadLineW(pFile,szLine,len);
	else
	{
		char szBuffer[MAX_BUFFER_LEN];
		memset(szBuffer,0,MAX_BUFFER_LEN);
		bool r = ReadLineA(pFile,szBuffer,len);
		if(int len = strlen(szBuffer)>0) 
		{
			int l = MultiByteToWideChar(CP_UTF8,0,szBuffer,strlen(szBuffer),szLine,MAX_BUFFER_LEN*sizeof(WORD));
			szLine[l] = 0;
		}
		return r;
	}
}

void ClearSyntax(CString& strText)
{
	int pos = strText.Find(_T("<Data ss:Type=\"String\""));
	if(pos != -1)
	{
		pos += _tcsclen(_T("<Data ss:Type=\"String\""));
		strText = strText.Right(strText.GetLength() - pos);
		pos = strText.Find(_T(">"));
		pos += _tcsclen(_T(">"));
		strText = strText.Right(strText.GetLength() - pos);
		if(pos==-1) return;
		
		pos = strText.Find(_T("</Data>"));
		if(pos==-1) return;
		strText = strText.Left(pos);
	}else
	{
		pos = strText.Find(_T("<ss:Data ss:Type=\"String\""));
		pos += _tcsclen(_T("<ss:Data ss:Type=\"String\""));
		strText = strText.Right(strText.GetLength() - pos);
		pos = strText.Find(_T(">"));
		pos += _tcsclen(_T(">"));
		strText = strText.Right(strText.GetLength() - pos);
		if(pos==-1) return;
		
		pos = strText.Find(_T("</ss:Data>"));
		if(pos==-1) return;
		strText = strText.Left(pos);
	}
}

bool CDataManager::ImportFromText(const char *szPath,void *pWnd, bool bAccordID)
{
	THD_INFO thdInfox;
	THD_INFO* pThdInfo = &thdInfox;
	pThdInfo->bMsgUpdate = false;
	pThdInfo->iPercent = 0;
	pThdInfo->iThdState = THD_RUN;
	pThdInfo->szProgMsg[0] = 0;
	pThdInfo->bAbort = false;

	CDlgProgress dlg(pThdInfo);
	THD_IMPORTTEXT_INFO thdInfo = {this, szPath, bAccordID, pThdInfo};
	HANDLE hThread = CreateThread(NULL, 0, ThdImportFromText, &thdInfo, 0, NULL);
	int iRet = dlg.DoModal();
	CloseHandle(hThread);
	return (iRet==IDOK);
}

bool CDataManager::ImportFromOrderText(const char *szPath, void *pWnd)
{
	THD_INFO thdInfox;
	THD_INFO* pThdInfo = &thdInfox;
	pThdInfo->bMsgUpdate = false;
	pThdInfo->iPercent = 0;
	pThdInfo->iThdState = THD_RUN;
	pThdInfo->szProgMsg[0] = 0;
	pThdInfo->bAbort = false;

	CDlgProgress dlg(pThdInfo);
	THD_IMPORTORDERTEXT_INFO thdInfo = {this, szPath, pThdInfo, (CElementDataView*)((CMainFrame*)AfxGetMainWnd())->GetLeftView()};
	HANDLE hThread = CreateThread(NULL, 0, ThdImportFromOrderText, &thdInfo, 0, NULL);
	int iRet = dlg.DoModal();
	CloseHandle(hThread);
	return (iRet==IDOK);
}

void CDataManager::EnumImportText(CTemplateBase *pTemplate, XmlLocItem* pItem, bool &bReplaced)
{
	if(pTemplate==NULL || !pItem) return;

	const TCHAR* szID = pItem->_items[XmlLocItem::eIDItem];
	const TCHAR* szOrg = pItem->_items[XmlLocItem::eOriginItem];
	const TCHAR* szTrans = pItem->_items[XmlLocItem::eTransItem];
	const TCHAR* szLabel = pItem->_items[XmlLocItem::eLabelItem];

	if(pItem->_items[XmlLocItem::eIDItem].GetLength() != 0 && !_tcsstr(szID, pTemplate->GetStrID()))
		return;
	int n = pTemplate->GetTextNum();
	for( int i = 0; i < n; ++i)
	{
		CTextBase *pText = pTemplate->GetTextByIdx(i);
		if (szID[0] && pText->GetStrID() != szID)
			continue;
		CString orgText1 = pText->GetOriginalText();
		orgText1.Replace(_T("	"),_T(" "));
		CString orgText2 = szOrg;
		orgText2.Replace(_T("	"),_T(" "));
		if(pText->GetOriginalText() && _tcsicmp(orgText2,orgText1)==0)
		{
			pText->SetTextLabel(szLabel);
			if(szTrans) 
			{
				int nStart,nEnd;
				if (!pText->CheckSyntax(szTrans,nStart,nEnd, false))
				{
					return;
				}
				if(_tcsclen(szTrans) < pText->GetCharUpperLimit() || pText->GetCharUpperLimit() < 0)
				{
					pText->ReplaceTransText(szTrans);
					bReplaced = true;
				}
			}
		}
	}

	n = pTemplate->GetChildNum();
	for( i = 0; i < n; ++i)
	{
		CTemplateBase *pChild = pTemplate->GetChildByIndex(i);
		EnumImportText(pChild, pItem, bReplaced);
	}
}
//输入本地数据
bool  CDataManager::Import(CDataManager *pNewMan, void *param, PROCESSPROC poc)
{
	if(pNewMan->GetVersion() != m_dwVersion)
	{
//		AfxMessageBox(_T("Different version, cannot import!"));
//		return false;
		AfxMessageBox(_T("Different version!"));
	}
	
	CSelectTransDlg selDlg;
	//element data
	
	g_nTotalPer = 10;
	CTemplateManager *pMan  = pNewMan->GetElementDataTM();
	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_DATA))		
		if(!ImportElementData_M(pMan, &selDlg, param, poc)) return false;
	
	g_nTotalPer = 20;
	pMan  = pNewMan->GetElementTaskTM();
	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_TASK))		
		if(!ImportTaskData_M(pMan, &selDlg, param, poc)) return false;
	
	g_nTotalPer = 30;
	pMan  = pNewMan->GetElementItemDescTM();
	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_ITEMDESC))		
		if(!ImportItemDescData_M(pMan, &selDlg, param, poc)) return false;

	pMan  = pNewMan->GetElementInstanceTextTM();
	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_INSTANCE_TEXT))		
		if(!ImportInstanceTextData_M(pMan, &selDlg, param, poc)) return false;	

	g_nTotalPer = 40;
	pMan  = pNewMan->GetElementItemExDescTM();
	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_ITEMEXDESC))		
		if(!ImportItemExDescData_M(pMan, &selDlg, param, poc)) return false;

	g_nTotalPer = 50;
	pMan  = pNewMan->GetElementSkillDescTM();
	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_SKILLDESC))		
		if(!ImportItemSkillDescData_M(pMan, &selDlg, param, poc)) return false;

	g_nTotalPer = 60;
	pMan  = pNewMan->GetElementFixMsgTM();
	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_FIXMSG))		
		if(!ImportItemFixMsgData_M(pMan, &selDlg, param, poc)) return false;

	g_nTotalPer = 70;
	pMan  = pNewMan->GetElementServerErrTM();
	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_SERVERERR))		
		if(!ImportItemServerErrData_M(pMan, &selDlg, param, poc)) return false;

	pMan  = pNewMan->GetElementBuffDescTM();
	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_BUFFDESC))		
		if(!ImportItemBuffDescData_M(pMan, &selDlg, param, poc)) return false;

	pMan  = pNewMan->GetElementTaskErrTM();
	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_TASKERR))		
		if(!ImportItemTaskErrData_M(pMan, &selDlg, param, poc)) return false;

	pMan  = pNewMan->GetElementLoadingHintTM();
	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_LOADINGHINT))		
		if(!ImportItemLoadingHintData_M(pMan, &selDlg, param, poc)) return false;

	pMan  = pNewMan->GetElementPetTalkTM();
	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_PETTALK))		
		if(!ImportItemPetTalkData_M(pMan, &selDlg, param, poc)) return false;

	pMan  = pNewMan->GetElementAddonStrTM();
	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_ADDONSTR))		
		if(!ImportItemAddonStrData_M(pMan, &selDlg, param, poc)) return false;

	g_nTotalPer = 80;
	pMan  = pNewMan->GetElementInterfaceHintTM();
	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_INTERFACEHINT))		
		if(!ImportInterfaceHintData_M(pMan, &selDlg, param, poc)) return false;

	g_nTotalPer = 90;
	pMan  = pNewMan->GetElementInterfaceStringTM();
	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_INTERFACESTRING))		
		if(!ImportInterfaceStringData_M(pMan, &selDlg, param, poc)) return false;

	g_nTotalPer = 91;
	pMan  = pNewMan->GetElementPolicyTM();
	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_POLICY))		
		if(!ImportPolicyData_M(pMan, &selDlg, param, poc)) return false;

	g_nTotalPer = 92;
	pMan  = pNewMan->GetElementPrecinctTM();
	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_PRECINCT))		
		if(!ImportPrecinctData_M(pMan, &selDlg, param, poc)) return false;

	g_nTotalPer = 93;
	pMan  = pNewMan->GetElementHelpScriptTM();
	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_HELPSCRIPT))		
		if(!ImportHelpScriptData_M(pMan, &selDlg, param, poc)) return false;

	g_nTotalPer = 97;
	pMan  = pNewMan->GetElementHelpTextTM();
	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_HELPTEXT))		
		if(!ImportHelpTextData_M(pMan, &selDlg, param, poc)) return false;

	g_nTotalPer = 99;
	if(!ImportDomainData_M(
			m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_DOMAININFOR) ? pNewMan->GetElementDomainInforTM() : NULL, 
			m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_DOMAININFOR1) ? pNewMan->GetElementDomainInfor1TM(): NULL, 
			m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_GSHOP) ? pNewMan->GetElementGShopTM() : NULL,
			m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_GSHOP_TYPES) ? pNewMan->GetElementGShopTypesTM() : NULL, 
			m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_GSHOP1) ?pNewMan->GetElementGShop1TM() : NULL,
			m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_GSHOP_TYPES1) ? pNewMan->GetElementGShopTypes1TM() : NULL,
			m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_GSHOP2) ? pNewMan->GetElementGShop2TM() : NULL,
			m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_GSHOP_TYPES2) ? pNewMan->GetElementGShopTypes2TM() : NULL,
			&selDlg, param, poc)) return false;
	if(!ImportTextFileData_M(pNewMan->GetElementTextFileTM(), &selDlg, param, poc)) return false;
	return  true;
}


bool CDataManager::ImportGameData(void *param, PROCESSPROC poc)
{
	g_nTotalPer = 10;
	if(m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_DATA))
		if(!ImportElementData(param, poc)) return false;
	g_nTotalPer = 20;
	if(m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_TASK))
		if(!ImportTaskData(param,poc)) return false;
	g_nTotalPer = 30;
	if(m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_ITEMDESC))
		if(!ImportItemDescData(param, poc)) return false;
	if(m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_INSTANCE_TEXT))
		if(!ImportInstanceTextData(param, poc)) return false;
	g_nTotalPer = 40;
	if(m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_ITEMEXDESC))
		if(!ImportItemExDescData(param, poc)) return false;
	g_nTotalPer = 50;
	if(m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_SKILLDESC))
		if(!ImportItemSkillDescData(param, poc)) return false;
	g_nTotalPer = 60;
	if(m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_FIXMSG))
		if(!ImportItemFixMsgData(param, poc)) return false;
	g_nTotalPer = 70;
	if(m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_SERVERERR))
		if(!ImportItemServerErrData(param, poc)) return false;
	g_nTotalPer = 75;
	if(m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_BUFFDESC))
		if(!ImportItemBuffDescData(param, poc)) return false;
	if(m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_TASKERR))
		if(!ImportItemTaskErrData(param, poc)) return false;
	if(m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_LOADINGHINT))
		if(!ImportItemLoadingHintData(param, poc)) return false;
	if(m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_PETTALK))
		if(!ImportItemPetTalkData(param, poc)) return false;
	if(m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_ADDONSTR))
		if(!ImportItemAddonStrData(param, poc)) return false;
	g_nTotalPer = 80;
	if(m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_INTERFACEHINT) || 
		m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_INTERFACESTRING) ||
		m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_HELPSCRIPT) ||
		m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_HELPTEXT))
		if(!ImportInterfaceData(param,poc)) return false;
	g_nTotalPer = 90;
	if(m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_POLICY))
		if(!ImportPolicyData(param,poc)) return false;
	g_nTotalPer = 99;
	if(m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_PRECINCT))
		if(!ImportPrecinctData(param,poc)) return false;
	g_nTotalPer = 99;
	if(m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_DOMAININFOR) || 
		m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_DOMAININFOR1) || 
		m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_GSHOP) ||
		m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_GSHOP_TYPES) || 
		m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_GSHOP1) ||
		m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_GSHOP_TYPES1) || 
		m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_GSHOP2) ||
		m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_GSHOP_TYPES2))
		if(!ImportDomainData(param,poc)) return false;
	if(m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_TEXTFILE))
		if(!ImportTextFileData(param,poc)) return false;

	if(m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_HOMETOWN))
		if(!ImportHomeTownData(param, poc)) return false;
	
	return true;
}

bool CDataManager::ImportElementData(void *param, PROCESSPROC poc)
{
	g_MD5Hash.init();
	g_MD5Hash.Update("ZPWDATA", 7);
	DataPathMan pathman;

	if (pathman.CalculateMd5("data\\path.data") == 0)
	{
		AfxMessageBox(_T("Faild to load data\\path.data"));
		return false;
	}

	elementdataman dataMan;
 	if(-1==dataMan.load_data("data\\elements.data"))
	{
		AfxMessageBox(_T("Faild to load data\\elements.data"));
		return false;
	}
	
	if(pTM_ElementData==NULL)
			pTM_ElementData = new CTM_ElementData(CHUNK_ELEMENT_DATA);

	if(m_dwVersion < 0x10000006) m_dwVersion = LOCALIZE_VERSION;

	for( int i = 0; i < ID_SPACE_CONFIG + 1; ++i)
	{
		int nNum = dataMan.get_data_num((ID_SPACE)i);
		for(int j = 0; j < nNum; ++j)
		{
			int per = int(((float)j/(float)nNum) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;
			
			DATA_TYPE type;
			unsigned int id = dataMan.get_data_id((ID_SPACE)i,j,type);
			const void *pEDData = dataMan.get_data_ptr(id,(ID_SPACE)i,type);
			CTemplateBase * pTemplate = NULL;
			pTemplate = pTM_ElementData->FindTemplate(id,type);
			if(pTemplate != NULL)
			{
				if (!pTemplate->Import((void *)pEDData))
					return false;
			}else if( g_pElementTypes[type] )
			{
				pTemplate = g_pElementTypes[type]->Clone();
				if (!pTemplate->Import((void *)pEDData))
				{
					delete pTemplate;
					return false;
				}
				pTM_ElementData->AddTemplate(pTemplate);
			}
		}
	}
	return true;
}

bool CDataManager::ImportHomeTownData(void *param, PROCESSPROC poc)
{
	HOMETOWN::HometownTemplate *tpl = HOMETOWN::HometownTemplate::GetInstance();
	if(-1 == tpl->load_data("data\\hometowndata"))
		return false;
	if(!pTM_ElementHomeTownData)
		pTM_ElementHomeTownData = new CTM_HomeTownData(CHUNK_ELEMENT_HOMETOWN);

	char sz[32] = {0};
	for (int i=0; i<HOMETOWN::ID_SPACE_MAX; ++i)
	{
		HOMETOWN::DATA_TYPE datatype;
		unsigned int id = tpl->get_first_data_id((HOMETOWN::ID_SPACE)i, datatype);
		const void* pData = 0;
		const HOMETOWN::CROP_ESSENCE *crop = 0;
		const HOMETOWN::FRUIT_ESSENCE *fruit = 0;
		const HOMETOWN::TOOL_FUNC *tool_func = 0;
		const HOMETOWN::TOOL_ESSENCE *tool_ess = 0;	
		while (id)
		{		
			switch(datatype)
			{
			case HOMETOWN::DT_CROP_ESSENCE:
				tpl->get_data_ptr(id, (HOMETOWN::ID_SPACE)i, datatype, crop);
				pData = crop;
				break;
			case HOMETOWN::DT_FRUIT_ESSENCE:
				tpl->get_data_ptr(id, (HOMETOWN::ID_SPACE)i, datatype, fruit);
				pData = fruit;
				break;
			case HOMETOWN::DT_TOOL_FUNC:
				tpl->get_data_ptr(id, (HOMETOWN::ID_SPACE)i, datatype, tool_func);
				pData = tool_func;
				break;
			case HOMETOWN::DT_TOOL_ESSENCE:
				tpl->get_data_ptr(id, (HOMETOWN::ID_SPACE)i, datatype, tool_ess);
				pData = tool_ess;
				break;
			}

			CTemplateBase * pTemplate = NULL;
			pTemplate = pTM_ElementHomeTownData->FindTemplate(id,datatype);
			if(pTemplate != NULL)
			{
				pTemplate->Import((void *)pData);
			}else if( g_pHomeTownTypes[datatype] )
			{
				pTemplate = g_pHomeTownTypes[datatype]->Clone();
				pTemplate->Import((void *)pData);
				pTM_ElementHomeTownData->AddTemplate(pTemplate);
			}

			id = tpl->get_next_data_id((HOMETOWN::ID_SPACE)i, datatype);
		}
	}
	return true;
}

bool CDataManager::ImportTaskData(void *param, PROCESSPROC poc)
{
	ATaskTemplMan taskData;
	if(!taskData.LoadTasksFromPack("data\\tasks.data","data\\forbidden_task.txt", true))
	{
		AfxMessageBox(_T("Faild to load data\\tasks.data"));
		return false;
	}
	if(!taskData.LoadNPCInfoFromPack("data\\task_npc.data"))
	{
		AfxMessageBox(_T("Faild to load data\\task_npc.data"));
		return false;
	}

	
	if(pTM_ElementTask==NULL)
			pTM_ElementTask = new CTM_TaskData(CHUNK_ELEMENT_TASK);
	
	TaskTemplMap& taskMap = taskData.GetAllTemplMap();
	int nNum = taskMap.size();
	int j = 0;
	TaskTemplMap::iterator it = taskMap.begin();
	for( it; it != taskMap.end(); ++it)
	{
		int per = int(((float)j/(float)nNum) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;
		
		ATaskTempl * pTask = it->second;
		
		CTemplateBase * pTemplate = NULL;
		pTemplate = pTM_ElementTask->FindTemplate(pTask->GetID(),-1);
		if(pTemplate != NULL)
		{
			pTemplate->Import((void *)pTask);
		}else
		{
			pTemplate = new CTaskTemplate;
			pTemplate->Import((void *)pTask);
			pTM_ElementTask->AddTemplate(pTemplate);
		}
		++j;
	}
	taskData.Release();
	return true;
}

bool CDataManager::ImportItemDescData(void  *param, PROCESSPROC poc)
{
	CECStringTab strTab(CECStringTab::SET_ITEMDESC,g_szWorkDir);
	if(!strTab.Load()) return false;
	
	if(pTM_ElementItemDesc==NULL)
		pTM_ElementItemDesc = new CTM_OthersData(CHUNK_ELEMENT_ITEMDESC);

	int n = strTab.GetTextNum();
	for( int i = 0; i < n; ++i )
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;

		CECStringTab::TEXT *pText = strTab.GetText(i);
		
		CTemplateBase * pTemplate = NULL;
		pTemplate = pTM_ElementItemDesc->FindTemplate(pText->id,-1);
		if(pTemplate != NULL)
		{
			if(!pTemplate->Import((void *)pText)) return false;
		}else
		{
			CTemplate_ElementStringTab *pNewTempl = new CTemplate_ElementStringTab;
			pNewTempl->Import(pText);
			pTM_ElementItemDesc->AddTemplate(pNewTempl);
		}
	}

	return true;
}

bool CDataManager::ImportInstanceTextData(void  *param, PROCESSPROC poc)
{
	//instacne text
	CECInstanceText strInstance;
	if(!strInstance.Load("Configs\\instance.txt")) 
		return false;
	if(pTM_ElementInstaceText==NULL)
		pTM_ElementInstaceText = new CTM_InstanceData(CHUNK_ELEMENT_INSTANCE_TEXT);

	int n = strInstance.GetTextNum();
	for(int i = 0; i < n; ++i )
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;

		CECInstanceText::_INSTANCE_TEXT *pText = strInstance.GetText(i);
		
		CTemplateBase * pTemplate = NULL;
		pTemplate = pTM_ElementInstaceText->FindTemplate(pText->pText->id,-1);
		if(pTemplate != NULL)
		{
			if(!pTemplate->Import((void *)pText)) return false;
		}else
		{
			CTemplate_ElementInstance *pNewTempl = new CTemplate_ElementInstance;
			pNewTempl->Import(pText);
			pTM_ElementInstaceText->AddTemplate(pNewTempl);
		}
	}
	return true;
}

bool CDataManager::ImportItemExDescData(void  *param, PROCESSPROC poc)
{
	CECStringTab strTab(CECStringTab::SET_ITEMEXTDESC,g_szWorkDir);
	if(!strTab.Load()) return false;
	
	if(pTM_ElementItemExDesc==NULL)
			pTM_ElementItemExDesc = new CTM_OthersData(CHUNK_ELEMENT_ITEMEXDESC);

	int n = strTab.GetTextNum();
	for( int i = 0; i < n; ++i )
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;
		
		CECStringTab::TEXT *pText = strTab.GetText(i);
		
		CTemplateBase * pTemplate = NULL;
		pTemplate = pTM_ElementItemExDesc->FindTemplate(pText->id,-1);
		if(pTemplate != NULL)
		{
			if(!pTemplate->Import((void *)pText)) return false;
		}else
		{
			CTemplate_ElementStringTab *pNewTempl = new CTemplate_ElementStringTab;
			pNewTempl->Import(pText);
			pTM_ElementItemExDesc->AddTemplate(pNewTempl);
		}
	}
	return true;
}
bool CDataManager::ImportItemSkillDescData(void  *param, PROCESSPROC poc)
{
	CECStringTab strTab(CECStringTab::SET_SKILLDESC,g_szWorkDir);
	if(!strTab.Load()) return true;
	
	if(pTM_ElementSkillDesc==NULL)
			pTM_ElementSkillDesc = new CTM_OthersData(CHUNK_ELEMENT_SKILLDESC);

	int n = strTab.GetTextNum();
	for( int i = 0; i < n; ++i )
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;
		CECStringTab::TEXT *pText = strTab.GetText(i);
		
		CTemplateBase * pTemplate = NULL;
		pTemplate = pTM_ElementSkillDesc->FindTemplate(pText->id,-1);
		if(pTemplate != NULL)
		{
			if(!pTemplate->Import((void *)pText)) return false;
		}else
		{
			CTemplate_ElementStringTab *pNewTempl = new CTemplate_ElementStringTab;
			pNewTempl->Import(pText);
			pTM_ElementSkillDesc->AddTemplate(pNewTempl);
		}
	}
	return true;
}
bool CDataManager::ImportItemFixMsgData(void  *param, PROCESSPROC poc)
{
	CECStringTab strTab(CECStringTab::SET_FIXMSG,g_szWorkDir);
	if(!strTab.Load()) return false;
	
	if(pTM_ElementFixMsg==NULL)
			pTM_ElementFixMsg = new CTM_MessageData(CHUNK_ELEMENT_FIXMSG);

	int n = strTab.GetTextNum();
	for( int i = 0; i < n; ++i )
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;
		
		CECStringTab::TEXT *pText = strTab.GetText(i);
		
		CTemplateBase * pTemplate = NULL;
		pTemplate = pTM_ElementFixMsg->FindTemplate(pText->id,-1);
		if(pTemplate != NULL)
		{
			if(!pTemplate->Import((void *)pText)) return false;
		}else
		{
			CTemplate_ElementMessage *pNewTempl = new CTemplate_ElementMessage;
			pNewTempl->Import(pText);
			pTM_ElementFixMsg->AddTemplate(pNewTempl);
		}
	}
	return true;
}
bool CDataManager::ImportItemServerErrData(void  *param, PROCESSPROC poc)
{
	CECStringTab strTab(CECStringTab::SET_SERVERERR,g_szWorkDir);
	if(!strTab.Load()) return false;
	
	if(pTM_ElementServerErr==NULL)
			pTM_ElementServerErr = new CTM_MessageData(CHUNK_ELEMENT_SERVERERR);

	int n = strTab.GetTextNum();
	for( int i = 0; i < n; ++i )
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;

		CECStringTab::TEXT *pText = strTab.GetText(i);
		
		CTemplateBase * pTemplate = NULL;
		pTemplate = pTM_ElementServerErr->FindTemplate(pText->id,-1);
		if(pTemplate != NULL)
		{
			if(!pTemplate->Import((void *)pText)) return false;
		}else
		{
			CTemplate_ElementMessage *pNewTempl = new CTemplate_ElementMessage;
			pNewTempl->Import(pText);
			pTM_ElementServerErr->AddTemplate(pNewTempl);
		}
	}
	return true;
}

bool CDataManager::ImportItemBuffDescData(void  *param, PROCESSPROC poc)
{
	CECStringTab strTab(CECStringTab::SET_BUFFDESC,g_szWorkDir);
	if(!strTab.Load()) return false;
	
	if(pTM_ElementBuffDesc==NULL)
			pTM_ElementBuffDesc = new CTM_MessageData(CHUNK_ELEMENT_BUFFDESC);

	int n = strTab.GetTextNum();
	for( int i = 0; i < n; ++i )
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;

		CECStringTab::TEXT *pText = strTab.GetText(i);
		
		CTemplateBase * pTemplate = NULL;
		pTemplate = pTM_ElementBuffDesc->FindTemplate(pText->id,-1);
		if(pTemplate != NULL)
		{
			if(!pTemplate->Import((void *)pText)) return false;
		}else
		{
			CTemplate_ElementMessage *pNewTempl = new CTemplate_ElementMessage;
			pNewTempl->Import(pText);
			pTM_ElementBuffDesc->AddTemplate(pNewTempl);
		}
	}
	return true;
}

bool CDataManager::ImportItemTaskErrData(void  *param, PROCESSPROC poc)
{
	CECStringTab strTab(CECStringTab::SET_TASKERROR,g_szWorkDir);
	if(!strTab.Load()) return false;
	
	if(pTM_ElementTaskErr==NULL)
			pTM_ElementTaskErr = new CTM_MessageData(CHUNK_ELEMENT_TASKERR);

	int n = strTab.GetTextNum();
	for( int i = 0; i < n; ++i )
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;

		CECStringTab::TEXT *pText = strTab.GetText(i);
		
		CTemplateBase * pTemplate = NULL;
		pTemplate = pTM_ElementTaskErr->FindTemplate(pText->id,-1);
		if(pTemplate != NULL)
		{
			if(!pTemplate->Import((void *)pText)) return false;
		}else
		{
			CTemplate_ElementMessage *pNewTempl = new CTemplate_ElementMessage;
			pNewTempl->Import(pText);
			pTM_ElementTaskErr->AddTemplate(pNewTempl);
		}
	}
	return true;
}

bool CDataManager::ImportItemLoadingHintData(void  *param, PROCESSPROC poc)
{
	CECStringTab strTab(CECStringTab::SET_LOADINGHINT,g_szWorkDir);
	if(!strTab.Load()) return false;
	
	if(pTM_ElementLoadingHint==NULL)
			pTM_ElementLoadingHint = new CTM_MessageData(CHUNK_ELEMENT_LOADINGHINT);

	int n = strTab.GetTextNum();
	for( int i = 0; i < n; ++i )
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;

		CECStringTab::TEXT *pText = strTab.GetText(i);
		
		CTemplateBase * pTemplate = NULL;
		pTemplate = pTM_ElementLoadingHint->FindTemplate(pText->id,-1);
		if(pTemplate != NULL)
		{
			if(!pTemplate->Import((void *)pText)) return false;
		}else
		{
			CTemplate_ElementMessage *pNewTempl = new CTemplate_ElementMessage;
			pNewTempl->Import(pText);
			pTM_ElementLoadingHint->AddTemplate(pNewTempl);
		}
	}
	return true;
}

bool CDataManager::ImportItemPetTalkData(void  *param, PROCESSPROC poc)
{
	CECStringTab strTab(CECStringTab::SET_PETTALK,g_szWorkDir);
	if(!strTab.Load()) return false;
	
	if(pTM_ElementPetTalk==NULL)
			pTM_ElementPetTalk = new CTM_MessageData(CHUNK_ELEMENT_PETTALK);

	int n = strTab.GetTextNum();
	for( int i = 0; i < n; ++i )
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;

		CECStringTab::TEXT *pText = strTab.GetText(i);
		
		CTemplateBase * pTemplate = NULL;
		pTemplate = pTM_ElementPetTalk->FindTemplate(pText->id,-1);
		if(pTemplate != NULL)
		{
			if(!pTemplate->Import((void *)pText)) return false;
		}else
		{
			CTemplate_ElementMessage *pNewTempl = new CTemplate_ElementMessage;
			pNewTempl->Import(pText);
			pTM_ElementPetTalk->AddTemplate(pNewTempl);
		}
	}
	return true;
}

bool CDataManager::ImportItemAddonStrData(void  *param, PROCESSPROC poc)
{
	CECStringTab strTab(CECStringTab::SET_ADDONSTR,g_szWorkDir);
	if(!strTab.Load()) return false;
	
	if(pTM_ElementAddonStr==NULL)
		pTM_ElementAddonStr = new CTM_MessageData(CHUNK_ELEMENT_ADDONSTR);

	int n = strTab.GetTextNum();
	for( int i = 0; i < n; ++i )
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;

		CECStringTab::TEXT *pText = strTab.GetText(i);
		
		CTemplateBase * pTemplate = NULL;
		pTemplate = pTM_ElementAddonStr->FindTemplate(pText->id,-1);
		if(pTemplate != NULL)
		{
			if(!pTemplate->Import((void *)pText)) return false;
		}else
		{
			CTemplate_ElementMessage *pNewTempl = new CTemplate_ElementMessage;
			pNewTempl->Import(pText);
			pTM_ElementAddonStr->AddTemplate(pNewTempl);
		}
	}
	return true;
}

bool CDataManager::ImportInterfaceString(void *param, PROCESSPROC poc, CElementInterfaceManager* pinter)
{
	if (!pinter)
		return false;

	CElementInterfaceManager& inter = *pinter;
	if(pTM_ElementInterfaceString==NULL)
		pTM_ElementInterfaceString = new CTM_InterfaceStringData(CHUNK_ELEMENT_INTERFACESTRING);
	InterfaceFileNames* fileNames = inter.GetStringTableFiles();
	
	int n = fileNames->size();
	fileNames = inter.GetStringTableFiles();
	for( int i = 0; i < n; ++i )
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;

		ACString name = (*fileNames)[i];
		StringTableFile* tableFile = inter.GetStringTable(name);
		if(tableFile==NULL) continue;
		StringTableFile::iterator it = tableFile->begin();
		for( ; it != tableFile->end(); ++it)
		{
			int sid = it->first;
			ACString text = it->second;
			CString xxx(text);
			if(4==xxx.Replace(_T("###"),_T(" ")) || sid == -1)
			{//这儿我是想屏蔽掉字体字符串
				continue;
			}
			
			INTER_STRING data;
			data.id = sid;
			lstrcpy(data.szIndex,name);
			lstrcpy(data.szText,text);
			
			CTemplateBase * pTemplate = NULL;
			pTemplate = ((CTM_InterfaceStringData*)pTM_ElementInterfaceString)->FindTemplate(data.szIndex,data.id);
			if(pTemplate != NULL)
			{
				if(!pTemplate->Import(&data)) return false;
			}else
			{
				CTemplate_ElementInterfaceString *pNewTempl = new CTemplate_ElementInterfaceString;
				pNewTempl->Import(&data);
				pTM_ElementInterfaceString->AddTemplate(pNewTempl);
			}
		}
	}

	return true;
}

bool CDataManager::ImportInterfaceHint(void *param, PROCESSPROC poc, CElementInterfaceManager* pinter)
{
	if (!pinter)
		return false;

	CElementInterfaceManager& inter = *pinter;
	if(pTM_ElementInterfaceHint==NULL)
		pTM_ElementInterfaceHint = new CTM_InterfaceHintData(CHUNK_ELEMENT_INTERFACEHINT);
	InterfaceFileNames* fileNames = inter.GetHintTableFiles();
	
	int n = fileNames->size();
	for( int i = 0; i < n; ++i )
	{
		int per = int(((float)i/(float)n) * 100); 
		if(!(*poc)(param,per,g_nTotalPer)) return false;
		
		ACString name = (*fileNames)[i];
		HintTableFile* tableFile = inter.GetHintTable(name);
		if(tableFile==NULL) continue;
		HintTableFile::iterator it = tableFile->begin();
		for( ; it != tableFile->end(); ++it)
		{
			ACString index = it->first;
			ACString text = it->second;
			INTER_HINT data;
			lstrcpy(data.szText,text);
			_stprintf(data.szIndex,_T("%s-%s"),name,index);
			
			CTemplateBase * pTemplate = NULL;
			pTemplate = ((CTM_InterfaceHintData*)pTM_ElementInterfaceHint)->FindTemplate(data.szIndex);
			if(pTemplate != NULL)
			{
				if(!pTemplate->Import(&data)) return false;
			}else
			{
				CTemplate_ElementInterfaceHint *pNewTempl = new CTemplate_ElementInterfaceHint;
				pNewTempl->Import(&data);
				pTM_ElementInterfaceHint->AddTemplate(pNewTempl);
			}
		}
	}

	return true;
}

bool CDataManager::ImportHelpScript(void *param, PROCESSPROC poc, CElementInterfaceManager* pinter)
{
	if (!pinter)
		return false;

	CElementInterfaceManager& inter = *pinter;
	if(pTM_ElementHelpScript == NULL)
		pTM_ElementHelpScript = new CTM_InterfaceHintData(CHUNK_ELEMENT_HELPSCRIPT);

	InterfaceFileNames* fileNames = inter.GetHelpScriptFiles();
	int n = fileNames->size();
	for( int i = 0; i < n; ++i )
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;
		
		ACString name = (*fileNames)[i];
		HintTableFile* tableFile = inter.GetHelpScriptTable(name);
		if(tableFile==NULL) continue;
		HintTableFile::iterator it = tableFile->begin();
		for( ; it != tableFile->end(); ++it)
		{
			ACString index = it->first;
			ACString text = it->second;
			INTER_HINT data;
			lstrcpy(data.szText,text);
			_stprintf(data.szIndex,_T("%s-%s"),name,index);
			
			CTemplateBase * pTemplate = NULL;
			pTemplate = ((CTM_InterfaceHintData*)pTM_ElementHelpScript)->FindTemplate(data.szIndex);
			if(pTemplate != NULL)
			{
				if(!pTemplate->Import(&data)) return false;
			}else
			{
				CTemplate_ElementInterfaceHint *pNewTempl = new CTemplate_ElementInterfaceHint;
				pNewTempl->Import(&data);
				pTM_ElementHelpScript->AddTemplate(pNewTempl);
			}
		}
	}

	return true;
}

bool CDataManager::ImportHelpText(void *param, PROCESSPROC poc, CElementInterfaceManager* pinter)
{
	if (!pinter)
		return false;

	CElementInterfaceManager& inter = *pinter;
	if (!pTM_ElementHelpText)
		pTM_ElementHelpText = new CTM_InterfaceHintData(CHUNK_ELEMENT_HELPTEXT);
	InterfaceFileNames* fileNames = inter.GetHelpTextFiles();
	int n = fileNames->size();
	for( int i = 0; i < n; ++i )
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;
		
		ACString name = (*fileNames)[i];
		HintTableFile* tableFile = inter.GetHelpTextTable(name);
		if(tableFile==NULL) continue;
		HintTableFile::iterator it = tableFile->begin();
		for( ; it != tableFile->end(); ++it)
		{
			ACString index = it->first;
			ACString text = it->second;
			INTER_HINT data;
			lstrcpy(data.szText,text);
			_stprintf(data.szIndex,_T("%s-%s"),name,index);
			
			int len = a_strlen(data.szText);
			
			CTemplateBase * pTemplate = NULL;
			pTemplate = ((CTM_InterfaceHintData*)pTM_ElementHelpText)->FindTemplate(data.szIndex);
			if(pTemplate != NULL)
			{
				if(!pTemplate->Import(&data)) return false;
			}else
			{
				CTemplate_ElementInterfaceHint *pNewTempl = new CTemplate_ElementInterfaceHint;
				pNewTempl->Import(&data);
				pTM_ElementHelpText->AddTemplate(pNewTempl);
			}
		}
	}

	return true;
}

bool CDataManager::ImportInterfaceData(void  *param, PROCESSPROC poc)
{
	CElementInterfaceManager inter(g_szWorkDir);
	if(!inter.LoadInterface()) return false;
	
	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_INTERFACEHINT))
	{
		if (!ImportInterfaceHint(param, poc, &inter))
			return false;
	}

	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_INTERFACESTRING))
	{
		if (!ImportInterfaceString(param, poc, &inter))
			return false;
	}
	
	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_HELPSCRIPT))
	{
		if (!ImportHelpScript(param, poc, &inter))
			return false;
	}

	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_HELPTEXT))
	{
		if (!ImportHelpText(param, poc, &inter))
			return false;
	}

	return true;
}

bool CDataManager::ImportTextFileData( void *param,  PROCESSPROC poc)
{
	CTextFileMgr inter(g_szWorkDir);
	if(!inter.LoadData()) return false;
	
	if(pTM_ElementTextFile == NULL)
		pTM_ElementTextFile = new CTM_TextFile(CHUNK_ELEMENT_TEXTFILE);

	FILE_DATA_MAP* fileMap = inter.GetFileDataMap();
	FILE_DATA_MAP::iterator it = fileMap->begin();
	for( ; it != fileMap->end(); ++it)
	{
		ACString index = it->first;
		ACString text = it->second;
		FILE_DATA data;
		lstrcpy(data.szText,text);
		lstrcpy(data.szIndex,index);
		CTemplateBase * pTemplate = NULL;
		pTemplate = ((CTM_TextFile*)pTM_ElementTextFile)->FindTemplate(data.szIndex);
		if(pTemplate != NULL)
		{
			if(!pTemplate->Import(&data)) return false;
		}else
		{
			CTemplate_ElementTextFile *pNewTempl = new CTemplate_ElementTextFile;
			pNewTempl->Import(&data);
			pTM_ElementTextFile->AddTemplate(pNewTempl);
		}
	}
	return true;
}

bool CDataManager::ImportPolicyData(void *param, PROCESSPROC poc)
{
	CPolicyDataManager pdMan;
	if(!pdMan.Load("data\\aipolicy.data")) 
	{
		g_Log.Log("Faild to load data\\aipolicy.data");
		AfxMessageBox(_T("Faild to load data\\aipolicy.data"),MB_OK);
		return false;
	}

	if(pTM_ElementPolicy==NULL) pTM_ElementPolicy = new CTM_PolicyData(CHUNK_ELEMENT_POLICY);

	int n = pdMan.GetPolicyNum();
	for( int i = 0; i < n; ++i)
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;

		CPolicyData *pData = pdMan.GetPolicy(i);	
		unsigned int uid = pData->GetID();
		CTemplateBase *pOldTempl = pTM_ElementPolicy->FindTemplate(uid,-1);
		if(pOldTempl!=NULL)
		{
			if(!pOldTempl->Import(pData)) return false;
		}else
		{
			CTemplateBase *pNewTempl = new CPolicyTemplate;
			if(!pNewTempl->Import(pData)) return false;
			pTM_ElementPolicy->AddTemplate(pNewTempl);
		}
	}
	pdMan.Release();
	return true;	
}

bool CDataManager::ImportPrecinctData(void *param, PROCESSPROC poc)
{
	if(pTM_ElementPrecinct==NULL) pTM_ElementPrecinct = new CTM_PrecinctData(CHUNK_ELEMENT_PRECINCT);
	CString strWildcard;
	strWildcard.Format(_T("%s\\Maps"),AS2AC(g_szWorkDir));
	if(!EnumImpPrecinct(strWildcard)) return false;
	
	return true;
}

/*
bool CDataManager::ImportTreasureData(void *param, PROCESSPROC poc)
{

	CTreasureMan pdMan;
	if(!pdMan.LoadL("data\\gshop.data")) 
	{
		g_Log.Log("Faild to load data\\gshop.data");
		return false;
	}

	if(pTM_ElementTreasure==NULL) pTM_ElementTreasure = new CTM_TreasureData;

	int n = pdMan.GetTreasureCount();
	for( int i = 0; i < n; ++i)
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;

		TREASURE_ITEM *pData = pdMan.GetTreasure(i);	
		unsigned int uid = pData->id;
		CTemplateBase *pOldTempl = pTM_ElementTreasure->FindTemplate(uid,-1);
		if(pOldTempl!=NULL)
		{
			if(!pOldTempl->Import(pData)) return false;
		}else
		{
			CTemplateBase *pNewTempl = new CTreasureTemplate;
			if(!pNewTempl->Import(pData)) return false;
			pTM_ElementTreasure->AddTemplate(pNewTempl);
		}
	}
	pdMan.Release();
	return true;
}
*/

bool CDataManager::_ImportDomainInfo(void *param, PROCESSPROC poc)
{
	vector<DOMAIN_INFO> * pListDomain = globaldata_getdomaininfos();
	if(pTM_ElementDomainInfor==NULL) pTM_ElementDomainInfor = new CTM_DomainInforData(CHUNK_ELEMENT_DOMAININFOR);

	int n = pListDomain->size();
	for(int i = 0; i < n; ++i)
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;

		DOMAIN_INFO data = (*pListDomain)[i];	
		unsigned int uid = data.id;
		CTemplateBase *pOldTempl = pTM_ElementDomainInfor->FindTemplate(uid,-1);
		if(pOldTempl!=NULL)
		{
			if(!pOldTempl->Import(&data)) return false;
		}else
		{
			CTemplateBase *pNewTempl = new CDomainInforTemplate;
			if(!pNewTempl->Import(&data)) return false;
			pTM_ElementDomainInfor->AddTemplate(pNewTempl);
		}
	}

	return true;
}

bool CDataManager::_ImportDomainInfo1(void *param, PROCESSPROC poc)
{
	vector<DOMAIN_INFO> * pListDomain = globaldata_getdomaininfos2();
	if(pTM_ElementDomainInfor1==NULL) pTM_ElementDomainInfor1 = new CTM_DomainInforData(CHUNK_ELEMENT_DOMAININFOR1);

	int n = pListDomain->size();
	for(int i = 0; i < n; ++i)
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;

		DOMAIN_INFO data = (*pListDomain)[i];	
		unsigned int uid = data.id;
		CTemplateBase *pOldTempl = pTM_ElementDomainInfor1->FindTemplate(uid,-1);
		if(pOldTempl!=NULL)
		{
			if(!pOldTempl->Import(&data)) return false;
		}else
		{
			CTemplateBase *pNewTempl = new CDomainInforTemplate;
			if(!pNewTempl->Import(&data)) return false;
			pTM_ElementDomainInfor1->AddTemplate(pNewTempl);
		}
	}

	return true;
}

bool CDataManager::_ImportGshop(void *param, PROCESSPROC poc)
{
	vector<GSHOP_ITEM> * pListGShop = globaldata_getgshopitems();
	if(pTM_ElementGShop==NULL) pTM_ElementGShop = new CTM_GShopData(CHUNK_ELEMENT_GSHOP);

	int n = pListGShop->size();
	for( int i = 0; i < n; ++i)
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;

		GSHOP_ITEM data = (*pListGShop)[i];	
		unsigned int uid;
		
		if(m_dwVersion > 7)//解决ID错误。
			uid = data.local_id;
		else uid = data.id;

		CTemplateBase *pOldTempl = pTM_ElementGShop->FindTemplate(uid,-1);
		if(pOldTempl!=NULL)
		{
			if(!pOldTempl->Import(&data)) return false;
		}else
		{
			CTemplateBase *pNewTempl = new CGShopTemplate;
			if(!pNewTempl->Import(&data)) return false;
			pTM_ElementGShop->AddTemplate(pNewTempl);
		}
	}
	m_dwVersion = LOCALIZE_VERSION;

	return true;
}

bool CDataManager::_ImportGshopTypes(void *param, PROCESSPROC poc)
{
	vector<GSHOP_MAIN_TYPE> * pListGShopMainTypes = globaldata_getgshopmaintypes();
	if(pTM_ElementGShopTypes==NULL) pTM_ElementGShopTypes = new CTM_GShopTypes(CHUNK_ELEMENT_GSHOP_TYPES);

	int n = pListGShopMainTypes->size();
	for( int i = 0; i < n; ++i)
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;

		GSHOP_MAIN_TYPE_PARAMS data;
		unsigned int uid = i;
		
		data.id = uid;
		data.pMainType = &(*pListGShopMainTypes)[i];
		
		CTemplateBase *pOldTempl = pTM_ElementGShopTypes->FindTemplate(uid,-1);
		if(pOldTempl!=NULL)
		{
			if(!pOldTempl->Import(&data)) return false;
		}else
		{
			CTemplateBase *pNewTempl = new CGShopTypesTemplate;
			if(!pNewTempl->Import(&data)) return false;
			pTM_ElementGShopTypes->AddTemplate(pNewTempl);
		}
	}

	return true;
}

bool CDataManager::_ImportGshop1(void *param, PROCESSPROC poc)
{
	vector<GSHOP_ITEM> * pListGShop1 = globaldata_getgshopitems2();
	if(pTM_ElementGShop1==NULL) pTM_ElementGShop1 = new CTM_GShopData(CHUNK_ELEMENT_GSHOP1);

	int n2 = pListGShop1->size();
	for( int i = 0; i < n2; ++i)
	{
		int per = int(((float)i/(float)n2) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;

		GSHOP_ITEM data = (*pListGShop1)[i];
		unsigned int uid;
		
		if(m_dwVersion > 7)//解决ID错误。
			uid = data.local_id;
		else uid = data.id;

		CTemplateBase *pOldTempl = pTM_ElementGShop1->FindTemplate(uid,-1);
		if(pOldTempl!=NULL)
		{
			if(!pOldTempl->Import(&data)) return false;
		}else
		{
			CTemplateBase *pNewTempl = new CGShopTemplate;
			if(!pNewTempl->Import(&data)) return false;
			pTM_ElementGShop1->AddTemplate(pNewTempl);
		}
	}

	if(m_dwVersion <= 7) m_dwVersion = LOCALIZE_VERSION;
	return true;
}

bool CDataManager::_ImportGshop1Types(void *param, PROCESSPROC poc)
{
	vector<GSHOP_MAIN_TYPE> * pListGShopMainTypes1 = globaldata_getgshopmaintypes2();
	if(pTM_ElementGShopTypes1==NULL) pTM_ElementGShopTypes1 = new CTM_GShopTypes(CHUNK_ELEMENT_GSHOP_TYPES1);

	int n2 = pListGShopMainTypes1->size();
	for(int i = 0; i < n2; ++i)
	{
		int per = int(((float)i/(float)n2) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;

		GSHOP_MAIN_TYPE_PARAMS data;
		unsigned int uid = i;
		
		data.id = uid;
		data.pMainType = &(*pListGShopMainTypes1)[i];
		
		CTemplateBase *pOldTempl = pTM_ElementGShopTypes1->FindTemplate(uid,-1);
		if(pOldTempl!=NULL)
		{
			if(!pOldTempl->Import(&data)) return false;
		}else
		{
			CTemplateBase *pNewTempl = new CGShopTypesTemplate;
			if(!pNewTempl->Import(&data)) return false;
			pTM_ElementGShopTypes1->AddTemplate(pNewTempl);
		}
	}

	return true;
}

bool CDataManager::_ImportGshop2(void *param, PROCESSPROC poc)
{
	vector<GSHOP_ITEM> * pListGShop2 = globaldata_getgshopitems3();
	if(pTM_ElementGShop2==NULL) pTM_ElementGShop2 = new CTM_GShopData(CHUNK_ELEMENT_GSHOP2);

	int n3 = pListGShop2->size();
	for( int i = 0; i < n3; ++i)
	{
		int per = int(((float)i/(float)n3) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;

		GSHOP_ITEM data = (*pListGShop2)[i];
		unsigned int uid;
		
		if(m_dwVersion > 7)//解决ID错误。
			uid = data.local_id;
		else uid = data.id;

		CTemplateBase *pOldTempl = pTM_ElementGShop2->FindTemplate(uid,-1);
		if(pOldTempl!=NULL)
		{
			if(!pOldTempl->Import(&data)) return false;
		}else
		{
			CTemplateBase *pNewTempl = new CGShopTemplate;
			if(!pNewTempl->Import(&data)) return false;
			pTM_ElementGShop2->AddTemplate(pNewTempl);
		}
	}

	if(m_dwVersion <= 7) m_dwVersion = LOCALIZE_VERSION;
	return true;
}

bool CDataManager::_ImportGshop2Types(void *param, PROCESSPROC poc)
{
	vector<GSHOP_MAIN_TYPE> * pListGShopMainTypes2 = globaldata_getgshopmaintypes3();
	if(pTM_ElementGShopTypes2==NULL) pTM_ElementGShopTypes2 = new CTM_GShopTypes(CHUNK_ELEMENT_GSHOP_TYPES2);

	int n3 = pListGShopMainTypes2->size();
	for(int i = 0; i < n3; ++i)
	{
		int per = int(((float)i/(float)n3) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;

		GSHOP_MAIN_TYPE_PARAMS data;
		unsigned int uid = i;
		
		data.id = uid;
		data.pMainType = &(*pListGShopMainTypes2)[i];
		
		CTemplateBase *pOldTempl = pTM_ElementGShopTypes2->FindTemplate(uid,-1);
		if(pOldTempl!=NULL)
		{
			if(!pOldTempl->Import(&data)) return false;
		}else
		{
			CTemplateBase *pNewTempl = new CGShopTypesTemplate;
			if(!pNewTempl->Import(&data)) return false;
			pTM_ElementGShopTypes2->AddTemplate(pNewTempl);
		}
	}

	return true;
}

bool CDataManager::ImportDomainData(void *param, PROCESSPROC poc)
{
	if(!globaldata_load()) 
	{
		AfxMessageBox(_T("globaldata_load(), fuction returns an error!"),MB_OK);
		return false;
	}
	
	bool bSuc = false;
	do {
	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_GSHOP) && !_ImportGshop(param, poc))
		break;
	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_GSHOP1) && !_ImportGshop1(param, poc))
		break;
	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_GSHOP2) && !_ImportGshop2(param, poc))
		break;
	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_GSHOP_TYPES) && !_ImportGshopTypes(param, poc))
		break;
	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_GSHOP_TYPES1) && !_ImportGshop1Types(param, poc))
		break;
	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_GSHOP_TYPES2) && !_ImportGshop2Types(param, poc))
		break;
	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_DOMAININFOR) && !_ImportDomainInfo(param, poc))
		break;
	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_DOMAININFOR1) && !_ImportDomainInfo1(param, poc))
		break;
	bSuc = true;
	} while(0);
	globaldata_release();
	
	return bSuc;
}


bool CDataManager::EnumImpPrecinct(CString path)
{
	CString fileName,ext;
	CString strWildcard = path;
	strWildcard +=_T("\\*.*");
	
	WIN32_FIND_DATAA dat;
	HANDLE hd;
	hd = FindFirstFileA(w_to_c(strWildcard),&dat);
	if(hd!=NULL)
	{
		do
		{
			if(dat.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{//paths
				if(stricmp("..",dat.cFileName)==0 || stricmp(".",dat.cFileName)==0) continue;
				CString tt = path;
				tt.Format(_T("%s\\%s"),path,c_to_w(dat.cFileName));
				if(!EnumImpPrecinct(tt)) return false;
				continue;
			}else if(dat.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
			{//files
				ext = c_to_w(dat.cFileName);
				fileName.Format(_T("%s\\%s"),path,c_to_w(dat.cFileName));
				ext.MakeLower();
				if(_tcsicmp(ext,_T("precinct.clt"))==0)
				{
					char szPathName[MAX_PATH];
					fileName.Replace(c_to_w(g_szWorkDir),_T(""));
					sprintf(szPathName,"%s",w_to_c(fileName));
					CPrecinctTemplate *pOldTempl = (CPrecinctTemplate*) ((CTM_PrecinctData*)pTM_ElementPrecinct)->FindTemplate(szPathName);
					if(pOldTempl==NULL)
					{
						CPrecinctTemplate *pNewTempl = new CPrecinctTemplate;
						if(!pNewTempl->Import(szPathName)) 
						{
							delete pNewTempl;
							return false;
						}
						pTM_ElementPrecinct->AddTemplate(pNewTempl);
					}else
					{
						if(!pOldTempl->Import(szPathName)) return false;
					}
				}
			}
		}while(FindNextFileA(hd,&dat));
		FindClose(hd);
	}
	return true;
}


//------------------------------------------------------------------------------------------------

bool CDataManager::EnumTemplateImport(CTemplateBase *pOldTempl, CTemplateBase *pNewTempl, CSelectTransDlg *pDlg)
{
	int m = pNewTempl->GetTextNum();
	for( int j = 0; j < m; ++j)
	{
		CTextBase *pNewText = pNewTempl->GetTextByIdx(j);
		if(pNewText==NULL) continue;
		
		CTextBase *pOldText = NULL;
		if(!pNewText->IsIdVaild()) 
		{
			if(j < pOldTempl->GetTextNum()) pOldText = pOldTempl->GetTextByIdx(j);
		}else
		{
			for( int i = 0; i < pOldTempl->GetTextNum(); ++i)
			{
				
				CTextBase * pTextTemp = pOldTempl->GetTextByIdx(i);
				if(pTextTemp->GetID() == pNewText->GetID() && pTextTemp->GetParam() == pNewText->GetParam())
				{
					pOldText = pTextTemp;
					break;
				}
			}
		}
		
		if(pOldText)
		{
			if (!pOldText->GetOriginalText() || !pNewText->GetOriginalText())
				continue;

			if (!CTextBase::DataIsEqual((char*)pOldText->GetOriginalText(), _tcslen(pOldText->GetOriginalText()) * sizeof(TCHAR),
										(char*)pNewText->GetOriginalText(), _tcslen(pNewText->GetOriginalText()) * sizeof(TCHAR))
				)
			{
				continue;
			}
			
			if(pNewText->GetTranslationText()==NULL) continue;
			if(pOldText->GetTranslationText()==NULL)
			{
				pOldText->ReplaceTransText(pNewText->GetTranslationText());
				pOldText->SetStatus(pNewText->GetStatus());
				continue;
			}
			
			if(0==_tcscmp(pNewText->GetTranslationText(),pOldText->GetTranslationText()))
				continue;
			else
			{	
				CString str1(pNewText->GetTranslationText());
				CString str2(pOldText->GetTranslationText());
				if(str1.IsEmpty() && !str2.IsEmpty())
					continue;
				
				if(!str1.IsEmpty() && str2.IsEmpty())
				{
					pOldText->ReplaceTransText(pNewText->GetTranslationText());
					pOldText->SetStatus(pNewText->GetStatus());
					continue;
				}
				
				//让用户选择保存那一个译文
				if(!pDlg->IsApplyAll())
				{
					pDlg->SetData(pOldText->GetOriginalText(),pNewText->GetOriginalText(),pOldText->GetTranslationText(),pNewText->GetTranslationText());
					pDlg->DoModal();
				}
				
				if(!pDlg->IsSelectedOne())
				{
					pOldText->ReplaceTransText(pNewText->GetTranslationText());
					if (pDlg->IsReplaceOrg())
						pOldText->ReplaceOriginalText(pNewText->GetOriginalText());
					pOldText->SetStatus(pNewText->GetStatus());
				}
			}
		}else
		{//需要把这个新数据加入到旧的模板中
			pOldTempl->AddText(pNewText);
			pNewTempl->SetTextToNull(j);
		}
	}
	
	//Chidren
	m = pNewTempl->GetChildNum();
	for( j = 0; j < m; ++j)
	{
		CTemplateBase *pChildNew = pNewTempl->GetChildByIndex(j);
		CTemplateBase *pChildOld = pOldTempl->GetChildById(pChildNew->GetID());
		if(pChildOld) 
		{
			if(!EnumTemplateImport(pChildOld,pChildNew,pDlg)) return false;
		}else 
		{
			pNewTempl->SetChildToNull(j);
			pOldTempl->AddChild(pChildNew);
		}
	}
	return true;
}

bool CDataManager::ImportElementData_M(CTemplateManager* pMan, CSelectTransDlg *pDlg, void *param, PROCESSPROC poc)
{
	if(pMan==NULL) return true;
	int n  = pMan->GetTempalteNum();
	if(n<=0) return true;
	if(pTM_ElementData==NULL) pTM_ElementData = new CTM_ElementData(CHUNK_ELEMENT_DATA);
	
	for(int i = 0; i < n; ++i)
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;

		CTemplateBase *pNewTemp = pMan->GetTemplateByIdx(i);
		CTemplateBase *pOldTemp = pTM_ElementData->FindTemplate(pNewTemp->GetID(),pNewTemp->GetTemplateType());
		if(pOldTemp == NULL) //新加入到原数据中
		{
			pTM_ElementData->AddTemplate(pNewTemp);
			pMan->SetTemplateToNull(i);//这样就不用再复制一份数据了
		}
		else
			if(!EnumTemplateImport(pOldTemp,pNewTemp,pDlg)) return false;
	}
	return true;
}

bool CDataManager::ImportHomeTownData_M(CTemplateManager* pMan, CSelectTransDlg *pDlg, void *param, PROCESSPROC poc)
{
	if(pMan==NULL) return true;
	int n  = pMan->GetTempalteNum();
	if(n<=0) return true;
	if(pTM_ElementHomeTownData==NULL) pTM_ElementHomeTownData = new CTM_HomeTownData(CHUNK_ELEMENT_HOMETOWN);
	
	for(int i = 0; i < n; ++i)
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;

		CTemplateBase *pNewTemp = pMan->GetTemplateByIdx(i);
		CTemplateBase *pOldTemp = pTM_ElementHomeTownData->FindTemplate(pNewTemp->GetID(),pNewTemp->GetTemplateType());
		if(pOldTemp == NULL) //新加入到原数据中
		{
			pTM_ElementHomeTownData->AddTemplate(pNewTemp);
			pMan->SetTemplateToNull(i);//这样就不用再复制一份数据了
		}
		else
			if(!EnumTemplateImport(pOldTemp,pNewTemp,pDlg)) return false;
	}
	return true;
}

bool CDataManager::ImportTaskData_M(CTemplateManager* pMan, CSelectTransDlg *pDlg, void *param, PROCESSPROC poc)
{
	if(pMan==NULL) return true;
	int n  = pMan->GetTempalteNum();
	if(n<=0) return true;
	if(pTM_ElementTask==NULL) pTM_ElementTask = new CTM_TaskData(CHUNK_ELEMENT_TASK);
	
	for(int i = 0; i < n; ++i)
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;
			
		CTemplateBase *pNewTemp = pMan->GetTemplateByIdx(i);
		CTemplateBase *pOldTemp = pTM_ElementTask->FindTemplate(pNewTemp->GetID(),-1);
		if(pOldTemp == NULL) //新加入到原数据中
		{
			pTM_ElementTask->AddTemplate(pNewTemp);
			pMan->SetTemplateToNull(i);
		}
		else
			if(!EnumTemplateImport(pOldTemp,pNewTemp,pDlg)) return false;
	}
	return true;
}

bool CDataManager::ImportItemDescData_M(CTemplateManager* pMan, CSelectTransDlg *pDlg, void  *param, PROCESSPROC poc)
{
	if(pMan==NULL) return true;
	int n  = pMan->GetTempalteNum();
	if(n<=0) return true;
	if(pTM_ElementItemDesc==NULL) pTM_ElementItemDesc = new CTM_OthersData(CHUNK_ELEMENT_ITEMDESC);
	
	for(int i = 0; i < n; ++i)
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;

		CTemplateBase *pNewTemp = pMan->GetTemplateByIdx(i);
		CTemplateBase *pOldTemp = pTM_ElementItemDesc->FindTemplate(pNewTemp->GetID(),-1);
		if(pOldTemp == NULL) //新加入到原数据中
		{
			pTM_ElementItemDesc->AddTemplate(pNewTemp);
			pMan->SetTemplateToNull(i);
		}
		else
			if(!EnumTemplateImport(pOldTemp,pNewTemp,pDlg)) return false;
	}
	return true;
}

bool CDataManager::ImportInstanceTextData_M(CTemplateManager* pMan, CSelectTransDlg *pDlg, void  *param, PROCESSPROC poc)
{
	if(pMan==NULL) return true;
	int n  = pMan->GetTempalteNum();
	if(n<=0) return true;
	if(pTM_ElementInstaceText==NULL) pTM_ElementInstaceText = new CTM_InstanceData(CHUNK_ELEMENT_INSTANCE_TEXT);
	
	for(int i = 0; i < n; ++i)
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;

		CTemplateBase *pNewTemp = pMan->GetTemplateByIdx(i);
		CTemplateBase *pOldTemp = pTM_ElementInstaceText->FindTemplate(pNewTemp->GetID(),-1);
		if(pOldTemp == NULL) //新加入到原数据中
		{
			pTM_ElementInstaceText->AddTemplate(pNewTemp);
			pMan->SetTemplateToNull(i);
		}
		else
			if(!EnumTemplateImport(pOldTemp,pNewTemp,pDlg)) return false;
	}
	return true;
}

bool CDataManager::ImportItemExDescData_M(CTemplateManager* pMan, CSelectTransDlg *pDlg, void  *param, PROCESSPROC poc)
{
	if(pMan==NULL) return true;
	int n  = pMan->GetTempalteNum();
	if(n<=0) return true;
	if(pTM_ElementItemExDesc==NULL) pTM_ElementItemExDesc = new CTM_OthersData(CHUNK_ELEMENT_ITEMEXDESC);
	
	for(int i = 0; i < n; ++i)
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;

		CTemplateBase *pNewTemp = pMan->GetTemplateByIdx(i);
		CTemplateBase *pOldTemp = pTM_ElementItemExDesc->FindTemplate(pNewTemp->GetID(),-1);
		if(pOldTemp == NULL) //新加入到原数据中
		{
			pTM_ElementItemExDesc->AddTemplate(pNewTemp);
			pMan->SetTemplateToNull(i);
		}
		else
			if(!EnumTemplateImport(pOldTemp,pNewTemp,pDlg)) return false;
	}
	return true;
}

bool CDataManager::ImportItemSkillDescData_M(CTemplateManager* pMan, CSelectTransDlg *pDlg, void  *param, PROCESSPROC poc)
{
	if(pMan==NULL) return true;
	int n  = pMan->GetTempalteNum();
	if(n<=0) return true;
	if(pTM_ElementSkillDesc==NULL) pTM_ElementSkillDesc = new CTM_OthersData(CHUNK_ELEMENT_SKILLDESC);
	
	for(int i = 0; i < n; ++i)
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;
		
		CTemplateBase *pNewTemp = pMan->GetTemplateByIdx(i);
		CTemplateBase *pOldTemp = pTM_ElementSkillDesc->FindTemplate(pNewTemp->GetID(),-1);
		if(pOldTemp == NULL) //新加入到原数据中
		{
			pTM_ElementSkillDesc->AddTemplate(pNewTemp);
			pMan->SetTemplateToNull(i);
		}else
			if(!EnumTemplateImport(pOldTemp,pNewTemp,pDlg)) return false;
	}
	return true;
}

bool CDataManager::ImportItemFixMsgData_M(CTemplateManager* pMan, CSelectTransDlg *pDlg, void  *param, PROCESSPROC poc)
{
	if(pMan==NULL) return true;
	int n  = pMan->GetTempalteNum();
	if(n<=0) return true;
	if(pTM_ElementFixMsg==NULL) pTM_ElementFixMsg = new CTM_MessageData(CHUNK_ELEMENT_FIXMSG);
	
	for(int i = 0; i < n; ++i)
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;
		
		CTemplateBase *pNewTemp = pMan->GetTemplateByIdx(i);
		CTemplateBase *pOldTemp = pTM_ElementFixMsg->FindTemplate(pNewTemp->GetID(),-1);
		if(pOldTemp == NULL) //新加入到原数据中
		{
			pTM_ElementFixMsg->AddTemplate(pNewTemp);
			pMan->SetTemplateToNull(i);
		}
		else
			if(!EnumTemplateImport(pOldTemp,pNewTemp,pDlg)) return false;
	}
	return true;
}

bool CDataManager::ImportItemServerErrData_M(CTemplateManager* pMan, CSelectTransDlg *pDlg, void  *param, PROCESSPROC poc)
{
	if(pMan==NULL) return true;
	int n  = pMan->GetTempalteNum();
	if(n<=0) return true;
	if(pTM_ElementServerErr==NULL) pTM_ElementServerErr = new CTM_MessageData(CHUNK_ELEMENT_SERVERERR);
	
	for(int i = 0; i < n; ++i)
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;
		
		CTemplateBase *pNewTemp = pMan->GetTemplateByIdx(i);
		CTemplateBase *pOldTemp = pTM_ElementServerErr->FindTemplate(pNewTemp->GetID(),-1);
		if(pOldTemp == NULL) //新加入到原数据中
		{
			pTM_ElementServerErr->AddTemplate(pNewTemp);
			pMan->SetTemplateToNull(i);
		}else
			if(!EnumTemplateImport(pOldTemp,pNewTemp,pDlg)) return false;
	}
	return true;
}

bool CDataManager::ImportItemBuffDescData_M(CTemplateManager* pMan, CSelectTransDlg *pDlg, void  *param, PROCESSPROC poc)
{
	if(pMan==NULL) return true;
	int n  = pMan->GetTempalteNum();
	if(n<=0) return true;
	if(pTM_ElementBuffDesc==NULL) pTM_ElementBuffDesc = new CTM_MessageData(CHUNK_ELEMENT_BUFFDESC);
	
	for(int i = 0; i < n; ++i)
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;
		
		CTemplateBase *pNewTemp = pMan->GetTemplateByIdx(i);
		CTemplateBase *pOldTemp = pTM_ElementBuffDesc->FindTemplate(pNewTemp->GetID(),-1);
		if(pOldTemp == NULL) //新加入到原数据中
		{
			pTM_ElementBuffDesc->AddTemplate(pNewTemp);
			pMan->SetTemplateToNull(i);
		}else
			if(!EnumTemplateImport(pOldTemp,pNewTemp,pDlg)) return false;
	}
	return true;
}

bool CDataManager::ImportItemTaskErrData_M(CTemplateManager* pMan, CSelectTransDlg *pDlg, void  *param, PROCESSPROC poc)
{
	if(pMan==NULL) return true;
	int n  = pMan->GetTempalteNum();
	if(n<=0) return true;
	if(pTM_ElementTaskErr==NULL) pTM_ElementTaskErr = new CTM_MessageData(CHUNK_ELEMENT_TASKERR);
	
	for(int i = 0; i < n; ++i)
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;
		
		CTemplateBase *pNewTemp = pMan->GetTemplateByIdx(i);
		CTemplateBase *pOldTemp = pTM_ElementTaskErr->FindTemplate(pNewTemp->GetID(),-1);
		if(pOldTemp == NULL) //新加入到原数据中
		{
			pTM_ElementTaskErr->AddTemplate(pNewTemp);
			pMan->SetTemplateToNull(i);
		}else
			if(!EnumTemplateImport(pOldTemp,pNewTemp,pDlg)) return false;
	}
	return true;
}

bool CDataManager::ImportItemLoadingHintData_M(CTemplateManager* pMan, CSelectTransDlg *pDlg, void  *param, PROCESSPROC poc)
{
	if(pMan==NULL) return true;
	int n  = pMan->GetTempalteNum();
	if(n<=0) return true;
	if(pTM_ElementLoadingHint==NULL) pTM_ElementLoadingHint = new CTM_MessageData(CHUNK_ELEMENT_LOADINGHINT);
	
	for(int i = 0; i < n; ++i)
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;
		
		CTemplateBase *pNewTemp = pMan->GetTemplateByIdx(i);
		CTemplateBase *pOldTemp = pTM_ElementLoadingHint->FindTemplate(pNewTemp->GetID(),-1);
		if(pOldTemp == NULL) //新加入到原数据中
		{
			pTM_ElementLoadingHint->AddTemplate(pNewTemp);
			pMan->SetTemplateToNull(i);
		}else
			if(!EnumTemplateImport(pOldTemp,pNewTemp,pDlg)) return false;
	}
	return true;
}

bool CDataManager::ImportItemPetTalkData_M(CTemplateManager* pMan, CSelectTransDlg *pDlg, void  *param, PROCESSPROC poc)
{
	if(pMan==NULL) return true;
	int n  = pMan->GetTempalteNum();
	if(n<=0) return true;
	if(pTM_ElementPetTalk==NULL) pTM_ElementPetTalk = new CTM_MessageData(CHUNK_ELEMENT_PETTALK);
	
	for(int i = 0; i < n; ++i)
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;
		
		CTemplateBase *pNewTemp = pMan->GetTemplateByIdx(i);
		CTemplateBase *pOldTemp = pTM_ElementPetTalk->FindTemplate(pNewTemp->GetID(),-1);
		if(pOldTemp == NULL) //新加入到原数据中
		{
			pTM_ElementPetTalk->AddTemplate(pNewTemp);
			pMan->SetTemplateToNull(i);
		}else
			if(!EnumTemplateImport(pOldTemp,pNewTemp,pDlg)) return false;
	}
	return true;
}

bool CDataManager::ImportItemAddonStrData_M(CTemplateManager* pMan, CSelectTransDlg *pDlg, void  *param, PROCESSPROC poc)
{
	if(pMan==NULL) return true;
	int n  = pMan->GetTempalteNum();
	if(n<=0) return true;
	if(pTM_ElementAddonStr==NULL) pTM_ElementAddonStr = new CTM_MessageData(CHUNK_ELEMENT_ADDONSTR);
	
	for(int i = 0; i < n; ++i)
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;
		
		CTemplateBase *pNewTemp = pMan->GetTemplateByIdx(i);
		CTemplateBase *pOldTemp = pTM_ElementAddonStr->FindTemplate(pNewTemp->GetID(),-1);
		if(pOldTemp == NULL) //新加入到原数据中
		{
			pTM_ElementAddonStr->AddTemplate(pNewTemp);
			pMan->SetTemplateToNull(i);
		}else
			if(!EnumTemplateImport(pOldTemp,pNewTemp,pDlg)) return false;
	}
	return true;
}

bool CDataManager::ImportInterfaceHintData_M(CTemplateManager* pMan, CSelectTransDlg *pDlg, void  *param, PROCESSPROC poc)
{
	if(pMan==NULL) return true;
	int n  = pMan->GetTempalteNum();
	if(n<=0) return true;
	if(pTM_ElementInterfaceHint==NULL) pTM_ElementInterfaceHint = new CTM_InterfaceHintData(CHUNK_ELEMENT_INTERFACEHINT);
	
	for(int i = 0; i < n; ++i)
	{
		
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;

		CTemplateBase *pNewTemp = pMan->GetTemplateByIdx(i);
		CTemplateBase *pOldTemp = ((CTM_InterfaceHintData*)pTM_ElementInterfaceHint)->FindTemplate(((CTemplate_ElementInterfaceHint*)pNewTemp)->GetIndexText());
		if(pOldTemp == NULL) //新加入到原数据中
		{
			pTM_ElementInterfaceHint->AddTemplate(pNewTemp);
			pMan->SetTemplateToNull(i);
		}
		else
			if(!EnumTemplateImport(pOldTemp,pNewTemp,pDlg)) return false;
		/*
		 其实该类的模板因为肯定没有子模板，所以还是可以用上面的枚举函数，一但该模板存在子模板
		 就必须单独为它写一套枚举函数才行。
		 */
	}
	return true;
}

bool CDataManager::ImportTextFileData_M(CTemplateManager* pMan, CSelectTransDlg *pDlg, void  *param, PROCESSPROC poc)
{
	if(pMan==NULL) return true;
	int n  = pMan->GetTempalteNum();
	if(n<=0) return true;
	if(pTM_ElementTextFile==NULL) pTM_ElementTextFile = new CTM_TextFile(CHUNK_ELEMENT_TEXTFILE);
	
	for(int i = 0; i < n; ++i)
	{
		
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;

		CTemplateBase *pNewTemp = pMan->GetTemplateByIdx(i);
		CTemplateBase *pOldTemp = ((CTM_TextFile*)pTM_ElementTextFile)->FindTemplate(((CTemplate_ElementTextFile*)pNewTemp)->GetIndexText());
		if(pOldTemp == NULL) //新加入到原数据中
		{
			pTM_ElementTextFile->AddTemplate(pNewTemp);
			pMan->SetTemplateToNull(i);
		}
		else
			if(!EnumTemplateImport(pOldTemp,pNewTemp,pDlg)) return false;
		/*
		 其实该类的模板因为肯定没有子模板，所以还是可以用上面的枚举函数，一但该模板存在子模板
		 就必须单独为它写一套枚举函数才行。
		 */
	}
	return true;
}

bool CDataManager::ImportInterfaceStringData_M(CTemplateManager* pMan, CSelectTransDlg *pDlg, void  *param, PROCESSPROC poc)
{
	if(pMan==NULL) return true;
	int n  = pMan->GetTempalteNum();
	if(n<=0) return true;
	if(pTM_ElementInterfaceString==NULL) pTM_ElementInterfaceString = new CTM_InterfaceStringData(CHUNK_ELEMENT_INTERFACESTRING);
	
	for(int i = 0; i < n; ++i)
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;
		
		CTemplate_ElementInterfaceString *pNewTemp = (CTemplate_ElementInterfaceString*)pMan->GetTemplateByIdx(i);
		CTemplateBase *pOldTemp = ((CTM_InterfaceStringData*)pTM_ElementInterfaceString)->FindTemplate(pNewTemp->GetIndexStr(),pNewTemp->GetID());
		if(pOldTemp == NULL) //新加入到原数据中
		{
			pTM_ElementInterfaceString->AddTemplate(pNewTemp);
			pMan->SetTemplateToNull(i);
		}
		else
			if(!EnumTemplateImport(pOldTemp,pNewTemp,pDlg)) return false;
	}
	return true;
}

bool CDataManager::ImportPolicyData_M(CTemplateManager* pMan, CSelectTransDlg *pDlg, void  *param, PROCESSPROC poc)
{
	if(pMan==NULL) return true;
	int n  = pMan->GetTempalteNum();
	if(n<=0) return true;
	if(pTM_ElementPolicy==NULL) pTM_ElementPolicy = new CTM_PolicyData(CHUNK_ELEMENT_POLICY);
	
	for(int i = 0; i < n; ++i)
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;
		
		CTemplateBase *pNewTemp = pMan->GetTemplateByIdx(i);
		CTemplateBase *pOldTemp = pTM_ElementPolicy->FindTemplate(pNewTemp->GetID(),-1);
		if(pOldTemp == NULL) //新加入到原数据中
		{
			pTM_ElementPolicy->AddTemplate(pNewTemp);
			pMan->SetTemplateToNull(i);
		}
		else
			if(!EnumTemplateImport(pOldTemp,pNewTemp,pDlg)) return false;
	}
	return true;
}

bool CDataManager::ImportPrecinctData_M(CTemplateManager* pMan, CSelectTransDlg *pDlg, void  *param, PROCESSPROC poc)
{
	if(pMan==NULL) return true;
	int n  = pMan->GetTempalteNum();
	if(n<=0) return true;
	if(pTM_ElementPrecinct==NULL) pTM_ElementPrecinct = new CTM_PrecinctData(CHUNK_ELEMENT_PRECINCT);
	
	for(int i = 0; i < n; ++i)
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;
		
		CPrecinctTemplate *pNewTemp = (CPrecinctTemplate*)pMan->GetTemplateByIdx(i);
		CTemplateBase *pOldTemp = ((CTM_PrecinctData*)pTM_ElementPrecinct)->FindTemplate(pNewTemp->GetFileName());
		if(pOldTemp == NULL) //新加入到原数据中
		{
			pTM_ElementPrecinct->AddTemplate(pNewTemp);
			pMan->SetTemplateToNull(i);
		}
		else
			if(!EnumTemplateImport(pOldTemp,pNewTemp,pDlg)) return false;
	}
	return true;
}

bool CDataManager::ImportHelpScriptData_M(CTemplateManager* pMan, CSelectTransDlg *pDlg, void  *param, PROCESSPROC poc)
{
	if(pMan==NULL) return true;
	int n  = pMan->GetTempalteNum();
	if(n<=0) return true;
	if(pTM_ElementHelpScript==NULL) pTM_ElementHelpScript = new CTM_InterfaceHintData(CHUNK_ELEMENT_HELPSCRIPT);
	
	for(int i = 0; i < n; ++i)
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;
		
		CTemplateBase *pNewTemp = pMan->GetTemplateByIdx(i);
		CTemplateBase *pOldTemp = ((CTM_InterfaceHintData*)pTM_ElementHelpScript)->FindTemplate(((CTemplate_ElementInterfaceHint*)pNewTemp)->GetIndexText());
		if(pOldTemp == NULL) //新加入到原数据中
		{
			pTM_ElementHelpScript->AddTemplate(pNewTemp);
			pMan->SetTemplateToNull(i);
		}
		else
			if(!EnumTemplateImport(pOldTemp,pNewTemp,pDlg)) return false;
		/*
		 其实该类的模板因为肯定没有子模板，所以还是可以用上面的枚举函数，一但该模板存在子模板
		 就必须单独为它写一套枚举函数才行。
		 */
	}
	return true;
}

bool CDataManager::ImportHelpTextData_M(CTemplateManager* pMan, CSelectTransDlg *pDlg, void  *param, PROCESSPROC poc)
{
	if(pMan==NULL) return true;
	int n  = pMan->GetTempalteNum();
	if(n<=0) return true;
	if(pTM_ElementHelpText==NULL) pTM_ElementHelpText = new CTM_InterfaceHintData(CHUNK_ELEMENT_HELPTEXT);
	
	for(int i = 0; i < n; ++i)
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;
		
		CTemplateBase *pNewTemp = pMan->GetTemplateByIdx(i);
		CTemplateBase *pOldTemp = ((CTM_InterfaceHintData*)pTM_ElementHelpText)->FindTemplate(((CTemplate_ElementInterfaceHint*)pNewTemp)->GetIndexText());
		if(pOldTemp == NULL) //新加入到原数据中
		{
			pTM_ElementHelpText->AddTemplate(pNewTemp);
			pMan->SetTemplateToNull(i);
		}
		else
			if(!EnumTemplateImport(pOldTemp,pNewTemp,pDlg)) return false;
		/*
		 其实该类的模板因为肯定没有子模板，所以还是可以用上面的枚举函数，一但该模板存在子模板
		 就必须单独为它写一套枚举函数才行。
		 */
	}
	return true;
}

/*
bool CDataManager::ImportTreasureData_M(CTemplateManager* pMan, CSelectTransDlg *pDlg, void  *param, PROCESSPROC poc)
{
	if(pMan==NULL) return true;
	
	int n  = pMan->GetTempalteNum();
	if(n<=0) return true;
	if(pTM_ElementTreasure==NULL) pTM_ElementTreasure = new CTM_TreasureData;
	
	for(int i = 0; i < n; ++i)
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;
		
		CTemplateBase *pNewTemp = pMan->GetTemplateByIdx(i);
		CTemplateBase *pOldTemp = pTM_ElementTreasure->FindTemplate(pNewTemp->GetID(),-1);
		if(pOldTemp == NULL) //新加入到原数据中
		{
			pTM_ElementTreasure->AddTemplate(pNewTemp);
			pMan->SetTemplateToNull(i);
		}
		else
			if(!EnumTemplateImport(pOldTemp,pNewTemp,pDlg)) return false;
	}
	return true;
}
*/

bool CDataManager::ImportDomainData_M(CTemplateManager* pManDomain, CTemplateManager* pManDomain1, 
									  CTemplateManager* pManGShop, CTemplateManager* pManGShopTypes, 
									  CTemplateManager* pManGShop1,CTemplateManager* pManGShopTypes1,  
									  CTemplateManager* pManGShop2,CTemplateManager* pManGShopTypes2,  
									  CSelectTransDlg *pDlg, void  *param, PROCESSPROC poc)
{
	if(pManDomain)
	{
		int n  = pManDomain->GetTempalteNum();
		if(pTM_ElementDomainInfor==NULL) pTM_ElementDomainInfor = new CTM_DomainInforData(CHUNK_ELEMENT_DOMAININFOR);
		for(int i = 0; i < n; ++i)
		{
			int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;
			
			CTemplateBase *pNewTemp = pManDomain->GetTemplateByIdx(i);
			CTemplateBase *pOldTemp = pTM_ElementDomainInfor->FindTemplate(pNewTemp->GetID(),-1);
			if(pOldTemp == NULL) //新加入到原数据中
			{
				pTM_ElementDomainInfor->AddTemplate(pNewTemp);
				pManDomain->SetTemplateToNull(i);
			}
			else
				if(!EnumTemplateImport(pOldTemp,pNewTemp,pDlg)) return false;
		}
	}
	
	if(pManDomain1)
	{
		int n  = pManDomain1->GetTempalteNum();
		if(pTM_ElementDomainInfor1==NULL) pTM_ElementDomainInfor1 = new CTM_DomainInforData(CHUNK_ELEMENT_DOMAININFOR1);
		for(int i = 0; i < n; ++i)
		{
			int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;
			
			CTemplateBase *pNewTemp = pManDomain1->GetTemplateByIdx(i);
			CTemplateBase *pOldTemp = pTM_ElementDomainInfor1->FindTemplate(pNewTemp->GetID(),-1);
			if(pOldTemp == NULL) //新加入到原数据中
			{
				pTM_ElementDomainInfor1->AddTemplate(pNewTemp);
				pManDomain1->SetTemplateToNull(i);
			}
			else
				if(!EnumTemplateImport(pOldTemp,pNewTemp,pDlg)) return false;
		}
	}

	if(pManGShop)
	{
		int n  = pManGShop->GetTempalteNum();
		if(pTM_ElementGShop==NULL) pTM_ElementGShop = new CTM_GShopData(CHUNK_ELEMENT_GSHOP);
		for(int i = 0; i < n; ++i)
		{
			int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;
			
			CTemplateBase *pNewTemp = pManGShop->GetTemplateByIdx(i);
			CTemplateBase *pOldTemp = pTM_ElementGShop->FindTemplate(pNewTemp->GetID(),-1);
			if(pOldTemp == NULL) //新加入到原数据中
			{
				pTM_ElementGShop->AddTemplate(pNewTemp);
				pManGShop->SetTemplateToNull(i);
			}
			else
				if(!EnumTemplateImport(pOldTemp,pNewTemp,pDlg)) return false;
		}
	}

	if(pManGShopTypes)
	{
		int n  = pManGShopTypes->GetTempalteNum();
		if(pTM_ElementGShopTypes==NULL) pTM_ElementGShopTypes = new CTM_GShopTypes(CHUNK_ELEMENT_GSHOP_TYPES);
		for(int i = 0; i < n; ++i)
		{
			int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;
			
			CTemplateBase *pNewTemp = pManGShopTypes->GetTemplateByIdx(i);
			CTemplateBase *pOldTemp = pTM_ElementGShopTypes->FindTemplate(pNewTemp->GetID(),-1);
			if(pOldTemp == NULL) //新加入到原数据中
			{
				pTM_ElementGShopTypes->AddTemplate(pNewTemp);
				pManGShopTypes->SetTemplateToNull(i);
			}
			else
				if(!EnumTemplateImport(pOldTemp,pNewTemp,pDlg)) return false;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	if(pManGShop1)
	{
		int n  = pManGShop1->GetTempalteNum();
		if(pTM_ElementGShop1==NULL) pTM_ElementGShop1 = new CTM_GShopData(CHUNK_ELEMENT_GSHOP1);
		for(int i = 0; i < n; ++i)
		{
			int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;
			
			CTemplateBase *pNewTemp = pManGShop1->GetTemplateByIdx(i);
			CTemplateBase *pOldTemp = pTM_ElementGShop1->FindTemplate(pNewTemp->GetID(),-1);
			if(pOldTemp == NULL) //新加入到原数据中
			{
				pTM_ElementGShop1->AddTemplate(pNewTemp);
				pManGShop1->SetTemplateToNull(i);
			}
			else
				if(!EnumTemplateImport(pOldTemp,pNewTemp,pDlg)) return false;
		}
	}

	if(pManGShopTypes1)
	{
		int n  = pManGShopTypes1->GetTempalteNum();
		if(pTM_ElementGShopTypes1==NULL) pTM_ElementGShopTypes1 = new CTM_GShopTypes(CHUNK_ELEMENT_GSHOP_TYPES1);
		for(int i = 0; i < n; ++i)
		{
			int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;
			
			CTemplateBase *pNewTemp = pManGShopTypes1->GetTemplateByIdx(i);
			CTemplateBase *pOldTemp = pTM_ElementGShopTypes1->FindTemplate(pNewTemp->GetID(),-1);
			if(pOldTemp == NULL) //新加入到原数据中
			{
				pTM_ElementGShopTypes1->AddTemplate(pNewTemp);
				pManGShopTypes1->SetTemplateToNull(i);
			}
			else
				if(!EnumTemplateImport(pOldTemp,pNewTemp,pDlg)) return false;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	if(pManGShop2)
	{
		int n  = pManGShop2->GetTempalteNum();
		if(pTM_ElementGShop2==NULL) pTM_ElementGShop2 = new CTM_GShopData(CHUNK_ELEMENT_GSHOP2);
		for(int i = 0; i < n; ++i)
		{
			int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;
			
			CTemplateBase *pNewTemp = pManGShop2->GetTemplateByIdx(i);
			CTemplateBase *pOldTemp = pTM_ElementGShop2->FindTemplate(pNewTemp->GetID(),-1);
			if(pOldTemp == NULL) //新加入到原数据中
			{
				pTM_ElementGShop2->AddTemplate(pNewTemp);
				pManGShop2->SetTemplateToNull(i);
			}
			else
				if(!EnumTemplateImport(pOldTemp,pNewTemp,pDlg)) return false;
		}
	}

	if(pManGShopTypes2)
	{
		int n  = pManGShopTypes2->GetTempalteNum();
		if(pTM_ElementGShopTypes2==NULL) pTM_ElementGShopTypes2 = new CTM_GShopTypes(CHUNK_ELEMENT_GSHOP_TYPES2);
		for(int i = 0; i < n; ++i)
		{
			int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;
			
			CTemplateBase *pNewTemp = pManGShopTypes2->GetTemplateByIdx(i);
			CTemplateBase *pOldTemp = pTM_ElementGShopTypes2->FindTemplate(pNewTemp->GetID(),-1);
			if(pOldTemp == NULL) //新加入到原数据中
			{
				pTM_ElementGShopTypes2->AddTemplate(pNewTemp);
				pManGShopTypes2->SetTemplateToNull(i);
			}
			else
				if(!EnumTemplateImport(pOldTemp,pNewTemp,pDlg)) return false;
		}
	}
	return true;
}


//------------------------------------------------------------------------------------------------

bool CDataManager::ExportElementData(char *szDistPath, void *param, PROCESSPROC poc)
{
	char szPath[256], szPathData[256];
	sprintf(szPath,"%s\\%s",szDistPath,"data\\elements.data");
	sprintf(szPathData,"%s\\%s", szDistPath,"data\\path.data");
	
	DWORD flag = GetFileAttributesA(szPath);
	if (flag & FILE_ATTRIBUTE_READONLY) 
	{
		flag ^= FILE_ATTRIBUTE_READONLY;
		if(!SetFileAttributesA(szPath,flag))
			g_Log.Log("Faild to set file attributes (%s)",szPath);
	}

 	g_MD5Hash.init();
	g_MD5Hash.Update("ZPWDATA", 7);
	DataPathMan pathman;

	if (pathman.CalculateMd5(szPathData) == 0)
	{
		AfxMessageBox(_T("Faild to load data\\path.data"));
		return false;
	}

	elementdataman dataMan;
	if(-1==dataMan.load_data(szPath))
	{
		g_Log.Log("Faild to load element data (%s)",szPath);
		return false;
	}
	
	for( int i = 0; i < ID_SPACE_CONFIG + 1; ++i)
	{
		int nNum = dataMan.get_data_num((ID_SPACE)i);
		for(int j = 0; j < nNum; ++j)
		{
			int per = int(((float)j/(float)nNum) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;
			
			DATA_TYPE type;
			unsigned int id = dataMan.get_data_id((ID_SPACE)i,j,type);
			const void *pEDData = dataMan.get_data_ptr(id,(ID_SPACE)i,type);
			CTemplateBase * pTemplate = NULL;
			pTemplate = pTM_ElementData->FindTemplate(id,type);
			if(pTemplate != NULL)
			{
				if(!pTemplate->Export((void *)pEDData)) return false;
			}
		}
	}

	g_MD5Hash.init();
	g_MD5Hash.Update("ZPWDATA", 7);
	DataPathMan pathman1;

	if (pathman1.CalculateMd5(szPathData) == 0)
	{
		AfxMessageBox(_T("Faild to load data\\path.data"));
		return false;
	}

	if(-1==dataMan.save_data(szPath))
	{
		g_Log.Log("Faild to save element data (%s)",szPath);
		return false;
	}
	return true;
}

bool CDataManager::ExportHomeTownData(char *szDistPath, void *param, PROCESSPROC poc)
{
	char szPath[256];
	sprintf(szPath,"%s\\%s",szDistPath,"data\\hometowndata");	
	
	DWORD flag = GetFileAttributesA(szPath);
	if (flag & FILE_ATTRIBUTE_READONLY) 
	{
		flag ^= FILE_ATTRIBUTE_READONLY;
		if(!SetFileAttributesA(szPath,flag))
			g_Log.Log("Failed to set file attributes (%s)",szPath);
	}

	HOMETOWN::HometownTemplate* tpl = HOMETOWN::HometownTemplate::GetInstance();
	if(-1==tpl->load_data(szPath))
	{
		g_Log.Log("Failed to load hometown data (%s)",szPath);
		return false;
	}
	
	for( int i = 0; i < HOMETOWN::ID_SPACE_MAX; ++i)
	{
		HOMETOWN::DATA_TYPE datatype;
		unsigned int id = tpl->get_first_data_id((HOMETOWN::ID_SPACE)i, datatype);
		const void* pData = 0;
		const HOMETOWN::CROP_ESSENCE *crop = 0;
		const HOMETOWN::FRUIT_ESSENCE *fruit = 0;
		const HOMETOWN::TOOL_FUNC *tool_func = 0;
		const HOMETOWN::TOOL_ESSENCE *tool_ess = 0;	
		while (id)
		{		
			switch(datatype)
			{
			case HOMETOWN::DT_CROP_ESSENCE:
				tpl->get_data_ptr(id, (HOMETOWN::ID_SPACE)i, datatype, crop);
				pData = crop;
				break;
			case HOMETOWN::DT_FRUIT_ESSENCE:
				tpl->get_data_ptr(id, (HOMETOWN::ID_SPACE)i, datatype, fruit);
				pData = fruit;
				break;
			case HOMETOWN::DT_TOOL_FUNC:
				tpl->get_data_ptr(id, (HOMETOWN::ID_SPACE)i, datatype, tool_func);
				pData = tool_func;
				break;
			case HOMETOWN::DT_TOOL_ESSENCE:
				tpl->get_data_ptr(id, (HOMETOWN::ID_SPACE)i, datatype, tool_ess);
				pData = tool_ess;
				break;
			}

			CTemplateBase * pTemplate = NULL;
			pTemplate = pTM_ElementHomeTownData->FindTemplate(id,datatype);
			if(pTemplate != NULL)
			{
				if(!pTemplate->Export((void *)pData)) return false;
			}

			id = tpl->get_next_data_id((HOMETOWN::ID_SPACE)i, datatype);
		}
	}


	if(-1==tpl->save_data(szPath))
	{
		g_Log.Log("Failed to save hometown data (%s)",szPath);
		return false;
	}
	return true;
}

bool CDataManager::ExportTaskData(char *szDistPath, void *param, PROCESSPROC poc)
{
	char szPath[256];
	sprintf(szPath,"%s\\%s",szDistPath,"data\\tasks.data");

	ATaskTemplMan taskData;
	if(!taskData.LoadTasksFromPack(szPath,"data\\forbidden_task.txt", true))
	{
		g_Log.Log("Faild to load task data %s",szPath);
		return false;
	}

	sprintf(szPath,"%s\\%s",szDistPath,"data\\task_npc.data");
	if(!taskData.LoadNPCInfoFromPack(szPath))
	{
		g_Log.Log("Faild to load task data %s",szPath);
		return false;
	}

	TaskTemplMap& taskMap = taskData.GetAllTemplMap();
	int nNum = taskMap.size();
	int j = 0;
	TaskTemplMap::iterator it = taskMap.begin();
	for( it; it != taskMap.end(); ++it)
	{
		int per = int(((float)j/(float)nNum) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;
		
		ATaskTempl * pTask = it->second;
		
		CTemplateBase * pTemplate = NULL;
		pTemplate = pTM_ElementTask->FindTemplate(pTask->GetID(),-1);
		if(pTemplate != NULL)
			if(!pTemplate->Export((void *)pTask)) return false;
		
		++j;
	}
	
	sprintf(szPath,"%s\\%s",szDistPath,"data\\tasks.data");
	DWORD flag = GetFileAttributesA(szPath);
	if (flag & FILE_ATTRIBUTE_READONLY) 
	{
		flag ^= FILE_ATTRIBUTE_READONLY;
		if(!SetFileAttributesA(szPath,flag))
			g_Log.Log("Faild to set file attributes (%s)",szPath);
	}
	if(!taskData.SaveTasksToPack(szPath,taskData.GetExportVersion()))
	{
		g_Log.Log("Faild to save task data %s",szPath);
		return false;
	}
	
	sprintf(szPath,"%s\\%s",szDistPath,"data\\task_npc.data");
	flag = GetFileAttributesA(szPath);
	if (flag & FILE_ATTRIBUTE_READONLY) 
	{
		flag ^= FILE_ATTRIBUTE_READONLY;
		if(!SetFileAttributesA(szPath,flag))
			g_Log.Log("Faild to set file attributes (%s)",szPath);
	}
	if(!taskData.SaveNPCInfoToPack(szPath))
	{
		g_Log.Log("Faild to save task data %s",szPath);
		return false;
	}

	taskData.Release();
	return true;
}

bool CDataManager::ExportItemDescData(char *szDistPath, void  *param, PROCESSPROC poc)
{
	if(pTM_ElementItemDesc==NULL) return  true;
	CECStringTab strTab(CECStringTab::SET_ITEMDESC,szDistPath);
	if(!strTab.Load()) 
	{
		g_Log.Log("Faild to load item desc data!");
		return false;
	}

	int n = strTab.GetTextNum();
	for( int i = 0; i < n; ++i )
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;

		CECStringTab::TEXT *pText = strTab.GetText(i);
		
		CTemplateBase * pTemplate = NULL;
		pTemplate = pTM_ElementItemDesc->FindTemplate(pText->id,-1);
		if(pTemplate != NULL)
			if(!pTemplate->Export((void *)pText)) return false;
	}

	if(!strTab.Save()) 
	{
		g_Log.Log("Faild to save item desc data!");
		return false;
	}
	return true;
}

bool CDataManager::ExportInstanceText(char *szDistPath, void  *param, PROCESSPROC poc)
{
	if(pTM_ElementInstaceText==NULL) return  true;
	CECInstanceText strInstance;
	if(!strInstance.Load("Configs\\instance.txt")) 
	{
		g_Log.Log("Faild to load instance text data!");
		return false;
	}

	int n = strInstance.GetTextNum();
	for( int i = 0; i < n; ++i )
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;

		CECInstanceText::_INSTANCE_TEXT *pText = strInstance.GetText(i);
		
		CTemplateBase * pTemplate = NULL;
		pTemplate = pTM_ElementInstaceText->FindTemplate(pText->pText->id,-1);
		if(pTemplate != NULL)
			if(!pTemplate->Export((void *)pText)) return false;
	}

	if(!strInstance.Save("Configs\\instance.txt")) 
	{
		g_Log.Log("Faild to save instance text data!");
		return false;
	}
	return true;
}

bool CDataManager::ExportItemExDescData(char *szDistPath, void  *param, PROCESSPROC poc)
{
	if(pTM_ElementItemExDesc==NULL) return  true;
	CECStringTab strTab(CECStringTab::SET_ITEMEXTDESC,szDistPath);
	if(!strTab.Load()) 
	{
		g_Log.Log("Faild to load itemEx desc data!");
		return false;
	}

	int n = strTab.GetTextNum();
	for( int i = 0; i < n; ++i )
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;
		
		CECStringTab::TEXT *pText = strTab.GetText(i);
		
		CTemplateBase * pTemplate = NULL;
		pTemplate = pTM_ElementItemExDesc->FindTemplate(pText->id,-1);
		if(pTemplate != NULL)
			if(!pTemplate->Export((void *)pText)) return false;
		
	}
	if(!strTab.Save())
	{
		g_Log.Log("Faild to save itemEx desc data!");
		return false;
	}
	return true;
}

bool CDataManager::ExportItemSkillDescData(char *szDistPath, void  *param, PROCESSPROC poc)
{
	if(pTM_ElementSkillDesc==NULL) return false;
	
	CECStringTab strTab(CECStringTab::SET_SKILLDESC,szDistPath);
	if(!strTab.Load()) 
	{
		g_Log.Log("Faild to save skill desc data!");
		return true;
	}
	

	int n = strTab.GetTextNum();
	for( int i = 0; i < n; ++i )
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;
		CECStringTab::TEXT *pText = strTab.GetText(i);
		
		CTemplateBase * pTemplate = NULL;
		pTemplate = pTM_ElementSkillDesc->FindTemplate(pText->id,-1);
		if(pTemplate != NULL)
			if(!pTemplate->Export((void *)pText)) return false;
	}

	if(!strTab.Save())
	{
		g_Log.Log("Faild to save skill desc data!");
		return false;
	}
	
	return true;
}

bool CDataManager::ExportItemFixMsgData(char *szDistPath, void  *param, PROCESSPROC poc)
{
	if(pTM_ElementFixMsg==NULL) return true;
	
	CECStringTab strTab(CECStringTab::SET_FIXMSG,szDistPath);
	if(!strTab.Load()) 
	{
		g_Log.Log("Faild to load fix msg data!");
		return false;
	}
	
	int n = strTab.GetTextNum();
	for( int i = 0; i < n; ++i )
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;
		
		CECStringTab::TEXT *pText = strTab.GetText(i);
		
		CTemplateBase * pTemplate = NULL;
		pTemplate = pTM_ElementFixMsg->FindTemplate(pText->id,-1);
		if(pTemplate != NULL)
			if(!pTemplate->Export((void *)pText)) return false;
	}

	if(!strTab.Save())
	{
		g_Log.Log("Faild to save fix msg data!");
		return false;
	}

	return true;
}

bool CDataManager::ExportItemServerErrData(char *szDistPath, void  *param, PROCESSPROC poc)
{
	if(pTM_ElementServerErr==NULL) return true;

	CECStringTab strTab(CECStringTab::SET_SERVERERR,szDistPath);
	if(!strTab.Load()) 
	{
		g_Log.Log("Faild to load server error msg data!");
		return false;
	}	
	int n = strTab.GetTextNum();
	for( int i = 0; i < n; ++i )
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;

		CECStringTab::TEXT *pText = strTab.GetText(i);
		
		CTemplateBase * pTemplate = NULL;
		pTemplate = pTM_ElementServerErr->FindTemplate(pText->id,-1);
		if(pTemplate != NULL)
			if(!pTemplate->Export((void *)pText)) return false;
		
	}

	if(!strTab.Save()) 
	{
		g_Log.Log("Faild to save server error msg data!");
		return false;
	}
	return true;
}

bool CDataManager::ExportItemBuffDescData(char *szDistPath, void  *param, PROCESSPROC poc)
{
	if(pTM_ElementBuffDesc==NULL) return true;

	CECStringTab strTab(CECStringTab::SET_BUFFDESC,szDistPath);
	if(!strTab.Load()) 
	{
		g_Log.Log("Faild to load buff desc msg data!");
		return false;
	}	
	int n = strTab.GetTextNum();
	for( int i = 0; i < n; ++i )
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;

		CECStringTab::TEXT *pText = strTab.GetText(i);
		
		CTemplateBase * pTemplate = NULL;
		pTemplate = pTM_ElementBuffDesc->FindTemplate(pText->id,-1);
		if(pTemplate != NULL)
			if(!pTemplate->Export((void *)pText)) return false;
		
	}

	if(!strTab.Save()) 
	{
		g_Log.Log("Faild to save buff desc data!");
		return false;
	}
	return true;
}

bool CDataManager::ExportItemTaskErrData(char *szDistPath, void  *param, PROCESSPROC poc)
{
	if(pTM_ElementTaskErr==NULL) return true;

	CECStringTab strTab(CECStringTab::SET_TASKERROR,szDistPath);
	if(!strTab.Load()) 
	{
		g_Log.Log("Faild to load task error msg data!");
		return false;
	}	
	int n = strTab.GetTextNum();
	for( int i = 0; i < n; ++i )
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;

		CECStringTab::TEXT *pText = strTab.GetText(i);
		
		CTemplateBase * pTemplate = NULL;
		pTemplate = pTM_ElementTaskErr->FindTemplate(pText->id,-1);
		if(pTemplate != NULL)
			if(!pTemplate->Export((void *)pText)) return false;
		
	}

	if(!strTab.Save()) 
	{
		g_Log.Log("Faild to save task error data!");
		return false;
	}
	return true;
}

bool CDataManager::ExportItemLoadingHintData(char *szDistPath, void  *param, PROCESSPROC poc)
{
	if(pTM_ElementLoadingHint==NULL) return true;

	CECStringTab strTab(CECStringTab::SET_LOADINGHINT,szDistPath);
	if(!strTab.Load()) 
	{
		g_Log.Log("Faild to load loading hint data!");
		return false;
	}	
	int n = strTab.GetTextNum();
	for( int i = 0; i < n; ++i )
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;

		CECStringTab::TEXT *pText = strTab.GetText(i);
		
		CTemplateBase * pTemplate = NULL;
		pTemplate = pTM_ElementLoadingHint->FindTemplate(pText->id,-1);
		if(pTemplate != NULL)
			if(!pTemplate->Export((void *)pText)) return false;
		
	}

	if(!strTab.Save()) 
	{
		g_Log.Log("Faild to save loading hint data!");
		return false;
	}
	return true;
}

bool CDataManager::ExportItemPetTalkData(char *szDistPath, void  *param, PROCESSPROC poc)
{
	if(pTM_ElementPetTalk==NULL) return true;

	CECStringTab strTab(CECStringTab::SET_PETTALK,szDistPath);
	if(!strTab.Load()) 
	{
		g_Log.Log("Faild to load pet talk data!");
		return false;
	}	
	int n = strTab.GetTextNum();
	for( int i = 0; i < n; ++i )
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;

		CECStringTab::TEXT *pText = strTab.GetText(i);
		
		CTemplateBase * pTemplate = NULL;
		pTemplate = pTM_ElementPetTalk->FindTemplate(pText->id,-1);
		if(pTemplate != NULL)
			if(!pTemplate->Export((void *)pText)) return false;
		
	}

	if(!strTab.Save()) 
	{
		g_Log.Log("Faild to save pet talk data!");
		return false;
	}
	return true;
}

bool CDataManager::ExportItemAddonStrData(char *szDistPath, void  *param, PROCESSPROC poc)
{
	if(pTM_ElementAddonStr==NULL) return true;

	CECStringTab strTab(CECStringTab::SET_ADDONSTR,szDistPath);
	if(!strTab.Load()) 
	{
		g_Log.Log("Faild to addon string talk data!");
		return false;
	}	
	int n = strTab.GetTextNum();
	for( int i = 0; i < n; ++i )
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;

		CECStringTab::TEXT *pText = strTab.GetText(i);
		
		CTemplateBase * pTemplate = NULL;
		pTemplate = pTM_ElementAddonStr->FindTemplate(pText->id,-1);
		if(pTemplate != NULL)
			if(!pTemplate->Export((void *)pText)) return false;
		
	}

	if(!strTab.Save()) 
	{
		g_Log.Log("Faild to save addon string data!");
		return false;
	}
	return true;
}

bool CDataManager::ExportInterfaceString(char *szDistPath, void *param, PROCESSPROC poc, CElementInterfaceManager* pinter)
{
	if (!pinter)
		return false;
	if (!pTM_ElementInterfaceString)
		return true;
	CElementInterfaceManager& inter = *pinter;

	InterfaceFileNames* fileNames = inter.GetStringTableFiles();
	
	int n = fileNames->size();
	for(int i = 0; i < n; ++i )
	{
		ACString name = (*fileNames)[i];
		StringTableFile* tableFile = inter.GetStringTable(name);
		if(tableFile==NULL) continue;
		StringTableFile::iterator it = tableFile->begin();
		for( ; it != tableFile->end(); ++it)
		{
			int sid = it->first;
			ACString text = it->second;
			
			CString xxx(text);
			if(4==xxx.Replace(_T("###"),_T(" ")) || sid == -1)
			{//这儿我是想屏蔽掉字体字符串
				continue;
			}
			
			INTER_STRING data;
			data.id = sid;
			lstrcpy(data.szIndex,name);
			lstrcpy(data.szText,text);
			
			CTemplateBase * pTemplate = NULL;
			pTemplate = ((CTM_InterfaceStringData*)pTM_ElementInterfaceString)->FindTemplate(data.szIndex,data.id);
			if(pTemplate != NULL)
			{
				if(!pTemplate->Export(&data)) 
				{
					g_Log.Log("export %s failed", name);
					return false;
				}
				it->second = data.szText;
			}
		}
	}

	return true;
}

bool CDataManager::ExportInterfaceHint(char *szDistPath, void *param, PROCESSPROC poc, CElementInterfaceManager* pinter)
{
	if (!pinter)
		return false;
	if (!pTM_ElementInterfaceHint)
		return true;
	CElementInterfaceManager& inter = *pinter;

	InterfaceFileNames* fileNames = inter.GetHintTableFiles();
	
	int n = fileNames->size();
	for( int i = 0; i < n; ++i )
	{
		int per = int(((float)i/(float)n) * 100); 
		if(!(*poc)(param,per,g_nTotalPer)) return false;
		
		ACString name = (*fileNames)[i];
		HintTableFile* tableFile = inter.GetHintTable(name);
		if(tableFile==NULL) continue;
		HintTableFile::iterator it = tableFile->begin();
		for( ; it != tableFile->end(); ++it)
		{
			ACString index = it->first;
			ACString text = it->second;
			INTER_HINT data;
			lstrcpy(data.szText,text);
			_stprintf(data.szIndex,_T("%s-%s"),name,index);
			
			CTemplateBase * pTemplate = NULL;
			pTemplate = ((CTM_InterfaceHintData*)pTM_ElementInterfaceHint)->FindTemplate(data.szIndex);
			if(pTemplate != NULL)
			{
				if(!pTemplate->Export(&data)) return false;
				it->second = data.szText;
			}
		}
	}

	return true;
}

bool CDataManager::ExportHelpScript(char *szDistPath, void *param, PROCESSPROC poc, CElementInterfaceManager* pinter)
{
	if (!pinter)
		return false;
	if (!pTM_ElementHelpScript)
		return true;
	CElementInterfaceManager& inter = *pinter;

	InterfaceFileNames* fileNames = inter.GetHelpScriptFiles();
	int n = fileNames->size();
	for( int i = 0; i < n; ++i )
	{
		int per = int(((float)i/(float)n) * 100); 
		if(!(*poc)(param,per,g_nTotalPer)) return false;
		
		ACString name = (*fileNames)[i];
		HintTableFile* tableFile = inter.GetHelpScriptTable(name);
		if(tableFile==NULL) continue;
		HintTableFile::iterator it = tableFile->begin();
		for( ; it != tableFile->end(); ++it)
		{
			ACString index = it->first;
			ACString text = it->second;
			INTER_HINT data;
			lstrcpy(data.szText,text);
			_stprintf(data.szIndex,_T("%s-%s"),name,index);
			
			CTemplateBase * pTemplate = NULL;
			pTemplate = ((CTM_InterfaceHintData*)pTM_ElementHelpScript)->FindTemplate(data.szIndex);
			if(pTemplate != NULL)
			{
				if(!pTemplate->Export(&data)) 						
				{
					g_Log.Log("export %s failed", name);
					return false;
				}
				it->second = data.szText;
			}
		}
	}

	return true;
}


bool CDataManager::ExportHelpText(char *szDistPath, void *param, PROCESSPROC poc, CElementInterfaceManager* pinter)
{
	if (!pinter)
		return false;
	if (!pTM_ElementHelpText)
		return true;
	CElementInterfaceManager& inter = *pinter;

	InterfaceFileNames* fileNames = inter.GetHelpTextFiles();
	int n = fileNames->size();
	for( int i = 0; i < n; ++i )
	{
		int per = int(((float)i/(float)n) * 100); 
		if(!(*poc)(param,per,g_nTotalPer)) return false;
		
		ACString name = (*fileNames)[i];
		HintTableFile* tableFile = inter.GetHelpTextTable(name);
		if(tableFile==NULL) continue;
		HintTableFile::iterator it = tableFile->begin();
		for( ; it != tableFile->end(); ++it)
		{
			ACString index = it->first;
			ACString text = it->second;
			INTER_HINT data;
			lstrcpy(data.szText,text);
			_stprintf(data.szIndex,_T("%s-%s"),name,index);
			
			CTemplateBase * pTemplate = NULL;
			pTemplate = ((CTM_InterfaceHintData*)pTM_ElementHelpText)->FindTemplate(data.szIndex);
			if(pTemplate != NULL)
			{
				if(!pTemplate->Export(&data)) 
				{
					g_Log.Log("export %s failed", name);
					return false;
				}
				it->second = data.szText;
			}
		}
	}

	return true;
}

bool CDataManager::ExportInterfaceData(char *szDistPath, void  *param, PROCESSPROC poc)
{
	if(pTM_ElementInterfaceHint==NULL && pTM_ElementInterfaceString==NULL) return true;
	
	CElementInterfaceManager inter(szDistPath);
	if(!inter.LoadInterface()) 
	{
		g_Log.Log("Faild to load interface data!");
		return false;
	}
	
	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_INTERFACEHINT) && 
		!ExportInterfaceHint(szDistPath, param, poc, &inter))
		return false;

	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_INTERFACESTRING) && 
		!ExportInterfaceString(szDistPath, param, poc, &inter))
		return false;
	
	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_HELPSCRIPT) && 
		!ExportHelpScript(szDistPath, param, poc, &inter))
		return false;
	
	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_HELPTEXT) && 
		!ExportHelpText(szDistPath, param, poc, &inter))
		return false;

	if(!inter.SaveInterface()) 
	{
		g_Log.Log("Faild to save interface data!");
		return false;
	}
	return true;
}

bool CDataManager::ExportTextFileData(char *szDistPath, void *param, PROCESSPROC poc)
{
	CTextFileMgr inter(szDistPath);
	if(!inter.LoadData()) 
	{
		g_Log.Log("Faild to load text file data!");
		return false;
	}
	
	FILE_DATA_MAP* tableFile = inter.GetFileDataMap();
	if(tableFile==NULL) return true;
	FILE_DATA_MAP::iterator it = tableFile->begin();
	for( ; it != tableFile->end(); ++it)
	{
		ACString index = it->first;
		ACString text = it->second;
		FILE_DATA data;
		lstrcpy(data.szText,text);
		lstrcpy(data.szIndex,index);
		
		CTemplateBase * pTemplate = NULL;
		pTemplate = ((CTM_TextFile*)pTM_ElementTextFile)->FindTemplate(data.szIndex);
		if(pTemplate != NULL)
		{
			if(!pTemplate->Export(&data)) return false;
			it->second = data.szText;
		}
	}

	if(!inter.SaveData())
	{
		g_Log.Log("Faild to save text file data!");
		return false;
	}
	return true;
}

bool CDataManager::ExportPolicyData(char *szDistPath, void *param, PROCESSPROC poc)
{
	if(pTM_ElementPolicy==NULL) return true;
	
	CPolicyDataManager pdMan;
	char szPath[MAX_PATH];
	sprintf(szPath,"%s\\data\\aipolicy.data",szDistPath);

	DWORD flag = GetFileAttributesA(szPath);
	if (flag & FILE_ATTRIBUTE_READONLY) 
	{
		flag ^= FILE_ATTRIBUTE_READONLY;
		if(!SetFileAttributesA(szPath,flag))
			g_Log.Log("Faild to set file attributes (%s)",szPath);
	}

	if(!pdMan.Load(szPath)) 
	{
		g_Log.Log("Faild to load data (%s)",szPath);
		return false;
	}
	
	int n = pdMan.GetPolicyNum();
	for( int i = 0; i < n; ++i)
	{
		CPolicyData *pData = pdMan.GetPolicy(i);	
		unsigned int uid = pData->GetID();
		CTemplateBase *pOldTempl = pTM_ElementPolicy->FindTemplate(uid,-1);
		if(pOldTempl!=NULL)
			if(!pOldTempl->Export(pData)) return false;
		
	}
	if(!pdMan.Save(szPath))
	{
		g_Log.Log("Faild to save policy data!");
		return false;
	}
	pdMan.Release();
	return true;	
}

bool CDataManager::ExportPrecinctData(char *szDistPath, void *param, PROCESSPROC poc)
{
	if(pTM_ElementPrecinct==NULL) return true;
	CString strWildcard;
	strWildcard.Format(_T("%s\\Maps"),c_to_w(szDistPath));
	if(!EnumExpPrecinct(strWildcard,szDistPath)) return false;
	return true;	
}

bool  CDataManager::EnumExpPrecinct(CString path,CString expPath)
{
	CString fileName,ext;
	CString strWildcard = path;
	strWildcard +=_T("\\*.*");
	
	WIN32_FIND_DATAA dat;
	HANDLE hd;
	hd = FindFirstFileA(w_to_c(strWildcard),&dat);
	if(hd!=NULL)
	{
		do
		{
			if(dat.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{//paths
				if(stricmp("..",dat.cFileName)==0 || stricmp(".",dat.cFileName)==0) continue;
				CString tt = path;
				tt.Format(_T("%s\\%s"),path,c_to_w(dat.cFileName));
				if(!EnumExpPrecinct(tt,expPath)) return false;
				continue;
			}else if(dat.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
			{//files
				ext = c_to_w(dat.cFileName);
				fileName.Format(_T("%s\\%s"),path,c_to_w(dat.cFileName));
				ext.MakeLower();
				if(_tcsicmp(ext,_T("precinct.clt"))==0)
				{
					char szPathName[MAX_PATH];
					fileName.Replace(expPath,_T(""));
					sprintf(szPathName,"%s",w_to_c(fileName));
					CPrecinctTemplate *pOldTempl = (CPrecinctTemplate*) ((CTM_PrecinctData*)pTM_ElementPrecinct)->FindTemplate(szPathName);
					if(pOldTempl!=NULL)
					{
						sprintf(szPathName,"%s\\%s",w_to_c(path),dat.cFileName);
						if(!pOldTempl->Export(szPathName))
						{
							g_Log.Log("Faild to export %s",szPathName);
							return false;
						}
					}
				}
			}
		}while(FindNextFileA(hd,&dat));
		FindClose(hd);
	}
	return true;
}

/*
bool CDataManager::ExportTreasureData(char *szDistPath, void *param, PROCESSPROC poc)
{
	if(pTM_ElementTreasure==NULL) return true;
	
	CTreasureMan pdMan;
	char szPath[MAX_PATH];
	sprintf(szPath,"%s\\data\\gshop.data",szDistPath);
	if(!pdMan.LoadL(szPath)) 
	{
		g_Log.Log("Faild to load data (%s)",szPath);
		return false;
	}
	
	int n = pdMan.GetTreasureCount();
	for( int i = 0; i < n; ++i)
	{
		TREASURE_ITEM *pData = pdMan.GetTreasure(i);	
		unsigned int uid = pData->id;
		CTemplateBase *pOldTempl = pTM_ElementTreasure->FindTemplate(uid,-1);
		if(pOldTempl!=NULL)
			if(!pOldTempl->Export(pData)) return false;
		
	}
	if(!pdMan.ExportL(szPath)) return false;
	pdMan.Release();
	return true;	
}
*/

bool CDataManager::_ExportDomainInfor(char *szDistPath, void *param, PROCESSPROC poc)
{
	if (pTM_ElementDomainInfor==NULL)
		return true;
	vector<DOMAIN_INFO> * pListDomain = globaldata_getdomaininfos();

	int i;
	int n = pListDomain->size();
	for( i = 0; i < n; ++i)
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;

		DOMAIN_INFO* data = &(*pListDomain)[i];	
		unsigned int uid = data->id;
		CTemplateBase *pOldTempl = pTM_ElementDomainInfor->FindTemplate(uid,-1);
		if(pOldTempl!=NULL)
		{
			if(!pOldTempl->Export(data)) 
			{
				g_Log.Log("failed to export DomainInfor %d", uid);
				return false;
			}
		}
	}
	return true;
}

bool CDataManager::_ExportDomainInfor1(char *szDistPath, void *param, PROCESSPROC poc)
{
	if(pTM_ElementDomainInfor1==NULL) 
		return true;
	vector<DOMAIN_INFO> * pListDomain = globaldata_getdomaininfos2();

	int n = pListDomain->size();
	for(int i = 0; i < n; ++i)
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;

		DOMAIN_INFO* data = &(*pListDomain)[i];	
		unsigned int uid = data->id;
		CTemplateBase *pOldTempl = pTM_ElementDomainInfor1->FindTemplate(uid,-1);
		if(pOldTempl!=NULL)
		{
			if(!pOldTempl->Export(data))
			{
				g_Log.Log("failed to export DomainInfor1 %d", uid);
				return false;
			}
		}
	}
	return true;
}

bool CDataManager::_ExportGshop(char *szDistPath, void *param, PROCESSPROC poc)
{
	if(pTM_ElementGShop==NULL) 
		return true;
	vector<GSHOP_ITEM> * pListGShop = globaldata_getgshopitems();

	int n = pListGShop->size();
	for(int i = 0; i < n; ++i)
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;

		GSHOP_ITEM data = (*pListGShop)[i];	
		unsigned int uid = data.local_id;
		CTemplateBase *pOldTempl = pTM_ElementGShop->FindTemplate(uid,-1);
		if(pOldTempl!=NULL)
		{
			if(!pOldTempl->Export(&data))
			{
				g_Log.Log("failed to export Gshop %d", uid);
				return false;
			}
			(*pListGShop)[i] = data;
		}
	}
	return true;
}

bool CDataManager::_ExportGshop1(char *szDistPath, void *param, PROCESSPROC poc)
{
	vector<GSHOP_ITEM> * pListGShop2 = globaldata_getgshopitems2();
	if(pTM_ElementGShop1==NULL) 
		return true;

	int n2 = pListGShop2->size();
	for(int i = 0; i < n2; ++i)
	{
		int per = int(((float)i/(float)n2) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;

		GSHOP_ITEM data = (*pListGShop2)[i];	
		unsigned int uid = data.local_id;
		CTemplateBase *pOldTempl = pTM_ElementGShop1->FindTemplate(uid,-1);
		if(pOldTempl!=NULL)
		{
			if(!pOldTempl->Export(&data))
			{
				g_Log.Log("failed to export Gshop1 %d", uid);
				return false;
			}
			(*pListGShop2)[i] = data;
		}
	}
	return true;
}

bool CDataManager::_ExportGshop2(char *szDistPath, void *param, PROCESSPROC poc)
{
	vector<GSHOP_ITEM> * pListGShop3 = globaldata_getgshopitems3();
	if(pTM_ElementGShop2==NULL) 
		return true;

	int n3 = pListGShop3->size();
	for(int i = 0; i < n3; ++i)
	{
		int per = int(((float)i/(float)n3) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;

		GSHOP_ITEM data = (*pListGShop3)[i];	
		unsigned int uid = data.local_id;
		CTemplateBase *pOldTempl = pTM_ElementGShop2->FindTemplate(uid,-1);
		if(pOldTempl!=NULL)
		{
			if(!pOldTempl->Export(&data)) 
			{
				g_Log.Log("failed to export Gshop2 %d", uid);
				return false;
			}
			(*pListGShop3)[i] = data;
		}
	}

	return true;
}

bool CDataManager::_ExportGshopTypes(char *szDistPath, void *param, PROCESSPROC poc)
{
	vector<GSHOP_MAIN_TYPE> * pListGShopTypes = globaldata_getgshopmaintypes();
	if(pTM_ElementGShopTypes==NULL) 
		return true;

	int n = pListGShopTypes->size();
	for(int i = 0; i < n; ++i)
	{
		int per = int(((float)i/(float)n) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;
		
		GSHOP_MAIN_TYPE_PARAMS data;
		data.id = i;
		data.pMainType = &(*pListGShopTypes)[i];	
		unsigned int uid = i;
		CTemplateBase *pOldTempl = pTM_ElementGShopTypes->FindTemplate(uid,-1);
		if(pOldTempl!=NULL)
		{
			if(!pOldTempl->Export(&data))
			{
				g_Log.Log("failed to export GshopTypes %d", uid);
				return false;
			}
		}
	}
	return true;
}

bool CDataManager::_ExportGshopTypes1(char *szDistPath, void *param, PROCESSPROC poc)
{
	vector<GSHOP_MAIN_TYPE> * pListGShopTypes1 = globaldata_getgshopmaintypes2();
	if(pTM_ElementGShopTypes1==NULL)
		return true;

	int n2 = pListGShopTypes1->size();
	for(int i = 0; i < n2; ++i)
	{
		int per = int(((float)i/(float)n2) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;

		
		GSHOP_MAIN_TYPE_PARAMS data;
		data.id = i;
		data.pMainType = &(*pListGShopTypes1)[i];	
		unsigned int uid = i;
		CTemplateBase *pOldTempl = pTM_ElementGShopTypes1->FindTemplate(uid,-1);
		if(pOldTempl!=NULL)
		{
			if(!pOldTempl->Export(&data))
			{
				g_Log.Log("failed to export GshopTypes1 %d", uid);
				return false;
			}
		}
	}
	return true;
}

bool CDataManager::_ExportGshopTypes2(char *szDistPath, void *param, PROCESSPROC poc)
{
	vector<GSHOP_MAIN_TYPE> * pListGShopTypes2 = globaldata_getgshopmaintypes3();
	if(pTM_ElementGShopTypes2==NULL) 
		return true;

	int n3 = pListGShopTypes2->size();
	for(int i = 0; i < n3; ++i)
	{
		int per = int(((float)i/(float)n3) * 100); 
			if(!(*poc)(param,per,g_nTotalPer)) return false;
		
		GSHOP_MAIN_TYPE_PARAMS data;
		data.id = i;
		data.pMainType = &(*pListGShopTypes2)[i];	
		unsigned int uid = i;
		CTemplateBase *pOldTempl = pTM_ElementGShopTypes2->FindTemplate(uid,-1);
		if(pOldTempl!=NULL)
		{
			if(!pOldTempl->Export(&data))
			{
				g_Log.Log("failed to export GshopTypes2 %d", uid);
				return false;
			}
		}
	}
	return true;
}

bool CDataManager::ExportDomainData(char *szDistPath, void *param, PROCESSPROC poc)
{
	if(!globaldata_load()) 
	{
		g_Log.Log("Faild to call globaldata_load() !");
		return false;
	}
	
	bool bSuc = false;
	do {
	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_DOMAININFOR) && !_ExportDomainInfor(szDistPath, param, poc))
		break;
	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_DOMAININFOR1) && !_ExportDomainInfor1(szDistPath, param, poc))
		break;
	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_GSHOP) && !_ExportGshop(szDistPath, param, poc))
		break;
	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_GSHOP1) && !_ExportGshop1(szDistPath, param, poc))
		break;
	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_GSHOP2) && !_ExportGshop2(szDistPath, param, poc))
		break;
	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_GSHOP_TYPES) && !_ExportGshopTypes(szDistPath, param, poc))
		break;
	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_GSHOP_TYPES1) && !_ExportGshopTypes1(szDistPath, param, poc))
		break;
	if (m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_GSHOP_TYPES2) && !_ExportGshopTypes2(szDistPath, param, poc))
		break;
	bSuc = true; 
	} while(0);

	if (bSuc)
	{
		char szPath[MAX_PATH];
		sprintf(szPath,"%s\\", szDistPath);
		if(!globaldata_save_to(szPath)) 
		{
			g_Log.Log("Faild to call globaldata_save_to()!");
			bSuc = false;
		}
	}

	globaldata_release();	
	return bSuc;
}


bool CDataManager::Export(char *szDistPath,void *param, PROCESSPROC poc)
{
	int nTotal,nDisuse,nNew,nTrans;
	Stat(nTotal,nDisuse,nNew,nTrans);
	if(nNew > 0) AfxMessageBox(_T("Warning! Some of texts need  translate or translate again.  The texts prefixed by \"!\" will not be exported."));

	
	EnumModifiedFileAttr(szDistPath);
	g_nTotalPer = 5;
	
	if(m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_DATA))
	if(!ExportElementData(szDistPath, param, poc)) 
	{
		AfxMessageBox(_T("Faild to export element data!"));
		return false;
	}
	g_nTotalPer = 20;
	if(m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_TASK))
	if(!ExportTaskData(szDistPath, param, poc)) 
	{
		AfxMessageBox(_T("Faild to export task data!"));
		return false;
	}
	g_nTotalPer = 30;
	if(m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_ITEMDESC))
	if(!ExportItemDescData(szDistPath, param, poc)) 
	{
		AfxMessageBox(_T("Faild to export item description data!"));
		return false;
	}

	if(m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_INSTANCE_TEXT))
	if(!ExportInstanceText(szDistPath, param, poc)) 
	{
		AfxMessageBox(_T("Faild to export instance text data!"));
		return false;
	}
	
	g_nTotalPer = 40;
	if(m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_ITEMEXDESC))
	if(!ExportItemExDescData(szDistPath, param, poc)) 
	{
		AfxMessageBox(_T("Faild to export item extend description data!"));
		return false;
	}

	g_nTotalPer = 50;
	if(m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_SKILLDESC))
	if(!ExportItemSkillDescData(szDistPath, param, poc)) 
	{
		AfxMessageBox(_T("Faild to export skill description data!"));
		return false;
	}
	g_nTotalPer = 60;
	if(m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_FIXMSG))
	if(!ExportItemFixMsgData(szDistPath, param, poc)) 
	{
		AfxMessageBox(_T("Faild to export fix message data!"));
		return false;
	}
	g_nTotalPer = 70;
	if(m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_SERVERERR))
	if(!ExportItemServerErrData(szDistPath, param, poc)) 
	{
		AfxMessageBox(_T("Faild to export server error message data!"));
		return false;
	}
	if(m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_BUFFDESC))
	if(!ExportItemBuffDescData(szDistPath, param, poc)) 
	{
		AfxMessageBox(_T("Faild to export buff description data!"));
		return false;
	}
	if(m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_TASKERR))
	if(!ExportItemTaskErrData(szDistPath, param, poc)) 
	{
		AfxMessageBox(_T("Faild to export task error data!"));
		return false;
	}

	if(m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_LOADINGHINT))
	if(!ExportItemLoadingHintData(szDistPath, param, poc)) 
	{
		AfxMessageBox(_T("Faild to export loading hint data!"));
		return false;
	}

	if(m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_PETTALK))
	if(!ExportItemPetTalkData(szDistPath, param, poc)) 
	{
		AfxMessageBox(_T("Faild to export pet talk data!"));
		return false;
	}

	if(m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_ADDONSTR))
	if(!ExportItemAddonStrData(szDistPath, param, poc)) 
	{
		AfxMessageBox(_T("Faild to export addon string data!"));
		return false;
	}
	
	g_nTotalPer = 80;
	if(m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_INTERFACESTRING) || 
		m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_INTERFACEHINT) ||
		m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_HELPSCRIPT) ||
		m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_HELPTEXT))
	if(!ExportInterfaceData(szDistPath, param, poc)) 
	{
		AfxMessageBox(_T("Faild to export interface data!"));
		return false;
	}
	g_nTotalPer = 90;
	if(m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_POLICY))
	if(!ExportPolicyData(szDistPath, param, poc)) 
	{
		AfxMessageBox(_T("Faild to export policy data!"));
		return false;
	}
	g_nTotalPer = 95;
	if(m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_PRECINCT))
	if(!ExportPrecinctData(szDistPath,param,poc)) 
	{
		AfxMessageBox(_T("Faild to export precinct data!"));
		return false;
	}
	g_nTotalPer = 99;
	if(m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_GSHOP) || 
		m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_GSHOP1) || 
		m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_GSHOP2) || 
		m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_GSHOP_TYPES) ||
		m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_GSHOP_TYPES1) ||
		m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_GSHOP_TYPES2) ||
		m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_DOMAININFOR) ||
		m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_DOMAININFOR1))
	if(!ExportDomainData(szDistPath,param,poc)) 
	{
		AfxMessageBox(_T("Faild to export domain data!"));
		return false;
	}
	if(m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_TEXTFILE))
	if(!ExportTextFileData(szDistPath,param,poc)) 
	{
		AfxMessageBox(_T("Faild to export text file data!"));
		return false;
	}

	if(m_pDlgSelectItems->GetDataSwitch(CHUNK_ELEMENT_HOMETOWN))
	if(!ExportHomeTownData(szDistPath,param,poc)) 
	{
		AfxMessageBox(_T("Faild to export hometown file data!"));
		return false;
	}
	
	return true;
}

bool CDataManager::Load(const TCHAR* szFile)
{
	Release();
	
	FILE *pFile = _tfopen(szFile,_T("rb"));
	if(pFile)
	{
		DWORD dwIdentify;
		DWORD dwVersion;
		int nChunkType;
		
		fread(&dwIdentify,sizeof(DWORD),1,pFile);
		if(dwIdentify!=LOCALIZE_IDENTIFY) goto faild;

		fread(&dwVersion,sizeof(DWORD),1,pFile);
		if(dwVersion > LOCALIZE_VERSION) goto faild;
		m_dwVersion = dwVersion;
		pTM_ElementTask = new CTM_TaskData(CHUNK_ELEMENT_TASK);
		pTM_ElementData = new CTM_ElementData(CHUNK_ELEMENT_DATA);
		pTM_ElementItemDesc = new CTM_OthersData(CHUNK_ELEMENT_ITEMDESC);
		pTM_ElementInstaceText = new CTM_InstanceData(CHUNK_ELEMENT_INSTANCE_TEXT);
		pTM_ElementItemExDesc = new CTM_OthersData(CHUNK_ELEMENT_ITEMEXDESC);
		pTM_ElementSkillDesc = new CTM_OthersData(CHUNK_ELEMENT_SKILLDESC);
		pTM_ElementFixMsg = new CTM_MessageData(CHUNK_ELEMENT_FIXMSG);
		pTM_ElementServerErr = new CTM_MessageData(CHUNK_ELEMENT_SERVERERR);
		pTM_ElementBuffDesc = new CTM_MessageData(CHUNK_ELEMENT_BUFFDESC);
		pTM_ElementTaskErr  = new CTM_MessageData(CHUNK_ELEMENT_TASKERR);
		pTM_ElementLoadingHint  = new CTM_MessageData(CHUNK_ELEMENT_LOADINGHINT);
		pTM_ElementPetTalk  = new CTM_MessageData(CHUNK_ELEMENT_PETTALK);
		pTM_ElementAddonStr  = new CTM_MessageData(CHUNK_ELEMENT_ADDONSTR);		
		pTM_ElementInterfaceHint = new CTM_InterfaceHintData(CHUNK_ELEMENT_INTERFACEHINT);
		pTM_ElementTextFile = new CTM_TextFile(CHUNK_ELEMENT_TEXTFILE);
		pTM_ElementInterfaceString = new CTM_InterfaceStringData(CHUNK_ELEMENT_INTERFACESTRING);
		pTM_ElementPolicy = new CTM_PolicyData(CHUNK_ELEMENT_POLICY);
		pTM_ElementPrecinct = new CTM_PrecinctData(CHUNK_ELEMENT_PRECINCT);
		pTM_ElementHelpScript = new CTM_InterfaceHintData(CHUNK_ELEMENT_HELPSCRIPT);
		pTM_ElementHelpText = new CTM_InterfaceHintData(CHUNK_ELEMENT_HELPTEXT);
		pTM_ElementDomainInfor = new CTM_DomainInforData(CHUNK_ELEMENT_DOMAININFOR);
		pTM_ElementDomainInfor1 = new CTM_DomainInforData(CHUNK_ELEMENT_DOMAININFOR1);
		pTM_ElementGShop       = new CTM_GShopData(CHUNK_ELEMENT_GSHOP);
		pTM_ElementGShopTypes  = new CTM_GShopTypes(CHUNK_ELEMENT_GSHOP_TYPES);

		pTM_ElementGShop1       = new CTM_GShopData(CHUNK_ELEMENT_GSHOP1);
		pTM_ElementGShopTypes1  = new CTM_GShopTypes(CHUNK_ELEMENT_GSHOP_TYPES1);
		pTM_ElementGShop2       = new CTM_GShopData(CHUNK_ELEMENT_GSHOP2);
		pTM_ElementGShopTypes2  = new CTM_GShopTypes(CHUNK_ELEMENT_GSHOP_TYPES2);
		pTM_ElementHomeTownData = new CTM_HomeTownData(CHUNK_ELEMENT_HOMETOWN);
		while(!feof(pFile))
		{
			fread(&nChunkType,sizeof(int),1,pFile);

			switch(nChunkType) 
			{
			case CHUNK_ELEMENT_DATA:
				if(pTM_ElementData)
				if(!pTM_ElementData->Load(pFile,dwVersion)) goto faild;
				break;
			case CHUNK_ELEMENT_TASK:
				if(pTM_ElementTask)
				if(!pTM_ElementTask->Load(pFile,dwVersion)) goto faild;
				break;
			case CHUNK_ELEMENT_POLICY:
				if(pTM_ElementPolicy)
				if(!pTM_ElementPolicy->Load(pFile,dwVersion)) goto faild;
				break;
			case CHUNK_ELEMENT_INTERFACEHINT:
				if(pTM_ElementInterfaceHint)
				if(!pTM_ElementInterfaceHint->Load(pFile,dwVersion)) goto faild;
				break;
			case CHUNK_ELEMENT_TEXTFILE:
				if(pTM_ElementTextFile)
				if(!pTM_ElementTextFile->Load(pFile,dwVersion)) goto faild;
				break;
			case CHUNK_ELEMENT_INTERFACESTRING:
				if(pTM_ElementInterfaceString)
				if(!pTM_ElementInterfaceString->Load(pFile,dwVersion)) goto faild;
				break;
			case CHUNK_ELEMENT_ITEMDESC:
				if(pTM_ElementItemDesc)
				if(!pTM_ElementItemDesc->Load(pFile,dwVersion)) goto faild;
				break;
			case CHUNK_ELEMENT_INSTANCE_TEXT:
				if(pTM_ElementInstaceText)
				if(!pTM_ElementInstaceText->Load(pFile,dwVersion)) goto faild;
				break;
			case CHUNK_ELEMENT_ITEMEXDESC:
				if(pTM_ElementItemExDesc)
				if(!pTM_ElementItemExDesc->Load(pFile,dwVersion)) goto faild;
				break;
			case CHUNK_ELEMENT_SKILLDESC:
				if(pTM_ElementSkillDesc)
				if(!pTM_ElementSkillDesc->Load(pFile,dwVersion)) goto faild;
				break;
			case CHUNK_ELEMENT_FIXMSG:
				if(pTM_ElementFixMsg)
				if(!pTM_ElementFixMsg->Load(pFile,dwVersion)) goto faild;
				break;
			case CHUNK_ELEMENT_SERVERERR:
				if(pTM_ElementServerErr)
				if(!pTM_ElementServerErr->Load(pFile,dwVersion)) goto faild;
				break;
			case CHUNK_ELEMENT_BUFFDESC:
				if(pTM_ElementBuffDesc)
				if(!pTM_ElementBuffDesc->Load(pFile,dwVersion)) goto faild;
				break;
			case CHUNK_ELEMENT_TASKERR:
				if(pTM_ElementTaskErr)
				if(!pTM_ElementTaskErr->Load(pFile,dwVersion)) goto faild;
				break;
			case CHUNK_ELEMENT_LOADINGHINT:
				if(pTM_ElementLoadingHint)
				if(!pTM_ElementLoadingHint->Load(pFile,dwVersion)) goto faild;
				break;
			case CHUNK_ELEMENT_PETTALK:
				if(pTM_ElementPetTalk)
				if(!pTM_ElementPetTalk->Load(pFile,dwVersion)) goto faild;
				break;
			case CHUNK_ELEMENT_ADDONSTR:
				if(pTM_ElementAddonStr)
				if(!pTM_ElementAddonStr->Load(pFile,dwVersion)) goto faild;
				break;
			case CHUNK_ELEMENT_PRECINCT:
				if(pTM_ElementPrecinct)
				if(!pTM_ElementPrecinct->Load(pFile,dwVersion)) goto faild;
				break;
			case CHUNK_ELEMENT_HELPSCRIPT:
				if(pTM_ElementHelpScript)
				if(!pTM_ElementHelpScript->Load(pFile,dwVersion)) goto faild;
				break;
			case CHUNK_ELEMENT_HELPTEXT:
				if(pTM_ElementHelpText)
				if(!pTM_ElementHelpText->Load(pFile,dwVersion)) goto faild;
				break;
			case CHUNK_ELEMENT_DOMAININFOR:
				if(pTM_ElementDomainInfor)
				if(!pTM_ElementDomainInfor->Load(pFile,dwVersion)) goto faild;
				break;
			case CHUNK_ELEMENT_DOMAININFOR1:
				if(pTM_ElementDomainInfor1)
				if(!pTM_ElementDomainInfor1->Load(pFile,dwVersion)) goto faild;
				break;
			case CHUNK_ELEMENT_GSHOP:
				if(pTM_ElementGShop)
				if(!pTM_ElementGShop->Load(pFile,dwVersion)) goto faild;
				break;
			case CHUNK_ELEMENT_GSHOP_TYPES:
				if(pTM_ElementGShopTypes)
				if(!pTM_ElementGShopTypes->Load(pFile,dwVersion)) goto faild;
				break;
			case CHUNK_ELEMENT_GSHOP1:
				if(pTM_ElementGShop1)
				if(!pTM_ElementGShop1->Load(pFile,dwVersion)) goto faild;
				break;
			case CHUNK_ELEMENT_GSHOP_TYPES1:
				if(pTM_ElementGShopTypes1)
				if(!pTM_ElementGShopTypes1->Load(pFile,dwVersion)) goto faild;
				break;
			case CHUNK_ELEMENT_HOMETOWN:
				if(pTM_ElementHomeTownData)
				if(!pTM_ElementHomeTownData->Load(pFile, dwVersion)) goto faild;
				break;
			case CHUNK_ELEMENT_GSHOP2:
				if(pTM_ElementGShop2)
				if(!pTM_ElementGShop2->Load(pFile,dwVersion)) goto faild;
				break;
			case CHUNK_ELEMENT_GSHOP_TYPES2:
				if(pTM_ElementGShopTypes2)
				if(!pTM_ElementGShopTypes2->Load(pFile,dwVersion)) goto faild;
				break;
			default:
				goto faild;
			}
		}
		
		fclose(pFile);
	}
	return true;

faild:
	fclose(pFile);
	return false;
}

bool CDataManager::Save(const TCHAR* szFile, CDlgSelectItems* pDlgOpt)
{
	if(m_dwVersion < 0x10000006)
	{
		AfxMessageBox(_T("You must import game data first, then save it !"));
		return false;
	}
	
	FILE *pFile = _tfopen(szFile,_T("wb"));
	if(pFile)
	{
		DWORD dwIdentify = LOCALIZE_IDENTIFY;
		DWORD dwVersion = LOCALIZE_VERSION;
		fwrite(&dwIdentify,sizeof(DWORD),1,pFile);
		fwrite(&dwVersion,sizeof(DWORD),1,pFile);

		if(pTM_ElementData && (!pDlgOpt || pDlgOpt->GetDataSwitch(CHUNK_ELEMENT_DATA))) 
		{
			int   nChunkType = CHUNK_ELEMENT_DATA;
			fwrite(&nChunkType,sizeof(int),1,pFile);
			pTM_ElementData->Save(pFile);
		}
		if(pTM_ElementTask && (!pDlgOpt || pDlgOpt->GetDataSwitch(CHUNK_ELEMENT_TASK))) 
		{
			int   nChunkType = CHUNK_ELEMENT_TASK;
			fwrite(&nChunkType,sizeof(int),1,pFile);
			pTM_ElementTask->Save(pFile);
		}
		if(pTM_ElementPolicy && (!pDlgOpt || pDlgOpt->GetDataSwitch(CHUNK_ELEMENT_POLICY))) 
		{
			int   nChunkType = CHUNK_ELEMENT_POLICY;
			fwrite(&nChunkType,sizeof(int),1,pFile);
			pTM_ElementPolicy->Save(pFile);
		}
		if(pTM_ElementInterfaceHint && (!pDlgOpt || pDlgOpt->GetDataSwitch(CHUNK_ELEMENT_INTERFACEHINT))) 
		{
			int   nChunkType = CHUNK_ELEMENT_INTERFACEHINT;
			fwrite(&nChunkType,sizeof(int),1,pFile);
			pTM_ElementInterfaceHint->Save(pFile);
		}
	
		if(pTM_ElementTextFile && (!pDlgOpt || pDlgOpt->GetDataSwitch(CHUNK_ELEMENT_TEXTFILE))) 
		{
			int   nChunkType = CHUNK_ELEMENT_TEXTFILE;
			fwrite(&nChunkType,sizeof(int),1,pFile);
			pTM_ElementTextFile->Save(pFile);
		}

		if(pTM_ElementInterfaceString && (!pDlgOpt || pDlgOpt->GetDataSwitch(CHUNK_ELEMENT_INTERFACESTRING))) 
		{
			int   nChunkType = CHUNK_ELEMENT_INTERFACESTRING;
			fwrite(&nChunkType,sizeof(int),1,pFile);
			pTM_ElementInterfaceString->Save(pFile);
		}
		if(pTM_ElementItemDesc && (!pDlgOpt || pDlgOpt->GetDataSwitch(CHUNK_ELEMENT_ITEMDESC))) 
		{
			int nChunkType = CHUNK_ELEMENT_ITEMDESC;
			fwrite(&nChunkType,sizeof(int),1,pFile);
			pTM_ElementItemDesc->Save(pFile);
		}

		if(pTM_ElementInstaceText && (!pDlgOpt || pDlgOpt->GetDataSwitch(CHUNK_ELEMENT_INSTANCE_TEXT))) 
		{
			int nChunkType = CHUNK_ELEMENT_INSTANCE_TEXT;
			fwrite(&nChunkType,sizeof(int),1,pFile);
			pTM_ElementInstaceText->Save(pFile);
		}

		if(pTM_ElementItemExDesc && (!pDlgOpt || pDlgOpt->GetDataSwitch(CHUNK_ELEMENT_ITEMEXDESC))) 
		{
			int nChunkType = CHUNK_ELEMENT_ITEMEXDESC;
			fwrite(&nChunkType,sizeof(int),1,pFile);
			pTM_ElementItemExDesc->Save(pFile);
		}
		if(pTM_ElementSkillDesc && (!pDlgOpt || pDlgOpt->GetDataSwitch(CHUNK_ELEMENT_SKILLDESC))) 
		{
			int nChunkType = CHUNK_ELEMENT_SKILLDESC;
			fwrite(&nChunkType,sizeof(int),1,pFile);
			pTM_ElementSkillDesc->Save(pFile);
		}
		if(pTM_ElementFixMsg && (!pDlgOpt || pDlgOpt->GetDataSwitch(CHUNK_ELEMENT_FIXMSG))) 
		{
			int nChunkType = CHUNK_ELEMENT_FIXMSG;
			fwrite(&nChunkType,sizeof(int),1,pFile);
			pTM_ElementFixMsg->Save(pFile);
		}
		if(pTM_ElementServerErr && (!pDlgOpt || pDlgOpt->GetDataSwitch(CHUNK_ELEMENT_SERVERERR))) 
		{
			int nChunkType = CHUNK_ELEMENT_SERVERERR;
			fwrite(&nChunkType,sizeof(int),1,pFile);
			pTM_ElementServerErr->Save(pFile);
		}
		if(pTM_ElementBuffDesc && (!pDlgOpt || pDlgOpt->GetDataSwitch(CHUNK_ELEMENT_BUFFDESC))) 
		{
			int nChunkType = CHUNK_ELEMENT_BUFFDESC;
			fwrite(&nChunkType,sizeof(int),1,pFile);
			pTM_ElementBuffDesc->Save(pFile);
		}

		if(pTM_ElementTaskErr && (!pDlgOpt || pDlgOpt->GetDataSwitch(CHUNK_ELEMENT_TASKERR))) 
		{
			int nChunkType = CHUNK_ELEMENT_TASKERR;
			fwrite(&nChunkType,sizeof(int),1,pFile);
			pTM_ElementTaskErr->Save(pFile);
		}

		if(pTM_ElementLoadingHint && (!pDlgOpt || pDlgOpt->GetDataSwitch(CHUNK_ELEMENT_LOADINGHINT))) 
		{
			int nChunkType = CHUNK_ELEMENT_LOADINGHINT;
			fwrite(&nChunkType,sizeof(int),1,pFile);
			pTM_ElementLoadingHint->Save(pFile);
		}

		if(pTM_ElementPetTalk && (!pDlgOpt || pDlgOpt->GetDataSwitch(CHUNK_ELEMENT_PETTALK))) 
		{
			int nChunkType = CHUNK_ELEMENT_PETTALK;
			fwrite(&nChunkType,sizeof(int),1,pFile);
			pTM_ElementPetTalk->Save(pFile);
		}

		if(pTM_ElementAddonStr && (!pDlgOpt || pDlgOpt->GetDataSwitch(CHUNK_ELEMENT_ADDONSTR))) 
		{
			int nChunkType = CHUNK_ELEMENT_ADDONSTR;
			fwrite(&nChunkType,sizeof(int),1,pFile);
			pTM_ElementAddonStr->Save(pFile);
		}		
		
		if(pTM_ElementPrecinct && (!pDlgOpt || pDlgOpt->GetDataSwitch(CHUNK_ELEMENT_PRECINCT))) 
		{
			int nChunkType = CHUNK_ELEMENT_PRECINCT;
			fwrite(&nChunkType,sizeof(int),1,pFile);
			pTM_ElementPrecinct->Save(pFile);
		}
		if(pTM_ElementHelpScript && (!pDlgOpt || pDlgOpt->GetDataSwitch(CHUNK_ELEMENT_HELPSCRIPT))) 
		{
			int nChunkType = CHUNK_ELEMENT_HELPSCRIPT;
			fwrite(&nChunkType,sizeof(int),1,pFile);
			pTM_ElementHelpScript->Save(pFile);
		}
		if(pTM_ElementHelpText && (!pDlgOpt || pDlgOpt->GetDataSwitch(CHUNK_ELEMENT_HELPTEXT))) 
		{
			int nChunkType = CHUNK_ELEMENT_HELPTEXT;
			fwrite(&nChunkType,sizeof(int),1,pFile);
			pTM_ElementHelpText->Save(pFile);
		}
	
		if(pTM_ElementDomainInfor && (!pDlgOpt || pDlgOpt->GetDataSwitch(CHUNK_ELEMENT_DOMAININFOR))) 
		{
			int nChunkType = CHUNK_ELEMENT_DOMAININFOR;
			fwrite(&nChunkType,sizeof(int),1,pFile);
			pTM_ElementDomainInfor->Save(pFile);
		}

		if(pTM_ElementDomainInfor1 && (!pDlgOpt || pDlgOpt->GetDataSwitch(CHUNK_ELEMENT_DOMAININFOR1))) 
		{
			int nChunkType = CHUNK_ELEMENT_DOMAININFOR1;
			fwrite(&nChunkType,sizeof(int),1,pFile);
			pTM_ElementDomainInfor1->Save(pFile);
		}

		if(pTM_ElementGShop && (!pDlgOpt || pDlgOpt->GetDataSwitch(CHUNK_ELEMENT_GSHOP))) 
		{
			int nChunkType = CHUNK_ELEMENT_GSHOP;
			fwrite(&nChunkType,sizeof(int),1,pFile);
			pTM_ElementGShop->Save(pFile);
		}
		if(pTM_ElementGShopTypes && (!pDlgOpt || pDlgOpt->GetDataSwitch(CHUNK_ELEMENT_GSHOP_TYPES))) 
		{

			int nChunkType = CHUNK_ELEMENT_GSHOP_TYPES;
			fwrite(&nChunkType,sizeof(int),1,pFile);
			pTM_ElementGShopTypes->Save(pFile);
		}

		if(pTM_ElementGShop1 && (!pDlgOpt || pDlgOpt->GetDataSwitch(CHUNK_ELEMENT_GSHOP1))) 
		{
			int nChunkType = CHUNK_ELEMENT_GSHOP1;
			fwrite(&nChunkType,sizeof(int),1,pFile);
			pTM_ElementGShop1->Save(pFile);
		}
		if(pTM_ElementGShopTypes1 && (!pDlgOpt || pDlgOpt->GetDataSwitch(CHUNK_ELEMENT_GSHOP_TYPES1))) 
		{

			int nChunkType = CHUNK_ELEMENT_GSHOP_TYPES1;
			fwrite(&nChunkType,sizeof(int),1,pFile);
			pTM_ElementGShopTypes1->Save(pFile);
		}
		if(pTM_ElementGShop2 && (!pDlgOpt || pDlgOpt->GetDataSwitch(CHUNK_ELEMENT_GSHOP2))) 
		{
			int nChunkType = CHUNK_ELEMENT_GSHOP2;
			fwrite(&nChunkType,sizeof(int),1,pFile);
			pTM_ElementGShop2->Save(pFile);
		}
		if(pTM_ElementGShopTypes2 && (!pDlgOpt || pDlgOpt->GetDataSwitch(CHUNK_ELEMENT_GSHOP_TYPES2))) 
		{
			int nChunkType = CHUNK_ELEMENT_GSHOP_TYPES2;
			fwrite(&nChunkType,sizeof(int),1,pFile);
			pTM_ElementGShopTypes2->Save(pFile);
		}
		if(pTM_ElementHomeTownData && (!pDlgOpt || pDlgOpt->GetDataSwitch(CHUNK_ELEMENT_HOMETOWN))) 
		{
			int nChunkType = CHUNK_ELEMENT_HOMETOWN;
			fwrite(&nChunkType,sizeof(int),1,pFile);
			pTM_ElementHomeTownData->Save(pFile);
		}
		
		
		fclose(pFile);
	}
	return true;
}

void CDataManager::EnumStatTemplate(CTemplateBase *pTemplate, int & nDisuse, int & nNew, int & nTrans)
{
	if(pTemplate==NULL) return;

	int n = pTemplate->GetTextNum();
	for( int i = 0; i < n; ++i)
	{
		CTextBase *pText = pTemplate->GetTextByIdx(i);
		if(pText->GetStatus()==CTextBase::status_disuse)
		{
			++nDisuse;
		}else if(pText->GetStatus()==CTextBase::status_new && pText->GetOriginalText() != NULL)
		{
			++nNew;
		}else ++nTrans;
	}

	n = pTemplate->GetChildNum();
	for( i = 0; i < n; ++i)
	{
		CTemplateBase *pChild = pTemplate->GetChildByIndex(i);
		EnumStatTemplate(pChild,nDisuse,nNew,nTrans);
	}
}

void CDataManager::Stat( int & nTotal, int & nDisuse, int & nNew, int & nTrans)
{
	nTotal = nDisuse = nTrans= nNew  = 0;

	if(pTM_ElementData)
	{
		int n = pTM_ElementData->GetTempalteNum();
		for( int i = 0; i < n; ++i)
		{
			CTemplateBase *pTempl = pTM_ElementData->GetTemplateByIdx(i);
			EnumStatTemplate(pTempl,nDisuse,nNew,nTrans);
		}
	}
	if(pTM_ElementTask)
	{
		int n = pTM_ElementTask->GetTempalteNum();
		for( int i = 0; i < n; ++i)
		{
			CTemplateBase *pTempl = pTM_ElementTask->GetTemplateByIdx(i);
			EnumStatTemplate(pTempl,nDisuse,nNew,nTrans);
		}
	}
	
	if(pTM_ElementPolicy)
	{
		int n = pTM_ElementPolicy->GetTempalteNum();
		for( int i = 0; i < n; ++i)
		{
			CTemplateBase *pTempl = pTM_ElementPolicy->GetTemplateByIdx(i);
			EnumStatTemplate(pTempl,nDisuse,nNew,nTrans);
		}
	}

	if(pTM_ElementInterfaceHint)
	{
		int n = pTM_ElementInterfaceHint->GetTempalteNum();
		for( int i = 0; i < n; ++i)
		{
			CTemplateBase *pTempl = pTM_ElementInterfaceHint->GetTemplateByIdx(i);
			EnumStatTemplate(pTempl,nDisuse,nNew,nTrans);
		}
	}

	if(pTM_ElementTextFile)
	{
		int n = pTM_ElementTextFile->GetTempalteNum();
		for( int i = 0; i < n; ++i)
		{
			CTemplateBase *pTempl = pTM_ElementTextFile->GetTemplateByIdx(i);
			EnumStatTemplate(pTempl,nDisuse,nNew,nTrans);
		}
	}

	if(pTM_ElementItemDesc)
	{
		int n = pTM_ElementItemDesc->GetTempalteNum();
		for( int i = 0; i < n; ++i)
		{
			CTemplateBase *pTempl = pTM_ElementItemDesc->GetTemplateByIdx(i);
			EnumStatTemplate(pTempl,nDisuse,nNew,nTrans);
		}
	}

	if(pTM_ElementInstaceText)
	{
		int n = pTM_ElementInstaceText->GetTempalteNum();
		for( int i = 0; i < n; ++i)
		{
			CTemplateBase *pTempl = pTM_ElementInstaceText->GetTemplateByIdx(i);
			EnumStatTemplate(pTempl,nDisuse,nNew,nTrans);
		}
	}

	if(pTM_ElementItemExDesc)
	{
		int n = pTM_ElementItemExDesc->GetTempalteNum();
		for( int i = 0; i < n; ++i)
		{
			CTemplateBase *pTempl = pTM_ElementItemExDesc->GetTemplateByIdx(i);
			EnumStatTemplate(pTempl,nDisuse,nNew,nTrans);
		}
	}

	if(pTM_ElementSkillDesc)
	{
		int n = pTM_ElementSkillDesc->GetTempalteNum();
		for( int i = 0; i < n; ++i)
		{
			CTemplateBase *pTempl = pTM_ElementSkillDesc->GetTemplateByIdx(i);
			EnumStatTemplate(pTempl,nDisuse,nNew,nTrans);
		}
	}

	if(pTM_ElementFixMsg)
	{
		int n = pTM_ElementFixMsg->GetTempalteNum();
		for( int i = 0; i < n; ++i)
		{
			CTemplateBase *pTempl = pTM_ElementFixMsg->GetTemplateByIdx(i);
			EnumStatTemplate(pTempl,nDisuse,nNew,nTrans);
		}
	}

	if(pTM_ElementServerErr)
	{
		int n = pTM_ElementServerErr->GetTempalteNum();
		for( int i = 0; i < n; ++i)
		{
			CTemplateBase *pTempl = pTM_ElementServerErr->GetTemplateByIdx(i);
			EnumStatTemplate(pTempl,nDisuse,nNew,nTrans);
		}
	}

	if(pTM_ElementBuffDesc)
	{
		int n = pTM_ElementBuffDesc->GetTempalteNum();
		for( int i = 0; i < n; ++i)
		{
			CTemplateBase *pTempl = pTM_ElementBuffDesc->GetTemplateByIdx(i);
			EnumStatTemplate(pTempl,nDisuse,nNew,nTrans);
		}
	}

	if(pTM_ElementTaskErr)
	{
		int n = pTM_ElementTaskErr->GetTempalteNum();
		for( int i = 0; i < n; ++i)
		{
			CTemplateBase *pTempl = pTM_ElementTaskErr->GetTemplateByIdx(i);
			EnumStatTemplate(pTempl,nDisuse,nNew,nTrans);
		}
	}

	if(pTM_ElementLoadingHint)
	{
		int n = pTM_ElementLoadingHint->GetTempalteNum();
		for( int i = 0; i < n; ++i)
		{
			CTemplateBase *pTempl = pTM_ElementLoadingHint->GetTemplateByIdx(i);
			EnumStatTemplate(pTempl,nDisuse,nNew,nTrans);
		}
	}

	if(pTM_ElementPetTalk)
	{
		int n = pTM_ElementPetTalk->GetTempalteNum();
		for( int i = 0; i < n; ++i)
		{
			CTemplateBase *pTempl = pTM_ElementPetTalk->GetTemplateByIdx(i);
			EnumStatTemplate(pTempl,nDisuse,nNew,nTrans);
		}
	}

	if(pTM_ElementAddonStr)
	{
		int n = pTM_ElementAddonStr->GetTempalteNum();
		for( int i = 0; i < n; ++i)
		{
			CTemplateBase *pTempl = pTM_ElementAddonStr->GetTemplateByIdx(i);
			EnumStatTemplate(pTempl,nDisuse,nNew,nTrans);
		}
	}	

	if(pTM_ElementInterfaceString)
	{
		int n = pTM_ElementInterfaceString->GetTempalteNum();
		for( int i = 0; i < n; ++i)
		{
			CTemplateBase *pTempl = pTM_ElementInterfaceString->GetTemplateByIdx(i);
			EnumStatTemplate(pTempl,nDisuse,nNew,nTrans);
		}
	}

	if(pTM_ElementPrecinct)
	{
		int n = pTM_ElementPrecinct->GetTempalteNum();
		for( int i = 0; i < n; ++i)
		{
			CTemplateBase *pTempl = pTM_ElementPrecinct->GetTemplateByIdx(i);
			EnumStatTemplate(pTempl,nDisuse,nNew,nTrans);
		}
	}

	if(pTM_ElementHelpScript)
	{
		int n = pTM_ElementHelpScript->GetTempalteNum();
		for( int i = 0; i < n; ++i)
		{
			CTemplateBase *pTempl = pTM_ElementHelpScript->GetTemplateByIdx(i);
			EnumStatTemplate(pTempl,nDisuse,nNew,nTrans);
		}
	}

	if(pTM_ElementHelpText)
	{
		int n = pTM_ElementHelpText->GetTempalteNum();
		for( int i = 0; i < n; ++i)
		{
			CTemplateBase *pTempl = pTM_ElementHelpText->GetTemplateByIdx(i);
			EnumStatTemplate(pTempl,nDisuse,nNew,nTrans);
		}
	}

	
	if(pTM_ElementDomainInfor)
	{
		int n = pTM_ElementDomainInfor->GetTempalteNum();
		for( int i = 0; i < n; ++i)
		{
			CTemplateBase *pTempl = pTM_ElementDomainInfor->GetTemplateByIdx(i);
			EnumStatTemplate(pTempl,nDisuse,nNew,nTrans);
		}
	}

	if(pTM_ElementDomainInfor1)
	{
		int n = pTM_ElementDomainInfor1->GetTempalteNum();
		for( int i = 0; i < n; ++i)
		{
			CTemplateBase *pTempl = pTM_ElementDomainInfor1->GetTemplateByIdx(i);
			EnumStatTemplate(pTempl,nDisuse,nNew,nTrans);
		}
	}

	if(pTM_ElementGShop)
	{
		int n = pTM_ElementGShop->GetTempalteNum();
		for( int i = 0; i < n; ++i)
		{
			CTemplateBase *pTempl = pTM_ElementGShop->GetTemplateByIdx(i);
			EnumStatTemplate(pTempl,nDisuse,nNew,nTrans);
		}
	}

	if(pTM_ElementGShopTypes)
	{
		int n = pTM_ElementGShopTypes->GetTempalteNum();
		for( int i = 0; i < n; ++i)
		{
			CTemplateBase *pTempl = pTM_ElementGShopTypes->GetTemplateByIdx(i);
			EnumStatTemplate(pTempl,nDisuse,nNew,nTrans);
		}
	}

	if(pTM_ElementGShop1)
	{
		int n = pTM_ElementGShop1->GetTempalteNum();
		for( int i = 0; i < n; ++i)
		{
			CTemplateBase *pTempl = pTM_ElementGShop1->GetTemplateByIdx(i);
			EnumStatTemplate(pTempl,nDisuse,nNew,nTrans);
		}
	}

	if(pTM_ElementGShopTypes1)
	{
		int n = pTM_ElementGShopTypes1->GetTempalteNum();
		for( int i = 0; i < n; ++i)
		{
			CTemplateBase *pTempl = pTM_ElementGShopTypes1->GetTemplateByIdx(i);
			EnumStatTemplate(pTempl,nDisuse,nNew,nTrans);
		}
	}

	if(pTM_ElementGShop2)
	{
		int n = pTM_ElementGShop2->GetTempalteNum();
		for( int i = 0; i < n; ++i)
		{
			CTemplateBase *pTempl = pTM_ElementGShop2->GetTemplateByIdx(i);
			EnumStatTemplate(pTempl,nDisuse,nNew,nTrans);
		}
	}

	if(pTM_ElementGShopTypes2)
	{
		int n = pTM_ElementGShopTypes2->GetTempalteNum();
		for( int i = 0; i < n; ++i)
		{
			CTemplateBase *pTempl = pTM_ElementGShopTypes2->GetTemplateByIdx(i);
			EnumStatTemplate(pTempl,nDisuse,nNew,nTrans);
		}
	}

	if(pTM_ElementHomeTownData)
	{
		int n = pTM_ElementHomeTownData->GetTempalteNum();
		for( int i = 0; i < n; ++i)
		{
			CTemplateBase *pTempl = pTM_ElementHomeTownData->GetTemplateByIdx(i);
			EnumStatTemplate(pTempl,nDisuse,nNew,nTrans);
		}
	}	

	nTotal = nDisuse + nTrans + nNew;
}

void EnumTansBig5(CTemplateBase *pTemplate)
{
	if(pTemplate==NULL) return;

	int n = pTemplate->GetTextNum();
	for( int i = 0; i < n; ++i)
	{
		CTextBase *pText = pTemplate->GetTextByIdx(i);
		if(pText->GetOriginalText() && pText->GetStatus() == CTextBase::status_new)
		{
			TCHAR *szBuffer = new TCHAR[_tcslen(pText->GetOriginalText()) + 1];
			j2b(pText->GetOriginalText(),szBuffer);
			pText->ReplaceTransText(szBuffer);
			g_bModified = true;
			pText->SetStatus(CTextBase::status_normal);
			if(szBuffer) delete[] szBuffer;
		}
	}

	n = pTemplate->GetChildNum();
	for( i = 0; i < n; ++i)
	{
		CTemplateBase *pChild = pTemplate->GetChildByIndex(i);
		EnumTansBig5(pChild);
	}
}

void EnumCopyOrgToTrans(CTemplateBase *pTemplate)
{
	if(pTemplate==NULL) return;
	
	int n = pTemplate->GetTextNum();
	for( int i = 0; i < n; ++i)
	{
		CTextBase *pText = pTemplate->GetTextByIdx(i);
		pText->ReplaceTransText(pText->GetOriginalText());
		if (pText->GetStatus() != CTextBase::status_disuse)
			pText->SetStatus(CTextBase::status_normal);
	}
	
	n = pTemplate->GetChildNum();
	for( i = 0; i < n; ++i)
	{
		CTemplateBase *pChild = pTemplate->GetChildByIndex(i);
		EnumCopyOrgToTrans(pChild);
	}
}

void EnumClearTrans(CTemplateBase *pTemplate)
{
	if(pTemplate==NULL) return;
	
	int n = pTemplate->GetTextNum();
	for( int i = 0; i < n; ++i)
	{
		CTextBase *pText = pTemplate->GetTextByIdx(i);
		TCHAR* pTrans = pText->GetTranslationText();
		if (pTrans)
		{
			pTrans[0] = 0;
			if (pText->GetStatus() != CTextBase::status_disuse)
				pText->SetStatus(CTextBase::status_new);
		}
	}
	
	n = pTemplate->GetChildNum();
	for( i = 0; i < n; ++i)
	{
		CTemplateBase *pChild = pTemplate->GetChildByIndex(i);
		EnumClearTrans(pChild);
	}
}

bool CDataManager::TranslateToBig5()
{
	return EnumOp(&EnumTansBig5);
}

bool CDataManager::CopyOrg2Trans()
{
	g_bModified = true;
	return EnumOp(&EnumCopyOrgToTrans);
}

bool CDataManager::ClearAllTrans()
{
	g_bModified = true;
	return EnumOp(&EnumClearTrans);
}

#define MAX_CHAR_IN_ONE_LINE 32



void GenCantImportOrder(const abase::vector<TEMP_TEXT>* vecNoReplace, const char* szPath)
{
	if (!vecNoReplace || vecNoReplace->empty())
		return;

	char szNewPath[MAX_PATH];
	int clen = strlen(szPath);
	for( int i = clen - 1; i > 0; --i)
	{
		if(szPath[i]=='\\') break;
	}
	memset(szNewPath,0,MAX_PATH);
	memcpy(szNewPath,szPath,i);
	
	sprintf(szNewPath,"%s\\CannotImportOdr.xml",szNewPath);
	CXmlLocalize file;
	if (!file.Open(szNewPath,CXmlLocalize::MODE_WRITE))
		return;
	
	for (int iPage = 0; iPage < CHUNK_NUM; ++iPage)
	{
		if (vecNoReplace[iPage].empty())
			continue;

		file.AddPage(g_szChunk[iPage]);
		for (size_t iRow=0; iRow<vecNoReplace[iPage].size(); ++iRow)
		{
			XmlLocItem item;
			item._items[XmlLocItem::eOriginItem] = vecNoReplace[iPage][iRow].strOrg;
			item._items[XmlLocItem::eTransItem] = vecNoReplace[iPage][iRow].strTrans;
			
			if (!file.WriteItem(item))
				return;
		}
	}
}


void GenCantImport(const abase::vector<TEMP_TEXT> vecNoReplace, const char* szPath)
{
	if(vecNoReplace.size() == 0)
		return;
	char szNewPath[MAX_PATH];
	int clen = strlen(szPath);
	for( int i = clen - 1; i > 0; --i)
	{
		if(szPath[i]=='\\') break;
	}
	memset(szNewPath,0,MAX_PATH);
	memcpy(szNewPath,szPath,i);
	
	sprintf(szNewPath,"%s\\CannotImport.xml",szNewPath);
	CXmlLocalize file;
	if (!file.Open(szNewPath,CXmlLocalize::MODE_WRITE))
		return;
	
	for (size_t iRow=0; iRow<vecNoReplace.size(); ++iRow)
	{
		XmlLocItem item;
		item._items[XmlLocItem::eOriginItem] = vecNoReplace[iRow].strOrg;
		item._items[XmlLocItem::eTransItem] = vecNoReplace[iRow].strTrans;
	
		if (!file.WriteItem(item))
			return;
	}
}


bool CDataManager::ExportAllText(const char *szPath, bool bOnlyExportUntrans, bool bIncludeID)
{
	THD_INFO thdInfox;
	THD_INFO* pThdInfo = &thdInfox;
	pThdInfo->bMsgUpdate = false;
	pThdInfo->iPercent = 0;
	pThdInfo->iThdState = THD_RUN;
	pThdInfo->szProgMsg[0] = 0;
	pThdInfo->bAbort = false;

	CDlgProgress dlg(pThdInfo);
	THD_EXPORTTEXT_INFO thdInfo = {this, szPath, bOnlyExportUntrans, bIncludeID, pThdInfo, (CElementDataView*)((CMainFrame*)AfxGetMainWnd())->GetLeftView()};
	HANDLE hThread = CreateThread(NULL, 0, ThdExportToText, &thdInfo, 0, NULL);
	int iRet = dlg.DoModal();
	CloseHandle(hThread);
	return (iRet==IDOK);
}


void CDataManager::EnumModifiedFileAttr(CString strName)
{

	CFileFind finder;
	CString fileName;
	CString strWildcard = strName;
	strWildcard +=_T("\\*.*");
	
	BOOL bWorking = finder.FindFile(strWildcard);
	int n = 0;
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		
		//是"."或是".." 跳过.
		if (finder.IsDots())
			continue;
		
		// 是文件夹，继续搜寻.
		if (finder.IsDirectory())
		{
			fileName = finder.GetFilePath();
			EnumModifiedFileAttr(fileName);
			continue;
		}
			
		// 是文件.
		fileName = finder.GetFilePath();
		DWORD flag = GetFileAttributes(fileName);
		if (flag & FILE_ATTRIBUTE_READONLY) 
		{
			flag ^= FILE_ATTRIBUTE_READONLY;
			if(!SetFileAttributes(fileName,flag))
				g_Log.Log("Faild to set file attributes (%s)",fileName);
		}
	}
	finder.Close();
}


DWORD _stdcall ThdImportFromOrderText(LPVOID lParam)
{
	THD_IMPORTORDERTEXT_INFO* pInfo = (THD_IMPORTORDERTEXT_INFO*) lParam;
	if (!pInfo || !pInfo->pThdInfo)
		return -1;

	const char* szPath = pInfo->szPath;
	CXmlLocalize file;
	if (!file.Open(szPath, CXmlLocalize::MODE_READ) || !pInfo->pMan || !pInfo->pView || !pInfo->pView->GetItems())
	{
		_tcscpy(pInfo->pThdInfo->szProgMsg, L"Invalid Parameters");
		pInfo->pThdInfo->iThdState = THD_FAIL;
		return -1;
	}

	CElementDataView* pView = pInfo->pView;
	vector<pair<HTREEITEM, BOOL> > vItems;
	GetCheckedItems(pView, pInfo->pMan->m_pDlgSelectItems, vItems);

	std::vector<BOOL> vFilter;
	for (int i=0; i<vItems.size(); ++i)
		vFilter.push_back(vItems[i].second);

	if (!file.LoadItems(pInfo->pThdInfo, vFilter))
		return -1;

	pInfo->pThdInfo->iPercent = 0;
	abase::vector<TEMP_TEXT> listNoReplaced[CHUNK_NUM];	
	for (i=0; i<vItems.size(); ++i)
	{
		pInfo->pThdInfo->iPercent = 0;
		if (!vItems[i].second)
			continue;

		const std::vector<XmlLocItem>* pData = file.GetItems(i);
		if (!pData)
			continue;
		
		_sntprintf(pInfo->pThdInfo->szProgMsg, 512, _T("Translating %s..."), g_szChunk[i]);
		pInfo->pThdInfo->bMsgUpdate = true;
		
		int index = 0;
		HTREEITEM hItem = vItems[i].first;
		while (!pInfo->pThdInfo->bAbort)
		{		
			hItem = pView->GetNextTextItem(hItem, vItems[i].first);
			if (!hItem || index >= pData->size())
				break;
			
			const XmlLocItem& item = (*pData)[index++];			
			pInfo->pThdInfo->iPercent = ((float)index / pData->size()) * 100;
			
			CTextBase* pText = (CTextBase*)(pView->m_treeData.GetItemData(hItem));
			if (!pText)
			{
				_tcscpy(pInfo->pThdInfo->szProgMsg, _T("exception: text null"));
				pInfo->pThdInfo->iThdState = THD_FAIL;
				return -1;
			}
			if (_tcscmp(item._items[XmlLocItem::eOriginItem], pText->GetOriginalText()) != 0)
			{
				TEMP_TEXT dat;
				dat.strOrg = item._items[XmlLocItem::eOriginItem];
				dat.strTrans = item._items[XmlLocItem::eTransItem];
				listNoReplaced[i].push_back(dat);
				continue;
			}
			
			pText->ReplaceTransText(item._items[XmlLocItem::eTransItem]);
		}
	}

	GenCantImportOrder(listNoReplaced, szPath);
	pInfo->pThdInfo->iThdState = THD_FIN;

	return 0;
}

DWORD _stdcall ThdImportFromText(LPVOID lParam)
{
	THD_IMPORTTEXT_INFO* pInfo = (THD_IMPORTTEXT_INFO*) lParam;
	if (!pInfo || !pInfo->pThdInfo)
		return -1;

	const char* szPath = pInfo->szPath;
	bool bAccordID = pInfo->bAccordID;
	CDataManager* pMan = pInfo->pMan;

	CXmlLocalize file;
	if(!file.Open(szPath, CXmlLocalize::MODE_READ) || !pMan)
	{
		_tcscpy(pInfo->pThdInfo->szProgMsg, L"Invalid Parameters");
		pInfo->pThdInfo->iThdState = THD_FAIL;
		return -1;
	}
	
	abase::vector<TEMP_TEXT> listNoReplaced;
	XmlLocItem item;
	while (file.ReadItem(item) && !pInfo->pThdInfo->bAbort)
	{
		pInfo->pThdInfo->iPercent = file.GetCurPercent();
		
		if (item._items[XmlLocItem::eOriginItem].GetLength() == 0 ||
			item._items[XmlLocItem::eTransItem].GetLength() == 0)
			continue;
	
		bool bReplaced = false;	
		bool bIncludeID = (bAccordID&&item._items[XmlLocItem::eIDItem].GetLength()!=0);
		int  iChunkType = -1;
		if (bIncludeID)
			iChunkType = _ttoi(item._items[XmlLocItem::eIDItem]);

#define IMPORT_TEXT(p,tp) \
		{\
			if (pMan->p && pMan->m_pDlgSelectItems->GetDataSwitch(tp) && ((bIncludeID&&iChunkType==tp) || !bIncludeID) && !pInfo->pThdInfo->bAbort)\
			{\
				int n = pMan->p->GetTempalteNum();\
				for( int i = 0; i < n && !pInfo->pThdInfo->bAbort; ++i)\
				{\
					if (pInfo->pThdInfo->bAbort)\
					{\
						pInfo->pThdInfo->iThdState = THD_ABORT;\
						return 0;\
					}\
					CTemplateBase *pTempl = pMan->p->GetTemplateByIdx(i);\
					pMan->EnumImportText(pTempl, &item, bReplaced);\
				}\
			}\
		}
		
		IMPORT_TEXT(pTM_ElementData,CHUNK_ELEMENT_DATA);
		IMPORT_TEXT(pTM_ElementTask,CHUNK_ELEMENT_TASK);
		IMPORT_TEXT(pTM_ElementPolicy,CHUNK_ELEMENT_POLICY);
		IMPORT_TEXT(pTM_ElementInterfaceHint,CHUNK_ELEMENT_INTERFACEHINT);
		IMPORT_TEXT(pTM_ElementInstaceText,CHUNK_ELEMENT_INSTANCE_TEXT);
		IMPORT_TEXT(pTM_ElementItemDesc,CHUNK_ELEMENT_ITEMDESC);
		IMPORT_TEXT(pTM_ElementItemExDesc,CHUNK_ELEMENT_ITEMEXDESC);
		IMPORT_TEXT(pTM_ElementSkillDesc,CHUNK_ELEMENT_SKILLDESC);
		IMPORT_TEXT(pTM_ElementFixMsg,CHUNK_ELEMENT_FIXMSG);
		IMPORT_TEXT(pTM_ElementServerErr,CHUNK_ELEMENT_SERVERERR);
		IMPORT_TEXT(pTM_ElementBuffDesc,CHUNK_ELEMENT_BUFFDESC);
		IMPORT_TEXT(pTM_ElementTaskErr,CHUNK_ELEMENT_TASKERR);
		IMPORT_TEXT(pTM_ElementLoadingHint,CHUNK_ELEMENT_LOADINGHINT);	
		IMPORT_TEXT(pTM_ElementPetTalk,CHUNK_ELEMENT_PETTALK);
		IMPORT_TEXT(pTM_ElementAddonStr,CHUNK_ELEMENT_ADDONSTR);
		IMPORT_TEXT(pTM_ElementInterfaceString,CHUNK_ELEMENT_INTERFACESTRING);
		IMPORT_TEXT(pTM_ElementPrecinct,CHUNK_ELEMENT_PRECINCT);
		IMPORT_TEXT(pTM_ElementHelpScript,CHUNK_ELEMENT_HELPSCRIPT);
		IMPORT_TEXT(pTM_ElementHelpText,CHUNK_ELEMENT_HELPTEXT);
		IMPORT_TEXT(pTM_ElementGShop,CHUNK_ELEMENT_GSHOP);
		IMPORT_TEXT(pTM_ElementGShopTypes,CHUNK_ELEMENT_GSHOP_TYPES);
		IMPORT_TEXT(pTM_ElementGShop1,CHUNK_ELEMENT_GSHOP1);
		IMPORT_TEXT(pTM_ElementGShopTypes1,CHUNK_ELEMENT_GSHOP_TYPES1);
		IMPORT_TEXT(pTM_ElementGShop2,CHUNK_ELEMENT_GSHOP2);
		IMPORT_TEXT(pTM_ElementGShopTypes2,CHUNK_ELEMENT_GSHOP_TYPES2);
		IMPORT_TEXT(pTM_ElementHomeTownData,CHUNK_ELEMENT_HOMETOWN);
		IMPORT_TEXT(pTM_ElementDomainInfor,CHUNK_ELEMENT_DOMAININFOR);
		IMPORT_TEXT(pTM_ElementDomainInfor1,CHUNK_ELEMENT_DOMAININFOR1);

		
		if(!bReplaced)
		{
			TEMP_TEXT dat;
			dat.strOrg = item._items[XmlLocItem::eOriginItem];
			dat.strTrans = item._items[XmlLocItem::eTransItem];
			listNoReplaced.push_back(dat);
		}
	}

	
	GenCantImport(listNoReplaced, szPath);

	pInfo->pThdInfo->iThdState = THD_FIN;
	return 0;
}



DWORD _stdcall ThdExportToText(LPVOID lParam)
{
	THD_EXPORTTEXT_INFO* pInfo = (THD_EXPORTTEXT_INFO*) lParam;
	if (!pInfo || !pInfo->pThdInfo)
		return -1;

	const char* szPath = pInfo->szPath;
	CXmlLocalize file;
	if (!file.Open(szPath, CXmlLocalize::MODE_WRITE) || !pInfo->pView)
	{
		_tcscpy(pInfo->pThdInfo->szProgMsg, L"Invalid Parameters");
		pInfo->pThdInfo->iThdState = THD_FAIL;
		return -1;
	}
	CElementDataView* pView = pInfo->pView;

	vector<pair<HTREEITEM, BOOL> > vItems;
	GetCheckedItems(pView, pInfo->pMan->m_pDlgSelectItems, vItems);
	int nTotal = pInfo->pView->GetTextItemNum();
	
	abase::vector<TEMP_TEXT> listNoReplaced;
	int nCurPos = 0;
	for (int i=0; i<vItems.size(); ++i)
	{
		if (!vItems[i].second)
			continue;

		HTREEITEM hItem = vItems[i].first;
		bool bAnyText = false;
		while (true)
		{
			if (pInfo->pThdInfo->bAbort)
			{
				pInfo->pThdInfo->iThdState = THD_ABORT;
				return 0;
			}
			hItem = pView->GetNextTextItem(hItem, vItems[i].first);
			if (!hItem)
				break;
						
			nCurPos++;
			pInfo->pThdInfo->iPercent = ((float)nCurPos / nTotal) * 100;

			CTextBase* pText = (CTextBase*)(pView->m_treeData.GetItemData(hItem));
			if(pInfo->bOnlyExportUntrans)
			{
				if(pText->GetStatus()!=CTextBase::status_new)
					continue;
			}

			if (!pText || !pText->GetTemplate())
			{
				_tcscpy(pInfo->pThdInfo->szProgMsg, _T("exception: text null"));
				pInfo->pThdInfo->iThdState = THD_FAIL;
				return -1;
			}
			
			XmlLocItem item;
			item._items[XmlLocItem::eLabelItem] = pText->GetTextLabel();
			item._items[XmlLocItem::eOriginItem] = pText->GetOriginalText();
			item._items[XmlLocItem::eLimitItem].Format(_T("Update Limit: %d"), pText->GetCharUpperLimit());
			item._items[XmlLocItem::eLinenumItem].Format(_T("Line Num: %d"), pText->GetOriginalTextLineNum());
			item._items[XmlLocItem::eTransItem] = pText->GetTranslationText();
			if (pInfo->bGenID)
				item._items[XmlLocItem::eIDItem] = pText->GetStrID();

			if (!bAnyText)
			{
				bAnyText = true;
				file.AddPage(g_szChunk[i]);
			}
			file.WriteItem(item);
			
		}
	}
	

	pInfo->pThdInfo->iThdState = THD_FIN;

	return 0;
}

inline void CDataManager::OpOnData(EnumFunc* pFunc, CTemplateManager* pData, int type)
{
	if (pFunc && pData && (!IsWindow(m_pDlgSelectItems->m_hWnd) || m_pDlgSelectItems->GetDataSwitch(type))) 
	{
		int n = pData->GetTempalteNum();
		for( int i = 0; i < n; ++i)
		{
			CTemplateBase *pTempl = pData->GetTemplateByIdx(i);
			pFunc(pTempl);
		}
	}
}

bool CDataManager::EnumOp(EnumFunc* pFunc)
{
	if (!pFunc)
		return false;

	OpOnData(pFunc, pTM_ElementData, CHUNK_ELEMENT_DATA);
	OpOnData(pFunc, pTM_ElementTask, CHUNK_ELEMENT_TASK);
	OpOnData(pFunc, pTM_ElementPolicy, CHUNK_ELEMENT_POLICY);
	OpOnData(pFunc, pTM_ElementInterfaceHint, CHUNK_ELEMENT_INTERFACEHINT);
	OpOnData(pFunc, pTM_ElementTextFile, CHUNK_ELEMENT_TEXTFILE);
	OpOnData(pFunc, pTM_ElementItemDesc, CHUNK_ELEMENT_ITEMDESC);
	OpOnData(pFunc, pTM_ElementInstaceText, CHUNK_ELEMENT_INSTANCE_TEXT);
	OpOnData(pFunc, pTM_ElementItemExDesc, CHUNK_ELEMENT_ITEMEXDESC);
	OpOnData(pFunc, pTM_ElementSkillDesc, CHUNK_ELEMENT_SKILLDESC);
	OpOnData(pFunc, pTM_ElementFixMsg, CHUNK_ELEMENT_FIXMSG);
	OpOnData(pFunc, pTM_ElementBuffDesc, CHUNK_ELEMENT_BUFFDESC);
	OpOnData(pFunc, pTM_ElementTaskErr, CHUNK_ELEMENT_TASKERR);
	OpOnData(pFunc, pTM_ElementLoadingHint, CHUNK_ELEMENT_LOADINGHINT);
	OpOnData(pFunc, pTM_ElementPetTalk, CHUNK_ELEMENT_PETTALK);
	OpOnData(pFunc, pTM_ElementAddonStr, CHUNK_ELEMENT_ADDONSTR);
	OpOnData(pFunc, pTM_ElementInterfaceString, CHUNK_ELEMENT_INTERFACESTRING);
	OpOnData(pFunc, pTM_ElementPrecinct, CHUNK_ELEMENT_PRECINCT);
	OpOnData(pFunc, pTM_ElementHelpScript, CHUNK_ELEMENT_HELPSCRIPT);
	OpOnData(pFunc, pTM_ElementHelpText, CHUNK_ELEMENT_HELPTEXT);
	OpOnData(pFunc, pTM_ElementDomainInfor, CHUNK_ELEMENT_DOMAININFOR);
	OpOnData(pFunc, pTM_ElementDomainInfor1, CHUNK_ELEMENT_DOMAININFOR1);
	OpOnData(pFunc, pTM_ElementGShop, CHUNK_ELEMENT_GSHOP);
	OpOnData(pFunc, pTM_ElementGShopTypes, CHUNK_ELEMENT_GSHOP_TYPES);
	OpOnData(pFunc, pTM_ElementGShop1, CHUNK_ELEMENT_GSHOP1);
	OpOnData(pFunc, pTM_ElementGShopTypes1, CHUNK_ELEMENT_GSHOP_TYPES1);
	OpOnData(pFunc, pTM_ElementGShop2, CHUNK_ELEMENT_GSHOP2);
	OpOnData(pFunc, pTM_ElementGShopTypes2, CHUNK_ELEMENT_GSHOP_TYPES2);
	OpOnData(pFunc, pTM_ElementHomeTownData, CHUNK_ELEMENT_HOMETOWN);

	return true;
}
