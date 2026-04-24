/********************************************************************
	created:	2008/05/19
	author:		kuiwu
	
	purpose:	
	Copyright (C) 2008 - All Rights Reserved
*********************************************************************/

#include <stdio.h>
#include <AString.h>
#include <vector.h>
#include <hashtab.h>
#include <A3DMatrix.h>
#include <A3DFuncs.h>
#include "ConvexHullData.h"
#include "aabbcd.h"
#include <AF.h>
#include <AFilePackMan.h>
#include "..\\ZCommon\\elementdataman.h"
#include "..\\ZElementClient\DataPathMan.h"
#include "EC_MD5Hash.h"


using namespace CHBasedCD;
//define
typedef abase::vector<CConvexHullData*>    ChdVec;
typedef abase::hashtab<ChdVec, int, abase::_hash_function>	EcmTable;

#define ECMODEL_PROJ_VERSION (unsigned int) (0x00000001)


const DWORD NMD_FILE_MAGIC = (DWORD)(('n'<<24)| ('m'<<16)|('d'<<8)|('f'));
const DWORD NMD_FILE_VERSION = 1;

const DWORD ELEMENTDATA_PRG_VERSION = 0;
const DWORD ELEMENTDATA_DAT_VERSION = ELEMENTDATA_VERSION;



struct ECModelBrushProjHead_t
{
	unsigned int	nMagic;
	unsigned int	nVersion;
	char			szECModelFile[MAX_PATH];
	int				nFrame;
	unsigned int	nNumHull;
};




//data
elementdataman   g_ElementDataMan;
DataPathMan		 g_DataPathMan;
DataPathMan		 g_DynDataPathMan;
AString          g_szWorkDir;
EcmTable		 g_NpcMine(128);
EcmTable		 g_DynObj(128);
bool			 g_FromPck = true;
const char  *    g_EcpPath = "Models\\ECModelHull";
AString          g_NmdPath;
bool			 g_Verbose	= false;
bool             g_FatalError = false;
//func
void PrintHelp();
void Init();
void OpenPck();
void Release();
void ReleaseChdVec(ChdVec& chdVec);
void ReleaseEcmTab(EcmTable& ecmTab);
void AddNpc(const NPC_ESSENCE * ess);
void AddMine(const MINE_ESSENCE * ess);
bool RetrieveEcpPath(const char * ecmPath, AString& ecpPath);
bool AddEcm(const char * path, const char * name, int id, EcmTable& ecmTab);
bool AddEcp(const char * path, const char * name, int id, EcmTable& ecmTab);
void ImportData();
bool ExportData(const char * path);
void ExportEcmTab(AFile& file, EcmTable& ecmTab);
bool RetrieveConfigInfo(const char * szFilePath);
void Panic();
//////////////////////////////////////////////////////////////////////////

void PrintHelp()
{
	printf("Export ZhuXian npc, mine and dynobj chd.\n");
	printf("author: kuiwu.\n");
	printf("usage:  zxNMD config_file\n");
}

void Panic()
{
	int *p = 0x0;
	*p = 100;
}

void OpenPck()
{
	const char * pcks[] = 
	{
		"models.pck",
		NULL
	};
		
	int i = 0;
	while (pcks[i])
	{
		if (!g_AFilePackMan.OpenFilePackage(pcks[i]))
		{
			g_FatalError = true;
			break;
		}
		
		++i;
	}
}
void Init()
{
	af_Initialize();
	af_SetBaseDir(g_szWorkDir);


	if (g_FromPck)
	{
		OpenPck();
	}
	
}

void ReleaseChdVec(ChdVec& chdVec)
{
	ChdVec::iterator  it;
	for (it = chdVec.begin(); it != chdVec.end(); ++it)
	{
		delete *it;
	}

	chdVec.clear();
}

void ReleaseEcmTab(EcmTable& ecmTab)
{
	EcmTable::iterator it;
	for (it = ecmTab.begin(); it != ecmTab.end(); ++it)
	{
		ReleaseChdVec(*it.value());
	}
	ecmTab.clear();
}


void Release()
{
	ReleaseEcmTab(g_NpcMine);
	ReleaseEcmTab(g_DynObj);
	af_Finalize();
}

bool RetrieveEcpPath(const char * ecmPath, AString& ecpPath)
{
	const char* szPath = strstr(ecmPath, "\\");
	if (!szPath)
	{
		return false;
	}
	char szProjFile[MAX_PATH];
	strcpy(szProjFile, g_EcpPath);
	strcat(szProjFile, szPath);

	char* ext = strrchr(szProjFile, '.');
	if (!ext)
	{
		return false;
	}
	strcpy(ext, ".ecp");

	ecpPath = szProjFile;
	return true;
}

bool AddEcp(const char * path, const char * name, int id, EcmTable& ecmTab)
{
	
	AFileImage file;
	if (!af_IsFileExist(path) || !file.Open(path, AFILE_OPENEXIST | AFILE_BINARY))
	{
		if (g_Verbose)
		{
			printf("%d %s open %s fail\n", id, name, path);
		}
		return false;
	}
	
	ECModelBrushProjHead_t ProjHead;
	DWORD dwLen;

	file.Read(&ProjHead, sizeof(ProjHead), &dwLen);
	
	if (ProjHead.nVersion != ECMODEL_PROJ_VERSION)
	{
		if (g_Verbose)
		{
			printf("ecp %s wrong version\n", path);
		}
		return false;
	}

	ChdVec   chd_vec;
	for (unsigned i = 0; i < ProjHead.nNumHull; i++)
	{
		CConvexHullData* pHull = new CConvexHullData;
		pHull->LoadBinaryData(&file);
		// currently, the ECModelHullEditor has turned the model 180 degree which is a mistake,
		// so we have to turn it back before we use it.
		pHull->Transform(TransformMatrix(A3DVECTOR3(0, 0, -1.0f), A3DVECTOR3(0, 1.0f, 0), A3DVECTOR3(0)));
		chd_vec.push_back(pHull);
	}

	if(!ecmTab.put(id, chd_vec))
	{
		ReleaseChdVec(chd_vec);
		if (g_Verbose)
		{
			printf("add %d %s to table, fail\n", id, name);
		}
		return false;
	}



	return true;
}

bool AddEcm(const char * path, const char * name, int id, EcmTable& ecmTab)
{
	AString ecp_path;
	if (!RetrieveEcpPath(path, ecp_path))
	{
		if (g_Verbose)
		{
			printf("retrieve %d %s ecp path from %s, fail\n", id, name, path);
		}
		return false;
	}

	if (!g_FromPck)
	{
		ecp_path.Format("%s%s", g_szWorkDir, ecp_path);
	}
	
	return AddEcp(ecp_path, name, id, ecmTab);
}

void AddNpc(const NPC_ESSENCE * ess)
{
	const char * szPath = g_DataPathMan.GetPathByID(ess->file_model);
	
	if (!szPath)
	{
		if (g_Verbose)
		{
			printf("npc %d null path\n", ess->id);
		}
		return;
	}

	AString name;
	name = WC2AS(ess->name);
	
	AddEcm(szPath, name, ess->id, g_NpcMine);
}

void AddMine(const MINE_ESSENCE * ess)
{
	const char * szPath = g_DataPathMan.GetPathByID(ess->file_model);
	
	if (!szPath)
	{
		if (g_Verbose)
		{
			printf("mine %d null path\n", ess->id);
		}
		return;
	}

	AString name;
	name = WC2AS(ess->name);
	
	AddEcm(szPath, name, ess->id, g_NpcMine);
}


void ImportData()
{
	g_MD5Hash.init();
	g_MD5Hash.Update("ZPWDATA", 7);

	AString data_path;
	data_path.Format("%sdata\\path.data", g_szWorkDir);
	if (g_DataPathMan.Load(data_path) == 0)
	{
		if (g_Verbose)
		{
			printf("load %s fail\n", data_path);
		}
		return;
	}

	printf("load pathdata from %s\n", data_path);
	data_path.Format("%sdata\\elements.data",g_szWorkDir);
	
	if (g_ElementDataMan.load_data(data_path) < 0)
	{
		if (g_Verbose)
		{
			printf("load %s fail\n", data_path);
		}
		return;
	}
	printf("load elementdata from %s\n", data_path);

	data_path.Format("%sdata\\dynamicobjects.data", g_szWorkDir);
	if (g_DynDataPathMan.Load(data_path) == 0)
	{
		if (g_Verbose)
		{
			printf("load %s fail\n", data_path);
		}
		g_FatalError = true;
		return;
	}
	printf("load dyn from %s\n", data_path);
	


	DATA_TYPE DataType = DT_INVALID;
	int tid = g_ElementDataMan.get_first_data_id(ID_SPACE_ESSENCE, DataType);

	while (tid)
	{
		const void* pData = g_ElementDataMan.get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);

		if (DataType == DT_NPC_ESSENCE)
		{
			const NPC_ESSENCE* pNpcEss = (const NPC_ESSENCE*)pData;
			AddNpc(pNpcEss);
		}
		else if (DataType == DT_MINE_ESSENCE)
		{
			const MINE_ESSENCE * pMineEss = (const MINE_ESSENCE*)pData;
			AddMine(pMineEss);
		}
		//	Get next item
		tid = g_ElementDataMan.get_next_data_id(ID_SPACE_ESSENCE, DataType);
	}

	printf("import %d npc&mine\n", g_NpcMine.size());


	const abase::hash_map<unsigned long, AString> * pIdPathMap = g_DynDataPathMan.GetIdPathMap();
	abase::hash_map<unsigned long, AString>::const_iterator it;
	for (it = pIdPathMap->begin(); it != pIdPathMap->end(); ++it)
	{
		const	AString& path = (it.value())->second;
		unsigned long id = *(it.key());
		AddEcm(path, "dyn", id, g_DynObj);
	}
	printf("import %d dynobj\n", g_DynObj.size());

}

void ExportEcmTab(AFile& file, EcmTable& ecmTab)
{
	int count;
	EcmTable::iterator e_it;
	ChdVec::iterator   c_it;
	DWORD dwLen;

	count = ecmTab.size();
	


	file.Write(&count, sizeof(int), &dwLen);

	for (e_it = ecmTab.begin(); e_it != ecmTab.end(); ++e_it)
	{
		int id =  *e_it.key();
		file.Write(&id, sizeof(int), &dwLen);
		ChdVec& chdVec = *e_it.value();
		count = chdVec.size();
		file.Write(&count, sizeof(int), &dwLen);
		for (c_it = chdVec.begin(); c_it != chdVec.end(); ++c_it)
		{
			(*c_it)->SaveBinaryData(&file);			
		}
	}
}

bool ExportData(const char * path)
{
	AFile  file;
	if (!file.Open(path, AFILE_BINARY|AFILE_CREATENEW|AFILE_NOHEAD))
	{
		printf("open %s fail\n", path);
		g_FatalError = true;
		return false;
	}

	printf("export npc,mine and dyn to %s\n", path);
	
	DWORD flag;
	DWORD dwLen;

	//header
	flag = NMD_FILE_MAGIC;
	file.Write(&flag, sizeof(DWORD), &dwLen);
	flag = NMD_FILE_VERSION;
	file.Write(&flag, sizeof(DWORD), &dwLen);
	flag = ELEMENTDATA_PRG_VERSION;
	file.Write(&flag, sizeof(DWORD), &dwLen);
	flag = ELEMENTDATA_DAT_VERSION;
	file.Write(&flag, sizeof(DWORD), &dwLen);
	
	ExportEcmTab(file, g_NpcMine);
	ExportEcmTab(file, g_DynObj);
	
	return true;
}

bool RetrieveConfigInfo(const char * szFilePath)
{
	FILE * fp = fopen(szFilePath, "r");
	if (!fp)
	{
		printf("open %s fail\n", szFilePath);
		return false;
	}

	char line[1000];
	char * token;
	char seps[]   = " =\t\n";

	while (fgets(line, 1000, fp))
	{
		token = strtok(line, seps);
		
		if (!token || token[0] == '#')
		{
			continue;
		}

		if (strcmp(token, "work_dir") == 0)
		{
			token = strtok(NULL, seps);
			g_szWorkDir= token;
			int len = strlen(token); 
			if (token[len-1] != '\\' && token[len-1] != '/')
			{
				g_szWorkDir += "\\";
			}
		}
		else if (strcmp(token, "out_file") == 0)
		{
			token = strtok(NULL, seps);
			 g_NmdPath = token;
		}
		else if (strcmp(token, "verbose") == 0)
		{
			token = strtok(NULL, seps);
			g_Verbose = (atoi(token) == 1);
		}
		else if (strcmp(token, "from_pck") == 0)
		{
			token = strtok(NULL, seps);
			g_FromPck = (atoi(token) == 1);
		}

	}

	fclose(fp);

	return true;
}

void Test()
{
	int tplId = 2;
	EcmTable::iterator it  = g_NpcMine.find(tplId);
	if (it == g_NpcMine.end())
	{
		return;
	}

	ChdVec * chd_vec = it.value();

	A3DVECTOR3 pos(0.0f, 180.0f, 0.0f);
	A3DVECTOR3 up(0.0f, 1.0f, 0.0f);
	A3DVECTOR3 dir(1.0f, 0.0f, 0.0f);

	A3DMATRIX4 mat;
	mat = a3d_TransformMatrix(dir, up, pos);

	BrushTraceInfo  trcInfo;
	trcInfo.Init(A3DVECTOR3(0.0f), A3DVECTOR3(0.0f), A3DVECTOR3(512.0f, 5000.0f, 512.0f));

	int i;
	bool ret;
	for (i = 0; i < chd_vec->size(); ++i)
	{
		CConvexHullData * chd =	chd_vec->at(i);
		chd->Transform(mat);		

		int j;
		printf("=======chd %d=======\n", i);
		int count;
		count = chd->GetVertexNum();
		for (j = 0; j < count; ++j)
		{
			A3DVECTOR3 pos = chd->GetVertex(j);
			printf("vert %d %f %f %f\n", j, pos.x, pos.y, pos.z);
		}
		count = chd->GetFaceNum();
		for (j = 0; j <count; ++j)
		{
			CFace * face = chd->GetFacePtr(j);
			A3DVECTOR3 norm = face->GetNormal();
			float d = face->GetDist();
			printf("face %d %f %f %f %f\n", j, norm.x, norm.y, norm.z, d);
		}



		CQBrush qBrush;
		qBrush.AddBrushBevels(chd);
		CCDBrush cdBrush;
		qBrush.Export(&cdBrush);

		ret = cdBrush.Trace(&trcInfo);

		printf("trace ret %d\n", ret);
	}

}



int main(int argc, char* argv[])
{
	PrintHelp();
	

	if (argc < 2)
	{
		Panic();
		return 0;
	}

	if (!RetrieveConfigInfo(argv[1]))
	{
		Panic();
		return 0;
	}

	Init();
	ImportData();
	ExportData(g_NmdPath);

	//Test();
	
	Release();
	printf("Done!\n");

	if (g_FatalError)
	{
		Panic();
	}
	return 0;
}