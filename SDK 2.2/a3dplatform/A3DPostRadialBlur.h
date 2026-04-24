/*
 * FILE: A3DPostRadialBlur.h
 *
 * DESCRIPTION: 径向模糊后处理
 *
 * CREATED BY: Panyupin, 2012/2/8
 *
 * HISTORY: 
 *
 * Copyright (c) 2012 Archosaur Studio, All Rights Reserved.
 */

#ifndef _A3D_POST_RADIALBLUR_H_
#define _A3D_POST_RADIALBLUR_H_

#include "A3DPost.h"

class A3DPostSystem;

class A3DPostRadialBlur : public A3DPost
{
public:
	virtual ~A3DPostRadialBlur();
	// 释放
	virtual void Release();
	// 渲染结果
	virtual bool Render(A3DViewport* pViewport);
	// 特效类型
	virtual A3DPOST_EFFECT GetEffectType() const;
	// 硬件
	static bool IsHardwareSupport(A3DDevice* pA3DDev);
    // 渲染结果
    virtual A3DRenderTarget* ApplyEffect(A3DViewport* pViewport, A3DRenderTarget* pSrcRT);

public:
	// 初始化
	bool	Init(A3DPostSystem* pSystem);

	void	SetBlurCenterWorldSpace(const A3DVECTOR3& vWorldPos, A3DViewport* pViewPort);
	void	SetBlurCenterScreenSpace(float x, float y); // (0.5, 0.5)是屏幕中间, (0,0)是左上角, (1,1)是右下角;
	void	GetBlurCenterScreenSpace(float* pX, float* pY);
	void	SetBlurLength(float fBlurLength);
	float	GetBlurLength();
	void	SetFocusRadius(float fInFocusRadius, float fOutFocusRadius);
	void	GetFocusRadius(float* pfInFocusRadius, float* pfOutFocusRadius);

	A3DPostRadialBlur()
	{
		m_pHLSLRB = NULL;
		m_pHLSLDownSamp = NULL;
		m_pHLSLMerge = NULL;
		m_vBlurCenter = A3DVECTOR3(0.5f, 0.5f, 0);		//屏幕中间
		m_fBlurLength = 0.2f;							
		m_fInFocusRadius = 0.2f;
		m_fOutFocusRadius = 0.7f;
	}
private:
	A3DHLSL*					m_pHLSLRB;	
	A3DHLSL*					m_pHLSLDownSamp;
	A3DHLSL*					m_pHLSLMerge;

	A3DVECTOR3					m_vBlurCenter;
	float						m_fBlurLength;
	float						m_fInFocusRadius;
	float						m_fOutFocusRadius;
};


#endif