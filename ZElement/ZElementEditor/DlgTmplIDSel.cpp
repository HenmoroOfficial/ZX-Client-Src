// DlgTmplIDSel.cpp : implementation file
//

#include "stdafx.h"
#include "global.h"
#include "ElementEditor.h"
#include "DlgTmplIDSel.h"
#include "EdtDataCache.h"
#include "elementdataman.h"
#include "ExpTypes.h"


// CDlgTmplIDSel dialog

IMPLEMENT_DYNAMIC(CDlgTmplIDSel, CDialog)

CDlgTmplIDSel::CDlgTmplIDSel(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTmplIDSel::IDD, pParent)
{

}

CDlgTmplIDSel::~CDlgTmplIDSel()
{
}

void CDlgTmplIDSel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_comboType);
	DDX_Control(pDX, IDC_COMBO_LIST, m_comboList);
}


BEGIN_MESSAGE_MAP(CDlgTmplIDSel, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgTmplIDSel::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, &CDlgTmplIDSel::OnCbnSelchangeComboType)
END_MESSAGE_MAP()


// CDlgTmplIDSel message handlers

void CDlgTmplIDSel::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	int index = m_comboList.GetCurSel();
	if(index!=-1)
	{	
		CString str;
		m_comboList.GetLBText(index,str);
		int pos = str.ReverseFind('>');
		if(pos==-1) 
		{
			MessageBox("名字中没有->符号!");
			CDialog::OnCancel();
		}

		m_strName = str.Left(pos-1);
		CString strId = str.Right(str.GetLength() - pos - 1);
		m_dwSelId = atoi(strId);
	}

	OnOK();
}

void CDlgTmplIDSel::OnCbnSelchangeComboType()
{
	int num = m_comboList.GetCount();
	for(int n = 0; n < num; n++) m_comboList.DeleteString(0);

	g_EdtDataCache.InitData();
	VecIds vecIdList;
	AString strName;
	AString strID;
	DATA_TYPE type;
	switch (m_comboType.GetCurSel())
	{
	case 0:
		vecIdList = g_EdtDataCache.GetRegionBlocks();
		int iNum = vecIdList.size();
		for (int i=0; i<iNum; ++i)
		{
			REGION_BLOCK_ESSENCE* pData;
			pData = (REGION_BLOCK_ESSENCE*)g_dataMan.get_data_ptr(vecIdList[i],ID_SPACE_ESSENCE,type);
			assert(type==DT_REGION_BLOCK_ESSENCE);
			strName = WC2AS((const wchar_t*)pData->name);
			strID.Format("->%d",pData->id);
			strName +=strID;
			m_comboList.AddString(strName);
		}
		break;
	}
}

BOOL CDlgTmplIDSel::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_comboType.AddString("地块");

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
