/*
 * FILE: A3DPostMotionBlur.h
 *
 * DESCRIPTION: 运动模糊后处理
 *
 * CREATED BY: Panyupin, 2012/3/6
 *
 * HISTORY: 
 *
 * Copyright (c) 2012 Archosaur Studio, All Rights Reserved.
 */

#ifndef _A3D_POST_MOTIONBLUR_H_
#define _A3D_POST_MOTIONBLUR_H_

#include "A3DPost.h"

class A3DPostSystem;

class A3DPostMotionBlur : public A3DPost
{
public:
	virtual ~A3DPostMotionBlur();
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

	void SetBlurMax(float fBlurMax){ m_fBlurMax = fBlurMax; }
	float GetBlurMax(){ return m_fBlurMax; }
	void SetBlurScale(float fBlurScale){ m_fBlurScale = fBlurScale; }
	float GetBlurScale(){ return m_fBlurScale; }

	void SetBlurScalePosition(float fBlurScalePosition){ m_fBlurScalePosition = fBlurScalePosition; }
	float GetBlurScalePosition(){ return m_fBlurScalePosition; }
	void SetBlurScaleRotation(float fBlurScaleRotation){ m_fBlurScaleRotation = fBlurScaleRotation; }
	float GetBlurScaleRotation(){ return m_fBlurScaleRotation; }

	void SetInFocusDistance(float fInFocusDistance){ m_fInFocusDistance = fInFocusDistance; }
	float GetInFocusDistance(){ return m_fInFocusDistance; }
	void SetOutFocusDistance(float fOutFocusDistance){ m_fOutFocusDistance = fOutFocusDistance; }
	float GetOutFocusDistance(){ return m_fOutFocusDistance; }

	A3DPostMotionBlur()
	{
		m_pHLSL = NULL;
		m_pDownSampHLSL = NULL;
		m_pMergeHLSL = NULL;
		m_pA3DStreamSphere = NULL;
		m_pA3DStreamSphereDecl = NULL;
		m_bFirstRun = true;

		m_fBlurMax = 0.03f;
		m_fBlurScale = 0.05f;
		m_fBlurScalePosition = 3.0f;
		m_fBlurScaleRotation = 10.0f;
		m_fInFocusDistance = 15.f;
		m_fOutFocusDistance = 80.f;
	}
private:
	A3DHLSL*					m_pHLSL;
	A3DHLSL*					m_pDownSampHLSL;
	A3DHLSL*					m_pMergeHLSL;

	bool						m_bFirstRun;
	A3DStream* m_pA3DStreamSphere;
	A3DVertexDeclCommon* m_pA3DStreamSphereDecl;
	A3DVECTOR3 m_vCameraPosLast;
	A3DVECTOR3 m_vCameraDirLast;
	A3DVECTOR3 m_vCameraUpLast;

	A3DVECTOR3 m_vRealCameraPosLast;
	A3DVECTOR3 m_vRealCameraDirLast;
	A3DVECTOR3 m_vRealCameraUpLast;

	float m_fBlurMax;	       //模糊量的最大值,0~1之间,一般0.03左右
	float m_fBlurScale;        //模糊量控制，即模糊量与摄像机运动速度之间的关系
 	float m_fBlurScalePosition;//摄像机位置移动造成的模糊量的权重 1~10
 	float m_fBlurScaleRotation;//摄像机旋转造成的模糊的权重 1~10
	float m_fInFocusDistance;  //在该深度范围内的像素不会被模糊
	float m_fOutFocusDistance; //在该深度范围之外的像素会被最大限度模糊
};


#endif