#pragma once
#include "sceneobject.h"

#include "AObject.h"

struct TMPL_ID_FUNCS : public CUSTOM_FUNCS
{
	virtual BOOL CALLBACK OnActivate(void);
	virtual LPCTSTR CALLBACK OnGetShowString(void) const;
	virtual AVariant CALLBACK OnGetValue(void) const;
	virtual void CALLBACK OnSetValue(const AVariant& var);

	AVariant	m_var;
	CString		m_strText;
};

class CECModel;

struct EC_GUILDBUILDING_PROPERTY
{
	int iTmplId;
	int iWidth;
	int iHeight;
};

class CSceneGuildBuilding :	public CSceneObject
{
public:
	CSceneGuildBuilding(void);
	virtual ~CSceneGuildBuilding(void);
	virtual void				Release();


	virtual A3DVECTOR3			GetMin();
	virtual A3DVECTOR3			GetMax();

	virtual void				Render(A3DViewport* pA3DViewport);
	virtual void				PostRender(A3DViewport* pA3DViewport);
	virtual void                RenderForLight(A3DViewport* pA3DViewport, float fOffsetX, float fOffsetZ);
	virtual void				RenderForLightMapShadow(A3DViewport* pA3DViewport);    

	virtual void				Tick(DWORD timeDelta);

	virtual void				Translate(const A3DVECTOR3& vDelta);
	virtual void				RotateX(float fRad);
	virtual void				RotateY(float fRad);
	virtual void				RotateZ(float fRad);
	virtual void				Rotate(const A3DVECTOR3& axis,float fRad);
	virtual void				Scale(float fScale);
	virtual void				SetPos(const A3DVECTOR3& vPos);
	virtual void				UpdateProperty(bool bGet);
	virtual void                ResetRotateMat();
	virtual void SetDirAndUp(const A3DVECTOR3& vDir, const A3DVECTOR3& vUp);

	//	Load data
	virtual bool Load(CELArchive& ar,DWORD dwVersion,int iLoadFlags);
	//	Save data
	virtual bool Save(CELArchive& ar,DWORD dwVersion);

	virtual int	GetTriangleNum();

	bool						LoadModel(AString strPathName);
	void                        BuildPropery();

	EC_GUILDBUILDING_PROPERTY&      GetProperty(){ return m_Property; };
	void                        SetProperty(EC_GUILDBUILDING_PROPERTY prop){ m_Property = prop; };

	//	Get EC model object
	inline CECModel* GetECModel() { return m_pECModel; }
	void ReLoadModel();

	int  GetCopyFlags(){ return m_nCopy; }
	void SetCopyFlags( int nCopy){ m_nCopy = nCopy; }

private:
	CECModel *m_pECModel;
	EC_GUILDBUILDING_PROPERTY m_Property;
	TMPL_ID_FUNCS* m_pIDSelFuncs;
	int m_nCopy;//数据套数，决定是第几套数据(0,1)
};
