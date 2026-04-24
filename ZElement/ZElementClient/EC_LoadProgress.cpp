/*
 * FILE: EC_LoadProgress.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/5/4
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_LoadProgress.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_Configs.h"

#ifdef BACK_VERSION
#include "EC_Viewport.h"
#else
#include "ECViewport.h"
#endif

#include "AAssist.h"
#include "A2DSprite.h"
#include "A3DEngine.h"
#include "A3DDevice.h"
#include "A3DMacros.h"
#include "A3DViewport.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "AUI\\AUIManager.h"
#include "AUI\\A3DFTFont.h"
#include "AUI\\A3DFTFontMan.h"
#include "AUI\\AUILuaManager.h"
#include "AUI\\AUILuaDialog.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIProgress.h"
#include "AF.h"

#define new A_DEBUG_NEW

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Reference to External variables and functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Local Types and Variables and Global variables
//	
///////////////////////////////////////////////////////////////////////////

//static const char* l_szProcBar = "进度条.tga";
static const char* l_szProcBar = "载入条.bmp";
static const char* l_szLogo = "loading_logo.tga";
static const char* l_szBarTop = "龙头层.tga";

#define LOADINGUI_WIDTH   1280
#define LOADINGUI_HEIGHT  960
#define LOADINGUI_RATIO   1024.F/1280.F

///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECLoadProgress
//	
///////////////////////////////////////////////////////////////////////////

CECLoadProgress::CECLoadProgress()
{
	m_pBackPic	= NULL;
	m_pBarPic	= NULL;
	m_pLogoPic	= NULL;
	m_pBarTopPic= NULL;
	m_pFont		= NULL;
	m_pLoadingUIMan	= NULL;
	m_iPos		= 0;
	m_iMin		= 0;
	m_iMax		= 100;
	m_iStep		= 10;
}

CECLoadProgress::~CECLoadProgress()
{
}

//	Initialize object
bool CECLoadProgress::Init(int iLoadID, const char* szBackPic)
{
	A3DDevice* pA3DDevice = g_pGame->GetA3DDevice();

	int iWid = g_pGame->GetConfigs()->GetSystemSettings().iRndWidth;
	int iHei = g_pGame->GetConfigs()->GetSystemSettings().iRndHeight;
	float fAspect = (float)iHei / iWid;

	float fScaleH = iWid / 1024.0f;
	float fScaleV = iHei / 768.0f;

	fScaleH = fScaleH * LOADINGUI_RATIO;
	fScaleV = fScaleV * LOADINGUI_RATIO;
	//	Load background picture
	char szFile[MAX_PATH];

	if (szBackPic)
		strcpy(szFile, szBackPic);
	else
		sprintf(szFile, "载入1024-%d.dds", a_Random(100, 799) / 100);

// 	A3DRECT Rect(0, 0, 1024, 768);
// 	m_pBackPic = new A2DSprite;
// 	if (!m_pBackPic->Init(pA3DDevice, szFile, 0, 0, 0, 1, &Rect))
// 	{
// 		delete m_pBackPic;
// 		m_pBackPic = NULL;
// 		return false;
// 	}
// 	m_pBackPic->SetScaleX(fScaleH);
// 	m_pBackPic->SetScaleY(fScaleV);
// 	m_pBackPic->SetLinearFilter(true);
// 
// 	//	Load process bar picture
// 	Rect.SetRect(0, 0, 8, 9);
// 	Rect.bottom = (int)(Rect.bottom * fScaleV + 0.5f);
// 
// 	m_pBarPic = new A2DSprite;
// 	m_pBarPic->Init(pA3DDevice, l_szProcBar, 0);
// 	m_pBarPic->SetLinearFilter(true);
// 
// 	//	Set process bar area
// 	//m_rcBar.SetRect(154, 722, 865, 731);
// 	m_rcBar.SetRect(224, 722, 805, 731);
// 	m_rcBar.left	= (int)(m_rcBar.left * fScaleH + 0.5f);
// 	m_rcBar.top		= (int)(m_rcBar.top * fScaleV + 0.5f);
// 	m_rcBar.right	= (int)(m_rcBar.right * fScaleH + 0.5f);
// 	m_rcBar.bottom	= (int)(m_rcBar.bottom * fScaleV + 0.5f);

 	m_pLogoPic = new A2DSprite;
 	m_pLogoPic->Init(pA3DDevice, l_szLogo, 0);
 	m_pLogoPic->SetLinearFilter(true);
 	m_pLogoPic->SetScaleX(fScaleH);
 	m_pLogoPic->SetScaleY(fScaleV);
 	m_pLogoPic->SetPosition(431 * fScaleH, 697 * fScaleV);

// 	m_pBarTopPic = new A2DSprite;
// 	m_pBarTopPic->Init(pA3DDevice, l_szBarTop, 0);
// 	m_pBarTopPic->SetLinearFilter(true);
// 	m_pBarTopPic->SetScaleX(fScaleH);
// 	m_pBarTopPic->SetScaleY(fScaleV);
// 	m_pBarTopPic->SetPosition(6 * fScaleH, 708 * fScaleV);
// 
 	int nCount = 0;
	CECStringTab hint;
	if( hint.Init("configs\\loading_hint.txt", true) )
	{
		while(hint.GetWideString(nCount))
			nCount++;
		int iCurHint = rand() % nCount;
		m_strHint = hint.GetWideString(iCurHint); 
	}
// 	m_xHint = (int)(160 * fScaleH);
// 	m_yHint = (int)(685 * fScaleV);
// 
// 	AWScriptFile s;
// 	if( s.Open("Configs\\loading_hint_font.txt") )
// 	{
// 		PAUIMANAGER pUI = (PAUIMANAGER)g_pGame->GetGameRun()->GetUIManager()->GetLoadingUIMan();
// 		if( pUI && s.GetNextToken(true) )
// 		{
// 			m_pFont = pUI->GetA3DFTFontMan()->CreatePointFont(12*fScaleH, s.m_szToken, STYLE_NORMAL, 0);
// 		}
// 		s.Close();
// 	}
// 	else
// 	{
// 		PAUIMANAGER pUI = (PAUIMANAGER)g_pGame->GetGameRun()->GetUIManager()->GetLoadingUIMan();
// 		if( pUI )
// 		{
// 			m_pFont = pUI->GetDefaultFont();
// 		}
// 	}
	
	A3DRELEASE(m_pLoadingUIMan)
	m_pLoadingUIMan = new AUILuaManager;
	if (!m_pLoadingUIMan)
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECLoadProgress::Init", __LINE__);
		return false;
	}

	if (!m_pLoadingUIMan->Init(g_pGame->GetA3DEngine(), g_pGame->GetA3DDevice(), g_pGame->GetConfigs()->GetLoadingUIDcf(), LOADINGUI_WIDTH, LOADINGUI_HEIGHT))
	{
		m_pLoadingUIMan = NULL;
		::MessageBoxA(NULL, "无法加载游戏数据文件：界面相关文件！\n此错误可能是更新不完全所致，请重新进行更新或进行版本验证！", "错误", MB_OK | MB_ICONSTOP);
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECLoadProgress::Init", __LINE__);

		return false;
	}

	A3DDEVFMT devFmt = g_pGame->GetA3DDevice()->GetDevFormat();
	A3DRECT rcOld, rcNew;
	rcOld.SetRect(0, 0, LOADINGUI_WIDTH, LOADINGUI_HEIGHT);
	rcNew.SetRect(0, 0, devFmt.nWidth, devFmt.nHeight);
	m_pLoadingUIMan->RearrangeWindows(rcOld, rcNew);
	PAUIDIALOG pDlg = m_pLoadingUIMan->GetDialog("Win_Loading");
	PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)pDlg->GetDlgItem("Img_Loading");
	AUIOBJECT_SETPROPERTY p;
	sprintf(p.fn, szFile);
	pImg->SetProperty("Image File", &p);

	bool bFit = true;
	float DevRatio, DlgRatio;
	DevRatio =  (float)devFmt.nWidth / (float)devFmt.nHeight;
	DlgRatio = (float)pDlg->GetSize().cx / (float)pDlg->GetSize().cy;
	if(devFmt.nWidth >= devFmt.nHeight && pDlg->GetSize().cx >=pDlg->GetSize().cy && fabs(DevRatio - DlgRatio) > 0.05)
	{
		bFit = false;
		int x, y;
		if(DevRatio > DlgRatio)
		{
			pDlg->SetSize(devFmt.nHeight * DlgRatio, devFmt.nHeight);				
			x = (devFmt.nWidth - devFmt.nHeight * DlgRatio) / 2;
			y = 0;
			pDlg->SetPosEx(x, y);
		}
		else
		{
			pDlg->SetSize(devFmt.nWidth, devFmt.nWidth / DlgRatio);
			x = 0;
			y = (devFmt.nHeight - devFmt.nWidth / DlgRatio) / 2;
			pDlg->SetPosEx(x, y);
		}
	}

	PAUIOBJECTLISTELEMENT pElement = pDlg->GetFirstControl();
	POINT ptdlg = pDlg->GetPos();
	pDlg->GetDlgItem("Txt_Tips")->SetText(m_strHint);
	
	if(bFit)
	{	
		while(pElement)
		{
			SIZE sz = pElement->pThis->GetSize();
			POINT pt = pElement->pThis->GetPos(true);
			pt.x += sz.cx / 2;
			pt.y += sz.cy / 2;
			if( fScaleH > fScaleV )
			{
				sz.cx = int(sz.cx * fScaleH / fScaleV);
				pt.x = int(pt.x * fScaleH / fScaleV);
				pElement->pThis->SetSize(sz.cx, sz.cy);
				pElement->pThis->SetPos(pt.x - sz.cx / 2 - ptdlg.x, pt.y - sz.cy / 2);
			}
			else
			{
				sz.cy = int(sz.cy * fScaleV / fScaleH);
				pt.y = int(pt.y * fScaleV / fScaleH);
				pElement->pThis->SetSize(sz.cx, sz.cy);
				pElement->pThis->SetPos(pt.x - sz.cx / 2, pt.y - sz.cy / 2 - ptdlg.y);
			}
			pElement = pDlg->GetNextControl(pElement);
		}
	}
	pDlg->Show(true, true, true);
//		luaevent_Loading_SetWorld(idInstance);

	if (iLoadID == LOAD_LOGIN)
	{
		SetProgressRange(0, 120);
	}
	else if (iLoadID == LOAD_ENTERGAME)
	{
		SetProgressRange(0, 200);
	}
	else
		SetProgressRange(0, 80);

	return true;
}

//	Release object
void CECLoadProgress::Release()
{
	A3DRELEASE(m_pBackPic);
	A3DRELEASE(m_pBarPic);
	A3DRELEASE(m_pLogoPic);
	A3DRELEASE(m_pBarTopPic);
	A3DRELEASE(m_pLoadingUIMan);
}

//	Advances the current position
int CECLoadProgress::OffsetProgressPos(int iOffset)
{
	m_iPos += iOffset;
	a_Clamp(m_iPos, m_iMin, m_iMax);
	if( m_pLoadingUIMan )
	{
		PAUIDIALOG pDlg = m_pLoadingUIMan->GetDialog("Win_Loading");
		if (pDlg)
		{
			PAUIPROGRESS pImg = (PAUIPROGRESS)pDlg->GetDlgItem("Prgs_Loading");
			if (pImg)
			{
				pImg->SetProgress(m_iPos);
			}
		}
	}
	return m_iPos;
}

//	Sets the current position 
void CECLoadProgress::SetProgressPos(int iPos)
{
	m_iPos = iPos;
	a_Clamp(m_iPos, m_iMin, m_iMax);
	if( m_pLoadingUIMan )
	{
		PAUIDIALOG pDlg = m_pLoadingUIMan->GetDialog("Win_Loading");
		if (pDlg)
		{
			PAUIPROGRESS pImg = (PAUIPROGRESS)pDlg->GetDlgItem("Prgs_Loading");
			if (pImg)
			{
				pImg->SetProgress(m_iPos);
			}
		}
	}
}

//	Sets the minimum and maximum ranges
void CECLoadProgress::SetProgressRange(int iMin, int iMax)
{
	ASSERT(iMin < iMax);
	m_iMin = iMin;
	m_iMax = iMax;
	a_Clamp(m_iPos, m_iMin, m_iMax);
	if( m_pLoadingUIMan )
	{
		PAUIDIALOG pDlg = m_pLoadingUIMan->GetDialog("Win_Loading");
		if (pDlg)
		{
			PAUIPROGRESS pImg = (PAUIPROGRESS)pDlg->GetDlgItem("Prgs_Loading");
			if (pImg)
			{
				pImg->SetRangeMax(iMax - iMin);
				pImg->SetProgress(m_iPos);
			}
		}
	}
}

//	Advances the current position by the step increment
int CECLoadProgress::StepProgress()
{
	m_iPos += m_iStep;
	a_Clamp(m_iPos, m_iMin, m_iMax);
	if( m_pLoadingUIMan )
	{
		PAUIDIALOG pDlg = m_pLoadingUIMan->GetDialog("Win_Loading");
		if (pDlg)
		{
			PAUIPROGRESS pImg = (PAUIPROGRESS)pDlg->GetDlgItem("Prgs_Loading");
			if (pImg)
			{
				pImg->SetProgress(m_iPos);
			}
		}
	}
	return m_iPos;
}

//	Render routine
bool CECLoadProgress::Render()
{
	A3DDevice* pA3DDevice = g_pGame->GetA3DDevice();
	A3DVIEWPORTPARAM* pvp = g_pGame->GetViewport()->GetA3DViewport()->GetParam();
	
	pA3DDevice->BeginRender();
	A3DDEVFMT devFmt = g_pGame->GetA3DDevice()->GetDevFormat();
	A3DVIEWPORTPARAM viewParam;
	viewParam.X = 0;
	viewParam.Y = 0;
	viewParam.Width  = devFmt.nWidth;
	viewParam.Height = devFmt.nHeight;
	viewParam.MinZ = 0.0f;
	viewParam.MaxZ = 1.0f;
	g_pGame->GetA3DDevice()->SetViewport(&viewParam);
	g_pGame->GetA3DDevice()->Clear(D3DCLEAR_TARGET, 0x00000000, 1.0f, 0);
	
 	if( m_pLoadingUIMan && m_pLoadingUIMan->GetDialog("Win_Loading") )
	{
 //       m_pLoadingUIMan->GetDialog("Win_Loading")->SetScale(1.0f/pGameUI->GetCustomUIScale()*LOADINGUI_RATIO);
 		m_pLoadingUIMan->GetDialog("Win_Loading")->Render();
	}
 	else
 	{
		if (m_pBackPic)
		{
			m_pBackPic->DrawToBack(0, 0);
			if (m_pBarPic)
			{
				float fLen = m_rcBar.Width() * (float)(m_iPos - m_iMin) / (m_iMax - m_iMin);
				if (fLen > (float)m_rcBar.Width())
					fLen = (float)m_rcBar.Width();
				
				m_pBarPic->SetPosition(m_rcBar.left, m_rcBar.top);
				m_pBarPic->SetScaleX(fLen / m_pBarPic->GetWidth());
				m_pBarPic->SetScaleY((float)m_rcBar.Height() / m_pBarPic->GetHeight());
				
				m_pBarPic->DrawToBack();
			}
			if (m_pLogoPic)
				m_pLogoPic->DrawToBack();
			
			if (m_pBarTopPic)
				m_pBarTopPic->DrawToBack();
			
			if( m_pFont )
			{
				
				m_pFont->TextOut(m_xHint, m_yHint, m_strHint, 0xFFE8CE17);
				m_pFont->Flush();
			}
		}
	}
	
	pA3DDevice->EndRender();
	pA3DDevice->Present();

	return true;
}

