/*
 * FILE: DlgAutoLocalTexture.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Liyi, 2005/12/2
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */
#include "DlgAutoLocalTexture.h"
#include "EC_HomeDlgsMgr.h"
#include "DlgAutoChooseTexture.h"

#include "aui/AUILabel.h"
#include "aui/AUISlider.h"
#include "aui/AUIManager.h"

#include "AutoTerrainCommon.h"

#include "A3DEngine.h"
#include "A3DViewport.h"

#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_World.h"
#include "AutoScene.h"
#include "AutoTerrain.h"

#include "windowsx.h"

#include "A3DMacros.h"

AUI_BEGIN_COMMAND_MAP(CDlgAutoLocalTexture, CDlgHomeBase)

AUI_ON_COMMAND("MoveSlider", OnCommandMoveSlider)
AUI_ON_COMMAND("OpenChooseTexture", OnCommandOpenChooseTexDlg)
AUI_ON_COMMAND("EditLocalTex", OnCommandEditLocalTex)
AUI_ON_COMMAND("CreateLocalTex", OnCommandCreateLocalTex)
AUI_ON_COMMAND("ClearLocalTex", OnCommandClearLocalTex)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgAutoLocalTexture, CDlgHomeBase)

AUI_END_EVENT_MAP()

CDlgAutoLocalTexture::CDlgAutoLocalTexture()
{
	m_pSliderLocalTexBorderScope = NULL;
	m_pSliderSeedLocalTexture = NULL;

	m_pLabelLocalTexBorderScope = NULL;
	m_pLabelSeedLocalTexture = NULL;

	m_nLocalTexBorderScope = 0;
	m_nSeedLocalTexture = 0;

	m_nMainTexture = 0;

	m_bChangeTexArea = true;
}

CDlgAutoLocalTexture::~CDlgAutoLocalTexture()
{
}

bool CDlgAutoLocalTexture::OnInitContext()
{
	SetSliderBound();
	LoadDefault();
	UpdateData(false);
	SetLabelsText();

	return true;
}

void CDlgAutoLocalTexture::OnShowDialog()
{
	CDlgHomeBase::OnShowDialog();

	SetCanMove(false);
}

AUIStillImageButton * CDlgAutoLocalTexture::GetSwithButton()
{
	return GetHomeDlgsMgr()->m_pBtnChooseLocalTexture;
}

void CDlgAutoLocalTexture::DoDataExchange(bool bSave)
{
	DDX_Control("Slider_LocalTexBorderScope", m_pSliderLocalTexBorderScope);
	DDX_Control("Slider_SeedLocalTexture", m_pSliderSeedLocalTexture);

	DDX_Control("Label_LocalTexBorderScope", m_pLabelLocalTexBorderScope);
	DDX_Control("Label_SeedLocalTexture", m_pLabelSeedLocalTexture);

	DDX_Slider(bSave, "Slider_LocalTexBorderScope", m_nLocalTexBorderScope);
	DDX_Slider(bSave, "Slider_SeedLocalTexture", m_nSeedLocalTexture);

	DDX_RadioButton(bSave, 1, m_nMainTexture);
}

void CDlgAutoLocalTexture::SetSliderBound()
{
	m_pSliderLocalTexBorderScope->SetTotal(100, 0);
	m_pSliderSeedLocalTexture->SetTotal(100, 0);
}

void CDlgAutoLocalTexture::LoadDefault()
{
	m_nLocalTexBorderScope = 50;
	m_nSeedLocalTexture = 1;
	m_nMainTexture = 2;

	m_texParamInfo.fTextureRatio	= 0.2f;
	m_texParamInfo.fMainTexture		= 0.4f;
	m_texParamInfo.fPatchTexture	= 1.2f;
	m_texParamInfo.fPatchDivision	= 30.0f;
	m_texParamInfo.dwTextureSeed	= 1;

	
	GetHomeDlgsMgr()->LoadTextureParamInfo(m_texParamInfo, TT_PART, TT_PART_PATCH);
}

void CDlgAutoLocalTexture::SetLabelsText()
{
	ACHAR szName[100];
	swprintf(szName, _AL("%-4.2f"), (float)(m_nLocalTexBorderScope * 0.01f));
	m_pLabelLocalTexBorderScope->SetText(szName);

	swprintf(szName, _AL("%d"), m_nSeedLocalTexture);
	m_pLabelSeedLocalTexture->SetText(szName);
}

void CDlgAutoLocalTexture::OnCommandMoveSlider(const char * szCommand)
{
	UpdateData(true);
	SetLabelsText();
}

void CDlgAutoLocalTexture::OnCommandOpenChooseTexDlg(const char* szCommand)
{
	CDlgAutoChooseTexture* pDlg = GetHomeDlgsMgr()->GetAutoChooseTextureDlg();
	pDlg->InitTextureParamInfo(m_texParamInfo, CTIP_LOCALTEX);
	pDlg->Show(true);
}
void CDlgAutoLocalTexture::OnCommandEditLocalTex(const char* szCommand)
{
	CAutoScene* pScene = GetAutoScene();
	pScene->SetOperationType(OT_TEXTURE_POLY);
}
void CDlgAutoLocalTexture::OnCommandCreateLocalTex(const char* szCommand)
{
	UpdateData(true);

	if (GetOperationType() != OT_TEXTURE_POLY)
	{
		GetAUIManager()->MessageBox("错误", _AL("当前不处于局部纹理操作!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}

	CAutoScene* pScene = GetAutoScene();
	CAutoPartTexture* pPartTexture = pScene->GetAutoPartTexture();
	if (NULL == pPartTexture)
		return;

	if (!pPartTexture->IsPolygonValid())
	{
		GetAUIManager()->MessageBox("错误", _AL("请选择封闭的多边形区域!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}

	if( !m_bChangeTexArea)
	{
		pPartTexture->StorePoints();
		pScene->UndoAction(TAT_TEXTURE_POLY);
		pPartTexture->RestorePoints();
	}
	
	if (!pPartTexture->CreatePartTexture(
		m_texParamInfo.fTextureRatio,
		m_texParamInfo.fMainTexture,
		m_texParamInfo.fPatchTexture,
		m_texParamInfo.fPatchDivision,
		m_nSeedLocalTexture,
		(float)(m_nLocalTexBorderScope * 0.01f),
		m_nMainTexture))
	{
		GetAUIManager()->MessageBox("错误", _AL("生成局部纹理失败!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}

	// Store action
	CAutoTerrainAction* pAction = pScene->GetTerrainAction();
	CClosedArea* pArea = pScene->GetAutoPartTexture()->GetArea();
	if (!pAction->AddActionPartTexture(
			m_texParamInfo.fTextureRatio,
			m_texParamInfo.fMainTexture, 
			m_texParamInfo.fPatchTexture,
			m_texParamInfo.fPatchDivision,
			m_nSeedLocalTexture,
			(float)(m_nLocalTexBorderScope * 0.01f),
			pArea->GetNumPoints(),
			pArea->GetFinalPoints(),
			m_nMainTexture))
	{
		GetAUIManager()->MessageBox("错误", _AL("保存操作失败!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}

	CAutoTexture* pTexture = pScene->GetAutoTerrain()->GetTexture();
	if (!pAction->AddActionPartTextureInfo(
			m_texParamInfo.fMainTextureU,
			m_texParamInfo.fMainTextureV,
			m_texParamInfo.fPatchTextureU,
			m_texParamInfo.fPatchTextureV,
			pTexture->GetTextureID(TT_PART),
			pTexture->GetTextureID(TT_PART_PATCH)))
	{
		GetAUIManager()->MessageBox("错误", _AL("保存操作失败!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}

	pScene->RecreateTerrainRender(false);

	m_bChangeTexArea = false;
}
void CDlgAutoLocalTexture::OnCommandClearLocalTex(const char* szCommand)
{
		CAutoScene* pScene = GetAutoScene();
	CAutoTerrain* pTerrain = pScene->GetAutoTerrain();
	CAutoTexture* pTexture = pTerrain->GetTexture();

	pScene->GetTerrainAction()->ClearAllPartTextureActions();

	// Save mask files
	pTexture->ResetPartTexture();

	pScene->RecreateTerrainRender(false);	
}

void CDlgAutoLocalTexture::OnLButtonDownScreen(WPARAM wParam, LPARAM lParam)
{
	APointI point;

	A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
	point.x = GET_X_LPARAM(lParam)  - p->X;
	point.y = GET_Y_LPARAM(lParam)  - p->Y;

	CAutoScene* pAutoScene = GetAutoScene();
	
	int nType = pAutoScene->GetOperationType();
	if( nType == OT_TEXTURE_POLY)
	{
		if( pAutoScene->GetAutoPartTexture())
		{
			pAutoScene->GetAutoPartTexture()->OnLButtonDown(NULL, point);
			m_bChangeTexArea = true;
		}
		return;
	}
	else
		return;

}
void CDlgAutoLocalTexture::OnRButtonUpScreen(WPARAM wParam, LPARAM lParam)
{
	APointI point;

	A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
	point.x = GET_X_LPARAM(lParam)  - p->X;
	point.y = GET_Y_LPARAM(lParam)  - p->Y;

	CAutoScene* pAutoScene = GetAutoScene();
	
	int nType = pAutoScene->GetOperationType();
	if( nType == OT_TEXTURE_POLY)
	{
		if( pAutoScene->GetAutoPartTexture())
		{
			pAutoScene->GetAutoPartTexture()->OnRButtonUp(NULL, point);
			m_bChangeTexArea = true;
		}
		return;
	}
	else
		return;
}

void CDlgAutoLocalTexture::OnRButtonDblClk(WPARAM wParam, LPARAM lParam)
{
	APointI point;

	A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
	point.x = GET_X_LPARAM(lParam)  - p->X;
	point.y = GET_Y_LPARAM(lParam)  - p->Y;

	CAutoScene* pAutoScene = GetAutoScene();
	
	int nType = pAutoScene->GetOperationType();
	if( nType == OT_TEXTURE_POLY)
	{
		if( pAutoScene->GetAutoPartTexture())
		{
			pAutoScene->GetAutoPartTexture()->OnRButtonDblClk(NULL, point);
			m_bChangeTexArea = true;
		}
		return;
	}
	else
		return;
}