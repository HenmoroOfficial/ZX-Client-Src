// Filename	: DlgHelpInfo.h
// Creator	: Wang Dongliang
// Date		: 2013/08/29
// Desc		: 帮助信息界面

#include "DlgHelpInfo.h"
#include "ScriptValue.h"
#include "LuaState.h"
#include "LuaUtil.h"
#include "LuaScript.h"
#include "LuaFuncCommon.h"
#include "AUI//AUITreeView.h"
#include "AUI//AUISubDialog.h"
#include "AUI//AUILabel.h"
#include "EC_GameUIMan.h"
#include "EC_Algorithm.h"

#define  HELPINFO_NAME  "HelpInfo"
#define  LUA_HELPINFO_FILE  "Interfaces\\script\\interfaces\\helpuicontent.lua"

class CompareHelpInfo
{
public:
	CDlgHelpInfo::SortType m_St;
	bool	m_bInc;
	CompareHelpInfo(CDlgHelpInfo::SortType st, bool bInc=true) : m_St(st),m_bInc(bInc){}
	bool operator ()(CDlgHelpInfo::SHelpInfo & info1, CDlgHelpInfo::SHelpInfo & info2)
	{
		bool bRet = true;
		switch (m_St)
		{
		case CDlgHelpInfo::ST_ID:
			bRet = info1.iId < info2.iId;
			break;
		}
		if(m_bInc)
			return bRet;
		else
			return !bRet;
	}
};
class CompareSubHelpInfo
{
public:
	CDlgHelpInfo::SortType m_St;
	bool	m_bInc;
	CompareSubHelpInfo(CDlgHelpInfo::SortType st, bool bInc=true) : m_St(st),m_bInc(bInc){}
	bool operator ()(CDlgHelpInfo::SSubHelpInfo & info1, CDlgHelpInfo::SSubHelpInfo & info2)
	{
		bool bRet = true;
		switch (m_St)
		{
		case CDlgHelpInfo::ST_ID:
			bRet = info1.iId < info2.iId;
			break;
		}
		if(m_bInc)
			return bRet;
		else
			return !bRet;
	}
};

AUI_BEGIN_COMMAND_MAP(CDlgHelpInfo, CDlgBase)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(CDlgHelpInfo,CDlgBase)
AUI_END_EVENT_MAP()


bool CDlgHelpInfo::OnInitDialog()
{
	m_pTreeView = (PAUITREEVIEW)(GetDlgItem("Tv_Help"));
	m_pSubDlg   = (PAUISUBDIALOG)(GetDlgItem("Sub_01"));

	m_pState = g_LuaStateMan.GetConfigState();
	assert(m_pState);
	m_pState->Lock();
	CLuaScript * pScript = m_pState->RegisterFile(LUA_HELPINFO_FILE);
	if (!pScript)
	{
		m_pState->Unlock();
		return false;
	}
	bool rt = LoadHelpinfo();
	assert(rt);
	m_pState->Unlock();

	if(rt)
	  RefreshHelpInfoTree();
	
	return rt;
}
void CDlgHelpInfo::OnShowDialog()
{
   m_pCurSelected = NULL;
   m_pSubDlg->SetDialog(NULL);
}
bool CDlgHelpInfo::Release()
{
	ReleaseHelpInfo();
	return CDlgBase::Release();
}
bool CDlgHelpInfo::LoadHelpinfo()
{
	abase::vector<CScriptValue> args;
	abase::vector<CScriptValue> results;

	if (!m_pState->Call(HELPINFO_NAME, "GetSelf", args, results))
	{
		return false;
	}
	ACString strText;
	CScriptValue& ret0 = results[0];

	for (int i = 0; i < (int)ret0.m_ArrVal.size(); i++)
	{
		SHelpInfo info;
		if(!luaf::GetValue(info.iId,ret0.m_ArrVal[i],_AL("id")))
		{
			return false;
		}
		if(!luaf::GetValue(info.strMainName,ret0.m_ArrVal[i],_AL("name")))
		{
			return false;
		}
		abase::vector<const CScriptValue*> vecSubInfo ;
		if (luaf::GetValue(vecSubInfo, ret0.m_ArrVal[i], _AL("subinfo")))
		{
			int j;
			for (j=0; j<(int)vecSubInfo.size(); ++j)
			{
				SSubHelpInfo subinfo;
				if(!luaf::GetValue(subinfo.iId,*(vecSubInfo[j]),_AL("id")))
				{
				   return false;
				}
				if(!luaf::GetValue(strText,*(vecSubInfo[j]),_AL("subname")))
				{
					 return false;;
				}
				subinfo.strSubName = strText;
				if(!luaf::GetValue(strText,*(vecSubInfo[j]),_AL("dlgname")))
				{
					 return false;;
				}
				subinfo.strDlg = AC2AS(strText);
				info.vecSubInfo.push_back(subinfo);
			}
		}
		else
		{
		   return false;
		}

		m_HelpInfo.push_back(info);
	}

	SortHelpInfo();
	return true; 
}

void CDlgHelpInfo::ReleaseHelpInfo()
{
	for (int i=0;i<(int)m_HelpInfo.size();++i)
    {
		m_HelpInfo[i].vecSubInfo.clear();
    }
	m_HelpInfo.clear();
	abase::vector<AString> rmTbls;
	rmTbls.push_back(HELPINFO_NAME);
	m_pState->Lock();
	m_pState->UnRegister(LUA_HELPINFO_FILE, &rmTbls);
	m_pState->Unlock();
}
void CDlgHelpInfo::SortHelpInfo(SortType st,bool bInc)
{
   for (int i=0;i<(int)m_HelpInfo.size();++i)
   {
	   SHelpInfo& helpinfo = m_HelpInfo[i];
	   BubbleSort(helpinfo.vecSubInfo.begin(), helpinfo.vecSubInfo.end(), CompareSubHelpInfo(st,bInc));

   }
   BubbleSort(m_HelpInfo.begin(), m_HelpInfo.end(), CompareHelpInfo(st,bInc));
}
void CDlgHelpInfo::RefreshHelpInfoTree()
{
    P_AUITREEVIEW_ITEM  pTreeViewItem = NULL;
	P_AUITREEVIEW_ITEM  pSubTreeViewItem = NULL;
	P_AUITREEVIEW_ITEM  pRootItem = m_pTreeView->GetRootItem();

	for (int i=0;i<(int)m_HelpInfo.size();++i)
	{
		const SHelpInfo& helpinfo = m_HelpInfo[i];
        pTreeViewItem = m_pTreeView->InsertItem(helpinfo.strMainName,pRootItem);
		m_pTreeView->SetItemData(pTreeViewItem,1);
        m_pTreeView->SetItemDataPtr(pTreeViewItem,(void*)(&helpinfo));
		if(NULL != pTreeViewItem)
		{
		   for(int j=0;j<(int)helpinfo.vecSubInfo.size();++j)
		   {
		      const SSubHelpInfo& subinfo = helpinfo.vecSubInfo[j];
			  pSubTreeViewItem = m_pTreeView->InsertItem(subinfo.strSubName,pTreeViewItem);
			  m_pTreeView->SetItemData(pSubTreeViewItem,2);
			  m_pTreeView->SetItemDataPtr(pSubTreeViewItem,(void*)(&subinfo));
			  assert(m_Name2ItemTable.find(subinfo.strDlg) == m_Name2ItemTable.end());
			  m_Name2ItemTable[subinfo.strDlg] = pSubTreeViewItem;
		   }

		}
	}

}
void CDlgHelpInfo::OnTick()
{
   P_AUITREEVIEW_ITEM pSelectedItem = m_pTreeView->GetSelectedItem();

   if(pSelectedItem != m_pCurSelected)
   {
       int  iSubType = m_pTreeView->GetItemData(pSelectedItem) ;
      
	   //子类别
	   if(iSubType == 2)
	   {
		   SSubHelpInfo* pSubHelpInfo = (SSubHelpInfo*)(m_pTreeView->GetItemDataPtr(pSelectedItem));
		   if(NULL != pSubHelpInfo)
		   {
			   PAUIDIALOG pDlg = GetGameUIMan()->GetDialog(pSubHelpInfo->strDlg);
			   if(pDlg != m_pSubDlg->GetSubDialog())
			   { 
				  m_pSubDlg->SetDialog(pDlg);
                  if( stricmp(pSubHelpInfo->strDlg, "Win_Qshop_Vip_Help") == 0 )
				  {
					  PAUILABEL pLab = (PAUILABEL)pDlg->GetDlgItem("Lab_1");
					  pLab->SetText(GetGameUIMan()->GetStringFromTable(12992));
				  }
			   }
			  
		   }
           
	   }
	   m_pCurSelected = pSelectedItem;
       
   }
}
int  CDlgHelpInfo::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
    if(strMsg == "open")
	{
		AString strDlg = *((AString*)wParam);
		if(m_Name2ItemTable.find(strDlg) != m_Name2ItemTable.end())
		{
		   P_AUITREEVIEW_ITEM  pTreeViewItem = m_Name2ItemTable[strDlg];
		   if(NULL != pTreeViewItem)
		   {
               P_AUITREEVIEW_ITEM  pParentViewItem = m_pTreeView->GetParentItem(pTreeViewItem);
			   if(NULL != pParentViewItem)
			   {
                  m_pTreeView->Expand(pParentViewItem,AUITREEVIEW_EXPAND_EXPAND);
			   }
		       m_pTreeView->SelectItem(pTreeViewItem);
               if(!this->IsShow())
				   this->Show(true);
			   GetGameUIMan()->BringWindowToTop(this);
		   }
		}
		

	}
    return 0;
}