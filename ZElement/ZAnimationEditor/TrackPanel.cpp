// TrackPanel.cpp : implementation file
//

#include "stdafx.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_Global.h"
#include "AnimationEditor.h"
#include "Animator.h"
#include "AnmActionManager.h"
#include "AnmBezierManager.h"
#include "TrackPanel.h"
#include "TrackGroupDlg.h"
#include "TrackChannelDlg.h"
#include "NewActionDlg.h"
#include "ActionMoveDlg.h"
#include "ActionRotateDlg.h"
#include "ActionScaleDlg.h"
#include "ActionFOVDlg.h"
#include "ActionMotionDlg.h"
#include "ActionJumpDlg.h"
#include "ActionBezierMoveDlg.h"
#include "ActionDisplayHideDlg.h"
#include "ActionDialogueDlg.h"
#include "ActionSoundDlg.h"
#include "ActionHookDlg.h"
#include "ActionSpeedControlDlg.h"
#include "ActionCameraBindDlg.h"
#include "ActionCameraFilterDlg.h"
#include "ActionCameraOscillatory.h"
#include "ActionTransparentDlg.h"
#include "ActionHideWorldDlg.h"
//#include "ActionBloomDlg.h"
//#include "ActionDOFDlg.h"
//#include "ActionRadialBlurDlg.h"
//#include "ActionMotionBlurDlg.h"
//#include "ActionWarpDlg.h"
#include "MoveActionsDlg.h"
#include "SetTimeDlg.h"

#include "AnmSystem.h"
#include "AnmActionGroup.h"
#include "AnmTranslator.h"
#include "AnmRotate.h"
#include "AnmFOV.h"

/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/

#define new A_DEBUG_NEW


/////////////////////////////////////////////////////////////////////////////
// CTrackPanel

IMPLEMENT_DYNCREATE(CTrackPanel, CFormView)

CTrackPanel::CTrackPanel()
	: CFormView(CTrackPanel::IDD)
{
    m_pSelectGroup = 0;
    m_pSelectChannel = 0;
    m_pSelectNode = 0;
    m_iRow = 0;
    m_vGroupList.clear();
	m_vSelNodes.clear();
    m_bButtonDown = false;
    m_pTempChannel = 0;
	m_bSelectedStart = false;
	//{{AFX_DATA_INIT(CTrackPanel)
	//}}AFX_DATA_INIT
}

CTrackPanel::~CTrackPanel()
{
    for (unsigned int i=0; i<m_vGroupList.size(); ++i)
    {
        delete m_vGroupList[i];
    }
    m_vGroupList.clear();
	m_vSelNodes.clear();
}

void CTrackPanel::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTrackPanel)
	//}}AFX_DATA_MAP
}

void CTrackPanel::SetSeconds(int v)
{
    m_iSeconds = v;
    m_TrackRuler.SetSeconds(m_iSeconds);
    for (unsigned int i=0; i<m_vGroupList.size(); ++i)
    {
        m_vGroupList[i]->SetSeconds(m_iSeconds);
    }
    m_HScrollBar.SetScrollRange(0, 20*m_iSeconds-1, false);
}

int CTrackPanel::GetSeconds() const
{
    return m_iSeconds;
}

void CTrackPanel::SetRow(int v)
{
    m_iRow = v;
    m_VScrollBar.SetScrollRange(0, m_iRow-1, false);
    InvalidateRect(NULL);
}

void CTrackPanel::CreateGroup()
{
    CAnmActionManager *pActionManager = g_Animator.GetActionManager();
    unsigned int uiID = pActionManager->CreateGroup();
    CAnmActionGroup *pGroup = pActionManager->GetGroup(uiID);
    pGroup->SetGroupName(_T("分组"));
    CTrackGroup *pTrackGroup = new CTrackGroup(pGroup);
    pTrackGroup->SetSeconds(m_iSeconds);
    pTrackGroup->OnHScroll(m_HScrollBar.GetScrollPos());
    m_vGroupList.push_back(pTrackGroup);
    SetRow(m_iRow+1);
}

void CTrackPanel::CreateChannel()
{
    if (!m_vGroupList.empty())
    {
        if (m_pSelectGroup)
        {
            m_pSelectGroup->CreateChannel();
            if (!m_pSelectGroup->IsFold())
            {
                m_pSelectGroup->SetSeconds(m_iSeconds);
                m_pSelectGroup->OnHScroll(m_HScrollBar.GetScrollPos());
                SetRow(m_iRow+1);
            }
            else
            {
                InvalidateRect(NULL);
            }
        }
    }
}

void CTrackPanel::DeleteGroup(const unsigned int uiID)
{
    std::vector<CTrackGroup *>::iterator pos;
    for (pos=m_vGroupList.begin(); pos!=m_vGroupList.end(); ++pos)
    {
        if ((*pos)->GetGroupID() == uiID)
        {
            if ((*pos)->IsFold())
            {
                SetRow(m_iRow - 1);
            }
            else
            {
                SetRow(m_iRow - (*pos)->GetChannelAmount() - 1);
            }
            g_Animator.GetActionManager()->DeleteGroup(uiID);
            delete (*pos);
            m_vGroupList.erase(pos);
            m_pSelectGroup = NULL;
            break;
        }
    }
}

void CTrackPanel::UpdatePanel()
{
	unsigned int i;
    for (i=0; i<m_vGroupList.size(); ++i)
    {
        delete m_vGroupList[i];
    }
    m_vGroupList.clear();
	m_vSelNodes.clear();
    m_pSelectGroup = 0;
    m_pSelectChannel = 0;
    m_pSelectNode = 0;
    m_HScrollBar.SetScrollPos(0);
    m_TrackRuler.OnHScroll(0);
    m_VScrollBar.SetScrollPos(0);
    m_iRow = 0;
    SetSeconds(g_Animator.GetTotalTime()/1000);

    CAnmActionManager *pActionManager = g_Animator.GetActionManager();
    std::vector<unsigned int> vList;
    pActionManager->GetGroupIDList(vList);
    for (i=0; i<vList.size(); ++i)
    {
        unsigned int uiID = vList[i];
        CAnmActionGroup *pGroup = pActionManager->GetGroup(uiID);
        CTrackGroup *pTrackGroup = new CTrackGroup(pGroup);
        pTrackGroup->SetSeconds(m_iSeconds);
		pTrackGroup->OnHScroll(0);
        pTrackGroup->UpdateGroup();
        m_vGroupList.push_back(pTrackGroup);
        m_iRow += pGroup->GetChannelAmount()+1;
    }
    SetRow(m_iRow);
}

CTrackNode* CTrackPanel::GetSelectedNode()
{
    if (m_pSelectGroup)
    {
        CTrackChannel *pTrackChannel = m_pSelectGroup->GetSelectChannel();
        if (pTrackChannel)
        {
            CTrackNode *pNode = pTrackChannel->GetSelectNode();
            return pNode;
        }
    }
    return 0;
}

void CTrackPanel::CopyAction(CAnmAction *pAction)
{
    if (pAction)
    {
        if (OpenClipboard())
        {
            TiXmlDocument doc = ConvertActionToXML(pAction);
            TiXmlPrinter printer;
            printer.SetStreamPrinting();
            doc.Accept(&printer);
            EmptyClipboard();
            HANDLE hClip = GlobalAlloc(GMEM_MOVEABLE, printer.Size() + 1);
            char *pBuff = (char*)GlobalLock(hClip);
            strcpy(pBuff, printer.CStr());
            GlobalUnlock(hClip);
            SetClipboardData(CF_TEXT, hClip);
            CloseClipboard();
        }
    }
}

void CTrackPanel::PasteAction()
{
    if (m_pSelectGroup)
    {
        CTrackChannel *pTrackChannel = m_pSelectGroup->GetSelectChannel();
        if (pTrackChannel)
        {
            CTrackNode *pNode = pTrackChannel->GetSelectNode();
            if (pNode)
            {
                CAnmAction *pAction = pNode->GetAction();
                if (!pAction)
                {
                    g_Animator.Stop();
                    if (OpenClipboard())
                    {
                        if (IsClipboardFormatAvailable(CF_TEXT))
                        {
                            HANDLE hClip = GetClipboardData(CF_TEXT);
                            char *pBuff = (char*)GlobalLock(hClip);
                            GlobalUnlock(hClip);
                            CloseClipboard();
                            TiXmlDocument doc;
                            doc.Parse(pBuff);
                            if (!doc.Error())
                            {
                                pAction = ConvertXMLToAction(&doc);
                                if (pAction)
                                {
                                    pAction->SetStartTime(pNode->GetStartTime());
                                    pAction->BindObjectID(pTrackChannel->GetObjectID());
                                    pNode->SetAction(pAction);
                                    InvalidateRect(NULL);
                                    return;
                                }
                            }
                        }
                        CloseClipboard();
                    }
                }
            }
        }
    }
    MessageBox(_T("粘贴事件失败!"), _T("动画编辑器"), MB_ICONEXCLAMATION | MB_OK);
}

void CTrackPanel::DeleteAction()
{
    if (m_pSelectGroup)
    {
        CTrackChannel *pTrackChannel = m_pSelectGroup->GetSelectChannel();
        if (pTrackChannel)
        {
            CTrackNode *pNode = pTrackChannel->GetSelectNode();
            if (pNode && pNode->GetAction())
            {
                int nBox = MessageBox(_T("确定是否要删除该事件?"), _T("动画编辑器"), MB_ICONQUESTION | MB_YESNOCANCEL);
                if (IDYES == nBox)
                {
                    pTrackChannel->DeleteAction(pNode->GetAction()->GetActionID());
                    g_Animator.Stop();
                    theApp.OnChanged();
                    InvalidateRect(NULL);
                }
            }
        }
    }
}

TiXmlDocument CTrackPanel::ConvertActionToXML(CAnmAction *pAction)
{
    TiXmlDocument doc;
    if (pAction)
    {
        TiXmlElement item("Action");
        item.SetAttribute("type", pAction->GetActionType());
        switch (pAction->GetActionType())
        {
            case CAnmAction::TYPE_MOVE:
            {
                CAnmMove *pMove = dynamic_cast<CAnmMove*>(pAction);
                item.SetDoubleAttribute("startPosX", pMove->GetStartPos().x);
                item.SetDoubleAttribute("startPosY", pMove->GetStartPos().y);
                item.SetDoubleAttribute("startPosZ", pMove->GetStartPos().z);
                item.SetDoubleAttribute("endPosX", pMove->GetEndPos().x);
                item.SetDoubleAttribute("endPosY", pMove->GetEndPos().y);
                item.SetDoubleAttribute("endPosZ", pMove->GetEndPos().z);
                item.SetDoubleAttribute("orientationW", pMove->GetOrientation().w);
                item.SetDoubleAttribute("orientationX", pMove->GetOrientation().x);
                item.SetDoubleAttribute("orientationY", pMove->GetOrientation().y);
                item.SetDoubleAttribute("orientationZ", pMove->GetOrientation().z);
                item.SetAttribute("axis", pMove->GetRotateAxis());
                item.SetDoubleAttribute("angle", pMove->GetAngle());
                item.SetDoubleAttribute("acceleration", pMove->GetAccelerationPercent());
                item.SetAttribute("totalFrames", pMove->GetTotalFrames());
                item.SetAttribute("closeTerrain", pMove->GetCloseTerrain());
                break;
            }
            case CAnmAction::TYPE_ROTATE:
            {
                CAnmRotate *pRotate = dynamic_cast<CAnmRotate*>(pAction);
                item.SetDoubleAttribute("startRotateW", pRotate->GetStartRotate().w);
                item.SetDoubleAttribute("startRotateX", pRotate->GetStartRotate().x);
                item.SetDoubleAttribute("startRotateY", pRotate->GetStartRotate().y);
                item.SetDoubleAttribute("startRotateZ", pRotate->GetStartRotate().z);
                item.SetDoubleAttribute("endRotateW", pRotate->GetEndRotate().w);
                item.SetDoubleAttribute("endRotateX", pRotate->GetEndRotate().x);
                item.SetDoubleAttribute("endRotateY", pRotate->GetEndRotate().y);
                item.SetDoubleAttribute("endRotateZ", pRotate->GetEndRotate().z);
                item.SetAttribute("totalFrames", pRotate->GetTotalFrames());
                break;
            }
            case CAnmAction::TYPE_JUMP:
            {
                CAnmJump *pJump = dynamic_cast<CAnmJump*>(pAction);
                item.SetDoubleAttribute("startPosX", pJump->GetStartPos().x);
                item.SetDoubleAttribute("startPosY", pJump->GetStartPos().y);
                item.SetDoubleAttribute("startPosZ", pJump->GetStartPos().z);
                item.SetDoubleAttribute("endPosX", pJump->GetEndPos().x);
                item.SetDoubleAttribute("endPosY", pJump->GetEndPos().y);
                item.SetDoubleAttribute("endPosZ", pJump->GetEndPos().z);
                item.SetAttribute("totalFrames", pJump->GetTotalFrames());
                break;
            }
            case CAnmAction::TYPE_BEZIER_MOVE:
            {
                CAnmBezierMove *pBezierMove = dynamic_cast<CAnmBezierMove*>(pAction);
                item.SetAttribute("bezierID", pBezierMove->GetBezierID());
                item.SetAttribute("closeTerrain", pBezierMove->GetCloseTerrain());
                break;
            }
            case CAnmAction::TYPE_MOTION:
            {
                CAnmMotion *pMotion = dynamic_cast<CAnmMotion*>(pAction);
                item.SetAttribute("motionType", pMotion->GetMotionType());
                item.SetAttribute("actionName", WC2AS(pMotion->GetActionName()));
                item.SetDoubleAttribute("speed", pMotion->GetActionSpeed());
                item.SetAttribute("showGFX", pMotion->IsShowGFX());
                item.SetAttribute("startFrame", pMotion->GetStartMotionFrame());
                item.SetAttribute("blendFrames", pMotion->GetBlendFrames());
                item.SetAttribute("totalFrames", pMotion->GetTotalFrames());
                break;
            }
            case CAnmAction::TYPE_DISPLAY_HIDE:
            {
                CAnmDisplayHide *pDisplayHide = dynamic_cast<CAnmDisplayHide*>(pAction);
                item.SetAttribute("visable", pDisplayHide->GetVisableType());
                break;
            }
            case CAnmAction::TYPE_DIALOGUE:
            {
                CAnmDialogue *pDialogue = dynamic_cast<CAnmDialogue*>(pAction);
                item.SetAttribute("dialogue", WC2AS(pDialogue->GetDialogue()));
                item.SetAttribute("totalFrames", pDialogue->GetTotalFrames());
                break;
            }
            case CAnmAction::TYPE_SOUND:
            {
                CAnmSound *pSound = dynamic_cast<CAnmSound*>(pAction);
                item.SetAttribute("startSeconds", pSound->GetStartSeconds());
                item.SetAttribute("volume", pSound->GetVolume());
                item.SetAttribute("soundFileName", WC2AS(pSound->GetSoundFileName()));
                item.SetAttribute("totalFrames", pSound->GetTotalFrames());
                break;
            }
            case CAnmAction::TYPE_HOOK:
            {
                CAnmHook *pHook = dynamic_cast<CAnmHook*>(pAction);
                item.SetAttribute("hangerName", WC2AS(pHook->GetHangerName()));
                item.SetAttribute("hookName", WC2AS(pHook->GetHookName()));
                item.SetAttribute("elseHookName", WC2AS(pHook->GetElseHookName()));
                item.SetAttribute("hookObjectID", pHook->GetHookObjectID());
                item.SetAttribute("totalFrames", pHook->GetTotalFrames());
                break;
            }
            case CAnmAction::TYPE_SPEED_CONTROL:
            {
                CAnmSpeedControl *pSpeed = dynamic_cast<CAnmSpeedControl*>(pAction);
                item.SetDoubleAttribute("speed", pSpeed->GetSpeed());
                item.SetAttribute("totalFrames", pSpeed->GetTotalFrames());
                break;
            }
            case CAnmAction::TYPE_SCALE:
            {
                CAnmScale *pScale = dynamic_cast<CAnmScale*>(pAction);
                item.SetDoubleAttribute("startScale", pScale->GetStartScale());
                item.SetDoubleAttribute("endScale", pScale->GetEndScale());
                item.SetAttribute("totalFrames", pScale->GetTotalFrames());
                break;
            }
            case CAnmAction::TYPE_FOV:
            {
                CAnmFOV *pFOV = dynamic_cast<CAnmFOV*>(pAction);
                item.SetDoubleAttribute("startFOV", pFOV->GetStartFOV());
                item.SetDoubleAttribute("endFOV", pFOV->GetEndFOV());
                item.SetAttribute("totalFrames", pFOV->GetTotalFrames());
                break;
            }
			case CAnmAction::TYPE_CAMERA_BIND:
			{
				CAnmCameraBind* pBind = dynamic_cast<CAnmCameraBind*>(pAction);
				item.SetAttribute("followobj",pBind->GetFllowObj());
				item.SetAttribute("totalFrames",pBind->GetTotalFrames());
				item.SetDoubleAttribute("offsetX",pBind->GetDistance().x);
				item.SetDoubleAttribute("offsetY",pBind->GetDistance().y);
				item.SetDoubleAttribute("offsetZ",pBind->GetDistance().z);
				break;
			}
            case CAnmAction::TYPE_CAMERA_FILTER:
            {
                CAnmCameraFilter *pFilter = dynamic_cast<CAnmCameraFilter*>(pAction);
                item.SetAttribute("color", pFilter->GetColor());
                item.SetAttribute("filterType", pFilter->GetFilterType());
                item.SetDoubleAttribute("transparent", pFilter->GetTransparent());
                item.SetAttribute("totalFrames", pFilter->GetTotalFrames());
                break;
            }
            case CAnmAction::TYPE_CAMERA_OSCILLATORY:
            {
                CAnmCameraOscillatory *pOscillatory = dynamic_cast<CAnmCameraOscillatory*>(pAction);
                item.SetAttribute("oscillatoryType", pOscillatory->GetOscillatoryType());
                item.SetAttribute("oscillatoryAxis", pOscillatory->GetOscillatoryAxis());
                item.SetAttribute("attenuation", pOscillatory->GetAttenuation());
                item.SetDoubleAttribute("displacement", pOscillatory->GetDisplacement());
                item.SetDoubleAttribute("frequency", pOscillatory->GetFrequency());
                item.SetAttribute("totalFrames", pOscillatory->GetTotalFrames());
                break;
            }
            case CAnmAction::TYPE_TRANSPARENT:
            {
                CAnmTransparent *pTransparent = dynamic_cast<CAnmTransparent*>(pAction);
                item.SetDoubleAttribute("startTransparent", pTransparent->GetStartTransparent());
                item.SetDoubleAttribute("endTransparent", pTransparent->GetEndTransparent());
                item.SetAttribute("totalFrames", pTransparent->GetTotalFrames());
                break;
            }
            case CAnmAction::TYPE_HIDE_WORLD_ACTION:
            {
                CAnmHideWorldAction *pHideWorldAction = dynamic_cast<CAnmHideWorldAction*>(pAction);
                item.SetAttribute("hideWorldType", pHideWorldAction->GetHideWorldType());
                item.SetAttribute("color", pHideWorldAction->GetColor());
                item.SetAttribute("totalFrames", pHideWorldAction->GetTotalFrames());
                break;
            }

            default: break;
        }
        doc.InsertEndChild(item);
    }
    return doc;
}

CAnmAction* CTrackPanel::ConvertXMLToAction(TiXmlDocument *pXml)
{
    if (pXml && !pXml->Error())
    {
        TiXmlElement *pItem = pXml->FirstChildElement();
        CTrackChannel *pTrackChannel = m_pSelectGroup->GetSelectChannel();
        if (pItem && pTrackChannel)
        {
            unsigned int uiType;
            int iResult;
            iResult = pItem->QueryUnsignedAttribute("type", &uiType);
            if (iResult == TIXML_SUCCESS)
            {
                unsigned int uiID = pTrackChannel->GetChannel()->CreateAction(CAnmAction::ActionType(uiType));
                CAnmAction *pAction = pTrackChannel->GetChannel()->GetAction(uiID);
                if (pAction)
                {
                    int iVal;
                    unsigned int uiVal;
                    float fVal;
                    bool bVal;
                    A3DVECTOR3 vVal;
                    A3DQUATERNION qVal;
                    std::string text;
                    switch (uiType)
                    {
                        case CAnmAction::TYPE_MOVE:
                        {
                            CAnmMove *pMove = dynamic_cast<CAnmMove*>(pAction);
                            pItem->QueryFloatAttribute("startPosX", &vVal.x);
                            pItem->QueryFloatAttribute("startPosY", &vVal.y);
                            pItem->QueryFloatAttribute("startPosZ", &vVal.z);
                            pMove->SetStartPos(vVal);
                            pItem->QueryFloatAttribute("endPosX", &vVal.x);
                            pItem->QueryFloatAttribute("endPosY", &vVal.y);
                            pItem->QueryFloatAttribute("endPosZ", &vVal.z);
                            pMove->SetEndPos(vVal);
                            pItem->QueryFloatAttribute("orientationW", &qVal.w);
                            pItem->QueryFloatAttribute("orientationX", &qVal.x);
                            pItem->QueryFloatAttribute("orientationY", &qVal.y);
                            pItem->QueryFloatAttribute("orientationZ", &qVal.z);
                            pMove->SetOrientation(qVal);
                            pItem->QueryIntAttribute("axis", &iVal);
                            pMove->SetRotateAxis(iVal);
                            pItem->QueryFloatAttribute("angle", &fVal);
                            pMove->SetAngle(fVal);
                            pItem->QueryFloatAttribute("acceleration", &fVal);
                            pMove->SetAccelerationPercent(fVal);
                            pItem->QueryIntAttribute("totalFrames", &iVal);
                            pMove->SetTotalFrames(iVal);
                            pItem->QueryBoolAttribute("closeTerrain", &bVal);
                            pMove->SetCloseTerrain(bVal);
                            break;
                        }
                        case CAnmAction::TYPE_ROTATE:
                        {
                            CAnmRotate *pRotate = dynamic_cast<CAnmRotate*>(pAction);
                            pItem->QueryFloatAttribute("startRotateW", &qVal.w);
                            pItem->QueryFloatAttribute("startRotateX", &qVal.x);
                            pItem->QueryFloatAttribute("startRotateY", &qVal.y);
                            pItem->QueryFloatAttribute("startRotateZ", &qVal.z);
                            pRotate->SetStartRotate(qVal);
                            pItem->QueryFloatAttribute("endRotateW", &qVal.w);
                            pItem->QueryFloatAttribute("endRotateX", &qVal.x);
                            pItem->QueryFloatAttribute("endRotateY", &qVal.y);
                            pItem->QueryFloatAttribute("endRotateZ", &qVal.z);
                            pRotate->SetEndRotate(qVal);
                            pItem->QueryIntAttribute("totalFrames", &iVal);
                            pRotate->SetTotalFrames(iVal);
                            break;
                        }
                        case CAnmAction::TYPE_JUMP:
                        {
                            CAnmJump *pJump = dynamic_cast<CAnmJump*>(pAction);
                            pItem->QueryFloatAttribute("startPosX", &vVal.x);
                            pItem->QueryFloatAttribute("startPosY", &vVal.y);
                            pItem->QueryFloatAttribute("startPosZ", &vVal.z);
                            pJump->SetStartPos(vVal);
                            pItem->QueryFloatAttribute("endPosX", &vVal.x);
                            pItem->QueryFloatAttribute("endPosY", &vVal.y);
                            pItem->QueryFloatAttribute("endPosZ", &vVal.z);
                            pJump->SetEndPos(vVal);
                            pItem->QueryIntAttribute("totalFrames", &iVal);
                            pJump->SetTotalFrames(iVal);
                            break;
                        }
                        case CAnmAction::TYPE_BEZIER_MOVE:
                        {
                            CAnmBezierMove *pBezierMove = dynamic_cast<CAnmBezierMove*>(pAction);
                            pItem->QueryUnsignedAttribute("bezierID", &uiVal);
                            pBezierMove->SetBezierID(uiVal);
                            pItem->QueryBoolAttribute("closeTerrain", &bVal);
                            pBezierMove->SetCloseTerrain(bVal);
                            break;
                        }
                        case CAnmAction::TYPE_MOTION:
                        {
                            CAnmMotion *pMotion = dynamic_cast<CAnmMotion*>(pAction);
                            pItem->QueryUnsignedAttribute("motionType", &uiVal);
                            pMotion->SetMotionType(CAnmMotion::MotionType(uiVal));
                            pItem->QueryStringAttribute("actionName", &text);
                            pMotion->SetActionName(AS2WC(text.c_str()));
                            pItem->QueryFloatAttribute("speed", &fVal);
                            pMotion->SetActionSpeed(fVal);
                            pItem->QueryBoolAttribute("showGFX", &bVal);
                            pMotion->SetShowGFX(bVal);
                            pItem->QueryIntAttribute("startFrame", &iVal);
                            pMotion->SetStartMotionFrame(iVal);
                            pItem->QueryIntAttribute("blendFrames", &iVal);
                            pMotion->SetBlendFrames(iVal);
                            pItem->QueryIntAttribute("totalFrames", &iVal);
                            pMotion->SetTotalFrames(iVal);
                            break;
                        }
                        case CAnmAction::TYPE_DISPLAY_HIDE:
                        {
                            CAnmDisplayHide *pDisplayHide = dynamic_cast<CAnmDisplayHide*>(pAction);
                            pItem->QueryUnsignedAttribute("visable", &uiVal);
                            pDisplayHide->SetVisableType(CAnmDisplayHide::VisableType(uiVal));
                            break;
                        }
                        case CAnmAction::TYPE_DIALOGUE:
                        {
                            CAnmDialogue *pDialogue = dynamic_cast<CAnmDialogue*>(pAction);
                            pItem->QueryStringAttribute("dialogue", &text);
                            pDialogue->SetDialogue(AS2WC(text.c_str()));
                            pItem->QueryIntAttribute("totalFrames", &iVal);
                            pDialogue->SetTotalFrames(iVal);
                            break;
                        }
                        case CAnmAction::TYPE_SOUND:
                        {
                            CAnmSound *pSound = dynamic_cast<CAnmSound*>(pAction);
                            pItem->QueryIntAttribute("startSeconds", &iVal);
                            pSound->SetStartSeconds(iVal);
                            pItem->QueryUnsignedAttribute("volume", &uiVal);
                            pSound->SetVolume(uiVal);
                            pItem->QueryStringAttribute("soundFileName", &text);
                            pSound->SetSoundFileName(AS2WC(text.c_str()));
                            pItem->QueryIntAttribute("totalFrames", &iVal);
                            pSound->SetTotalFrames(iVal);
                            break;
                        }
                        case CAnmAction::TYPE_HOOK:
                        {
                            CAnmHook *pHook = dynamic_cast<CAnmHook*>(pAction);
                            pItem->QueryStringAttribute("hangerName", &text);
                            pHook->SetHangerName(AS2WC(text.c_str()));
                            pItem->QueryStringAttribute("hookName", &text);
                            pHook->SetHookName(AS2WC(text.c_str()));
                            pItem->QueryStringAttribute("elseHookName", &text);
                            pHook->SetElseHookName(AS2WC(text.c_str()));
                            pItem->QueryUnsignedAttribute("hookObjectID", &uiVal);
                            pHook->SetHookObjectID(uiVal);
                            pItem->QueryIntAttribute("totalFrames", &iVal);
                            pHook->SetTotalFrames(iVal);
                            break;
                        }
                        case CAnmAction::TYPE_SPEED_CONTROL:
                        {
                            CAnmSpeedControl *pSpeed = dynamic_cast<CAnmSpeedControl*>(pAction);
                            pItem->QueryFloatAttribute("speed", &fVal);
                            pSpeed->SetSpeed(fVal);
                            pItem->QueryIntAttribute("totalFrames", &iVal);
                            pSpeed->SetTotalFrames(iVal);
                            break;
                        }
                        case CAnmAction::TYPE_SCALE:
                        {
                            CAnmScale *pScale = dynamic_cast<CAnmScale*>(pAction);
                            pItem->QueryFloatAttribute("startScale", &fVal);
                            pScale->SetStartScale(fVal);
                            pItem->QueryFloatAttribute("endScale", &fVal);
                            pScale->SetEndScale(fVal);
                            pItem->QueryIntAttribute("totalFrames", &iVal);
                            pScale->SetTotalFrames(iVal);
                            break;
                        }
                        case CAnmAction::TYPE_FOV:
                        {
                            CAnmFOV *pFOV = dynamic_cast<CAnmFOV*>(pAction);
                            pItem->QueryFloatAttribute("startFOV", &fVal);
                            pFOV->SetStartFOV(fVal);
                            pItem->QueryFloatAttribute("endFOV", &fVal);
                            pFOV->SetEndFOV(fVal);
                            pItem->QueryIntAttribute("totalFrames", &iVal);
                            pFOV->SetTotalFrames(iVal);
                            break;
                        }
						case CAnmAction::TYPE_CAMERA_BIND:
						{
							CAnmCameraBind* pBind = dynamic_cast<CAnmCameraBind*>(pAction);
							pItem->QueryUnsignedAttribute("followobj",&uiVal);
							pBind->SetFllowObj(uiVal);
							pItem->QueryIntAttribute("totalFrames",&iVal);
							pBind->SetTotalFrames(iVal);
							pItem->QueryFloatAttribute("offsetX", &vVal.x);
                            pItem->QueryFloatAttribute("offsetY", &vVal.y);
                            pItem->QueryFloatAttribute("offsetZ", &vVal.z);
							pBind->SetDistance(vVal);
							break;
						}
                        case CAnmAction::TYPE_CAMERA_FILTER:
                        {
                            CAnmCameraFilter *pFilter = dynamic_cast<CAnmCameraFilter*>(pAction);
                            pItem->QueryUnsignedAttribute("color", &uiVal);
                            pFilter->SetColor(uiVal);
                            pItem->QueryUnsignedAttribute("filterType", &uiVal);
                            pFilter->SetFilterType(uiVal);
                            pItem->QueryFloatAttribute("transparent", &fVal);
                            pFilter->SetTransparent(fVal);
                            pItem->QueryIntAttribute("totalFrames", &iVal);
                            pFilter->SetTotalFrames(iVal);
                            break;
                        }
                        case CAnmAction::TYPE_CAMERA_OSCILLATORY:
                        {
                            CAnmCameraOscillatory *pOscillatory = dynamic_cast<CAnmCameraOscillatory*>(pAction);
                            pItem->QueryUnsignedAttribute("oscillatoryType", &uiVal);
                            pOscillatory->SetOscillatoryType(uiVal);
                            pItem->QueryIntAttribute("oscillatoryAxis", &iVal);
                            pOscillatory->SetOscillatoryAxis(iVal);
                            pItem->QueryIntAttribute("attenuation", &iVal);
                            pOscillatory->SetAttenuation(iVal);
                            pItem->QueryFloatAttribute("displacement", &fVal);
                            pOscillatory->SetDisplacement(fVal);
                            pItem->QueryFloatAttribute("frequency", &fVal);
                            pOscillatory->SetFrequency(fVal);
                            pItem->QueryIntAttribute("totalFrames", &iVal);
                            pOscillatory->SetTotalFrames(iVal);
                            break;
                        }
                        case CAnmAction::TYPE_TRANSPARENT:
                        {
                            CAnmTransparent *pTransparent = dynamic_cast<CAnmTransparent*>(pAction);
                            pItem->QueryFloatAttribute("startTransparent", &fVal);
                            pTransparent->SetStartTransparent(fVal);
                            pItem->QueryFloatAttribute("endTransparent", &fVal);
                            pTransparent->SetEndTransparent(fVal);
                            pItem->QueryIntAttribute("totalFrames", &iVal);
                            pTransparent->SetTotalFrames(iVal);
                            break;
                        }
                        case CAnmAction::TYPE_HIDE_WORLD_ACTION:
                        {
                            CAnmHideWorldAction *pHideWorldAction = dynamic_cast<CAnmHideWorldAction*>(pAction);
                            pItem->QueryUnsignedAttribute("hideWorldType", &uiVal);
                            pHideWorldAction->SetHideWorldType(uiVal);
                            pItem->QueryUnsignedAttribute("color", &uiVal);
                            pHideWorldAction->SetColor(uiVal);
                            pItem->QueryIntAttribute("totalFrames", &iVal);
                            pHideWorldAction->SetTotalFrames(iVal);
                            break;
                        }
                        default: break;
                    }
                    return pAction;
                }
            }
        }
    }
    return 0;
}

void CTrackPanel::CopyChannel(CAnmActionChannel *pChannel)
{
    if (pChannel)
    {
        if (m_pTempChannel)
        {
            delete m_pTempChannel;
            m_pTempChannel = 0;
        }
        m_pTempChannel = new CAnmActionChannel(0);
        m_pTempChannel->CopyDataFrom(pChannel);
    }
}

void CTrackPanel::PasteChannel()
{
    if (m_pSelectGroup && m_pTempChannel && m_pSelectGroup->GetGroup())
    {
        CAnmActionGroup *pGroup = m_pSelectGroup->GetGroup();
        if (pGroup)
        {
            unsigned int uiID = pGroup->CreateChannel(CAnmActionGroup::TYPE_NPC);
            CAnmActionChannel *pChannel = pGroup->GetChannel(uiID);
            pChannel->CopyDataFrom(m_pTempChannel);
            pChannel->SetChannelName(m_pTempChannel->GetChannelName() + _T("_Copy"));
            UpdatePanel();
        }
    }
}

BEGIN_MESSAGE_MAP(CTrackPanel, CFormView)
	//{{AFX_MSG_MAP(CTrackPanel)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_CLOSE()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON_NEW_GROUP, OnButtonNewGroup)
    ON_BN_CLICKED(IDC_BUTTON_NEW_CHANNEL, OnButtonNewChannel)
	ON_BN_CLICKED(IDC_BUTTON_SET_TIME, OnButtonSetFrames)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_IMPORT_ACTION_GROUP, OnImportGroup)
	ON_COMMAND(ID_EXPORT_ACTION_GROUP, OnExportGroup)
	ON_COMMAND(ID_ANIMATION_START_TIME, OnAnimationStartTime)
	ON_UPDATE_COMMAND_UI(ID_ANIMATION_START_TIME, OnUpdateAnimationStartTime)
	ON_COMMAND(ID_ANIMATION_NOW_TIME, OnAnimationNowTime)
	ON_UPDATE_COMMAND_UI(ID_ANIMATION_NOW_TIME, OnUpdateAnimationNowTime)
	ON_COMMAND(ID_ANIMATION_END_TIME, OnAnimationEndTime)
	ON_UPDATE_COMMAND_UI(ID_ANIMATION_END_TIME, OnUpdateAnimationEndTime)
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_ACTION_COPY, OnActionCopy)
	ON_UPDATE_COMMAND_UI(ID_ACTION_COPY, OnUpdateActionCopy)
	ON_COMMAND(ID_ACTION_DELETE, OnActionDelete)
	ON_UPDATE_COMMAND_UI(ID_ACTION_DELETE, OnUpdateActionDelete)
	ON_COMMAND(ID_ACTION_PASTE, OnActionPaste)
	ON_UPDATE_COMMAND_UI(ID_ACTION_PASTE, OnUpdateActionPaste)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_MOVE_ACTIONS, OnMoveActions)
	ON_UPDATE_COMMAND_UI(ID_MOVE_ACTIONS, OnUpdateMoveActions)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_CHANNEL_COPY, OnChannelCopy)
	ON_UPDATE_COMMAND_UI(ID_CHANNEL_COPY, OnUpdateChannelCopy)
	ON_COMMAND(ID_CHANNEL_PASTE, OnChannelPaste)
	ON_UPDATE_COMMAND_UI(ID_CHANNEL_PASTE, OnUpdateChannelPaste)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrackPanel diagnostics

#ifdef _DEBUG
void CTrackPanel::AssertValid() const
{
	CFormView::AssertValid();
}

void CTrackPanel::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTrackPanel message handlers

void CTrackPanel::Draw(CDC* pDC)
{
    int pos = m_VScrollBar.GetScrollPos();
    int height = 20*(m_iRow-pos)+30;
	// TODO: Add your specialized code here and/or call the base class

    // Draw ruler
    m_TrackRuler.OnDraw(pDC);

    // Draw time mark
    DWORD dwStartTime = g_Animator.GetStartTime();
    DWORD dwNowTime = g_Animator.GetNowTime();
    DWORD dwEndTime = g_Animator.GetEndTime();
    int scrollPos = 10 * m_HScrollBar.GetScrollPos();

    int timePos = 120 + (dwStartTime / 5) - scrollPos;
    pDC->FillSolidRect(timePos, 0, 2, height, 0x00A000);

    timePos = 120 + (dwNowTime / 5) - scrollPos;
    pDC->FillSolidRect(timePos, 0, 11, height, 0x0000A0);
    pDC->SetTextColor(0xFFFFFF);
    CString text;
    text.Format(_T("%.0f"), 1 + dwNowTime * 0.02f);
    pDC->TextOut(timePos, 0, text);

    timePos = 120 + (dwEndTime / 5) - scrollPos;
    pDC->FillSolidRect(timePos, 0, 2, height, 0xE00000);

    // Draw background
    pDC->FillSolidRect(0, 0, 120, height, 0x303030);
    pDC->FillSolidRect(0, 0, 120, 30, 0x000000);
    pDC->FillSolidRect(110, 0, 8, height, 0x808080);

    // Draw groups
    height = 30;
    int row = 0;
    for (unsigned int i=0; i<m_vGroupList.size(); ++i)
    {
        m_vGroupList[i]->SetPos(0, height+20*(row-pos));
        m_vGroupList[i]->OnDraw(pDC);
        if (m_vGroupList[i]->IsFold())
        {
            ++row;
        }
        else
        {
            row += m_vGroupList[i]->GetChannelAmount() + 1;
        }
    }

	if(m_bSelectedStart)
	{
	   CRect rect;
	   UpdateSelectedRect(rect);
	   //pDC->FillSolidRect(&rect,0x000077);
	   CPen NewPen(PS_SOLID,2,RGB(255,0,0));
	   CPen* oldPen = pDC->SelectObject(&NewPen);
	   CBrush* oldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
	   pDC->Rectangle(&rect);
	   pDC->SelectObject(oldPen);
	   pDC->SelectObject(oldBrush);
	   NewPen.DeleteObject();
	}
}

void CTrackPanel::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
    int scrollPos = pScrollBar->GetScrollPos();
    switch(nSBCode)
    {
        case SB_LINELEFT:
            --scrollPos;
            break;
        case SB_PAGELEFT:
            scrollPos-= 10;
            break;
        case SB_LINERIGHT:
            ++scrollPos;
            break;
        case SB_PAGERIGHT:
            scrollPos+= 10;
            break;
        case SB_THUMBTRACK:
            SCROLLINFO info;
            pScrollBar->GetScrollInfo(&info, SIF_TRACKPOS);
            scrollPos= info.nTrackPos;
            break;
        default:
            return;
    }
    
    /*if (scrollPos < 0)
    {
        scrollPos = 0;
    }
    else if (scrollPos > 20*m_iSeconds)
    {
        scrollPos = 20*m_iSeconds;
    }
    pScrollBar->SetScrollPos(scrollPos);
    m_TrackRuler.OnHScroll(scrollPos);
    for (unsigned int i=0; i<m_vGroupList.size(); ++i)
    {
        m_vGroupList[i]->OnHScroll(scrollPos);
    }
	InvalidateRect(NULL);*/
	SetHScrollPos(pScrollBar,scrollPos);
	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CTrackPanel::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
    int scrollPos = pScrollBar->GetScrollPos();
    switch(nSBCode)
    {
    case SB_LINELEFT:
        --scrollPos;
        break;
    case SB_PAGELEFT:
        scrollPos-= 10;
        break;
    case SB_LINERIGHT:
        ++scrollPos;
        break;
    case SB_PAGERIGHT:
        scrollPos+= 10;
        break;
    case SB_THUMBTRACK:
        SCROLLINFO info;
        pScrollBar->GetScrollInfo(&info, SIF_TRACKPOS);
        scrollPos= info.nTrackPos;
        break;
    default:
        return;
    }
    
    if (scrollPos < 0)
    {
        scrollPos = 0;
    }
    else if (scrollPos > 20*m_iSeconds)
    {
        scrollPos = 20*m_iSeconds;
    }
    pScrollBar->SetScrollPos(scrollPos);
    
	InvalidateRect(NULL);
	CFormView::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CTrackPanel::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
    CRect rect;
    GetClientRect(&rect);
    m_HScrollBar.SetWindowPos(this, 120, rect.Height()-15, rect.Width()-120, 15, SWP_NOZORDER | SWP_NOACTIVATE);
    m_VScrollBar.SetWindowPos(this, rect.Width()-15, 30, 15, rect.Height()-45, SWP_NOZORDER | SWP_NOACTIVATE);
}

BOOL CTrackPanel::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

int CTrackPanel::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    if (CFormView::OnCreate(lpCreateStruct) == -1)
        return -1;
    
    // TODO: Add your specialized creation code here
    CRect rect;
    GetClientRect(&rect);
    m_HScrollBar.Create(SBS_HORZ | SBS_BOTTOMALIGN | WS_CHILD | WS_VISIBLE,
        rect, this, IDC_HSCROLLBAR);
    m_VScrollBar.Create(SBS_VERT | SBS_RIGHTALIGN | WS_CHILD | WS_VISIBLE,
        rect, this, IDC_VSCROLLBAR);

    CClientDC dc(this);
    m_dcWnd.CreateCompatibleDC(&dc);
    m_bmpWnd.CreateCompatibleBitmap(&dc, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	m_pbmpOld = m_dcWnd.SelectObject(&m_bmpWnd);

    SetSeconds(8);
    m_TrackRuler.SetPos(120, 0);
    return 0;
}

void CTrackPanel::OnMouseMove(UINT nFlags, CPoint point) 
{
    // TODO: Add your message handler code here and/or call default
    if (m_bButtonDown && g_Animator.GetState() != CAnimator::STATE_PLAYING)
    {
        if (m_TrackRuler.IsPressed())
        {
            int iTime = m_TrackRuler.GetTimePos(point.x);
            if (iTime >= 0 && g_Animator.GetNowTime() != DWORD(iTime))
            {
                CWaitCursor cursor;
                g_Animator.JumpTo(DWORD(iTime));
                g_AnmSystem.m_fPlayedTime = 0.0f;
                InvalidateRect(NULL);
            }
        }
        else if (m_pSelectChannel)
        {
            if (m_pSelectNode && m_pSelectNode->GetActionID() != -1)
            {
                if (m_pSelectChannel->OnLButtonDown(point.x, point.y))
                {
                    CTrackNode *pNode = m_pSelectChannel->GetSelectNode();
                    if (pNode && pNode->GetActionID() == -1)
                    {
						if(m_vSelNodes.size())
						   MoveMutiSelectedNodes(pNode);
						else
						{
							CAnmAction *pAction = m_pSelectNode->GetAction();
							pAction->SetStartTime(pNode->GetStartTime());
							pNode->SetAction(pAction);
							m_pSelectNode->SetAction(0);
							m_pSelectNode = pNode;
							InvalidateRect(NULL);
							g_Animator.Stop();
							theApp.OnChanged();
						}
                    }
                }
            }
            else if (!m_pSelectNode)
            {
                if (m_pSelectGroup->OnLButtonDown(point.x, point.y))
                {
                    CTrackChannel *pChannel = m_pSelectGroup->GetSelectChannel();
                    if (pChannel && m_pSelectChannel->GetChannelID() != pChannel->GetChannelID())
                    {
                        CAnmActionChannel *pActionChannel = m_pSelectChannel->GetChannel();
                        m_pSelectChannel->SetChannel(pChannel->GetChannel());
                        pChannel->SetChannel(pActionChannel);
                        m_pSelectGroup->GetGroup()->SwapChannels(m_pSelectChannel->GetChannelID(), pChannel->GetChannelID());
                        m_pSelectChannel = pChannel;
                        InvalidateRect(NULL);
                        g_Animator.Stop();
                        theApp.OnChanged();
                    }
                }
                else
                {
                    CTrackGroup *pGroup = 0;
                    for (unsigned int i=0; i<m_vGroupList.size(); ++i)
                    {
                        if (m_vGroupList[i]->OnLButtonDown(point.x, point.y))
                        {
                            pGroup = m_vGroupList[i];
                        }
                    }
                    if (pGroup)
                    {
                        m_pSelectGroup->RemoveChannel(m_pSelectChannel->GetChannelID());
                        pGroup->AddChannel(m_pSelectChannel);
                        if (pGroup->IsFold())
                        {
                            SetRow(m_iRow - 1);
                        }
                        pGroup->OnLButtonDown(0, 0);
                        m_pSelectGroup = 0;
                        m_pSelectChannel = 0;
                        InvalidateRect(NULL);
                        g_Animator.Stop();
                        theApp.OnChanged();
                    }
                }
            }
        }
    }


	if(!(GetAsyncKeyState(VK_SHIFT) & 0x8000))
	{
		if(m_bSelectedStart)
		{
           m_bSelectedStart = false;
		   InvalidateRect(NULL);
		}
	}
	if(m_bSelectedStart)
	{
		m_rectSelected.right = point.x;
		m_rectSelected.bottom = point.y;
		InvalidateRect(NULL);	
	}
	
    CFormView::OnMouseMove(nFlags, point);
}

void CTrackPanel::OnLButtonDown(UINT nFlags, CPoint point) 
{
    // TODO: Add your message handler code here and/or call default
    m_bButtonDown = true;
	bool resetSelected = true;
	bool resetRectSelected = true;

    if (m_TrackRuler.OnLButtonDown(point.x, point.y) && g_Animator.GetState() != CAnimator::STATE_PLAYING)
    {
        int iTime = m_TrackRuler.GetTimePos(point.x);
        //if (iTime >= 0 && g_Animator.GetNowTime() != DWORD(iTime))
        //{
          //  CWaitCursor cursor;
          //  g_Animator.JumpTo(DWORD(iTime));
          //  g_AnmSystem.m_fPlayedTime = 0;
        //}
		JumpToFrame((DWORD)iTime);
    }
    else
    {
        m_pSelectGroup = 0;
        m_pSelectChannel = 0;
        for (unsigned int i=0; i<m_vGroupList.size(); ++i)
        {
            if (m_vGroupList[i]->OnLButtonDown(point.x, point.y))
            {
                m_pSelectGroup = m_vGroupList[i];
            }
        }
        if (m_pSelectGroup)
        {
            m_pSelectChannel = m_pSelectGroup->GetSelectChannel();
            if (m_pSelectChannel)
            {
                m_pSelectNode = m_pSelectChannel->GetSelectNode();
                g_Animator.GetObjectManager()->SelectObject(m_pSelectChannel->GetObjectID());
                if (m_pSelectNode)
                {
                    CAnmAction *pAction = m_pSelectNode->GetAction();
                    if (pAction && pAction->GetActionType() == CAnmAction::TYPE_BEZIER_MOVE)
                    {
                        CAnmBezierMove *pBezierMove = dynamic_cast<CAnmBezierMove*>(pAction);
                        g_Animator.GetBezierManager()->SelectBezier(pBezierMove->GetBezierID());
                    }

					if((GetAsyncKeyState(VK_CONTROL) & 0x8000))
					{ 
						resetSelected = false;
						if(pAction)
						{
					       UpdateSelectedNodes(m_pSelectNode);
						}
						SetMutiNodesSelected(true);
					}

					if ((GetAsyncKeyState(VK_SHIFT) & 0x8000))
					{
						if(m_bSelectedStart)
						{
						   m_bSelectedStart = false;
						   m_rectSelected.right = point.x;
						   m_rectSelected.bottom = point.y;
						   CRect rect;
						   UpdateSelectedRect(rect);
						   SelectNodesByRect(rect);
						   SetMutiNodesSelected(true);

						}
						else
						{
							m_bSelectedStart = true;
							resetRectSelected = false;
				            m_rectSelected.left = point.x;
							m_rectSelected.top  = point.y;
							m_rectSelected.right = point.x;
							m_rectSelected.bottom = point.y;
							
						}
						resetSelected = false;
						SetMutiNodesSelected(true);
					}
                 
                }
            }
            else
            {
                if  (point.x < 120)
                {
                    if (m_pSelectGroup->IsFold())
                    {
                        SetRow(m_iRow + m_pSelectGroup->GetChannelAmount());
                        m_pSelectGroup->SetFold(false);
                    }
                    else
                    {
                        SetRow(m_iRow - m_pSelectGroup->GetChannelAmount());
                        m_pSelectGroup->SetFold(true);
                    }
                }
            }
        }
    }
	if(resetSelected)
	{
		SetMutiNodesSelected(false);
		m_vSelNodes.clear();
	}

	if(resetRectSelected)
	{
		m_bSelectedStart = false;
	}
    InvalidateRect(NULL);
    CFormView::OnLButtonDown(nFlags, point);
    theApp.UpdateList();
}

void CTrackPanel::OnLButtonUp(UINT nFlags, CPoint point) 
{
    // TODO: Add your message handler code here and/or call default
    m_bButtonDown = false;
    m_TrackRuler.SetPressed(false);
    CFormView::OnLButtonUp(nFlags, point);
}

void CTrackPanel::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SetMutiNodesSelected(false);
	m_vSelNodes.clear();
    if (g_Animator.GetState() != CAnimator::STATE_PLAYING)
    {
        if (!m_TrackRuler.OnLButtonDown(point.x, point.y) && m_pSelectGroup)
        {
            if (!m_pSelectChannel)
            {
                if (m_pSelectGroup->IsFold())
                {
                    SetRow(m_iRow + m_pSelectGroup->GetChannelAmount());
                    m_pSelectGroup->SetFold(false);
                }
                CTrackGroupDlg dlg(this, this, m_pSelectGroup);
                dlg.DoModal();
            }
            else
            {
                if (!m_pSelectNode)
                {
                    CTrackChannelDlg dlg(this, m_pSelectGroup, m_pSelectChannel);
                    if (dlg.DoModal() != 0)
                    {
                        SetRow(m_iRow - 1);
                    }
                }
                else
                {
                    if (m_pSelectNode->GetActionID() == -1)
                    {
                        CNewActionDlg dlg(this, m_pSelectChannel, m_pSelectNode);
                        dlg.DoModal();
                    }
                    else
                    {
                        if (m_pSelectNode->GetAction()->GetActionType() == CAnmAction::TYPE_MOVE)
                        {
                            CActionMoveDlg dlg(this, m_pSelectChannel, m_pSelectNode);
                            dlg.DoModal();
                        }
                        else if (m_pSelectNode->GetAction()->GetActionType() == CAnmAction::TYPE_JUMP)
                        {
                            CActionJumpDlg dlg(this, m_pSelectChannel, m_pSelectNode);
                            dlg.DoModal();
                        }
                        else if (m_pSelectNode->GetAction()->GetActionType() == CAnmAction::TYPE_BEZIER_MOVE)
                        {
                            CActionBezierMoveDlg dlg(this, m_pSelectChannel, m_pSelectNode);
                            dlg.DoModal();
                        }
                        else if (m_pSelectNode->GetAction()->GetActionType() == CAnmAction::TYPE_MOTION)
                        {
                            CActionMotionDlg dlg(this, m_pSelectChannel, m_pSelectNode);
                            dlg.DoModal();
                        }
                        else if (m_pSelectNode->GetAction()->GetActionType() == CAnmAction::TYPE_DISPLAY_HIDE)
                        {
                            CActionDisplayHideDlg dlg(this, m_pSelectChannel, m_pSelectNode);
                            dlg.DoModal();
                        }
                        else if (m_pSelectNode->GetAction()->GetActionType() == CAnmAction::TYPE_DIALOGUE)
                        {
                            CActionDialogueDlg dlg(this, m_pSelectChannel, m_pSelectNode);
                            dlg.DoModal();
                        }
                        else if (m_pSelectNode->GetAction()->GetActionType() == CAnmAction::TYPE_SOUND)
                        {
                            CActionSoundDlg dlg(this, m_pSelectChannel, m_pSelectNode);
                            dlg.DoModal();
                        }
                        else if (m_pSelectNode->GetAction()->GetActionType() == CAnmAction::TYPE_HOOK)
                        {
                            CActionHookDlg dlg(this, m_pSelectChannel, m_pSelectNode);
                            dlg.DoModal();
                        }
                        else if (m_pSelectNode->GetAction()->GetActionType() == CAnmAction::TYPE_SPEED_CONTROL)
                        {
                            CActionSpeedControlDlg dlg(this, m_pSelectChannel, m_pSelectNode);
                            dlg.DoModal();
                        }
                        else if (m_pSelectNode->GetAction()->GetActionType() == CAnmAction::TYPE_ROTATE)
                        {
                            CActionRotateDlg dlg(this, m_pSelectChannel, m_pSelectNode);
                            dlg.DoModal();
                        }
                        else if (m_pSelectNode->GetAction()->GetActionType() == CAnmAction::TYPE_SCALE)
                        {
                            CActionScaleDlg dlg(this, m_pSelectChannel, m_pSelectNode);
                            dlg.DoModal();
                        }
                        else if (m_pSelectNode->GetAction()->GetActionType() == CAnmAction::TYPE_FOV)
                        {
                            CActionFOVDlg dlg(this, m_pSelectChannel, m_pSelectNode);
                            dlg.DoModal();
                        }
						else if(m_pSelectNode->GetAction()->GetActionType() == CAnmAction::TYPE_CAMERA_BIND)
						{
							  CActionCameraBindDlg dlg(this,m_pSelectChannel,m_pSelectNode);
							  dlg.DoModal();
						}
                        else if (m_pSelectNode->GetAction()->GetActionType() == CAnmAction::TYPE_CAMERA_FILTER)
                        {
                            CActionCameraFilterDlg dlg(this, m_pSelectChannel, m_pSelectNode);
                            dlg.DoModal();
                        }
                        else if (m_pSelectNode->GetAction()->GetActionType() == CAnmAction::TYPE_CAMERA_OSCILLATORY)
                        {
                            CActionCameraOscillatory dlg(this, m_pSelectChannel, m_pSelectNode);
                            dlg.DoModal();
                        }
                        else if (m_pSelectNode->GetAction()->GetActionType() == CAnmAction::TYPE_TRANSPARENT)
                        {
                            CActionTransparentDlg dlg(this, m_pSelectChannel, m_pSelectNode);
                            dlg.DoModal();
                        }
                        else if (m_pSelectNode->GetAction()->GetActionType() == CAnmAction::TYPE_HIDE_WORLD_ACTION)
                        {
                            CActionHideWorldDlg dlg(this, m_pSelectChannel, m_pSelectNode);
                            dlg.DoModal();
                        }
                    }
                }
            }
            g_Animator.Stop();
            theApp.OnChanged();
        }
    }

    InvalidateRect(NULL);
	CFormView::OnLButtonDblClk(nFlags, point);
}

void CTrackPanel::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
    //CreateGroup();
    CFormView::OnRButtonDown(nFlags, point);
}

void CTrackPanel::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
    //CreateChannel();
	CFormView::OnRButtonUp(nFlags, point);
}

void CTrackPanel::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CFormView::OnClose();
}

void CTrackPanel::OnImportGroup()
{
    CFileDialog dlg(TRUE, NULL, NULL,
        OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR,
        _T("Animation group files(*.anmgroup)|*.anmgroup||"), NULL);
    CString strDirPath;
    strDirPath.Format(_T("%s"), _T("videos\\animations"));
    dlg.m_ofn.lpstrInitialDir = strDirPath;
    if (dlg.DoModal()!=IDOK)
        return;
    CString m_sFileName = dlg.GetPathName();

    CWaitCursor cursor;

    g_Animator.Stop();
    g_Animator.ImportActionGroup(m_sFileName);
    UpdatePanel();
}

void CTrackPanel::OnExportGroup()
{
    if (m_pSelectGroup && m_pSelectGroup->GetGroup())
    {
        CFileDialog dlg(FALSE, NULL, NULL,
            OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR,
            _T("Animation files(*.anmgroup)|*.anmgroup||"), NULL);
        CString strDirPath;
        strDirPath.Format(_T("%s"), _T("videos\\animations"));
        dlg.m_ofn.lpstrInitialDir = strDirPath;
        if (dlg.DoModal()!=IDOK)
            return;
        CWaitCursor cursor;
        CString m_sFileName = dlg.GetPathName() + _T(".anmgroup");
        g_Animator.ExportActionGroup(m_sFileName, m_pSelectGroup->GetGroup());
        return;
    }
    MessageBox(_T("导出事件组失败!（没有选定的事件组）"), _T("动画编辑器"), MB_ICONEXCLAMATION | MB_OK);
}

void CTrackPanel::OnButtonNewGroup() 
{
	// TODO: Add your control notification handler code here
	CreateGroup();
    g_Animator.Stop();
    theApp.OnChanged();
}

void CTrackPanel::OnButtonNewChannel() 
{
    // TODO: Add your control notification handler code here
    CreateChannel();
    g_Animator.Stop();
    theApp.OnChanged();
}

void CTrackPanel::OnButtonSetFrames() 
{
	// TODO: Add your control notification handler code here
    CSetTimeDlg dlg(this);
    dlg.DoModal();
    InvalidateRect(NULL);
}

void CTrackPanel::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

    CRect rc;
    GetClientRect(&rc);
    m_dcWnd.FillSolidRect(0, 0, rc.Width(), rc.Height(), 0xFFFFFF);
    
    Draw(&m_dcWnd);
    
    dc.BitBlt(0, 0, rc.Width(), rc.Height(),
              &m_dcWnd, 0, 0, SRCCOPY);
	// Do not call CFormView::OnPaint() for painting messages
}

void CTrackPanel::OnDestroy() 
{
	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
    m_dcWnd.SelectObject(m_pbmpOld);
    m_dcWnd.DeleteDC();
    m_bmpWnd.DeleteObject();
}

void CTrackPanel::OnAnimationStartTime() 
{
	// TODO: Add your command handler code here
    CTrackNode *pNode = GetSelectedNode();
    if (pNode)
    {
        DWORD dwTime = pNode->GetStartTime();
        g_Animator.SetStartTime(dwTime);
        InvalidateRect(NULL);
    }
}

void CTrackPanel::OnUpdateAnimationStartTime(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (GetSelectedNode())
    {
        pCmdUI->Enable(true);
    }
    else
    {
        pCmdUI->Enable(false);
    }
}

void CTrackPanel::OnAnimationNowTime() 
{
	// TODO: Add your command handler code here
	CTrackNode *pNode = GetSelectedNode();
    if (pNode)
    {
        DWORD dwTime = pNode->GetStartTime();
        g_Animator.Stop();
        g_Animator.Play();
        g_Animator.Tick(dwTime);
        g_Animator.Pause();
        InvalidateRect(NULL);
    }
}

void CTrackPanel::OnUpdateAnimationNowTime(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (GetSelectedNode())
    {
        pCmdUI->Enable(true);
    }
    else
    {
        pCmdUI->Enable(false);
    }
}

void CTrackPanel::OnAnimationEndTime() 
{
	// TODO: Add your command handler code here
    CTrackNode *pNode = GetSelectedNode();
    if (pNode)
    {
        DWORD dwTime = pNode->GetStartTime();
        g_Animator.SetEndTime(dwTime+CAnmAction::TIME_PER_FRAME);
        InvalidateRect(NULL);
    }
}

void CTrackPanel::OnUpdateAnimationEndTime(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (GetSelectedNode())
    {
        pCmdUI->Enable(true);
    }
    else
    {
        pCmdUI->Enable(false);
    }
}

BOOL CTrackPanel::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
    if (zDelta < 0)
    {
        //g_Animator.MoveBack(0.01f * float(-zDelta));
        OnVScroll(SB_LINERIGHT, 0, &m_VScrollBar);
    }
    else
    {
        //g_Animator.MoveFront(0.01f * float(zDelta));
        OnVScroll(SB_LINELEFT, 0, &m_VScrollBar);
    }
	return CFormView::OnMouseWheel(nFlags, zDelta, pt);
}

void CTrackPanel::OnActionCopy() 
{
	// TODO: Add your command handler code here
    CTrackNode *pNode = GetSelectedNode();
    if (pNode)
    {
        CAnmAction *pAction = pNode->GetAction();
        if (pAction)
        {
            CopyAction(pAction);
        }
    }
}

void CTrackPanel::OnUpdateActionCopy(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CTrackNode *pNode = GetSelectedNode();
    if (pNode)
    {
        CAnmAction *pAction = pNode->GetAction();
        if (pAction)
        {
            pCmdUI->Enable(true);
            return;
        }
    }
    pCmdUI->Enable(false);
}

void CTrackPanel::OnActionDelete() 
{
	// TODO: Add your command handler code here
    CTrackNode *pNode = GetSelectedNode();
    if (pNode)
    {
        CAnmAction *pAction = pNode->GetAction();
        if (pAction)
        {
            DeleteAction();
        }
    }
}

void CTrackPanel::OnUpdateActionDelete(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (g_Animator.GetState() == CAnimator::STATE_PLAYING)
    {
        pCmdUI->Enable(false);
        return;
    }
    CTrackNode *pNode = GetSelectedNode();
    if (pNode)
    {
        CAnmAction *pAction = pNode->GetAction();
        if (pAction)
        {
            pCmdUI->Enable(true);
            return;
        }
    }
    pCmdUI->Enable(false);
}

void CTrackPanel::OnActionPaste() 
{
    CTrackNode *pNode = GetSelectedNode();
    if (pNode)
    {
        CAnmAction *pAction = pNode->GetAction();
        if (!pAction)
        {
            PasteAction();
        }
    }
}

void CTrackPanel::OnUpdateActionPaste(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (g_Animator.GetState() == CAnimator::STATE_PLAYING)
    {
        pCmdUI->Enable(false);
        return;
    }
    CTrackNode *pNode = GetSelectedNode();
    if (pNode)
    {
        CAnmAction *pAction = pNode->GetAction();
        if (!pAction)
        {
            pCmdUI->Enable(true);
            return;
        }
    }
    pCmdUI->Enable(false);
}

void CTrackPanel::OnEditCopy()
{
	// TODO: Add your command handler code here
	OnActionCopy();
}

void CTrackPanel::OnEditPaste()
{
	// TODO: Add your command handler code here
	OnActionPaste();
}

void CTrackPanel::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (g_Animator.GetState() == CAnimator::STATE_PLAYING)
    {
        pCmdUI->Enable(false);
        return;
    }
    CTrackNode *pNode = GetSelectedNode();
    if (pNode)
    {
        CAnmAction *pAction = pNode->GetAction();
        if (pAction)
        {
            pCmdUI->Enable(true);
            return;
        }
    }
    pCmdUI->Enable(false);
}

void CTrackPanel::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (g_Animator.GetState() == CAnimator::STATE_PLAYING)
    {
        pCmdUI->Enable(false);
        return;
    }
    if (g_Animator.GetState() == CAnimator::STATE_PLAYING)
    {
        pCmdUI->Enable(false);
        return;
    }
    CTrackNode *pNode = GetSelectedNode();
    if (pNode)
    {
        CAnmAction *pAction = pNode->GetAction();
        if (!pAction)
        {
            pCmdUI->Enable(true);
            return;
        }
    }
    pCmdUI->Enable(false);
}

void CTrackPanel::OnMoveActions()
{
	// TODO: Add your command handler code here
    CMoveActionsDlg dlg(this, m_pSelectGroup);
    if (dlg.DoModal() != 0)
    {
        g_Animator.Stop();
        theApp.OnChanged();
        UpdatePanel();
    }
}

void CTrackPanel::OnUpdateMoveActions(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (g_Animator.GetState() != CAnimator::STATE_PLAYING)
    {
        pCmdUI->Enable(true);
        return;
    }
    pCmdUI->Enable(false);
}

BOOL CTrackPanel::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CTrackPanel::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CFormView::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CTrackPanel::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
    switch (pMsg->message)
    {
        case WM_KEYDOWN:
            if (pMsg->wParam == 'W' || pMsg->wParam == 'A' ||
                pMsg->wParam == 'S' || pMsg->wParam == 'D' ||
                pMsg->wParam == 'Q' || pMsg->wParam == 'E' ||
                pMsg->wParam == 'Z' || pMsg->wParam == 'X')
                return true;
            break;

        default:
            break;
    }
	return CFormView::PreTranslateMessage(pMsg);
}

void CTrackPanel::OnChannelCopy() 
{
	// TODO: Add your command handler code here
    if (m_pSelectGroup)
    {
        CTrackChannel *pTrackChannel = m_pSelectGroup->GetSelectChannel();
        if (pTrackChannel && pTrackChannel->GetChannel())
        {
            CopyChannel(pTrackChannel->GetChannel());
        }
    }
}

void CTrackPanel::OnUpdateChannelCopy(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (g_Animator.GetState() == CAnimator::STATE_PLAYING)
    {
        pCmdUI->Enable(false);
        return;
    }
    if (m_pSelectGroup)
    {
        CTrackChannel *pTrackChannel = m_pSelectGroup->GetSelectChannel();
        if (pTrackChannel && pTrackChannel->GetChannel())
        {
            pCmdUI->Enable(true);
            return;
        }
    }
    pCmdUI->Enable(false);
}

void CTrackPanel::OnChannelPaste() 
{
	// TODO: Add your command handler code here
    if (m_pSelectGroup && m_pTempChannel)
    {
        PasteChannel();
    }
}

void CTrackPanel::OnUpdateChannelPaste(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (g_Animator.GetState() == CAnimator::STATE_PLAYING)
    {
        pCmdUI->Enable(false);
        return;
    }
    if (m_pSelectGroup && m_pTempChannel)
    {
        pCmdUI->Enable(true);
        return;
    }
    pCmdUI->Enable(false);
}

void CTrackPanel::JumpToFrame(DWORD time)
{
	if(g_Animator.GetState() == CAnimator::STATE_PLAYING)
		return;
	if(time >=0 && time < g_Animator.GetTotalTime() && g_Animator.GetNowTime() != time )
	{
		CWaitCursor cursor;
		g_Animator.JumpTo(time);
		g_AnmSystem.m_fPlayedTime = 0;
		InvalidateRect(NULL);
		UpdateHScrollPos();
	}
}
void CTrackPanel::UpdateHScrollPos()
{
	int time = g_Animator.GetNowTime();
	CRect rect;
	m_HScrollBar.GetWindowRect(rect);
	int width = rect.Width();
	int start = 10*m_HScrollBar.GetScrollPos();
	int end =   start + width;
	int dest =  10*time/50;
	if(dest>end || dest < start)
	{
		SetHScrollPos(&m_HScrollBar,time/50);
	}
}
void CTrackPanel::SetHScrollPos(CScrollBar* pScrollBar,int scrollPos)
{
	if (scrollPos < 0)
    {
        scrollPos = 0;
    }
    else if (scrollPos > 20*m_iSeconds)
    {
        scrollPos = 20*m_iSeconds;
    }
    pScrollBar->SetScrollPos(scrollPos);
    m_TrackRuler.OnHScroll(scrollPos);
    for (unsigned int i=0; i<m_vGroupList.size(); ++i)
    {
        m_vGroupList[i]->OnHScroll(scrollPos);
    }
	InvalidateRect(NULL);
}

void CTrackPanel::SetMutiNodesSelected(bool isSelected)
{
   for (unsigned int i=0;i<m_vSelNodes.size();++i)
   {
	   CTrackNode* pNode = m_vSelNodes[i];
	   assert(pNode && pNode->GetAction());
	   pNode->SetSelected(isSelected);
   }
}
bool CTrackPanel::UpdateSelectedNodes(CTrackNode* pNode)
{
	std::vector<CTrackNode *>::iterator pos;
    for (pos=m_vSelNodes.begin(); pos!=m_vSelNodes.end(); ++pos)
    {
        if ((*pos) == pNode)
        {
			pNode->SetSelected(false);
            m_vSelNodes.erase(pos);
			return false;
        }
    }
	m_vSelNodes.push_back(pNode);
	return true;

}
void CTrackPanel::MoveMutiSelectedNodes(CTrackNode* pNode)
{
	std::vector<CTrackNode *>::iterator pos;
    for (pos=m_vSelNodes.begin(); pos!=m_vSelNodes.end(); ++pos)
    {
        if ((*pos) == m_pSelectNode)
        {
            break;
        }
    }
	if(pos == m_vSelNodes.end())
		m_vSelNodes.push_back(m_pSelectNode);


	int diff = (int)(pNode->GetStartTime() - m_pSelectNode->GetStartTime())/50;
	std::vector<CTrackNode*>  vecNodes;

	for (pos=m_vSelNodes.begin(); pos!=m_vSelNodes.end(); ++pos)
    {
        CTrackNode* pNode = *pos;
		CTrackChannel* pChannel = pNode->GetTrackChannel();
        int dest = pNode->GetStartTime()/50 + diff;
		if(dest < 0 || dest > pChannel->GetNodeAmount()-1) return;
		CTrackNode* pDestNode = pChannel->GetNodeAt(dest);
		if(pDestNode && pDestNode->GetAction()) return;
		vecNodes.push_back(pChannel->GetNodeAt(dest));
    }

	assert(vecNodes.size() == m_vSelNodes.size());


	for (unsigned int i = 0; i< vecNodes.size(); ++i)
	{
		CTrackNode* srcNode = m_vSelNodes[i];
		CTrackNode* destNode = vecNodes[i];
	    CAnmAction *pAction = srcNode->GetAction();
	    pAction->SetStartTime(destNode->GetStartTime());
	    destNode->SetAction(pAction);
	    srcNode->SetAction(NULL);
		srcNode->SetSelected(false);
		destNode->SetSelected(true);
	}

	m_vSelNodes.swap(vecNodes);
	m_pSelectNode = pNode;
	InvalidateRect(NULL);
	g_Animator.Stop();
	theApp.OnChanged();
}

void CTrackPanel::UpdateSelectedRect(CRect& rect)
{
	if(m_rectSelected.left > m_rectSelected.right)
	{
		rect.left = m_rectSelected.right;
		rect.right = m_rectSelected.left;
		
	}
	else
	{
		rect.left = m_rectSelected.left;
		rect.right = m_rectSelected.right;
	}
	if(m_rectSelected.top > m_rectSelected.bottom)
	{
		rect.top = m_rectSelected.bottom;
		rect.bottom = m_rectSelected.top;
	}
	else
	{
	   rect.top = m_rectSelected.top;
	   rect.bottom = m_rectSelected.bottom;
	}
}

void CTrackPanel::SelectNodesByRect(const CRect& rect)
{
	for (unsigned int i=0; i<m_vGroupList.size(); ++i)
	{
		CTrackGroup* pGroup = m_vGroupList[i];
		if(pGroup->IsFold())
			continue;
		
		for ( int j = 0; j<pGroup->GetChannelAmount();++j)
		{
			CTrackChannel* pChannel = pGroup->GetChannelAt(j);
			int scrollPos = m_HScrollBar.GetScrollPos();
			unsigned int drawNodeCount = pChannel->GetNodeAmount() - scrollPos;
			if (drawNodeCount > 200)
			{
				drawNodeCount = 200;
			}
			for ( unsigned int k = scrollPos; k< scrollPos + drawNodeCount;++k)
			{
				CTrackNode* pNode = pChannel->GetNodeAt(k);
				if(pNode && pNode->GetAction() && pNode->IsIntersectRect(rect))
				{
				    UpdateSelectedNodes(pNode);
				}
				
			}
		}
		
	}
}
