/*
 * FILE: EC_Bezier.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: XinQingFeng, 2005/2/24
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#pragma	once

#include "ABaseDef.h"
#include "A3DVector.h"
#include "AString.h"
#include "..\\ZElementClient\\EC_SceneObject.h"
#include <stdio.h>

#define BEZIER_FILE_VERSION		5

class AFile;

class CECBezierPoint
{

public:
	
	CECBezierPoint(){};
	virtual ~CECBezierPoint(){};
	
public:
	
	const A3DVECTOR3& GetPos() const { return m_vPos; };
	const A3DVECTOR3& GetDir() const { return m_vDir; };
	void SetPos( const A3DVECTOR3& pos)	{ m_vPos = pos; };
	void SetDir( const A3DVECTOR3& dir) { m_vDir = dir; };

	const float GetFov(){ return m_fFov; }
	const float GetRatio(){ return m_fRatio; }
	const float GetSpeed(){ return m_fSpeed; }

	void SetFov( float fFov ){ m_fFov = fFov; }
	void SetRatio( float fRatio ){ m_fRatio = fRatio; }
	void SetSpeed( float fSpeed ){ m_fSpeed = fSpeed; }

protected:	

	A3DVECTOR3			m_vPos;
	A3DVECTOR3          m_vDir;
	
	float           m_fFov;
	float           m_fRatio;
	float           m_fSpeed;

private:

};

class CECBezierSeg
{

public:
	
	CECBezierSeg();
	virtual ~CECBezierSeg();

public:

	void        Init( CECBezierPoint *pListPt);
	
	A3DVECTOR3	Bezier( float u ,bool bForward);
	A3DVECTOR3	Vector( float u ,bool bForward);

	float GetHeadSpeed()	{ return m_pListPoint[m_nHeadPoint].GetSpeed(); }
	float GetTailSpeed()	{ return m_pListPoint[m_nTailPoint].GetSpeed(); }
	float GetHeadRatio()	{ return m_pListPoint[m_nHeadPoint].GetRatio(); }
	float GetTailRatio()	{ return m_pListPoint[m_nTailPoint].GetRatio(); }
	float GetHeadFov()		{ return m_pListPoint[m_nHeadPoint].GetFov(); }
	float GetTailFov()		{ return m_pListPoint[m_nTailPoint].GetFov(); }

	inline A3DVECTOR3	GetAnchorHead() const { return m_vAnchorHead; };
	inline A3DVECTOR3	GetAnchorTail() const { return m_vAnchorTail; };

	inline int			GetHeadPoint() const { return m_nHeadPoint; };
	inline int			GetTailPoint() const { return m_nTailPoint; };

	inline float		GetSegLength() const { return m_fLength; }
	inline void			SetSegLength( const float length) { m_fLength = length; }

	inline void		SetAnchorHead( const A3DVECTOR3 v ) { m_vAnchorHead = v; };
	inline void		SetAnchorTail( const A3DVECTOR3 v ) { m_vAnchorTail = v; };

	inline void     SetHeadPoint( const int index ) { m_nHeadPoint = index; };
	inline void		SetTailPoint( const int index ) { m_nTailPoint = index; };

	inline void		SetSegLengh( const float lenght) { m_fLength = lenght; }

	inline void		SetSegTime(float fTime)			{ m_fSegTime = fTime; }
	inline float	GetSegTime() const				{ return m_fSegTime; }
	inline void		SetAcceleration(float fAcc)		{ m_fAcceleration = fAcc; }
	inline float	GetAcceleration() const			{ return m_fAcceleration; }

protected:
	
	A3DVECTOR3	m_vAnchorHead;		
	A3DVECTOR3	m_vAnchorTail;	
	int         m_nHeadPoint;
	int         m_nTailPoint;          
	float       m_fLength;  
	float		m_fSegTime;
	float		m_fAcceleration;
	
private:
	CECBezierPoint *m_pListPoint;
};

class CECBezier : public CECSceneObject
{

public:
	CECBezier();
	CECBezier& operator=(const CECBezier& src);
	virtual ~CECBezier();

	//User interface
public:

	bool			Load(AFile* pFile);

	void            SetOffset(A3DVECTOR3 vOffset);
	inline int	    GetObjectID() const { return m_nObjectID; };
	void            Release();

	int				GetSegmentNum() { return m_nNumSeg; }
	CECBezierSeg*	GetSegment(int n) { ASSERT(n >= 0 && n < m_nNumSeg); return &m_pListSeg[n]; }
	int				GetPointNum()	{ return m_nNumPoint; }
	CECBezierPoint*	GetPoint(int n)	{ ASSERT(n >= 0 && n < m_nNumPoint); return &m_pListPoint[n]; }

	//	Set / Get global ID
	void SetGlobalID(int iID) { m_iGlobalID = iID; }
	int GetGlobalID() { return m_iGlobalID; }
	//	Set / Get next route's global ID
	void SetNextGlobalID(int iID) { m_iNextGlobalID = iID; }
	int GetNextGlobalID() { return m_iNextGlobalID; }
	inline int      GetLoopType(){ return m_iLoopType; }
	inline int      GetBezierType() { return m_iBezierType; }
	//Only useful for CEditerBezier's object translate to CECBezier object
public:
	void            Assign(int nNumPt,int nNumSeg);
	inline void     SetObjectID( int id){  m_nObjectID = id; };
	void            AddBezierPoint( CECBezierPoint *pt , int i);
	void            AddBezierSeg( CECBezierSeg *seg , int i);

protected:

	int             m_nObjectID;
	int				m_iGlobalID;		//	Global ID used to link bezier routes
	int				m_iNextGlobalID;	//	Next bezier route's global ID
	int				m_nNumPoint;
	int				m_nNumSeg;
	int             m_iLoopType;        //  0:非循环; 1:首尾循环; 2:原路返回
	int             m_iBezierType;      //  0:一般曲线；1:摄像机路径
	AString			m_strActName;		//	Action name

	CECBezierPoint*	m_pListPoint;
	CECBezierSeg*	m_pListSeg;
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECBezierWalker
//	
///////////////////////////////////////////////////////////////////////////

class CECBezierWalker
{
public:		//	Types

public:		//	Constructor and Destructor

	CECBezierWalker();
	virtual ~CECBezierWalker();

public:		//	Attributes

public:		//	Operations

	//	Bind bezier route
	bool BindBezier(CECBezier* pBezier);
	//	Start walk
	bool StartWalk(bool bLoop, bool bForward);
	//	Pause walk
	void Pause(bool bPause);

	//	Tick routine
	bool Tick(int iDeltaTime);

	//	Get current position
	A3DVECTOR3 GetPos();
	//	Get current direction
	A3DVECTOR3 GetDir();
	//	Get current ratio
	float GetRatio();
	//	Get current fov
	float GetFov();

	//	Forward flag
	void SetForwardFlag(bool bForward);
	bool GetForwardFlag() { return m_bForward; }
	//	Move speed
	void SetSpeed(float fSpeed);
	float GetSpeed() { return m_fSpeed; }
	//	Get total time
	int GetTotalTime() { return m_iTotalTime; }
	//	Loop flag
	void SetLoopFlag(bool bLoop) { m_bLoop = bLoop; }
	bool GetLoopFlag() { return m_bLoop; }
	//	Get walking flag
	bool IsWalking() { return m_bWalking; }
	//	Get pause flag
	bool IsPause() { return m_bPause; }

protected:	//	Attributes

	CECBezier	m_Bezier;		//	Bezier route data
	bool		m_bForward;		//	Forward flag
	float		m_fSpeed;		//	Moving speed
	int			m_iTotalTime;	//	Total time of whole bezier route
	int			m_iTimeCnt;		//	Time counter
	int			m_iCurSeg;		//	Current segment
	int			m_iCurSegTime;	//	Total time of current segment
	int			m_iPassSegTime;	//	Total time of passed segments
	bool		m_bLoop;		//	Loop flag
	bool		m_bWalking;		//	true, is walking
	bool		m_bPause;		//	Pause flag
	bool		m_bForwardStop;	//	Stop flag

protected:	//	Operations
	float GetSegRatioPos(int* pSeg, bool &bForward);
	
};
