// Filename	: DlgCameraPath.cpp
// Creator	: Fu Chonggang
// Date		: 2010/4/1

#include "AFI.h"
#include "AFileImage.h"
#include "DlgCameraPath.h"
#include "DlgAchievement.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_IvtrConsume.h"
#include "EC_HostPlayer.h"
#include "EC_GameUIMan.h"
#include "EC_Inventory.h"
#include "EC_Configs.h"

#define new A_DEBUG_NEW

const int s_dwTimeFontIn = 500; // 文字淡入
const int s_dwTimeFontOut = 500; // 文字淡出
const int s_dwEdgeIn = 1000;	//边框出现
const int s_dwEdgeOut = 500;	//边框消失


AUI_BEGIN_COMMAND_MAP(CDlgCameraPath, CDlgBase)


AUI_END_COMMAND_MAP()

CDlgCameraPath::CDlgCameraPath()
{
}

CDlgCameraPath::~CDlgCameraPath()
{
}

bool CDlgCameraPath::OnInitDialog()
{
	m_pLabText = (PAUILABEL)GetDlgItem("Txt_Tips");
	m_pImgBlack = (PAUIIMAGEPICTURE)GetDlgItem("Img_Black");
	m_pImgBlack->Show(false);
	m_bEnableTextAlpha = true;

	m_nAppearAction = ACTION_NONE;
	m_fAlphaDeltaBlack = 0.0f;
	m_fAlphaDeltaWhite = 0.0f;
	
	m_nHeightTop = GetDlgItem("Shape1")->GetSize().cy;
	m_nHeightBottom = GetDlgItem("Shape2")->GetSize().cy;
	return true;
}

void CDlgCameraPath::OnShowDialog()
{
	m_dwAlphaTick = GetTickCount();
	m_nTextBackAction = ACTION_WHITE;
}

void CDlgCameraPath::OnCommandCancel(const char* szCommand)
{
}

bool CDlgCameraPath::Render()
{
	int iWid = g_pGame->GetConfigs()->GetSystemSettings().iRndWidth;
	int iHei = g_pGame->GetConfigs()->GetSystemSettings().iRndHeight;
	if (iWid!=GetSize().cx || iHei!=GetSize().cy)
	{
		SetPosEx(0,0);
		SetSize(iWid, iHei);
		GetDlgItem("Shape1")->SetPos(0,0);
		GetDlgItem("Shape1")->SetSize(iWid, GetDlgItem("Shape1")->GetSize().cy);
		GetDlgItem("Shape2")->SetPos(0,iHei-GetDlgItem("Shape2")->GetSize().cy);
		GetDlgItem("Shape2")->SetSize(iWid, GetDlgItem("Shape2")->GetSize().cy);
		//m_pLabText->SetPos(int(iWid/2), m_pLabText->GetPos(true).y);
		m_pLabText->SetPos(int(iWid/2), iHei - int(GetDlgItem("Shape2")->GetSize().cy * 0.42));
		m_pImgBlack->SetSize(iWid, iHei);
	}
//	if(GetHostPlayer()->GetCameraPath()->IsUsingCameraPath() == false)
//		return true;
	DWORD dwCurTick = GetTickCount();
	float s = (dwCurTick - m_dwAlphaTick) / 300.0f;
	int nAlpha = (int)((s / (s + 1)) * 255.0f);
	SetAlpha(nAlpha);
	
	if (m_nTextBackAction==ACTION_WHITE)
	{
		if ((dwCurTick - m_dwAlphaTick)<s_dwEdgeIn)
		{
			GetDlgItem("Shape1")->SetPos(0, (int)(dwCurTick - m_dwAlphaTick - s_dwEdgeIn)*(GetDlgItem("Shape1")->GetSize().cy)/s_dwEdgeIn);
			GetDlgItem("Shape2")->SetPos(0, iHei - GetDlgItem("Shape2")->GetSize().cy*(int)(dwCurTick - m_dwAlphaTick)/s_dwEdgeIn);
		}
		else
		{
			GetDlgItem("Shape1")->SetPos(0, 0);
			GetDlgItem("Shape2")->SetPos(0, iHei-GetDlgItem("Shape2")->GetSize().cy);
			m_nTextBackAction = ACTION_NONE;
		}
	}
	else if (m_nTextBackAction==ACTION_BLACK)
	{
		if ((dwCurTick - m_dwAlphaTick)<s_dwEdgeOut)
		{
			GetDlgItem("Shape1")->SetPos(0, (int)(m_dwAlphaTick-dwCurTick)*(GetDlgItem("Shape1")->GetSize().cy)/s_dwEdgeOut);
			GetDlgItem("Shape2")->SetPos(0, iHei - GetDlgItem("Shape2")->GetSize().cy*(int)(s_dwEdgeOut - dwCurTick + m_dwAlphaTick)/s_dwEdgeOut);
		}
		else
		{
			GetDlgItem("Shape1")->Show(false);
			GetDlgItem("Shape2")->Show(false);
			m_pLabText->Show(false);
			m_nTextBackAction = ACTION_NONE;
		}
	}
	if (m_bEnableTextAlpha)
	{
		if(m_dwTickBase + m_dwElapse < dwCurTick)
		{
			m_pLabText->SetText(IncSubtitle() ? m_strCurrent : _AL(""));
		}

		if (dwCurTick<=(m_dwTickBase+s_dwTimeFontIn) && dwCurTick>=m_dwTickBase)
		{
			m_pLabText->SetAlpha(int((dwCurTick-m_dwTickBase)*255/s_dwTimeFontIn));
		}
		else if ( dwCurTick + s_dwTimeFontOut >= m_dwTickBase + m_dwElapse && dwCurTick <= m_dwTickBase + m_dwElapse)
		{
			m_pLabText->SetAlpha(int((m_dwTickBase + m_dwElapse - dwCurTick)*255/s_dwTimeFontOut));
		}
		else
			m_pLabText->SetAlpha(255);
	}
	else
	{
		m_pLabText->SetAlpha(255);
	}

	static DWORD dwTime = GetTickCount();
	if (m_nAppearAction!=ACTION_NONE)
	{
		if (m_nAppearAction==ACTION_BLACK)
		{
			float alphaNow = m_pImgBlack->GetAlpha() + m_fAlphaDeltaBlack*(dwCurTick-dwTime);
			if (alphaNow>255.0)
			{
				m_pImgBlack->SetAlpha(255);
				m_nAppearAction=ACTION_WHITE;
			}
			else
				m_pImgBlack->SetAlpha((int)alphaNow);
		}
		else if (m_nAppearAction==ACTION_WHITE)
		{
			float alphaNow = m_pImgBlack->GetAlpha() + m_fAlphaDeltaWhite*(dwCurTick-dwTime);
			if (alphaNow<0.0)
			{
				m_pImgBlack->SetAlpha(0);
				m_nAppearAction=ACTION_NONE;
			}
			else
				m_pImgBlack->SetAlpha((int)alphaNow);

		}
	}
	dwTime = dwCurTick;

	return CDlgBase::Render();
}

void CDlgCameraPath::ShowText(const ACHAR* szText)
{
	m_idxBegin = 0;
	m_strTotalSubtitle = szText;
	if(IncSubtitle())
		m_pLabText->SetText(m_strCurrent);
	if (!IsShow())
	{
		Show(true);
	}
}

void CDlgCameraPath::ShowTextBackGround(bool bShow)
{
	if (bShow && !m_pLabText->IsShow())
	{
		m_dwAlphaTick = GetTickCount();
		m_nTextBackAction = ACTION_WHITE;
	}
	else if (!bShow && m_pLabText->IsShow())
	{
		m_dwAlphaTick = GetTickCount();
		m_nTextBackAction = ACTION_BLACK;
	}
	GetDlgItem("Shape1")->Show(true);
	GetDlgItem("Shape2")->Show(true);
	m_pLabText->Show(true);
}

// 是否允许文字淡入淡出
void CDlgCameraPath::EnableTextAlpha(bool bEnable)
{
	m_bEnableTextAlpha = bEnable;
}

bool CDlgCameraPath::IsEnableTextAlpha() const
{
	return m_bEnableTextAlpha;
}

void CDlgCameraPath::SetBgImage(AString name)
{
	AUIOBJECT_SETPROPERTY p;
	sprintf(p.fn, "ingame\\%s", name);
	m_pImgBlack->SetProperty("Image File",&p);
}

void CDlgCameraPath::Shadow(DWORD dwTimeDark, DWORD dwTimeWhite)
{
	m_pImgBlack->Show(true);
	m_pImgBlack->SetAlpha(0);
	m_nAppearAction = ACTION_BLACK;
	m_fAlphaDeltaBlack = 255.0f/dwTimeDark;
	m_fAlphaDeltaWhite = -255.0f/dwTimeWhite;
}

void CDlgCameraPath::AlphaInfo(int alpha)
{
	a_Clamp(alpha, 0, 255);
	m_nAppearAction = ACTION_NONE;
	m_pImgBlack->Show(true);
	m_pImgBlack->SetAlpha(alpha);
}

BOOL CDlgCameraPath::IncSubtitle()
{
	if(m_idxBegin >= m_strTotalSubtitle.GetLength() )
	{
		m_dwElapse = (DWORD)-1;
		return false;
	}
	int nEnd = m_strTotalSubtitle.Find(_AL('\n'), m_idxBegin);
	if(nEnd < 0)
		nEnd = m_strTotalSubtitle.GetLength();
	
	m_strCurrent = m_strTotalSubtitle.Left(nEnd);
	m_strCurrent.CutLeft(m_idxBegin);
	
	m_dwElapse = 1000;
	if(m_strCurrent[0] == _AL('$'))
	{
		m_strCurrent.CutLeft(1);
		int nDollar = m_strCurrent.Find(_AL('$'));
		if(nDollar > 0)
		{
			ACString strElapse = m_strCurrent.Left(nDollar);
			m_strCurrent.CutLeft(nDollar + 1);
			m_dwElapse = strElapse.ToInt();
		}
	}
	
	m_dwTickBase = GetTickCount();
	m_idxBegin = nEnd + 1;
	return TRUE;
}