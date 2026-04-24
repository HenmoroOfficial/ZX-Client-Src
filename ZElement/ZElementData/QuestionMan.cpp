// QuestionMan.cpp: implementation of the CQuestionMan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "elementdata.h"
#include "QuestionMan.h"
#include "Global.h"
#include "VssOperation.h"
#include <AFile.h>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const char* _format_folder = "PID = %u, ID = %u, FName = %s";
CQuestionsFolderData g_QuestionsFolder;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define QUESTIONS_VERSION 3

bool CQuestionData::Load(CString strPath)
{
	FILE *pFile = fopen(strPath,"rb");
	if(pFile==NULL) return false;
	
	DWORD dwVersion;
	fread(&dwVersion,sizeof(DWORD),1,pFile);
	if(dwVersion > QUESTIONS_VERSION) return false;
	if(dwVersion < 2)
	{
		fread(&nid,sizeof(int),1,pFile);
		fread(&nAnswer,sizeof(int),1,pFile);
		fread(&key,1,1,pFile);
		int nLength = 0;
		char szBuffer[512];
		
		memset(szBuffer,0,512);
		fread(&nLength, sizeof(int),1,pFile);
		if(nLength > 0) fread(szBuffer,nLength,1,pFile);
		strQuestion = szBuffer;
		int i;
		for( i = 0; i < 6; ++i)
		{
			nLength = 0;
			memset(szBuffer,0,512);
			fread(&nLength, sizeof(int),1,pFile);
			if(nLength > 0) fread(szBuffer,nLength,1,pFile);
			strAnswer[i] = szBuffer;
		}

		nAnswer = 6;
		for( i = 0; i < 6; ++i)
		{
			if(strAnswer[i].GetLength()<=0)
			{
				nAnswer = i;
				break;
			}
		}
	}else if(dwVersion < 3)
	{
		fread(&nid,sizeof(int),1,pFile);
		fread(&nAnswer,sizeof(int),1,pFile);
		fread(&key,1,1,pFile);
		fread(&bDeleted,sizeof(bool),1,pFile);
		fread(&nparent, sizeof(int),1,pFile);
		int nLength = 0;
		char szBuffer[512];
		
		memset(szBuffer,0,512);
		fread(&nLength, sizeof(int),1,pFile);
		if(nLength > 0) fread(szBuffer,nLength,1,pFile);
		strQuestion = szBuffer;
		
		int i;
		for( i = 0; i < 6; ++i)
		{
			nLength = 0;
			memset(szBuffer,0,512);
			fread(&nLength, sizeof(int),1,pFile);
			if(nLength > 0) fread(szBuffer,nLength,1,pFile);
			strAnswer[i] = szBuffer;
		}

		nAnswer = 6;
		for( i = 0; i < 6; ++i)
		{
			if(strAnswer[i].GetLength()<=0)
			{
				nAnswer = i;
				break;
			}
		}
	}else
	{
		fread(&nid,sizeof(int),1,pFile);
		fread(&nAnswer,sizeof(int),1,pFile);
		fread(&key,1,1,pFile);
		fread(&bDeleted,sizeof(bool),1,pFile);
		fread(&nparent, sizeof(int),1,pFile);
		int nLength = 0;
		char szBuffer[512];
		
		memset(szBuffer,0,512);
		fread(&nLength, sizeof(int),1,pFile);
		if(nLength > 0) fread(szBuffer,nLength,1,pFile);
		strQuestion = szBuffer;
		
		for( int i = 0; i < 6; ++i)
		{
			nLength = 0;
			memset(szBuffer,0,512);
			fread(&nLength, sizeof(int),1,pFile);
			if(nLength > 0) fread(szBuffer,nLength,1,pFile);
			strAnswer[i] = szBuffer;
		}
	}
	fclose(pFile);

	return true;
}


bool CQuestionData::Save(CString strPath)
{
	FILE *pFile = fopen(strPath,"wb");
	if(pFile==NULL) return false;
	DWORD dwVersion = QUESTIONS_VERSION;
	fwrite(&dwVersion,sizeof(DWORD),1,pFile);
	fwrite(&nid,sizeof(int),1,pFile);
	fwrite(&nAnswer,sizeof(int),1,pFile);
	fwrite(&key,1,1,pFile);
	fwrite(&bDeleted,sizeof(bool),1,pFile);
	fwrite(&nparent, sizeof(int),1,pFile);

	int nLength = strQuestion.GetLength();
	fwrite(&nLength, sizeof(int),1,pFile);
	if(nLength > 0) fwrite(strQuestion.GetBuffer(0),nLength,1,pFile);

	for( int i = 0; i < 6; ++i)
	{
		nLength = strAnswer[i].GetLength();
		fwrite(&nLength, sizeof(int),1,pFile);
		if(nLength > 0) fwrite(strAnswer[i].GetBuffer(0),nLength,1,pFile);
	}
	fclose(pFile);

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
bool CQuestionsFolderData::Load()
{
	listFolders.clear();
	
	AFile file;
	if (!file.Open("Questions\\Questions_Folder.dat", AFILE_OPENEXIST | AFILE_TEXT))
		return false;

	ulMaxID = 0;
	char	szLine[AFILE_LINEMAXLEN];
	char	szBuf[AFILE_LINEMAXLEN];
	DWORD	dwRead;

	while (true)
	{
		szLine[0] = '\0';
		if (!file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead)
		|| strlen(szLine) == 0) break;

		unsigned long ulPID = 0;
		unsigned long ulID = 0;
		szBuf[0] = '\0';

		sscanf(szLine, _format_folder, &ulPID, &ulID, szBuf);
		if (ulID == 0 || strlen(szBuf) == 0) continue;
		
		FOLDER_Q fq;
		fq.nParentID = ulPID;
		fq.nID = ulID;
		fq.strFolderName = szBuf;
		listFolders.push_back(fq);

		if (ulID > ulMaxID) ulMaxID = ulID;
	}

	file.Close();
	return true;
}

bool CQuestionsFolderData::Save()
{
	AFile file;
	if (!file.Open("Questions\\Questions_Folder.dat", AFILE_CREATENEW | AFILE_TEXT))
		return false;

	char	szLine[AFILE_LINEMAXLEN];

	for (int i = 0; i < listFolders.size(); ++i)
	{
		sprintf(szLine, _format_folder, listFolders[i].nParentID, listFolders[i].nID, listFolders[i].strFolderName);
		file.WriteLine(szLine);
		
	}

	file.Close();
	return true;
}

int  CQuestionsFolderData::AddFolder(CString &fld)
{
	FOLDER_Q fq;
	//g_VSS.SetProjectPath("Questions");
	//g_VSS.CheckOutFile("Questions\\Questions_Folder.dat");
	if(!Load()) goto faild;
	if(FindFolder(fld)) goto faild;
	fq.nID = ++ulMaxID;
	fq.nParentID = 0;
	fq.strFolderName = fld;
	listFolders.push_back(fq);
	if(!Save()) goto faild;
	//g_VSS.CheckInFile("Questions_Folder.dat");
	return fq.nID;
faild:
	g_VSS.UndoCheckOut("Questions\\Questions_Folder.dat");
	return 0;
}

int CQuestionsFolderData::FindFolder(CString &fld)
{
	for (int i = 0; i < listFolders.size(); ++i)
	{
		if(listFolders[i].strFolderName==fld)
			return listFolders[i].nID;
	}
	return 0;
}

bool CQuestionsFolderData::DeleteFolder( unsigned long id)
{
	
	for (int i = 0; i < listFolders.size(); ++i)
	{
		if(listFolders[i].nID == id)
		{
			listFolders.erase(&listFolders[i]);
			return true;
		}
	}
	return false;
}




