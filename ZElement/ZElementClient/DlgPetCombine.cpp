// Filename	: DlgPetCombine.cpp
// Creator	: Fu Chonggang
// Date		: 2012-6-20

#include "AFI.h"
#include "AUI\\AUICTranslate.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUILabel.h"
#include "DlgPetCombine.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrConsume.h"
#include "DlgShopExchange.h"
#include "elementdataman.h"
#ifdef BACK_VERSION
#include "EC_Manager.h"
#else
#include "ECManager.h"
#include "ECManagerID.h"
#endif
#include "LuaEvent.h"

#define new A_DEBUG_NEW

/************************************************************************
CDlgPetCombine
************************************************************************/
AUI_BEGIN_COMMAND_MAP(CDlgPetCombine, CDlgItemSvrBase)

AUI_ON_COMMAND("Btn_Cancel",		OnCommandStopOpt)
AUI_ON_COMMAND("IDCANCEL",			OnCommandCancel)

AUI_END_COMMAND_MAP()

bool CDlgPetCombine::OnInitDialog()
{
	bool bRet = CDlgItemSvrBase::OnInitDialog();
	m_vecSuitIds.clear();
	if (m_szName=="Win_Pet_ChangeHoroscope")	// 修炼星相
	{
		m_vecSuitIds.push_back(31246);
		m_vecSuitIds.push_back(16302);
		m_vecSuitIds.push_back(62262);
	}
	else if (m_szName=="Win_Pet_ChangeImage") // 修炼外观
	{
		int i;
		for (i=22992; i<=23008; ++i)
		{
			m_vecSuitIds.push_back(i);
		}
		m_vecSuitIds.push_back(17782);
		m_vecSuitIds.push_back(17819);
		m_vecSuitIds.push_back(17820);
		m_vecSuitIds.push_back(18726);
		m_vecSuitIds.push_back(23338);
	}	
	else if (m_szName=="Win_Pet_ChangeLife")	// 补充寿命
	{
		m_vecSuitIds.push_back(16304);
		m_vecSuitIds.push_back(16315);
	}
	m_iMoney = GetGameRun()->GetSpecialIDConfig().fee_pet_refine;

	return bRet;
}

void CDlgPetCombine::OnTick()
{
	PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)GetDlgItem("Item_1");
	CECIvtrItem *pItem = (CECIvtrItem*)pImg->GetDataPtr("ptr_CECIvtrItem");
	if (pItem && pItem!=GetHostPlayer()->GetPack()->GetItem(pImg->GetData()))
	{
		pImg->SetData(0xffffffff);
		pImg->SetDataPtr(NULL, "ptr_CECIvtrItem");
		GetGameUIMan()->SetIvtrInfo(pImg, 0);
	}

	CDlgItemSvrBase::OnTick();
}

void CDlgPetCombine::SetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg)
{
	if (!pImg)
		pImg = (PAUIIMAGEPICTURE)GetDlgItem("Item_1");

	int i;
	for (i=0; i<(int)m_vecSuitIds.size(); ++i)
	{
		if (m_vecSuitIds[i]==pItem->GetTemplateID())
		{
			OnEventLButtonDown(0, 0, pImg);
			GetGameUIMan()->SetIvtrInfo(pImg, pItem);
			pItem->Freeze(true);
			pImg->SetDataPtr(pItem, "ptr_CECIvtrItem");
			pImg->SetData(nSlot);
			m_pBtn_Confirm->Enable(true);
		}
	}
}

void CDlgPetCombine::ServerStart()
{
	GetGameSession()->c2s_CmdNPCSevCombine(GetData("pet_index"), m_pImg_Item->GetData(), -1);
	LuaEvent::FirstChangePetShape();
}

void CDlgPetCombine::Reset()
{
	OnEventLButtonDown(0, 0, m_pImg_Item);
}

void CDlgPetCombine::OnCommandCancel(const char *szCommand)
{
	Show(false);
}

void CDlgPetCombine::OnCommandStopOpt(const char *szCommand)
{
	GetGameRun()->PostMessage(MSG_HST_PRESSCANCEL, MAN_PLAYER, 0);
}

void CDlgPetCombine::OnHideDialog()
{
	GetGameUIMan()->GetDialog("Win_Inventory")->Show(false);
	Reset();
}



/************************************************************************
CDlgPetCombineZiZhi
************************************************************************/

AUI_BEGIN_COMMAND_MAP(CDlgPetCombineZiZhi, CDlgBase)

AUI_ON_COMMAND("Rdo_*",				OnCommandSubType)
AUI_ON_COMMAND("Btn_Add",			OnCommandAdd)
AUI_ON_COMMAND("Btn_Minus",			OnCommandSub)
AUI_ON_COMMAND("Btn_Max",			OnCommandMax)
AUI_ON_COMMAND("start",				OnCommandStart)
AUI_ON_COMMAND("Btn_Cancel",		OnCommandStopOpt)
AUI_ON_COMMAND("IDCANCEL",			OnCommandCancel)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgPetCombineZiZhi, CDlgBase)

AUI_ON_EVENT("Img_Num*",		WM_LBUTTONDOWN,		OnEventLButtonDown)

AUI_END_EVENT_MAP()

const int	CDlgPetCombineZiZhi::m_iIdXianDou = 18794;
const int	CDlgPetCombineZiZhi::m_iIdXianDouNeed[12][3] = { 
	{1,2,5}, {1,2,5}, {1,2,5}, {1,2,5}, {1,2,5},
	{1,1,2}, {1,1,2}, {1,1,2}, {1,1,2}, {1,1,2}, {1,1,2}, {1,1,2} };
const int CDlgPetCombineZiZhi::m_iIds[12][3] = {
	{16030, 16031, 16032},
	{16050, 16051, 16052},
	{16070, 16071, 16072},
	{16090, 16091, 16092},
	{16110, 16111, 16112},
	{16130, 16131, 16132},
	{16150, 16151, 16152},
	{16170, 16171, 16172},
	{16190, 16191, 16192},
	{16250, 16251, 16252},
	{16210, 16211, 16212},
	{16230, 16231, 16232} };

bool CDlgPetCombineZiZhi::OnInitDialog()
{
	int iMoney = GetGameRun()->GetSpecialIDConfig().fee_pet_refine;
	
	ACString strText;
	PAUILABEL pLab = (PAUILABEL)GetDlgItem("Txt_Money1");
	strText.Format(_AL("%d"), iMoney/10000);
	pLab->SetText(strText);
	pLab = (PAUILABEL)GetDlgItem("Txt_Money2");
	strText.Format(_AL("%d"), (iMoney%10000)/100);
	pLab->SetText(strText);
	pLab = (PAUILABEL)GetDlgItem("Txt_Money3");
	strText.Format(_AL("%d"), iMoney%100);
	pLab->SetText(strText);

	// 表示选中的gfx
	AString strName;
	int i;
	for (i=0; i<12; i++)
	{
		strName.Format("Img_ItemBg%d", i+1);
		GetDlgItem(strName)->Show(false);
	}
	
	// 初始化
	CheckRadioButton(1, 1);
	m_iCurSel = -1;
	OnEventLButtonDown(0, 0, GetDlgItem("Img_Num1"));
	pLab = (PAUILABEL)GetDlgItem("Txt_Item2");
	pLab->SetText(_AL("1"));
	
	return true;
}

void CDlgPetCombineZiZhi::OnCommandSubType(const char * szCommand)
{
	m_bStopContinue = true;
	OnCommandMax("");
	PAUILABEL pLab = (PAUILABEL)GetDlgItem("Txt_Item2");
	ACString strText;
	strText.Format(_AL("%d"), m_iIdXianDouNeed[m_iCurSel][GetCheckedRadioButton(1)-1]);
	pLab->SetText(strText);
	
	PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)GetDlgItem("Item_1");
	pImg->FixFrame(GetCheckedRadioButton(1)-1);
	int i;
	AString strName;
	for (i=0; i<12; ++i)
	{
		strName.Format("Img_Num%d", i+1);
		((PAUIIMAGEPICTURE)GetDlgItem(strName))->FixFrame(GetCheckedRadioButton(1)-1);

	}
}

void CDlgPetCombineZiZhi::OnCommandAdd(const char * szCommand)
{
	PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("DEFAULT_Txt_Time");
	int iNum = a_atoi(pEdt->GetText());
	iNum++;
	ACString strText;
	strText.Format(_AL("%d"), iNum);
	pEdt->SetText(strText);
}

void CDlgPetCombineZiZhi::OnCommandSub(const char * szCommand)
{
	PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("DEFAULT_Txt_Time");
	int iNum = a_atoi(pEdt->GetText());
	if (iNum<=0)
		return;

	iNum--;
	ACString strText;
	strText.Format(_AL("%d"), iNum);
	pEdt->SetText(strText);
}

void CDlgPetCombineZiZhi::OnCommandMax(const char * szCommand)
{
	int iNum = MaxOptCount();
	ACString strText;
	strText.Format(_AL("%d"), iNum);
	PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("DEFAULT_Txt_Time");
	pEdt->SetText(strText);
}

void CDlgPetCombineZiZhi::OnShowDialog()
{
	int i;
	AString strName;
	CECPetBedge *pPet = (CECPetBedge*)GetHostPlayer()->GetPack()->GetItem(GetData("pet_index"));
	if (!pPet)
	{
		return;
	}
	int iIndexMain = pPet->GetEssence().main_type;
	for (i=0; i<12; ++i)
	{
		strName.Format("Lab_Num%d", i+1);
		PAUILABEL pLab = (PAUILABEL)GetDlgItem(strName);
		pLab->SetColor(iIndexMain==i ? A3DCOLORRGBA(255,0,0,255):A3DCOLORRGBA(128,255,255,255));
	}
	OnCommandMax("");
}

void CDlgPetCombineZiZhi::OnCommandStart(const char * szCommand)
{
	m_bStopContinue = true;
	PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("DEFAULT_Txt_Time");
	int iNum = a_atoi(pEdt->GetText());
	if (iNum<=0)
		return;

	CECPetBedge *pPet = (CECPetBedge*)GetHostPlayer()->GetPack()->GetItem(GetData("pet_index"));
	if (!pPet)
		return;
	if ( (pPet->GetEssence().star>=12 && GetCheckedRadioButton(1)==1) ||
		(pPet->GetEssence().star>=18 && GetCheckedRadioButton(1)==2) )
	{
		GetGameUIMan()->AddChatMessage(GetStringFromTable(15015), GP_CHAT_MISC);
		return;
	}
	
	m_bStopContinue = false;
	int iPropId = m_iIds[m_iCurSel][GetCheckedRadioButton(1)-1];
	if (iPropId==16031)		// 俗品血灵玉有一种绑定的，id=23881
	{
		if (GetHostPlayer()->GetPack()->GetItemTotalNum(23881)>0)
			iPropId = 23881;
	}
	if (GetHostPlayer()->GetPack()->GetItemTotalNum(iPropId)>0)
	{
		int iIndex = GetHostPlayer()->GetPack()->FindItem(iPropId);
		GetGameSession()->c2s_CmdNPCSevCombine(GetData("pet_index"), iIndex, -1);
		return;
	}

	if (GetHostPlayer()->GetPack()->GetItemTotalNum(m_iIdXianDou)>=m_iIdXianDouNeed[m_iCurSel][GetCheckedRadioButton(1)-1])
	{
		GetGameSession()->c2s_CmdNPCSevPetUpgradeWithXianDou(GetData("pet_index"), 3*m_iCurSel+GetCheckedRadioButton(1)-1);
	}
}

void CDlgPetCombineZiZhi::OnCommandCancel(const char *szCommand)
{
	m_bStopContinue = true;
	Show(false);
	GetGameUIMan()->GetDialog("Win_Inventory")->Show(false);
}

void CDlgPetCombineZiZhi::OnCommandStopOpt(const char *szCommand)
{
	GetGameRun()->PostMessage(MSG_HST_PRESSCANCEL, MAN_PLAYER, 0);
}

void CDlgPetCombineZiZhi::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	int iNewSel = atoi(pObj->GetName()+strlen("Img_Num"))-1;
	if (iNewSel==m_iCurSel)
		return;

	m_bStopContinue = true;
	AString strName;
	if (m_iCurSel>=0)
	{
		strName.Format("Img_ItemBg%d", m_iCurSel+1);
		GetDlgItem(strName)->Show(false);
	}
	m_iCurSel = iNewSel;
	strName.Format("Img_ItemBg%d", m_iCurSel+1);
	GetDlgItem(strName)->Show(true);

	PAUIIMAGEPICTURE pImgOrg = (PAUIIMAGEPICTURE)pObj;
	PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)GetDlgItem("Item_1");
	AUIOBJECT_SETPROPERTY p;
	pImgOrg->GetProperty("Image File", &p);
	pImg->SetProperty("Image File", &p);
	pImgOrg->GetProperty("Frames Number", &p);
	pImg->SetProperty("Frames Number", &p);
	pImg->FixFrame(GetCheckedRadioButton(1)-1);
	PAUILABEL pLab = (PAUILABEL)GetDlgItem("Txt_Item2");
	ACString strText;
	strText.Format(_AL("%d"), m_iIdXianDouNeed[m_iCurSel][GetCheckedRadioButton(1)-1]);
	pLab->SetText(strText);
}

int CDlgPetCombineZiZhi::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="continue_opt")
	{
		if (m_bStopContinue)
		{
			m_bStopContinue = false;
			return 0;
		}
		PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("DEFAULT_Txt_Time");
		int iNum = a_atoi(pEdt->GetText());
		iNum--;
		if (iNum<0)
			iNum=0;
		ACString strText;
		strText.Format(_AL("%d"), iNum);
		pEdt->SetText(strText);

		OnCommandStart("");
	}
	else if (strMsg=="stop_opt")
	{
		m_bStopContinue = true;
	}
	return 0;
}

int CDlgPetCombineZiZhi::MaxOptCount()
{
	int iPropId = m_iIds[m_iCurSel][GetCheckedRadioButton(1)-1];
	int iRet = GetHostPlayer()->GetPack()->GetItemTotalNum(iPropId);
	if (iPropId==16031)		// 俗品血灵玉有一种绑定的，id=23881
		iRet += GetHostPlayer()->GetPack()->GetItemTotalNum(23881);
	iRet += GetHostPlayer()->GetPack()->GetItemTotalNum(m_iIdXianDou)/m_iIdXianDouNeed[m_iCurSel][GetCheckedRadioButton(1)-1];
	return iRet;
}


/************************************************************************
CDlgPetCombineExchange
************************************************************************/
AUI_BEGIN_COMMAND_MAP(CDlgPetCombineExchange, CDlgItemSvrBase)

AUI_ON_COMMAND("confirm",			OnCommandExchangeConfirm)
AUI_ON_COMMAND("Btn_Set*",			OnCommandSet)
AUI_ON_COMMAND("Btn_Add",			OnCommandAdd)
AUI_ON_COMMAND("Btn_Minus",			OnCommandSub)
AUI_ON_COMMAND("Btn_Max",			OnCommandMax)
AUI_ON_COMMAND("Btn_Cancel",		OnCommandStopOpt)
AUI_ON_COMMAND("Btn_Close",			OnCommandCancel)
AUI_ON_COMMAND("IDCANCEL",			OnCommandCancel)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgPetCombineExchange, CDlgItemSvrBase)

AUI_ON_EVENT("Item_Sell*",		WM_LBUTTONDOWN,		OnEventLButtonDownExchange)
AUI_ON_EVENT("Item_Sell*",		WM_LBUTTONDBLCLK,	OnEventRButtonDownExchange)
AUI_ON_EVENT("Item_Sell*",		WM_RBUTTONDOWN,		OnEventRButtonDownExchange)

AUI_END_EVENT_MAP()

CDlgPetCombineExchange::CDlgPetCombineExchange() :
m_bCombining(false)
{
}

CDlgPetCombineExchange::~CDlgPetCombineExchange()
{
}

bool CDlgPetCombineExchange::OnInitDialog()
{
	bool bRet = CDlgItemSvrBase::OnInitDialog();
	int i;
	AString strName;
	for (i=0; i<12; ++i)
	{
		strName.Format("Item_Sell%d", i+1);
		m_pImgSell[i] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
	}
	for (i=0; i<5; ++i)
	{
		strName.Format("Btn_Set%d", i+1);
		m_pBtnSet[i] = (PAUISTILLIMAGEBUTTON)GetDlgItem(strName);
	}
	for( i = 0; i < 2; i++ )
	{
		strName.Format("Img_%d", i + 1);
		m_pImg_ItemReq[i] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		strName.Format("Txt_%d", i + 1);
		m_pTxt_ItemReq[i] = (PAUILABEL)GetDlgItem(strName);
		strName.Format("Lab_%d", i + 3);
		m_pTxt_CreditReq[i] = (PAUILABEL)GetDlgItem(strName);
	}
	m_pTxt_SpecialReq = (PAUILABEL)GetDlgItem("Txt_ItemReqBattle");

	memset(&(m_iIds[0][0]), 0, 12*5*sizeof(int));
	m_vecExtraIds.clear();
	if (m_szName=="Win_Pet_ChangeBone")	// 修炼舍利
	{
		for (i=0; i<10; ++i)
		{
			m_iIds[0][i] = 20048+i;
			m_iIds[1][i] = 20068+i;
		}
	}
	else if (m_szName=="Win_Pet_ChangeFight") // 修炼斗气
	{
		m_iIds[0][0] = 20481;
		m_iIds[0][1] = 20481;
		m_iIds[0][2] = 20484;
		m_iIds[0][3] = 23880;
	}	
	else if (m_szName=="Win_Pet_ChangeNature")	// 改变特质
	{
		m_iIds[0][0] = 16343;
	}
	else if (m_szName=="Win_Pet_ChangePrelife")	// 改变前世
	{
		m_iIds[0][0] = 19183;
	}
	else if (m_szName=="Win_Pet_ChangeSkill")	// 技能修炼
	{
		for (i=0; i<12; ++i)
		{
			m_iIds[0][i] = 23229+i;
			m_iIds[1][i] = 23241+i;
			m_iIds[2][i] = 23253+i;
			m_iIds[3][i] = 23268+i;
			m_iIds[4][i] = 23280+i;
		}

		m_iIds[2][9] = 23263; m_iIds[2][10] = 23264; m_iIds[2][11] = 23266; 
		for (i=19356; i<=19415; ++i)
		{
			m_vecExtraIds.push_back(i);
		}
		for (i=15845; i<=15864; ++i)
		{
			m_vecExtraIds.push_back(i);
		}
		for (i=60541; i<=60582; ++i)
		{
			m_vecExtraIds.push_back(i);
		}
		for (i=61424; i<=61429; ++i)
		{
			m_vecExtraIds.push_back(i);
		}
		for (i=61431; i<=61445; ++i)
		{
			m_vecExtraIds.push_back(i);
		}
		m_vecExtraIds.push_back(62254);
	}
	else if (m_szName=="Win_Pet_ExpandSkill")	// 技能扩充
	{
		m_iIds[0][0] = 22393;
	}
	else if (m_szName=="Win_Pet_ChangeBasicType")	// 改变限界
	{
		m_iIds[0][0] = 16320;
	}

	m_iMoney = GetGameRun()->GetSpecialIDConfig().fee_pet_refine;
	m_iCurSel = -1;
	return bRet;
}

void CDlgPetCombineExchange::OnShowDialog()
{
	OnCommandSet("Btn_Set1");
}

void CDlgPetCombineExchange::OnHideDialog()
{
	OnEventLButtonDown(0, 0, m_pImg_Item);
	if (GetDlgItem("Item_2"))
	{
		OnEventLButtonDown(0, 0, GetDlgItem("Item_2"));
	}
	Reset();
}

void CDlgPetCombineExchange::OnTick()
{
	UpdateItemInfo();
	
	if (!m_pImg_Item->GetDataPtr("ptr_CECIvtrItem")&&m_pBtn_Confirm->IsEnabled())
	{
		m_pBtn_Confirm->Enable(false);
		if(m_pTxt_Bronze)
			m_pTxt_Bronze->SetText(_AL("0"));
		if(m_pTxt_Silver)
			m_pTxt_Silver->SetText(_AL("0"));
		if(m_pTxt_Silver)
			m_pTxt_Gold->SetText(_AL("0"));
	}
	
	UpdateMoney();

	CECIvtrItem *pItem = (CECIvtrItem*)m_pImg_Item->GetDataPtr("ptr_CECIvtrItem");
	if (pItem && pItem!=GetHostPlayer()->GetPack()->GetItem(m_pImg_Item->GetData()))
	{
		m_pImg_Item->SetData(0xffffffff);
		m_pImg_Item->SetDataPtr(NULL, "ptr_CECIvtrItem");
		GetGameUIMan()->SetIvtrInfo(m_pImg_Item, 0);
	}	

	//  道具数量 连续修炼
	if (m_szName=="Win_Pet_ChangeFight" ||	// 修炼斗气 缺少自动放物品逻辑
		m_szName=="Win_Pet_ChangeSkill")	// 技能修炼
	{
		if (m_bCombining && m_pImg_Item->GetData()!=0xffffffff)
		{
			if (m_pImg_Item->GetDataPtr("ptr_CECIvtrItem")!=GetHostPlayer()->GetPack()->GetItem(m_pImg_Item->GetData()))
			{
				int iIndex = GetHostPlayer()->GetPack()->FindItem(m_iCombineId);
				if (iIndex>=0)
				{
					SetItem(GetHostPlayer()->GetPack()->GetItem(iIndex), iIndex, m_pImg_Item);
				}
				else
					m_bCombining = false;
			}
		}
		CECIvtrItem* pItem = (CECIvtrItem*)m_pImg_Item->GetDataPtr("ptr_CECIvtrItem");
		PAUILABEL pLabName = (PAUILABEL)GetDlgItem("Lab_Item");
		pLabName->SetText(pItem ? pItem->GetName():_AL(""));
		PAUILABEL pLabNum = (PAUILABEL)GetDlgItem("Txt_Item");
		ACString strText;
		strText.Format(_AL("%d"), pItem ? GetHostPlayer()->GetPack()->GetItemTotalNum(pItem->GetTemplateID()):0);
		pLabNum->SetText(strText);
		if (!pItem)
		{
			PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("DEFAULT_Txt_Time");
			pEdt->SetText(_AL(""));
		}
	}
	else if (m_szName=="Win_Pet_ChangeNature"	||	// 改变特质	道具数量显示
			m_szName=="Win_Pet_ChangePrelife"	||	// 改变前世
			m_szName=="Win_Pet_ChangeBasicType")	// 改变限界
	{
		CECIvtrItem* pItem = (CECIvtrItem*)m_pImg_Item->GetDataPtr("ptr_CECIvtrItem");
		PAUILABEL pLab = (PAUILABEL)GetDlgItem("Txt_Item");
		ACString strText;
		strText.Format(_AL("^ffffff%d^ffffff/1"), pItem ? GetHostPlayer()->GetPack()->GetItemTotalNum(m_iIds[0][0]):0);
		pLab->SetText(strText);
	}
	else if (m_szName=="Win_Pet_ExpandSkill")
	{
		m_pBtn_Confirm->Enable(GetDlgItem("Item_1")->GetDataPtr("ptr_CECIvtrItem") && GetDlgItem("Item_2")->GetDataPtr("ptr_CECIvtrItem"));
	}
}

void CDlgPetCombineExchange::SelectItem(int nSel)
{	
	if( m_iCurSel != -1 )
		m_pImgSell[m_iCurSel]->SetCover(NULL, 0, 1);
	m_iCurSel = nSel;
	if( m_iCurSel != -1 )
	{
		m_pImgSell[m_iCurSel]->SetCover(GetGameUIMan()->m_pA2DSpriteMask, 0, 1);
		GetDlgItem("Btn_Exchange")->Enable(true);
	}
	else
		GetDlgItem("Btn_Exchange")->Enable(false);
	UpdateItemInfo();
}

void CDlgPetCombineExchange::UpdateItemInfo(int nCount)
{
	int i;
	for(i = 0 ; i < 2; i++)
	{
		m_pImg_ItemReq[i]->Show(false);
		m_pImg_ItemReq[i]->ClearCover();
		m_pImg_ItemReq[i]->SetHint(_AL(""));
		m_pTxt_ItemReq[i]->SetText(_AL(""));
		m_pTxt_CreditReq[i]->SetText(_AL(""));
	}
	m_pTxt_SpecialReq->SetText(_AL(""));

	if( m_iCurSel == -1 )
		return;

	DATA_TYPE DataType;
	NPC_ESSENCE	*pCurNPCEssence = GetGameUIMan()->m_pCurNPCEssence;
	ITEM_TRADE_CONFIG *pData = NULL;

	CDlgShopExchange::SetShopIndex(0);
	if (pCurNPCEssence)
	{
		pData = (ITEM_TRADE_CONFIG *)GetGame()->GetElementDataMan()->
		get_data_ptr(pCurNPCEssence->id_item_trade_service[0], ID_SPACE_ESSENCE, DataType);

	}
	if (!pData || DataType != DT_ITEM_TRADE_CONFIG)
		return;
	
	int nMax = 9999;
	bool bEnable = true;
	if( nCount == 0 )
	{
		nCount = 1;
		bEnable = false;
	}
	
	int nCurSel = m_pImgSell[m_iCurSel]->GetData("shop_index")%48;
	CDlgShopExchange::m_nCurSel = nCurSel;
	int nShopSet = m_pImgSell[m_iCurSel]->GetData("shop_index")/48;
	CDlgShopExchange::m_nShopSet = nShopSet;
	for(i = 0 ; i < 2; i++)
	{
		ACHAR szText[100];
		if( pData->pages[nShopSet].goods[nCurSel].item_required[i].id && pData->pages[nShopSet].goods[nCurSel].item_required[i].count > 0 )
		{
			CECIvtrItem* pItem = CECIvtrItem::CreateItem(pData->pages[nShopSet].goods[nCurSel].item_required[i].id, 0, pData->pages[nShopSet].goods[nCurSel].item_required[i].count * nCount);
			if( pItem )
			{
				AString strFile;
				af_GetFileTitle(pItem->GetIconFile(), strFile);
				strFile.MakeLower();
				m_pImg_ItemReq[i]->Show(true);
				m_pImg_ItemReq[i]->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
					GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
				pItem->GetDetailDataFromLocal();

				const ACHAR* szDesc = pItem->GetDesc();
				AUICTranslate trans;
				if( szDesc )
					m_pImg_ItemReq[i]->SetHint(trans.Translate(szDesc));
				else
					m_pImg_ItemReq[i]->SetHint(pItem->GetName());
				if( pData->pages[nShopSet].goods[nCurSel].item_required[i].count * nCount > 1 )
				{
					a_sprintf(szText, _AL("%d"), pData->pages[nShopSet].goods[nCurSel].item_required[i].count * nCount);
					m_pImg_ItemReq[i]->SetText(szText);
				}
				else
					m_pImg_ItemReq[i]->SetText(_AL(""));
				m_pTxt_ItemReq[i]->SetText(pItem->GetName());
				delete pItem;
				int nNum = GetHostPlayer()->GetPack()->GetItemTotalNum(pData->pages[nShopSet].goods[nCurSel].item_required[i].id) / pData->pages[nShopSet].goods[nCurSel].item_required[i].count;
				if( nNum >= nCount )
					m_pTxt_ItemReq[i]->SetColor(A3DCOLORRGB(255, 255, 255));
				else
				{
					m_pTxt_ItemReq[i]->SetColor(A3DCOLORRGB(255, 0, 0));
					bEnable = false;
				}
				a_ClampRoof(nMax, nNum);
			}
		}
		if( pData->pages[nShopSet].goods[nCurSel].repu_required[i].id && pData->pages[nShopSet].goods[nCurSel].repu_required[i].count > 0 )
		{
			a_sprintf(szText, GetStringFromTable(6600 + pData->pages[nShopSet].goods[nCurSel].repu_required[i].id), 
				pData->pages[nShopSet].goods[nCurSel].repu_required[i].count * nCount);
			m_pTxt_CreditReq[i]->SetText(szText);
			int nNum = GetHostPlayer()->GetRegionReputation(pData->pages[nShopSet].goods[nCurSel].repu_required[i].id) / pData->pages[nShopSet].goods[nCurSel].repu_required[i].count;
			if( nNum >= nCount )
				m_pTxt_CreditReq[i]->SetColor(A3DCOLORRGB(255, 255, 255));
			else
			{
				m_pTxt_CreditReq[i]->SetColor(A3DCOLORRGB(255, 0, 0));
				bEnable = false;
			}
			a_ClampRoof(nMax, nNum);
		}
	}
	if( pData->pages[nShopSet].goods[nCurSel].special_required[0].type && pData->pages[nShopSet].goods[nCurSel].special_required[0].count > 0 )
	{
		ACHAR szText[100];
		a_sprintf(szText, GetStringFromTable(6599), 
			pData->pages[nShopSet].goods[nCurSel].special_required[0].count * nCount);
		m_pTxt_SpecialReq->SetText(szText);
		int nNum = GetHostPlayer()->GetBattleScore() / pData->pages[nShopSet].goods[nCurSel].special_required[0].count;
		if( nNum >= nCount )
			m_pTxt_SpecialReq->SetColor(A3DCOLORRGB(255, 255, 255));
		else
		{
			m_pTxt_SpecialReq->SetColor(A3DCOLORRGB(255, 0, 0));
			bEnable = false;
		}
		a_ClampRoof(nMax, nNum);
	}
	PAUIOBJECT pButton = GetDlgItem("Btn_Exchange");
	if( pButton )
		pButton->Enable(bEnable);
}

void CDlgPetCombineExchange::OnCommandExchangeConfirm(const char *szCommand)
{
	if( !GetDlgItem("Btn_Exchange")->IsEnabled() )
		return;
	
	CDlgShopExchange* pDlg = (CDlgShopExchange*)GetGameUIMan()->GetDialog("Win_ShopExchange_InputNO");
	pDlg->UpdateItemInfo(CDlgShopExchange::m_nCurSel);
	pDlg->Show(true, true);
	CECIvtrItem *pItem = (CECIvtrItem*)m_pImgSell[m_iCurSel]->GetDataPtr("ptr_CECIvtrItem");
	if( pItem )
	{
		AString strFile;
		af_GetFileTitle(pItem->GetIconFile(), strFile);
		strFile.MakeLower();
		pDlg->m_pImg_Item->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
			GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
		ACString strText = m_pImgSell[m_iCurSel]->GetText();
		pDlg->m_pImg_Item->SetText(strText);
		int iNum = 1;
		if (strText.GetLength()>0)
		{
			iNum = strText.ToInt();
		}
		pItem->GetDetailDataFromLocal();
		const ACHAR* szDesc = pItem->GetDesc();
		AUICTranslate trans;
		if( szDesc )
			pDlg->m_pImg_Item->SetHint(trans.Translate(szDesc));
		else
			pDlg->m_pImg_Item->SetHint(pItem->GetName());
		pDlg->m_pTxt_Name->SetText(pItem->GetName());
		pDlg->m_pImg_Item->SetDataPtr(pItem, "ptr_CECIvtrItem");
		pDlg->m_pTxt_Amount->SetText(_AL("1"));
		pDlg->SetData(pItem->GetPileLimit()/iNum);
	}
}

void CDlgPetCombineExchange::OnCommandSet(const char *szCommand)
{
	int iIndex = atoi(szCommand + strlen("Btn_Set"))-1;
	int i;
	for (i=0; i<5; ++i)
	{
		if (m_pBtnSet[i])
		{
			m_pBtnSet[i]->SetPushed(iIndex==i ? true:false);
		}
	}
	SelectItem(-1);

	CDlgShopExchange::SetShopIndex(0);
	DATA_TYPE DataType;
	ITEM_TRADE_CONFIG *pData = (ITEM_TRADE_CONFIG *)GetGame()->GetElementDataMan()->
		get_data_ptr(GetGameUIMan()->m_pCurNPCEssence->id_item_trade_service[0], ID_SPACE_ESSENCE, DataType);
	if (!pData || DataType != DT_ITEM_TRADE_CONFIG)
		return;

	
	int j, k;
	bool bFind = false;
	CECIvtrItem *pItem;
	for (k=0; k<12; ++k)	// 查找所有当前显示页位于商城4*48中的位置
	{
		pItem = (CECIvtrItem*)m_pImgSell[k]->GetDataPtr("ptr_CECIvtrItem");
		if( pItem )
		{
			delete pItem;
			m_pImgSell[k]->SetDataPtr(NULL, "ptr_CECIvtrItem");
		}
		bFind = false;
		if(m_iIds[iIndex][k]==0)
		{
			GetGameUIMan()->SetIvtrInfo(m_pImgSell[k], 0);
			m_pImgSell[k]->SetData(0xffffffff, "shop_index");
			continue;
		}
		for (i=0; i<4; ++i)				// 哪一页
		{
			for (j=0; j<48; ++j)		// 当前页第几个
			{
				if (pData->pages[i].goods[j].id_goods==0)
					continue;
				if ((int)pData->pages[i].goods[j].id_goods==m_iIds[iIndex][k])
				{		
					pItem = CECIvtrItem::CreateItem(pData->pages[i].goods[j].id_goods, 0, 1);
					if( pItem )
					{
						if (k<11 && bFind && m_iIds[iIndex][k]==m_iIds[iIndex][k+1])	// 可能一个物品有2中兑换方法
						{
							k++;
							CECIvtrItem *pItemOld = (CECIvtrItem*)m_pImgSell[k]->GetDataPtr("ptr_CECIvtrItem");
							if( pItemOld )
							{
								delete pItemOld;
								m_pImgSell[k]->SetDataPtr(NULL, "ptr_CECIvtrItem");
							}
						}
						pItem->GetDetailDataFromLocal();

						GetGameUIMan()->SetIvtrInfo(m_pImgSell[k], pItem, pData->pages[i].goods[j].goods_num);
						m_pImgSell[k]->SetData(i*48+j, "shop_index");
						m_pImgSell[k]->SetDataPtr(pItem, "ptr_CECIvtrItem");
						bFind = true;
					}
				}
			}
		}
	}
}



void CDlgPetCombineExchange::OnCommandAdd(const char * szCommand)
{
	CECIvtrItem *pItem = (CECIvtrItem*)m_pImg_Item->GetDataPtr("ptr_CECIvtrItem");
	if (!pItem)
	{
		return;
	}
	int iNumMax = GetHostPlayer()->GetPack()->GetItemTotalNum(pItem->GetTemplateID());
	PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("DEFAULT_Txt_Time");
	int iNum = a_atoi(pEdt->GetText());
	iNum++;
	if (iNum>iNumMax)
		iNum = iNumMax;

	ACString strText;
	strText.Format(_AL("%d"), iNum);
	pEdt->SetText(strText);
}

void CDlgPetCombineExchange::OnCommandSub(const char * szCommand)
{
	PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("DEFAULT_Txt_Time");
	int iNum = a_atoi(pEdt->GetText());
	if (iNum<=0)
		return;
	
	iNum--;
	ACString strText;
	strText.Format(_AL("%d"), iNum);
	pEdt->SetText(strText);
}

void CDlgPetCombineExchange::OnCommandMax(const char * szCommand)
{
	CECIvtrItem *pItem = (CECIvtrItem*)m_pImg_Item->GetDataPtr("ptr_CECIvtrItem");
	if (!pItem)
	{
		return;
	}
	int iNum = GetHostPlayer()->GetPack()->GetItemTotalNum(pItem->GetTemplateID());
	ACString strText;
	strText.Format(_AL("%d"), iNum);
	PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("DEFAULT_Txt_Time");
	pEdt->SetText(strText);
}

void CDlgPetCombineExchange::OnCommandCancel(const char *szCommand)
{
	Show(false);
}

void CDlgPetCombineExchange::OnCommandStopOpt(const char *szCommand)
{
	GetGameRun()->PostMessage(MSG_HST_PRESSCANCEL, MAN_PLAYER, 0);
}

void CDlgPetCombineExchange::OnEventLButtonDownExchange(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if( 0xffffffff==pObj->GetData("shop_index") ) return;
	
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	POINT ptCapture = { GET_X_LPARAM(lParam) - p->X, GET_Y_LPARAM(lParam) - p->Y };
	
	GetGameUIMan()->InvokeDragDrop(this, pObj, ptCapture);
	SelectItem(atoi(pObj->GetName() + strlen("Item_Sell")) - 1);
}

void CDlgPetCombineExchange::OnEventRButtonDownExchange(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if( 0xffffffff==pObj->GetData("shop_index") ) return;
	
	SelectItem(atoi(pObj->GetName() + strlen("Item_Sell")) - 1);
	OnCommandExchangeConfirm("");
	((CDlgShopExchange*)GetGameUIMan()->GetDialog("Win_ShopExchange_InputNO"))->OnCommand_max("");
}

int CDlgPetCombineExchange::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="continue_opt")
	{
		if (!m_bCombining || m_pImg_Item->GetData()==0xffffffff)
		{
			return 0;
		}
		
		PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("DEFAULT_Txt_Time");
		int iNum = a_atoi(pEdt->GetText());
		iNum--;
		if (iNum<0)
			iNum=0;
		ACString strText;
		strText.Format(_AL("%d"), iNum);
		pEdt->SetText(strText);
		ServerStart();
	}
	else if (strMsg=="stop_opt")
	{
		m_bCombining = false;
	}
	return 0;
}

void CDlgPetCombineExchange::SetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg)
{
	if (!pImg && pItem)
	{
		if(pItem->GetClassID()==CECIvtrItem::ICID_PET_REFINE)
			pImg = (PAUIIMAGEPICTURE)GetDlgItem("Item_1");
		else if(pItem->GetClassID()==CECIvtrItem::ICID_PET_ASSIST_REFINE)
			pImg = (PAUIIMAGEPICTURE)GetDlgItem("Item_2");
	}
	
	if (pImg==GetDlgItem("Item_1") && pItem->GetClassID()==CECIvtrItem::ICID_PET_REFINE)
	{
		int i, j;
		CDlgPetCombineExchange* pDlg = (CDlgPetCombineExchange*)GetGameUIMan()->GetDialog("Win_Pet_ChangeSkill");
		for (i=0; i<5; ++i)
		{
			for (j=0; j<12; ++j)
			{
				if (m_szName=="Win_Pet_ExpandSkill")
				{
					if (pDlg->m_iIds[i][j]!=0 && pDlg->m_iIds[i][j]==pItem->GetTemplateID())
					{
						DoSetItem(pItem, nSlot, pImg);
						m_pBtn_Confirm->Enable(true);
						if (m_szName=="Win_Pet_ChangeFight" || m_szName=="Win_Pet_ChangeSkill")
						{
							PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("DEFAULT_Txt_Time");
							ACString strText;
							strText.Format(_AL("%d"), GetHostPlayer()->GetPack()->GetItemTotalNum(pItem->GetTemplateID()));
							pEdt->SetText(strText);
							m_iCombineId = pItem->GetTemplateID();
							OnCommandMax("");
						}
						return;
					}
				}
				else if (m_iIds[i][j]!=0 && m_iIds[i][j]==pItem->GetTemplateID())
				{					
					DoSetItem(pItem, nSlot, pImg);
					m_pBtn_Confirm->Enable(true);
					if (m_szName=="Win_Pet_ChangeFight" || m_szName=="Win_Pet_ChangeSkill")
					{
						PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("DEFAULT_Txt_Time");
						ACString strText;
						strText.Format(_AL("%d"), GetHostPlayer()->GetPack()->GetItemTotalNum(pItem->GetTemplateID()));
						pEdt->SetText(strText);
						m_iCombineId = pItem->GetTemplateID();
						OnCommandMax("");
					}
					return;
				}
			}
		}
		
		for (i=0; i<(int)m_vecExtraIds.size(); ++i)
		{
			if (m_vecExtraIds[i]==pItem->GetTemplateID())
			{
				DoSetItem(pItem, nSlot, pImg);
				m_pBtn_Confirm->Enable(true);
				PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("DEFAULT_Txt_Time");
				ACString strText;
				strText.Format(_AL("%d"), GetHostPlayer()->GetPack()->GetItemTotalNum(pItem->GetTemplateID()));
				pEdt->SetText(strText);
				m_iCombineId = pItem->GetTemplateID();
				OnCommandMax("");
				return;
			}
		}
		if (m_szName=="Win_Pet_ExpandSkill")
		{
			for (i=0; i<(int)pDlg->m_vecExtraIds.size(); ++i)
			{		
				if (pDlg->m_vecExtraIds[i]==pItem->GetTemplateID())
				{
					DoSetItem(pItem, nSlot, pImg);
					m_pBtn_Confirm->Enable(true);
					if (m_szName=="Win_Pet_ChangeFight" || m_szName=="Win_Pet_ChangeSkill")
					{
						PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("DEFAULT_Txt_Time");
						ACString strText;
						strText.Format(_AL("%d"), GetHostPlayer()->GetPack()->GetItemTotalNum(pItem->GetTemplateID()));
						pEdt->SetText(strText);
						m_iCombineId = pItem->GetTemplateID();
						OnCommandMax("");
					}
					return;
				}
			}
		}

	}

	if (pImg==GetDlgItem("Item_2") && pItem->GetClassID()==CECIvtrItem::ICID_PET_ASSIST_REFINE)
	{
		int i, j;
		for (i=0; i<5; ++i)
		{
			for (j=0; j<12; ++j)
			{
				if (m_iIds[i][j]!=0 && m_iIds[i][j]==pItem->GetTemplateID())
				{
					DoSetItem(pItem, nSlot, pImg);
				}
			}
		}
	}
}

void CDlgPetCombineExchange::DoSetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg)
{
	OnEventLButtonDown(0, 0, pImg);
	GetGameUIMan()->SetIvtrInfo(pImg, pItem);
	pImg->SetDataPtr(pItem, "ptr_CECIvtrItem");
	pImg->SetData(nSlot);
	pItem->Freeze(true);
}

void CDlgPetCombineExchange::ServerStart()
{
	m_bCombining = false;
	PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("DEFAULT_Txt_Time");
	if (pEdt)
	{
		int iNum = a_atoi(pEdt->GetText());
		if (iNum<=0)
			return;
	}
	
	PAUIIMAGEPICTURE pImgAssist = (PAUIIMAGEPICTURE)GetDlgItem("Item_2");
	if(pImgAssist && pImgAssist->GetDataPtr("ptr_CECIvtrItem"))
	{
		GetGameSession()->c2s_CmdNPCSevCombine(GetData("pet_index"), m_pImg_Item->GetData(), pImgAssist->GetData());
		OnEventLButtonDown(0, 0, m_pImg_Item);
		OnEventLButtonDown(0, 0, pImgAssist);
	}
	else
	{
		GetGameSession()->c2s_CmdNPCSevCombine(GetData("pet_index"), m_pImg_Item->GetData(), -1);
	}
	if (m_szName=="Win_Pet_ChangeFight"||m_szName=="Win_Pet_ChangeSkill")
		m_bCombining = true;
	LuaEvent::FirstChangePetShape();
}

void CDlgPetCombineExchange::Reset()
{
	int i;
	for(i = 0; i < 12; i++)
	{
		CECIvtrItem *pItem = (CECIvtrItem*)m_pImgSell[i]->GetDataPtr("ptr_CECIvtrItem");
		if( pItem )
			delete pItem;
		m_pImgSell[i]->SetDataPtr(NULL);
	}
}
