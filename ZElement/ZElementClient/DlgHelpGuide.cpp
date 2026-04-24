// Filename	: DlgHelpGuide.cpp
// Creator	: Wang Dongliang
// Date		: 2013/07/11
// Desc		: 指引相关界面
#include "AUI\\AUITreeView.h"
#include "AUI\\AUIScroll.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUIFrame.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUITextArea.h"
#include "AUI\\AUISubDialog.h"
#include "DlgHelpGuide.h"
#include "DlgInventory.h"
#include "DlgDailyPromp.h"
#include "DlgMiniMap.h"
#include "DlgSystem.h"
#include "DlgSystem2.h"
#include "DlgSystem3.h"
#include "DlgSystem4.h"
#include "DlgDynamicSystem.h"
//#include "A3DFuncs.h"
//#include "AUI\\AUIImagePicture.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_World.h"
#include "EC_HostPlayer.h"
#include "EC_GameUIMisc.h"
#include "EC_Utility.h"
#include "EC_AutoMove.h"
#include "EC_Instance.h"
#include "EC_Model.h"
//#include "EC_World.h"
//#include "EC_Inventory.h"
//#include "EC_IvtrItem.h"
//#include "EC_IvtrTaskItem.h"
#include "EC_TaskInterface.h"
#include "TaskTemplMan.h"
#include "elementdataman.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "EC_Configs.h"
#include "EC_Secretary.h"
#include "EC_NewHelpGuide.h"
#include "EC_Shortcut.h"
//#include "A3DFuncs.h"
#include "A3DEngine.h"
#include "A3DDevice.h"

extern ACString g_AutoMoveDestName;
//#define  GUIDE_SHOW_TIME  15000


//获取“任务追踪界面”指定任务ID的树控件结构
_AUITREEVIEW_ITEM* GetTaskTraceItemByTaskId(int taskid)
{
	CECTaskInterface *pTask = g_pGame->GetGameRun()->GetHostPlayer()->GetTaskInterface();
	if(!pTask->HasTask(taskid))
	return NULL;
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();  
	PAUIDIALOG pDlg = pGameUI->GetDialog("Win_QuestMinion");
	PAUITREEVIEW pTreeView = (PAUITREEVIEW)(pDlg->GetDlgItem("Tv_List"));
	P_AUITREEVIEW_ITEM pRootItem = pTreeView->GetRootItem();
	P_AUITREEVIEW_ITEM pMajorItem = pTreeView->GetFirstChildItem(pRootItem);

	while(pMajorItem)
	{
	    if(pTreeView->GetItemData(pMajorItem) == taskid)
	        break;
	    pMajorItem = pTreeView->GetNextSiblingItem(pMajorItem);
	}
	//假定其只有一个子树
	if(pMajorItem)
	{
	     P_AUITREEVIEW_ITEM pChildItem = pTreeView->GetFirstChildItem(pMajorItem);
	     if(pChildItem)
	     {  	
	        return pChildItem;
	     }
	}
	return NULL;
}
//将指定任务ID的树控件展开
void  ExpandTaskTraceItemByTaskId(int taskId)
{
	//将该任务展开
	P_AUITREEVIEW_ITEM pTreeItem = GetTaskTraceItemByTaskId(taskId);
	if(pTreeItem)
	{
		CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		PAUIDIALOG pDlg = pGameUI->GetDialog("Win_QuestMinion");
		PAUITREEVIEW pTreeView = (PAUITREEVIEW)(pDlg->GetDlgItem("Tv_List"));
		pTreeView->Expand(pTreeItem,AUITREEVIEW_EXPAND_EXPAND);
	}
}
//获取物品控件编号0-47(CECDLGSHOP_PACKMAX-1)
int GetInventoryItemByItemId(int itemid)
{
	CECInventory* pInventory = g_pGame->GetGameRun()->GetHostPlayer()->GetPack();
	int itemPos = pInventory->FindItem(itemid);
	//无此物品
	if(itemPos == -1) 
		return -1;
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	CDlgInventory* pPackDlg = (CDlgInventory*)(pGameUI->GetDialog("Win_Inventory"));
	//当前显示非普通物品页
	if(pPackDlg->GetShowItem() != CDlgInventory::INVENTORY_ITEM_NORMAL) 
		return -1;
	if(itemPos >= CECDLGSHOP_PACKMAX)
	{
		PAUISCROLL pScl_Item = (PAUISCROLL)(pPackDlg->GetDlgItem("Scl_Item"));
		if(pPackDlg->GetIsPageMode())
		{
            int page = itemPos/CECDLGSHOP_PACKMAX;
			int curpage = pScl_Item->GetBarLevel()/(CECDLGSHOP_PACKMAX/CECDLGSHOP_PACKLINEMAX);
			//此物品没有在当前页
			if(page != curpage) 
				return -1;
			itemPos = itemPos%CECDLGSHOP_PACKMAX;
		}
		else
		{
            int line = itemPos/CECDLGSHOP_PACKLINEMAX;
			int minlevel = line- CECDLGSHOP_PACKMAX/CECDLGSHOP_PACKLINEMAX +1;
			int maxlevel = minlevel + CECDLGSHOP_PACKMAX/CECDLGSHOP_PACKLINEMAX -1;
			int curlevel = pScl_Item->GetBarLevel();
			//此物品没有在当前页
			if(curlevel > maxlevel || curlevel < minlevel)
				return -1;
			int col  = itemPos%CECDLGSHOP_PACKLINEMAX;
			itemPos = (maxlevel-curlevel)*CECDLGSHOP_PACKLINEMAX+col;
		}
	}
	return itemPos;

}
//显示包裹中指定ID的物品
void ExpandInventoryItemByItemId(int itemid)
{
	CECInventory* pInventory = g_pGame->GetGameRun()->GetHostPlayer()->GetPack();
	int itemPos = pInventory->FindItem(itemid);
	//无此物品
	if(itemPos == -1) 
		return;
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	CDlgInventory* pPackDlg = (CDlgInventory*)(pGameUI->GetDialog("Win_Inventory"));
	//当前显示非普通物品页
	if(pPackDlg->GetShowItem() != CDlgInventory::INVENTORY_ITEM_NORMAL) 
		return;
	if(itemPos >= CECDLGSHOP_PACKMAX)
	{
		PAUISCROLL pScl_Item = (PAUISCROLL)(pPackDlg->GetDlgItem("Scl_Item"));
		if(pPackDlg->GetIsPageMode())
		{
			int page = itemPos/CECDLGSHOP_PACKMAX;
			int curpage = pScl_Item->GetBarLevel()/(CECDLGSHOP_PACKMAX/CECDLGSHOP_PACKLINEMAX);
			if(page != curpage) 
			{
			   pPackDlg->SetPage(page);
			}
		}
		else
		{
			int line = itemPos/CECDLGSHOP_PACKLINEMAX;
			int minlevel = line- CECDLGSHOP_PACKMAX/CECDLGSHOP_PACKLINEMAX +1;
			int maxlevel = minlevel + CECDLGSHOP_PACKMAX/CECDLGSHOP_PACKLINEMAX -1;
			int curlevel = pScl_Item->GetBarLevel();
			if(curlevel > maxlevel || curlevel < minlevel)
			{
			   pScl_Item->SetBarLevel((maxlevel+minlevel)/2);
			}
			
		}
	}
}
//获取NPC对话界面的列表框指定条目
bool GetNpcTalkListItemRect(int nline,tagPOINT& point,tagSIZE& size1,bool isMain)
{
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	PAUIDIALOG pNpcDlg = pGameUI->GetDialog(isMain ? "Win_NPC_Main" : "Win_NPC");
	PAUILISTBOX pListMain = (PAUILISTBOX)(pNpcDlg->GetDlgItem("Lst_Main"));
	if(pListMain->GetCount() <= nline)
		return false;
	//此处假定每个列表项只有一行一列
	AUIOBJECT_SETPROPERTY property;
	if(!pListMain->GetProperty("Frame mode",&property))
		return false;
	int framemode = property.i;
	if(!pListMain->GetProperty("Frame Image",&property))
		return false;
	AString frameimage;
	frameimage.Format("%s",property.fn);
	AUIFrame* pFrame = new AUIFrame;
	if(!pFrame)
       return false;
	if(!pFrame->Init(g_pGame->GetA3DEngine(),g_pGame->GetA3DDevice(),frameimage,framemode))
	{
	   delete pFrame;
	   return false;
	}
	int requireheight = pListMain->GetRequiredHeight();
	SIZE sLimit = { 0, 0 };
    sLimit = pFrame->GetSizeLimit();
	int lineHeight = (requireheight-1-sLimit.cy)/pListMain->GetCount();
	point.x = pListMain->GetPos().x;
	point.y = pListMain->GetPos().y + nline*lineHeight + sLimit.cy/2;
	size1.cx = pListMain->GetSize().cx;
	size1.cy = lineHeight;

	delete pFrame;
	return true;
}
AUI_BEGIN_COMMAND_MAP(CDlgHelpGuide, CDlgBase)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(CDlgHelpGuide,CDlgBase)
AUI_END_EVENT_MAP()
CDlgHelpGuide::CDlgHelpGuide()
:m_pDestDlg(NULL),
m_pDestControl(NULL),
m_iType(0),
m_iDlgType(0),
m_iTaskId(-1),
m_iItemId(-1),
m_iNpcIndex(-1),
m_iStepId(0)
{
}
void CDlgHelpGuide::GetDestPos(int& offsetX,int& offsetY,POINT point,SIZE size1)
{ 
   SIZE  size2 = this->GetSize();
   SIZE  size3 = this->GetDlgItem("Img_Arrow")->GetSize();
   POINT pos3  = this->GetDlgItem("Img_Arrow")->GetPos(true);
   int   width0  = 0;
   int   height0 = 0;
   //上箭头
   if(m_iType == 0)
   {
	   //offsetX = point.x + (size1.cx-size2.cx)/2;
	   offsetX = point.x + size1.cx/2 -size3.cx;
	   offsetY = point.y + (size1.cy + height0); 
   }
   //下箭头
   else if(m_iType == 1)
   {
	   //offsetX = point.x + (size1.cx-size2.cx)/2;
	   offsetX = point.x + size1.cx/2-pos3.x;
	   offsetY = point.y - (size2.cy + height0);
   }
   //左箭头
   else if(m_iType == 2)
   {
	   //offsetY = point.y + (size1.cy - size2.cy)/2;
	   offsetY = point.y + size1.cy/2 ;
	   offsetX = point.x + (size1.cx + width0);
   }
   //右箭头
   else if(m_iType == 3)
   {
	   //offsetY = point.y + (size1.cy - size2.cy)/2 + size2.cy/2;
	   offsetY = point.y + size1.cy /2;
	   offsetX = point.x - (size2.cx + width0);
   }
}

int  CDlgHelpGuide::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if(strMsg == "open")
	{
		CECNewHelpGuide::SGuideStep* pGuideStep = (CECNewHelpGuide::SGuideStep*)wParam;
		m_iStepId = pGuideStep->stepId;
		m_iType = pGuideStep->type;
		ACString strTip = WC2AC(pGuideStep->strDesc);
		((PAUITEXTAREA)(this->GetDlgItem("Txt_Help")))->SetText(strTip);
		
		if(pGuideStep->strDlg == L"HGForTaskTrace")
		{
		     m_iDlgType = EHGDT_TASKTRACE;
			 m_iTaskId = (int)lParam;
			 ExpandTaskTraceItemByTaskId(m_iTaskId);
			 
		}
		else if(pGuideStep->strDlg == L"HGForInventory")
		{
		     m_iDlgType = EHGDT_INVENTORY;
			 m_iItemId = (int)lParam;
			 ExpandInventoryItemByItemId(m_iItemId);
		}
		else if(pGuideStep->strDlg == L"HGForNpcTalk")
		{
		     m_iDlgType = EHGDT_NPCTALK;
			 m_iNpcIndex = (int)lParam;
		}
		else if(pGuideStep->strDlg == L"HGForNpcTalkMain")
		{
			m_iDlgType = EHGDT_NPCTALKMAIN;
			m_iNpcIndex = (int)lParam;
		}
		else
		{
			 m_iDlgType = EHGDT_COMMON;
		     m_pDestDlg = GetGameUIMan()->GetDialog(WC2AS(pGuideStep->strDlg));
		     m_pDestControl = m_pDestDlg->GetDlgItem(WC2AS(pGuideStep->strControl));
		}
		this->Show(true);
		
	}
	else if(strMsg == "close")
	{
		m_pDestControl = NULL;
		m_pDestDlg     = NULL;
		m_iType        = 0;
		m_iDlgType     = 0;
		m_iTaskId      =-1;
		m_iItemId      =-1;
		m_iNpcIndex    =-1;
		m_iStepId      = 0;
	    this->Show(false);
	}
	return 0;
}
void CDlgHelpGuide::OnTick()
{
	POINT point;
	SIZE  size1;
	//任务追踪
	if(m_iDlgType == EHGDT_TASKTRACE)
	{
	   m_pDestDlg = NULL;
	   m_pDestControl = NULL;
	   P_AUITREEVIEW_ITEM pTreeItem = GetTaskTraceItemByTaskId(m_iTaskId);
	   if(pTreeItem)
	   {
		   CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		   PAUIDIALOG pDlg = pGameUI->GetDialog("Win_QuestMinion");
		   PAUITREEVIEW pTreeView = (PAUITREEVIEW)(pDlg->GetDlgItem("Tv_List"));
           if(pTreeView->GetItemState(pTreeItem) & AUITREEVIEW_IS_EXPANDED)
		   {
			   m_pDestDlg = pDlg;
			   m_pDestControl = pTreeView;
			   int offSetWidth = 0;
			   //此处假设文本中都含有寻径图标
			   abase::vector<A2DSprite*>* pImageList = pGameUI->GetControlImgLst();
			   A2DSprite* pSpriteTransfer = (*pImageList)[CECGameUIMan::ICONLIST_TRANSFER];
			   if(pSpriteTransfer)
			   {
                   offSetWidth = pSpriteTransfer->GetWidth() * pGameUI->GetWindowScale();
			   }
			   A3DRECT  rect = pTreeView->GetItemRect(pTreeItem);
			   point.x = rect.left;
			   point.y = rect.top;
			   size1.cx = rect.Width()-offSetWidth;
			   size1.cy = rect.Height();
		   }
	   }
	}
	//包裹界面
	else if(m_iDlgType == EHGDT_INVENTORY)
	{
	   m_pDestDlg = NULL;
	   m_pDestControl = NULL;
	   int pos = GetInventoryItemByItemId(m_iItemId);
	   if(pos != -1)
	   {
		   CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		   m_pDestDlg = pGameUI->GetDialog("Win_Inventory");
		   AString strItem;
		   strItem.Format("Item_%02d",pos);
		   m_pDestControl = m_pDestDlg->GetDlgItem(strItem);
		   point = m_pDestControl->GetPos();
		   size1 = m_pDestControl->GetSize();
	   }
	}
	//NPC对话界面
	else if(m_iDlgType == EHGDT_NPCTALK)
	{
		m_pDestDlg = NULL;
		m_pDestControl = NULL;
	    if(GetNpcTalkListItemRect(m_iNpcIndex,point,size1))
		{
			m_pDestDlg = GetGameUIMan()->GetDialog("Win_NPC");
			m_pDestControl = m_pDestDlg->GetDlgItem("Lst_Main");
		}
	}
	//NPC主线对话界面
	else if(m_iDlgType == EHGDT_NPCTALKMAIN)
	{
		m_pDestDlg = NULL;
		m_pDestControl = NULL;
		if(GetNpcTalkListItemRect(m_iNpcIndex,point,size1,true))
		{
			m_pDestDlg = GetGameUIMan()->GetDialog("Win_NPC_Main");
			m_pDestControl = m_pDestDlg->GetDlgItem("Lst_Main");
		}
	}
	else if(m_iDlgType == EHGDT_COMMON)
	{
	   point = m_pDestControl->GetPos();
	   size1 = m_pDestControl->GetSize();
	}
    
	if(m_pDestDlg && m_pDestControl && m_pDestDlg->IsShow() && m_pDestControl->IsShow())
	{
        bool  bCanShow = true;
		//针对特定步骤进行特殊条件的判断
		{
			if(m_iStepId == ESTEP_SPECIAL_USESKILL)
			{
				bCanShow  = false;
				if(NULL != m_pDestControl->GetDataPtr("ptr_CECShortcut"))
				{
					CECShortcut* pShortCut = (CECShortcut*)(m_pDestControl->GetDataPtr("ptr_CECShortcut"));
                     if(pShortCut->GetType() == CECShortcut::SCT_SKILL)
					 {
					     bCanShow = true;
					 }
				}
			}
			else if(m_iStepId == ESTEP_SPECIAL_FIGHT)
			{
				bCanShow  = false;
				if(NULL != m_pDestControl->GetDataPtr("ptr_CECShortcut"))
				{
					CECShortcut* pShortCut = (CECShortcut*)(m_pDestControl->GetDataPtr("ptr_CECShortcut"));
					if(pShortCut->GetType() == CECShortcut::SCT_COMMAND || pShortCut->GetType() == CECShortcut::SCT_SKILL)
					{
						bCanShow = true;
					}
				}
			}
			else if(m_iStepId == ESTEP_SPECIAL_USEMEDICINE)
			{
				bCanShow  = false;
				if(NULL != m_pDestControl->GetDataPtr("ptr_CECShortcut"))
				{
					CECShortcut* pShortCut = (CECShortcut*)(m_pDestControl->GetDataPtr("ptr_CECShortcut"));
					if(pShortCut->GetType() == CECShortcut::SCT_ITEM)
					{
					   CECSCItem* pSCItem = (CECSCItem*)pShortCut;
					   if(pSCItem && pSCItem->GetItemTID() == 3999)
					   {
					        bCanShow = true;
					   }
					}
					
				}
			}
		}

	   if(bCanShow)
	   {
	      int posX = 0;
	      int posY = 0;
	      GetDestPos(posX,posY,point,size1);
	      this->SetPosEx(posX,posY);
          g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->InsertDialogBefore(this,m_pDestDlg);
	      return;
	   }
	}
	
   g_pGame->GetGameRun()->GetNewHelpGuide()->StopGuide();
	
}

AUI_BEGIN_COMMAND_MAP(CDlgHelpAimGuideMin, CDlgBase)
AUI_ON_COMMAND("Btn_Max",		OnCommand_Open)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(CDlgHelpAimGuideMin,CDlgBase)
AUI_END_EVENT_MAP()

//打开目的引导入口界面
void CDlgHelpAimGuideMin::OnCommand_Open(const char * szCommand)
{
    bool isshow = GetGameUIMan()->GetDialog("Win_Help_GameGuide")->IsShow();
    GetGameUIMan()->GetDialog("Win_Help_GameGuide")->Show(!isshow);
}

AUI_BEGIN_COMMAND_MAP(CDlgHelpAimGuideEnter, CDlgBase)
AUI_ON_COMMAND("Btn_Type*",		OnCommand_View)
AUI_ON_COMMAND("IDCANCEL",	OnCommand_Close)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(CDlgHelpAimGuideEnter,CDlgBase)
AUI_END_EVENT_MAP()

void CDlgHelpAimGuideEnter::OnCommand_View(const char * szCommand)
{
    int type = atoi(szCommand+strlen("Btn_Type"))-1;
	if(0 == GetGameUIMan()->CommonMessage("Win_Help_GameGuide_Detail","open",(int)type,0))
	{
		this->Show(false);
	}
	else
	{
	    GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(17929));
	}

}
void CDlgHelpAimGuideEnter::OnCommand_Close(const char* szCommand)
{
    this->Show(false);
}


AUI_BEGIN_COMMAND_MAP(CDlgHelpAimGuide, CDlgBase)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_Close)
AUI_ON_COMMAND("Btn_Return",		OnCommand_Return)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(CDlgHelpAimGuide,CDlgBase)
AUI_ON_EVENT("*",				WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_ON_EVENT(NULL,				WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_END_EVENT_MAP()

#define   MAX_COUNT_AIMGUIDE_PERPAGE  3
bool CDlgHelpAimGuide::OnInitDialog()
{
   m_pSclItem = (PAUISCROLL)GetDlgItem("Scl_01");
   m_pSubDlg1 = (PAUISUBDIALOG)GetDlgItem("Sub_01");
   m_pSubDlg2 = (PAUISUBDIALOG)GetDlgItem("Sub_02");
   m_pSubDlg3 = (PAUISUBDIALOG)GetDlgItem("Sub_03");
   return true;
}
void CDlgHelpAimGuide::OnCommand_Close(const char* szCommand)
{
    this->Show(false);
}
void CDlgHelpAimGuide::OnCommand_Return(const char* szCommand)
{
    this->Show(false);
	if(!GetGameUIMan()->GetDialog("Win_Help_GameGuide")->IsShow())
		GetGameUIMan()->GetDialog("Win_Help_GameGuide")->Show(true);
}
void CDlgHelpAimGuide::OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	int zDelta = (short)HIWORD(wParam);
	if( zDelta > 0 )
		zDelta = 1;
	else
		zDelta = -1;
	if( m_pSclItem->IsShow() )
		m_pSclItem->SetBarLevel(m_pSclItem->GetBarLevel() - zDelta);
}
int  CDlgHelpAimGuide::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if(strMsg == "open")
	{
		int type = (int)wParam;
		assert(type>= CECNewHelpGuide::EAIMGUIDE_GETMONEY && type< CECNewHelpGuide::EAIMGUIDE_NUM);
        CECNewHelpGuide* pHelpGuide = g_pGame->GetGameRun()->GetNewHelpGuide();
		pHelpGuide->SortAimGuideInfo(CECNewHelpGuide::ST_STARTLEVEL,false);
		pHelpGuide->GetAimGuideIndexs(type,m_vecIndex);
		if(m_vecIndex.size() > 0)
		{
		     m_nFirstLine = 0;
			 GetDlgItem("Lab_00")->SetText(GetGameUIMan()->GetStringFromTable(17930+type));
			if(m_vecIndex.size() > MAX_COUNT_AIMGUIDE_PERPAGE)
			{
				m_pSclItem->SetScrollRange(0, m_vecIndex.size()-MAX_COUNT_AIMGUIDE_PERPAGE);
				m_pSclItem->Show(true);
				m_pSclItem->SetBarLength(-1);
				m_pSclItem->SetScrollStep(1);
				m_pSclItem->SetBarLevel(0);
			}
			else
			{
				m_pSclItem->SetScrollRange(0, 0);
				m_pSclItem->SetBarLevel(0);
				m_pSclItem->Show(false);
			}
		    if(!this->IsShow())
		    {
		      this->Show(true);
		    }
			RefreshAimGuideInfo();
			return 0;
		}
		else
		{
		    return 1;
		}
	}
	else if(strMsg == "MouseWheel")
	{
        this->OnEventMouseWheel(wParam,lParam,NULL);
	}


	return 0;
}
void CDlgHelpAimGuide::OnTick()
{
	if( m_nFirstLine != m_pSclItem->GetBarLevel() )
	{
		m_nFirstLine = m_pSclItem->GetBarLevel();
		RefreshAimGuideInfo();
	}
}
void CDlgHelpAimGuide::RefreshAimGuideInfo()
{
    int nStart = m_nFirstLine * 1;

	CECNewHelpGuide* pHelpGuide = g_pGame->GetGameRun()->GetNewHelpGuide();
	CECNewHelpGuide::SAimGuideInfo* pInfo = NULL;


	if(nStart < (int)m_vecIndex.size())
	{
		int index = m_vecIndex[nStart];
		pInfo = pHelpGuide->GetAimGuideInfo(index);
		assert(pInfo != NULL);
		AString strDlg = pInfo->iId != -1 ? "Win_Help_GameGuide_DetailSub1" : "Win_Help_GameGuide_DetailSub4";
		PAUIDIALOG  pDlg =GetGameUIMan()->GetDialog(strDlg);
		m_pSubDlg1->SetDialog(pDlg);
		GetGameUIMan()->CommonMessage(strDlg,"open",(int)index,0);
		
	}
	else
	{
	    GetGameUIMan()->CommonMessage("Win_Help_GameGuide_DetailSub1","close",0,0);
		GetGameUIMan()->CommonMessage("Win_Help_GameGuide_DetailSub4","close",0,0);
	}
	if(nStart+1 < (int)m_vecIndex.size())
	{
		int index = m_vecIndex[nStart+1];
		pInfo = pHelpGuide->GetAimGuideInfo(index);
		assert(pInfo != NULL);
		AString strDlg = pInfo->iId != -1 ? "Win_Help_GameGuide_DetailSub2" : "Win_Help_GameGuide_DetailSub5";
		PAUIDIALOG  pDlg =GetGameUIMan()->GetDialog(strDlg);
		m_pSubDlg2->SetDialog(pDlg);
		GetGameUIMan()->CommonMessage(strDlg,"open",(int)index,0);
	}
	else
	{
		GetGameUIMan()->CommonMessage("Win_Help_GameGuide_DetailSub2","close",0,0);
		GetGameUIMan()->CommonMessage("Win_Help_GameGuide_DetailSub5","close",0,0);
	}
	if(nStart+2 < (int)m_vecIndex.size())
	{
		int index = m_vecIndex[nStart+2];
		pInfo = pHelpGuide->GetAimGuideInfo(index);
		assert(pInfo != NULL);
		AString strDlg = pInfo->iId != -1 ? "Win_Help_GameGuide_DetailSub3" : "Win_Help_GameGuide_DetailSub6";
		PAUIDIALOG  pDlg =GetGameUIMan()->GetDialog(strDlg);
		m_pSubDlg3->SetDialog(pDlg);
		GetGameUIMan()->CommonMessage(strDlg,"open",(int)index,0);
	}
	else
	{
		GetGameUIMan()->CommonMessage("Win_Help_GameGuide_DetailSub3","close",0,0);
		GetGameUIMan()->CommonMessage("Win_Help_GameGuide_DetailSub6","close",0,0);
	}
	
}

AUI_BEGIN_COMMAND_MAP(CDlgHelpAimGuideDetail2, CDlgHelpAimGuideDetail)
AUI_ON_COMMAND("Btn_OpenDlg",		OnCommand_OpenDlg)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(CDlgHelpAimGuideDetail2,CDlgHelpAimGuideDetail)
AUI_END_EVENT_MAP()

void CDlgHelpAimGuideDetail2::OnCommand_OpenDlg(const char * szCommand)
{
   if(m_strDlgName != "")
   {
	   CDlgMiniMap* pMiniMap = GetGameUIMan()->m_pDlgMiniMap;
       if(m_strDlgName == "ThemeLink")
       {
	        pMiniMap->OnCommand_ThemeLink("");
		    return;
       }
      PAUIDIALOG  pDlg =GetGameUIMan()->GetDialog(m_strDlgName);
	  if(pDlg && !pDlg->IsShow())
	  {
		  CDlgSystem*   pSysTem = GetGameUIMan()->m_pDlgSystem;
		  CDlgSystem2* pSysTem2 = GetGameUIMan()->m_pDlgSystem2;
		  CDlgSystem3* pSystem3 = GetGameUIMan()->m_pDlgSystem3;
		  CDlgSystem4* pSystem4 = GetGameUIMan()->m_pDlgSystem4;
		 

	      if(m_strDlgName == "Win_QShop")
		  {
              pSysTem->OnCommand_QShop("");
		  }
		  else if(m_strDlgName == "Win_FriendSeeking")
		  {
		      pSystem4->OnCommandFriendSeek("");
		  }
		  else if(m_strDlgName == "Win_RankList")
		  {
              pSysTem2->OnCommandRankList("");
		  }
		  else if(m_strDlgName == "Win_Quest")
		  {
			  if(!GetGameUIMan()->GetDialog("Win_Quest")->IsShow() && !GetGameUIMan()->GetDialog("Win_Quest_Main")->IsShow())
                   pSysTem->OnCommandQuest("wquest");
		  }
		  else if(m_strDlgName == "Win_GameCasual")
		  {
              pSystem4->OnCommandGameCasual("");
		  }
		  else if(m_strDlgName == "Win_Activity")
		  {
			  CDlgDynamicMin* pDynamicMin = (CDlgDynamicMin*)(GetGameUIMan()->GetDialog("Win_Activity_Min"));
		      pDynamicMin->OnCommandOpen("");
		  }
		  else if(m_strDlgName == "Win_QShop_vip")
		  {
              pMiniMap->OnCommand_Vip("");
		  }
		  else if(m_strDlgName == "Win_Auction")
		  {
              pMiniMap->OnCommand_Auction("");
		  }
		  else
		  {
               pDlg->Show(true);
		  }
	  }
   }
}
bool CDlgHelpAimGuideDetail2::RefreshAimGuideInfo(int index)
{
	m_strDlgName = "";
	CECNewHelpGuide* pHelpGuide = g_pGame->GetGameRun()->GetNewHelpGuide();
	CECNewHelpGuide::SAimGuideInfo* pInfo = pHelpGuide->GetAimGuideInfo(index);
	assert(pInfo != NULL);
	if(!pInfo)
		return false;
	GetDlgItem("Lab_Task")->SetText(WC2AC(pInfo->strName));
	PAUIIMAGEPICTURE pImageIcon = (PAUIIMAGEPICTURE)(GetDlgItem("Img_Task"));
	pImageIcon->ClearCover();
	pImageIcon->SetText(_AL(""));
	pImageIcon->SetColor(A3DCOLORRGB(255,255,255));
	pImageIcon->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_DAILYPROMP32],
		GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_DAILYPROMP32][pInfo->strIcon]);
	pImageIcon->SetHint(WC2AC(pInfo->strName));
	AString strName;
	for (int i =1; i<=5;++i)
	{
		strName.Format("Img_Star%02d",i);
		GetDlgItem(strName)->Show(pInfo->GetStarLevel() >= i);
	}
	GetDlgItem("Txt_Money")->SetText(WC2AC(pInfo->strCondition));
	GetDlgItem("Txt_Lv")->SetText(WC2AC(pInfo->strLevelInfo));
	GetDlgItem("Txt_Description")->SetText(WC2AC(pInfo->strDesc));
	GetDlgItem("Btn_OpenDlg")->SetText(WC2AC(pInfo->strBtnName));
	m_strDlgName = WC2AS(pInfo->strDlgName);
	return true;
}

AUI_BEGIN_COMMAND_MAP(CDlgHelpAimGuideDetail, CDlgBase)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(CDlgHelpAimGuideDetail,CDlgBase)
AUI_ON_EVENT("Txt_Map",		WM_LBUTTONDOWN,		OnEventLButtonDownMapPos)
AUI_ON_EVENT("Txt_Map",		WM_MOUSEMOVE,		OnEventMouseMoveMapPos)
AUI_ON_EVENT("*",				WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_ON_EVENT(NULL,				WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_END_EVENT_MAP()

int  CDlgHelpAimGuideDetail::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if(strMsg == "open")
	{
		int index = (int)wParam;
		if(!RefreshAimGuideInfo(index))
		    return 0;
		if(!this->IsShow())
		   this->Show(true);
	}
	else if(strMsg == "close")
	{
	   if(this->IsShow())
		   this->Show(false);
	}

	return 0;
}

bool CDlgHelpAimGuideDetail::RefreshAimGuideInfo(int index)
{
	CECNewHelpGuide* pHelpGuide = g_pGame->GetGameRun()->GetNewHelpGuide();
	CECNewHelpGuide::SAimGuideInfo* pInfo = pHelpGuide->GetAimGuideInfo(index);
	assert(pInfo != NULL);
	if(!pInfo)
	  return false;
	GetDlgItem("Lab_Task")->SetText(WC2AC(pInfo->strName));
	PAUIIMAGEPICTURE pImageIcon = (PAUIIMAGEPICTURE)(GetDlgItem("Img_Task"));
	pImageIcon->ClearCover();
	pImageIcon->SetText(_AL(""));
	pImageIcon->SetColor(A3DCOLORRGB(255,255,255));
	pImageIcon->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_DAILYPROMP32],
		GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_DAILYPROMP32][pInfo->strIcon]);
	pImageIcon->SetHint(WC2AC(pInfo->strName));
    AString strName;
	for (int i =1; i<=5;++i)
	{
		strName.Format("Img_Star%02d",i);
		GetDlgItem(strName)->Show(pInfo->GetStarLevel() >= i);
	}
	GetDlgItem("Txt_Money")->SetText(WC2AC(pInfo->strCondition));
	GetDlgItem("Txt_Lv")->SetText(WC2AC(pInfo->strLevelInfo));
	GetDlgItem("Txt_Date")->SetText(WC2AC(pInfo->strDay));
	GetDlgItem("Txt_Time")->SetText(WC2AC(pInfo->strTime));
	GetDlgItem("Txt_Description")->SetText(WC2AC(pInfo->strDesc));


	int iFinishCount = 0;
	CECTaskInterface* pTask = g_pGame->GetGameRun()->GetHostPlayer()->GetTaskInterface();
	ATaskTemplMan* pMan = g_pGame->GetTaskTemplateMan();
	int idTask = CDlgDailyPromp::GetSuitTask(pInfo->uiTaskID,pInfo->uiNumID);
	ATaskTempl* pTemp = pMan->GetTaskTemplByID(idTask);
	if (pTemp)
	{
		if (pTemp->m_ulMaxFinishCount>0)
		{
			TaskFinishTimeList* pList = (TaskFinishTimeList*)pTask->GetFinishedTimeList();
			TaskFinishTimeEntry* pEntry = pList->Search(idTask);
			if (pEntry)
				iFinishCount = pEntry->m_uFinishCount;
		}
		else if (pInfo->iPermitCount==1&&pTemp->CheckDeliverTime(pTask, pTask->GetCurTime()))
		{
			iFinishCount = 1;
		}
	}
 
	ACString strCount;
	if(pInfo->iPermitCount == -1)
	{
	   strCount = GetGameUIMan()->GetStringFromTable(7140);
	}
	else
	{
	   strCount.Format(_AL("%d/%d"),iFinishCount,pInfo->iPermitCount);
	}
	GetDlgItem("Txt_Frequency")->SetText(strCount);
     
    AUITextArea* pNpcMap = (AUITextArea*)(GetDlgItem("Txt_Map"));
	pNpcMap->SetImageList(GetGameUIMan()->GetControlImgLst());
	pNpcMap->SetItemMask(0xffffffff);
    pNpcMap->SetText(_AL(""));
	ACString  strText;
	ACString  strHint;

	if(pInfo->bTaskNpc)
	{
		elementdataman *pDataMan = GetGame()->GetElementDataMan();
		ATaskTemplMan *pMan = GetGame()->GetTaskTemplateMan();
		ATaskTempl* lstTask = pMan->GetTaskTemplByID(idTask);
		if (lstTask)
		{
			int nNPC = lstTask->GetDeliverNPC();
			if (nNPC>0)
			{
				DATA_TYPE dt;
				NPC_ESSENCE *pNPC = (NPC_ESSENCE *)pDataMan->get_data_ptr(
					nNPC, ID_SPACE_ESSENCE, dt);
				const NPC_INFO *info = pMan->GetTaskNPCInfo(nNPC);
				if( info && info->x != 0 && info->z != 0)
				{
					EditBoxItemBase item(enumEICoord);
					item.SetName(pNPC->name);
					ACHAR szPos[100];
					a_sprintf(szPos, _AL("%d %d %d %d"), info->x, info->y, info->z, info->map_id);
					item.SetInfo(szPos);
					item.SetColor(A3DCOLORRGB(0, 255, 0));
					ACString strName = (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
					CECInstance *pInstance = GetGameRun()->GetInstance(info->map_id);
					if( pInstance )
					{
						strText = pInstance->GetNameByCoord(info->x, info->z);
						strText += _AL(" ");
						strText += strName;
						strHint.Format(_AL("%d,%d"), info->x, info->z);
					}
					else
					{
						strText = strName;
						strHint.Format(_AL("%d,%d"), info->x, info->z);
					}

					if (pInfo->bIsTransport)
					{
						strText += _AL(" ");
						EditBoxItemBase itemImage(enumEIImage);
						itemImage.SetImageIndex(CECGameUIMan::ICONLIST_TRANSFER);
						itemImage.SetImageFrame(0);
						ACHAR szId[100];
						a_sprintf(szId, _AL("%d"),pInfo->iId);
						itemImage.SetInfo(szId);
						strText += (ACHAR)AUICOMMON_ITEM_CODE_START + itemImage.Serialize();
					}
				}
				else
				{
					strText.Format(GetStringFromTable(GetCheckedRadioButton(1)==1 ? 6009 : 6010), pNPC->name);
				}
			}
			pNpcMap->SetText(strText);
		}
		pNpcMap->SetHint(strHint);
		m_strHintNpc = strHint;
	}
	else
	{
		
		ACString strName;
		int x, y, z;
		a_sscanf(pInfo->strNpcPos, _AL("%d,%d,%d"), &x, &y, &z);
		const int invalidPosTag = 9999;
		if (x!=invalidPosTag&&y!=invalidPosTag)
		{
			EditBoxItemBase item(enumEICoord);
			item.SetName(pInfo->strNPC);
			y = 0;
			ACHAR szPos[100];
			a_sprintf(szPos, _AL("%d %d %d %d"), x, y, z, pInfo->iNpcMapid);
			item.SetInfo(szPos);
			item.SetColor(A3DCOLORRGB(0, 255, 0));
			strName = (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
			strHint.Format(_AL("%d,%d"), x, z);
		}
		else
			strName = pInfo->strNPC;
		CECInstance *pInstance = GetGameRun()->GetInstance(pInfo->iNpcMapid);
		if( pInstance )
		{
			strText = pInstance->GetNameByCoord((float)x, (float)z);
			strText += _AL(" ");
			strText += strName;
		}
		else
		{
			strText = pInfo->strNPC;
		}

		if (pInfo->bIsTransport)
		{
			strText += _AL(" ");
			EditBoxItemBase itemImage(enumEIImage);
			itemImage.SetImageIndex(CECGameUIMan::ICONLIST_TRANSFER);
			itemImage.SetImageFrame(0);
			ACHAR szId[100];
			a_sprintf(szId, _AL("%d"), pInfo->iId);
			itemImage.SetInfo(szId);
			strText += (ACHAR)AUICOMMON_ITEM_CODE_START + itemImage.Serialize();
		}
		pNpcMap->SetText(strText);
		pNpcMap->SetHint(strHint);
		m_strHintNpc = strHint;
	}
	return true;
}
void CDlgHelpAimGuideDetail::OnEventLButtonDownMapPos(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - p->X;
	int y = GET_Y_LPARAM(lParam) - p->Y;

	AUITEXTAREA_EDITBOX_ITEM Item;
	Item.m_pItem = NULL;
	GetItemLinkOnText(x, y, (PAUITEXTAREA)pObj, &Item);

	if( Item.m_pItem != NULL )
	{
		if( Item.m_pItem->GetType() == enumEICoord )
		{
			A3DVECTOR3 vec;
			int idInstance;
			a_sscanf(Item.m_pItem->GetInfo(), _AL("%f%f%f%d"), 
				&vec.x, &vec.y, &vec.z, &idInstance);
			((CDlgMiniMap*)GetGameUIMan()->GetDialog("Win_MiniMap"))->m_dwTempTargetTime = GetTickCount();
			((CDlgMiniMap*)GetGameUIMan()->GetDialog("Win_MiniMap"))->m_vecTempTarget = vec;
			((CDlgMiniMap*)GetGameUIMan()->GetDialog("Win_MiniMap"))->m_idTempTargetInstance = idInstance;

			g_ECAutoMove.SearchPath(GetHostPlayer()->GetPos(), vec, idInstance);
			g_AutoMoveDestName = Item.m_pItem->GetName();

			if( idInstance == GetWorld()->GetInstanceID() && GetHostPlayer()->GetModelTempArrow() )
				GetHostPlayer()->GetModelTempArrow()->PlayActionByName("动画", 1.0f, true);
		}
		else if( Item.m_pItem->GetType() == enumEIImage )
		{
			int id;
			a_sscanf(Item.m_pItem->GetInfo(), _AL("%d"), &id);
			GetGameUIMan()->CheckCanDailyPrompFly(id);
		}
	}
}
void CDlgHelpAimGuideDetail::OnEventMouseMoveMapPos(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	AUITEXTAREA_EDITBOX_ITEM Item;
	Item.m_pItem = NULL;
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - p->X;
	int y = GET_Y_LPARAM(lParam) - p->Y;
	GetItemLinkOnText(x, y, (PAUITEXTAREA)pObj, &Item);
	pObj->SetHint(m_strHintNpc);
	if( Item.m_pItem != NULL )
	{
		if( Item.m_pItem->GetType() == enumEIImage )
		{
			pObj->SetHint(GetGameUIMan()->GetStringFromTable(3302));
		}
	}	
}
void CDlgHelpAimGuideDetail::OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
   AUITextArea* pTextArea = (AUITextArea*)(GetDlgItem("Txt_Description"));
   A3DRECT rcWindow = GetGameUIMan()->GetRect();
   int x = GET_X_LPARAM(lParam) - rcWindow.left;
   int y = GET_Y_LPARAM(lParam) - rcWindow.top;
   POINT ptClient = { 0, 0 };
   ClientToScreen(GetGameUIMan()->GetHWnd() ? GetGameUIMan()->GetHWnd() : g_pGame->GetA3DDevice()->GetDeviceWnd(), &ptClient);
   x -= ptClient.x;
   y -= ptClient.y;

   if(!pTextArea->IsInHitArea(x,y,WM_MOUSEWHEEL))
       GetGameUIMan()->CommonMessage("Win_Help_GameGuide_Detail","MouseWheel",wParam,lParam);
}


AUI_BEGIN_COMMAND_MAP(CDlgHelpWelcom, CDlgBase)
AUI_ON_COMMAND("Btn_Confirm",		OnCommand_Agree)
AUI_ON_COMMAND("Btn_Close",		    OnCommand_DisAgree)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(CDlgHelpWelcom,CDlgBase)
AUI_END_EVENT_MAP()

void CDlgHelpWelcom::OnCommand_Agree(const char * szCommand)
{
    EC_GAME_SETTING gs = g_pGame->GetConfigs()->GetGameSettings();
    gs.bHideSecretary = false;
    g_pGame->GetConfigs()->SetGameSettings(gs);
	g_pGame->GetGameRun()->GetSecretary()->SetHelpGuideEnable(true);
	NewHelpGuide::FinishGuide_Welcome();
    NewHelpGuide::StartGuide_Move();
    Show(false);
	
}
void CDlgHelpWelcom::OnCommand_DisAgree(const char * szCommand)
{
	EC_GAME_SETTING gs = g_pGame->GetConfigs()->GetGameSettings();
	gs.bHideSecretary = true;
	g_pGame->GetConfigs()->SetGameSettings(gs);
	g_pGame->GetGameRun()->GetSecretary()->SetHelpGuideEnable(false);
	Show(false);
	NewHelpGuide::FinishGuide_Welcome();
}

AUI_BEGIN_COMMAND_MAP(CDlgHelpMove, CDlgBase)
AUI_ON_COMMAND("Btn_Confirm",		OnCommand_Confirm)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(CDlgHelpMove,CDlgBase)
AUI_END_EVENT_MAP()
bool CDlgHelpMove::OnInitDialog()
{
    m_pImageKeyBoard = (AUIImagePicture*)(GetDlgItem("Img_01"));
	m_pImageMouse = (AUIImagePicture*)(GetDlgItem("Img_02"));
	return true;
}
void CDlgHelpMove::OnShowDialog()
{
   m_pImageKeyBoard->FixFrame(0);
   m_pImageMouse->FixFrame(0);
   m_dwStartTime = 0;
}
void CDlgHelpMove::OnCommand_Confirm(const char * szCommand)
{
    Show(false);
	if(m_dwStartTime != 0)
	  NewHelpGuide::FinishGuide_Move();
	else
	  NewHelpGuide::StopGuide();

}
int CDlgHelpMove::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if(!IsShow())
	   return 0;

    if (strMsg == "KeyboradMove")
    {
       m_pImageKeyBoard->FixFrame(1);
    }
	else if(strMsg == "MouseMove")
	{
	   m_pImageMouse->FixFrame(1);
	}

	if(m_dwStartTime == 0 && m_pImageKeyBoard->GetFixFrame() == 1 && m_pImageMouse->GetFixFrame() == 1)
	{
		m_dwStartTime = GetTickCount();
	}
	return 0;
}
void CDlgHelpMove::OnTick() 
{
	if(m_dwStartTime != 0 && m_pImageKeyBoard->GetFixFrame() == 1 && m_pImageMouse->GetFixFrame() == 1)
	{
		if(GetTickCount()-m_dwStartTime>=1000)
		{
		    OnCommand_Confirm("");
		}
	}
}
AUI_BEGIN_COMMAND_MAP(CDlgHelpFight, CDlgBase)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_Close)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(CDlgHelpFight,CDlgBase)
AUI_END_EVENT_MAP()

void CDlgHelpFight::OnCommand_Close(const char * szCommand)
{
	NewHelpGuide::StopGuide();
	Show(false);
}
int CDlgHelpFight::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if(!IsShow())
		return 0;

	if (strMsg == "KeyboradTab")
	{
		if(NewHelpGuide::StartGuide_Step_2(GUIDE_HELP_FIGHT_2))
		    Show(false);
	}
	
	return 0;
}