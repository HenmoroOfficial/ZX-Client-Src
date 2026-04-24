/*
 * FILE: A3DPostWarp.h
 *
 * DESCRIPTION: 扭曲后处理
 *
 * CREATED BY: Panyupin, 2012/3/6
 *
 * HISTORY: 
 *
 * Copyright (c) 2012 Archosaur Studio, All Rights Reserved.
 */

#ifndef _A3D_POST_WARP_H_
#define _A3D_POST_WARP_H_

#include "A3DPost.h"

class A3DPostSystem;

class A3DPostWarp : public A3DPost
{
public:
	virtual ~A3DPostWarp();
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
	bool Init(A3DPostSystem* pSystem);

	void SetScale(float fScale){ m_fScale = fScale; }
	float GetScale(){ return m_fScale; }
	void SetSpeed(float fSpeed){ m_fSpeed = fSpeed; }
	float GetSpeed(){ return m_fSpeed; }
	void EnableMask(bool bEnable) { m_bEnableMask = bEnable; }
	bool IsEnableMask() { return m_bEnableMask; }
	const AString& GetMaskTextureName() { return m_strMaskTexture; }
	void SetMaskTexture(const AString& str);
	A3DPostWarp()
	{
		m_pHLSL = NULL;
		m_pHLSL_Mask = NULL;
		m_pWarpBumpTexture = NULL;
		m_pWarpMaskTexture = NULL;
		m_fScale = 0.01f;
		m_fSpeed = 0.5f;
		m_bEnableMask = false;
		m_strMaskTexture = "";
	}
private:
	A3DHLSL*					m_pHLSL;
	A3DHLSL*					m_pHLSL_Mask;
	A3DTexture*					m_pWarpBumpTexture;
	A3DTexture*					m_pWarpMaskTexture;
	float	m_fScale;			//	扭曲大小
	float	m_fSpeed;			//	扭曲速度
	bool	m_bEnableMask;		//	是否开启mask
	AString m_strMaskTexture;
};


#endif