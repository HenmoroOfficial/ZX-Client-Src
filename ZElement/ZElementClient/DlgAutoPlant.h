/*
 * FILE: DlgAutoPlant.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Liyi, 2005/12/2
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#ifndef _DLG_AUTO_PLANT_H_
#define _DLG_AUTO_PLANT_H_

#include "DlgHomeBase.h"

class AUISlider;
class AUILabel;
class AUIListBox;

typedef AArray<PLANTIDWEIGHTINFO, PLANTIDWEIGHTINFO&> InfosArray_t;

class CDlgAutoPlant: public CDlgHomeBase
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
protected:
	
	AUISlider* m_pSliderTreeSeed;
	AUISlider* m_pSliderTreeDensity;
	AUISlider* m_pSliderTreeSlope;
	AUISlider* m_pSliderTreeSpaceX;
	AUISlider* m_pSliderTreeSpaceZ;
	AUISlider* m_pSliderTreeSpaceLine;
	AUISlider* m_pSliderTreeSpaceNoise;
	AUISlider* m_pSliderGrassDensity;
	AUISlider* m_pSliderGrassSeed;

	AUILabel* m_pLabelTreeSeed;
	AUILabel* m_pLabelTreeDensity;
	AUILabel* m_pLabelTreeSlope;
	AUILabel* m_pLabelTreeSpaceX;
	AUILabel* m_pLabelTreeSpaceZ;
	AUILabel* m_pLabelTreeSpaceLine;
	AUILabel* m_pLabelTreeSpaceNoise;
	AUILabel* m_pLabelGrassDensity;
	AUILabel* m_pLabelGrassSeed;

	int m_nTreeSeed;
	int m_nTreeDensity;
	int m_nTreeSlope;
	int m_nTreeSpaceX;
	int m_nTreeSpaceZ;
	int m_nTreeSpaceLine;
	int m_nTreeSpaceNoise;
	int m_nGrassDensity;
	int m_nGrassSeed;

	bool m_bCheckTreeRegular;

	AUIListBox* m_pListBoxTrees;
	AUIListBox* m_pListBoxLandGrass;
	AUIListBox* m_pListBoxWaterGrass;

	InfosArray_t m_aTreeInfos;
	InfosArray_t m_aLandGrassInfos;
	InfosArray_t m_aWaterGrassInfos;

	bool m_bChangeForestPoly;
	bool m_bChangeForestLine;
	bool m_bChangeGrassPoly;
public:
	CDlgAutoPlant();
	virtual ~CDlgAutoPlant();

	virtual AUIStillImageButton * GetSwithButton();

	void OnLButtonDownScreen(WPARAM wParam, LPARAM lParam);
	void OnRButtonUpScreen(WPARAM wParam, LPARAM lParam);
	void OnRButtonDblClk(WPARAM wParam, LPARAM lParam);

	InfosArray_t* GetTreeInfosArray() { return &m_aTreeInfos;}
	InfosArray_t* GetLandGrassInfosArray() { return &m_aLandGrassInfos;}
	InfosArray_t* GetWaterGrassInfosArray() { return &m_aWaterGrassInfos;}

	void ShowTreeInfos();
	void ShowLandGrassInfos();
	void ShowWaterGrassInfos();

protected:
	virtual bool OnInitContext();
	virtual void OnShowDialog();

	virtual void DoDataExchange(bool bSave);

	void SetSliderBound();
	void LoadDefault();
	void SetLabelsText();

	void OnCommandMoveSlider(const char * szCommand);
	void OnCommandOpenChooseTreeDlg(const char* szCommand);
	void OnCommandOpenChooseLandGrassDlg(const char* szCommand);
	void OnCommandOpenChooseWaterGrassDlg(const char* szCommand);

	void OnCommandEditTreePoly(const char* szCommand);
	void OnCommandEditTreeLine(const char* szCommand);
	void OnCommandEditGrass(const char* szCommand);

	void OnCommandCreateTreePoly(const char* szCommand);
	void OnCommandCreateTreeLine(const char* szCommand);
	void OnCommandCreateLandGrass(const char* szCommand);
	void OnCommandCreateWaterGrass(const char* szCommand);
	
	void OnCommandPerspectiveView(const char* szCommand);

	void OnCommandClearAllTreeInfo(const char* szCommand);
	void OnCommandClearAllLandGrassInfo(const char* szCommand);
	void OnCommandClearAllWaterGrassInfo(const char* szCommand);
	
	void OnCommandDelTreeInfo(const char* szCommand);
	void OnCommandDelLandGrassInfo(const char* szCommand);
	void OnCommandDelWaterGrassInfo(const char* szCommand);

	void OnCommandClearAllTrees(const char* szCommand);
	void OnCommandClearAllGrass(const char* szCommand);

};

#endif