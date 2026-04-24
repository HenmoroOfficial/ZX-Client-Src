// StaticModelObject.h: interface for the CStaticModelObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATICMODELOBJECT_H__59519995_09C2_479E_8E2E_D44AF89064B0__INCLUDED_)
#define AFX_STATICMODELOBJECT_H__59519995_09C2_479E_8E2E_D44AF89064B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STATIC_OBJ_LITMAP_SIZE_FACTOR 16.0f

#include "SceneObject.h"
#include "A3DModel.h"

#include "ModelContainer.h"
#include "ElementResMan.h"

struct LIGHTDATA;
struct MTLLIST_DATA;
class CStaticModelObject;

struct LIGHTMAP_SIZE_FUNCS : public CUSTOM_FUNCS
{
    LIGHTMAP_SIZE_FUNCS(CStaticModelObject* pOwner): m_pOwner(pOwner) {}
    virtual BOOL CALLBACK OnActivate(void);
    virtual LPCTSTR CALLBACK OnGetShowString(void) const;
    virtual AVariant CALLBACK OnGetValue(void) const;
    virtual void CALLBACK OnSetValue(const AVariant& var);

    CStaticModelObject* m_pOwner;
    AVariant	m_var;
    CString		m_strText;

};

struct EFFECT_PROPERTY_FUNCS : public CUSTOM_FUNCS
{
	void SetModel(A3DLitModel* pA3DLitModel);
	virtual BOOL CALLBACK OnActivate(void);
	virtual LPCTSTR CALLBACK OnGetShowString(void) const;
	virtual AVariant CALLBACK OnGetValue(void) const;
	virtual void CALLBACK OnSetValue(const AVariant& var);

	A3DLitModel* m_pA3DLitModel;
	AVariant	m_var;
	CString		m_strText;
};

class CStaticModelObject : public CSceneObject
{
public:
	CStaticModelObject();
	virtual ~CStaticModelObject();
	
	virtual A3DVECTOR3	GetMin();
	virtual A3DVECTOR3	GetMax();

	virtual void ReInit() /*override*/; // 在拷贝构造或者赋值后调用，将指针字段初始化为NULL或者新对象。赋值前还应调用Release
	virtual void PostRender(A3DViewport* pA3DViewport);
	virtual void Render(A3DViewport* pA3DViewport);
	virtual void RenderForLightMapShadow(A3DViewport* pA3DViewport);   
	virtual void RenderForLight(A3DViewport* pA3DViewport, float fOffsetX, float fOffsetZ);
	virtual void Translate(const A3DVECTOR3& vDelta){ CSceneObject::Translate(vDelta); UpdateModel();}
	virtual void RotateX(float fRad){ CSceneObject::RotateX(fRad); UpdateModel();}
	virtual void RotateY(float fRad){ CSceneObject::RotateY(fRad); UpdateModel();}
	virtual void RotateZ(float fRad){ CSceneObject::RotateZ(fRad); UpdateModel();}
//	virtual void SetRotateMatrix(const A3DMATRIX4& mat) { CSceneObject::SetRotateMatrix(mat); UpdateModel(); }
	virtual void Rotate(const A3DVECTOR3& axis,float fRad){ CSceneObject::Rotate(axis,fRad); UpdateModel();}
	virtual void Scale(float fScale){ CSceneObject::Scale(fScale); UpdateModel();}
//	virtual void SetScaleMatrix(const A3DMATRIX4& mat) { CSceneObject::SetScaleMatrix(mat); UpdateModel(); }
	virtual void SetPos(const A3DVECTOR3& vPos){ CSceneObject::SetPos(vPos); UpdateModel();}
	virtual void UpdateProperty(bool bGet);
	virtual void Release();
	virtual void ResetRotateMat(){ CSceneObject::ResetRotateMat(); UpdateModel();}
	virtual void SetDirAndUp(const A3DVECTOR3& vdir, const A3DVECTOR3& vu){ CSceneObject::SetDirAndUp(vdir,vu); UpdateModel();}
	virtual void SetScale(const A3DVECTOR3& vs){ CSceneObject::SetScale(vs); UpdateModel();}							
	
	//  Copy object
	virtual CSceneObject*		CopyObject();
	inline void SetA3dModel(PESTATICMODEL pModel)
	{ 
		m_pModel = pModel; 
	};
	PESTATICMODEL GetA3DModel()
	{
		return m_pModel;
	}

	//	Load data
	virtual bool Load(CELArchive& ar,DWORD dwVersion,int iLoadFlags);
	//	Save data
	virtual bool Save(CELArchive& ar,DWORD dwVersion);
	
	virtual int	GetTriangleNum();
	virtual int	GetMeshNum();

	bool IsNoShadow() { return m_bNoShadow; };
	void SetNoShadow(bool bNoShadow) { m_bNoShadow = bNoShadow; };

	bool IsShadow() { return !m_bNoShadow; };
	void SetShadow(bool bShadow) { m_bNoShadow = !bShadow; };

	//	Get reflect flag
	bool GetReflectFlag() { return m_bReflect; }
	//	Get refract flag
	bool GetRefractFlag() { return m_bRefraction; }
	
	bool RayTrace(const A3DVECTOR3& vStart,const A3DVECTOR3& vEnd,A3DVECTOR3& inter);
	bool BuildProperty();

	//判断区域是否跟该区域相交
	bool IsIntersectionWithArea(const A3DVECTOR3& vMin, const A3DVECTOR3& vMax);

	//查询自己模型引用是否已经加入到资源表中，如果已经加入就不再加入了，否则就写到资源表中
	void QueryRef(ElementResMan *pResMan);

	//把静态模型设置成只有碰撞没有显示的特殊模型
	void SetCollideFlags(DWORD bCollide);

	DWORD GetCollideFlags();

	bool IsTree(){ return m_bIsTree; }
	void SetTreeFlag( bool bIsTree){ m_bIsTree = bIsTree; }
	AString GetModelPath(){ return m_strModelPath; }
	void SetModelPath( AString path){ m_strModelPath = path; }
	bool IsSLAffect(){ return m_bSLAffect; }

	int GetAutoLightmapSize(){ return m_nAutoLightmapSize; }
    int GetCustomLightmapSize(){ return m_nCustomLightmapSize; }
	void GetLitScales(float* fOutDirect, float* fOutIndirect, float* fOutAO);
	void GetEmissiveData(LIGHTDATA* pOutData, AString* pstrMapFile, int* nSamples);
	int	GetFeetSound(){ return m_nFeetSound; }
    A3DAABB GetRealAABB() const;

	int GetConvexHullFaceNum() const;
    int GetFaceCount() const;
    int GetMeshCount() const;
	float GetLoadRadius() const { return m_fLoadRadius; }
    bool GetUseConsistentNormal() const { return m_bUseConsistentNormal; }
    bool GetIgnoreInnerShadow() const { return m_bIgnoreInnerShadow; }

private:
	// Property data
	PESTATICMODEL m_pModel;
	bool m_bNoShadow;
	bool m_bReflect;
	bool m_bRefraction;
	bool m_bOptimizeEnable;//是否可被优化
	//bool m_bCollideFlags;
	DWORD m_dwCollideFlags; // 0 - 显示、碰撞。1 - 只碰撞的虚模型。2 - 不带碰撞数据的模型
	bool m_bIsTree;//是否是树，因为可能在做某些渲染操作时可能不需要显示树
	AString m_strModelPath;//模型路径
	A3DVECTOR3 m_vMin;
	A3DVECTOR3 m_vMax;
	bool m_bSLAffect; //是否受场景里面的方向光影响，不是主光源
	// 光照图相关属性
	int		m_nAutoLightmapSize;
    int     m_nCustomLightmapSize;
	float	m_fLitDirectScale;
	float	m_fLitIndirectScale;
	float	m_fLitAOScale;
	float   m_fLoadRadius;

	// 自发光相关属性
	AString	m_strEmissiveMap; //自发光纹理路径
	float	m_fPower;
	float	m_fRange;
    float	m_fAttenuation0;
    float	m_fAttenuation1;
    float	m_fAttenuation2;
	int		m_nEffectType;
	int		m_nSampleCount;
	//地面音效
	int		m_nFeetSound;
    A3DOBB  m_Obb;
    bool    m_bUseConsistentNormal;
    bool    m_bIgnoreInnerShadow;
    friend struct LIGHTMAP_SIZE_FUNCS;
    LIGHTMAP_SIZE_FUNCS* m_pLightMapSizeFuns;
	EFFECT_PROPERTY_FUNCS* m_pEffectPropertyFuns;

    AString m_strDetailMap;

public:
	inline bool GetReflect()		{ return m_bReflect; }
	inline bool GetRefract()		{ return m_bRefraction; }
	inline bool GetShadow()			{ return m_bNoShadow; }
	inline bool GetOptimizeFlag()   { return m_bOptimizeEnable; }

	void SetReflect(bool bReflect){ m_bReflect = bReflect; }
	void SetReflact(bool bRefract){ m_bRefraction = bRefract; }
	void SetOptimizeFlag( bool bOptimize ){ m_bOptimizeEnable = bOptimize; }
	void UpdateModel();
    float ComputeModelArea() const;
    int GetAdaptLightmapSize(float fScale) const;
    float GetLightmapRate() const;
    A3DOBB GetOBB() const           { return m_Obb; }  
    float ComputeVolume() const;

private:	
	void ReBuiludAABBBox();
	void ReloadModel();
	// Only for render selected,don't save to file;
	DWORD m_dwLastTime;
	bool  m_bCircle;
	ASetTemplate<int> m_aLitmapSize;
	ASetTemplate<int> light_effect_type;
	ASetTemplate<int> m_aSoundSet;
	ASetTemplate<int> m_CollideType;

};

typedef CStaticModelObject* PSTATICMODELOBJECT;

#endif // !defined(AFX_STATICMODELOBJECT_H__59519995_09C2_479E_8E2E_D44AF89064B0__INCLUDED_)
