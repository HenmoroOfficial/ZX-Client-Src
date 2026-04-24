/*
 * FILE: d:\Dev\Projects2.2\A3DPlatform\A3DMirrorPlane.cpp
 *
 * DESCRIPTION: The renderer used to draw mirror plane objects.
 *
 * CREATED BY: LiChen, 2012/8/9
 *
 * HISTORY: 
 *
 * Copyright (c) 2012 Perfect World Entertainment Inc, All Rights Reserved.
 */

#include "ABaseDef.h"
#include "A3DPI.h"
#include "A3DHLSL.h"
#include "A3DStream.h"
#include "A3DCamera.h"
#include "A3DViewport.h"
#include "A3DEngine.h"
#include "A3DMacros.h"
#include "A3DEnvironment.h"
#include "A3DVertexShader.h"
#include "A3DMirrorPlane.h"
#include "A3DMirrorPlaneRender.h"



///////////////////////////////////////////////////////////////////////////
//	
//	Class A3DMirrorPlaneRender
//	
///////////////////////////////////////////////////////////////////////////


A3DMirrorPlaneRender::A3DMirrorPlaneRender()
	: m_pA3DEngine							(NULL)
	, m_pMirrorRender						(NULL)
	, m_pShaderVertexColor					(NULL)
	, m_pShaderVertexColorWithAlphaMask		(NULL)
	, m_pShaderLightMap						(NULL)
	, m_pShaderLightMapWithAlphaMask		(NULL)
	, m_pVertDecl							(NULL)
{
}

A3DMirrorPlaneRender::A3DMirrorPlaneRender( A3DMirrorPlaneRender& Other )
{
}

A3DMirrorPlaneRender::~A3DMirrorPlaneRender()
{
}

bool A3DMirrorPlaneRender::Init( A3DEngine* pA3DEngine )
{
	m_pA3DEngine = pA3DEngine;

	if( !_CreateDeviceResources() )
	{
		return false;
	}

	A3DDevice* pA3DDevice = m_pA3DEngine->GetA3DDevice();

	m_pMirrorRender = new A3DMirrorRender;
	if( !m_pMirrorRender->Init( pA3DDevice ) )
	{
		A3DRELEASE( m_pMirrorRender );
		return false;
	}

	return true;
}

void A3DMirrorPlaneRender::Release()
{
	A3DRELEASE( m_pMirrorRender );

	_ReleaseDeviceResources();

	m_pA3DEngine = NULL;
}

// Registers a mirror plane which is ready for rendering.
bool A3DMirrorPlaneRender::RegisterRenderMirrorPlane( A3DViewport* pViewport, A3DMirrorPlane* pMirrorPlane )
{
	m_PtrArrayMirrorPlanes.Add( pMirrorPlane );
	return true;
}

// Renders all mirror planes.
void A3DMirrorPlaneRender::Render( A3DViewport* pViewport, LPRENDERFORMIRROR fnRenderMirrorCallback, void* pParam )
{
	if( m_pMirrorRender == NULL )
		return;

	const int nNumMirrorPlanes = m_PtrArrayMirrorPlanes.GetSize();
	if( nNumMirrorPlanes <= 0 )
		return;

	A3DDevice* pA3DDevice = m_pA3DEngine->GetA3DDevice();

	for( int nPlane=0; nPlane<nNumMirrorPlanes; ++nPlane )
	{
		A3DMirrorPlane* pMirrorPlane = m_PtrArrayMirrorPlanes[ nPlane ];

		// Draws reflection target for this mirror plane.
		A3DPLANE ReflectionPlane = pMirrorPlane->GetReflectionPlane();
		m_pMirrorRender->Render( pViewport, &ReflectionPlane, fnRenderMirrorCallback, pParam );
		A3DRenderTarget* pReflectionTarget = m_pMirrorRender->GetMirrorRT();

		// Draws the mesh of mirror plane itself.
		pViewport->Active();

		pMirrorPlane->m_pStream->Appear( 0, false );

		m_pVertDecl->Appear();

		A3DHLSL* pShader = NULL;
		const bool bEnableAlphaMaskTexture = ( pMirrorPlane->m_pAlphaMaskTexture != NULL );
		if( pMirrorPlane->m_eMode == A3DMirrorPlane::MODE_LIGHT_MAP )
		{
			pShader = bEnableAlphaMaskTexture ? m_pShaderLightMapWithAlphaMask : m_pShaderLightMap;
		}
		else
		{
			pShader = bEnableAlphaMaskTexture ? m_pShaderVertexColorWithAlphaMask : m_pShaderVertexColor;
		}

		// Sets shader constants.
		A3DMATRIX4 matWorld = pMirrorPlane->GetWorldMatrix();
		A3DCameraBase* pCamera = pViewport->GetCamera();
		A3DMATRIX4 matViewProj = pCamera->GetVPTM();
		A3DMATRIX4 matWVP = matWorld * matViewProj;
		pShader->SetConstantMatrix( "g_matWVP", matWVP );

		A3DMATRIX4 matIWVP = matWVP.GetInverseTM();
		A3DMATRIX4 matITWVP = matIWVP.GetTranspose();
		pShader->SetConstantMatrix( "g_matITWVP", matITWVP );

		pShader->SetValue1f( "g_UVOffsetMultipler", pMirrorPlane->m_fUVOffsetMultipler );
		pShader->SetValue1f( "g_fFakeReflectPower", pMirrorPlane->m_fFakeReflectPower );

		const A3DVECTOR3& vCameraDir = pViewport->GetCamera()->GetDir();
		pShader->SetValue3f( "g_ViewDirection", &vCameraDir );

		const A3DVECTOR3& vPlaneNormal = pMirrorPlane->GetReflectionPlane().vNormal;
		pShader->SetValue3f( "g_ReflectionNormal", &vPlaneNormal );

		// Sets common textures.
		pShader->SetTexture( "g_ReflectionSampler", pReflectionTarget );
		pA3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		pA3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		pA3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		pA3DDevice->SetTextureAddress(0, A3DTADDR_CLAMP, A3DTADDR_CLAMP);

		pShader->SetTexture( "g_UVNoiseSampler", pMirrorPlane->m_pUVNoiseTexture );
		pA3DDevice->SetSamplerState(4, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		pA3DDevice->SetSamplerState(4, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		pA3DDevice->SetSamplerState(4, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		pA3DDevice->SetTextureAddress(4, A3DTADDR_CLAMP, A3DTADDR_CLAMP);

		pShader->SetTexture( "g_FakeReflectSampler", pMirrorPlane->m_pFakeReflectionTexture );
		pA3DDevice->SetSamplerState(5, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		pA3DDevice->SetSamplerState(5, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		pA3DDevice->SetSamplerState(5, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		pA3DDevice->SetTextureAddress(5, A3DTADDR_CLAMP, A3DTADDR_CLAMP);

		// Sets light map factors.
		A3DEnvironment* pA3DEnvironment = m_pA3DEngine->GetA3DEnvironment();
		const A3DEnvironment::LITMAP* pLitMap = pA3DEnvironment->GetLightMapParam();
		const float fDNFactor = pA3DEnvironment->GetDNFactor();
		pShader->Setup4f( "g_LightMapParam", pLitMap->MaxLight, pLitMap->MaxLightNight, 0.0f, fDNFactor );

		// Sets alpha mask texture.
		if( bEnableAlphaMaskTexture )
		{
			pShader->SetValue1f( "g_fAlphaMask", pMirrorPlane->m_fAlphaMaskValue );

			pShader->SetTexture( "g_AlphaMaskSampler", pMirrorPlane->m_pAlphaMaskTexture );
			pA3DDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			pA3DDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			pA3DDevice->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
			pA3DDevice->SetTextureAddress(1, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
		}

		// Sets light map textures.
		if( pMirrorPlane->m_eMode == A3DMirrorPlane::MODE_LIGHT_MAP )
		{
			pShader->SetTexture( "g_LightMapDaySampler", pMirrorPlane->m_pLightMapTextureDay );
			pA3DDevice->SetSamplerState(2, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			pA3DDevice->SetSamplerState(2, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			pA3DDevice->SetSamplerState(2, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
			pA3DDevice->SetTextureAddress(2, A3DTADDR_CLAMP, A3DTADDR_CLAMP);

			pShader->SetTexture( "g_LightMapNightSampler", pMirrorPlane->m_pLightMapTextureNight );
			pA3DDevice->SetSamplerState(3, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			pA3DDevice->SetSamplerState(3, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			pA3DDevice->SetSamplerState(3, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
			pA3DDevice->SetTextureAddress(3, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
		}

		pShader->Appear();

		const unsigned int nNumVertices = (unsigned int)pMirrorPlane->m_ArrayVertices.GetSize();
		const unsigned int nNumIndices = (unsigned int)pMirrorPlane->m_ArrayIndices.GetSize();
		const unsigned int nNumTriangles = nNumIndices / 3;

		pA3DDevice->DrawIndexedPrimitive( A3DPT_TRIANGLELIST, 0, 0, nNumVertices, 0, nNumTriangles );

		pShader->Disappear();
	}

	// Clears render queue.
	m_PtrArrayMirrorPlanes.RemoveAll();
}

// Creates all device resources used to render the mirror plane mesh.
bool A3DMirrorPlaneRender::_CreateDeviceResources()
{
	// Creates vertex declaration
	static const D3DVERTEXELEMENT9 pVertexElements[] =
	{
		{0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},		// Position
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},			// Normal
		{0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},		// Pre-baked diffuse lighting color in the day
		{0, 28, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 1},		// Pre-baked diffuse lighting color in the night
		{0, 32, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},		// Custom texture UV
		{0, 40, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},		// Light map texture UV
		D3DDECL_END()
	};

	m_pVertDecl = new A3DVertexDecl;
	A3DDevice* pDevice = m_pA3DEngine->GetA3DDevice();
	if( !m_pVertDecl || !m_pVertDecl->Init( pDevice, pVertexElements ) )
	{
		g_A3DErrLog.Log( __FUNCTION__ " Failed to create vertex declaration." );
		return false;
	}

	// Creates shader
	A3DHLSLMan* pHLSLManager = m_pA3DEngine->GetA3DHLSLMan();
	if( pHLSLManager != NULL )
	{
		const char szMirrorPlaneShaderFilePath[] = "Shaders\\2.2\\HLSL\\MirrorPlane\\MirrorPlane.hlsl";

		m_pShaderVertexColor = pHLSLManager->LoadShader(
			szMirrorPlaneShaderFilePath,
			"vs_main",
			szMirrorPlaneShaderFilePath,
			"ps_main",
			NULL,
			0
			);
		if( !m_pShaderVertexColor )
		{
			g_A3DErrLog.Log( __FUNCTION__ " Failed to create shader with vertex color." );
			return false;
		}

		m_pShaderVertexColorWithAlphaMask = pHLSLManager->LoadShader(
			szMirrorPlaneShaderFilePath,
			"vs_main",
			szMirrorPlaneShaderFilePath,
			"ps_main",
			"_USE_ALPHA_MASK_TEXTURE_",
			0
			);
		if( !m_pShaderVertexColorWithAlphaMask )
		{
			g_A3DErrLog.Log( __FUNCTION__ " Failed to create shader with vertex color and alpha mask texture." );
			return false;
		}

		m_pShaderLightMap = pHLSLManager->LoadShader(
			szMirrorPlaneShaderFilePath,
			"vs_main",
			szMirrorPlaneShaderFilePath,
			"ps_main",
			"_USE_LIGHT_MAP_",
			0
			);
		if( !m_pShaderLightMap )
		{
			g_A3DErrLog.Log( __FUNCTION__ " Failed to create shader with light map." );
			return false;
		}

		m_pShaderLightMapWithAlphaMask = pHLSLManager->LoadShader(
			szMirrorPlaneShaderFilePath,
			"vs_main",
			szMirrorPlaneShaderFilePath,
			"ps_main",
			"_USE_LIGHT_MAP_;_USE_ALPHA_MASK_TEXTURE_",
			0
			);
		if( !m_pShaderLightMapWithAlphaMask )
		{
			g_A3DErrLog.Log( __FUNCTION__ " Failed to create shader with light map and alpha mask texture." );
			return false;
		}
	}

	return true;
}

// Release all device resources used to render the mirror plane mesh.
void A3DMirrorPlaneRender::_ReleaseDeviceResources()
{
	if( m_pA3DEngine == NULL )
		return;

	A3DHLSLMan* pHLSLMan = m_pA3DEngine->GetA3DHLSLMan();
	if( pHLSLMan != NULL )
	{
		if( m_pShaderVertexColor != NULL )
		{
			pHLSLMan->ReleaseShader( m_pShaderVertexColor );
			m_pShaderVertexColor = NULL;
		}

		if( m_pShaderVertexColorWithAlphaMask != NULL )
		{
			pHLSLMan->ReleaseShader( m_pShaderVertexColorWithAlphaMask );
			m_pShaderVertexColorWithAlphaMask = NULL;
		}

		if( m_pShaderLightMap != NULL )
		{
			pHLSLMan->ReleaseShader( m_pShaderLightMap );
			m_pShaderLightMap = NULL;
		}

		if( m_pShaderLightMapWithAlphaMask != NULL )
		{
			pHLSLMan->ReleaseShader( m_pShaderLightMapWithAlphaMask );
			m_pShaderLightMapWithAlphaMask = NULL;
		}
	}

	A3DRELEASE( m_pVertDecl );
}
