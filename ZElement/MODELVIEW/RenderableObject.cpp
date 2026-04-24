// RenderableObject.cpp: implementation of the CRenderableObject class.
//
//////////////////////////////////////////////////////////////////////

#include "Global.h"
#include "ModelView.h"
#include "a3d.h"
#include "RenderableObject.h"
#include "Render.h"


#define new A_DEBUG_NEW

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRenderableObject::CRenderableObject()
{
	m_vRotate = A3DVECTOR3(0.0f,0.0f,0.0f);
}

CRenderableObject::~CRenderableObject()
{

}

void CRenderableObject::Release()
{
	for( int i = 0; i < m_listSkin.size(); i++)
	{
		A3DSkinMan* pSkinMan = g_Render.GetA3DEngine()->GetA3DSkinMan();
		pSkinMan->ReleaseSkin(&m_listSkin[i]);
	}
	m_listSkin.clear();
}

A3DSkin* CRenderableObject::LoadA3DSkin(const char* szFile)
{
	A3DSkinMan* pSkinMan = g_Render.GetA3DEngine()->GetA3DSkinMan();
	A3DSkin* pSkin = pSkinMan->LoadSkinFile(szFile);

	bool bSkip = false;
	for( int i = 0; i < m_listSkin.size(); ++i)
	{
		if(m_listSkin[i] == pSkin) 
		{
			bSkip = true;
			break;
		}
	}
	if(!bSkip) m_listSkin.push_back(pSkin);
	
	return pSkin;
}
