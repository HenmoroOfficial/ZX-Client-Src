/*
 * FILE: ELOutlineTexture.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Wangkuiwu, 2005/6/28
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */
//#include "StdAfx.h"
#include "A3DDevice.h"
#include "A3DViewport.h"
#include "A3DTexture.h"
#include "A3DStream.h"
#include "A3DEngine.h"
#include "A3DTextureMan.h"
#include "A3DTerrain2.h"
#include "A3DTerrain2LOD.h"
#include "A3DCameraBase.h"
#include "A3DOutlineTexture.h"
#include "A3DTerrainOutline2.h"
#include "A3DOutlineQuadTree.h"
//#include "EC_Global.h"

//#define   EL_OTL_DEBUG
#define _EA(x) (x)

int g_iOutlineTileWidth = 1024;
int g_iOutlineTileHeight = 1024;

#ifdef EL_OTL_DEBUG
char  msg[200];
#endif

int A3DOutlineTexture::m_nTileGridLen = 1;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

A3DOutlineTexture::A3DOutlineTexture()
{
	m_pDayTexture = NULL;
	m_pNightTexture  = NULL;

	m_dwFrameStamp = 0;

	m_pA3DDevice = NULL;
	m_pOutline = NULL;
	m_iLevel = 0;
	m_iSubIndex = 0;
	
	
	int i;
	for (i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m_iTiles[i][j] = -1;
		}
	}

	m_bLoadOk = false;
	
	m_dwSmoothFlag = EL_OTL_SMOOTH_NONE;
}



A3DOutlineTexture::~A3DOutlineTexture()
{
	Release();
}



bool A3DOutlineTexture::Init(A3DTerrainOutline2 * pOutline, int iLevel, int iSubIndex)
{
	assert(iLevel >= 0 && iLevel < 5 && _EA("invalid level"));
	assert(iSubIndex > 0 && iSubIndex <= 88 && _EA("invalid subindex"));
	m_pA3DDevice = pOutline->m_pA3DDevice;
	m_pOutline = pOutline;
	m_iLevel = iLevel;
	m_iSubIndex = iSubIndex;
	m_bLoadOk = false;
	m_szName.Format(_EA("%d-%02d.dds"), iLevel, iSubIndex);

	if (!_LoadTex(pOutline))
	{
		return false;
	}

	float fw = m_pOutline->m_OutlineTexSize[iLevel*2];
	float fh = m_pOutline->m_OutlineTexSize[iLevel*2+1];
	int iw = (iSubIndex-1) % pOutline->m_OutlineTexLevel[iLevel*2];
	int ih = (iSubIndex-1) / pOutline->m_OutlineTexLevel[iLevel*2];
	m_RectWorld.left = m_pOutline->m_x + iw*fw;
	m_RectWorld.top  = m_pOutline->m_z - ih*fh;
	m_RectWorld.right = m_RectWorld.left + fw;
	m_RectWorld.bottom = m_RectWorld.top - fh;
	
	m_vBBMins = A3DVECTOR3(m_RectWorld.left, -9000, m_RectWorld.bottom);
	m_vBBMaxs = A3DVECTOR3(m_RectWorld.right, +9000, m_RectWorld.top);

	m_vTexScale =(float)((fw> fh)?(1.0/fw):(1.0/fh));



	
	switch(m_iLevel) {
	case 0:
		m_iTiles[0][0] = m_iSubIndex;
		break;
	case 1:
		A3DOutlineTextureCache::GetChildTex(1, m_iSubIndex, m_iTiles[0], m_pOutline->m_OutlineTexLevel);
		break;
	case 2:
		{
			int child[4];
			A3DOutlineTextureCache::GetChildTex(2, m_iSubIndex, child, m_pOutline->m_OutlineTexLevel);
			for (int i = 0; i < 4; i++) {
				A3DOutlineTextureCache::GetChildTex(1, child[i], m_iTiles[i], m_pOutline->m_OutlineTexLevel);
			}
		}
		break;
	default:
		assert(0);
		break;
	}

	m_bLoadOk = true;
	m_dwSmoothFlag = EL_OTL_SMOOTH_NONE;

	return true;
}


//void A3DOutlineTexture::ActivateTiles()
//{
//	int i, j;
//	for (i = 0; i < 4; i++) {
//		for (j = 0; j < 4; j++) {
//			if (m_iTiles[i][j] > 0) {
//				m_pOutline->m_pOutlineTexCache->ActivateTile(this, m_iTiles[i][j]);				
//			}
//		}
//	}
//}



void  A3DOutlineTexture::ZPrePass(DWORD dwFrame, A3DCameraBase * pCamera)
{
	if (!m_bLoadOk || dwFrame != m_dwFrameStamp/* || !_LoadTex(m_pOutline)*/) {
		return;
	}
	if (!pCamera->AABBInViewFrustum(m_vBBMins, m_vBBMaxs)) {
		return;
	}
	//if ((m_iLevel == 0)
	//	&&(((m_dwSmoothFlag & EL_OTL_SMOOTH_DAY_LEFT) != EL_OTL_SMOOTH_DAY_LEFT)
	//	||((m_dwSmoothFlag & EL_OTL_SMOOTH_DAY_TOP) != EL_OTL_SMOOTH_DAY_TOP)
	//	||((m_dwSmoothFlag & EL_OTL_SMOOTH_NIGHT_LEFT) != EL_OTL_SMOOTH_NIGHT_LEFT)
	//	||((m_dwSmoothFlag & EL_OTL_SMOOTH_NIGHT_TOP) != EL_OTL_SMOOTH_NIGHT_TOP)))
	//{
	//	m_pOutline->m_pOutlineTexCache->SmoothTex(this);
	//}
	//

	//if ((m_pOutline->m_fDNFactor == 0.0f) && (m_pDayTexture != NULL))
	//{//day
	//	m_pDayTexture->Appear(0);
	//}
	//else if ((m_pOutline->m_fDNFactor == 1.0f) && (m_pNightTexture != NULL) )
	//{//night
	//   m_pNightTexture->Appear(0);
	//} 
	//else if((m_pNightTexture != NULL) && (m_pDayTexture != NULL))
	//{//morning or evening
	// m_pDayTexture->Appear(0);
	// m_pNightTexture->Appear(1);
	//}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (m_iTiles[i][j] > 0) {
				A3DOutlineQuadTree * pTile = m_pOutline->m_pOutlineTexCache->GetTile(this, m_iTiles[i][j]);
				if (pTile != NULL) {
					pTile->ZPrePass(pCamera);
				}
			}
		}
	}
	

	//if ((m_pOutline->m_fDNFactor == 0.0f) && (m_pDayTexture != NULL))
	//{//day
	//	m_pDayTexture->Disappear(0);
	//}
	//else if ((m_pOutline->m_fDNFactor == 1.0f) && (m_pNightTexture != NULL) )
	//{//night
	//   m_pNightTexture->Disappear(0);
	//} 
	//else if((m_pNightTexture != NULL) && (m_pDayTexture != NULL))
	//{//morning or evening
	// m_pDayTexture->Disappear(0);
	// m_pNightTexture->Disappear(1);
	//}


}
void  A3DOutlineTexture::Render(DWORD dwFrame, A3DCameraBase * pCamera)
{
	if (!m_bLoadOk || dwFrame != m_dwFrameStamp || !_LoadTex(m_pOutline)) {
		return;
	}
	if (!pCamera->AABBInViewFrustum(m_vBBMins, m_vBBMaxs)) {
		return;
	}
	if ((m_iLevel == 0)
		&&(((m_dwSmoothFlag & EL_OTL_SMOOTH_DAY_LEFT) != EL_OTL_SMOOTH_DAY_LEFT)
		||((m_dwSmoothFlag & EL_OTL_SMOOTH_DAY_TOP) != EL_OTL_SMOOTH_DAY_TOP)
		||((m_dwSmoothFlag & EL_OTL_SMOOTH_NIGHT_LEFT) != EL_OTL_SMOOTH_NIGHT_LEFT)
		||((m_dwSmoothFlag & EL_OTL_SMOOTH_NIGHT_TOP) != EL_OTL_SMOOTH_NIGHT_TOP)))
	{
		m_pOutline->m_pOutlineTexCache->SmoothTex(this);
	}


	if ((m_pOutline->m_fDNFactor == 0.0f) && (m_pDayTexture != NULL))
	{//day
		m_pDayTexture->Appear(0);
	}
	else if ((m_pOutline->m_fDNFactor == 1.0f) && (m_pNightTexture != NULL) )
	{//night
		m_pNightTexture->Appear(0);
	} 
	else if((m_pNightTexture != NULL) && (m_pDayTexture != NULL))
	{//morning or evening
		m_pDayTexture->Appear(0);
		m_pNightTexture->Appear(1);
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (m_iTiles[i][j] > 0) {
				A3DOutlineQuadTree * pTile = m_pOutline->m_pOutlineTexCache->GetTile(this, m_iTiles[i][j]);
				if (pTile != NULL) {
					pTile->Render(pCamera);
				}
			}
		}
	}


	if ((m_pOutline->m_fDNFactor == 0.0f) && (m_pDayTexture != NULL))
	{//day
		m_pDayTexture->Disappear(0);
	}
	else if ((m_pOutline->m_fDNFactor == 1.0f) && (m_pNightTexture != NULL) )
	{//night
		m_pNightTexture->Disappear(0);
	} 
	else if((m_pNightTexture != NULL) && (m_pDayTexture != NULL))
	{//morning or evening
		m_pDayTexture->Disappear(0);
		m_pNightTexture->Disappear(1);
	}


}
void A3DOutlineTexture::Stitch(DWORD dwFrame, A3DTerrain2::ACTBLOCKS * pActBlocks, const ARectF& rcSeamWorld)
{
	if (!m_bLoadOk || dwFrame != m_dwFrameStamp)  {
		return;
	}
	//test if two rect intersect
	if (!A3DOutlineTextureCache::RcOverlapRc(m_RectWorld, rcSeamWorld)) {
		return;
	}

	A3DTerrain2LOD::GRID* pTerrain2Grids = m_pOutline->m_pTerrain2->GetLODManager()->GetGrids();

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (m_iTiles[i][j] > 0) {
				A3DOutlineQuadTree * pTile = m_pOutline->m_pOutlineTexCache->GetTile(this, m_iTiles[i][j]);
				if (pTile != NULL) {
					pTile->StitchAndCompleteTerrain(pActBlocks, m_pOutline->m_BlockVisibleRect, pTerrain2Grids);
				}
			}
		}
	}

}

void A3DOutlineTexture::Release()
{
	if (m_pDayTexture) {
		m_pA3DDevice->GetA3DEngine()->GetA3DTextureMan()->ReleaseTexture(m_pDayTexture);
		m_pDayTexture = NULL;
	}

	if (m_pNightTexture)
	{
		m_pA3DDevice->GetA3DEngine()->GetA3DTextureMan()->ReleaseTexture(m_pNightTexture);
		m_pNightTexture = NULL;
	}
	
	int i;
	for (i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (m_iTiles[i][j] > 0) {
				m_pOutline->m_pOutlineTexCache->ReleaseTile(this, m_iTiles[i][j]);			
			}
		}
	}
}






void A3DOutlineTexture::RenderReplaceTerrain(const A3DVECTOR3 * pVerts, int nVerts, const WORD * pIndices, int nIndices)
{

	if ((!m_bLoadOk) || (nVerts <=0)  || (nIndices <= 0) || !_LoadTex(m_pOutline) ){
		return;
	}

	if ((m_iLevel == 0)
		&&(((m_dwSmoothFlag & EL_OTL_SMOOTH_DAY_LEFT) != EL_OTL_SMOOTH_DAY_LEFT)
		||((m_dwSmoothFlag & EL_OTL_SMOOTH_DAY_TOP) != EL_OTL_SMOOTH_DAY_TOP)
		||((m_dwSmoothFlag & EL_OTL_SMOOTH_NIGHT_LEFT) != EL_OTL_SMOOTH_NIGHT_LEFT)
		||((m_dwSmoothFlag & EL_OTL_SMOOTH_NIGHT_TOP) != EL_OTL_SMOOTH_NIGHT_TOP)))
	{
		m_pOutline->m_pOutlineTexCache->SmoothTex(this);
	}


	A3DStream * pFakeTerrainStream = m_pOutline->m_pFakeTerrainStream;
	assert(pFakeTerrainStream != NULL);

	A3DLVERTEX * pVB;
	if( !pFakeTerrainStream->LockVertexBufferDynamic(0, 0, (BYTE **)&pVB, 0) ){
		a_LogOutput(1, "A3DOutlineTexture::RenderReplaceTerrain, Failed to lock vertex buffer!");
		return ;
	} 
		
	int i;
	for (i = 0; i < nVerts; i++) {
		*pVB  = A3DLVERTEX(pVerts[i], OUTLINE_DIFFUSE, OUTLINE_SPECULAR,
				(pVerts[i].x - m_RectWorld.left)*m_vTexScale, (m_RectWorld.top - pVerts[i].z)*m_vTexScale);
		pVB++;
	}
    pFakeTerrainStream->UnlockVertexBufferDynamic();

    WORD * pIB;
    if( !pFakeTerrainStream->LockIndexBufferDynamic(0, 0, (BYTE **)&pIB, 0) ){
        a_LogOutput(1, "A3DOutlineTexture::RenderReplaceTerrain, Failed to lock index buffer!");
        return ;
    }

    for (i = 0; i < nIndices; i++) {
		*pIB = pIndices[i];
		pIB++;
	}
	pFakeTerrainStream->UnlockIndexBufferDynamic();

	if (m_pOutline->m_fDNFactor == 0.0f)
	{//day
		m_pDayTexture->Appear(0);
		m_pA3DDevice->SetTextureAddress(0, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	}
	else if (m_pOutline->m_fDNFactor == 1.0f) 
	{//night
		m_pNightTexture->Appear(0);
		m_pA3DDevice->SetTextureAddress(0, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	} 
	else 
	{//morning or evening
		m_pDayTexture->Appear(0);
		m_pNightTexture->Appear(1);
		m_pA3DDevice->SetTextureAddress(0, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
		m_pA3DDevice->SetTextureAddress(1, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	}

	pFakeTerrainStream->Appear(0, false);
	m_pA3DDevice->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, nVerts, 0, nIndices/3);


	if (m_pOutline->m_fDNFactor == 0.0f)
	{//day
		m_pDayTexture->Disappear(0);
		m_pA3DDevice->SetTextureAddress(0, A3DTADDR_WRAP, A3DTADDR_WRAP);
	}
	else if (m_pOutline->m_fDNFactor == 1.0f) 
	{//night
		m_pNightTexture->Disappear(0);
		m_pA3DDevice->SetTextureAddress(0, A3DTADDR_WRAP, A3DTADDR_WRAP);
	} 
	else 
	{//morning or evening
		m_pDayTexture->Disappear(0);
		m_pNightTexture->Disappear(1);
		m_pA3DDevice->SetTextureAddress(0, A3DTADDR_WRAP, A3DTADDR_WRAP);
		m_pA3DDevice->SetTextureAddress(1, A3DTADDR_WRAP, A3DTADDR_WRAP);
	}
}


//@todo : refine this method. By Kuiwu[26/8/2005]
bool A3DOutlineTexture::_LoadTex(A3DTerrainOutline2 * pOutline)
{
	//@note : keep the unused texture resident in memory, 
	//		  e.g : keep the night texture when day. 
	//        Is it a better way to release the unused texture immediately? By Kuiwu[26/8/2005]
#define  _DELETE_TEX_DYNAMIC
#ifndef   _DELETE_TEX_DYNAMIC
	if (m_pDayTexture && m_pNightTexture)
	{
		return true;
	}
#endif

	float   fDN = pOutline->m_fDNFactor;
	char    szDayPath[200];
	char    szNightPath[200];

	sprintf(szDayPath, _EA("%sday\\Level-%d\\%s"), pOutline->m_szTexDir, m_iLevel, m_szName );
	sprintf(szNightPath, _EA("%snight\\Level-%d\\%s"), pOutline->m_szTexDir, m_iLevel, m_szName );

	bool bDay = true, bNight = true;
	//check day
	if ((bNight) && (fDN < 1.0f) && (m_pDayTexture == NULL))
	{
		bDay = m_pA3DDevice->GetA3DEngine()->GetA3DTextureMan()->LoadTextureFromFile(szDayPath, &m_pDayTexture, A3DTF_NODOWNSAMPLE);
	}
	//check night
	if ((bDay) && (fDN > 0.0f) && (m_pNightTexture == NULL))
	{
		bNight = m_pA3DDevice->GetA3DEngine()->GetA3DTextureMan()->LoadTextureFromFile(szNightPath, &m_pNightTexture, A3DTF_NODOWNSAMPLE);
	}

#ifdef _DELETE_TEX_DYNAMIC

	if ((fDN == 1.0f ) && (m_pDayTexture != NULL))
	{
		m_pA3DDevice->GetA3DEngine()->GetA3DTextureMan()->ReleaseTexture(m_pDayTexture);
		m_pDayTexture = NULL;
	}

	if ((fDN == 0.0f) && (m_pNightTexture != NULL))
	{
		m_pA3DDevice->GetA3DEngine()->GetA3DTextureMan()->ReleaseTexture(m_pNightTexture);
		m_pNightTexture = NULL;
	}
#endif

#undef  _DELETE_TEX_DYNAMIC

	return (bDay && bNight);

}

int A3DOutlineTexture::GetTileGridLen()
{
	return m_nTileGridLen;
}

void A3DOutlineTexture::SetTileGridLen( int nLength )
{
	m_nTileGridLen = nLength;
}

//void A3DOutlineTexture::CreateView(const ARectI& rcSeam, const A3DVECTOR3& origin, float radius,  DWORD dwFrame)
//{
//	if (dwFrame != m_dwFrameStamp)  {
//		return;
//	}
//	//test if intersect
//	if (!A3DOutlineTextureCache::RcOverlapCircle(m_RectWorld, radius, origin.x, origin.z)) {
//		return;
//	}
//	
//	for (int i = 0; i < 4; i++) {
//		for (int j = 0; j < 4; j++) {
//			if (m_iTiles[i][j] > 0) {
//				A3DOutlineQuadTree * pTile = m_pOutline->m_pOutlineTexCache->GetTile(this, m_iTiles[i][j]);
//				if (pTile != NULL) {
//					pTile->CreateView(rcSeam, origin, radius);
//				}
//			}
//		}
//	}
//}

//////////////////////////////////////////////////////////////////////////
// A3DOutlineTextureCache
//////////////////////////////////////////////////////////////////////////

A3DOutlineTextureCache::A3DOutlineTextureCache()
{
	int i;
	for (i = 0; i < OUTLINE_TEX_CACHE_SIZE; i++) {
		m_pCache[i] = NULL;
	}
	m_nSize = 0;
	m_pOutline = NULL;

	for (i = 0; i < OUTLINE_QUAD_TREE_SIZE; i++) {
		m_pTiles[i] = NULL;
	}
}


A3DOutlineTextureCache::~A3DOutlineTextureCache()
{
	Release();	
}


void A3DOutlineTextureCache::Release()
{
	int i;
	for (i = 0; i < m_nSize; i++) {
		if (m_pCache[i]) {
			delete m_pCache[i];
			m_pCache[i] = NULL;
		}
	}
	for (i = 0; i < m_pOutline->m_OutlineTexLevel[0] * m_pOutline->m_OutlineTexLevel[1]; i++) {
		if (m_pTiles[i]) {
			m_pTiles[i]->Release();
			delete m_pTiles[i];
			m_pTiles[i] = NULL;
		} 
	}
}


bool A3DOutlineTextureCache::Init(A3DTerrainOutline2 * pOutline)
{
	m_pOutline = pOutline;
	return true;
}

void A3DOutlineTextureCache::ActivateTile(const A3DVECTOR3& origin, float radius, DWORD dwFrame)
{
	int iTopH, iBottomH, iLeftW, iRightW;
	iTopH = (int)((m_pOutline->m_z - (origin.z + radius ))/ g_iOutlineTileHeight);	
	iTopH = max(0, iTopH);
	iBottomH = (int)((m_pOutline->m_z - (origin.z - radius ))/ g_iOutlineTileHeight);	
	iBottomH = min(m_pOutline->m_OutlineTexLevel[1]-1, iBottomH);
	iLeftW = (int)(((origin.x - radius) - m_pOutline->m_x) / g_iOutlineTileWidth);
	iLeftW = max(0, iLeftW);
	iRightW = (int)(((origin.x + radius) - m_pOutline->m_x) / g_iOutlineTileWidth);
	iRightW = min(m_pOutline->m_OutlineTexLevel[0]-1, iRightW);
	
	ARectF rcViewLevel0, rcViewLevel1, rcViewLevel2;
	rcViewLevel0.left = origin.x - m_pOutline->m_fLevelEndDist[0];
	rcViewLevel0.right = origin.x + m_pOutline->m_fLevelEndDist[0];
	rcViewLevel0.top = origin.z + m_pOutline->m_fLevelEndDist[0];
	rcViewLevel0.bottom = origin.z - m_pOutline->m_fLevelEndDist[0];

	rcViewLevel1.left = origin.x - m_pOutline->m_fLevelEndDist[1];
	rcViewLevel1.right = origin.x + m_pOutline->m_fLevelEndDist[1];
	rcViewLevel1.top = origin.z + m_pOutline->m_fLevelEndDist[1];
	rcViewLevel1.bottom = origin.z - m_pOutline->m_fLevelEndDist[1];

	rcViewLevel2.left = origin.x - m_pOutline->m_fLevelEndDist[2];
	rcViewLevel2.right = origin.x + m_pOutline->m_fLevelEndDist[2];
	rcViewLevel2.top = origin.z + m_pOutline->m_fLevelEndDist[2];
	rcViewLevel2.bottom = origin.z - m_pOutline->m_fLevelEndDist[2];


	int iw, ih, iSubIndex, iLevel;
	ARectF rcTileWorld;
	for (ih = iTopH; ih <= iBottomH; ih++) {
		for (iw = iLeftW; iw <= iRightW; iw++) {
			rcTileWorld.left = m_pOutline->m_x + iw* g_iOutlineTileWidth;
			rcTileWorld.top  = m_pOutline->m_z - ih* g_iOutlineTileHeight;
			rcTileWorld.right = rcTileWorld.left + g_iOutlineTileWidth;
			rcTileWorld.bottom = rcTileWorld.top - g_iOutlineTileHeight;
			if (!RcOverlapCircle(rcTileWorld, radius, origin.x, origin.z)) {
				continue;
			}
			iSubIndex = ih * m_pOutline->m_OutlineTexLevel[0] + iw +1;
			
			iLevel = 0;
			
			int iTexSubIndex = -1;
			if (RcOverlapRc(rcTileWorld, rcViewLevel0)) {
				iLevel = 0;
				iTexSubIndex = iSubIndex;
			}
			else if (RcOverlapRc(rcTileWorld, rcViewLevel1)) {
				iLevel = 1;
				iTexSubIndex = GetParentTex(0, iSubIndex, m_pOutline->m_OutlineTexLevel);
			}
			else if (RcOverlapRc(rcTileWorld, rcViewLevel2)) {
				iLevel = 2;
				iTexSubIndex = GetParentTex(0, iSubIndex, m_pOutline->m_OutlineTexLevel);
				iTexSubIndex = GetParentTex(1, iTexSubIndex, m_pOutline->m_OutlineTexLevel);
			}
			else{
				assert(0);
				//g_Log.Log(_EA("A3DOutlineTextureCache::ActivateTile() tileworld %f %f %f %f seems invalid!"), rcTileWorld.left, rcTileWorld.top,rcTileWorld.right, rcTileWorld.bottom);
			}
			
			A3DOutlineTexture * pTex = NULL;
			if (iTexSubIndex > 0)
			{
			   pTex = FindTex(iLevel, iTexSubIndex);	
	   			if (!pTex) 
				{
					pTex = AddTex(iLevel, iTexSubIndex);			
				}
			}
			if (pTex) {
				pTex->SetFrame(dwFrame);
				ActivateTile(pTex, iSubIndex);
			}
			
		}
	}

	_CompactCache(dwFrame);

}



A3DOutlineTexture* A3DOutlineTextureCache::FindTex(int iLevel, int iSubIndex)
{
	int i;
	A3DOutlineTexture * pTex = NULL;
	for (i = 0; i < m_nSize; i++) {
		if ((iLevel == m_pCache[i]->m_iLevel) 
			&& (iSubIndex == m_pCache[i]->m_iSubIndex)){
			pTex = m_pCache[i];
			break;
		}
	}

	return pTex;
}


/*
 *	@note: the caller make sure the tex is not in cache yet.
 */
A3DOutlineTexture * A3DOutlineTextureCache::AddTex(int iLevel, int iSubIndex)
{
	A3DOutlineTexture * pTex = new A3DOutlineTexture;
	if(!pTex->Init(m_pOutline, iLevel, iSubIndex)){
		delete pTex;
		pTex = NULL;
		return NULL;
	}

	if (m_nSize < OUTLINE_TEX_CACHE_SIZE) {
		m_pCache[m_nSize] = pTex;
		m_nSize++;
	}
	else{
		ReplaceTex(pTex);
	}
	return pTex;
}

/*
 *	replace a tex with the given pTex in the LRU way
 */
void A3DOutlineTextureCache::ReplaceTex(A3DOutlineTexture * pTex)
{
	assert(m_nSize > 0);
	int iReplace = _GetMinFrameTex();
	if (iReplace < 0)
	{
		iReplace = 0;
	}
	

	A3DOutlineTexture * pOldTex = m_pCache[iReplace];
	m_pCache[iReplace] = pTex;
	//free the old tex
	if (pOldTex)
	{
		delete pOldTex;
	}
	

}




void A3DOutlineTextureCache::Render(DWORD dwFrame, A3DCameraBase * pCamera)
{
	
	int i;
	for (i = 0; i < m_nSize; i++) {
		m_pCache[i]->Render(dwFrame, pCamera);
	}
}

void A3DOutlineTextureCache::ZPrePass(DWORD dwFrame, A3DCameraBase * pCamera)
{

	int i;
	for (i = 0; i < m_nSize; i++) {
		m_pCache[i]->ZPrePass(dwFrame, pCamera);
	}
}

void A3DOutlineTextureCache::Stitch(DWORD dwFrame, A3DTerrain2::ACTBLOCKS * pActBlocks)
{
	int i;

	for (i = 0; i < m_pOutline->m_OutlineTexLevel[0] * m_pOutline->m_OutlineTexLevel[1]; i++) {
		if (m_pTiles[i]) {
			m_pTiles[i]->ClearSeamStream();
		} 
	}

	ARectI rcSeamBlk(m_pOutline->m_BlockVisibleRect);
	rcSeamBlk.Inflate(1,1);

	ARectF rcSeamWorld;
	int blksize = OUTLINE_BLK_LEN * A3DOutlineTexture::GetTileGridLen();
	rcSeamWorld.left = m_pOutline->m_x + rcSeamBlk.left * blksize;
	rcSeamWorld.top = m_pOutline->m_z - rcSeamBlk.top* blksize;
	rcSeamWorld.right = rcSeamWorld.left + rcSeamBlk.Width() * blksize;
	rcSeamWorld.bottom = rcSeamWorld.top - rcSeamBlk.Height()* blksize;


	for (i = 0; i < m_nSize; i++) {
		if (m_pOutline->m_iMinLevel == m_pCache[i]->m_iLevel) {
			m_pCache[i]->Stitch( dwFrame, pActBlocks, rcSeamWorld);
		}
	}
}






void A3DOutlineTextureCache::CreateView(DWORD dwFrame, const A3DVECTOR3& origin,  float radius)
{

	ARectI rcSeam(m_pOutline->m_BlockVisibleRect);
	rcSeam.Inflate(1,1);
	rcSeam.left *= OUTLINE_BLK_LEN;
	rcSeam.top *= OUTLINE_BLK_LEN;
	rcSeam.right *= OUTLINE_BLK_LEN;
	rcSeam.bottom *= OUTLINE_BLK_LEN;

//	int i;
//	for (i = 0; i < m_nSize; i++) {
//		m_pCache[i]->CreateView(rcSeam, origin, radius, dwFrame);
//	}

    ARectI rcView;
	rcView.left = (int)(((origin.x -radius) - m_pOutline->m_x) / A3DOutlineTexture::GetTileGridLen());
	rcView.right = (int)(((origin.x +radius) - m_pOutline->m_x) / A3DOutlineTexture::GetTileGridLen());
	rcView.top = (int)((m_pOutline->m_z -(origin.z + radius)) / A3DOutlineTexture::GetTileGridLen());
	rcView.bottom = (int)((m_pOutline->m_z -(origin.z - radius)) / A3DOutlineTexture::GetTileGridLen());

	int i;
	for (i = 0; i < m_pOutline->m_OutlineTexLevel[0] * m_pOutline->m_OutlineTexLevel[1]; i++ ) {
		if (m_pTiles[i]) {
			m_pTiles[i]->CreateView(dwFrame, rcSeam, rcView);
		}
	}

	
}


/*
 *    -----------
 *	  |		|    |	
 *	  |-----|----|
 *    |		|	 |
 *    ------------ 
 */
int A3DOutlineTextureCache::GetChildTex(int iLevel, int iSubIndex, int iChild[], const int *pOutlineTexLevel)
{
    if (!pOutlineTexLevel || iLevel <=0 || iSubIndex <= 0) {
		return 0;
	}

	int iw = (iSubIndex-1) % pOutlineTexLevel[iLevel*2];
	int ih = (iSubIndex-1) / pOutlineTexLevel[iLevel*2];
	
	int iChildIndex;
	int count = 0;
    int i = 0;
	int maxChildIndex = pOutlineTexLevel[(iLevel-1)*2] * pOutlineTexLevel[(iLevel-1)*2+1];

	iChildIndex = ih * 2 * pOutlineTexLevel[(iLevel-1)*2] + iw*2 + 1;
	if (iChildIndex <= maxChildIndex) {
		iChild[i] = iChildIndex;
		count++;
	}
	else{
		iChild[i] = -1;
	}
	i++;

	iChildIndex++;
	if (iChildIndex <= maxChildIndex) {
		iChild[i] = iChildIndex;
		count++;
	}
	else{
		iChild[i] = -1;
	}
	i++;

	iChildIndex += pOutlineTexLevel[(iLevel-1)*2] - 1;
	if (iChildIndex <= maxChildIndex) {
		iChild[i] = iChildIndex;
		count++;
	}
	else{
		iChild[i] = -1;
	}
	i++;

	iChildIndex++;
	if (iChildIndex <= maxChildIndex) {
		iChild[i] = iChildIndex;
		count++;
	}
	else{
		iChild[i] = -1;
	}

	return count;

}

int A3DOutlineTextureCache::GetParentTex(int iLevel, int iSubIndex, const int *pOutlineTexLevel)
{
	if (!pOutlineTexLevel || iLevel < 0 || iSubIndex <= 0) {
		return 0;
	}

	int iw = (iSubIndex-1) % pOutlineTexLevel[iLevel*2];
	int ih = (iSubIndex-1) / pOutlineTexLevel[iLevel*2];
	
	int iParentW = iw/2;
	int iParentH = ih/2;

	return (iParentH * pOutlineTexLevel[(iLevel+1)*2] + iParentW +1);
}


bool A3DOutlineTextureCache::IsActive(DWORD dwFrame, int iLevel, int iSubIndex)
{
	bool bActive = false;
	A3DOutlineTexture * pTex = FindTex(iLevel, iSubIndex);
	if (pTex) {
		bActive = (dwFrame == pTex->m_dwFrameStamp);
	}

	return bActive;
}


int A3DOutlineTextureCache::GetActiveTexCount(DWORD dwFrame, int iLevel)
{
	int i;
	int count = 0;
	for (i = 0; i < m_nSize; i++) {
		if((m_pCache[i]->m_iLevel == iLevel) &&( m_pCache[i]->m_dwFrameStamp == dwFrame) ){
			count++;
		}
	}
	return count;
}



bool A3DOutlineTextureCache::RcOverlapCircle(const ARectF& rc, float radius, float cx, float cz)
{
	float s, d = 0;
	
	if (cx < rc.left) {
		s= cx - rc.left;
		d += s*s;
	}
	else if (cx > rc.right) {
		s = cx - rc.right;
		d += s*s;
	}
	if (cz > rc.top) {
		s = cz - rc.top;
		d += s*s;
	}
	else if (cz < rc.bottom) {
		s = cz - rc.bottom;
		d += s*s;
	}

	return (d <= radius*radius );

}







void A3DOutlineTextureCache::RenderReplaceTerrain(DWORD dwFrame, int iLevel, const A3DVECTOR3 * pVerts, int nVerts, const WORD * pIndices, int nIndices)
{
	
	int iSubIndex = GetTexSubIndex(iLevel, (pVerts[0].x + pVerts[1].x)*0.5f, (pVerts[0].z + pVerts[1].z)*0.5f);
	A3DOutlineTexture * pTex = FindTex(iLevel, iSubIndex);
	assert((pTex != NULL) && (pTex->m_dwFrameStamp == dwFrame));

	if ((pTex == NULL) || (pTex->m_dwFrameStamp != dwFrame) ){
		//@todo: kuiwu fix me!
		return;
	}

	pTex->RenderReplaceTerrain(pVerts, nVerts, pIndices, nIndices);
}


int A3DOutlineTextureCache::GetTexSubIndex(int iLevel, float x, float z)
{
	int c =(int) ((x - m_pOutline->m_x + 0.5) / m_pOutline->m_OutlineTexSize[iLevel*2]);
	int r = (int)( (m_pOutline->m_z - z + 0.5) / m_pOutline->m_OutlineTexSize[iLevel*2+1]);
	return (r * m_pOutline->m_OutlineTexLevel[iLevel*2] + c +1);
}


bool A3DOutlineTextureCache::RcOverlapRc(const ARectF& rc1, const ARectF& rc2)
{
	return	 (rc1.left < rc2.right - 0.1)
			 &&(rc1.right > rc2.left + 0.1)
			 &&(rc1.top > rc2.bottom + 0.1)
			 &&(rc1.bottom < rc2.top - 0.1);

}
bool A3DOutlineTextureCache::RcOverlapRc(const ARectI& rc1, const ARectI& rc2)
{
	return	 (rc1.left < rc2.right )
			 &&(rc1.right > rc2.left )
			 &&(rc1.top < rc2.bottom )
			 &&(rc1.bottom > rc2.top );

}

A3DOutlineQuadTree * A3DOutlineTextureCache::GetTile(A3DOutlineTexture * pTex, int iSubIndex)
{
	int index = iSubIndex -1;
	assert(index >= 0);
	A3DOutlineQuadTree * pTile ;
	pTile = m_pTiles[index];
	if ((pTile != NULL) && (pTile->m_pELOTexture == pTex)) {
		return pTile;
	}
	else{
		return NULL;
	}
}

bool    A3DOutlineTextureCache::ReleaseTile(A3DOutlineTexture * pTex, int iSubIndex)
{
	int index = iSubIndex -1;
	assert(index >= 0);
	A3DOutlineQuadTree * pTile ;
	pTile = m_pTiles[index];
	if ((pTile != NULL) && (pTile->m_pELOTexture == pTex)) {
		pTile->Release();
		delete pTile;
		m_pTiles[index] = NULL;
		return true;
	}
	
	return false;
}

bool A3DOutlineTextureCache::ActivateTile(A3DOutlineTexture * pTex, int iSubIndex)
{
	int index = iSubIndex -1;
	assert(index >= 0);
	A3DOutlineQuadTree * pTile ;
	pTile = m_pTiles[index];
	if (pTile == NULL) {
		pTile = new A3DOutlineQuadTree;
		pTile->Init(m_pOutline->m_pA3DDevice, iSubIndex, m_pOutline->m_x, m_pOutline->m_z, m_pOutline->m_OutlineTexLevel[0]);
		pTile->SetOwnerTex(pTex);
		if(!pTile->Load(m_pOutline->m_szTileDir)){
			delete pTile;
			return false;
		}
		m_pTiles[index] = pTile;
		return true;
	}

	if (pTile->m_pELOTexture == pTex) {
		return true;
	}
	
	if (pTile->m_pELOTexture->m_iLevel > pTex->m_iLevel
		|| pTile->m_pELOTexture->m_dwFrameStamp < pTex->m_dwFrameStamp) {
		pTile->SetOwnerTex(pTex);
		return true;
	}

	return false;
}


bool A3DOutlineTextureCache::_CompactCache(DWORD dwFrame)
{
	//@note : pre-condition. By Kuiwu[22/9/2005]
	assert(m_pOutline && m_pOutline->m_iCurViewScheme <=4 
		&& m_pOutline->m_iCurViewScheme >= 0 && _EA("pre-condition fail!"));

	int nExpectCacheSize = m_pOutline->m_nExpectCacheSize[m_pOutline->m_iCurViewScheme];
	assert(nExpectCacheSize <= OUTLINE_TEX_CACHE_SIZE);
	

	while (m_nSize > nExpectCacheSize)
	{
		int index = _GetMinFrameTex(dwFrame);
		if (index < 0)
		{
			break;
		}
		_RemoveTex(index);
	}
	
	return (m_nSize <= nExpectCacheSize);

}

int A3DOutlineTextureCache::_GetMinFrameTex(DWORD dwFrame)
{
	int i;
	DWORD minFrm = dwFrame;
	int   index = -1;
	for (i = 0; i < m_nSize; i++)
	{
		if (m_pCache[i] && m_pCache[i]->m_dwFrameStamp < minFrm)
		{
			minFrm = m_pCache[i]->m_dwFrameStamp;
			index = i;
		}
	}
	//assert(index >= 0);
	return index;
}

void A3DOutlineTextureCache::_RemoveTex(int index)
{
	//@note : pre-condition By Kuiwu[22/9/2005]
	assert(index >= 0 && index < m_nSize && _EA("invalid index"));

	A3DOutlineTexture * pTex = m_pCache[index];
	if (pTex)
	{
		delete pTex;
	}
	int i;
	for (i = index+1; i < m_nSize; i++)
	{
		m_pCache[i-1] = m_pCache[i];
	}
	--m_nSize;
}


void A3DOutlineTextureCache::SmoothTex(A3DOutlineTexture * pTex)
{
	//@note : disable or enable smoothing. By Kuiwu[15/11/2005]
	//return;

	assert(pTex != NULL && _EA("invalid input"));
	if (pTex->m_iLevel != 0 )
	{//only handle level0
		return ;  
	}

	A3DOutlineTexture * pNeighbor = NULL;
	if (!(pTex->m_dwSmoothFlag & EL_OTL_SMOOTH_DAY_LEFT))
	{
		pNeighbor = GetNeighborTex(pTex, EL_OTL_SMOOTH_DAY_LEFT);
		if (pNeighbor && _SmoothTex(pTex, EL_OTL_SMOOTH_DAY_LEFT, pNeighbor))
		{
			pTex->m_dwSmoothFlag |= EL_OTL_SMOOTH_DAY_LEFT;
		}

	}
	if (!(pTex->m_dwSmoothFlag & EL_OTL_SMOOTH_NIGHT_LEFT))
	{
		pNeighbor = GetNeighborTex(pTex, EL_OTL_SMOOTH_NIGHT_LEFT);
		if (pNeighbor && _SmoothTex(pTex, EL_OTL_SMOOTH_NIGHT_LEFT, pNeighbor))
		{
			pTex->m_dwSmoothFlag |= EL_OTL_SMOOTH_NIGHT_LEFT;
		}

	}
	if (!(pTex->m_dwSmoothFlag & EL_OTL_SMOOTH_DAY_TOP))
	{
		pNeighbor = GetNeighborTex(pTex, EL_OTL_SMOOTH_DAY_TOP);
		if (pNeighbor && _SmoothTex(pTex, EL_OTL_SMOOTH_DAY_TOP, pNeighbor))
		{
			pTex->m_dwSmoothFlag |= EL_OTL_SMOOTH_DAY_TOP;
		}

	}
	if (!(pTex->m_dwSmoothFlag & EL_OTL_SMOOTH_NIGHT_TOP))
	{
		pNeighbor = GetNeighborTex(pTex, EL_OTL_SMOOTH_NIGHT_TOP);
		if (pNeighbor && _SmoothTex(pTex, EL_OTL_SMOOTH_NIGHT_TOP, pNeighbor))
		{
			pTex->m_dwSmoothFlag |= EL_OTL_SMOOTH_NIGHT_TOP;
		}

	}

}

/*
 * 
 * @desc :
 * @param :     
 * @return :
 * @note:
 * @todo:   
 * @author: kuiwu [14/11/2005]
 * @ref:
 */
A3DOutlineTexture * A3DOutlineTextureCache::GetNeighborTex(A3DOutlineTexture * pTex, DWORD dwDir)
{
	assert((pTex != NULL)  && _EA("invalid input"));
	int iSubIndex = pTex->m_iSubIndex;
	int iLevel = pTex->m_iLevel;

	int iw = (iSubIndex-1) % m_pOutline->m_OutlineTexLevel[iLevel*2];
	int ih = (iSubIndex-1) / m_pOutline->m_OutlineTexLevel[iLevel*2];
	int iNeighbor;
	if (dwDir & (EL_OTL_SMOOTH_DAY_LEFT| EL_OTL_SMOOTH_NIGHT_LEFT) )
	{
		if (iw <= 0)
		{
			return NULL;
		}
		--iw;
	}
	else if (dwDir & (EL_OTL_SMOOTH_DAY_TOP|EL_OTL_SMOOTH_NIGHT_TOP) )
	{
		if (ih <= 0)
		{
			return NULL;
		}
		--ih;

	}
//	else if ((dwDir & EL_OTL_SMOOTH_RIGHT) == EL_OTL_SMOOTH_RIGHT)
//	{
//		if (iw >= OUTLINE_TEX_LEVEL[iLevel*2]-1)
//		{
//			return NULL;
//		}
//		++iw;
//	}
//	else if ((dwDir & EL_OTL_SMOOTH_BOTTOM) == EL_OTL_SMOOTH_BOTTOM)
//	{
//		if (ih >= OUTLINE_TEX_LEVEL[iLevel*2+1]-1)
//		{
//			return NULL;
//		}
//		++ih;
//	}
	else
	{
		assert(0);
		return NULL;
	}
	iNeighbor = ih * m_pOutline->m_OutlineTexLevel[iLevel*2] + iw +1;
	return FindTex(iLevel, iNeighbor);
}

/*
 *
 * @desc :
 * @param dwDir :    the direction that pTex1 is relative to pTex2.
 * @return :
 * @note:
 * @todo:   
 * @author: kuiwu [14/11/2005]
 * @ref:
 */
bool A3DOutlineTextureCache::_SmoothTex(A3DOutlineTexture * pTex1, DWORD dwDir, A3DOutlineTexture * pTex2)
{
	A3DDevice * pDevice = pTex1->m_pA3DDevice;

	A3DTexture * pA3DTex1 = NULL;
	A3DTexture * pA3DTex2 = NULL;
	bool bRet = false;
	if (dwDir & (EL_OTL_SMOOTH_DAY_LEFT| EL_OTL_SMOOTH_DAY_TOP))
	{
		
		pA3DTex1 = pTex1->m_pDayTexture;
		pA3DTex2 = pTex2->m_pDayTexture;
	}
	else
	{//night
		pA3DTex1 = pTex1->m_pNightTexture;
		pA3DTex2 = pTex2->m_pNightTexture;
	}
	if (pDevice && pA3DTex1 && pA3DTex2)
	{
		bRet = _SmoothTex(pDevice, pA3DTex1, dwDir, pA3DTex2);
#ifdef EL_OTL_DEBUG
		sprintf(msg, _EA("tex1 %d  dir %x tex2 %d\n"), pTex1->m_iSubIndex, dwDir, pTex2->m_iSubIndex);
		OutputDebugStringA(msg);
#endif
	}
	return bRet;
}

/*
 *
 * @desc :
 * @param :     
 * @return :
 * @note:   make it simple to use. By kuiwu.
 * @todo:   
 * @author: hedi [14/11/2005]
 * @ref:
 */
bool A3DOutlineTextureCache::_SmoothTex(A3DDevice * pDevice, A3DTexture * pA3DTex1, DWORD dwDir, A3DTexture * pA3DTex2)
{
	IDirect3DTexture9 * pD3DTex1 = pA3DTex1->GetD3DTexture();
	IDirect3DTexture9 * pD3DTex2 = pA3DTex2->GetD3DTexture();

	if( !pD3DTex1 || !pD3DTex2 )
		return true;

	int nWidth, nHeight;
	pA3DTex1->GetDimension(&nWidth, &nHeight);
	A3DFORMAT fmt = pA3DTex1->GetFormat(0);

	if( fmt == A3DFMT_DXT1 )
	{
		// it is impossible to smooth compressed texture with good visual result, and the game will not allow outline to use small bird-view texture
		// so DXT1 means no smooth needed.

		/*
		// we force lock the compressed image and set the adjacent pixels to the same
		D3DLOCKED_RECT lockRect1;
		D3DLOCKED_RECT lockRect2;

		HRESULT hval;
		hval = pD3DTex1->LockRect(0, &lockRect1, NULL, 0);
		if( D3D_OK != hval )
			goto EXIT;
		hval = pD3DTex2->LockRect(0, &lockRect2, NULL, D3DLOCK_READONLY);
		if( D3D_OK != hval )
		{
			pD3DTex2->UnlockRect(0);
			goto EXIT;
		}

		typedef struct _DXT1_BLOCK
		{
			WORD color0;
			WORD color1;
			BYTE bt1;
			BYTE bt2;
			BYTE bt3;
			BYTE bt4;
		} DXT1_BLOCK;

		int rb = nWidth >> 2;
		int cb = nHeight >> 2;

		if( dwDir & (EL_OTL_SMOOTH_DAY_LEFT| EL_OTL_SMOOTH_NIGHT_LEFT))
		{
			// we move right edge on tex2 to left edge on tex1
			for(int i=0; i<cb; i++)
			{
				DXT1_BLOCK& block1 = ((DXT1_BLOCK *) ((BYTE *)lockRect1.pBits + i * lockRect1.Pitch))[0];
				DXT1_BLOCK& block2 = ((DXT1_BLOCK *) ((BYTE *)lockRect2.pBits + i * lockRect2.Pitch))[rb - 1];

				bool bAlpha1 = false, bAlpha2 = false;
				if( block1.color0 <= block1.color1 )
					bAlpha1 = true;
				if( block2.color0 <= block2.color1 )
					bAlpha2 = true;
				
				block1.color0 = block2.color0;
				block1.color1 = block2.color1;
				
				block1.bt1 = (block1.bt1 & 0xfc) | ((block2.bt1 & 0xc0) >> 6);
				block1.bt2 = (block1.bt2 & 0xfc) | ((block2.bt2 & 0xc0) >> 6);
				block1.bt3 = (block1.bt3 & 0xfc) | ((block2.bt3 & 0xc0) >> 6);
				block1.bt4 = (block1.bt4 & 0xfc) | ((block2.bt4 & 0xc0) >> 6);
				
				if( !bAlpha1 && bAlpha2 )
				{
					// get rid of 11
					if( (block1.bt1 & 0xc) == 0xc )
						block1.bt1 &= 0x8;
					if( (block1.bt1 & 0x30) == 0x30 )
						block1.bt1 &= 0x20;
					if( (block1.bt1 & 0xc0) == 0xc0 )
						block1.bt1 &= 0x80;

					if( (block1.bt2 & 0xc) == 0xc )
						block1.bt2 &= 0x8;
					if( (block1.bt2 & 0x30) == 0x30 )
						block1.bt2 &= 0x20;
					if( (block1.bt2 & 0xc0) == 0xc0 )
						block1.bt2 &= 0x80;

					if( (block1.bt3 & 0xc) == 0xc )
						block1.bt3 &= 0x8;
					if( (block1.bt3 & 0x30) == 0x30 )
						block1.bt3 &= 0x20;
					if( (block1.bt3 & 0xc0) == 0xc0 )
						block1.bt3 &= 0x80;

					if( (block1.bt4 & 0xc) == 0xc )
						block1.bt4 &= 0x8;
					if( (block1.bt4 & 0x30) == 0x30 )
						block1.bt4 &= 0x20;
					if( (block1.bt4 & 0xc0) == 0xc0 )
						block1.bt4 &= 0x80;
				}
			}
		}
		else // top
		{
			// we move bottom edge on tex2 to top edge on tex1
			for(int i=0; i<rb; i++)
			{
				DXT1_BLOCK& block1 = ((DXT1_BLOCK *) (lockRect1.pBits))[i];
				DXT1_BLOCK& block2 = ((DXT1_BLOCK *) ((BYTE *)lockRect2.pBits + (cb - 1) * lockRect2.Pitch))[i];

				bool bAlpha1 = false, bAlpha2 = false;
				if( block1.color0 <= block1.color1 )
					bAlpha1 = true;
				if( block2.color0 <= block2.color1 )
					bAlpha2 = true;

				block1.color0 = block2.color0;
				block1.color1 = block2.color1;

				block1.bt1 = block2.bt4;

				if( !bAlpha1 && bAlpha2 )
				{
					// get rid of 11
					if( (block1.bt2 & 0x3) == 0x3 )
						block1.bt2 &= 0x2;
					if( (block1.bt2 & 0xc) == 0xc )
						block1.bt2 &= 0x8;
					if( (block1.bt2 & 0x30) == 0x30 )
						block1.bt2 &= 0x20;
					if( (block1.bt2 & 0xc0) == 0xc0 )
						block1.bt2 &= 0x80;
					
					if( (block1.bt3 & 0x3) == 0x3 )
						block1.bt3 &= 0x2;
					if( (block1.bt3 & 0xc) == 0xc )
						block1.bt3 &= 0x8;
					if( (block1.bt3 & 0x30) == 0x30 )
						block1.bt3 &= 0x20;
					if( (block1.bt3 & 0xc0) == 0xc0 )
						block1.bt3 &= 0x80;

					if( (block1.bt4 & 0x3) == 0x3 )
						block1.bt4 &= 0x2;
					if( (block1.bt4 & 0xc) == 0xc )
						block1.bt4 &= 0x8;
					if( (block1.bt4 & 0x30) == 0x30 )
						block1.bt4 &= 0x20;
					if( (block1.bt4 & 0xc0) == 0xc0 )
						block1.bt4 &= 0x80;
				}
			}
		}

		pD3DTex1->UnlockRect(0);
		pD3DTex2->UnlockRect(0);
		*/
	}
	else
	{
		IDirect3DSurface9 * pSurface1 = NULL;
		IDirect3DSurface9 * pSurface2 = NULL;

		HRESULT hval;
		hval = pD3DTex1->GetSurfaceLevel(0, &pSurface1);
		if( D3D_OK != hval )
			goto EXIT;
		hval = pD3DTex2->GetSurfaceLevel(0, &pSurface2);
		if( D3D_OK != hval )
		{
			pSurface1->Release();
			goto EXIT;
		}

		RECT rect;
		POINT pt;
		if( dwDir & (EL_OTL_SMOOTH_DAY_LEFT| EL_OTL_SMOOTH_NIGHT_LEFT))
		{
			rect.left = nWidth - 1;
			rect.right = nWidth;
			rect.top = 0;
			rect.bottom = nHeight;
			pt.x = 0;
			pt.y = 0;
		}
		else // top
		{
			rect.left = 0;
			rect.right = nWidth;
			rect.top = nHeight - 1;
			rect.bottom = nHeight;
			pt.x = 0;
			pt.y = 0;
		}

		RECT rcDest = {pt.x, pt.y, rect.right - rect.left, rect.bottom - rect.top};
		hval = pDevice->StretchRect(pSurface2, &rect, pSurface1, &rcDest, A3DTEXF_POINT);
		
		pSurface1->Release();
		pSurface2->Release();
	}

	return true;

EXIT:
	// next time it will fail again, so we will not make it happen again.
	return true;
}