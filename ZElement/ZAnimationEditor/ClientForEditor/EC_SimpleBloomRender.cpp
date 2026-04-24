/*
 * FILE: EC_SimpleBloomRender.cpp
 *
 * DESCRIPTION: Simple Bloom render for the Element Client
 *
 * CREATED BY: Hedi, 2006/7/21 
 *
 * HISTORY:
 *
 * Copyright (c) 2006 Archosaur Studio, All Rights Reserved.
 */

#include <A3DTypes.h>
#include <A3DMacros.h>
#include <A3DVertex.h>
#include <A3DDevice.h>
#include <A3DEngine.h>
#include <A3DRenderTarget.h>
#include <A3DStream.h>
#include <A3DViewport.h>
#include <A3DGDI.h>

#include "EC_SimpleBloomRender.h"
#include "EC_Global.h"
#include "EC_Game.h"
#ifdef BACK_VERSION
#include "EC_Viewport.h"
#else
#include "ECViewport.h"
#endif
#include "EC_Configs.h"
#include "EC_GameRun.h"
#include "EC_World.h"

CECSimpleBloomRender::CECSimpleBloomRender()
{
	m_pA3DDevice = NULL;
	m_pBloomTarget = NULL;
	m_pSmallTarget = NULL;
	m_pStreamDownSize = NULL;
	m_pStreamUpSize = NULL;

	m_bForcePow2 = false;
	m_pRenderForBloom = NULL;
}

CECSimpleBloomRender::~CECSimpleBloomRender()
{
	Release();
}

bool CECSimpleBloomRender::Init(A3DDevice * pA3DDevice)
{
	m_pA3DDevice = pA3DDevice;

	if( !LoadResource() )
	{
		g_Log.Log("CECSimpleBloomRender::Init(), failed to load resources!");
		return false;
	}

	return true;
}

bool CECSimpleBloomRender::Release()
{
	ReleaseResource();
	return true;
}

bool CECSimpleBloomRender::LoadResource()
{
	bool bNeedDepth = false;
	m_nWidth = m_pA3DDevice->GetDevFormat().nWidth;
	m_nHeight = m_pA3DDevice->GetDevFormat().nHeight;

	m_bForcePow2 = false;

#ifdef BACK_VERSION
	D3DADAPTER_IDENTIFIER8 idAdapter;
	m_pA3DDevice->GetA3DEngine()->GetD3D()->GetAdapterIdentifier(D3DADAPTER_DEFAULT, D3DENUM_NO_WHQL_LEVEL, &idAdapter);
#else
	D3DADAPTER_IDENTIFIER9 idAdapter;
	m_pA3DDevice->GetA3DEngine()->GetD3D()->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0/*D3DENUM_NO_WHQL_LEVEL*/, &idAdapter);
#endif
	if( idAdapter.DeviceId == 0x00003344 && idAdapter.VendorId == 0x00001106 )
	{
		// VIA/S3G UniChrome Pro IGP
		g_Log.Log("CECSimpleBloomRender::Init(), VIA/S3C cases!");
		m_bForcePow2 = true;
		m_nWidth = 1024;
		m_nHeight = 1024;
		bNeedDepth = true;		
	}
	else
	{
		if( m_pA3DDevice->GetD3DCaps().TextureCaps & D3DPTEXTURECAPS_POW2 )
		{
			if( !(m_pA3DDevice->GetD3DCaps().TextureCaps & D3DPTEXTURECAPS_NONPOW2CONDITIONAL) )
			{
				m_bForcePow2 = true;
				m_nWidth = 512;
				m_nHeight = 512;
				bNeedDepth = true;
			}
		}
	}

#ifdef BACK_VERSION
	A3DDEVFMT devFmt;
	devFmt.bWindowed	= true;
	devFmt.nWidth		= m_nWidth;
	devFmt.nHeight		= m_nHeight;
#else
	A3DRenderTarget::RTFMT devFmt;
	devFmt.iWidth		= m_nWidth;
	devFmt.iHeight		= m_nHeight;
#endif

	devFmt.fmtTarget	= m_pA3DDevice->GetDevFormat().fmtTarget;
	devFmt.fmtDepth		= m_pA3DDevice->GetDevFormat().fmtDepth;

	m_pBloomTarget = new A3DRenderTarget();
	if( !m_pBloomTarget->Init(m_pA3DDevice, devFmt, true, bNeedDepth) )
	{
		g_Log.Log("CECSimpleBloomRender::LoadResource(), failed to create bloom target!");
		return false;
	}
	
#ifdef BACK_VERSION
	devFmt.nWidth		= m_nWidth / 4;
	devFmt.nHeight		= m_nHeight / 4;
#else
	devFmt.iWidth		= m_nWidth / 4;
	devFmt.iHeight		= m_nHeight / 4;
#endif
	m_pSmallTarget = new A3DRenderTarget();
	if( !m_pSmallTarget->Init(m_pA3DDevice, devFmt, true, false) )
	{
		g_Log.Log("CECSimpleBloomRender::LoadResource(), failed to create small target!");
		return false;
	}

	m_pStreamDownSize = new A3DStream();
	if( !m_pStreamDownSize->Init(m_pA3DDevice, sizeof(A3DSBVERTEX), A3DSBVERT_FVF, 4, 6, A3DSTRM_REFERENCEPTR, A3DSTRM_REFERENCEPTR) )
	{
		g_Log.Log("CECSimpleBloomRender::LoadResource(), failed to create down size stream!");
		return false;
	}

	m_pStreamUpSize = new A3DStream();
	if( !m_pStreamUpSize->Init(m_pA3DDevice, sizeof(A3DSBVERTEX), A3DSBVERT_FVF, 4, 6, A3DSTRM_REFERENCEPTR, A3DSTRM_REFERENCEPTR) )
	{
		g_Log.Log("CECSimpleBloomRender::LoadResource(), failed to create down size stream!");
		return false;
	}


	m_indices[0] = 0; m_indices[1] = 1; m_indices[2] = 2;
	m_indices[3] = 2; m_indices[4] = 1; m_indices[5] = 3;

	int w = m_nWidth / 4;
	int h = m_nHeight / 4;

	float dx = 0.75f / w;
	float dy = 0.75f / h;
	m_vertDownSize[0] = A3DSBVERTEX(A3DVECTOR4(-0.5f, -0.5f, 0.0f, 1.0f), 0x80ffffff, -dx, 0.0f, dx, 0.0f);
	m_vertDownSize[1] = A3DSBVERTEX(A3DVECTOR4((float)w - 0.5f, -0.5f, 0.0f, 1.0f), 0x80ffffff, 1.0f - dx, 0.0f, 1.0f + dx, 0.0f);
	m_vertDownSize[2] = A3DSBVERTEX(A3DVECTOR4(-0.5f, (float)h - 0.5f, 0.0f, 1.0f), 0x80ffffff, -dx, 1.0f, dx, 1.0f);
	m_vertDownSize[3] = A3DSBVERTEX(A3DVECTOR4((float)w - 0.5f, (float)h - 0.5f, 0.0f, 1.0f), 0x80ffffff, 1.0f - dx, 1.0f, 1.0f + dx, 1.0f);
	m_pStreamDownSize->SetVerts((BYTE *) m_vertDownSize, 4);
	m_pStreamDownSize->SetVertexRef((BYTE *) m_vertDownSize);
	m_pStreamDownSize->SetIndices((BYTE*) m_indices, 6);
	m_pStreamDownSize->SetIndexRef((void *) m_indices);

	int sw = m_pA3DDevice->GetDevFormat().nWidth;
	int sh = m_pA3DDevice->GetDevFormat().nHeight;
	m_vertUpSize[0] = A3DSBVERTEX(A3DVECTOR4(-0.5f, -0.5f, 0.0f, 1.0f), 0x80ffffff, 0.0f, -dy, 0.0f, dy);
	m_vertUpSize[1] = A3DSBVERTEX(A3DVECTOR4((float)sw - 0.5f, -0.5f, 0.0f, 1.0f), 0x80ffffff, 1.0f, -dy, 1.0f, dy);
	m_vertUpSize[2] = A3DSBVERTEX(A3DVECTOR4(-0.5f, (float)sh - 0.5f, 0.0f, 1.0f), 0x80ffffff, 0.0f, 1.0f-dy, 0.0f, 1.0f+dy);
	m_vertUpSize[3] = A3DSBVERTEX(A3DVECTOR4((float)sw - 0.5f, (float)sh - 0.5f, 0.0f, 1.0f), 0x80ffffff, 1.0f, 1.0f-dy, 1.0f, 1.0f+dy);
	m_pStreamUpSize->SetVerts((BYTE *) m_vertUpSize, 4);
	m_pStreamUpSize->SetVertexRef((BYTE *) m_vertUpSize);
	m_pStreamUpSize->SetIndices((BYTE*) m_indices, 6);
	m_pStreamUpSize->SetIndexRef((void *) m_indices);

	return true;
}

bool CECSimpleBloomRender::ReleaseResource()
{
	if( m_pStreamUpSize )
	{
		m_pStreamUpSize->Release();
		delete m_pStreamUpSize;
		m_pStreamUpSize = NULL;
	}

	if( m_pStreamDownSize )
	{
		m_pStreamDownSize->Release();
		delete m_pStreamDownSize;
		m_pStreamDownSize = NULL;
	}

	if( m_pSmallTarget )
	{
		m_pSmallTarget->Release();
		delete m_pSmallTarget;
		m_pSmallTarget = NULL;
	}

	if( m_pBloomTarget )
	{
		m_pBloomTarget->Release();
		delete m_pBloomTarget;
		m_pBloomTarget = NULL;
	}

	m_bForcePow2 = false;
	return true;
}

bool CECSimpleBloomRender::Update(int nDeltaTime)
{
	return true;
}

bool CECSimpleBloomRender::DoSimpleBloom(A3DViewport * pViewport, LPRENDERFORBLOOM pRenderForBloom, void * pArg)
{
#ifdef BACK_VERSION
	m_pA3DDevice->SetRenderTarget(m_pBloomTarget);
#else
	m_pBloomTarget->ApplyToDevice();
#endif

	A3DVIEWPORTPARAM param;
	param.X = 0;
	param.Y = 0;
	param.Width = m_nWidth;
	param.Height = m_nHeight;
	param.MinZ = 0.0f;
	param.MaxZ = 1.0f;
	m_pA3DDevice->SetViewport(&param);

	bool bClearZ = false;
	// first of all clear backbuffer to be pure black, and not clear z buffer
	if( m_bForcePow2 )
	{
		bClearZ = true;	
	}

	if( bClearZ  )
		g_pGame->GetA3DDevice()->Clear(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
	else
		g_pGame->GetA3DDevice()->Clear(D3DCLEAR_TARGET, 0, 1.0f, 0);

	// draw a black point on this device to ensure driver get to know that the content changed
	// for on VIA/S3C chips, driver will not clear a device unless some things are drawn on it.
	g_pA3DGDI->Draw2DPoint(A3DPOINT2(0, 0), 0xff000000, 1.0f);

	(*pRenderForBloom)(pViewport, pArg);

	m_pA3DDevice->SetLightingEnable(false);
	m_pA3DDevice->SetFogEnable(false);
	m_pA3DDevice->SetZWriteEnable(false);
	m_pA3DDevice->SetZTestEnable(false);

	// first down size the content on bloom target to small target
#ifdef BACK_VERSION
	m_pA3DDevice->SetRenderTarget(m_pSmallTarget);
#else
	m_pBloomTarget->WithdrawFromDevice();
	m_pSmallTarget->ApplyToDevice();
#endif
	param.X = 0;
	param.Y = 0;
	param.Width = m_nWidth / 4;
	param.Height = m_nHeight / 4;
	param.MinZ = 0.0f;
	param.MaxZ = 1.0f;
	m_pA3DDevice->SetViewport(&param);

	m_pA3DDevice->SetTextureFilterType(0, A3DTEXF_LINEAR);
	m_pA3DDevice->SetTextureFilterType(1, A3DTEXF_LINEAR);
	m_pA3DDevice->SetTextureAddress(0, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	m_pA3DDevice->SetTextureAddress(1, A3DTADDR_CLAMP, A3DTADDR_CLAMP);

	m_pA3DDevice->SetTextureColorArgs(0, A3DTA_TEXTURE, A3DTA_DIFFUSE);
	m_pA3DDevice->SetTextureColorOP(0, A3DTOP_SELECTARG1);
	m_pA3DDevice->SetTextureColorArgs(1, A3DTA_TEXTURE, A3DTA_CURRENT);
	m_pA3DDevice->SetTextureColorOP(1, A3DTOP_BLENDDIFFUSEALPHA);
	m_pA3DDevice->SetTextureAlphaArgs(0, A3DTA_TEXTURE, A3DTA_DIFFUSE);
	m_pA3DDevice->SetTextureAlphaOP(0, A3DTOP_SELECTARG1);
	m_pA3DDevice->SetTextureAlphaArgs(1, A3DTA_TEXTURE, A3DTA_TFACTOR);
	m_pA3DDevice->SetTextureAlphaOP(1, A3DTOP_SELECTARG2);
	m_pA3DDevice->SetTextureFactor(A3DCOLORRGBA(255, 255, 255, 75));

#ifdef BACK_VERSION
	m_pA3DDevice->GetD3DDevice()->SetTexture(0, m_pBloomTarget->GetTargetTexture());
	m_pA3DDevice->GetD3DDevice()->SetTexture(1, m_pBloomTarget->GetTargetTexture());
#else
	m_pBloomTarget->AppearAsTexture(0);
	m_pBloomTarget->AppearAsTexture(1);
#endif

	if( m_pSmallTarget->HasFilled() )
	{
		// consider motion blur, which can make a trail
		m_pA3DDevice->SetSourceAlpha(A3DBLEND_ONE);
		m_pA3DDevice->SetDestAlpha(A3DBLEND_INVSRCALPHA);
	}
	else
	{
		// not filled target yet, so just copy to it
		m_pA3DDevice->SetSourceAlpha(A3DBLEND_ONE);
		m_pA3DDevice->SetDestAlpha(A3DBLEND_ZERO);
		m_pSmallTarget->SetHasFilled();
	}

	m_pStreamDownSize->Appear();
	m_pA3DDevice->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, 4, 0, 2);
	m_pA3DDevice->ClearTexture(0);
	m_pA3DDevice->ClearTexture(1);

#ifdef BACK_VERSION
	m_pA3DDevice->RestoreRenderTarget();
#else
	m_pSmallTarget->WithdrawFromDevice();
#endif
	g_pGame->GetViewport()->GetA3DViewport()->Active();

#ifdef BACK_VERSION
	m_pA3DDevice->GetD3DDevice()->SetTexture(0, m_pSmallTarget->GetTargetTexture());
	m_pA3DDevice->GetD3DDevice()->SetTexture(1, m_pSmallTarget->GetTargetTexture());
#else
	m_pSmallTarget->AppearAsTexture(0);
	m_pSmallTarget->AppearAsTexture(1);
#endif

	m_pA3DDevice->SetSourceAlpha(A3DBLEND_ONE);
	m_pA3DDevice->SetDestAlpha(A3DBLEND_ONE);

	m_pStreamUpSize->Appear();
	m_pA3DDevice->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, 4, 0, 2);
	m_pA3DDevice->ClearTexture(0);
	m_pA3DDevice->ClearTexture(1);

	m_pA3DDevice->SetTextureAddress(0, A3DTADDR_WRAP, A3DTADDR_WRAP);
	m_pA3DDevice->SetTextureAddress(1, A3DTADDR_WRAP, A3DTADDR_WRAP);

	m_pA3DDevice->SetTextureColorArgs(0, A3DTA_TEXTURE, A3DTA_DIFFUSE);
	m_pA3DDevice->SetTextureColorOP(0, A3DTOP_MODULATE);
	m_pA3DDevice->SetTextureColorArgs(1, A3DTA_TEXTURE, A3DTA_CURRENT);
	m_pA3DDevice->SetTextureColorOP(1, A3DTOP_DISABLE);
	m_pA3DDevice->SetTextureAlphaArgs(0, A3DTA_TEXTURE, A3DTA_DIFFUSE);
	m_pA3DDevice->SetTextureAlphaOP(0, A3DTOP_MODULATE);
	m_pA3DDevice->SetTextureAlphaArgs(1, A3DTA_TEXTURE, A3DTA_CURRENT);
	m_pA3DDevice->SetTextureAlphaOP(1, A3DTOP_DISABLE);

	m_pA3DDevice->SetSourceAlpha(A3DBLEND_SRCALPHA);
	m_pA3DDevice->SetDestAlpha(A3DBLEND_INVSRCALPHA);

	m_pA3DDevice->SetLightingEnable(true);
	m_pA3DDevice->SetFogEnable(true);
	m_pA3DDevice->SetZWriteEnable(true);
	m_pA3DDevice->SetZTestEnable(true);
	return true;
}

bool CECSimpleBloomRender::ReloadResource()
{
	// first try to release old ones.
	ReleaseResource();

	if( !LoadResource() )
	{
		g_Log.Log("CECSimpleBloomRender::ReloadResource(), failed to load resource!");
		return false;
	}
		
	return true;
}
