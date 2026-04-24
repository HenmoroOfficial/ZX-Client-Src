/*
 * FILE: EC_SceneLights.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Hedi, 2005/12/23
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#include <A3DTypes.h>
#include <A3DFuncs.h>
#include <vector.h>
#include <AFileImage.h>

#include "A3DGFXExMan.h"

#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_SceneLights.h"

CECSceneLights::CECSceneLights()
{
	m_pA3DDevice		= NULL;
	m_pStaticLights		= NULL;
	m_nNumStaticLights	= 0;

	m_pLightBlocks		= NULL;
	m_vecBlocksCenter	= A3DVECTOR3(-99999.9f, -99999.9f, -99999.9f);
	m_BlocksAABB.Clear();
}

CECSceneLights::~CECSceneLights()
{
	Release();
}

bool CECSceneLights::Init(A3DDevice * pA3DDevice, float vBlockSize, int nBlockDim)
{
	m_pA3DDevice	= pA3DDevice;
	m_vBlockSize	= vBlockSize;
	m_nBlockDim		= nBlockDim;

	m_pLightBlocks = new SCENELIGHTBLOCK[m_nBlockDim * m_nBlockDim];
	m_BlocksAABB.Clear();

	return true;
}

bool CECSceneLights::Load(const char * szLightFile)
{
	AFileImage fileImage;

	if( !fileImage.Open(szLightFile, AFILE_OPENEXIST | AFILE_BINARY | AFILE_TEMPMEMORY) )
	{
		a_LogOutput(1, "CECSceneLights::Load(), failed to open file %s!", szLightFile);
		return false;
	}

	DWORD dwReadLen;
	DWORD dwVersion;
	if( !fileImage.Read(&dwVersion, sizeof(DWORD), &dwReadLen) )
	{
		a_LogOutput(1, "CECSceneLights::Load(), read file failed!");
		return false;
	}

	if( dwVersion < 0x80000001 )
	{
		a_LogOutput(1, "CECSceneLights::Load(), wrong version!");
		return false;
	}

	int nNumLights;
	
	if( !fileImage.Read(&nNumLights, sizeof(int), &dwReadLen) )
	{
		a_LogOutput(1, "CECSceneLights::Load(), read file failed!");
		return false;
	}

	m_pStaticLights = new SCENELIGHT[nNumLights];
	m_nNumStaticLights = nNumLights;

	if( !fileImage.Read(m_pStaticLights, sizeof(SCENELIGHT) * m_nNumStaticLights, &dwReadLen) )
	{
		a_LogOutput(1, "CECSceneLights::Load(), read file part 2 failed!");
		delete [] m_pStaticLights;
		m_pStaticLights = NULL;
		m_nNumStaticLights = 0;
		return false;
	}
#ifndef BACK_VERSION
	//老的衰减参数转换为新的衰减参数 by pyp 
	//TODO：先判断版本号，如果是老版本，那就是老参数，转换之；新版本判断标志的值，如果是老版本数据，则转换，否则不转换。
	for(int i = 0; i < m_nNumStaticLights; i++)
	{
		SCENELIGHT& light = m_pStaticLights[i];
		light.ConvertPointLightToNewAttenuation();
	}
#endif

	return true;
}

#ifndef BACK_VERSION
void CECSceneLights::SCENELIGHT::ConvertPointLightToNewAttenuation()
{
	float A0 = vAttenuation0;
	float A1 = vAttenuation1;
	float A2 = vAttenuation2;
	float R = vRange;
	//YCH的转换， 取0.25R和0.5R的距离
	float d1 = (R * R * A2 + 4.f * R * A1 + 16.f * A0);
	float d2 = (R * R * A2 + 2.f * R * A1 + 4.f * A0);

	if(d1 != 0)
		vAttenuation1 = 5.3333f * logf((4.f * R * R * A2 + 8.f * R * A1 + 16.f * A0) / d1);
	if(d2 != 0)
		vAttenuation0 = 4.f * expf(0.25f * vAttenuation1) / d2;
	vAttenuation1 = sqrtf(vAttenuation1);

	//PYP的转换，取0.1R和0.5R
	//  	float d1 = (R * R * A2 + 10.f * R * A1 + 100.f * A0);
	//  	float d2 = d1;//(R * R * A2 + 2.f * R * A1 + 4.f * A0);
	//  
	//  	if(d1 != 0)
	//  		vAttenuation1 = 25.f / 6.f * logf((25.f * R * R * A2 + 50.f * R * A1 + 100.f * A0) / d1);
	//  	if(d2 != 0)
	//  		vAttenuation0 = 100.f * expf(0.01f * vAttenuation1) / d2;
}
#endif

bool CECSceneLights::Release()
{
	if( m_pStaticLights )
	{
		delete [] m_pStaticLights;
		m_pStaticLights = NULL;
	}

	m_nNumStaticLights = 0;

	if( m_pLightBlocks )
	{
		delete [] m_pLightBlocks;
		m_pLightBlocks = NULL;
	}
	m_BlocksAABB.Clear();
	return true;
}

bool CECSceneLights::GetSceneLight(const A3DVECTOR3& vecPos, SCENELIGHT& light)
{
	if( !m_BlocksAABB.IsPointIn(vecPos) )
		return false;

	float rs = 1.0f / m_vBlockSize;

	int x = int((vecPos.x - m_BlocksAABB.Mins.x) * rs);
	int y = int((m_BlocksAABB.Maxs.z - vecPos.z) * rs);
	
	a_Clamp(x, 0, m_nBlockDim - 1);
	a_Clamp(y, 0, m_nBlockDim - 1);

	SCENELIGHTBLOCK& theBlock = m_pLightBlocks[y * m_nBlockDim + x];
	if( theBlock.lights.size() == 0 && theBlock.dynamicLights.size() == 0 )
		return false;

	unsigned int i;
	float vMinDis = 99999.9f;
	
	for(i=0; i<theBlock.lights.size(); i++)
	{
		SCENELIGHT& theLight = m_pStaticLights[theBlock.lights[i]];
		float vDis = a3d_Magnitude(theLight.vecPos - vecPos);
		if( vDis < vMinDis )
		{
			light = theLight;
			vMinDis = vDis;
		}
	}

	light.vAttenuation0 = 0.0f;
	light.vAttenuation1 = 0.4f;
	light.vAttenuation2 = 0.0f;

	if( light.flags & SCENELIGHT_NIGHT_ONLY )
	{
		light.diffuse *= m_fDNFactor;
		light.ambient *= m_fDNFactor;
	}

	for(i=0; i<theBlock.dynamicLights.size(); i++)
	{
		A3DLIGHTPARAM * pDynamicLight = theBlock.dynamicLights[i];
		float vDis = a3d_Magnitude(pDynamicLight->Position - vecPos);
		if( vDis < vMinDis )
		{
			light.diffuse		= pDynamicLight->Diffuse * pDynamicLight->Diffuse.a * 2.0f;
			light.ambient		= pDynamicLight->Diffuse * pDynamicLight->Diffuse.a * 0.2f;
			light.vecPos		= pDynamicLight->Position;
			light.vRange		= 10.0f;
			light.vAttenuation0 = 0.0f;
			light.vAttenuation1 = 0.4f;
			light.vAttenuation2 = 0.0f;
			vMinDis = vDis;
		}
	}

	light.diffuse.a = 1.0f;
	light.ambient.a = 1.0f;
#ifndef BACK_VERSION
	light.ConvertPointLightToNewAttenuation();
#endif
	return true;
}

bool CECSceneLights::Update(DWORD dwDeltaTime, float fDNFactor, const A3DVECTOR3& vecCenter)
{
	m_fDNFactor = fDNFactor;

	int i;
	float rs = 1.0f / m_vBlockSize;

	if( fabs(vecCenter.x - m_vecBlocksCenter.x) < m_vBlockSize &&
		fabs(vecCenter.z - m_vecBlocksCenter.z) < m_vBlockSize )
		goto UPDATEDYNAMICS;

	// now we need rebuild the whole light blocks.
	m_vecBlocksCenter = vecCenter;
	m_BlocksAABB.Clear();
	m_BlocksAABB.Mins = m_vecBlocksCenter + A3DVECTOR3(-m_vBlockSize, 0.0f, -m_vBlockSize) * (m_nBlockDim * 0.5f);
	m_BlocksAABB.Mins.y = 0.0f;
	m_BlocksAABB.Maxs = m_vecBlocksCenter + A3DVECTOR3( m_vBlockSize, 0.0f,  m_vBlockSize) * (m_nBlockDim * 0.5f);
	m_BlocksAABB.Maxs.y = 800.0f;
	m_BlocksAABB.CompleteCenterExts();

	for(i=0; i<m_nBlockDim * m_nBlockDim; i++)
		m_pLightBlocks[i].lights.clear();

	for(i=0; i<m_nNumStaticLights; i++)
	{
		SCENELIGHT light = m_pStaticLights[i];

		if( light.aabb.Mins.x > m_BlocksAABB.Maxs.x || light.aabb.Maxs.x < m_BlocksAABB.Mins.x )
			continue;
		if( light.aabb.Mins.z > m_BlocksAABB.Maxs.z || light.aabb.Maxs.z < m_BlocksAABB.Mins.z )
			continue;

		// put into these blocks.
		int xs = int((light.aabb.Mins.x - m_BlocksAABB.Mins.x) * rs);
		int ys = int((m_BlocksAABB.Maxs.z - light.aabb.Maxs.z) * rs);
		int xe = int((light.aabb.Maxs.x - m_BlocksAABB.Mins.x) * rs);
		int ye = int((m_BlocksAABB.Maxs.z - light.aabb.Mins.z) * rs);

		a_Clamp(xs, 0, m_nBlockDim - 1);
		a_Clamp(ys, 0, m_nBlockDim - 1);
		a_Clamp(xe, 0, m_nBlockDim - 1);
		a_Clamp(ye, 0, m_nBlockDim - 1);

		for(int y=ys; y<=ye; y++)
			for(int x=xs; x<=xe; x++)
				m_pLightBlocks[y * m_nBlockDim + x].lights.push_back(i);
	}

UPDATEDYNAMICS:
	for(i=0; i<m_nBlockDim * m_nBlockDim; i++)
		m_pLightBlocks[i].dynamicLights.clear();

	A3DGFXExMan * pGfxMan = g_pGame->GetA3DGFXExMan();
	GfxLightParamList& lightList = pGfxMan->GetLightParamList();
	pGfxMan->LockLightParamList();

	for(i=0; i<(int)lightList.size(); i++)
	{
		A3DLIGHTPARAM * pLight = &lightList[i]->GetLightParam();
		A3DAABB lightAABB;
		lightAABB.Center = pLight->Position;
		lightAABB.Extents = A3DVECTOR3(pLight->Range);
		lightAABB.CompleteMinsMaxs();

		if( lightAABB.Mins.x > m_BlocksAABB.Maxs.x || lightAABB.Maxs.x < m_BlocksAABB.Mins.x )
			continue;
		if( lightAABB.Mins.z > m_BlocksAABB.Maxs.z || lightAABB.Maxs.z < m_BlocksAABB.Mins.z )
			continue;

		// put into these blocks.
		int xs = int((lightAABB.Mins.x - m_BlocksAABB.Mins.x) * rs);
		int ys = int((m_BlocksAABB.Maxs.z - lightAABB.Maxs.z) * rs);
		int xe = int((lightAABB.Maxs.x - m_BlocksAABB.Mins.x) * rs);
		int ye = int((m_BlocksAABB.Maxs.z - lightAABB.Mins.z) * rs);

		a_Clamp(xs, 0, m_nBlockDim - 1);
		a_Clamp(ys, 0, m_nBlockDim - 1);
		a_Clamp(xe, 0, m_nBlockDim - 1);
		a_Clamp(ye, 0, m_nBlockDim - 1);

		for(int y=ys; y<=ye; y++)
		{
			for(int x=xs; x<=xe; x++)
			{
				int id = y * m_nBlockDim + x;
				if( m_pLightBlocks[id].dynamicLights.size() < 3 )
					m_pLightBlocks[id].dynamicLights.push_back(pLight);
			}
		}
	}

	pGfxMan->UnlockLightParamList();
	pGfxMan->ClearLightParamList();

	return true;
}