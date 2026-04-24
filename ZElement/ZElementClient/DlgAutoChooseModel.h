/*
 * FILE: DlgAutoChooseModel.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Liyi, 2005/12/19
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "DlgHomeBase.h"

#ifndef _DlG_AUTO_CHOOSE_MODEL_H_
#define _DLG_AUTO_CHOOSE_MODEL_H_

class AUIListBox;

class CDlgAutoChooseModel: public CDlgHomeBase
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

protected:
	AUIListBox* m_pListBoxModels;
	int m_nModelIdx;

	CAutoBuilding::MODELINFO* m_pModelInfos;
	int m_nNumModelInfos;
	
public:
	CDlgAutoChooseModel();
	virtual ~CDlgAutoChooseModel();

protected:
	virtual bool OnInitContext();
	virtual void OnShowDialog();
	virtual void DoDataExchange(bool bSave);
	
	bool InitResource();
	void ClearListBox();

	void OnCommandOK(const char* szCommand);
	void OnCommandCancel(const char* szCommand);
};

#endif