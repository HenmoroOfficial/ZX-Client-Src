// Filename	: DlgDragDrop.h
// Creator	: Tom Zhou
// Date		: October 20, 2005

#pragma once

#include "DlgBase.h"
#include "AUI\\AUIImagePicture.h"
#include "hashmap.h"

class CECIvtrItem;

class CDlgDragDrop : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP();

public:
	CDlgDragDrop();
	virtual ~CDlgDragDrop();

	typedef void (CDlgDragDrop::*PDRAGFUNC)();
	typedef abase::hash_map<AString, PDRAGFUNC>	FuncMap;

	void OnEventLButtonUp(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventMouseMove(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	virtual void SetCapture(bool bCapture, int x = -1, int y = -1);
	POINT m_ptDragPos;

protected:
	PAUIIMAGEPICTURE m_pImgGoods;
	PAUIDIALOG m_pDlgSrc;
	PAUIOBJECT m_pObjSrc;
	PAUIDIALOG m_pDlgDst;
	PAUIOBJECT m_pObjDst;
	CECIvtrItem *m_pIvtrSrc;
	CECIvtrItem *m_pIvtrDst;
	int m_nSrcInv;
	int m_nDstInv;
	FuncMap		m_mapFuncs;

	SIZE		m_nOriSize;	// xp技能栏图标尺寸不是36*36, 所以在拖图标前把拖拽图标尺寸改大了, 拖拽结束时需改回原尺寸 wuweixin

	virtual bool OnInitDialog();
	void DragExecute();
	void DragTrade();
	void DragGardenInventory();
	void DragInventory();
	void DragInventoryPet();
	void DragInventoryCostume();
	void DragInventoryWing();
	void DragCharacter();
	void DragAction();
	void DragSkillComboEdit();
	void DragSkill();
	void DragTalent();
	void DragTalismanSmall();
	void DragTalisman();
	void DragPet();
	void DragQuickBar();
	void DragShop();
	void DragShopReputation();
	void DragStorage();
	void DragSplit();
	void DragIdentify();
	void DragPShop2();
	void DragPetEquip();
	void DragTarget();
	void DragRune2013();
};
