/*
 * FILE: Capusule.h
 *
 * DESCRIPTION: Capsule description
 *
 * CREATED BY: YangXiao
 *
 * HISTORY: 
 *
 * Copyright (c) 2010 Perfect World, All Rights Reserved.
 */
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAPUSULE_H__BB96BC06_403C_4AFF_B0F9_F684001C1211__INCLUDED_)
#define AFX_CAPUSULE_H__BB96BC06_403C_4AFF_B0F9_F684001C1211__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <A3D.h>

namespace CHBasedCD{

class CCapsule  
{

protected:
	A3DVECTOR3 m_pos;//Capsule center position
	float m_height;//capsule height(cylinder part)
	float m_radius;//capsule radius

	float m_centerHei;//center to bottom height

	A3DVECTOR3 m_up; //capsule up direction
	A3DVECTOR3 m_right; //capsule right direction (used when capsule brush trace)
	A3DVECTOR3 m_front; //capsule front direction (used when capsule brush trace)

	void InitDir(); //generate right direction and front direction
	
public:

	//get center to bottom height.
	//character model can be positioned GetPos() + A3DVector(0, -GetCenterHei(), 0).  
	float GetCenterHei() const { return m_centerHei; }

	//get & set capsule center position
	A3DVECTOR3 GetPos() const { return m_pos; }
	void SetPos(const A3DVECTOR3& pos){ m_pos = pos; }

	//get & set capsule height
	float GetHeight() const { return m_height; }
	void SetHeight(float height) { m_height = height; m_centerHei = fabsf(m_up.y) * (m_height/2) + m_radius;}

	//get & set capsule radius
	float GetRadius() const { return m_radius; }
	void SetRadius(float radius) { m_radius = radius; m_centerHei = fabsf(m_up.y) * (m_height/2) + m_radius;}

	//get & set capsule up direction
	void SetUp(const A3DVECTOR3& up )
	{
		m_up = Normalize(up);
		InitDir();
		
	}
	A3DVECTOR3 GetUp() const
	{
		return m_up;
	}

	//get right direction
	A3DVECTOR3 GetRight() const
	{
		return m_right;
	}
	//get front direction
	A3DVECTOR3 GetFront() const
	{
		return m_front;
	}
	

	//render wired capsule
	void RenderWire(A3DWireCollector* pWireCollector,  DWORD color = 0xff800000);

	//construction & deconstruction
	CCapsule();
	virtual ~CCapsule();

};

}

#endif // !defined(AFX_CAPUSULE_H__BB96BC06_403C_4AFF_B0F9_F684001C1211__INCLUDED_)
