#ifndef _ANM_OBJECT_MANAGER_H_
#define _ANM_OBJECT_MANAGER_H_

#include "AnmNPC.h"
#include "AnmGFX.h"
#include "AnmCamera.h"
#include "AnmPlayer.h"
#include <map>

class CAnmObjectManager
{
    public:
        CAnmObjectManager();

        void SetCamera(CAnmCamera *pCamera);

        void SetPlayer(CAnmPlayer *pPlayer);

        void CreateObject(const unsigned int uiID, const CAnmObject::ObjectType type);

        unsigned int CreateObject(const CAnmObject::ObjectType type);

        unsigned int CopyObject(const unsigned int uiID);

        void RemoveObject(const unsigned int uiID);

        CAnmObject* GetObject(const unsigned int uiID);

        CAnmObject* GetSelectedObject() const;

        void SelectObject(const unsigned int uiID);

        void GetObjectIDList(std::vector<unsigned int> &vList);

        int GetObjectAmount() const;

        bool Tick(const DWORD dwDeltaTime);

        bool Render(A3DViewport *viewport, bool bRenderShadow);

        void Reset();

        ~CAnmObjectManager();

    private:
        std::map<unsigned int, CAnmObject*> m_mObjectMap;
        CAnmCamera *m_pCamera;
        CAnmPlayer *m_pPlayer;
        CAnmObject *m_pSelectedObject;
};

#endif
