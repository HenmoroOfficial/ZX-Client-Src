// Filename	: DlgCameraPath.h
// Creator	: Fu Chonggang
// Date		: 2010/4/1

#pragma once

#include "DlgBase.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagePicture.h"
class CDlgCameraPath : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();

	enum
	{
		ACTION_NONE,
		ACTION_BLACK,
		ACTION_WHITE,
	};
public:
	CDlgCameraPath();
	virtual ~CDlgCameraPath();

	//显示文字信息及上下边框
	void ShowText(const ACHAR* szText);
	void ShowTextBackGround(bool bShow = true);
	void EnableTextAlpha(bool bEnable); // 是否允许文字淡入淡出
	bool IsEnableTextAlpha() const;
	
	//显示及调整背景图
	void SetBgImage(AString name);
	void Shadow(DWORD dwTimeDark, DWORD dwTimeWhite);
	void AlphaInfo(int alpha);

	//以上两套函数相互独立，互不影响
	void OnCommandCancel(const char* szCommand);
	
protected:
	virtual	bool OnInitDialog();
	virtual void OnShowDialog();
	virtual bool Render();

	BOOL IncSubtitle();

	PAUILABEL			m_pLabText;
	PAUIIMAGEPICTURE	m_pImgBlack;
	
	ACString	m_strTotalSubtitle;
	ACString	m_strCurrent;
	
	int			m_idxBegin;
	DWORD		m_dwTickBase;
	DWORD		m_dwElapse;
	DWORD		m_dwAlphaTick;

	int			m_nAppearAction;
	float		m_fAlphaDeltaBlack;
	float		m_fAlphaDeltaWhite;
	// 界面显示过程中上下出现shape
	int			m_nTextBackAction;
	int			m_nHeightTop;
	int			m_nHeightBottom;

	bool		m_bEnableTextAlpha;
};
