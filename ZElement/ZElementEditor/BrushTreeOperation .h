// ObstructOperation2.h: interface for the CObstructOperation class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _BRUSH_TREE_OPERATION_H_
#define _BRUSH_TREE_OPERATION_H_ 

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Operation.h"
#include "EditerBezier.h"
#include "A2DSprite.h"
#include "TerrainStretchBrush.h"
#include <vector>

class CTerrainLayer;

#define OBS_MAP_WIDTH 1024
#define OBS_MAP_DIVIDE 32


class CBrushTreeOperation :public COperation    
{
public:
	enum
	{
		BRUSH_REBUILD = 0,
		BRUSH_CHECK,						// set the seed to expand the reachable region
		BRUSH_UNCHECK,								// set the pixel to "Unreachable" state
		BRUSH_SPLIT,
		BRUSH_MERGE,
	};

	CBrushTreeOperation();
	virtual ~CBrushTreeOperation();
	virtual bool OnLButtonDown(int x, int y, DWORD dwFlags);
	virtual bool OnLButtonUp(int x,int y, DWORD dwFlags);
	virtual bool OnMouseMove(int x,int y,DWORD dwFlags);
	virtual void Render(A3DViewport* pA3DViewport);

	inline  void SetOperationType(int type){ m_nOperationType = type; };
	void         Init(int type);
	virtual void Release();

	void         SaveObsToFile(CString strPathName);
	
protected:
	void DrawGrid(A3DVECTOR3 v1, A3DVECTOR3 v2, A3DVECTOR3 v3, A3DVECTOR3 v4,int nSel);
	void DrawLine(A3DVECTOR3 v1,A3DVECTOR3 v2, DWORD clr);
	void DrawObstruct();
	void TransObstructToBitmap();
	void UpdateBitmap(ARectF rc, bool bCheck);
	void UpdateObsData();

private:
	int							m_nOperationType;
	A2DSprite*					m_pSprite;
	BYTE*                       m_pData;
	bool                        m_bStartPaint;
	CTerrainStretchBrush*       m_pBrush;

	//For obstruct npc map
	bool  m_bEndAddLine;
	A3DVECTOR3 m_vLineStart;
	A3DVECTOR3 m_vLineEnd;
	// for debug test
	A3DVECTOR3 m_vHitPos;
};

#endif // !defined(AFX_OBSTRUCTOPERATION_H__CD9882A0_DBC2_4C30_A60D_B28CACF461BB__INCLUDED_)
