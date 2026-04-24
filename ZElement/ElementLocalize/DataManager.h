// DataManager.h: interface for the CDataManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAMANAGER_H__BC5CF222_DEDD_411F_BF52_486160B69440__INCLUDED_)
#define AFX_DATAMANAGER_H__BC5CF222_DEDD_411F_BF52_486160B69440__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "DlgProgress.h"
#include "common.h"
#include "XmlLocalize.h"

typedef BOOL (WINAPI* PROCESSPROC)(void *param, int per, int totalper);

class CTemplateManager;
class CTemplateBase;
class CSelectTransDlg;
class CExcel;
class CDlgSelectItems;
class CElementInterfaceManager;
class CElementDataView;

const WORD* c_to_w( const char * szSrc);


enum 
{
	CL_ORIGIN, CL_TRANS, CL_MAX, CL_LINE, CL_ID
};

class CDataManager  
{
public:
	CDataManager();
	virtual ~CDataManager();

public:
    //输入本地多版本数据，数据做比较后进行更新处理
	bool Import(CDataManager *pNewMan,void *param, PROCESSPROC poc);
	//输入原始游戏数据
	bool ImportGameData(void *param, PROCESSPROC poc);
	//输出翻译后的数据进入目标游戏
	bool Export(char *szDistPath,void *param, PROCESSPROC poc);
	bool ImportFromText(const char *szPath, void *pWnd, bool bAccordID);
	bool ImportFromOrderText(const char *szPath, void *pWnd);

	bool Load(const TCHAR* szFile);
	bool Save(const TCHAR* szFile, CDlgSelectItems* pDlgOpt = NULL);

	static void     InitElementDataTypes();
	static void     DestoryElementDataTypes();

	static void     InitHomeTownDataTypes();
	static void     DestoryHomeTownDataTypes();

	CTemplateManager *GetElementDataTM(){ return pTM_ElementData; }
	CTemplateManager *GetElementTaskTM(){ return pTM_ElementTask; }
	CTemplateManager *GetElementPolicyTM(){ return pTM_ElementPolicy; }
	CTemplateManager *GetElementInterfaceHintTM(){ return pTM_ElementInterfaceHint; }
	CTemplateManager *GetElementInterfaceStringTM(){ return pTM_ElementInterfaceString; }
	CTemplateManager *GetElementItemDescTM(){ return pTM_ElementItemDesc; }
	CTemplateManager *GetElementInstanceTextTM(){ return pTM_ElementInstaceText;}
	CTemplateManager *GetElementItemExDescTM(){ return pTM_ElementItemExDesc; }
	CTemplateManager *GetElementSkillDescTM(){ return pTM_ElementSkillDesc; }
	CTemplateManager *GetElementFixMsgTM(){ return pTM_ElementFixMsg; }
	CTemplateManager *GetElementServerErrTM(){ return pTM_ElementServerErr; }
	CTemplateManager *GetElementBuffDescTM(){ return pTM_ElementBuffDesc; }
	CTemplateManager *GetElementTaskErrTM(){ return pTM_ElementTaskErr; }
	CTemplateManager *GetElementLoadingHintTM(){ return pTM_ElementLoadingHint; }
	CTemplateManager *GetElementPetTalkTM(){ return pTM_ElementPetTalk; }
	CTemplateManager *GetElementAddonStrTM(){ return pTM_ElementAddonStr; }	
	CTemplateManager *GetElementPrecinctTM(){ return pTM_ElementPrecinct; }
	CTemplateManager *GetElementHelpScriptTM(){ return pTM_ElementHelpScript; }
	CTemplateManager *GetElementHelpTextTM(){ return pTM_ElementHelpText; }
	//CTemplateManager *GetElementTreasureTM(){ return pTM_ElementTreasure; }
	//CTemplateManager *GetElementTransTargetTM(){return pTM_ElementTransTarget; }
	CTemplateManager *GetElementDomainInforTM(){ return pTM_ElementDomainInfor; }
	CTemplateManager *GetElementDomainInfor1TM(){ return pTM_ElementDomainInfor1; }
	CTemplateManager *GetElementGShopTM(){ return pTM_ElementGShop; }
	CTemplateManager *GetElementGShopTypesTM(){ return pTM_ElementGShopTypes; }
	CTemplateManager *GetElementGShop1TM(){ return pTM_ElementGShop1; }
	CTemplateManager *GetElementGShopTypes1TM(){ return pTM_ElementGShopTypes1; }
	CTemplateManager *GetElementGShop2TM(){ return pTM_ElementGShop2; }
	CTemplateManager *GetElementGShopTypes2TM(){ return pTM_ElementGShopTypes2; }
	CTemplateManager *GetElementTextFileTM(){ return pTM_ElementTextFile; }
	CTemplateManager *GetElementHomeTownDataTM(){ return pTM_ElementHomeTownData; }
	
	friend DWORD _stdcall ThdImportFromText(LPVOID lParam);
	friend DWORD _stdcall ThdExportToText(LPVOID lParam);
	friend DWORD _stdcall ThdImportFromOrderText(LPVOID lParam);

	//释放所有数据
	void Release();

	//统计
	void Stat( int & nTotal, int & nDisuse, int & nNew, int & nTrans);

	//自动全部转换成繁体中文
	bool TranslateToBig5();
	bool CopyOrg2Trans();
	bool ClearAllTrans();
	//输出所有文字
	bool ExportAllText(const char *szPath, bool bOnlyExportUntrans, bool bIncludeID);
	DWORD GetVersion(){ return m_dwVersion; }

	void	SetDlgSelectItems(CDlgSelectItems* pDlgSelectItems) { m_pDlgSelectItems = pDlgSelectItems; }

protected:
	DWORD            m_dwVersion;

	CDlgSelectItems*	m_pDlgSelectItems;

	CTemplateManager *pTM_ElementData;
	CTemplateManager *pTM_ElementTask;
	CTemplateManager *pTM_ElementPolicy;
	CTemplateManager *pTM_ElementInterfaceHint;
	CTemplateManager *pTM_ElementItemDesc;
	CTemplateManager *pTM_ElementItemExDesc;
	CTemplateManager *pTM_ElementSkillDesc;
	CTemplateManager *pTM_ElementFixMsg;
	CTemplateManager *pTM_ElementServerErr;
	CTemplateManager *pTM_ElementBuffDesc;
	CTemplateManager *pTM_ElementTaskErr;
	CTemplateManager *pTM_ElementLoadingHint;
	CTemplateManager *pTM_ElementPetTalk;
	CTemplateManager *pTM_ElementAddonStr;
	CTemplateManager *pTM_ElementInterfaceString;
	CTemplateManager *pTM_ElementPrecinct;
	CTemplateManager *pTM_ElementHelpScript;
	CTemplateManager *pTM_ElementHelpText;
	//CTemplateManager *pTM_ElementTreasure;
	
	//CTemplateManager *pTM_ElementTransTarget;
	CTemplateManager *pTM_ElementDomainInfor;
	CTemplateManager *pTM_ElementDomainInfor1;
	CTemplateManager *pTM_ElementGShop;
	CTemplateManager *pTM_ElementGShopTypes;
	CTemplateManager *pTM_ElementGShop1;
	CTemplateManager *pTM_ElementGShopTypes1;
	CTemplateManager *pTM_ElementGShop2;
	CTemplateManager *pTM_ElementGShopTypes2;
	CTemplateManager *pTM_ElementTextFile;
	CTemplateManager *pTM_ElementInstaceText;
	CTemplateManager *pTM_ElementHomeTownData;

	//原始数据输入
	bool			ImportElementData(void *param, PROCESSPROC poc);
	bool            ImportTaskData(void *param, PROCESSPROC poc);
	bool            ImportItemDescData(void  *param, PROCESSPROC poc);
	bool			ImportInstanceTextData(void  *param, PROCESSPROC poc);
	bool            ImportItemExDescData(void  *param, PROCESSPROC poc);
	bool            ImportItemSkillDescData(void  *param, PROCESSPROC poc);
	bool            ImportItemFixMsgData(void  *param, PROCESSPROC poc);
	bool            ImportItemServerErrData(void  *param, PROCESSPROC poc);
	bool            ImportItemBuffDescData(void *param, PROCESSPROC poc);
	bool            ImportItemTaskErrData(void *param, PROCESSPROC poc);
	bool			ImportItemLoadingHintData(void  *param, PROCESSPROC poc);
	bool			ImportItemPetTalkData(void  *param, PROCESSPROC poc);
	bool			ImportItemAddonStrData(void  *param, PROCESSPROC poc);
	bool            ImportInterfaceData(void  *param, PROCESSPROC poc);
	bool            ImportInterfaceString(void  *param, PROCESSPROC poc, CElementInterfaceManager* inter);
	bool            ImportInterfaceHint(void  *param, PROCESSPROC poc, CElementInterfaceManager* inter);
	bool            ImportHelpText(void  *param, PROCESSPROC poc, CElementInterfaceManager* inter);
	bool            ImportHelpScript(void  *param, PROCESSPROC poc, CElementInterfaceManager* inter);

	bool            ImportPolicyData(void *param, PROCESSPROC poc);
	bool            ImportPrecinctData(void *param, PROCESSPROC poc);
	//bool          ImportTreasureData(void *param, PROCESSPROC poc);
	bool            ImportDomainData(void *param, PROCESSPROC poc);
	bool			_ImportDomainInfo(void *param, PROCESSPROC poc);
	bool			_ImportDomainInfo1(void *param, PROCESSPROC poc);
	bool			_ImportGshop(void *param, PROCESSPROC poc);
	bool			_ImportGshop1(void *param, PROCESSPROC poc);
	bool			_ImportGshop2(void *param, PROCESSPROC poc);
	bool			_ImportGshopTypes(void *param, PROCESSPROC poc);
	bool			_ImportGshop1Types(void *param, PROCESSPROC poc);
	bool			_ImportGshop2Types(void *param, PROCESSPROC poc);

	bool			ImportTextFileData( void *param,  PROCESSPROC poc);
	bool			ImportHomeTownData( void *param,  PROCESSPROC poc);

	bool            EnumImpPrecinct(CString path);
	

	//多版本合并输入
	bool			ImportElementData_M(CTemplateManager *pMan, CSelectTransDlg *pDlg, void *param, PROCESSPROC poc);
	bool            ImportTaskData_M(CTemplateManager *pMan, CSelectTransDlg *pDlg, void *param, PROCESSPROC poc);
	bool            ImportItemDescData_M(CTemplateManager *pMan, CSelectTransDlg *pDlg, void  *param, PROCESSPROC poc);
	bool            ImportItemExDescData_M(CTemplateManager *pMan, CSelectTransDlg *pDlg, void  *param, PROCESSPROC poc);
	bool            ImportItemSkillDescData_M(CTemplateManager *pMan, CSelectTransDlg *pDlg,void  *param, PROCESSPROC poc);
	bool            ImportItemFixMsgData_M(CTemplateManager *pMan, CSelectTransDlg *pDlg, void  *param, PROCESSPROC poc);
	bool            ImportItemServerErrData_M(CTemplateManager *pMan, CSelectTransDlg *pDlg, void  *param, PROCESSPROC poc);
	bool            ImportItemBuffDescData_M(CTemplateManager *pMan, CSelectTransDlg *pDlg, void *param,PROCESSPROC poc);
	bool            ImportItemTaskErrData_M(CTemplateManager *pMan, CSelectTransDlg *pDlg, void *param,PROCESSPROC poc);
	bool			ImportItemLoadingHintData_M(CTemplateManager* pMan, CSelectTransDlg *pDlg, void  *param, PROCESSPROC poc);
	bool			ImportItemPetTalkData_M(CTemplateManager* pMan, CSelectTransDlg *pDlg, void  *param, PROCESSPROC poc);
	bool			ImportItemAddonStrData_M(CTemplateManager* pMan, CSelectTransDlg *pDlg, void  *param, PROCESSPROC poc);

	bool            ImportInterfaceHintData_M(CTemplateManager *pMan, CSelectTransDlg *pDlg, void  *param, PROCESSPROC poc);
	bool            ImportInterfaceStringData_M(CTemplateManager *pMan, CSelectTransDlg *pDlg, void  *param, PROCESSPROC poc);
	bool            ImportPolicyData_M(CTemplateManager* pMan, CSelectTransDlg *pDlg, void  *param, PROCESSPROC poc);
	bool            ImportPrecinctData_M(CTemplateManager* pMan, CSelectTransDlg *pDlg, void  *param, PROCESSPROC poc);
	bool            ImportHelpScriptData_M(CTemplateManager* pMan, CSelectTransDlg *pDlg, void  *param, PROCESSPROC poc);
	bool            ImportHelpTextData_M(CTemplateManager* pMan, CSelectTransDlg *pDlg, void  *param, PROCESSPROC poc);
	//bool          ImportTreasureData_M(CTemplateManager* pMan, CSelectTransDlg *pDlg, void  *param, PROCESSPROC poc);
	bool            ImportDomainData_M(CTemplateManager* pManDomain, CTemplateManager* pManDomain2, 
							CTemplateManager* pManGShop,CTemplateManager* pManGShopTypes, 
							CTemplateManager* pManGShop2,CTemplateManager* pManGShopTypes2, 
							CTemplateManager* pManGShop3,CTemplateManager* pManGShopTypes3, 
							CSelectTransDlg *pDlg, void  *param, PROCESSPROC poc);
	bool            ImportTextFileData_M(CTemplateManager* pMan, CSelectTransDlg *pDlg, void  *param, PROCESSPROC poc);
	bool			ImportInstanceTextData_M(CTemplateManager* pMan, CSelectTransDlg *pDlg, void  *param, PROCESSPROC poc);
	bool			ImportHomeTownData_M(CTemplateManager* pMan, CSelectTransDlg *pDlg, void  *param, PROCESSPROC poc);
	bool            EnumTemplateImport(CTemplateBase *pOldTempl, CTemplateBase *pNewTempl, CSelectTransDlg *pDlg);
		
	//译文输出
	bool			ExportElementData(char *szDistPath, void *param, PROCESSPROC poc);
	bool            ExportTaskData(char *szDistPath, void *param, PROCESSPROC poc);
	bool            ExportItemDescData(char *szDistPath, void  *param, PROCESSPROC poc);
	bool			ExportInstanceText(char *szDistPath, void  *param, PROCESSPROC poc);
	bool            ExportItemExDescData(char *szDistPath, void  *param, PROCESSPROC poc);
	bool            ExportItemSkillDescData(char *szDistPath, void  *param, PROCESSPROC poc);
	bool            ExportItemFixMsgData(char *szDistPath, void  *param, PROCESSPROC poc);
	bool            ExportItemServerErrData(char *szDistPath, void  *param, PROCESSPROC poc);
	bool            ExportItemBuffDescData(char *szDistPath, void  *param, PROCESSPROC poc);
	bool            ExportItemTaskErrData(char *szDistPath, void *param, PROCESSPROC poc);
	bool			ExportItemLoadingHintData(char *szDistPath, void  *param, PROCESSPROC poc);
	bool			ExportItemPetTalkData(char *szDistPath, void  *param, PROCESSPROC poc);
	bool			ExportItemAddonStrData(char *szDistPath, void  *param, PROCESSPROC poc);
	bool            ExportInterfaceData(char *szDistPath, void  *param, PROCESSPROC poc);
	bool            ExportInterfaceString(char *szDistPath, void  *param, PROCESSPROC poc, CElementInterfaceManager* inter);
	bool            ExportInterfaceHint(char *szDistPath, void  *param, PROCESSPROC poc, CElementInterfaceManager* inter);
	bool            ExportHelpScript(char *szDistPath, void  *param, PROCESSPROC poc, CElementInterfaceManager* inter);
	bool            ExportHelpText(char *szDistPath, void  *param, PROCESSPROC poc, CElementInterfaceManager* inter);

	bool            ExportPolicyData(char *szDistPath, void *param, PROCESSPROC poc);
	bool            ExportPrecinctData(char *szDistPath, void *param, PROCESSPROC poc);
	//bool          ExportTreasureData(char *szDistPath, void *param, PROCESSPROC poc);
	bool            ExportDomainData(char *szDistPath, void *param, PROCESSPROC poc);
	bool			_ExportDomainInfor(char *szDistPath, void *param, PROCESSPROC poc);
	bool			_ExportDomainInfor1(char *szDistPath, void *param, PROCESSPROC poc);
	bool			_ExportGshop(char *szDistPath, void *param, PROCESSPROC poc);
	bool			_ExportGshop1(char *szDistPath, void *param, PROCESSPROC poc);
	bool			_ExportGshop2(char *szDistPath, void *param, PROCESSPROC poc);
	bool			_ExportGshopTypes(char *szDistPath, void *param, PROCESSPROC poc);
	bool			_ExportGshopTypes1(char *szDistPath, void *param, PROCESSPROC poc);
	bool			_ExportGshopTypes2(char *szDistPath, void *param, PROCESSPROC poc);

	bool            ExportTextFileData(char *szDistPath, void *param, PROCESSPROC poc);
	bool			ExportHomeTownData(char *szDistPath, void *param, PROCESSPROC poc);
	bool            EnumExpPrecinct(CString path,CString expPath);
	
	void            EnumStatTemplate(CTemplateBase *pTemplate, int & nDisuse, int & nNew, int & nTrans);

	void            EnumModifiedFileAttr(CString strName);

	
	typedef void EnumFunc(CTemplateBase*);
	bool			EnumOp(EnumFunc* pFunc);
	void			OpOnData(EnumFunc* pFunc, CTemplateManager* pData, int type);
	void			EnumImportText(CTemplateBase *pTemplate, XmlLocItem* pItem, bool &bReplaced);
	
};
void	EnumTansBig5(CTemplateBase *pTemplate);
void	EnumCopyOrgToTrans(CTemplateBase *pTemplate);
void	EnumClearTrans(CTemplateBase *pTemplate);

struct THD_EXPORTTEXT_INFO
{
	CDataManager*	pMan;
	const char*		szPath;
	bool			bOnlyExportUntrans;
	bool			bGenID;
	THD_INFO*		pThdInfo;
	CElementDataView* pView;
};

struct THD_IMPORTTEXT_INFO
{
	CDataManager*	pMan;
	const char*		szPath;
	bool			bAccordID;
	THD_INFO*		pThdInfo;
};

struct THD_IMPORTORDERTEXT_INFO
{
	CDataManager*	pMan;
	const char*		szPath;
	THD_INFO*		pThdInfo;
	CElementDataView* pView;
};
extern CDataManager g_DataManager;

#endif // !defined(AFX_DATAMANAGER_H__BC5CF222_DEDD_411F_BF52_486160B69440__INCLUDED_)
