// Filename	: DlgPetFactory.h
// Creator	: Xiao Zhou
// Date		: 2008/4/24

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

#define CDLGPETFACTORY_MAX_MATERIALS	6
#define CDLGPETFACTORY_MAX_PRODUCTS		3

class CDlgPetFactory : public CDlgBase  
{
	friend class CECGameUIMan;

	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgPetFactory();
	virtual ~CDlgPetFactory();

	void OnCommand_add(const char * szCommand);
	void OnCommand_minus(const char * szCommand);
	void OnCommand_max(const char * szCommand);
	void OnCommand_begin(const char * szCommand);
	void OnCommand_stop(const char * szCommand);
	void OnCommand_prolist(const char * szCommand);
	void OnCommand_CANCEL(const char * szCommand);

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
	PAUIIMAGEPICTURE m_pItem_Product[CDLGPETFACTORY_MAX_PRODUCTS];
	PAUILABEL m_pTxt_Product[CDLGPETFACTORY_MAX_PRODUCTS];
	PAUILABEL m_pTxt_Rate[CDLGPETFACTORY_MAX_PRODUCTS];
	PAUISTILLIMAGEBUTTON m_pBtn_Produce;
	PAUIIMAGEPICTURE m_pImgItem[CDLGPETFACTORY_MAX_MATERIALS];
	PAUILABEL m_pTxt_name[CDLGPETFACTORY_MAX_MATERIALS];
	PAUILABEL m_pTxt_no[CDLGPETFACTORY_MAX_MATERIALS];
	PAUICOMBOBOX m_pCombo_ProList;
	PAUITREEVIEW m_pTv_Recipe;
	int m_nLive[7];

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
