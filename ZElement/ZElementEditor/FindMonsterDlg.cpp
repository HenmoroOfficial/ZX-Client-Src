// FindMonsterDlg.cpp : implementation file
//

#include "global.h"
#include "elementeditor.h"
#include "FindMonsterDlg.h"
#include "A3D.h"
#include "MonsterFaction.h"
#include "elementdataman.h"
#include "EdtDataCache.h"
//#define new A_DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// CFindMonsterDlg dialog


CFindMonsterDlg::CFindMonsterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFindMonsterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFindMonsterDlg)
	m_strKey = _T("");
	m_bExa = FALSE;
	//}}AFX_DATA_INIT
	m_nType = DT_MONSTER_ESSENCE;
}


void CFindMonsterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFindMonsterDlg)
	DDX_Control(pDX, IDC_LIST_MONSTER, m_listMonster);
	DDX_Text(pDX, IDC_MONSTER_KEY, m_strKey);
	DDX_Check(pDX, IDC_CHECK_EXA, m_bExa);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFindMonsterDlg, CDialog)
	//{{AFX_MSG_MAP(CFindMonsterDlg)
	ON_BN_CLICKED(IDC_BUTTON_FIND, OnButtonFind)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindMonsterDlg message handlers

void CFindMonsterDlg::OnButtonFind() 
{
	// TODO: Add your control notification handler code here
	int itemNum = m_listMonster.GetCount();
	int i;
	for(i = 0; i < itemNum ; i++)
		m_listMonster.DeleteString(0);

	UpdateData(true);

	g_EdtDataCache.InitData();
	VecIds vecIdList;

	AString strName;
	AString strID;
	AString strTemp;
	DATA_TYPE type;

	if (m_nType==DT_MONSTER_ESSENCE)
	{
		vecIdList= g_EdtDataCache.GetMonsters();
		int iNum = vecIdList.size();
		for (i=0; i<iNum; ++i)
		{
			MONSTER_ESSENCE* pData;
			pData = (MONSTER_ESSENCE*)g_dataMan.get_data_ptr(vecIdList[i],ID_SPACE_ESSENCE,type);
			strName = WC2AS((const wchar_t*)pData->name);
			strTemp = strName;
			strID.Format("->%d",pData->id);
			strName +=strID;
			if(m_bExa)
			{
				if(stricmp(strTemp,m_strKey)==0)
					m_listMonster.AddString(strName);
			}else
			{
				CString temp(strName);
				if(strName.Find(m_strKey)!=-1)
					m_listMonster.AddString(strName);
			}

		}
	}
	else if (m_nType==DT_NPC_ESSENCE)
	{
		vecIdList= g_EdtDataCache.GetNPCs();
		int iNum = vecIdList.size();
		for (i=0; i<iNum; ++i)
		{
			NPC_ESSENCE* pData;
			pData = (NPC_ESSENCE*)g_dataMan.get_data_ptr(vecIdList[i],ID_SPACE_ESSENCE,type);
			strName = WC2AS((const wchar_t*)pData->name);
			strTemp = strName;
			strID.Format("->%d",pData->id);
			strName +=strID;
			if(m_bExa)
			{
				if(stricmp(strTemp,m_strKey)==0)
					m_listMonster.AddString(strName);
			}else
			{
				CString temp(strName);
				if(strName.Find(m_strKey)!=-1)
					m_listMonster.AddString(strName);
			}

		}
	}
	else if (m_nType==DT_MINE_ESSENCE)
	{
		vecIdList= g_EdtDataCache.GetMines();
		int iNum = vecIdList.size();
		for (i=0; i<iNum; ++i)
		{
			MINE_ESSENCE* pData;
			pData = (MINE_ESSENCE*)g_dataMan.get_data_ptr(vecIdList[i],ID_SPACE_ESSENCE,type);
			strName = WC2AS((const wchar_t*)pData->name);
			strTemp = strName;
			strID.Format("->%d",pData->id);
			strName +=strID;
			if(m_bExa)
			{
				if(stricmp(strTemp,m_strKey)==0)
					m_listMonster.AddString(strName);
			}else
			{
				CString temp(strName);
				if(strName.Find(m_strKey)!=-1)
					m_listMonster.AddString(strName);
			}

		}
	}
	else if (m_nType==DT_INTERACTION_OBJECT_ESSENCE)
	{
		vecIdList= g_EdtDataCache.GetInterObjects();
		int iNum = vecIdList.size();
		for (i=0; i<iNum; ++i)
		{
			INTERACTION_OBJECT_ESSENCE* pData;
			pData = (INTERACTION_OBJECT_ESSENCE*)g_dataMan.get_data_ptr(vecIdList[i],ID_SPACE_ESSENCE,type);
			strName = WC2AS((const wchar_t*)pData->name);
			strTemp = strName;
			strID.Format("->%d",pData->id);
			strName +=strID;
			if(m_bExa)
			{
				if(stricmp(strTemp,m_strKey)==0)
					m_listMonster.AddString(strName);
			}else
			{
				CString temp(strName);
				if(strName.Find(m_strKey)!=-1)
					m_listMonster.AddString(strName);
			}

		}
	}

	return;
}

void CFindMonsterDlg::OnOK()
{
	int index = m_listMonster.GetCurSel();
	if(index!=-1)
		m_listMonster.GetText(index,m_strSelectedMonster);
	else m_strSelectedMonster = "";
	
	CDialog::OnOK();
}

BOOL CFindMonsterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CEdit *pEdit = (CEdit*)GetDlgItem(IDC_MONSTER_KEY);
	if(pEdit) pEdit->SetFocus();
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
