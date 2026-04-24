// Filename	: DlgProduce.h
// Creator	: Tom Zhou
// Date		: October 11, 2005

#pragma once

#include "DlgBase.h"
#include "EC_IvtrTypes.h"
#include "AUI\\AUIEditBox.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIProgress.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUITreeView.h"
#include "AUI\\AUIComboBox.h"
#include "AUI\\AUICheckBox.h"

#define CDLGPRODUCE_MAX_MATERIALS	6

class CDlgProduce : public CDlgBase  
{
	friend class CECGameUIMan;

	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgProduce();
	virtual ~CDlgProduce();

	void OnCommand_add(const char * szCommand);
	void OnCommand_minus(const char * szCommand);
	void OnCommand_max(const char * szCommand);
	void OnCommand_begin(const char * szCommand);
	void OnCommand_stop(const char * szCommand);
	void OnCommand_prolist(const char * szCommand);
	void OnCommand_help(const char * szCommand);
	void OnCommand_CANCEL(const char * szCommand);
	
	void SetCanAutoSell(bool bAutoSell);
	void ShowAutoSell(bool bShow);
	bool GetAutoSellOpt();
	void ProduceOne();
	void ProduceEnd();

protected:
	bool m_bFirstShow;
	DWORD m_idCurRecipe, m_idProducing;
	int	m_nRecipeSum;

	PAUIPROGRESS m_pPrgs_1;
	PAUIEDITBOX m_pTxt_Number;
	PAUILABEL m_pTxt_out;
	PAUILABEL m_pTxt_CoolTime;
	PAUIIMAGEPICTURE m_pItem_out;
	PAUISTILLIMAGEBUTTON m_pBtn_Produce;
	PAUILABEL m_pTxt_Level;
	PAUILABEL m_pTxt_Exp;
	PAUIIMAGEPICTURE m_pImgItem[CDLGPRODUCE_MAX_MATERIALS];
	PAUILABEL m_pTxt_name[CDLGPRODUCE_MAX_MATERIALS];
	PAUILABEL m_pTxt_no[CDLGPRODUCE_MAX_MATERIALS];
	PAUICOMBOBOX m_pCombo_ProList;
	PAUITREEVIEW m_pTv_Recipe;
	PAUICHECKBOX m_pChk_AutoSell;

	virtual bool OnInitDialog();
	virtual void OnTick();
	virtual bool Render();
	virtual void OnShowDialog();

	void UpdateRecipe(int idRecipe);
	void UpdateMaxAmount();
	void UpdateSelectItem();
	void CleanUpRecipe();

	void AddOneRecipe(int idRecipe);
};
