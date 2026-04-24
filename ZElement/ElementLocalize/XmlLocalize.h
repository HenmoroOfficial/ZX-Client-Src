// XmlLocalize.h: interface for the CXmlLocalize class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLLOCALIZE_H__4689EA47_2866_44B7_9F53_97998DEBD604__INCLUDED_)
#define AFX_XMLLOCALIZE_H__4689EA47_2866_44B7_9F53_97998DEBD604__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <vector>
#include "common.h"

struct XmlLocItem
{
	enum
	{ eLabelItem = 0, eOriginItem, eTransItem, eLimitItem, eLinenumItem, eIDItem, eItemNum };
	CString _items[eItemNum];
	void clear()
	{
		for (int i = eLabelItem; i < eItemNum; i++)
			_items[i].ReleaseBuffer();
	}
};

class CXmlLocalize  
{	
public:	
	enum 
	{
		MODE_READ,
		MODE_WRITE
	};
	CXmlLocalize();
	virtual ~CXmlLocalize();
	bool Open(const char* szXmlFile, int mode);
	bool LoadItems(void* pThdParam, const std::vector<BOOL>& vFilter);
	void Close();
	bool ReadItem(XmlLocItem& item);
	bool ReadItem(XmlLocItem& item, const std::vector<BOOL>* pFilter);
	bool WriteItem(const XmlLocItem& item);
	bool AddPage(const TCHAR* szPage);
	int GetCurPercent(); // 0-100
	const std::vector<XmlLocItem>* GetItems(int chunktype);

protected:
	bool CheckUnicode(FILE* fp);	
	bool ReadLine(FILE *pFile,CString& strLine,bool bUnicode);
	bool ReadLine(FILE *pFile,TCHAR *szLine,int& len,bool bUnicode);
	bool ReadLineA(FILE *pFile,char *szLine,int& len);
	bool ReadLineW(FILE *pFile,TCHAR *szLine,int& len);
	bool WriteCell(const TCHAR* content);
	bool WriteHeader();
	TCHAR* GetContent(TCHAR* szXmlItem);
	CString str_to_xml(const TCHAR* szBuf);
	CString xml_to_str(const TCHAR* szBuf);
	int GetPageID(const TCHAR* szBuf);

protected:
	bool m_bUnicode;
	FILE* m_fp;
	int m_nRowInPage;
	int m_nPage;
	int m_idCurPage;
	int m_mode;
	int m_filesize;
	std::vector<XmlLocItem> m_page[CHUNK_NUM];
};


#endif // !defined(AFX_XMLLOCALIZE_H__4689EA47_2866_44B7_9F53_97998DEBD604__INCLUDED_)
