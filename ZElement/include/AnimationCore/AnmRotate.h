#pragma once

#include "AnmAction.h"

class CAnmRotate : public CAnmAction
{
public:
	explicit CAnmRotate(const unsigned int uiID);

	virtual ~CAnmRotate();

	void SetStartRotate(const A3DQUATERNION &q);

	A3DQUATERNION GetStartRotate() const;

	void SetEndRotate(const A3DQUATERNION &q);

	A3DQUATERNION GetEndRotate() const;

	void SetTotalFrames(const int iFrame);

	int GetTotalFrames() const;

	virtual ActionType GetActionType() const;

	virtual void CopyDataFrom(CAnmAction *pAction);

	virtual bool Tick(const DWORD dwNowTime, CAnmObjectManager *pObjectManager);

	virtual DWORD GetTotalTime() const;

private:
	CAnmRotate();

private:
	A3DQUATERNION m_qStartRotate;
	A3DQUATERNION m_qEndRotate;
	int m_iTotalFrames;
};