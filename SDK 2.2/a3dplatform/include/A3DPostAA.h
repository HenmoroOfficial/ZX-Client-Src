/*
 * FILE: A3DPostAA.h
 *
 * DESCRIPTION: 反走样后处理
 *
 * CREATED BY: Panyupin, 2012/2/3
 *
 * HISTORY: 
 *
 * Copyright (c) 2012 Archosaur Studio, All Rights Reserved.
 */
#ifndef _A3D_POST_AA_H_
#define _A3D_POST_AA_H_

#include "A3DPost.h"

class A3DPostSystem;

class A3DPostAA : public A3DPost
{
public:
	virtual ~A3DPostAA();
	// 释放
	virtual void Release();
	// 渲染结果
	virtual bool Render(A3DViewport* pViewport);
	// 特效类型
	virtual A3DPOST_EFFECT GetEffectType() const;
	//硬件
	static bool IsHardwareSupport(A3DDevice* pA3DDev);

    bool RenderForRT(A3DViewport* pViewport, A3DRenderTarget* pSrc, A3DRenderTarget* pDst);

    virtual A3DRenderTarget* ApplyEffect(A3DViewport* pViewport, A3DRenderTarget* pSrcRT);

public:
	// 初始化
	bool Init(A3DPostSystem* pSystem);

	A3DPostAA()
	{
		m_pFXAAVS = NULL;
		m_pFXAAPS = NULL;
		m_pAreaTex = NULL;
		m_pSearchTex = NULL;
		m_pAreaTex = NULL;

		m_pSMAAEdgeDetect = NULL;
		m_pSMAABlend = NULL;
		m_pSMAANeighbor = NULL;
		m_pVertexDeclaration = NULL;
	}
private:
	A3DHLSL*			m_pFXAAVS;
	A3DHLSL*			m_pFXAAPS;

	A3DTexture*			m_pAreaTex;
	A3DTexture*			m_pSearchTex;

	A3DHLSL*			m_pSMAAEdgeDetect;
	A3DHLSL*			m_pSMAABlend;
	A3DHLSL*			m_pSMAANeighbor;

	A3DVertexDeclCommon* m_pVertexDeclaration;
	void DrawQuad(int iWidth, int iHeight);
	bool LoadSMAAShader();
    bool DoRender(A3DViewport* pViewport, A3DRenderTarget* pSrc, A3DRenderTarget* pDst);

};



#endif