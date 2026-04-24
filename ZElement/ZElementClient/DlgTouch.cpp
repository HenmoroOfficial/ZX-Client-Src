// Filename	: DlgTouch.cpp
// Creator	: Wang Dongliang
// Date		: 2013/04/10
// Desc		: TOUCH合作相关界面
#include "AUI\\AUIEditBox.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "DlgTouch.h"
#include "EC_GameUIMan.h"
#include "EC_IvtrItem.h"
#include "EC_GPDataType.h"
#include "EC_GameSession.h"
#include "EC_TaskInterface.h"
#include "TaskTemplMan.h"
#include "touchpointexchange_re.hpp"
#include "gettouchpoint_re.hpp"
#include "getpassportcashadd_re.hpp"

#define  TOUCH_AWARD_ITEM 55977
#define  TOUCH_GOLD_ITEM  23314
#define  TOUCH_SILVER_ITEM 23315
#define  TOUCH_TASK_ID 28317
#define  TOUCH_GOLD_PRICE 50
#define  TOUCH_SILVER_PRICE 5

static int GetRCoin(int64_t  cashadd)
{
	int coin = 0;
	if(cashadd >= 10000*100)
		coin = 200000;
	else if(cashadd >= 5000*100)
		coin = 100000;
	else if(cashadd >= 2000*100)
		coin = 60000;
	else if(cashadd >= 1000*100)
		coin = 30000;
	else if(cashadd >= 500*100)
		coin = 20000;
	else if(cashadd >= 300*100)
		coin = 15000;
	else if(cashadd >= 100*100)
		coin = 5000;
	return coin;
	   
}
static bool IsMaxPoint(int64_t point)
{
   return point == 3000; 
}


AUI_BEGIN_COMMAND_MAP(CDlgTouchMin, CDlgBase)
AUI_ON_COMMAND("Btn_Touch",		OnCommand_Open)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(CDlgTouchMin,CDlgBase)
AUI_END_EVENT_MAP()
void CDlgTouchMin::OnCommand_Open(const char * szCommand)
{
   if(GetHostPlayer()->GetSrcServerID() != 0)
   {
	   GetGameUIMan()->MessageBox("", GetStringFromTable(17900), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	   return;
   }
   this->Show(false);
   GetGameUIMan()->SetShowTouchMin(false);
   //GetGameUIMan()->DisplayMiniButtons();
   if(GetGameUIMan()->GetDialog("Win_Game_Touch")->IsShow())  return;
   GetGameSession()->touch_getpassportcashadd(GetHostPlayer()->GetCharacterID());
   GetGameSession()->touch_getpoint(GetHostPlayer()->GetCharacterID());
   
}
AUI_BEGIN_COMMAND_MAP(CDlgTouchInfo, CDlgBase)
AUI_ON_COMMAND("Btn_Touch",		OnCommand_Open)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_Close)
AUI_ON_COMMAND("Btn_Exchange02",		OnCommand_ExchangeSilver)
AUI_ON_COMMAND("Btn_Exchange01",		OnCommand_ExchangeGold)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(CDlgTouchInfo,CDlgBase)
AUI_END_EVENT_MAP()
bool CDlgTouchInfo::OnInitDialog()
{
	CECIvtrItem *pItem = NULL;
	pItem = CECIvtrItem::CreateItem(TOUCH_AWARD_ITEM, 0, 1);
	if(pItem)
	{
		AUIImagePicture* pObj =(AUIImagePicture*)GetDlgItem("Img_55977");
		GetGameUIMan()->SetIvtrInfo(pObj,pItem);
		delete pItem;
		pItem =NULL;
	}
	pItem = CECIvtrItem::CreateItem(TOUCH_GOLD_ITEM, 0, 1);
	if(pItem)
	{
		AUIImagePicture* pObj =(AUIImagePicture*)GetDlgItem("Img_23314");
		GetGameUIMan()->SetIvtrInfo(pObj,pItem);
		delete pItem;
		pItem =NULL;
	}
	pItem = CECIvtrItem::CreateItem(TOUCH_SILVER_ITEM, 0, 1);
	if(pItem)
	{
		AUIImagePicture* pObj =(AUIImagePicture*)GetDlgItem("Img_23315");
		GetGameUIMan()->SetIvtrInfo(pObj,pItem);
		delete pItem;
		pItem =NULL;
	}
	m_bPointReady = false;
	m_bCashReady = false;
	m_iPoint     = 0;
	return true;
}
void CDlgTouchInfo::OnTick()
{
	if(GetHostPlayer()->GetSrcServerID() != 0)
	{
		Show(false);
		return;
	}
	ATaskTemplMan *pMan = GetGame()->GetTaskTemplateMan();	
	CECTaskInterface *pTaskInterafce = GetHostPlayer()->GetTaskInterface();
    FinishedTaskList* pTaskFinished = static_cast<FinishedTaskList*>(pTaskInterafce->GetFinishedTaskList());
    ATaskTempl *pTemp = pMan->GetTaskTemplByID(TOUCH_TASK_ID);
	if( !pTemp )
		return;
	GetDlgItem("Lab_Award")->Show(!(pTaskFinished->SearchTask(TOUCH_TASK_ID) == 0));
	GetDlgItem("Img_55977")->Show(!(pTaskFinished->SearchTask(TOUCH_TASK_ID) == 0));

}
void CDlgTouchInfo::OnCommand_Open(const char * szCommand)
{
	GetGameSession()->open_url_by_token("touch",30000,0);
	ATaskTemplMan *pMan = GetGame()->GetTaskTemplateMan();	
	CECTaskInterface *pTaskInterafce = GetHostPlayer()->GetTaskInterface();
    FinishedTaskList* pTaskFinished = static_cast<FinishedTaskList*>(pTaskInterafce->GetFinishedTaskList());
    ATaskTempl *pTemp = pMan->GetTaskTemplByID(TOUCH_TASK_ID);
	if( !pTemp || pTaskFinished->SearchTask(TOUCH_TASK_ID) == 0)
		return;
    GetGame()->GetTaskTemplateMan()->ManualTrigTask(GetHostPlayer()->GetTaskInterface(), TOUCH_TASK_ID);
}
void CDlgTouchInfo::OnCommand_Close(const char * szCommand)
{
   this->Show(false);
}
void CDlgTouchInfo::OnCommand_ExchangeGold(const char * szCommand)
{
    PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Game_TouchExchange");
	pDlg->SetData(1);
	pDlg->Show(true,true);
}
void CDlgTouchInfo::OnCommand_ExchangeSilver(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Game_TouchExchange");
	pDlg->SetData(2);
	pDlg->Show(true,true);
}
int CDlgTouchInfo::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
   if (strMsg == "getpoint")
   {
	   GetTouchPoint_Re* p = (GetTouchPoint_Re*)wParam;
	   if(p->retcode == ERR_SUCCESS)
	   {
            int64_t points = p->points;
			ACString strText;
			strText.Format(_AL("%d"),points);
			((PAUILABEL)GetDlgItem("Txt_Score"))->SetText(strText);
			 GetDlgItem("Lab_Score02")->Show(IsMaxPoint(points));
			 m_iPoint = points;
			 m_bPointReady = true;
	   }
	   
   }
   else if(strMsg == "getpassport")
   {
       GetPassportCashAdd_Re* p = (GetPassportCashAdd_Re*)wParam;
	   if(p->retcode == ERR_SUCCESS)
	   {
		   	int64_t cashadd = p->cashadd;
			int rcoin = GetRCoin(cashadd);
			ACString strText;
			strText.Format(_AL("%d"),rcoin);
			((PAUILABEL)GetDlgItem("Txt_08"))->SetText(strText);
			m_bCashReady = true;
	   }
	   
   }
   else if(strMsg == "pointexchange")
   {
       TouchPointExchange_Re* p= (TouchPointExchange_Re*)wParam;
	   if(p->retcode == ERR_SUCCESS)
	   {
		   int64_t points = p->new_points;
		   ACString strText;
		   strText.Format(_AL("%d"),points);
		   ((PAUILABEL)GetDlgItem("Txt_Score"))->SetText(strText);
		   GetDlgItem("Lab_Score02")->Show(IsMaxPoint(points));
			m_iPoint = points;
	   }
   }

   if(CanOpen())
	{
	   Show(true);
	   ResetOpenFlag();
	}
   return 0;
}
AUI_BEGIN_COMMAND_MAP(CDlgTouchExchange, CDlgBase)
AUI_ON_COMMAND("Btn_Confirm",		OnCommand_Confirm)
AUI_ON_COMMAND("IDCANCEL",		    OnCommand_Cancel)
AUI_ON_COMMAND("Btn_Cancel",		OnCommand_Cancel)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(CDlgTouchExchange,CDlgBase)
AUI_ON_EVENT("Btn_Prev",		WM_LBUTTONUP,	OnEventLButtonUp)
AUI_ON_EVENT("Btn_Next",		WM_LBUTTONUP,	OnEventLButtonUp)
AUI_ON_EVENT("Edt_Input",	    WM_LBUTTONDOWN, OnEventLButtonDown)
AUI_END_EVENT_MAP()

bool CDlgTouchExchange::OnInitDialog()
{
   m_pEdt = (PAUIEDITBOX)GetDlgItem("Edt_Input");
   return true;
}
void CDlgTouchExchange::OnShowDialog()
{
   m_dwTimeClickStart = 0;
   m_pEdt->SetText(_AL("1"));
   ((PAUILABEL)GetDlgItem("Title"))->SetText(GetGameUIMan()->GetStringFromTable(17900+GetData()));
   
}
void CDlgTouchExchange::OnCommand_Confirm(const char * szCommand)
{
	int nNum = a_atoi(m_pEdt->GetText());
	if(nNum <= 0 ) return;
	//弹出提示框
	ACString strMsg;
	strMsg.Format(GetGameUIMan()->GetStringFromTable(17902+GetData()),nNum*GetItemPrice(),nNum);
	PAUIDIALOG pDlg = NULL;
	GetGameUIMan()->MessageBox("TouchExchange",strMsg,MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160),&pDlg);
	pDlg->SetData(GetData());
	this->Show(false);
}
void CDlgTouchExchange::ProcessTouchExchange(bool isAgree,int type)
{
    if(isAgree)
	{
		int nNum = a_atoi(m_pEdt->GetText());
	    if(nNum <= 0 ) return;
		GetGameSession()->c2s_CmdGetTouchAward(type,nNum);
	}
}
void CDlgTouchExchange::OnCommand_Cancel(const char * szCommand)
{
   this->Show(false);
}
void CDlgTouchExchange::OnEventLButtonUp(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
   ChangeFocus(m_pEdt);
}
void CDlgTouchExchange::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if( a_atoi(pObj->GetText()) == 0 )
		pObj->SetText(_AL(""));
}
void CDlgTouchExchange::OnCommand_AddMinus(const char * szCommand)
{
	if( m_dwTimeClickStart == 0 )
		m_dwTimeClickStart = GetTickCount();
	else if( GetTickCount() - m_dwTimeClickStart < 500 )
		return;
	ACHAR szText[40];
	int nNum = a_atoi(m_pEdt->GetText());
	if(stricmp(szCommand,"add") == 0 && nNum < 9999)
	     nNum++;
	else if(stricmp(szCommand,"minus") == 0 && nNum > 1 )	 
         nNum--;
	a_sprintf(szText, _AL("%d"), nNum);
	m_pEdt->SetText(szText);
	
}
int  CDlgTouchExchange::GetItemPrice()
{
	return GetData()==1 ? TOUCH_GOLD_PRICE : TOUCH_SILVER_PRICE;
}
void CDlgTouchExchange::OnTick()
{
	if(GetHostPlayer()->GetSrcServerID() != 0)
	{
		Show(false);
		return;
	}
	if( GetFocus() != m_pEdt && a_atoi(m_pEdt->GetText()) == 0 )
			m_pEdt->SetText(_AL("0"));
	if( ((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Next"))->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK )
		OnCommand_AddMinus("add");
	else if( ((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Prev"))->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK )
		OnCommand_AddMinus("minus");
	else
		m_dwTimeClickStart = 0;


	int nNum = a_atoi(m_pEdt->GetText());
	PAUILABEL pLable = (PAUILABEL)GetDlgItem("Txt_Num");
	ACString strText;
	strText.Format(_AL("%d"),nNum*GetItemPrice());
	pLable->SetText(strText);

	CDlgTouchInfo* pTouchInfo = (CDlgTouchInfo*)(GetGameUIMan()->GetDialog("Win_Game_Touch"));
	GetDlgItem("Btn_Confirm")->Enable(nNum>0 && pTouchInfo->GetPoint() >= nNum*GetItemPrice());
}