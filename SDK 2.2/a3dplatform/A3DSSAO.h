/*
 * FILE: A3DSSAO.h
 *
 * DESCRIPTION: 渲染SSAO
 *
 * CREATED BY: Panyupin, 2012/5/21
 *
 * HISTORY: 
 *
 * Copyright (c) 2012 Archosaur Studio, All Rights Reserved.
 */
#ifndef _A3DSSAO_H_
#define _A3DSSAO_H_
#include "A3DVertexShader.h"
#include "A3DMatrix.h"

class A3DDevice;
class A3DPixelShader;
class A3DVertexShader;
class A3DViewport;
class A3DRenderTarget;
class A3DTexture;
class A3DHLSL;

class A3DSSAO
{
public:
	A3DSSAO();
	bool Init(A3DDevice * pDevice);
	void Release();

	bool Render(A3DViewport * pViewPort, A3DRenderTarget* pInputRT);
	
	float	GetRadius() const;					//AO半径 0~2
	void	SetRaidus(float fRadius);
	float	GetContrast() const;				//AO对比度 0~6
	void	SetContrast(float fContrast);
	float	GetDarkest() const;					//AO最暗处亮度 0~1
	void	SetDarkest(float fDarkest);

	~A3DSSAO(){}
private:
	void ReloadAo2Shader();

	A3DDevice * m_pDevice;

	A3DHLSL* m_pAo2CommonVS;		//通用的后处理VertexShader
	A3DHLSL* m_pAo2SSAOVS;			//SSAO pass用的VertexShader
	A3DHLSL* m_pAo2DownSampPS;		//降采样深度的PixelShader
	A3DHLSL* m_pAo2Cry3PS;			//CryEngine3的SSAO算法
	A3DHLSL* m_pAo2LinearDepthPS;	//将非线性深度转换为线性的(同时也计算出了法线).
	
	A3DHLSL* m_pAo2BlurSmallXPS;		//将一半大小的SSAO结果进行Blur;
	A3DHLSL* m_pAo2BlurSmallYPS;		//将一半大小的SSAO结果进行Blur;
	A3DHLSL* m_pAo2BlurNew;

	A3DRenderTarget* m_pLinearDepthRT;				//存储线性深度的RT
	A3DRenderTarget* m_pSmallGBufferRT;				//缩小一半的深度RT
	A3DRenderTarget* m_pSmallGBufferRTLast;			//上一帧的深度RT
	A3DRenderTarget* m_pSSAORT;						//存储SSAO结果的的RT(一半大小)
	A3DRenderTarget* m_pSSAORTLast;					//上一帧的SSAO结果
	A3DRenderTarget* m_pInputFrameBuffer;			//存储进入ssao时的帧缓存

	A3DMATRIX4		m_matViewProjLastTM;
	A3DVertexDeclCommon* m_pVertDecl;
	A3DTexture* m_pD3DRandom4x4Texture;//随机纹理

	float m_fRadius;
	float m_fContrast;
	float m_fDarkest;
};

#endif