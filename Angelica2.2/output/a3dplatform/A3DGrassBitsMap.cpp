#include "A3DDevice.h"
#include "A3DTexture.h"
#include "A3DVertexShader.h"
#include "A3DPixelShader.h"
#include "A3DCameraBase.h"
#include "A3DViewport.h"
#include "A3DStream.h"
#include "A3DFuncs.h"
#include "A3DGrassType.h"
#include "A3DGrassland.h"
#include "A3DGrassBlock.h"
#include "A3DGrassBitsMap.h"
#include "A3DGrassRender.h"
#include "A3DWireCollector.h"
#include "A3DEngine.h"

///////////////////////////////////////////////////////////////////////////
//	
//	Class A3DGrassBitMap
//		a map used to determine where the grasses grow
//	
///////////////////////////////////////////////////////////////////////////

A3DGrassBitsMap::A3DGrassBitsMap()
{
	m_pGrassRender			= NULL;
	m_pGrassType			= NULL;

	m_pBitsMap				= NULL;
	m_nWidth = m_nHeight	= 0;

	m_nNumBlockX			= 0;
	m_nNumBlockY			= 0;
	m_nNumBlocks			= 0;
	m_ppBlocks				= NULL;

	m_pTerrainLM			= NULL;
	m_pTerrainLM_n			= NULL;

	m_dwExportID			= 0;
	m_bAllBitsZero			= true;
}

A3DGrassBitsMap::~A3DGrassBitsMap()
{
}

bool A3DGrassBitsMap::Init(A3DDevice * pA3DDevice, A3DGrassType * pGrassType, DWORD dwID, const A3DVECTOR3& vecCenter, float vGridSize, int nWidth, int nHeight)
{
	m_pA3DDevice		= pA3DDevice;
	m_pGrassType		= pGrassType;
	m_pGrassRender		= pGrassType->GetGrassLand()->GetGrassRender();
	m_dwID				= dwID;
	m_vGridSize			= vGridSize;
	m_nWidth			= nWidth;
	m_nHeight			= nHeight;
	m_vecCenter			= vecCenter;

	m_vecOrigin			= vecCenter - A3DVECTOR3(m_vGridSize * m_nWidth / 2.0f, 0.0f, 0.0f) - A3DVECTOR3(0.0f, 0.0f, -m_vGridSize * m_nHeight / 2.0f);

	assert(((nWidth >> 3) << 3) == nWidth);

	// now create a bit map to hold the bits data
	int nSizeMap = (nWidth >> 3) * nHeight;
	m_pBitsMap = new BYTE[nSizeMap];
	if( !m_pBitsMap ) return false;
	memset(m_pBitsMap, 0, nSizeMap);

	// create all block objects
	if( !CreateBlocks() )
		return false;

	return true;
}

bool A3DGrassBitsMap::Release()
{
	ReleaseBlocks();

	if( m_pBitsMap )
	{
		delete [] m_pBitsMap;
		m_pBitsMap = NULL;
	}

	return true;
}

bool A3DGrassBitsMap::UpdateBitsMap(BYTE * pBitsMap)
{
	memcpy(m_pBitsMap, pBitsMap, (m_nWidth * m_nHeight) >> 3);

	return true;
}

bool A3DGrassBitsMap::UpdateGrassesForRender(A3DViewport * pViewport)
{
	// update the block's grasses information, only update blocks that have been built
	// for those that have not been built yet, the other call to Update will build them
	for(int i=0; i<m_nNumBlocks; i++)
	{
		A3DGrassBlock * pBlock = m_ppBlocks[i];
		if( pBlock && pBlock->IsBuilt() )
		{
			pBlock->BuildGrasses();
		}
	}

	return true;
}

bool A3DGrassBitsMap::AdjustForLODLevel()
{
	// update the block's grasses information, only update blocks that have been built
	for(int i=0; i<m_nNumBlocks; i++)
	{
		A3DGrassBlock * pBlock = m_ppBlocks[i];
		if( pBlock )
		{
			if( pBlock->IsBuilt() )
				pBlock->BuildGrasses();
		}
	}

	return true;
}

int A3DGrassBitsMap::GetGrassBlkSize() const
{
	return m_pGrassRender->GetGrassBlockSize();
}

bool A3DGrassBitsMap::UpdateVisibleBlocks(const A3DVECTOR3& vecCenter)
{
	// first we should determine the sight range
	int cx, cy;
	int nSightRangeInGrid = int(m_pGrassType->GetDefineData().vSightRange / m_vGridSize);

	cx = int((vecCenter.x - m_vecOrigin.x) / m_vGridSize);
	cy = int((m_vecOrigin.z - vecCenter.z) / m_vGridSize);

	ARectI rectSight;
	rectSight.left = cx - nSightRangeInGrid;
	rectSight.right = rectSight.left + nSightRangeInGrid * 2;
	rectSight.top = cy - nSightRangeInGrid;
	rectSight.bottom = rectSight.top + nSightRangeInGrid * 2;
	
	const int nGrassBlockSize = GetGrassBlkSize();

	int		nBlockX = cx / nGrassBlockSize;
	int		nBlockY = cy / nGrassBlockSize;

	int		i, j;

	// first let all visible blocks disappear if it can not be activated again by below test
	for(i=0; i<(int)m_VisibleBlocks.size(); i++)
		m_VisibleBlocks[i]->SetAboutToDestroy();

	int	r = (int)ceilf((float)nSightRangeInGrid / (float)nGrassBlockSize);
	for(i = nBlockY - r; i <= nBlockY + r; i++)
	{
		for(j = nBlockX - r; j <= nBlockX + r; j++)
		{
			if( i < 0 || i >= m_nNumBlockY || j < 0 || j >= m_nNumBlockX )
				continue;

			A3DGrassBlock * pBlock = m_ppBlocks[i * m_nNumBlockX + j];
			if( pBlock )
			{
				ARectI rectBlock = pBlock->GetBlockRect();
				ARectI rectCombined = rectBlock & rectSight;

				if( !rectCombined.IsEmpty() )
				{
					if( !pBlock->IsBuilt() && !pBlock->IsAboutToBuild() )
					{
						m_VisibleBlocks.push_back(pBlock);
						pBlock->SetAboutToBuild();
					}
					pBlock->ClearAboutToDestroy();
				}
			}
		}
	}

	// now destroy some old ones;
	for(i=0; i<(int)m_VisibleBlocks.size(); )
	{
		A3DGrassBlock * pBlock = m_VisibleBlocks[i];
		if( pBlock->IsAboutToDestroy() )
		{
			pBlock->DestroyGrasses();

			// remove from visible blocks list
			m_VisibleBlocks.erase(m_VisibleBlocks.begin() + i);
			continue;
		}

		i ++;
	}
	return true;
}

bool A3DGrassBitsMap::Update(const A3DVECTOR3& vecCenter, float fDeltaTime)
{
	// first see if we need to update it.
	float vRadius = MagnitudeH(m_vecCenter - m_vecOrigin);
	if( MagnitudeH(vecCenter - m_vecCenter) > vRadius + m_pGrassType->GetDefineData().vSightRange)
		return true;

	if( !UpdateVisibleBlocks(vecCenter) )
		return false;

	int i;
	for(i=0; i<(int)m_VisibleBlocks.size(); i++)
	{
		A3DGrassBlock * pBlock = m_VisibleBlocks[i];
		if( !pBlock->IsBuilt() && pBlock->IsAboutToBuild() )
		{
			pBlock->BuildGrasses();
			if( m_pGrassType->GetGrassLand()->GetProgressLoad() )
			{
				if( a_GetTime() > m_pGrassType->GetGrassLand()->GetUpdateStartTime() + 6 )
					break;
			}
		}

		if (!m_pGrassType->GetGrassLand()->GetGrassInteractorEnable())
			continue;

		pBlock->UpdateGrassOffset(fDeltaTime);
	}
	return true;
}

bool A3DGrassBitsMap::Render(A3DViewport* pViewport)
{
	if (m_pGrassRender)
		m_pGrassRender->DoRender(pViewport, this);

	return true;
}

bool A3DGrassBitsMap::ShiftCenterTo(const A3DVECTOR3& vecCenter)
{
	m_vecCenter = vecCenter;
	m_vecOrigin	= vecCenter - A3DVECTOR3(m_vGridSize * m_nWidth / 2.0f, 0.0f, 0.0f) - A3DVECTOR3(0.0f, 0.0f, -m_vGridSize * m_nHeight / 2.0f);

	return CreateBlocks();
}

float A3DGrassBitsMap::GetMaxRenderDist() const
{
	return GetGrassBlkSize() * GetGridSize() * 1.414f + m_pGrassType->GetDefineData().vSightRange;
}

bool A3DGrassBitsMap::CreateBlocks()
{
	// try to release old ones.
	ReleaseBlocks();

	const int nGrassBlockSize = GetGrassBlkSize();
	// now create all blocks, but not build them
	assert(m_nWidth / nGrassBlockSize * nGrassBlockSize == m_nWidth);
	assert(m_nHeight / nGrassBlockSize * nGrassBlockSize == m_nHeight);

	m_nNumBlockX = m_nWidth / nGrassBlockSize;
	m_nNumBlockY = m_nHeight / nGrassBlockSize;

	m_nNumBlocks = m_nNumBlockX * m_nNumBlockY;
	m_ppBlocks = new A3DGrassBlock* [m_nNumBlocks];
	for(int y=0; y<m_nNumBlockY; y++)
	{
		for(int x=0; x<m_nNumBlockX; x++)
		{
			ARectI rectBlock;
			rectBlock.left = x * nGrassBlockSize;
			rectBlock.right = rectBlock.left + nGrassBlockSize;
			rectBlock.top = y * nGrassBlockSize;
			rectBlock.bottom = rectBlock.top + nGrassBlockSize;

			A3DGrassBlock * pNewBlock = m_pGrassRender->CreateBlock();

			if( !pNewBlock->Init(this, rectBlock) )
				return false;

			m_ppBlocks[y * m_nNumBlockX + x] = pNewBlock;
		}
	}

	return true;
}

bool A3DGrassBitsMap::ReleaseBlocks()
{
	if( m_ppBlocks )
	{
		for(int i=0; i<m_nNumBlocks; i++)
		{
			A3DGrassBlock * pBlock = m_ppBlocks[i];

			if( pBlock )
			{
				pBlock->Release();
				delete pBlock;
			}
		}

		delete [] m_ppBlocks;
		m_ppBlocks = NULL;
	}

	m_nNumBlockX = 0;
	m_nNumBlockY = 0;
	m_nNumBlocks = 0;

	m_VisibleBlocks.clear();
	return true;
}

//	Used when export grass data. Return true if not all bits are zero,
//	this function also set m_bAllBitsZero flag
bool A3DGrassBitsMap::BitsMapCheck()
{
	m_bAllBitsZero = true;

	if (!m_pBitsMap)
		return false;

	int i, iNumByte = m_nWidth * m_nHeight / 8;
	for (i=0; i < iNumByte; i++)
	{
		if (m_pBitsMap[i])
		{
			m_bAllBitsZero = false;
			return true;
		}
	}

	return false;
}