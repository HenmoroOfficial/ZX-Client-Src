#include "global.h"
#include "MainFrm.h"
#include "UndoMaskAction.h"
#include "TerrainLayer.h"
#include "MaskModifier.h"


//#define new A_DEBUG_NEW


//向当前撤消动作中加入影响元素的原始数据,以便恢复
//向当前撤消动作中加入影响元素的原始数据,以便恢复
void CUndoMaskAction::AddGrid( int w,int h,BYTE by)
{
	int idx = m_iMapSize * h + w;
	if(m_mapGridData.find(idx) != m_mapGridData.end()) return;
	
	GrayGridData dat;
	dat.byteData = by;
	dat.nW = w;
	dat.nH = h;
	m_mapGridData[idx] = dat;
}

void CUndoMaskAction::AddData(ARectI&rc,  CELBitmap::LOCKINFO &lockInfor)
{
	BYTE* pLine = lockInfor.pData;
	for( int h = 0; h < lockInfor.iHeight; ++h)
	{
		// we should add iPitch after processing each line, NOT before.
		// pLine += lockInfor.iPitch;
		for( int w = 0; w < lockInfor.iWidth; ++w)
		{
			BYTE dat = *(pLine + w);
			AddGrid(w + rc.left,h + rc.top, dat);
		}
		pLine += lockInfor.iPitch;
	}
}

//	Release action
void CUndoMaskAction::Release()
{
	m_mapGridData.clear();
	m_rcUpdateRect.Clear();
}

//	Undo action
bool CUndoMaskAction::Undo()
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CTerrain *pTerrain = pMap->GetTerrain();
		CTerrainLayer *pCurrentLay = pTerrain->GetLayer(m_nLay);
		if(pCurrentLay==NULL) return false;
		CELBitmap *pMaskBitmap = pCurrentLay->GetMaskBitmap();
		if(pMaskBitmap==NULL) return false;
		CMaskModifier *pMaskModifier = pTerrain->GetMaskModifier();
		
		
		CELBitmap::LOCKINFO lockInfor;
		pMaskBitmap->LockRect(m_rcUpdateRect,&lockInfor);
		
		abase::hash_map<int,GrayGridData>::iterator it = m_mapGridData.begin();
		for(it; it != m_mapGridData.end(); ++it)
		{
			int  w = (it->second).nW;
			int  h = (it->second).nH;
			BYTE old = (it->second).byteData;
			int index = (h-m_rcUpdateRect.top)*lockInfor.iPitch+(w-m_rcUpdateRect.left);
			BYTE height =lockInfor.pData[index];
			lockInfor.pData[index] = (BYTE)old;
			(it->second).byteData = height;
		}
		pMap->GetTerrain()->GetMaskModifier()->UpdateMaskMapToRender(m_rcUpdateRect);
	}
	return true;
}

//	Redo action
bool CUndoMaskAction::Redo()
{
	Undo();
	return true;
}


#ifdef _ANGELICA22
/////////////////////////////////////////////////////////////////////////////COLORMAP////////////////////////////////////////////////////////////

void CUndoColorMapAction::AddGrid( int w,int h,unsigned int by)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap == NULL) return;
	int iMapSize = pMap->GetTerrain()->GetColorMap()->iWidth;

	int idx = iMapSize * h + w;
	if(m_mapGridData.find(idx) != m_mapGridData.end()) return;

	GrayGridData dat;
	dat.dwData = by;
	dat.nW = w;
	dat.nH = h;
	m_mapGridData[idx] = dat;
}

void CUndoColorMapAction::AddData(ARectI&rc)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CTerrain *pTerrain = pMap->GetTerrain();
		unsigned int * pColor = pTerrain->GetColorMap()->pColor;
		for( int h = 0; h < rc.Height(); ++h)
		{
			for( int w = 0; w < rc.Width(); ++w)
			{
				int iRow = rc.top + h;
				int iCol = rc.left + w;

				if( iRow < 0 || iCol < 0) continue;
				if( iRow >= pTerrain->GetColorMap()->iWidth || iCol >= pTerrain->GetColorMap()->iWidth) continue;

				int idx = iRow*pTerrain->GetColorMap()->iWidth + iCol;
				AddGrid(iCol,iRow, pColor[idx]);
			}
		}
	}
}

//	Release action
void CUndoColorMapAction::Release()
{
	m_mapGridData.clear();
	m_rcUpdateRect.Clear();
}

//	Undo action
bool CUndoColorMapAction::Undo()
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CTerrain *pTerrain = pMap->GetTerrain();
		unsigned int * pColor = pTerrain->GetColorMap()->pColor;

		abase::hash_map<int,GrayGridData>::iterator it = m_mapGridData.begin();
		for(it; it != m_mapGridData.end(); ++it)
		{
			int  w = (it->second).nW;
			int  h = (it->second).nH;
			unsigned int old = (it->second).dwData;
			int index = h* pTerrain->GetColorMap()->iWidth + w;
			unsigned int height = pColor[index];
			pColor[index] = old;
			(it->second).dwData = height;
		}
		pMap->GetTerrain()->UpdateVertexColorMapToRender(m_rcUpdateRect);
	}
	return true;
}

//	Redo action
bool CUndoColorMapAction::Redo()
{
	Undo();
	return true;
}
#endif