// ElementResMan.cpp: implementation of the ElementResMan class.
//
//////////////////////////////////////////////////////////////////////

#include "global.h"
#include "elementeditor.h"
#include "ec_md5hash.h"
#include "Render.h"
#include "VssOperation.h"
#include "Shlwapi.h"
#include "ElementResMan.h"
#include "SceneObjectManager.h"

#include "A3d.h"
#include "EL_Building.h"
#include "EL_Forest.h"
#include "EL_Tree.h"

//#define new A_DEBUG_NEW

char folder_name[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','1','2','3','4','5','6','7','8','9'};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ElementResMan::ElementResMan()
{
	for(int i = 0; i < RES_TYPE_COUNT; i++)
	mIdCount[i] = 0;
}

ElementResMan::~ElementResMan()
{
	ReleaseResTable();
}

bool ElementResMan::AddRes( const int iType,const char *szName, char *szNewName)
{
	if(szName==NULL) return false;
	if(iType >= RES_TYPE_COUNT || iType < 0) return false;
	
	bool bResult = false;
	
	switch( iType )
	{
	case RES_TYPE_BUILDING:
		 bResult = ProcessBuildingRes(szName,szNewName);
	case RES_TYPE_GRASS:
		 //bResult = ProcessGrassRes(szName);
		 break;
	case RES_TYPE_TREE:
		 //bResult = ProcessTreeRes(szName,szNewName);
		 break;
	case RES_TYPE_GFX:
	     //bResult = ProcessGfxRes(szName);
		 break;
	}
	return bResult;
}

bool ElementResMan::ProcessBuildingRes(const char *szName, char *szNewName)
{
	CString strPath,strName;
	GetPathAndName(szName,strPath,strName);
	if(ResIsExistByName(RES_TYPE_BUILDING,strName)!=NULL) 
	{
		AfxMessageBox("资源已经存在于资源管理器中！不能加入，只能替换。");
		return false;
	}
	
	Res_Item* pNewItem = new Res_Item;
	if(pNewItem==NULL) 
	{
		g_Log.Log("ElementResMan::ProcessBuildingRes(),没有足够的内存!");
		return false;
	}

	//检验文件是否已经加入
	
	CELBuilding mModel;
	CString strPathName = szName;
	strPathName.MakeLower();
	strPathName.Replace(".mox",".chf");
	if(!mModel.LoadFromMOXAndCHF(g_Render.GetA3DDevice(),szName,strPathName))
	{
		CString msg;
		msg.Format("不能加载文件%s",msg);
		AfxMessageBox(msg);
		delete pNewItem;
		return false;
	}
	
	int folder_index = ResTable[RES_TYPE_BUILDING].GetCount()/300;	
	sprintf(pNewItem->szID,"Building\\Models\\%d\\%s",folder_index,strName);
	strcpy(pNewItem->szOrgName,TransOrgName(szName));
	//更改模型纹理路径，并把纹理文件加入到资源管理器中 
	abase::hash_map<AString, AString> tempMap;
	int nMesh = mModel.GetA3DLitModel()->GetNumMeshes();
	for( int i = 0; i < nMesh; i++)
	{
		A3DLitMesh *pMesh = mModel.GetA3DLitModel()->GetMesh(i);
		if(pMesh)
		{
			abase::hash_map<AString, AString>::iterator it = tempMap.find(pMesh->GetTextureMap());
			AString szTextures = it == tempMap.end() ? "" : it->second;;
			if(szTextures.IsEmpty())
			{
				szTextures = ProcessBuildingTextureRes(pMesh->GetTextureMap());
				if(szTextures.IsEmpty())
				{
					g_Log.Log("ElementResMan::ProcessBuildingRes(),纹理 %s 加入失败!", pMesh->GetTextureMap());
					mModel.Release();
					delete pNewItem;
					return false;
				}
				tempMap[pMesh->GetTextureMap()] = szTextures;
			}
			pMesh->ChangeTextureMap(szTextures);
		}
	}
	tempMap.clear();

	//拷贝模型到building\models下
	CString path,name;
	name = pNewItem->szID;
	GetPathAndName(name,path,name);
	CreateFolder(path);
	AFile sFile;
	if(!sFile.Open(pNewItem->szID,AFILE_CREATENEW))
	{
		g_Log.Log("ElementResMan::ProcessBuildingRes(),创建新文件%s失败!",pNewItem->szID);
		mModel.Release();
		delete pNewItem;
		return false;
	}
	if(!mModel.Save(&sFile))
	{
		g_Log.Log("ElementResMan::ProcessBuildingRes(),保存文件%s失败!",pNewItem->szID);
		mModel.Release();
		delete pNewItem;
		return false;
	}
	sFile.Close();
	mModel.Release();
	
	CString dstPath,dstName;
	GetPathAndName(path,dstPath,dstName);
	g_VSS.SetProjectPath(dstPath);
	g_VSS.CreateProject(dstName);
	g_VSS.SetProjectPath(path);
	g_VSS.AddFile(pNewItem->szID);

	//文件写入资源表
	strcpy(szNewName,pNewItem->szID);
	ResTable[RES_TYPE_BUILDING].AddTail(pNewItem);
	return true;
}

bool ElementResMan::ProcessGfxRes( const char *szName)
{
	return true;
}

char* ElementResMan::ProcessBuildingTextureRes( const char *szName)
{
	CString sPath,sName;
	GetPathAndName(szName,sPath,sName);
	Res_Item* pOldItem = ResIsExistByName(RES_TYPE_BUILDING_TEXTURE, sName);
	Res_Item* pNewItem = NULL;
	
	if(pOldItem==NULL)
	{
		pNewItem = new Res_Item;
		if(pNewItem==NULL) 
		{
			g_Log.Log("ElementResMan::ProcessTextureRes(),没有足够的内存!");
			return NULL;
		}	
		
		int folder_index = ResTable[RES_TYPE_BUILDING_TEXTURE].GetCount()/300;
		sprintf(pNewItem->szID,"Building\\textures\\%d\\%s",folder_index,sName);
	}else pNewItem = pOldItem;

	strcpy(pNewItem->szOrgName,TransOrgName(szName));
	//下面拷贝文件到Building\textures
	if(!CopyResFile(pNewItem->szID,szName))
	{
		if(pOldItem==NULL)
		{
			delete pNewItem;
			--mIdCount[RES_TYPE_BUILDING_TEXTURE];
		}
		return NULL; 
	}
	
	//文件写入资源表
	if(pOldItem==NULL) ResTable[RES_TYPE_BUILDING_TEXTURE].AddTail(pNewItem);
	return pNewItem->szID;
}

bool ElementResMan::CopyResFile( const char *szDst, const char *szSrc)
{
	CFile sFile,dFile;
	bool  bFileIsExist = false;
	bool  bFileReadOnly = false;
	CString path,name;
	
	GetPathAndName(szDst,path,name);
	bFileIsExist = AUX_FileIsExist(szDst);
	if(bFileIsExist)
	{
		bFileReadOnly = AUX_FileIsReadOnly(szDst);
		if(!bFileReadOnly) 
		{
			g_VSS.SetProjectPath(path);
			g_VSS.CheckInFile(name);
			bFileReadOnly = AUX_FileIsReadOnly(szDst);
			if(!bFileReadOnly) 
			{
				DWORD flag = GetFileAttributes(szDst);
				flag ^= FILE_ATTRIBUTE_READONLY;
				if(!SetFileAttributes(szDst,flag))
				{
					g_Log.Log("Faild to set file attributes (%s)",szDst);
					return false;
				}
				bFileReadOnly = true;
			}
		}
	}
	
	CreateFolder(path);
	if(bFileReadOnly)
	{
		g_VSS.SetProjectPath(path);
		g_VSS.CheckOutFile(szDst);
	};
	
	if(!sFile.Open(szSrc,CFile::modeRead | CFile::typeBinary | CFile::shareDenyWrite))
	{
		g_Log.Log("ElementResMan::CopyResFile(),打开文件%s失败!",szSrc);
		return false;
	}
	if(!dFile.Open(szDst,CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
	{
		sFile.Close();
		g_Log.Log("ElementResMan::CopyResFile(),创建文件%s失败!",szDst);
		return false;
	}
	DWORD dwFileLen = sFile.GetLength();
	DWORD dwLoaded = 0;
	BYTE* pBuffer = new BYTE[dwFileLen];
	ASSERT(pBuffer);

#ifdef _ANGELICA22
	if(0==sFile.Read(pBuffer,dwFileLen))
		goto failed;
	dFile.Write(pBuffer,dwFileLen);
#else
	if(0==sFile.ReadHuge(pBuffer,dwFileLen))
		goto failed;
	dFile.WriteHuge(pBuffer,dwFileLen);
#endif // #ifdef _ANGELICA22
	
	sFile.Close();
	dFile.Close();
	delete pBuffer;

	if(!bFileIsExist)
	{
		CString dstPath,dstName;
		GetPathAndName(path,dstPath,dstName);
		g_VSS.SetProjectPath(dstPath);//Building\textures
		g_VSS.CreateProject(dstName);//e
		g_VSS.SetProjectPath(path);//Building\textures\e
		g_VSS.AddFile(szDst);
	}else
	{
		g_VSS.SetProjectPath(path);
		g_VSS.CheckInFile(name);
	}
	return true;

failed:
	sFile.Close();
	dFile.Close();
	delete pBuffer;
	if(bFileIsExist)
	{
		g_VSS.SetProjectPath(path);
		g_VSS.UndoCheckOut(name);
	}
	return false;
}

Res_Item* ElementResMan::ResIsExistByPath( const int iType, const char *szPath)
{
	ALISTPOSITION pos = ResTable[iType].GetHeadPosition();
	while(pos)
	{
		Res_Item *pItem = ResTable[iType].GetNext(pos);
		if(stricmp(pItem->szID,szPath)==0)
			return pItem;
	}
	return NULL;
}

void ElementResMan::GetPathAndName(const CString strPathName, CString& strPath, CString& strName)
{
	AString temp = strPathName;
	
	int pos = temp.ReverseFind('\\');
	if(pos == -1)
	{
		pos = 0;
	}
	strPath = temp.Mid(0, pos);
	strName = temp.Mid(pos+1,temp.GetLength());
}

bool ElementResMan::ReplaceRes( const int iType, const char *szName, const char *szNewRes)
{
	bool bResult = false;
	switch(iType)
	{
	case RES_TYPE_BUILDING:
		bResult = ReplaceBuildingRes(szName,szNewRes);
		break;
	}
	return bResult;
}

bool ElementResMan::ReplaceBuildingRes(const char *szName, const char *szNewRes)
{
	ALISTPOSITION pos = ResTable[RES_TYPE_BUILDING].GetHeadPosition();
	while(pos)
	{
		Res_Item *pItem = ResTable[RES_TYPE_BUILDING].GetNext(pos);
		if(strcmp(pItem->szID,szName)==0)
		{
		
			Res_Item* pOldItem = pItem;
			
			CELBuilding mNewModel;
			CString strHullName = szNewRes;
			strHullName.Replace(".mox",".chf");
			if(!mNewModel.LoadFromMOXAndCHF(g_Render.GetA3DDevice(),szNewRes,strHullName))
			{
				AfxMessageBox("ElementResMan::ReplaceBuildingRes(),替换失败! 不能加载新文件.");
				return false;
			}
			
			abase::hash_map<AString, AString> tempMap;
			int nMesh = mNewModel.GetA3DLitModel()->GetNumMeshes();
			for( int i = 0; i < nMesh; i++)
			{
				A3DLitMesh *pMesh = mNewModel.GetA3DLitModel()->GetMesh(i);
				if(pMesh)
				{
					const char *szOrgTexture = pMesh->GetTextureMap();
					abase::hash_map<AString, AString>::iterator it = tempMap.find(szOrgTexture);
					AString szTextures = it == tempMap.end() ? "" : it->second;;
					if(szTextures.IsEmpty())
					{
						szTextures = ProcessBuildingTextureRes(szOrgTexture);
						if(szTextures.IsEmpty())
						{
							g_Log.Log("ElementResMan::ReplaceBuildingRes(),纹理 %s 加入失败!", pMesh->GetTextureMap());
							tempMap.clear();
							mNewModel.Release();
							return false;
						}
						tempMap[szOrgTexture] = szTextures;
					}
					pMesh->ChangeTextureMap(szTextures);
				}
			}
			tempMap.clear();
			
			CString name,path;
			name = pOldItem->szID;
			GetPathAndName(name,path,name);
			g_VSS.SetProjectPath(path);
			g_VSS.CheckOutFile(pOldItem->szID);
			//替换旧的模型文件
			AFile sFile;
			if(!sFile.Open(pOldItem->szID,AFILE_CREATENEW))
			{
				g_VSS.CheckInFile(name);
				g_Log.Log("ElementResMan::ReplaceBuildingRes(),创建新文件%s失败!", pOldItem->szID);
				return false;
			}
			if(!mNewModel.Save(&sFile))
			{
				sFile.Close();
				g_VSS.CheckInFile(name);
				g_Log.Log("ElementResMan::ReplaceBuildingRes(),保存文件%s失败!", pOldItem->szID);
				return false;
			}
			mNewModel.Release();
			sFile.Close();
			g_VSS.CheckInFile(name);
			
			//更新资源表数据
			strcpy(pOldItem->szOrgName,TransOrgName(szNewRes));
			return true;
		}
	}
	AfxMessageBox("ElementResMan::ReplaceBuildingRes(),替换失败! 被替换文件不存在资源表中.");
	return false;
}

bool ElementResMan::ProcessGrassRes( const char *szName)
{
	return true;
}

bool ElementResMan::ProcessTreeRes(const char *szName, const char *szCompositeMap)
{
#ifdef _ANGELICA22
#else
	CELTree tree;
	if( !tree.Init(g_Render.GetA3DDevice(), NULL, 0 , szName, szCompositeMap, &g_Log) )
	{
		g_Log.Log("ElementResMan::ProcessTreeRes(), failed to init tree type [%s]", szName);
		return false;
	}
	
	//把spt文件以及其中的纹理都加入服务器
	CString strPathName = szName;
	CString strName,strPath;
	strPathName.MakeLower();
	int pos = strPathName.Find("trees");
	if(pos==-1) 
	{
		AfxMessageBox("原文件的路径中不包括<trees>文件夹!");
		tree.Release();
		return false;
	}
	strPath = strPathName.Left(pos);
	strPathName = szName;
	strPathName.Replace(strPath,NULL);
	g_VSS.SetProjectPath("");
	strPath = "";
	while(1)
	{
		pos = strPathName.Find("\\");
		if(pos==-1) 
		{
			strName = strPathName;
			break;
		}
		strName = strPathName.Left(pos + 1);
		strPathName.Replace(strName,NULL);
		strName.Replace("\\",NULL);
		strPath +=strName;
		g_VSS.CreateProject(strName);
		g_VSS.SetProjectPath(strPath);
		strPath +="\\";
	}
	g_VSS.AddFile(szName);
	//把纹理放到服务器上
	CString strMap = tree.GetCompositeMap();
	g_VSS.AddFile(strMap);
	strMap = tree.GetBranchMap();
	g_VSS.AddFile(strMap);
	tree.Release();
	
	//文件写入资源表
	Res_Item *pNewItem = new Res_Item;
	strcpy(pNewItem->szID,szName);
	strcpy(pNewItem->szOrgName,szName);
	ResTable[RES_TYPE_BUILDING].AddTail(pNewItem);
	tree.Release();
#endif // #ifdef _ANGELICA22
	return true;
}

Res_Item *ElementResMan::ResIsExistByName( const int iType, const char *szName)
{
	ALISTPOSITION pos = ResTable[iType].GetHeadPosition();
	while(pos)
	{
		Res_Item *pItem = ResTable[iType].GetNext(pos);
		CString strPath,strName;
		GetPathAndName(pItem->szOrgName,strPath,strName);
		if(stricmp(strName,szName)==0)
			return pItem;
	}
	return NULL;
}

bool ElementResMan::ResIsExistByOrgName( const int iType, const char *szOrgName)
{
	ALISTPOSITION pos = ResTable[iType].GetHeadPosition();
	while(pos)
	{
		Res_Item *pItem = ResTable[iType].GetNext(pos);
		CString orgName1 = pItem->szOrgName;
		CString orgName2 = szOrgName;
		int temp = orgName1.ReverseFind('\\');
		if(temp!=-1) orgName1 = orgName1.Right(orgName1.GetLength() - temp);
		temp = orgName2.ReverseFind('\\');
		if(temp!=-1) orgName2 = orgName2.Right(orgName2.GetLength() - temp);

		if(orgName1 == orgName2)
			return true;
	}
	return false;
}

int	ElementResMan::GetResCount( const int iType) const
{
	return (int)(ResTable[iType].GetCount());
}

Res_Item* ElementResMan::GetRes(const int iType, int iIndex)
{
	return ResTable[iType].GetByIndex(iIndex);
}

void ElementResMan::DeleteRes( const int iType, const char*szID)
{
	ALISTPOSITION pos = ResTable[iType].GetHeadPosition();
	
	while(pos)
	{
		ALISTPOSITION oldPos = pos;
		Res_Item* pItem = ResTable[iType].GetNext(pos);
		if(strcmp(pItem->szID,szID)==0)
		{
			
			if(iType == RES_TYPE_BUILDING)
			{
				
				//去掉旧的纹理的引用
				CELBuilding mModel;
				AFile mFile;
				if(!mFile.Open(pItem->szID,AFILE_OPENEXIST))
				{
					AfxMessageBox("ElementResMan::DeleteRes(),删除失败! 不能模型文件.");
					return;
				}
				if(!mModel.Load(g_Render.GetA3DDevice(),&mFile))
				{
					AfxMessageBox("ElementResMan::DeleteRes(),删除失败! 不能加模型文件.");
					return;
				}
				mFile.Close();
				
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
                /* /纹理不能删除，因为你不能确定是否一个纹理被多个模型用                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
				int nMesh = mModel.GetA3DLitModel()->GetNumMeshes();
				for( int i = 0; i < nMesh; i++)
				{
					A3DLitMesh *pMesh = mModel.GetA3DLitModel()->GetMesh(i);
					if(pMesh)
					{
						DeleteRes(RES_TYPE_BUILDING_TEXTURE,pMesh->GetTextureMap());
					}
				}*/

			}
			
			//依次在本地和服务器删除模型文件
			CString path,name,org;
			org = pItem->szID;
			GetPathAndName(org,path,name);
			g_VSS.SetProjectPath(path);
			g_VSS.CheckOutFile(pItem->szID);
			
			if(!DeleteFile(pItem->szID))
				AfxMessageBox("ElementResMan::DeleteRes(),删除本地文件失败!");
			g_VSS.CheckInFile(name);
			g_VSS.DeleteFile(name);
			
			delete pItem;
			ResTable[iType].RemoveAt(oldPos);
			return;
		}
	}
}

bool ElementResMan::WriteTable()
{
	AFile sFile;
	if(!sFile.Open("ResTable\\ElementRes.table",AFILE_CREATENEW))
	{
		g_Log.Log("ElementResMan::Save(),不能打开文件ElementRes.table.");
		return false;
	}
	DWORD dwVersion = RES_FILE_VERSION;
	DWORD dwWrited;
	sFile.Write(&dwVersion,sizeof(DWORD),&dwWrited);
	sFile.Write(mIdCount,sizeof(DWORD)*RES_TYPE_COUNT,&dwWrited);
	DWORD dwResType[RES_TYPE_COUNT] = { RES_TYPE_BUILDING, RES_TYPE_GRASS,RES_TYPE_TREE, RES_TYPE_GFX, RES_TYPE_BUILDING_TEXTURE };
	for(int i = 0; i < RES_TYPE_COUNT; i++)
	{
		DWORD dwResCount = ResTable[dwResType[i]].GetCount();
		sFile.Write(dwResType+i,sizeof(DWORD),&dwWrited);
		sFile.Write(&dwResCount,sizeof(DWORD),&dwWrited);	
		sFile.Write(&iFolderIndex[i],sizeof(int),&dwWrited);
		ALISTPOSITION pos = ResTable[dwResType[i]].GetTailPosition();
		while(pos)
		{
			Res_Item* pItem = ResTable[dwResType[i]].GetPrev(pos);
			sFile.Write(pItem->szID,LENGTH_ID,&dwWrited);
			sFile.Write(pItem->szOrgName,LENGTH_ORG_NAME,&dwWrited);
		}
	}
	sFile.Close();
	return true;
}

bool ElementResMan::ReadTable(const char *szPath)
{
	ReleaseResTable();
	
	AFile sFile;
	if(!sFile.Open(szPath,AFILE_OPENEXIST))
	{
		g_Log.Log("ElementResMan::Load(),不能打开文件 %s",szPath);
		return false;
	}
	DWORD dwVersion;
	DWORD dwReaded;
	sFile.Read(&dwVersion,sizeof(DWORD),&dwReaded);
	if(dwVersion>0x2)
	{
		g_Log.Log("ElementResMan::Load(),不认识的文件版本!");
		return false;
	}
	sFile.Read(mIdCount,sizeof(DWORD)*RES_TYPE_COUNT,&dwReaded);
	if(dwVersion < 0x2)
	{
		DWORD dwResType,dwResCount, dwRefCount;
		for( int n = 0; n < RES_TYPE_COUNT; n++)
		{
			sFile.Read(&dwResType,sizeof(DWORD),&dwReaded);
			sFile.Read(&dwResCount,sizeof(DWORD),&dwReaded);	
			sFile.Read(&iFolderIndex[n],sizeof(int),&dwReaded);
			for( int i = 0; i < dwResCount; i++)
			{
				Res_Item* pItem = new Res_Item;
				char szTemp[LENGTH_MD5_CODE];
				memset(pItem->szID,0,LENGTH_ID);
				memset(szTemp,0,LENGTH_MD5_CODE);
				memset(pItem->szOrgName,0,LENGTH_ORG_NAME);
				
				sFile.Read(pItem->szID,LENGTH_ID,&dwReaded);
				sFile.Read(szTemp,LENGTH_MD5_CODE,&dwReaded);
				sFile.Read(pItem->szOrgName,LENGTH_ORG_NAME,&dwReaded);
				sFile.Read(&dwRefCount,sizeof(DWORD),&dwReaded);
				for(int j = 0; j < dwRefCount; j++)
				{
					char szTemp2[LENGTH_ORG_NAME];
					sFile.Read(szTemp2,LENGTH_ORG_NAME,&dwReaded);
				}
				ResTable[dwResType].AddHead(pItem);
			}
		}
	}else
	{
		DWORD dwResType,dwResCount;
		for( int n = 0; n < RES_TYPE_COUNT; n++)
		{
			sFile.Read(&dwResType,sizeof(DWORD),&dwReaded);
			sFile.Read(&dwResCount,sizeof(DWORD),&dwReaded);	
			sFile.Read(&iFolderIndex[n],sizeof(int),&dwReaded);
			for( int i = 0; i < dwResCount; i++)
			{
				Res_Item* pItem = new Res_Item;
				memset(pItem->szID,0,LENGTH_ID);
				memset(pItem->szOrgName,0,LENGTH_ORG_NAME);
				ASSERT(pItem);
				sFile.Read(pItem->szID,LENGTH_ID,&dwReaded);
				sFile.Read(pItem->szOrgName,LENGTH_ORG_NAME,&dwReaded);
				ResTable[dwResType].AddHead(pItem);
			}
		}
	}
	sFile.Close();
	return true;
}


bool ElementResMan::Load(const char *szPath)
{
    return ReadTable(szPath);		
}

bool ElementResMan::Load()
{
	return ReadTable("ResTable\\ElementRes.table");	
}

bool ElementResMan::Save()
{
	return WriteTable();
}

INT64 ElementResMan::GetFileSize(const char *szName) const
{
	CFileException ex;
	CFile sourceFile;
	CString pathName = szName;
	if (!sourceFile.Open(pathName,
		CFile::modeRead, &ex))
	{
		TCHAR szError[1024];
		ex.GetErrorMessage(szError, 1024);
		CString error;
		error.Format(_T("Couldn't open source file: %s,%s"),pathName,szError);
		g_Log.Log(error);
		return 0;
	}
    INT64 size = sourceFile.GetLength();
	sourceFile.Close();
	return size;
}

void ElementResMan::ReleaseResTable()
{
	for( int i = 0; i < RES_TYPE_COUNT; i++)
	{
		
		ALISTPOSITION pos = ResTable[i].GetHeadPosition();
		while(pos)
		{
			Res_Item* pItem = ResTable[i].GetNext(pos);
			delete pItem;
		}
		ResTable[i].RemoveAll();
	}
}


CString ElementResMan::TransOrgName(const char *szOrg)
{
	CString org = szOrg;
	org.MakeLower();
	int pos = org.Find("auto_sorts\\");
	if(pos==-1) 
	{
		org.Replace("models\\static\\",NULL);
		return org;
	}
	CString temp = org.Left(pos);
	temp = temp + "auto_sorts\\";
	org.Replace(temp,NULL);
	return org;
}

CString ElementResMan::GetParentPath(CString  path)
{
	if(path[path.GetLength() - 1] == '\\')
	{
		path = path.Mid(0, path.GetLength() - 1);
	}

	CString parent;
	int pos = path.ReverseFind('\\');
	if(pos == -1)
	{
		pos = 0;
	}

	parent = path.Mid(0, pos);
	return parent;
}

BOOL ElementResMan::CreateFolder(CString path)
{
	if(PathFileExists(path))
		return TRUE;
	
	if(!CreateDirectory(path, NULL))
	{
		if(path.GetLength() == 2)
		{
			return FALSE;
		}

		if(!CreateFolder(GetParentPath(path)))
		{
			return FALSE;
		}

		if(!CreateDirectory(path, NULL))
		{
			return FALSE;
		}
	}
	return TRUE;
}
