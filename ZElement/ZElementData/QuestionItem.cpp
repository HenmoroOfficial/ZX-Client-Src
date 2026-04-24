// QuestionItem.cpp : implementation file
//

#include "stdafx.h"
#include "elementdata.h"
#include "QuestionItem.h"
#include "QuestionMan.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQuestionItem dialog


CQuestionItem::CQuestionItem(CWnd* pParent /*=NULL*/)
	: CDialog(CQuestionItem::IDD, pParent)
{
	//{{AFX_DATA_INIT(CQuestionItem)
	m_strQuestion = _T("");
	m_strAnswerF = _T("");
	m_strAnswerE = _T("");
	m_strAnswerD = _T("");
	m_strAnswerC = _T("");
	m_strAnswerB = _T("");
	m_strAnswerA = _T("");
	m_bKeyA = FALSE;
	m_bKeyB = FALSE;
	m_bKeyC = FALSE;
	m_bKeyD = FALSE;
	m_bKeyE = FALSE;
	m_bKeyF = FALSE;
	m_bDeleted = FALSE;
	//}}AFX_DATA_INIT

	m_pCurQuestion = 0;
	m_nAnswer = 0;
	m_bReadOnly = false;
}


void CQuestionItem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQuestionItem)
	DDX_Text(pDX, IDC_EDIT_QUESTION, m_strQuestion);
	DDX_Text(pDX, IDC_EDIT_ANSWER_F, m_strAnswerF);
	DDX_Text(pDX, IDC_EDIT_ANSWER_E, m_strAnswerE);
	DDX_Text(pDX, IDC_EDIT_ANSWER_D, m_strAnswerD);
	DDX_Text(pDX, IDC_EDIT_ANSWER_C, m_strAnswerC);
	DDX_Text(pDX, IDC_EDIT_ANSWER_B, m_strAnswerB);
	DDX_Text(pDX, IDC_EDIT_ANSWER_A, m_strAnswerA);
	DDX_Check(pDX, IDC_CHECK_A, m_bKeyA);
	DDX_Check(pDX, IDC_CHECK_B, m_bKeyB);
	DDX_Check(pDX, IDC_CHECK_C, m_bKeyC);
	DDX_Check(pDX, IDC_CHECK_D, m_bKeyD);
	DDX_Check(pDX, IDC_CHECK_E, m_bKeyE);
	DDX_Check(pDX, IDC_CHECK_F, m_bKeyF);
	DDX_Check(pDX, IDC_CHECK_DELETED, m_bDeleted);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CQuestionItem, CDialog)
	//{{AFX_MSG_MAP(CQuestionItem)
	ON_BN_CLICKED(IDC_RADIO_2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO_3, OnRadio3)
	ON_BN_CLICKED(IDC_RADIO_4, OnRadio4)
	ON_BN_CLICKED(IDC_RADIO_5, OnRadio5)
	ON_BN_CLICKED(IDC_RADIO_6, OnRadio6)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQuestionItem message handlers
bool CQuestionItem::LoadQuestion()
{
	if(m_pCurQuestion) delete m_pCurQuestion;
	m_pCurQuestion = new CQuestionData;
	bool br = m_pCurQuestion->Load(m_strPathName);
	if(br) return true;
	delete m_pCurQuestion;
	m_pCurQuestion = NULL;
	return false;
}

BOOL CQuestionItem::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(FileIsReadOnly(AString(m_strPathName)))
		m_bReadOnly = true;
	
	
	if(!LoadQuestion()) 
	{
		CString msg;
		msg.Format("不能打开文件：%s!",m_strPathName);
		MessageBox(msg,"错误",MB_ICONSTOP);
		EndDialog(1);
		return FALSE;
	}

	switch(m_pCurQuestion->nAnswer)
	{
	case 2: OnRadio2(); break;
	case 3: OnRadio3(); break;
	case 4: OnRadio4(); break;
	case 5: OnRadio5(); break;
	case 6: OnRadio6(); break;
	}

	m_strQuestion = m_pCurQuestion->strQuestion;
	m_strAnswerA = m_pCurQuestion->strAnswer[0];
	m_strAnswerB = m_pCurQuestion->strAnswer[1];
	m_strAnswerC = m_pCurQuestion->strAnswer[2];
	m_strAnswerD = m_pCurQuestion->strAnswer[3];
	m_strAnswerE = m_pCurQuestion->strAnswer[4];
	m_strAnswerF = m_pCurQuestion->strAnswer[5];

	m_bKeyA = (m_pCurQuestion->key & 1) == 0 ? FALSE : TRUE;
	m_bKeyB = (m_pCurQuestion->key & 2) == 0 ? FALSE : TRUE;
	m_bKeyC = (m_pCurQuestion->key & 4) == 0 ? FALSE : TRUE;
	m_bKeyD = (m_pCurQuestion->key & 8) == 0 ? FALSE : TRUE;
	m_bKeyE = (m_pCurQuestion->key & 16) == 0 ? FALSE : TRUE;
	m_bKeyF = (m_pCurQuestion->key & 32) == 0 ? FALSE : TRUE;
	m_bDeleted = m_pCurQuestion->bDeleted;
	UpdateData(FALSE);
	SetWindowText(m_strPathName);
	if(m_bReadOnly) DisableInterface();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CQuestionItem::EnableItem(char c,BOOL bEnable)
{
	CWnd *pwnd = NULL;
	switch(c)
	{
	case 'a':
		pwnd = GetDlgItem(IDC_EDIT_ANSWER_A);
		if(pwnd) pwnd->EnableWindow(bEnable);
		pwnd = GetDlgItem(IDC_CHECK_A);
		if(pwnd) pwnd->EnableWindow(bEnable);
		break;
	case 'b':
		pwnd = GetDlgItem(IDC_EDIT_ANSWER_B);
		if(pwnd) pwnd->EnableWindow(bEnable);
		pwnd = GetDlgItem(IDC_CHECK_B);
		if(pwnd) pwnd->EnableWindow(bEnable);
		break;
	case 'c':
		pwnd = GetDlgItem(IDC_EDIT_ANSWER_C);
		if(pwnd) pwnd->EnableWindow(bEnable);
		pwnd = GetDlgItem(IDC_CHECK_C);
		if(pwnd) pwnd->EnableWindow(bEnable);
		break;
	case 'd':
		pwnd = GetDlgItem(IDC_EDIT_ANSWER_D);
		if(pwnd) pwnd->EnableWindow(bEnable);
		pwnd = GetDlgItem(IDC_CHECK_D);
		if(pwnd) pwnd->EnableWindow(bEnable);
		break;
	case 'e':
		pwnd = GetDlgItem(IDC_EDIT_ANSWER_E);
		if(pwnd) pwnd->EnableWindow(bEnable);
		pwnd = GetDlgItem(IDC_CHECK_E);
		if(pwnd) pwnd->EnableWindow(bEnable);
		break;
	case 'f':
		pwnd = GetDlgItem(IDC_EDIT_ANSWER_F);
		if(pwnd) pwnd->EnableWindow(bEnable);
		pwnd = GetDlgItem(IDC_CHECK_F);
		if(pwnd) pwnd->EnableWindow(bEnable);
		break;
	}
}

void CQuestionItem::DisableAllItem()
{
	EnableItem('a',false);
	EnableItem('b',false);
	EnableItem('c',false);
	EnableItem('d',false);
	EnableItem('e',false);
	EnableItem('f',false);
}

void CQuestionItem::DisableInterface()
{
	DisableAllItem();
	
	CWnd *pwnd = GetDlgItem(IDC_EDIT_QUESTION);
	pwnd->EnableWindow(false);
	
	pwnd = GetDlgItem(IDC_RADIO_2);
	pwnd->EnableWindow(false);

	pwnd = GetDlgItem(IDC_RADIO_3);
	pwnd->EnableWindow(false);

	pwnd = GetDlgItem(IDC_RADIO_4);
	pwnd->EnableWindow(false);

	pwnd = GetDlgItem(IDC_RADIO_5);
	pwnd->EnableWindow(false);

	pwnd = GetDlgItem(IDC_RADIO_6);
	pwnd->EnableWindow(false);

	pwnd = GetDlgItem(IDC_CHECK_DELETED);
	pwnd->EnableWindow(false);
}

void CQuestionItem::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	DisableAllItem();
	EnableItem('a',true);
	EnableItem('b',true);
	m_nAnswer = 2;
	SetItemCheck(m_nAnswer);
}

void CQuestionItem::OnRadio3() 
{
	// TODO: Add your control notification handler code here
	DisableAllItem();
	EnableItem('a',true);
	EnableItem('b',true);
	EnableItem('c',true);
	m_nAnswer = 3;
	SetItemCheck(m_nAnswer);
}

void CQuestionItem::OnRadio4() 
{
	// TODO: Add your control notification handler code here
	DisableAllItem();
	EnableItem('a',true);
	EnableItem('b',true);
	EnableItem('c',true);
	EnableItem('d',true);
	m_nAnswer = 4;
	SetItemCheck(m_nAnswer);
}

void CQuestionItem::OnRadio5() 
{
	// TODO: Add your control notification handler code here
	DisableAllItem();
	EnableItem('a',true);
	EnableItem('b',true);
	EnableItem('c',true);
	EnableItem('d',true);
	EnableItem('e',true);
	m_nAnswer = 5;
	SetItemCheck(m_nAnswer);
}

void CQuestionItem::OnRadio6() 
{
	// TODO: Add your control notification handler code here
	DisableAllItem();
	EnableItem('a',true);
	EnableItem('b',true);
	EnableItem('c',true);
	EnableItem('d',true);
	EnableItem('e',true);
	EnableItem('f',true);
	m_nAnswer = 6;
	SetItemCheck(m_nAnswer);
}

void CQuestionItem::SetItemCheck(int n)
{
	CButton *pbutton = NULL;
	pbutton = (CButton*)GetDlgItem(IDC_RADIO_2);
	if(n!=2) pbutton->SetCheck(0); else pbutton->SetCheck(1);
	pbutton = (CButton*)GetDlgItem(IDC_RADIO_3);
	if(n!=3) pbutton->SetCheck(0); else pbutton->SetCheck(1);
	pbutton = (CButton*)GetDlgItem(IDC_RADIO_4);
	if(n!=4) pbutton->SetCheck(0); else pbutton->SetCheck(1);
	pbutton = (CButton*)GetDlgItem(IDC_RADIO_5);
	if(n!=5) pbutton->SetCheck(0); else pbutton->SetCheck(1);
	pbutton = (CButton*)GetDlgItem(IDC_RADIO_6);
	if(n!=6) pbutton->SetCheck(0); else pbutton->SetCheck(1);
}


bool CQuestionItem::IsChanged()
{
	UpdateData(true);

	if(m_pCurQuestion->nAnswer != m_nAnswer) return true;
	if(m_pCurQuestion->strQuestion != m_strQuestion) return true;
	if(m_pCurQuestion->strAnswer[0] != m_strAnswerA) return true; 
	if(m_pCurQuestion->strAnswer[1] != m_strAnswerB) return true; 
	if(m_pCurQuestion->strAnswer[2] != m_strAnswerC) return true; 
	if(m_pCurQuestion->strAnswer[3] != m_strAnswerD) return true; 
	if(m_pCurQuestion->strAnswer[4] != m_strAnswerE) return true; 
	if(m_pCurQuestion->strAnswer[5] != m_strAnswerF) return true; 

	BYTE key = 0;
	if(m_bKeyA) key = key | 1;
	if(m_bKeyB) key = key | 2;
	if(m_bKeyC && m_nAnswer > 2) key = key | 4;
	if(m_bKeyD && m_nAnswer > 3) key = key | 8;
	if(m_bKeyE && m_nAnswer > 4) key = key | 16;
	if(m_bKeyF && m_nAnswer > 5) key = key | 32;
	bool bDel = m_bDeleted ? true : false;

	if(key != m_pCurQuestion->key) return true;
	if(bDel != m_pCurQuestion->bDeleted) return true;

	return false;
}

void CQuestionItem::OnOK()
{
	if(m_pCurQuestion)
	{
		UpdateData(true);
		
		m_pCurQuestion->nAnswer = m_nAnswer;
		m_pCurQuestion->strQuestion = m_strQuestion;
		m_pCurQuestion->strAnswer[0] = m_strAnswerA; 
		m_pCurQuestion->strAnswer[1] = m_strAnswerB; 
		m_pCurQuestion->strAnswer[2] = m_strAnswerC; 
		m_pCurQuestion->strAnswer[3] = m_strAnswerD; 
		m_pCurQuestion->strAnswer[4] = m_strAnswerE; 
		m_pCurQuestion->strAnswer[5] = m_strAnswerF; 
		
		m_pCurQuestion->key = 0;
		if(m_bKeyA) m_pCurQuestion->key = m_pCurQuestion->key | 1;
		if(m_bKeyB) m_pCurQuestion->key = m_pCurQuestion->key | 2;
		if(m_bKeyC && m_nAnswer > 2) m_pCurQuestion->key = m_pCurQuestion->key | 4;
		if(m_bKeyD && m_nAnswer > 3) m_pCurQuestion->key = m_pCurQuestion->key | 8;
		if(m_bKeyE && m_nAnswer > 4) m_pCurQuestion->key = m_pCurQuestion->key | 16;
		if(m_bKeyF && m_nAnswer > 5) m_pCurQuestion->key = m_pCurQuestion->key | 32;
		m_pCurQuestion->bDeleted = m_bDeleted ? true : false;
		if(!m_bReadOnly)
		{
			if(!m_pCurQuestion->Save(m_strPathName))
			{
				CString msg;
				msg.Format("保存文件失败：%s",m_strPathName);
				MessageBox(msg);
				return;
			}
		}
		delete m_pCurQuestion;
		m_pCurQuestion = NULL;
	}

	CDialog::OnOK();
}
void CQuestionItem::OnCancel()
{
	if(!m_bReadOnly && IsChanged())
	{
		if(IDYES==AfxMessageBox("数据已经改变, 你需要保存吗?",MB_YESNO))
			OnOK();
	}
	if(m_pCurQuestion)
	{
		delete m_pCurQuestion;
		m_pCurQuestion = NULL;
	}

	CDialog::OnCancel();
}
