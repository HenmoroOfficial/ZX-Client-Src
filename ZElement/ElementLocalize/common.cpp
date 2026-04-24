#include "stdafx.h"
#include "common.h"
#include "AFilePackage.h"
#include "AFI.h"

const TCHAR* g_szChunk[CHUNK_NUM] = 
{
	_T("Element"),
	_T("Task"),
	_T("Policy"),
	_T("Interface Hint"),
	_T("Interface String"),
	_T("Item Description"),
	_T("ItemEx Description"),
	_T("Skill Description"),
	_T("Fixed Message"),
	_T("Server Error"),
	_T("Precinct"),
	_T("Help Script"),
	_T("Help Text"),
	_T("GShop"),
	_T("GShopTypes"),
	_T("Buff Description"),
	_T("Task Error"),
	_T("Text Files"),
	_T("Instance Text"),
	_T("Load Hints"),
	_T("Pet Talk"),
	_T("GShop2"),
	_T("Gshop2Types"),
	_T("Addon String"),
	_T("HomeTown"),
	_T("Domain Information"),
	_T("Domain Information2"),
	_T("Gshop3"),
	_T("Gshop3Types")
};

bool CleanUpPckRedundancy(const char* szFilePath)
{
	AFilePackage::FILEENTRY entry;
	AFilePackage *fPack=new AFilePackage;
	if(!fPack->Open(szFilePath,"",AFilePackage::OPENEXIST))
	{
		delete fPack;
		return false;
	}
	int sumfiles=fPack->GetFileNumber();
	int totfilesize=0;
	for(int j=0;j<sumfiles;j++)
		totfilesize+=fPack->GetFileEntryByIndex(j)->dwCompressedLength;
	
	bool bOK = false;
	AFilePackage *newpack=new AFilePackage;
	
	char szTempName[MAX_PATH] = {0};
	GetTempPathA(MAX_PATH, szTempName);
	strncat(szTempName, "\\temp.pck", MAX_PATH);
	if(newpack->Open(szTempName,"",AFilePackage::CREATENEW))
	{
		for(int j=0;j<sumfiles;j++)
		{
			entry=*fPack->GetFileEntryByIndex(j);
			unsigned char *fbuf=new unsigned char[entry.dwCompressedLength+1];
			fPack->ReadCompressedFile(entry.szFileName,fbuf,&entry.dwCompressedLength);
			newpack->AppendFileCompressed(entry.szFileName,fbuf,entry.dwLength,entry.dwCompressedLength);
			delete fbuf;
		}
		newpack->Close();
		fPack->Close();

		char szDestFilePath[MAX_PATH] = {0};
		_snprintf(szDestFilePath, MAX_PATH, "%s\\%s", af_GetBaseDir(), szFilePath);
		if(DeleteFileA(szDestFilePath) && MoveFileA(szTempName, szDestFilePath))
		{
			DeleteFileA(szTempName);
			bOK = true;
		}
		else
		{
			DWORD err = GetLastError();
			int x = err;
		}
	}
	else
	{
		fPack->Close();
	}

	delete fPack;
	delete newpack;

	return bOK;
}