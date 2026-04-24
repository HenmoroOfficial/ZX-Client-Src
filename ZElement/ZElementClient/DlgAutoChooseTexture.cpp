/*
 * FILE: DlgAutoChooseTexture.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Liyi, 2005/12/5
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */
#include "DlgAutoChooseTexture.h"
#include "EC_HomeDlgsMgr.h"
#include "DlgAutoTerrain.h"
#include "DlgAutoMountain.h"
#include "DlgAutoWater.h"
#include "DlgAutoRoad.h"
#include "DlgAutoLocalTexture.h"

#include "aui/AUILabel.h"
#include "aui/AUIListBox.h"
#include "aui/AUIImagePicture.h"

#include "A2DSprite.h"
#include "A3DEngine.h"
#include "A3DViewport.h"
#include "A3DSurfaceMan.h"

#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_World.h"
#include "AutoScene.h"
#include "AutoTerrain.h"

#include "WindowsX.h"

AUI_BEGIN_COMMAND_MAP(CDlgAutoChooseTexture, CDlgHomeBase)

AUI_ON_COMMAND("MoveSlider", OnCommandMoveSlider)
AUI_ON_COMMAND("OK", OnCommandOK)
AUI_ON_COMMAND("Cancel", OnCommandCancel)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgAutoChooseTexture, CDlgHomeBase)

AUI_ON_EVENT("ListBox_MainTexture", WM_LBUTTONUP, OnLButtonUpListBoxMainTexture)
AUI_ON_EVENT("ListBox_PatchTexture", WM_LBUTTONUP, OnLButtonUpListBoxPatchTexture)

AUI_END_EVENT_MAP()

CDlgAutoChooseTexture::CDlgAutoChooseTexture()
{
	m_pSliderTextureRatio = NULL;
	m_pSliderMainTexture = NULL;
	m_pSliderPatchTexture = NULL;
	m_pSliderPatchDivision = NULL;
	m_pSliderTextureSeed = NULL;
	m_pSliderMainTextureU = NULL;
	m_pSliderMainTextureV = NULL;
	m_pSliderPatchTextureU = NULL;
	m_pSliderPatchTextureV = NULL;
	
	m_pLabelTextureRatio = NULL;
	m_pLabelMainTexture = NULL;
	m_pLabelPatchTexture = NULL;
	m_pLabelPatchDivision = NULL;
	m_pLabelTextureSeed = NULL;
	m_pLabelMainTextureU = NULL;
	m_pLabelMainTextureV = NULL;
	m_pLabelPatchTextureU = NULL;
	m_pLabelPatchTextureV = NULL;

	m_nTextureRatio = 0;
	m_nMainTexture = 0;
	m_nPatchTexture = 0;
	m_nPatchDivision = 0;
	m_nTextureSeed = 0;
	m_nMainTextureU = 0;
	m_nMainTextureV = 0;
	m_nPatchTextureU = 0;
	m_nPatchTextureV = 0;

	m_pListBoxPatchTexture = NULL;
	m_pListBoxMainTexture = NULL;

	m_nMainTexIdx = 0;
	m_nPatchTexIdx = 0;

	m_pTexInfo = NULL;
	m_nNumTexInfos = 0;

	m_pImagePicMainTexture = NULL;
	m_pImagePicPatchTexture = NULL;
	
	m_pA2DSpriteMainTexture = new A2DSprite;
	m_pA2DSpritePatchTexture = new A2DSprite;

	m_nTexInitParam = CTIP_NULL;
}

CDlgAutoChooseTexture::~CDlgAutoChooseTexture()
{
	

	delete m_pA2DSpriteMainTexture;
	delete m_pA2DSpritePatchTexture;
}

bool CDlgAutoChooseTexture::OnInitContext()
{
	ClearListBox();

	if( !InitResource())
		return false;

	SetSliderBound();
	LoadDefault();
	UpdateData(false);
	SetLabelsText();

	FillTextureParam(0, 0);


	return true;
}

void CDlgAutoChooseTexture::OnShowDialog()
{
	CDlgHomeBase::OnShowDialog();
	SetCanMove(false);
}

void CDlgAutoChooseTexture::DoDataExchange(bool bSave)
{
	DDX_Control("Slider_TextureRatio", m_pSliderTextureRatio);
	DDX_Control("Slider_MainTexture", m_pSliderMainTexture);
	DDX_Control("Slider_PatchTexture", m_pSliderPatchTexture);
	DDX_Control("Slider_PatchDivision", m_pSliderPatchDivision);
	DDX_Control("Slider_TextureSeed", m_pSliderTextureSeed);
	DDX_Control("Slider_MainTextureU", m_pSliderMainTextureU);
	DDX_Control("Slider_MainTextureV", m_pSliderMainTextureV);
	DDX_Control("Slider_PatchTextureU", m_pSliderPatchTextureU);
	DDX_Control("Slider_PatchTextureV", m_pSliderPatchTextureV);

	DDX_Control("Label_TextureRatio", m_pLabelTextureRatio);
	DDX_Control("Label_MainTexture", m_pLabelMainTexture);
	DDX_Control("Label_PatchTexture", m_pLabelPatchTexture);
	DDX_Control("Label_PatchDivision", m_pLabelPatchDivision);
	DDX_Control("Label_TextureSeed", m_pLabelTextureSeed);
	DDX_Control("Label_MainTextureU", m_pLabelMainTextureU);
	DDX_Control("Label_MainTextureV", m_pLabelMainTextureV);
	DDX_Control("Label_PatchTextureU", m_pLabelPatchTextureU);
	DDX_Control("Label_PatchTextureV", m_pLabelPatchTextureV);

	DDX_Control("ListBox_MainTexture", m_pListBoxMainTexture);
	DDX_Control("ListBox_PatchTexture", m_pListBoxPatchTexture);

	DDX_ListBox(bSave, "ListBox_MainTexture", m_nMainTexIdx);
	DDX_ListBox(bSave, "ListBox_PatchTexture", m_nPatchTexIdx);

	DDX_Control("ImagePic_MainTexture", m_pImagePicMainTexture);
	DDX_Control("ImagePic_PatchTexture", m_pImagePicPatchTexture);
	
	DDX_Slider(bSave, "Slider_TextureRatio", m_nTextureRatio);
	DDX_Slider(bSave, "Slider_MainTexture", m_nMainTexture);
	DDX_Slider(bSave, "Slider_PatchTexture", m_nPatchTexture);
	DDX_Slider(bSave, "Slider_PatchDivision", m_nPatchDivision);
	DDX_Slider(bSave, "Slider_TextureSeed", m_nTextureSeed);
	DDX_Slider(bSave, "Slider_MainTextureU", m_nMainTextureU);
	DDX_Slider(bSave, "Slider_MainTextureV", m_nMainTextureV);
	DDX_Slider(bSave, "Slider_PatchTextureU", m_nPatchTextureU);
	DDX_Slider(bSave, "Slider_PatchTextureV", m_nPatchTextureV);
}

void CDlgAutoChooseTexture::SetSliderBound()
{
	m_pSliderTextureRatio->SetTotal(200, 0);
	m_pSliderMainTexture->SetTotal(100, 0);
	m_pSliderPatchTexture->SetTotal(400, 0);
	m_pSliderPatchDivision->SetTotal(101, 1);
	m_pSliderTextureSeed->SetTotal(100, 0);
	
	m_pSliderMainTextureU->SetTotal(50, 5);
	m_pSliderMainTextureV->SetTotal(50, 5);
	m_pSliderPatchTextureU->SetTotal(50, 5);
	m_pSliderPatchTextureV->SetTotal(50, 5);
}

void CDlgAutoChooseTexture::LoadDefault()
{
	m_nTextureRatio = 120;
	m_nMainTexture = 40;
	m_nPatchTexture = 120;
	m_nPatchDivision = 30;
	m_nTextureSeed = 1;

	m_nMainTextureU = 10;
	m_nMainTextureV = 10;
	m_nPatchTextureU = 10;
	m_nPatchTextureV = 10;
}

void CDlgAutoChooseTexture::SetLabelsText()
{
	ACHAR szName[100];
	swprintf(szName, _AL("%-4.2f"), (float)((m_nTextureRatio - 100)* 0.01f));
	m_pLabelTextureRatio->SetText(szName);

	swprintf(szName, _AL("%-4.2f"), (float)(m_nMainTexture * 0.01f));
	m_pLabelMainTexture->SetText(szName);

	swprintf(szName, _AL("%-4.2f"), (float)(m_nPatchTexture * 0.01f));
	m_pLabelPatchTexture->SetText(szName);

	swprintf(szName, _AL("%d"), m_nPatchDivision);

	m_pLabelPatchDivision->SetText(szName);

	swprintf(szName, _AL("%d"), m_nTextureSeed);
	m_pLabelTextureSeed->SetText(szName);

	swprintf(szName, _AL("%d"), m_nMainTextureU);
	m_pLabelMainTextureU->SetText(szName);

	swprintf(szName, _AL("%d"), m_nMainTextureV);
	m_pLabelMainTextureV->SetText(szName);

	swprintf(szName, _AL("%d"), m_nPatchTextureU);
	m_pLabelPatchTextureU->SetText(szName);

	swprintf(szName, _AL("%d"), m_nPatchTextureV);
	m_pLabelPatchTextureV->SetText(szName);

}

void CDlgAutoChooseTexture::OnCommandMoveSlider(const char * szCommand)
{
	UpdateData(true);
	SetLabelsText();
}

void CDlgAutoChooseTexture::OnCommandOK(const char* szCommand)
{
	UpdateData(true);

	int nMainTexSel = m_pListBoxMainTexture->GetCurSel();
	if( nMainTexSel < 0 || nMainTexSel > m_pListBoxMainTexture->GetCount())
	{
		this->Show(false);
		return;
	}

	int nPatchTexSel = m_pListBoxPatchTexture->GetCurSel();
	if( nPatchTexSel < 0 || nPatchTexSel > m_pListBoxPatchTexture->GetCount())
	{
		this->Show(false);
		return;
	}

	FillTextureParam(nMainTexSel, nPatchTexSel);
	
	
	switch(m_nTexInitParam)
	{
	case CTIP_TERRAIN:
		{
			TextureParamInfo_t* pTexParamInfo = GetHomeDlgsMgr()->GetAutoTerrainDlg()->GetTextureParamInfo();
			*pTexParamInfo = m_texParamInfo;
			if(!GetHomeDlgsMgr()->SaveTextureParamInfo(m_texParamInfo, TT_MAIN, TT_MAIN_PATCH))
			{
				a_LogOutput(1, "CDlgAutoChooseTexture::OnCommandOK(), Failed to Save Terrain TextureParamInfo!");
				AutoHomeAssert();
			}
		}
		break;
	case CTIP_HILL:
		{
			TextureParamInfo_t* pTexParamInfo = GetHomeDlgsMgr()->GetAutoMountainDlg()->GetTextureParamInfo();
			*pTexParamInfo = m_texParamInfo;
			if(!GetHomeDlgsMgr()->SaveTextureParamInfo(m_texParamInfo, TT_BRIDGE, TT_BRIDGE_PATCH))
			{
				a_LogOutput(1, "CDlgAutoChooseTexture::OnCommandOK(), Failed to Save Mountain TextureParamInfo!");
				AutoHomeAssert();
			}
		}
		break;
	case CTIP_WATER:
		{
			TextureParamInfo_t* pTexParamInfo = GetHomeDlgsMgr()->GetAutoWaterDlg()->GetTextureParamInfo();
			*pTexParamInfo = m_texParamInfo;
			if(!GetHomeDlgsMgr()->SaveTextureParamInfo(m_texParamInfo,TT_RIVERBED, TT_RIVERSAND))
			{
				a_LogOutput(1, "CDlgAutoChooseTexture::OnCommandOK(), Failed to Save water TextureParamInfo!");
				AutoHomeAssert();
			}
		}
		break;
	case CTIP_ROAD:
		{
			TextureParamInfo_t* pTexParamInfo = GetHomeDlgsMgr()->GetAutoRoadDlg()->GetTextureParamInfo();
			*pTexParamInfo = m_texParamInfo;
			if(!GetHomeDlgsMgr()->SaveTextureParamInfo(m_texParamInfo,TT_ROADBED, TT_ROAD))
			{
				a_LogOutput(1, "CDlgAutoChooseTexture::OnCommandOK(), Failed to Save Road TextureParamInfo!");
				AutoHomeAssert();
			}
		}
		break;
	case CTIP_LOCALTEX:
		{
			TextureParamInfo_t* pTexParamInfo = GetHomeDlgsMgr()->GetAutoLocalTextureDlg()->GetTextureParamInfo();
			*pTexParamInfo = m_texParamInfo;
			if(!GetHomeDlgsMgr()->SaveTextureParamInfo(m_texParamInfo,TT_PART, TT_PART_PATCH))
			{
				a_LogOutput(1, "CDlgAutoChooseTexture::OnCommandOK(), Failed to Save LocalTexture TextureParamInfo!");
				AutoHomeAssert();
			}
		}
		break;
	default:
		break;
	};
	

	this->Show(false);
	
}

void CDlgAutoChooseTexture::OnCommandCancel(const char* szCommand)
{
	this->Show(false);
}

bool CDlgAutoChooseTexture::InitResource()
{
	CAutoTexture* pAutoTexture = GetAutoScene()->GetAutoTerrain()->GetTexture();

	m_pTexInfo = pAutoTexture->GetTextureInfos();
	if( !m_pTexInfo)
	{
		a_LogOutput(1, "CDlgAutoChooseTexture::InitResource(), Can not get CAutoTexture::TEXTURETYPEINFO !");
		return false;
	}

	m_nNumTexInfos = pAutoTexture->GetNumTextureInfos();

	if( m_nNumTexInfos < 0)
	{
		a_LogOutput(1, "CDlgAutoChooseTexture::InitResource(), Number of TextureInfos Less than 0 !");
		return false;
	}

	
	
	for( int i = 0; i < m_nNumTexInfos; ++i)
	{
		m_pListBoxMainTexture->InsertString(i, AS2AC(m_pTexInfo[i].strName));
		m_pListBoxMainTexture->SetItemData(i, m_pTexInfo->dwID);
		
		m_pListBoxPatchTexture->InsertString(i, AS2AC(m_pTexInfo[i].strName));
		m_pListBoxPatchTexture->SetItemData(i, m_pTexInfo->dwID);
	
	}

	g_pGame->GetA3DEngine()->GetA3DSurfaceMan()->SetFolderName(".\\");

	A3DRECT rect; 
	rect.left = rect.top = 0;
	rect.right = rect.bottom = 64;

	if(!m_pA2DSpriteMainTexture->Init(g_pGame->GetA3DDevice(), m_pTexInfo[0].strTexture, 64, 64, 0, 1, &rect))
	{
		a_LogOutput(1, "CDlgAutoChooseTexture::InitResource(), Can not Init A2DSprite!");

		AutoHomeAssert();
		return false;
		
	}
	m_pA2DSpriteMainTexture->SetSrcBlend(A3DBLEND_ONE);
	m_pA2DSpriteMainTexture->SetDestBlend(A3DBLEND_ZERO);
	
	if(!m_pA2DSpritePatchTexture->Init(g_pGame->GetA3DDevice(), m_pTexInfo[0].strTexture, 64, 64, 0, 1, &rect))
	{
		a_LogOutput(1, "CDlgAutoChooseTexture::InitResource(), Can not Init A2DSprite!");

		AutoHomeAssert();
		return false;
		
	}
	m_pA2DSpritePatchTexture->SetSrcBlend(A3DBLEND_ONE);
	m_pA2DSpritePatchTexture->SetDestBlend(A3DBLEND_ZERO);
		


	g_pGame->GetA3DEngine()->GetA3DSurfaceMan()->SetFolderName("surfaces");

	m_pImagePicMainTexture->SetCover(m_pA2DSpriteMainTexture, 0);
	m_pImagePicPatchTexture->SetCover(m_pA2DSpritePatchTexture, 0);
	

	
	return true;
}

void CDlgAutoChooseTexture::ClearListBox()
{
	if(m_pListBoxMainTexture)
		m_pListBoxMainTexture->ResetContent();
	if( m_pListBoxPatchTexture)
		m_pListBoxPatchTexture->ResetContent();

}

void CDlgAutoChooseTexture::OnLButtonUpListBoxMainTexture(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	//判断是否点中列表框中的选项
	POINT ptPos = pObj->GetPos();

	A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
	int nMouseX = GET_X_LPARAM(lParam) - ptPos.x - p->X;
	int nMouseY = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;

	if(m_pListBoxMainTexture->GetHitArea(nMouseX, nMouseY)!= AUILISTBOX_RECT_TEXT)
		return;

	int nCurSel = m_pListBoxMainTexture->GetCurSel();
	if(nCurSel < 0 || nCurSel > m_pListBoxMainTexture->GetCount())
		return;

	g_pGame->GetA3DEngine()->GetA3DSurfaceMan()->SetFolderName(".\\");

	if(!m_pA2DSpriteMainTexture->Init(g_pGame->GetA3DDevice(), m_pTexInfo[nCurSel].strTexture, 0))
	{
		a_LogOutput(1, "CDlgAutoChooseTexture::InitResource(), Can not Init A2DSprite!");

		AutoHomeAssert();
		return ;
		
	}
	g_pGame->GetA3DEngine()->GetA3DSurfaceMan()->SetFolderName("surfaces");

	m_pImagePicMainTexture->SetCover(m_pA2DSpriteMainTexture, 0);
		return ;
}

void CDlgAutoChooseTexture::OnLButtonUpListBoxPatchTexture(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	//判断是否点中列表框中的选项
	POINT ptPos = pObj->GetPos();

	A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
	int nMouseX = GET_X_LPARAM(lParam) - ptPos.x - p->X;
	int nMouseY = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;

	if(m_pListBoxPatchTexture->GetHitArea(nMouseX, nMouseY)!= AUILISTBOX_RECT_TEXT)
		return;

	int nCurSel = m_pListBoxPatchTexture->GetCurSel();
	if(nCurSel < 0 || nCurSel > m_pListBoxPatchTexture->GetCount())
		return;

	g_pGame->GetA3DEngine()->GetA3DSurfaceMan()->SetFolderName(".\\");
	if(!m_pA2DSpritePatchTexture->Init(g_pGame->GetA3DDevice(), m_pTexInfo[nCurSel].strTexture, 0))
	{
		a_LogOutput(1, "CDlgAutoChooseTexture::InitResource(), Can not Init A2DSprite!");

		AutoHomeAssert();
		return ;
		
	}
	g_pGame->GetA3DEngine()->GetA3DSurfaceMan()->SetFolderName("surfaces");

	m_pImagePicPatchTexture->SetCover(m_pA2DSpritePatchTexture, 0);

	return ;
}

void CDlgAutoChooseTexture::FillTextureParam(int nMainTexSel, int nPatchTexSel)
{
	m_texParamInfo.fTextureRatio = (float)((m_nTextureRatio - 100)* 0.01f);
	m_texParamInfo.fMainTexture = (float)(m_nMainTexture * 0.01f);
	m_texParamInfo.fPatchTexture = (float)(m_nPatchTexture * 0.01f);
	m_texParamInfo.fPatchDivision =(float)m_nPatchDivision;
	m_texParamInfo.dwTextureSeed = m_nTextureSeed;

	m_texParamInfo.fMainTextureU = (float)m_nMainTextureU;
	m_texParamInfo.fMainTextureV = (float)m_nMainTextureV;
	m_texParamInfo.fPatchTextureU = (float)m_nPatchTextureU;
	m_texParamInfo.fPatchTextureV = (float)m_nPatchTextureV;

	m_texParamInfo.strMainTextureFile = m_pTexInfo[nMainTexSel].strTexture;
	m_texParamInfo.strPatchTextureFile = m_pTexInfo[nPatchTexSel].strTexture;
	
	m_texParamInfo.nMainTexID = m_pTexInfo[nMainTexSel].dwID;
	m_texParamInfo.nPatchTexID = m_pTexInfo[nPatchTexSel].dwID;

	m_texParamInfo.nMainTexIdx = nMainTexSel;
	m_texParamInfo.nPatchTexIdx = nPatchTexSel;
	
}

bool CDlgAutoChooseTexture::InitTextureParamInfo(const TextureParamInfo_t& param, ChooseTexInitParam_e texInitParam)
{
	m_texParamInfo = param;
	
	m_nTextureRatio = (int)( m_texParamInfo.fTextureRatio * 100.0f + 100.0f + 0.5f);
	m_nMainTexture = (int)( m_texParamInfo.fMainTexture * 100.0f + 0.5f);
	m_nPatchTexture = (int)( m_texParamInfo.fPatchTexture * 100.0f + 0.5f);
	m_nPatchDivision = (int)(m_texParamInfo.fPatchDivision + 0.5f);
	m_nTextureSeed = m_texParamInfo.dwTextureSeed;

	m_nMainTextureU = (int)(m_texParamInfo.fMainTextureU + 0.5f);
	m_nMainTextureV = (int)(m_texParamInfo.fMainTextureV + 0.5f);
	m_nPatchTextureU = (int)(m_texParamInfo.fPatchTextureU + 0.5f);
	m_nPatchTextureV = (int)(m_texParamInfo.fPatchTextureV + 0.5f);

	

	if( m_texParamInfo.nMainTexIdx >= 0 && m_texParamInfo.nMainTexIdx < m_pListBoxMainTexture->GetCount())
	{
		//m_pListBoxMainTexture->SetSel(m_texParamInfo.nMainTexIdx, true);
		m_nMainTexIdx = m_texParamInfo.nMainTexIdx;
	}
	else
	{
		return false;
	}

	if( m_texParamInfo.nPatchTexIdx >= 0 && m_texParamInfo.nPatchTexIdx < m_pListBoxPatchTexture->GetCount())
	{
		//m_pListBoxPatchTexture->SetSel(m_texParamInfo.nPatchTexIdx, true);
		m_nPatchTexIdx = m_texParamInfo.nPatchTexIdx;
	}
	else
	{
		return false;
	}

	UpdateData(false);
	SetLabelsText();

	g_pGame->GetA3DEngine()->GetA3DSurfaceMan()->SetFolderName(".\\");

	if(!m_pA2DSpriteMainTexture->Init(g_pGame->GetA3DDevice(), m_pTexInfo[m_nMainTexIdx].strTexture, 0))
	{
		a_LogOutput(1, "CDlgAutoChooseTexture::InitTextureParamInfo(), Can not Init A2DSprite!");

		AutoHomeAssert();
		return false;
		
	}

	if(!m_pA2DSpritePatchTexture->Init(g_pGame->GetA3DDevice(), m_pTexInfo[m_nPatchTexIdx].strTexture, 0))
	{
		a_LogOutput(1, "CDlgAutoChooseTexture::InitTextureParamInfo(), Can not Init A2DSprite!");

		AutoHomeAssert();
		return false;
		
	}

	g_pGame->GetA3DEngine()->GetA3DSurfaceMan()->SetFolderName("surfaces");

	m_pImagePicMainTexture->SetCover(m_pA2DSpriteMainTexture, 0);
	m_pImagePicPatchTexture->SetCover(m_pA2DSpritePatchTexture, 0);
	
	if(texInitParam == CTIP_NULL)
	{
		return false;
	}
	m_nTexInitParam = texInitParam;

	return true;
	
}