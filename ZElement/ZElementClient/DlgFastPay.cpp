// Filename	: DlgFastPay.cpp
// Creator	: ice
// Date		: 2012/07/04
// Desc		: 快捷充值

#include "DlgFastPay.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_IvtrItem.h"
#include "AUI\\AUICTranslate.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagepicture.h"
#include "afi.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameUIMisc.h"
#include "EC_GameUIMan.h"

#include "A2DSprite.h"
#include "ExpTypes.h"
#include "DlgFastPayCode.h"

#define new A_DEBUG_NEW

extern CECGame* g_pGame;

AUI_BEGIN_COMMAND_MAP(CDlgFastPay, CDlgBase)

AUI_ON_COMMAND("confirm",		OnCommand_Confirm)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgFastPay, CDlgBase)


AUI_END_EVENT_MAP()

CDlgFastPay::CDlgFastPay()
{
	m_nYuanbaoNum = 0;

	m_nDiscount = 100;

	m_npaymode = 1;
	m_pvecdiscountgrade = NULL;
}

CDlgFastPay::~CDlgFastPay()
{
}

bool CDlgFastPay::OnInitDialog()
{
	if (!CDlgBase::OnInitDialog())
		return false;

	m_pTxtInfo = (PAUITEXTAREA)GetDlgItem("DEFAULT_Txt_Money");

	return true;
}

void CDlgFastPay::OnShowDialog()
{	
	ACString strText;

	strText.Format(_AL("%s"), GetHostPlayer()->GetName());
	GetDlgItem("Txt_PlayerName")->SetText(strText);

	strText.Format(_AL("%s-%s"), GetGameRun()->GetCurGroup(), GetGameRun()->GetServerName());
	GetDlgItem("Txt_Server")->SetText(strText);
	
	//获取当前支付方式对应的折扣等级，目前只有支付宝一种，暂时把代码写在这个函数里
	m_pvecdiscountgrade = NULL;
	for(int i = 0; i < m_vecMerchantDiscount.size(); i++)
	{
		if(m_vecMerchantDiscount[i].id == m_npaymode)
		{
			m_pvecdiscountgrade = m_vecMerchantDiscount[i].	pvecdiscountgrade;
			break;
		}
	}

	if(m_pvecdiscountgrade)
	{
		ACString strText, strDiscount;
		for(int i = 0; i < m_pvecdiscountgrade->size(); i++)
		{
			strDiscount.Format(GetGameUIMan()->GetStringFromTable(16200), (*m_pvecdiscountgrade)[i].amount_begin / 100, (*m_pvecdiscountgrade)[i].discount);
			strText += strDiscount;
		}
		GetDlgItem("Img_Discount")->SetHint(strText);
	}	
}

bool CDlgFastPay::Tick(void)
{
	ACString strText = m_pTxtInfo->GetText();
	m_nYuanbaoNum = a_atoi(strText);
	
	if(m_nYuanbaoNum < 6 || m_nYuanbaoNum > 1000)
	{
		GetDlgItem("Btn_Confirm")->Enable(false);
	}
	else
	{
		GetDlgItem("Btn_Confirm")->Enable(true);
	}

	m_nDiscount = 100;
	if(m_pvecdiscountgrade)
	{
		for(int i = 0; i < m_pvecdiscountgrade->size(); i++)
			if(m_nYuanbaoNum * 100 >= (*m_pvecdiscountgrade)[i].amount_begin)
				m_nDiscount = (*m_pvecdiscountgrade)[i].discount;
			else break;
	}

	ACString strMoney;
	strMoney.Format(_AL("%.2f"), float(m_nYuanbaoNum*m_nDiscount)/100.0f);

	GetDlgItem("Txt_Money")->SetText(strMoney);

	// discount
	strMoney.Format(_AL("%d"), m_nDiscount);
	GetDlgItem("Txt_Discount")->SetText(strMoney);

	return true;
}
void CDlgFastPay::OnCommand_Confirm(const char * szCommand)
{
	SendFastPayCmd();
	Show(false);
}

void CDlgFastPay::SendFastPayCmd()
{
	if(m_nYuanbaoNum < 6 || m_nYuanbaoNum > 1000)
		return;

	// 注意：单位为银元宝，所以要*100
	GetGameSession()->fast_pay(m_nYuanbaoNum*100, m_nYuanbaoNum*m_nDiscount, 1, 0);
}

bool CDlgFastPay::Release()
{
	for(int i = 0; i < m_vecMerchantDiscount.size(); i++)
	{
		if(m_vecMerchantDiscount[i].pvecdiscountgrade)
		{
			delete m_vecMerchantDiscount[i].pvecdiscountgrade;
			m_vecMerchantDiscount[i].pvecdiscountgrade = NULL;
		}
	}
	return CDlgBase::Release();
}

void CDlgFastPay::AddBindInfo(int bindID)
{
	abase::vector<int>::iterator iter;
	for(iter = m_vecBindInfo.begin();iter!=m_vecBindInfo.end();++iter)
	{
		if(*iter == bindID)
			return;
	}

	m_vecBindInfo.push_back(bindID);
}

void CDlgFastPay::AddMerchantDiscount(int id, abase::vector<DISCOUNT_GRADE> *pvecdiscountgrade)
{
	abase::vector<MERCHANT_DISCOUNT>::iterator iter;
	for(iter = m_vecMerchantDiscount.begin();iter!=m_vecMerchantDiscount.end();++iter)
	{
		if((*iter).id == id)
			break;
	}

	if(iter == m_vecMerchantDiscount.end())
	{
		MERCHANT_DISCOUNT mDiscount;
		mDiscount.id = id;
		mDiscount.pvecdiscountgrade = pvecdiscountgrade;
		m_vecMerchantDiscount.push_back(mDiscount);	
	}

//	if(id == 1)	// 支付宝
//		m_nDiscount = discount;
}

bool CDlgFastPay::HasBind(int idBind)
{
	abase::vector<int>::iterator iter;
	for(iter = m_vecBindInfo.begin();iter!=m_vecBindInfo.end();++iter)
	{
		if(*iter == idBind)
			return true;
	}

	return false;
}

