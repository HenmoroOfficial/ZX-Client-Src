#pragma once

#include <ABaseDef.h>
#include <A3DVector.h>

class A3DViewport;

class Brush
{
public:
	Brush(void);
	~Brush(void);
public:
	virtual void	Render(A3DViewport* pA3DViewport);
	virtual void	Tick(DWORD dwTimeDelta);

	virtual void	SetCenter(float x, float z);
	void			GetCenter(float &x, float &z) const { x = m_fCenterX; z = m_fCenterY; };
	void			SetBrushColor( DWORD clr) { m_dwColor = clr; };

	int				GetHard() const { return m_iHard; }
	void			SetHard(int iHard) { m_iHard = iHard; }

	int				GetDispelNum() const { return m_iDispelNum; }
	void			SetDispelNum(int iDispelNum) { m_iDispelNum = iDispelNum; }

	int				GetDitherColor() const { return m_iDitherColor; }
	void			SetDitherColor(int iDitherColor) { m_iDitherColor = iDitherColor; }

	int				GetDitherArea() const { return m_iDitherArea; }
	void			SetDitherArea(int iDitherArea) { m_iDitherArea = iDitherArea; }

	int				GetDitherAngle() const { return m_iDitherAngle; }
	void			SetDitherAngle(int iDitherAngle) { m_iDitherAngle = iDitherAngle; }

	int				GetDitherCircle() const { return m_iDitherCircle; }
	void			SetDitherCircle(int iDitherCircle) { m_iDitherCircle = iDitherCircle; }

	int				GetDispel() const { return m_iDispel; }
	void			SetDispel(int iDispel) { m_iDispel = iDispel; }

	bool			IsDefaultBrush() const { return m_bDefaultBrush; }
	void			UseDefaultBrush(bool bUseDefaultBrush) { m_bDefaultBrush = bUseDefaultBrush; }

	void			SetBrushData(COLORREF* pColorData, int iWidth, int iHeight);

	bool			IsPressureDispelNum() const { return m_bPressureDispelNum; }
	bool			IsPressureDitherColor() const { return m_bPressureDitherColor; }
	bool			IsPressureDitherArea() const { return m_bPressureDitherArea; }
	bool			IsPressureDitherAngle() const { return m_bPressureDitherAngle; }
	bool			IsPressureDitherCircle() const { return m_bPressureDitherCircle; }

	void			EnablePressureDispelNum(bool bEnable) { m_bPressureDispelNum = bEnable; }
	void			EnablePressureDitherColor(bool bEnable) { m_bPressureDitherColor = bEnable; }
	void			EnablePressureDitherArea(bool bEnable) { m_bPressureDitherArea = bEnable; }
	void			EnablePressureDitherAngle(bool bEnable) { m_bPressureDitherAngle = bEnable; }
	void			EnablePressureDitherCircle(bool bEnable) { m_bPressureDitherCircle = bEnable; }

	void			TurnX(bool bTurn) { m_bTurnX = bTurn; }
	bool			IsTurnX() const { return m_bTurnX; }
	void			TurnY(bool bTurn) { m_bTurnY = bTurn; }
	bool			IsTurnY() const { return m_bTurnY; }

	BYTE*			GetBrushData() const { return m_pData; }
	BYTE*			GetBrushResultData() const { return m_pResultData; }
	int				GetWidth() const { return m_iWidth; }
	int				GetHeight() const { return m_iHeight; }
	void			TransformBrush(int iPressure);
	void			SetBrushName(const char* szName) { m_strBrushName = szName; }
	const char*		GetBrushName() { return m_strBrushName.GetBuffer(); }

protected:
	virtual void	DrawRect();
	virtual void	DrawCircle(float fRadius);
	void			DrawLine(A3DVECTOR3 *pVertices, DWORD dwNum);
	void			releaseBrushData();
	void			rotateData(int iPressure);
	void			turnXData();
	void			turnYData();
	void			circleData(int iPressure);

protected:	
	float			m_fCenterX;
	float			m_fCenterY;
	DWORD			m_dwColor;
	int				m_iHard;
	int				m_iDispelNum;
	int				m_iDitherColor;
	int				m_iDitherArea;
	int				m_iDitherAngle;
	int				m_iDitherCircle;
	int				m_iDispel;
	bool			m_bDefaultBrush;
	BYTE*			m_pData;
	BYTE*			m_pResultData;
	BYTE*			m_pTempData;
	bool			m_bPressureDispelNum;
	bool			m_bPressureDitherColor;
	bool			m_bPressureDitherArea;
	bool			m_bPressureDitherAngle;
	bool			m_bPressureDitherCircle;
	int				m_iWidth;
	int				m_iHeight;
	bool			m_bTurnX;
	bool			m_bTurnY;
	CString			m_strBrushName;
};
