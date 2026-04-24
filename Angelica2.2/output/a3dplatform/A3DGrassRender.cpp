/*
* FILE: A3DGrassRender.cpp
*
* DESCRIPTION: 
*
* CREATED BY: Zhangyachuan (zhangyachuan000899@wanmei.com), 2012/2/29
*
* HISTORY: 
*
* Copyright (c) 2012 Archosaur Studio, All Rights Reserved.
*/

#include "A3DDevice.h"
#include "A3DEngine.h"
#include "A3DWireCollector.h"
#include "A3DFuncs.h"
#include "A3DStream.h"
#include "A3DTexture.h"
#include "A3DViewport.h"
#include "A3DCamera.h"
#include "A3DGrassRender.h"
#include "A3DGrassland.h"
#include "A3DHLSL.h"
#include "A3DVertexShader.h"
#include "A3DGrassBlock.h"
#include "a3dgrassbitsmap.h"
#include "A3DEnvironment.h"
#include "A3DTextureMan.h"
#include <memory>

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
//	Implement A3DGrassRender
//	
///////////////////////////////////////////////////////////////////////////

A3DGrassRender* A3DGrassRender::CreateRender(A3DDevice* pA3DDevice)
{
	if (pA3DDevice->TestPixelShaderVersion(3, 0))
	{
		return new A3DGrassHWInstanceRender();
	}
	else
		return new A3DGrassCollectRender();
}

A3DGrassRender::A3DGrassRender(RENDER_TYPE rt)
: m_pGrassLand(NULL)
, m_pHLSL(NULL)
, m_pA3DDevice(NULL)
, m_pA3DEngine(NULL)
, m_pVertexDecl(NULL)
, m_nGrassCount(0)
, m_nGrassBlkSize(16)
, m_Type(rt)
{

}

A3DGrassRender::~A3DGrassRender()
{
	Release();
}

bool A3DGrassRender::Init(A3DGrassLand* pGrassLand)
{
	m_pGrassLand = pGrassLand;
	m_pA3DDevice = pGrassLand->GetA3DDevice();
	m_pA3DEngine = GetA3DDevice()->GetA3DEngine();

	std::auto_ptr<A3DVertexDecl> pVertexDecl(new A3DVertexDecl);
	if (!pVertexDecl->Init(GetA3DDevice(), GetD3DVertexElement()))
		return false;

	m_pVertexDecl = pVertexDecl.release();
	return true;
}

void A3DGrassRender::Release()
{
	if (m_pA3DEngine && m_pHLSL)
	{
		m_pA3DEngine->GetA3DHLSLMan()->ReleaseShader(m_pHLSL);
		m_pHLSL = NULL;
	}

	A3DRELEASE(m_pVertexDecl);
}

bool A3DGrassRender::LoadHLSL(const char* szFile)
{
	m_pHLSL = m_pA3DEngine->GetA3DHLSLMan()->LoadShader(szFile, szFile, NULL);
    if (m_pHLSL)
    {
        A3DEnvironment* pEnvironment = m_pA3DEngine->GetA3DEnvironment();	// 取环境对象
        A3DCCDBinder* pBinder = pEnvironment->GetCCDBinder();			// 取环境对象的绑定对象
        m_pHLSL->BindCommConstData(pBinder);
    }
	return m_pHLSL != NULL;
}

bool A3DGrassRender::SetupHLSLParamsForBitsMap(A3DGrassBitsMap* pGrassBitsMap)
{
	A3DGrassType* pGrassType = pGrassBitsMap->GetGrassType();
	ASSERT( pGrassType );
	A3DVECTOR3 vecCenter = pGrassBitsMap->GetCenter();
	ARectF rectLM = pGrassBitsMap->GetRectLM();
	A3DTexture* pTerrainLM = NULL, *pTerrainLM_n = NULL;
	GetGrassLand()->GetTerrainLMAt(vecCenter, &rectLM, &pTerrainLM, &pTerrainLM_n);
	GetHLSL()->SetTexture("g_BaseSampler", pGrassType->GetTextureGrass());
	if( pTerrainLM && pTerrainLM_n )
	{
		GetHLSL()->SetTexture("g_LMDaySampler", pTerrainLM);
		GetHLSL()->SetTexture("g_LMNightSampler", pTerrainLM_n);
	}
	else
	{
		GetA3DDevice()->SetTextureColorArgs(0, A3DTA_TEXTURE, A3DTA_CURRENT);
		GetA3DDevice()->SetTextureColorOP(0, A3DTOP_SELECTARG1);
		GetA3DDevice()->SetTextureColorOP(1, A3DTOP_DISABLE);

		GetA3DDevice()->SetTextureAlphaArgs(0, A3DTA_TEXTURE, A3DTA_CURRENT);
		GetA3DDevice()->SetTextureAlphaOP(0, A3DTOP_MODULATE);
		GetA3DDevice()->SetTextureAlphaOP(1, A3DTOP_DISABLE);

		GetHLSL()->SetTexture("g_LMDaySampler", m_pA3DEngine->GetA3DTextureMan()->GetErrLMTexture());
		GetHLSL()->SetTexture("g_LMNightSampler", m_pA3DEngine->GetA3DTextureMan()->GetErrLMTexture());
	}


	float vSightRange = pGrassType->GetDefineData().vSightRange;
	A3DVECTOR4 v(rectLM.left, rectLM.top, 1.0f / rectLM.Width(), 0);
	GetHLSL()->SetValue4f("g_vGenMaskUV", &v);
	GetHLSL()->SetValue1f("g_InvSquaredSightRange", 1.0f / (vSightRange * vSightRange));
	GetHLSL()->SetValue1f("g_fSoftness", pGrassType->GetDefineData().vSoftness);
	return true;
}

bool A3DGrassRender::DestroyBlock(A3DGrassBlock* pBlock)
{
	delete pBlock;
	return true;
}

bool A3DGrassRender::Render(A3DViewport* pViewport, bool bAlphaBlend)
{
	ASSERT( GetHLSL() && "Assume init has succeed" );

	int nNumGrassType = GetGrassLand()->GetNumGrassType();

	GetA3DDevice()->SetLightingEnable(false);
	GetA3DDevice()->SetSpecularEnable(false);
	GetA3DDevice()->SetWorldMatrix(IdentityMatrix());
	GetA3DDevice()->SetTextureAddress(0, A3DTADDR_WRAP, A3DTADDR_WRAP);
	GetA3DDevice()->SetTextureAddress(1, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	GetA3DDevice()->SetTextureAddress(2, A3DTADDR_CLAMP, A3DTADDR_CLAMP);

	GetA3DDevice()->SetTextureCoordIndex(0, 0);
	GetA3DDevice()->SetTextureCoordIndex(1, 1);
	GetA3DDevice()->SetTextureCoordIndex(2, 2);

	A3DEnvironment* pEnv = m_pA3DEngine->GetA3DEnvironment();
	ASSERT( pEnv );

	const A3DEnvironment::LITMAP* pLM = pEnv->GetLightMapParam();
	A3DVECTOR4 vLMParams(pLM->MaxLight, pLM->MaxLightNight, 0, pEnv->GetDNFactor());
	GetHLSL()->SetValue4f("g_LitmapParams", &vLMParams);
	GetHLSL()->SetConstantMatrix("g_matViewProjection", pViewport->GetCamera()->GetVPTM());
	GetHLSL()->SetConstantArrayF("g_WindOffset", GetGrassLand()->GetWindOffset(), WIND_BUF_LEN);
    GetHLSL()->SetValue3f("g_vecEyePos", &pViewport->GetCamera()->GetPos());

	if (m_pVertexDecl)
		m_pVertexDecl->Appear();

	ResetGrassCount();
	if( !bAlphaBlend )
	{
		// render grasses here
		// first we render grasses that are not using alpha blending
		for(int i=0; i<nNumGrassType; i++)
		{
			A3DGrassType * pGrassType = GetGrassLand()->GetGrassType(i);

			if( pGrassType && !pGrassType->GetDefineData().bAlphaBlendEnable )
				pGrassType->RenderGrasses(pViewport);
		}
	}
	else
	{
		// then we render grasses that are using alpha blending
		for(int i=0; i<nNumGrassType; i++)
		{
			A3DGrassType * pGrassType = GetGrassLand()->GetGrassType(i);

			if( pGrassType && pGrassType->GetDefineData().bAlphaBlendEnable )
				pGrassType->RenderGrasses(pViewport);
		}
	}

	GetA3DDevice()->SetTextureAddress(1, A3DTADDR_WRAP, A3DTADDR_WRAP);
	GetA3DDevice()->SetTextureAddress(2, A3DTADDR_WRAP, A3DTADDR_WRAP);
	GetA3DDevice()->SetLightingEnable(true);
	GetA3DDevice()->SetSpecularEnable(true);

	return true;
}

bool A3DGrassRender::RawRender(int iNumVert, int iNumFace)
{
	m_pA3DDevice->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, 0, iNumVert, 0, iNumFace);
	return true;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement A3DGrassCollectRender
//	
///////////////////////////////////////////////////////////////////////////

A3DGrassCollectRender::A3DGrassCollectRender()
: base_class(RT_COLLECT)
{
}

A3DGrassCollectRender::~A3DGrassCollectRender()
{

}

const D3DVERTEXELEMENT9* A3DGrassCollectRender::GetD3DVertexElement() const
{
	static const D3DVERTEXELEMENT9 aDecls[] =
	{
		{0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
		D3DDECL_END()
	};

	return aDecls;
}

bool A3DGrassCollectRender::Init(A3DGrassLand* pGrassLand)
{
	if (!base_class::Init(pGrassLand))
		return false;

	return LoadHLSL("Shaders\\2.2\\HLSL\\grass\\grass.hlsl");
}

void A3DGrassCollectRender::Release()
{
	base_class::Release();
}

bool A3DGrassCollectRender::DoRender(A3DViewport* pViewport, A3DGrassBitsMap* pGrassBitsMap)
{
#ifdef _DEBUG
	A3DWireCollector* pWC = GetA3DDevice()->GetA3DEngine()->GetA3DWireCollector();
#endif

	A3DVECTOR3 vecCenter = pGrassBitsMap->GetCenter();
	A3DVECTOR3 vecOrigin = pGrassBitsMap->GetOrigin();
	A3DGrassType* pGrassType = pGrassBitsMap->GetGrassType();

	// first see if we need to render it.
	float vRadius = MagnitudeH(vecCenter - vecOrigin);
	if( MagnitudeH(pViewport->GetCamera()->GetPos() - vecCenter) > vRadius + pGrassType->GetDefineData().vSightRange )
		return true;

	A3DStream * pStream = GetGrassLand()->GetStream();
	if( !pStream )
		return true;

	pStream->Appear(0, false);

	SetupHLSLParamsForBitsMap(pGrassBitsMap);

	GetHLSL()->Appear(GetA3DEngine()->GetA3DEnvironment()->GetCommonConstTable());

	BYTE * pVerts;
	WORD * pIndices;

	if( !pStream->LockVertexBufferDynamic(0, 0, (BYTE **) &pVerts, 0) )
		return false;
	if( !pStream->LockIndexBufferDynamic(0, 0, (BYTE **) &pIndices, 0) )
		return false;

	int nVertCount = 0;
	int nFaceCount = 0;

	int i, s, t;
	
	const A3DGrassBitsMap::BLOCKARRAY& VisibleBlocks = pGrassBitsMap->m_VisibleBlocks;
	for(i=0; i<(int)VisibleBlocks.size(); i++)
	{
		A3DGrassBlock * pBlock = VisibleBlocks[i];

		if( pBlock && pBlock->IsBuilt() && pBlock->GetNumGrasses() && pBlock->GetGrassesVerts() )
		{
			if( !pViewport->GetCamera()->AABBInViewFrustum(pBlock->GetAABB()) )
				continue;

			float vDis = MagnitudeH(pViewport->GetCamera()->GetPos() - pBlock->GetCenter());
			if( vDis > GetGrassBlockSize() * pGrassBitsMap->GetGridSize() * 1.414f + pGrassType->GetDefineData().vSightRange )
				continue;

			int nNumGrasses = pBlock->GetNumGrasses();

			int nNumVerts = nNumGrasses * pGrassType->GetNumOneGrassVert();
			int nNumFaces = nNumGrasses * pGrassType->GetNumOneGrassFace();

			if( nVertCount + nNumVerts > GetGrassLand()->GetMaxVerts() ||
				nFaceCount + nNumFaces > GetGrassLand()->GetMaxFaces() )
			{
				if( !pStream->UnlockVertexBufferDynamic() )
					return false;
				if( !pStream->UnlockIndexBufferDynamic() )
					return false;

				// flush it once here
				GetA3DDevice()->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, nVertCount, 0, nFaceCount);
				AddGrassCount(nFaceCount);

				// lock it again;
				if( !pStream->LockVertexBufferDynamic(0, 0, (BYTE **) &pVerts, 0) )
					return false;
				if( !pStream->LockIndexBufferDynamic(0, 0, (BYTE **) &pIndices, 0) )
					return false;

				nVertCount = 0;
				nFaceCount = 0;
			}

			{
				GRASSVERTEX* pVertsB = (GRASSVERTEX*)pVerts;
				memcpy(pVerts, pBlock->GetGrassesVerts(), nNumVerts * sizeof(GRASSVERTEX));
				pVerts += nNumVerts * sizeof(GRASSVERTEX);

				// 草的抖动
				A3DAABB aabbBlock = pBlock->GetAABB();
				GRASSCLUSTER* pClusters = pBlock->GetClusters();

				const A3DVECTOR3& vHostPos = GetGrassLand()->GetHostPos();
				const float fHostRadius = 1.5f;
				const DWORD F_HOSTEFFECT   = 0x00000001;
				const DWORD F_HOSTHOVER    = 0x00000002;
				const DWORD F_BLOCKHASWAVE = 0x00000004;	// 这个储存在Cluster[0]的标志里,但是是Block的标志

				aabbBlock.Mins -= fHostRadius;
				aabbBlock.Maxs += fHostRadius;
				if(aabbBlock.IsPointIn(vHostPos) || (pClusters[0].dwFlag & F_BLOCKHASWAVE) != 0)
				{

					float fTime = GetGrassLand()->GetTimeCounter();
					int nCount = pGrassType->GetNumOneGrassVert();
					pClusters[0].dwFlag &= (~F_BLOCKHASWAVE);

					for(int ii = 0; ii < nNumGrasses; ii++)
					{
						const A3DVECTOR3& vHostDelta = GetGrassLand()->GetHostPosDelta();
						const A3DVECTOR3& vHostPress = GetGrassLand()->GetHostPress();

						A3DVECTOR3 vDelta = pClusters[ii].aabb.Center - vHostPos;
						const float fCommRad = 1.0f;
						if( fabs(vDelta.x) <= pClusters[ii].aabb.Extents.x * fCommRad &&
							fabs(vDelta.y) <= pClusters[ii].aabb.Extents.y * fCommRad * 2 &&
							fabs(vDelta.z) <= pClusters[ii].aabb.Extents.z * fCommRad)
						{
							pClusters[0].dwFlag |= F_BLOCKHASWAVE;

							float fDirDot = vDelta.x * vHostPress.x + vDelta.z * vHostPress.z;

							const float fLenH = vDelta.MagnitudeH();
							const float fInvLenH = 1.0f / fLenH;
							float fOffset = -fLenH + 1;
							if(fOffset > 1.0f)
								fOffset = 1.0f;
							else if(fOffset < 0.0f)
								goto RESTORE_WAVE;
							fOffset *= 2;
							pClusters[ii].offset = fOffset;

							if((pClusters[ii].dwFlag & F_HOSTHOVER) != 0 && fDirDot < /*0.7071f*/0.383f)
							{
								vDelta.x = pClusters[ii].vDir.x;
								vDelta.z = pClusters[ii].vDir.z;
							}
							else
							{
								pClusters[ii].dwFlag |= F_HOSTEFFECT;


								vDelta.x *= fInvLenH;
								vDelta.z *= fInvLenH;

								pClusters[ii].vDir.x = vDelta.x;
								pClusters[ii].vDir.z = vDelta.z;

							}

							pClusters[ii].dwFlag |= F_HOSTHOVER;
							pClusters[ii].vDir.y = fTime;

							int nTopIndex = nCount * ii;
							for(int n = 0; n < nCount; n++)
							{
								float soft = pVertsB[nTopIndex + n].normal.y * fOffset;
								float a = 1 - cos(atan2(soft, 1.0f));
								pVertsB[nTopIndex + n].pos.x = pVertsB[nTopIndex + n].pos.x + vDelta.x * soft;
								pVertsB[nTopIndex + n].pos.z = pVertsB[nTopIndex + n].pos.z + vDelta.z * soft;
								pVertsB[nTopIndex + n].pos.y = pVertsB[nTopIndex + n].pos.y - a; 
							}
						}
						else
						{
RESTORE_WAVE:
							pClusters[ii].dwFlag &= (~F_HOSTHOVER);
							if(pClusters[ii].dwFlag & F_HOSTEFFECT)
							{
								pClusters[0].dwFlag |= F_BLOCKHASWAVE;

								int nTopIndex = nCount * ii;
								float fElapse = fTime - pClusters[ii].vDir.y;

								for(int n = 0; n < nCount; n++)
								{
									float soft = pVertsB[nTopIndex + n].normal.y * pClusters[ii].offset * cos(fElapse * 1.5f);
									float a = 1 - cos(atan2(soft, 1.0f));
									pVertsB[nTopIndex + n].pos.x = pVertsB[nTopIndex + n].pos.x + pClusters[ii].vDir.x * soft;
									pVertsB[nTopIndex + n].pos.z = pVertsB[nTopIndex + n].pos.z + pClusters[ii].vDir.z * soft;
									pVertsB[nTopIndex + n].pos.y = pVertsB[nTopIndex + n].pos.y - a;
								}
								pClusters[ii].offset -= fElapse * 0.001f;
								if(pClusters[ii].offset <= 0)
									pClusters[ii].dwFlag &= (~F_HOSTEFFECT);
							}
						}
					}
				}
			}
#ifdef _DEBUG
			if((pBlock->GetClusters()[0].dwFlag & 4) != 0)
				pWC->AddAABB(pBlock->GetAABB(), 0xff00ff00);
#endif // _DEBUG

			for(s=0; s<pBlock->GetNumGrasses(); s++)
			{
				WORD * pOneIndices = pGrassType->GetIndicesTemplate();
				for(t=0; t<pGrassType->GetNumOneGrassFace() * 3; t++)
				{
					*pIndices = nVertCount + (*pOneIndices);

					pIndices ++;
					pOneIndices ++;
				}

				nVertCount += pGrassType->GetNumOneGrassVert();
			}

			nFaceCount += nNumFaces;
		}
	}

	if( !pStream->UnlockVertexBufferDynamic() )
		return false;
	if( !pStream->UnlockIndexBufferDynamic() )
		return false;

	if( nFaceCount > 0 )
	{
		GetA3DDevice()->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, nVertCount, 0, nFaceCount);
		AddGrassCount(nFaceCount);
	}

	GetA3DDevice()->SetTextureColorOP(0, A3DTOP_MODULATE);
	GetA3DDevice()->SetTextureColorOP(1, A3DTOP_DISABLE);
	GetA3DDevice()->SetTextureAlphaOP(0, A3DTOP_MODULATE);
	GetA3DDevice()->SetTextureAlphaOP(1, A3DTOP_DISABLE);

#ifdef _DEBUG
	GetA3DDevice()->SetFillMode(A3DFILL_SOLID);
#endif // _DEBUG

	return true;
}

A3DGrassBlock* A3DGrassCollectRender::CreateBlock()
{
	return new A3DGrassBlock();
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement A3DGrassHWInstanceRender
//	
///////////////////////////////////////////////////////////////////////////

A3DGrassHWInstanceRender::A3DGrassHWInstanceRender()
: base_class(RT_HWINSTANCE)
{
	SetGrassBlkSizeBig();
}

A3DGrassHWInstanceRender::~A3DGrassHWInstanceRender()
{

}

const D3DVERTEXELEMENT9* A3DGrassHWInstanceRender::GetD3DVertexElement() const
{
	static const D3DVERTEXELEMENT9 aDecls[] =
	{
		{0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
		{1, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2},
		{1, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3},
		{1, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4},
		D3DDECL_END()
	};

	return aDecls;
}

bool A3DGrassHWInstanceRender::Init(A3DGrassLand* pGrassLand)
{
	if (!base_class::Init(pGrassLand))
		return false;

	return LoadHLSL("Shaders\\2.2\\HLSL\\grass\\grass_instance.hlsl");
}

void A3DGrassHWInstanceRender::Release()
{
	base_class::Release();
}

A3DGrassBlock* A3DGrassHWInstanceRender::CreateBlock()
{
	return new A3DGrassBlockHW();
}

//	Render dispatch function used when A3DGrassBitsMap render
bool A3DGrassHWInstanceRender::DoRender(A3DViewport* pViewport, A3DGrassBitsMap* pGrassBitsMap)
{
	return InternalRender(pViewport, pGrassBitsMap);
}

bool A3DGrassHWInstanceRender::InternalRender(A3DViewport* pViewport, A3DGrassBitsMap* pGrassBitsMap)
{
	A3DGrassType* pGrassType = pGrassBitsMap->GetGrassType();
	const float fMaxRenderDist = pGrassBitsMap->GetMaxRenderDist();
	const float fSightRange = pGrassType->GetDefineData().vSightRange;

	SetupHLSLParamsForBitsMap(pGrassBitsMap);

    GetHLSL()->Appear(GetA3DEngine()->GetA3DEnvironment()->GetCommonConstTable());
	for (A3DGrassBitsMap::BLOCKARRAY::const_iterator itr = pGrassBitsMap->GetVisibleBlocks().begin()
		; itr != pGrassBitsMap->GetVisibleBlocks().end()
		; ++itr)
	{
		A3DGrassBlock* pBlock = *itr;

		if (!pBlock || !pBlock->IsBuilt() || !pBlock->GetNumGrasses() || !pBlock->GetInstStream())
			continue;

		if (!pViewport->GetCamera()->AABBInViewFrustum(pBlock->GetAABB()))
			continue;

		//	Check if block is out of sight
		float vDis = a3d_MagnitudeH(pViewport->GetCamera()->GetPos() - pBlock->GetCenter());
		if (vDis > fMaxRenderDist)
			continue;

		int nRenderGrassNum = pBlock->GetRenderGrassNum();
		//if (GetGrassLand()->GetLODByDist())
		//{
		//	nRenderGrassNum = int((1.25f - vDis / fSightRange) * pBlock->GetRenderGrassNum());

		//	a_Clamp(nRenderGrassNum, 0, pBlock->GetRenderGrassNum());
		//	if (nRenderGrassNum == 0)
		//		continue;
		//}

		A3DStream* pStreamTemplate = pGrassType->GetStreamTemplate();
		A3DStream* pStreamInstance = pBlock->GetInstStream();

		ASSERT ( pStreamTemplate && pStreamInstance );

		// Set up the geometry data stream
		pStreamTemplate->Appear(0, false);
		//GetA3DDevice()->SetStreamSourceFreq(0, (D3DSTREAMSOURCE_INDEXEDDATA | nRenderGrassNum));
		GetA3DDevice()->EnableInstancing(0, nRenderGrassNum);

		// Set up the instance data stream
		pStreamInstance->AppearVertexOnly(1, false);
		//GetA3DDevice()->GetD3DDevice()->SetStreamSourceFreq(1, (D3DSTREAMSOURCE_INSTANCEDATA | 1));
		GetA3DDevice()->EnableInstancing(1, 0);

		RawRender(pGrassType->GetNumOneGrassVert(), pGrassType->GetNumOneGrassFace());

		// grass number rendered
		AddGrassCount(pGrassType->GetNumOneGrassFace() * nRenderGrassNum);
	}

	//GetA3DDevice()->GetD3DDevice()->SetStreamSourceFreq(0, 1);
	//GetA3DDevice()->GetD3DDevice()->SetStreamSourceFreq(1, 1);
	GetA3DDevice()->DisableInstancing(0);
	GetA3DDevice()->DisableInstancing(1);
	return true;
}