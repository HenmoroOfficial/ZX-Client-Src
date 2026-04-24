// FixedNpcSelDlg.cpp : implementation file
//

#include "global.h"
#include "elementeditor.h"
#include "FixedNpcSelDlg.h"
#include "FindMonsterDlg.h"
#include "EdtDataCache.h"
#include "A3D.h"


//#define new A_DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// CFixedNpcSelDlg dialog


CFixedNpcSelDlg::CFixedNpcSelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFixedNpcSelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFixedNpcSelDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFixedNpcSelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFixedNpcSelDlg)
	DDX_Control(pDX, IDC_COMBO_SEL, m_comboSel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFixedNpcSelDlg, CDialog)
	//{{AFX_MSG_MAP(CFixedNpcSelDlg)
	ON_COMMAND(ID_FILE_SORT_MAP, OnFileSortMap)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RADIO_NPC, OnRadioNpc)
	ON_BN_CLICKED(IDC_RADIO_MONSTER, OnRadioMonster)
	ON_BN_CLICKED(IDC_BUTTON_FIND, OnButtonFind)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RADIO_INTERACTION, &CFixedNpcSelDlg::OnBnClickedRadioInteraction)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFixedNpcSelDlg message handlers

BOOL CFixedNpcSelDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	FreshList();
	CString strTemp;
	strTemp.Format("%s->%d",m_strName,m_dwSelId);
	int index = m_comboSel.FindString(0,m_strName);
	m_comboSel.SetCurSel(index);
	CButton *pButton = (CButton *)GetDlgItem(IDC_RADIO_NPC);
	if(m_cType==0) pButton->SetCheck(1);
	else pButton->SetCheck(0);

	pButton = (CButton *)GetDlgItem(IDC_RADIO_MONSTER);
	if(m_cType==1) pButton->SetCheck(1);
	else pButton->SetCheck(0);

	pButton = (CButton *)GetDlgItem(IDC_RADIO_INTERACTION);
	if(m_cType==2) pButton->SetCheck(1);
	else pButton->SetCheck(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFixedNpcSelDlg::OnOK()
{
	int index = m_comboSel.GetCurSel();
	if(index!=-1)
	{	
		CString str;
		m_comboSel.GetLBText(index,str);
		m_strName = AString(str);
		m_dwSelId = GetNpcID(m_strName);
		int pos = m_strName.FindOneOf("->");
		if(pos==-1) 
		{
			MessageBox("NPC的名字中没有->符号!");
			CDialog::OnCancel();
		}
		m_strName = m_strName.Left(pos);
	}
	CDialog::OnOK();
}

DWORD CFixedNpcSelDlg::GetNpcID(CString strName)
{

	g_EdtDataCache.InitData();
	VecIds vecIdList;
	int i;
	DATA_TYPE type;
	AString strID;
	AString name;
	vecIdList = g_EdtDataCache.GetNPCs();
	int iNum = vecIdList.size();
	for (i=0; i<iNum; ++i)
	{
		NPC_ESSENCE* pData;
		pData = (NPC_ESSENCE*)g_dataMan.get_data_ptr(vecIdList[i],ID_SPACE_ESSENCE,type);
		assert(type==DT_NPC_ESSENCE);
		name = WC2AS((const wchar_t*)pData->name);
		strID.Format("->%d",pData->id);
		name +=strID;
		if(strcmp(name,strName)==0)
			return vecIdList[i];
	}
	vecIdList = g_EdtDataCache.GetMonsters();
	iNum = vecIdList.size();
	for (i=0; i<iNum; ++i)
	{
		MONSTER_ESSENCE* pData;
		pData = (MONSTER_ESSENCE*)g_dataMan.get_data_ptr(vecIdList[i],ID_SPACE_ESSENCE,type);
		assert(type==DT_MONSTER_ESSENCE);
		name = WC2AS((const wchar_t*)pData->name);
		strID.Format("->%d",pData->id);
		name +=strID;
		if(strcmp(name,strName)==0)
			return vecIdList[i];
	}
	vecIdList = g_EdtDataCache.GetInterObjects();
	iNum = vecIdList.size();
	for (i=0; i<iNum; ++i)
	{
		INTERACTION_OBJECT_ESSENCE* pData;
		pData = (INTERACTION_OBJECT_ESSENCE*)g_dataMan.get_data_ptr(vecIdList[i],ID_SPACE_ESSENCE,type);
		assert(type==DT_INTERACTION_OBJECT_ESSENCE);
		name = WC2AS((const wchar_t*)pData->name);
		strID.Format("->%d",pData->id);
		name +=strID;
		if(strcmp(name,strName)==0)
			return vecIdList[i];
	}

	return 0;
}

void CFixedNpcSelDlg::OnFileSortMap() 
{
	// TODO: Add your command handler code here
	
}

void CFixedNpcSelDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
}

void CFixedNpcSelDlg::OnRadioNpc() 
{
	// TODO: Add your control notification handler code here
	if(m_cType!=0)
	{
		m_cType = 0;
		m_dwSelId = -1;
	    m_strName = "";
		FreshList();
	}
}

void CFixedNpcSelDlg::OnRadioMonster() 
{
	// TODO: Add your control notification handler code here
	if(m_cType!=1)
	{
		m_cType = 1;
		m_dwSelId = -1;
	    m_strName = "";
		FreshList();
	}
}

void  CFixedNpcSelDlg::FreshList()
{
	int num = m_comboSel.GetCount();
	for(int n = 0; n < num; n++) m_comboSel.DeleteString(0);

	g_EdtDataCache.InitData();
	VecIds vecIdList;
	int i;
	DATA_TYPE type;
	AString strName;
	AString strID;
	if (m_cType==0)
	{
		vecIdList = g_EdtDataCache.GetNPCs();
		int iNum = vecIdList.size();
		for (i=0; i<iNum; ++i)
		{
			NPC_ESSENCE* pData;
			pData = (NPC_ESSENCE*)g_dataMan.get_data_ptr(vecIdList[i],ID_SPACE_ESSENCE,type);
			assert(type==DT_NPC_ESSENCE);
			strName = WC2AS((const wchar_t*)pData->name);
			strID.Format("->%d",pData->id);
			strName +=strID;
			m_comboSel.AddString(strName);
		}
	}
	else if (m_cType==1)
	{
		vecIdList = g_EdtDataCache.GetMonsters();
		int iNum = vecIdList.size();
		for (i=0; i<iNum; ++i)
		{
			MONSTER_ESSENCE* pData;
			pData = (MONSTER_ESSENCE*)g_dataMan.get_data_ptr(vecIdList[i],ID_SPACE_ESSENCE,type);
			assert(type==DT_MONSTER_ESSENCE);
			strName = WC2AS((const wchar_t*)pData->name);
			strID.Format("->%d",pData->id);
			strName +=strID;
			m_comboSel.AddString(strName);
		}
	}
	else if (m_cType==2)
	{
		vecIdList = g_EdtDataCache.GetInterObjects();
		int iNum = vecIdList.size();
		for (i=0; i<iNum; ++i)
		{
			INTERACTION_OBJECT_ESSENCE* pData;
			pData = (INTERACTION_OBJECT_ESSENCE*)g_dataMan.get_data_ptr(vecIdList[i],ID_SPACE_ESSENCE,type);
			assert(type==DT_INTERACTION_OBJECT_ESSENCE);
			strName = WC2AS((const wchar_t*)pData->name);
			strID.Format("->%d",pData->id);
			strName +=strID;
			m_comboSel.AddString(strName);
		}
	}

	return;
}

void CFixedNpcSelDlg::OnButtonFind() 
{
	// TODO: Add your control notification handler code here
	CFindMonsterDlg dlg;
	if(m_cType==0)
		dlg.m_nType = DT_NPC_ESSENCE;
	else if (m_cType==2)
		dlg.m_nType = DT_INTERACTION_OBJECT_ESSENCE;
	else
		dlg.m_nType = DT_MONSTER_ESSENCE;

	if(dlg.DoModal()==IDOK)
	{
		int sel = m_comboSel.FindString(0,dlg.m_strSelectedMonster);
		if(sel!=-1) m_comboSel.SetCurSel(sel);
	}
}

void CFixedNpcSelDlg::OnBnClickedRadioInteraction()
{
	if(m_cType!=2)
	{
		m_cType = 2;
		m_dwSelId = -1;
		m_strName = "";
		FreshList();
	}
}
