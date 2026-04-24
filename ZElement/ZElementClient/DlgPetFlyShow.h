// Filename	: DlgPetFlyShow.h
// Creator	: Fu Chonggang
// Date		: 2013.8.16
// 宠物飞升过程展示PetLvUp_Img.xml

#pragma once

#include "DlgBase.h"

class AUIImagePicture;
class CECModel;
class CECPetBedge;

class CDlgPetFlyShow : public CDlgBase  
{

	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgPetFlyShow();
	virtual ~CDlgPetFlyShow();

	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);

protected:
	virtual bool OnInitDialog();
	virtual bool Release();
	virtual void OnTick();
	virtual void OnHideDialog();
	void OnCommand_Help(const char * szCommand);

	void LoadPetModel();

	AUIImagePicture *m_pImgPet;
	CECModel	*m_pPet;

	CECPetBedge	*m_pIvtrPet; // 飞升中的宠物，随时改变形象
	int			m_iShape;	 // 宠物飞升以后形象改变，用这个记录看是否改变
	float		m_fModelSize;
};
