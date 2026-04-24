// File		: DlgCityBuilding.cpp
// Creator	: Guo Jie
// Date		: 2008/2/27

#include "AUI\\AUIListBox.h"
#include "DlgCityBuilding.h"
#include "DlgHost.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_GPDataType.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_Faction.h"
#include "elementdataman.h"
#include "ids.hxx"

////////////////////////////////////////////////////////////////
// Macros
////////////////////////////////////////////////////////////////
#define new A_DEBUG_NEW

static const A3DCOLOR MSGBOX_COLOR = A3DCOLORRGBA(255, 255, 255, 160);

static const char DAMAGE_STR[]	= "Damage";
static const char DEFENCE_STR[] = "Defence";
static const char RANGE_STR[]	= "Range";
static const char SKILL_STR[]	= "Skill";
static const char HP_STR[]		= "Hp";

static const char TOWER_STR[]	= "Tower";
static const char GATE_STR[]	= "Gate";
static const int TOWER_STR_LEN	= 5;
static const int GATE_STR_LEN	= 4;

static const ACHAR* TOWER_NAME	= NULL;
static const ACHAR* GATE_NAME	= NULL;

static const ACHAR EMPTY_STR[]	= _AL("---");

static const int BUILDING_MAX_HP = 6000000;

static const int REFRESH_ALL_CONSINFO_INTRERVAL = 67000;
static const int REFRESH_SINGLE_CONSINFO_INTRERVAL = 33000;

/********************************************
塔为01—07，
01对应NPC的唯一ID为14254，对应界面是Tower01，
02对应NPC的唯一ID为14255，对应界面是Tower02，
03对应NPC的唯一ID为14256，对应界面是Tower03，
04对应NPC的唯一ID为14257，对应界面是Tower04，
05对应NPC的唯一ID为14260，对应界面是Tower05，
06对应NPC的唯一ID为14259，对应界面是Tower06，
07对应NPC的唯一ID为14258，对应界面是Tower07
********************************************
门为08—10，
08对应NPC的唯一ID为14247，对应界面是Gate01，
09对应NPC的唯一ID为14246，对应界面是Gate02，
10对应NPC的唯一ID为14245，对应界面是Gate03，
********************************************/
static const int TOWER_NUM	= 7;
static const int GATE_NUM	= 3;
static const int BUILDING_ID[TOWER_NUM + GATE_NUM] = {
	14254, 14255, 14256, 14257, 14250, 14249, 14248, 
	14247, 14246, 14245
};

////////////////////////////////////////////////////////////////
// CMD/EVENT Mappings
////////////////////////////////////////////////////////////////
AUI_BEGIN_COMMAND_MAP(CDlgCityBuilding, CDlgBase)
// {
AUI_ON_COMMAND("Cons*",			OnCommandConstruct)
AUI_ON_COMMAND("DonateRes*",	OnCommandDonateRes)
AUI_ON_COMMAND("Upgrade*",		OnCommandUpgrade)
AUI_ON_COMMAND("PrevPage",		OnCommandPrevPage)
AUI_ON_COMMAND("NextPage",		OnCommandNextPage)
AUI_ON_COMMAND("RefreshList",	OnCommandRefreshList)
// }
AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgCityBuilding, CDlgBase)
// {
AUI_ON_EVENT("Img_*",	WM_LBUTTONDOWN,	OnEventLButtonDown_Icon)
// }
AUI_END_EVENT_MAP()

////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////
CDlgCityBuilding::CDlgCityBuilding()
{
	m_pConsInfo = NULL;
	m_pWarRoleCfgs = NULL;
	m_lastRefreshAllConsInfoTime = 0;
	m_lastRefreshSingleConsInfoTime = 0;
	m_curPage = m_maxPage = -1;
	m_curSelectedBuilding = -1;
	m_bInBuildService = false;
	m_idRes[0] = m_idRes[1] = 0;
}

CDlgCityBuilding::~CDlgCityBuilding()
{
	SAFE_DELETE(m_pConsInfo);
	SAFE_DELETE_ARRAY(m_pWarRoleCfgs);
}

bool CDlgCityBuilding::OnInitDialog()
{
	const SPECIAL_ID_CONFIG& ids = GetGame()->GetGameRun()->GetSpecialIDConfig();
	m_idRes[0] = ids.id_war_material1;
	m_idRes[1] = ids.id_war_material2;

	TOWER_NAME = GetStringFromTable(4111);
	GATE_NAME = GetStringFromTable(4112);

	if (stricmp(m_szName, "Win_CityBuilding") == 0) {

		// Tower/Gate Current Info
		m_pConsInfo = new S2C::war_construction_info;
		memset(m_pConsInfo, 0, sizeof(m_pConsInfo));
		m_pConsInfo->count = TOWER_NUM + GATE_NUM;
		m_pConsInfo->nodes = new S2C::war_construction_info::node_t[m_pConsInfo->count];
		memset(m_pConsInfo->nodes, 0, m_pConsInfo->count * sizeof(m_pConsInfo->nodes[0]));

		// Tower/Gate NPC Essence
		DATA_TYPE dt;
		m_pWarRoleCfgs = new WAR_ROLE_CONFIG*[TOWER_NUM + GATE_NUM];
		memset(m_pWarRoleCfgs, 0, (TOWER_NUM + GATE_NUM) * sizeof(m_pWarRoleCfgs[0]));
		elementdataman *pDataMan = GetGame()->GetElementDataMan();
		for (int i = 0; i < TOWER_NUM + GATE_NUM; ++i) {
			const NPC_ESSENCE* pEss = (NPC_ESSENCE*)pDataMan->get_data_ptr(
				BUILDING_ID[i], ID_SPACE_ESSENCE, dt);
			if (dt != DT_NPC_ESSENCE || !pEss) {
				assert(0);
				return false;
			}
			WAR_ROLE_CONFIG *pCfg = (WAR_ROLE_CONFIG*)pDataMan->get_data_ptr(
				pEss->war_role_config, ID_SPACE_ESSENCE, dt);
			if (dt != DT_WAR_ROLE_CONFIG || !pCfg) {
				assert(0);
				return false;
			}
			m_pWarRoleCfgs[i] = pCfg;

			// 初始化血量
			m_pConsInfo->nodes[i].attr.curHp_level = pCfg->init_hp;
		}

		FillCityDlg();
	}
	else if (stricmp(m_szName, "Win_CityTower") == 0 ||
		stricmp(m_szName, "Win_CityGate") == 0)
	{
		FillBuildingDlg(this);
	}

	return true;
}

// From NPC
void CDlgCityBuilding::OnCityBuildService()
{
	if (stricmp(m_szName, "Win_CityBuilding") != 0) {
		assert(0);
		return;
	}

	// 一定要在城战里才能建设
	if (!GetHostPlayer()->IsInWar()) {
		GetGameUIMan()->MessageBox("", GetStringFromTable(974), MB_OK, MSGBOX_COLOR);
		return;
	}
	// 是否为守城方成员（决定是否能捐献）
	const CECHostPlayer::WAR_INFO& warInfo = GetHostPlayer()->GetWarInfo();
	bool bIsOwnerFaction = (warInfo.iWarDefenceFID == GetHostPlayer()->GetFactionID());
	if (!bIsOwnerFaction) {
		GetGameUIMan()->MessageBox("", GetStringFromTable(964), MB_OK, MSGBOX_COLOR);
		return;
	}

	m_bInBuildService = true;
	GetGameSession()->war_get_construction_info(0, 0, -1); // 查看所有建筑信息
	m_lastRefreshAllConsInfoTime = a_GetTime();
}

void CDlgCityBuilding::OnBuyArcherService(const NPC_WAR_BUY_ARCHER_SERVICE* p)
{
	if (!p) return;

	if (p->item_wanted &&
		GetHostPlayer()->GetPack()->GetItemTotalNum(p->item_wanted) < 1)
	{ // 没有令牌
		GetGameUIMan()->MessageBox("", GetStringFromTable(975), MB_OK, MSGBOX_COLOR);
		return;
	}
	else if (p->price > 0 && GetHostPlayer()->GetMoneyAmount() < p->price)
	{ // 金钱不够
		GetGameUIMan()->MessageBox("", GetStringFromTable(947), MB_OK, MSGBOX_COLOR);
		return;
	}
	else if ((p->war_material_id==0 || p->war_material_id==1) && p->war_material_count > 0 &&
		GetHostPlayer()->GetPack()->GetItemTotalNum(m_idRes[p->war_material_id]) < p->war_material_count)
	{ // 没有足够物资
		GetGameUIMan()->MessageBox("", GetStringFromTable(968), MB_OK, MSGBOX_COLOR);
		return;
	}

	GetGameSession()->war_buy_archer();
}

void CDlgCityBuilding::OnGetConstructionInfo(const S2C::war_construction_info* p)
{
	if (stricmp(m_szName, "Win_CityBuilding") != 0) {
		assert(0);
		return;
	}
	// 物资信息
	m_pConsInfo->res_a = p->res_a;
	m_pConsInfo->res_b = p->res_b;
	PAUIDIALOG pGate = GetGameUIMan()->GetDialog("Win_CityGate");
	PAUIDIALOG pTower = GetGameUIMan()->GetDialog("Win_CityTower");
	if (!pGate || !pTower) {
		assert(0);
		return;
	}
	if (this->IsShow() || m_bInBuildService) FillCityResInfo();
	if (pTower->IsShow()) FillBuildingResInfo(pTower);
	if (pGate->IsShow()) FillBuildingResInfo(pGate);

	// 建筑属性
	int i;
	for (i = 0; i < p->count; ++i) {
		const S2C::war_construction_info::node_t& node = p->nodes[i];
		// 合法性检查
		if (node.key < 1 || node.key > TOWER_NUM + GATE_NUM) { // key从1到10
			assert(0);
			continue;
		}
		const WAR_ROLE_CONFIG* pCfg = m_pWarRoleCfgs[node.key - 1];
		if (!pCfg) {
			assert(0);
			continue;
		}
		if ((node.key <= TOWER_NUM && node.attr.attack_level >= pCfg->attack_max_lev) || 
			node.attr.defence_level >= pCfg->defence_max_lev ||
			(node.key <= TOWER_NUM && node.attr.range_level >= pCfg->range_max_lev) ||
			node.attr.ai_level >= pCfg->strategy_max_lev ||
			node.attr.curHp_level > BUILDING_MAX_HP)
		{
			//assert(0);
			//continue;
		}
		// 更新属性		
		m_pConsInfo->nodes[node.key - 1] = node;

		if (this->IsShow() && m_curSelectedBuilding == node.key)
			FillCitySelectedInfoArea();
		if (pTower->IsShow() && (int)pTower->GetData() == node.key)
			FillBuildingDlg(pTower);
		if (pGate->IsShow() && (int)pGate->GetData() == node.key)
			FillBuildingDlg(pGate);
	}

	if (m_bInBuildService) {
		GetGameSession()->war_get_construction_info(0, 1, 0); // 查看玩家捐献信息(第0页)
		m_bInBuildService = false;	
		if (!IsShow()) {
			// 是否为帮主/副帮主（决定是否能点建设按钮）
			int fRoleID = GetHostPlayer()->GetFRoleID();
			bool bIsMaster = (fRoleID == GNET::_R_MASTER || fRoleID == GNET::_R_VICEMASTER);
			FillCityConstructButton(bIsMaster);
			Show(true);
		}
	}
}

void CDlgCityBuilding::OnGetContributionInfo(const S2C::war_contribution_info* p)
{
	if (stricmp(m_szName, "Win_CityBuilding") != 0) {
		assert(0);
		return;
	}

	size_t i;
	ACString strText;
	PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("List_Member");
	pList->ResetContent();
	for (i = 0; i < p->count; ++i)
	{
		S2C::war_contribution_info::node_t& node = p->nodes[i];
		int playerId = node.player_id;
		const ACHAR *pszName = GetGameRun()->GetPlayerName(playerId, false);
		if (!pszName) 
			GetGameSession()->GetPlayerBriefInfo(1, &playerId, 2);
		else // Tick中不用再更新
			playerId = 0;

		strText.Format(_AL("%s\t%d"), 
			pszName ? pszName : EMPTY_STR,	node.res_a + node.res_b);
		pList->AddString(strText);
		pList->SetItemData(pList->GetCount() - 1, playerId,	0);
		pList->SetItemData(pList->GetCount() - 1, node.res_a + node.res_b,	1);
	}

	if (p->page >= 0) m_curPage = p->page;
	else assert(0);

	if (p->max_page >= p->page) m_maxPage = p->max_page;
	else assert(0);

	GetDlgItem("Btn_Prec")->Enable(m_curPage > 0);
	GetDlgItem("Btn_Next")->Enable(m_curPage < m_maxPage);
}

// Donation List
void CDlgCityBuilding::OnCommandPrevPage(const char* szCmd)
{
	if (m_curPage > 0) {
		GetGameSession()->war_get_construction_info(0, 1, --m_curPage);
		if (m_curPage == 0)
			GetDlgItem("Btn_Prec")->Enable(false);
	}
}
void CDlgCityBuilding::OnCommandNextPage(const char* szCmd)
{
	if (m_curPage < m_maxPage) {
		GetGameSession()->war_get_construction_info(0, 1, ++m_curPage);
		if (m_curPage > 0)
			GetDlgItem("Btn_Prec")->Enable(true);
	}
}
void CDlgCityBuilding::OnCommandRefreshList(const char* szCmd)
{
	// 重置为第一页
	m_curPage = m_maxPage = -1;
	GetDlgItem("Btn_Prec")->Enable(false);
	GetDlgItem("Btn_Next")->Enable(false);	
	// 刷新第一页
	GetGameSession()->war_get_construction_info(0, 1, 0);
}

// Construct Info
void CDlgCityBuilding::OnEventLButtonDown_Icon(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	const char* name = pObj->GetName();

	// Gate/Tower Index: 1, ..., 10
	int index = 0;
	AString strTmp(name + strlen("Img_"));
	if (stricmp(strTmp.Left(TOWER_STR_LEN), TOWER_STR) == 0) // "Img_Tower01"
	{
		index = atoi(strTmp.Mid(TOWER_STR_LEN));
		assert(index >= 1 && index <= TOWER_NUM);
	}
	else if (stricmp(strTmp.Left(GATE_STR_LEN), GATE_STR) == 0) // "Img_Gate01"
	{
		index = atoi(strTmp.Mid(GATE_STR_LEN));
		assert(index >= 1 && index <= GATE_NUM);
		index += TOWER_NUM;
	}
	else
	{
		assert(0);
		return;
	}
	m_curSelectedBuilding = index;

	FillCitySelectedInfoArea();
}

void CDlgCityBuilding::OnCommandConstruct(const char* szCmd)
{
	if (strlen(szCmd) < 10) { assert(0); return; }

	// Tower/Gate Index : 01 - 10
	int index = 0;
	PAUIDIALOG pDlg = NULL;
	AString strTmp(szCmd + strlen("Cons"));
	if (stricmp(strTmp.Left(TOWER_STR_LEN), TOWER_STR) == 0) // "ConsTower01"
	{
		index = atoi(strTmp.Mid(TOWER_STR_LEN));
		assert(index >= 1 && index <= TOWER_NUM);
		pDlg = GetGameUIMan()->GetDialog("Win_CityTower");
	}
	else if (stricmp(strTmp.Left(GATE_STR_LEN), GATE_STR) == 0) // "ConsGate01"
	{
		index = atoi(strTmp.Mid(GATE_STR_LEN));
		assert(index >= 1 && index <= GATE_NUM);
		index += TOWER_NUM;
		pDlg = GetGameUIMan()->GetDialog("Win_CityGate");
	}
	else
	{
		assert(0);
		return;
	}
	pDlg->SetData(index);	// index of this Tower/Gate
	if (m_curSelectedBuilding != index) {
		m_curSelectedBuilding = index;
		FillCitySelectedInfoArea();
	}

	// Fill the UI
	if (FillBuildingDlg(pDlg))
		pDlg->Show(true);
}

// Upgrade
void CDlgCityBuilding::OnCommandUpgrade(const char* szCmd)
{
	// szCmd: "UpgradeTowerDamage" ...
	// Gate/Tower Index: 1, ..., 10
	int index = this->GetData();
	if (index < 1 && index > TOWER_NUM + GATE_NUM) {
		assert(0);
		return;
	}

	AString postfix;
	AString strTmp(szCmd + strlen("Upgrade"));
	if (stricmp(strTmp.Left(TOWER_STR_LEN), TOWER_STR) == 0) // "Tower***"
		postfix = strTmp.Mid(TOWER_STR_LEN); // "Damage", "Defence", "Range", "Skill", "Hp"
	else if (stricmp(strTmp.Left(GATE_STR_LEN), GATE_STR) == 0) // "Gate***"
		postfix = strTmp.Mid(GATE_STR_LEN); // "Defence", "Skill", "Hp"
	else {
		assert(0);
		return;
	}
	int type = -1;
	if (stricmp(postfix, DAMAGE_STR) == 0) type = 0;
	else if (stricmp(postfix, DEFENCE_STR) == 0) type = 1;
	else if (stricmp(postfix, RANGE_STR) == 0) type = 2;
	else if (stricmp(postfix, SKILL_STR) == 0) type = 3;
	else if (stricmp(postfix, HP_STR) == 0) type = 4;
	else {
		assert(0);
		return;
	}

	int curLevel = GetDlgItem("Txt_" + postfix)->GetData(); // 当前等级（从0开始）
	int maxLevel = GetDlgItem("Btn_" + postfix)->GetData(); // 最大等级（从0开始）
	if (curLevel >= maxLevel) { // 已达到最高级别
		GetGameUIMan()->MessageBox("", GetStringFromTable(973), MB_OK, MSGBOX_COLOR);
		GetDlgItem("Btn_" + postfix)->Enable(false);
		return;
	}

	int resHave = 0; // 现有物资值
	int resNeed = GetDlgItem("Txt_" + postfix + "Need1")->GetData(); // 升级需要的物资值
	if (resNeed == 0) {
		resNeed = GetDlgItem("Txt_" + postfix + "Need2")->GetData();
		resHave = GetDlgItem("Txt_Resource02")->GetData(); 
	} else {
		resNeed = GetDlgItem("Txt_" + postfix + "Need1")->GetData();
		resHave = GetDlgItem("Txt_Resource01")->GetData(); 
	}	
	if (resHave < resNeed) { // 物资不够进行建设
		GetGameUIMan()->MessageBox("", GetStringFromTable(972), MB_OK, MSGBOX_COLOR);
		return;
	}

	// Upgrade ...
	GetGameSession()->war_get_construction_info(2/*mode为升级建筑*/, 
		type/*type表示升级哪种属性*/, index/*key从1到10*/, curLevel);
	m_lastRefreshSingleConsInfoTime = a_GetTime();
}

// Donate
void CDlgCityBuilding::OnCommandDonateRes(const char* szCmd)
{
	int index = atoi(szCmd+strlen("DonateRes"));
	if (index != 1 && index != 2) {
		assert(0);
		return;
	}

	CECInventory *pPack = GetHostPlayer()->GetPack();
	int count = pPack->GetItemTotalNum(m_idRes[index - 1]);
	if (count < 1) { // 物资不足
		GetGameUIMan()->MessageBox("", GetStringFromTable(968), MB_OK, MSGBOX_COLOR);
	} else { // 捐献物资
		GetGameSession()->war_get_construction_info(1, m_idRes[index - 1], 1/*key无意义，一次只能捐一个*/);
	}
}

/////////////////////////////////////////////////////////
// protected functions
/////////////////////////////////////////////////////////

// Tick
void CDlgCityBuilding::OnTick()
{
	if (stricmp(m_szName, "Win_CityBuilding") == 0)
	{
		int i;
		ACString strText;

		// 捐献列表
		PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("List_Member");
		for (i = 0; i < pList->GetCount(); ++i)
		{
			if (pList->GetItemData(i, 0) != 0) {
				const ACHAR *pszName = GetGameRun()->GetPlayerName(pList->GetItemData(i, 0), false);
				if (pszName) {
					strText.Format(_AL("%s\t%d"), pszName, pList->GetItemData(i, 1));
					pList->SetText(i, strText);
					pList->SetItemData(i, 0, 0); // 以后不用再更新
				}
			}
		}
//		// 在对话框活动时，每隔固定时间向服务器请求一次所有建筑信息
//		if (IsActive() &&
//			int(a_GetTime() - m_lastRefreshAllConsInfoTime) > REFRESH_ALL_CONSINFO_INTRERVAL)
//		{
//			GetGameSession()->war_get_construction_info(0, 0, -1); // 刷新所有建筑信息
//			m_lastRefreshAllConsInfoTime = a_GetTime();
//		}
	}
//	else if (stricmp(m_szName, "Win_CityTower")==0 || stricmp(m_szName, "Win_CityGate")==0)
//	{
//		// 在对话框活动时，每隔固定时间向服务器请求一次该建筑的信息
//		if (IsActive() &&
//			a_GetTime() - m_lastRefreshSingleConsInfoTime > REFRESH_SINGLE_CONSINFO_INTRERVAL)
//		{
//			GetGameSession()->war_get_construction_info(0, 0, this->GetData()); // 刷新该建筑的信息
//			m_lastRefreshSingleConsInfoTime = a_GetTime();
//		}
//	}
}

static inline float CalcBinomial(const float co[3], int x)
{
	return co[0]*x*x + co[1]*x + co[2];
}

bool CDlgCityBuilding::FillBuildingResInfo(PAUIDIALOG pDlg)
{
	if (!pDlg) {assert(0); return false;}
	CDlgCityBuilding* pCityBuilding = 
		(CDlgCityBuilding*)(GetGameUIMan()->GetDialog("Win_CityBuilding"));

	PAUIOBJECT pLab = NULL;
	ACString strText;
	// 物资1
	pLab = pDlg->GetDlgItem("Txt_Resource01");
	if (!pLab) {assert(0); return false;}
	pLab->SetData(pCityBuilding->m_pConsInfo->res_a);	
	strText.Format(_AL("%d"), pCityBuilding->m_pConsInfo->res_a);
	pLab->SetText(strText);
	// 物资2
	pLab = pDlg->GetDlgItem("Txt_Resource02");
	if (!pLab) {assert(0); return false;}
	pLab->SetData(pCityBuilding->m_pConsInfo->res_b);
	strText.Format(_AL("%d"), pCityBuilding->m_pConsInfo->res_b);
	pLab->SetText(strText);

	return true;
}

bool CDlgCityBuilding::FillBuildingDlg(PAUIDIALOG pDlg)
{
	// index : 01 - 10.	
	int index = pDlg->GetData();
	if (index == 0) // 0 表示尚未指定
		return false;
	else if (index < 1 || index > TOWER_NUM + GATE_NUM) {
		assert(0);
		return false;
	}

	CDlgCityBuilding* pCityBuilding = (CDlgCityBuilding*)(GetGameUIMan()->GetDialog("Win_CityBuilding"));
	// NPC essense
	// 此config中：攻击力/防御力max_level是20，射程/特殊技max_level是5，血量max_level是10000
	const WAR_ROLE_CONFIG* pCfg = pCityBuilding->m_pWarRoleCfgs[index - 1];
	if (!pCfg) { assert(0);	return false; }
	// Construction info
	// 此attr中：攻击力level是0-19，射程/特殊技level是0-4，血量level是0-9999
	const S2C::war_construction_info::node_t::attr_t& attr =
					pCityBuilding->m_pConsInfo->nodes[index - 1].attr;

	ACString strText;
	PAUIOBJECT pLab = NULL, pLab1 = NULL;

	// 建筑属性
	strText.Format(_AL("%s%d"), 
		index <= TOWER_NUM ? TOWER_NAME : GATE_NAME, 
		index <= TOWER_NUM ? index : index - TOWER_NUM);
	pLab = pDlg->GetDlgItem("Title");
	pLab->SetText(strText);

	int iLevel = 0, iValue = 0;
	if (index <= TOWER_NUM) {
		// 攻击力（等级0-19）
		iLevel = attr.attack_level;
		pDlg->GetDlgItem("Btn_Damage")->Enable(iLevel < pCfg->attack_max_lev - 1);
		pDlg->GetDlgItem("Btn_Damage")->SetData(pCfg->attack_max_lev - 1); // 攻击等级上限
		iValue = pCfg->attack_level[iLevel].attack_value;
		strText.Format(_AL("%d"), iValue);
		pLab = pDlg->GetDlgItem("Txt_Damage");
		pLab->SetData(iLevel);
		pLab->SetText(strText);
		// 升级所需物资
		pLab = pLab1 = NULL;
		if (pCfg->attack_war_material == 0) {
			pLab = pDlg->GetDlgItem("Txt_DamageNeed1");
			pLab1 = pDlg->GetDlgItem("Txt_DamageNeed2");
		} else if (pCfg->attack_war_material == 1) {
			pLab = pDlg->GetDlgItem("Txt_DamageNeed2");
			pLab1 = pDlg->GetDlgItem("Txt_DamageNeed1");
		} else {
			assert(0);
		}
		if (pLab) {
			iValue = (int)CalcBinomial(pCfg->attack_co, iLevel);
			strText.Format(_AL("%d"), iValue);
			pLab->SetData(iValue);
			pLab->SetText(strText);
		}
		if (pLab1) {
			pLab1->SetData(0);
			pLab1->SetText(EMPTY_STR);
		}
		// 下一级攻击力
		strText = EMPTY_STR;
		if (iLevel < pCfg->attack_max_lev - 1) {
			iValue = pCfg->attack_level[++iLevel].attack_value;
			strText.Format(_AL("%d"), iValue);
		}
		pLab = pDlg->GetDlgItem("Txt_DamageNext");
		pLab->SetData(iLevel);
		pLab->SetText(strText);
	}

	// 防御力（等级0-19）
	iLevel = attr.defence_level;
	pDlg->GetDlgItem("Btn_Defence")->Enable(iLevel < pCfg->defence_max_lev - 1);
	pDlg->GetDlgItem("Btn_Defence")->SetData(pCfg->defence_max_lev - 1); // 防御等级上限
	iValue = pCfg->defence_level[iLevel].defence_extra;
	strText.Format(_AL("%d"), iValue);
	pLab = pDlg->GetDlgItem("Txt_Defence");
	pLab->SetData(iLevel);
	pLab->SetText(strText);
	// 升级所需物资
	pLab = pLab1 = NULL;
	if (pCfg->defence_war_material == 0) { 
		pLab = pDlg->GetDlgItem("Txt_DefenceNeed1");
		pLab1 = pDlg->GetDlgItem("Txt_DefenceNeed2");

	} else if (pCfg->defence_war_material == 1) {
		pLab = pDlg->GetDlgItem("Txt_DefenceNeed2");
		pLab1 = pDlg->GetDlgItem("Txt_DefenceNeed1");
	} else {
		assert(0);
	}
	if (pLab) {
		iValue = (int)CalcBinomial(pCfg->defence_co, iLevel);
		strText.Format(_AL("%d"), iValue);
		pLab->SetData(iValue);
		pLab->SetText(strText);
	}
	if (pLab1) {
		pLab1->SetData(0);
		pLab1->SetText(EMPTY_STR);
	}
	// 下一级防御力
	strText = EMPTY_STR;
	if (iLevel < pCfg->defence_max_lev - 1) {
		iValue = pCfg->defence_level[++iLevel].defence_extra;
		strText.Format(_AL("%d"), iValue);
	}
	pLab = pDlg->GetDlgItem("Txt_DefenceNext");
	pLab->SetData(iLevel);
	pLab->SetText(strText);

	if (index <= TOWER_NUM) {
		// 射程等级（0-4）
		iLevel = attr.range_level;
		pDlg->GetDlgItem("Btn_Range")->Enable(iLevel < pCfg->range_max_lev - 1);
		pDlg->GetDlgItem("Btn_Range")->SetData(pCfg->attack_max_lev - 1); // 射程等级上限
		iValue = pCfg->range_values[iLevel];
		strText.Format(_AL("%d"), iValue);
		pLab = pDlg->GetDlgItem("Txt_Range");
		pLab->SetData(iLevel);
		pLab->SetText(strText);
		// 升级所需物资
		pLab = pLab1 = NULL;
		if (pCfg->range_war_material == 0) {
			pLab = pDlg->GetDlgItem("Txt_RangeNeed1");
			pLab1 = pDlg->GetDlgItem("Txt_RangeNeed2");
		} else if (pCfg->range_war_material == 1) {
			pLab = pDlg->GetDlgItem("Txt_RangeNeed2");
			pLab1 = pDlg->GetDlgItem("Txt_RangeNeed1");
		} else {
			assert(0);
		}
		if (pLab) {
			iValue = (int)CalcBinomial(pCfg->range_co, iLevel);
			strText.Format(_AL("%d"), iValue);
			pLab->SetData(iValue);
			pLab->SetText(strText);
		}
		if (pLab1) {
			pLab1->SetData(0);
			pLab1->SetText(EMPTY_STR);
		}
		// 下一级射程等级
		strText = EMPTY_STR;
		if (iLevel < pCfg->range_max_lev - 1) {
			iValue = pCfg->range_values[++iLevel];
			strText.Format(_AL("%d"), iValue);
		}
		pLab = pDlg->GetDlgItem("Txt_RangeNext");
		pLab->SetData(iLevel);
		pLab->SetText(strText);
	}

	// 特殊技能（0-4）
	iLevel = attr.ai_level;
	pDlg->GetDlgItem("Btn_Skill")->Enable(iLevel < pCfg->strategy_max_lev - 1);
	pDlg->GetDlgItem("Btn_Skill")->SetData(pCfg->strategy_max_lev - 1); // 特殊技能上限
	iValue = pCfg->strategy_id[iLevel];
	strText.Format(_AL("%s"), GetStringFromTable((index<=TOWER_NUM ? 990 : 995) + iLevel));
	pLab = pDlg->GetDlgItem("Txt_Skill");
	pLab->SetData(iLevel);
	pLab->SetText(strText);
	// 升级所需物资
	pLab = pLab1 = NULL;
	if (pCfg->strategy_war_material == 0) {
		pLab = pDlg->GetDlgItem("Txt_SkillNeed1");
		pLab1 = pDlg->GetDlgItem("Txt_SkillNeed2");
	} else if (pCfg->strategy_war_material == 1) {
		pLab = pDlg->GetDlgItem("Txt_SkillNeed2");
		pLab1 = pDlg->GetDlgItem("Txt_SkillNeed1");
	} else {
		assert(0);
	}
	if (pLab) {
		iValue = (int)CalcBinomial(pCfg->strategy_co, iLevel);
		strText.Format(_AL("%d"), iValue);
		pLab->SetData(iValue);
		pLab->SetText(strText);
	}
	if (pLab1) {
		pLab1->SetData(0);
		pLab1->SetText(EMPTY_STR);
	}
	// 下一级特殊技能
	strText = EMPTY_STR;
	if (iLevel < pCfg->strategy_max_lev - 1) {
		iValue = pCfg->strategy_id[++iLevel];
		strText.Format(_AL("%s"), GetStringFromTable((index<=TOWER_NUM ? 990 : 995) + iLevel));
	}
	pLab = pDlg->GetDlgItem("Txt_SkillNext");
	pLab->SetData(iLevel);
	pLab->SetText(strText);

	// 血量
	iLevel = attr.curHp_level; // TODO: verify
	pDlg->GetDlgItem("Btn_Hp")->Enable(iLevel < BUILDING_MAX_HP);
	pDlg->GetDlgItem("Btn_Hp")->SetData(BUILDING_MAX_HP); // HP上限
	iValue = iLevel;
	strText.Format(_AL("%d"), iValue);
	pLab = pDlg->GetDlgItem("Txt_Hp");
	pLab->SetData(iLevel);
	pLab->SetText(strText);
	// 升级所需物资
	pLab = pLab1 = NULL;
	if (pCfg->hp_war_material == 0) {
		pLab = pDlg->GetDlgItem("Txt_HpNeed1");
		pLab1 = pDlg->GetDlgItem("Txt_HpNeed2");
	} else if (pCfg->hp_war_material == 1) {	
		pLab = pDlg->GetDlgItem("Txt_HpNeed2");
		pLab1 = pDlg->GetDlgItem("Txt_HpNeed1");
	} else {
		assert(0);
	}
	if (pLab) {
		iValue = pCfg->hp_material_num;
		strText.Format(_AL("%d"), iValue);
		pLab->SetData(iValue);
		pLab->SetText(strText);
	}
	if (pLab1) {
		pLab1->SetData(0);
		pLab1->SetText(EMPTY_STR);
	}

	// 下一级血量
	strText = EMPTY_STR;
	if (iLevel < BUILDING_MAX_HP) {
		iLevel += pCfg->lvlup_hp;
		if (iLevel > BUILDING_MAX_HP)
			iLevel = BUILDING_MAX_HP;
		iValue = iLevel;
		strText.Format(_AL("%d"), iValue);
	}
	pLab = pDlg->GetDlgItem("Txt_HpNext");
	pLab->SetData(iLevel);
	pLab->SetText(strText);

	// 物资1
	pLab = pDlg->GetDlgItem("Txt_Resource01");
	pLab->SetData(pCityBuilding->m_pConsInfo->res_a);	
	strText.Format(_AL("%d"), pCityBuilding->m_pConsInfo->res_a);
	pLab->SetText(strText);
	// 物资2
	pLab = pDlg->GetDlgItem("Txt_Resource02");
	pLab->SetData(pCityBuilding->m_pConsInfo->res_b);
	strText.Format(_AL("%d"), pCityBuilding->m_pConsInfo->res_b);
	pLab->SetText(strText);

	return true;
}

bool CDlgCityBuilding::FillCitySelectedInfoArea()
{
	if (stricmp(m_szName, "Win_CityBuilding") != 0) {
		assert(0);
		return false;
	}

	const WAR_ROLE_CONFIG *pCfg = NULL;
	S2C::war_construction_info::node_t::attr_t attr = {0};
	int index = m_curSelectedBuilding;
	bool bErr = false;
	if (index < 1 || index > TOWER_NUM + GATE_NUM) {
		bErr = true;
	} else {
		// NPC essense
		pCfg = m_pWarRoleCfgs[index - 1];
		if (!pCfg) {
			assert(0);
			bErr = true;
		}
		// Construction info
		attr = m_pConsInfo->nodes[index - 1].attr;
		if (attr.defence_level > pCfg->defence_max_lev
			|| attr.ai_level > pCfg->strategy_max_lev
			|| attr.curHp_level > BUILDING_MAX_HP)
		{
			assert(0);
			bErr = true;
		}
	}

	// Fill the UI
	PAUIOBJECT pLab = NULL;
	ACString strText;
	int iLevel = 0, iValue = 0;
	// 建筑名
	if (index >= 1 && index <= TOWER_NUM + GATE_NUM) {
		strText.Format(_AL("%s%d"), 
			index <= TOWER_NUM ? TOWER_NAME : GATE_NAME, 
			index <= TOWER_NUM ? index : index - TOWER_NUM);
		pLab = GetDlgItem("Lab_BuildingAttr");
		if (pLab) pLab->SetText(strText);
	}
	// 攻击力
	pLab = GetDlgItem("Txt_Damage");
	if (!bErr && index >= 1 && index <= TOWER_NUM) { // 塔
		iLevel = attr.attack_level;
		iValue = pCfg->attack_level[iLevel].attack_value;
		strText.Format(_AL("%d"), iValue);
		pLab->SetData(iLevel);
		pLab->SetText(strText);
	} else { // 其它
		pLab->SetData(0);
		pLab->SetText(EMPTY_STR);
	}
	// 防御力
	pLab = GetDlgItem("Txt_Defence");
	if (!bErr) { // 塔/门
		iLevel = attr.defence_level;
		iValue = pCfg->defence_level[iLevel].defence_extra;
		strText.Format(_AL("%d"), iValue);
		pLab->SetData(iLevel);
		pLab->SetText(strText);
	} else { // 其它
		pLab->SetData(0);
		pLab->SetText(EMPTY_STR);
	}
	// 射程级别
	pLab = GetDlgItem("Txt_Range");
	if (!bErr && index >= 1 && index <= TOWER_NUM) { // 塔
		iLevel = attr.range_level;
		iValue = pCfg->range_values[iLevel];
		strText.Format(_AL("%d"), iValue);
		pLab->SetData(iLevel);
		pLab->SetText(strText);
	} else { // 其它
		pLab->SetData(0);
		pLab->SetText(EMPTY_STR);
	}
	// 特殊技能
	pLab = GetDlgItem("Txt_Skill");
	if (!bErr) { // 塔/门
		iLevel = attr.ai_level;
		iValue = pCfg->strategy_id[iLevel];
		strText.Format(_AL("%s"), GetStringFromTable((index<=TOWER_NUM ? 990 : 995) + iLevel));
		pLab->SetData(iLevel);
		pLab->SetText(strText);
	} else { // 其它
		pLab->SetData(0);
		pLab->SetText(EMPTY_STR);
	}
	// 血量
	pLab = GetDlgItem("Txt_Hp");
	if (!bErr) { // 塔/门
		iLevel = attr.curHp_level;
		iValue = iLevel;
		strText.Format(_AL("%d"), iValue);
		pLab->SetData(iLevel);
		pLab->SetText(strText);
	} else { // 其它
		pLab->SetData(0);
		pLab->SetText(EMPTY_STR);
	}

	return true;
}

void CDlgCityBuilding::FillCityConstructButton(bool bEnable)
{
	int i;
	AString strName;
	for (i = 0; i < TOWER_NUM; ++i) {
		strName.Format("Btn_Tower%02d", i+1);
		GetDlgItem(strName)->Enable(bEnable);
	}
	for (i = 0; i < GATE_NUM; ++i) {
		strName.Format("Btn_Gate%02d", i+1);
		GetDlgItem(strName)->Enable(bEnable);
	}
}

bool CDlgCityBuilding::FillCityResInfo()
{
	if (stricmp(m_szName, "Win_CityBuilding") != 0) {
		assert(0);
		return false;
	}
	PAUIOBJECT pLab = GetDlgItem("Txt_Resource01");
	ACString strRes;
	strRes.Format(_AL("%d"), m_pConsInfo->res_a);
	pLab->SetData(m_pConsInfo->res_a);
	pLab->SetText(strRes);
	pLab = GetDlgItem("Txt_Resource02");
	strRes.Format(_AL("%d"), m_pConsInfo->res_b);
	pLab->SetData(m_pConsInfo->res_b);
	pLab->SetText(strRes);
	return true;
}

bool CDlgCityBuilding::FillCityDlg()
{	
	if (!FillCitySelectedInfoArea() || // 所选建筑信息
		!FillCityResInfo()) // 城建物资
		return false;

	// 建筑按钮
	int fRoleID = GetHostPlayer()->GetFRoleID();
	bool bIsMaster = (fRoleID == GNET::_R_MASTER || fRoleID == GNET::_R_VICEMASTER);
	FillCityConstructButton(bIsMaster);

	// 捐献列表
	GetDlgItem("Btn_Prec")->Enable(m_curPage > 0);
	GetDlgItem("Btn_Next")->Enable(m_curPage < m_maxPage);

	return true;
}
