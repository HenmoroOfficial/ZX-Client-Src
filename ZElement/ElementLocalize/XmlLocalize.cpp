// XmlLocalize.cpp: implementation of the CXmlLocalize class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XmlLocalize.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define MAX_BUFFER_LEN 102400
#define MAX_CHAR_IN_ONE_LINE 32
#define MAX_ROW_INPAGE 10000


static const TCHAR *g_szHeader = _T("\
<?xml version=\"1.0\"?>\r\n\
<?mso-application progid=\"Excel.Sheet\"?>\r\n\
<Workbook xmlns=\"urn:schemas-microsoft-com:office:spreadsheet\"\r\n\
xmlns:o=\"urn:schemas-microsoft-com:office:office\"\r\n\
xmlns:x=\"urn:schemas-microsoft-com:office:excel\"\r\n\
xmlns:ss=\"urn:schemas-microsoft-com:office:spreadsheet\"\r\n\
xmlns:html=\"http://www.w3.org/TR/REC-html40\">\r\n\
<ExcelWorkbook xmlns=\"urn:schemas-microsoft-com:office:excel\">\r\n\
<ProtectStructure>False</ProtectStructure>\r\n\
<ProtectWindows>False</ProtectWindows>\r\n\
</ExcelWorkbook>\r\n\
<Styles>\r\n\
<Style ss:ID=\"Default\" ss:Name=\"Normal\">\r\n\
</Style>\r\n\
<Style ss:ID=\"s21\" ss:Name=\"s21\">\r\n\
<Alignment ss:WrapText=\"1\"/>\r\n\
</Style>\r\n\
</Styles>\r\n\
");

static const TCHAR *g_szTail = _T("\
</Workbook>\r\n\
");

static const TCHAR *g_szSheetTail = _T("\
</Table>\r\n\
</Worksheet>\r\n\
"); 

static const TCHAR *g_szSheetHeader = _T("\
<Worksheet ss:Name=\"%s\">\r\n\
<Table ss:DefaultColumnWidth=\"400\">\r\n\
");

CXmlLocalize::CXmlLocalize()
{
	m_bUnicode = true;
	m_fp = NULL;
	m_nRowInPage = 0;
	m_mode = MODE_READ;
	m_nPage = 0;
	m_filesize = 0;
	m_idCurPage = -1;
}

CXmlLocalize::~CXmlLocalize()
{
	Close();
}

int CXmlLocalize::GetCurPercent()
{
	if (m_mode == MODE_WRITE || m_filesize == 0 || !m_fp)
		return 100;

	return (int) ( (ftell(m_fp) / (float)m_filesize) * 100 );
}

bool CXmlLocalize::Open(const char* szXmlFile, int mode)
{
	m_filesize = 0;
	m_fp = fopen(szXmlFile, mode == MODE_READ ? "rb" : "wb");
	if (!m_fp)
		return false;
	
	if (mode == MODE_READ && !CheckUnicode(m_fp))
	{
		Close();
		return false;
	}

	m_mode = mode;
	return true;
}

void CXmlLocalize::Close()
{
	if (!m_fp)
		return;
	if (m_mode == MODE_WRITE)
	{
		if (0 == ftell(m_fp))
		{
			if (!WriteHeader())
			{
				fclose(m_fp);
				return;
			}
		}
		else
			fwrite(g_szSheetTail,_tcsclen(g_szSheetTail)*sizeof(TCHAR),1,m_fp);
		fwrite(g_szTail,_tcsclen(g_szTail)*sizeof(TCHAR),1,m_fp);
	}
	fclose(m_fp);
	m_fp = NULL;
}

bool CXmlLocalize::WriteCell(const TCHAR* content)
{
	if (!m_fp || m_mode != MODE_WRITE)
		return false;

	CString strTemp;	
	strTemp = str_to_xml(content);
	fwrite(strTemp.GetBuffer(0),strTemp.GetLength()*sizeof(TCHAR),1,m_fp);
	fwrite(_T("\r\n"), 4, 1, m_fp);
	strTemp.ReleaseBuffer();

	return true;
}

bool CXmlLocalize::WriteHeader()
{
	if (!m_fp || m_mode != MODE_WRITE)
		return false;

	WORD wFlag = 0xfeff;
	fwrite(&wFlag,2,1,m_fp);
	fwrite(g_szHeader,_tcsclen(g_szHeader)*sizeof(TCHAR),1,m_fp);
	
	m_nPage = 0;

	return true;
}


bool CXmlLocalize::AddPage(const TCHAR* szPage)
{
	if (!m_fp || m_mode != MODE_WRITE)
		return false;

	if (0 == ftell(m_fp))
	{
		if (!WriteHeader())
			return false;
	}

	if (m_nPage > 0)
	{
		fwrite(g_szSheetTail,_tcsclen(g_szSheetTail)*sizeof(TCHAR),1,m_fp);
		fwrite(_T("\r\n\r\n"), 8, 1, m_fp);		
	}
	TCHAR szSheetHead[256];
	_stprintf(szSheetHead,g_szSheetHeader,szPage);
	fwrite(szSheetHead,_tcsclen(szSheetHead)*sizeof(TCHAR),1,m_fp);	

	m_nRowInPage = 0;
	m_nPage++;

	return true;
}

bool CXmlLocalize::WriteItem(const XmlLocItem& item)
{
	if (!m_fp || m_mode != MODE_WRITE)
		return false;
	if (0 == ftell(m_fp))
	{
		if (!WriteHeader())
			return false;
	}

	if (m_nPage == 0)
		AddPage(_T("Localize0"));

	fwrite(_T("<Row>"),_tcsclen(_T("<Row>"))*sizeof(TCHAR),1,m_fp);
	fwrite(_T("\r\n"), 4, 1, m_fp);
	
	for (int i = XmlLocItem::eLabelItem; i < XmlLocItem::eItemNum; ++i)
		WriteCell(item._items[i]);
	
	fwrite(_T("</Row>"),_tcsclen(_T("</Row>"))*sizeof(TCHAR),1,m_fp);
	fwrite(_T("\r\n\r\n"), 8, 1, m_fp);

	m_nRowInPage++;

	return true;
}

bool CXmlLocalize::CheckUnicode(FILE* fp)
{
	if (!fp || m_mode != MODE_READ)
		return false;

	// size
	fseek(fp, 0, SEEK_END);
	m_filesize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	
	WORD flag;
	fread(&flag,2,1,fp);
	if(flag!=0xfeff)
	{
		fseek(fp,0,SEEK_SET);
		m_bUnicode = false;
	}
	else
	{
		m_bUnicode = true;
	}

	return true;
}

TCHAR* CXmlLocalize::GetContent(TCHAR* szXmlItem)
{
	if (!szXmlItem) 
		return _T("");

	TCHAR* posBeg = _tcsstr(szXmlItem, _T("<Data ss:Type=\"String\""));
	if(posBeg != NULL)
	{
		posBeg += _tcsclen(_T("<Data ss:Type=\"String\""));
		posBeg = _tcsstr(posBeg, _T(">"));
		if (!posBeg)
			return _T("");
		posBeg += _tcslen(_T(">"));
		
		TCHAR* posEnd = _tcsstr(posBeg, _T("</Data>"));
		if (!posEnd)
			return _T("");
		*posEnd = '\0';
		return posBeg;

	}else
	{
		posBeg = _tcsstr(szXmlItem, _T("<ss:Data ss:Type=\"String\""));
		if (!posBeg)
			return _T("");
		posBeg += _tcsclen(_T("<ss:Data ss:Type=\"String\""));
		posBeg = _tcsstr(posBeg, _T(">"));
		if (!posBeg)
			return _T("");
		posBeg += _tcslen(_T(">"));
		
		TCHAR* posEnd = _tcsstr(posBeg, _T("</ss:Data>"));
		if (!posEnd)
			return _T("");
		*posEnd = '\0';
		return posBeg;
	}
}

int CXmlLocalize::GetPageID(const TCHAR* szBuf)
{
	CString str = szBuf;
	str = str.Right(str.GetLength() - _tcslen(_T("<Worksheet ss:Name=\"")));
	str = str.Left(str.GetLength() - _tcslen(_T("\">")));
	for (int i=0; i<CHUNK_NUM; ++i)
	{
		if (str.CompareNoCase(g_szChunk[i]) == 0)
			return i;
	}
	return -1;
}

bool CXmlLocalize::ReadItem(XmlLocItem& item)
{
	return ReadItem(item, NULL);
}

bool CXmlLocalize::ReadItem(XmlLocItem& item, const std::vector<BOOL>* pFilter)
{
	item.clear();
	FILE* fp = m_fp;
	if (!fp || m_mode != MODE_READ)
		return false;

	while (true)
	{
		int nLen = MAX_BUFFER_LEN;
		CString strLine;
		if (!ReadLine(fp, strLine, m_bUnicode))
			return false;

		if (_tcsstr(strLine, _T("<Row")) != NULL)
			break;
		if (_tcsstr(strLine, _T("<Worksheet ss:Name=")) != NULL)
			m_idCurPage = GetPageID(strLine);
	}

	if (pFilter)
	{
		if (m_idCurPage < 0 || m_idCurPage >= CHUNK_NUM || !(*pFilter)[m_idCurPage])
			return true;
	}

	int iCell = XmlLocItem::eLabelItem;
	bool bCellStart = false;
	CString strCell;
	while (true)
	{
		int nLen = MAX_BUFFER_LEN;
		CString strLine;
		if (!ReadLine(fp, strLine, m_bUnicode))
			return false;

		if (_tcsstr(strLine, _T("Row>")) != NULL)
			break;

		if (!bCellStart && _tcsstr(strLine, _T("<Cell")) == NULL)
			continue;

		strCell += strLine;
		if (_tcsstr(strLine, _T("/Cell>")) != NULL)
			bCellStart = false;
		else
			bCellStart = true;

		if (!bCellStart)
		{
			item._items[iCell] = xml_to_str(GetContent(strCell.GetBuffer(0)));
			strCell.ReleaseBuffer();
			strCell = _T("");
			iCell++;
		}

		if (iCell >= XmlLocItem::eItemNum)
			break;
	}
	
	return true;
}

bool CXmlLocalize::ReadLine(FILE *pFile,CString& strLine,bool bUnicode)
{
	int len = MAX_BUFFER_LEN;
	strLine = _T("");
	do 
	{
		TCHAR szLine[MAX_BUFFER_LEN] = {0};
		if (!ReadLine(pFile, szLine, len, bUnicode))
			return false;
		strLine += szLine;
	} while (len >= MAX_BUFFER_LEN - 2);

	return true;
}

bool CXmlLocalize::ReadLine(FILE *pFile,TCHAR *szLine,int& len,bool bUnicode)
{
	if(bUnicode) return ReadLineW(pFile,szLine,len);
	else
	{
		char szBuffer[MAX_BUFFER_LEN];
		memset(szBuffer,0,MAX_BUFFER_LEN);
		int nMaxlen = MAX_BUFFER_LEN;
		bool r = ReadLineA(pFile,szBuffer,nMaxlen);
		len = strlen(szBuffer);
		if(len>0) 
		{
			int l = MultiByteToWideChar(CP_UTF8,0,szBuffer,strlen(szBuffer),szLine,MAX_BUFFER_LEN*sizeof(TCHAR));
			szLine[l] = 0;
		}
		return r;
	}
}

bool CXmlLocalize::ReadLineW(FILE *pFile,TCHAR *szLine,int& len)
{
	WCHAR ch,ch2;
	int nMaxLen = len;
	len = 0;
	while(true)
	{
		if (len >= nMaxLen - 2)
		{
			szLine[len] = 0;
			return true;
		}

		if(0==fread(&ch,sizeof(WCHAR),1,pFile)) 
		{
			szLine[len] = 0;
			return false;
		}
		if(ch=='\r') 
		{
			if(0==fread(&ch2,sizeof(WCHAR),1,pFile)) 
			{
				szLine[len] = 0;
				return false;
			}
			if(ch2=='\n')
			{
				szLine[len] = 0;
				return true;
			}
			else
			{
				szLine[len++] = ch;
				ch = ch2;
			}
		}
		szLine[len++] = ch;
	}
}

bool CXmlLocalize::ReadLineA(FILE *pFile,char *szLine,int& len)
{
	char ch,ch2;
	int nMaxLen = len;
	len = 0;
	bool bStart = false;
	while(true)
	{
		if (len >= nMaxLen - 2)
		{
			szLine[len] = 0;
			return true;
		}

		if(0==fread(&ch,sizeof(char),1,pFile)) 
		{
			szLine[len] = 0;
			return false;
		}
		if(ch=='\r') 
		{
			if(0==fread(&ch2,sizeof(char),1,pFile)) 
			{
				szLine[len] = 0;
				return false;
			}
			if(ch2=='\n')
			{
				szLine[len] = 0;
				return true;
			}
			return true;
		}
		
		if( ch != ' ')
			bStart = true;
		if( ch == ' ' && !bStart) continue;
		
		szLine[len++] = ch;
	}
}



CString CXmlLocalize::str_to_xml(const TCHAR* szBuf)
{	
	CString str;

	CString strTemp = szBuf;
	strTemp.Replace(_T("&"),_T("&amp;"));
	strTemp.Replace(_T("<"),_T("&lt;"));
	strTemp.Replace(_T(">"),_T("&gt;"));
	
	int len = 0;
	if (szBuf)
		len = _tcslen(szBuf);

	if( len > MAX_CHAR_IN_ONE_LINE )
		str.Format(_T("<Cell ss:StyleID=\"s21\"><Data ss:Type=\"String\">%s</Data></Cell>"), strTemp);
	else
		str.Format(_T("<Cell><Data ss:Type=\"String\">%s</Data></Cell>"), strTemp);
		
	str.Replace(_T("\r\n"),_T("&#10;"));
	
	return str;
}

CString CXmlLocalize::xml_to_str(const TCHAR* szBuf)
{
	CString temp = szBuf;
	temp.Replace(_T("&lt;"),_T("<"));
	temp.Replace(_T("&gt;"),_T(">"));
	temp.Replace(_T("&amp;"),_T("&"));
	temp.Replace(_T("&#10;"),_T("\r\n"));
	temp.Replace(_T("&quot;"),_T("\""));
	temp.Replace(_T("&#45;"),_T("-"));
	temp.Replace(_T("&amp;"),_T("&"));
	
	return temp;
}

bool CXmlLocalize::LoadItems(void* pThdParam, const std::vector<BOOL>& vFilter)
{
	for (int i=0; i<CHUNK_NUM; ++i)
		m_page[i].clear();

	if (!m_fp || m_mode != MODE_READ || pThdParam == NULL)
		return false;

	THD_INFO* pThdInfo = (THD_INFO*) pThdParam;

	XmlLocItem item;
	int idPage = m_idCurPage;
	while (ReadItem(item, &vFilter))
	{
		bool bLoad = (m_idCurPage != -1 && vFilter[m_idCurPage]);
		if (bLoad)
			m_page[m_idCurPage].push_back(item);

		pThdInfo->iPercent = GetCurPercent();
		if (idPage != m_idCurPage && m_idCurPage != -1)
		{
			_sntprintf(pThdInfo->szProgMsg, 512, _T("%s %s..."), bLoad ? _T("Loading") : _T("Skipping"), g_szChunk[m_idCurPage]);
			pThdInfo->bMsgUpdate = true;
		}

		idPage = m_idCurPage;
	}

	return true;
}

const std::vector<XmlLocItem>* CXmlLocalize::GetItems(int chunktype)
{
	if (chunktype < 0 || chunktype >= CHUNK_NUM)
		return NULL;

	return &m_page[chunktype];
}