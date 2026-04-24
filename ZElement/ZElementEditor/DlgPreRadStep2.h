// PreRadiosityStep2.h: interface for the CPreRadiosityStep2 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRERADIOSITYSTEP2_H__4DBB86AD_4683_4DC4_914C_074E3F2F52F0__INCLUDED_)
#define AFX_PRERADIOSITYSTEP2_H__4DBB86AD_4683_4DC4_914C_074E3F2F52F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CElementMap;
struct PRSTEP1_PARAM;
struct LIGHTDATA;

struct PRERADSTEP2_CONFIG
{
	BOOL	bGenTerrainLitmap;
	BOOL	bSwitchLitmapRender;
	BOOL	bGenSelectOnly;
	BOOL	bReadOnly;
	INT		nIterCount;
	INT		emidDayNight;
	float	fGamma;
	int		idDayNight;

	PRERADSTEP2_CONFIG()
		: bGenTerrainLitmap(FALSE)
		, bSwitchLitmapRender(TRUE)
		, bGenSelectOnly(TRUE)
		, bReadOnly(FALSE)
		, nIterCount(1)
		, emidDayNight(0)
		, fGamma(0.6f)
		, idDayNight(IDC_DAY)
	{

	}
};

class CDlgPreRadStep2 : public CDialog
{
public:
	enum { IDD = IDD_PRE_RADIOSITY_STEP2 };
	static PRERADSTEP2_CONFIG s_Config;
private:
	CElementMap*			m_pMap;
	PRSTEP1_PARAM			m_Step1Param;
	PRERADSTEP2_CONFIG*		m_pCurConfig;
	//int						m_nGammaValue;

	virtual BOOL OnInitDialog();
	//virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg void OnCheck1();
	afx_msg void OnCheck2();
	afx_msg void OnRadioButton();
	afx_msg void OnCheck_SelOnly();
	afx_msg void OnEdit1Change();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);


	BOOL ExportTerrainDiffuse();
	BOOL ExportHeightMap();
	BOOL ExportXML(int nDayNight, BOOL bExportLitModel);
	VOID GenLitmap();

	DECLARE_MESSAGE_MAP()
public:
	CDlgPreRadStep2(CElementMap* pMap, const PRSTEP1_PARAM* pStep1Param, PRERADSTEP2_CONFIG* pCurConfig);
	virtual ~CDlgPreRadStep2();

	static void  ReloadLitmap(CElementMap* pMap, bool bLoadVertLight);
	static void  ReloadTerrainLitmap(CElementMap* pMap);
	static void  ReloadTerrainLitmap(CElementMap* pMap, int nDayNight);
	static float ComputeLightRadius(const LIGHTDATA& LightData);
};

#endif // !defined(AFX_PRERADIOSITYSTEP2_H__4DBB86AD_4683_4DC4_914C_074E3F2F52F0__INCLUDED_)
