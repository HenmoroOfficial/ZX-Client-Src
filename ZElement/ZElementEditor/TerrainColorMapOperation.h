#pragma once
#include "operation.h"
#include "UndoMaskAction.h"

class Brush;

class CTerrainColorMapOperation : public COperation
{
public:
	enum PAINT_TYPE
	{
		PAINT_COLOR,			//	设置颜色
		PAINT_BLEND,			//	叠加颜色
		PAINT_BLUR,				//	模糊颜色
	//	PAINT_ERASE,			//	橡皮擦
	};
public:
	CTerrainColorMapOperation(void);
	~CTerrainColorMapOperation(void);
public:
	BYTE		GetAlpha() const { return m_btAlpha; }
	int			GetPower() const { return m_iPower; }
	float		GetOpacity() const { return m_fOpacity; }
	DWORD		GetColor() const { return m_dwColor; }
	void		SetAlpha(BYTE btAlpha) { m_btAlpha = btAlpha; }
	void		SetPower(int iPower) { m_iPower = iPower; }
	void		SetOpacity(float fOpacity) { m_fOpacity = fOpacity; }
	void		SetColor(DWORD dwColor) { m_dwColor = dwColor; }
	PAINT_TYPE	GetPaintType() const { return m_PaintType; }
	void		SetPaintType(PAINT_TYPE type) { m_PaintType = type; }
	void		RecreateTerrainRender();
	Brush*		GetBrush() const { return m_pBrush; }
protected:
//	virtual bool OnEvent(UINT message, WPARAM wParam, LPARAM lParam);
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
	virtual void OnKeyDown(DWORD dwChar, DWORD dwRepCnt, DWORD dwFlags);
	virtual void OnKeyUp(DWORD dwChar, DWORD dwRepCnt, DWORD dwFlags);
	void UpdateMaskMap(unsigned int iPressure);
	void CalculateMask(unsigned int iPressure);
	void TranslateData(BYTE *pSrc,int sw,int sh,int*pDst,int dw,int dh, int press);
	void PickColor(int x, int y);
protected:
	BYTE		m_btAlpha;
	int			m_iPower;
	float		m_fOpacity;
	DWORD		m_dwColor;
	PAINT_TYPE	m_PaintType;
	CPoint		m_ptOldPos;
	bool		m_bLMouseDown;

	int			m_nVertexColWidth;
	int			m_nVertexColHeight;
	int			m_nSmoothFactory;
	ARect<int>	m_rcUpdateRect;
	Brush*		m_pBrush;

	DWORD  m_dwLastTime;
	DWORD  m_dwTimeUsed;
	DWORD  m_dwFrequency;
	int		m_iPressure;
	bool	m_bPick;
	HCURSOR	m_hPickCursor;
	CUndoColorMapAction* m_pUndoColorMap;
};
