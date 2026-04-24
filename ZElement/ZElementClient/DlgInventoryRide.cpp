// Filename	: DlgInventoryRide.cpp
// Creator	: Fu Chonggang
// Date		: 2012.7.31

#include "DlgInventoryRide.h"
#include "DlgTalisman.h"
#include "AFI.h"
#include "AUI\\AUIDef.h"
#include "AUI\\AUICTranslate.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIScroll.h"
#include "EC_GameUIMan.h"
#include "EC_LoginPlayer.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_World.h"
#include "EC_ManPlayer.h"
#include "EC_HostPlayer.h"
#include "EC_IvtrEquip.h"
#include "EC_IvtrTalismanMain.h"
#include "EC_Inventory.h"
#include "EC_ModelRender.h"
#include "EC_GameSession.h"
#include "EC_Model.h"
#include "EC_Configs.h"
#include "EC_Helper.h"
#include "EC_AutoHelper.h"
#include "LuaEvent.h"
#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgInventoryRide, CDlgFormatText)

AUI_ON_COMMAND("Btn_Cancel",		OnCommand_Resume)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgInventoryRide, CDlgFormatText)

AUI_ON_EVENT("Img_Item*",			WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("Img_Item*",			WM_RBUTTONDOWN,		OnEventRButtonDown)
AUI_ON_EVENT("Img_Item*",			WM_LBUTTONDBLCLK,	OnEventRButtonDown)

AUI_ON_EVENT("Img_Vechile01",			WM_LBUTTONDOWN,		OnMountEventLButtonDownItem)
AUI_ON_EVENT("Img_Vechile01",			WM_LBUTTONDBLCLK,		OnMountEventRButtonDownItem)
AUI_ON_EVENT("Img_Vechile01",				WM_RBUTTONDOWN,	OnMountEventRButtonDownItem)

AUI_ON_EVENT("Img_Vechile02",			WM_LBUTTONDOWN,		OnFlyEventLButtonDownItem)
AUI_ON_EVENT("Img_Vechile02",			WM_LBUTTONDBLCLK,		OnFlyEventRButtonDownItem)
AUI_ON_EVENT("Img_Vechile02",				WM_RBUTTONDOWN,	OnFlyEventRButtonDownItem)
AUI_END_EVENT_MAP()

const int	CDlgInventoryRide::ms_PackLineMax = 6;

CDlgInventoryRide::CDlgInventoryRide()
{
	m_pPlayer = 0;
	m_nAngle = 0;
	
	m_nOldMountType = 0;		// 旧坐骑背包类型
	m_nOldMountID = 0;			// 旧坐骑位置
	m_nOldMountItemID = 0;
}

CDlgInventoryRide::~CDlgInventoryRide()
{
	A3DRELEASE(m_pPlayer);
}

bool CDlgInventoryRide::OnInitDialog()
{
	m_pScl_Item = (PAUISCROLL)GetDlgItem("Scl_Item");
	m_pImg_Char = (PAUIIMAGEPICTURE)GetDlgItem("Pic_Part");
	m_pBtn_LeftTurn = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_LeftTurn");
	m_pBtn_RightTurn = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_RightTurn");
	m_pImg_Mounts = (PAUIIMAGEPICTURE)GetDlgItem("Img_Vechile01");
	m_pImg_Fly = (PAUIIMAGEPICTURE)GetDlgItem("Img_Vechile02");
	if (!m_pPlayer)
	{
		ReLoadModel();
	}
	UpdateTextArea((PAUITEXTAREA)GetDlgItem("Txt_Hint"));

	UpdataMountsOrFly();

	return true;
}

void CDlgInventoryRide::OnShowDialog()
{
	m_pPlayer->GetOffPet();
	m_pPlayer->ClearState(GP_STATE_FLY);
	m_pPlayer->ShowWing(false);
	int aEquips[SIZE_EQUIPIVTR];
	memcpy(aEquips, GetHostPlayer()->GetEquips(), sizeof(aEquips));
	m_pPlayer->ShowEquipments(aEquips, false, true);
	if (GetHostPlayer()->GetPetModel())
	{
		m_pPlayer->RideOnPet(GetHostPlayer()->GetRidingPetInfo().id, 1, 0);
	}
	else if (GetHostPlayer()->IsFlying())
	{
		m_pPlayer->SetState(GP_STATE_FLY);
		m_pPlayer->ShowWing(true);
	}
	UpdataMountsOrFly();
}

void CDlgInventoryRide::OnTick()
{
	if (m_pPlayer)
	{
		if (m_pPlayer->IsFlying())
		{
			m_pPlayer->Fly();
		}
		else
			m_pPlayer->Stand();
		m_pPlayer->Tick(GetGame()->GetTickTime());
		
		if( m_pBtn_LeftTurn->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK )
			m_nAngle = (m_nAngle + 360 - GetGame()->GetTickTime() / 6 ) % 360;
		else if( m_pBtn_RightTurn->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK )
			m_nAngle = (m_nAngle + GetGame()->GetTickTime() / 6) % 360;
		m_pImg_Char->SetRenderCallback(
			DlgModelMountWingRender,
			(DWORD)m_pPlayer,
			m_nAngle);
	}
	UpdateInventory();
}

void CDlgInventoryRide::UpdateInventory()
{
	CECInventory *pInventory = GetHostPlayer()->GetMountWingPack();
	int nIvtrSize = pInventory->GetSize();
	ACString strText;
	AString strName;
	strText.Format(_AL("%d"), nIvtrSize);
	GetDlgItem("Txt_Hint")->Show(nIvtrSize==0);
	PAUILABEL pLab = (PAUILABEL)GetDlgItem("Txt_Limit");
	AUIClockIcon *pClock;
	pLab->SetText(strText);
	int i, nMax;
	PAUIIMAGEPICTURE pImg;
	CECIvtrItem *pItem;
	for( i = 0; i < IVTRSIZE_RIDEPACK; i++ )
	{
		if (i>=pInventory->GetSize())
		{
			break;
		}
		strName.Format("Img_Item%d", i+1);
		pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		pImg->Show(true);
		pClock = pImg->GetClockIcon();
		pClock->SetProgressRange(0, 1);
		pClock->SetProgressPos(1);
		pItem = pInventory->GetItem(i);
		if (pItem)
		{
			GetGameUIMan()->SetIvtrInfo(pImg, pItem, pItem->GetCount(), false);
			pImg->SetDataPtr(pItem, "ptr_CECIvtrItem");
			if( pItem->GetCoolTime(&nMax) > 0 )
			{
				pClock->SetProgressRange(0, nMax);
				pClock->SetProgressPos(nMax - pItem->GetCoolTime());
				pClock->SetColor(A3DCOLORRGBA(0, 0, 0, 128));
			}
		}
		else
		{
			pImg->SetText(_AL(""));
			pImg->SetDataPtr(NULL, "ptr_CECIvtrItem");
			pImg->SetCover(NULL, -1);
			pImg->SetColor(A3DCOLORRGB(255, 255, 255));
		}		
	}
	if(m_pImg_Fly)
	{
		pClock = m_pImg_Fly->GetClockIcon();
		pClock->SetProgressRange(0, 1);
		pClock->SetProgressPos(1);
		CECInventory* pPack = GetHostPlayer()->GetPack(IVTRTYPE_EQUIPPACK);
		if(pPack)
		{
			CECIvtrItem* pTempItem = pPack->GetItem(EQUIPIVTR_WING);
			if(pTempItem)
			{
				GetGameUIMan()->SetIvtrInfo(m_pImg_Fly, pTempItem, pTempItem->GetCount(), false);
				m_pImg_Fly->SetDataPtr(pTempItem, "ptr_CECIvtrItem");
				if( pTempItem->GetCoolTime(&nMax) > 0 )
				{
					pClock->SetProgressRange(0, nMax);
					pClock->SetProgressPos(nMax - pTempItem->GetCoolTime());
					pClock->SetColor(A3DCOLORRGBA(0, 0, 0, 128));
				}
			}
			else
			{
				m_pImg_Fly->SetDataPtr(NULL, "ptr_CECIvtrItem");
				m_pImg_Fly->SetColor(A3DCOLORRGB(255, 255, 255));
			}
		}
	}
	if(m_pImg_Mounts && m_nOldMountID != -1)
	{
		pClock = m_pImg_Mounts->GetClockIcon();
		pClock->SetProgressRange(0, 1);
		pClock->SetProgressPos(1);
		CECInventory* pPack = GetHostPlayer()->GetPack(m_nOldMountType);
		if(pPack)
		{
			CECIvtrItem* pTempItem = pPack->GetItem(m_nOldMountID);
			if(pTempItem)
			{
				GetGameUIMan()->SetIvtrInfo(m_pImg_Mounts, pTempItem, pTempItem->GetCount(), false);
				m_pImg_Mounts->SetDataPtr(pTempItem, "ptr_CECIvtrItem");
				if( pTempItem->GetCoolTime(&nMax) > 0 )
				{
					pClock->SetProgressRange(0, nMax);
					pClock->SetProgressPos(nMax - pTempItem->GetCoolTime());
					pClock->SetColor(A3DCOLORRGBA(0, 0, 0, 128));
				}
			}
			else
			{
				m_pImg_Mounts->SetDataPtr(NULL, "ptr_CECIvtrItem");
				m_pImg_Mounts->SetColor(A3DCOLORRGB(255, 255, 255));
			}
		}
	}
	for (; i < IVTRSIZE_RIDEPACK; i++ )
	{
		strName.Format("Img_Item%d", i+1);
		pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		pImg->Show(false);
	}
}

void CDlgInventoryRide::OnCommand_Resume(const char * szCommand)
{
	m_pPlayer->GetOffPet();
	m_pPlayer->ClearState(GP_STATE_FLY);
	int aEquips[SIZE_EQUIPIVTR];
	memcpy(aEquips, GetHostPlayer()->GetEquips(), sizeof(aEquips));
	aEquips[EQUIPIVTR_WING] = 0;
	m_pPlayer->ShowEquipments(aEquips, false, true);
}

void CDlgInventoryRide::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if( !pObj || !pObj->GetDataPtr("ptr_CECIvtrItem") )
		return;
	
	CECIvtrItem *pItem = (CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem");
	if( !pItem->IsFrozen() )
	{
		A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
		POINT pt = 
		{
			GET_X_LPARAM(lParam) - p->X,
			GET_Y_LPARAM(lParam) - p->Y,
		};
		
		GetGameUIMan()->InvokeDragDrop(this, pObj, pt);
		GetGameUIMan()->GetDialog("DragDrop")->SetData(0);
		GetGameUIMan()->PlayItemSound((CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem"), false);
	}
}

void CDlgInventoryRide::OnEventRButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if( !pObj || !pObj->GetDataPtr("ptr_CECIvtrItem") )
		return;
	CECIvtrItem* pIvtrSrc = (CECIvtrItem*)pObj->GetDataPtr("ptr_CECIvtrItem");
	int iSrc = atoi(pObj->GetName() + strlen("Img_Item"))-1;

	if( ((CECIvtrEquip*)pIvtrSrc)->IsTalismanMain() && 
		((CECIvtrTalismanMain*)pIvtrSrc)->GetEssence().ess.master_id == 0 )
	{
		PAUIDIALOG pMsgBox;
		GetGameUIMan()->MessageBox("Game_EquipBind_Wing", GetStringFromTable(873), 
			MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		pMsgBox->SetData(iSrc);
		return;
	}
	else if ((pIvtrSrc->GetStateFlag() & ITEM_STATE_BINDAFTEREQUIP) &&
			!(pIvtrSrc->GetStateFlag() & ITEM_STATE_BINDED))
	{
		CommonMessage("wear", (int)pObj, 0);
		PAUIDIALOG pMsgBox;
		GetGameUIMan()->MessageBox("Game_EquipBind_Mount", GetStringFromTable(922), 
			MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		pMsgBox->SetData(iSrc);
		return;
	}
	else
	{
		if (pIvtrSrc->IsEquipment())
		{
			GetHostPlayer()->UseItemInPack(IVTRTYPE_MOUNTWING, iSrc);
		}
		else
		{
			CommonMessage("wear", (int)pObj, 0);
			GetGameSession()->c2s_CmdUseItem(IVTRTYPE_MOUNTWING, (BYTE)iSrc, pIvtrSrc->GetTemplateID(), 1);
			GetHostPlayer()->CheckRecordMount(IVTRTYPE_MOUNTWING,iSrc,pIvtrSrc->GetTemplateID());
		}
	}
}

int CDlgInventoryRide::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="wear")
	{
		PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)wParam;
		CECIvtrItem* pIvtrSrc = (CECIvtrItem*)pImg->GetDataPtr("ptr_CECIvtrItem");		
		if (pIvtrSrc->GetClassID()==CECIvtrItem::ICID_TALISMAN_MAINPART)
		{
			int aEquips[SIZE_EQUIPIVTR];
			memcpy(aEquips, GetHostPlayer()->GetEquips(), sizeof(aEquips));
			aEquips[EQUIPIVTR_WING] = pIvtrSrc->GetTemplateID();
			m_pPlayer->ShowEquipments(aEquips, false, true);
			m_pPlayer->GetOffPet();
			m_pPlayer->SetState(GP_STATE_FLY);
			m_pPlayer->ShowWing(true);
		}
		else if (pIvtrSrc->GetClassID()==CECIvtrItem::ICID_VEHICLE)
		{
			m_pPlayer->ClearState(GP_STATE_FLY);
			m_pPlayer->RideOnPet(pIvtrSrc->GetTemplateID(), 1, 0);

		}
	}
	return 0;
}

void CDlgInventoryRide::ReLoadModel()
{
	A3DRELEASE(m_pPlayer);
	m_pPlayer = new CECLoginPlayer(GetGame()->GetGameRun()->GetWorld()->GetPlayerMan());
	const GNET::RoleInfo& info = GetGame()->GetGameRun()->GetSelectedRoleInfo();
	
	if (!m_pPlayer->Load(CECPlayer::GetPhysiqueByProf(info.occupation), info.occupation, info.faceid, info.hairid, info.gender, NULL))
	{
		m_pPlayer->Release();
		delete m_pPlayer;
		m_pPlayer = NULL;
		return;
	}

	m_pPlayer->SetPos(A3DVECTOR3(0));
	m_pPlayer->SetDirAndUp(A3DVECTOR3(0, 0, 1.0f), A3DVECTOR3(0, 1.0f, 0));
	m_pPlayer->GetPlayerModel()->SetAutoUpdateFlag(false);
	
	int aEquips[SIZE_EQUIPIVTR];
	memcpy(aEquips, GetHostPlayer()->GetEquips(), sizeof(aEquips));
	m_pPlayer->ShowEquipments(aEquips, false, true);
}







void CDlgInventoryRide::UpdataMountsOrFly()
{
	EC_LOCAL_SETTING setting = GetGame()->GetConfigs()->GetLocalSettings();
	int nId = 0;
	CECInventory* pPack = GetHostPlayer()->GetPack(IVTRTYPE_EQUIPPACK);
	if(pPack)
	{
		CECIvtrItem* pTempItem = pPack->GetItem(EQUIPIVTR_WING);
		if(pTempItem)
		{
			nId = pTempItem->GetTemplateID();
		}
		GetGroupItem(nId,1);
		GetGame()->GetConfigs()->SaveLocalSetting();
	}


	if(setting.nRecordMountType != m_nOldMountType
		|| setting.nRecordMountID != m_nOldMountID
		|| setting.nRecordMountItemID != m_nOldMountItemID )
	{
		m_nOldMountType = setting.nRecordMountType;
		m_nOldMountID = setting.nRecordMountID;
		m_nOldMountItemID =setting.nRecordMountItemID;

		int nId = 0;
		CECInventory* pPack = GetHostPlayer()->GetPack(m_nOldMountType);
		if(pPack)
		{
			if(m_nOldMountID == -1)
			{
				GetGroupItem(0,0);
			}
			else
			{
				CECIvtrItem* pTempItem = pPack->GetItem(m_nOldMountID);
				if(pTempItem)
				{
					if(pTempItem->GetClassID() == CECIvtrItem::ICID_VEHICLE)
					{
						GetGroupItem(pTempItem->GetTemplateID(),0);
					}
					else
					{
						setting.nRecordMountType=0;
						setting.nRecordMountID = -1;
						GetGame()->GetConfigs()->SetLocalSettings(setting);
						UpdataMountsOrFly();
					}
				}
			}
		}
	}
}

void  CDlgInventoryRide::SendMsgMoveItem(int nType)
{
	ACString strText; 
	if (nType == 0)
	{
		int iSrc = EQUIPIVTR_WING, iDst = -1;
		CECInventory *pInventory;
		pInventory = GetHostPlayer()->GetPack(IVTRTYPE_MOUNTWING);
		iDst = pInventory->SearchEmpty();
		if ( -1!=iDst)
		{
			GetGameSession()->c2s_CmdExgEquMountWingItem(iSrc, iDst);
			return;
		}
		else	// 2013-1-4 add by zy 添加如果没有骑乘栏的话 返回背包
		{
			CECInventory *pInventory;
			pInventory = GetHostPlayer()->GetPack(IVTRTYPE_PACK);
			iDst = pInventory->SearchEmpty();
			if(iDst != -1)
			{
				GetGameSession()->c2s_CmdEquipItem(iDst, iSrc);
			}
			else
			{
				strText = GetStringFromTable(17600);
				GetGameUIMan()->MessageBox("", strText,MB_OK);
				return;
			}
		}
	}
	else
	{
		if(m_nOldMountID == -1)
		{
			strText = GetStringFromTable(17601);
			GetGameUIMan()->MessageBox("", strText,MB_OK);
			return;
		}
		CECIvtrItem* pIvtrSrc = GetHostPlayer()->GetPack(m_nOldMountType)->GetItem(m_nOldMountID);
		if(!pIvtrSrc)
		{
			strText = GetStringFromTable(17601);
			GetGameUIMan()->MessageBox("", strText,MB_OK);
			return;
		}

		if(pIvtrSrc)
		{
			if(pIvtrSrc->GetClassID() == CECIvtrItem::ICID_VEHICLE)	// 2013-1-6 add by zy 添加对物品使用前判断
			{
				GetGameSession()->c2s_CmdUseItem(m_nOldMountType, (BYTE)m_nOldMountID, pIvtrSrc->GetTemplateID(), 1);
			}
			else	// 	// 2013-1-6 add by zy 添加如果记录的物品位置错误 那么清空,重新刷新一次
			{
				EC_LOCAL_SETTING setting = GetGame()->GetConfigs()->GetLocalSettings();
				setting.nRecordMountType=0;
				setting.nRecordMountID = -1;
				GetGame()->GetConfigs()->SetLocalSettings(setting);
				strText = GetStringFromTable(17601);
				GetGameUIMan()->MessageBox("", strText,MB_OK);
				return;
			}
		}
		
	}
}

void CDlgInventoryRide::SendMsgUseItem(int nType)
{
	ACString strText; 
	if (nType == 0)
	{
		CDlgTalisman *pDlg = (CDlgTalisman *)GetGameUIMan()->GetDialog("Win_Talisman");
		if(pDlg)
		{
			CECInventory* pPack = GetHostPlayer()->GetPack(IVTRTYPE_EQUIPPACK);
			if(pPack)
			{
				CECIvtrItem* pTempItem = pPack->GetItem(EQUIPIVTR_MAIN_TALISMAN);
				if(!pTempItem)
				{
					strText =  GetStringFromTable(17602);
					GetGameUIMan()->MessageBox("EnabledFly", strText,MB_OK);
					if(GetDlgItem("Img_Vechile02"))
					{
						GetDlgItem("Img_Vechile02")->Enable(false);
					}
					return;
				}
			}
			CECIvtrTalismanMain *pItem = (CECIvtrTalismanMain*)pPack->GetItem(EQUIPIVTR_MAIN_TALISMAN);
			CECIvtrTalismanMain *pItem1 = (CECIvtrTalismanMain*)pPack->GetItem(EQUIPIVTR_WING);
			int nNow, nMax;
			nNow = GetHostPlayer()->GetBasicProps().iTalismanStamina;
			nMax = pItem->GetEssence().ess.stamina;
			if(!GetGameRun()->GetHelper()->IsRunning() && pItem1 && (GetHostPlayer()->IsFlying() || nNow >= nMax))
			{
				if( !GetHostPlayer()->IsFlying() )
					GetGameSession()->c2s_CmdStartFly();
				else
					GetGameSession()->c2s_CmdStopFly();
			}
			else
			{
				strText =  GetStringFromTable(17602);
				GetGameUIMan()->MessageBox("EnabledFly", strText,MB_OK);
				if(GetDlgItem("Img_Vechile02"))
				{
					GetDlgItem("Img_Vechile02")->Enable(false);
				}
				return;
			}
		}
	}
	else
	{
		if(m_nOldMountID == -1)
		{
			strText =  GetStringFromTable(17601);
			GetGameUIMan()->MessageBox("EnabledMount", strText,MB_OK);
			if(GetDlgItem("Img_Vechile01"))
			{
				GetDlgItem("Img_Vechile01")->Enable(false);
			}
			return;
		}
		CECIvtrItem* pIvtrSrc = GetHostPlayer()->GetPack(m_nOldMountType)->GetItem(m_nOldMountID);
		if(!pIvtrSrc)
		{
			strText = GetStringFromTable(17601);
			GetGameUIMan()->MessageBox("EnabledMount", strText,MB_OK);
			if(GetDlgItem("Img_Vechile01"))
			{
				GetDlgItem("Img_Vechile01")->Enable(false);
			}
			return;
		}
		if (pIvtrSrc && (pIvtrSrc->GetStateFlag() & ITEM_STATE_BINDAFTEREQUIP) &&
			!(pIvtrSrc->GetStateFlag() & ITEM_STATE_BINDED))
		{
			PAUIDIALOG pMsgBox;
			GetGameUIMan()->MessageBox("Game_Bind_Mount", GetStringFromTable(922), 
				MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
			pMsgBox->SetData(m_nOldMountID);
			pMsgBox->SetDataPtr((void*)m_nOldMountType,"Type");
			return;
		}
		if(pIvtrSrc)
		{
			if(pIvtrSrc->GetClassID() == CECIvtrItem::ICID_VEHICLE)	// 2013-1-6 add by zy 添加对物品使用前判断
			{
				GetGameSession()->c2s_CmdUseItem(m_nOldMountType, (BYTE)m_nOldMountID, pIvtrSrc->GetTemplateID(), 1);
			}
			else // 	// 2013-1-6 add by zy 添加如果记录的物品位置错误 那么清空,重新刷新一次
			{
				EC_LOCAL_SETTING setting = GetGame()->GetConfigs()->GetLocalSettings();
				setting.nRecordMountType=0;
				setting.nRecordMountID = -1;
				GetGame()->GetConfigs()->SetLocalSettings(setting);
				strText = GetStringFromTable(17601);
				GetGameUIMan()->MessageBox("", strText,MB_OK);
				GetDlgItem("Img_Vechile01")->Enable(false);
				return;
			}
			
		}
	}
}

void  CDlgInventoryRide::OnMountEventLButtonDownItem(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if( !pObj || !pObj->GetDataPtr("ptr_CECIvtrItem") )
		return;
	int nMax;
	CECInventory* pPack = GetHostPlayer()->GetPack(m_nOldMountType);
	if(pPack)
	{
		CECIvtrItem* pTempItem = pPack->GetItem(m_nOldMountID);
		if(pTempItem)
		{
			GetGameUIMan()->SetIvtrInfo(m_pImg_Mounts, pTempItem, pTempItem->GetCount(), false);
			m_pImg_Mounts->SetDataPtr(pTempItem, "ptr_CECIvtrItem");
			if( pTempItem->GetCoolTime(&nMax) > 0 )
			{
				return;
			}
		}
	}
		
	CECIvtrItem *pItem = (CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem");
	if( !pItem->IsFrozen() )
	{
		A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
		POINT pt = 
		{
			GET_X_LPARAM(lParam) - p->X,
			GET_Y_LPARAM(lParam) - p->Y,
		};
		
		GetGameUIMan()->InvokeDragDrop(this, pObj, pt);
		GetGameUIMan()->GetDialog("DragDrop")->SetData(0);
		GetGameUIMan()->PlayItemSound((CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem"), false);
	}
}

void  CDlgInventoryRide::OnMountEventRButtonDownItem(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	SendMsgMoveItem(1);
}

void  CDlgInventoryRide::OnFlyEventLButtonDownItem(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if( !pObj || !pObj->GetDataPtr("ptr_CECIvtrItem") )
		return;
	
	CECIvtrItem *pItem = (CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem");
	if( !pItem->IsFrozen() )
	{
		A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
		POINT pt = 
		{
			GET_X_LPARAM(lParam) - p->X,
			GET_Y_LPARAM(lParam) - p->Y,
		};
		
		GetGameUIMan()->InvokeDragDrop(this, pObj, pt);
		GetGameUIMan()->GetDialog("DragDrop")->SetData(0);
		GetGameUIMan()->PlayItemSound((CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem"), false);
	}
}

void  CDlgInventoryRide::OnFlyEventRButtonDownItem(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	SendMsgMoveItem(0);
}

void CDlgInventoryRide::GetGroupItem(int nID,int nType)
{
	PAUIIMAGEPICTURE pPicture;
	if(nType == 0)
	{
		pPicture = (PAUIIMAGEPICTURE)GetDlgItem("Img_Vechile01");
	}
	else if(nType == 1)
	{
		pPicture = (PAUIIMAGEPICTURE)GetDlgItem("Img_Vechile02");
	}
	if(nID == 0)
	{
		pPicture->ClearCover();
		pPicture->SetText(_AL(""));
		pPicture->SetDataPtr(NULL);
		return;
	}

	CECIvtrItem* pItem = CECIvtrItem::CreateItem(nID, 0,1);
	AString strFile;
	AUICTranslate trans;


	if (pItem )
	{
		pItem->SetNeedUpdate(false);
		af_GetFileTitle(pItem->GetIconFile(), strFile);
		strFile.MakeLower();
		pPicture->SetCover(GetGameUIMan()->GetIconSet(CECGameUIMan::ICONS_INVENTORY),
			GetGameUIMan()->GetIconId(CECGameUIMan::ICONS_INVENTORY,strFile));
		pPicture->SetCover(NULL, 0, 1);
		pPicture->SetHint(trans.Translate(pItem->GetDesc()));
		delete pItem;
	}
	else
	{
		pPicture->ClearCover();
		pPicture->SetText(_AL(""));
		pPicture->SetDataPtr(NULL);
	}
}