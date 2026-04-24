// File		: DlgEPEquip.h
// Creator	: Xiao Zhou
// Date		: 2005/12/15

#pragma once

#include "DlgBase.h"
#include "EC_IvtrTypes.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUICheckBox.h"

class CECLoginPlayer;

class CDlgEPEquip : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgEPEquip();
	virtual ~CDlgEPEquip();
	void OnCommand_Fashion(const char * szCommand);
	void OnCommand_ShowEquip(const char * szCommand);
	void OnCommand_Information(const char * szCommand);
	void OnCommand_CANCEL(const char * szCommand);


	void UpdateEquip(int idPlayer);
	void UpdateIcon();

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual bool Tick(void);

	PAUIIMAGEPICTURE		m_pImgEquip[SIZE_EQUIPIVTR];
	PAUIIMAGEPICTURE		m_pImgPet;
	PAUILABEL				m_pTxt_Name;
	PAUILABEL				m_pTxt_Prof;
	PAUILABEL				m_pTxt_NickName;
	PAUIIMAGEPICTURE		m_pImg_Char;
	PAUISTILLIMAGEBUTTON	m_pBtn_LeftTurn;
	PAUISTILLIMAGEBUTTON	m_pBtn_RightTurn;
	PAUICHECKBOX			m_pChk_ShowEquip;
	
	int					m_nAngle;
	int					m_nID;

	CECLoginPlayer*		m_pPlayer;
};
