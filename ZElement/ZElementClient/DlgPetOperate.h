// Filename	: DlgPetOperate.h
// Creator	: Fu Chonggang
// Date		: 2012-6-20

#pragma once

#include "DlgItemSvrBase.h"

/************************************************************************
宠物驯养
宠物放生
************************************************************************/
class CDlgPetOperate : public CDlgItemSvrBase  
{
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgPetOperate();
	virtual ~CDlgPetOperate();
	
	void OnCommandStopOpt(const char * szCommand);

	virtual void SetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg=NULL);
	virtual void ServerStart();
	virtual void Reset();
	virtual void UpdateDisplay();

	void PetFree();
protected:
	virtual bool OnInitDialog();
};

/************************************************************************
宠物改名
************************************************************************/
class CDlgPetReName : public CDlgItemSvrBase
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
public:
	CDlgPetReName() {}
	virtual ~CDlgPetReName() {}

	void OnCommandCancel(const char * szCommand);
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	
	virtual void SetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg=NULL);
	virtual void ServerStart();
	virtual void Reset();
	
protected:
	bool CheckNameValid(ACHAR* szText);

};

/************************************************************************
宠物装备修炼
************************************************************************/
class CDlgPetEquipService : public CDlgItemSvrBase  
{
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgPetEquipService(){}
	virtual ~CDlgPetEquipService(){}
	
	void OnCommandStopOpt(const char * szCommand);
	
	virtual void SetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg=NULL);
	virtual void ServerStart();
	virtual void Reset();
};
