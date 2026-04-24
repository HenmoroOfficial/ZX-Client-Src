// Filename	: Dlgm_pAchivItem.cpp
// Creator	: Fu Chonggang
// Date		: 2010/1/27

#include "AUI\\AUISubDialog.h"
#include "AUI\\AUICTranslate.h"
#include "AUI\\AUIDef.h"
#include "AUI\\AStringWithWildcard.h"
#include "AFI.h"
#include "AFileImage.h"
#include "DlgAchivMain.h"
#include "DlgAchivItem.h"
#include "DlgAchivItemMan.h"
#include "DlgBaseExplorer.h"
#include "DlgChat.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_IvtrConsume.h"
#include "EC_HostPlayer.h"
#include "EC_GameUIMan.h"
#include "EC_Inventory.h"
#include "EC_Achievement.h"
#include "EC_ManPlayer.h"
#include "EC_World.h"
#include "EC_GameSession.h"
#include "time.h"

#define new A_DEBUG_NEW

#define BTN_GET_AWARD "Btn_Get_Award"
#define BTN_OBTAIN_AWARD "Btn_Obtain_Award"
#define BTN_SINA "Btn_SinaW"
#define BTN_TENCENT "Btn_TxW"
#define BTN_LINK "Btn_Link"
#define IMG_AWARD "Img_Award"

AUI_BEGIN_COMMAND_MAP(CDlgAchivItem, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",			OnCommandCancel)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgAchivItem, CDlgBase)

AUI_ON_EVENT("*",					WM_LBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("NULL",				WM_LBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("*",					WM_MOUSEWHEEL,	OnEventMouseWheel)
AUI_ON_EVENT(NULL,					WM_MOUSEWHEEL,	OnEventMouseWheel)

AUI_END_EVENT_MAP()

extern const ACHAR* szFinishColor;
extern const ACHAR* szWhiteColor;

CDlgAchivItem::CDlgAchivItem()
{
}

CDlgAchivItem::~CDlgAchivItem()
{
}

bool CDlgAchivItem::OnInitDialog()
{
	m_pLab_Name = (PAUILABEL)GetDlgItem("Title");
	m_pLab_Desc = (PAUILABEL)GetDlgItem("Lab_Description");
	m_pLab_Point = (PAUILABEL)GetDlgItem("Lab_Point");
	m_pLab_Time = (PAUILABEL)GetDlgItem("Lab_Time");
	m_pImg_Title = (PAUIIMAGEPICTURE)GetDlgItem("Img_Title");
	m_pImg_Metal = (PAUIIMAGEPICTURE)GetDlgItem("Img_Achievement");
	m_pImg_Point = (PAUIIMAGEPICTURE)GetDlgItem("Img_Point");
	m_pEdt_Back = (PAUIEDITBOX)GetDlgItem("Edt_Back");
	m_pAchiv	= NULL;
	m_pDlgAchivitemMan = (CDlgAchivItemMan*)GetGameUIMan()->GetDialog("Win_Achievement_Sub");

	return true;
}

bool CDlgAchivItem::Release()
{
	if(m_pAchiv)
	{
		SAFE_DELETE(m_pAchiv);
	}
	return CDlgBase::Release();
}

void CDlgAchivItem::OnCommandCancel(const char* szCommand)
{
	CDlgAchivMain* pDlgAchhivMain = (CDlgAchivMain*)GetGameUIMan()->GetDialog("Win_Achievement");
	pDlgAchhivMain->OnCommand_CANCEL(szCommand);
}

void CDlgAchivItem::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if (IsMatch(GetName(), "Win_Achievement_Done_Other_*", false))
		return;

	if( stricmp(pObj->GetName(), BTN_LINK) == 0 || AUI_PRESS(VK_CONTROL) || AUI_PRESS(VK_SHIFT) )
	{
		if (m_pAchiv)
		{
			PAUIDIALOG pChat = GetGameUIMan()->m_pDlgChat;
			PAUIEDITBOX pEdit = (PAUIEDITBOX)pChat->GetDlgItem("DEFAULT_Txt_Speech");

			ACString strName;
			A3DCOLOR color;


			CDlgChat::TransformAchievementColorName(m_pAchiv, strName, color);

			if (GetGameUIMan()->GetDialog("Win_ChatSpec")->IsShow())
			{
				pChat = GetGameUIMan()->GetDialog("Win_ChatSpec");
				pEdit = (PAUIEDITBOX)pChat->GetDlgItem("Txt_Content");
			}

			if( pEdit->GetItemCount(enumEIIvtrlItem) < 1 && pEdit->GetItemCount(enumEITask) < 1 )
				pEdit->AppendItem(enumEITask, color, strName, L"", &m_pAchiv->id, sizeof(unsigned int));

			GetGameUIMan()->BringWindowToTop(pChat);
			pChat->ChangeFocus(pEdit);
		}
		return;
	}

	if (stricmp(pObj->GetName(), BTN_GET_AWARD) == 0)
	{
		g_pGame->GetGameSession()->c2s_CmdGetAchievementAward(m_pAchiv->id, m_pAchiv->awardItem);
		return;
	}
	else if (stricmp(pObj->GetName(), BTN_SINA) == 0)
	{
		AString url, content;
		content.Format(WC2AS(GetGameUIMan()->GetStringFromTable(20123)), WC2AS(m_pAchiv->strDesc));
		url.Format("http://service.weibo.com/share/share.php?title=%s", content);

		CDlgBaseExplorer* pDlgExplorer = (CDlgBaseExplorer*)GetGameUIMan()->GetDialog("win_explorer");
		pDlgExplorer->Show(true);
		pDlgExplorer->NavigateUrl(url);
	}
	else if (stricmp(pObj->GetName(), BTN_TENCENT) == 0)
	{
		AString url, content;
		content.Format(WC2AS(GetGameUIMan()->GetStringFromTable(20123)), WC2AS(m_pAchiv->strDesc));
		url.Format("http://v.t.qq.com/share/share.php?title=%s", content);

		CDlgBaseExplorer* pDlgExplorer = (CDlgBaseExplorer*)GetGameUIMan()->GetDialog("win_explorer");
		pDlgExplorer->Show(true);
		pDlgExplorer->NavigateUrl(url);
	}

	CDlgAchivItemExpand *pDlgAchivItemExpand = (CDlgAchivItemExpand*)GetGameUIMan()->GetDialog("Win_Achievement_DoneFolder");
	m_pDlgAchivitemMan->ExpandItem(this, pDlgAchivItemExpand != this);
}

void CDlgAchivItem::OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	PAUISUBDIALOG pSubD = (PAUISUBDIALOG)GetGameUIMan()->GetDialog("Win_Achievement")->GetDlgItem("Sub_Detail");
	if(pSubD)
		pSubD->OnDlgItemMessage(WM_MOUSEWHEEL, wParam, lParam);
}

void CDlgAchivItem::SetAchiv(sAchievement *pAchiv)
{
	if(m_pAchiv)
	{
		SAFE_DELETE(m_pAchiv);
	}
	UINT i=0;
	m_pAchiv = new sAchievement;
	m_pAchiv->id = pAchiv->id;
	m_pAchiv->name = pAchiv->name;
	m_pAchiv->strDesc = pAchiv->strDesc;
	m_pAchiv->strPreCondDesc = pAchiv->strPreCondDesc;
	m_pAchiv->strMsg = pAchiv->strMsg;
	m_pAchiv->iClass = pAchiv->iClass;
	m_pAchiv->iSubClass = pAchiv->iSubClass;
	m_pAchiv->awardScore = pAchiv->awardScore;
	m_pAchiv->awardItem = pAchiv->awardItem;
	m_pAchiv->awardMoney = pAchiv->awardMoney;
	m_pAchiv->bFinish = pAchiv->bFinish;
	m_pAchiv->bActive = pAchiv->bActive;
	m_pAchiv->bMarkFinish = pAchiv->bMarkFinish;
	m_pAchiv->iFinishTime = pAchiv->iFinishTime;
	m_pAchiv->vecPremissID.clear();
	for (i=0; i<pAchiv->vecPremissID.size(); i++)
	{
		m_pAchiv->vecPremissID.push_back(pAchiv->vecPremissID[i]);
	}
	m_pAchiv->vecCond.clear();
	for (i=0; i<pAchiv->vecCond.size(); i++)
	{
		m_pAchiv->vecCond.push_back(pAchiv->vecCond[i]);
	}
	m_pAchiv->bGetAward = pAchiv->bGetAward;
	
	ACString strText;
	strText = m_pAchiv->strDesc;
	FilterColor(strText);
	m_pLab_Desc->SetText(strText);
	strText.Format(_AL("%d"), m_pAchiv->awardScore);
	m_pLab_Point->SetText(strText);
	strText.Empty();
	if (m_pAchiv->bMarkFinish && m_pAchiv->iFinishTime>0)
	{
		long t = m_pAchiv->iFinishTime;
		if (t>0)
		{
			t -= g_pGame->GetTimeZoneBias() * 60;
			tm *gtime = gmtime((const time_t*)&t);
			strText.Format(_AL("%d/%d/%d"), gtime->tm_year+1900, gtime->tm_mon+1, gtime->tm_mday);
		}
	}
	m_pLab_Time->SetText(strText);
	strText = m_pAchiv->name;
	AUIOBJECT_SETPROPERTY p, pt;
	if (m_pAchiv->bFinish)
	{
		sprintf(p.fn, "Frame\\AchievementDone.tga");
		sprintf(pt.fn, "Icon\\TitleDone.tga");
		GetDlgItem("Img_Frame")->SetColor(A3DCOLORRGB(255, 255, 255));
		m_pImg_Point->SetColor(A3DCOLORRGB(255, 255, 255));
		m_pImg_Metal->SetColor(A3DCOLORRGB(255, 255, 255));
		m_pLab_Point->SetColor(A3DCOLORRGB(255, 255, 0));
	}
	else
	{
		sprintf(p.fn, "Frame\\AchievementUnDone.tga");
		sprintf(pt.fn, "Icon\\TitleUndone.tga");
		GetDlgItem("Img_Frame")->SetColor(A3DCOLORRGB(128, 128, 128));
		m_pImg_Point->SetColor(A3DCOLORRGB(128, 128, 128));
		m_pImg_Metal->SetColor(A3DCOLORRGB(128, 128, 128));
		m_pLab_Point->SetColor(A3DCOLORRGB(128, 128, 128));
		FilterColor(strText);
	}

	if (m_pAchiv->awardItem == 0)
	{
		GetDlgItem(BTN_GET_AWARD)->Show(false);
		GetDlgItem(BTN_OBTAIN_AWARD)->Show(false);
		GetDlgItem(IMG_AWARD)->Show(false);
	}
	else if (m_pAchiv->bGetAward)
	{
		GetDlgItem(BTN_GET_AWARD)->Show(false);
		GetDlgItem(BTN_OBTAIN_AWARD)->Show(true);
	}
	else if (m_pAchiv->bFinish)
	{
		GetDlgItem(BTN_GET_AWARD)->Show(true);
		GetDlgItem(BTN_OBTAIN_AWARD)->Show(false);
	}
	else
	{
		GetDlgItem(BTN_GET_AWARD)->Show(false);
		GetDlgItem(BTN_OBTAIN_AWARD)->Show(false);
	}

	m_pLab_Name->SetText(strText);
	m_pEdt_Back->SetProperty("Frame Image", &p);
	m_pImg_Title->SetProperty("Image File", &pt);
	ACString strName = pAchiv->name;
	strName.CutLeft(7);
	strName.CutRight(7);
	sprintf(p.fn, "Ingame\\achivicon\\%d.dds", pAchiv->id);
	m_pImg_Metal->SetProperty("Image File", &p);
}

void CDlgAchivItem::FilterColor(ACString& str)
{
	ACString strTemp;
	int nLength = str.GetLength();
	int nFrom = 0;
	int index = str.Find(_AL('^'));
	strTemp.Empty();
	while(index>=0)
	{
		if (index+7<=nLength)
		{
			strTemp += str.Mid(nFrom, index-nFrom);
		}
		else
			break;
		nFrom = index+7;
		index = str.Find(_AL('^'), nFrom);
	}
	strTemp += str.Right(nLength-nFrom);
	str = strTemp;
}

AUI_BEGIN_COMMAND_MAP(CDlgAchivItemExpand, CDlgAchivItem)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgAchivItemExpand, CDlgAchivItem)

AUI_END_EVENT_MAP()

bool CDlgAchivItemExpand::ProgressHintRenderCallback(const POINT &pt, DWORD param1, DWORD param2, DWORD param3)
{
	CDlgAchivItemExpand *This = (CDlgAchivItemExpand *) param1;
	DWORD index = param2;
	if (This == NULL || index >= ACHIV_COND_MAX)
		return false;

	This->m_pLab_Prog[index]->Render();
	return true;
}

bool CDlgAchivItemExpand::OnInitDialog()
{
	AString strName;
	int i;
	for (i=0; i<ACHIV_PREV_MAX; i++)
	{
		strName.Format("Lab_Pre%d", i+1);
		m_pLab_Prev[i] = (PAUILABEL)GetDlgItem(strName);
	}
	for (i=0; i<ACHIV_COND_MAX; i++)
	{
		strName.Format("Lab_%d", i+1);
		m_pLab_Cond[i] = (PAUILABEL)GetDlgItem(strName);

		strName.Format("Prgs_%d", i+1);
		m_pProg_Cond[i] = (PAUIPROGRESS)GetDlgItem(strName);
		m_pProg_Cond[i]->SetRenderHintCallback(ProgressHintRenderCallback, (DWORD) this, i);

		strName.Format("Txt_Prgs_%d", i+1);
		m_pLab_Prog[i] = (PAUILABEL)GetDlgItem(strName);
		m_pLab_Prog[i]->Show(false);
	}
	m_pImg_Award = (PAUIIMAGEPICTURE)GetDlgItem("Img_Award");
	m_pEdt_Frame = (PAUIEDITBOX)GetDlgItem("Edt_Back");
	m_nOrigHeight = GetSize().cy;
	m_nOrigPerHeight = m_pLab_Cond[1]->GetPos().y - m_pLab_Cond[0]->GetPos().y;
	m_nOrigPerLabHeight = m_pProg_Cond[0]->GetPos().y - m_pLab_Cond[0]->GetPos().y;
	m_nOrigPerProgHeight = m_pLab_Prog[0]->GetPos().y - m_pProg_Cond[0]->GetPos().y;
	
	return CDlgAchivItem::OnInitDialog();
}

void CDlgAchivItemExpand::OnShowDialog()
{
	assert(m_pAchiv);
	ACString strText, strDesc, strName;
	CDlgAchivMain* pDlgAchhivMain = (CDlgAchivMain*)GetGameUIMan()->GetDialog("Win_Achievement");
	CECPlayer *pPlayer = pDlgAchhivMain->GetTargetPlayer();
	CECAchievementMan* pMan = pPlayer->GetAchievementMan();
	UINT iSizePrev = m_pAchiv->vecPremissID.size();
	UINT i = 0;
	AUIOBJECT_SETPROPERTY p;
	if (iSizePrev>0)
	{
		for (i=0; i<iSizePrev; i++)
		{
			strText = GetStringFromTable(8474);
			sAchievement* pAchivC = pMan->GetAchievement(m_pAchiv->vecPremissID[i]);
			if (pAchivC->bFinish)
			{
				strText += szFinishColor;
			}
			strName = pAchivC->name;
			FilterColor(strName);
			strText += strName;
			m_pLab_Prev[i]->SetText(strText);
			m_pLab_Prev[i]->Show(true);
		}
	}
	
	if (m_pAchiv->awardItem == 0)
	{
		GetDlgItem(BTN_GET_AWARD)->Show(false);
		GetDlgItem(BTN_OBTAIN_AWARD)->Show(false);
	}
	else if (m_pAchiv->bGetAward)
	{
		GetDlgItem(BTN_GET_AWARD)->Show(false);
		GetDlgItem(BTN_OBTAIN_AWARD)->Show(true);
	}
	else if (m_pAchiv->bFinish)
	{
		GetDlgItem(BTN_GET_AWARD)->Show(true);
		GetDlgItem(BTN_OBTAIN_AWARD)->Show(false);
	}
	else
	{
		GetDlgItem(BTN_GET_AWARD)->Show(false);
		GetDlgItem(BTN_OBTAIN_AWARD)->Show(false);
	}

	for (; i<ACHIV_PREV_MAX; i++)
	{
		m_pLab_Prev[i]->Show(false);
	}
	UINT iSize = m_pAchiv->vecCond.size();
	int nPerHeight = m_pLab_Prev[1]->GetPos().y - m_pLab_Prev[0]->GetPos().y;

	p.i = m_pLab_Prev[0]->GetPos(true).y + iSizePrev*nPerHeight;
	for (i=0; i<iSize; i++)
	{
		m_pAchiv->vecCond[i].GetStrDesc(strDesc);
		FilterColor(strDesc);
		if (m_pAchiv->vecCond[i].bFinish)
			strText = ACString(_AL("^00ff00")) + strDesc;
		else
			strText = strDesc;
		m_pLab_Cond[i]->SetText(strText);
		p.i = m_pLab_Prev[0]->GetPos(true).y + iSizePrev*nPerHeight + i*m_nOrigPerHeight;
		m_pLab_Cond[i]->SetProperty("Y", &p);
		m_pLab_Cond[i]->Show(true);

		int curProgValue, maxProgValue;
		if ( m_pAchiv->vecCond[i].GetProgress(curProgValue, maxProgValue) )
		{
			m_pProg_Cond[i]->SetProgress(curProgValue * 100 / maxProgValue);
			p.i += m_nOrigPerLabHeight;
			m_pProg_Cond[i]->SetProperty("Y", &p);
			m_pProg_Cond[i]->Show(true);

			ACString hint;
			hint.Format(_AL("%d/%d"), curProgValue, maxProgValue);

			p.i += m_nOrigPerProgHeight;
			m_pLab_Prog[i]->SetProperty("Y", &p);
			m_pLab_Prog[i]->SetText(hint);
		}
		else
		{
			m_pLab_Cond[i]->Show(false);
			m_pProg_Cond[i]->Show(false);
		}
	}
	for (; i<ACHIV_COND_MAX; i++)
	{
		m_pLab_Cond[i]->Show(false);
		m_pProg_Cond[i]->Show(false);
	}
	//根据成就条件数量动态调整界面大小
	float fWindowScale = m_pAUIManager->GetWindowScale() * m_fScale;
	SetDefaultSize(GetDefaultSize().cx, m_nOrigHeight - nPerHeight*(ACHIV_PREV_MAX-iSizePrev) - m_nOrigPerHeight*(ACHIV_COND_MAX-iSize));

	
	p.i = int((m_nOrigHeight - nPerHeight*(ACHIV_PREV_MAX-iSizePrev) - m_nOrigPerHeight*(ACHIV_COND_MAX-iSize)) * fWindowScale);
	m_pEdt_Frame->SetProperty("Height", &p);

	m_pImg_Award->Show(false);
	if (m_pAchiv->awardItem>0)
	{
		CECIvtrItem *pItem = CECIvtrItem::CreateItem(m_pAchiv->awardItem, 0, 1);
		if (pItem)
		{
			pItem->GetDetailDataFromLocal();
			AUICTranslate trans;
			AString strFile;
			m_pImg_Award->SetHint(trans.Translate(pItem->GetDesc()));
			af_GetFileTitle(pItem->GetIconFile(), strFile);
			strFile.MakeLower();
			int ItemIndex;
			A2DSprite* pSprite = GetGameUIMan()->GetSpriteIcon(CECGameUIMan::ICONS_INVENTORY, strFile.GetBuffer(0), &ItemIndex);
			m_pImg_Award->Show(true);
			m_pImg_Award->SetCover( pSprite, ItemIndex);
			m_pImg_Award->SetColor(GetGameUIMan()->GetItemColor(pItem));
			delete pItem;
		}
	}	
}

AUI_BEGIN_COMMAND_MAP(CDlgAchivPop, CDlgBase)

AUI_ON_COMMAND("Btn_Achievement", OnCommmandAchiv)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgAchivPop, CDlgBase)

AUI_END_EVENT_MAP()

CDlgAchivPop::CDlgAchivPop()
{

}

CDlgAchivPop::~CDlgAchivPop()
{

}

bool CDlgAchivPop::OnInitDialog()
{
	m_pLab_Name = (PAUILABEL)GetDlgItem("Title");
	m_pLab_Point = (PAUILABEL)GetDlgItem("Lab_Point");
	m_pImg_Title = (PAUIIMAGEPICTURE)GetDlgItem("Img_Title");
	m_pImg_Metal = (PAUIIMAGEPICTURE)GetDlgItem("Img_Achievement");
	m_pImg_Point = (PAUIIMAGEPICTURE)GetDlgItem("Img_Point");
	m_pAchiv	= NULL;
	
	return true;
}

bool CDlgAchivPop::Release()
{
	if(m_pAchiv)
	{
		SAFE_DELETE(m_pAchiv);
	}
	return CDlgBase::Release();
}

void CDlgAchivPop::OnCommmandAchiv(const char* szCommand)
{
	Show(false);
	CDlgAchivMain* pDlgAchievement = (CDlgAchivMain*)GetGameUIMan()->GetDialog("Win_Achievement");
 	pDlgAchievement->SetTarget(0);
	pDlgAchievement->Show(true);
	pDlgAchievement->ShowAchiv(m_pAchiv);
}

void CDlgAchivPop::SetAchiv(sAchievement *pAchiv)
{
	if(m_pAchiv)
	{
		SAFE_DELETE(m_pAchiv);
	}
	UINT i=0;
	m_pAchiv = new sAchievement;
	m_pAchiv->id = pAchiv->id;
	m_pAchiv->name = pAchiv->name;
	m_pAchiv->strDesc = pAchiv->strDesc;
	m_pAchiv->strPreCondDesc = pAchiv->strPreCondDesc;
	m_pAchiv->strMsg = pAchiv->strMsg;
	m_pAchiv->iClass = pAchiv->iClass;
	m_pAchiv->iSubClass = pAchiv->iSubClass;
	m_pAchiv->awardScore = pAchiv->awardScore;
	m_pAchiv->awardItem = pAchiv->awardItem;
	m_pAchiv->awardMoney = pAchiv->awardMoney;
	m_pAchiv->bFinish = pAchiv->bFinish;
	m_pAchiv->bActive = pAchiv->bActive;
	m_pAchiv->bMarkFinish = pAchiv->bMarkFinish;
	m_pAchiv->iFinishTime = pAchiv->iFinishTime;
	m_pAchiv->bGetAward = pAchiv->bGetAward;
	m_pAchiv->vecPremissID.clear();
	for (i=0; i<pAchiv->vecPremissID.size(); i++)
	{
		m_pAchiv->vecPremissID.push_back(pAchiv->vecPremissID[i]);
	}
	m_pAchiv->vecCond.clear();
	for (i=0; i<pAchiv->vecCond.size(); i++)
	{
		m_pAchiv->vecCond.push_back(pAchiv->vecCond[i]);
	}
	
	ACString strText;
	strText.Format(_AL("%d"), m_pAchiv->awardScore);
	m_pLab_Point->SetText(strText);
	strText.Empty();
	strText = m_pAchiv->name;
	AUIOBJECT_SETPROPERTY pt;
	if (m_pAchiv->bFinish)
	{
		sprintf(pt.fn, "Icon\\TitleDone.tga");
		GetDlgItem("Img_Frame")->SetColor(A3DCOLORRGB(255, 255, 255));
		m_pImg_Point->SetColor(A3DCOLORRGB(255, 255, 255));
		m_pImg_Metal->SetColor(A3DCOLORRGB(255, 255, 255));
		m_pLab_Point->SetColor(A3DCOLORRGB(255, 255, 0));
	}
	else
	{
		sprintf(pt.fn, "Icon\\TitleUndone.tga");
		GetDlgItem("Img_Frame")->SetColor(A3DCOLORRGB(128, 128, 128));
		m_pImg_Point->SetColor(A3DCOLORRGB(128, 128, 128));
		m_pImg_Metal->SetColor(A3DCOLORRGB(128, 128, 128));
		m_pLab_Point->SetColor(A3DCOLORRGB(128, 128, 128));
	}
	
	m_pImg_Title->SetProperty("Image File", &pt);
	ACString strName = pAchiv->name;
	strName.CutLeft(7);
	strName.CutRight(7);
	m_pLab_Name->SetText(strName);
	AUIOBJECT_SETPROPERTY p;
	sprintf(p.fn, "Ingame\\achivicon\\%d.dds", pAchiv->id);
	m_pImg_Metal->SetProperty("Image File", &p);
}

void CDlgAchivItemOther::SetAchiv( sAchievement *pAchiv )
{
	if(m_pAchiv)
	{
		SAFE_DELETE(m_pAchiv);
	}
	UINT i=0;
	m_pAchiv = new sAchievement;
	m_pAchiv->id = pAchiv->id;
	m_pAchiv->name = pAchiv->name;
	m_pAchiv->strDesc = pAchiv->strDesc;
	m_pAchiv->strPreCondDesc = pAchiv->strPreCondDesc;
	m_pAchiv->strMsg = pAchiv->strMsg;
	m_pAchiv->iClass = pAchiv->iClass;
	m_pAchiv->iSubClass = pAchiv->iSubClass;
	m_pAchiv->awardScore = pAchiv->awardScore;
	m_pAchiv->awardItem = pAchiv->awardItem;
	m_pAchiv->awardMoney = pAchiv->awardMoney;
	m_pAchiv->bFinish = pAchiv->bFinish;
	m_pAchiv->bActive = pAchiv->bActive;
	m_pAchiv->bMarkFinish = pAchiv->bMarkFinish;
	m_pAchiv->iFinishTime = pAchiv->iFinishTime;
	m_pAchiv->vecPremissID.clear();
	for (i=0; i<pAchiv->vecPremissID.size(); i++)
	{
		m_pAchiv->vecPremissID.push_back(pAchiv->vecPremissID[i]);
	}
	m_pAchiv->vecCond.clear();
	for (i=0; i<pAchiv->vecCond.size(); i++)
	{
		m_pAchiv->vecCond.push_back(pAchiv->vecCond[i]);
	}
	m_pAchiv->bGetAward = pAchiv->bGetAward;

	AUIOBJECT_SETPROPERTY p, pt;
	if (m_pAchiv->bFinish)
	{
		sprintf(p.fn, "Frame\\AchievementDone.tga");
		sprintf(pt.fn, "Icon\\TitleDone.tga");
		GetDlgItem("Img_Frame")->SetColor(A3DCOLORRGB(255, 255, 255));
		m_pImg_Point->SetColor(A3DCOLORRGB(255, 255, 255));
		m_pImg_Metal->SetColor(A3DCOLORRGB(255, 255, 255));
		m_pLab_Point->SetColor(A3DCOLORRGB(255, 255, 0));
		m_pLab_Status->SetText(GetGameUIMan()->GetStringFromTable(20122));
	}
	else
	{
		sprintf(p.fn, "Frame\\AchievementUnDone.tga");
		sprintf(pt.fn, "Icon\\TitleUndone.tga");
		GetDlgItem("Img_Frame")->SetColor(A3DCOLORRGB(128, 128, 128));
		m_pImg_Point->SetColor(A3DCOLORRGB(128, 128, 128));
		m_pImg_Metal->SetColor(A3DCOLORRGB(128, 128, 128));
		m_pLab_Point->SetColor(A3DCOLORRGB(128, 128, 128));
		m_pLab_Status->SetText(GetGameUIMan()->GetStringFromTable(20121));
	}

	m_pEdt_Back->SetProperty("Frame Image", &p);
	sprintf(p.fn, "Ingame\\achivicon\\%d.dds", pAchiv->id);
	m_pImg_Metal->SetProperty("Image File", &p);
}

bool CDlgAchivItemOther::OnInitDialog()
{
	m_pLab_Point = (PAUILABEL)GetDlgItem("Lab_Point");
	m_pImg_Metal = (PAUIIMAGEPICTURE)GetDlgItem("Img_Achievement");
	m_pImg_Point = (PAUIIMAGEPICTURE)GetDlgItem("Img_Point");
	m_pEdt_Back = (PAUIEDITBOX)GetDlgItem("Edt_Back");
	m_pLab_Status = (PAUILABEL)GetDlgItem("Lab_Status");

	m_pAchiv	= NULL;
	m_pDlgAchivitemMan = (CDlgAchivItemMan*)GetGameUIMan()->GetDialog("Win_Achievement_Other");

	return true;
}