// Filename	: DlgLoginServerList.cpp
// Creator	: Xiao Zhou
// Date		: 2005/11/4

#include <AFileImage.h>

#include "AFI.h"
#include "A3DEngine.h"
#include "A3DViewPort.h"
#include "DlgLoginServerSearch.h"
#include "DlgLoginServerList.h"
#include "AUI\\CSplit.h"
#include "AIniFile.h"
#include "EC_LoginUIMan.h"
#include "EC_Game.h"
#include "EC_Global.h"
#include "EC_GameRun.h"
#include "EC_GameSession.h"
#include "gameclient.h"
#include "LuaState.h"
#include <windowsx.h>
#include "EC_Resource.h"

#define SVR_HIS_FILE   "userdata\\server\\serverhis.lua"
#define SVR_HIS_TBL    "ServerHis"

AUI_BEGIN_COMMAND_MAP(DlgLoginServerSearch, AUIDialog)

AUI_ON_COMMAND("Btn_Search",		OnCommandSearch)
AUI_ON_COMMAND("DEFAULT_Edt_1",		OnCommandSearch)
AUI_ON_COMMAND("IDCANCEL",			OnCommandCancel)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(DlgLoginServerSearch, AUIDialog)

AUI_ON_EVENT("Txt_Content",	WM_LBUTTONDOWN,		OnEventLButtonDown_Txt_Info)
AUI_ON_EVENT("Txt_Content",	WM_MOUSEMOVE,		OnEventMouseMove_Txt_Info)

AUI_END_EVENT_MAP()

//查询合服信息
struct   SvrHisInfo 
{
	AWString  group;
	bool      alive;
	AWString  curSvr;
};

bool _RetrieveSvrHistory(const AWString& szSvr, abase::vector<SvrHisInfo>&  vecHisInfo)
{
	vecHisInfo.clear();
	CLuaState * pState = g_LuaStateMan.GetAIState();
	abase::vector<CScriptValue>  args, results;
	CScriptValue  param;
	param.SetVal(szSvr);
	args.push_back(param);
	if (!pState->Call(SVR_HIS_TBL, "GetHistory", args, results) 
		|| results.size() !=1 
		|| results[0].m_Type != CScriptValue::SVT_ARRAY
		|| results[0].m_ArrKey.empty())
	{
		return false;
	}
	
	unsigned int i;
	for (i = 0; i < results[0].m_ArrKey.size(); ++i)
	{
		SvrHisInfo info;
		results[0].m_ArrKey[i].RetrieveAWString(info.group);
		CScriptValue &val = results[0].m_ArrVal[i];
		info.alive = val.m_ArrVal[0].GetBool();
		if (!info.alive)
		{
			val.m_ArrVal[1].RetrieveAWString(info.curSvr);
		}
		vecHisInfo.push_back(info);
	}
	
	return true;
}

DlgLoginServerSearch::DlgLoginServerSearch()
{
	m_bOnItem = false;
	m_pTxtInfo = NULL;
	m_vecGroups.clear();
	m_vecServers.clear();
}

DlgLoginServerSearch::~DlgLoginServerSearch()
{
	m_vecGroups.clear();
	m_vecServers.clear();
	if (g_LuaStateMan.GetAIState())
	{
		abase::vector<AString> tbl;
		tbl.push_back(SVR_HIS_TBL);
		g_LuaStateMan.GetAIState()->UnRegister(SVR_HIS_FILE, &tbl);
	}

}

bool DlgLoginServerSearch::OnInitDialog()
{
	DDX_Control("Txt_Content", m_pTxtInfo);
	//加载合服表
	CLuaState * pState = g_LuaStateMan.GetAIState();
	assert(pState != NULL);
	pState->RegisterFile(SVR_HIS_FILE);


	return true;
}

void DlgLoginServerSearch::OnShowDialog()
{
}

void DlgLoginServerSearch::OnTick()
{
/*	CECLoginUIMan *pLoginUIMan = (CECLoginUIMan *)m_pAUIManager;
	AlignTo(pLoginUIMan->GetDialog("Win_LoginServerListSearch"),
		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_LEFT,
		AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_BOTTOM);
*/}

void DlgLoginServerSearch::OnCommandSearch(const char* szCommand)
{
	PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("DEFAULT_Edt_1");
	SetInfo(pEdt->GetText());
}

void DlgLoginServerSearch::OnCommandCancel(const char* szCommand)
{
	Show(false);
	//	CECLoginUIMan *pLoginUIMan = (CECLoginUIMan *)m_pAUIManager;
	//	pLoginUIMan->GetDialog("Win_LoginServerListSearch")->Show(false);
	//	m_bOnItem = false;
}

void DlgLoginServerSearch::OnEventLButtonDown_Txt_Info(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - p->X;
	int y = GET_Y_LPARAM(lParam) - p->Y;
	
	AUITEXTAREA_EDITBOX_ITEM Item;
	Item.m_pItem = NULL;
	GetItemLinkItemOn(x, y, pObj, &Item);
	
	if( Item.m_pItem != NULL )
	{
		if( Item.m_pItem->GetType() == enumEICoord )
		{
			int iIndex = Item.m_pItem->GetMsgIndex();
			a_sscanf(Item.m_pItem->GetInfo(), _AL("%d"), 
				&iIndex);
			CECLoginUIMan *pLoginUIMan = (CECLoginUIMan *)m_pAUIManager;
			CDlgLoginServerList *pDlgServerList = (CDlgLoginServerList *)pLoginUIMan->GetDialog("Win_LoginServerList");
			pDlgServerList->ChangeGroup(m_vecGroups[iIndex], m_vecServers[iIndex]);
			OnCommandCancel("");
// 			A3DVECTOR3 vec;
// 			int idInstance;
// 			a_sscanf(Item.m_pItem->GetInfo(), _AL("%f%f%f%d"), 
// 				&vec.x, &vec.y, &vec.z, &idInstance);
// 			//			if( idInstance != GetWorld()->GetInstanceID() )
// 			//			{
// 			//				PAUIDIALOG pMsgBox;
// 			//				m_pAUIManager->MessageBox("", GetStringFromTable(762), MB_OK,
// 			//					A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
// 			//				pMsgBox->SetLife(3);
// 			//			}
// 			//			else
// 			{
// 				GetGameUIMan()->m_pDlgMiniMap->m_dwTempTargetTime = GetTickCount();
// 				GetGameUIMan()->m_pDlgMiniMap->m_vecTempTarget = vec;
// 				GetGameUIMan()->m_pDlgMiniMap->m_idTempTargetInstance = idInstance;
// 				
// 				g_ECAutoMove.SearchPath(GetHostPlayer()->GetPos(), vec, idInstance);
// 				
// 				if( idInstance == GetWorld()->GetInstanceID() && GetHostPlayer()->GetModelTempArrow() )
// 					GetHostPlayer()->GetModelTempArrow()->PlayActionByName("动画", 1.0f, true);
//			}
		}
	}

}

void DlgLoginServerSearch::OnEventMouseMove_Txt_Info(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	AUITEXTAREA_EDITBOX_ITEM Item;
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - p->X;
	int y = GET_Y_LPARAM(lParam) - p->Y;
	GetItemLinkItemOn(x, y, pObj, &Item);
}

void DlgLoginServerSearch::GetItemLinkItemOn(int x, int y,
								 PAUIOBJECT pObj, AUITEXTAREA_EDITBOX_ITEM *pLink)
{
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	POINT ptPos = pObj->GetPos();
	PAUITEXTAREA pText = (PAUITEXTAREA)pObj;
	
	g_pGame->ChangeCursor(RES_CUR_NORMAL);
	m_bOnItem = false;
	if( pText->GetHitArea(x - ptPos.x, y - ptPos.y) == AUITEXTAREA_RECT_FRAME )
	{
		int i;
		abase::vector<AUITEXTAREA_EDITBOX_ITEM> &vecItemLink = pText->GetEditBoxItems();
		
		x += p->X;
		y += p->Y;
		for( i = 0; i < (int)vecItemLink.size(); i++ )
		{
			if( (vecItemLink[i].m_pItem->GetType() == enumEIIvtrlItem ||
				vecItemLink[i].m_pItem->GetType() == enumEICoord) &&
				vecItemLink[i].rc.PtInRect(x, y) )
			{
				g_pGame->ChangeCursor(RES_CUR_HAND);
				*pLink = vecItemLink[i];
				m_bOnItem = true;
				break;
			}
		}
	}
}

void DlgLoginServerSearch::SetInfo(ACString strInfo)
{
	m_vecGroups.clear();
	m_vecServers.clear();
	bool bAlive = false;
	//const AWString szGroup = g_pGame->GetGameRun()->GetCurGroup();
	const AWString szSvr = strInfo;
	abase::vector<SvrHisInfo>  vecHisInfo;
	bool bRet = _RetrieveSvrHistory(szSvr, vecHisInfo);
	CECLoginUIMan *pLoginUIMan = (CECLoginUIMan *)m_pAUIManager;
	if (!bRet)
	{
		pLoginUIMan->MessageBox("Login_SearchServer", GetStringFromTable(251), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	AWString strText;
	AWString strTemp;

	int iSize = vecHisInfo.size();
	for(int i=0; i<iSize; i++)
	{
		if (i!=0)
		{
			strText += _AL("\r\r\r");
		}
		strTemp.Format(GetStringFromTable(252),szSvr);
		strText += strTemp;

		SvrHisInfo info = vecHisInfo[i];
		if (info.alive)
		{
			strTemp.Format(GetStringFromTable(253),info.group);
			EditBoxItemBase item(enumEICoord);
			item.SetName(GetStringFromTable(255));
			item.SetColor(A3DCOLORRGB(0, 255, 0));
			ACHAR szIndex[100];
			a_sprintf(szIndex, _AL("%d"),i);
			item.SetInfo(szIndex);
			m_vecGroups.push_back(info.group);
			m_vecServers.push_back(szSvr);
			AWString strEnter = (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
			strText += strEnter;
		} 
		else
		{
			strTemp.Format(GetStringFromTable(254),info.group,info.curSvr);
			EditBoxItemBase item(enumEICoord);
			item.SetName(GetStringFromTable(255));
			item.SetColor(A3DCOLORRGB(0, 255, 0));
			ACHAR szIndex[100];
			a_sprintf(szIndex, _AL("%d"),i);
			item.SetInfo(szIndex);
			m_vecGroups.push_back(info.group);
			m_vecServers.push_back(info.curSvr);
			AWString strName = (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
			strTemp += strName;
		}
		strText += _AL("\r\r");
		strText += strTemp;
	}
	m_pTxtInfo->SetText(strText);
}