// Filename	: DlgVipAward.cpp
// Creator	: Fu Chonggang
// Date		: 2012/02/01

#include "AFI.h"
#include "AIniFile.h"
#include "A2DSprite.h"
#include "AUI\\CSplit.h"
#include "AUI\\AUIDef.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIScroll.h"
#include "AUI\\AUIProgress.h"
#include "AUI\\AUISubDialog.h"
#include "DlgVipAward.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_ModelRender.h"
#include "EC_LoginPlayer.h"
#include "EC_HostPlayer.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrEquip.h"
#include "EC_GameUIMisc.h"
#include "EC_Calendar.h"
#include "globaldataman.h"
#include "EC_GPDataType.h"


#define new A_DEBUG_NEW

/************************************************************************/
/*			           CDlgVipAwardItem                                 */
/************************************************************************/

A2DSprite* CDlgVipAwardItem::m_pA2DSpriteDaily = 0;
A2DSprite* CDlgVipAwardItem::m_pA2DSpriteOnce = 0;

AUI_BEGIN_COMMAND_MAP(CDlgVipAwardItem, CDlgBase)

AUI_ON_COMMAND("Btn_GetAward",		OnCommand_GetAward)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgVipAwardItem, CDlgBase)

AUI_ON_EVENT("Pic_Goods",	WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("Pic_Goods",	WM_LBUTTONDBLCLK,	OnEventLButtonDown)

AUI_END_EVENT_MAP()


bool CDlgVipAwardItem::OnInitDialog()
{
	if (!m_pA2DSpriteDaily)
	{
		m_pA2DSpriteDaily = new A2DSprite;
		if( !m_pA2DSpriteDaily ) return AUI_ReportError(__LINE__, __FILE__);
		
		bool bval = m_pA2DSpriteDaily->Init(m_pA3DDevice, "Icon\\PShopVipDaily.tga", 0);
		if( !bval ) return AUI_ReportError(__LINE__, __FILE__);
	}

	if (!m_pA2DSpriteOnce)
	{
		m_pA2DSpriteOnce = new A2DSprite;
		if( !m_pA2DSpriteOnce ) return AUI_ReportError(__LINE__, __FILE__);
		
		bool bval = m_pA2DSpriteOnce->Init(m_pA3DDevice, "Icon\\PShopVipOnce.tga", 0);
		if( !bval ) return AUI_ReportError(__LINE__, __FILE__);
	}

	return true;
}

bool CDlgVipAwardItem::Release()
{
	A3DRELEASE(m_pA2DSpriteDaily);
	A3DRELEASE(m_pA2DSpriteOnce);
	return CDlgBase::Release();
}

void CDlgVipAwardItem::OnCommand_GetAward(const char * szCommand)
{
	GetGame()->GetGameSession()->c2s_CmdObtainVipAwardByID(m_pItem->tid, m_pItem->item_id);
}

void CDlgVipAwardItem::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if (!m_pItem)
		return;

	CECIvtrItem *pItem = CECIvtrItem::CreateItem(m_pItem->item_id, 0, 1);
	if( pItem )
	{
		if( pItem->IsEquipment() )
		{
			CECIvtrEquip *pEquip = (CECIvtrEquip*)pItem;
			CDlgVipAward *pDlgVipAward = (CDlgVipAward*)GetGameUIMan()->GetDialog("Win_QShop_vip");
			if( pEquip->GetDBEssence() )
				pDlgVipAward->ChangeEquip(pEquip->GetDBEssence());
		}
		delete pItem;
	}
}

int CDlgVipAwardItem::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="set_info")
	{
		m_pItem = (VIP_AWARD_ITEM*)wParam;
		m_iCanGetInfo = lParam;
		UpdateDisplay();
	}

	return 0;
}

void CDlgVipAwardItem::UpdateDisplay()
{
	ACString strText;
	PAUILABEL pLabNum = (PAUILABEL)GetDlgItem("Txt_Number");
	strText.Format(GetStringFromTable(1507), m_pItem->count);
	pLabNum->SetText(strText);
	PAUILABEL pLabExprireTime = (PAUILABEL)GetDlgItem("Txt_Time");
	strText.Format(GetStringFromTable(1504), GetTimeText1(m_pItem->expire_time));
	pLabExprireTime->SetText(strText);
	
	CECIvtrItem *pItem = CECIvtrItem::CreateItem(m_pItem->item_id, 0, m_pItem->count);
	if (pItem)
	{
		PAUILABEL pLabName = (PAUILABEL)GetDlgItem("Txt_ItemName");
		pLabName->SetText(pItem->GetName());
		PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)GetDlgItem("Pic_Goods");
		pItem->GetDetailDataFromLocal();
		GetGameUIMan()->SetIvtrInfo(pImg, pItem);
		delete pItem;
	}

	GetDlgItem("Btn_GetAward")->Enable(m_iCanGetInfo==0);
	GetDlgItem("Btn_GetAward")->SetText(m_iCanGetInfo==1 ? GetGameUIMan()->GetStringFromTable(12991) : GetGameUIMan()->GetStringFromTable(12990));

	PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)GetDlgItem("Img_AwardType");
	if (m_pItem->award_obtain_type==0)
		pImg->SetCover(m_pA2DSpriteDaily, 0, 0);
	else
		pImg->SetCover(m_pA2DSpriteOnce, 0, 0);
}


/************************************************************************/
/*			               CDlgVipAward                                 */
/************************************************************************/

AUI_BEGIN_COMMAND_MAP(CDlgVipAward, CDlgBase)

AUI_ON_COMMAND("resumeequip",		OnCommand_ResumeEquip)
AUI_ON_COMMAND("maintype*",			OnCommand_MainType)
AUI_ON_COMMAND("subtype*",			OnCommand_SubType)
AUI_ON_COMMAND("Btn_Help",			OnCommand_Help)
AUI_ON_COMMAND("Btn_LvUpHint",		OnCommand_HelpLvUp)

AUI_END_COMMAND_MAP()

const int	CDlgVipAward::ms_C2SCoolTime = 15000;

CDlgVipAward::CDlgVipAward() : m_pPlayer(0)
{
	memset(m_aEquips, 0, sizeof(m_aEquips));
}

CDlgVipAward::~CDlgVipAward()
{
	A3DRELEASE(m_pPlayer);
}

bool CDlgVipAward::OnInitDialog()
{
	m_pLv		= (PAUILABEL)GetDlgItem("Txt_Lv");
	m_pVipLv	= (PAUILABEL)GetDlgItem("Txt_VipLv");
	m_pEndTime	= (PAUILABEL)GetDlgItem("Txt_Time");
	m_pLabDateLeave	= (PAUILABEL)GetDlgItem("Txt_Time2");
	
	m_pImg_Char = (PAUIIMAGEPICTURE)GetDlgItem("Pic_Part");
	m_pBtn_LeftTurn = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_LeftTurn");
	m_pBtn_RightTurn = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_RightTurn");
	m_nAngle = 0;

	int i;
	AString strName;
	for (i=0; i<VIP_AWARD_PERPAGE; ++i)
	{
		strName.Format("Sub_QshopVipItemDefault%d", i);
		m_pSubItems[i] = (PAUISUBDIALOG)GetDlgItem(strName);
		strName.Format("Shape_QshopVipItemDefault%d", i);
		m_pEdtShape[i] = (PAUIEDITBOX)GetDlgItem(strName);
	}
	
	for (i=0; i<VIP_SUBBUTTON_MAX; ++i)
	{
		strName.Format("Btn_SubType%d", i);
		m_pBtn_SubType[i] = (PAUISTILLIMAGEBUTTON)GetDlgItem(strName);
		strName.Format("Gfx_Sub_%d", i);
		m_pImgGfxSub[i] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
	}

	m_pImgGfxMain[0] = (PAUIIMAGEPICTURE)GetDlgItem("Gfx_Main_1");
	m_pImgGfxMain[1] = (PAUIIMAGEPICTURE)GetDlgItem("Gfx_Main_2");

	ReLoadModel(m_pPlayer);
	memset(m_aEquips, 0, sizeof(m_aEquips));

	m_nAwardType = -1;
	m_nAwardLevel = -1;

	m_iExpireTime = 0;
	m_iLastC2STime = 0;

	return true;
}

void CDlgVipAward::OnShowDialog()
{
	if (m_pPlayer)
	{
		CECHostPlayer* pHost = GetHostPlayer();
		pHost->SyncDemonstrationPlayer(m_pPlayer, 1);
		memcpy(m_aEquips, pHost->GetEquips(), sizeof(m_aEquips));
		m_pPlayer->ShowEquipments(m_aEquips, false, true);
		m_pPlayer->SetFashionMode(true);
	}

	if (m_nAwardType<0)
	{
		OnCommand_MainType("maintype0");
	}

	if (GetTickCount()-m_iLastC2STime > ms_C2SCoolTime)
	{
/* 如果没有vip信息服务器不回，则需要这几行代码
		m_iExpireTime = 0;
		m_mapAwardId.clear();
*/		GetGameSession()->c2s_CmdGetVipAwardInfo();
		m_iLastC2STime = GetTickCount();
	}
}

void CDlgVipAward::OnCommand_MainType(const char * szCommand)
{
	int nType = atoi(szCommand + strlen("maintype"));
	if (nType==m_nAwardType)
		return;

	int i;
	m_nAwardType = nType;
	if (m_nAwardType==0)
	{
		((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_MainType0"))->SetPushed(true);
		((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_MainType1"))->SetPushed(false);
		for(i=0; i<VIP_SUBBUTTON_MAX; ++i)
		{
			m_pBtn_SubType[i]->SetText(GetStringFromTable(13011+i));
		}
		m_pBtn_SubType[VIP_SUBBUTTON_MAX-1]->Show(true);
		m_pBtn_SubType[VIP_SUBBUTTON_MAX-2]->Show(true);

		int iLv = GetHostPlayer()->GetBasicProps().iLevel;
		if (GetHostPlayer()->GetRebornInfo().size()==0)
		{
			if (iLv>=135)
				OnCommand_SubType("subtype3");
			else if (iLv>=120)
				OnCommand_SubType("subtype2");
			else if (iLv>=90)
				OnCommand_SubType("subtype1");
			else
				OnCommand_SubType("subtype0");
		}
		else
		{
			if (iLv>=135)
				OnCommand_SubType("subtype7");
			else if (iLv>=120)
				OnCommand_SubType("subtype6");
			else if (iLv>=90)
				OnCommand_SubType("subtype5");
			else
				OnCommand_SubType("subtype4");
		}
	}
	else
	{
		((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_MainType0"))->SetPushed(false);
		((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_MainType1"))->SetPushed(true);
		for(i=0; i<VIP_SUBBUTTON_MAX; ++i)
		{
			m_pBtn_SubType[i]->SetText(GetStringFromTable(13020+i));
		}
		m_pBtn_SubType[VIP_SUBBUTTON_MAX-1]->Show(false);
		m_pBtn_SubType[VIP_SUBBUTTON_MAX-2]->Show(false);
		OnCommand_SubType("subtype0");
	}
}

void CDlgVipAward::OnCommand_SubType(const char * szCommand)
{
	m_nAwardLevel = atoi(szCommand + strlen("subtype"));

	int i;
	for (i=0; i<VIP_SUBBUTTON_MAX; ++i)
	{
		if (i==m_nAwardLevel)
			m_pBtn_SubType[i]->SetPushed(true);
		else
			m_pBtn_SubType[i]->SetPushed(false);
	}

	m_vecItemIndex.clear();

	vector<VIP_AWARD_ITEM>& items = *globaldata_get_vipaward_items();
	for (i=0; i<(int)items.size(); ++i)
	{
		VIP_AWARD_ITEM& item = items[i];
		if ((int)item.award_type==m_nAwardType && (int)item.award_level==(m_nAwardLevel+1))	// 注意：+1
		{
			m_vecItemIndex.push_back(i);
		}
	}

	UpdateDisplay();
}

void CDlgVipAward::OnCommand_Help(const char * szCommand)
{
	//PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Qshop_Vip_Help");
	//PAUILABEL pLab = (PAUILABEL)pDlg->GetDlgItem("Lab_1");
	//pLab->SetText(GetGameUIMan()->GetStringFromTable(12992));
	//pDlg->Show(!pDlg->IsShow());
	//pDlg->AlignTo(this,
	//	AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT,
	//	AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_BOTTOM);
	AString strDlg = "Win_Qshop_Vip_Help";
	GetGameUIMan()->CommonMessage("Win_HelpInfo","open",(WPARAM)(&strDlg),0);
}

void CDlgVipAward::OnCommand_HelpLvUp(const char * szCommand)
{
	//PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Qshop_Vip_LvupHelp");
	//pDlg->Show(!pDlg->IsShow());
	AString strDlg = "Win_Qshop_Vip_LvupHelp";
	GetGameUIMan()->CommonMessage("Win_HelpInfo","open",(WPARAM)(&strDlg),0);
}

void CDlgVipAward::OnCommand_ResumeEquip(const char * szCommand)
{
	if (m_pPlayer)
	{
		CECHostPlayer* pHost = GetHostPlayer();
		memcpy(m_aEquips, pHost->GetEquips(), sizeof(m_aEquips));
		m_pPlayer->ShowEquipments(m_aEquips, false, true);
		m_pPlayer->SetFashionMode(true);
	}
}

void CDlgVipAward::OnTick()
{
	ACString strText;
	strText.Format(_AL("%d"), GetHostPlayer()->GetBasicProps().iLevel);
	m_pLv->SetText(strText);
	strText.Format(_AL("%d"), GetHostPlayer()->GetVipLv());
	m_pVipLv->SetText(GetHostPlayer()->GetVipLv()>0 ? strText:_AL("----"));
	time_t endTime = m_iExpireTime;
	tm *timeTM = gmtime(&endTime);
	strText.Format(_AL("%d.%d.%d"), 1900+timeTM->tm_year, timeTM->tm_mon+1, timeTM->tm_mday);
	m_pEndTime->SetText(m_iExpireTime>0 ? strText : _AL("----"));
	if (m_iExpireTime>0)
	{
		CECCalendar	tEnd(timeTM->tm_year, timeTM->tm_mon, timeTM->tm_mday);
		tm tmNow = GetGame()->GetServerLocalTime();
		CECCalendar tNow(tmNow.tm_year, tmNow.tm_mon, tmNow.tm_mday);
		strText.Format(_AL("%d"), tEnd-tNow);
		if (tEnd>=tNow)
			m_pLabDateLeave->SetText(strText);
		else
			m_pLabDateLeave->SetText(_AL("----"));
	}
	else
		m_pLabDateLeave->SetText(_AL("----"));

	if( m_pBtn_LeftTurn->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK )
		m_nAngle = (m_nAngle + 360 - GetGame()->GetTickTime() / 6 ) % 360;
	else if( m_pBtn_RightTurn->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK )
		m_nAngle = (m_nAngle + GetGame()->GetTickTime() / 6) % 360;

	if (m_pPlayer)
	{
		m_pPlayer->Stand();
		m_pPlayer->Tick(GetGame()->GetTickTime());
			
		m_pImg_Char->SetRenderCallback(
			PlayerRenderDemonstration,
			(DWORD)m_pPlayer,
			m_nAngle);
	}
}

int CDlgVipAward::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	// 收到协议更新个人vip信息
	if (strMsg=="award_info")
	{
		S2C::player_vip_award_info *pInfo = (S2C::player_vip_award_info*)wParam;
		m_iExpireTime = pInfo->award_expire_time;
		m_mapAwardId.clear();
		int i=0;
		bool bToGet = false;
		for (; i<pInfo->award_count; ++i)
		{
			m_mapAwardId[pInfo->award_id_list[i].award_id] = pInfo->award_id_list[i].is_obtained;
			if (pInfo->award_id_list[i].is_obtained==0)
			{
				bToGet = true;
			}
		}
        if(NULL != GetGameUIMan()->GetDialog("Win_MiniMap")->GetDlgItem("Gfx_1"))
		    GetGameUIMan()->GetDialog("Win_MiniMap")->GetDlgItem("Gfx_1")->Show(bToGet);
		if(NULL != GetGameUIMan()->GetDialog("Win_Minimap_Recommend")->GetDlgItem("Gfx_1"))
			GetGameUIMan()->GetDialog("Win_Minimap_Recommend")->GetDlgItem("Gfx_1")->Show(bToGet);

		ACString strText;
		PAUILABEL pLab = (PAUILABEL)GetDlgItem("Txt_MoneyNum");
		strText.Format(_AL("%d"), pInfo->recent_total_cash);
		pLab->SetText(strText);

		PAUIPROGRESS pPrg = (PAUIPROGRESS)GetDlgItem("Prgs_LvUp");
		PAUIPROGRESS pPrg1 = (PAUIPROGRESS)GetGameUIMan()->GetDialog("Win_Qshop_Vip_LvupHelp")->GetDlgItem("Prgs_LvUp");
		PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)GetDlgItem("Img_VipLvNow");
		PAUIIMAGEPICTURE pImg1 = (PAUIIMAGEPICTURE)GetGameUIMan()->GetDialog("Win_Qshop_Vip_LvupHelp")->GetDlgItem("Img_VipLvNow");
		pImg->Show(GetHostPlayer()->GetVipLv()>0);
		pImg->FixFrame(GetHostPlayer()->GetVipLv()-1);
		pImg1->Show(GetHostPlayer()->GetVipLv()>0);
		pImg1->FixFrame(GetHostPlayer()->GetVipLv()-1);
		pImg = (PAUIIMAGEPICTURE)GetDlgItem("Img_VipLvNext");
		pImg1 = (PAUIIMAGEPICTURE)GetGameUIMan()->GetDialog("Win_Qshop_Vip_LvupHelp")->GetDlgItem("Img_VipLvNext");
		
		int iLvUpCount[10] = {100, 400, 800, 1500, 2500, 5000, 6000, 7000, 8000, 9000};
		int iLvMax = 6;
		AIniFile IniFile;
		if (IniFile.Open("Configs\\element_client.cfg"))
		{
			AString strVipCash = IniFile.GetValueAsString("Settings", "vipcash", "");
			CSplit sp(strVipCash);
			CSplit::VectorAString vec = sp.Split(",");
			if (vec.size()>0)
			{
				iLvMax = vec.size();
				for (int i=0; i<iLvMax; ++i)
				{
					iLvUpCount[i] = vec[i].ToInt();
				}
			}
		}
		if (GetHostPlayer()->GetVipLv()==iLvMax)
		{
			strText.Format(_AL("%d"), 0);
			pPrg->SetProgress(AUIPROGRESS_MAX);
			pImg->Show(false);
			pPrg1->SetProgress(AUIPROGRESS_MAX);
			pImg1->Show(false);
		}
		else
		{
			strText.Format(_AL("%d"), iLvUpCount[GetHostPlayer()->GetVipLv()]-pInfo->recent_total_cash);
			pPrg->SetProgress(AUIPROGRESS_MAX*pInfo->recent_total_cash/iLvUpCount[GetHostPlayer()->GetVipLv()]);
			pImg->Show(true);
			pImg->FixFrame(GetHostPlayer()->GetVipLv());
			pPrg1->SetProgress(AUIPROGRESS_MAX*pInfo->recent_total_cash/iLvUpCount[GetHostPlayer()->GetVipLv()]);
			pImg1->Show(true);
			pImg1->FixFrame(GetHostPlayer()->GetVipLv());
		}
		pLab = (PAUILABEL)GetDlgItem("Txt_Prgs_LvUp");
		pLab->SetText(strText);
		pLab = (PAUILABEL)GetGameUIMan()->GetDialog("Win_Qshop_Vip_LvupHelp")->GetDlgItem("Txt_Prgs_LvUp");
		pLab->SetText(strText);
		UpdateDisplay();
	}
	return 0;
}

void CDlgVipAward::ChangeEquip(const EQUIPMENT_ESSENCE* pEquip)
{
	if (!m_pPlayer)
		return;
	
	if (pEquip->require_gender != 2 && (int)pEquip->require_gender != GetHostPlayer()->GetGender())
		return;
	
	if (pEquip->equip_type == 3 || pEquip->equip_type == 4) // 时装或翅膀
	{
		int i;
		for (i = 0; i < SIZE_EQUIPIVTR; i++)
			if ((1 << i) & pEquip->equip_mask)
				break;
			
		if (i == SIZE_EQUIPIVTR)
			return;
			
		m_aEquips[i] = pEquip->id;
		m_pPlayer->ShowEquipments(m_aEquips, false, true);
		m_pPlayer->SetFashionMode(true);
	}
}

void CDlgVipAward::UpdateDisplay()
{
	unsigned int i;
	vector<VIP_AWARD_ITEM>& items = *globaldata_get_vipaward_items();
	
	for(i = 0; i < m_vecItemIndex.size(); i++)
	{
		int index = m_vecItemIndex[i];
		CDlgVipAwardItem* pDlg = dynamic_cast<CDlgVipAwardItem*>(m_pSubItems[i]->GetSubDialog());
		pDlg->CommonMessage("set_info", (unsigned int)(&(items[index])), 
			m_mapAwardId.find(items[index].tid)==m_mapAwardId.end() ? -1 : m_mapAwardId[items[index].tid]);
		m_pSubItems[i]->Show(true);
		m_pEdtShape[i]->Show(true);
	}
	for(; i < VIP_AWARD_PERPAGE; i++)
	{
		m_pSubItems[i]->Show(false);
		m_pEdtShape[i]->Show(false);
	}

	//更新闪烁信息
	m_pImgGfxMain[0]->Show(false);
	m_pImgGfxMain[1]->Show(false);
	for (i=0; i<VIP_SUBBUTTON_MAX; ++i)
	{
		m_pImgGfxSub[i]->Show(false);
	}
	for (i=0; i<(int)items.size(); ++i)
	{
		VIP_AWARD_ITEM& item = items[i];
		if (m_mapAwardId.find(item.tid)!=m_mapAwardId.end() && m_mapAwardId[item.tid]==0)
		{
			if(item.award_type<2)
				m_pImgGfxMain[item.award_type]->Show(true);
			if ((int)item.award_type==m_nAwardType && item.award_level<=VIP_SUBBUTTON_MAX)
				m_pImgGfxSub[item.award_level-1]->Show(true);
		}
	}

}