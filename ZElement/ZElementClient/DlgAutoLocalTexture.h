/*
 * FILE: DlgAutoLocalTexture.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Liyi, 2005/12/2
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#ifndef _DLG_AUTO_LOCAL_TEXTURE_H_
#define _DLG_AUTO_LOCAL_TEXTURE_H_

#include "DlgHomeBase.h"

class AUISlider;
class AUILabel;

class CDlgAutoLocalTexture : public CDlgHomeBase
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
protected:
	AUISlider* m_pSliderLocalTexBorderScope;
	AUISlider* m_pSliderSeedLocalTexture;

	AUILabel* m_pLabelLocalTexBorderScope;
	AUILabel* m_pLabelSeedLocalTexture;

	int m_nLocalTexBorderScope;
	int m_nSeedLocalTexture;

	int m_nMainTexture;

	TextureParamInfo_t m_texParamInfo;
	
	bool m_bChangeTexArea;
	
public:
	CDlgAutoLocalTexture();
	virtual ~CDlgAutoLocalTexture();

	virtual AUIStillImageButton * GetSwithButton();

	TextureParamInfo_t* GetTextureParamInfo() { return &m_texParamInfo;}

	void OnLButtonDownScreen(WPARAM wParam, LPARAM lParam);
	void OnRButtonUpScreen(WPARAM wParam, LPARAM lParam);
	void OnRButtonDblClk(WPARAM wParam, LPARAM lParam);

protected:
	virtual bool OnInitContext();
	virtual void OnShowDialog();

	virtual void DoDataExchange(bool bSave);
	void SetSliderBound();
	void LoadDefault();
	void SetLabelsText();

	void OnCommandMoveSlider(const char * szCommand);
	void OnCommandOpenChooseTexDlg(const char* szCommand);
	void OnCommandEditLocalTex(const char* szCommand);
	void OnCommandCreateLocalTex(const char* szCommand);
	void OnCommandClearLocalTex(const char* szCommand);
};

#endif