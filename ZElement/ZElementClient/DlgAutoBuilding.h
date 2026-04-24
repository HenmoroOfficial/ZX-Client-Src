/*
 * FILE: DlgAutoBuilding.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Liyi, 2005/12/2
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#ifndef _DLG_AUTO_BUILDING_H_
#define _DLG_AUTO_BUILDING_H_

#include "DlgHomeBase.h"

class AUIListBox;
class CDlgAutoBuilding:public CDlgHomeBase
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
protected:
	AArray<DWORD, DWORD>		m_aAddedModelId;
	
	AUIListBox* m_pListBoxBuildings;
	int m_nBuildingIdx;

public:
	CDlgAutoBuilding();
	virtual ~CDlgAutoBuilding();
	
	AUIStillImageButton * GetSwithButton();
	
	AArray<DWORD, DWORD>* GetAddedModelID() { return &m_aAddedModelId;}

	void ShowModelInfos();
	bool UpdateModelInfos();
	bool DeleteLastBuilding();
	bool DeleteAllBuildings();

	void OnLButtonDownScreen(WPARAM wParam, LPARAM lParam);
	void OnLButtonUpScreen(WPARAM wParam, LPARAM lParam);

protected:
	virtual bool OnInitContext();
	virtual void OnShowDialog();
	virtual void DoDataExchange(bool bSave);



	void OnCommandOpenChooseModelDlg(const char* szCommand);
	void OnCommandDelModelInfo(const char* szCommand);
	void OnCommandClearAllModelInfo(const char* szCommand);
	
	void OnCommandEditBuilding(const char* szCommand);
	void OnCommandPerspectiveView(const char* szCommand);
};

#endif