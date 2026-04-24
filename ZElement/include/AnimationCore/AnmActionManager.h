#pragma once

#ifndef _ANM_ACTION_MANAGER_H_
#define _ANM_ACTION_MANAGER_H_

#include <A3D.h>
#include <vector>
#include <list>
#include <map>
#include <algorithm>

class CAnmAction;
class CAnmActionGroup;
class CAnmObjectManager;

class CAnmActionManager
{
    public:
        typedef std::list<CAnmAction*> ActionList;
        typedef std::vector<CAnmAction*> ActionVector;
        typedef std::vector<CAnmAction*>::iterator ActionVectorIterator;

    private:
        struct ActionNode
        {
            CAnmAction *mpAction;
            DWORD mEndTime;
            ActionNode *mpLeftNode;
            ActionNode *mpRightNode;
        };

    public:
        CAnmActionManager();

        void CreateGroup(const unsigned int uiID);

        unsigned int CreateGroup();

        void DeleteGroup(const unsigned int uiID);

        CAnmActionGroup* GetGroup(const unsigned int uiID);

        void GetGroupIDList(std::vector<unsigned int> &vList);

        int GetGroupAmount() const;

        int GetActionsCount(const DWORD dwStartTime, const DWORD dwEndTime);

        void MoveActions(const DWORD dwStartTime, const DWORD dwEndTime, const DWORD dwOffset, const bool left=false);

        void Prepare();

        void Reset();

        float CalculateTotalTime();

        bool Tick(const DWORD dwNowTime, CAnmObjectManager *pObjectManager);

        ~CAnmActionManager();

    private:
        void _ClearActiveActionTree();

        void _RemoveActiveActionNode(ActionNode *&rpActionNode);

        void _InsertActiveAction(CAnmAction *pAction);

        bool _TickActiveActionTree(const DWORD dwNowTime, CAnmObjectManager *pObjectManager);

        bool _TickActiveActionNode(ActionNode *&rpActionNode, const DWORD dwNowTime, CAnmObjectManager *pObjectManager);
        
    public:
        static bool ActionCompareStartTime(const CAnmAction * const &a, const CAnmAction * const &b);

    private:
        std::map<unsigned int, CAnmActionGroup*> m_mGroupMap;
        ActionNode m_ActiveActionTreeRoot;
        ActionList m_lPrepareActionList;
};

#endif	// _ANM_ACTION_MANAGER_H_