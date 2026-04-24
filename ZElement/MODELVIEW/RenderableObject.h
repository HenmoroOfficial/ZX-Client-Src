// RenderableObject.h: interface for the CRenderableObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RENDERABLEOBJECT_H__0A8E6F2E_348E_4353_8573_7D502AEEFEF5__INCLUDED_)
#define AFX_RENDERABLEOBJECT_H__0A8E6F2E_348E_4353_8573_7D502AEEFEF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "vector.h"

class A3DViewport;
class A3DVECTOR3;
class A3DSkin;
class AString;



enum RENDERABLE_OBJECT_TYPE
{
	RENDERABLE_OBJECT_PLAYER = 0,
	RENDERABLE_OBJECT_NPC,
	RENDERABLE_OBJECT_NUM,
};

class CRenderableObject  
{
public:
	CRenderableObject();
	virtual ~CRenderableObject();

	virtual void Rotate(A3DVECTOR3 vRotate){ m_vRotate +=vRotate; }
	virtual A3DMATRIX4 GetAbsoluteTM() = 0;
	virtual void Render(A3DViewport *pA3dViewPort) = 0;
	virtual void Tick(DWORD dwTimeDelta) = 0;
	virtual void PlayAction(AString strAction , bool bLoop = false){};
	virtual void StopAction(){};
	virtual void Release();
	virtual A3DVECTOR3 GetDefaultCamaraPos() = 0;
	virtual void SaveConfig(){};
	virtual RENDERABLE_OBJECT_TYPE GetObjectType(){ return m_nObjectType; }
	virtual void GetPathList(abase::vector<AString>& listPath)=0;
	virtual void GetRotateAndPos(A3DVECTOR3& r, A3DVECTOR3& pos)=0;
	virtual bool PlayAttackAction(const char* szActName, int idSkill) = 0;
	virtual int GetWeaponID(){ return -1; }
public:

	A3DSkin* CRenderableObject::LoadA3DSkin(const char* szFile);


protected:
	A3DVECTOR3 m_vRotate;
	RENDERABLE_OBJECT_TYPE m_nObjectType;
	abase::vector<A3DSkin *> m_listSkin;
};

#endif // !defined(AFX_RENDERABLEOBJECT_H__0A8E6F2E_348E_4353_8573_7D502AEEFEF5__INCLUDED_)
