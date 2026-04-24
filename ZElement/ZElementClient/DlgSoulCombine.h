// Filename	: DlgPetEquip.h
// Creator	: Fu Chonggang
// Date		: 2009/1/06

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

#define CDLGSOULCOMBINE_MAX_MATERIALS	6

class CDlgSoulCombine : public CDlgBase  
{
	friend class CECGameUIMan;
	
	AUI_DECLARE_COMMAND_MAP()
		
public:
	CDlgSoulCombine();
	virtual ~CDlgSoulCombine();
	void OnCommand_add(const char * szCommand);
	void OnCommand_minus(const char * szCommand);
	void OnCommand_max(const char * szCommand);
	void OnCommand_begin(const char * szCommand);
	void OnCommand_stop(const char * szCommand);
	void OnCommand_prolist(const char * szCommand);
	void OnCommand_help(const char * szCommand);
	void OnCommand_CANCEL(const char * szCommand);

	void ProduceOne();
	void ProduceEnd();

protected:
	DWORD m_idCurRecipe, m_idProducing;
	int	m_nRecipeSum;
	int m_idTerritory; // 缓存npc身上挂的领土id

	PAUIPROGRESS m_pPrgs_1;
	PAUIEDITBOX m_pTxt_Number;
	PAUILABEL m_pTxt_lab[3];
	PAUILABEL m_pTxt_out[3];
	PAUIIMAGEPICTURE m_pItem_out[3];
	PAUISTILLIMAGEBUTTON m_pBtn_Produce;
	PAUIIMAGEPICTURE m_pImgItem[CDLGSOULCOMBINE_MAX_MATERIALS];
	PAUILABEL m_pTxt_name[CDLGSOULCOMBINE_MAX_MATERIALS];
	PAUILABEL m_pTxt_no[CDLGSOULCOMBINE_MAX_MATERIALS];
	PAUICOMBOBOX m_pCombo_ProList;
	PAUITREEVIEW m_pTv_Recipe;

	virtual bool OnInitDialog();
	virtual void OnTick();
	virtual bool Render();
	virtual void OnShowDialog();

	void UpdateRecipe(int idRecipe);
	void UpdateMaxAmount();
	void UpdateSelectItem();
	void CleanUpRecipe();
	void LoadAvailableSoulRecipe();

	void AddOneRecipe(int idRecipe);
};
