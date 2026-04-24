// Filename	: DlgBaseFadeOut.cpp
// Creator	: Fu Chonggang
// Date		: 2012.5.4


#include "DlgBaseFadeOut.h"
#include "A3DTexture.h"
#include "A3DVertex.h"
#include "A3DDevice.h"
#include "EC_Game.h"

// disable warning "extern on member function"
#pragma warning ( disable : 4630 )

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgBaseFadeOut, CDlgBase)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_Fade)
AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgBaseFadeOut, CDlgBase)
AUI_END_EVENT_MAP()

const int CDlgBaseFadeOut::ms_iFadeTime = 300;

CDlgBaseFadeOut::CDlgBaseFadeOut()
{
	m_iStage = FADE_NOTHING;
	m_pA3DTexture = NULL;
}

CDlgBaseFadeOut::~CDlgBaseFadeOut()
{
	A3DRELEASE(m_pA3DTexture);
}

void CDlgBaseFadeOut::Show(bool bShow, bool bModal, bool bActive)
{
	if (!m_pA3DTexture)
		m_pA3DTexture = new A3DTexture();

	if (bShow)
	{
		CDlgBase::Show(bShow, bModal, bActive);
		m_iScale = 10;
		RenderRTToTexture(m_pA3DTexture);
		m_iStage = FADE_IN;
	}
	else
	{
		if ( m_iStage==FADE_OUT )
		{
			CDlgBase::Show(bShow, bModal, bActive);
			m_iStage = FADE_NOTHING;
		}
		else
			m_iStage = FADE_OUT;
	}
}

bool CDlgBaseFadeOut::Tick(void)
{
	bool bRet =	CDlgBase::Tick();
	if ( m_iStage==FADE_IN )
	{
		if (m_iScale<ms_iFadeTime)
		{
			m_iScale += GetGame()->GetTickTime();
		}
		if ( m_iScale>=ms_iFadeTime)
		{
			m_iScale = ms_iFadeTime;
			m_iStage = FADE_NOTHING;
		}
	}
	else if ( m_iStage==FADE_OUT )
	{
		if (m_iScale>0)
		{
			m_iScale -= GetGame()->GetTickTime();
		}
		if (m_iScale<=0)
		{
			m_iScale = 10;
			Show(false);
		}
	}

	return bRet;
}

bool CDlgBaseFadeOut::Render(void)
{
	if (m_iStage!=FADE_NOTHING)
		return RenderAnimation();
	
	return AUIDialog::Render();
}

void CDlgBaseFadeOut::OnCommand_Fade(const char * szCommand)
{
	m_iStage = FADE_OUT;
}

bool CDlgBaseFadeOut::RenderAnimation()
{
	int i;
	A3DTLVERTEX vert[4];
	A3DRECT rc = GetRect();
	float fOffset_x = rc.Width() * m_iScale / 300 *0.5f;
	float fOffset_y = rc.Height() * m_iScale / 300 *0.5f;
	int left = (int)(rc.CenterPoint().x - fOffset_x);
	int right = (int)(rc.CenterPoint().x + fOffset_x);
	int top = (int)(rc.CenterPoint().y - fOffset_y);
	int bottom = (int)(rc.CenterPoint().y + fOffset_y);
	static WORD a_wIndex[6] = { 0, 1, 2, 2, 1, 3 };
	
	for( i = 0; i < 4; i++ )
	{
		vert[i].z = 0.0f;
		vert[i].rhw = 1.0f;
		vert[i].diffuse = 0xFFFFFFFF;
		vert[i].specular = 0xFF000000;
		vert[i].tu = float(i % 2);
		vert[i].tv = float(i / 2);
	}
	
	rc.SetRect(left, top, right, bottom);
	vert[0].x = float(rc.left);
	vert[0].y = float(rc.top);
	vert[1].x = float(rc.right);
	vert[1].y = float(rc.top);
	vert[2].x = float(rc.left);
	vert[2].y = float(rc.bottom);
	vert[3].x = float(rc.right);
	vert[3].y = float(rc.bottom);
	
	m_pA3DDevice->SetLightingEnable(false);
	//m_pA3DDevice->SetDeviceVertexShader(A3DFVF_A3DTLVERTEX);
	m_pA3DDevice->ClearPixelShader();
	m_pA3DDevice->ClearTexture(0);
	m_pA3DTexture->Appear(0);
	m_pA3DDevice->SetTextureAddress(0, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	
	if (true)
	{
		m_pA3DDevice->SetTextureFilterType(0, A3DTEXF_POINT);
		m_pA3DDevice->SetSourceAlpha(A3DBLEND_ONE);
		m_pA3DDevice->SetDestAlpha(A3DBLEND_SRCALPHA);
	}
	
	bool bval = m_pA3DDevice->DrawIndexedPrimitiveUP(A3DPT_TRIANGLELIST, 0,
		4, 2, a_wIndex, A3DFMT_INDEX16, vert, sizeof(A3DTLVERTEX));
	if( !bval ) 
		return AUI_ReportError(__LINE__, __FILE__);
	
	if (true)
	{					
		m_pA3DDevice->SetTextureFilterType(0, A3DTEXF_LINEAR);
		m_pA3DDevice->SetSourceAlpha(A3DBLEND_SRCALPHA);
		m_pA3DDevice->SetDestAlpha(A3DBLEND_INVSRCALPHA);
	}
	
	m_pA3DDevice->SetTextureAddress(0, A3DTADDR_WRAP, A3DTADDR_WRAP);
	m_pA3DTexture->Disappear(0);
	
	return true;
}