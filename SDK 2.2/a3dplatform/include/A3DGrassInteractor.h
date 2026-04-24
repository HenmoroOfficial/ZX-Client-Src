/*
* FILE: A3DGrassInteractor.h
*
* DESCRIPTION: This class is used for tracking the actors walking on the grass
*				User should update this every tick, by calling SetHostPlayerPos first 
*				then update all the interactors' positions by SetInteractorPos
*				The interactor ONLY take effect when the render method is set to A3DGrassLand::RM_HWINSTANCE
*				Also the pointer get from A3DGrassLand is not valid in other render modes
*
* CREATED BY: Zhangyachuan, 2011/7/28
*
* HISTORY: 
*
* Copyright (c) 2011 Archosaur Studio, All Rights Reserved.
*/

#ifndef _A3DGrassInteractor_H_
#define _A3DGrassInteractor_H_

#include "A3DVector.h"
#include "ARect.h"
#include "ABaseDef.h"
#include <vector>

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////

typedef __int64 a_int64;

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class A3DGrassInteractor
//	
///////////////////////////////////////////////////////////////////////////

class A3DGrassInteractor
{
public:

	enum QUERY_STATUS
	{
		NONE		= 0x00,
		LEFTTOP		= 0x01,		// left top grid is set
		RIGHTTOP	= 0x02,
		LEFTBOTTOM	= 0x04,
		RIGHTBOTTOM	= 0x08,
	};

public:

	A3DGrassInteractor();
	~A3DGrassInteractor();

public:

	void Release() {}

	//	Do reset for each tick
	//	Reset clear all the tracked interactor
	void Reset();
	//	We can only track a limited area of grass' state, so we define a rect of 32 * 32 grid (each of which is 2 meters long)
	//	as we want the effect always goes right, we need to set the center
	//	position, in order to move the interaction area
	//	normally, we make the center position as the same as host player's position in our game
	//	This function call Reset() automatically
	void SetInteractorCenterPos(const A3DVECTOR3& vCenter);
	//	Set interactor pos for each object that may press the grass
	void SetInteractorPos(const A3DVECTOR3& vPos);
	//	Get grass nearby status (a combination of QUERY_STATUS)
	int GetGrassNearbyStatus(const A3DVECTOR3& vPos) const;
	//	Check a grass is pressed or not
	//	vOffset is the offset between the presser and the grass
	bool IsGrassBePressed(const A3DVECTOR3& vPos, float fMinPressDist, float fMaxPressDist, float fHeight, A3DVECTOR3& vOffset) const;
	//	Get area
	const ARectF& GetArea() const { return m_rcArea; }

	//	For performance counting
	void AddUpdateTime(a_int64 dwTime) { m_UpdateTimeDuringOneFrame += dwTime; }
	//	In microsecond
	a_int64 GetUpdateTimeDuringOneFrame() const { return m_UpdateTimeDuringOneFrame; }

    const A3DVECTOR3& GetCenter() const { return m_vCenter; }
private:

	A3DVECTOR3 m_vCenter;
	A3DVECTOR3 m_vOriginPoint;
	std::vector<bool> m_aObjectMap;
	std::vector<A3DVECTOR3> m_aObjectPosMap;
	const float m_fGridSize;
	static const int m_iGridNum;
	float m_fAreaLen;
	ARectF m_rcArea;
	a_int64 m_UpdateTimeDuringOneFrame;

private:
	void UpdateRcArea(float centerX, float centerZ);
	bool GetIndexByPos(const A3DVECTOR3& vPos, int* piIndex, int iSurroundIndex[4]) const;

};
///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////


#endif	//	_A3DGrassInteractor_H_

