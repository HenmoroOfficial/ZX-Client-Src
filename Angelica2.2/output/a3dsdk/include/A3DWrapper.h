/*
* FILE: A3DWrapper.h
*
* DESCRIPTION: RAII Wrappers for paired A3D calls
*
* CREATED BY: Zhangyachuan (zhangyachuan000899@wanmei.com), 2012/3/8
*
* HISTORY: 
*
* Copyright (c) 2012 Archosaur Studio, All Rights Reserved.
*/

#ifndef _A3DWrapper_H_
#define _A3DWrapper_H_

#include "A3DDevice.h"
#include "A3DViewport.h"
#include "A3DCamera.h"
#include "A3DMatrix.h"

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class A3DClipPlaneWrapper
//	This is a RAII (Resource Acquisition is Initialization) wrapper for clip plane setup
//	User don't need to clean the setting explicitly for we do it in the destructor of the wrapper
//	
///////////////////////////////////////////////////////////////////////////

class A3DClipPlaneWrapper
{
public:		//	Types

public:		//	Constructor and Destructor

	//	Keep it inline in order to make it the same as explicit calls to compiler
	A3DClipPlaneWrapper(A3DDevice* pA3DDevice, A3DViewport* pViewport)
		: m_pA3DDevice(pA3DDevice)
		, m_pViewport(pViewport)
		, m_bUserClip(false)
	{
		DWORD dwState = m_pA3DDevice->GetDeviceRenderState(D3DRS_CLIPPLANEENABLE);
		if ( dwState == D3DCLIPPLANE0 ) // only one clip plane supported now
		{
			m_bUserClip = true;

			D3DXPLANE hcp;
			m_pA3DDevice->GetClipPlane(0, (float *)&m_cp);
			A3DMATRIX4 matVP = pViewport->GetCamera()->GetVPTM();
			matVP.InverseTM();
			matVP.Transpose();
			D3DXPlaneTransform(&hcp, &m_cp, (D3DXMATRIX *) &matVP);
			m_pA3DDevice->SetClipPlane(0, hcp);
		}
	}

	//	For those who need to know the user clip plane state
	bool IsUserClip() const { return m_bUserClip; }

	~A3DClipPlaneWrapper()
	{
		DWORD dwState = m_pA3DDevice->GetDeviceRenderState(D3DRS_CLIPPLANEENABLE);
		if ( dwState == D3DCLIPPLANE0 ) // only one clip plane supported now
		{
			m_pA3DDevice->SetClipPlane(0, m_cp);
		}
	}

private:	//	Attributes

	A3DDevice* m_pA3DDevice;
	A3DViewport* m_pViewport;
	D3DXPLANE m_cp;
	bool m_bUserClip;

};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////


#endif	//	_A3DWrapper_H_

