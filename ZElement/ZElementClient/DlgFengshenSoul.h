// Filename	:	DlgFengshenSoul.h
// Creator	:	Fu Chonggang
// Date		:	Sep 7, 2010

#pragma once


#include "DlgBase.h"
class AUIEditBox;
class AUILabel;
class AUIRadioButton;
class AUIStillImageButton;
class AUIImagePicture;
class AUIProgress;
class CECModel;
class CECPortraitRender;

class CDlgFengshenSoul : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
	enum
	{
		STATUS_CIRCLE,
		STATUS_MOUSEON,
		STATUS_CLICK,
		STATUS_CLICK_SECONDTIME,
	};

public:
	CDlgFengshenSoul();
	virtual ~CDlgFengshenSoul();

	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	virtual bool Release();

	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	
	void OnCommand_Page(const char * szCommand);
	void OnCommand_Upgrade(const char * szCommand);
	void OnCommand_Help(const char * szCommand);
	void OnCommand_Detail(const char * szCommand);
	void OnCommand_CANCEL(const char * szCommand);

	CECPortraitRender * GetPortrait()		{ return m_pPortraitRender; }

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	virtual bool Render();
	
	void _UpdateSoulInfo();
	AString _GetSuitGfxName(int status=-1);

	AUIEditBox *m_pShape4;
	AUILabel *m_pTitle;
	AUILabel *m_pLab_Mate;
	AUILabel *m_pTxt_DeTaoism;			//克仙
	AUILabel *m_pLab_Contribution;
	AUILabel *m_pTxt_DeBuddhism;	//克佛
	AUILabel *m_pLab_Devotion;
	AUILabel *m_pTxt_DeEvil;		//克魔
	AUIRadioButton *m_pRdo_1;
	AUIRadioButton *m_pRdo_2;
	AUIRadioButton *m_pRdo_3;
	AUIRadioButton *m_pRdo_4;
	AUIStillImageButton *m_pBtn_Cancel;
	AUIStillImageButton *m_pBtn_Upgrade;
	AUILabel *m_pLab_Combine;
	AUILabel *m_pTxt_Sp;		//神圣力
	AUIImagePicture *m_pImg_Soul;
	AUIImagePicture *m_pImg_Item;
	AUIProgress		*m_pPrg_DeityExp;
	AUILabel		*m_pLab_DeityExp;
	AUIProgress		*m_pPrg_Dp;
	AUILabel		*m_pLab_Dp;
	AUILabel		*m_pLab_Lv;
	
	CECModel*	m_pModel;
	int			m_nStatus;
	int			m_nCultivation;
	int			m_nDeityLv;
	
	const static int STAGELEVEL;
	const static int UPBOUNDLEVEL;
	CECPortraitRender *		m_pPortraitRender;		// viewport to render the portrait map

	void EnterState(int stat);
};
