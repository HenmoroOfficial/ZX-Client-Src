// Filename	: PolyFilter.cpp
// Creator	: 
// Date		: 2009/06/29
// Date     : 2009/07/10

#pragma once

#include <A3DTypes.h>
#include <vector.h>

// namespace: PolyFilter
// usage    : Filter image colors within and around a serial of polygons
//
namespace PolyFilter
{
	// Image type definition and related interface
	//
	typedef unsigned char RGBType;
	extern RGBType g_maxRGBValue;

	// Polygon type definitions
	//
	typedef A3DPOINT2 Vertex;
	typedef abase::vector<Vertex> Polygon;
	typedef abase::vector<Polygon> PolygonArray;

	// Filter Interface 1: Pixel RGB color is given in separated RGBType
	//                     , with an additional color to blend with it
	void Execute(RGBType *rgb, int width, int height
		, const PolygonArray &polygons, float alpha, int expandWidth
		, const RGBType *blendColor = 0, float blendAlpha = 0);
}
