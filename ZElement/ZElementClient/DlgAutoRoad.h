/*
 * FILE: DlgAutoRoad.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Liyi, 2005/12/2
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#ifndef _DLG_AUTO_ROAD_H_
#define _DLG_AUTO_ROAD_H_

#include "DlgHomeBase.h"

class AUILabel;
class AUISlider;

class CDlgAutoRoad : public CDlgHomeBase
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
protected:
	AUISlider* m_pSliderMaxRoadHeight;
	AUISlider* m_pSliderMainSmoothRoad;
	AUISlider* m_pSliderPartSmoothRoad;
	AUISlider* m_pSliderRoadWidth;
	AUISlider* m_pSliderSeedRoad;

	AUILabel* m_pLabelMaxRoadHeight;
	AUILabel* m_pLabelMainSmoothRoad;
	AUILabel* m_pLabelPartSmoothRoad;
	AUILabel* m_pLabelRoadWidth;
	AUILabel* m_pLabelSeedRoad;
	
	int m_nMaxRoadHeight;
	int m_nMainSmoothRoad;
	int m_nPartSmoothRoad;
	int m_nRoadWidth;
	int m_nSeedRoad;

	bool m_bCheckFlatRoad;

	TextureParamInfo_t m_texParamInfo;

	bool m_bChangeRoad;
	
public:
	CDlgAutoRoad();
	virtual ~CDlgAutoRoad();

	virtual AUIStillImageButton * GetSwithButton();
	TextureParamInfo_t* GetTextureParamInfo() { return &m_texParamInfo;}

	void OnLButtonDownScreen(WPARAM wParam, LPARAM lParam);
	void OnRButtonUpScreen(WPARAM wParam, LPARAM lParam);
	void OnRButtonDblClk(WPARAM wParam, LPARAM lParam);

protected:

	virtual void DoDataExchange(bool bSave);
	void SetSliderBound();
	void LoadDefault();
	void SetLabelsText();

	virtual bool OnInitContext();
	virtual void OnShowDialog();

	void OnCommandMoveSlider(const char * szCommand);
	void OnCommandOpenChooseTexDlg(const char* szCommand);
	void OnCommandEditRoad(const char* szCommand);
	void OnCommandEditFlat(const char* szCommand);
	void OnCommandPerspectiveView(const char* szCommand);
	void OnCommandCreateRoad(const char* szCommand);
	void OnCommandClearAllRoad(const char* szCommand);
	void OnCommandCreateFlat(const char* szCommand);

};

#endif