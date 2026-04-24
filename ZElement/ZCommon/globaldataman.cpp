/*
 * FILE: globaldataman.cpp
 *
 * DESCRIPTION: global data loader and manager
 *
 * CREATED BY: Hedi, 2005/7/18
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#include <A3DTypes.h>
#include <vector.h>
#include <globaldataman.h>
#include <AWScriptFile.h>
#include <AChar.h>
#include <ALog.h>
#include <AFileImage.h>

struct BattleTime { int nDay, nHour, nMin; };
static vector<DOMAIN_INFO>			global_domain_infos;
static vector<DOMAIN_INFO>			global_domain_infos2;
static vector<DOMAIN_INFO>			global_domain_infos3;	// 跨服地图信息
static vector<DOMAIN_INFO>			global_domain_infos4;	// 总体地图信息
static vector<DOMAIN_INFO>			global_domain_infos5;	// 上古世界地图信息
static vector<DOMAIN_INFO_SERV>		global_domain_infos_server;
static vector<BattleTime>			global_domain_vectime;
static vector<BattleTime>			global_domain_vectime2;
static int							global_domain_timemax;
static int							global_domain_timemax2;

static vector<GSHOP_ITEM>			global_gshop_items;
static vector<GSHOP_MAIN_TYPE>		global_gshop_maintypes;
static DWORD						global_gshop_timestamp;

static vector<GSHOP_ITEM>			global_gshop_items2;
static vector<GSHOP_MAIN_TYPE>		global_gshop_maintypes2;
static DWORD						global_gshop_timestamp2;

static vector<GSHOP_ITEM>			global_gshop_items3;
static vector<GSHOP_MAIN_TYPE>		global_gshop_maintypes3;
static DWORD						global_gshop_timestamp3;

typedef hashtab<int, int, _hash_function> MAINTYPE_IDMAP;
static MAINTYPE_IDMAP global_gshop_maintype_id_to_index(128);
static MAINTYPE_IDMAP global_gshop_maintype_id_to_index2(128);
static MAINTYPE_IDMAP global_gshop_maintype_id_to_index3(128);
//-----------------------------------------------------------------------------
static vector<VIP_AWARD_ITEM>		global_vipaward_items;
static DWORD						global_vipaward_timestamp;
//-----------------------------------------------------------------------------

bool LoadDomainData(AFileImage& file, const char* szFile, vector<DOMAIN_INFO>& domain_info);

vector<DOMAIN_INFO> * globaldata_getdomaininfos()
{
	return &global_domain_infos;
}

vector<DOMAIN_INFO> * globaldata_getdomaininfos2()
{
	return &global_domain_infos2;
}

vector<DOMAIN_INFO> * globaldata_getdomaininfos3()
{
	return &global_domain_infos3;
}

vector<DOMAIN_INFO> * globaldata_getdomaininfos4()
{
	return &global_domain_infos4;
}

vector<DOMAIN_INFO> * globaldata_getdomaininfos5()
{
	return &global_domain_infos5;
}

vector<DOMAIN_INFO_SERV> * globaldata_getdomaininfosserver()
{
	return &global_domain_infos_server;
}

vector<GSHOP_MAIN_TYPE> * globaldata_getgshopmaintypes()
{
	return &global_gshop_maintypes;
}

GSHOP_MAIN_TYPE * globaldata_getgshopmaintype_by_id(int id)
{
	MAINTYPE_IDMAP::iterator it = global_gshop_maintype_id_to_index.find(id);
	if( it != global_gshop_maintype_id_to_index.end() )
		return &global_gshop_maintypes[*it.value()];

	return NULL;
}

int globaldata_getgshopitemindex_by_id(unsigned int id, unsigned int num)
{
	UINT nSize = global_gshop_items.size();
	UINT i;
	for (i=0; i<nSize; i++)
	{
		GSHOP_ITEM item = global_gshop_items[i];
		if(item.id == id && item.num == num)
			return i;
	}
	return -1;
}

vector<GSHOP_ITEM> * globaldata_getgshopitems()
{
	return &global_gshop_items;
}

DWORD globaldata_getgshop_timestamp()
{
	return global_gshop_timestamp;
}

// gshop2
vector<GSHOP_MAIN_TYPE> * globaldata_getgshopmaintypes2()
{
	return &global_gshop_maintypes2;
}

GSHOP_MAIN_TYPE * globaldata_getgshopmaintype_by_id2(int id)
{
	MAINTYPE_IDMAP::iterator it = global_gshop_maintype_id_to_index2.find(id);
	if( it != global_gshop_maintype_id_to_index2.end() )
		return &global_gshop_maintypes2[*it.value()];

	return NULL;
}

int globaldata_getgshopitemindex_by_id2(unsigned int id, unsigned int num)
{
	UINT nSize = global_gshop_items2.size();
	UINT i;
	for (i=0; i<nSize; i++)
	{
		GSHOP_ITEM item = global_gshop_items2[i];
		if(item.id == id && item.num == num)
			return i;
	}
	return -1;
}

vector<GSHOP_ITEM> * globaldata_getgshopitems2()
{
	return &global_gshop_items2;
}

DWORD globaldata_getgshop_timestamp2()
{
	return global_gshop_timestamp2;
}

// gshop3
vector<GSHOP_MAIN_TYPE> * globaldata_getgshopmaintypes3()
{
	return &global_gshop_maintypes3;
}

GSHOP_MAIN_TYPE * globaldata_getgshopmaintype_by_id3(int id)
{
	MAINTYPE_IDMAP::iterator it = global_gshop_maintype_id_to_index3.find(id);
	if( it != global_gshop_maintype_id_to_index3.end() )
		return &global_gshop_maintypes3[*it.value()];

	return NULL;
}

int globaldata_getgshopitemindex_by_id3(unsigned int id, unsigned int num)
{
	UINT nSize = global_gshop_items3.size();
	UINT i;
	for (i=0; i<nSize; i++)
	{
		GSHOP_ITEM item = global_gshop_items3[i];
		if(item.id == id && item.num == num)
			return i;
	}
	return -1;
}

vector<GSHOP_ITEM> * globaldata_getgshopitems3()
{
	return &global_gshop_items3;
}

DWORD globaldata_getgshop_timestamp3()
{
	return global_gshop_timestamp3;
}

//=============================================================================
//VIP奖励相关接口 Added 2011-12-29.
DWORD globaldata_get_vipaward_timestamp()
{
	return global_vipaward_timestamp;
}

vector<VIP_AWARD_ITEM>* globaldata_get_vipaward_items()
{
	return &global_vipaward_items;
}

bool LoadVIPAwardData()
{
	AFileImage vipAwardFile;
	DWORD dwReadLen = 0;
	if ( !vipAwardFile.Open( "Data\\VIPAward.data", AFILE_OPENEXIST | AFILE_BINARY ) )
	{
		a_LogOutput(1, "GlobalData_Load(), Failed to open vip award data file");
		return false;
	}

	if ( !vipAwardFile.Read(&global_vipaward_timestamp, sizeof(DWORD), &dwReadLen) )
	{
		a_LogOutput(1, "GlobalData_Load(), corrupted vip award file!");
		return false;
	}
	
	int nDataItemCount = 0;
	if ( !vipAwardFile.Read(&nDataItemCount, sizeof(int), &dwReadLen) )
	{
		a_LogOutput(1, "GlobalData_Load(), corrupted vip award file!");
		return false;
	}

	for ( int i = 0; i < nDataItemCount; ++i )
	{
		VIP_AWARD_ITEM vipAwardItem;
		memset( &vipAwardItem, 0, sizeof(VIP_AWARD_ITEM) );
		if ( !vipAwardFile.Read( &vipAwardItem, sizeof(VIP_AWARD_ITEM), &dwReadLen) )
		{
			a_LogOutput(1, "GlobalData_Load(), corrupted vip award file!");
			return false;
		}
		global_vipaward_items.push_back( vipAwardItem );
	}

	return true;
}
//Added end.
//=============================================================================

bool globaldata_load()
{
	// now load domain info file
	AFileImage domainFile;
	DWORD dwReadLen;
	int	nTimeMax = 0;
	int i,j;

	if (LoadDomainData(domainFile, "Data\\domain.data", global_domain_infos))
	{
		// load time list
		int num;
		if( !domainFile.Read(&num, sizeof(int), &dwReadLen) )
		{
			a_LogOutput(1, "GlobalData_Load(), corrupted domain data file!");
			return false;
		}
		global_domain_vectime.clear();
		for(i=0; i<num; i++)
		{
			BattleTime theTime;
			if( !domainFile.Read(&theTime, sizeof(BattleTime), &dwReadLen) )
			{
				a_LogOutput(1, "GlobalData_Load(), corrupted domain data file!");
				return false;
			}
			global_domain_vectime.push_back(theTime);
		}
		if( !domainFile.Read(&global_domain_timemax, sizeof(int), &dwReadLen) )
		{
			a_LogOutput(1, "GlobalData_Load(), corrupted domain data file!");
			return false;
		}
		domainFile.Close();
	}
	else
	{
		return false;
	}

	// territory domain load
	/////////////////////////////////////////////////////////////////////////

	if (LoadDomainData(domainFile, "Data\\domain1.data", global_domain_infos2))
	{	
		// load time list
		int num;
		global_domain_vectime2.clear();
		if( !domainFile.Read(&num, sizeof(int), &dwReadLen) )
		{
			a_LogOutput(1, "GlobalData_Load(), corrupted domain data file!");
			return false;
		}
		for(i=0; i<num; i++)
		{
			BattleTime theTime;
			if( !domainFile.Read(&theTime, sizeof(BattleTime), &dwReadLen) )
			{
				a_LogOutput(1, "GlobalData_Load(), corrupted domain data file!");
				return false;
			}
			global_domain_vectime2.push_back(theTime);
		}
		if( !domainFile.Read(&global_domain_timemax2, sizeof(int), &dwReadLen) )
		{
			a_LogOutput(1, "GlobalData_Load(), corrupted domain data file!");
			return false;
		}
		domainFile.Close();
	}
	else
	{
		return false;
	}

	/////////////////////////////////////////////////////////////////////////////

	// now load land data
	AFileImage landFile;
	if( !landFile.Open("configs\\land.txt", AFILE_OPENEXIST | AFILE_TEXT) )
	{
		a_LogOutput(1, "GlobalData_Load(), Failed to open domain data file");
		return false;
	}
	
	char szBuffer[2000];
	while(landFile.ReadLine(szBuffer, 2000, &dwReadLen))
	{
		int id, p1, p2, p3, p4;
		char szName[20], szNeighbour[100];
		sscanf(szBuffer, "%d %s %d %d %d %d %s", &id, szName, &p1, &p2, &p3, &p4, szNeighbour);
		for(int i = 0; i < (int)global_domain_infos2.size(); i++)
			if( global_domain_infos2[i].id == id )
			{
				switch(p1)
				{
				case 1:
					global_domain_infos2[i].type = DOMAIN_TYPE_1ST_CLASS;
					break;
				case 2:
					global_domain_infos2[i].type = DOMAIN_TYPE_2ND_CLASS;
					break;
				case 3:
					global_domain_infos2[i].type = DOMAIN_TYPE_3RD_CLASS;
					break;
				default:
					global_domain_infos2[i].type = DOMAIN_TYPE_NULL;
					break;
				}
//				global_domain_infos2[i].type = p1;
				global_domain_infos2[i].challenge_itemid = p2;
				global_domain_infos2[i].item_lowerlimit = p3;
				global_domain_infos2[i].item_upperlimit = p4;
				global_domain_infos2[i].neighbours.clear(); 
				AString s(szNeighbour);
				abase::vector<AString> vec;
				// split the string 
				SplitString(vec, s, ";");
			
// 				CSplit s(szNeighbour);
// 				CSplit::VectorAString vec = s.Split(";");
				for(int j = 0; j < (int)vec.size(); j++)
				{
					global_domain_infos2[i].neighbours.push_back(atoi(vec[j]));
				}
				ACString strName = AS2AC(szName);
				a_sprintf(global_domain_infos2[i].name, _AL("%s"), strName.GetBuffer(0));
				break;
			}
	}
	landFile.Close();	


	// now load domain3 data

	if (!LoadDomainData(domainFile, "Data\\domain2.data", global_domain_infos3))
	{
		return false;
	}
	else
	{
		domainFile.Close();
	}

	// now load domain4 data
	if (!LoadDomainData(domainFile, "Data\\domain3.data", global_domain_infos4))
	{
		a_LogOutput(1, "GlobalData_Load(), domain3 load error!");
		return false;
	}
	else
	{
		domainFile.Close();
	}

	// now load domain5 data
	if (!LoadDomainData(domainFile, "Data\\domain4.data", global_domain_infos5))
	{
		a_LogOutput(1, "GlobalData_Load(), domain4 load error!");
		return false;
	}
	else
	{
		domainFile.Close();
	}


	// now load gshop items.
	AFileImage gshopFile;
	if( !gshopFile.Open("Data\\gshop.data", AFILE_OPENEXIST | AFILE_BINARY) )
	{
		a_LogOutput(1, "GlobalData_Load(), Failed to open domain data file");
		return false;
	}
	if( !gshopFile.Read(&global_gshop_timestamp, sizeof(DWORD), &dwReadLen) )
	{
		a_LogOutput(1, "GlobalData_Load(), corrupted gshop file!");
		return false;
	}
	int size;
	if( !gshopFile.Read(&size, sizeof(int), &dwReadLen) )
	{
		a_LogOutput(1, "GlobalData_Load(), corrupted gshop file!");
		return false;
	}
	for(i=0; i<size; i++)
	{
		GSHOP_ITEM item;
		if( !gshopFile.Read(&item, sizeof(item), &dwReadLen) )
		{
			a_LogOutput(1, "GlobalData_Load(), corrupted gshop file!");
			return false;
		}
		global_gshop_items.push_back(item);
	}
	int num_main;
	if( !gshopFile.Read(&num_main, sizeof(int), &dwReadLen) || num_main < 7 )
	{
		a_LogOutput(1, "GlobalData_Load(), corrupted gshop file!");
		return false;
	}
	for(i=0; i<num_main; i++)
	{
		GSHOP_MAIN_TYPE main_type;
		if( !gshopFile.Read(&main_type.id, sizeof(int), &dwReadLen) )
		{
			a_LogOutput(1, "GlobalData_Load(), corrupted gshop file!");
			return false;
		}
		if( !gshopFile.Read(&main_type.szName, sizeof(WORD) * 64, &dwReadLen) )
		{
			a_LogOutput(1, "GlobalData_Load(), corrupted gshop file!");
			return false;
		}
		int num_sub;
		if( !gshopFile.Read(&num_sub, sizeof(int), &dwReadLen) )
		{
			a_LogOutput(1, "GlobalData_Load(), corrupted gshop file!");
			return false;
		}
		for(j=0; j<num_sub; j++)
		{
			WORD szSubName[64];
			if( !gshopFile.Read(&szSubName, sizeof(WORD) * 64, &dwReadLen) )
			{
				a_LogOutput(1, "GlobalData_Load(), corrupted gshop file!");
				return false;
			}
			main_type.sub_types.push_back((WCHAR*)szSubName);
		}
		global_gshop_maintype_id_to_index.put(main_type.id, i);
		global_gshop_maintypes.push_back(main_type);
	}
	gshopFile.Close();

	//gshop 2
	if( !gshopFile.Open("Data\\gshop1.data", AFILE_OPENEXIST | AFILE_BINARY) )
	{
		a_LogOutput(1, "GlobalData_Load(), Failed to open gshop2 data file");
		return false;
	}
	if( !gshopFile.Read(&global_gshop_timestamp2, sizeof(DWORD), &dwReadLen) )
	{
		a_LogOutput(1, "GlobalData_Load(), corrupted gshop file!");
		return false;
	}
	
	if( !gshopFile.Read(&size, sizeof(int), &dwReadLen) )
	{
		a_LogOutput(1, "GlobalData_Load(), corrupted gshop file!");
		return false;
	}
	for(i=0; i<size; i++)
	{
		GSHOP_ITEM item;
		if( !gshopFile.Read(&item, sizeof(item), &dwReadLen) )
		{
			a_LogOutput(1, "GlobalData_Load(), corrupted gshop file!");
			return false;
		}
		global_gshop_items2.push_back(item);
	}
	if( !gshopFile.Read(&num_main, sizeof(int), &dwReadLen) || num_main < 7 )
	{
		a_LogOutput(1, "GlobalData_Load(), corrupted gshop file!");
		return false;
	}
	for(i=0; i<num_main; i++)
	{
		GSHOP_MAIN_TYPE main_type;
		if( !gshopFile.Read(&main_type.id, sizeof(int), &dwReadLen) )
		{
			a_LogOutput(1, "GlobalData_Load(), corrupted gshop file!");
			return false;
		}
		if( !gshopFile.Read(&main_type.szName, sizeof(WORD) * 64, &dwReadLen) )
		{
			a_LogOutput(1, "GlobalData_Load(), corrupted gshop file!");
			return false;
		}
		int num_sub;
		if( !gshopFile.Read(&num_sub, sizeof(int), &dwReadLen) )
		{
			a_LogOutput(1, "GlobalData_Load(), corrupted gshop file!");
			return false;
		}
		for(j=0; j<num_sub; j++)
		{
			WORD szSubName[64];
			if( !gshopFile.Read(&szSubName, sizeof(WORD) * 64, &dwReadLen) )
			{
				a_LogOutput(1, "GlobalData_Load(), corrupted gshop file!");
				return false;
			}
			main_type.sub_types.push_back((WCHAR*)szSubName);
		}
		global_gshop_maintype_id_to_index2.put(main_type.id, i);
		global_gshop_maintypes2.push_back(main_type);
	}
	gshopFile.Close();


	//gshop 3
	if( !gshopFile.Open("Data\\gshop2.data", AFILE_OPENEXIST | AFILE_BINARY) )
	{
		a_LogOutput(1, "GlobalData_Load(), Failed to open gshop3 data file");
		return false;
	}
	if( !gshopFile.Read(&global_gshop_timestamp3, sizeof(DWORD), &dwReadLen) )
	{
		a_LogOutput(1, "GlobalData_Load(), corrupted gshop file!");
		return false;
	}
	
	if( !gshopFile.Read(&size, sizeof(int), &dwReadLen) )
	{
		a_LogOutput(1, "GlobalData_Load(), corrupted gshop file!");
		return false;
	}
	for(i=0; i<size; i++)
	{
		GSHOP_ITEM item;
		if( !gshopFile.Read(&item, sizeof(item), &dwReadLen) )
		{
			a_LogOutput(1, "GlobalData_Load(), corrupted gshop file!");
			return false;
		}
		global_gshop_items3.push_back(item);
	}
	if( !gshopFile.Read(&num_main, sizeof(int), &dwReadLen) || num_main < 7 )
	{
		a_LogOutput(1, "GlobalData_Load(), corrupted gshop file!");
		return false;
	}
	for(i=0; i<num_main; i++)
	{
		GSHOP_MAIN_TYPE main_type;
		if( !gshopFile.Read(&main_type.id, sizeof(int), &dwReadLen) )
		{
			a_LogOutput(1, "GlobalData_Load(), corrupted gshop file!");
			return false;
		}
		if( !gshopFile.Read(&main_type.szName, sizeof(WORD) * 64, &dwReadLen) )
		{
			a_LogOutput(1, "GlobalData_Load(), corrupted gshop file!");
			return false;
		}
		int num_sub;
		if( !gshopFile.Read(&num_sub, sizeof(int), &dwReadLen) )
		{
			a_LogOutput(1, "GlobalData_Load(), corrupted gshop file!");
			return false;
		}
		for(j=0; j<num_sub; j++)
		{
			WORD szSubName[64];
			if( !gshopFile.Read(&szSubName, sizeof(WORD) * 64, &dwReadLen) )
			{
				a_LogOutput(1, "GlobalData_Load(), corrupted gshop file!");
				return false;
			}
			main_type.sub_types.push_back((WCHAR*)szSubName);
		}
		global_gshop_maintype_id_to_index3.put(main_type.id, i);
		global_gshop_maintypes3.push_back(main_type);
	}
	gshopFile.Close();

	//

	if( strstr(GetCommandLineA(), "globalsave") )
	{
		int	nCount;
		int i;

		FILE * fpServer = NULL;
		fpServer = fopen("domain.sev", "wb");
		if( NULL == fpServer ) return false;
		nCount = global_domain_infos2.size();
		fwrite(&nCount, 1, sizeof(int), fpServer);
		// now output server side domain info
		for(i=0; i<nCount; i++)
		{
			DOMAIN_INFO domain_info = global_domain_infos2[i];
			fwrite(&domain_info.id, 1, sizeof(int), fpServer);
			fwrite(&domain_info.type, 1, sizeof(int), fpServer);
//			fwrite(&domain_info.challenge_itemid, 1, sizeof(int), fpServer);
			fwrite(&domain_info.item_lowerlimit, 1, sizeof(int), fpServer);
			fwrite(&domain_info.item_upperlimit, 1, sizeof(int), fpServer);
			int nNumNeighbours = domain_info.neighbours.size();
			fwrite(&nNumNeighbours, 1, sizeof(int), fpServer);
			for(int j=0; j<nNumNeighbours; j++)
				fwrite(&domain_info.neighbours[j], 1, sizeof(int), fpServer);
		}

		// output time list
		nCount = global_domain_vectime2.size();
		fwrite(&nCount, sizeof(int), 1, fpServer);
		for(i=0; i<nCount; i++)
			fwrite(&global_domain_vectime2[i], sizeof(BattleTime), 1, fpServer);
		fwrite(&nTimeMax, sizeof(int), 1, fpServer);

		fclose(fpServer);

		if( !globaldata_loadserver() )
			return false;
		globaldata_releaseserver();
	}

	if ( !LoadVIPAwardData() ) //加载VIP奖励数据Added 2012-02-01
	{
		return false;
	}

	return true;
}

bool globaldata_save_to(const char * szPath)
{
	FILE * file;
	size_t i, j;
	char szFullPath[MAX_PATH];
	
	// save domain.data
	sprintf(szFullPath, "%s%s", szPath, "data\\domain.data");
	DWORD flag = GetFileAttributesA(szFullPath);
	if (flag & FILE_ATTRIBUTE_READONLY) 
	{
		flag ^= FILE_ATTRIBUTE_READONLY;
		SetFileAttributesA(szFullPath,flag);
	}
	file = fopen(szFullPath, "wb");
	if( NULL == file )
		return false;

	size_t nNumDomains = global_domain_infos.size();
	fwrite(&nNumDomains, sizeof(int), 1, file);

	for(i=0; i<nNumDomains; i++)
	{
		const DOMAIN_INFO &theDomain = global_domain_infos[i];

		fwrite(&theDomain.name, 16 * sizeof(WORD), 1, file);
		fwrite(&theDomain.id, sizeof(int), 1, file);
		fwrite(&theDomain.type, sizeof(int), 1, file);
		fwrite(&theDomain.reward, sizeof(int), 1, file);
		fwrite(&theDomain.center, sizeof(A3DPOINT2), 1, file);

		size_t num = theDomain.verts.size();
		fwrite(&num, sizeof(int), 1, file);
		for(j=0; j<num; j++)
		{
			const A3DPOINT2& vert = theDomain.verts[j];
			fwrite(&vert, sizeof(A3DPOINT2), 1, file);
		}

		num = theDomain.faces.size()/3;
		fwrite(&num, sizeof(int), 1, file);
		for(j=0; j<num * 3; j++)
		{
			fwrite(&theDomain.faces[j], sizeof(int), 1, file);
		}

		num = theDomain.neighbours.size();
		fwrite(&num, sizeof(int), 1, file);
		for(j=0; j<num; j++)
		{
			fwrite(&theDomain.neighbours[j], sizeof(int), 1, file);
		}
	}
	// skip time list (not used)
	size_t num = global_domain_vectime.size();
	fwrite(&num, sizeof(int), 1, file);
	for (i=0; i<num; i++)
	{
		fwrite(&global_domain_vectime[i], sizeof(BattleTime), 1, file);
	}
	fwrite(&global_domain_timemax, sizeof(int), 1, file);
	fclose(file);

	// save domain1.data
	sprintf(szFullPath, "%s%s", szPath, "data\\domain1.data");
	flag = GetFileAttributesA(szFullPath);
	if (flag & FILE_ATTRIBUTE_READONLY) 
	{
		flag ^= FILE_ATTRIBUTE_READONLY;
		SetFileAttributesA(szFullPath,flag);
	}
	file = fopen(szFullPath, "wb");
	if( NULL == file )
		return false;

	nNumDomains = global_domain_infos2.size();
	fwrite(&nNumDomains, sizeof(int), 1, file);
	for(i=0; i<nNumDomains; i++)
	{
		const DOMAIN_INFO &theDomain = global_domain_infos2[i];

		fwrite(&theDomain.name, 16 * sizeof(WORD), 1, file);
		fwrite(&theDomain.id, sizeof(int), 1, file);
		fwrite(&theDomain.type, sizeof(int), 1, file);
		fwrite(&theDomain.reward, sizeof(int), 1, file);
		fwrite(&theDomain.center, sizeof(A3DPOINT2), 1, file);

		size_t num = theDomain.verts.size();
		fwrite(&num, sizeof(int), 1, file);
		for(j=0; j<num; j++)
		{
			const A3DPOINT2& vert = theDomain.verts[j];
			fwrite(&vert, sizeof(A3DPOINT2), 1, file);
		}

		num = theDomain.faces.size()/3;
		fwrite(&num, sizeof(int), 1, file);
		for(j=0; j<num * 3; j++)
		{
			fwrite(&theDomain.faces[j], sizeof(int), 1, file);
		}

		num = theDomain.neighbours.size();
		fwrite(&num, sizeof(int), 1, file);
		for(j=0; j<num; j++)
		{
			fwrite(&theDomain.neighbours[j], sizeof(int), 1, file);
		}
	}
	// time list
	num = global_domain_vectime2.size();
	fwrite(&num, sizeof(int), 1, file);
	for(i=0; i<num; i++)
	{
		fwrite(&global_domain_vectime2[i], sizeof(BattleTime), 1, file);
	}
	fwrite(&global_domain_timemax2, sizeof(int), 1, file);
	fclose(file);


	// now save gshop items.
	sprintf(szFullPath, "%s%s", szPath, "data\\gshop.data");
	flag = GetFileAttributesA(szFullPath);
	if (flag & FILE_ATTRIBUTE_READONLY) 
	{
		flag ^= FILE_ATTRIBUTE_READONLY;
		SetFileAttributesA(szFullPath,flag);
	}
	file = fopen(szFullPath, "wb");
	if( NULL == file )
		return false;

	fwrite(&global_gshop_timestamp, 1, sizeof(DWORD), file);
	size_t size = global_gshop_items.size();
	fwrite(&size, 1, sizeof(DWORD), file);
	for(i=0; i<size; i++)
	{
		GSHOP_ITEM item = global_gshop_items[i];
		fwrite(&item, 1, sizeof(item), file);
	}

	size_t nummain = global_gshop_maintypes.size();
	fwrite(&nummain, 1, sizeof(int), file);

	for(i=0; i<nummain; i++)
	{
		const GSHOP_MAIN_TYPE& main_type = global_gshop_maintypes[i];
		fwrite(&main_type.id, sizeof(int), 1, file);
		fwrite(main_type.szName, sizeof(WORD) * 64, 1, file);
		size_t num_sub = main_type.sub_types.size();
		fwrite(&num_sub, sizeof(int), 1, file);
		for(j=0; j<num_sub; j++)
		{
			WORD szSubName[64];
			wcscpy((WCHAR*)szSubName, main_type.sub_types[j]);
			fwrite(szSubName, sizeof(WORD) * 64, 1, file);
		}
	}
	fclose(file);
	
	// now save gshop1 items.
	sprintf(szFullPath, "%s%s", szPath, "data\\gshop1.data");
	DWORD flag2 = GetFileAttributesA(szFullPath);
	if (flag2 & FILE_ATTRIBUTE_READONLY) 
	{
		flag2 ^= FILE_ATTRIBUTE_READONLY;
		SetFileAttributesA(szFullPath,flag2);
	}
	file = fopen(szFullPath, "wb");
	if( NULL == file )
		return false;

	fwrite(&global_gshop_timestamp2, 1, sizeof(DWORD), file);
	size_t size2 = global_gshop_items2.size();
	fwrite(&size2, 1, sizeof(DWORD), file);
	for(i=0; i<size2; i++)
	{
		GSHOP_ITEM item = global_gshop_items2[i];
		fwrite(&item, 1, sizeof(item), file);
	}

	size_t nummain2 = global_gshop_maintypes2.size();
	fwrite(&nummain2, 1, sizeof(int), file);

	for(i=0; i<nummain2; i++)
	{
		const GSHOP_MAIN_TYPE& main_type = global_gshop_maintypes2[i];
		fwrite(&main_type.id, sizeof(int), 1, file);
		fwrite(main_type.szName, sizeof(WORD) * 64, 1, file);
		size_t num_sub = main_type.sub_types.size();
		fwrite(&num_sub, sizeof(int), 1, file);
		for(j=0; j<num_sub; j++)
		{
			WORD szSubName[64];
			wcscpy((WCHAR*)szSubName, main_type.sub_types[j]);
			fwrite(szSubName, sizeof(WORD) * 64, 1, file);
		}
	}
	fclose(file);

	// now save gshop2 items.
	sprintf(szFullPath, "%s%s", szPath, "data\\gshop2.data");
	DWORD flag3 = GetFileAttributesA(szFullPath);
	if (flag3 & FILE_ATTRIBUTE_READONLY) 
	{
		flag3 ^= FILE_ATTRIBUTE_READONLY;
		SetFileAttributesA(szFullPath,flag3);
	}
	file = fopen(szFullPath, "wb");
	if( NULL == file )
		return false;

	fwrite(&global_gshop_timestamp3, 1, sizeof(DWORD), file);
	size_t size3 = global_gshop_items3.size();
	fwrite(&size3, 1, sizeof(DWORD), file);
	for(i=0; i<size3; i++)
	{
		GSHOP_ITEM item = global_gshop_items3[i];
		fwrite(&item, 1, sizeof(item), file);
	}

	size_t nummain3 = global_gshop_maintypes3.size();
	fwrite(&nummain3, 1, sizeof(int), file);

	for(i=0; i<nummain3; i++)
	{
		const GSHOP_MAIN_TYPE& main_type = global_gshop_maintypes3[i];
		fwrite(&main_type.id, sizeof(int), 1, file);
		fwrite(main_type.szName, sizeof(WORD) * 64, 1, file);
		size_t num_sub = main_type.sub_types.size();
		fwrite(&num_sub, sizeof(int), 1, file);
		for(j=0; j<num_sub; j++)
		{
			WORD szSubName[64];
			wcscpy((WCHAR*)szSubName, main_type.sub_types[j]);
			fwrite(szSubName, sizeof(WORD) * 64, 1, file);
		}
	}
	fclose(file);

	return true;
}

bool globaldata_release()
{
	global_domain_infos.clear();
	global_domain_infos2.clear();
	global_domain_infos3.clear();
	global_domain_infos4.clear();

	global_gshop_items.clear();
	global_gshop_maintypes.clear();

	global_gshop_items2.clear();
	global_gshop_maintypes2.clear();    

	global_gshop_items3.clear();
	global_gshop_maintypes3.clear();    

	return true;
}

bool globaldata_loadserver()
{
	FILE * fpServer = NULL;
	int nCount, i;
	
	fpServer = fopen("domain.sev", "rb");
	if( !fpServer )	return false;
	fread(&nCount, 1, sizeof(int), fpServer);
	for(i=0; i<nCount; i++)
	{
		DOMAIN_INFO_SERV domain_info;
		fread(&domain_info.id, 1, sizeof(int), fpServer);
		fread(&domain_info.type, 1, sizeof(int), fpServer);
//		fread(&domain_info.challenge_itemid, 1, sizeof(int), fpServer);
		fread(&domain_info.item_lowerlimit, 1,sizeof(int), fpServer);
		fread(&domain_info.item_upperlimit, 1, sizeof(int), fpServer);
		int nNumNeighbours;
		fread(&nNumNeighbours, 1, sizeof(int), fpServer);
		for(int j=0; j<nNumNeighbours; j++)
		{
			int idNeighbour;
			fread(&idNeighbour, 1, sizeof(int), fpServer);
			domain_info.neighbours.push_back(idNeighbour);
		}
		global_domain_infos_server.push_back(domain_info);
	}
	return true;
}

bool globaldata_releaseserver()
{
	global_domain_infos_server.clear();
	return true;
}

void SplitString(vector<AString>& vec, AString& s, const char* split)
{
	char *str = new char[s.GetLength() + 1];
	ASSERT(str);
	strcpy(str, s);

	char *pchStart = str, *pch = NULL;
	while( true )
	{
		pch = strstr(pchStart, split);
		if( pch ) *pch = '\0';

		if( strlen(pchStart) > 0 )
			vec.push_back(pchStart);

		if( !pch ) break;

		pchStart = pch + strlen(split);
	}

	delete str;
}

bool LoadDomainData(AFileImage& domainFile, const char* szFile, vector<DOMAIN_INFO>& domain_info)
{
	DWORD dwReadLen;
	int nNumDomains;
	int i,j;
	if( !domainFile.Open(szFile, AFILE_OPENEXIST | AFILE_BINARY) )
	{
		a_LogOutput(1, "GlobalData_Load(), Failed to open domain data file");
		return false;
	}

	if( !domainFile.Read(&nNumDomains, sizeof(int), &dwReadLen) )
	{
		a_LogOutput(1, "GlobalData_Load(), corrupted domain data file!");
		return false;
	}

	for(i=0; i<nNumDomains; i++)
	{
		DOMAIN_INFO		theDomain;
		int				num;

		memset(&theDomain, 0, sizeof(DOMAIN_INFO));
		if( !domainFile.Read(&theDomain.name, 16 * sizeof(WORD), &dwReadLen) )
		{
			a_LogOutput(1, "GlobalData_Load(), corrupted domain data file!");
			return false;
		}

		if( !domainFile.Read(&theDomain.id, sizeof(int), &dwReadLen) )
		{
			a_LogOutput(1, "GlobalData_Load(), corrupted domain data file!");
			return false;
		}
		if( !domainFile.Read(&theDomain.type, sizeof(int), &dwReadLen) )
		{
			a_LogOutput(1, "GlobalData_Load(), corrupted domain data file!");
			return false;
		}
		if( !domainFile.Read(&theDomain.reward, sizeof(int), &dwReadLen) )
		{
			a_LogOutput(1, "GlobalData_Load(), corrupted domain data file!");
			return false;
		}

		if( !domainFile.Read(&theDomain.center, sizeof(A3DPOINT2), &dwReadLen) )
		{
			a_LogOutput(1, "GlobalData_Load(), corrupted domain data file!");
			return false;
		}

		if( !domainFile.Read(&num, sizeof(int), &dwReadLen) )
		{
			a_LogOutput(1, "GlobalData_Load(), corrupted domain data file!");
			return false;
		}
		for(j=0; j<num; j++)
		{
			A3DPOINT2 vert;
			if( !domainFile.Read(&vert, sizeof(A3DPOINT2), &dwReadLen) )
			{
				a_LogOutput(1, "GlobalData_Load(), corrupted domain data file!");
				return false;
			}
			theDomain.verts.push_back(vert);
		}

		if( !domainFile.Read(&num, sizeof(int), &dwReadLen) )
		{
			a_LogOutput(1, "GlobalData_Load(), corrupted domain data file!");
			return false;
		}
		for(j=0; j<num * 3; j++)
		{
			int		index;
			if( !domainFile.Read(&index, sizeof(int), &dwReadLen) )
			{
				a_LogOutput(1, "GlobalData_Load(), corrupted domain data file!");
				return false;
			}
			theDomain.faces.push_back(index);
		}

		if( !domainFile.Read(&num, sizeof(int), &dwReadLen) )
		{
			a_LogOutput(1, "GlobalData_Load(), corrupted domain data file!");
			return false;
		}
		for(j=0; j<num; j++)
		{
			int		id;
			if( !domainFile.Read(&id, sizeof(int), &dwReadLen) )
			{
				a_LogOutput(1, "GlobalData_Load(), corrupted domain data file!");
				return false;
			}
			theDomain.neighbours.push_back(id);
		}
		domain_info.push_back(theDomain);
	}
//	domainFile.Close();
	return true;
}