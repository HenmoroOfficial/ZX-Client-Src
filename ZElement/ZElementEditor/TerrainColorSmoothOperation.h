//----------------------------------------------------------
// Filename	: TerrainMaskSmoothOperation.h
// Creator	: QingFeng Xin
// Date		: 2004.8.2
// Desc		: ²Ù×÷ÍØÕ¹Àà¡£
//-----------------------------------------------------------

#ifndef _TERRAIN_COLOR_SMOOTH_OPERATION_H_
#define _TERRAIN_COLOR_SMOOTH_OPERATION_H_

#include "Operation.h"
#include "TerrainLayer.h"
#include "UndoMaskAction.h"
#include "Brush.h"

class Brush;

class CTerrainColorSmoothOperation :public COperation
{

public:
	CTerrainColorSmoothOperation()
	{
		m_bLMouseDown = false;
		m_nSmoothFactory = 10;
		m_pBrush = new Brush();
		m_pUndoColorMap = 0;
	};
	virtual ~CTerrainColorSmoothOperation()
	{
		if(m_pBrush) delete m_pBrush;

	};

	virtual bool OnEvent(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void Render(A3DViewport* pA3DViewport);
	virtual void Tick(DWORD dwTimeDelta);


	//	Mouse action handler,
	virtual bool OnMouseMove(int x, int y, DWORD dwFlags);
	virtual bool OnLButtonDown(int x, int y, DWORD dwFlags);
	virtual bool OnRButtonDown(int x, int y, DWORD dwFlags);
	virtual bool OnLButtonUp(int x, int y, DWORD dwFlags);
	virtual bool OnRButtonUp(int x, int y, DWORD dwFlags);
	virtual bool OnLButtonDbClk(int x, int y, DWORD dwFlags);
	virtual bool OnRButtonDbClk(int x, int y, DWORD dwFlags);


	Brush* GetBrush(){ return m_pBrush; }; 
	void SetSmoothFactory( int nFactory){ m_nSmoothFactory = nFactory; };
	int GetSmoothFactory(){ return m_nSmoothFactory; }; 

	void RecreateTerrainRender();

protected:
	//void UpdateMaskMap();
	//void CalculateColor(CElementMap *pMap,MaskChunk *pChunk);
	void UpdateColorMap();
	void CalculateColor();
	void StartPaint(bool bStart = true);
private:
	Brush *m_pBrush;
	CPoint m_ptOldPos;
	bool   m_bLMouseDown;

	int			m_nVertexColWidth;
	int			m_nVertexColHeight;

	int m_nSmoothFactory;
	ARect<int>           m_rcUpdateRect;
	CUndoColorMapAction* m_pUndoColorMap;	
};

#endif //_TERRAIN_COLOR_SMOOTH_OPERATION_H_