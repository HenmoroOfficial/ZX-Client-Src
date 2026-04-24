/*
 * FILE: GL_Viewport.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2003/12/16
 *
 * HISTORY: 
 *
 * Copyright (c) 2003 Archosaur Studio, All Rights Reserved.
 */

#include "A3D.h"
#include "A3DPI.h"
#include "ECViewport.h"

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
//	Implement ECViewport
//	
///////////////////////////////////////////////////////////////////////////

ECViewport::ECViewport()
{
	m_pA3DViewport		= NULL;
	m_pA3DCamera		= NULL;
	m_pA3DOrthoCamera	= NULL;
	m_bWrapperOnly		= false;
}

ECViewport::~ECViewport()
{
	if( !m_bWrapperOnly )
	{
		A3DRELEASE(m_pA3DViewport);
		A3DRELEASE(m_pA3DCamera);
		A3DRELEASE(m_pA3DOrthoCamera);
	}
}

//	Initialize object
bool ECViewport::Init(A3DDevice* pA3DDevice, const INITPARAMS& Params)
{
	m_pA3DDevice = pA3DDevice;
	//ASSERT(g_pGame && g_pGame->GetA3DDevice());

	//A3DDevice* pA3DDevice = g_pGame->GetA3DDevice();

	//	Create perspective camera
	if (!(m_pA3DCamera = new A3DCamera))
	{
		//glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, _EA("ECViewport::Init"), __LINE__);
		return false;
	}

	if (!m_pA3DCamera->Init(m_pA3DDevice, Params.fFOV, Params.fNearPlane, Params.fFarPlane, Params.rcView.Width() * 1.0f / Params.rcView.Height()))
	{
		//glb_ErrorOutput(ECERR_FAILEDTOCALL, _EA("ECViewport::Init"), __LINE__);
		return false;
	}

	m_pA3DCamera->SetPos(A3D::g_vOrigin + A3D::g_vAxisY * 50.0f);
	m_pA3DCamera->SetDirAndUp(A3D::g_vAxisZ, A3D::g_vAxisY);

	//	Create ortho-camera
	if (Params.bOrthoCamera)
	{
		if (!(m_pA3DOrthoCamera = new A3DOrthoCamera))
		{
			//glb_ErrorOutput(ECERR_FAILEDTOCALL, _EA("ECViewport::Init"), __LINE__);
			return false;
		}

		if (!m_pA3DOrthoCamera->Init(m_pA3DDevice, Params.fOCLeft, Params.fOCRight, Params.fOCBottom, Params.fOCTop, Params.fOCZNear, Params.fOCZFar))
		{
			//glb_ErrorOutput(ECERR_FAILEDTOCALL, _EA("ECViewport::Init"), __LINE__);
			return false;
		}

		m_pA3DOrthoCamera->SetPos(A3D::g_vOrigin + A3D::g_vAxisY * 50.0f);
		m_pA3DOrthoCamera->SetDirAndUp(-A3D::g_vAxisY, A3D::g_vAxisZ);
	}

	//	Create Viewport
	if (!(m_pA3DViewport = new A3DViewport))
	{
		//glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, _EA("ECViewport::Init"), __LINE__);
		return false;
	}

	A3DVIEWPORTPARAM ViewParams;

	ViewParams.X		= Params.rcView.left;
	ViewParams.Y		= Params.rcView.top;
	ViewParams.Width	= Params.rcView.Width();
	ViewParams.Height	= Params.rcView.Height();
	ViewParams.MinZ		= Params.fMinZ;
	ViewParams.MaxZ		= Params.fMaxZ;

	if (!m_pA3DViewport->Init(m_pA3DDevice, &ViewParams, Params.bClearColor, Params.bClearZ, Params.colClear))
	{
		//glb_ErrorOutput(ECERR_FAILEDTOCALL, _EA("ECViewport::Init"), __LINE__);
		return false;
	}

	m_pA3DViewport->SetCamera(m_pA3DCamera);

	return true;
}

//	Initialize object from an A3DViewport
bool ECViewport::InitFromA3D(A3DViewport * pA3DViewport)
{
	m_pA3DDevice = pA3DViewport->GetA3DDevice();
	m_pA3DViewport = pA3DViewport;
	if( pA3DViewport->GetCamera()->GetClassID() == A3D_CID_CAMERA )
		m_pA3DCamera = (A3DCamera *)pA3DViewport->GetCamera();
	else
		m_pA3DCamera = NULL;

	if( m_pA3DViewport->GetCamera()->GetClassID() == A3D_CID_ORTHOCAMERA)
		m_pA3DOrthoCamera = (A3DOrthoCamera*)pA3DViewport->GetCamera();
	else
		m_pA3DOrthoCamera = NULL;


	m_bWrapperOnly = true;
	return true;
}

//	Switch camera
bool ECViewport::SwitchCamera(bool bOrthoCamera)
{
	if (bOrthoCamera)
	{
		ASSERT(m_pA3DOrthoCamera);
		if (m_pA3DOrthoCamera)
			m_pA3DViewport->SetCamera(m_pA3DOrthoCamera);
	}
	else
	{
		ASSERT(m_pA3DCamera);
		if (m_pA3DCamera)
			m_pA3DViewport->SetCamera(m_pA3DCamera);
	}

	return true;
}

//	Activate viewport
bool ECViewport::Activate(bool bClear)
{
	ASSERT(m_pA3DViewport);

	if (!m_pA3DViewport->Active())
		return false;

	if (bClear)
	{
		if (!m_pA3DViewport->ClearDevice())
			return false;
	}

	return true;
}

//	Move viewport
bool ECViewport::Move(int x, int y, int iWidth, int iHeight, bool bWideScreen)
{
	if (!m_pA3DViewport)
		return true;

	A3DVIEWPORTPARAM vp = *m_pA3DViewport->GetParam();

	vp.X		= x;
	vp.Y		= y;
	vp.Width	= iWidth;
	vp.Height	= iHeight;

	if (bWideScreen)
	{
		if (iHeight * 16 > iWidth * 9)
		{
			int iNewHeight = iWidth * 9 / 16;
			vp.Y = (iHeight - iNewHeight) >> 1;
			vp.Height = iNewHeight;
		}
        if (iHeight * 16 < iWidth * 9)
        {
            int iNewWidth = iHeight * 16 / 9;
            vp.X = (iWidth - iNewWidth) >> 1;
            vp.Width = iNewWidth;
        }
	}

	m_pA3DViewport->SetParam(&vp);

	//	Adjust perspective camera
	if (m_pA3DCamera)
	{
		float f, zn, zf, fRatio;
		m_pA3DCamera->GetProjectionParam(&f, &zn, &zf, &fRatio);
		fRatio = (float)vp.Width / vp.Height;
		m_pA3DCamera->SetProjectionParam(f, zn, zf, fRatio);
	}

	//	Adjust ortho-camera
	if (m_pA3DOrthoCamera)
	{
		float w = (float)vp.Width;
		float h = (float)vp.Height;
		float zn = m_pA3DOrthoCamera->GetZFront();
		float zf = m_pA3DOrthoCamera->GetZBack();
		m_pA3DOrthoCamera->SetProjectionParam(-w, w, -h, h, zn, zf);
	}

	return true;
}

A3DCameraBase* ECViewport::GetA3DCameraBase()
{
	if(m_pA3DCamera)
		return m_pA3DCamera;
	else if(m_pA3DOrthoCamera)
		return m_pA3DOrthoCamera;
	else
		return NULL;
}


