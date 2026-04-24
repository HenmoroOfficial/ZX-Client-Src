/*
 * FILE: DlgAutoTerrain.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Liyi, 2005/12/2
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#ifndef _DLG_AUTO_TERRAIN_H_
#define _DLG_AUTO_TERRAIN_H_

#include "DlgHomeBase.h"

class AUISlider;
class AUILabel;
class CDlgAutoTerrain : public CDlgHomeBase
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
	
protected:

	AUISlider* m_pSliderMaxHeight;
	AUISlider* m_pSliderMainSmooth;
	AUISlider* m_pSliderPartSmooth;
	AUISlider* m_pSliderTerrainSeed;

	AUILabel* m_pLabelMaxHeight;
	AUILabel* m_pLabelMainSmooth;
	AUILabel* m_pLabelPartSmooth;
	AUILabel* m_pLabelTerrainSeed;

	int m_nMaxHeight;
	int m_nMainSmooth;
	int m_nPartSmooth;
	int m_nTerrainSeed;

	TextureParamInfo_t m_texParamInfo;

public:
	CDlgAutoTerrain();
	virtual ~CDlgAutoTerrain();

	virtual AUIStillImageButton * GetSwithButton();

	TextureParamInfo_t* GetTextureParamInfo() { return &m_texParamInfo;}
	
protected:
	virtual bool OnInitContext();
	virtual void OnShowDialog();
	virtual void DoDataExchange(bool bSave);

	void SetSliderBound();
	void LoadDefault();
	void SetLabelsText();

	void OnCommandMoveSlider(const char * szCommand);
	void OnCommandCreateTerrain(const char* szCommand);
	void OnCommandOpenChooseTexture(const char* szCommand);
	void OnCommandCreateTexture(const char* szCommand);
};
#endif
