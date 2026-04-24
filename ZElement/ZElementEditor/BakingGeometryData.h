/*
 * FILE: d:\Dev\Projects2.2\SDSElement\SDSElementEditor\BakingGeometryData.h
 *
 * DESCRIPTION: Common geometry data used to bake light maps or vertex color.
 *
 * CREATED BY: LiChen, 2012/8/16
 *
 * HISTORY: 
 *
 * Copyright (c) 2012 Perfect World Entertainment Inc, All Rights Reserved.
 */

#ifndef _BAKING_GEOMETRY_DATA_H_
#define _BAKING_GEOMETRY_DATA_H_


enum BAKING_TYPE
{
	BAKING_VERTEX_COLOR,
	BAKING_LIGHT_MAP,
};

struct BAKING_GEOMETRY_DATA 
{
	BAKING_TYPE			eBakingType;
	int     			nLightMapSize;
	float				fLitDirectScale;
	float				fLitIndirectScale;
	float				fLitAOScale;
	unsigned int		nNumVertices;
	unsigned int		nNumIndices;
	bool				bHasNormal;
	bool				bHasAlphaTestTexture;
};

struct BAKING_UV
{
	float				fU;
	float				fV;
};

/*
File format for mirror plane object


struct BAKING_GEOMETRY_DATA

AlphaTestTextureFileName		AString													: [optional] if BAKING_GEOMETRY_DATA.bHasAlphaTestTexture == true

Positions						A3DVECTOR3 * BAKING_GEOMETRY_DATA.nNumVertices

Normals							A3DVECTOR3 * BAKING_GEOMETRY_DATA.nNumVertices			: [optional] if BAKING_GEOMETRY_DATA.bHasNormal == true

LightMapTexcoord				BAKING_UV * BAKING_GEOMETRY_DATA.nNumVertices

Indices							unsigned short * BAKING_GEOMETRY_DATA.nNumIndices

*/

#endif /* #ifdef _BAKING_GEOMETRY_DATA_H_ */