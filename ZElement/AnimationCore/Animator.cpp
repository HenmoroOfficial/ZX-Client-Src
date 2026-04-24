#include "stdafx.h"
#include "Animator.h"
#include "EC_World.h"
#include "EC_GameRun.h"
#include "AnmCamera.h"
#include "AnmPlayer.h"
#include "AnmObjectManager.h"
#include "AnmBezierManager.h"
#include "AnmActionManager.h"
#include "AnmTranslator.h"
#include "AnmRayTrace.h"
#include "AnmAxes.h"
#include "AnmInstance.h"
#include "SmartCamera.h"
#include "AnmCommonActionManager.h"
#ifdef BACK_VERSION
#include "EC_SunMoon.h"
#include "EC_Viewport.h"
#else
#include <A3DSunMoon.h>
#include <ECViewport.h>
#endif

CAnimator g_Animator;

CAnimator::CAnimator()
{
    m_pFont = 0;
    m_pCamera = 0;
    m_pPlayer = 0;
    m_pObjectManager = 0;
    m_pBezierManager = 0;
    m_pActionManager = 0;
    m_pTranslator = new CAnmTranslator(this);
    m_pRayTrace = new CAnmRayTrace(this);
    m_pAxes = 0;
	m_pSmartCamera			= 0;
    m_eAnimationType = TYPE_EDITOR_ANIMATION;
    m_iWorldInstance = 1;
    m_bUseHostPlayer = false;
    m_bShowHostPlayerWeapon = true;
    m_bSpecifyTime = false;
    m_iHour = 0;
    m_iMinute = 0;
    m_iSecond = 0;
    m_bSpecifyWeather = false;
    m_bRain = false;
    m_bSnow = false;
	m_bShowLetterbox = false;
    m_dwVolume = 100;
	m_bBlendFrame = false;
	m_bPreparePreFrame = false;
    m_uiMaskColor = 0x00000000;
    m_uiMonoColor = 0xFFFFFFFF;
    m_uiBackgroundColor = 0xFF000000;
    m_eBlackWhiteType = BWT_NONE;
    m_uiHideWorldType = 0;
    m_fMotionBlurLevel = 0.0f;
    m_dwTotalTime = 8000;
    m_dwStartTime = 0;
    m_dwEndTime = 8000;
    m_dwNowTime = 0;
    m_fSpeed = 1.0f;
    m_bAnimatorReady = false;
    m_bDisableCamera = false;
    m_bBindCamera = false;
    m_vBindCameraOffset = A3DVECTOR3(0.0f);
    m_eState = STATE_WAITING;
    m_bMoveCamera = false;
    m_fMoveCameraSpeed = 1.0f;
    m_iMoveLastTime = 0;

    float f = -0.03f;
    m_aFrustum[0] = A3DVECTOR3(-f, -f, f);
    m_aFrustum[1] = A3DVECTOR3(f, -f, f);
    m_aFrustum[2] = A3DVECTOR3(f, f, f);
    m_aFrustum[3] = A3DVECTOR3(-f, f, f);
    f = -0.05f;
    m_aFrustum[4] = A3DVECTOR3(-f, -f, 2*f);
    m_aFrustum[5] = A3DVECTOR3(f, -f, 2*f);
    m_aFrustum[6] = A3DVECTOR3(f, f, 2*f);
    m_aFrustum[7] = A3DVECTOR3(-f, f, 2*f);

	m_pCommonActionManager = NULL;
	m_pImageLetterboxTop = NULL;
	m_pImageLetterboxBottom = NULL;
	m_pImagePreFrame = NULL;
}

CAnimator::AnimationType CAnimator::GetAnimationType() const
{
    return m_eAnimationType;
}

bool CAnimator::Init(CAnmInstance* pInst)
{
	ASSERT( pInst );
	m_pAnmInst = pInst;

    // Create Font
    A3DFontMan *pFontMan = GetA3DEngine()->GetA3DFontMan();
    wchar_t chSample('W');
    HA3DFONT font = pFontMan->RegisterFontType(L"Á¥Êé", 24, 0, chSample);
    m_pFont = pFontMan->CreateA3DFont(font, 512, 128, 128);

    // Create Camera
    m_pCamera = new CAnmCamera();
    if (!m_pCamera->Init())
    {
        a_LogOutput(1, "CAnimator::InitA3DEngine: Failed to create camera object!");
        return false;
    }

	if (m_pCommonActionManager == NULL)
		m_pCommonActionManager = new CAnmCommonActionManager();

    m_eAnimationType = TYPE_EDITOR_ANIMATION;

    // Create Player
    m_pPlayer = new CAnmPlayer();

    // Create Objects Manager
    m_pObjectManager = new CAnmObjectManager();
    m_pObjectManager->SetCamera(m_pCamera);
    m_pObjectManager->SetPlayer(m_pPlayer);

    // Create Bezier Manager
    m_pBezierManager = new CAnmBezierManager(GetA3DEngine()->GetA3DWireCollector());

    // Create Axes
    m_pAxes = new CAnmAxes(GetA3DEngine()->GetA3DWireCollector());

    // Create Action Manager
    m_pActionManager = new CAnmActionManager();

    m_dwTotalTime = 8000;
    m_dwStartTime = 0;
    m_dwEndTime = 8000;
    m_dwNowTime = 0;
    m_fSpeed = 1.0f;
    m_bAnimatorReady = true;

	m_pSmartCamera = new SmartCamera( GetA3DCamera() );

	AIniFile iniFile;
	if (iniFile.Open("configs\\animations\\anm.ini"))
	{
		m_bShowLetterbox = iniFile.GetValueAsBoolean("LETTERBOX", "Visible", false);
	}

    return true;
}

void CAnimator::Release()
{
	if (m_pSmartCamera)
    {
        delete m_pSmartCamera;
        m_pSmartCamera = 0;
    }

    m_eAnimationType = TYPE_EDITOR_ANIMATION;
    m_iWorldInstance = 1;
    m_bUseHostPlayer = false;
    m_bShowHostPlayerWeapon = true;
    m_bSpecifyTime = false;
    m_bSpecifyWeather = false;
	m_bBlendFrame = false;
    m_uiMaskColor = 0x00000000;
    m_uiMonoColor = 0xFFFFFFFF;
    m_uiBackgroundColor = 0xFF000000;
    m_eBlackWhiteType = BWT_NONE;
    m_uiHideWorldType = 0;
    m_fMotionBlurLevel = 0.0f;
    m_dwTotalTime = 8000;
    m_dwNowTime = 0;
    m_fSpeed = 1.0f;
    m_bAnimatorReady = false;
    m_bDisableCamera = false;
    m_eState = STATE_WAITING;

    if (m_pFont)
    {
        A3DFontMan *pFontMan = GetA3DEngine()->GetA3DFontMan();
        pFontMan->ReleaseA3DFont(m_pFont);
        m_pFont = 0;
    }

    if (m_pAxes)
    {
        delete m_pAxes;
        m_pAxes = 0;
    }
    
    if (m_pActionManager)
    {
        delete m_pActionManager;
        m_pActionManager = 0;
    }

    if (m_pBezierManager)
    {
        delete m_pBezierManager;
        m_pBezierManager = 0;
    }

    if (m_pObjectManager)
    {
        delete m_pObjectManager;
        m_pObjectManager = 0;
    }

    if (m_pPlayer)
    {
        delete m_pPlayer;
        m_pPlayer = 0;
    }

    if (m_pCamera)
    {
        delete m_pCamera;
        m_pCamera = 0;
    }

	if (m_pImageLetterboxTop)
	{
		delete m_pImageLetterboxTop;
		m_pImageLetterboxTop = NULL;
	}

	if (m_pImageLetterboxBottom)
	{
		delete m_pImageLetterboxBottom;
		m_pImageLetterboxBottom = NULL;
	}

	if(m_pImagePreFrame)
	{
	   delete m_pImagePreFrame;
	   m_pImagePreFrame = NULL;
	}
}

bool CAnimator::Tick(DWORD dwDeltaTime)
{
    bool flag = true;

    if (m_fMoveCameraSpeed > 1.0f && m_fMoveCameraSpeed < 40.0f)
    {
        m_fMoveCameraSpeed += 0.1f * float(dwDeltaTime);
    }

    if (m_eState == STATE_PLAYING)
    {
        SetDialogue(L"");
		SetBlendFrame(false);
        SetMaskColor(0x00000000);
        SetMonoColor(0xFFFFFFFF);
        SetMonoColor(0xFF000000);
        SetBlackWhiteType(BWT_NONE);
        SetHideWorldType(0);
        SetMotionBlurLevel(0.0f);

        m_fTickTime += m_fSpeed * dwDeltaTime;
        dwDeltaTime = DWORD(m_fTickTime);
        m_fTickTime -= dwDeltaTime;

        m_dwNowTime += dwDeltaTime;
        if (m_dwNowTime > m_dwEndTime)
        {
            dwDeltaTime -= m_dwNowTime - m_dwEndTime;
            m_dwNowTime = m_dwEndTime;
        }
        if (!m_pActionManager->Tick(m_dwNowTime, m_pObjectManager))
        {
            flag = false;
        }
        if (!m_pObjectManager->Tick(dwDeltaTime))
        {
            flag = false;
        }
        if (m_dwNowTime >= m_dwEndTime)
        {
            Stop();
        }
    }
    else
    {
        if (!m_pObjectManager->Tick(0))
        {
            flag = false;
        }
    }
    AdjustCameraPos();

    return flag;
}

bool CAnimator::TickSmartCamera(DWORD dwDeltaTime)
{
    if (m_pSmartCamera && m_pSmartCamera->IsPlaying())
    {
        if (!m_pSmartCamera->Tick(dwDeltaTime))
        {
            m_pSmartCamera->Stop();
        }
        if (m_pCamera)
        {
            m_pCamera->SetPos(m_pSmartCamera->GetPos());
            m_pCamera->SetDirAndUp(m_pSmartCamera->GetDir(), m_pSmartCamera->GetUp());
            m_pCamera->SetFOV(m_pSmartCamera->GetFOV());
        }
        return true;
    }
    return false;
}

void CAnimator::InitLetterboxImage()
{
	AString szImgTopFile, szImgBottomFile;
	AIniFile iniFile;
	if (iniFile.Open("configs\\animations\\anm.ini"))
	{
		szImgTopFile = iniFile.GetValueAsString("LETTERBOX", "Top", "");
		szImgBottomFile = iniFile.GetValueAsString("LETTERBOX", "Bottom", "");
	}

	if (!m_pImageLetterboxTop)
	{
		m_pImageLetterboxTop = new A2DSprite();
		m_pImageLetterboxTop->Init(GetA3DDevice(), szImgTopFile, 0);
	}
	if (!m_pImageLetterboxBottom)
	{
		m_pImageLetterboxBottom = new A2DSprite();
		m_pImageLetterboxBottom->Init(GetA3DDevice(), szImgBottomFile, 0);
	}
}

bool CAnimator::Render(A3DViewport *pViewport, bool bRenderShadow)
{
    return m_pObjectManager->Render(pViewport, bRenderShadow);
}

#ifdef _PLATFORM_RENDER
bool CAnimator::Render(const ECRENDERPARAM* pRenderParam)
{
	return Render(pRenderParam->pViewport->GetA3DViewport());
}
#endif // _PLATFORM_RENDER

void CAnimator::RenderMask(A3DViewport *pViewport)
{
    // Render Mask
    if (m_uiMaskColor & 0xFF000000)
    {
        A3DDEVFMT devFmt = GetA3DDevice()->GetDevFormat();
		if(!m_bBlendFrame)
		{
            GetA3DEngine()->GetA3DFlatCollector()->AddRect_2D(0, 0, devFmt.nWidth, devFmt.nHeight, m_uiMaskColor);
            GetA3DEngine()->GetA3DFlatCollector()->Flush();	
		}
		else
		{
			A3DVIEWPORTPARAM *pMainView = pViewport->GetParam();
			if (m_bPreparePreFrame && m_pImagePreFrame)
			{
				m_pImagePreFrame->SetAlpha((m_uiMaskColor & 0xFF000000) >> 24);
				m_pImagePreFrame->SetScaleX((FLOAT)pMainView->Width / m_pImagePreFrame->GetWidth());
				m_pImagePreFrame->SetScaleY((FLOAT)pMainView->Height / m_pImagePreFrame->GetHeight());
				m_pImagePreFrame->DrawToBack(0, 0);
			}
		}
	
    }

}

void CAnimator::RenderText(A3DViewport *pViewport)
{
	bool bDone = false;
	if (m_bShowLetterbox)
		bDone = m_pAnmInst->RenderLetterboxAndText();
	if (!bDone)
	{
		A3DVIEWPORTPARAM *pMainView = pViewport->GetParam();

		// Render letterbox image
		if (m_bShowLetterbox)
		{
			if (!m_pImageLetterboxTop || !m_pImageLetterboxBottom)
				InitLetterboxImage();

			A3DVIEWPORTPARAM *pMainView = pViewport->GetParam();
			if (m_pImageLetterboxTop)
			{
				m_pImageLetterboxTop->SetScaleX((FLOAT)pMainView->Width / m_pImageLetterboxTop->GetWidth());
				m_pImageLetterboxTop->DrawToBack(0, 0);
			}

			if (m_pImageLetterboxBottom)
			{
				m_pImageLetterboxBottom->SetScaleX((FLOAT)pMainView->Width / m_pImageLetterboxBottom->GetWidth());
				m_pImageLetterboxBottom->DrawToBack(0, pMainView->Height - m_pImageLetterboxBottom->GetHeight());
			}
		}

		// Render Text
		m_pFont->TextOut(pMainView->X + pMainView->Width / 2 - m_sDialogue.GetLength() * 12,
						 pMainView->Y + pMainView->Height - 50, m_sDialogue, 0xFFFFFFFF);
		m_pFont->Flush();
	}
}

void CAnimator::RenderWire(A3DViewport *pViewport)
{
    CAnmObject *pObject = m_pObjectManager->GetSelectedObject();
    if (pObject)
    {
        const A3DAABB objectAABB = pObject->GetAABB();
        GetA3DEngine()->GetA3DWireCollector()->AddAABB(objectAABB, 0xFF00FF00);
        GetA3DEngine()->GetA3DWireCollector()->Flush();
        A3DVECTOR3 vPos = pObject->GetPos() + A3DVECTOR3(0.0f, 2.0f, 0.0f);
        A3DVECTOR3 vPosIn = pObject->GetPos()+A3DVECTOR3(0.0f, objectAABB.Maxs.y-objectAABB.Mins.y + 0.5f, 0.0f);
        A3DVECTOR3 vPosOut;
        GetA3DViewport()->Transform(vPosIn, vPosOut);
        GetA3DEngine()->GetA3DFlatCollector()->AddRect_2D(
            int(vPosOut.x-20), int(vPosOut.y-5),
            int(vPosOut.x+20), int(vPosOut.y+6),
            0x7F3F007F, vPosOut.z);
        GetA3DEngine()->GetA3DFlatCollector()->Flush();

        ACString number;
        number.Format(L"%u", pObject->GetObjectID());
        GetA3DEngine()->GetSystemFont()->TextOut3D(number, g_Animator.GetA3DViewport(), vPosIn, 0, A3DCOLORRGB(255, 255, 255));
        GetA3DEngine()->GetSystemFont()->Flush();

        if (pObject->GetObjectID())
        {
            pObject->UpdateOrientation();
            A3DQUATERNION qOrientation = pObject->GetOrientation();
            A3DVECTOR3 aFrustum[8];
            for (unsigned int i=0; i<8; ++i)
            {
                aFrustum[i] = qOrientation ^ m_aFrustum[i];
                aFrustum[i] += vPos;
            }
            GetA3DEngine()->GetA3DWireCollector()->Add3DLine(aFrustum[0], aFrustum[1], A3DCOLORRGB(255, 0, 255));
            GetA3DEngine()->GetA3DWireCollector()->Add3DLine(aFrustum[1], aFrustum[2], A3DCOLORRGB(255, 0, 255));
            GetA3DEngine()->GetA3DWireCollector()->Add3DLine(aFrustum[2], aFrustum[3], A3DCOLORRGB(255, 0, 255));
            GetA3DEngine()->GetA3DWireCollector()->Add3DLine(aFrustum[3], aFrustum[0], A3DCOLORRGB(255, 0, 255));
        
            GetA3DEngine()->GetA3DWireCollector()->Add3DLine(aFrustum[0], aFrustum[4], A3DCOLORRGB(255, 0, 255));
            GetA3DEngine()->GetA3DWireCollector()->Add3DLine(aFrustum[1], aFrustum[5], A3DCOLORRGB(255, 0, 255));
            GetA3DEngine()->GetA3DWireCollector()->Add3DLine(aFrustum[2], aFrustum[6], A3DCOLORRGB(255, 0, 255));
            GetA3DEngine()->GetA3DWireCollector()->Add3DLine(aFrustum[3], aFrustum[7], A3DCOLORRGB(255, 0, 255));
        
            GetA3DEngine()->GetA3DWireCollector()->Add3DLine(aFrustum[4], aFrustum[5], A3DCOLORRGB(0, 255, 0));
            GetA3DEngine()->GetA3DWireCollector()->Add3DLine(aFrustum[5], aFrustum[6], A3DCOLORRGB(255, 0, 255));
            GetA3DEngine()->GetA3DWireCollector()->Add3DLine(aFrustum[6], aFrustum[7], A3DCOLORRGB(0, 0, 255));
            GetA3DEngine()->GetA3DWireCollector()->Add3DLine(aFrustum[7], aFrustum[4], A3DCOLORRGB(255, 0, 255));

            A3DVECTOR3 vCenter = pObject->GetPos();
            GetA3DEngine()->GetA3DWireCollector()->Add3DLine(vCenter - A3DVECTOR3(objectAABB.Extents.x, 0.0f, 0.0f),
                                                                      vCenter + A3DVECTOR3(objectAABB.Extents.x, 0.0f, 0.0f),
                                                                      A3DCOLORRGB(255, 0, 0));
            GetA3DEngine()->GetA3DWireCollector()->Add3DLine(vCenter - A3DVECTOR3(0.0f, objectAABB.Extents.y, 0.0f),
                                                                      vCenter + A3DVECTOR3(0.0f, objectAABB.Extents.y, 0.0f),
                                                                      A3DCOLORRGB(0, 255, 0));
            GetA3DEngine()->GetA3DWireCollector()->Add3DLine(vCenter - A3DVECTOR3(0.0f, 0.0f, objectAABB.Extents.z),
                                                                      vCenter + A3DVECTOR3(0.0f, 0.0f, objectAABB.Extents.z),
                                                                      A3DCOLORRGB(0, 0, 255));
        }
    }
    
    if (m_eState != STATE_PLAYING)
    {
        m_pBezierManager->Render();
        GetA3DEngine()->GetA3DWireCollector()->Flush();
        m_pAxes->Render();
        GetA3DEngine()->GetA3DWireCollector()->Flush();
    }

    /*
    // Render Black border
    A3DVIEWPORTPARAM *pMainView = pViewport->GetParam();
    RECT rect;
    rect.left = pMainView->X;
    rect.right = pMainView->Width;
    rect.top = pMainView->Y;
    rect.bottom = pMainView->Height;
    if (pMainView->Height * 16 > pMainView->Width * 9)
    {
        rect.bottom = pMainView->Width * 9 / 16;
        rect.top = (pMainView->Height - rect.bottom) >> 1;
        g_pGame->GetA3DEngine()->GetA3DFlatCollector()->AddRect_2D(0, 0, rect.right, rect.top, 0xFF000000);
        g_pGame->GetA3DEngine()->GetA3DFlatCollector()->AddRect_2D(0, pMainView->Height - rect.top, rect.right, pMainView->Height, 0xFF000000);
        g_pGame->GetA3DEngine()->GetA3DFlatCollector()->Flush();
    }
    else if (pMainView->Height * 16 < pMainView->Width * 9)
    {
        rect.right = pMainView->Height * 16 / 9;
        rect.left = (pMainView->Width - rect.right) >> 1;
        g_pGame->GetA3DEngine()->GetA3DFlatCollector()->AddRect_2D(0, 0, rect.left, rect.bottom, 0xFF000000);
        g_pGame->GetA3DEngine()->GetA3DFlatCollector()->AddRect_2D(pMainView->Width - rect.left, 0, pMainView->Width, rect.bottom, 0xFF000000);
        g_pGame->GetA3DEngine()->GetA3DFlatCollector()->Flush();
    }
    */
}

bool CAnimator::IsReady() const
{
    return m_bAnimatorReady;
}

void CAnimator::MoveFront(const float d)
{
    if (m_eState != STATE_PLAYING || m_bDisableCamera)
    {
        m_pCamera->SetPos(m_pCamera->GetPos() + (d * m_pCamera->GetDir()));
    }
}

void CAnimator::MoveBack(const float d)
{
    if (m_eState != STATE_PLAYING || m_bDisableCamera)
    {
        m_pCamera->SetPos(m_pCamera->GetPos() + (-d * m_pCamera->GetDir()));
    }
}

void CAnimator::MoveLeft(const float d)
{
    if (m_eState != STATE_PLAYING || m_bDisableCamera)
    {
        m_pCamera->SetPos(m_pCamera->GetPos() + (d * m_pCamera->GetLeft()));
    }
}

void CAnimator::MoveRight(const float d)
{
    if (m_eState != STATE_PLAYING || m_bDisableCamera)
    {
        m_pCamera->SetPos(m_pCamera->GetPos() + (-d * m_pCamera->GetLeft()));
    }
}

void CAnimator::MoveUp(const float d)
{
    if (m_eState != STATE_PLAYING || m_bDisableCamera)
    {
        m_pCamera->SetPos(m_pCamera->GetPos() + A3DVECTOR3(0.0f, d, 0.0f));
    }
}

void CAnimator::MoveDown(const float d)
{
    if (m_eState != STATE_PLAYING || m_bDisableCamera)
    {
        m_pCamera->SetPos(m_pCamera->GetPos() + A3DVECTOR3(0.0f, -d, 0.0f));
    }
}

void CAnimator::DegDelta(const float d)
{
    if (m_eState != STATE_PLAYING || m_bDisableCamera)
    {
        GetA3DCamera()->DegDelta(d);
        m_pCamera->UpdateOrientation();
		/*A3DVECTOR3 vDir = m_pCamera->GetDir();
        A3DVECTOR3 vUp = m_pCamera->GetUp();
		A3DVECTOR3 vRight;
		vRight.CrossProduct(vUp,vDir);
		A3DQUATERNION qOri;
		qOri.ConvertFromAxisAngle(vUp,d*0.01);
		vDir = qOri ^ vDir;
		vRight = qOri ^ vRight;
		vUp.CrossProduct(vDir,vRight);
		m_pCamera->SetDirAndUp(vDir,vUp);*/
    }
}

void CAnimator::PitchDelta(const float d)
{
    if (m_eState != STATE_PLAYING || m_bDisableCamera)
    {
        //GetA3DCamera()->PitchDelta(d);
		//m_pCamera->UpdateOrientation();
		A3DVECTOR3 vDir = m_pCamera->GetDir();
        A3DVECTOR3 vUp = m_pCamera->GetUp();
		A3DVECTOR3 vRight;
		vRight.CrossProduct(vUp,vDir);
		A3DQUATERNION qOri;
		qOri.ConvertFromAxisAngle(vRight,-0.01*d);
		vDir = qOri ^ vDir;
		vUp  = qOri ^ vUp;
        m_pCamera->SetDirAndUp(vDir,vUp);
    }
}

void CAnimator::RollDelta(const float d)
{
    if (m_eState != STATE_PLAYING || m_bDisableCamera)
    {
        A3DVECTOR3 vDir = m_pCamera->GetDir();
        A3DVECTOR3 vUp = m_pCamera->GetUp();
		//A3DVECTOR3 vRight;
		//vRight.CrossProduct(vUp,vDir);
        A3DQUATERNION qOri;
        qOri.ConvertFromAxisAngle(vDir, d);
        vDir = qOri ^ vDir;
        vUp = qOri ^ vUp;
		//vRight = qOri ^ vRight;
		//vDir.CrossProduct(vRight,vUp);

        m_pCamera->SetDirAndUp(vDir, vUp);
    }
}

void CAnimator::AddBezierPoint(const A3DVECTOR3 &v)
{
    CAnmBezier *pBezier = m_pBezierManager->GetSelectedBezier();
    if (pBezier)
    {
        pBezier->AddPoint(v);
    }
}

void CAnimator::Play()
{
    if (m_eAnimationType == TYPE_EDITOR_ANIMATION)
    {
        if (m_bSpecifyTime)
        {
            int nTimeInDay = g_Animator.GetHour() * 3600 + g_Animator.GetMinute() * 60 + g_Animator.GetSecond();
            GetWorld()->GetSunMoon()->SetTimeOfTheDay(nTimeInDay / (4.0f * 3600.0f));
        }

        if (m_bSpecifyWeather)
        {
//             A3DRain *pRain = GetWorld()->m_pRain;
//             if (pRain)
//             {
//                 if (m_bRain)
//                 {
//                     pRain->StartRain();
//                 }
//                 else
//                 {
//                     pRain->Stop();
//                 }
//             }
// 
//             A3DSnow *pSnow = GetWorld()->m_pSnow;
//             if (pSnow)
//             {
//                 if (m_bSnow)
//                 {
//                     pSnow->StartSnow();
//                 }
//                 else
//                 {
//                     pSnow->Stop();
//                 }
//             }
        }
    }

    if (m_eState != STATE_PAUSING)
    {
        m_pActionManager->Prepare();
        SetNowTime(m_dwStartTime);
        m_fSpeed = 1.0f;
        m_fTickTime = 0.0f;
    }
    m_eState = STATE_PLAYING;
    //a_LogOutput(1, "Play");
}

void CAnimator::JumpTo(const DWORD dwNowTime)
{
    if (dwNowTime >= 0 && GetNowTime() != dwNowTime)
    {
        if (GetNowTime() > dwNowTime)
        {
            Stop();
            Play();
            SetNowTime(dwNowTime);
            Tick(0);
            Pause();
        }
        else
        {
            Play();
            DWORD dwDeltaTime = dwNowTime - GetNowTime();
            SetDialogue(L"");
			SetBlendFrame(false);
            SetMaskColor(0x00000000);
            SetMonoColor(0xFFFFFFFF);
            SetMonoColor(0xFF000000);
            SetBlackWhiteType(BWT_NONE);
            SetHideWorldType(0);
            SetNowTime(dwNowTime);
            m_pActionManager->Tick(GetNowTime(), m_pObjectManager);
            m_pObjectManager->Tick(dwDeltaTime);
            Pause();
        }
   }
}

void CAnimator::Stop()
{
    m_eState = STATE_WAITING;
    m_pActionManager->Reset();
    m_pObjectManager->Reset();
    SetDialogue(L"");
    m_fTickTime = 0.0f;
    //a_LogOutput(1, "Stop");
}

void CAnimator::Pause()
{
    //a_LogOutput(1, "Pause");
    m_eState = STATE_PAUSING;
    m_fTickTime = 0.0f;
}

A3DEngine* CAnimator::GetA3DEngine() const
{
    return m_pAnmInst->GetA3DEngine();
}

A3DDevice* CAnimator::GetA3DDevice() const
{
    return GetA3DEngine()->GetA3DDevice();
}

A3DCamera* CAnimator::GetA3DCamera() const
{
    return m_pAnmInst->GetA3DCamera();
}

A3DViewport* CAnimator::GetA3DViewport() const
{
    return m_pAnmInst->GetA3DViewport();
}

CECWorld* CAnimator::GetWorld() const
{
   return m_pAnmInst->GetWorld();
}
CECGameRun* CAnimator::GetGameRun() const
{
   return m_pAnmInst->GetGameRun();
}
CAnmObjectManager* CAnimator::GetObjectManager() const
{
    return m_pObjectManager;
}

CAnmBezierManager* CAnimator::GetBezierManager() const
{
    return m_pBezierManager;
}

CAnmActionManager* CAnimator::GetActionManager() const
{
    return m_pActionManager;
}

CAnmRayTrace* CAnimator::GetRayTrace() const
{
    return m_pRayTrace;
}

CAnmAxes* CAnimator::GetAxes() const
{
    return m_pAxes;
}

CAnmCommonActionManager* CAnimator::GetCommonActionManager() const
{
	return m_pCommonActionManager;
}

void CAnimator::SetUseHostPlayer(const bool b)
{
    m_bUseHostPlayer = b;
}

bool CAnimator::IsUseHostPlayer() const
{
    return m_bUseHostPlayer;
}

void CAnimator::SetShowHostPlayerWeapon(const bool b)
{
    m_bShowHostPlayerWeapon = b;
}

bool CAnimator::IsShowHostPlayerWeapon() const
{
    return m_bShowHostPlayerWeapon;
}

void CAnimator::SetSpecifyTime(const bool b)
{
    m_bSpecifyTime = b;
}

bool CAnimator::IsSpecifyTime() const
{
    return m_bSpecifyTime;
}

void CAnimator::SetWorldTime(const int hour, const int minute, const int second)
{
    m_iHour = hour % 24;
    m_iMinute = minute % 60;
    m_iSecond = second % 60;
}

int CAnimator::GetHour() const
{
    return m_iHour;
}

int CAnimator::GetMinute() const
{
    return m_iMinute;
}

int CAnimator::GetSecond() const
{
    return m_iSecond;
}

void CAnimator::SetSpecifyWeather(const bool b)
{
    m_bSpecifyWeather = b;
}

bool CAnimator::IsSpecifyWeahter() const
{
    return m_bSpecifyWeather;
}

void CAnimator::SetWeatherRain(const bool b)
{
    m_bRain = b;
}

bool CAnimator::IsRain() const
{
    return m_bRain;
}

void CAnimator::SetWeatherSnow(const bool b)
{
    m_bSnow = b;
}

bool CAnimator::IsSnow() const
{
    return m_bSnow;
}

void CAnimator::ShowLetterbox(const bool b)
{
	m_bShowLetterbox = true;
}

bool CAnimator::IsShowLetterbox() const
{
	return m_bShowLetterbox;
}

void CAnimator::SetVolume(DWORD dwVolume)
{
    m_dwVolume = dwVolume;
}

DWORD CAnimator::GetVolume() const
{
    return m_dwVolume;
}

void CAnimator::SetDialogue(const ACString &st)
{
    m_sDialogue = st;
	m_pAnmInst->UpdateText(&st);
}

ACString CAnimator::GetDialogue() const
{
    return m_sDialogue;
}

void CAnimator::SetMaskColor(const unsigned int color)
{
    m_uiMaskColor = color;
}

unsigned int CAnimator::GetMaskColor() const
{
    return m_uiMaskColor;
}
void CAnimator::SetBlendFrame(bool flag)
{
	m_bBlendFrame = flag;  
}
bool CAnimator::IsBlendFrame() const
{
   return m_bBlendFrame;
}
void CAnimator::GeneratePreFrame()
{
	   m_bPreparePreFrame = false;
	   int nWidth = GetA3DDevice()->GetDevFormat().nWidth;
	   int nHeight = GetA3DDevice()->GetDevFormat().nHeight;
	   if(!m_pImagePreFrame)
	   {
	      m_pImagePreFrame = new A2DSprite();
		  m_pImagePreFrame->InitWithoutSurface(GetA3DDevice(),nWidth,nHeight,GetA3DDevice()->GetDevFormat().fmtTarget,1,NULL);
	   }
	   assert(m_pImagePreFrame);
	   A3DSurface * pTempSurface = NULL;
	   pTempSurface = new A3DSurface();
	   if( !pTempSurface->Create(GetA3DDevice(), nWidth, nHeight, GetA3DDevice()->GetDevFormat().fmtTarget) )
	   {
		   delete pTempSurface;
		   pTempSurface = NULL;
	   }
	   GetGameRun()->Render(false);
	   Sleep(100);
	   //GetA3DDevice()->ExportColorToFile("²âÊÔ¶¯»­½ØÍ¼.bmp");
	   HRESULT hval;
#ifdef BACK_VERSION
	   hval = GetA3DDevice()->GetD3DDevice()->CopyRects(GetA3DDevice()->GetBackBuffer(), NULL, 0, pTempSurface->GetDXSurface(), NULL);
#else
	   hval = D3DXLoadSurfaceFromSurface(pTempSurface->GetDXSurface(), NULL, NULL, GetA3DDevice()->GetBackBuffer()->m_pD3DSurface, NULL, NULL, D3DX_DEFAULT, 0);
	   //hval = GetA3DDevice()->StretchRect(GetA3DDevice()->GetBackBuffer()->m_pD3DSurface, NULL, pTempSurface->GetDXSurface(), NULL, A3DTEXF_LINEAR);
#endif
	   if( hval == D3D_OK )
	   {
		   D3DLOCKED_RECT rectLocked;
		   hval = pTempSurface->GetDXSurface()->LockRect(&rectLocked, NULL, D3DLOCK_READONLY);
		   if( hval == D3D_OK )
		   {
			   {
				   	BYTE * pTempLine = new BYTE[rectLocked.Pitch];
					int  y;
					BYTE * pLines = NULL;
					BYTE * pLines2 = NULL;
					for(y=0; y<nHeight/2; y++)
					{
						pLines = (BYTE *)((BYTE*)rectLocked.pBits + y * rectLocked.Pitch);
						pLines2 = (BYTE *)((BYTE*)rectLocked.pBits + (nHeight-y-1) * rectLocked.Pitch);
						memcpy(pTempLine,pLines,rectLocked.Pitch);
						memcpy(pLines,pLines2,rectLocked.Pitch);
						memcpy(pLines2,pTempLine,rectLocked.Pitch);
						
					}

					delete [] pTempLine;
			   }
			   m_pImagePreFrame->UpdateTextures((BYTE*)(rectLocked.pBits),(DWORD)(rectLocked.Pitch),pTempSurface->GetFormat());
			   pTempSurface->GetDXSurface()->UnlockRect();
		   }
		   
	   }
	   if( pTempSurface )
	   {
		   pTempSurface->Release();
		   delete pTempSurface;
		   pTempSurface = NULL;
		}
	   m_bPreparePreFrame = true;
}
void CAnimator::SetMonoColor(const unsigned int color)
{
    m_uiMonoColor = color;
}

unsigned int CAnimator::GetMonoColor() const
{
    return m_uiMonoColor;
}

void CAnimator::SetBackgroundColor(const unsigned int color)
{
    m_uiBackgroundColor = color;
}

unsigned int CAnimator::GetBackgroundColor() const
{
    return m_uiBackgroundColor;
}

void CAnimator::SetBlackWhiteType(const BlackWhiteType &bt)
{
    m_eBlackWhiteType = bt;
}

CAnimator::BlackWhiteType CAnimator::GetBlackWhiteType() const
{
    return m_eBlackWhiteType;
}

void CAnimator::SetHideWorldType(const unsigned int t)
{
    m_uiHideWorldType = t;
}

unsigned int CAnimator::GetHideWorldType() const
{
    return m_uiHideWorldType;
}

void CAnimator::SetMotionBlurLevel(const float fLevel)
{
    if (fLevel > 0.0f)
    {
        m_fMotionBlurLevel = fLevel;
    }
    else
    {
        m_fMotionBlurLevel = 0.0f;
    }
}

float CAnimator::GetMotionBlurLevel() const
{
    return m_fMotionBlurLevel;
}

bool CAnimator::IsMotionBlur() const
{
    return m_fMotionBlurLevel > 0.0f;
}

void CAnimator::SetTotalTime(const DWORD dwTotalTime)
{
    m_dwTotalTime = dwTotalTime;
    m_dwStartTime = 0;
    m_dwNowTime = 0;
    m_dwEndTime = dwTotalTime;
    m_fSpeed = 1.0f;
}

DWORD CAnimator::GetTotalTime() const
{
    return m_dwTotalTime;
}

void CAnimator::SetNowTime(const DWORD dwNowTime)
{
    m_dwNowTime = dwNowTime;
}

DWORD CAnimator::GetNowTime() const
{
    return m_dwNowTime;
}

void CAnimator::SetStartTime(const DWORD dwStartTime)
{
    m_dwStartTime = dwStartTime;
}

DWORD CAnimator::GetStartTime() const
{
    return m_dwStartTime;
}

void CAnimator::SetEndTime(const DWORD dwEndTime)
{
    m_dwEndTime = dwEndTime;
}

DWORD CAnimator::GetEndTime() const
{
    return m_dwEndTime;
}

void CAnimator::SetSpeed(const float speed)
{
    m_fSpeed = speed;
}

float CAnimator::GetSpeed() const
{
    return m_fSpeed;
}

CAnimator::AnimatorState CAnimator::GetState() const
{
    return m_eState;
}

float CAnimator::GetPosHeight(const A3DVECTOR3 &vPos) const
{
    A3DVECTOR3 vDir = A3DVECTOR3(0.0f, -1000.0f, 0.0f);
    if (m_pRayTrace->RayTrace(vPos, vDir, CAnmRayTrace::FILTER_TERRAIN))
    {
        return m_pRayTrace->GetHitPos().y;
    }
    return 0.0f;
}

void CAnimator::MoveCamera(const char key, const bool bSlow)
{
    if (!m_bMoveCamera)
    {
        m_bMoveCamera = true;
        m_fMoveCameraSpeed = 2.0f;
    }
    if (bSlow)
    {
        m_fMoveCameraSpeed = 0.1f;
    }

    int nowTime = clock();
    if (m_iMoveLastTime == 0)
    {
        m_iMoveLastTime = nowTime - 1;
    }
    float delta = 0.001f * (nowTime - m_iMoveLastTime);
    m_iMoveLastTime = nowTime;
    float d = m_fMoveCameraSpeed * delta;

    switch (key)
    {
        case 'W':
        case 'w':
        {
            MoveFront(d);
            break;
        }
        case 'S':
        case 's':
        {
            MoveBack(d);
            break;
        }
        case 'A':
        case 'a':
        {
            MoveLeft(d);
            break;
        }
        case 'D':
        case 'd':
        {
            MoveRight(d);
            break;
        }
        case 'Q':
        case 'q':
        {
            MoveDown(d);
            break;
        }
        case 'E':
        case 'e':
        {
            MoveUp(d);
            break;
        }
        case 'Z':
        case 'z':
        {
            RollDelta(0.1f * d);
            break;
        }
        case 'X':
        case 'x':
        {
            RollDelta(-0.1f * d);
            break;
        }
		case 'v':
		case 'V':
		{
			PitchDelta(-1.0f*d);
			break;
		}
		case 'b':
		case 'B':
		{
			PitchDelta(1.0f*d);
			break;
		}
		case 'n':
		case 'N':
		{
            DegDelta(-1.0f*d);
			break;
		}
		case 'm':
		case 'M':
		{
            DegDelta(1.0f*d);
			break;
		}
        default:
            break;
    }
}

void CAnimator::StopCamera()
{
    m_bMoveCamera = false;
    m_fMoveCameraSpeed = 1.0f;
    m_iMoveLastTime = 0;
}

void CAnimator::DisableCamera(const bool b)
{
    m_bDisableCamera = b;
}

bool CAnimator::IsDisabledCamera() const
{
    return m_bDisableCamera;
}

void CAnimator::BindCamera(const bool b)
{
    m_bBindCamera = b;
}

bool CAnimator::IsBindCamera() const
{
    if (m_bBindCamera && m_pObjectManager)
    {
        CAnmObject *pObject = m_pObjectManager->GetSelectedObject();
        if (pObject && pObject->GetObjectID())
        {
            return true;
        }
    }
    return false;
}

void CAnimator::SetBindCameraOffset(const A3DVECTOR3 &v)
{
    m_vBindCameraOffset = v;
}

A3DVECTOR3 CAnimator::GetBindCameraOffset() const
{
    return m_vBindCameraOffset;
}

bool CAnimator::LoadWorld(int idInst, A3DVECTOR3 vHostPos)
{
    m_eState = STATE_WAITING;
    m_iWorldInstance = idInst;
	m_pCamera->SetPos(vHostPos);
    m_pAnmInst->LoadWorld(idInst, vHostPos);
    return true;
}

void CAnimator::UnloadWorld()
{
	m_eState = STATE_WAITING;
	m_iWorldInstance = 0;
	m_pAnmInst->UnloadWorld();
}


int CAnimator::GetWorldInstance() const
{
    return m_iWorldInstance;
}

void CAnimator::SetHostPlayerModel(CECModel *pModel)
{
    if (m_bUseHostPlayer && m_pPlayer && pModel)
    {
        m_pPlayer->SetHostPlayerModel(pModel);
    }
}

bool CAnimator::SaveFile(const wchar_t *szFile)
{
    return m_pTranslator->SaveFile(szFile);
}

bool CAnimator::ImportActionGroup(const wchar_t *szFile)
{
    return m_pTranslator->ImportActionGroup(szFile);
}

bool CAnimator::ExportActionGroup(const wchar_t *szFile, CAnmActionGroup *pActionGroup)
{
    return m_pTranslator->ExportActionGroup(szFile, pActionGroup);
}

CAnimator::~CAnimator()
{
    Release();
    delete m_pTranslator;
    delete m_pRayTrace;
	delete m_pCommonActionManager;
}

void CAnimator::AdjustCameraPos()
{
	CECWorld* pWorld = GetWorld();
    if( ! pWorld )
		return;
	A3DVECTOR3 vPos = m_pCamera->GetPos();
	float fHeight = pWorld->GetTerrainHeight( vPos );
	if( vPos.y > fHeight + 1.0f )
		return;
	vPos.y = fHeight + 1.0f;
	m_pCamera->SetPos( vPos );
}

bool CAnimator::LoadFileFromEditor( const wchar_t* szFile, const A3DVECTOR3& c_vCameraPos )
{
	m_eAnimationType = TYPE_EDITOR_ANIMATION;
	bool bRet = m_pTranslator->LoadFile( szFile, c_vCameraPos );
	if( ! bRet )
	{
		a_LogOutput( 1, "CAnimator::LoadFileFromEditor: Failed to animation file!" );
		return false;
	}
	Play();
	Tick(1);
	Stop();
	m_iCurrentAnimationID = 0;
	return true;
}

bool CAnimator::LoadFileFromEditor( int iAnimationID, const A3DVECTOR3& c_vCameraPos )
{
	AnmConfig* pAnimation = GetAnimation( iAnimationID );
	if( ! pAnimation )
		return false;
	
	bool bRet = LoadFileFromEditor( AS2WC( pAnimation->m_szPath ), c_vCameraPos );
	if( ! bRet )
		return false;
	
	m_iCurrentAnimationID = iAnimationID;
	return true;
}

bool CAnimator::LoadFileFromGame( const wchar_t* szFile, const A3DVECTOR3& c_vCameraPos )
{
	m_eAnimationType = TYPE_GAME_ANIMATION;
	bool bRet = m_pTranslator->LoadFile( szFile, c_vCameraPos );
	if( ! bRet )
	{
		a_LogOutput( 1, "CAnimator::LoadFileFromGame: Failed to camera animation file!" );
		return false;
	}
	m_iCurrentAnimationID = 0;
	return true;
}

bool CAnimator::LoadFileFromGame( int iAnimationID, const A3DVECTOR3& c_vCameraPos )
{
	AnmConfig* pAnimation = GetAnimation( iAnimationID );
	if( ! pAnimation )
		return false;
	
	bool bRet = LoadFileFromGame( AS2WC( pAnimation->m_szPath ), c_vCameraPos );
	if( ! bRet )
		return false;
	
	m_iCurrentAnimationID = iAnimationID;
	return true;
}

bool CAnimator::LoadObject( CAnmObject* pObject )
{
	return m_pAnmInst->LoadObject( pObject );
}

void CAnimator::UnloadObject( CAnmObject* pObject )
{
	m_pAnmInst->UnloadObject( pObject );
}

bool CAnimator::TickObject( CAnmObject* pObject, DWORD dwDeltaTime )
{
	return m_pAnmInst->TickObject( pObject, dwDeltaTime );
}

#ifdef BACK_VERSION
bool CAnimator::RenderObject( CAnmObject* pObject, bool bRenderShadow )
{
	return m_pAnmInst->RenderObject( pObject, bRenderShadow );
}
#else
bool CAnimator::RenderObject( CAnmObject* pObject, A3DViewport* pA3DViewport, bool bRenderShadow )
{
	return m_pAnmInst->RenderObject( pObject, pA3DViewport, bRenderShadow );
}
#endif

bool CAnimator::SaveConfigFile( const char* szFile )
{
	return m_pTranslator->SaveConfigFile( szFile );
}

bool CAnimator::LoadConfigFile( const char* szFile )
{
	return m_pTranslator->LoadConfigFile( szFile );
}

AnmConfig* CAnimator::GetAnimation( int iAnimationID )
{
	AnimationMap::const_iterator c_iterAnm = m_AnmMap.find( iAnimationID );
	return c_iterAnm == m_AnmMap.end() ? 0 : c_iterAnm->second;
}