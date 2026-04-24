/*
 * FILE: DlgAutoChoosePlant.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Liyi, 2005/12/16
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#ifndef _DLG_AUTO_CHOOSE_PLANT_H_
#define _DLG_AUTO_CHOOSE_PLANT_H_

#include "DlgHomeBase.h"

class AUISlider;
class AUIListBox;
class AUILabel;

class CDlgAutoChoosePlant : public CDlgHomeBase
{	
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgAutoChoosePlant();
	virtual ~CDlgAutoChoosePlant();
	enum PlantType_e
	{
		PT_NULL,
		PT_TREE,
		PT_LANDGRASS,
		PT_WATERGRASS,
	};
	
	void InitChoosePlant(PlantType_e pt);

protected:
	virtual bool OnInitContext();
	virtual void OnShowDialog();
	virtual void DoDataExchange(bool bSave);
	
	void SetSliderBound();
	void LoadDefault();
	void SetLabelsText();
	bool InitResource();
	void ClearListBox();
	
	void FillListBox(PlantType_e pt);

	void OnCommandMoveSlider(const char * szCommand);
	void OnCommandOK(const char* szCommand);
	void OnCommandCancel(const char* szCommand);
	
protected:
	AUISlider* m_pSliderWeight;
	int m_nWeight;
	float m_fWeight;
	
	AUILabel* m_pLabelWeight;
	AUIListBox* m_pListBoxPlants;

	PlantType_e m_PlantType;

	CAutoGrass::GRASSINFO* m_pLandGrassInfo;
	int m_nNumLandGrassInfos;

	CAutoGrass::GRASSINFO* m_pWaterGrassInfo;
	int m_nNumWaterGrassInfos;

	CAutoForest::TREEINFO* m_pTreeInfo;
	int m_nNumTreeInfos;

	PlantType_e m_PT;//÷≤ŒÔ¿‡–Õ
};

#endif