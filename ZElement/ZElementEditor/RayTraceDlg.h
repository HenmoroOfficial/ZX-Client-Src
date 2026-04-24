#if !defined(AFX_RAYTRACEDLG_H__69663C31_0571_4D84_B5AA_6AA0D61C3259__INCLUDED_)
#define AFX_RAYTRACEDLG_H__69663C31_0571_4D84_B5AA_6AA0D61C3259__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RayTraceDlg.h : header file
//
#include<AString.h>
#include <vector>
#include "afxwin.h"
#include "afxcmn.h"

class CElementMap;
struct PRSTEP1_PARAM;
struct LIGHTDATA;
class A3DCameraBase;
class A3DVECTOR3;
class CELArchive;
struct RAYTRACE_CONFIG
{
	BOOL bEnableDirect;
	BOOL bMainLightOnly;
	float fShadowHardness;
	int nSampleForTerrain;
	int nSampleForModel;
	float fContantColor;
	float fOrenNayarRho;
	BOOL bEnableAO;
	int nAOSample;
	float fAOEnhance;
	bool bAOAllDir;
	float fAODist;
	float fAOStart;
	BOOL bEnableIndirect;
	int nEmitPhoton;
	int nMaxBounce;
	int nGatherSample;
	float fMaxCacheErr;
	bool bSkipBright;
	float fDirectScale;
	float fIndirectScale;
	float fAOScale;
	float fGamma;
	int iDayNight;
	int iDefautSize;
	BOOL bSelectOnly;
	BOOL bGenTerrain;
	BOOL bReloadMap;
	BOOL bOnlyTerrain;
	int iStartObjIdx;
	int iEndObjIdx;
	BOOL bHDREncode;
	float fMaxVolume;
	float fMaxSize;
	BOOL bBlockRender;
	float fBlockSize;
	BOOL bVertLitOnly;
};

struct RTExportParamsDesc;

/////////////////////////////////////////////////////////////////////////////
// CRayTraceDlg dialog

class CRayTraceDlg : public CDialog
{
private:
	CElementMap* m_pCurMap;
	RAYTRACE_CONFIG* m_pConfig;
	bool m_bError;
	BOOL ExportXMLForSingleMap(RTExportParamsDesc* pParams);
	BOOL ExportXMLFor4Maps(int index, CString strMaps[4], bool bGenMap[4]);
	BOOL ExportXML_Camera(FILE* fp, A3DCameraBase* pCamera);
	BOOL ExportXML_Solvers(FILE* fp);
	BOOL ExportXML_Models(FILE* fp, RTExportParamsDesc* pParams, const A3DVECTOR3& vOffset);
	BOOL ExportXML_MirrorPlanes(FILE* fp, RTExportParamsDesc* pParams, const A3DVECTOR3& vOffset);
	BOOL ExportXML_MainLights(FILE* fp, RTExportParamsDesc* pParams);
	BOOL ExportXML_Lights(FILE* fp, RTExportParamsDesc* pParams, const A3DVECTOR3& vOffset);
	BOOL ExportXML_Terrain(FILE* fp, RTExportParamsDesc* pParams, const A3DVECTOR3& vOffset);


	void ConfigToControl(RAYTRACE_CONFIG* pConfig);
	void ControlToConfig(RAYTRACE_CONFIG* pConfig);
	BOOL PrepareCacheDir(RTExportParamsDesc* pParams);
	BOOL CreateLitmapSceneCacheDir(RTExportParamsDesc* pParams);
	BOOL ExportHeightMap(RTExportParamsDesc* pParams);
	BOOL ExportTerrainDiffuse(RTExportParamsDesc* pParams);
	BOOL ExportCache(RTExportParamsDesc* pParams);
	BOOL ExportModelCache(RTExportParamsDesc* pParams);
	BOOL ExportMirrorPlaneCache(RTExportParamsDesc* pParams);
	BOOL ExportBAT();
	BOOL GenLightMap(RTExportParamsDesc* pParams);
	int GetObjCount(CElementMap* pMap);
	void ComputeLightMapSize(CElementMap* pMap);

	// Construction
public:
	CRayTraceDlg(CElementMap* pMap);   // standard constructor

	BOOL ExportAll(bool bUpdateConfig = true);
	static AString CheckLightMapResources(CElementMap* pMap);
	static RAYTRACE_CONFIG* CreateDefaultConfig();
	static bool LoadConfig(CELArchive& ar, RAYTRACE_CONFIG* pConfig);
	static bool SaveConfig(CELArchive& ar, const RAYTRACE_CONFIG* pConfig);

	CString m_strXMLFile;
	CString m_strToolPath;
	CString m_strToolFile;
	std::vector<CString> m_lXmlFiles;

	// Dialog Data
	//{{AFX_DATA(CRayTraceDlg)
	enum { IDD = IDD_DIALOG_RAYTRACE };
	CButton	m_chkNight;
	CButton	m_chkDay;
	CComboBox	m_cmbAOSample;
	CButton	m_chkEnableAO;
	CComboBox	m_cmbAOEnhance;
	CComboBox	m_cmbAODist;
	CSliderCtrl	m_sldAOScale;
	CComboBox	m_cmbTerrainSample;
	CSliderCtrl	m_sldHardness;
	CButton	m_rdDay;
	CComboBox	m_cmbModelSample;
	CSliderCtrl	m_sldIndirectScale;
	CSliderCtrl	m_sldGamma;
	CEdit	m_edtEmitPhoton;
	CEdit	m_edtTool;
	CSliderCtrl	m_sldDirectScale;
	CComboBox	m_cmbGatherSample;
	CComboBox	m_cmbBounce;
	CButton	m_chkSkipBright;
	CButton	m_chkEnableIndirect;
	CButton	m_chkSelOnly;
	CButton	m_chkEnableDirect;
	CButton m_chkGenTerrain;
	CButton m_chkReloadMap;
	CSliderCtrl	m_sldConstantColor;
	CEdit	m_edtStartObj;
	CEdit	m_edtEndObj;
	CButton	m_chkAOAllDir;
	CComboBox m_cmbMaxErr;
	CButton m_btnExport;
	CButton	m_chkHDR;
	CButton	m_chkMainLightOnly;
	CButton	m_chkOnlyTerrain;
	CButton	m_btnOK;
	CComboBox m_cmbDefautSize;
	CEdit m_edtMaxVolume;
	CSliderCtrl	m_sldOrenNayarRho;

	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRayTraceDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRayTraceDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnChkDirectlight();
	afx_msg void OnChkIndirectlight();
	afx_msg void OnChkAo();
	afx_msg void OnBrowse();
	afx_msg void OnExport();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonCalcSize();
	CButton m_btnCalcSize;
	CEdit m_edtSizeLimit;
	CEdit m_edtSizeLitmitPassword;
	afx_msg void OnBnClickedButton2();
	CSliderCtrl m_sldAOStart;
	afx_msg void OnBnClickedCheckBlock();
	CButton m_chkBlock;
	CComboBox m_edtBlockSize;
	CButton m_chkVertLitOnly;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RAYTRACEDLG_H__69663C31_0571_4D84_B5AA_6AA0D61C3259__INCLUDED_)
