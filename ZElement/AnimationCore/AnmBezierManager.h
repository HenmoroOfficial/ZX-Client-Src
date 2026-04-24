#ifndef _ANM_BEZIER_MANAGER_H_
#define _ANM_BEZIER_MANAGER_H_

#include "AnmBezier.h"
#include <vector>

class CAnmBezierManager
{
    public:
        explicit CAnmBezierManager(A3DWireCollector *pA3DWireCollector);

        void CreateBezier(const unsigned int uiID);

        unsigned int CreateBezier();

        void CopyBezier(const unsigned int uiID);

        void RemoveBezier(const unsigned int uiID);

        CAnmBezier* GetBezier(const unsigned int uiID);

        CAnmBezier* GetSelectedBezier() const;

        void SelectBezier(const unsigned int uiID);

        void GetBezierIDList(std::vector<unsigned int> &vList);

        void SetAllVisable(const bool b);

        void Render();

        int GetBezierAmount() const;

        ~CAnmBezierManager();

    private:
        CAnmBezierManager();

        CAnmBezierManager(const CAnmBezierManager&);

        CAnmBezierManager& operator = (const CAnmBezierManager&);

    private:
        A3DWireCollector *m_pA3DWireCollector;
        std::map<unsigned int, CAnmBezier*> m_mBezierMap;
        CAnmBezier *m_pSelectedBezier;
};

#endif
