//----------------------------------------------------------
// Filename	: TerrainMaskPaintOperation.h
// Creator	: QingFeng Xin
// Date		: 2004.7.28
// Desc		: 操作拓展类。
//-----------------------------------------------------------

#ifndef _TERRAIN_MASK_PAINT_OPERATION_H_
#define _TERRAIN_MASK_PAINT_OPERATION_H_

#include "Operation.h"
#include "TerrainStretchBrush.h"
#include "TerrainLayer.h"
#include "UndoMaskAction.h"

class CTerrainMaskPaintOperation :public COperation
{

public:
	CTerrainMaskPaintOperation()
	{
		m_bLMouseDown = false;
		m_nCurrentLayer = -1;
		m_bColorAdd = true;
		m_pBrush = new CTerrainStretchBrush();
		m_pBrush2 = new CTerrainStretchBrush();
		m_bUserBrush = false;
		m_pUserData = NULL;
		m_nUserBrushW = 0;
	    m_nUserBrushH = 0; 
		m_dwFrequency = 0;
		m_nRotateAngleMax = 0;
		m_nOffetMax = 0;
		m_nScaleMax = 1;
		m_bTurnX = false;
		m_bTurnY = false;
		m_nCircleMax = 0;
		m_nDitherGray = 0;

		m_bDitherAnglePress = false;
		m_bDitherOffsetPress = false;
		m_bDitherScalePress = false;
		m_bDitherCirclePress = false;
		m_bFrequencyPress = false;
		m_bDitherGrayPress = false;

		m_bPenDown = false;
		m_nOldBrushRad = 0;
		m_pUndoAction = 0;
	};
	virtual ~CTerrainMaskPaintOperation()
	{
		if(m_pBrush) delete m_pBrush;
		if(m_pBrush2) delete m_pBrush2;
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
	virtual bool OnWTPacket(int x, int y, DWORD button, DWORD param);


	CTerrainStretchBrush* GetBrush(){ return m_pBrush; }; 
	void SetCurrentLayer( int nLay);
	void SetPaintColorAdd(bool bColorAdd){ m_bColorAdd = bColorAdd; };
	bool IsPaintColorAdd(){ return m_bColorAdd; };
	void RecreateTerrainRender();
	
	void SetUserData(BYTE *pData,int width,int height)
	{
		m_pUserData = pData;
		m_nUserBrushW = width;
		m_nUserBrushH = height;
	}

	void EnableUserBrush( bool bEnable = true)
	{
		m_bUserBrush = bEnable;
	}

	void SetDitherAngle( int nAngle){ m_nRotateAngleMax = nAngle; }
	int  GetDitherAngle(){ return m_nRotateAngleMax; }
	void EnableDAPressPen(bool b){ m_bDitherAnglePress = b; }

	void SetDitherOffset( int nOffset){ m_nOffetMax = nOffset; }
	int  GetDitherOffset(){ return m_nOffetMax; }
	void EnableDOPressPen(bool b){ m_bDitherOffsetPress = b; }
	
	void SetDitherScale( int nPercent){ m_nScaleMax = nPercent; }
	int GetDitherScale(){ return m_nScaleMax; }
	void EnableDSPressPen(bool b){ m_bDitherScalePress = b; }

	void TurnX(bool b){ m_bTurnX = b; }
	void TurnY(bool b){ m_bTurnY = b; }

	void SetDitherCircle( int nAngle){ m_nCircleMax = nAngle; }
	int  GetDitherCircle(){ return m_nCircleMax; }
	void EnableDCPressPen(bool b){ m_bDitherCirclePress = b; }

	void SetFrequency(int nFrequency ){ m_dwFrequency = nFrequency; }
	void EnableFPressPen(bool b){ m_bFrequencyPress = b; }
	void SetDitherGray(int nGray){ m_nDitherGray = nGray; }
	void EnableDGPressPen(bool b){ m_bDitherGrayPress = b; }

protected:
	void UpdateMaskMap( int nPressure);
	void CalculateMask( int nPressure);
	void StartPaint(bool bStart = true);
	void TranslateData(BYTE *pSrc,int sw,int sh,int*pDst,int dw,int dh, int press);
	
	void RoateData(BYTE *pSrc,int sw,int sh,double d);//0-360
	void TurnXData(BYTE *pSrc,int sw,int sh);
	void TurnYData(BYTE *pSrc,int sw,int sh);
	void CircleData(BYTE *pSrc,int sw,int sh, double d);//0-90
private:
	CTerrainStretchBrush *m_pBrush;
	CTerrainStretchBrush *m_pBrush2;
	CPoint m_ptOldPos;
	bool   m_bLMouseDown;
	int    m_nCurrentLayer;
	int    m_nMaskWidth;
	int    m_nMaskHeight;

	bool   m_bColorAdd;
	bool   m_bTurnX;
	bool   m_bTurnY;
	bool   m_bPenDown;
	int    m_nOldBrushRad;

	//是否用用户自定义的Brush
	bool   m_bUserBrush;
	//用户刷子的宽高
	int    m_nUserBrushW;
	int    m_nUserBrushH;
	//用户刷子的灰度数据
	BYTE*  m_pUserData;
	DWORD  m_dwLastTime;
	DWORD  m_dwTimeUsed;
	DWORD  m_dwFrequency;
	int    m_nRotateAngleMax;
	int    m_nCircleMax;
	int    m_nOffetMax;
	int    m_nScaleMax;
	float  m_nDitherGray;
	CUndoMaskAction*       m_pUndoAction;	
	ARect<int>             m_rcUpdateRect;

	//压力笔开关
	bool m_bDitherAnglePress;
	bool m_bDitherOffsetPress;
	bool m_bDitherScalePress;
	bool m_bDitherCirclePress;
	bool m_bFrequencyPress;
	bool m_bDitherGrayPress;

	
};

#endif //_TERRAIN_MASK_PAINT_OPERATION_H_