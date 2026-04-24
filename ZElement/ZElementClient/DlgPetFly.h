// Filename	: DlgPetFly.h
// Creator	: Fu Chonggang
// Date		: 2013.8.16
// 宠物飞升PetLevelUp.xml

#pragma once

#include "DlgItemSvrBase.h"

class CDlgPetFly : public CDlgItemSvrBase  
{
public:
	CDlgPetFly();
	virtual ~CDlgPetFly();

	virtual void SetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg=NULL);
	virtual void ServerStart();
	virtual void Reset();

protected:
	virtual void OnTick();

	static int ms_iFlyProp;	// 宠物飞升道具
};
