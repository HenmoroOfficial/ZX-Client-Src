/*
 * FILE: EC_Decal.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/1/18
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_Decal.h"
#include "EC_Game.h"
#include "EC_ManDecal.h"
#include "EC_ImageRes.h"
#include "EC_GameRun.h"
//#include "EC_UIManager.h"
//#include "EC_GameUIMan.h"
#include "EC_World.h"

#include "A3DFont.h"
#include "A2DSprite.h"
#include "A2DSpriteItem.h"
//#include "AUI\\AUITextArea.h"
#include "A3DViewport.h"
#include "A3DCameraBase.h"
#include "A3DGFXEx.h"
#include "A3dGfxElement.h"

#ifdef BACK_VERSION
#include "EC_Viewport.h"
#else
#include "ECViewport.h"
#endif

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


///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECDecal
//	
///////////////////////////////////////////////////////////////////////////

//	Create a decal object according to class ID
CECDecal* CECDecal::CreateDecal(int iCID)
{
	CECDecal* pDecal = NULL;

	switch (iCID)
	{
	case CECDecal::DCID_DECAL:			pDecal = new CECDecal;			break;
	case CECDecal::DCID_ICONDECAL:		pDecal = new CECIconDecal;		break;
	case CECDecal::DCID_SPRITEDECAL:	pDecal = new CECSpriteDecal;	break;
	case CECDecal::DCID_FACEDECAL:		pDecal = new CECFaceDecal;		break;
	default:
		ASSERT(0);
		break;
	}

	return pDecal;
}

CECDecal::CECDecal()
{
	m_iCID	= DCID_DECAL;
	m_Color	= 0xffffffff;
	m_fBaseScale = 1.0f;
}

CECDecal::~CECDecal()
{
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIconDecal
//	
///////////////////////////////////////////////////////////////////////////

CECIconDecal::CECIconDecal()
{
	m_iCID			= DCID_ICONDECAL;
	m_fScaleX		= 1.0f;
	m_fScaleY		= 1.0f;
	m_iExtX			= 0;
	m_iExtY			= 0;
	m_bScreenPos	= false;
	m_sx			= 0;
	m_sy			= 0;
}

CECIconDecal::~CECIconDecal()
{
}

//	Initialize object
bool CECIconDecal::AddIcons(int iImageSlot, int iNumIcon, int* aIconIdx, A3DCOLOR col)
{
	CECImageRes* pImageRes = g_pGame->GetImageRes();

	for (int i=0; i < iNumIcon; i++)
	{
		int cx, cy;
		pImageRes->GetImageItemSize(iImageSlot, aIconIdx[i], &cx, &cy);

		ICON Icon;
		Icon.iImageSlot	= iImageSlot;
		Icon.iIcon		= aIconIdx[i];
		Icon.iExtX		= cx;
		Icon.dwCol		= col;
		m_aIcons.Add(Icon);

		m_iExtX += cx;
		if (cy > m_iExtY)
			m_iExtY = cy;
	}

	return true;
}

bool CECIconDecal::AddIcon(int iImageSlot, int iIconIdx, A3DCOLOR col)
{
	return AddIcons(iImageSlot, 1, &iIconIdx, col);
}

//	Initialize a number string
bool CECIconDecal::AddNumIcons(int iImageSlot, int iNumber, A3DCOLOR col, bool bShowMinus)
{
	if (iNumber < 0)
	{
		bShowMinus = true;
		iNumber = -iNumber;
	}

	char szNum[20];
	itoa(iNumber, szNum, 10);

	int aIndices[20];
	int i, iLen = strlen(szNum);

	if (bShowMinus)
	{
		aIndices[0] = 10;

		for (i = 0; i < iLen; i++)
			aIndices[i+1] = szNum[i] - '0';

		iLen++;
	}
	else
	{
		for (i = 0; i < iLen; i++)
			aIndices[i] = szNum[i] - '0';
	}

	return AddIcons(iImageSlot, iLen, aIndices, col);
}

//	Render routine
bool CECIconDecal::Render(CECViewport* pViewport)
{
	if (!m_aIcons.GetSize())
		return true;

	if (m_bScreenPos)
	{
		return Render(m_sx, m_sy, 0.0f, 1.0f);
	}
	else
	{
		//	Calculate decal center position on screen
		A3DViewport* pA3DView = pViewport->GetA3DViewport();
		A3DVIEWPORTPARAM* pViewPara = pA3DView->GetParam();

		A3DMATRIX4 matVPS = pA3DView->GetCamera()->GetVPTM() * pA3DView->GetViewScale();
		A3DVECTOR4 vScrPos = TransformToScreen(m_vPos, matVPS);

		if (vScrPos.z < pViewPara->MinZ || vScrPos.z > pViewPara->MaxZ)
			return true;

		int iExtX = int(m_iExtX * m_fScaleX * m_fBaseScale);
		int iExtY = int(m_iExtY * m_fScaleY * m_fBaseScale);

		int x = (int)(vScrPos.x - (iExtX >> 1));
		int y = (int)(vScrPos.y - (iExtY >> 1));
		return Render(x, y, vScrPos.z, vScrPos.w);
	}
}

bool CECIconDecal::Render(int x, int y, float z, float rhw)
{
	if (!m_aIcons.GetSize())
		return true;

	int rx = x;
	int cx, cy;

	CECImageRes* pImageRes = g_pGame->GetImageRes();

	for (int i=0; i < m_aIcons.GetSize(); i++)
	{
		const ICON& Icon = m_aIcons[i];
		DWORD dwCol = (Icon.dwCol & 0x00ffffff) | (m_Color & 0xff000000);
		pImageRes->GetImageItemSize(Icon.iImageSlot, Icon.iIcon, &cx, &cy);
		cx = int(cx * m_fScaleX * m_fBaseScale);
		cy = int(cy * m_fScaleY * m_fBaseScale);
		pImageRes->DrawImage(Icon.iImageSlot, rx, y, dwCol, Icon.iIcon, z, rhw, cx, cy);
		rx += cx;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECSpriteDecal
//	
///////////////////////////////////////////////////////////////////////////

CECSpriteDecal::CECSpriteDecal()
{
	m_pA2DSprite	= NULL;
	m_iIconIdx		= 0;
	m_fScaleX		= 1.0f;
	m_fScaleY		= 1.0f;
	m_ix			= 0;
	m_iy			= 0;
	m_fz			= 0.0f;
	m_rhw			= 1.0f;
	m_iExtX			= 0;
	m_iExtY			= 0;
}

CECSpriteDecal::~CECSpriteDecal()
{
}

//	Change sprite object and icon index
bool CECSpriteDecal::ChangeSpriteIcon(A2DSprite* pA2DSprite, int iIconIdx)
{
	m_pA2DSprite	= pA2DSprite;
	m_iIconIdx		= iIconIdx;

	//	Get decal extent
	A2DSpriteItem* pItem = m_pA2DSprite->GetItem(m_iIconIdx);
	A3DRECT Rect = pItem->GetRect();
	m_iExtX = Rect.right - Rect.left;
	m_iExtY = Rect.bottom - Rect.top;

	return true;
}

//	Render routine
bool CECSpriteDecal::Render(CECViewport* pViewport)
{
	if (!m_pA2DSprite)
		return true;

	//	Save image properties
	float sx = m_pA2DSprite->GetScaleX();
	float sy = m_pA2DSprite->GetScaleY();
//	DWORD dwCol = m_pA2DSprite->GetColor();
	float z = m_pA2DSprite->GetZPos();

	m_pA2DSprite->SetCurrentItem(m_iIconIdx);

	m_pA2DSprite->SetScaleX(m_fScaleX * m_fBaseScale);
	m_pA2DSprite->SetScaleY(m_fScaleY * m_fBaseScale);
//	m_pA2DSprite->SetColor(m_Color);
	m_pA2DSprite->SetZPos(m_fz, m_rhw);
	m_pA2DSprite->SetNeedStereo(true);
	m_pA2DSprite->DrawToInternalBuffer(m_ix, m_iy);
	m_pA2DSprite->SetNeedStereo(false);

	//	Restore image properties
	m_pA2DSprite->SetScaleX(sx);
	m_pA2DSprite->SetScaleY(sy);
//	m_pA2DSprite->SetColor(dwCol);
	m_pA2DSprite->SetZPos(z);

	return true;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECFaceDecal
//	
///////////////////////////////////////////////////////////////////////////

CECFaceDecal::CECFaceDecal() : CECSpriteDecal()
{
	m_iStartFrame	= 0;
	m_iNumFrame		= 0;
	m_iFrameCnt		= 0;
	m_iFace			= -1;

	m_AnimCnt.SetPeriod(100);
}

CECFaceDecal::~CECFaceDecal()
{
}

//	Initialize object
bool CECFaceDecal::Init(int nEmotionSet, int iFace)
{
	m_iFace = iFace;

	return true;
}

//	Tick routine
bool CECFaceDecal::Tick(DWORD dwDeltaTime)
{
//	if (m_AnimCnt.IncCounter(dwDeltaTime))
	{
		// m_AnimCnt.Reset();
		int nTick = int(GetTickCount() * 0.06);
		int nThisTick = nTick % m_pFrameTick[m_iNumFrame - 1];

		if (nThisTick < m_pFrameTick[m_iFrameCnt])
			m_iFrameCnt = 0;

		while (m_iFrameCnt < m_iNumFrame - 1 && nThisTick > m_pFrameTick[m_iFrameCnt])
			m_iFrameCnt++;
	}

	return true;
}

//	Render routine
bool CECFaceDecal::Render(CECViewport* pViewport)
{
	if (!m_pA2DSprite)
		return true;

	m_iIconIdx = m_iStartFrame + m_iFrameCnt;
	return CECSpriteDecal::Render(pViewport);
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECBubbleDecal
//	
///////////////////////////////////////////////////////////////////////////

CECBubbleDecal::CECBubbleDecal(int iDCID)
{
	m_pDecal = CECDecal::CreateDecal(iDCID);
	ASSERT(m_pDecal);

	m_bLatent	= false;
	m_bLastOne	= true;
	m_bJumpOut	= false;

	m_LifeCnt.SetPeriod(1000);
}

CECBubbleDecal::~CECBubbleDecal()
{
	if (m_pDecal)
	{
		delete m_pDecal;
	}
}

//	Move decal from one place to another
void CECBubbleDecal::SetMoveInfo(const A3DVECTOR3& vVel, DWORD dwMoveTime)
{
	m_vMoveVel = vVel;
	m_MoveCnt.SetPeriod(dwMoveTime);
}

//	Tick routine
bool CECBubbleDecal::Tick(DWORD dwDeltaTime)
{
	if (IsLatent() || IsDead())
		return true;

	if( m_bJumpOut && m_LifeCnt.GetCounter() < 200 )
	{
		if( m_LifeCnt.GetCounter() < 150 )
		{
			m_pDecal->SetBaseScale(1.0f + (150 - m_LifeCnt.GetCounter()) / 150.0f * 1.0f);
		}
		else
			m_pDecal->SetBaseScale(1.0f);
	}
	else
	{
		if (!m_MoveCnt.IsFull())
		{
			m_MoveCnt.IncCounter(dwDeltaTime);

			A3DVECTOR3 vPos = GetPos();
			vPos += m_vMoveVel * (dwDeltaTime * 0.001f);
			SetPos(vPos);
		}
		m_pDecal->SetBaseScale(1.0f);
	}

	m_LifeCnt.IncCounter(dwDeltaTime);
	int nTickLeft = m_LifeCnt.GetPeriod() - m_LifeCnt.GetCounter();
	if( nTickLeft <= 0 )
		m_pDecal->SetAlpha(0.0f);
	else if( nTickLeft < 800 )
		m_pDecal->SetAlpha(nTickLeft / 800.0f);
	else
		m_pDecal->SetAlpha(1.0f );

	return true;
}

//	Render routine
bool CECBubbleDecal::Render(CECViewport* pViewport)
{
	if (IsLatent() || IsDead())
		return true;

	return m_pDecal->Render(pViewport);
}


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECBubbleDecalList
//	
///////////////////////////////////////////////////////////////////////////

CECBubbleDecalList::CECBubbleDecalList()
{
	m_dwInterval	= 100;
	m_dwLifeTime	= 1300;
	m_fSpeed		= 1.2f;
	m_dwMoveTime	= 1000;
	m_dwTimeCnt		= m_dwInterval;
}

CECBubbleDecalList::~CECBubbleDecalList()
{
}

//	Tick routine
bool CECBubbleDecalList::Tick(DWORD dwDeltaTime)
{
	m_dwTimeCnt += dwDeltaTime;

	if (m_DecalList.GetCount())
	{
		if (m_dwTimeCnt >= m_dwInterval)
		{
			m_dwTimeCnt = 0;

			while (m_DecalList.GetCount())
			{
				CECBubbleDecal* pDecal = m_DecalList.RemoveHead();
				pDecal->SetLatentFlag(false);

				if (pDecal->IsLastOne())
					break;
			}
		}
	}
	
	return true;
}

//	Add a bubble decal
CECBubbleDecal* CECBubbleDecalList::AddDecal(const A3DVECTOR3& vPos, int iDCID, bool bLastOne)
{
	CECDecalMan* pDecalMan = (CECDecalMan*)g_pGame->GetGameRun()->GetWorld()->GetDecalMan();
	CECBubbleDecal* pDecal = pDecalMan->CreateBubbleDecal(CECDecal::DCID_ICONDECAL);
	if (!pDecal)
		return NULL;

	pDecal->SetPos(vPos);
	pDecal->SetLifeTime(m_dwLifeTime);
	pDecal->SetMoveInfo(A3DVECTOR3(0.0f, m_fSpeed, 0.0f), m_dwMoveTime);
	pDecal->SetLatentFlag(true);
	pDecal->SetLastOneFlag(bLastOne);

	m_DecalList.AddTail(pDecal);

	return pDecal;
}

