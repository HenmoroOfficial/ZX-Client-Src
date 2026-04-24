// TextBase.cpp: implementation of the CTextBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ElementLocalize.h"
#include "TextBase.h"
#include "../ZCommon/EC_MD5Hash.h"
#include "vector.h"
#include "LuaState.h"
#include "luaUtil.h"
#include "DataManager.h"
#include "TemplateBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern int Aux_GetLineNum(CString str);



#define SYNTAX_KEY_WORD 5
const TCHAR cSyntax[] = 
{
	'%',
	'^',
	'&',
	'#',
	'$'
};

int ProcessPS(CString txt, CString &strSyntax, int index)//%
{
	int ext = 0;
	int len = txt.GetLength();
	
	for( int i = 1; i < 6; ++i)
	{
		if(index + i >= len) return 0;
		
		TCHAR cSrc = txt.GetAt(index + i);
		if(cSrc=='-' || cSrc=='+' || cSrc=='o' || cSrc=='#')
		{
			++ext;
		}else if( cSrc >= '0' && cSrc <= '9')
		{
			++ext;
		}else if( cSrc == '.')
		{
			++ext;
		}else if(cSrc=='c' || cSrc=='C' ||
			cSrc=='d' || cSrc=='i' ||
			cSrc=='u' || cSrc=='o' ||
			cSrc=='x' || cSrc=='X' ||
			cSrc=='e' || cSrc=='E' ||
			cSrc=='f' || cSrc=='g' ||
			cSrc=='G' || cSrc=='s' ||
			cSrc=='S' )
		{
			++ext;
			strSyntax = txt.Mid(index,ext+1);
			return ext;
		}else if(cSrc=='%')
		{
			++ext;
			strSyntax = txt.Mid(index,ext+1);
			return ext;
		}else return 0;
	}
	
	return 0;
}

int ProcessPX(CString txt, CString &strSyntax, int index)//^
{
	int ext = 0;
	int len = txt.GetLength();
	
	for( int i = 1; i < 7; ++i)
	{
		if(index + i >= len) return 0;
		
		TCHAR cSrc = txt.GetAt(index + i);
		if(cSrc >= '0' && cSrc <='9')
			++ext;
		else if(cSrc >= 'a' && cSrc <='f')
			++ext;
		else if(cSrc >= 'A' && cSrc <='F')
			++ext;
		else return 0;
	}
	
	strSyntax = txt.Mid(index,ext+1);
	return ext;
}

int ProcessPA(CString txt, CString &strSyntax, int index)//&
{
	int ext = 0;
	int len = txt.GetLength();
	
	
	if(index + 1 >= len) return 0;
	TCHAR cSrc = txt.GetAt(index + 1);
	if(cSrc == '%')
		++ext;
	else return 0;
	
	if(index + 2 >= len) return 0;
	cSrc = txt.GetAt(index + 2);
	if(cSrc == 's' || cSrc == 'S')
		++ext;
	else return 0;
	
	if(index + 3 >= len) return 0;
	cSrc = txt.GetAt(index + 3);
	if(cSrc == '&' || cSrc == '&')
		++ext;
	else return 0;
	
	strSyntax = txt.Mid(index,ext+1);
	return ext;
}

int ProcessPE(CString txt, CString &strSyntax, int index)//#
{
	int ext = 0;
	int len = txt.GetLength();
	
	
	if(index + 1 >= len) return 0;
	TCHAR cSrc = txt.GetAt(index + 1);
	if(cSrc == '#')
		++ext;
	else return 0;
	
	if(index + 2 >= len) return 0;
	cSrc = txt.GetAt(index + 2);
	if(cSrc == '#')
		++ext;
	else return 0;
	
	strSyntax = txt.Mid(index,ext+1);
	return ext;
}

int ProcessPD(CString txt, CString &strSyntax, int index)//#
{
	int ext = 0;
	int len = txt.GetLength();
	
	
	if(index + 1 >= len) return 0;
	TCHAR cSrc = txt.GetAt(index + 1);
	if((cSrc >= 'a' && cSrc <= 'z') || (cSrc >= 'A' && cSrc <= 'Z'))
	{
		++ext;
		strSyntax = txt.Mid(index,ext+1);
	}
	return ext;
}

void GetSyntaxString(CString txt, abase::vector<SYNTAX_ITEM> *listSyntax)
{
	int startPos = 0;
	CString strText = txt;
	CString temp;
	int len = strText.GetLength();

	
	for( int i = 0; i < len; ++i)
	{
		int r = 0;
		TCHAR cSrc = strText.GetAt(i);
		for( int j = 0; j < SYNTAX_KEY_WORD; ++j)
		{
			if(cSrc == cSyntax[j])
			{
				switch(cSyntax[j]) 
				{
				case '%'://%[flags] [width] [.precision] [{h | l | I64 | L}]type
					r = ProcessPS(txt,temp,i);
					break;
				case '^':
					r = ProcessPX(txt,temp,i);
					break;
				case '&':
					r = ProcessPA(txt,temp,i);
					break;
				case '#':
					r = ProcessPE(txt,temp,i);
					break;
				case '$':
					r = ProcessPD(txt,temp,i);
					break;
				}

				if(r!=0) 
				{
					SYNTAX_ITEM item;
					item.txt = temp;
					item.start = i;
					item.len = r+1;
					listSyntax->push_back(item);
					i = r+i;
				}
				break;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTextBase::CTextBase()
{
	memset(szLabel, 0, 32*sizeof(TCHAR));
	_tcscpy(szLabel, _T("default"));
	pTemp = NULL;
	index = 0;
}

CTextBase::~CTextBase()
{
}

void CTextBase::SetStrIDInfo(CTemplateBase* pTemplate, int idx) 
{
	pTemp = pTemplate; 
	index = idx;
}

CString CTextBase::GetStrID()
{
	CString strID;
	if (pTemp)
		strID.Format(_T("%s%s%d"), pTemp->GetStrID(), STRID_TOK, index);
	else
		strID.Format(_T("%d"), index);
	strID.MakeLower();
	return strID;
}

bool CTextBase::ChangeLmtSize(int nNewLimit)
{
	if (nNewLimit < nLimit)
		return false;

	TCHAR* pszNewTrans = new TCHAR [nNewLimit];
	TCHAR* pszNewOrigi = new TCHAR [nNewLimit];
	
	memset(pszNewTrans, '\0' , sizeof(TCHAR)*nNewLimit);
	memset(pszNewOrigi, '\0' , sizeof(TCHAR)*nNewLimit);

	if (pOriginalText)
	{
		_tcscpy(pszNewOrigi, pOriginalText);
		delete[] pOriginalText;
	}
	if (pTranslationText)
	{
		_tcscpy(pszNewTrans, pTranslationText);
		delete[] pTranslationText;
	}

	pOriginalText = pszNewOrigi;
	pTranslationText = pszNewTrans;
	nLimit = nNewLimit;

	return true;
}

bool  CTextBase::DataIsEqual(char *pData1, int nLen1, char *pData2, int nLen2)
{
	/*
	unsigned int nLenght = 64; w
	char szBuffer[64];
	char szCode[64];
	memset(szBuffer,'\0',64);
	memset(szCode,'\0',64);
	
	PATCH::MD5Hash md;
	md.Update(pData1,nLen1);
	md.Final();
	md.GetString(szBuffer,nLenght);

	nLenght = 64; 
	PATCH::MD5Hash md2;
	md2.Update(pData2,nLen2);
	md2.Final();
	md2.GetString(szCode,nLenght);

	if(strcmp(szBuffer,szCode) == 0) return true;
	else return false;
	*/
	if(0==_tcscmp((TCHAR*)pData1,(TCHAR*)pData2)) return true;
	else return false;
	
}

int CTextBase::GetOriginalTextLineNum()
{
	if(pOriginalText==0) return 0;
	return Aux_GetLineNum(pOriginalText);
}

void CTextBase::Init(unsigned int nLmt, bool bMuil, TCHAR *pOrgText, TCHAR *pTransText, TCHAR *pTextName)
{
	nLimit = nLmt;
	bMuiltiLine = bMuil;
	if(nLimit!=0)
	{
		pOriginalText = new TCHAR[nLimit];
		pTranslationText = new TCHAR[nLimit];
		memset(pOriginalText, '\0' , sizeof(TCHAR)*nLimit);
		memset(pTranslationText, '\0' , sizeof(TCHAR)*nLimit);
	}
	if(pOriginalText) lstrcpy(pOriginalText, pOrgText ? pOrgText : _T(""));
	if(pTransText) lstrcpy(pTranslationText, pTransText ? pTransText : _T(""));
	if(pTextName) lstrcpy(szTextName, pTextName ? pTextName : _T(""));
}

bool CTextBase::ReplaceOriginalText(const TCHAR* pNewText)
{
	lstrcpy(pOriginalText,pNewText);
	return true;	
}

void CTextBase::SetTextName(TCHAR *pTextName)
{
	lstrcpy(szTextName,pTextName);
}

void CTextBase::SetTextLabel(const TCHAR* pLabel)
{
	lstrcpy(szLabel,pLabel);
}

bool CTextBase::ReplaceTransText(const TCHAR *pNewText)
{
	//if(!CheckSyntax(pNewText)) return false;
//	if(_tcscmp(pTranslationText,pNewText)==0) return false;
	if (_tcslen(pNewText) == 0)
	{
		return false;
	}
	lstrcpy(pTranslationText,pNewText);
	if(nStatus==status_new) nStatus = status_normal;
	return true;
}

bool CTextBase::Load(FILE *pFile, DWORD dwVersion)
{
	fread(&nStatus,sizeof(int),1,pFile);
	fread((char*)pOriginalText,nLimit*sizeof(TCHAR),1,pFile);
	fread((char*)pTranslationText,nLimit*sizeof(TCHAR),1,pFile);
	if(dwVersion > 0x10000007)
	{
		fread((char*)szLabel, 32*sizeof(TCHAR), 1, pFile);
	}

	if(_tcscmp(pOriginalText,_T(""))==0) _tcscpy(pTranslationText,_T(""));
	
	return true;
}

bool CTextBase::Save(FILE *pFile)
{
	fwrite(&nStatus,sizeof(int),1,pFile);
	fwrite((char*)pOriginalText,nLimit*sizeof(TCHAR),1,pFile);
	fwrite((char*)pTranslationText,nLimit*sizeof(TCHAR),1,pFile);
	fwrite((char*)szLabel, 32*sizeof(TCHAR), 1, pFile);
	return true;
}

void CTextBase::Release()
{
	if(pOriginalText) { delete[] pOriginalText; pOriginalText= NULL;}
	if(pTranslationText) { delete[] pTranslationText; pTranslationText= NULL;}
}

bool CTextBase::UpdateFromData(const TCHAR* pOrgText)
{
	if (pOrgText == NULL || _tcslen(pOrgText) == 0)
		return true;

	if (pOriginalText &&_tcslen(pOrgText) == _tcslen(pOriginalText) && DataIsEqual((char*)pOriginalText, 0, (char*)pOrgText, 0))
	{
		if (pTranslationText && _tcslen(pTranslationText) > 0)
			SetStatus(status_normal);
	}
	else
	{
		if (!ReplaceOriginalText(pOrgText))
			return false; 
		SetStatus(status_new);
	}

	return true;
}

bool CTextBase::ExportToData(TCHAR*& pOrgText, bool bReallocate)
{
	if (GetStatus() != status_normal || !pTranslationText)
		return true;
	
	int len = _tcslen(pTranslationText);
	if (len == 0)
		return true;

	if (bReallocate)
	{
		if (pOrgText) delete[] pOrgText;
		pOrgText = new TCHAR[len+1];
	}
	if (!pOrgText)
		return true;

	_tcscpy(pOrgText, pTranslationText);
	pOrgText[len] = 0;

	return true;
}

bool CTextBase::ExportToData(TCHAR* pDataText)
{
	if (GetStatus() != status_normal || !pTranslationText)
		return true;
	
	int len = _tcslen(pTranslationText);
	if (len == 0)
		return true;

	if (!pDataText)
		return true;
	
	_tcscpy(pDataText, pTranslationText);
	pDataText[len] = 0;
	
	return true;
}
//////////////////////////////////////////////////////////////////////
//CText_Normal
//////////////////////////////////////////////////////////////////////

CText_Normal::CText_Normal()
{
	nLimit = 0;
	nStatus = status_new;
	pOriginalText = NULL;
	pTranslationText = NULL;
	bIsIdVaild = false;
}

CText_Normal::~CText_Normal()
{
	Release();
}

bool CText_Normal::CheckSyntax(CString strDest, int& start, int& len, bool bMsg)
{
	start = len = 0;
	if(strDest.GetLength()==0)
	{
		if(bMsg)
		AfxMessageBox(_T("Translation text can't empty!"));
		return false;
	}else if(strDest.GetLength() > nLimit-1)
	{
		if(bMsg)
		AfxMessageBox(_T("Translation text overflow!"));
		return false;
	}
	if(Aux_GetLineNum(pOriginalText)!=Aux_GetLineNum(strDest))
	{
		if(bMsg)
		AfxMessageBox(_T("Translation text line number not equal to original text!"));
		return false;
	}
	return true;
}

CTextBase* CText_Normal::Clone()
{
	CText_Normal *pNew = new CText_Normal();
	if(pNew==NULL) return NULL;
	pNew->Init(nLimit,bMuiltiLine,pOriginalText,pTranslationText,szTextName);
	pNew->SetStatus(nStatus);
	return pNew;
}


//////////////////////////////////////////////////////////////////////
//CText_TalkWindow
//////////////////////////////////////////////////////////////////////

CText_TalkWindow::CText_TalkWindow()
{
	nLimit = -1;//表示没有长度的限制
	id = 0;
	bIsIdVaild = true; 
	bMuiltiLine = true;
	nStatus = status_new;
	pOriginalText = NULL;
	pTranslationText = NULL;
}

CText_TalkWindow::~CText_TalkWindow()
{
	Release();
}

bool CText_TalkWindow::Load(FILE *pFile, DWORD dwVersion)
{
	Release();
	
	int norg = 0;
	int ntrans = 0;
	fread(&id,sizeof(int),1,pFile);
	fread(&norg,sizeof(int),1,pFile);
	fread(&ntrans,sizeof(int),1,pFile);
	fread(&nStatus,sizeof(int),1,pFile);
	
	if(norg > 0) pOriginalText = new TCHAR[norg+1];
	if(ntrans > 0) pTranslationText = new TCHAR[ntrans+1];

	if(pOriginalText) memset(pOriginalText,'\0',(norg + 1)*sizeof(TCHAR));
	if(pTranslationText) memset(pTranslationText,'\0',(ntrans + 1)*sizeof(TCHAR));
	
	if(norg > 0) fread((char*)pOriginalText,norg*sizeof(TCHAR),1,pFile);
	if(ntrans > 0) fread((char*)pTranslationText,ntrans*sizeof(TCHAR),1,pFile);
	if(dwVersion > 0x10000007)
	{
		fread((char*)szLabel, 32*sizeof(TCHAR), 1, pFile);
	}
	return true;
}

bool CText_TalkWindow::Save(FILE *pFile)
{
	fwrite(&id,sizeof(int),1,pFile);
	int norg = 0;
	int ntrans = 0;

	if(pOriginalText) norg = _tcslen(pOriginalText);
	if(pTranslationText) ntrans = _tcslen(pTranslationText);

	fwrite(&norg,sizeof(int),1,pFile);
	fwrite(&ntrans,sizeof(int),1,pFile);
	fwrite(&nStatus,sizeof(int),1,pFile);
	
	if(pOriginalText) fwrite((char*)pOriginalText,norg*sizeof(TCHAR),1,pFile);
	if(pTranslationText) fwrite((char*)pTranslationText,ntrans*sizeof(TCHAR),1,pFile);
	fwrite((char*)szLabel, 32*sizeof(TCHAR), 1, pFile);
	return true;
}

void CText_TalkWindow::Init(bool bMuil, TCHAR *pOrgText, TCHAR *pTransText, TCHAR *pTextName)
{
	pOriginalText = NULL;
	pTranslationText = NULL;

	int norg = 0;
	int ntrans = 0;

	if(pOrgText) norg = _tcslen(pOrgText);
	if(pTransText) ntrans = _tcslen(pTransText);
	
	bMuiltiLine = bMuil;
	
	if(norg > 0) pOriginalText = new TCHAR[norg+1];
	if(ntrans > 0) pTranslationText = new TCHAR[ntrans+1];
	
	if(pOriginalText) lstrcpy(pOriginalText, pOrgText);
	if(pTransText) lstrcpy(pTranslationText,pTransText);
	if(pTextName) lstrcpy(szTextName,pTextName);
}

bool CText_TalkWindow::CheckSyntax(CString strDest, int& start, int& len, bool bMsg)
{
	if(strDest.GetLength()==0)
	{
		if(bMsg)
		AfxMessageBox(_T("Translation text can't empty!"));
		return false;
	}
	
	if(pOriginalText)
	{
		abase::vector<SYNTAX_ITEM> listSyntaxOrg;
		abase::vector<SYNTAX_ITEM> listSyntaxDest;
		GetSyntaxString(pOriginalText,&listSyntaxOrg);
		GetSyntaxString(strDest,&listSyntaxDest);
		
		bool bPass = true;
		
		for( int i = 0; i < listSyntaxOrg.size(); ++i)
		{
			if(i < listSyntaxDest.size())
			{
				if(_tcsicmp(listSyntaxOrg[i].txt,listSyntaxDest[i].txt)!=0)
				{
					start = listSyntaxOrg[i].start;
					len = listSyntaxOrg[i].len;
					bPass = false;
					break;
				}
			}else 
			{
				start = listSyntaxOrg[i].start;
				len = listSyntaxOrg[i].len;
				bPass = false;
				break;
			}
		}
	

		if(!bPass)
		{
			CString msg(_T("Syntax Error!\nOriginal Syntax:\n"));
			for( int i = 0; i < listSyntaxOrg.size(); ++i)
			{
				msg +=_T("  ");
				msg += listSyntaxOrg[i].txt;
			}
			
			msg +=_T("\nTranslation Syntax:\n");
			for( i = 0; i < listSyntaxDest.size(); ++i)
			{
				msg +=_T("  ");
				msg += listSyntaxDest[i].txt;
			}
			if(bMsg)
			AfxMessageBox(msg);
			return false;
		}

		if(listSyntaxOrg.size() < listSyntaxDest.size())
		{
			CString msg(_T("Syntax Error!\nTranslation syntax of being redundant:\n"));
			
			for( int i = 0; i < listSyntaxDest.size() - listSyntaxOrg.size(); ++i)
			{
				msg +=_T("  ");
				msg += listSyntaxDest[listSyntaxOrg.size() + i].txt;
			}
			if(bMsg)
			AfxMessageBox(msg);
			return false;
		}

	}
	return true;
}

CTextBase* CText_TalkWindow::Clone()
{
	CText_TalkWindow *pNew = new CText_TalkWindow();
	if(pNew==NULL) return NULL;
	pNew->Init(bMuiltiLine,pOriginalText,pTranslationText,szTextName);
	pNew->SetStatus(nStatus);
	pNew->SetID(id);
	return pNew;
}

bool CText_TalkWindow::ReplaceOriginalText(const TCHAR* pNewText)
{
	int len = 0;
	if(pNewText) len = _tcslen(pNewText);
	if(len>0)
	{
		TCHAR *pNew = new TCHAR[len + 1];
		lstrcpy(pNew,pNewText);
		if(pOriginalText) delete[] pOriginalText;
		pOriginalText = pNew;
	}else
	{
		if(pOriginalText)
		{
			delete[] pOriginalText;
			pOriginalText = NULL;
		}
	}
	return true;
}

bool CText_TalkWindow::ReplaceTransText(const TCHAR *pNewText)
{
	//if(!CheckSyntax(pNewText)) return false;
	int len = 0;
	if(pNewText) len = _tcslen(pNewText);

	if(len>0)
	{
		TCHAR *pNew = new TCHAR[len+1];
		lstrcpy(pNew,pNewText);
		if(pTranslationText) delete[] pTranslationText;
		pTranslationText = pNew;
	}else
	{
		if(pTranslationText)
		{
			delete[] pTranslationText;
			pTranslationText = NULL;
		}
	}
	if(nStatus==status_new) nStatus = status_normal;
	return true;
}

//////////////////////////////////////////////////////////////////////
//CText_TalkOption
//////////////////////////////////////////////////////////////////////
CText_TalkOption::CText_TalkOption()
{
	nLimit = 0;
	id = -1;
	par = -1;
	bIsIdVaild = true;
	nStatus = status_new;
	pOriginalText = NULL;
	pTranslationText = NULL;
}

CText_TalkOption::~CText_TalkOption()
{
	Release();
}

CTextBase* CText_TalkOption::Clone()
{
	CText_TalkOption *pNew = new CText_TalkOption();
	if(pNew==NULL) return NULL;
	pNew->Init(nLimit,bMuiltiLine,pOriginalText,pTranslationText,szTextName);
	pNew->SetStatus(nStatus);
	pNew->SetID(GetID());
	return pNew;
}

bool CText_TalkOption::Load(FILE *pFile, DWORD dwVersion)
{
	if(dwVersion <= 2)
	{
		fread(&nStatus,sizeof(int),1,pFile);
		fread((char*)pOriginalText,nLimit*sizeof(TCHAR),1,pFile);
		fread((char*)pTranslationText,nLimit*sizeof(TCHAR),1,pFile);
	}else if(dwVersion <=4)
	{
		fread(&id,sizeof(int),1,pFile);
		fread(&nStatus,sizeof(int),1,pFile);
		fread((char*)pOriginalText,nLimit*sizeof(TCHAR),1,pFile);
		fread((char*)pTranslationText,nLimit*sizeof(TCHAR),1,pFile);
	}else
	{
		fread(&id,sizeof(int),1,pFile);
		fread(&par,sizeof(int),1,pFile);
		fread(&nStatus,sizeof(int),1,pFile);
		fread((char*)pOriginalText,nLimit*sizeof(TCHAR),1,pFile);
		fread((char*)pTranslationText,nLimit*sizeof(TCHAR),1,pFile);
	}

	if(dwVersion > 0x10000007)
	{
		fread((char*)szLabel, 32*sizeof(TCHAR), 1, pFile);
	}

	return true;
}

bool CText_TalkOption::Save(FILE *pFile)
{
	fwrite(&id,sizeof(int),1,pFile);
	fwrite(&par,sizeof(int),1,pFile);
	fwrite(&nStatus,sizeof(int),1,pFile);
	fwrite((char*)pOriginalText,nLimit*sizeof(TCHAR),1,pFile);
	fwrite((char*)pTranslationText,nLimit*sizeof(TCHAR),1,pFile);
	fwrite((char*)szLabel, 32*sizeof(TCHAR), 1, pFile);
	return true;
}


//////////////////////////////////////////////////////////////////////
//CText_Resize
//////////////////////////////////////////////////////////////////////

CText_Resize::CText_Resize()
{
	nLimit = -1;//表示没有长度的限制
	bMuiltiLine = true;
	nStatus = status_new;
	pOriginalText = NULL;
	pTranslationText = NULL;
	bIsIdVaild = false;
	m_nMaxLimit = -1;
}

CText_Resize::~CText_Resize()
{
	Release();
}

bool CText_Resize::Load(FILE *pFile, DWORD dwVersion)
{
	Release();
	
	int norg = 0;
	int ntrans = 0;

	fread(&norg,sizeof(int),1,pFile);
	fread(&ntrans,sizeof(int),1,pFile);
	fread(&nStatus,sizeof(int),1,pFile);
	
	if(norg > 0) pOriginalText = new TCHAR[norg+1];
	if(ntrans > 0) pTranslationText = new TCHAR[ntrans+1];

	if(pOriginalText) memset(pOriginalText,'\0',(norg + 1)*sizeof(TCHAR));
	if(pTranslationText) memset(pTranslationText,'\0',(ntrans + 1)*sizeof(TCHAR));
	
	if(norg > 0) fread((char*)pOriginalText,norg*sizeof(TCHAR),1,pFile);
	if(ntrans > 0) fread((char*)pTranslationText,ntrans*sizeof(TCHAR),1,pFile);

	if(dwVersion > 0x10000007)
	{
		fread((char*)szLabel, 32*sizeof(TCHAR), 1, pFile);
	}

	
	if(dwVersion < 10)
	{
		int ln1 = Aux_GetLineNum(pOriginalText);
		int ln2 = Aux_GetLineNum(pTranslationText);
		if(ln1 < ln2)
		{
			CString temp(pTranslationText);
			int ln = ln2 - ln1;
			for( int i = 0; i < ln; ++i)
			{
				int pos = temp.Find(_T("\r\n"));
				CString left = temp.Left(pos);
				CString right = temp.Right(temp.GetLength() - pos - 2);
				temp = left + right;
			}
			lstrcpy(pTranslationText,temp);
		}
	}

	return true;
}

bool CText_Resize::Save(FILE *pFile)
{
	int norg = 0;
	int ntrans = 0;

	if(pOriginalText) norg = _tcslen(pOriginalText);
	if(pTranslationText) ntrans = _tcslen(pTranslationText);

	fwrite(&norg,sizeof(int),1,pFile);
	fwrite(&ntrans,sizeof(int),1,pFile);
	fwrite(&nStatus,sizeof(int),1,pFile);
	
	if(pOriginalText) fwrite((char*)pOriginalText,norg*sizeof(TCHAR),1,pFile);
	if(pTranslationText) fwrite((char*)pTranslationText,ntrans*sizeof(TCHAR),1,pFile);

	fwrite((char*)szLabel, 32*sizeof(TCHAR), 1, pFile);
	
	return true;
}

void CText_Resize::Init(bool bMuil, TCHAR *pOrgText, TCHAR *pTransText, TCHAR *pTextName)
{
	pOriginalText = NULL;
	pTranslationText = NULL;

	int norg = 0;
	int ntrans = 0;

	if(pOrgText) norg = _tcslen(pOrgText);
	if(pTransText) ntrans = _tcslen(pTransText);
	
	bMuiltiLine = bMuil;
	
	if(norg > 0) pOriginalText = new TCHAR[norg+1];
	if(ntrans > 0) pTranslationText = new TCHAR[ntrans+1];
	
	if(pOriginalText) lstrcpy(pOriginalText, pOrgText);
	if(pTransText) lstrcpy(pTranslationText,pTransText);
	if(pTextName) lstrcpy(szTextName,pTextName);
}

bool CText_Resize::CheckSyntax(CString strDest, int& start, int& len, bool bMsg)
{
	if(strDest.GetLength()==0)
	{
		if(bMsg)
		AfxMessageBox(_T("Translation text cannot empty!"));
		return false;
	}

	if(m_nMaxLimit != -1 && strDest.GetLength() >= m_nMaxLimit)
	{		
		if(bMsg)
		{
			CString csWarning;
			csWarning.Format(_T("The length of translation text is longer than the limit %d characters"), m_nMaxLimit);
			AfxMessageBox(csWarning);
		}
		return false;
	}
	
	if(pOriginalText)
	{
		if(Aux_GetLineNum(pOriginalText)!=Aux_GetLineNum(strDest))
		{
			if(bMsg)
			AfxMessageBox(_T("Translation text line number not equal to original text!"));
			return false;
		}

		abase::vector<SYNTAX_ITEM> listSyntaxOrg;
		abase::vector<SYNTAX_ITEM> listSyntaxDest;
		GetSyntaxString(pOriginalText,&listSyntaxOrg);
		GetSyntaxString(strDest,&listSyntaxDest);
		
		bool bPass = true;
		
		for( int i = 0; i < listSyntaxOrg.size(); ++i)
		{
			if(i < listSyntaxDest.size())
			{
				if(_tcsicmp(listSyntaxOrg[i].txt,listSyntaxDest[i].txt)!=0)
				{
					start = listSyntaxOrg[i].start;
					len = listSyntaxOrg[i].len;
					bPass = false;
					break;
				}
			}else 
			{
				start = listSyntaxOrg[i].start;
				len = listSyntaxOrg[i].len;
				bPass = false;
				break;
			}
		}
	

		if(!bPass)
		{
			CString msg(_T("Syntax Error!\nOriginal Syntax:\n"));
			for( int i = 0; i < listSyntaxOrg.size(); ++i)
			{
				msg +=_T("  ");
				msg += listSyntaxOrg[i].txt;
			}
			
			msg +=_T("\nTranslation Syntax:\n");
			for( i = 0; i < listSyntaxDest.size(); ++i)
			{
				msg +=_T("  ");
				msg += listSyntaxDest[i].txt;
			}
			if(bMsg)
			AfxMessageBox(msg);
			return false;
		}

		if(listSyntaxOrg.size() < listSyntaxDest.size())
		{
			CString msg(_T("Syntax Error!\nTranslation syntax of being redundant:\n"));
			
			for( int i = 0; i < listSyntaxDest.size() - listSyntaxOrg.size(); ++i)
			{
				msg +=_T("  ");
				msg += listSyntaxDest[listSyntaxOrg.size() + i].txt;
			}
			if(bMsg)
			AfxMessageBox(msg);
			return false;
		}

	}
	return true;
}

CTextBase* CText_Resize::Clone()
{
	CText_Resize *pNew = new CText_Resize();
	if(pNew==NULL) return NULL;
	pNew->Init(bMuiltiLine,pOriginalText,pTranslationText,szTextName);
	pNew->SetStatus(nStatus);
	return pNew;
}

bool CText_Resize::ReplaceOriginalText(const TCHAR* pNewText)
{
	int len = 0;
	if(pNewText) len = _tcslen(pNewText);
	if(len>0)
	{
		TCHAR *pNew = new TCHAR[len + 1];
		lstrcpy(pNew,pNewText);
		if(pOriginalText) delete [] pOriginalText;
		pOriginalText = pNew;
	}else
	{
		if(pOriginalText)
		{
			delete [] pOriginalText;
			pOriginalText = NULL;
		}
	}
	return true;
}

bool CText_Resize::ReplaceTransText(const TCHAR *pNewText)
{
	//if(!CheckSyntax(pNewText)) return false;
	int len = 0;
	if(pNewText) len = _tcslen(pNewText);

	if(len>0)
	{
		TCHAR *pNew = new TCHAR[len+1];
		lstrcpy(pNew,pNewText);
		if(pTranslationText) delete [] pTranslationText;
		pTranslationText = pNew;
	}else
	{
		if(pTranslationText)
		{
			delete [] pTranslationText;
			pTranslationText = NULL;
		}
	}
	if(nStatus==status_new) nStatus = status_normal;
	return true;
}

bool CText_ScriptFile::CheckSyntax(CString strDest, int& start, int& len, bool bMsg)
{
	if(strDest.GetLength()==0)
	{
		if(bMsg)
		AfxMessageBox(_T("Translation text cannot empty!"));
		return false;
	}

	int nBufLen = WideCharToMultiByte(CP_ACP,0,strDest,strDest.GetLength(),NULL,0,NULL,NULL);
	char *pBuffer = new char[nBufLen+1];
	int rlen = WideCharToMultiByte(CP_ACP,0,strDest,strDest.GetLength(),pBuffer,nBufLen,NULL,NULL);
	pBuffer[rlen] = 0;
	bool bRet = LuaBind::DoBuffer(g_LuaStateMan.GetConfigState()->GetVM(), pBuffer, rlen, "test");
	delete []pBuffer;
	return bRet;
}





