// Filename	: DlgTab3D.cpp
// Creator	: Fu Chonggang
// Date		: 2012-6-12

#include "DlgTab3D.h"
#include "AUI\\AUIStillImageButton.h"
#include "EC_GameUIMan.h"
#include "EC_Game.h"

#include "A3DTexture.h"
#include "A3DStream.h"
#include "A3DDevice.h"
#include "A3DVector.h"

#define new A_DEBUG_NEW


TabInfo operator + (const TabInfo& v1, const TabInfo& v2)
{ 
	TabInfo info;
	info.alpha = v1.alpha;
	int i,j;
	for (i=0; i<4; ++i)
	{
		for (j=0; j<2; ++j)
		{
			info.pos[i][j] = v1.pos[i][j]+v2.pos[i][j];
		}
	}
	return info; 
}

TabInfo operator * (const TabInfo& v, float f) 
{ 
	TabInfo info;
	info.alpha = v.alpha;
	int i,j;
	for (i=0; i<4; ++i)
	{
		for (j=0; j<2; ++j)
		{
			info.pos[i][j] = v.pos[i][j]*f;
		}
	}
	return info; 
}

AUI_BEGIN_COMMAND_MAP(CDlgTab3D, CDlgBase)

AUI_ON_COMMAND("Btn_Left",		OnCommand_Prev)
AUI_ON_COMMAND("Btn_Right",		OnCommand_Next)

AUI_END_COMMAND_MAP()

int		CDlgTab3D::ms_iSpeed = 500;

CDlgTab3D::CDlgTab3D() : m_fDeltaX(0), m_fDeltaY(0)
{
}

CDlgTab3D::~CDlgTab3D()
{
}

bool CDlgTab3D::OnInitDialog()
{
	A3DRECT rcOld(0, 0, 1024, 768), rcNew(0, 0, 1024, 768);
	Resize(rcOld, rcNew);

	ReleaseSprites();
	AddTextrures();
	m_pTab3DStream = new A3DStream();
	if( !m_pTab3DStream->Init(m_pA3DDevice, A3DVT_TLVERTEX, 6, 0, A3DSTRM_REFERENCEPTR, 0) )
	{
		return AUI_ReportError(__LINE__, __FILE__);
	}

	m_iCurPos = 0;
	m_iStage = ms_iSpeed;

	int i;
	AString strName;
	for (i=0; i<6; ++i)
	{
		strName.Format("Img_%d", i+1);
		if (GetDlgItem(strName))
		{
			GetDlgItem(strName)->Show(false);
		}
	}

	m_pBtnLeft = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Left");
	m_pBtnRight = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Right");
	return true;
}

void CDlgTab3D::AddTextrures()
{
	AddTextrure("Surfaces\\Icon\\dungeon_tower_1.tga");
	AddTextrure("Surfaces\\Icon\\dungeon_tower_2.tga");
	AddTextrure("Surfaces\\Icon\\dungeon_tower_3.tga");
	AddTextrure("Surfaces\\Icon\\dungeon_tower_4.tga");
	AddTextrure("Surfaces\\Icon\\dungeon_tower_5.tga");
}

bool CDlgTab3D::Release()
{
	if( m_pTab3DStream )
	{
		m_pTab3DStream->Release();
		delete m_pTab3DStream;
		m_pTab3DStream = NULL;
	}
	ReleaseSprites();
	return CDlgBase::Release();
}

void CDlgTab3D::OnTick()
{
	int iOldStage = m_iStage;
	m_iStage += GetGame()->GetTickTime();
	if (iOldStage<ms_iSpeed && m_iStage>=ms_iSpeed)
	{
		if (m_pBtnLeft->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK)
		{
			OnCommand_Prev("");
		}
		else if (m_pBtnRight->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK)
		{
			OnCommand_Next("");
		}
	}
}

bool CDlgTab3D::Render(void)
{
	bool bRet = CDlgBase::Render();
	RenderSprites();
	return bRet;
}

void CDlgTab3D::Resize(A3DRECT rcOld, A3DRECT rcNew)
{
	AUIDialog::Resize(rcOld, rcNew);
	float fWindowScale = m_pAUIManager->GetWindowScale() * m_fScale;
	ms_TabInfo[0] = TabInfo(0x0,	(-200+m_fDeltaX)*fWindowScale,	(-80+m_fDeltaY)*fWindowScale,	(-150+m_fDeltaX)*fWindowScale,(-120+m_fDeltaY)*fWindowScale,
		(-200+m_fDeltaX)*fWindowScale,(120+m_fDeltaY)*fWindowScale,	(-150+m_fDeltaX)*fWindowScale,(80+m_fDeltaY)*fWindowScale);


	ms_TabInfo[1] = TabInfo(0xFF,	(-150+m_fDeltaX)*fWindowScale,	(-80+m_fDeltaY)*fWindowScale,	(-100+m_fDeltaX)*fWindowScale,(-120+m_fDeltaY)*fWindowScale,
		(-150+m_fDeltaX)*fWindowScale,(120+m_fDeltaY)*fWindowScale,	(-100+m_fDeltaX)*fWindowScale,(80+m_fDeltaY)*fWindowScale);
	ms_TabInfo[2] = TabInfo(0xFF,	(-100+m_fDeltaX)*fWindowScale,	(-80+m_fDeltaY)*fWindowScale,	(-50+m_fDeltaX)*fWindowScale,(-120+m_fDeltaY)*fWindowScale,
		(-100+m_fDeltaX)*fWindowScale,(120+m_fDeltaY)*fWindowScale,	(-50+m_fDeltaX)*fWindowScale,(80+m_fDeltaY)*fWindowScale);
	ms_TabInfo[3] = TabInfo(0xFF,	(-80+m_fDeltaX)*fWindowScale,	(-100+m_fDeltaY)*fWindowScale,	(80+m_fDeltaX)*fWindowScale,(-100+m_fDeltaY)*fWindowScale,	
		(-80+m_fDeltaX)*fWindowScale,(100+m_fDeltaY)*fWindowScale,	(80+m_fDeltaX)*fWindowScale,(100+m_fDeltaY)*fWindowScale);
	ms_TabInfo[4] = TabInfo(0xFF,	(50+m_fDeltaX)*fWindowScale,	(-120+m_fDeltaY)*fWindowScale,	(100+m_fDeltaX)*fWindowScale,(-80+m_fDeltaY)*fWindowScale,	
		(50+m_fDeltaX)*fWindowScale,(80+m_fDeltaY)*fWindowScale,	(100+m_fDeltaX)*fWindowScale,(120+m_fDeltaY)*fWindowScale);
	ms_TabInfo[5] = TabInfo(0xFF,	(100+m_fDeltaX)*fWindowScale,	(-120+m_fDeltaY)*fWindowScale,	(150+m_fDeltaX)*fWindowScale,(-80+m_fDeltaY)*fWindowScale,	
		(100+m_fDeltaX)*fWindowScale,(80+m_fDeltaY)*fWindowScale,	(150+m_fDeltaX)*fWindowScale,(120+m_fDeltaY)*fWindowScale);

	ms_TabInfo[6] = TabInfo(0x0,	(150+m_fDeltaX)*fWindowScale,	(-120+m_fDeltaY)*fWindowScale,	(200+m_fDeltaX)*fWindowScale,(-80+m_fDeltaY)*fWindowScale,	
		(150+m_fDeltaX)*fWindowScale,(80+m_fDeltaY)*fWindowScale,	(200+m_fDeltaX)*fWindowScale,(120+m_fDeltaY)*fWindowScale);
}

void CDlgTab3D::RenderSprites()
{
	if (m_vecTextures.size()==0)
	{
		return;
	}

	if (m_iStage>=ms_iSpeed)
	{
		if(m_iCurPos>1)
			RenderSprite(ms_TabInfo[1], m_iCurPos-2, GetPos().x+GetSize().cx/2, GetPos().y+GetSize().cy/2);
		if(m_iCurPos>0)
			RenderSprite(ms_TabInfo[2], m_iCurPos-1, GetPos().x+GetSize().cx/2, GetPos().y+GetSize().cy/2);
		if(m_iCurPos<int(m_vecTextures.size()-1))
			RenderSprite(ms_TabInfo[4], m_iCurPos+1, GetPos().x+GetSize().cx/2, GetPos().y+GetSize().cy/2);
		if(m_iCurPos<int(m_vecTextures.size()-2))
			RenderSprite(ms_TabInfo[5], m_iCurPos+2, GetPos().x+GetSize().cx/2, GetPos().y+GetSize().cy/2);
		RenderSprite(ms_TabInfo[3], m_iCurPos, GetPos().x+GetSize().cx/2, GetPos().y+GetSize().cy/2);
		return;
	}
	else if (m_bNext)
	{
		TabInfo info;
/*		if(m_iCurPos<int(m_vecTextures.size()-1))
		{
			info = ms_TabInfo[3]*((float)m_iStage/ms_iSpeed)+ms_TabInfo[4]*((float)(ms_iSpeed-m_iStage)/ms_iSpeed);
			info.alpha = ms_TabInfo[3].alpha*m_iStage/ms_iSpeed+ms_TabInfo[4].alpha*(ms_iSpeed-m_iStage)/ms_iSpeed;
			RenderSprite(info, m_iCurPos+1, GetPos().x+GetSize().cx/2, GetPos().y+GetSize().cy/2);
		}
		
		if(m_iCurPos-2>=0)
		{
			info = ms_TabInfo[0]*((float)m_iStage/ms_iSpeed)+ms_TabInfo[1]*((float)(ms_iSpeed-m_iStage)/ms_iSpeed);
			info.alpha = ms_TabInfo[0].alpha*m_iStage/ms_iSpeed+ms_TabInfo[1].alpha*(ms_iSpeed-m_iStage)/ms_iSpeed;
			RenderSprite(info, m_iCurPos-2, GetPos().x+GetSize().cx/2, GetPos().y+GetSize().cy/2);
		}
		
		if(m_iCurPos-1>=0)
		{
			info = ms_TabInfo[1]*((float)m_iStage/ms_iSpeed)+ms_TabInfo[2]*((float)(ms_iSpeed-m_iStage)/ms_iSpeed);
			RenderSprite(info, m_iCurPos-1, GetPos().x+GetSize().cx/2, GetPos().y+GetSize().cy/2);
		}
*/
		int i;
		for (i=0; i<6; ++i)
		{
			if (i!=3 && (m_iCurPos+(i-3))>=0 && (m_iCurPos+(i-3))<int(m_vecTextures.size()))
			{
				info = ms_TabInfo[i]*((float)m_iStage/ms_iSpeed)+ms_TabInfo[i+1]*((float)(ms_iSpeed-m_iStage)/ms_iSpeed);
				info.alpha = ms_TabInfo[i].alpha*m_iStage/ms_iSpeed+ms_TabInfo[i+1].alpha*(ms_iSpeed-m_iStage)/ms_iSpeed;
				RenderSprite(info, m_iCurPos+(i-3), GetPos().x+GetSize().cx/2, GetPos().y+GetSize().cy/2);
			}
		}

		info = ms_TabInfo[3]*((float)m_iStage/ms_iSpeed)+ms_TabInfo[4]*((float)(ms_iSpeed-m_iStage)/ms_iSpeed);
		RenderSprite(info, m_iCurPos, GetPos().x+GetSize().cx/2, GetPos().y+GetSize().cy/2);
	}
	else
	{
		TabInfo info;
/*		if(m_iCurPos>0)
		{
			info = ms_TabInfo[1]*((float)m_iStage/ms_iSpeed)+ms_TabInfo[0]*((float)(ms_iSpeed-m_iStage)/ms_iSpeed);
			info.alpha = ms_TabInfo[1].alpha*m_iStage/ms_iSpeed+ms_TabInfo[0].alpha*(ms_iSpeed-m_iStage)/ms_iSpeed;
			RenderSprite(info, m_iCurPos-1, GetPos().x+GetSize().cx/2, GetPos().y+GetSize().cy/2);
		}
		
		if (m_iCurPos+2<=int(m_vecTextures.size()-1))
		{
			info = ms_TabInfo[4]*((float)m_iStage/ms_iSpeed)+ms_TabInfo[3]*((float)(ms_iSpeed-m_iStage)/ms_iSpeed);
			info.alpha = ms_TabInfo[4].alpha*m_iStage/ms_iSpeed+ms_TabInfo[3].alpha*(ms_iSpeed-m_iStage)/ms_iSpeed;
			RenderSprite(info, m_iCurPos+2, GetPos().x+GetSize().cx/2, GetPos().y+GetSize().cy/2);
		}
		if (m_iCurPos+1<=int(m_vecTextures.size()-1))
		{
			info = ms_TabInfo[3]*((float)m_iStage/ms_iSpeed)+ms_TabInfo[2]*((float)(ms_iSpeed-m_iStage)/ms_iSpeed);
			RenderSprite(info, m_iCurPos+1, GetPos().x+GetSize().cx/2, GetPos().y+GetSize().cy/2);
		}
*/

		int i;
		for (i=1; i<=6; ++i)
		{
			if (i!=3 && (m_iCurPos+(i-3))>=0 && (m_iCurPos+(i-3))<int(m_vecTextures.size()))
			{
				info = ms_TabInfo[i]*((float)m_iStage/ms_iSpeed)+ms_TabInfo[i-1]*((float)(ms_iSpeed-m_iStage)/ms_iSpeed);
				info.alpha = ms_TabInfo[i].alpha*m_iStage/ms_iSpeed+ms_TabInfo[i-1].alpha*(ms_iSpeed-m_iStage)/ms_iSpeed;
				RenderSprite(info, m_iCurPos+(i-3), GetPos().x+GetSize().cx/2, GetPos().y+GetSize().cy/2);
			}
		}
		info = ms_TabInfo[3]*((float)m_iStage/ms_iSpeed)+ms_TabInfo[2]*((float)(ms_iSpeed-m_iStage)/ms_iSpeed);
		RenderSprite(info, m_iCurPos, GetPos().x+GetSize().cx/2, GetPos().y+GetSize().cy/2);
	}
}

void CDlgTab3D::RenderSprite(TabInfo info, int index, int deltaX, int deltaY)
{
	m_pA3DDevice->SetLightingEnable(false);

	m_vecTextures[index]->Appear(0);
	
	A3DTLVERTEX * pVerts = NULL;
	if( !m_pTab3DStream->LockVertexBuffer(0, 0, (BYTE**) &pVerts, 0) )
		return;
	
	pVerts[0] = A3DTLVERTEX(A3DVECTOR4(info.pos[0][0]+deltaX, info.pos[0][1]+deltaY, 0.0f, 1.0f), (info.alpha<<24)+0xFFFFFF, 0xff000000, 0.0f, 0.0f);
	pVerts[1] = A3DTLVERTEX(A3DVECTOR4(info.pos[1][0]+deltaX, info.pos[1][1]+deltaY, 0.0f, 1.0f), (info.alpha<<24)+0xFFFFFF, 0xff000000, 1.0f, 0.0f);
	pVerts[2] = A3DTLVERTEX(A3DVECTOR4(info.pos[2][0]+deltaX, info.pos[2][1]+deltaY, 0.0f, 1.0f), (info.alpha<<24)+0xFFFFFF, 0xff000000, 0.0f, 1.0f);
	
	pVerts[3] = A3DTLVERTEX(A3DVECTOR4(info.pos[2][0]+deltaX, info.pos[2][1]+deltaY, 0.0f, 1.0f), (info.alpha<<24)+0xFFFFFF, 0xff000000, 0.0f, 1.0f);
	pVerts[4] = A3DTLVERTEX(A3DVECTOR4(info.pos[1][0]+deltaX, info.pos[1][1]+deltaY, 0.0f, 1.0f), (info.alpha<<24)+0xFFFFFF, 0xff000000, 1.0f, 0.0f);
	pVerts[5] = A3DTLVERTEX(A3DVECTOR4(info.pos[3][0]+deltaX, info.pos[3][1]+deltaY, 0.0f, 1.0f), (info.alpha<<24)+0xFFFFFF, 0xff000000, 1.0f, 1.0f);
	
	m_pTab3DStream->UnlockVertexBuffer();
	m_pTab3DStream->Appear();
	bool bval = m_pA3DDevice->DrawPrimitive(A3DPT_TRIANGLELIST, 0, 2);
	
	m_vecTextures[index]->Disappear(0);
	
	m_pA3DDevice->SetLightingEnable(true);
}

void CDlgTab3D::OnCommand_Prev(const char * szCommand)
{
	if (m_iCurPos==0 || m_iStage<ms_iSpeed)
	{
		return;
	}

	m_iCurPos--;
	m_iStage = 0;
	m_bNext = false;
	OnSelOneItem();
}

void CDlgTab3D::OnCommand_Next(const char * szCommand)
{
	if (m_iCurPos>=int(m_vecTextures.size()-1) || m_iStage<ms_iSpeed)
	{
		return;
	}
	
	m_iCurPos++;
	m_iStage = 0;
	m_bNext = true;
	OnSelOneItem();
}

bool CDlgTab3D::AddTextrure(AString strPath)
{
	A3DTexture* pTexture = new A3DTexture;
	if (!pTexture)
	{
		return AUI_ReportError(__LINE__, __FILE__);
	}

	pTexture->SetNoDownSample(true);
	if (!pTexture->LoadFromFile(m_pA3DDevice, strPath, strPath))
	{
		delete pTexture;
		return AUI_ReportError(__LINE__, __FILE__);
	}
	m_vecTextures.push_back(pTexture);
	return true;
}

void CDlgTab3D::ReleaseSprites()
{
	int i;
	for (i=0; i<(int)m_vecTextures.size(); ++i)
	{
		A3DRELEASE(m_vecTextures[i]);
	}
	m_vecTextures.clear();
}