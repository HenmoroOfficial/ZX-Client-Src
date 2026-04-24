/*
 * FILE: DlgAutoMountain.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Liyi, 2005/12/2
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#ifndef _DLG_AUTO_MOUNTAIN_H_
#define _DLG_AUTO_MOUNTAIN_H_

#include "DlgHomeBase.h"


class AUISlider;
class AUILabel;
class CCurvePoint;

class CDlgAutoMountain : public CDlgHomeBase
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

protected:
	AUISlider* m_pSliderMaxHeightHill;
	AUISlider* m_pSliderMainSmoothHill;
	AUISlider* m_pSliderPartSmoothHill;
	AUISlider* m_pSliderTerrainSeedHill;
	AUISlider* m_pSliderBridgeScope;
	AUISlider* m_pSliderBridgeDepth;
	AUISlider* m_pSliderBorderErode;
	AUISlider* m_pSliderLineHillWidth;

	AUILabel* m_pLabelMaxHeightHill;
	AUILabel* m_pLabelMainSmoothHill;
	AUILabel* m_pLabelPartSmoothHill;
	AUILabel* m_pLabelTerrainSeedHill;
	AUILabel* m_pLabelBridgeScope;
	AUILabel* m_pLabelBridgeDepth;
	AUILabel* m_pLabelBorderErode;
	AUILabel* m_pLabelLineHillWidth;
	
	int m_nMaxHeightHill;
	int m_nMainSmoothHill;
	int m_nPartSmoothHill;
	int m_nTerrainSeedHill;
	int m_nBridgeScope;
	int m_nBridgeDepth;
	int m_nBorderErode;
	int m_nLineHillWidth;

	bool m_bCheckHillUp;
	int m_nUpDownPri;

	CCurvePoint* m_pCurveHeight;
	CCurvePoint* m_pCurveLine;

	TextureParamInfo_t m_texParamInfo;

	bool m_bChangeHillPoly;
	bool m_bChangeHillLine;

public:
	CDlgAutoMountain();
	virtual ~CDlgAutoMountain();

	virtual AUIStillImageButton * GetSwithButton();

	void OnLButtonDownScreen(WPARAM wParam, LPARAM lParam);
	void OnRButtonUpScreen(WPARAM wParam, LPARAM lParam);
	void OnRButtonDblClk(WPARAM wParam, LPARAM lParam);

	inline TextureParamInfo_t* GetTextureParamInfo() { return &m_texParamInfo;}
	
protected:


	virtual bool OnInitContext();
	virtual void OnShowDialog();

	virtual void DoDataExchange(bool bSave);

	void SetSliderBound();
	void LoadDefault();
	void SetLabelsText();

	void OnCommandMoveSlider(const char * szCommand);
	void OnCommandEditHillPoly(const char* szCommand);
	void OnCommandCreateHillPoly(const char* szCommand);
	void OnCommandEditHillLine(const char* szCommand);
	void OnCommandCreateHillLine(const char* szCommand);
	
	void OnCommandPerspectiveView(const char* szCommand);
	void OnCommandOpenHeightCurveDlg(const char* szCommand);
	void OnCommandOpenLineCurveDlg(const char* szCommand);
	void OnCommandOpenChooseTextureDlg(const char* szCommand);
	void OnCommandOnRadio(const char* szCommand);
	void OnCommandClearAllHill(const char* szCommand);
	void OnCommandRefresh(const char* szCommand);

};
#endif