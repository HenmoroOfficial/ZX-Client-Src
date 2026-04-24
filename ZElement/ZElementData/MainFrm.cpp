// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Global.h"
#include "ElementData.h"
#include "MainFrm.h"
#include "OptionVssDlg.h"
#include "VssOperation.h"
#include "EnumTypes.h"
#include "BaseDataTemplate.h"
#include "ExtendDataTempl.h"
#include "ExpTypes.h"
#include "BaseDataExp.h"
#include "TalkModifyDlg.h"
#include "BaseDataIDMan.h"
#include "WipeOffDlg.h"
#include "PasswdDlg.h"
#include "PolicyDlg.h"
#include "MounsterFallListDlg.h"
#include "TreasureDlg.h"
#include "QuestionsDlg.h"
#include "EC_MD5Hash.h"
#include "FindDlg.h"
#include "GeneralDataManDlg.h"
#include "GeneralDataManUtil.h"

#define new A_DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_OPTION_VSS, OnOptionVss)
	ON_COMMAND(ID_VSS_FRESH, OnVssFresh)
	ON_COMMAND(ID_EXPORT_DATA, OnExportData)
	ON_COMMAND(ID_TEST_TALK, OnTestTalk)
	ON_COMMAND(ID_CHECK, OnCheck)
	ON_COMMAND(ID_POLICY_EDITOR, OnPolicyEditor)
	ON_COMMAND(ID_EXPORT_POLICY_DATA, OnExportPolicyData)
	ON_COMMAND(ID_TRANS_VERSION, OnTransVersion)
	ON_COMMAND(ID_GET_POLICY_DATA, OnGetPolicyData)
	ON_COMMAND(ID_MOUNSTER_FALL_ITEM_EDITOR, OnMounsterFallItemEditor)
	ON_COMMAND(ID_CLOSE_ALL_DOC, OnCloseAllDoc)
	ON_COMMAND(ID_ITEMS_EDITOR, OnItemsEditor)
	ON_COMMAND(ID_QUESTIONS_EDITOR, OnQuestionsEditor)
	ON_COMMAND(ID_ITEMS2_EDITOR, OnItems2Editor)
	ON_COMMAND(ID_ITEMS3_EDITOR, OnItems3Editor)
	ON_COMMAND(ID_FIND_BYID, OnFindByID)
	ON_COMMAND(ID_FIND_NEXT, OnFindNext)
	ON_COMMAND(ID_FIND_BYFILE, OnFindByFile)
	ON_COMMAND(ID_VERSION_CHANGE, OnVersionChange)
	ON_UPDATE_COMMAND_UI(ID_VERSION_CHANGE, OnUpdateVersionChange)
	ON_COMMAND(ID_AWARD_ITEMS_SEL, OnAwardItemsSel)
	ON_COMMAND(ID_MERGE_VERSION, OnMergeVersion)
	ON_COMMAND(ID_MERGE_VERSION_GEN_FILES, OnMergeVersionGenFiles)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	
	m_wndListBar.Create(this,IDD_DIALOG_LIST_BAR,WS_CHILD | WS_VISIBLE | 
			CBRS_LEFT| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY |
			CBRS_SIZE_DYNAMIC,IDD_DIALOG_LIST_BAR);
	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	//cs.style |= WS_MAXIMIZE;
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers




void CMainFrame::OnOptionVss() 
{
	// TODO: Add your command handler code here
	COptionVssDlg dlg;
	if(IDOK==dlg.DoModal())
	{
		strcpy(g_szUserName,dlg.m_strName);
		strcpy(g_szPassword,dlg.m_strPassword);
		strcpy(g_szServerPath,dlg.m_strServerPath);
		((CElementDataApp*)AfxGetApp())->InitVss();
	}
}

extern BOOL RefreshBaseDataSettings();
extern CEnumTypes g_EnumTypes;
void CMainFrame::OnVssFresh()
{
	if(AfxMessageBox("你确定要从服务器上取得最新数据吗？",MB_YESNO|MB_ICONQUESTION)!=IDYES)
		return;
	FILE* p = fopen("BaseData\\RefreshLog.txt", "at");
	CString str;
	struct tm* ptm;
	time_t t;
	time(&t);
	ptm = localtime(&t);
	str.Format("刷新模板开始时间：%d-%d-%d  %02d:%02d:%02d\r\n",ptm->tm_year + 1900, ptm->tm_mon +1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
	fwrite(str, str.GetLength(), 1, p);
	fclose(p);

	// TODO: Add your command handler code here
	((CElementDataApp*)AfxGetApp())->CloseAllDoc();
	m_wndListBar.m_ListDlg.UpdateRecord();
	
	CString msg;
	if(g_ulVersion>g_ulVersionCurrent)
	{
		AString strPath;
		strPath.Format("%xBaseData", g_ulVersion);
		g_VSS.SetProjectPath(strPath.GetBuffer(0));
		g_VSS.GetAllFiles(strPath.GetBuffer(0));
		msg = g_VSS.GetLastCommandResult();
		if(msg.GetLength() > 1024) msg = msg.Right(1023);
		g_Log.Log("+++++++++++++++++++++++++++++++++%s\r\n",msg);
	}

	g_VSS.SetProjectPath("BaseData");
	g_VSS.GetAllFiles("BaseData");
	msg = g_VSS.GetLastCommandResult();
	if(msg.GetLength() > 1024) msg = msg.Right(1023);
	g_Log.Log("+++++++++++++++++++++++++++++++++%s\r\n",msg);
	
	g_VSS.SetProjectPath("TalkData");
	g_VSS.GetAllFiles("TalkData");
	msg = g_VSS.GetLastCommandResult();
	if(msg.GetLength() > 1024) msg = msg.Right(1023);
	g_Log.Log("+++++++++++++++++++++++++++++++++%s\r\n",msg);

	g_EnumTypes.Release();
	g_EnumTypes.ReadTypes();
	RefreshBaseDataSettings();

	CString strWildcard;
	strWildcard = g_szWorkDir;
	strWildcard += "BaseData";
	m_wndListBar.m_ListDlg.DeleteAllItems();
	m_wndListBar.m_ListDlg.FreshObjectList(strWildcard,NULL,2);

	//记录刷新开始时的格式化时间Added 2011-03-22.
	CString strBegin = str;
	//Added end.
	p = fopen("BaseData\\RefreshLog.txt", "at");
	time(&t);
	ptm = localtime(&t);
	str.Format("刷新模板结束时间：%d-%d-%d  %02d:%02d:%02d\r\n",ptm->tm_year + 1900, ptm->tm_mon +1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
	fwrite(str, str.GetLength(), 1, p);
	fclose(p);

	//通过对话框显示刷新模板的开始和结束时间。Added 2011-03-22.
	CString strResult = "刷新模板完成!\n" + strBegin + str;
	AfxMessageBox( strResult );
	//Added end.
}

void CMainFrame::OnExportData()
{
	//	Let user select a place to store exporting data
	BROWSEINFO Info;
	memset(&Info, 0, sizeof (Info));

	static char szPath[MAX_PATH] = "\0";
	if (!szPath[0])
		strcpy(szPath, g_szWorkDir);

	Info.hwndOwner		= m_hWnd;
	Info.pidlRoot		= NULL;
	Info.pszDisplayName	= szPath;
	Info.lpszTitle		= "请选择导出数据的存放路径：";
	Info.ulFlags		= BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT | BIF_DONTGOBELOWDOMAIN ;
	
	LPITEMIDLIST pPIDL;
	if (!(pPIDL = SHBrowseForFolder(&Info)))
		return;

	SHGetPathFromIDList(pPIDL, szPath);

	CString temp = szPath;
	g_MD5Hash.init();
	g_MD5Hash.Update("ZPWDATA", 7);

	//记录开始导出数据的时间Added 2011-03-22.
	time_t timeBegin, timeEnd;
	time(&timeBegin);
	//Added end.

	if(0==g_PathIDMan.Export(temp + "\\path.data"))
		MessageBox("输出路径数据失败!");
	if(!ExportAllBaseData(temp + "\\elements.data")) 
		MessageBox("输出数据失败!");
	
	//计算导出数据的用时，并以消息框的形式显示Added 2011-03-22.
	time(&timeEnd);
	time_t deltaTime = timeEnd - timeBegin;
	CString strResult;
	strResult.Format( "数据导出完成!\n导出用时：%d 秒\n", deltaTime );
	AfxMessageBox(strResult);
	//Added end.
}

LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(message==WM_CLOSE)
	{
		((CElementDataApp*)AfxGetApp())->UpdateAllDocData();
	}
	return CMDIFrameWnd::WindowProc(message, wParam, lParam);
}

void CMainFrame::OnTestTalk() 
{
	// TODO: Add your command handler code here
	CTalkModifyDlg dlg;
	dlg.DoModal();
}

void CMainFrame::OnCheck()
{
	CPasswdDlg dlgPasswd;
	if (dlgPasswd.DoModal() != IDOK) return;

	if (dlgPasswd.m_strPasswd != "debug")
	{
		AfxMessageBox("密码错误！");
		return;
	}

	if (AfxMessageBox("此操作前请确认更新本地数据，继续吗？", MB_YESNO) != IDYES)
		return;

	abase::vector<AString> PathArray, WOArrayBase, WOArrayFace, WOArrayRecipe, WOArrayConfig, WOArrayExt;
	CStringArray WipeOffArray;
	CUIntArray IDArray;

	size_t i;
	g_BaseIDMan.GeneratePathArray(PathArray);
	for (i = 0; i < PathArray.size(); i++)
		if (GetFileAttributes(PathArray[i]) == -1)
		{
			WipeOffArray.Add(PathArray[i]);
			IDArray.Add(g_BaseIDMan.GetIDByPath(PathArray[i]));
			WOArrayBase.push_back(PathArray[i]);
		}

	PathArray.clear();
	g_RecipeIDMan.GeneratePathArray(PathArray);
	for (i = 0; i < PathArray.size(); i++)
		if (GetFileAttributes(PathArray[i]) == -1)
		{
			WipeOffArray.Add(PathArray[i]);
			IDArray.Add(g_RecipeIDMan.GetIDByPath(PathArray[i]));
			WOArrayRecipe.push_back(PathArray[i]);
		}
		
	PathArray.clear();
	g_ConfigIDMan.GeneratePathArray(PathArray);
	for (i = 0; i < PathArray.size(); i++)
		if (GetFileAttributes(PathArray[i]) == -1)
		{
			WipeOffArray.Add(PathArray[i]);
			IDArray.Add(g_ConfigIDMan.GetIDByPath(PathArray[i]));
			WOArrayConfig.push_back(PathArray[i]);
		}
		
	PathArray.clear();
	g_ExtBaseIDMan.GeneratePathArray(PathArray);
	for (i = 0; i < PathArray.size(); i++)
		if (GetFileAttributes(PathArray[i]) == -1)
		{
			WipeOffArray.Add(PathArray[i]);
			IDArray.Add(g_ExtBaseIDMan.GetIDByPath(PathArray[i]));
			WOArrayExt.push_back(PathArray[i]);
		}

	if (WipeOffArray.GetSize() == 0)
	{
		AfxMessageBox("无错误模板！");
		return;
	}

	CWipeOffDlg dlg(&WipeOffArray, &IDArray);
	if (dlg.DoModal() == IDOK)
	{
		g_BaseIDMan.WipeOffByPath(WOArrayBase);
		g_RecipeIDMan.WipeOffByPath(WOArrayRecipe);
		g_ConfigIDMan.WipeOffByPath(WOArrayConfig);
		g_ExtBaseIDMan.WipeOffByPath(WOArrayExt);
	}
}

void CMainFrame::OnPolicyEditor() 
{
	// TODO: Add your command handler code here
	CPolicyDlg dlg;
	dlg.DoModal();
}

void CMainFrame::OnExportPolicyData() 
{
	// TODO: Add your command handler code here
	CFileDialog dlg(
		FALSE,
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR);

	if (dlg.DoModal() == IDOK
	&& !ExportPolicyData(dlg.GetPathName()))
		AfxMessageBox("输出失败！", MB_ICONSTOP);
}

void CMainFrame::OnTransVersion() 
{
	/*
	// TODO: Add your command handler code here
	if(IDYES==AfxMessageBox("该操作前需要手动取出所有模板文件!你确定要进行添加MD5校验码的操作吗？",MB_YESNO|MB_ICONQUESTION))
		TempTransVersion();
	*/
}

void CMainFrame::OnGetPolicyData() 
{
	// TODO: Add your command handler code here
	if(AfxMessageBox("你确定要从服务器上取得最新策略数据吗？",MB_YESNO|MB_ICONQUESTION)!=IDYES)
		return;
	// TODO: Add your command handler code here
	CreateDirectory("PolicyData", NULL);
	BeginWaitCursor();
	g_VSS.SetProjectPath("PolicyData");
	g_VSS.GetAllFiles("PolicyData");
	EndWaitCursor();
}

void CMainFrame::OnMounsterFallItemEditor() 
{
	// TODO: Add your command handler code here
	CMounsterFallListDlg dlg;
	dlg.DoModal();
}

void CMainFrame::OnCloseAllDoc() 
{
	// TODO: Add your command handler code here
	((CElementDataApp*)AfxGetApp())->CloseAllDoc();
}

void CMainFrame::OnItemsEditor() 
{
	// TODO: Add your command handler code here
	CTreasureDlg dlg;
	dlg.Init("treasure.dat");
	dlg.DoModal();
}

void CMainFrame::OnQuestionsEditor() 
{
	// TODO: Add your command handler code here
	CQuestionsDlg dlg;
	dlg.DoModal();
}

void CMainFrame::OnItems2Editor() 
{
	// TODO: Add your command handler code here
	CTreasureDlg dlg;
	dlg.Init("treasure1.dat");
	dlg.DoModal();
}

void CMainFrame::OnItems3Editor() 
{
	// TODO: Add your command handler code here
	CTreasureDlg dlg;
	dlg.Init("treasure2.dat");
	dlg.DoModal();
}


CString _templ_to_find;
void CMainFrame::OnFindNext()
{
	if (_templ_to_find.IsEmpty())
		return;	

	m_wndListBar.m_ListDlg.OnFindNext(_templ_to_find);
}

void CMainFrame::OnFindByID()
{
	FindDlg dlg;
	if (dlg.DoModal() != IDOK) return;	
	
	m_wndListBar.m_ListDlg.FindTemplByID(TVI_ROOT, atoi(dlg.m_strText));
	m_wndListBar.m_ListDlg.SetFocus();
}

void CMainFrame::OnFindByFile()
{
	FindDlg dlg;
	if (dlg.DoModal()!= IDOK) return;
	_templ_to_find = dlg.m_strText;

	m_wndListBar.m_ListDlg.FindTempl(TVI_ROOT, dlg.m_strText);
	m_wndListBar.m_ListDlg.SetFocus();
}


//Added 2011-04-20
void CMainFrame::OnVersionChange() 
{
	//首先，更新海外ID预留相关的控制变量
	g_bOverseasEnable = !g_bOverseasEnable;

	//更新主界面的title的显示
	CString titile;
	const char szOverseasVersion[] = "海外版本";
	const char szNationalVersion[] = "国内版本";
	if ( g_bOverseasEnable )
	{
		titile.Format("ElementData-[%s--%s]",g_szProject, szOverseasVersion);
	}
	else
	{
		titile.Format("ElementData-[%s--%s]",g_szProject, szNationalVersion);
	}

	SetWindowText(titile);
}
//Added end.

void CMainFrame::OnUpdateVersionChange(CCmdUI* pCmdUI) 
{
	//此函数暂时使用
	pCmdUI->Enable(FALSE);//不再需要在编辑器运行过程中切换海外版本和国内版本，屏蔽 Added 2011-05-19
}

void CMainFrame::OnAwardItemsSel() 
{
	//
	CGeneralDataManDlg dlg;
	dlg.Init( "GeneralData.dat", "GeneralData\\GeneralDataID.dat", "GeneralData\\GeneralData", szVIPAwardMainType, nVIPAwardMainTypeCount, GENERAL_DATA_TEMPL_TYPE_VIPAWARD );
	dlg.DoModal();
}

void CMainFrame::EnumMergeFile(CString strPathName, bool bConflitTest, AFile *pF)
{
	CFileFind finder;
	CString strWildcard = strPathName + "\\*.*";
	
	BOOL bWorking = finder.FindFile(strWildcard);
	while (bWorking)
	{		
		bWorking = finder.FindNextFile();
		
		//是"."或是".." 跳过.
		if (finder.IsDots())
			continue;
		
		// 是文件夹，继续搜寻.
		if (finder.IsDirectory())
		{
			EnumMergeFile(finder.GetFilePath(), bConflitTest, pF);
			continue;

		}

		// 文件
		CString strFileName = finder.GetFilePath();

		if (strFileName.Find(".tmpl")==-1)
			continue;
		
		strFileName.Delete(0, strlen(g_szWorkDir));

		CString strPrefix;
		strPrefix.Format("%x", g_ulVersion);
		CString strFileName1 = strFileName;
		int index = strFileName1.Find(strPrefix);
		if (index>=0)
		{
			strFileName1.Delete(index, strPrefix.GetLength());
		}

		if (bConflitTest)
		{
			CString strUser1, strUser2;
			int t1, t2;
			g_VSS.GetFileLastWriteInfo(strFileName.GetBuffer(0), strUser1, t1);
			g_VSS.GetFileLastWriteInfo(strFileName1.GetBuffer(0), strUser2, t2);
			
			// 判断两个文件最后修改时间与上个版本分界点的时间先后
			AString strLine;
			if (t2>g_ulVersionTime)
			{
				strLine.Format("warning: %s: 新版本:%s(%s) 老版本:%s(%s)", strFileName1, strUser1, g_VSS.GetTimeString(t1), strUser2, g_VSS.GetTimeString(t2));
				m_bMergeOK = false;
				pF->WriteLine(strLine);
			}
			else
			{
				strLine.Format("File Version OK%s.", strFileName);
			}
		}
		else
		{
			int index = strFileName1.ReverseFind('\\');
			if (index<=0)
				continue;

			g_VSS.SetProjectPath(strFileName1.Left(index+1));
			g_VSS.CheckOutFile(strFileName1);
			
			if(FileIsReadOnly(strFileName1.GetBuffer(0)))
			{
				CString strMsg;				
				CString user = g_VSS.GetUserCheckout(strFileName1);
				strMsg.Format("取出文件%s失败，文件被%s checkout了", strFileName1, user);
				MessageBox(strMsg);
				return;
			}
			
			AFile f;
			if(f.Open(strFileName1, AFILE_CREATENEW | AFILE_TEXT))
			{
				AFile fileNew;
				if (fileNew.Open(strFileName, AFILE_OPENEXIST | AFILE_TEXT))
				{
					char szBuffer[4098];
					DWORD readLen, writeLen;
					while(fileNew.Read(szBuffer, 4096, &readLen))
					{
						if(readLen==0)
							break;
						f.Write(szBuffer, readLen, &writeLen);
					}
					fileNew.Close();
				}
				f.Close();
			}
			
			g_VSS.SetProjectPath(strFileName1.Left(index+1));
			g_VSS.CheckInFile(strFileName1.Right(strFileName1.GetLength()-index-1));
		}
	}
}

void CMainFrame::OnMergeVersionGenFiles() 
{
	AFile f;
	if(!f.Open("file_to_merge.txt", AFILE_CREATENEW | AFILE_TEXT))
		return;
	f.WriteLine("下面文件采用新老编辑器修改过，请相关人员确定最新版本并用新版本编辑器进行最终修改，合并完成后老版本将丢失");
	f.WriteLine("修改完成后通知负责人，负责人通知程序");
	m_bMergeOK = true;
	CString strPath;
	strPath.Format("%s%xBaseData", g_szWorkDir, g_ulVersion);
	EnumMergeFile(strPath, true, &f);

	f.Close();
	
	if (m_bMergeOK)
		AfxMessageBox( "合并完成，没有冲突" );
	else
		AfxMessageBox( "文件生成完毕，请处理file_to_merge.txt中冲突" );	
}

void CMainFrame::OnMergeVersion() 
{	
	CPasswdDlg dlgPasswd;
	if (dlgPasswd.DoModal() != IDOK) return;
	
	if (dlgPasswd.m_strPasswd != "debug")
	{
		AfxMessageBox("密码错误！");
		return;
	}
	
	m_bMergeOK = true;
	
	CString strPath;
	strPath.Format("%s%xBaseData", g_szWorkDir, g_ulVersion);
	EnumMergeFile(strPath);
	
	AfxMessageBox( "合并完成" );
}
