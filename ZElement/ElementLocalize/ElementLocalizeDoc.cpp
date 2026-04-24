// ElementLocalizeDoc.cpp : implementation of the CElementLocalizeDoc class
//

#include "stdafx.h"
#include "ElementLocalize.h"

#include "ElementLocalizeDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CElementLocalizeDoc

IMPLEMENT_DYNCREATE(CElementLocalizeDoc, CDocument)

BEGIN_MESSAGE_MAP(CElementLocalizeDoc, CDocument)
	//{{AFX_MSG_MAP(CElementLocalizeDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CElementLocalizeDoc construction/destruction

CElementLocalizeDoc::CElementLocalizeDoc()
{
	// TODO: add one-time construction code here

}

CElementLocalizeDoc::~CElementLocalizeDoc()
{
}

BOOL CElementLocalizeDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CElementLocalizeDoc serialization

void CElementLocalizeDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CElementLocalizeDoc diagnostics

#ifdef _DEBUG
void CElementLocalizeDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CElementLocalizeDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CElementLocalizeDoc commands
