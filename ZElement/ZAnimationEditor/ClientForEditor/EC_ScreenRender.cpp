/*
 * FILE: EC_ScreenRender.cpp
 *
 * DESCRIPTION: Screen render for the Element Client
 *
 * CREATED BY: Hedi, 2007/4/6 
 *
 * HISTORY:
 *
 * Copyright (c) 2007 Archosaur Studio, All Rights Reserved.
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
#include <A3DCamera.h>
#include <A3DViewport.h>

#include "EC_ScreenRender.h"
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

CECScreenRender::CECScreenRender()
{
	m_pA3DDevice = NULL;
	m_pScreenTargets[0] = NULL;
	m_pScreenTargets[1] = NULL;
	m_pShootingCam = NULL;
	m_pViewport = NULL;

	m_pRenderForScreen = NULL;

	m_nCurScreen = 0;
	m_nLastScreen = 1;
}

CECScreenRender::~CECScreenRender()
{
	Release();
}

bool CECScreenRender::Init(A3DDevice * pA3DDevice)
{
	m_pA3DDevice = pA3DDevice;

	if( !LoadResource() )
	{
		g_Log.Log("CECScreenRender::Init(), failed to load resources!");
		return false;
	}

	return true;
}

bool CECScreenRender::Release()
{
	ReleaseResource();
	return true;
}

bool CECScreenRender::LoadResource()
{
	m_nWidth = 512;
	m_nHeight = 512;

#ifdef BACK_VERSION
	D3DADAPTER_IDENTIFIER8 idAdapter;
	m_pA3DDevice->GetA3DEngine()->GetD3D()->GetAdapterIdentifier(D3DADAPTER_DEFAULT, D3DENUM_NO_WHQL_LEVEL, &idAdapter);
#else
	D3DADAPTER_IDENTIFIER9 idAdapter;
	m_pA3DDevice->GetA3DEngine()->GetD3D()->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0/*D3DENUM_NO_WHQL_LEVEL*/, &idAdapter);
#endif
	if( m_pA3DDevice->GetD3DCaps().TextureCaps & D3DPTEXTURECAPS_POW2 )
	{
		if( !(m_pA3DDevice->GetD3DCaps().TextureCaps & D3DPTEXTURECAPS_NONPOW2CONDITIONAL) )
		{
			m_nWidth = 256;
			m_nHeight = 256;
		}
	}
	
#ifdef BACK_VERSION
	A3DDEVFMT devFmt;
	devFmt.bWindowed	= true;
	devFmt.nWidth		= m_nWidth;
	devFmt.nHeight		= m_nHeight;
#else
	A3DRenderTarget::RTFMT devFmt;

	//devFmt.bWindowed	= true;
	devFmt.iWidth		= m_nWidth;
	devFmt.iHeight		= m_nHeight;
#endif
	devFmt.fmtTarget	= m_pA3DDevice->GetDevFormat().fmtTarget;
	devFmt.fmtDepth		= m_pA3DDevice->GetDevFormat().fmtDepth;

	m_pScreenTargets[0] = new A3DRenderTarget();
	if( !m_pScreenTargets[0]->Init(m_pA3DDevice, devFmt, true, true) )
	{
		g_Log.Log("CECScreenRender::LoadResource(), failed to create screen target 0!");
		return false;
	}
	
	m_pScreenTargets[1] = new A3DRenderTarget();
	if( !m_pScreenTargets[1]->Init(m_pA3DDevice, devFmt, true, true) )
	{
		g_Log.Log("CECScreenRender::LoadResource(), failed to create screen target 1!");
		return false;
	}

	m_pShootingCam = new A3DCamera();
	if( !m_pShootingCam->Init(m_pA3DDevice) )
	{
		g_Log.Log("CECShadowRender::Init(), failed to create the shadow map camera!");
		return false;
	}
	
	//	Create a Viewport
	RECT rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = m_nWidth;
	rc.bottom = m_nHeight;
	if( !m_pA3DDevice->CreateViewport(&m_pViewport, 0, 0, rc.right, rc.bottom,
									0.0f, 1.0f, true, true, 0x00000000) )
	{
		g_Log.Log("CECScreenRender::Create shadow map viewport fail");
		return false;
	}
	m_pViewport->SetCamera(m_pShootingCam);

	m_ScreenTexture.SetA3DDevice(m_pA3DDevice);
	m_ScreenTexture.SetDXTexture(m_pScreenTargets[0]->GetTargetTexture());
	return true;
}

bool CECScreenRender::ReleaseResource()
{
	if( m_pScreenTargets[0] )
	{
		m_pScreenTargets[0]->Release();
		delete m_pScreenTargets[0];
		m_pScreenTargets[0] = NULL;
	}

	if( m_pScreenTargets[1] )
	{
		m_pScreenTargets[1]->Release();
		delete m_pScreenTargets[1];
		m_pScreenTargets[1] = NULL;
	}

	if( m_pShootingCam )
	{
		m_pShootingCam->Release();
		delete m_pShootingCam;
		m_pShootingCam = NULL;
	}

	if( m_pViewport )
	{
		m_pViewport->Release();
		delete m_pViewport;
		m_pViewport = NULL;
	}

	return true;
}

bool CECScreenRender::Update(int nDeltaTime)
{
	return true;
}

bool CECScreenRender::RenderScreen(A3DViewport * pRestoreViewport, LPRENDERFORSCREEN pRenderForScreen, void * pArg)
{
	FlipScreens();

#ifdef BACK_VERSION
	m_pA3DDevice->SetRenderTarget(m_pScreenTargets[m_nCurScreen]);
	m_pA3DDevice->SetRestoreToRenderTarget(m_pScreenTargets[m_nCurScreen]);
#else
	m_pScreenTargets[m_nCurScreen]->ApplyToDevice();
#endif
	
	m_pViewport->Active();
	m_pViewport->ClearDevice();

	// draw a black point on this device to ensure driver get to know that the content changed
	// for on VIA/S3C chips, driver will not clear a device unless some things are drawn on it.
	g_pA3DGDI->Draw2DPoint(A3DPOINT2(0, 0), 0xff000000, 1.0f);

	(*pRenderForScreen)(m_pViewport, pArg);

#ifdef BACK_VERSION
	m_pA3DDevice->SetDefaultRestoreToRenderTarget();
	m_pA3DDevice->RestoreRenderTarget();
#else
	m_pScreenTargets[m_nCurScreen]->WithdrawFromDevice();
#endif
	pRestoreViewport->Active();

	return true;
}

bool CECScreenRender::ReloadResource()
{
	// first try to release old ones.
	ReleaseResource();

	if( !LoadResource() )
	{
		g_Log.Log("CECScreenRender::ReloadResource(), failed to load resource!");
		return false;
	}
		
	return true;
}

bool CECScreenRender::FlipScreens()
{
	if( m_nCurScreen == 0 )
	{
		m_nCurScreen = 1;
		m_nLastScreen = 0;
	}
	else
	{
		m_nCurScreen = 0;
		m_nLastScreen = 1;
	}

	m_ScreenTexture.SetDXTexture(m_pScreenTargets[m_nCurScreen]->GetTargetTexture());
	return true;
}