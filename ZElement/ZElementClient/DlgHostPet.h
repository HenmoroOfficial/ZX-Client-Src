// File		: DlgHostPet.h
// Creator	: Xiao Zhou
// Date		: 2005/8/17

#pragma once

#include "DlgBase.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIProgress.h"
#include "AUI\\AUICheckBox.h"
#include "AUI\\AUILabel.h"

class CDlgHostPet : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgHostPet();
	virtual ~CDlgHostPet();

	void OnCommandAutoFood(const char * szCommand);
	void OnCommandPetSetting(const char * szCommand);
	void OnCommandCombine1(const char * szCommand);
	void OnCommandCombine2(const char * szCommand);
	void OnCommandWithDraw(const char * szCommand);
	void OnCommandDetail(const char * szCommand);
	void OnCommandCancel(const char * szCommand);
	void OnCommand_close(const char * szCommand);
	void OnEventLButtonUp_Head(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	
	void AutoFeed();

	static int GetAutoFoodHp() { return m_nAutoFoodHp; }
	static int GetAutoFoodMp() { return m_nAutoFoodMp; }
	static int GetAutoFoodFull() { return m_nAutoFoodFull; }
	static void SetAutoFoodHp(int value) { m_nAutoFoodHp = value; }
	static void SetAutoFoodMp(int value) { m_nAutoFoodMp = value; }
	static void SetAutoFoodFull(int value) { m_nAutoFoodFull = value; }

protected:
	virtual void OnTick();
	virtual bool OnInitDialog();

	PAUISTILLIMAGEBUTTON m_pBtn_Feed;
	PAUIIMAGEPICTURE	m_pPic_HeadFrame;
	PAUIIMAGEPICTURE	m_pImg_Hungry;
	PAUIPROGRESS		m_pPrgs_Hp;
	PAUIPROGRESS		m_pPrgs_Mp;
	PAUIPROGRESS		m_pPrgs_Exp;
	PAUICHECKBOX		m_pChk_Unit;
	PAUICHECKBOX		m_pChk_Use;
	PAUILABEL			m_pTxt_Name;
	PAUILABEL			m_pTxt_Lv;
	
	static int					m_nAutoFoodHp;
	static int					m_nAutoFoodMp;
	static int					m_nAutoFoodFull;
	static DWORD				m_dwTickAutoFoodHp;
	static DWORD				m_dwTickAutoFoodMp;
	static DWORD				m_dwTickAutoFoodFull;

};
