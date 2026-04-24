/*
 * FILE: A3DPostDof.h
 *
 * DESCRIPTION: 景深后处理
 *
 * CREATED BY: Panyupin, 2012/2/7
 *
 * HISTORY: 
 *
 * Copyright (c) 2012 Archosaur Studio, All Rights Reserved.
 */
#ifndef _A3D_POST_DOF_H_
#define _A3D_POST_DOF_H_

#include "A3DPost.h"

class A3DPostSystem;

class A3DPostDof : public A3DPost
{

public:
	virtual ~A3DPostDof();
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

	void		SetClampBlurFar(float fBlurFar);
	float		GetClampBlurFar(){ return m_fClampBlurFar;}
	void		SetFocus(float fNearBlur, float fNearFocus, float fFarFocus, float fFarBlur);
	void		GetFocus(float* pfNearBlur, float* pfNearFocus, float* pfFarFocus, float* pfFarBlur);

	A3DPostDof()
	{
		m_fNearFocus = 1.f;
		m_fFarFocus = 10.f;
		m_fNearBlur = 0.1f;
		m_fFarBlur = 30.f;
		m_fClampBlurFar = 1.f;
	}
private:

	A3DHLSL*			m_pHLSL_FillAlphaWithCoC;
	A3DHLSL*			m_pHLSL_DownScaleRGBCoC;
	A3DHLSL*			m_pHLSL_BlurMaxRGBCoCX;
	A3DHLSL*			m_pHLSL_BlurMaxRGBCoCY;
	A3DHLSL*			m_pHLSL_DownScaleDepth;
	A3DHLSL*			m_pHLSL_BlurDepthX;
	A3DHLSL*			m_pHLSL_BlurDepthY;
	A3DHLSL*			m_pHLSL_SC2_Dof;

	float m_fNearFocus;	//焦点范围的其中一端
	float m_fFarFocus;	//焦点范围的其中另一端,在焦点范围两端之内的物体没有模糊
	float m_fNearBlur;   //近端,到达或者近于此距离为模糊最大
	float m_fFarBlur;     //远端,到达或者超过此距离为模糊最大
	float m_fClampBlurFar;//远端模糊的最大值,0到1之间.一般设为1就可以
};



#endif