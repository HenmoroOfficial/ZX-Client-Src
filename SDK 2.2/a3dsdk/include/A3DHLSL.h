/*
 * FILE: A3DHLSL.h
 *
 * DESCRIPTION: DX9 High Level Shading Language
 *
 * CREATED BY: liuchenglong 2011-08-23
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
 */

#ifndef _A3DHLSL_H_
#define _A3DHLSL_H_

#include <hashmap.h>
#include "A3DMatrix.h"
#include "AString.h"
#include "A3DObject.h"
#include "A3DPlatform.h"

class A3DDevice;
class A3DHLSLMan;
class A3DHLSL;
class A3DHLSLCore;
class AFilePackage;
class A3DVertexDecl;
class A3DHLSLDataSrc;
class A3DRenderTarget;
struct SHADERINSTANCE;
struct A3DCOMMCONSTTABLE;

#define HLSLCORE_VERSION_1		0x80000001
#define HLSLMANAGER_VERSION_1	0xf0000001
#define MAX_SAMPLERCOUNT		16

#define APPEAR_SETSHADERONLY ((const A3DCOMMCONSTTABLE*)-1)

#define ERROR_MTL_FILE "Shaders\\2.2\\HLSL\\Materials\\!error.hlsl"

// A3DHLSLMAN 标志
#define A3DHLSLMAN_FLAG_DONOTREPLACEBYOBJ	0x00000001		// 编辑器中设置该标志读取指定的文件,而不是优先读取编译文件

#define A3DHLSL_CANONICALIZEDMACROS			0x00010000		// 规范化的宏

struct A3DSHDMACRO
{
	AString Name;
	AString Definition;
};

typedef abase::vector<AString>	AStringArray;	// 这个如果被其他地方用到建议提到更公用的头文件中
typedef abase::hash_map<AString, DWORD> Text2EnumDict;
typedef abase::vector<A3DSHDMACRO>			MacroArray;
typedef abase::vector<D3DXMACRO>			D3DXMacroArray;

struct A3DCOMMONCONSTDESC
{
	enum{
		CCD_SAMPLER   = -4,
		CCD_FLOAT     = sizeof(float),
		CCD_FLOAT2    = sizeof(float) * 2,
		CCD_FLOAT3    = sizeof(float) * 3,
		CCD_FLOAT4    = sizeof(float) * 4,
		CCD_COLORVAL  = sizeof(float) * 4,
		CCD_FLOAT4X4  = sizeof(A3DMATRIX4),
	};
	char*	szConstName;
	int		uMemberOffset;
	int		cbSize;
	int		nRef;
};

class A3DCCDBinder
{
public:
	typedef abase::hash_map<DWORD, A3DCOMMONCONSTDESC> CCDDict;
private:
	CCDDict m_CommConstDescDict;
	int		m_cbTable;	// 表中-映射数据的-连续结构的-字节-长度
	int		m_nRefCount;
public:
	A3DCCDBinder();
	bool		Init			(const A3DCOMMONCONSTDESC* pConstDesc, int cbTable);
	bool		Release			();
	CCDDict&	GetDict			();
	const int*	GetBindCountPtr	(char* szName) const;	// 获得某个常量被绑定次数的指针
};

class A3DHLSLCore : public A3DObject
{
	friend class A3DDevice;
	friend class A3DHLSLMan;
	friend class A3DHLSL;
public:
	struct CONSTANTDESC
	{
		D3DXCONSTANT_DESC cd;
		AString			  Semantic;
		DWORD             dwNameID;
		DWORD             dwSemaID;
		unsigned int      bVertexShader : 1;
		unsigned int      bUnique       : 1;
		unsigned int      nIdx          : 16;
		CONSTANTDESC() : dwNameID(0),dwSemaID(0),bVertexShader(0),bUnique(0),nIdx(0)
		{
			memset(&cd, 0, sizeof(D3DXCONSTANT_DESC));
		}
	};
	struct ANNOTATION
	{
		AString				Name;
		union
		{
			BOOL			bValue;
			INT				nValue;
			FLOAT			fValue;
		};
		void*				pData;
		D3DXPARAMETER_CLASS Class;
		D3DXPARAMETER_TYPE	Type;
		UINT				cbDateSize;
	};

	struct SAMPLER_STATE
	{
		AString strSamplerName;
		AString strTextureName;
		AString strSemantic;
		DWORD	SampStateMask;		// 使用标志
		DWORD	SampState[13];		// 记录13种状态
		AString strDefault;

		// 设置采样器状态
		void SetState(D3DSAMPLERSTATETYPE Type, DWORD dwState);

		// 重置采样器状态!!!注意!!!这个只是表示Appear时不再关心状态,而不是恢复到默认状态
		void ResetState(D3DSAMPLERSTATETYPE Type);

		void Cleanup();
	};

	struct MACROPROP
	{
		AString strMacroName;	// 宏的名字
		AString strDesc;		// 描述名字,如果是*开头的则是给程序用的宏
		DWORD	dwGroup;		// 组的掩码, 最大32组, 相同组会互斥
	};

	typedef abase::vector<ANNOTATION>					AnnotationArray;
	struct PARAMETER_DESC
	{
		AString				strSemantic;
		D3DXPARAMETER_CLASS Class;
		D3DXPARAMETER_TYPE	Type;
		AnnotationArray		aAnnotation;
	};

	typedef abase::vector<CONSTANTDESC>					ConstArray;
	typedef abase::hash_map<AString, CONSTANTDESC*>		ConstDict;
	typedef abase::hash_map<AString, PARAMETER_DESC>	Name2ParamDict;
	typedef const SAMPLER_STATE*						LPCSAMPLER_STATE;
	typedef const CONSTANTDESC*							LPCCONSTANTDESC;
	typedef abase::vector<MACROPROP>					MacroPropArray;
protected:
	// 增加成员的话要修改 Transfer 函数
	A3DDevice*               m_pDevice;
	int						 m_nRefCount;
	DWORD					 m_dwID;
	A3DHLSLMan*				 m_pHLSLMgr;

	A3DHLSLDataSrc*			 m_pVSDataSource;
	A3DHLSLDataSrc*			 m_pPSDataSource;

	const SHADERINSTANCE*	 m_pVertexShaderInst;
	const SHADERINSTANCE*	 m_pPixelShaderInst;
	
	ConstArray				 m_aConst;
	ConstDict				 m_NameSemaToConstDict;

	//Name2ParamDict*			 m_pName2ParamDict;
	INT						 m_aSampler[MAX_SAMPLERCOUNT];	// 指向 A3DHLSLDataSrc::SAMPLER_STATE
	UINT					 m_nMaxSamplerIdx;
	AString					 m_strVSFuncName;
	AString					 m_strPSFuncName;
protected:
	bool	GenerateConstTable		(A3DHLSLDataSrc* pDataSrc, const SHADERINSTANCE* pShaderInst, bool bVertexShader);
	bool	GenerateAnnoAndSema		();
	//void	Transfer				(A3DHLSLCore& Src);
	void	BuildConstDict			(/*DWORD dwFlags*/);
	void	GetDefaultTexturePath	(const char* szTextureName, AString& strPath);
	void	GenerateSamplerState	();
	int		GetSemaCount			();
	void	IntGetApproximately		(const SHADERINSTANCE* pShaderInst, int* nTotalInst, int* nTexInst, int* nAriInst);
public:
	A3DHLSLCore(A3DDevice* pA3DDevice, A3DHLSLMan* pHLSLMgr, A3DHLSLDataSrc* pVSDataSrc, A3DHLSLDataSrc* pPSDataSrc);
	~A3DHLSLCore();

	bool	Init			();
	int		AddRef			();
	int		Release			();
	void	Cleanup			();
	bool	LoadCore		(const char* szVSFuncName, const char* szPSFuncName, const char* szMacro);
	bool	Rebuild			();
	void	GetApproximately(int* nVSTotalInst, int* nVSTexInst, int* nVSAriInst, int* nPSTotalInst, int* nPSTexInst, int* nPSAriInst);

	virtual void		SetName			(const char* szName);

public:
	bool	Appear				();
	bool	Disappear			();
	inline  A3DDevice*			GetA3DDevice	();
	inline  A3DHLSLMan*			GetHLSLMgr() const { return m_pHLSLMgr; }
	inline	ConstDict&			GetConstDict();
	inline	ConstArray&			GetConstArray();
			Name2ParamDict*		GetParamDict();
	inline	A3DHLSLDataSrc*		GetDataSource(int nShader);
			LPCSAMPLER_STATE	GetSamplerState(int nSamplerIdx);
	inline	LPCCONSTANTDESC		GetConstDescByName(const char* szName);
	inline	DWORD				GetID();
};
 

class A3DHLSL : public A3DObject
{
	friend class A3DHLSLMan;
public:
	struct CONSTREGCONTEXT
	{
		int nMin, nMax;
		A3DVECTOR4* pConstBuffer;
		CONSTREGCONTEXT();
		virtual ~CONSTREGCONTEXT();
		void Build();
	};
	struct CONSTMAPDESC
	{
		int					uMemberOffset;
		A3DHLSLCore::CONSTANTDESC cd;
		CONSTMAPDESC()
			: uMemberOffset(0){}
	};
	typedef abase::vector<CONSTMAPDESC> ConstMapArray;
	typedef A3DHLSLCore::ConstDict ConstDict;
	typedef A3DHLSLCore::ConstArray ConstArray;
	typedef A3DHLSLCore::ANNOTATION ANNOTATION;

	struct SAMPLERSLOT
	{
		A3DObject*				pA3DObject;
		LPDIRECT3DBASETEXTURE9	pTextureBase;
		unsigned int			bDepthTex;		// 设置 A3DRenderTarget 做纹理时以DepthTex作为纹理
		//DWORD					SampStateMask;
		//DWORD					SampState[13];

		void UpdateD3DTexPtr(A3DObject* _pA3DObject);
	};


	typedef A3DCCDBinder::CCDDict				CCDDict;

private:
	A3DHLSLCore*			m_pCore;
	CONSTMAPDESC*			m_aConstName;
	//int						m_cbConstTableSize;	// 只是为了检查
	CONSTREGCONTEXT			m_VertexRegContext;
	CONSTREGCONTEXT			m_PixelRegContext;
	//A3DCOMMONCONSTDESC*		m_lpCommConstDesc;
	A3DCCDBinder*			m_lpDataBinder;
	SAMPLERSLOT				m_aSamplerSlot[MAX_SAMPLERCOUNT];

	A3DHLSL(A3DHLSLCore* pCore);

	bool	Init			();
	bool	GenConstBuffer	(CONSTREGCONTEXT* pContext, bool bVertexShader);
	bool	Release			();
	bool	Rebuild			();
public:
	virtual ~A3DHLSL();
	bool	UnbindCommConstData	();
	//bool	BindCommConstData	(A3DCOMMONCONSTDESC* lpCommConstDesc, int cbTable);	// 第一个参数必须是静态结构, 不能是局部变量, cbTable 是数据源的长度,不是A3DCOMMONCONSTDESC的长度
	bool	BindCommConstData	(A3DCCDBinder* lpCCDBinder);
	bool	Appear				(const A3DCOMMCONSTTABLE* lpCommConstTable = NULL); // 如果参数是-1的话,只设置Shader,不设置任何常量寄存器
	bool	Commit				(const A3DCOMMCONSTTABLE* lpCommConstTable = NULL);
	bool	Disappear			();
	bool	SubmitCommTable		(const A3DCOMMCONSTTABLE* lpCommConstTable);		// 只提交常量表
	bool	SetValue1f			(const char* szName, const float fConstant);
	bool	SetValue2f			(const char* szName, const float fConstant1, const float fConstant2);
	bool	SetValue3f			(const char* szName, const A3DVECTOR3* pConstant);
	bool	SetValue4f			(const char* szName, const A3DVECTOR4* pConstant);
	bool	SetValue1i			(const char* szName, const int n1);
	bool	SetValue2i			(const char* szName, const int n1, const int n2);
	bool	SetValue3i			(const char* szName, const int n1, const int n2, const int n3);
	bool	SetValue4i			(const char* szName, const int n1, const int n2, const int n3, const int n4);
	bool	SetValue1b			(const char* szName, const bool b1);
	bool	SetValue2b			(const char* szName, const bool b1, const bool b2);
	bool	SetValue3b			(const char* szName, const bool b1, const bool b2, const bool b3);
	bool	SetValue4b			(const char* szName, const bool b1, const bool b2, const bool b3, const bool b4);
	bool	SetConstantArrayF	(const char* szName, const A3DVECTOR4* pConstant, int nFloat4Count);
	bool	SetConstantMatrix	(const char* szName, const A3DMATRIX4& mat);

	//为了与2.1中的A3DHLSL兼容
	bool	Setup1f(const char* szName, float v0){ return SetValue1f(szName, v0); }
	bool	Setup2f(const char* szName, float v0, float v1){ return SetValue2f(szName, v0, v1); }
	bool	Setup3f(const char* szName, float v0, float v1, float v2){ return SetValue3f(szName, &A3DVECTOR3(v0, v1, v2)); }
	bool	Setup4f(const char* szName, float v0, float v1, float v2, float v3){ return SetValue4f(szName, &A3DVECTOR4(v0, v1, v2, v3)); }
	bool	SetupVec3(const char* szName, float v[3]){ return Setup3f(szName, v[0], v[1], v[2]); }
	bool	SetupVec3(const char* szName, const A3DVECTOR3& v3){ return SetValue3f(szName, &v3);}
	bool	SetupVec4(const char* szName, float v[4]){ return Setup4f(szName, v[0], v[1], v[2], v[3]); }
	bool	SetupVec4(const char* szName, const A3DVECTOR4& v4){ return SetValue4f(szName, &v4); }
	bool	SetupArray(const char* szName, const float* v, int iFloatCnt){ ASSERT(iFloatCnt % 4 == 0); return SetConstantArrayF(szName, (const A3DVECTOR4*)v, iFloatCnt / 4); }
	bool	SetupMatrix(const char* szName, const A3DMATRIX4& mat){ return SetConstantMatrix(szName, mat); }


	bool	SetTexture			(const char* szName, A3DObject* pA3DObject);	// szName 是Sampler的名字,不是Texture的名字
	bool	SetTextureFromRT	(const char* szName, A3DRenderTarget* pRanderTarget, bool bUseDepth = false);
	bool	SetupTextureFromRT	(const char* szName, A3DRenderTarget* pRanderTarget, bool bUseDepth = false){ return SetTextureFromRT(szName, pRanderTarget, bUseDepth); }
	A3DObject* GetTexture		(int nSamplerIdx);
	LPDIRECT3DBASETEXTURE9 GetD3DTexture(int nSamplerIdx) { return m_aSamplerSlot[nSamplerIdx].pTextureBase; }

	bool	GetConstantArrayF	(const char* szName, A3DVECTOR4* pConstant, int nFloat4Count);
	bool	ReloadTexture		();

	//A3DHLSL*Clone				(bool bRegisterToMgr);

	bool	SetMacroAdapter		(const char* szMacros, DWORD dwFlags);		// 设置私有宏(序列)
	bool	GetMacroAdapter		(AString& strMacros);		// 获得私有宏
	bool	CombineMacro		(const char* szMacros, DWORD dwFlags);		// 把参数中的宏合并到私有宏中
	bool	SubtractMacro		(const char* szMacros);		// 私有宏减掉参数宏

	const ANNOTATION* FindAnnotationByName(const char* szName, const char* szAnnoName);
	const CONSTMAPDESC* GetConstMapByName(const char* szName);
	inline const CONSTMAPDESC* GetConstMapArray() const;
	inline int GetConstCount() const;
	inline A3DHLSLCore* GetCore() const;
	inline DWORD GetCoreID() const;

	static A3DCCDBinder* CreateCommonConstDataBinder(const A3DCOMMONCONSTDESC* pConstDesc, int cbTable);
};

class A3DHLSLMan
{
	friend class A3DHLSLDataSrc;
	friend class A3DHLSL;
	friend class A3DHLSLCore;
	friend class A3DEffect;
public:
	typedef abase::vector<A3DHLSLCore*>			ShaderCorePtrArray;
	typedef abase::vector<A3DHLSL*>				ShaderInstPtrArray;
	typedef abase::hash_map<AString, A3DHLSLDataSrc*> HLSLDataSrcDict;
	typedef abase::hash_map<AString, FILETIME>	FileTimeDict;
	typedef void (*LPLOGRECEIVER)(const char*);

private:
	A3DDevice*			m_pDevice;
	ShaderCorePtrArray	m_aShaderCorePtrArray;
	bool				m_bChanged;
	FileTimeDict		m_FileTimeDict;
	ShaderInstPtrArray	m_aShaderInstPtr;
	bool				m_bEnableCache;
	LPLOGRECEIVER		m_lpLogReceiver;
	//DWORD				m_dwFlags;
	HLSLDataSrcDict		m_DataSrcDict;
	CRITICAL_SECTION	m_Locker;

	static Text2EnumDict* s_pText2EnumDict;

	ShaderCorePtrArray::iterator
					FindByName			(const AString& strName);
	bool			RegisterRefFile		(const AString& strFile);
	bool			CheckUpdate			();
	static bool		GenCompiledFilename	(const char* szVertexShaderFile, const char* szPixelShaderFile, DWORD dwNameID, AString& strFilename);
	A3DHLSLDataSrc*	MountDataSource		(const char* szShaderFile);
	A3DHLSLCore*	LoadCore			(const char* pszVertexShaderFile, const char* szVSFuncName, const char* pszPixelShaderFile, const char* szPSFuncName, const char* szMacro, DWORD dwFlags);
	bool			ReleaseCore			(A3DHLSLCore* pCore);
public:
	void			AppendErrorMsg		(const char* szFormat, ...);
	static bool		GenerateName		(const char* pszVertexShaderFile, const char* szVSFuncName, const char* pszPixelShaderFile, const char* szPSFuncName, MacroArray& aMacros, AString& strName);
	static bool		ParseName			(const AString& strName, AString* pVertexShaderFile, AString* pVSFuncName, AString* pPixelShaderFile, AString* pPSFuncName, AString* pMacros);
	static bool		ParseNameFromFile	(const char* szShaderName, AString* pVertexShaderFile, AString* pVSFuncName, AString* pPixelShaderFile, AString* pPSFuncName, AString* pMacros);
	static bool		BuildD3DMacro			(const MacroArray& aMacros, D3DXMacroArray& aD3DXMacros);
	static bool		StringToMacroArray		(const char* szMacros, MacroArray& aMacros);
	static bool		MacroArrayToString		(const MacroArray& aMacros, AString& strMacros);
	static bool		StringArrayToMacroArray	(const AStringArray& aStrings, MacroArray& aMacros);
	static bool		CanonicalizeMacroArray	(MacroArray& aMacros);
	static bool		CanonicalizeMacroString	(const char* szMacros, AString& strMacros);
public:
	A3DHLSLMan(A3DDevice* pA3DDevice);
	~A3DHLSLMan();
	bool		Init				(bool bEnableCache);
	bool		Release				();
	A3DHLSL*	LoadShader			(const char* pszVertexShaderFile, const char* szVSFuncName, const char* pszPixelShaderFile, const char* szPSFuncName, const char* szMacro, DWORD dwFlags);
	A3DHLSL*	LoadShader			(const char* pszVertexShaderFile, const char* pszPixelShaderFile, const char* szMacro);
	void		ReleaseShader		(A3DHLSL* pHLSL);
	bool		RebuildAll			();
	bool		RebuildByKeyword	(const char* pszKeyword);
	void		SetLogReceiver		(LPLOGRECEIVER lpLogReceiver);
	//inline DWORD GetFlags			();
	//inline void  SetFlags			(DWORD dwFlags);
};

int		SplitString				(const AString& str, char ch, AStringArray& aString);

//	Helper class for a3dhlsl
class A3DHLSLScopedPtr
{
	A3DHLSL* m_pHLSL;
public:
	A3DHLSLScopedPtr(A3DHLSL* pHLSL)
		: m_pHLSL(pHLSL)
	{

	}

	~A3DHLSLScopedPtr()
	{
		if (m_pHLSL)
		{
			m_pHLSL->GetCore()->GetHLSLMgr()->ReleaseShader(m_pHLSL);
		}
	}

	A3DHLSL* operator -> () const { ASSERT(m_pHLSL != 0); return m_pHLSL; }
	A3DHLSL& operator * () const { ASSERT(m_pHLSL != 0); return *m_pHLSL; }
	A3DHLSL* get() const { return m_pHLSL; }

private:
	A3DHLSLScopedPtr(const A3DHLSLScopedPtr&);
	A3DHLSLScopedPtr& operator = (const A3DHLSLScopedPtr&);
};


//////////////////////////////////////////////////////////////////////////
//
// inline
//
inline A3DDevice* A3DHLSLCore::GetA3DDevice()
{
	return m_pDevice;
}

inline A3DHLSLCore::ConstDict& A3DHLSLCore::GetConstDict()
{
	return m_NameSemaToConstDict;
}
inline A3DHLSLCore::ConstArray& A3DHLSLCore::GetConstArray()
{
	return m_aConst;
}
inline A3DHLSLDataSrc* A3DHLSLCore::GetDataSource(int nShader)
{
	if(nShader == 0)
		return m_pVSDataSource;
	else
		return m_pPSDataSource;
}

inline A3DHLSLCore::LPCCONSTANTDESC A3DHLSLCore::GetConstDescByName(const char* szName)
{
	ConstDict::iterator it = m_NameSemaToConstDict.find(szName);
	if(it == m_NameSemaToConstDict.end()) {
		return NULL;
	}
	return it->second;
}

inline DWORD A3DHLSLCore::GetID()
{
	return m_dwID;
}

inline const A3DHLSL::CONSTMAPDESC* A3DHLSL::GetConstMapArray() const
{
	return m_aConstName;
}

inline int A3DHLSL::GetConstCount() const
{
	return (int)m_pCore->GetConstArray().size();
}

inline A3DHLSLCore* A3DHLSL::GetCore() const
{
	return m_pCore;
}

inline DWORD A3DHLSL::GetCoreID() const
{
	return m_pCore ? m_pCore->GetID() : NULL;
}

//inline DWORD A3DHLSLMan::GetFlags()
//{
//	return m_dwFlags;
//}
//inline void  A3DHLSLMan::SetFlags(DWORD dwFlags)
//{
//	m_dwFlags = dwFlags;
//}

//////////////////////////////////////////////////////////////////////////
// LoadShader szMacro格式: "MACRO1;MACRO2=x;MACRO03;..."

//////////////////////////////////////////////////////////////////////////
// SetCommConstDesc	(A3DCOMMONCONSTDESC* lpCommConstDesc, A3DCOMMONCONSTDESC* lpCommMark, int cbTable);
//		lpCommConstDesc: common const 描述表的指针
//		cbTable: common const 变量表的指针


//全局函数, 2.1与2.2通用
A3DHLSL* CreateA3DHLSL_Common(const char* szVSFileName, const char* szVSEntryName,
								  const char* szPSFileName, const char* szPSEntryName,
								  int nMacroNum, const char** szMacroList,
								  A3DDevice* pA3DDev, const D3DVERTEXELEMENT9* aVertElement9);
void ReleaseA3DHLSL_Common(A3DDevice* pDev, A3DHLSL* pHLSL);
bool ReloadA3DHLSL_Common(A3DHLSL* pHLSL);


#endif // _A3DHLSL_H_