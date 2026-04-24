/*
 * FILE: ELTerrainOutline2.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Wangkuiwu, 2005/6/22
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */
#include "StdAfx.h"
#include "A3DDevice.h"
#include "A3DEngine.h"
#include "A3DTextureMan.h"
#include "A3DShadowMap.h"
#include "A3DFont.h"
#include "A3DViewport.h"
#include "A3DTexture.h"
#include "A3DStream.h"
#include "A3DTerrain2.h"
#include "A3DTerrain2Blk.h"
#include "AFileImage.h"
#include "A3DCameraBase.h"
#include "A3DFuncs.h"

#include <io.h>

#include "A3DTerrainOutline2.h"
#include "A3DOutlineTexture.h"
#include "A3DOutlineQuadTree.h"
#include "A3DHLSL.h"
#include "A3DEnvironment.h"

//#include "ShellSDK/EncStr.h"
#define _EA(x) (x)
#define OUTLINE_VIEW_DIST        2000
#define OUTLINE_VIEW_DIST_LEVEL0 800
#define OUTLINE_VIEW_DIST_LEVEL1 1200

static const D3DVERTEXELEMENT9 s_aVertexDecl[] =
{
    {0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
    {0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
    {0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 1},
    {0, 20, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
    D3DDECL_END()
};

A3DTerrainOutline2::A3DTerrainOutline2()
{
	m_pA3DDevice		= NULL;
	m_pTerrain2         = NULL;

	m_nWorldWidth       = 0;
	m_nWorldHeight      = 0;
	m_x                 = 0.0f;
	m_z                 = 0.0f;


	m_nMaxReplaceTerrainFace = 0;
	m_nMaxReplaceTerrainVert = 0;
	
	m_pReplaceTerrainIndices = NULL;
	m_pReplaceTerrainVerts = NULL;
	
	m_pFakeTerrainStream = NULL;
	
	m_BlockVisibleRect.Clear();

	m_pOutlineTexCache	= NULL;
	m_dwFrame       = 0;
	
	m_pTerrain2BlkInfo = NULL;
	m_iMinLevel        = 0;
	m_bReplaceTerrain  = false;

	m_pDetailTexture = NULL;
	m_bNeedUpdate    = true;

    m_pHLSLOutline = NULL;
    m_pHLSLReplace = NULL;
	m_pHLSLReplaceShadow = NULL;
    m_pVertDecl = NULL;

	//m_fDNFactor = 0.0f;   // day as default 
	//m_fDNFactor = 1.0f;
	m_fDNFactor = 0.5f;

	m_iCurViewScheme = 4; // furthest as default

    m_OutlineTexLevel[0] = 8;  //level0
    m_OutlineTexLevel[1] = 11;
    m_OutlineTexLevel[2] = 4;  //level1
    m_OutlineTexLevel[3] = 6;
    m_OutlineTexLevel[4] = 2;  //level2
    m_OutlineTexLevel[5] = 3;
    m_OutlineTexLevel[6] = 1;  //level3
    m_OutlineTexLevel[7] = 2;
    m_OutlineTexLevel[8] = 1;  //level4
	m_OutlineTexLevel[9] = 1;
}



A3DTerrainOutline2::~A3DTerrainOutline2()
{
	Release();
}



bool A3DTerrainOutline2::Init(A3DDevice * pA3DDevice,  const char * szTexDir, const char * szTileDir, const char * szDetailTex,
							  A3DTerrain2* pTerrain2, unsigned int nWTileCount, unsigned int nHTileCount, int nTileGridLen)
{
	//@note : release first. By Kuiwu[21/9/2005]
	Release();

	//@note : pre-condition. By Kuiwu[21/9/2005]
	//@todo : Ugly. Remove the hard-code. By Kuiwu[21/9/2005]
	assert(nWTileCount <= 8 && nHTileCount <= 11 && _EA("invalid input"));

	A3DOutlineTexture::SetTileGridLen(nTileGridLen);


	m_pA3DDevice = pA3DDevice;
	m_pTerrain2 = pTerrain2;
	m_szTexDir = szTexDir;
	//@todo : fixme, using the input param By Kuiwu[26/8/2005]
	//m_szTexDir = _EA("E:\\完美世界\\element\\loddata\\512\\");

	m_szTileDir = szTileDir;

	//set the level resolution
	//@note : ugly but efficient :). By Kuiwu[21/9/2005]
	m_OutlineTexLevel[0] = nWTileCount;                          //level0
	m_OutlineTexLevel[1] = nHTileCount;
	m_OutlineTexLevel[2] = (int)ceil(m_OutlineTexLevel[0] *0.5f);//level1
	m_OutlineTexLevel[3] = (int)ceil(m_OutlineTexLevel[1] *0.5f);
	m_OutlineTexLevel[4] = (int)ceil(m_OutlineTexLevel[2] *0.5f);//level2
	m_OutlineTexLevel[5] = (int)ceil(m_OutlineTexLevel[3] *0.5f);
	m_OutlineTexLevel[6] = (int)ceil(m_OutlineTexLevel[4] *0.5f);//level3
	m_OutlineTexLevel[7] = (int)ceil(m_OutlineTexLevel[5] *0.5f);
	m_OutlineTexLevel[8] = (int)ceil(m_OutlineTexLevel[6] *0.5f);//level4
	m_OutlineTexLevel[9] = (int)ceil(m_OutlineTexLevel[7] *0.5f);


	// now try to open one olm file
	char path[MAX_PATH];
	sprintf(path, _EA("%s%d.olm"), m_szTileDir, 1);
	AFileImage fileImage;
	if( !fileImage.Open(path, AFILE_OPENEXIST | AFILE_TEMPMEMORY) )
		return false;
	DWORD dwReadLen;
	DWORD dwMagic;
	if (!fileImage.Read(&dwMagic, sizeof(DWORD), &dwReadLen)) 
	{
		fileImage.Close();
		return false;
	}
	DWORD dwVersion;
	if (!fileImage.Read(&dwVersion, sizeof(DWORD), &dwReadLen)) 
	{
		fileImage.Close();
		return false;
	}
	if (dwMagic != OUTLINE_TILE_MAGIC || dwVersion != OUTLINE_TILE_VERSION) 
	{
		fileImage.Close();
		return false;
	}
	int nVert, nMaxFace, nQNodes;
	A3DVECTOR3* pVertPos = NULL;
	A3DOutlineQuadNode* pQNodes = NULL;
	if( !fileImage.Read(&nVert, sizeof(int), &dwReadLen) )
	{
		fileImage.Close();
		return false;
	}
	pVertPos = new A3DVECTOR3[nVert];
	if (!fileImage.Read(pVertPos, sizeof(A3DVECTOR3)*nVert, &dwReadLen)) 
	{
		delete [] pVertPos;
		pVertPos = NULL;
		fileImage.Close();
		return false;
	}
	if (!fileImage.Read(&nMaxFace, sizeof(int), &dwReadLen)) 
	{
		delete [] pVertPos;
		pVertPos = NULL;
		fileImage.Close();
		return false;
	}
	if (!fileImage.Read(&nQNodes, sizeof(int), &dwReadLen)) 
	{
		delete [] pVertPos;
		pVertPos = NULL;
		fileImage.Close();
		return false;
	}
	pQNodes = new A3DOutlineQuadNode[nQNodes];
	int idx;
	
	struct export_head{
		WORD ix, iz;
		WORD iLen;
		WORD iActiveFlag;
		WORD iVertCenter;
		int iParent;
		int iChildren[4];
	} ;
	
	//export_head  _head;
	for (idx = 0; idx < nQNodes; idx++) {
		if (!fileImage.Read(&pQNodes[idx], sizeof(export_head), &dwReadLen)) 
		{
			assert(0);
			delete [] pVertPos;
			pVertPos = NULL;
			delete [] pQNodes;
			pQNodes = NULL;
			fileImage.Close();
			return false;
		}
		if (pQNodes[idx].iParent == -1)
			break;
	}
	
	if (idx == nQNodes)
	{
		delete [] pVertPos;
		pVertPos = NULL;
		delete [] pQNodes;
		pQNodes = NULL;
		fileImage.Close();
		return false;		// No root node, error olm file.
	}

	// Get outline tile width and height from head node
	g_iOutlineTileWidth = pQNodes[idx].iLen * A3DOutlineTexture::GetTileGridLen();
	g_iOutlineTileHeight = pQNodes[idx].iLen * A3DOutlineTexture::GetTileGridLen();

	delete [] pVertPos;
	pVertPos = NULL;
	delete [] pQNodes;
	pQNodes = NULL;
	fileImage.Close();


	m_fLevelEndDist[0]  = OUTLINE_VIEW_DIST_LEVEL0;
	m_fLevelEndDist[1]  = OUTLINE_VIEW_DIST_LEVEL1;
	m_fLevelEndDist[2]  = OUTLINE_VIEW_DIST;
	m_fLevelEndDist[3]  = 5000.0F;
	m_fLevelEndDist[4]  = 10000.0F;

	m_nWorldWidth = g_iOutlineTileWidth * m_OutlineTexLevel[0];
	m_nWorldHeight = g_iOutlineTileHeight * m_OutlineTexLevel[1];

	m_x  = (float)(-(m_nWorldWidth >>1));
	m_z  = (float)(m_nWorldHeight>>1);

	m_OutlineTexSize[0] = (float)g_iOutlineTileWidth;
	m_OutlineTexSize[1] = (float)g_iOutlineTileHeight;
	for (int i = 1; i < OUTLINE_MAX_LEVEL; i++) {
		m_OutlineTexSize[i*2] = m_OutlineTexSize[(i-1)*2]*2;	
		m_OutlineTexSize[i*2+1] = m_OutlineTexSize[(i-1)*2+1]*2;
	} 
	

	if (!m_pOutlineTexCache) {
		m_pOutlineTexCache = new A3DOutlineTextureCache;
		m_pOutlineTexCache->Init(this);
	}
	

	m_dwFrame  = 0;
	m_iMinLevel = 0;
	m_bReplaceTerrain = false;
	
	int iMaskBlock = m_pTerrain2->GetMaskGrid() / m_pTerrain2->GetBlockGrid();
	iMaskBlock *= iMaskBlock;
	m_nMaxReplaceTerrainVert  = (m_pTerrain2->GetBlockGrid() + 1) * (m_pTerrain2->GetBlockGrid() + 1) * iMaskBlock;
	m_nMaxReplaceTerrainFace  = m_pTerrain2->GetBlockGrid() * m_pTerrain2->GetBlockGrid() * 2 * iMaskBlock;


	//load detail texture
	if (NULL == szDetailTex || strlen(szDetailTex) <= 0)
		return false;
	if( !m_pA3DDevice->GetA3DEngine()->GetA3DTextureMan()->LoadTextureFromFile(szDetailTex, &m_pDetailTexture, A3DTF_NODOWNSAMPLE) )
		return false;


	m_nExpectCacheSize[0] = 8;
	m_nExpectCacheSize[1] = 12;
	m_nExpectCacheSize[2] = 12;
	m_nExpectCacheSize[3] = 16;
	m_nExpectCacheSize[4] = 20;
	m_iCurViewScheme = 4;

	m_bNeedUpdate = true;
	
    AString strShaderFile = "shaders\\2.2\\HLSL\\Terrain\\terrain_outline.hlsl";

    m_pHLSLOutline = m_pA3DDevice->GetA3DEngine()->GetA3DHLSLMan()->LoadShader(strShaderFile, "vs_main", strShaderFile, "ps_main", "_FOG_", 0);
    if (!m_pHLSLOutline)
    {
        a_LogOutput(1, "A3DTerrainOutline2::Init, Failed to load terrain outline shader!");
        return false;
    }

    m_pHLSLReplace = m_pA3DDevice->GetA3DEngine()->GetA3DHLSLMan()->LoadShader(strShaderFile, "vs_main", strShaderFile, "ps_main_detail", "_FOG_;", 0);
    if (!m_pHLSLReplace)
    {
        a_LogOutput(1, "A3DTerrainOutline2::Init, Failed to load terrain replace shader!");
    }
	AString strMacro;
	if(A3DShadowMap::SupportHWPCF(m_pA3DDevice))
		strMacro = "_SHADOW_;_HWPCF_;_FOG_";
	else
		strMacro = "_SHADOW_;_FOG_";
	m_pHLSLReplaceShadow = m_pA3DDevice->GetA3DEngine()->GetA3DHLSLMan()->LoadShader(strShaderFile, "vs_main", strShaderFile, "ps_main_detail", strMacro, 0);
	if (!m_pHLSLReplaceShadow)
	{
		a_LogOutput(1, "A3DTerrainOutline2::Init, Failed to load terrain replace shader with shadow!");
        return false;
	}


    A3DEnvironment* pEnvironment = m_pA3DDevice->GetA3DEngine()->GetA3DEnvironment();	// 取环境对象
    A3DCCDBinder* pBinder = pEnvironment->GetCCDBinder();			// 取环境对象的绑定对象
    if (m_pHLSLOutline) m_pHLSLOutline->BindCommConstData(pBinder);
    if (m_pHLSLReplace) m_pHLSLReplace->BindCommConstData(pBinder);
	if (m_pHLSLReplaceShadow) m_pHLSLReplaceShadow->BindCommConstData(pBinder);
    m_pVertDecl = new A3DVertexDecl;
    if (!m_pVertDecl->Init(pA3DDevice, s_aVertexDecl))
    {
        a_LogOutput(1, "A3DTerrainOutline2::Init, Failed to create vertex declaration!");
        return false;
    }
	return true;
}

void A3DTerrainOutline2::Release()
{
	if( m_pDetailTexture )
	{
		m_pA3DDevice->GetA3DEngine()->GetA3DTextureMan()->ReleaseTexture(m_pDetailTexture);
		m_pDetailTexture = NULL;
	}


	if (m_pOutlineTexCache) {
		delete m_pOutlineTexCache;
		m_pOutlineTexCache = NULL;
	}


	
	if (m_pTerrain2BlkInfo) {
		delete[]  m_pTerrain2BlkInfo;
		m_pTerrain2BlkInfo = NULL;
	}

	_FreeReplaceTerrainBuf();

    if (m_pA3DDevice)
    {
        if (m_pHLSLOutline)
        {
            m_pA3DDevice->GetA3DEngine()->GetA3DHLSLMan()->ReleaseShader(m_pHLSLOutline);
            m_pHLSLOutline = NULL;
        }
        if (m_pHLSLReplace)
        {
            m_pA3DDevice->GetA3DEngine()->GetA3DHLSLMan()->ReleaseShader(m_pHLSLReplace);
            m_pHLSLReplace = NULL;
        }
		if(m_pHLSLReplaceShadow)
		{
			m_pA3DDevice->GetA3DEngine()->GetA3DHLSLMan()->ReleaseShader(m_pHLSLReplaceShadow);
			m_pHLSLReplaceShadow = NULL;

		}
    }
    A3DRELEASE(m_pVertDecl);
    m_pTerrain2 = NULL;
}




void A3DTerrainOutline2::SetReplaceTerrain(bool bVal)
{
	m_bReplaceTerrain = bVal;
	if (m_bReplaceTerrain) {
		_AllocReplaceTerrainBuf();
	}
	else{
		_FreeReplaceTerrainBuf();
	}
}


void A3DTerrainOutline2::_AllocReplaceTerrainBuf()
{
	if((m_pReplaceTerrainVerts != NULL) && (m_pReplaceTerrainIndices != NULL)) {
		return;
	}
	
	m_pReplaceTerrainVerts = new A3DVECTOR3[m_nMaxReplaceTerrainVert];
	assert(m_pReplaceTerrainVerts != NULL);
	m_pReplaceTerrainIndices  = new WORD[m_nMaxReplaceTerrainFace*3];
	assert(m_pReplaceTerrainIndices != NULL);

	m_pFakeTerrainStream = new A3DStream;
	if(!m_pFakeTerrainStream->Init(m_pA3DDevice, A3DVT_LVERTEX, m_nMaxReplaceTerrainVert,
		m_nMaxReplaceTerrainFace*3, A3DSTRM_REFERENCEPTR, A3DSTRM_REFERENCEPTR)){
			assert(0);
			return;
		}
	
	
}

void A3DTerrainOutline2::_FreeReplaceTerrainBuf()
{
	if (m_pReplaceTerrainVerts) {
		delete[] m_pReplaceTerrainVerts;
		m_pReplaceTerrainVerts = NULL;
	}
	if (m_pReplaceTerrainIndices) {
		delete[] m_pReplaceTerrainIndices;
		m_pReplaceTerrainIndices = NULL;
	}

	if (m_pFakeTerrainStream) {
		m_pFakeTerrainStream->Release();
		delete m_pFakeTerrainStream;
		m_pFakeTerrainStream = NULL;
	}
}



void A3DTerrainOutline2::Update(A3DCameraBase * pCamera, const A3DVECTOR3& vecCenter)
{
	A3DVECTOR3  dir = pCamera->GetDirH();
	A3DVECTOR3  pos = vecCenter;
	
	bool   bVisibleChange = false;
	ARectI rcVisible; 
	m_pTerrain2->CalcAreaInBlocks(pos, m_pTerrain2->GetViewRadius(), rcVisible );
	A3DTerrain2::ACTBLOCKS * pActBlocks = m_pTerrain2->GetActiveBlocks();
	
	

	if ((m_bNeedUpdate) || (rcVisible != m_BlockVisibleRect) ){
		if (rcVisible.Width() * rcVisible.Height() != 
			m_BlockVisibleRect.Width()* m_BlockVisibleRect.Height()) {
			
			if (m_pTerrain2BlkInfo) {
				delete[] m_pTerrain2BlkInfo;
			}
			m_pTerrain2BlkInfo = new TerrainBlockInfo[rcVisible.Width()*rcVisible.Height()];
		}
		m_BlockVisibleRect = rcVisible;
		
		//increase frame
		m_dwFrame++;
		//activate 
		m_pOutlineTexCache->ActivateTile(pos, m_fLevelEndDist[m_iMinLevel+2], m_dwFrame);

		//view
		m_pOutlineTexCache->CreateView(m_dwFrame, pos, m_fLevelEndDist[m_iMinLevel+2]);
		//m_pOutlineTexCache->CreateView(m_dwFrame, pos, m_fLevelEndDist[m_iMinLevel+1]);
		

		bVisibleChange = true;
	}

	bool bBlkChange;
	_CheckBlkInfo(pActBlocks, m_BlockVisibleRect, bBlkChange);
	if (m_bNeedUpdate || bVisibleChange || bBlkChange) {
		m_pOutlineTexCache->Stitch(m_dwFrame, pActBlocks);
	}
	m_bNeedUpdate = false;

}

void A3DTerrainOutline2::_CheckBlkInfo(A3DTerrain2::ACTBLOCKS *pActBlocks, const ARectI& rcVisible, bool& bBlkChange)
{
	bBlkChange = false;

	int r, c;
	A3DTerrain2Block * pBlock;

#define   TV_INDEX(c, r)  ((r- rcVisible.top) * rcVisible.Width() + (c- rcVisible.left))

	for (r = rcVisible.top; r < rcVisible.bottom; r++) {
		for (c = rcVisible.left;  c < rcVisible.right; c++ ) {
			pBlock = pActBlocks->GetBlock(r, c, false);
			bool bReady = (pBlock != NULL);
			int  tvIndex = TV_INDEX(c, r);
			int iLod = -1;
			if (bReady) {
				iLod = pBlock->GetLODLevel();
			}
			if (!m_pTerrain2BlkInfo[tvIndex].Equal(r, c, bReady, iLod)) {
				m_pTerrain2BlkInfo[tvIndex].Set(r, c, bReady, iLod);
				bBlkChange =true;		
			}
		}
	}
#undef  TV_INDEX	


}

void A3DTerrainOutline2::_RenderReplaceTerrainHLSL(A3DViewport* pViewport)
{
    if (!m_bReplaceTerrain) 
    {
        return;
    }
    assert(m_pHLSLReplace != NULL || m_pHLSLReplaceShadow != NULL);

    if (m_pDetailTexture)
        m_pDetailTexture->Appear(2);
    m_pA3DDevice->SetTextureFilterType(2, A3DTEXF_LINEAR);

    //	Update terrain block visibility info
    m_pTerrain2->DoBlocksCull(pViewport);

    //make sure buffer is ready
    assert((m_pReplaceTerrainIndices != NULL) &&(m_pReplaceTerrainVerts != NULL));
    A3DTerrain2Render * pTer2Render = m_pTerrain2->GetTerrainRender();
    int nMesh = pTer2Render->GetRenderMeshNum();
 
    for (int i = 0; i < nMesh; i++)
    {
        int nVert, nIndex;
        pTer2Render->GetRenderMesh(i, (BYTE*)m_pReplaceTerrainVerts, sizeof(A3DVECTOR3),
            m_pReplaceTerrainIndices, &nVert, &nIndex);
        if ((nVert <=0)  || (nIndex <= 0) ){
            continue;
        }
        m_pOutlineTexCache->RenderReplaceTerrain(m_dwFrame, m_iMinLevel, m_pReplaceTerrainVerts, nVert,
            m_pReplaceTerrainIndices, nIndex);
    }

    if (m_pDetailTexture)
        m_pDetailTexture->Disappear(2);
}

void A3DTerrainOutline2::Render2(A3DViewport * pViewport, bool bShadowMap)
{
    if (!pViewport)
        return;

    if (m_pHLSLOutline == NULL || m_pHLSLReplace == NULL || m_pVertDecl == NULL)
        return;

	A3DShadowMap* pShadowMap = m_pA3DDevice->GetA3DEngine()->GetA3DShadowMap();
	if(!pShadowMap)
		bShadowMap = false;


	A3DHLSL* pReplaceHLSL = 0;
	if(bShadowMap)
		pReplaceHLSL = m_pHLSLReplaceShadow;
	else
		pReplaceHLSL = m_pHLSLReplace;

    m_pA3DDevice->SetTextureAddress(0, A3DTADDR_CLAMP, A3DTADDR_CLAMP);

    bool bFogEnableOld = m_pA3DDevice->GetFogEnable();
    bool bAlphaTestEnable = m_pA3DDevice->GetAlphaTestEnable();
    bool bAlphaBlendEnable = m_pA3DDevice->GetAlphaBlendEnable();
    A3DCULLTYPE cullOld = m_pA3DDevice->GetFaceCull();
    m_pA3DDevice->SetAlphaBlendEnable(false);
    m_pA3DDevice->SetAlphaTestEnable(false);
    m_pA3DDevice->SetFaceCull(A3DCULL_NONE);
    m_pVertDecl->Appear();
    if (m_fDNFactor < 1.0f && m_fDNFactor > 0.0f)
    {
        m_pHLSLOutline->SetValue1f("g_fDNFactor", m_fDNFactor);
        pReplaceHLSL->SetValue1f("g_fDNFactor", m_fDNFactor);
        m_pA3DDevice->SetTextureFilterType(0, A3DTEXF_LINEAR);
        m_pA3DDevice->SetTextureFilterType(1, A3DTEXF_LINEAR);
    }
    else
    {
        m_pHLSLOutline->SetValue1f("g_fDNFactor", 0.0f);
        pReplaceHLSL->SetValue1f("g_fDNFactor", 0.0f);
    }

    if (m_pA3DDevice->GetA3DEngine()->GetA3DEnvironment()->GetFogParam()->bGlobalFogEnable)
    {
        m_pA3DDevice->SetFogEnable(false);
    }
    m_pHLSLOutline->SetConstantMatrix("g_matViewProjection", pViewport->GetCamera()->GetVPTM());
    pReplaceHLSL->SetConstantMatrix("g_matViewProjection", pViewport->GetCamera()->GetVPTM());
    m_pHLSLOutline->SetValue3f("g_vecEyePos", &pViewport->GetCamera()->GetPos());
    pReplaceHLSL->SetValue3f("g_vecEyePos", &pViewport->GetCamera()->GetPos());
    pReplaceHLSL->SetValue1f("g_fScaleDetail", 88.0f);

	//设置阴影
	if(bShadowMap)
	{
		using namespace A3D;
		pReplaceHLSL->SetConstantMatrix("g_matLightWVP", pShadowMap->GetShadowMatrix());
		A3DVECTOR4 vShadowMapSize((float)pShadowMap->GetShadowMapSize(), 1, 0.f, 0.f);
		pReplaceHLSL->SetValue4f("g_SettingShadowMapSize", &vShadowMapSize);
		pReplaceHLSL->SetTexture("g_ShadowMapSampler", pShadowMap->GetShadowMapTexture());
		pReplaceHLSL->SetTexture("g_ShadowMapFloat", pShadowMap->GetShadowMapFloatTexture());
		const int iShadowMapSlot = 3;
		m_pA3DDevice->SetTextureAddress(iShadowMapSlot, A3DTADDR_BORDER, A3DTADDR_BORDER);
		m_pA3DDevice->SetSamplerState(iShadowMapSlot, D3DSAMP_BORDERCOLOR,  0xffffffff);

		const int iShadowMapFloatSlot = 4;
		m_pA3DDevice->SetTextureAddress(iShadowMapFloatSlot, A3DTADDR_BORDER, A3DTADDR_BORDER);
		m_pA3DDevice->SetSamplerState(iShadowMapFloatSlot, D3DSAMP_BORDERCOLOR,  0xffffffff);

		if (pShadowMap->IsUsingHWPcf() || pShadowMap->IsUsingVSM())
		{
			m_pA3DDevice->SetTextureFilterType(iShadowMapSlot, A3DTEXF_LINEAR);
			m_pA3DDevice->SetTextureFilterType(iShadowMapFloatSlot, A3DTEXF_LINEAR);
		}
		else
		{
			m_pA3DDevice->SetTextureFilterType(iShadowMapSlot, A3DTEXF_POINT);
			m_pA3DDevice->SetTextureFilterType(iShadowMapFloatSlot, A3DTEXF_POINT);
		}
	}



    m_pHLSLOutline->Appear(m_pA3DDevice->GetA3DEngine()->GetA3DEnvironment()->GetCommonConstTable());

    m_pOutlineTexCache->Render(m_dwFrame, pViewport->GetCamera());

    if (m_bReplaceTerrain) 
    {
        pReplaceHLSL->Appear(m_pA3DDevice->GetA3DEngine()->GetA3DEnvironment()->GetCommonConstTable());
        _RenderReplaceTerrainHLSL(pViewport);
        pReplaceHLSL->Disappear();
    }

    m_pA3DDevice->SetFogEnable(bFogEnableOld);

    m_pA3DDevice->SetAlphaBlendEnable(bAlphaTestEnable);
    m_pA3DDevice->SetAlphaTestEnable(bAlphaBlendEnable);
    m_pA3DDevice->SetTextureAddress(0, A3DTADDR_WRAP, A3DTADDR_WRAP);
    m_pA3DDevice->SetFaceCull(cullOld);
}

void A3DTerrainOutline2::ZPrePass(A3DViewport * pViewport)
{
	if (!pViewport)
	{
		return;
	}
	m_pA3DDevice->ClearPixelShader();
	m_pA3DDevice->ClearVertexShader();

	m_pA3DDevice->SetWorldMatrix(IdentityMatrix());

	m_pOutlineTexCache->ZPrePass(m_dwFrame, pViewport->GetCamera());
	if (m_bReplaceTerrain) {
		_RenderReplaceTerrainHLSL(pViewport);
	}
}













