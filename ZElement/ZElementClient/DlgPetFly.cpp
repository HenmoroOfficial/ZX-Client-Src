// Filename	: DlgPetFly.cpp
// Creator	: Fu Chonggang
// Date		: 2013.8.16

#include "AUI\\AUIImagePicture.h"
#include "DlgPetFly.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_IvtrConsume.h"
#include "EC_HostPlayer.h"

#define new A_DEBUG_NEW

int CDlgPetFly::ms_iFlyProp = 58852;

CDlgPetFly::CDlgPetFly()
{
}

CDlgPetFly::~CDlgPetFly()
{
}

void CDlgPetFly::OnTick()
{
	// 宠物飞升需要玩家飞升，宠物达到150级12星
	bool bCanStart = true;
	CECPetBedge *pPet = (CECPetBedge*)GetDlgItem("Item_a")->GetDataPtr("ptr_CECIvtrItem");
	CECIvtrItem *pTool = (CECIvtrItem*)GetDlgItem("Item_d")->GetDataPtr("ptr_CECIvtrItem");
	if (!pPet || !pTool)
		bCanStart = false;
	else
	{
		if (pPet->GetEssence().level<150)
			bCanStart = false;

		if (pPet->GetEssence().star<12)
			bCanStart = false;
	}

	if (GetHostPlayer()->GetRebornCount()==0)
		bCanStart = false;

	GetDlgItem("Btn_Start")->Enable(bCanStart);
}

void CDlgPetFly::SetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg)
{
	// 合法性校验
	if( pImg && 0 == stricmp(pImg->GetName(), "Item_a") && pItem->GetClassID() != CECIvtrItem::ICID_PET_BEDGE )
		return;

	if( pImg && 0 == stricmp(pImg->GetName(), "Item_d") && pItem->GetTemplateID() != ms_iFlyProp )
		return;

	// 对于右键点击选择合适的目标
	if ( !pImg )
	{
		if ( pItem->GetClassID() == CECIvtrItem::ICID_PET_BEDGE )
		{
			pImg = (PAUIIMAGEPICTURE)GetDlgItem("Item_a");
		}
		else if ( pItem->GetTemplateID() == ms_iFlyProp )
		{
			pImg = (PAUIIMAGEPICTURE)GetDlgItem("Item_d");
		}
	}

	if(!pImg)
		return;

	OnEventLButtonDown(0, 0, pImg);
	GetGameUIMan()->SetIvtrInfo(pImg, pItem);
	pItem->Freeze(true);
	pImg->SetData(nSlot, "index");
	pImg->SetDataPtr(pItem, "ptr_CECIvtrItem");

}

void CDlgPetFly::ServerStart()
{
	CECPetBedge *pPetItem = (CECPetBedge*)GetDlgItem("Item_a")->GetDataPtr("ptr_CECIvtrItem");
	if (0==pPetItem->GetEssence().identify || pPetItem->GetOwnerName()!=ACString(GetHostPlayer()->GetName()))
	{
		GetGameUIMan()->AddChatMessage(GetStringFromTable(15014), GP_CHAT_MISC);
		return;
	}

	// 宠物飞升完成需要展示，所以暂时将宠物freeze，等Win_PetLvUp_Img关闭再解锁
	GetGameSession()->c2s_CmdNPCSevPetFly((int)GetDlgItem("Item_a")->GetData("index"), (int)GetDlgItem("Item_d")->GetData("index"));
	int iIndex = GetDlgItem("Item_a")->GetData("index");
	// Freeze(false)
	OnCommandCancel("");
	// Freeze(true)
	GetGameUIMan()->CommonMessage("Win_PetLvUp_Img", "set_pet", iIndex, 0);
}

void CDlgPetFly::Reset()
{
	OnEventLButtonDown(0, 0, GetDlgItem("Item_a"));
	OnEventLButtonDown(0, 0, GetDlgItem("Item_d"));
}
