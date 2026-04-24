// QuestionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "elementdata.h"
#include "QuestionsDlg.h"
#include "QuestionItem.h"
#include "BaseDataIDMan.h"
#include "PolicyCreateDlg.h"
#include "QuestionMan.h"
#include "Global.h"
#include "VssOperation.h"
#include "ExcelOperation.h"
#include "vector_string.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

char cFolder[26] = 
{
	'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'
};

/////////////////////////////////////////////////////////////////////////////
// CQuestionsDlg dialog


CQuestionsDlg::CQuestionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQuestionsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CQuestionsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CQuestionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQuestionsDlg)
	DDX_Control(pDX, IDC_TREE_QUESTIONS, m_Tree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CQuestionsDlg, CDialog)
	//{{AFX_MSG_MAP(CQuestionsDlg)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_QUESTIONS, OnRclickTreeQuestions)
	ON_COMMAND(ID_CREATE_QUESTION, OnCreateQuestion)
	ON_COMMAND(ID_QUESTION_CHECK_IN, OnQuestionCheckIn)
	ON_COMMAND(ID_QUESTION_CHECK_OUT, OnQuestionCheckOut)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_QUESTIONS, OnDblclkTreeQuestions)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, OnButtonExport)
	ON_BN_CLICKED(IDC_BUTTON_FRESH, OnButtonFresh)
	ON_COMMAND(ID_CREATE_FOLDER, OnCreateFolder)
	ON_NOTIFY(TVN_ITEMEXPANDED, IDC_TREE_QUESTIONS, OnItemexpandedTreeQuestions)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, OnButtonImport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQuestionsDlg message handlers

BOOL CQuestionsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_ImageList.Create(IDB_LISTTAG, 16, 3, RGB(255, 0, 255));
    m_Tree.SetImageList(&m_ImageList,TVSIL_NORMAL);
	FreshQuestions();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

bool CQuestionsDlg::LoadIDTemplate()
{
	g_QuestionsIDMan.Release();
	if (g_QuestionsIDMan.Load("Questions\\QuestionsID.dat") != 0)
	{
		AfxMessageBox("打开Questions\\QuestionsID.dat文件失败", MB_ICONSTOP);
		return false;
	}
	return true;
}

bool CQuestionsDlg::LoadFolderTemplate()
{
	if(!g_QuestionsFolder.Load())
	{
		AfxMessageBox("打开Questions\\Questions_folder.dat文件失败", MB_ICONSTOP);
		return false;
	}
	return true;
}

void CQuestionsDlg::OnRclickTreeQuestions(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CPoint point;
	GetCursorPos(&point);
	POINT pt = {point.x, point.y};
	m_Tree.ScreenToClient(&pt);
	CPoint client(pt.x,pt.y);
	pt.x = point.x;
	pt.y = point.y;
	
	CMenu Menu;
	Menu.LoadMenu(IDR_MENU_QUESTION);
	
	CMenu* pPopupMenu = Menu.GetSubMenu(0);
	ASSERT(pPopupMenu);
	UpdateMenu(pPopupMenu);
	pPopupMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, this);
	*pResult = 0;
}

void CQuestionsDlg::OnCreateQuestion() 
{
	HTREEITEM hSel = m_Tree.GetSelectedItem();
	if(hSel==NULL) return;
	if(m_Tree.GetParentItem(hSel)!=NULL) return;
	int nFolder = m_Tree.GetItemData(hSel);
	if(nFolder == 0) return;

	CPolicyCreateDlg dlg;
	CQuestionData mQuestionData;

	//添加与海外ID预留相关的控制代码 Added 2011-04-20.
	dlg.m_bOverseasEnable = g_bOverseasEnable;
	//Added end.

	if(IDOK == dlg.DoModal())
	{
		CString pathName;
		int k = 0;
		pathName.Format("%s.qst",dlg.m_szPolicyName);
			
		//添加与海外ID预留相关的控制代码 Added 2011-04-20.
		int nQuestionID = 0;
		if ( g_bOverseasEnable )
		{
			nQuestionID = g_QuestionsIDMan.CreateIDExt( dlg.m_ulOverseasPolicyID, AString(pathName) ); //海外ID预留，直接输入ID
			//再判断下，好定位创建模板时的错误信息
			if ( !nQuestionID )
			{
				AfxMessageBox( "创建失败，输入模板名字错误或者输入模板的ID错误!" );
				return;
			}
		}
		else
		{
			nQuestionID = g_QuestionsIDMan.CreateID(AString(pathName));
		}
		//Added end.

		if(nQuestionID==0) 
		{
			MessageBox("创建失败，可能已经有相同的问题名字存在!");
			return;
		}
		
		int nf = nQuestionID/1000;
		if(nf > 25) nf = 25;

		mQuestionData.SetID(nQuestionID);
		mQuestionData.nparent = nFolder;
		pathName.Format("Questions\\%c",cFolder[nf]);
		CreateDirectory(pathName, NULL);
		pathName.Format("Questions\\%c\\%s.qst",cFolder[nf],dlg.m_szPolicyName);
		if(!mQuestionData.Save(pathName))
		{
			MessageBox("CQuestionsDlg::OnCreateQuestion(),保存文件失败!");
			return;
		}
		g_VSS.SetProjectPath("Questions");
		CString strNewFolder;
		strNewFolder.Format("%c",cFolder[nf]);
		g_VSS.CreateProject(strNewFolder);
		strNewFolder.Format("Questions\\%c",cFolder[nf]);
		g_VSS.SetProjectPath(strNewFolder);
		g_VSS.AddFile(pathName);
		g_VSS.CheckOutFile(pathName);
		pathName.Format("%s.qst",dlg.m_szPolicyName);
		HTREEITEM hNewItem;
		hNewItem = m_Tree.InsertItem(pathName,hSel);
		pathName.Format("%sQuestions\\%c\\%s.qst",g_szWorkDir,cFolder[nf],dlg.m_szPolicyName);
		if(FileIsReadOnly(AString(pathName)))
		{
			m_Tree.SetItemImage(hNewItem,4,4);
		}else
		{
			m_Tree.SetItemImage(hNewItem,5,5);
		}
	}
}

void CQuestionsDlg::OnQuestionCheckIn() 
{
	// TODO: Add your command handler code here
	HTREEITEM itemp = m_Tree.GetSelectedItem();
	CString fileName = m_Tree.GetItemText(itemp);
	if(-1==fileName.Find(".qst")) return;
	
	int nf =  g_QuestionsIDMan.GetIDByPath(AString(fileName));
	if(nf==0) return;
	nf = nf/1000;
	if(nf>25) nf = 25;
	CString pathName;
	pathName.Format("Questions\\%c\\%s",cFolder[nf],fileName);
	CString curProj;
	curProj.Format("Questions\\%c",cFolder[nf]);
	g_VSS.SetProjectPath(curProj);
	g_VSS.CheckInFile(fileName);
	
	if(FileIsReadOnly(AString(pathName)))
	{
		m_Tree.SetItemImage(itemp,4,4);
	}else
	{
		m_Tree.SetItemImage(itemp,5,5);
	}	
}

void CQuestionsDlg::OnQuestionCheckOut() 
{
	// TODO: Add your command handler code here
	HTREEITEM itemp = m_Tree.GetSelectedItem();
	CString fileName = m_Tree.GetItemText(itemp);
	if(-1==fileName.Find(".qst")) return;
	
	int nf =  g_QuestionsIDMan.GetIDByPath(AString(fileName));
	if(nf==0) return;
	nf = nf/1000;
	if(nf>25) nf = 25;
	CString pathName;
	pathName.Format("Questions\\%c\\%s",cFolder[nf],fileName);
	CString curProj;
	curProj.Format("Questions\\%c",cFolder[nf]);
	g_VSS.SetProjectPath(curProj);
	g_VSS.CheckOutFile(pathName);
	
	if(FileIsReadOnly(AString(pathName)))
	{
		m_Tree.SetItemImage(itemp,4,4);
	}else
	{
		m_Tree.SetItemImage(itemp,5,5);
	}	
}

void CQuestionsDlg::FreshQuestions()
{
	LoadIDTemplate();
	LoadFolderTemplate();

	m_Tree.DeleteAllItems();
	abase::vector<AString> pathArray;
	g_QuestionsIDMan.GeneratePathArray(pathArray);

	for( int j = 0; j < g_QuestionsFolder.GetFolderNum(); ++j)
	{	
		FOLDER_Q *pFQ = g_QuestionsFolder.GetFolder(j);
		HTREEITEM hParent = m_Tree.InsertItem(pFQ->strFolderName,0,0);
		m_Tree.SetItemData(hParent,pFQ->nID);
		for( int i = 0; i < pathArray.size(); ++i)
		{
			CString pathName;
			int nf =  g_QuestionsIDMan.GetIDByPath(AString(pathArray[i]));
			nf = nf/1000;
			if(nf>25) nf = 25;
			CQuestionData data;
			pathName.Format("Questions\\%c\\%s",cFolder[nf],pathArray[i]);
			if(data.Load(pathName))
			{
				if(data.nparent == pFQ->nID)
				{
					HTREEITEM hTreeItem = m_Tree.InsertItem(pathArray[i],hParent);
					if(FileIsReadOnly(AString(pathName)))
					{
						m_Tree.SetItemImage(hTreeItem,4,4);
					}else
					{
						m_Tree.SetItemImage(hTreeItem,5,5);
					}
					break;
				}
			}
		}
		
	}
	
}

void CQuestionsDlg::OnDblclkTreeQuestions(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	HTREEITEM itemp = m_Tree.GetSelectedItem();
	if(itemp==NULL) return;
	CString fileName = m_Tree.GetItemText(itemp);
	if(fileName.IsEmpty()) return;
	
	CQuestionItem dlg;
	CString pathName;
	int nf =  g_QuestionsIDMan.GetIDByPath(AString(fileName));
	if(nf == 0) return;
	nf = nf/1000;
	if(nf>25) nf = 25;
	pathName.Format("Questions\\%c\\%s",cFolder[nf],fileName);
	dlg.Init(pathName);
	dlg.DoModal();
	*pResult = 0;
}

void CQuestionsDlg::OnButtonExport() 
{
	// TODO: Add your control notification handler code here
	BROWSEINFO Info;
	memset(&Info, 0, sizeof (Info));

	static char szPath[MAX_PATH] = "\0";

	Info.hwndOwner		= m_hWnd;
	Info.pidlRoot		= NULL;
	Info.pszDisplayName	= szPath;
	Info.lpszTitle		= "请选择导出数据的存放路径：";
	Info.ulFlags		= BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT | BIF_DONTGOBELOWDOMAIN ;
	
	LPITEMIDLIST pPIDL;
	if (!(pPIDL = SHBrowseForFolder(&Info)))
		return;

	SHGetPathFromIDList(pPIDL, szPath);
	sprintf(szPath,"%s\\Questions.data",szPath);
	FILE *pExpFile = fopen(szPath,"wb");
	if(pExpFile==NULL) return;
	
	int nLength = 0;
	WORD szBuffer[256];
	fwrite(&nLength,sizeof(int),1,pExpFile);//先写纪录长度，后面再重新写。
	
	abase::vector<AString> pathArray;
	g_QuestionsIDMan.GeneratePathArray(pathArray);
	for( int i = 0; i < pathArray.size(); ++i)
	{
		CString pathName;
		CQuestionData qst;
		int nf =  g_QuestionsIDMan.GetIDByPath(AString(pathArray[i]));
		nf = nf/1000;
		if(nf>25) nf = 25;
		pathName.Format("Questions\\%c\\%s",cFolder[nf],pathArray[i]);
		if(qst.Load(pathName))
		{
			if(qst.bDeleted) continue;
			if(qst.strQuestion.GetLength()==0) continue;
			if(!IsValid(&qst)) 
			{
				g_Log.Log("Question error: %s",pathName);
				continue;
			}
			
			//答案：一个字节，从1位到第6位分别对应A，B，C，D，E，F。正确答案是1，错误答案为0
			fwrite(&qst.key,1,1,pExpFile);                 
			
			//问题:
			memset(szBuffer,0,256*sizeof(WORD));
			MultiByteToWideChar(CP_ACP,0,(LPCSTR)qst.strQuestion.GetBuffer(0),qst.strQuestion.GetLength(),(WCHAR*)szBuffer,256);
			int num = wcslen((WCHAR*)szBuffer);
			fwrite(&num, sizeof(int),1,pExpFile);
			if(num > 0) fwrite(szBuffer,num*sizeof(WORD),1,pExpFile);
		
			//被选答案:
			fwrite(&qst.nAnswer,sizeof(int),1,pExpFile);//备选答案数
			for( int j = 0; j < qst.nAnswer; ++j)
			{
				/*
				memset(szBuffer,0,256*sizeof(WORD));
				MultiByteToWideChar(CP_ACP,0,qst.strAnswer[j].GetBuffer(0),qst.strAnswer[j].GetLength(),szBuffer,256);
				num = wcslen(szBuffer);
				fwrite(&num, sizeof(int),1,pExpFile);
				if(num > 0) fwrite(szBuffer,num*sizeof(WORD),1,pExpFile);
				*/
				if(qst.strAnswer[j].GetLength() < 1) 
				{
					fclose(pExpFile);
					MessageBox("输出错误！问题的答案不能为空。",NULL,MB_ICONSTOP);
					return;
				}
				WriteVectorString(pExpFile,qst.strAnswer[j]);
			}

			nLength++;	
		}
	}
	fseek(pExpFile,0,SEEK_SET);
	fwrite(&nLength,sizeof(int),1,pExpFile);//重新纪录写长度
	fclose(pExpFile);
}

bool CQuestionsDlg::WriteVectorString(FILE *pFile, CString str)
{
	CDC *pDC = GetDC();
	DWORD dwSize;
	HANDLE hMem;
	LPTTPOLYGONHEADER lpph;
	LPBYTE lpb;
	long cbOutline, cbTotal;
        
	MAT2 m2;
	GLYPHMETRICS gm;         
	TEXTMETRIC tm;

    CFont font;
	VERIFY(font.CreateFont(
	   16,						  // nHeight
	   0,                         // nWidth
	   0,                         // nEscapement
	   0,                         // nOrientation
	   FW_NORMAL,                 // nWeight
	   FALSE,                     // bItalic
	   FALSE,                     // bUnderline
	   0,                         // cStrikeOut
	   ANSI_CHARSET,              // nCharSet
	   OUT_DEFAULT_PRECIS,        // nOutPrecision
	   CLIP_DEFAULT_PRECIS,       // nClipPrecision
	   DEFAULT_QUALITY,           // nQuality
	   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
	   "宋体")); 
	CFont* pOldFont=pDC->SelectObject(&font);
    pDC->SelectObject(&font);
	pDC->GetTextMetrics(&tm);

	m2.eM11 = FloatToFixed(1.0);
	m2.eM12 = FloatToFixed(0.0);
	m2.eM21 = FloatToFixed(0.0);
	m2.eM22 = FloatToFixed(1.0);

    //replace above code with follow code, there will be a surprise
	/*
	int ro = rand() % 10;
	float fro = 1.0f;
	if(ro > 0) fro = 3.1415926/ro;
	m2.eM11 = FloatToFixed(cos(fro));
	m2.eM12 = FloatToFixed(sin(fro));
	m2.eM21 = FloatToFixed(-sin(fro));
	m2.eM22 = FloatToFixed(cos(fro));
	*/
    
	CVectorString* pNewString = new CVectorString;
	int nChar = 0;  
    for (int i = 0; i < str.GetLength(); i++)
    {
        if (str.GetAt(i) >= 0) //deal singlebyte char or multibyte char
            nChar = str.GetAt(i);
        else
        {
            int th = str.GetAt(i);
            int tl = str.GetAt(i + 1);
            nChar = ((th & 0x00ff)<<8) + (tl & 0x00ff);
            i++;
        }
		
		VECTOR_CHAR *pNewChar = new VECTOR_CHAR;
		dwSize = pDC->GetGlyphOutline(nChar,GGO_NATIVE,&gm,0L,NULL,&m2); //get the size of outline-information
		hMem=GlobalAlloc(GHND,dwSize);
		lpph=(LPTTPOLYGONHEADER)GlobalLock(hMem);
		pDC->GetGlyphOutline(nChar,GGO_NATIVE,&gm,dwSize,lpph,&m2); //get outline-line
		cbTotal = dwSize;
		
		abase::vector<VECTOR_PATH*> listChar;
		while( cbTotal > 0 )
		{
			abase::vector<CPoint> listPath;
			int xOld = mapFXY( lpph->pfxStart.x );
			int yOld = mapFXY( lpph->pfxStart.y );
            listPath.push_back(CPoint(xOld,yOld));
			lpb   = (LPBYTE)lpph + sizeof(TTPOLYGONHEADER);
			cbOutline = (long)lpph->cb - sizeof(TTPOLYGONHEADER);
        
			while( cbOutline > 0 )
			{
				int  n;
				LPTTPOLYCURVE lpc;
				lpc = (LPTTPOLYCURVE)lpb;                                 
                GetOutLine(lpc->wType,lpc->cpfx,lpc->apfx,listPath);	//draw outline			
				n = sizeof(TTPOLYCURVE) + sizeof(POINTFX) * (lpc->cpfx - 1);
				lpb += n;
				cbOutline -= n;
			}                   
			listPath.push_back(CPoint(xOld,yOld));
			cbTotal -= lpph->cb;
			lpph     = (LPTTPOLYGONHEADER)lpb;

			//Spline
			if(listPath.size() > 0)
			{
				VECTOR_PATH *pNewPath = new VECTOR_PATH;
				pNewPath->m_nPtNum = listPath.size();
				pNewPath->m_pPtData = new char[pNewPath->m_nPtNum];
				TranslateToByte(listPath,pNewPath->m_pPtData);
				listChar.push_back(pNewPath);
			}
			listPath.clear();
		}
		pNewChar->m_nPathNum = listChar.size();
		pNewChar->m_pPathData = (VECTOR_PATH**) new DWORD[pNewChar->m_nPathNum];
		for( int j = 0; j < pNewChar->m_nPathNum; ++j)
			pNewChar->m_pPathData[j] = listChar[j];
		pNewString->AddChar(pNewChar);
		
		GlobalUnlock(hMem);
	    GlobalFree(hMem);
	}
    
	if(pNewString)
	{
		pNewString->Save(pFile);
		/*
		int size = pNewString->GetSize();
		FILE *pTest = fopen("d:\\test_vector.dat","wb");
		CVectorString* pTHa = pNewString->Disorder();
		size = pTHa->GetSize();
		pTHa->Save(pTest);
		delete pTHa;
		fclose(pTest);
		*/
		delete pNewString;
	}
	
	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);
	return true;
}

void CQuestionsDlg::TranslateToByte(abase::vector<CPoint>& listPath, char* pBuffer)
{
	for( int i = 0; i < listPath.size(); ++i)
	{
		if(listPath[i].x > 15) 
			listPath[i].x = 15;
		if(listPath[i].y > 15) 
			listPath[i].y = 15;

		if(listPath[i].x < 0) 
			listPath[i].x = 0;
		if(listPath[i].y < 0) 
			listPath[i].y = 0;
		
		char x = listPath[i].x;
		char y = listPath[i].y;
		y = y & 0xf;
		pBuffer[i] = x<<4 | y;
	}
}

void CQuestionsDlg::GetOutLine(int wType,int cpfx, POINTFX apfx[],abase::vector<CPoint>& listPath)
{
	
    CPoint lpPoint[1000];
	CPoint bezi[2];
    int i;
	for (i=0; i <cpfx;i++) 
	{
		lpPoint[i].x = mapFXY( apfx[i].x );
	    lpPoint[i].y = mapFXY( apfx[i].y );
	}
	switch (wType)
	{
	case TT_PRIM_LINE:
	case TT_PRIM_QSPLINE:
	case TT_PRIM_CSPLINE:
		for (i=0 ;i<cpfx;i++) listPath.push_back(lpPoint[i]);
		break;
	default:
		MessageBox("字体不支持");
		break;
	}

}

FIXED CQuestionsDlg::FloatToFixed( double d )
{
	long l;
	l = (long)(d * 65536L);
	return *(FIXED *)&l;
}

int CQuestionsDlg::mapFXY(FIXED fxy)
{
	long   lxy;
	lxy = *(LONG *)&fxy;
	return (int)((double)(lxy)/65536.0);
}

bool CQuestionsDlg::IsValid(CQuestionData *pQs)
{
	if(pQs->key == 0) return false; 
	if(pQs->key & 1) 
		if(pQs->nAnswer < 1) return false;
	if(pQs->key & 2) 
		if(pQs->nAnswer < 2) return false;
	if(pQs->key & 4) 
		if(pQs->nAnswer < 3) return false;
	if(pQs->key & 8) 
		if(pQs->nAnswer < 4) return false;
	if(pQs->key & 16) 
		if(pQs->nAnswer < 5) return false;
	if(pQs->key & 32) 
		if(pQs->nAnswer < 6) return false;

	if(pQs->nAnswer < 2 ) return false;

	for( int i = 0; i < pQs->nAnswer; ++i)
	{
		if(pQs->strAnswer[i].IsEmpty()) return false;
	}
	
	return true;
}

void CQuestionsDlg::OnButtonFresh() 
{
	// TODO: Add your control notification handler code here
	BeginWaitCursor();
	CString path;
	path.Format("%sQuestions",g_szWorkDir);
	CreateDirectory(path, NULL);
	g_VSS.SetProjectPath("Questions");
	g_VSS.GetAllFiles("Questions");
	FreshQuestions();
	EndWaitCursor();
}

void CQuestionsDlg::OnCreateFolder() 
{
	// TODO: Add your command handler code here
	CPolicyCreateDlg dlg;
	if(IDOK == dlg.DoModal())
	{
		int id = g_QuestionsFolder.AddFolder(dlg.m_szPolicyName);
		if(id==0)
		{
			MessageBox("创建文件夹失败！");
			return;
		}
		HTREEITEM hItem = m_Tree.InsertItem(dlg.m_szPolicyName);
		m_Tree.SetItemImage(hItem,1,1);
		m_Tree.SetItemData(hItem,id);
	}
}
void CQuestionsDlg::OnOK()
{
	//CDialog::OnOK();
}
void CQuestionsDlg::OnCancel()
{
	CDialog::OnCancel();
}

void CQuestionsDlg::UpdateMenu(CMenu *pMenu)
{
	HTREEITEM hsel = m_Tree.GetSelectedItem();
	if(hsel==NULL)
	{
		pMenu->EnableMenuItem(ID_CREATE_FOLDER,MF_ENABLED);
		pMenu->EnableMenuItem(ID_CREATE_QUESTION,MF_GRAYED);
		pMenu->EnableMenuItem(ID_QUESTION_CHECK_OUT,MF_GRAYED);
		pMenu->EnableMenuItem(ID_QUESTION_CHECK_IN,MF_GRAYED);
	}else if(m_Tree.GetParentItem(hsel) == NULL)
	{
		pMenu->EnableMenuItem(ID_CREATE_FOLDER,MF_ENABLED);
		pMenu->EnableMenuItem(ID_CREATE_QUESTION,MF_ENABLED);
		pMenu->EnableMenuItem(ID_QUESTION_CHECK_OUT,MF_GRAYED);
		pMenu->EnableMenuItem(ID_QUESTION_CHECK_IN,MF_GRAYED);
	}else
	{
		pMenu->EnableMenuItem(ID_CREATE_FOLDER,MF_ENABLED);
		pMenu->EnableMenuItem(ID_CREATE_QUESTION,MF_GRAYED);
		pMenu->EnableMenuItem(ID_QUESTION_CHECK_OUT,MF_ENABLED);
		pMenu->EnableMenuItem(ID_QUESTION_CHECK_IN,MF_ENABLED);
	}
}

void CQuestionsDlg::OnItemexpandedTreeQuestions(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	HTREEITEM hParent = pNMTreeView->itemNew.hItem;
	int nImage,nSelImage;
	m_Tree.GetItemImage(hParent,nImage,nSelImage);
	// TODO: Add your control notification handler code here
	BeginWaitCursor();
	if(pNMTreeView->action == TVE_EXPAND && nImage==0)
	{
	
		HTREEITEM hChild = m_Tree.GetChildItem(hParent);
		int nParentID = m_Tree.GetItemData(hParent);
		if(hChild) m_Tree.DeleteItem(hChild);
		m_Tree.SetItemImage(hParent,1,1);
		abase::vector<AString> pathArray;
		g_QuestionsIDMan.GeneratePathArray(pathArray);
		for( int i = 0; i < pathArray.size(); ++i)
		{
			CString pathName;
			int nf =  g_QuestionsIDMan.GetIDByPath(AString(pathArray[i]));
			nf = nf/1000;
			if(nf>25) nf = 25;
			CQuestionData data;
			pathName.Format("Questions\\%c\\%s",cFolder[nf],pathArray[i]);
			if(data.Load(pathName))
			{
				if(data.nparent == nParentID)
				{
					HTREEITEM hTreeItem = m_Tree.InsertItem(pathArray[i],hParent);
					if(FileIsReadOnly(AString(pathName)))
					{
						m_Tree.SetItemImage(hTreeItem,4,4);
					}else
					{
						m_Tree.SetItemImage(hTreeItem,5,5);
					}
				}
			}
		}
	}
	EndWaitCursor();
	*pResult = 0;
}

void CQuestionsDlg::AddQuestion( CString *pData, CString sFolder)
{
	//创建文件夹
	int id = g_QuestionsFolder.FindFolder(sFolder);
	if(id==0) 
	{
		id = g_QuestionsFolder.AddFolder(sFolder);
		if(id==0)
		{
			MessageBox("创建文件夹失败！");
			return;
		}
	}

	//创建问题文件
	CQuestionData mQuestionData;
	mQuestionData.nAnswer = 6;
	mQuestionData.strQuestion = pData[0];
	for( int i = 0; i < 6; ++i)
	{
		CString temp = pData[i+1];
		temp.MakeLower();
		temp.Replace("num","");
		mQuestionData.strAnswer[i] = temp;
		if(mQuestionData.strAnswer[i].IsEmpty())
		{
			mQuestionData.nAnswer = i;
			break;
		}
	}


	CString key = pData[7];
	key.MakeLower();
	mQuestionData.key = 0;
	if(key.Find("a") != -1) mQuestionData.key = mQuestionData.key | 1;
	if(key.Find("b") != -1) mQuestionData.key = mQuestionData.key | 2;
	if(key.Find("c") != -1) mQuestionData.key = mQuestionData.key | 4;
	if(key.Find("d") != -1) mQuestionData.key = mQuestionData.key | 8;
	if(key.Find("e") != -1) mQuestionData.key = mQuestionData.key | 16;
	if(key.Find("f") != -1) mQuestionData.key = mQuestionData.key | 32;
	
	
	CString pathName,Name;
	int k = 0;
	Name.Format("auto_%d.qst",g_QuestionsIDMan.GetIdNum());
	int nQuestionID = g_QuestionsIDMan.CreateID(AString(Name));
	if(nQuestionID==0) 
	{
		MessageBox("CQuestionsDlg::AddQuestion(): 创建ID失败，可能已经有相同的问题名字存在!");
		return;
	}
	
	int nf = nQuestionID/1000;
	if(nf > 25) nf = 25;
	
	mQuestionData.SetID(nQuestionID);
	mQuestionData.nparent = id;
	pathName.Format("Questions\\%c",cFolder[nf]);
	CreateDirectory(pathName, NULL);
	pathName.Format("Questions\\%c\\%s",cFolder[nf],Name);
	if(!mQuestionData.Save(pathName))
	{
		MessageBox("CQuestionsDlg::AddQuestion(),保存文件失败!");
		return;
	}
	
	/*
	g_VSS.SetProjectPath("Questions");
	CString strNewFolder;
	strNewFolder.Format("%c",cFolder[nf]);
	g_VSS.CreateProject(strNewFolder);
	strNewFolder.Format("Questions\\%c",cFolder[nf]);
	g_VSS.SetProjectPath(strNewFolder);
	g_VSS.AddFile(pathName);
	*/
	
	FreshQuestions();
	
}

void CQuestionsDlg::OnButtonImport() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE,
					NULL,
					NULL,
					OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR,
					"Excel files(*.xls)|*.xls|",
					NULL);
	if(dlg.DoModal()!=IDOK) return;
	
	BeginWaitCursor();
	CString name = dlg.GetFileTitle();
	CSpreadSheet ss(dlg.GetPathName(),"Sheet",false);
	int col = ss.GetTotalColumns();
	for( int i = 1; i < col + 1; i += 1)
	{
		CStringArray strArray1,strArray2;
		ss.ReadColumn(strArray1,i);
		int k = strArray1.GetSize();
		if(k%8 != 1) 
		{
			MessageBox("有些列的行数不对！应该8n + 1行。");
			EndWaitCursor();
			return;
		}
		for( int j = 1; j < k; j += 8)
		{
			
			CString key[8];
			for( int l = 0; l < 8; ++l)
				key[l] = strArray1.GetAt(j + l);
			if(key[0].IsEmpty()) continue;
			AddQuestion(key,name);
		}
	}
	EndWaitCursor();
}
