// Filename	: DlgSnSInfo.h
// Creator	: Fu Chonggang
// Date		: 2009/02/15

#pragma once

#include "DlgBase.h"
#include "DlgFriendSeeking.h"
#include "AUI\\AUIComboBox.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUICheckBox.h"
#include "AUI\\AUIEditBox.h"

class A2DSprite;

class CDlgSnsInfo : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
		
public:
	CDlgSnsInfo();
	virtual ~CDlgSnsInfo();

	void OnCommand_ReturnBack(const char * szCommand);
	void OnCommand_ApplyInfo(const char * szCommand);
	void OnCommand_Province(const char * szCommand);
	void OnCommand_Calender(const char * szCommand);
	void OnCommand_Cancel(const char * szCommand);
	void OnCommand_RequestFriend(const char * szCommand);
	
	void OnEventLButtonDown_Img_Photo(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	void SetInfo(CDlgFriendSeek::PlayerInfoCache info,bool bShow);
	void SetSelfInfo(CDlgFriendSeek::PlayerInfoCache info,bool bShow);
	void SetBirth(int iyear, int iMonth, int iDay);
	void SetPhoto(char *szPath);

	typedef abase::vector<ACString> CityVector;
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	virtual bool Release();

	void Lock();
	void UnLock();

	typedef abase::hash_map<int, AString> ID2PhotoPathMap;

protected:
	HRESULT GetPostData(LPVARIANT   pvPostData,LPCTSTR   szPostData,   DWORD   postLength);
	int SendHTTPByIE(LPCSTR   URL,LPCSTR   headers,const   BYTE   *Post,   DWORD   postLength);
	
protected:
	static A2DSprite* m_pA2DSpriteBoy;
	static A2DSprite* m_pA2DSpriteGirl;
	abase::vector<CityVector> m_vecvecCity;
	CityVector	m_vecProvince;
	int m_iBirth;
	int m_iIdTarget;
	PAUILABEL m_pTxt_Name;
	PAUILABEL m_pTxt_Level;
	PAUILABEL m_pTxt_Gender;
	PAUILABEL m_pTxt_Faction;
	PAUILABEL m_pTxt_Teacher;
	PAUILABEL m_pTxt_Guild;
	PAUILABEL m_pTxt_Family;
	PAUILABEL m_pTxt_Marriage;

	PAUIEDITBOX m_pEdt_NickName;
	PAUIEDITBOX m_pEdt_Age;
	PAUIEDITBOX m_pEdt_City;
	PAUIEDITBOX m_pEdt_Profession;
	PAUIEDITBOX m_pEdt_Province;
	PAUIEDITBOX	m_pEdt_Birth;
	PAUIEDITBOX m_pEdt_Gender;

	PAUILABEL m_pLab_NickName;
	PAUILABEL m_pLab_Age;
	PAUILABEL m_pLab_City;
	PAUILABEL m_pLab_Profession;
	PAUILABEL m_pLab_Province;
	PAUILABEL m_pLab_Birth;
	PAUILABEL m_pLab_Gender;
	PAUILABEL m_pLab_Address;
	
	PAUICOMBOBOX m_pCombo_Province;
	PAUICOMBOBOX m_pCombo_City;
	PAUIEDITBOX m_pEdt_Address;
	PAUIEDITBOX m_pEdt_SelfIntro;
	PAUITEXTAREA m_pTxt_SelfIntro;
	PAUICHECKBOX m_pChk_ShowToFriendOnly;
	PAUIIMAGEPICTURE m_pPic_Photo;

	bool m_bSet;
	static ID2PhotoPathMap m_PhotoPathMap;
	CRITICAL_SECTION	m_csPhoto;
};
