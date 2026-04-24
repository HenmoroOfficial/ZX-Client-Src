/*
 * FILE: A3DPostCC.h
 *
 * DESCRIPTION: 色彩校正后处理
 *
 * CREATED BY: Panyupin, 2012/2/1
 *
 * HISTORY: 
 *
 * Copyright (c) 2012 Archosaur Studio, All Rights Reserved.
 */
#ifndef _A3D_POSTCC_H_
#define _A3D_POSTCC_H_
#include "A3DPost.h"

#ifdef _ANGELICA_2_2
#include "ElementRenderParam.h"
#endif

class A3DPostSystem;
class A3DVolumeTexture;

class A3DPostCC : public A3DPost
{
public:
	virtual ~A3DPostCC();
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

	void			SetFactor(float f);
	float			GetFactor(){ return m_fFactor; }

	const char*		GetCCTexture1(){ return m_strCurTex1; }
	const char*		GetCCTexture2(){ return m_strCurTex2; }

	bool SetCCTexture1(const char* pVolTex1);
	bool SetCCTexture2(const char* pVolTex2);

	bool SetCCTexture(const char* pVolTex1, const char* pVolTex2);

	A3DPostCC()
	{
		m_fFactor = 0.f;
		m_pColorCorrectionTex1 = NULL;
		m_pColorCorrectionTex2 = NULL;
		m_pHLSLcc = NULL;
		m_strCurTex1 = "";
		m_strCurTex2 = "";
#ifdef _ANGELICA_2_2
		m_bIsManual1 = false;
		m_bIsManual2 = false;
#endif
	}
private:
	A3DVolumeTexture*			m_pColorCorrectionTex1;
	A3DVolumeTexture*			m_pColorCorrectionTex2;
	A3DHLSL*					m_pHLSLcc;	
	
	float						m_fFactor;
	AString						m_strCurTex1;
	AString						m_strCurTex2;

	static A3DVolumeTexture* CreateDefaultCCTexture(A3DDevice* pDev);

	bool ReLoadVolumeTexture(A3DVolumeTexture** ppVolumeTexture, const char* szFileName);

#ifdef _ANGELICA_2_2
private:
	bool						m_bIsManual1;
	bool						m_bIsManual2;

	A3DPOST_EFFECT_CC_PARAM::ManualParam m_manualParam1;
	A3DPOST_EFFECT_CC_PARAM::ManualParam m_manualParam2;

	static A3DVolumeTexture* ConvertVolumeTextureByManualParam(A3DDevice* pDev, A3DVolumeTexture* pVolume, 
		const A3DPOST_EFFECT_CC_PARAM::ManualParam& manualParam);

public:
	static bool CreateCCVolumeTexture(A3DDevice* pDev, const char* szOutputTextureFileName, const char* szInputTextureFileName,
		const A3DPOST_EFFECT_CC_PARAM::ManualParam& manualParam);



	bool SetCCTexture1ByManualParam(const A3DPOST_EFFECT_CC_PARAM::ManualParam& manualParam);
	bool SetCCTexture2ByManualParam(const A3DPOST_EFFECT_CC_PARAM::ManualParam& manualParam);
	
	bool IsCC1Manual(){ return m_bIsManual1; }
	bool IsCC2Manual(){ return m_bIsManual2; }

	A3DPOST_EFFECT_CC_PARAM::ManualParam GetManualParam1(){ return m_manualParam1; }
	A3DPOST_EFFECT_CC_PARAM::ManualParam GetManualParam2(){ return m_manualParam2; }

#endif
};



#endif