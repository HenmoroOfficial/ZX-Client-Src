#include "AAssist.h"
#include "hashmap.h"

typedef abase::hash_map<int, ACString> StringTableFile;
typedef abase::hash_map<ACString, ACString> HintTableFile;
typedef abase::vector<ACString> InterfaceFileNames;

typedef abase::hash_map<ACString, StringTableFile*> InterfaceStringTables;
typedef abase::hash_map<ACString, HintTableFile*> InterfaceHintTables;

class CElementInterfaceManager
{
public:
	CElementInterfaceManager(const char *szWorkPath);
	~CElementInterfaceManager();

	bool LoadInterface();
	bool SaveInterface();
	InterfaceFileNames* GetStringTableFiles() { return &m_vecStf; }
	InterfaceFileNames* GetHintTableFiles() { return &m_vecHtf; }
	InterfaceFileNames* GetHelpScriptFiles() { return &m_vecHelpScriptFile; }
	InterfaceFileNames* GetHelpTextFiles() { return &m_vecHelpTextFile; }
	StringTableFile* GetStringTable(const ACHAR *szFileName);
	HintTableFile* GetHintTable(const ACHAR *szFileName);
	HintTableFile* GetHelpScriptTable(const ACHAR *szFileName);
	HintTableFile* GetHelpTextTable(const ACHAR *szFileName);

private:
	bool ImportTables();
	void Release();

private:
	InterfaceFileNames m_vecStf;
	InterfaceFileNames m_vecHtf;
	InterfaceFileNames m_vecHelpScriptFile;
	InterfaceFileNames m_vecHelpTextFile;
	InterfaceStringTables m_mapInterfaceStringTables;
	InterfaceHintTables m_mapInterfaceHintTables;
	InterfaceHintTables m_mapHelpScriptTables;
	InterfaceHintTables m_mapHelpTextTables;
	char m_szWorkPath[2000];
};