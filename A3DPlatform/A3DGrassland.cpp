/*
 * FILE: EL_Grassland.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Hedi, 2004/10/21
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

//#include "StdAfx.h"
#include "A3DDevice.h"
#include "A3DFuncs.h"
#include "A3DCamera.h"
#include "A3DEngine.h"
#include "A3DTexture.h"
#include "A3DTextureMan.h"
#include "A3DTerrain2.h"
#include "A3DViewport.h"
#include "A3DVertexShader.h"
#include "A3DPixelShader.h"
#include "A3DShaderMan.h"
#include "A3DFont.h"
#include "AFile.h"
#include "A3DStream.h"

#include "A3DGrassLand.h"
#include "A3DGrassType.h"
#include "A3DGrassRender.h"
#include "A3DGrassInteractor.h"
#include "AGPAPerfIntegration.h"
//#include "ShellSDK/EncStr.h"
#define _EA(x) (x)
#define _EW(x) (x)

#define EC_RELEASE_DELETE(p) \
if (p) \
{ \
	(p)->Release(); \
	delete (p); \
	(p) = NULL; \
}


// grass block's size (in bits)
#define GRASS_BLOCKSIZE	64

static const D3DVERTEXELEMENT9 aVertexShaderDecl[] =
{
	{0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
	{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
	{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
	D3DDECL_END()
};


A3DGrassLand::A3DGrassLand()
: m_vHostDelta	(NULL)
, m_vHostPos	(NULL)
, m_vHostPress	(NULL)
, m_pGrassInteractor (NULL)
, m_pGrassRender(NULL)
, m_pStream		(NULL)
, m_pSoftStream (NULL)
, m_nGrassVertsBufSize (0)
, m_nGrassInstDataSize (0)
{
	m_pA3DDevice		= NULL;
	m_pA3DTerrain		= NULL;
	m_pLogFile			= NULL;

	m_bProgressLoad		= false;
	m_bForceRebuild		= false;
	m_bInteractorEnable	= false;

	m_vTime				= 0.0f;

	m_vecWindDir		= A3DVECTOR3(1.0f, -0.5f, 0.0f);

	m_pGrassVertexShader	= NULL;
	m_pGrassRenderShader	= NULL;

	m_bIsRenderForRefract = false;

	memset(m_vecWindOffset, 0, sizeof(A3DVECTOR4) * WIND_BUF_LEN);

	m_vLODLevel			= 1.0f;
	m_fDNFactor			= 0.0f;
}

A3DGrassLand::~A3DGrassLand()
{
}

bool A3DGrassLand::Init(A3DDevice * pA3DDevice, A3DTerrain2 * pTerrain, ALog * pLogFile)
{
	m_pA3DDevice	= pA3DDevice;
	m_pA3DTerrain	= pTerrain;
	m_pLogFile		= pLogFile;

	m_vTime			= 0.0f;
	m_wind.Init(256, 1.0f, 256, 0.95f, 8, 0x12345);
	m_wind.SetTurbulence(true);

	// now load the vertex shader
	m_pGrassVertexShader = m_pA3DDevice->GetA3DEngine()->GetA3DShaderMan()->LoadVertexShader(_EA("Shaders\\2.2\\vs\\grass_wave.txt"), false, (D3DVERTEXELEMENT9*)aVertexShaderDecl);
	if( NULL == m_pGrassVertexShader )
	{
		m_pLogFile->Log(_EA("A3DGrassLand::Init(), failed to load grass_wave.txt"));
	}

	m_pGrassRenderShader = m_pA3DDevice->GetA3DEngine()->GetA3DShaderMan()->LoadPixelShader(_EA("Shaders\\2.2\\ps\\grassrender.txt"), false);
	if( NULL == m_pGrassRenderShader )
	{
		m_pLogFile->Log(_EA("A3DGrassLand::Init(), failed to load grass_render.txt"));
	}
	
	m_pGrassRender = A3DGrassRender::CreateRender(m_pA3DDevice);

	if (!m_pGrassRender->Init(this))
	{
		m_pLogFile->Log(_EA("A3DGrassLand::Init(), failed to initialize A3DGrassRender instance!"));
		return false;
	}

	if (m_pGrassRender->GetRenderType() == A3DGrassRender::RT_HWINSTANCE)
	{
		m_pGrassInteractor = new A3DGrassInteractor();
	}

	m_nMaxVerts = 20000;
	m_nMaxFaces = 20000;

	// now create the stream for render
	DWORD dwVertexFlags = A3DSTRM_REFERENCEPTR;
	DWORD dwIndexFlags = A3DSTRM_REFERENCEPTR;

	m_pStream = new A3DStream();
	if( !m_pStream->Init(m_pA3DDevice, sizeof(GRASSVERTEX), GRASS_FVF_VERTEX, m_nMaxVerts, m_nMaxFaces * 3, dwVertexFlags, dwIndexFlags) )
	{
		m_pLogFile->Log(_EA("A3DGrassLand::Init(), failed to create stream for rendering!"));
		return false;
	}

	m_pSoftStream = new A3DStream();
	if( !m_pSoftStream->Init(m_pA3DDevice, A3DVT_LVERTEX, m_nMaxVerts, m_nMaxFaces * 3, dwVertexFlags, dwIndexFlags) )
	{
		m_pLogFile->Log(_EA("A3DGrassLand::Init(), failed to create softstream for rendering!"));
		return false;
	}
	return true;
}

bool A3DGrassLand::Release()
{

	if( m_pSoftStream )
	{
		m_pSoftStream->Release();
		delete m_pSoftStream;
		m_pSoftStream = NULL;
	}

	if( m_pStream )
	{
		m_pStream->Release();
		delete m_pStream;
		m_pStream = NULL;
	}

	if( m_pGrassRenderShader )
	{
		m_pA3DDevice->GetA3DEngine()->GetA3DShaderMan()->ReleasePixelShader(&m_pGrassRenderShader);
		m_pGrassRenderShader = NULL;
	}

	if( m_pGrassVertexShader )
	{
		m_pA3DDevice->GetA3DEngine()->GetA3DShaderMan()->ReleaseVertexShader(&m_pGrassVertexShader);
		m_pGrassVertexShader = NULL;
	}

	int nNumGrassType = m_GrassTypes.GetSize();

	for(int i=0; i<nNumGrassType; i++)
	{
		A3DGrassType * pGrassType = m_GrassTypes[i];

		if( pGrassType )
		{
			pGrassType->Release();
			delete pGrassType;
			pGrassType = NULL;
		}
	}

	m_GrassTypes.RemoveAll();

	//	Grass render must be released after all grass types released
	A3DRELEASE(m_pGrassRender);
	A3DRELEASE(m_pGrassInteractor);

	return true;
}

bool A3DGrassLand::Update(const A3DVECTOR3& vecCenter, DWORD dwDeltaTime)
{
	int i;

	m_dwUpdateStart = a_GetTime();

	m_vTime += dwDeltaTime * 0.001f;

	// now update wind parameters
	for(i=0; i<WIND_BUF_LEN - 1; i++)
		m_vecWindOffset[i] = m_vecWindOffset[i + 1];

	float vWind;
	m_wind.GetValue(m_vTime * 20.0f, &vWind, 1);
	
	m_windWaver.SetWindForce(m_vecWindDir * vWind);
	m_windWaver.Tick(dwDeltaTime);
	A3DVECTOR3 offset = m_windWaver.GetPos();
	m_vecWindOffset[WIND_BUF_LEN - 1] = A3DVECTOR4(offset.x, offset.y, offset.z, 0.0f);
	
	float fTimeDelta = dwDeltaTime / 1000.0f;
	// now update each type of grass
	int nNumGrassType = m_GrassTypes.GetSize();
	for(i=0; i<nNumGrassType; i++)
	{
		A3DGrassType * pGrassType = m_GrassTypes[i];

		if( pGrassType )
			pGrassType->Update(vecCenter, fTimeDelta);
	}

	return true;
}

bool A3DGrassLand::Render(A3DViewport * pViewport, bool bRenderAlpha)
{
	A3D_ITTX_SCOPED_TASK_NAMED(AGPAPerfIntegration::GetInstance().GetA3DDomain(), TEXT(__FUNCTION__));

	if (m_pGrassRender)
		return m_pGrassRender->Render(pViewport, bRenderAlpha);

	return true;
}

void A3DGrassLand::SetDNFactor(float f)
{
	m_fDNFactor = f;
	if( m_fDNFactor < 0.0f )
		m_fDNFactor = 0.0f;
	if( m_fDNFactor > 1.0f )
		m_fDNFactor = 1.0f;
}

bool A3DGrassLand::Load(const char * szGrassLandFile)
{
	AFile fileToLoad;
	if( !fileToLoad.Open(szGrassLandFile, AFILE_OPENEXIST) )
	{
		m_pLogFile->Log(_EA("A3DGrassLand::Load(), failed to open file [%s]"), szGrassLandFile);
		return true;
	}

	DWORD dwReadLen;
	
	// then read types count
	int nNumTypes;
	if( !fileToLoad.Read(&nNumTypes, sizeof(int), &dwReadLen) )
		goto READFAIL;

	int i;
	// now load each grass's information out
	for(i=0; i<nNumTypes; i++)
	{
		// first type id of the grass type
		DWORD dwTypeID;
		if( !fileToLoad.Read(&dwTypeID, sizeof(DWORD), &dwReadLen) )
			goto READFAIL;
		
		// grass file's name
		char szGrassFile[256];
		if( !fileToLoad.Read(szGrassFile, 256, &dwReadLen) )
			goto READFAIL;

		// max visible count
		int nMaxVisibleCount;
		if( !fileToLoad.Read(&nMaxVisibleCount, sizeof(int), &dwReadLen) )
			goto READFAIL;

		// then grass type's definition data
		A3DGrassType::GRASSDEFINE_DATA data;
		if( !fileToLoad.Read(&data, sizeof(data), &dwReadLen) )
			goto READFAIL;	

		// initialize a new grass type object
		A3DGrassType * pGrassType = new A3DGrassType();
		if( !pGrassType->Init(m_pA3DDevice, this, dwTypeID, nMaxVisibleCount, szGrassFile, m_pLogFile, &data) )
		{
			m_pLogFile->Log(_EA("A3DGrassLand::Load(), failed to init A3DGrassType!"));
			EC_RELEASE_DELETE(pGrassType);	
			return false;
		}

		// then load the grass type's data (grass bits map)
		if( !pGrassType->Load(&fileToLoad) )
		{
			m_pLogFile->Log(_EA("A3DGrassLand::Load(), failed to call A3DGrassType::Load()!"));
			EC_RELEASE_DELETE(pGrassType);
			return false;
		}

		// add this type
		m_GrassTypes.Add(pGrassType);
	}

	fileToLoad.Close();
	return true;

READFAIL:
	m_pLogFile->Log(_EA("A3DGrassLand::Load(), Read from file failed!"));
	return false;
}

bool A3DGrassLand::Save(const char * szGrassLandFile)
{
	AFile fileToSave;
	if( !fileToSave.Open(szGrassLandFile, AFILE_CREATENEW | AFILE_BINARY) )
	{
		m_pLogFile->Log(_EA("A3DGrassLand::Save(), failed to create file [%s]"), szGrassLandFile);
		return false;
	}

	DWORD dwWriteLen;
	
	// then write types count
	int nNumTypes = m_GrassTypes.GetSize();
	if( !fileToSave.Write(&nNumTypes, sizeof(int), &dwWriteLen) )
		goto WRITEFAIL;

	int i;
	// now write each grass's information out
	for(i=0; i<nNumTypes; i++)
	{
		A3DGrassType * pGrassType = m_GrassTypes[i];

		// first type id of the grass type
		DWORD dwTypeID = pGrassType->GetTypeID();
		if( !fileToSave.Write(&dwTypeID, sizeof(DWORD), &dwWriteLen) )
			goto WRITEFAIL;
		
		// grass file's name
		if( !fileToSave.Write((void *)pGrassType->GetGrassFile(), 256, &dwWriteLen) )
			goto WRITEFAIL;

		// max visible count
		int nMaxVisibleCount = 0;
		if( !fileToSave.Write(&nMaxVisibleCount, sizeof(int), &dwWriteLen) )
			goto WRITEFAIL;

		// then grass type's definition data
		A3DGrassType::GRASSDEFINE_DATA data = pGrassType->GetDefineData();
		if( !fileToSave.Write(&data, sizeof(data), &dwWriteLen) )
			goto WRITEFAIL;

		if( !pGrassType->Save(&fileToSave) )
		{
			m_pLogFile->Log(_EA("A3DGrassLand::Save(), failed to call A3DGrassType::Save()!"));
			return false;
		}
	}

	fileToSave.Close();
	return true;

WRITEFAIL:
	m_pLogFile->Log(_EA("A3DGrassLand::Save(), Write to the file failed!"));
	return false;
}

int A3DGrassLand::GetGrassBlkSize() const
{
	if (m_pGrassRender)
		return m_pGrassRender->GetGrassBlockSize();
	else
	{
		ASSERT( FALSE && "You should not call this function before Grass render is initialized" );
		return GRASS_BLOCKSIZE;
	}
}

A3DGrassType * A3DGrassLand::GetGrassTypeByID(DWORD dwTypeID)
{
	int nNumType = m_GrassTypes.GetSize();

	for(int i=0; i<nNumType; i++)
	{
		A3DGrassType * pGrassType = m_GrassTypes[i];

		if( pGrassType->GetTypeID() == dwTypeID )
			return pGrassType;
	}

	return NULL;
}

bool A3DGrassLand::AddGrassType(DWORD dwTypeID, A3DGrassType::GRASSDEFINE_DATA& defData, int nMaxVisibleCount, const char * szGrassFile, A3DGrassType ** ppGrassType)
{
	A3DGrassType * pGrassType = new A3DGrassType();
	if( !pGrassType->Init(m_pA3DDevice, this, dwTypeID, nMaxVisibleCount, szGrassFile, m_pLogFile, &defData) )
	{
		m_pLogFile->Log(_EA("A3DGrassLand::AddGrassType(), failed to init A3DGrassType!"));
		EC_RELEASE_DELETE(pGrassType);
		return false;
	}

	m_GrassTypes.Add(pGrassType);

	*ppGrassType = pGrassType;
	return true;
}

bool A3DGrassLand::DeleteGrassType(A3DGrassType * pGrassType)
{
	// first find it in the array
	int nNumTypes = m_GrassTypes.GetSize();

	for(int i=0; i<nNumTypes; i++)
	{
		if( pGrassType == m_GrassTypes[i] )
		{
			pGrassType->Release();
			delete pGrassType;

			m_GrassTypes.RemoveAt(i);
			return true;
		}
	}
	
	return false;
}

bool A3DGrassLand::SetLODLevel(float level)
{
	m_vLODLevel = level;

	int i;
	// now update each type of grass
	int nNumGrassType = m_GrassTypes.GetSize();
	for(i=0; i<nNumGrassType; i++)
	{
		A3DGrassType * pGrassType = m_GrassTypes[i];

		if( pGrassType )
			pGrassType->AdjustForLODLevel();
	}
	
	return true;
}

// Delete all grass types, by jdl
bool A3DGrassLand::DeleteAllGrassTypes()
{
	// first find it in the array
	int nNumTypes = m_GrassTypes.GetSize();

	for(int i=0; i<nNumTypes; i++)
	{
		A3DRELEASE(m_GrassTypes[i]);
	}

	m_GrassTypes.RemoveAll();

	return true;
}

bool A3DGrassLand::RenderForRefract(A3DViewport* pViewport, float vRefractSurfaceHeight)
{
	m_bIsRenderForRefract = true;
	m_vRefractSurfaceHeight = vRefractSurfaceHeight;

	Render(pViewport, false);

	m_bIsRenderForRefract = false;
	return true;
}

int A3DGrassLand::GetGrassCount() const
{
	return m_pGrassRender->GetGrassCount();
}

///////////////////////////////////////////////////////////////////////////
// class to wave in the wind
///////////////////////////////////////////////////////////////////////////
A3DWindWaver::A3DWindWaver()
{
	m_k1 = 2.0f;
	m_k2 = 0.0f;
	m_f = 0.1f;

	m_vMass = 0.1f;
	m_vecPos = A3DVECTOR3(0.0f);
	m_vecAccel = A3DVECTOR3(0.0f);
	m_vecVelocity = A3DVECTOR3(0.0f);
	m_vecSpringForce = A3DVECTOR3(0.0f);
	m_vecWindForce = A3DVECTOR3(0.0f);
}

A3DWindWaver::~A3DWindWaver()
{
}

bool A3DWindWaver::CalculateForce(const A3DVECTOR3& vecPos, const A3DVECTOR3& vecVelocity)
{
	m_vecSpringForce = m_k1 * (-vecPos) + m_k2 * (-vecPos) - m_f * vecVelocity;
	m_vecForceCombined = m_vecWindForce + m_vecSpringForce;

	return true;
}

bool A3DWindWaver::UpdatePos(float dt)
{
	m_vecAccel = m_vecForceCombined / m_vMass;
	m_vecPos = m_vecPos + m_vecVelocity * dt + m_vecAccel * (0.5f * dt * dt);
	m_vecVelocity = m_vecVelocity + m_vecAccel * dt;

	return true;
}

bool A3DWindWaver::Tick(int nDeltaTime)
{
	// Limits the maximum delta time to avoid the wind strength becomes very strong with long frame interval.
	int nClampedDeltaTime = a_Min(nDeltaTime, 200);
	float dt = nClampedDeltaTime * 0.001f;

	m_vecSpringForce = m_k1 * (-m_vecPos) + m_k2 * (-m_vecPos) - m_f * m_vecVelocity;

	// L-K Method 2-level
	A3DVECTOR3		vecNewPos;

	// k1;
	A3DVECTOR3 vecOldPos = m_vecPos;
	A3DVECTOR3 vecK1 = m_vecVelocity;

	// k2;
	vecNewPos = vecOldPos + dt * vecK1;
	CalculateForce(vecNewPos, m_vecVelocity);
	UpdatePos(dt);
	A3DVECTOR3 vecK2 = m_vecVelocity;

	m_vecVelocity = (vecK1 + vecK2) * 0.5f;
	m_vecPos = vecOldPos + m_vecVelocity * dt; 

	return true;
}


