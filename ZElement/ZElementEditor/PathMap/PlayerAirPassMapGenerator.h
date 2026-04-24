/********************************************************************
	created:	2008/04/01
	author:		kuiwu
	
	purpose:	player air move map
	Copyright (C) 2008 - All Rights Reserved
*********************************************************************/

#ifndef _PLAYER_AIR_PASSMAP_GENERATOR_H_
#define _PLAYER_AIR_PASSMAP_GENERATOR_H_

#include "math.h"
#include "A3DVector.h"

#include "BitImage.h"
#include <vector.h>
typedef unsigned char UCHAR;

#define STANDARD_PASSMAP_SAMPLE_STEP 1.0f
#define STANDARD_PASSMAP_SIZE 1024

struct PLAYER_SAMPLE_POS;


class CPlayerAirPassMapGenerator
{
public:
	CPlayerAirPassMapGenerator();
	virtual ~CPlayerAirPassMapGenerator();	

// Interface
	bool ExportMaps(CBitImage* pReachableImage);
	bool ImportMaps(const CBitImage* pReachableImage);


	// it seems we haven't employ this function
	void Reset()
	{
		int iSize=m_iWidth*m_iLength;
		// 初始化区域 对于m_arrPassMap而言，0表示未计算，1表示可达，2表示不可达
		memset(m_arrPassMap, 0, iSize*sizeof(UCHAR));

	}
	void Reset(const PLAYER_SAMPLE_POS& sPos);
	
	void Release();
	void SetSampleInfo(float fss =STANDARD_PASSMAP_SAMPLE_STEP, int iTWidth=STANDARD_PASSMAP_SIZE, int iTLength=STANDARD_PASSMAP_SIZE);

	
	void GetMapSize(int& iWidth, int& iLength) const { iWidth=m_iWidth; iLength=m_iLength; }
	UCHAR* GetPassMap(){ return m_arrPassMap;}

	
	// uv 坐标系到地形三维坐标系的转化
	void Transf2DTo3D(const PLAYER_SAMPLE_POS& sPos, A3DVECTOR3& vPos);
	void Transf3DTo2D(const A3DVECTOR3& vPos, PLAYER_SAMPLE_POS& sPos);
	
	void FillUnreachableArea();				// 将未计算区域置为不可达区域

	// 直接对象素操作！
	void SetPosReachable(const PLAYER_SAMPLE_POS& sPos);
	void SetPosUnreachable(const PLAYER_SAMPLE_POS& sPos);

	bool IsPosReachable(const PLAYER_SAMPLE_POS& sPos);


	bool IsPosReachable(int u, int v);
	bool IsPosReachable(const A3DVECTOR3 & vPos);

	
	float GetPosDeltaHeight(const PLAYER_SAMPLE_POS& sPos);
	


	float GetPosDeltaHeight(const A3DVECTOR3 & vPos);

	
	
protected:
// Attributes
private:
	// Terrain sample step and resolution
	float m_fSampleStep;
	int m_iWidth;
	int m_iLength;
	
	// Pass Map: a 2D bit set, but by now stored as a  char array
	UCHAR *m_arrPassMap;

	
	typedef struct  
	{
		float h;         //height
		A3DVECTOR3 n;    //normal
	}Surface_Info ;

	Surface_Info * m_arrSurface;   //the surface height  (terrain or building)

	
	
};

#endif