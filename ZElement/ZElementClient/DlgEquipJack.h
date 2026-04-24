// Filename	:	DlgEquipJack.h
// Creator	:	Fu Chonggang
// Date		:	Mar 15, 2011
// 插槽相关功能

#pragma once

#include "DlgItemSvrBase.h"

class AUIImagePicture;
class CECIvtrGem;

enum
{
	EQUIP_JACK_NUM = 3,
};

//////////////////////////////////////////////////////////////////////////
// CDlgEquipJack
//////////////////////////////////////////////////////////////////////////

class CDlgEquipJack : public CDlgItemSvrBase
{
	AUI_DECLARE_COMMAND_MAP()
	enum
	{
		JACK_TYPE_NONE = 0,
		JACK_TYPE_PERFECT = 13,
		JACK_TYPE_NUM = 14,
	};
public:
	CDlgEquipJack();
	virtual ~CDlgEquipJack();
	
	void OnCommand_Lock(const char * szCommand);
	
	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	virtual void SetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg=NULL);
	virtual void ServerStart();
	virtual void UpdateDisplay();
	virtual void Reset();

protected:
	virtual bool OnInitDialog();
	virtual void OnTick();

	AUIImagePicture *m_pImg_Jack[EQUIP_JACK_NUM];
	AUIImagePicture *m_pImg_Props[EQUIP_JACK_NUM];
	AUILabel		*m_pLab_Names[EQUIP_JACK_NUM];
	AUIImagePicture *m_pImg_Back[EQUIP_JACK_NUM];
	AUIImagePicture *m_pImg_BackGem[EQUIP_JACK_NUM];
	AUIImagePicture *m_pImg_BackProp[EQUIP_JACK_NUM];
	AUIImagePicture *m_pImg_Gfx[EQUIP_JACK_NUM];
	bool			m_bEnable;
};

//////////////////////////////////////////////////////////////////////////
// CDlgEquipJack
//////////////////////////////////////////////////////////////////////////


class CDlgEquipGem : public CDlgItemSvrBase
{
	AUI_DECLARE_COMMAND_MAP()
		
	enum
	{
		JACK_TYPE_NONE = 0,
		JACK_TYPE_PERFECT = 13,
		JACK_TYPE_NUM = 14,
	};
public:
	CDlgEquipGem();
	virtual ~CDlgEquipGem();
	
	void OnCommand_Detach(const char * szCommand);
	
	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	virtual void SetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg=NULL);
	virtual void ServerStart();
	virtual void UpdateDisplay();
	virtual void Reset();
	
protected:
	virtual bool OnInitDialog();
	virtual void OnTick();
	
	AUIImagePicture *m_pImg_Jack[EQUIP_JACK_NUM];
	AUILabel		*m_pLab_Names[EQUIP_JACK_NUM];
	AUIImagePicture *m_pImg_Back[EQUIP_JACK_NUM];
	AUIImagePicture *m_pImg_BackGem[EQUIP_JACK_NUM];
	AUIImagePicture *m_pImg_Gfx[EQUIP_JACK_NUM];
	
	/** 寻找合适插槽，返回0、1、2，没有合适的返回-1 2011.5.13 */
	int FindSuitSlot(CECIvtrGem *pGem);
	void SetGemConfirm(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg);
};
