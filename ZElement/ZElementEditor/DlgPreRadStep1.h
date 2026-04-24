// PreRadiosityStep1.h: interface for the CPreRadiosityStep1 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRERADIOSITYSTEP1_H__AEF75559_4D58_464F_AA6F_5D50A75BCD72__INCLUDED_)
#define AFX_PRERADIOSITYSTEP1_H__AEF75559_4D58_464F_AA6F_5D50A75BCD72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CElementMap;

struct PRERADSTEP1_CONFIG
{
	int emidGenLitmap;
	BOOL bExportNewScene;
	BOOL bDeleteOldLitmap;
	BOOL bReadOnly;

	PRERADSTEP1_CONFIG()
		: emidGenLitmap(IDC_RADIO2)
		, bExportNewScene(FALSE)
		, bDeleteOldLitmap(FALSE)
		, bReadOnly(FALSE)
	{

	}
};

struct PRSTEP1_PARAM
{
	BOOL bExportModel;
	enum {
		VertLit,
		QuadLitmap,
		Product,
	}emLitmapQuality;
};

class CDlgPreRadStep1 : public CDialog
{
public:
	enum { IDD = IDD_PRE_RADIOSITY_STEP1 };

private:
	CElementMap*				m_pMap;
	PRSTEP1_PARAM				m_Param;

	virtual BOOL OnInitDialog();
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();

	afx_msg void OnCheck1();
	afx_msg void OnCheck2();

	BOOL CheckExportScene();	// 如果场景改变需要导出,返回TRUE
	BOOL DeleteLitmapSceneCache();
	BOOL CreateLitmapSceneCacheDir();
	BOOL DeleteLitmapCache();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	static PRERADSTEP1_CONFIG	s_Config;
	PRERADSTEP1_CONFIG*			m_pCurConfig;

	CDlgPreRadStep1(CElementMap* pMap, PRERADSTEP1_CONFIG* pConfig);
	virtual ~CDlgPreRadStep1();

	//inline BOOL NeedExportScene(){return m_bNeedExportScene;}
	const PRSTEP1_PARAM* GetParam() { return &m_Param; }
};

#endif // !defined(AFX_PRERADIOSITYSTEP1_H__AEF75559_4D58_464F_AA6F_5D50A75BCD72__INCLUDED_)
