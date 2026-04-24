/*
 * FILE: A3DPostFlare.h
 *
 * DESCRIPTION: 渲染太阳和月亮光晕
 *
 * CREATED BY: Yaochunhui, 2012/4/6
 *
 * HISTORY: 
 *
 * Copyright (c) 2012 Archosaur Studio, All Rights Reserved.
 */
#ifndef _A3D_POST_FLARE_H_
#define _A3D_POST_FLARE_H_

#include "A3DPost.h"

class A3DPostSystem;
class A3DSunMoon;

class A3DPostFlare : public A3DPost
{
public:
	virtual ~A3DPostFlare();
	// 释放
	virtual void Release();
	// 渲染结果
	virtual bool Render(A3DViewport* pViewport);
	// 特效类型
	virtual A3DPOST_EFFECT GetEffectType() const;
	//硬件
	static bool IsHardwareSupport(A3DDevice* pA3DDev);
    // 渲染结果
    virtual A3DRenderTarget* ApplyEffect(A3DViewport* pViewport, A3DRenderTarget* pSrcRT);

public:
	// 初始化
	bool Init(A3DPostSystem* pSystem);

	A3DPostFlare()
	{
        m_pSunMoon = NULL;
	}

    void SetSunMoon(A3DSunMoon* pSunMoon) { m_pSunMoon = pSunMoon; }
private:
	A3DSunMoon*		m_pSunMoon;

};



#endif