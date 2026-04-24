/********************************************************************
	created:	2007/04/09
	author:		kuiwu
	
	purpose:	tranfer info between maps
	Copyright (C) 2007 - All Rights Reserved
*********************************************************************/

#pragma once

#include "vector.h"
using namespace abase;
#include "a3dvector.h"

namespace AutoMove
{


class CTransferMap  
{
public:
	CTransferMap();
	virtual ~CTransferMap();

	bool  GetTransferInfo(int from, int to, A3DVECTOR3& throughPos);
	bool  Load(const char * szFilePath);
	
	void  Release();
private:
	vector<int>  m_Id;			//all map ids
	int **		 m_TransTab;    //transfer table
	
	typedef	struct  
	{
		//id1->id2
		int			 id1;
		A3DVECTOR3   pos1;
		int			 id2;
	}TransEdge;

	vector<TransEdge> m_TransEdge;
	
	//get the index by the map id
	int _GetIndex(int id);  

};


} //end of namespace