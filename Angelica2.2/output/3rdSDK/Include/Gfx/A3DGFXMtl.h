/*
* FILE: A3DGFXMtl.h
*
* DESCRIPTION: 
*
* CREATED BY: Zhangyachuan (zhangyachuan000899@wanmei.com), 2012/4/11
*
* HISTORY: 
*
* Copyright (c) 2012 Archosaur Studio, All Rights Reserved.
*/

#ifndef _A3DGFXMtl_H_
#define _A3DGFXMtl_H_

#include "AString.h"
#include <vector>
#include <map>

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////

class A3DEngine;
class A3DHLSLWrapper;
class A3DHLSLParamRT;
class A3DHLSLTexParams;

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class A3DGFXMtl
//	
///////////////////////////////////////////////////////////////////////////

class A3DGFXMtl
{
public:		//	Types
	
	//typedef std::pair<AString, int> SamplerInfo;
	//class TEXTURE_SLOT
	//{
	//public:
	//	TEXTURE_SLOT(A3DEngine* pA3DEngine)
	//		: m_pTexture(NULL)
	//		, m_pA3DEngine(pA3DEngine)
	//	{
	//	}
	//	~TEXTURE_SLOT();
	//	TEXTURE_SLOT(const TEXTURE_SLOT&);
	//	TEXTURE_SLOT& operator = (const TEXTURE_SLOT&);
	//	void Swap(TEXTURE_SLOT& rhs);

	//	const char* GetTexFile() const { return m_strTexture; }
	//	void SetTexFile(const char* szTex);
	//	A3DTexture* GetTexture() const { return m_pTexture; }
	//	void AddRelatedSampler(const char* szSampler, int nSamplerIdx) { m_aSamplers.push_back(std::make_pair(szSampler, nSamplerIdx)); }
	//	int GetSamplerNum() const { return static_cast<int>(m_aSamplers.size()); }
	//	const SamplerInfo& GetSamplerParam(int nIdx) const { return m_aSamplers[nIdx]; }
	//private:
	//	AString m_strTexture;
	//	A3DEngine* m_pA3DEngine;
	//	A3DTexture* m_pTexture;
	//	std::vector<SamplerInfo> m_aSamplers;
	//private:
	//	void Release();
	//};

public:		//	Constructor and Destructor

	A3DGFXMtl(void);
	virtual ~A3DGFXMtl(void);

	A3DGFXMtl(const A3DGFXMtl& rhs);
	A3DGFXMtl& operator = (const A3DGFXMtl& rhs);
	void Swap(A3DGFXMtl& rhs);

public:		//	Attributes

public:		//	Operations

	bool Init(A3DEngine* pA3DEngine);
	void Release();

	bool IsValid() const { return m_pHLSLWrapper && m_pParamRT; }
	//	UI & Edit related
	//	Set material ps file
	bool SetPSFile(const char* szFile);

	//int GetTextureNum() const;
	//const char* GetTextureUIName(int nIdx) const;
	//const char* GetTextureTex(int nIdx) const;
	//void SetTextureFile(int nIdx, const char* szTex);

	//	Runtime constant setup
	bool SetupTexture(const char* szParam, A3DTexture* pTexture);
	bool SetupArray(const char* szParam, const float* v, int iFloatCnt);

	bool Appear();
	bool Disappear();
	
	inline A3DEngine* GetA3DEngine() const { return m_pA3DEngine; }

private:	//	Attributes

	A3DEngine* m_pA3DEngine;
	A3DHLSLWrapper* m_pHLSLWrapper;
	A3DHLSLParamRT* m_pParamRT;
	//A3DHLSLTexParams* m_pTexParams;
	//typedef std::vector<TEXTURE_SLOT*> TEXTURESLOTARRAY;
	//TEXTURESLOTARRAY m_aTextureSlots;

private:	//	Operations

//	TEXTURE_SLOT* FindTextureSlot(const char* szTexParam) const;
	bool InitHLSLWrapper(const char* szFile);
	bool ParseUIParamInfo(const char* szFile);

};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////


#endif	//	_A3DGFXMtl_H_

