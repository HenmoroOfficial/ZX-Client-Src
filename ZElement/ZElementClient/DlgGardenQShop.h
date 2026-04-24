// Filename	: DlgGardenQShop.h
// Creator	: Fu Chonggang
// Date		: 2009/08/21

#pragma once

#include "DlgBase.h"

class AUILabel;
class AUIImagePicture;
class AUIScroll;
class A2DSprite;

namespace HOMETOWN
{
	struct TOOL_ESSENCE;
}

class CDlgGardenQShop : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

	enum
	{
		GARDENQSHOPITEM_NUM = 6,
	};
	
public:
	CDlgGardenQShop();
	virtual ~CDlgGardenQShop();
	void OnCommand_Confirm(const char * szCommand);
	void OnCommand_Close(const char * szCommand);
	void OnEventLButtonDownItem(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	
	void OnPrtc_BuyItem(void*);
	static A2DSprite* m_pA2DSpriteItemSelect;
protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	virtual bool Release();
	void UpdateItems();


	AUILabel *m_pLabItem[GARDENQSHOPITEM_NUM][3];
	AUIImagePicture *m_pImgItem[GARDENQSHOPITEM_NUM];
	AUIScroll *m_pScl_Item;
	abase::vector<const HOMETOWN::TOOL_ESSENCE*> m_vecToolItems;

	int m_iFirstLine;
	int m_iCapacity;
	int m_iCurType;
	int m_iCurIndex;
};
