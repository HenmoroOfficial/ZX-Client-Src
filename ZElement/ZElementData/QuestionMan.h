// QuestionMan.h: interface for the CQuestionMan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTIONMAN_H__048109C0_67CB_44B6_B966_D2BDEE005535__INCLUDED_)
#define AFX_QUESTIONMAN_H__048109C0_67CB_44B6_B966_D2BDEE005535__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector.h>

struct  FOLDER_Q
{
	unsigned long nParentID;
	unsigned long nID;
	CString strFolderName;
	
	FOLDER_Q()
	{
		nParentID = 0;
		nID       = 0;
	}
};

class CQuestionData
{
public:
	CQuestionData()
	{
		nid = 0;
		nAnswer = 2;
		key = 0;
		bDeleted = false;
		nparent = 0;
	}
	~CQuestionData()
	{

	}
bool Load(CString strPath);
bool Save(CString strPath);

void SetID( int id ){ nid = id; }
public:
	int     nid;
	CString strQuestion;	//问题
	int     nAnswer;        //被选答案数目
	CString strAnswer[6];	//被选答案
	BYTE    key;			//答案 00111111 从低到高分别表示ABCDEF
	bool    bDeleted;       //是否被删除
	int     nparent;        //父文件夹的ID
};


class CQuestionsFolderData
{
public:

	CQuestionsFolderData()
	{
		ulMaxID = 0; 
	}

	~CQuestionsFolderData(){}

	bool Load();
	bool Save();

	int  AddFolder(CString &fld);
	bool DeleteFolder( unsigned long id);

	int GetFolderNum(){ return listFolders.size();}
	FOLDER_Q *GetFolder( int idx ){ return &listFolders[idx]; }
	int FindFolder(CString &fld);
private:
	abase::vector<FOLDER_Q> listFolders;
	unsigned long ulMaxID;
};

extern CQuestionsFolderData g_QuestionsFolder;

#endif // !defined(AFX_QUESTIONMAN_H__048109C0_67CB_44B6_B966_D2BDEE005535__INCLUDED_)
