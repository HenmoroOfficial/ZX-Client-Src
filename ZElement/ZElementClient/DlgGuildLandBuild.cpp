#include "AFI.h"
#include "Network\\ids.hxx"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_World.h"
#include "EC_AutoMove.h"

#include "AUI\\AUICommon.h"
#include "AUI\\AUIDef.h"
#include "AUI\\AUICTranslate.h"
#include "AUI\\AUISubDialog.h"
#include "AUI\\AUITreeView.h"
#include "aui\\CSplit.h"
#include "aui\\AUICheckBox.h"

#include "DlgGuildLandBuild.h"

#include "elementdataman.h"
#include "EC_TaskInterface.h"
#include "TaskProcess.h"
#include "TaskTempl.h"
#include "TaskTemplMan.h"
#include "DlgTask.h"

AUI_BEGIN_COMMAND_MAP(DlgGuildCreateLandBuild, CDlgBase)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(DlgGuildCreateLandBuild, CDlgBase)
AUI_ON_EVENT("Img_*",					WM_LBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("NULL",				WM_LBUTTONDOWN,	OnEventLButtonDown)
AUI_END_EVENT_MAP()


DlgGuildCreateLandBuild::DlgGuildCreateLandBuild()
{

}
DlgGuildCreateLandBuild::~DlgGuildCreateLandBuild()
{

}
bool DlgGuildCreateLandBuild::OnInitDialog()
{

	AString strText;
	for(int nIndex = 0; nIndex < 12; nIndex++)
	{
		strText.Format("Lab_%d", nIndex + 1);
		m_pTxt[nIndex] =  (AUILabel*)GetDlgItem(strText);
		m_pTxt[nIndex]->Show(false);
		strText.Format("Img_%d", nIndex + 1);
		m_pImg_Icon[nIndex] =  (AUIImagePicture*)GetDlgItem(strText);
		m_pImg_Icon[nIndex]->Show(false);
	}

	return true;
}

void DlgGuildCreateLandBuild::ClearBuildTemp() 
{
	for(int index = 0; index < 12; index++)
	{
		m_pTxt[index]->Show(false);
		m_pImg_Icon[index]->Show(false);
	}
}


BUILDING_ESSENCE* GetTempData(CECGame *pGame,int nIndex)
{
	DATA_TYPE DataType ;
	BUILDING_ESSENCE* pBuildEssence = (BUILDING_ESSENCE *)pGame->GetElementDataMan()->
		get_data_ptr(nIndex, ID_SPACE_ESSENCE, DataType);
	return pBuildEssence;
}

ACString DlgGuildCreateLandBuild::GetBuildHint( BUILDING_ESSENCE* pBuildEssence,int nLevel ) 
{
	ACString szTemp,szProDesc;
	if(pBuildEssence->up_building[nLevel].grass_num != 0)
	{
		szTemp.Format(GetStringFromTable(20211),pBuildEssence->up_building[nLevel].grass_num);
		if(pBuildEssence->up_building[nLevel].grass_num > GetHostPlayer()->GetFactionGrass())
		{
			szProDesc = _AL("^ff0000")+ szTemp;
		}
		else
		{
			szProDesc = _AL("^ffffff")+szTemp;
		}
	}
	if(pBuildEssence->up_building[nLevel].mine_num != 0)
	{
		szTemp.Format(GetStringFromTable(20212),pBuildEssence->up_building[nLevel].mine_num);
		if(pBuildEssence->up_building[nLevel].mine_num > GetHostPlayer()->GetFactionMine())
		{
			szProDesc += _AL("^ff0000") + szTemp;
		}
		else
		{
			szProDesc += _AL("^ffffff") + szTemp;
		}
	}
	if(pBuildEssence->up_building[nLevel].monster_food_num != 0)
	{
		szTemp.Format(GetStringFromTable(20213),pBuildEssence->up_building[nLevel].monster_food_num);
		if(pBuildEssence->up_building[nLevel].monster_food_num > GetHostPlayer()->GetFactionMonsterFood())
		{
			szProDesc +=_AL("^ff0000") + szTemp;
		}
		else
		{
			szProDesc += _AL("^ffffff") + szTemp;
		}
	}		
	if(pBuildEssence->up_building[nLevel].monster_core_num != 0)
	{
		szTemp.Format(GetStringFromTable(20214),pBuildEssence->up_building[nLevel].monster_core_num);
		if(pBuildEssence->up_building[nLevel].monster_core_num > GetHostPlayer()->GetFactionMonsterCore())
		{
			szProDesc += _AL("^ff0000")+ szTemp;
		}
		else
		{
			szProDesc+= _AL("^ffffff") + szTemp;
		}
	}
	if(pBuildEssence->up_building[nLevel].money_num != 0)
	{
		szTemp.Format(GetStringFromTable(20215),pBuildEssence->up_building[nLevel].money_num);
		if(pBuildEssence->up_building[nLevel].money_num > GetHostPlayer()->GetFactionMoney())
		{
			szProDesc += _AL("^ff0000") + szTemp;
		}
		else
		{
			szProDesc += _AL("^ffffff") + szTemp;
		}
	}
	for(int nNeedIndex = 0;nNeedIndex < 10; nNeedIndex++)
	{
		if(pBuildEssence->up_building[nLevel].req_building[nNeedIndex].building_id != 0)
		{
			BUILDING_ESSENCE* pData = GetTempData(GetGame(),pBuildEssence->up_building[nLevel].req_building[nNeedIndex].building_id);
			szTemp.Format(GetStringFromTable(20216),pData->name,pBuildEssence->up_building[nLevel].req_building[nNeedIndex].level);
			CECHostPlayer::Fac_BuildField* pInfo = GetHostPlayer()->GetBuildInfoTemp(pBuildEssence->up_building[nLevel].req_building[nNeedIndex].building_id);
			if(pInfo && pInfo->nLevel < pBuildEssence->up_building[nLevel].req_building[nNeedIndex].level)
			{
				szProDesc += _AL("^ff0000")+ szTemp;
			}
			else if(pInfo == NULL)
			{
				szProDesc += _AL("^ff0000")+ szTemp;
			}
			else
			{
				szProDesc += _AL("^ffffff") + szTemp;
			}
		}
	}
	return szProDesc + _AL("^ffffff");
}



void DlgGuildCreateLandBuild::CreateBuild(int nTempIndex, int nCreateId,bool isUpDrade)
{
	ClearBuildTemp();
	m_bIsCreate = true;
	m_nFId = nCreateId;
	DATA_TYPE DataType;
	REGION_BLOCK_ESSENCE * m_pEssence = (REGION_BLOCK_ESSENCE *)GetGame()->GetElementDataMan()->
		get_data_ptr(nTempIndex, ID_SPACE_ESSENCE, DataType);
	ACString szDesc;
	ACString szProDesc;
	ACString szTempDesc;
	AUIOBJECT_SETPROPERTY p;
	for(int nIndex = 0; nIndex < 10; nIndex++)
	{
		BUILDING_ESSENCE* pBuildEssence = GetTempData(GetGame(),m_pEssence->building[nIndex]); 
		if(pBuildEssence)
		{
			ACString szTemp;
			szTemp.Format(GetStringFromTable(20221),1,pBuildEssence->name);
			m_pTxt[nIndex]->SetText(szTemp);
			m_pTxt[nIndex]->Show(true);
			AString strFile;
			AUICTranslate trans;
			af_GetRelativePathNoBase(GetGame()->GetDataPath(pBuildEssence->up_building[0].ordinary), "surfaces\\", strFile);
			strcpy(p.fn,strFile);
			m_pImg_Icon[nIndex]->SetProperty("Image File",&p);
			
			szProDesc = 	GetBuildHint(pBuildEssence, 0);

			szTempDesc.Format(GetStringFromTable(20219),pBuildEssence->building_desc);
			szDesc.Format(GetStringFromTable(20207),pBuildEssence->name,1);
			szDesc+= szProDesc + szTempDesc;
			m_pImg_Icon[nIndex]->SetHint(trans.Translate(szDesc));
			m_pImg_Icon[nIndex]->Show(true);
			m_pImg_Icon[nIndex]->SetDataPtr(pBuildEssence);
		}
	}
}

void DlgGuildCreateLandBuild::UpDradeBuild(int nTempIndex,int nIndex, int nLevel)
{
	ClearBuildTemp();
	ACString szDesc;
	ACString szProDesc;
	ACString szTempDesc;
	m_bIsCreate = false;
	m_nFId = nTempIndex;
	AUIOBJECT_SETPROPERTY p;
	BUILDING_ESSENCE* pBuildEssence = GetTempData(GetGame(),nIndex);  
	if(pBuildEssence)
	{
		if(pBuildEssence->up_building[nLevel].building_model != 0)
		{
			ACString szTemp;
			szTemp.Format(GetStringFromTable(20221),nLevel + 1,pBuildEssence->name);
			m_pTxt[0]->SetText(szTemp);
			m_pTxt[0]->Show(true);
			AString strFile;
			AUICTranslate trans;
			af_GetRelativePathNoBase(GetGame()->GetDataPath(pBuildEssence->up_building[nLevel].ordinary), "surfaces\\", strFile);
			strcpy(p.fn,strFile);
			m_pImg_Icon[0]->SetProperty("Image File",&p);
			m_pImg_Icon[0]->Show(true);
			m_pImg_Icon[0]->SetDataPtr(pBuildEssence);
			m_pImg_Icon[0]->SetData(nLevel);
			szProDesc = 	GetBuildHint(pBuildEssence, nLevel);
			szTempDesc.Format(GetStringFromTable(20219),pBuildEssence->building_desc);
			szDesc.Format(GetStringFromTable(20207),pBuildEssence->name,nLevel + 1);
			szDesc+= szProDesc + szTempDesc;
			m_pImg_Icon[0]->SetHint(trans.Translate(szDesc));
		}
	}
}

bool DlgGuildCreateLandBuild::CheckBuild( BUILDING_ESSENCE * m_pEssence, int nLevel ) 
{
	if(GetHostPlayer()->GetFactionGrass() < m_pEssence->up_building[nLevel].grass_num)
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(20180), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return false;
	}
	else if(GetHostPlayer()->GetFactionMine() < m_pEssence->up_building[nLevel].mine_num)
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(20181), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return false;
	}
	else if(GetHostPlayer()->GetFactionMonsterFood() < m_pEssence->up_building[nLevel].monster_food_num)
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(20182), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return false;
	}
	else if(GetHostPlayer()->GetFactionMonsterCore() < m_pEssence->up_building[nLevel].monster_core_num)
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(20183), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return false;
	}
	else if(GetHostPlayer()->GetFactionMoney() < m_pEssence->up_building[nLevel].money_num)
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(20184), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return false ;
	}
	ACString szTemp;
	for(int nIndex = 0; nIndex < 10; nIndex++)
	{
		if(m_pEssence->up_building[nLevel].req_building[nIndex].building_id != 0)
		{
			CECHostPlayer::Fac_BuildField* pInfo = GetHostPlayer()->GetBuildInfoTemp(m_pEssence->up_building[nLevel].req_building[nIndex].building_id);
			if(pInfo && pInfo->nLevel < m_pEssence->up_building[nLevel].req_building[nIndex].level)
			{
				GetGameUIMan()->MessageBox("", GetStringFromTable(20217), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
				return false ;		
			}
			else if(pInfo == NULL)
			{
				GetGameUIMan()->MessageBox("", GetStringFromTable(20217), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
				return false ;		
			}
		}
	}
	return true;
}

void DlgGuildCreateLandBuild::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if(pObj->GetDataPtr() == NULL)
	{
		return;
	}
	BUILDING_ESSENCE * m_pEssence = (BUILDING_ESSENCE *)pObj->GetDataPtr();
	if(m_bIsCreate)
	{
		if(CheckBuild(m_pEssence,0) == false)
		{
			return;
		}
		m_nFId = m_nFId + 1;			// 地块ID
		m_nTempID = m_pEssence->id;		// 模板ID
		ACString szTemp;
		szTemp.Format(GetStringFromTable(20204),m_pEssence->name);
		GetGameUIMan()->MessageBox("SendCreateBuild", szTemp, MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160));
	}
	else
	{
		if(CheckBuild(m_pEssence,pObj->GetData()) == false)
		{
			return;
		}
		m_nFId = m_nFId;			// 地块ID
		m_nTempID = m_pEssence->id;		// 模板ID
		m_nCurLevel = pObj->GetData();	// 当前等级
		ACString szTemp;
		szTemp.Format(GetStringFromTable(20205),m_pEssence->name);
		GetGameUIMan()->MessageBox("SendUpDradeBuild", szTemp, MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160));
	}
	Show(false);
}

void DlgGuildCreateLandBuild::SendCreateBuild()
{
	GetGameSession()->c2s_SendAddFacBuild(GetHostPlayer()->GetFactionID(),m_nFId,m_nTempID);
}
void DlgGuildCreateLandBuild::SendUpDradeBuild()
{
	GetGameSession()->c2s_SendUpGradeFacBuild(GetHostPlayer()->GetFactionID(),m_nFId,m_nTempID,m_nCurLevel);
}

AUI_BEGIN_COMMAND_MAP(DlgGuildLandBuild, CDlgBase)
AUI_ON_COMMAND("Chk_01", OnCommand_CreateBuild)
AUI_ON_COMMAND("Chk_02", OnCommand_DestroyBuild)
AUI_ON_COMMAND("Btn_Build_*", OnCommand_Build)
AUI_ON_COMMAND("Btn_Quest", OnCommand_Quest)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(DlgGuildLandBuild, CDlgBase)
//AUI_ON_EVENT("Btn_Build_*",					WM_LBUTTONDOWN,	OnEventLButtonDown)
//AUI_ON_EVENT("NULL",				WM_LBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("Tv_Quest",	WM_LBUTTONUP,	OnEventButtonDBCLK)

AUI_END_EVENT_MAP()
DlgGuildLandBuild::DlgGuildLandBuild()
{

}
DlgGuildLandBuild::~DlgGuildLandBuild()
{

}
bool DlgGuildLandBuild::OnInitDialog()
{
	AString szTemp;
	for(int nIndex = 0; nIndex < 20; nIndex++)
	{
		szTemp.Format("Btn_Build_%d",nIndex+1);
		m_pBtn[nIndex] = (PAUISTILLIMAGEBUTTON)GetDlgItem(szTemp);
		m_pBtn[nIndex]->Show(false);
	}
	m_nCurSelect = -1;
	m_nState = 0;
	DATA_TYPE DataType;
	BUILDING_REGION_CONFIG * m_pEssence = (BUILDING_REGION_CONFIG *)GetGame()->GetElementDataMan()->
		get_data_ptr(805, ID_SPACE_CONFIG, DataType);

	for(int nIndex = 0; nIndex < 20; nIndex++)
	{
		for(int nBuilIndex = 0; nBuilIndex < 30; nBuilIndex++)
		{
			if(nIndex + 1 == m_pEssence->build_region[nBuilIndex].region_index && m_pEssence->build_region[nBuilIndex].region_index != 0)
			{
				m_pBtn[nIndex]->SetPos(m_pEssence->build_region[nBuilIndex].centerx,m_pEssence->build_region[nBuilIndex].centery);
				m_pBtn[nIndex]->SetSize(m_pEssence->build_region[nBuilIndex].width,m_pEssence->build_region[nBuilIndex].length);
				m_pBtn[nIndex]->SetText(m_pEssence->build_region[nBuilIndex].name);
				m_pBtn[nIndex]->Show(true);
				m_pBtn[nIndex]->SetData(m_pEssence->build_region[nBuilIndex].region_id);
				m_pBtn[nIndex]->SetDataPtr((void*)nIndex);
				m_pBtn[nIndex]->SetHint(m_pEssence->build_region[nBuilIndex].name);
				m_pBtn[nIndex]->SetColor(A3DCOLORRGBA(255,255,0,255));
			}
		}	
	}
	AUICheckBox* pItem = (AUICheckBox*)GetDlgItem("Chk_01");
	pItem->Check(false);
	pItem = (AUICheckBox*)GetDlgItem("Chk_02");
	pItem->Check(false);

	m_pPrg =  (PAUIPROGRESS)GetDlgItem("Prgs_01");

	m_pTreeTask = (PAUITREEVIEW)GetDlgItem("Tv_Quest");
	m_pTreeTask->Show(false);
	return true;
}

void DlgGuildLandBuild::OnTick()
{
	CECTaskInterface* pTask = GetHostPlayer()->GetTaskInterface();
	int nNum = 0;
	if(pTask)
	{
		TaskFinishTimeList* pFinishLst = static_cast<TaskFinishTimeList*>(pTask->GetFinishedTimeList());
		if(!pFinishLst)
			return ;

		unsigned short count = 0;
		for (unsigned short i = 0; i < pFinishLst->m_uCount; i++)
		{
			TaskFinishTimeEntry& entry = pFinishLst->m_aList[i];

			ATaskTempl* pTempl = GetTaskTemplMan()->GetTaskTemplByID(entry.m_uTaskId);

			if(pTempl && pTempl->m_bFaction && pTempl->m_bRecFinishCount)
				nNum += entry.m_uFinishCount;			
		}
	}
	ACString szTemp;
	szTemp.Format(_AL("%d/%d"),nNum,10);
	GetDlgItem("Txt_Quest")->SetText(szTemp);
	szTemp.Format(_AL("%d"),GetHostPlayer()->GetFactionGrass());
	GetDlgItem("Txt_02")->SetText(szTemp);
	szTemp.Format(_AL("%d"),GetHostPlayer()->GetFactionMonsterCore());
	GetDlgItem("Txt_01")->SetText(szTemp);
	szTemp.Format(_AL("%d"),GetHostPlayer()->GetFactionGoldNote());
	GetDlgItem("Txt_03")->SetText(szTemp);
	szTemp.Format(_AL("%d"),GetHostPlayer()->GetFactionMine());
	GetDlgItem("Txt_05")->SetText(szTemp);
	szTemp.Format(_AL("%d"),GetHostPlayer()->GetFactionMonsterFood());
	GetDlgItem("Txt_06")->SetText(szTemp);
	szTemp.Format(_AL("%d"),GetHostPlayer()->GetFactionMoney());
	GetDlgItem("Txt_07")->SetText(szTemp);
	m_pPrg->SetProgress(int(double(GetHostPlayer()->GetFactionTask_count()) / GetHostPlayer()->GetFactionTask_need() * AUIPROGRESS_MAX));
	szTemp.Format(_AL("%d/%d"), GetHostPlayer()->GetFactionTask_count(),GetHostPlayer()->GetFactionTask_need());
	m_pPrg->SetHint(szTemp);
	GetDlgItem("Txt_Prgs")->SetText(szTemp);
	GetDlgItem("Lab_Prgs")->SetText(_AL(""));
	GetDlgItem("Txt_Prgs")->Show(false);
	m_pPrg->Show(false);
	for(int nIndex = 0; nIndex < 20; nIndex++)
	{
		CECHostPlayer::Fac_BuildField* pInfo = GetHostPlayer()->GetBuildInfo(nIndex +1);
		if(pInfo && pInfo->nStatus == 0)
		{
			BUILDING_ESSENCE * m_pEssence = GetTempData(GetGame(),pInfo->nId);
			ACString szTemp;
			szTemp.Format(GetStringFromTable(20196),pInfo->nLevel + 1,m_pEssence->name);
			GetDlgItem("Lab_Prgs")->SetText(szTemp);
			m_pPrg->Show(true);
			GetDlgItem("Txt_Prgs")->Show(true);
			break;
		}
	}
}

void DlgGuildLandBuild::OnShowDialog()
{

	m_nState = 0;
	AUICheckBox* pItem = (AUICheckBox*)GetDlgItem("Chk_01");
	pItem->Check(false);
	pItem = (AUICheckBox*)GetDlgItem("Chk_02");
	pItem->Check(false);
	AString strFile;
	AUICTranslate trans;
	AUIOBJECT_SETPROPERTY p;
	BUILDING_ESSENCE * m_pEssence = NULL;
	GetDlgItem("Lab_Prgs")->SetText(_AL(""));
	GetDlgItem("Txt_Prgs")->SetText(_AL(""));
	strcpy(p.fn,"guildbuild\\GuildLand_01Y.tga");
	for(int nIndex = 0; nIndex < 20; nIndex++)
	{
		m_pBtn[nIndex]->SetProperty("Up Frame File",&p);
		m_pBtn[nIndex]->SetProperty("Down Frame File",&p);
		m_pBtn[nIndex]->SetProperty("OnHover Frame File",&p);
	}
	ACString szDesc;
	for(int nIndex = 0; nIndex < 20; nIndex++)
	{
		CECHostPlayer::Fac_BuildField* pInfo = GetHostPlayer()->GetBuildInfo(nIndex +1);
		if(pInfo)
		{
			 m_pEssence = GetTempData(GetGame(),pInfo->nId);
			 if(pInfo->nStatus == 0 && pInfo->nLevel == 0)
			 {
				 af_GetRelativePathNoBase(GetGame()->GetDataPath(m_pEssence->up_building[pInfo->nLevel].ordinary), "surfaces\\", strFile);
				 szDesc.Format(GetStringFromTable(20201),m_pEssence->name,1,GetStringFromTable(pInfo->nStatus ? 20203 : 20202),m_pEssence->building_desc);
			 }
			 else if(pInfo->nStatus == 0)
			 {
				 szDesc.Format(GetStringFromTable(20201),m_pEssence->name,pInfo->nLevel + 1,GetStringFromTable(pInfo->nStatus ? 20203 : 20202),m_pEssence->building_desc);
			 }
			 else
			 {
				 szDesc.Format(GetStringFromTable(20201),m_pEssence->name,pInfo->nLevel,GetStringFromTable(pInfo->nStatus ? 20203 : 20202),m_pEssence->building_desc);
				 af_GetRelativePathNoBase(GetGame()->GetDataPath(m_pEssence->up_building[pInfo->nLevel - 1].ordinary), "surfaces\\", strFile);
			 }
			strcpy(p.fn,strFile);
			m_pBtn[nIndex]->SetProperty("Up Frame File",&p);
			m_pBtn[nIndex]->SetProperty("Down Frame File",&p);
			m_pBtn[nIndex]->SetProperty("OnHover Frame File",&p);

			m_pBtn[nIndex]->SetHint(trans.Translate(szDesc));
		}
	}
	GetDlgItem("Chk_01")->Show(false);
	GetDlgItem("Chk_02")->Show(false);
	if (GetHostPlayer()->GetFRoleID() == GNET::_R_MASTER
		|| GetHostPlayer()->GetFRoleID() == GNET::_R_VICEMASTER
		|| GetHostPlayer()->GetFRoleID() == GNET::_R_BODYGUARD)
	{
		GetDlgItem("Chk_01")->Show(true);
		GetDlgItem("Chk_02")->Show(true);
	}
	m_pTreeTask->Show(false);
}


void  DlgGuildLandBuild::OnCommand_Build(const char * szCommand)
{
	int nIndex = (int)GetDlgItem(szCommand)->GetDataPtr();
	if(nIndex > 20)
	{
		return;
	}
	int nTempID = GetDlgItem(szCommand)->GetData();
	m_nCurSelect = nIndex;
	if(m_nState == 0)
	{
		CECHostPlayer::Fac_BuildField* pInfo = GetHostPlayer()->GetBuildInfo(nIndex +1);
		if(pInfo)
		{
			if(pInfo->nStatus == 1)
			{
				BUILDING_ESSENCE * m_pEssence = GetTempData(GetGame(),pInfo->nId);
				ACString szTemp;
				szTemp.Format(GetStringFromTable(20210),m_pEssence->name);
				GetGameUIMan()->MessageBox("TransferBuild",szTemp , MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160));
			}
			else
			{
				GetGameUIMan()->MessageBox("", GetStringFromTable(20198), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			}
		}
	}
	else if(m_nState == 1)
	{
		DlgGuildCreateLandBuild* pDlg = (DlgGuildCreateLandBuild*)GetGameUIMan()->GetDialog("Win_Guild_LandBuild_List");
		CECHostPlayer::Fac_BuildField* pInfo = GetHostPlayer()->GetBuildInfo(nIndex +1);
		if(pInfo && pInfo->nStatus == 1)
		{
			pDlg->UpDradeBuild(pInfo->nIndex,pInfo->nId,pInfo->nLevel);
		}
		else if(pInfo&&pInfo->nStatus == 0)
		{
			GetGameUIMan()->MessageBox("", GetStringFromTable(20206), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			return;
		}
		else
		{
			pDlg->CreateBuild(nTempID,nIndex);
		}

		pDlg->Show(true);
	}
	else if(m_nState == 2)
	{
		CECHostPlayer::FacBuildField& MapInfo =  GetHostPlayer()->GetMapBuildInfo();
		CECHostPlayer::FacBuildField::iterator pIter = MapInfo.begin();
		for(;pIter != MapInfo.end();++pIter)
		{
			CECHostPlayer::Fac_BuildField* pInfo = &pIter->second ;
			if(pInfo->nStatus == 0)
			{
				GetGameUIMan()->MessageBox("", GetStringFromTable(20246), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
				return;
			}
		}
		CECHostPlayer::Fac_BuildField* pInfo = GetHostPlayer()->GetBuildInfo(nIndex +1);
		if(pInfo && pInfo->nStatus == 1)
		{
			GetGameUIMan()->MessageBox("MessageRemoveBuild", GetStringFromTable(20208), MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160));
		
		}
		else if(pInfo && pInfo->nStatus == 0)
		{
			GetGameUIMan()->MessageBox("", GetStringFromTable(20206), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		}
	}
}
void DlgGuildLandBuild::MessageBoxBuild()
{
	GetGameUIMan()->MessageBox("RemoveBuild", GetStringFromTable(20209), MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160));
}

void DlgGuildLandBuild::RemoveBuild()
{
	CECHostPlayer::Fac_BuildField* pInfo = GetHostPlayer()->GetBuildInfo(m_nCurSelect +1);
	if(pInfo)
	{
		GetGameSession()->c2s_SendRemoveFacBuild(GetHostPlayer()->GetFactionID(),pInfo->nIndex,pInfo->nId);
	}
}

void DlgGuildLandBuild::SendTransferBuild()
{
	CECHostPlayer::Fac_BuildField* pInfo = GetHostPlayer()->GetBuildInfo(m_nCurSelect +1);
	if(pInfo)
	{
		GetGameSession()->c2s_SendCmdFacBaseTransfer(pInfo->nIndex);
	}
}

void DlgGuildLandBuild::OnCommand_CreateBuild(const char * szCommand)
{
	if(m_nState == 1)
	{
		m_nState = 0;
		ChangeBuildGrid(false);
	}
	else if(m_nState != 1)
	{
		m_nState = 1;
		ChangeBuildGrid(true);
	}
	AUICheckBox* pItem = (AUICheckBox*)GetDlgItem("Chk_02");
	pItem->Check(false);
}

void DlgGuildLandBuild::OnCommand_DestroyBuild(const char * szCommand)
{
	if(m_nState == 2)
	{
		m_nState = 0;
		ChangeBuildGrid(false);
	}
	else if(m_nState != 2)
	{
		m_nState = 2;
		ChangeBuildGrid(true);
	}
	AUICheckBox* pItem = (AUICheckBox*)GetDlgItem("Chk_01");
	pItem->Check(false);
}

ACString GetHintText(ACString strTemp)
{
	ACString strHint;
	while(strTemp.GetLength()>25)
	{
		//判断换行
		int nFind = strTemp.Find(_AL('\r'));
		if (nFind>0&&nFind<=25)
		{
			strHint += strTemp.Left(nFind+1);
			strTemp.CutLeft(nFind+1);
			continue;
		}
		nFind = strTemp.Find(_AL('\n'));
		if (nFind>0&&nFind<=25)
		{
			strHint += strTemp.Left(nFind+1);
			strTemp.CutLeft(nFind+1);
			continue;
		}
		//颜色码
		int nCurLen = 25;
		int nStart = 0;
		while( (nFind = strTemp.Find(_AL('^'), nStart)), (nFind>=0 && nFind<nCurLen))
		{
			nStart = nFind+1;
			nCurLen += 7;
			if (nCurLen>50)
				break;
		}

		strHint += strTemp.Left(nCurLen);
		strHint += _AL("\r");
		strTemp.CutLeft(nCurLen);
	}
	strHint += strTemp;
	while (strHint.Right(1) == _AL("\r") || strHint.Right(1) == _AL("\n"))
	{
		strHint.CutRight(1);
	}
	return strHint;
}

void DlgGuildLandBuild::OnCommand_Quest(const char * szCommand)
{
	m_pTreeTask->Show(!m_pTreeTask->IsShow());
	if(m_pTreeTask->IsShow())
	{
		CECTaskInterface *pTaskInterafce = GetGame()->GetGameRun()->GetHostPlayer()->GetTaskInterface();
		ActiveTaskList* pLst =static_cast<ActiveTaskList*>(pTaskInterafce->GetActiveTaskList());

		m_pTreeTask->DeleteAllItems();
		ATaskTemplMan *pMan = GetGame()->GetTaskTemplateMan();
		std::map<int,vector<int>> mapBuild;
		if(pMan)
		{
			TaskTemplMap::iterator it =  pMan->GetAllTemplMap().begin();

			for (; it !=  pMan->GetAllTemplMap().end(); ++it)
			{
				ATaskTempl* pTop = it->second;
				if(pTop && pTop->m_bFaction && !pLst->GetEntry(pTop->GetID()))
				{
					CECHostPlayer::Fac_BuildField* pInfo = GetHostPlayer()->GetBuildInfoTemp(pTop->m_nBuildId);
					if(pInfo && pInfo->nLevel == pTop->m_nBuildLevel)
					{
						mapBuild[pTop->m_nBuildId].push_back(pTop->m_ID);
					}
				}
			}
		}
		ACString szTemp;
		BUILDING_ESSENCE * m_pEssence = NULL;
		P_AUITREEVIEW_ITEM pRoot, pItem;
		ATaskTempl*pTask = pMan->GetTaskTemplByID(GetHostPlayer()->GetFactionTask_id());
		if(pTask)
		{	
			pRoot = m_pTreeTask->InsertItem( GetStringFromTable(20224) );
			szTemp.Format(GetStringFromTable(20223),pTask->GetName());
			pItem = m_pTreeTask->InsertItem(szTemp, pRoot, AUITREEVIEW_FIRST);
			m_pTreeTask->SetItemData(pItem,  pTask->GetDeliverNPC());
			m_pTreeTask->SetItemHint(pItem,GetHintText(pTask->GetHintText()));
		}
	
		std::map<int,vector<int>>::iterator pIter = mapBuild.begin();
		for(; pIter != mapBuild.end(); pIter++)
		{
			vector<int> vecTask = pIter->second; 
			if(pIter->first == 59985)
			{
				BUILDING_ESSENCE * m_pEssence = GetTempData(GetGame(),59985);
				pRoot = m_pTreeTask->InsertItem( m_pEssence->name );
				for(int nIndex = 0; nIndex < (int)vecTask.size(); nIndex++)
				{
					ATaskTempl*pTask = pMan->GetTaskTemplByID(vecTask[nIndex]);
					szTemp.Format(GetStringFromTable(20223),pTask->GetName());
					pItem = m_pTreeTask->InsertItem(szTemp, pRoot, AUITREEVIEW_FIRST);
					m_pTreeTask->SetItemData(pItem, pTask->GetDeliverNPC());
					m_pTreeTask->SetItemHint(pItem, GetHintText(pTask->GetHintText()));
				 }
			}
			if(pIter->first == 59986)
			{
				BUILDING_ESSENCE * m_pEssence = GetTempData(GetGame(),59986);
				pRoot = m_pTreeTask->InsertItem( m_pEssence->name );
				for(int nIndex = 0; nIndex < (int)vecTask.size(); nIndex++)
				{
					ATaskTempl*pTask = pMan->GetTaskTemplByID(vecTask[nIndex]);
					szTemp.Format(GetStringFromTable(20223),pTask->GetName());
					pItem = m_pTreeTask->InsertItem(szTemp, pRoot, AUITREEVIEW_FIRST);
					m_pTreeTask->SetItemData(pItem, pTask->GetDeliverNPC());
					m_pTreeTask->SetItemHint(pItem, GetHintText(pTask->GetHintText()));
				}
			}
			if(pIter->first == 60006)
			{
				BUILDING_ESSENCE * m_pEssence =GetTempData(GetGame(),60006);
				pRoot = m_pTreeTask->InsertItem( m_pEssence->name );
				for(int nIndex = 0; nIndex < (int)vecTask.size(); nIndex++)
				{
					ATaskTempl*pTask = pMan->GetTaskTemplByID(vecTask[nIndex]);
					szTemp.Format(GetStringFromTable(20223),pTask->GetName());
					pItem = m_pTreeTask->InsertItem(szTemp, pRoot, AUITREEVIEW_FIRST);
					m_pTreeTask->SetItemData(pItem, pTask->GetDeliverNPC());
					m_pTreeTask->SetItemHint(pItem, GetHintText(pTask->GetHintText()));
				}
			}
			if(pIter->first == 60007)
			{
				BUILDING_ESSENCE * m_pEssence = GetTempData(GetGame(),60007);
				pRoot = m_pTreeTask->InsertItem( m_pEssence->name );
				for(int nIndex = 0; nIndex < (int)vecTask.size(); nIndex++)
				{
					ATaskTempl*pTask = pMan->GetTaskTemplByID(vecTask[nIndex]);
					szTemp.Format(GetStringFromTable(20223),pTask->GetName());
					pItem = m_pTreeTask->InsertItem(szTemp, pRoot, AUITREEVIEW_FIRST);
					m_pTreeTask->SetItemData(pItem, pTask->GetDeliverNPC());
					m_pTreeTask->SetItemHint(pItem, GetHintText(pTask->GetHintText()));
				}
			}
			if(pIter->first == 60008)
			{
				BUILDING_ESSENCE * m_pEssence =GetTempData(GetGame(),60008);
				pRoot = m_pTreeTask->InsertItem( m_pEssence->name );
				for(int nIndex = 0; nIndex < (int)vecTask.size(); nIndex++)
				{
					ATaskTempl*pTask = pMan->GetTaskTemplByID(vecTask[nIndex]);
					szTemp.Format(GetStringFromTable(20223),pTask->GetName());
					pItem = m_pTreeTask->InsertItem(szTemp, pRoot, AUITREEVIEW_FIRST);
					m_pTreeTask->SetItemData(pItem, pTask->GetDeliverNPC());
					m_pTreeTask->SetItemHint(pItem, GetHintText(pTask->GetHintText()));
				}
			}
			if(pIter->first == 60011)
			{
				BUILDING_ESSENCE * m_pEssence =  GetTempData(GetGame(),60011);
				pRoot = m_pTreeTask->InsertItem( m_pEssence->name );
				for(int nIndex = 0; nIndex < (int)vecTask.size(); nIndex++)
				{
					ATaskTempl*pTask = pMan->GetTaskTemplByID(vecTask[nIndex]);
					szTemp.Format(GetStringFromTable(20223),pTask->GetName());
					pItem = m_pTreeTask->InsertItem(szTemp, pRoot, AUITREEVIEW_FIRST);
					m_pTreeTask->SetItemData(pItem, pTask->GetDeliverNPC());
					m_pTreeTask->SetItemHint(pItem, GetHintText(pTask->GetHintText()));
				}
			}
		}
	}
}



void DlgGuildLandBuild::ResultBuilding(int nTempId, int nIndex)
{
	BUILDING_ESSENCE * m_pEssence = GetTempData(GetGame(),nTempId);
	if(m_pEssence)
	{
		if(nIndex <= 20)
		{
			AString strFile;

			AUIOBJECT_SETPROPERTY p;
			af_GetRelativePathNoBase(GetGame()->GetDataPath(m_pEssence->up_building[0].ordinary), "surfaces\\", strFile);
			strcpy(p.fn,strFile);
			m_pBtn[nIndex]->SetProperty("Up Frame File",&p);
			m_pBtn[nIndex]->SetProperty("Down Frame File",&p);
			m_pBtn[nIndex]->SetProperty("OnHover Frame File",&p);
			CECHostPlayer::Fac_BuildField* pInfo = GetHostPlayer()->GetBuildInfo(nIndex +1);
			if(pInfo)
			{
				AUICTranslate trans;
				ACString szDesc;
				szDesc.Format(GetStringFromTable(20201),m_pEssence->name,1,GetStringFromTable(pInfo->nStatus ? 20203 : 20202),m_pEssence->building_desc);
				m_pBtn[nIndex]->SetHint(trans.Translate(szDesc));
			}
		}
	}
}
void DlgGuildLandBuild::ResultBuildUpgrade(int nTempId, int nIndex, int nLevel)
{
	BUILDING_ESSENCE * m_pEssence = GetTempData(GetGame(),nTempId);
	if(m_pEssence)
	{
		if(nIndex <= 20)
		{
			AString strFile;
			AUIOBJECT_SETPROPERTY p;
			af_GetRelativePathNoBase(GetGame()->GetDataPath(m_pEssence->up_building[nLevel].ordinary), "surfaces\\", strFile);
			strcpy(p.fn,strFile);
			m_pBtn[nIndex]->SetProperty("Up Frame File",&p);
			m_pBtn[nIndex]->SetProperty("Down Frame File",&p);
			m_pBtn[nIndex]->SetProperty("OnHover Frame File",&p);
			CECHostPlayer::Fac_BuildField* pInfo = GetHostPlayer()->GetBuildInfo(nIndex + 1);
			if(pInfo)
			{
				AUICTranslate trans;
				ACString szDesc;
				szDesc.Format(GetStringFromTable(20201),m_pEssence->name,nLevel + 1,GetStringFromTable(pInfo->nStatus ? 20203 : 20202),m_pEssence->building_desc);
				m_pBtn[nIndex]->SetHint(trans.Translate(szDesc));
			}
		}
	}
}
void DlgGuildLandBuild::ResultBuildRemove(int nIndex)
{
	if(nIndex <= 20)
	{
		AUIOBJECT_SETPROPERTY p;
		strcpy(p.fn,"guildbuild\\GuildLand_01Y.tga");
		m_pBtn[nIndex-1]->SetProperty("Up Frame File",&p);
		m_pBtn[nIndex-1]->SetProperty("Down Frame File",&p);
		m_pBtn[nIndex-1]->SetProperty("OnHover Frame File",&p);
		AUICTranslate trans;
		ACString szDesc;
		szDesc.Format(_AL(""));
		m_pBtn[nIndex-1]->SetHint(trans.Translate(szDesc));

	}
}

void DlgGuildLandBuild::ResultBuildComplete(int nTempId, int nIndex , int nLevel)
{
	BUILDING_ESSENCE * m_pEssence = GetTempData(GetGame(),nTempId);
	if(m_pEssence)
	{
		if(nIndex <= 20)
		{
			AString strFile;
			AUIOBJECT_SETPROPERTY p;
			af_GetRelativePathNoBase(GetGame()->GetDataPath(m_pEssence->up_building[nLevel].ordinary), "surfaces\\", strFile);
			strcpy(p.fn,strFile);
			m_pBtn[nIndex]->SetProperty("Up Frame File",&p);
			m_pBtn[nIndex]->SetProperty("Down Frame File",&p);
			m_pBtn[nIndex]->SetProperty("OnHover Frame File",&p);
			CECHostPlayer::Fac_BuildField* pInfo = GetHostPlayer()->GetBuildInfo(nIndex +1);
			if(pInfo)
			{
				AUICTranslate trans;
				ACString szDesc;
				szDesc.Format(GetStringFromTable(20201),m_pEssence->name,pInfo->nLevel,GetStringFromTable(pInfo->nStatus ? 20203 : 20202),m_pEssence->building_desc);
				m_pBtn[nIndex]->SetHint(trans.Translate(szDesc));
			}
		}
	}
}

void DlgGuildLandBuild::ChangeBuildGrid(bool isChange)
{
	AUIOBJECT_SETPROPERTY p;
	AString strFile;
	BUILDING_ESSENCE * m_pEssence = NULL;
	for(int nIndex = 0; nIndex < 20; nIndex++)
	{
		CECHostPlayer::Fac_BuildField* pInfo = GetHostPlayer()->GetBuildInfo(nIndex + 1);
		if(isChange)
		{
			if(pInfo)
			{
				if(pInfo->nStatus == 1)
				{
					m_pEssence = GetTempData(GetGame(),pInfo->nId);
					af_GetRelativePathNoBase(GetGame()->GetDataPath(m_pEssence->up_building[pInfo->nLevel - 1].hightlight), "surfaces\\", strFile);
					strcpy(p.fn,strFile);
					m_pBtn[nIndex]->SetProperty("Up Frame File",&p);
					m_pBtn[nIndex]->SetProperty("Down Frame File",&p);
					m_pBtn[nIndex]->SetProperty("OnHover Frame File",&p);
				}
			}
			else
			{
				strcpy(p.fn,"guildbuild\\GuildLand_01Y.tga");
				m_pBtn[nIndex]->SetProperty("Up Frame File",&p);
				m_pBtn[nIndex]->SetProperty("Down Frame File",&p);
				m_pBtn[nIndex]->SetProperty("OnHover Frame File",&p);
			}
		}
		else
		{

			if(pInfo)
			{
				if(pInfo->nStatus == 1)
				{
					m_pEssence = GetTempData(GetGame(),pInfo->nId);
					af_GetRelativePathNoBase(GetGame()->GetDataPath(m_pEssence->up_building[pInfo->nLevel - 1].ordinary), "surfaces\\", strFile);
					strcpy(p.fn,strFile);
					m_pBtn[nIndex]->SetProperty("Up Frame File",&p);
					m_pBtn[nIndex]->SetProperty("Down Frame File",&p);
					m_pBtn[nIndex]->SetProperty("OnHover Frame File",&p);
				}
			}
			else
			{
				strcpy(p.fn,"guildbuild\\GuildLand_01Y.tga");
				m_pBtn[nIndex]->SetProperty("Up Frame File",&p);
				m_pBtn[nIndex]->SetProperty("Down Frame File",&p);
				m_pBtn[nIndex]->SetProperty("OnHover Frame File",&p);
			}
		}
	}
}

void DlgGuildLandBuild::OnEventButtonDBCLK(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	bool bOnIcon;
	POINT ptPos = pObj->GetPos();
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - ptPos.x - p->X;
	int y = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;
	PAUITREEVIEW pTree = (PAUITREEVIEW)pObj;
	P_AUITREEVIEW_ITEM pItem = pTree->HitTest(x, y, &bOnIcon);

	if( pItem && !bOnIcon && pTree->GetParentItem(pItem) != pTree->GetRootItem() )
	{
		int nNpc =  m_pTreeTask->GetItemData(pItem);
		ATaskTemplMan *pMan = GetGame()->GetTaskTemplateMan();
		const NPC_INFO *info = pMan->GetTaskNPCInfo(nNpc);
		if(info)
		{
			A3DVECTOR3 vec;
			int idInstance;
			vec.x = (float)(info->x);
			vec.y = (float)(info->y);
			vec.z = (float)(info->z);
			idInstance = info->map_id;
			g_ECAutoMove.SearchPath(GetHostPlayer()->GetPos(), vec, idInstance);
			m_pTreeTask->Show(false);
		}
		//AUIDialog *pMsgBox = NULL;
		//GetGameUIMan()->MessageBox("GuildReceiveTask", GetGameUIMan()->GetStringFromTable(20218), MB_OKCANCEL,
		//	A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		//pMsgBox->SetData();
	}

}