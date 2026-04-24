// ElementLocalizeDoc.h : interface of the CElementLocalizeDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ELEMENTLOCALIZEDOC_H__120A9E1A_F86D_4403_AD75_60267CCD4052__INCLUDED_)
#define AFX_ELEMENTLOCALIZEDOC_H__120A9E1A_F86D_4403_AD75_60267CCD4052__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CElementLocalizeDoc : public CDocument
{
protected: // create from serialization only
	CElementLocalizeDoc();
	DECLARE_DYNCREATE(CElementLocalizeDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CElementLocalizeDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CElementLocalizeDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CElementLocalizeDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELEMENTLOCALIZEDOC_H__120A9E1A_F86D_4403_AD75_60267CCD4052__INCLUDED_)
