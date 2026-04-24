// Filename	: DlgSnSInfo.cpp
// Creator	: Fu Chonggang
// Date		: 2009/02/15

#include "AFI.h"

#include "DlgSnSInfo.h"
#include "AWScriptFile.h"
#include "A2DSprite.h"

#include "snsgetplayerinfo_re.hpp"

#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_Faction.h"
#include "EC_Friend.h"
#include "elementdataman.h"
#include "EC_Inventory.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_SendC2SCmds.h"
#include "EC_PlayerImage.h"
#include "EC_UIManager.h"
#include "EC_Configs.h"

#include "atlbase.h" 
#include "exdisp.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgSnsInfo, CDlgBase)

AUI_ON_COMMAND("back",				OnCommand_ReturnBack)
AUI_ON_COMMAND("apply",				OnCommand_ApplyInfo)
AUI_ON_COMMAND("province",			OnCommand_Province)
AUI_ON_COMMAND("calendar",			OnCommand_Calender)
AUI_ON_COMMAND("friend",			OnCommand_RequestFriend)
AUI_ON_COMMAND("IDCANCEL",			OnCommand_Cancel)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgSnsInfo, CDlgBase)

AUI_ON_EVENT("Img_Photo",	WM_LBUTTONDOWN,		OnEventLButtonDown_Img_Photo)

AUI_END_EVENT_MAP()

A2DSprite* CDlgSnsInfo::m_pA2DSpriteBoy = NULL;
A2DSprite* CDlgSnsInfo::m_pA2DSpriteGirl = NULL;
typedef abase :: hash_map<int, AString> ID2PhotoPathMap;
ID2PhotoPathMap CDlgSnsInfo::m_PhotoPathMap;

const char *szPhotoUpLoadUrl = "http://photo.zhuxian.wanmei.com/jsp/zxphoto/index.jsp";

CDlgSnsInfo::CDlgSnsInfo()
{
}

CDlgSnsInfo::~CDlgSnsInfo()
{
}

bool CDlgSnsInfo::OnInitDialog()
{
	bool bval;
	InitializeCriticalSection(&m_csPhoto);
	if( !m_pA2DSpriteBoy )
	{
		m_pA2DSpriteBoy = new A2DSprite;
		if( !m_pA2DSpriteBoy ) return AUI_ReportError(__LINE__, __FILE__);

		m_pA2DSpriteBoy->SetDynamicTex(true);
		bval = m_pA2DSpriteBoy->Init(m_pA3DDevice, "InGame\\…Ÿœ¿ƒ–.dds", 0);
		if( !bval ) return AUI_ReportError(__LINE__, __FILE__);
	}
	if( !m_pA2DSpriteGirl )
	{
		m_pA2DSpriteGirl = new A2DSprite;
		if( !m_pA2DSpriteGirl ) return AUI_ReportError(__LINE__, __FILE__);

		m_pA2DSpriteGirl->SetDynamicTex(true);
		bval = m_pA2DSpriteGirl->Init(m_pA3DDevice, "InGame\\…Ÿœ¿≈Æ.dds", 0);
		if( !bval ) return AUI_ReportError(__LINE__, __FILE__);
	}
	AWScriptFile s;
	if( s.Open("configs\\province.txt") )
	{
		int i = 0, j;
		while( s.PeekNextToken(true) )
		{
			int n = s.GetNextTokenAsInt(true);
			s.GetNextToken(true);
			ACString strName = s.m_szToken;
			m_vecProvince.push_back(strName);
			CityVector info;
			for(j = 0; j < n; j++ )
			{
				int n = s.GetNextTokenAsInt(true);
				s.GetNextToken(true);
				strName = s.m_szToken;
				info.push_back(strName);
			}
			m_vecvecCity.push_back(info);
		}
		s.Close();
	}
	else
		AUI_ReportError(__LINE__, 1,"CDlgRankList::OnInitDialog(), taptable.txt error!");
	if( m_szName == "Win_PersInfo" )
	{
		m_pTxt_Name			= (PAUILABEL)GetDlgItem("Txt_Name");
		m_pTxt_Level		= (PAUILABEL)GetDlgItem("Txt_Lv");
		m_pTxt_Gender		= (PAUILABEL)GetDlgItem("Txt_Gender");
		m_pTxt_Faction		= (PAUILABEL)GetDlgItem("Txt_Desc");
		m_pTxt_Teacher		= (PAUILABEL)GetDlgItem("Txt_Teacher");
		m_pTxt_Guild		= (PAUILABEL)GetDlgItem("Txt_Guild");
		m_pTxt_Family		= (PAUILABEL)GetDlgItem("Txt_Family");
		m_pTxt_Marriage		= (PAUILABEL)GetDlgItem("Txt_MarriSta");
	
		m_pEdt_NickName		= (PAUIEDITBOX)GetDlgItem("Edit_NickName");
		m_pEdt_Age			= (PAUIEDITBOX)GetDlgItem("Edit_Age");
		if (m_pEdt_Age)
		{
			m_pEdt_Age->SetIsNumberOnly(true);
		}
		m_pEdt_Profession	= (PAUIEDITBOX)GetDlgItem("Edit_Job");
		m_pEdt_Birth		= (PAUIEDITBOX)GetDlgItem("Edit_Birth");
		if (m_pEdt_Birth)
		{
			m_pEdt_Birth->SetAcceptMouseMessage(false);
		}
		m_pCombo_City		= (PAUICOMBOBOX)GetDlgItem("Combo_City");
		m_pCombo_Province	= (PAUICOMBOBOX)GetDlgItem("Combo_Province");
		m_pEdt_Address		= (PAUIEDITBOX)GetDlgItem("Edit_Contact");
		m_pEdt_SelfIntro	= (PAUIEDITBOX)GetDlgItem("Edt_SelfIntro");
		m_pPic_Photo		= (PAUIIMAGEPICTURE)GetDlgItem("Img_Photo");
//		if (m_pPic_Photo)
//		{
//			m_pPic_Photo->SetCover(m_pA2DSpriteGirl,0);
//		}
		if (m_pEdt_SelfIntro)
		{
			m_pEdt_SelfIntro->SetIsAutoReturn(true);
		}
		m_pChk_ShowToFriendOnly = (PAUICHECKBOX)GetDlgItem("Chk_Read"); 

		if (m_pCombo_Province)
		{
			for (unsigned int i=0; i<m_vecProvince.size(); i++)
			{
				m_pCombo_Province->AddString(m_vecProvince[i]);
			}
		}
		
		if (m_pCombo_City)
		{
			for (unsigned int i=0; i<(m_vecvecCity[0]).size(); i++)
			{
				m_pCombo_City->AddString(m_vecvecCity[0][i]);
			}
		}
		m_iIdTarget = GetHostPlayer()->GetCharacterID();
	}
	else if( m_szName == "Win_PersInfoCheck" )
	{
		m_iIdTarget			= 0;
		m_pTxt_Name			= (PAUILABEL)GetDlgItem("Txt_Name");
		m_pTxt_Level		= (PAUILABEL)GetDlgItem("Txt_Lv");
		m_pTxt_Gender		= (PAUILABEL)GetDlgItem("Txt_Gender");
		m_pTxt_Faction		= (PAUILABEL)GetDlgItem("Txt_Desc");
		m_pTxt_Teacher		= (PAUILABEL)GetDlgItem("Txt_Teacher");
		m_pTxt_Guild		= (PAUILABEL)GetDlgItem("Txt_Guild");
		m_pTxt_Family		= (PAUILABEL)GetDlgItem("Txt_Family");
		m_pTxt_Marriage		= (PAUILABEL)GetDlgItem("Txt_MarriSta");
	
		m_pLab_NickName		= (PAUILABEL)GetDlgItem("Txt_NickName");
		m_pLab_Age			= (PAUILABEL)GetDlgItem("Txt_Age");
		m_pLab_City			= (PAUILABEL)GetDlgItem("Txt_City");
		m_pLab_Profession	= (PAUILABEL)GetDlgItem("Txt_Job");
		m_pLab_Birth		= (PAUILABEL)GetDlgItem("Txt_Birth");
		m_pLab_Province		= (PAUILABEL)GetDlgItem("Txt_Province");
		m_pLab_Address		= (PAUILABEL)GetDlgItem("Txt_Contact");
		m_pTxt_SelfIntro	= (PAUITEXTAREA)GetDlgItem("Txt_SelfIntro");
		m_pPic_Photo		= (PAUIIMAGEPICTURE)GetDlgItem("Img_Photo");
//		if (m_pPic_Photo)
//		{
//			m_pPic_Photo->SetCover(m_pA2DSpriteGirl,0);
//		}
	}
	m_iBirth = 0;
	return true;
}

bool CDlgSnsInfo::Release()
{
	A3DRELEASE(m_pA2DSpriteBoy)
	A3DRELEASE(m_pA2DSpriteGirl)

	DeleteCriticalSection(&m_csPhoto);

	return CDlgBase::Release();
}

void CDlgSnsInfo::OnShowDialog()
{
	if( m_szName == "Win_PersInfo")
	{
		ACHAR szText[20];
		CDlgFriendSeek::MsgID2PlayerInfoMap::iterator iter = CDlgFriendSeek::m_id2PlayerInfoCache.find(GetHostPlayer()->GetCharacterID());
		if (iter != CDlgFriendSeek::m_id2PlayerInfoCache.end())
		{
			CDlgFriendSeek::PlayerInfoCache playerInfoCache = CDlgFriendSeek::m_id2PlayerInfoCache[GetHostPlayer()->GetCharacterID()];
			m_pEdt_NickName->SetText(playerInfoCache.playerinfo.nickname);
			m_pEdt_Profession->SetText(playerInfoCache.playerinfo.career);
			m_pEdt_Address->SetText(playerInfoCache.playerinfo.address);
			m_pEdt_SelfIntro->SetText(playerInfoCache.playerinfo.introduce);
			m_pCombo_Province->SetCurSel(playerInfoCache.playerinfo.province);
			m_pCombo_City->ResetContent();
			int prov = playerInfoCache.playerinfo.province;
			if (prov<0 || prov>1000)
			{
				prov = 0;
			}
			for (unsigned int i=0; i<(m_vecvecCity[prov]).size(); i++)
			{
				m_pCombo_City->AddString(m_vecvecCity[prov][i]);
			}
			m_pCombo_City->SetCurSel(playerInfoCache.playerinfo.city);
			a_sprintf(szText,_AL("%d-%d-%d"),(playerInfoCache.playerinfo.birthday&0x8FFF0000)>>16,
				(playerInfoCache.playerinfo.birthday&0x0000FF00)>>8,
				(playerInfoCache.playerinfo.birthday&0x000000FF));
			m_pEdt_Birth->SetText(szText);
			m_iBirth = playerInfoCache.playerinfo.birthday;
			m_pChk_ShowToFriendOnly->Check(playerInfoCache.playerinfo.is_private>0);
		}
		else
		{
			GetGameSession()->sns_get_playerinfo(0, GetHostPlayer()->GetCharacterID());
		}
	}
	m_bSet = false;
	AString strImagePath;
	if ( g_pGame->GetConfigs()->IsSnsPhotoOpen() &&
		CECPlayerImage::PI_AVAILABLE == g_pGame->GetGameRun()->GetUIManager()->GetPlayerImage()->GetPlayerImage(GetGameSession()->GetDistrictID(),
		m_iIdTarget, strImagePath))
	{
		AUIOBJECT_SETPROPERTY p;
		strcpy(p.fn, strImagePath);
		m_pPic_Photo->SetProperty("Image File", &p);
		m_bSet = true;
	}
	else
	{
		AUIOBJECT_SETPROPERTY p;
		strcpy(p.fn, "");
		m_pPic_Photo->SetProperty("Image File", &p);
	}
}

void CDlgSnsInfo::OnTick()
{
	CECHostPlayer *pHost = GetHostPlayer();
	const ROLEEXTPROP& ep = pHost->GetExtendProps();
	const ROLEBASICPROP& bp = pHost->GetBasicProps();
	if( m_szName == "Win_PersInfo")
	{
		ACHAR szText[20];
		ACString strText;
		m_pTxt_Name->SetText(pHost->GetName());
		a_sprintf(szText,_AL("%d"),bp.iLevel);
		strText = szText;
		if (GetHostPlayer()->GetRebornInfo().size()>0)
		{
			strText += _AL(" ");
			strText += GetStringFromTable(7012);
			strText += _AL(" ");
			strText += GetStringFromTable(7000 +GetHostPlayer()->GetCultivation());
		}
		m_pTxt_Level->SetText(strText);
		m_pTxt_Gender->SetText(GetStringFromTable(pHost->GetGender() ? 7142 : 7141));
		m_pTxt_Faction->SetText(GetStringFromTable(PROFESSION_START + pHost->GetProfession()));

		int idTeacher = pHost->GetMaster();
		if (idTeacher>0 && idTeacher != pHost->GetCharacterID())
		{
			ACString szName;
			szName = GetGame()->GetGameRun()->GetPlayerName(idTeacher, false);
			if (szName.IsEmpty())
			{
				static DWORD timeLast = 0;
				if( timeLast + 1000 < timeGetTime() )
				{
					g_pGame->GetGameSession()->GetPlayerBriefInfo(1, &idTeacher, 2);
					timeLast = timeGetTime();
				}
				
			}
			m_pTxt_Teacher->SetText(szName);
		}
		else
			m_pTxt_Teacher->SetText(GetStringFromTable(7199));

		CECFactionMan* pFMan = GetGame()->GetFactionMan();
		Faction_Info *pInfo;
		int idFaction = pHost->GetFactionID();
		if (idFaction>0)
		{
			pInfo = pFMan->GetFaction(idFaction);
			if( pInfo )
			{
				m_pTxt_Guild->SetText(pInfo->GetName());
			}
		}
		else
			m_pTxt_Guild->SetText(GetStringFromTable(7199));
		int idFamily = pHost->GetFamilyID();
		if (idFamily>0)
		{
			pInfo = pFMan->GetFaction(idFamily, 0 ,true);
			if( pInfo )
			{
				m_pTxt_Family->SetText(pInfo->GetName());
			}
		}
		else
			m_pTxt_Family->SetText(GetStringFromTable(7199));
	}
	AUIOBJECT_SETPROPERTY p;
	if ( !m_bSet )
	{
		Lock();
		ID2PhotoPathMap::iterator iter = m_PhotoPathMap.find(m_iIdTarget);
		if (iter!=m_PhotoPathMap.end())
		{
			AString strPath = m_PhotoPathMap[m_iIdTarget];
			strcpy(p.fn, strPath);
			m_pPic_Photo->SetProperty("Image File", &p);
			m_bSet = true;
		}
		UnLock();
	}
}

void CDlgSnsInfo::OnCommand_ReturnBack(const char * szCommand)
{
	Show(false);
	GetGameUIMan()->GetDialog("Win_Character")->Show(true);
}

void CDlgSnsInfo::OnCommand_ApplyInfo(const char * szCommand)
{
	int iProvience = m_pCombo_Province->GetCurSel();
	int iCity = m_pCombo_City->GetCurSel();
	ACString strNickName =  m_pEdt_NickName->GetText();
	ACString strCareer =  m_pEdt_Profession->GetText();
	ACString strAddress =  m_pEdt_Address->GetText();
	ACString strIntroduce = m_pEdt_SelfIntro->GetText();
	short iAge = a_atoi(m_pEdt_Age->GetText());
	ACString strTemp;
	ACHAR szText[512];

	strTemp = strNickName;
	GetGameUIMan()->FilterBadWords(strTemp);
	if (strTemp != strNickName)
	{
		GetGameUIMan()->MessageBox("",GetStringFromTable(8176), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	AUI_ConvertChatString(strTemp, szText, false);
	strNickName = szText;

	strTemp = strCareer;
	GetGameUIMan()->FilterBadWords(strTemp);
	if (strTemp != strCareer)
	{
		GetGameUIMan()->MessageBox("",GetStringFromTable(8176), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	AUI_ConvertChatString(strTemp, szText, false);
	strCareer = szText;

	strTemp = strAddress;
	GetGameUIMan()->FilterBadWords(strTemp);
	if (strTemp != strAddress)
	{
		GetGameUIMan()->MessageBox("",GetStringFromTable(8176), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	AUI_ConvertChatString(strTemp, szText, false);
	strAddress = szText;

	strTemp = strIntroduce;
	GetGameUIMan()->FilterBadWords(strTemp);
	if (strTemp != strIntroduce)
	{
		GetGameUIMan()->MessageBox("",GetStringFromTable(8176), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	AUI_ConvertChatString(strTemp, szText, false);
	strIntroduce = szText;


	CDlgFriendSeek *pFriendSeekMsg = (CDlgFriendSeek*)GetGameUIMan()->GetDialog("Win_FriendSeekingMsg");
	pFriendSeekMsg->SetPlayerInfo(m_pChk_ShowToFriendOnly->IsChecked(), strNickName, m_iBirth, iProvience,
		iCity, strCareer, strAddress, strIntroduce);
	GetGameSession()->sns_set_playerinfo(m_pChk_ShowToFriendOnly->IsChecked(), strNickName, iAge, m_iBirth, iProvience,
		iCity, strCareer, strAddress, strIntroduce);

}

void CDlgSnsInfo::OnCommand_Cancel(const char * szCommand)
{
	Show(false);
}

void CDlgSnsInfo::OnCommand_RequestFriend(const char * szCommand)
{
	if (m_iIdTarget<=0) 
	{
		return;
	}
	const ACHAR *pszName = GetGameRun()->GetPlayerName(m_iIdTarget, true);
	CECFriendMan::FRIEND* pFriend = GetHostPlayer()->GetFriendMan()->GetFriendByID(m_iIdTarget);

	//	In friend list, one player ID may have two names, so check name here !
	if( pFriend && !a_strcmp(pszName, pFriend->GetName()) )
		GetGameUIMan()->AddChatMessage(GetStringFromTable(565), GP_CHAT_MISC);
	else
	{
		GetGameSession()->friend_Add(m_iIdTarget, pszName);
		GetGameUIMan()->AddChatMessage(GetStringFromTable(533), GP_CHAT_MISC);
	}
}

void CDlgSnsInfo::OnCommand_Calender(const char * szCommand)
{
	GetGameUIMan()->GetDialog("Win_Calendar")->Show(true);
}

void CDlgSnsInfo::OnCommand_Province(const char * szCommand)
{
	int iProv = m_pCombo_Province->GetCurSel();
	m_pCombo_City->ResetContent();
	for (unsigned int i=0; i<(m_vecvecCity[iProv]).size(); i++)
	{
		m_pCombo_City->AddString(m_vecvecCity[iProv][i]);
	}
}

int CDlgSnsInfo::SendHTTPByIE(LPCSTR   URL,LPCSTR   headers,const   BYTE   *Post,   DWORD   postLength)
{   
    HRESULT   hr;   
    USES_CONVERSION;   
    
	
    IWebBrowserApp*   pWBApp   =   NULL;   //   Derived   from   IWebBrowser   
    BSTR   bstrURL   =   NULL,   bstrHeaders   =   NULL;   
    VARIANT   vFlags   =   {0},   
		vTargetFrameName   =   {0},   
		vPostData   =   {0},   
		vHeaders   =   {0};   
    __try{     
		if   (FAILED(hr   =   CoInitialize(NULL)))   
		{   
			return   FALSE;   
		}                 
		if   (FAILED(hr   =   CoCreateInstance(CLSID_InternetExplorer,   
			NULL,   
			CLSCTX_SERVER,   
			IID_IWebBrowserApp,   
			(LPVOID*)&pWBApp)))   
		{   
			goto   Error;   
		}     
		bstrURL   =   SysAllocString(A2W(URL));   
		if   (!bstrURL)   
		{   
			goto   Error;   
		}       
		if(headers!=NULL){   
			bstrHeaders   =   SysAllocString(A2W(headers));   
			if   (!bstrHeaders)   
			{   
				goto   Error;   
			}           
			V_VT(&vHeaders)   =   VT_BSTR;   
			V_BSTR(&vHeaders)   =   bstrHeaders;               
		}   
		if(Post!=NULL){   
			hr   =   GetPostData(&vPostData,(LPCTSTR)Post,postLength);   
		}   
		hr   =   pWBApp->Navigate(bstrURL,   &vFlags,   
			&vTargetFrameName,   &vPostData,   &vHeaders);   
		pWBApp->put_Visible(VARIANT_TRUE);     
Error:;   
    }   
	
    __finally{   
		
		if   (bstrURL)   SysFreeString(bstrURL);   
		if   (bstrHeaders)   SysFreeString(bstrHeaders);   
		if(Post!=NULL)VariantClear(&vPostData);   
		if   (pWBApp){   
			pWBApp->Release();   
			pWBApp=NULL;   
		}   
		CoUninitialize();   
    }   
	return   TRUE;   
}   
HRESULT   CDlgSnsInfo::GetPostData(LPVARIANT   pvPostData,LPCTSTR   szPostData,   DWORD   postLength)   
{   
	HRESULT   hr;   
	LPSAFEARRAY   psa;   
	
	LPSTR   pPostData;       
    if   (!pvPostData)   
	{   
		return   E_POINTER;   
	}       
    VariantInit(pvPostData);     
    psa   =   SafeArrayCreateVector(VT_UI1,   0,   postLength);   
	if   (!psa)   
	{   
		return   E_OUTOFMEMORY;   
	}     
    hr   =   SafeArrayAccessData(psa,   (LPVOID*)&pPostData);   
	memcpy(pPostData,   szPostData,   postLength);   
	hr   =   SafeArrayUnaccessData(psa);   
	V_VT(pvPostData)   =   VT_ARRAY   |   VT_UI1;     
	V_ARRAY(pvPostData)   =   psa;   
	return   NOERROR;   
}

void CDlgSnsInfo::OnEventLButtonDown_Img_Photo(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if ( !g_pGame->GetConfigs()->IsSnsPhotoOpen() )
		return;
	if( m_szName == "Win_PersInfo")
	{
		if (GetHostPlayer()->GetRebornInfo().size()<1 && GetHostPlayer()->GetBasicProps().iLevel<60)
		{
			GetGameUIMan()->MessageBox("",GetStringFromTable(7313), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			return;
		}
		char * szurl = "http://photo.zhuxian.wanmei.com/jsp/zxphoto/index.jsp";
//		const char* szpost = "account=99301984&rolename=testt&areaid=6&roleid=12377491";
		char szpost[100];
		ACString strName = GetHostPlayer()->GetName();
		sprintf(szpost,"account=%d&rolename=%s&areaid=%d&roleid=%d",
			GetGameSession()->GetUserID(),
			GetGameUIMan()->URLEncode(strName.GetBuffer(0), CP_ACP),
			GetGameSession()->GetDistrictID(),
			GetHostPlayer()->GetCharacterID());
		AString szPostHeaders0 = "Referer: ";
		AString  szPostHeaders1 = "Content-Type: application/x-www-form-urlencoded";
		
		SendHTTPByIE(szurl, szPostHeaders1, (const unsigned char *)szpost, strlen(szpost));
		SetCapture(false);
		
		//GetGameUIMan()->MessageBox("Game_UploadPhoto",GetStringFromTable(7312), MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160));
	}
}

void CDlgSnsInfo::SetInfo(CDlgFriendSeek::PlayerInfoCache info, bool bShow)
{
	ACString strText;
	ACHAR szText[20];
	m_iIdTarget = info.playerinfo.roleid;
	m_pTxt_Name->SetText(info.playerinfo.rolename);
	a_sprintf(szText,_AL("%d"),info.roleinfo.level);
	strText = szText;
	if (info.roleinfo.sub_level>0)
	{
		strText += _AL(" ");
		strText += GetStringFromTable(7012);
		strText += _AL(" ");
	}
	m_pTxt_Level->SetText(strText);
	m_pTxt_Gender->SetText(GetStringFromTable(info.roleinfo.gender>0 ? 7142 : 7141));
	//m_pPic_Photo->SetCover(info.roleinfo.gender>0 ? m_pA2DSpriteGirl : m_pA2DSpriteBoy,0);
	m_pTxt_Faction->SetText(GetStringFromTable(info.roleinfo.occupation + PROFESSION_START));
	m_pTxt_Teacher->SetText(info.roleinfo.sectname.IsEmpty()? GetStringFromTable(7199):info.roleinfo.sectname);
	m_pTxt_Guild->SetText(info.roleinfo.factionname.IsEmpty()? GetStringFromTable(7199):info.roleinfo.factionname);
	m_pTxt_Family->SetText(info.roleinfo.familyname.IsEmpty()? GetStringFromTable(7199):info.roleinfo.familyname);
	m_pTxt_Marriage->SetText(GetStringFromTable(info.roleinfo.spouse>0? 7160:7161));

	m_pLab_NickName->SetText(info.playerinfo.nickname);
	UINT idProvince = info.playerinfo.province;
	if (idProvince<0)
		idProvince = 0;
	else if(idProvince>=m_vecProvince.size())
		idProvince = m_vecProvince.size()-1;
	m_pLab_Province->SetText(m_vecProvince[idProvince] );
	a_sprintf(szText,_AL("%d"),info.playerinfo.age);
	m_pLab_Age->SetText(szText);
	UINT idCity = info.playerinfo.city;
	if (idCity<0)
		idCity = 0;
	else if(idCity>=m_vecvecCity[idProvince].size())
		idCity = m_vecvecCity[idProvince].size()-1;
	m_pLab_City->SetText(m_vecvecCity[idProvince][idCity]);
	m_pLab_Profession->SetText(info.playerinfo.career);
	a_sprintf(szText,_AL("%d-%d-%d"),(info.playerinfo.birthday&0x8FFF0000)>>16,
		(info.playerinfo.birthday&0x0000FF00)>>8,
		(info.playerinfo.birthday&0x000000FF));
	m_pLab_Birth->SetText(szText);
	a_sprintf(szText,_AL("%d"),info.playerinfo.age);
	m_pLab_Age->SetText(szText);
	m_pLab_Address->SetText(info.playerinfo.address);
	m_pTxt_SelfIntro->SetText(info.playerinfo.introduce);
	Show(bShow);
}

void CDlgSnsInfo::SetSelfInfo(CDlgFriendSeek::PlayerInfoCache info, bool bShow)
{
	ACString strText;
	ACHAR szText[20];
	m_iIdTarget = GetHostPlayer()->GetCharacterID();
	m_pTxt_Name->SetText(info.playerinfo.rolename);
	a_sprintf(szText,_AL("%d"),info.roleinfo.level);
	strText = szText;
	if (info.roleinfo.sub_level>0)
	{
		strText += _AL(" ");
		strText += GetStringFromTable(7012);
		strText += _AL(" ");
		strText += GetStringFromTable(7000 +GetHostPlayer()->GetCultivation());
	}
	m_pTxt_Level->SetText(strText);
	m_pTxt_Gender->SetText(GetStringFromTable(info.roleinfo.gender>0 ? 7142 : 7141));
	//m_pPic_Photo->SetCover(info.roleinfo.gender>0 ? m_pA2DSpriteGirl : m_pA2DSpriteBoy,0);
	m_pTxt_Faction->SetText(GetStringFromTable(info.roleinfo.occupation + PROFESSION_START));
	m_pTxt_Teacher->SetText(info.roleinfo.sectname.IsEmpty()? GetStringFromTable(7199):info.roleinfo.sectname);
	m_pTxt_Guild->SetText(info.roleinfo.factionname.IsEmpty()? GetStringFromTable(7199):info.roleinfo.factionname);
	m_pTxt_Family->SetText(info.roleinfo.familyname.IsEmpty()? GetStringFromTable(7199):info.roleinfo.familyname);
	m_pTxt_Marriage->SetText(GetStringFromTable(info.roleinfo.spouse>0? 7160:7161));

	m_pEdt_NickName->SetText(info.playerinfo.nickname);
	m_pCombo_Province->SetCurSel(info.playerinfo.province);
	if (m_pCombo_City && info.playerinfo.province>0 && info.playerinfo.province<(int)m_vecvecCity.size())
	{
		m_pCombo_City->ResetContent();
		for (unsigned int i=0; i<(m_vecvecCity[info.playerinfo.province]).size(); i++)
		{
			m_pCombo_City->AddString(m_vecvecCity[info.playerinfo.province][i]);
		}
	}
	m_pCombo_City->SetCurSel(info.playerinfo.city);
	m_pEdt_Profession->SetText(info.playerinfo.career);
	a_sprintf(szText,_AL("%d-%d-%d"),(info.playerinfo.birthday&0x8FFF0000)>>16,
		(info.playerinfo.birthday&0x0000FF00)>>8,
		(info.playerinfo.birthday&0x000000FF));
	m_pEdt_Birth->SetText(szText);
	a_sprintf(szText,_AL("%d"),info.playerinfo.age);
	m_pEdt_Age->SetText(szText);
	m_pEdt_Address->SetText(info.playerinfo.address);
	m_pEdt_SelfIntro->SetText(info.playerinfo.introduce);
}

void CDlgSnsInfo::SetBirth(int iyear, int iMonth, int iDay)
{
	m_iBirth = (iyear<<16&0x8FFF0000) + (iMonth<<8&0x0000FF00) + (iDay&0x000000FF);
	ACHAR szText[20];
	a_sprintf(szText,_AL("%d.%d.%d"),iyear,iMonth,iDay);
	m_pEdt_Birth->SetText(szText);
}

void CDlgSnsInfo::SetPhoto(char *szPath)
{
	Lock();
	AString strPath(szPath);
	AString strFullPath;
	af_GetFullPath(strFullPath, szPath);
	int idStart = strPath.Find('-');
	if (idStart>0)
	{
		strPath.CutLeft(idStart+1);
		int idEnd = strPath.Find('.');
		if (idEnd)
		{
			strPath.CutRight(strPath.GetLength()-idEnd);
			int id = strPath.ToInt();
			m_PhotoPathMap[id] = strFullPath;
		}
	}
	UnLock();
}

void CDlgSnsInfo::Lock()
{
	EnterCriticalSection(&m_csPhoto);
}


void CDlgSnsInfo::UnLock()
{
	LeaveCriticalSection(&m_csPhoto);
}

