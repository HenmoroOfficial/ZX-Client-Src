// Filename	: DlgInventoryCostume.h
// Creator	: Fuchonggang
// Date		: 2009/04/08

#pragma once

#include "DlgBase.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIComboBox.h"
#include "AUI\\AUIScroll.h"
#include "EC_IvtrTypes.h"
#include "EC_Counter.h"

#define COSTUME_NUM			8
#define COSTUME_TYPE_NUM	3
#define CECDLGSHOP_PACKLINEMAX 6
#define CECDLGINVCOSTUME_PACKMAX 30
class CECLoginPlayer;

class CDlgInventoryCostume : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgInventoryCostume();
	virtual ~CDlgInventoryCostume();
	void OnCommand_ChangeQuickBar(const char * szCommand);
	void OnCommand_ResumeEquip(const char * szCommand);
	void OnCommand_ChangeCostume(const char * szCommand);
	void OnCommand_SaveQuickBar(const char * szCommand);
	void OnCommand_FashionPocketHelp(const char * szCommand);
	void OnCommand_AutoChangeType(const char * szCommand);
	void OnCommand_AutoChange(const char * szCommand);
	void OnCommand_ChangeTime(const char * szCommand);
	void OnCommand_CANCEL(const char * szCommand);

	void OnEventLButtonDownQuickHat(WPARAM, LPARAM, AUIObject *pObj);
	void OnEventLButtonDownQuickCloth(WPARAM, LPARAM, AUIObject *pObj);
	void OnEventLButtonDownQuickShoe(WPARAM, LPARAM, AUIObject *pObj);
	void OnEventLButtonDown(WPARAM, LPARAM, AUIObject *pObj);
	void OnLButtonDbClkItem(WPARAM, LPARAM, AUIObject *pObj);
	void OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	bool InCostumePacket(int id);
	void ChangeCostume(int index);
	void UpdateAutoChange(DWORD dwTime);
	void ReLoadModel();
	int GetFirstItem() { return m_nFirstLine * CECDLGSHOP_PACKLINEMAX; }
	
protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual bool Release();
	virtual void OnTick();

	bool UpdateInventory();
	bool UpdateCostomeQuick();

	PAUIIMAGEPICTURE		m_pImgItem [CECDLGINVCOSTUME_PACKMAX];
	PAUIIMAGEPICTURE		m_pImgItemQuick[COSTUME_NUM][COSTUME_TYPE_NUM];
	PAUIIMAGEPICTURE		m_pImg_Char;
	PAUISTILLIMAGEBUTTON	m_pBtn_LeftTurn;
	PAUISTILLIMAGEBUTTON	m_pBtn_RightTurn;
	PAUICOMBOBOX			m_pComboTime;
	PAUISCROLL				m_pScl_Item;
//	int					m_nCostumeB[COSTUME_NUM][COSTUME_TYPE_NUM];
	int					m_nAngle;
	static int					m_aEquips[SIZE_EQUIPIVTR];	//	Equipment item ID array
	static CECLoginPlayer*		m_pPlayer;
	CECCounter		m_ChangeTimeCnt;
	bool			m_bRandom;
	bool			m_bAutoChange;
	int				m_iCurIndex;
	int				m_nFirstLine;
};
