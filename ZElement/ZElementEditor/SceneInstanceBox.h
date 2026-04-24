// SceneInstanceBox.h: interface for the SceneInstanceBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCENEINSTANCEBOX_H__E2652C22_A8ED_450D_B360_F62871AC1236__INCLUDED_)
#define AFX_SCENEINSTANCEBOX_H__E2652C22_A8ED_450D_B360_F62871AC1236__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SceneObject.h"

struct CONTROLLER_INSTANCE_ID_FUNCS : public CUSTOM_FUNCS
{
	virtual BOOL CALLBACK OnActivate(void);
	virtual LPCTSTR CALLBACK OnGetShowString(void) const;
	virtual AVariant CALLBACK OnGetValue(void) const;
	virtual void CALLBACK OnSetValue(const AVariant& var);
	
	AVariant	m_var;
	CString		m_strText;
};

struct INSTANCE_DATA
{
	int nSelfWorldId;//自己所属的世界ID
	int nWorldId;    //目标世界ID
	int nLimitLevel; //等级限制
	A3DVECTOR3 vTargetPos; //目标世界位置
	float fEdgeHei;
	float fEdgeLen;
	float fEdgeWth;
	int  nLine; //0-15 目标线（080326）

	int   nControllerId;//控制器ID
};

void init_instance_data( INSTANCE_DATA &dat);

class SceneInstanceBox : public CSceneObject
{
public:
	SceneInstanceBox();
	virtual ~SceneInstanceBox();

	virtual void Render(A3DViewport* pA3DViewport);
	virtual void PostRender(A3DViewport* pA3DViewport);
	virtual void Tick(DWORD timeDelta){};
	virtual void UpdateProperty(bool bGet);
	virtual void Release();
	virtual void RotateX(float fRad){};
	virtual void RotateY(float fRad){};
	virtual void RotateZ(float fRad){};
	virtual void Rotate(const A3DVECTOR3& axis,float fRad){};
	//  Copy object
	virtual CSceneObject*		CopyObject();
	//	Load data
	virtual bool Load(CELArchive& ar,DWORD dwVersion,int iLoadFlags);
	//	Save data
	virtual bool Save(CELArchive& ar,DWORD dwVersion);

	INSTANCE_DATA &GetProperty(){ return m_Data; };
	void SetProperty( const INSTANCE_DATA& data){ m_Data = data; };
	void GetAABB(A3DVECTOR3 &vMin, A3DVECTOR3 &vMax);
	//判断区域是否跟该区域相交
	bool IsIntersectionWithArea(const A3DVECTOR3& vMin, const A3DVECTOR3& vMax);
	int  GetCopyFlags(){ return m_nCopy; }
	void SetCopyFlags( int nCopy){ m_nCopy = nCopy; }
protected:
	void BuildProperty();

private:
	INSTANCE_DATA m_Data;
	int     m_nCopy;//数据套数，决定是第几套数据(0,1)

	CONTROLLER_INSTANCE_ID_FUNCS *m_pControllerSelFuncs;
};

#endif // !defined(AFX_SCENEINSTANCEBOX_H__E2652C22_A8ED_450D_B360_F62871AC1236__INCLUDED_)
