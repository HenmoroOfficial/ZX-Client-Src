/*
 * FILE: CHullArchive.h
 *
 * DESCRIPTION: Load & Save CCDBrush
 *
 * CREATED BY: 
 *
 * HISTORY:
 *
 * 
 */

#ifndef _BRUSHARCHIVE_H_
#define _BRUSHARCHIVE_H_

#include "aabbcd.h"

#include <AFile.h>
#include <A3DFuncs.h>
///////////////////////////////////////////////////////////////////////////
//
//	new version of convex hull data
//
// unsigned int magic
// unsigned int version
// char szModelFile[MAX_PATH]
// unsigned int  nNumHull;
// convex hull data 1..nNumHull
///////////////////////////////////////////////////////////////////////////

namespace CHBasedCD
{

class CCDBrush;

class BrushArchive
{

public:
	BrushArchive();
	~BrushArchive();

	//get ecm file the convex hull generate from
	const char* GetModelFile() const;
	//get hull number
	int GetNumHull() const;

	void SetModelFile(const char* szModelFile);
	void SetNumHull(int nNumHull) { m_nNumHull = nNumHull; }

	bool LoadHeader(AFile& af);
	inline bool LoadBrush(AFile& af, CHBasedCD::CCDBrush* pBrush)
	{	
		if (!pBrush->Load(&af))
		{
			return false;
		}
		return true;
	}
	bool SaveHeader(AFile& af);
	inline bool SaveBrush(AFile& af, CHBasedCD::CCDBrush* pBrush)
	{
		return pBrush->Save(&af);
	}

protected:
	int m_nNumHull;

	char m_szModelFile[MAX_PATH];
};


}

#endif