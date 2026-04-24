// Filename	: DlgAwardAccountInfo.cpp
// Creator	: ice
// Date		: 2012/06/28
// Desc		: 账号信息完善奖励

#include "DlgAwardAccountInfo.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_IvtrItem.h"
#include "AUI\\AUICTranslate.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagepicture.h"
#include "afi.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameUIMisc.h"
#include "EC_GameUIMan.h"
#include "EC_TaskInterface.h"

#include "A2DSprite.h"
#include "ExpTypes.h"
#include "elementdataman.h"

#include "TaskTempl.h"

#define new A_DEBUG_NEW

extern CECGame* g_pGame;

AUI_BEGIN_COMMAND_MAP(CDlgAwardAccountInfo, CDlgBase)

AUI_ON_COMMAND("Btn_Link",		OnCommand_Link)
AUI_ON_COMMAND("Btn_Get1",		OnCommand_GetAward1)
AUI_ON_COMMAND("Btn_Get2",		OnCommand_GetAward2)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgAwardAccountInfo, CDlgBase)

//AUI_ON_EVENT("Item_MainType*",		WM_LBUTTONDOWN,		OnEventLButtonDownImage)

AUI_END_EVENT_MAP()

CDlgAwardAccountInfo::CDlgAwardAccountInfo()
{
	m_pBtnLink		= NULL;
}

CDlgAwardAccountInfo::~CDlgAwardAccountInfo()
{
}

bool CDlgAwardAccountInfo::OnInitDialog()
{
	if (!CDlgBase::OnInitDialog())
		return false;

	m_pBtnLink = (AUIStillImageButton*)GetDlgItem("Btn_Link");


	m_pTxt_ItemName[0] = (AUILabel*)GetDlgItem("Lab_Item1");
	m_pTxt_ItemName[1] = (AUILabel*)GetDlgItem("Lab_Item2");

	m_pImg_Item[0] = (AUIImagePicture*)GetDlgItem("Img_Item1");
	m_pImg_Item[1] = (AUIImagePicture*)GetDlgItem("Img_Item2");

	// 获取奖励物品图标
	for(int i=0;i<2;i++)
	{
		int idItem = 0;
		if(i==0)
			idItem = 52475;
		else if(i==1)
			idItem = 52474;

		CECIvtrItem *pItem = CECIvtrItem::CreateItem(idItem, 0, 1);
		if( !pItem )
			return false;

		AString strFile;
		af_GetFileTitle(pItem->GetIconFile(), strFile);
		strFile.MakeLower();
		m_pImg_Item[i]->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
			GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
		AUICTranslate trans;
		pItem->GetDetailDataFromLocal();
		const ACHAR *pszDesc = pItem->GetDesc();
		if( pszDesc )
			m_pImg_Item[i]->SetHint(trans.Translate(pszDesc));
		else
			m_pImg_Item[i]->SetHint(pItem->GetName());

		m_pTxt_ItemName[i]->SetText(pItem->GetName());

		delete pItem;
		pItem = NULL;
	}

	GetDlgItem("Btn_Get1")->Show(false);
	GetDlgItem("Btn_Get2")->Show(false);

	return true;
}
bool CDlgAwardAccountInfo::Tick(void)
{
	// 15100 "（已领取）"
	// 15101 "（未补填）"
	// 15102 "（已补填）"
	CECTaskInterface *pTaskInterafce = GetHostPlayer()->GetTaskInterface();
	if(pTaskInterafce)
	{
		FinishedTaskList* pTaskFinished = static_cast<FinishedTaskList*>(pTaskInterafce->GetFinishedTaskList());

		if(pTaskFinished->SearchTask(26560) == -1 )	//还没领奖
		{
			if((g_pGame->GetGameRun()->GetPlatformMask()&0x10) > 0) //  完善了身份证+姓名信息
			{
				GetDlgItem("Btn_Get1")->Show(true);
				GetDlgItem("Lab_1")->SetText(GetStringFromTable(15102));
			}
			else	// 显示“未补填”
			{
				GetDlgItem("Btn_Get1")->Show(false);
				GetDlgItem("Lab_1")->SetText(GetStringFromTable(15101));
			}
		}
		else	// 显示“已领取”
		{
			GetDlgItem("Btn_Get1")->Show(false);
			GetDlgItem("Lab_1")->SetText(GetStringFromTable(15100));
		}

		if(pTaskFinished->SearchTask(26561) == -1)	//还没领奖
		{
			if((g_pGame->GetGameRun()->GetPlatformMask()&0x20) > 0)	//  完善了账号+密码+邮箱信息
			{
				GetDlgItem("Btn_Get2")->Show(true);
				GetDlgItem("Lab_2")->SetText(GetStringFromTable(15102));
			}
			else	// 显示“未补填”
			{
				GetDlgItem("Btn_Get2")->Show(false);
				GetDlgItem("Lab_2")->SetText(GetStringFromTable(15101));
			}
		}
		else	// 显示“已领取”
		{
			GetDlgItem("Btn_Get2")->Show(false);
			GetDlgItem("Lab_2")->SetText(GetStringFromTable(15100));
		}
	}

	return true;
}
void CDlgAwardAccountInfo::OnCommand_Link(const char * szCommand)
{
	// 跟以前人物头像下的按钮处理方法相同
	GetGameSession()->open_url_by_token("accountcomplete");
}

void CDlgAwardAccountInfo::OnCommand_GetAward1(const char * szCommand)
{
	GetGameSession()->c2s_CmdGetTaskAward(1);
}

void CDlgAwardAccountInfo::OnCommand_GetAward2(const char * szCommand)
{
	GetGameSession()->c2s_CmdGetTaskAward(2);
}

bool CDlgAwardAccountInfo::Release()
{
	return CDlgBase::Release();
}

