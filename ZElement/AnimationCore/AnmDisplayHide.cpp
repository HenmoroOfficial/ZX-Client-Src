#include "stdafx.h"
#include "AnmDisplayHide.h"

CAnmDisplayHide::CAnmDisplayHide(const unsigned int uiID)
    : CAnmAction(uiID)
{
    m_eVisableType = VT_HIDE;
}

void CAnmDisplayHide::SetVisableType(const VisableType &vt)
{
    m_eVisableType = vt;
}

CAnmDisplayHide::VisableType CAnmDisplayHide::GetVisableType() const
{
    return m_eVisableType;
}

CAnmAction::ActionType CAnmDisplayHide::GetActionType() const
{
    return CAnmAction::TYPE_DISPLAY_HIDE;
}

void CAnmDisplayHide::CopyDataFrom(CAnmAction *pAction)
{
    CAnmDisplayHide *pDataAction = dynamic_cast<CAnmDisplayHide*>(pAction);
    if (pDataAction)
    {
        SetVisableType(pDataAction->GetVisableType());
    }
}

bool CAnmDisplayHide::Tick(const DWORD dwNowTime, CAnmObjectManager *pObjectManager)
{
    if (IsDead())
    {
        return false;
    }

    CAnmObject *pObject = pObjectManager->GetObject(m_uiObjectID);
    if (pObject)
    {
        if (dwNowTime >= m_dwStartTime)
        {
            switch (m_eVisableType)
            {
                case VT_HIDE:
                    pObject->SetVisible(false);
                    break;

                case VT_DISPLAY:
                    pObject->SetVisible(true);
                    if (pObject->GetObjectID() > 1)
                    {
                        CAnmNPC *pNPC = dynamic_cast<CAnmNPC*>(pObject);
                        if (pNPC)
                        {
                            pNPC->UseShader(0);
                        }
                    }
                    break;

                case VT_BLACK_AND_WHITE:
                {
                    if (pObject->GetObjectID() > 1)
                    {
                        CAnmNPC *pNPC = dynamic_cast<CAnmNPC*>(pObject);
                        if (pNPC)
                        {
                            pNPC->UseShader(1);
                        }
                    }
                    break;
                }

                default:
                    break;
            }
        }
    }

    if (dwNowTime >= m_dwStartTime)
    {
        m_bDead = true;
    }
    return IsDead();
}

DWORD CAnmDisplayHide::GetTotalTime() const
{
    return 0;
}

CAnmDisplayHide::~CAnmDisplayHide()
{
}
