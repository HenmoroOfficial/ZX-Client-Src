#ifndef _ANM_CAMERA_BIND_H_
#define _ANM_CAMERA_BIND_H_

#include "AnmAction.h"

class CAnmCameraBind : public CAnmAction
{
public:
	
public:
	explicit CAnmCameraBind(const unsigned int uiID);

	void SetTotalFrames(const int iFrame);
	
    int GetTotalFrames() const;

	void SetFllowObj(const unsigned int iObj);

	unsigned int GetFllowObj()const;

	void SetDistance(const A3DVECTOR3& vDis);

	A3DVECTOR3 GetDistance() const;
	
	virtual ActionType GetActionType() const;
	
	virtual void CopyDataFrom(CAnmAction *pAction);
	
	virtual bool Tick(const DWORD dwNowTime, CAnmObjectManager *pObjectManager);
	
	virtual DWORD GetTotalTime() const;
	
	virtual ~CAnmCameraBind();
	
private:
	CAnmCameraBind();
	
private:
	  int m_iTotalFrames;
	  unsigned int m_iFllowObj;
	  A3DVECTOR3 m_vDistance;
	
};

#endif