#ifndef BASEDATAIDMAN_H_
#define BASEDATAIDMAN_H_

#include "hashmap.h"
#include "AString.h"

typedef abase::hash_map<AString, unsigned long> BaseDataPathIDMap;
typedef abase::hash_map<unsigned long, AString> BaseDataIDPathMap;
typedef abase::hash_map<unsigned long, unsigned long> BaseDataIDVersionMap;

class BaseDataIDManBase
{
public:
	BaseDataIDManBase() : m_ulMaxID(0) {}
	virtual ~BaseDataIDManBase() {}

protected:
	BaseDataPathIDMap m_PathIDMap;
	BaseDataIDPathMap m_IDPathMap;
	BaseDataIDVersionMap	m_IDVersionMap;
	AString m_strFile;
	unsigned long m_ulMaxID;

protected:
	static bool CheckOut(const char* szPath);
	static bool CheckIn(const char* szPath);
	static bool CanEdit(const char* szPath);
	virtual bool IsRightType(const char* szType) const;
	virtual const char* GetType() const;
	virtual bool Save();
	virtual unsigned long IncID(unsigned long ulID) { return ulID + 1; }
	int Load(
		const char* szPath,
		BaseDataPathIDMap& PathMap,
		BaseDataIDPathMap& IDMap,
		unsigned long& ulMaxID);

public:
	/*
	 *	return 0  成功
	 *		   -1 文件打开失败
	 *		   -2 有ID或Path重名
	 *		   -3 非此类型文件
	 */
	virtual int Load(const char* szPath);
	virtual void Release()
	{ 
		m_PathIDMap.clear();
		m_IDPathMap.clear();
		m_IDVersionMap.clear();
		m_strFile.Empty();
	}
	virtual const char* GetPathByID(unsigned long ulID) const
	{
		BaseDataIDPathMap::const_iterator it = m_IDPathMap.find(ulID);
		return it == m_IDPathMap.end() ? "" : it->second;
	}
	
	virtual unsigned long GetIDByPath(const AString& strPath) const
	{
		BaseDataPathIDMap::const_iterator it = m_PathIDMap.find(strPath);
		return it == m_PathIDMap.end() ? 0 : it->second;
	}

	virtual unsigned long CreateID(const AString& strPath);
	virtual unsigned long CreateIDExt( unsigned long ulID, const AString& strPath ); //通过指定模板ID来实现，Added 2011-04-18
	void GeneratePathArray(abase::vector<AString>& PathArray);
	void WipeOffByPath(abase::vector<AString>& WipeOffArray);
	void WipeOffByID(abase::vector<unsigned long>& WipeOffArray);
	BaseDataIDPathMap& GetIDPathMap() { return m_IDPathMap; }	//提供一个这样的接口，方便对这个map数据进行相关导出操作，暂时不做其他使用！Added 2012-03-27.

	bool CanViewTempl(const AString& strPath) const;
	int  VersionState(unsigned long ulID) const;		// return value: enum VersionState
	int  VersionState(const AString& strPath) const;

	unsigned long GetIdNum()
	{
		return m_PathIDMap.size();
	}
	unsigned long GetMaxID()
	{
		return m_ulMaxID;
	}
	
	bool CanExport(unsigned long ulID) const;
	bool HasVersionInfo(char *str);
};

class ExtDataIDMan : public BaseDataIDManBase
{
public:
	ExtDataIDMan() {}
	virtual ~ExtDataIDMan() {}

protected:
	virtual bool IsRightType(const char* szType) const;
	virtual const char* GetType() const;
	virtual unsigned long IncID(unsigned long ulID);
};

class DataPathIDMan : public BaseDataIDManBase
{
public:
	DataPathIDMan() {}
	virtual ~DataPathIDMan() {}

	int Export(const char *szPath);

protected:
	virtual bool IsRightType(const char* szType) const;
	virtual const char* GetType() const;
};

extern BaseDataIDManBase g_BaseIDMan;
extern BaseDataIDManBase g_TaskIDMan;
extern BaseDataIDManBase g_TalkIDMan;
extern BaseDataIDManBase g_PolicyIDMan;
extern BaseDataIDManBase g_RecipeIDMan;
extern BaseDataIDManBase g_ConfigIDMan;
extern BaseDataIDManBase g_QuestionsIDMan;
extern ExtDataIDMan g_ExtBaseIDMan;
extern DataPathIDMan  g_PathIDMan;
//海外预留ID相关控制变量 Added 2011-04-18.
extern bool	g_bOverseasEnable;
//Added end.


#endif