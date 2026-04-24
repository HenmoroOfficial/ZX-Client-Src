// Filename	: DlgPetFlyChangeAppearance.cpp
// Creator	: Fu Chonggang
// Date		: 2013.8.16
// 改变宠物形象PetLvUp_SetImg.xml

#pragma once

#include "DlgBase.h"

class AUIImagePicture;
class CECModel;
class CECPetBedge;

class CDlgPetFlyChangeAppearance : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();

public:
	CDlgPetFlyChangeAppearance();
	virtual ~CDlgPetFlyChangeAppearance();

	void OnCommandSelect(const char * szCommand);
	void OnCommandSave(const char * szCommand);

	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);

protected:
	virtual bool OnInitDialog();
	virtual bool Release();
	virtual void OnTick();

	bool ChangeShape(int index);

	AUIImagePicture	*m_pImgPet;
	CECModel	*m_pPet;
	CECPetBedge	*m_pIvtrPet;
	float		m_fModelSize;
	bool		m_bLoaded;
};
