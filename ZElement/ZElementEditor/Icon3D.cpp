#include "StdAfx.h"
#include "Icon3D.h"
#include <A3DDevice.h>
#include <A3DTexture.h>

//////////////////////////////////////////////////////////////////////////
//
//	Local Variable & Constants
//
//////////////////////////////////////////////////////////////////////////

static const int g_iRectCount = 2000;
static const int g_iVertCount = 4 * g_iRectCount;
static const int g_iIdxCount = 6 * g_iRectCount;

//////////////////////////////////////////////////////////////////////////
//
//	Impl for Icon3DRender
//
//////////////////////////////////////////////////////////////////////////

Icon3DRender::Icon3DRender()
{
}

Icon3DRender::~Icon3DRender()
{
	m_stream.Release();
}

bool Icon3DRender::presetIndexBuf()
{
	// preset index buf
	WORD* pIndices;
	
	if (!m_stream.LockIndexBuffer(0, 0, (BYTE**)&pIndices, 0))
		return false;

	int nRectNum = m_stream.GetIndexCount() / 6;
	for (int i = 0; i < nRectNum; i++)
	{
		int index = i * 6;
		WORD nBase = (WORD)(i * 4);
		pIndices[index  ] = nBase;
		pIndices[index+1] = nBase + 1;
		pIndices[index+2] = nBase + 2;
		pIndices[index+3] = nBase + 2;
		pIndices[index+4] = nBase + 1;
		pIndices[index+5] = nBase + 3;
	}

	m_stream.UnlockIndexBuffer();
	return true;
}


bool Icon3DRender::Init(A3DDevice* pDevice)
{
	if (!pDevice)
		return false;

	m_pDevice = pDevice;
	if (!m_stream.Init(pDevice, sizeof(A3DLVERTEX), A3DFVF_A3DLVERTEX, g_iVertCount, g_iIdxCount, A3DSTRM_REFERENCEPTR, A3DSTRM_STATIC))
		return false;

	if (!presetIndexBuf())
		return false;
	
	return true;
}

void Icon3DRender::AddIcon(int id, const Icon3DItem& item)
{
	if(m_setHash.find(id) != m_setHash.end())
		return;

	m_setHash.insert(id);
	m_mapIcons[item.m_pTexture].push_back(item);
}

void Icon3DRender::RenderFlush(A3DDevice* pDevice)
{
	m_pDevice->SetLightingEnable(false);

	bool bFogEnable = m_pDevice->GetFogEnable();
	if (bFogEnable)
		m_pDevice->SetFogEnable(false);
	
	m_pDevice->SetAlphaBlendEnable(false);	
	m_pDevice->SetWorldMatrix(A3DMATRIX4::IDENTITY);
	m_pDevice->SetSpecularEnable(false);
	for (IconMap::iterator mapItr = m_mapIcons.begin()
		; mapItr != m_mapIcons.end()
		; ++mapItr)
	{
		const IconArray& arr = mapItr->second;
		preRender(mapItr->first);
	
		size_t max_vert = min(static_cast<size_t>(g_iVertCount), arr.size() * 4);
		A3DLVERTEX* pVerts = NULL;
		m_stream.LockVertexBuffer(0, static_cast<DWORD>(max_vert * sizeof(A3DLVERTEX)), (BYTE**)&pVerts, D3DLOCK_DISCARD);
		size_t cur_vert = 0;

		for (IconArray::const_iterator arrItr = arr.begin()
			; arrItr != arr.end() && cur_vert + 3 < max_vert
			; ++arrItr)
		{
			const Icon3DItem& item = *arrItr;
			
			pVerts[cur_vert++] = item.m_vVerts[0];
			pVerts[cur_vert++] = item.m_vVerts[1];
			pVerts[cur_vert++] = item.m_vVerts[2];
			pVerts[cur_vert++] = item.m_vVerts[3];
		}
		
		m_stream.UnlockVertexBuffer();

		m_stream.Appear();		
		pDevice->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, 0, 4 * static_cast<DWORD>(arr.size()), 0, 2 * static_cast<DWORD>(arr.size()));
		postRender(mapItr->first);
	}

	m_pDevice->SetAlphaBlendEnable(true);
	m_pDevice->SetLightingEnable(true);
	m_pDevice->SetFogEnable(bFogEnable);
	m_pDevice->SetSpecularEnable(true);
	m_mapIcons.clear();
	m_setHash.clear();
}

void Icon3DRender::preRender(A3DTexture* pTexture)
{
	if (!pTexture)
		m_pDevice->ClearTexture(0);
	else
		pTexture->Appear();
}

void Icon3DRender::postRender(A3DTexture* pTexture)
{
	if (pTexture)
		pTexture->Disappear();
}