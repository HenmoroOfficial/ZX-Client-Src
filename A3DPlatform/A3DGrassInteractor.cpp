/*
* FILE: A3DGrassInteractor.cpp
*
* DESCRIPTION: 
*
* CREATED BY: Zhangyachuan, 2011/7/28
*
* HISTORY: 
*
* Copyright (c) 2011 Archosaur Studio, All Rights Reserved.
*/

#include "A3DGrassInteractor.h"
#include "ABaseDef.h"
#include "A3DPI.h"

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Reference to External variables and functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Local Types and Variables and Global variables
//	
///////////////////////////////////////////////////////////////////////////

const int A3DGrassInteractor::m_iGridNum = 32;

///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Implement A3DGrassInteractor
//	
///////////////////////////////////////////////////////////////////////////

A3DGrassInteractor::A3DGrassInteractor()
: m_aObjectMap(m_iGridNum * m_iGridNum, false)
, m_aObjectPosMap(m_iGridNum * m_iGridNum, A3DVECTOR3(0.0f))
, m_vOriginPoint(A3D::g_vOrigin)
, m_vCenter(A3D::g_vOrigin)
, m_fGridSize(2.0f) 
, m_fAreaLen(m_fGridSize * m_iGridNum)
, m_UpdateTimeDuringOneFrame(0)
{
}

A3DGrassInteractor::~A3DGrassInteractor()
{

}

//	Do reset for each tick
void A3DGrassInteractor::Reset()
{
	m_aObjectMap.assign(m_iGridNum * m_iGridNum, false);
	m_aObjectPosMap.assign(m_iGridNum * m_iGridNum, 0);
	m_UpdateTimeDuringOneFrame = 0;
}

void A3DGrassInteractor::UpdateRcArea(float centerX, float centerZ)
{
	float fLeft = centerX - m_fAreaLen / 2;
	float fTop  = centerZ - m_fAreaLen / 2;
	m_rcArea = ARectF(fLeft, fTop, fLeft + m_fAreaLen, fTop + m_fAreaLen);
	m_rcArea.Normalize();
}

//	Set Host Player Pos will displace the interaction buffer's offset
void A3DGrassInteractor::SetInteractorCenterPos(const A3DVECTOR3& vCenter)
{
	Reset();
	m_vCenter = vCenter;
	UpdateRcArea(vCenter.x, vCenter.z);
	const float fHalfLen = m_fAreaLen / 2.0f;
	m_vOriginPoint.Set(vCenter.x - fHalfLen, 0, vCenter.z - fHalfLen);
	SetInteractorPos(vCenter);
}

//	Set interactor pos will 
void A3DGrassInteractor::SetInteractorPos(const A3DVECTOR3& vPos)
{
	int iIndex = -1;
	int iSurroundIndex[4];
	if (!GetIndexByPos(vPos, &iIndex, iSurroundIndex))
		return;

	m_aObjectMap[iIndex] = true;
	m_aObjectPosMap[iIndex] = vPos;
}

//	Get grass nearby status (a combination of QUERY_STATUS)
int A3DGrassInteractor::GetGrassNearbyStatus(const A3DVECTOR3& vPos) const
{
	int iIndex = -1;
	int iSurroundIndex[4];
	if (!GetIndexByPos(vPos, &iIndex, iSurroundIndex))
		return NONE;

	int iRet = NONE;
	for (int i = 0; i < _countof(iSurroundIndex); ++i)
	{
		if (iSurroundIndex[i] < 0)
			continue;

		if (!m_aObjectMap[iSurroundIndex[i]])
			continue;

		iRet |= (1 << i);
	}

	return iRet;
}

//	Check a grass is pressed or not
bool A3DGrassInteractor::IsGrassBePressed(const A3DVECTOR3& vPos, float fMinPressDist, float fMaxPressDist, float fHeight, A3DVECTOR3& vOffset) const
{
	int iIndex = -1;
	int iSurroundIndex[4];
	if (!GetIndexByPos(vPos, &iIndex, iSurroundIndex))
		return false;

	const float fSquaredMinDist = fMinPressDist * fMinPressDist;
	const float fSquaredMaxDist = fMaxPressDist * fMaxPressDist;

	bool bRet = false;
	float fMostNearPressSquaredDist = 99999.9f;
	for (int i = 0; i < _countof(iSurroundIndex); ++i)
	{
		int iIndex = iSurroundIndex[i];
		if (iIndex < 0)
			continue;

		if (!m_aObjectMap[iIndex])
			continue;

		A3DVECTOR3 vActorPos = m_aObjectPosMap[iIndex];
		float fSquaredDist = (vActorPos - vPos).SquaredMagnitudeH();
		if (fSquaredDist > fSquaredMinDist
			&& fSquaredDist < fSquaredMaxDist
			&& fabs(vActorPos.y - vPos.y) < fHeight)
		{
			if (fMostNearPressSquaredDist > fSquaredDist)
			{
				fMostNearPressSquaredDist = fSquaredDist;
				vOffset = vPos - m_aObjectPosMap[iIndex];
			}

			bRet = true;
		}
	}

	return bRet;
}

bool A3DGrassInteractor::GetIndexByPos(const A3DVECTOR3& vPos, int* piIndex, int iSurroundIndex[4]) const
{
	ASSERT( piIndex );

	float fXDelta = vPos.x - m_vOriginPoint.x;
	float fZDelta = vPos.z - m_vOriginPoint.z;

	if (fXDelta < 0 || fXDelta >= m_fAreaLen
		|| fZDelta < 0 || fZDelta >= m_fAreaLen)
		return false;

	float fNumGridX = fXDelta / m_fGridSize;
	float fNumGridZ = fZDelta / m_fGridSize;
	int iNumGridX = static_cast<int>(fNumGridX);
	int iNumGridZ = static_cast<int>(fNumGridZ);
	if (iNumGridX < 0 || iNumGridX >= m_iGridNum
		|| iNumGridZ < 0 || iNumGridZ >= m_iGridNum)
		return false;

	*piIndex = iNumGridZ * m_iGridNum + iNumGridX;

	int iLeftIdx = -1;
	int iTopIdx = -1;
	if (fNumGridX - iNumGridX < 0.5f)
		iLeftIdx = iNumGridX - 1;
	else
		iLeftIdx = iNumGridX;

	if (fNumGridZ - iNumGridZ < 0.5f)
		iTopIdx = iNumGridZ - 1;
	else
		iTopIdx = iNumGridZ;

	iSurroundIndex[0] = iTopIdx * m_iGridNum + iLeftIdx;
	iSurroundIndex[1] = iTopIdx * m_iGridNum + iLeftIdx + 1;
	iSurroundIndex[2] = (iTopIdx + 1) * m_iGridNum + iLeftIdx;
	iSurroundIndex[3] = (iTopIdx + 1) * m_iGridNum + iLeftIdx + 1;

	if (iTopIdx == -1)
	{
		iSurroundIndex[0] = iSurroundIndex[1] = -1;
	}

	if (iLeftIdx == -1)
	{
		iSurroundIndex[0] = iSurroundIndex[2] = -1;
	}

	if (iTopIdx == m_iGridNum - 1)
	{
		iSurroundIndex[2] = iSurroundIndex[3] = -1;
	}

	if (iLeftIdx == m_iGridNum - 1)
	{
		iSurroundIndex[1] = iSurroundIndex[3] = -1;
	}

	return true;
}