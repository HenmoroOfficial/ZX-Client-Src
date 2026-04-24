// Filename	:	DlgAniversaryPet5.h
// Creator	:	Lei Dongyu
// Date		:	Apr 17, 2012

#pragma once


#include "DlgBase.h"

class AUILabel;
class AUIImagePicture;
class AUIStillImageButton;
class AUIProgress;

class CECModel;
struct LITTLE_PET_UPGRADE_CONFIG;

class CDlgAniversaryPet5 : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgAniversaryPet5();
	virtual ~CDlgAniversaryPet5();

	virtual bool Release();

	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	void OnCommand_CANCEL(const char * szCommand);
	void OnCommand_Adopt(const char * szCommand);
	void OnCommand_get(const char * szCommand);
	void OnCommand_raise(const char * szCommand);

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	virtual bool Render();
	void ChangeModel();

	AUIImagePicture *m_pPic_Part;
	AUIStillImageButton *m_pBtn_Adopt;
	AUILabel *m_pTxt_Lv;
	AUIProgress *m_pPrgs_Exp;
	AUIStillImageButton *m_pBtn_Get;
	AUIStillImageButton *m_pBtn_Raise;
	AUIImagePicture *m_pImg[5];
	//AUIImagePicture *m_pImg_2;
	//AUIImagePicture *m_pImg_3;
	//AUIImagePicture *m_pImg_4;
	//AUIImagePicture *m_pImg_5;
	AUIImagePicture *m_pImg_Cover[5];
// 	AUIImagePicture *m_pImg_Cover2;
// 	AUIImagePicture *m_pImg_Cover3;
// 	AUIImagePicture *m_pImg_Cover4;
// 	AUIImagePicture *m_pImg_Cover5;
	AUIImagePicture * m_pImg_Raise[2];
	AUIImagePicture * m_pImg_Choose[2];
	AUILabel *m_pLab_Hint1;
	AUILabel *m_pLab_Hint2;
	AUILabel *m_pTxt_Exp;

	CECModel* m_pModel;
	int m_iLevel;
	int m_iExp;
	int m_iNextAward;	//还没领取的最小等级奖品

	int m_iSelectFood;
	
	bool m_bLoaded;
	LITTLE_PET_UPGRADE_CONFIG* m_pConfig;
	//猪小仙结构
};

class CDlganiversarypet5_min : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP()
		
public:
	CDlganiversarypet5_min();
	virtual ~CDlganiversarypet5_min();
	
	void OnCommand_max(const char * szCommand);
	
protected:
	virtual bool OnInitDialog();
	
	AUIStillImageButton *m_pBtn_Max;
};

class CDlgAniversaryMail5Min : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP()
		
public:
	CDlgAniversaryMail5Min(){}
	virtual ~CDlgAniversaryMail5Min(){}
	
	void OnCommand_max(const char * szCommand);
};
