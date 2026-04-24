// Filename	: DlgModule.cpp
// Creator	: Fu Chonggang
// Date		: 2009/08/21

#include "AFI.h"
#include "AUI\\AUILabel.h"
#include "DlgModule.h"
#include "DlgBaseExplorer.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_HTPDataType.h"
#include "EC_HTC2SRawCmds.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_Secretary.h"
#include "EC_GameRun.h"
#include "EC_Configs.h"

#include "hometowntemplate.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgModule, CDlgBase)

AUI_ON_COMMAND("Btn_InteractPage",		OnCommandInteractPage)
// list modules
AUI_ON_COMMAND("Btn_Garden",	OnCommandGardenMain)
AUI_ON_COMMAND("Btn_OpenAdd",	OnCommandGardenAddModule)
// add module
AUI_ON_COMMAND("Btn_Add*",		OnCommandGardenAddModuleType)
AUI_ON_COMMAND("Btn_Close",		OnCommand_Close)

AUI_END_COMMAND_MAP()

abase::vector<int> CDlgModule::m_vecModules;
INT64 CDlgModule::m_iMoney	=0;

using namespace HT_S2C;
using namespace HT_S2C::CMD;

CDlgModule::CDlgModule()
{
	m_iModuleNum = 0;
	m_bOwn = true;
}

CDlgModule::~CDlgModule()
{
}

bool CDlgModule::OnInitDialog()
{
	if (stricmp(m_szName, "Win_Option")==0)
	{
		m_iModuleNum = 0;
		m_vecModules.clear();
		m_iMoney = 0;
		m_iOwnerId = GetHostPlayer()->GetCharacterID();
	}

	return true;
}

void CDlgModule::OnCommand_Close(const char * szCommand)
{
	Show(false);
}

void CDlgModule::OnTick()
{
	if (stricmp(m_szName, "Win_Option")==0)
	{
// 		if (m_vecModules.size()>0)
// 		{
// 		}
	}
	if (stricmp(m_szName, "Win_Garden_Option")==0)
	{
		//模板中读取全部可用组件并显示
	}
}

void CDlgModule::OnShowDialog()
{
	if (stricmp(m_szName, "Win_Option")==0)
	{
// 		if (m_bOwn)
// 		{
// 			GetDlgItem("Btn_OpenAdd")->Show(true);
// 			GetDlgItem("Lab_2")->Show(true);
// 		}
// 		else
// 		{
// 			GetDlgItem("Btn_OpenAdd")->Show(false);
// 			GetDlgItem("Lab_2")->Show(false);
// 		}
	}
	if (stricmp(m_szName, "Win_Garden_Option")==0)
	{
		//模板中读取全部可用组件并显示
	}
}

void CDlgModule::OnCommandInteractPage(const char* szCommand)
{
	const char *szCountry = g_pGame->GetConfigs()->GetCountryRegion();
	if ( 0 == stricmp(szCountry, "cn") )
	{
		Show(false);
		CDlgBaseExplorer* pDlgExplorer = (CDlgBaseExplorer*)GetGameUIMan()->GetDialog("win_explorer_xf");
		pDlgExplorer->Show(true);
		pDlgExplorer->NavigateUrl(GetStringFromTable(9173));
	}
}

// list modules
void CDlgModule::OnCommandGardenMain(const char* szCommand)
{
	//发送获取庄园信息协议
	if (m_iModuleNum>0)
		htc2s_SendCmdViewFarm(m_iOwnerId);
	else
	{
		if (m_bOwn)
			GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(8673),MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		else
			GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(8670),MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	}
}
void CDlgModule::OnCommandGardenAddModule(const char* szCommand)
{
	//获取所有可添加组件并打开相应对话框
	GetGameUIMan()->GetDialog("Win_Garden_Option")->Show(true);
}
// add module
void CDlgModule::OnCommandGardenAddModuleType(const char* szCommand)
{
	//添加一个组件
	HometownTemplate* pDB = g_pGame->GetHomeTownDataMan();
	const HOMETOWN::HOMETOWN_CONFIG* ess = pDB->get_hometown_config();
	UINT iType = atoi(szCommand + strlen("addtype"));
	if (iType>ess->compo_list.size())
	{
		return;
	}
	int id = ess->compo_list[iType-1];
	htc2s_SendCmdAddCompo(id);
	Show(false);
}


void CDlgModule::OnPrtc_ModuleList(void *pData)
{
	compo_list* pList = (compo_list*)pData;
	m_iMoney = pList->money;
	m_iModuleNum = pList->count;
	m_bOwn = pList->roleid == GetHostPlayer()->GetCharacterID();
	m_iOwnerId = pList->roleid;
	if (m_iModuleNum==0)
	{
		//没有默认组件，初始自动发送添加一个
		PAUILABEL pLab = (PAUILABEL)GetDlgItem("Lab_1");
		pLab->SetText(GetGameUIMan()->GetStringFromTable(780));
		GetDlgItem("Btn_Garden")->Show(false);
		Show(true);
		return;
	}
	else if (m_iModuleNum!=1)
	{
		//当前界面不好显示多个，需要修改界面
		assert(0);
		return;
	}
	PAUILABEL pLab = (PAUILABEL)GetDlgItem("Lab_1");
	GetDlgItem("Btn_Garden")->Show(true);
	pLab->SetText(GetGameUIMan()->GetStringFromTable(8650+pList->compos[0]));
	Show(true);
}

void CDlgModule::OnPrtc_AddModule(int id)
{
	PAUILABEL pLab = (PAUILABEL)GetDlgItem("Lab_1");
	GetDlgItem("Btn_Garden")->Show(true);
	pLab->SetText(GetGameUIMan()->GetStringFromTable(8650+id));
	m_iOwnerId = GetHostPlayer()->GetCharacterID();
	m_iModuleNum ++;
	Show(true);
}