/*
 * FILE: A3DPostFullGlow.h
 *
 * DESCRIPTION: FullGlow后处理
 *
 * CREATED BY: Panyupin, 2012/3/8
 *
 * HISTORY: 
 *
 * Copyright (c) 2012 Archosaur Studio, All Rights Reserved.
 */
#ifndef _A3D_POST_FULLGLOW_H_
#define _A3D_POST_FULLGLOW_H_

#include "A3DPost.h"

class A3DPostSystem;

class A3DPostFullGlow : public A3DPost
{
public:
	virtual ~A3DPostFullGlow();
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

	void	SetBlurSize(float fBlurSize);
	float	GetBlurSize(){ return m_fBlurSize; }
	void	SetGlowLevel(float fGlowLevel){ m_fGlowLevel = fGlowLevel; }
	float	GetGlowLevel(){ return m_fGlowLevel; }
	void	SetGlowPower(float fGlowPower){ m_fGlowPower = fGlowPower; }
	float	GetGlowPower(){ return m_fGlowPower; }
	void	SetGlowColor(const A3DVECTOR4& color){ m_vGlowColor = color; }
	const A3DVECTOR4& GetGlowColor(){ return m_vGlowColor; }

	A3DPostFullGlow()
	{
		m_pThresholdPS = NULL;
		m_pBlurXPS = NULL;
		m_pBlurYPS = NULL;
		m_pMergePS = NULL;

		//默认值
		m_fBlurSize = 1.f;
		m_fGlowLevel = 0.5f;
		m_fGlowPower = 0.5f;
		m_vGlowColor = A3DVECTOR4(0.0f, 0.0f, 1.f, 1.0);

	}
private:
	A3DHLSL*			m_pThresholdPS;
	A3DHLSL*			m_pBlurXPS;
	A3DHLSL*			m_pBlurYPS;
	A3DHLSL*			m_pMergePS;

	float			m_fBlurSize;				//模糊距离，占屏幕大小的百分比，0~0.1
	float			m_fGlowLevel;
	float			m_fGlowPower;
	A3DVECTOR4	m_vGlowColor;
};


#endif