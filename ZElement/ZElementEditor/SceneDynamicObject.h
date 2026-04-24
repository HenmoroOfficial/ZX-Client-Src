#ifndef __SCENE_DYNAMIC_OBJECT__
#define __SCENE_DYNAMIC_OBJECT__


#include "SceneObject.h"
class A3DGFXEx;
class CECModel;

struct DYNAMIC_SEL_FUNCS : public CUSTOM_FUNCS
{
	virtual BOOL CALLBACK OnActivate(void);
	virtual LPCTSTR CALLBACK OnGetShowString(void) const;
	virtual AVariant CALLBACK OnGetValue(void) const;
	virtual void CALLBACK OnSetValue(const AVariant& var);

	AVariant	m_var;
	CString		m_strText;
};

struct CONTROLLER_DYNAMIC_ID_FUNCS : public CUSTOM_FUNCS
{
	virtual BOOL CALLBACK OnActivate(void);
	virtual LPCTSTR CALLBACK OnGetShowString(void) const;
	virtual AVariant CALLBACK OnGetValue(void) const;
	virtual void CALLBACK OnSetValue(const AVariant& var);

	AVariant	m_var;
	CString		m_strText;
};

class CSceneDynamicObject : public CSceneObject
{
public:
	CSceneDynamicObject();
	virtual ~CSceneDynamicObject();
	
	virtual void ReInit() /*override*/; // 在拷贝构造或者赋值后调用，将指针字段初始化为NULL或者新对象。赋值前还应调用Release
	virtual void Render(A3DViewport* pA3DViewport);
	virtual void PostRender(A3DViewport* pA3DViewport);
	virtual void Tick(DWORD timeDelta);
	virtual void UpdateProperty(bool bGet);
	virtual void Release();
	void         SetDynamicObjectID( unsigned int id){ m_uDynamicObjectID = id; }
	unsigned int GetDynamicObjectID(){ return m_uDynamicObjectID; }
	void         SetControllerID( unsigned int id){ m_uControllerID = id; }
	unsigned int GetControllerID(){ return m_uControllerID;}
	void		 SetPhase(int iPhase) { m_iPhase = iPhase; }
	int			 GetPhase() { return m_iPhase; }
	//  Copy object
	virtual CSceneObject*		CopyObject();
	//Load data
	virtual bool Load(CELArchive& ar,DWORD dwVersion,int iLoadFlags);
	//Save data
	virtual bool Save(CELArchive& ar,DWORD dwVersion);


	virtual void				Translate(A3DVECTOR3 vDelta);
	virtual void				RotateX(float fRad);
	virtual void				RotateY(float fRad);
	virtual void				RotateZ(float fRad);
	virtual void				Rotate(A3DVECTOR3 axis,float fRad);
	virtual void				Scale(float fScale);
	virtual void				SetPos(A3DVECTOR3 vPos);
	virtual void                ResetRotateMat();
	virtual void SetDirAndUp(const A3DVECTOR3& vDir, const A3DVECTOR3& vUp);
	virtual void SetPropertyPtr(DYNAMIC_SEL_FUNCS *pPtr){ m_pIDSelFuncs = pPtr; }
	void SetAllPrtToNull()
	{
		m_pGfxEx = NULL;
		m_pECModel = NULL;
	}
	void ReloadModels();
	int  GetCopyFlags(){ return m_nCopy; }
	void SetCopyFlags( int nCopy){ m_nCopy = nCopy; }
private:
	void BuildProperty();
	unsigned int m_uDynamicObjectID;
	unsigned int m_uControllerID;
	int			 m_iPhase;
	DYNAMIC_SEL_FUNCS* m_pIDSelFuncs;

	A3DGFXEx*      m_pGfxEx;
	CECModel*      m_pECModel;

	CONTROLLER_DYNAMIC_ID_FUNCS* m_pControllerSelFuncs;
	int     m_nCopy;//数据套数，决定是第几套数据(0,1)
};

#endif