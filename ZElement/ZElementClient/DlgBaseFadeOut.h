// Filename	: DlgBaseFadeOut.h
// Creator	: Fu Chonggang
// Date		: 2012.5.4

#pragma once

#include "DlgBase.h"

class CDlgBaseFadeOut : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

	enum
	{
		FADE_NOTHING,
		FADE_IN,
		FADE_OUT,
	};
public:
	CDlgBaseFadeOut();
	virtual ~CDlgBaseFadeOut();

	virtual bool Render(void);
	virtual void Show(bool bShow, bool bModal = false, bool bActive = true);
	virtual bool Tick(void);
	
	void OnCommand_Fade(const char * szCommand);
protected:
	bool RenderAnimation();

	A3DTexture*	m_pA3DTexture;
private:
	int m_iScale;
	int	m_iStage;

	static const int ms_iFadeTime;
};

