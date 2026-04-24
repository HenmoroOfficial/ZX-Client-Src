// ImportTimeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "elementeditor.h"
#include "ImportTimeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern const char* szWeek[];
/////////////////////////////////////////////////////////////////////////////
// CImportTimeDlg dialog


CImportTimeDlg::CImportTimeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImportTimeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImportTimeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CImportTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImportTimeDlg)
	DDX_Control(pDX, IDC_COMBO_YEAR, m_cbYear);
	DDX_Control(pDX, IDC_COMBO_WEEK, m_cbWeek);
	DDX_Control(pDX, IDC_COMBO_MOUTH, m_cbMouth);
	DDX_Control(pDX, IDC_COMBO_MINU, m_cbMinu);
	DDX_Control(pDX, IDC_COMBO_HOUR, m_cbHour);
	DDX_Control(pDX, IDC_COMBO_DAY, m_cbDay);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CImportTimeDlg, CDialog)
	//{{AFX_MSG_MAP(CImportTimeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImportTimeDlg message handlers

void CImportTimeDlg::InitCombox()
{
	CString str,other;
	int sel = 0;
	
	//2
	if(m_Time.nYear==-1) sel = 0;
	else sel = m_Time.nYear - 2005;
	
	m_cbYear.InsertString(0,"任意");
	m_cbYear.InsertString(1,"2006");
	m_cbYear.InsertString(2,"2007");
	m_cbYear.InsertString(3,"2008");
	m_cbYear.InsertString(4,"2009");
	m_cbYear.InsertString(5,"2010");
	m_cbYear.InsertString(6,"2011");
	m_cbYear.InsertString(7,"2012");
	m_cbYear.InsertString(8,"2013");
	m_cbYear.InsertString(9,"2014");
	m_cbYear.InsertString(10,"2015");
	m_cbYear.InsertString(11,"2016");
	m_cbYear.InsertString(12,"2017");
	m_cbYear.InsertString(13,"2018");
	m_cbYear.SetCurSel(sel);
	
	other = szWeek[m_Time.nWeek+1];
	int i;
	for( i =0; i < 8; ++i)
	{
		str.Format(szWeek[i]);
		m_cbWeek.InsertString(i,str);
		if(other==str) sel = i;
	}
	m_cbWeek.SetCurSel(sel);
	
	other.Format("%d",m_Time.nMouth + 1);
	sel=0;
	m_cbMouth.InsertString(0,"任意");
	for( i = 0; i < 12; ++i)
	{
		str.Format("%d",i+1);
		m_cbMouth.InsertString(i+1,str);
		if(other==str) sel = i+1;
	}
	m_cbMouth.SetCurSel(sel);
	
	other.Format("%d",m_Time.nMinutes);
	sel=0;
	m_cbMinu.InsertString(0,"任意");
	for( i = 0; i < 60; ++i)
	{
		str.Format("%d",i);
		m_cbMinu.InsertString(i+1,str);
		if(other==str) sel = i + 1;
	}
	m_cbMinu.SetCurSel(sel);

	other.Format("%d",m_Time.nHours);
	sel=0;
	m_cbHour.InsertString(0,"任意");
	for( i = 0; i < 24; ++i)
	{
		str.Format("%d",i);
		m_cbHour.InsertString(i+1,str);
		if(other==str) sel = i+1;
	}
	m_cbHour.SetCurSel(sel);
	
	other.Format("%d",m_Time.nDay);
	sel=0;
	m_cbDay.InsertString(0,"任意");
	for( i = 1; i < 32; ++i)
	{
		str.Format("%d",i);
		m_cbDay.InsertString(i,str);
		if(other==str) sel = i;
	}
	m_cbDay.SetCurSel(sel);
}


void CImportTimeDlg::OnOK()
{
	
	CString txt;
	int sel = m_cbYear.GetCurSel();
	m_cbYear.GetLBText(sel,txt);
	if(txt=="任意") m_Time.nYear = -1;
	else m_Time.nYear = (int)atof(txt);

	sel = m_cbMouth.GetCurSel();
	m_cbMouth.GetLBText(sel,txt);
	if(txt=="任意") m_Time.nMouth = -1;
	else m_Time.nMouth = (int)atof(txt) - 1;

	sel = m_cbWeek.GetCurSel();
	m_cbWeek.GetLBText(sel,txt);
	if(txt=="任意") m_Time.nWeek = -1;
	else m_Time.nWeek = sel - 1;

	sel = m_cbDay.GetCurSel();
	m_cbDay.GetLBText(sel,txt);
	if(txt=="任意") m_Time.nDay = -1;
	else m_Time.nDay = (int)atof(txt);

	sel = m_cbHour.GetCurSel();
	m_cbHour.GetLBText(sel,txt);
	if(txt=="任意") m_Time.nHours = -1;
	else m_Time.nHours = (int)atof(txt);

	sel = m_cbMinu.GetCurSel();
	m_cbMinu.GetLBText(sel,txt);
	if(txt=="任意") m_Time.nMinutes = -1;
	else m_Time.nMinutes = (int)atof(txt);
	
	CDialog::OnOK();
}

BOOL CImportTimeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitCombox();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
