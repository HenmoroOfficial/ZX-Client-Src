/*
 * FILE: EL_GrassType.cpp
 *
 * DESCRIPTION: Grass Type class for element
 *
 * CREATED BY: Hedi, 2004/10/23
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "A3DDevice.h"
#include "A3DFuncs.h"
#include "A3DEngine.h"
#include "A3DTexture.h"
#include "A3DTextureMan.h"
#include "A3DViewport.h"
#include "A3DStream.h"
#include "A3DPixelShader.h"
#include "A3DVertexShader.h"
#include "AF.h"
#include "A3DCameraBase.h"
#include "A3DFrame.h"
#include "A3DMesh.h"
#include "A3DTerrain2.h"
#ifdef _DEBUG
#include "A3DWireCollector.h"
#endif // _DEBUG

#include "A3DGrassLand.h"
#include "A3DGrassType.h"
#include "A3DGrassBlock.h"
#include "A3DGrassBitsMap.h"
#include <memory>
#define _EA(x) (x)
#define _EW(x) (x)
#define EC_RELEASE_DELETE(p) \
if (p) \
{ \
	(p)->Release(); \
	delete (p); \
	(p) = NULL; \
}


///////////////////////////////////////////////////////////////////////////
//	
//	Class A3DGrassType
//	
///////////////////////////////////////////////////////////////////////////

A3DGrassType::A3DGrassType()
{
	m_pA3DDevice			= NULL;
	m_pLogFile				= NULL;
	m_pGrassLand			= NULL;

	m_pTextureGrass			= NULL;
	m_pVertsTemplate		= NULL;
	m_pIndicesTemplate		= NULL;
	m_pStreamTemplate		= NULL;

	m_dwIDNext				= 0;
	m_fGrassHeight			= 0.0f;
	m_bBadGrass				= true;
}

A3DGrassType::~A3DGrassType()
{
}

bool A3DGrassType::Init(A3DDevice * pA3DDevice, A3DGrassLand * pGrassLand, DWORD dwTypeID, int nMaxVisibleCount, const char * szGrassFile, ALog * pLogFile, A3DGrassType::GRASSDEFINE_DATA * pDefineData)
{
	m_pA3DDevice		= pA3DDevice;
	m_pGrassLand		= pGrassLand;
	m_dwTypeID			= dwTypeID;
	m_pLogFile			= pLogFile;
	strncpy(m_szGrassFile, szGrassFile, 256);

	A3DFrame * pGrassFrame = new A3DFrame();

	AFileImage moxFile;
	if( !moxFile.Open(szGrassFile, AFILE_OPENEXIST | AFILE_BINARY | AFILE_TEMPMEMORY) )
	{
		m_pLogFile->Log(_EA("A3DGrassType::Init(), can not found grass file [%s]!"), szGrassFile);

		// we use bad grass instead
		if( !moxFile.Open(_EA("grasses\\badgrass.mox"), AFILE_OPENEXIST | AFILE_BINARY) )
		{
			m_pLogFile->Log(_EA("A3DGrassType::Init(), can not found bad grass file, so grass type will not be loaded"));
			delete pGrassFrame;
			return false;
		}
	}

	// load from a mox file
	if( !pGrassFrame->Load(m_pA3DDevice, &moxFile) )
	{
		EC_RELEASE_DELETE(pGrassFrame);
		return false;
	}
	moxFile.Close();

	if( pGrassFrame->GetNumChilds() != 0 || pGrassFrame->GetNumMeshes() != 1 )
	{
		EC_RELEASE_DELETE(pGrassFrame);
		return false;
	}

	m_bBadGrass = false;

	A3DMesh * pGrassMesh = pGrassFrame->GetFirstMesh();
	
	// use texture directly from this mesh
	m_pTextureGrass = m_pA3DDevice->GetA3DEngine()->GetA3DTextureMan()->DuplicateTexture(pGrassMesh->GetTexture());
	strncpy(m_szGrassMap, m_pTextureGrass->GetMapFile(), MAX_PATH);

	// vertex and face count
	m_nNumOneGrassVert = pGrassMesh->GetVertCount();
	m_nNumOneGrassFace = pGrassMesh->GetIndexCount() / 3;
	m_pVertsTemplate = new GRASSVERTEX[m_nNumOneGrassVert];
	m_pIndicesTemplate = new WORD[m_nNumOneGrassFace * 3];

	// retrieve vertex data
	int i;

	A3DVERTEX * pMeshVerts = pGrassMesh->GetVerts(0);
	A3DAABB aabb = pGrassMesh->GetMeshAutoAABB(0);
	float	vHeight = aabb.Maxs.y - aabb.Mins.y;
	for(i=0; i<m_nNumOneGrassVert; i++)
	{
		GRASSVERTEX vert;
		vert.pos.x = pMeshVerts[i].x;
		vert.pos.y = pMeshVerts[i].y;
		vert.pos.z = pMeshVerts[i].z;

		vert.normal.x = vert.normal.z = 0.0f;
		vert.normal.y = vert.pos.y / vHeight;
		vert.normal.y *= vert.normal.y;

		vert.u = pMeshVerts[i].tu;
		vert.v = pMeshVerts[i].tv;

		m_pVertsTemplate[i] = vert;
	}

	m_fGrassHeight = aabb.Maxs.y - aabb.Mins.y;
	// retrieve index data
	memcpy(m_pIndicesTemplate, pGrassMesh->GetIndices(), sizeof(WORD) * m_nNumOneGrassFace * 3);

	// no longer use frame, so delete it now.
	EC_RELEASE_DELETE(pGrassFrame);
	
	// set a default define data
	if( pDefineData )
	{
		m_GrassDefineData = *pDefineData;
		m_GrassDefineData.bAlphaBlendEnable = false;
		m_GrassDefineData.bRandomPos = false;
		m_GrassDefineData.nAlphaRefValue = 84;
	}
	else
	{	// set to defaults value
		m_GrassDefineData.bAlphaBlendEnable = false;
		m_GrassDefineData.bRandomPos = false;
		m_GrassDefineData.nAlphaRefValue = 84;
		m_GrassDefineData.vSize = 1.0f;
		m_GrassDefineData.vSizeVar = 0.2f;
		m_GrassDefineData.vSoftness = 1.0f;
		m_GrassDefineData.vSightRange = 50.0f;
	}

	std::auto_ptr<A3DStream> pStreamTemplate(new A3DStream);
	if (!pStreamTemplate->Init(m_pA3DDevice, sizeof(GRASSVERTEX), 0, m_nNumOneGrassVert, m_nNumOneGrassFace * 3, A3DSTRM_STATIC, A3DSTRM_STATIC))
		return false;

	pStreamTemplate->SetVerts(m_pVertsTemplate, m_nNumOneGrassVert);
	pStreamTemplate->SetIndices(m_pIndicesTemplate, m_nNumOneGrassFace * 3);
	m_pStreamTemplate = pStreamTemplate.release();

	return true;
}

bool A3DGrassType::Release()
{
	// first release all grass bits maps
	int nNumMaps = (int)m_GrassBitsMaps.size();
	for(int i=0; i<nNumMaps; i++)
	{
		A3DGrassBitsMap * pGrassBitsMap = m_GrassBitsMaps[i];
		pGrassBitsMap->Release();
		delete pGrassBitsMap;
	}
	m_GrassBitsMaps.clear();

	if( m_pTextureGrass )
	{
		m_pA3DDevice->GetA3DEngine()->GetA3DTextureMan()->ReleaseTexture(m_pTextureGrass);
		m_pTextureGrass = NULL;
	}

	if( m_pIndicesTemplate )
	{
		delete [] m_pIndicesTemplate;
		m_pIndicesTemplate = NULL;
	}

	if( m_pVertsTemplate )
	{
		delete [] m_pVertsTemplate;
		m_pVertsTemplate = NULL;
	}

	A3DRELEASE(m_pStreamTemplate);
	return true;
}

bool A3DGrassType::AdjustDefineData(A3DViewport * pViewport, const GRASSDEFINE_DATA& data)
{
	m_GrassDefineData = data;

	return RebuildAllGrasses(pViewport);
}

bool A3DGrassType::AdjustForLODLevel()
{
	// now update the blocks that are rendering
	int nNumMaps = (int)m_GrassBitsMaps.size();

	for(int i=0; i<nNumMaps; i++)
	{
		A3DGrassBitsMap * pGrassBitsMap = m_GrassBitsMaps[i];
		if( pGrassBitsMap )
		{
			pGrassBitsMap->AdjustForLODLevel();
		}
	}
	return true;
}

bool A3DGrassType::RebuildAllGrasses(A3DViewport * pViewport)
{
	// now update the blocks that are rendering
	int nNumMaps = (int)m_GrassBitsMaps.size();

	for(int i=0; i<nNumMaps; i++)
	{
		A3DGrassBitsMap * pGrassBitsMap = m_GrassBitsMaps[i];
		if( pGrassBitsMap )
		{
			pGrassBitsMap->UpdateGrassesForRender(pViewport);
		}
	}

	return true;
}

bool A3DGrassType::Load(AFile * pFileToLoad)
{
	DWORD dwReadLen;

	// first read the next bits map id
	if( !pFileToLoad->Read(&m_dwIDNext, sizeof(DWORD), &dwReadLen) )
		goto READFAIL;

	// then, save all bits map
	int nNumBitsMap; 
	if( !pFileToLoad->Read(&nNumBitsMap, sizeof(int), &dwReadLen) )
		goto READFAIL;

	int	i;
	for(i=0; i<nNumBitsMap; i++)
	{
		// first write out the bits map's id
		DWORD dwID;
		if( !pFileToLoad->Read(&dwID, sizeof(DWORD), &dwReadLen) )
			goto READFAIL;

		// width and height of the bits map
		int nWidth, nHeight;
		if( !pFileToLoad->Read(&nWidth, sizeof(int), &dwReadLen) )
			goto READFAIL;
		if( !pFileToLoad->Read(&nHeight, sizeof(int), &dwReadLen) )
			goto READFAIL;

		// center and grid size
		A3DVECTOR3 vecCenter;
		if( !pFileToLoad->Read(&vecCenter, sizeof(A3DVECTOR3), &dwReadLen) )
			goto READFAIL;
		float vGridSize;
		if( !pFileToLoad->Read(&vGridSize, sizeof(float), &dwReadLen) )
			goto READFAIL;

		// now create the A3DGrassBitsMap
		A3DGrassBitsMap * pBitsMap = new A3DGrassBitsMap();
		if( !pBitsMap->Init(m_pA3DDevice, this, dwID, vecCenter, vGridSize, nWidth, nHeight) )
			return false;

		// now read the bits from file
		if( !pFileToLoad->Read(pBitsMap->GetBitsMap(), sizeof(BYTE) * nWidth / 8 * nHeight, &dwReadLen) )
			goto READFAIL;

		m_GrassBitsMaps.push_back(pBitsMap);
	}

	return true;

READFAIL:
	//m_pLogFile->Log(_EA("A3DGrassType::Load(), Read from the file failed!"));
	return false;
}

bool A3DGrassType::Save(AFile * pFileToSave)
{
	DWORD dwWriteLen;

	// first write the next bits map id
	if( !pFileToSave->Write(&m_dwIDNext, sizeof(DWORD), &dwWriteLen) )
		goto WRITEFAIL;

	// then, save all bits map
	int nNumBitsMap;
	nNumBitsMap = (int)m_GrassBitsMaps.size();
	if( !pFileToSave->Write(&nNumBitsMap, sizeof(int), &dwWriteLen) )
		goto WRITEFAIL;

	int	i;
	for(i=0; i<nNumBitsMap; i++)
	{
		A3DGrassBitsMap * pBitsMap = m_GrassBitsMaps[i];

		// first write out the bits map's id
		DWORD dwID = pBitsMap->GetID();
		if( !pFileToSave->Write(&dwID, sizeof(DWORD), &dwWriteLen) )
			goto WRITEFAIL;

		// width and height of the bits map
		int nWidth, nHeight;
		nWidth = pBitsMap->GetWidth();
		nHeight = pBitsMap->GetHeight();
		if( !pFileToSave->Write(&nWidth, sizeof(int), &dwWriteLen) )
			goto WRITEFAIL;
		if( !pFileToSave->Write(&nHeight, sizeof(int), &dwWriteLen) )
			goto WRITEFAIL;

		// center and grid size
		A3DVECTOR3 vecCenter = pBitsMap->GetCenter();
		float vGridSize = pBitsMap->GetGridSize();
		if( !pFileToSave->Write(&vecCenter, sizeof(A3DVECTOR3), &dwWriteLen) )
			goto WRITEFAIL;
		if( !pFileToSave->Write(&vGridSize, sizeof(float), &dwWriteLen) )
			goto WRITEFAIL;

		// now write the bits out
		if( !pFileToSave->Write(pBitsMap->GetBitsMap(), sizeof(BYTE) * nWidth / 8 * nHeight, &dwWriteLen) )
			goto WRITEFAIL;
	}

	return true;

WRITEFAIL:
	//m_pLogFile->Log(_EA("A3DGrassType::Save(), Write to the file failed!"));
	return false;
}

bool A3DGrassType::Update(const A3DVECTOR3& vecCenter, float fDeltaTime)
{
	int nNumMaps = (int)m_GrassBitsMaps.size();
	for(int i=0; i<nNumMaps; i++)
	{
		A3DGrassBitsMap * pGrassBitsMap = m_GrassBitsMaps[i];
		if( pGrassBitsMap )
		{
			pGrassBitsMap->Update(vecCenter, fDeltaTime);
		}
	}
	return true;
}
	
bool A3DGrassType::RenderGrasses(A3DViewport * pViewport)
{
	int nNumMaps = (int)m_GrassBitsMaps.size();
	if( !nNumMaps )
		return true;

	A3DCULLTYPE cullType = m_pA3DDevice->GetFaceCull();
	m_pA3DDevice->SetFaceCull(A3DCULL_NONE);

	if( m_GrassDefineData.bAlphaBlendEnable )
	{
		m_pA3DDevice->SetAlphaBlendEnable(true);
		m_pA3DDevice->SetAlphaTestEnable(false);
		m_pA3DDevice->SetZWriteEnable(false);
	}
	else
	{
		m_pA3DDevice->SetAlphaTestEnable(true);
		m_pA3DDevice->SetAlphaBlendEnable(false);
		m_pA3DDevice->SetAlphaFunction(A3DCMP_GREATER);
		m_pA3DDevice->SetAlphaRef(m_GrassDefineData.nAlphaRefValue);
	}
	
	m_pTextureGrass->Appear(0);
	for(int i=0; i<nNumMaps; i++)
	{
		A3DGrassBitsMap * pGrassBitsMap = m_GrassBitsMaps[i];
		if( pGrassBitsMap )
		{
			pGrassBitsMap->Render(pViewport);
		}
	}
	m_pA3DDevice->SetFaceCull(cullType);
	m_pTextureGrass->Disappear(0);
	
	m_pA3DDevice->SetAlphaBlendEnable(true);
	m_pA3DDevice->SetAlphaTestEnable(false);
	m_pA3DDevice->SetZWriteEnable(true);

	return true;
}

bool A3DGrassType::AddGrassBitsMap(BYTE * pBitsMap, const A3DVECTOR3& vecCenter, float vGridSize, int nWidth, int nHeight, DWORD& dwID)
{
	dwID = GenerateID();

	A3DGrassBitsMap * pGrassBitsMap = new A3DGrassBitsMap();

	if( !pGrassBitsMap->Init(m_pA3DDevice, this, dwID, vecCenter, vGridSize, nWidth, nHeight) )
	{
		m_pLogFile->Log(_EA("A3DGrassType::AddGrassBitsMap(), failed to initialize new grass bits map!"));
		return false;
	}

	pGrassBitsMap->UpdateBitsMap(pBitsMap);

	m_GrassBitsMaps.push_back(pGrassBitsMap);
	return true;
}

A3DGrassBitsMap * A3DGrassType::GetGrassBitsMap(DWORD dwID)
{
	int nNumMaps = (int)m_GrassBitsMaps.size();

	for(int i=0; i<nNumMaps; i++)
	{
		A3DGrassBitsMap * pGrassBitsMap = m_GrassBitsMaps[i];
		if( pGrassBitsMap->GetID() == dwID )
			return pGrassBitsMap;
	}

	return NULL;
}

bool A3DGrassType::DeleteGrassBitsMap(DWORD dwID)
{
	int nNumMaps = (int)m_GrassBitsMaps.size();

	for(int i=0; i<nNumMaps; i++)
	{
		A3DGrassBitsMap * pGrassBitsMap = m_GrassBitsMaps[i];
		if( pGrassBitsMap->GetID() == dwID )
		{
			pGrassBitsMap->Release();
			delete pGrassBitsMap;

			m_GrassBitsMaps.erase(m_GrassBitsMaps.begin() + i);
			return true;
		}
	}

	return true;
}


///////////////////////////////////////////////////////////////////////////
//	
//	class A3DGrassBlock
//	
///////////////////////////////////////////////////////////////////////////
