/*
 * FILE: EL_Precinct.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/6/2
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#ifndef _EL_REGION_H_
#define _EL_REGION_H_

#include "vector.h"
#include <stdio.h>

#ifdef _ELEMENTCLIENT
#include "AAssist.h"
#include "A3DVector.h"
#include "AArray.h"
#endif

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

#define ELRGNFILE_VERSION		8	// 7->8 add buff region

///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////

#pragma pack(1)

//	Data file header
struct REGIONFILEHEADER1
{
	unsigned int	dwVersion;		//	File version
	int				iNumRegion;		//	Number of region
};

struct REGIONFILEHEADER2
{
	unsigned int	dwVersion;		//	File version
	int				iNumRegion;		//	Number of region
	int				iNumTrans;		//	Number of transport box
};

struct REGIONFILEHEADER4
{
	unsigned int	dwVersion;		//	File version
	int				iNumRegion;		//	Number of region
	int				iNumTrans;		//	Number of transport box
	unsigned int	dwTimeStamp;	//	Time stamp of this data file
};

struct REGIONFILEHEADER5
{
	unsigned int	dwVersion;		//	File version
	int				iNumRegion;		//	Number of region
	int				iNumTrans;		//	Number of transport box
	unsigned int	dwTimeStamp;	//	Time stamp of this data file
	int				iNumBuffRegion;	//	Number of buff region
};

#pragma pack()

#ifdef _ELEMENTCLIENT
class AWScriptFile;
#endif

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CELRegion
//	
///////////////////////////////////////////////////////////////////////////

class CELRegion
{
public:		//	Types

	//	Region type
	enum
	{
		REGION_PK = 0,
		REGION_SANCTUARY,
	};

	struct VECTOR3
	{
		float x, y, z;
	};

public:		//	Constructor and Destructor

	CELRegion();
	virtual ~CELRegion();

public:		//	Attributes

	float GetLeft() { return m_fLeft;}
	float GetRight() { return m_fRight;}
	float GetTop() { return m_fTop;}
	float GetBottom() { return m_fBottom;}

public:		//	Operations

	//	Is specified position in this region ?
	bool IsPointIn(float x, float z);
	//	Get region type
	int GetType() { return m_iType; }

#ifdef _ELEMENTCLIENT

	bool Load(AWScriptFile* pFile, int iVersion);

	const ACHAR* GetName() { return m_strName; }

#else

	//	Load data from file
	bool Load(FILE* fp, int iVersion);

#endif

protected:	//	Attributes

#ifdef _ELEMENTCLIENT

	ACString	m_strName;

#endif

	int		m_iType;		//	Region type
	
	float	m_fLeft;		//	Bound box of precinct
	float	m_fTop;
	float	m_fRight;
	float	m_fBottom;

	abase::vector<VECTOR3>	m_aPoints;	//	Region points

protected:	//	Operations

	bool IsCrossLine(float x, float z, int iIndex);
	//	Build precinct bound box
	void BuildBoundBox();
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CELTransportBox
//	
///////////////////////////////////////////////////////////////////////////

class CELTransportBox
{
public:		//	Types

	struct VECTOR3
	{
		float x, y, z;
	};

public:		//	Constructor and Destructor

	CELTransportBox();
	virtual ~CELTransportBox();

public:		//	Attributes

public:		//	Operations

	//	Is specified position in this transport box ?
	bool IsPointIn(float x, float y, float z)
	{
		if (x < m_vPos.x - m_vExts.x || x > m_vPos.x + m_vExts.x ||
			y < m_vPos.y - m_vExts.y || y > m_vPos.y + m_vExts.y ||
			z < m_vPos.z - m_vExts.z || z > m_vPos.z + m_vExts.z)
			return false;

		return true;
	}

#ifdef _ELEMENTCLIENT

	bool Load(AWScriptFile* pFile, int iVersion);

#else

	//	Load data from file
	bool Load(FILE* fp, int iVersion);

#endif

	//	Get instance id
	int GetInstanceID() { return m_idInst; }
	//	Get source instance id
	int GetSrcInstanceID() { return m_idSrcInst; }
	//	Get level limit
	int GetLevelLimit() { return m_iLevelLmt; }
	//  Get transport line
	int GetTransportLine(){ return m_iLine; }
	//	Get target position
	const VECTOR3& GetTargetPos() { return m_vTarget; }
	//	Get area position
	const VECTOR3& GetPos() { return m_vPos; }
	//	Get area extents
	const VECTOR3& GetExtents() { return m_vExts; }

	//	Set index in manager
	void SetIndex(int iIndex) { m_iIndex = iIndex; }
	//	Get index in manager
	int GetIndex() { return m_iIndex; }
	//	Get controller
	int GetControllerID(){ return m_iCtrlID; }
protected:	//	Attributes

	int			m_idInst;		//	Instance id
	int			m_idSrcInst;	//	Source instance id
	int			m_iLevelLmt;	//	Level limit
	int         m_iLine;        //  Transport line
	VECTOR3		m_vTarget;		//	Target position
	VECTOR3		m_vPos;			//	Area position
	VECTOR3		m_vExts;		//	Transport box area extents
	int			m_iIndex;		//	Transport box's index in manager
	int			m_iCtrlID;		//	controller's id

protected:	//	Operations

};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CELBuffRegion
//	
///////////////////////////////////////////////////////////////////////////

class CELBuffRegion : public CELRegion
{
public:		//	Constructor and Destructor

	CELBuffRegion();
	virtual ~CELBuffRegion();

#ifdef _ELEMENTCLIENT
#else
	//	Load data from file
	bool Load(FILE* fp, int iVersion);
#endif

	int GetRegionTemplID() { return m_nRegionTemplID;	}
	int GetMonsterID() { return m_nMonsterID;	}
protected:	//	Attributes

	int     m_nRegionTemplID;	//区域模板ID
	int     m_nMonsterID;     //关联怪物ID
};


///////////////////////////////////////////////////////////////////////////
//	
//	Class CELBuffRegion
//	
///////////////////////////////////////////////////////////////////////////
struct CELGuildBuilding
{
public:
	struct VECTOR3
	{
		float x, y, z;
	};
	CELGuildBuilding() {}
	~CELGuildBuilding() {}

#ifdef _ELEMENTCLIENT
	bool Load(AWScriptFile* pFile, int iVersion);
#endif

	int		m_iTmplId;		//  template id of REGION_BLOCK_ESSENCE
	int		m_iWidht;			//  block width
	int		m_iHeight;		//  block height
	VECTOR3	m_vPos;		//	Position
	VECTOR3	m_vDir;		//	Orientation
	VECTOR3	m_vUp;			//	Up direction
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CELRegionSet
//	
///////////////////////////////////////////////////////////////////////////

class CELRegionSet
{
public:		//	Types

public:		//	Constructor and Destructor

	CELRegionSet();
	virtual ~CELRegionSet();

public:		//	Attributes

public:		//	Operations

	//	Load data from file
	bool Load(const char* szFileName);

	//	Get time stamp
	unsigned int GetTimeStamp() { return m_dwTimeStamp; }

	//	Get the region specified position is in
	CELRegion* IsPointInRegion(float x, float z);
	//	Get the PK region specified position is in
	CELRegion* IsPointInPKRegion(float x, float z);
	//	Get the peace region specified position is in
	CELRegion* IsPointInSanctuary(float x, float z);
	//	Get region number
	int GetRegionNum() { return m_aRegions.size(); }
	//	Get region by index
	CELRegion* GetRegion(int n)
	{
		if (n >= 0 && n < (int)m_aRegions.size())
			return m_aRegions[n];
		else 
			return NULL;
	} 

	//	Get the transport box specified position is in
	CELTransportBox* IsPointInTransport(float x, float y, float z, int idSrcInst);
	//	Get transport box number
	int GetTransportBoxNum() { return m_aTransBoxes.size(); }
	//	Get transport box by index
	CELTransportBox* GetTransportBox(int n)
	{
		if (n >= 0 && n < (int)m_aTransBoxes.size())
			return m_aTransBoxes[n];
		else
			return NULL;
	}


	int GetGuildBuildingNum() { return m_aGuildBuildings.size(); }
	//	Get transport box by index
	CELGuildBuilding* GetGuildBuilding(int n)
	{
		if (n >= 0 && n < (int)m_aGuildBuildings.size())
			return m_aGuildBuildings[n];
		else
			return NULL;
	}


	//	Get the buff region specified position is in
	CELBuffRegion* IsPointInBuffRegion(float x, float z);
	//	Get buff region number
	int GetBuffRegionNum() { return m_aBuffRegions.size(); }
	//	Get buff region by index
	CELBuffRegion* GetBuffRegion(int n)
	{
		if (n >= 0 && n < (int)m_aBuffRegions.size())
			return m_aBuffRegions[n];
		else
			return NULL;
	}

protected:	//	Attributes

	unsigned int	m_dwTimeStamp;		//	Time stamp of data

	abase::vector<CELRegion*>	m_aRegions;			//	Region array
	abase::vector<CELRegion*>	m_aPKRegions;		//	PK region array
	abase::vector<CELRegion*>	m_aPeaceRegions;	//	Peace region array

	abase::vector<CELBuffRegion*>	m_aBuffRegions;	//  Buff region array

	abase::vector<CELTransportBox*>	m_aTransBoxes;	//	Transport boxes
	abase::vector<CELGuildBuilding*>	m_aGuildBuildings;
	
protected:	//	Operations

};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////

#endif	//	_EL_REGION_H_
