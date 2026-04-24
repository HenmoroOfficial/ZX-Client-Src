// TreasureItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "elementdata.h"
#include "TreasureItemDlg.h"
#include "PropertyList.h"
#include "AObject.h"
#include "TemplIDSelDlg.h"
#include "MultiTextDlg.h"
#include "TreasureMskDlg.h"
#include "TreasureValidTimeDlg.h"

#define new A_DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// CTreasureItemDlg dialog


CTreasureItemDlg::CTreasureItemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTreasureItemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTreasureItemDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	for( int i = 0; i < 1; ++i)
	{
		m_Item.buy_fashion[i].price = 0;
		m_Item.buy_fashion[i].time = 0;
	}
	strcpy(m_Item.szName,"宝物1");
	memset(m_Item.desc,0,sizeof(WORD)*512);
	memset(m_Item.file_icon,0,128);
	m_Item.id = 0;
	m_Item.count = 1;
	m_Item.discount = 0;
	m_Item.others = 0x00ff0000;
	
	//新加赠品
	m_Item.bHasPresent = false;
	memset(m_Item.szPresentName,0,128); 
	m_Item.Presentid = 0;
	m_Item.Presentcount = 0;
	m_Item.Presenttime = 0;
	memset(m_Item.Present_file_icon,0,128);
	m_Item.bPresentBind = false;
	memset(m_Item.Presentdesc,0,sizeof(WORD)*512);

	m_pListIDSel = 0;
	m_Item.bonus = 0;
	m_bReadOnly = false;
	m_pMultiLineFunc = new MULTILINE_TEXT_FUNCS;

	m_pMaskDiscount = new MASK_SEL_FUNCS;
	m_pMaskSale = new MASK_SEL_FUNCS;

	m_pValidTime = new VALID_TIME_SEL_FUNCS;	//Added 2011-11-07.
	
	m_pListIDSel_ForPresent = 0;
	m_pMultiLineFunc_ForPresent = new MULTILINE_TEXT_FUNCS;
	m_pMultiLineFunc_ForKey = new MULTILINE_TEXT_FUNCS;
}


void CTreasureItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTreasureItemDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTreasureItemDlg, CDialog)
	//{{AFX_MSG_MAP(CTreasureItemDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreasureItemDlg message handlers

BOOL CTreasureItemDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_pList = new CPropertyList();
	m_pProperty = new ADynPropertyObject;
	
	CWnd *pWnd = GetDlgItem(IDC_LIST_TREASURE);
	CRect rc,rc1;
	pWnd->GetClientRect(&rc);

	int width = rc.Width();
	int length = rc.Height();

	pWnd->GetWindowRect(&rc);
	GetWindowRect(&rc1);
	rc.left = rc.left - rc1.left;
	rc.top = rc.top - rc1.top - 20;
	rc.right = rc.left + width;
	rc.bottom = rc.top + length;

	BOOL bResult = m_pList->Create("宝物", WS_CHILD | WS_VISIBLE, rc, this, 121);
	
	BuildProperty();
	m_pList->AttachDataObject(m_pProperty);
	UpdateProperty(false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTreasureItemDlg::BuildProperty()
{
	m_pListIDSel = new CTemplIDSel;
	m_pListIDSel->Init("BaseData",".tmpl",AVariant(0),enumBaseID);

	m_pListIDSel_ForPresent = new CTemplIDSel;
	m_pListIDSel_ForPresent->Init("BaseData",".tmpl",AVariant(0),enumBaseID);

	DWORD flag = 0;
	if(m_bReadOnly) flag = WAY_READONLY;
	m_pProperty->DynAddProperty(AVariant(AString(m_Item.szName)), "宝物名(32字符)",NULL,NULL,flag);
	m_pProperty->DynAddProperty(AVariant(m_Item.id), "物品",(ASet*)m_pListIDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | flag);
	m_pProperty->DynAddProperty(AVariant(m_Item.id), "物品ID",NULL, NULL, WAY_READONLY);

	m_pProperty->DynAddProperty(AVariant(m_Item.count), "物品数量",NULL,NULL,flag);

	//Added 2011-11-07.
//	AString tbl = "";
//	m_pProperty->DynAddProperty(AVariant(tbl), "生效时间",(ASet*)m_pValidTime, NULL, WAY_CUSTOM | flag);
	//Added end.
	
	m_pProperty->DynAddProperty(AVariant(m_Item.buy_fashion[0].price), "购买方式(价格)",NULL,NULL,flag);
	m_pProperty->DynAddProperty(AVariant(m_Item.buy_fashion[0].time), "购买方式(租借时间[秒])",NULL,NULL,flag);
	m_pProperty->DynAddProperty(AVariant(m_Item.bonus), "返点比例",NULL,NULL,flag);
	/*
	m_pProperty->DynAddProperty(AVariant(m_Item.buy_fashion[1].price), "购买方式2(价格)",NULL,NULL,flag);
	m_pProperty->DynAddProperty(AVariant(m_Item.buy_fashion[1].time), "购买方式2(租借时间[秒])",NULL,NULL,flag);

	m_pProperty->DynAddProperty(AVariant(m_Item.buy_fashion[2].price), "购买方式3(价格)",NULL,NULL,flag);
	m_pProperty->DynAddProperty(AVariant(m_Item.buy_fashion[2].time), "购买方式3(租借时间[秒])",NULL,NULL,flag);

	m_pProperty->DynAddProperty(AVariant(m_Item.buy_fashion[3].price), "购买方式4(价格)",NULL,NULL,flag);
	m_pProperty->DynAddProperty(AVariant(m_Item.buy_fashion[3].time), "购买方式4(租借时间[秒])",NULL,NULL,flag);
	*/
	BYTE tb = 0;
	m_pProperty->DynAddProperty(AVariant(m_Item.discount), "打折比例",NULL,NULL,flag);
	m_pProperty->DynAddProperty(AVariant(tb), "打折方案",(ASet*)m_pMaskDiscount, NULL, WAY_CUSTOM | flag);
	m_pProperty->DynAddProperty(AVariant(tb), "出售方案",(ASet*)m_pMaskSale, NULL, WAY_CUSTOM | flag);
	
	bool temp = false;
	m_pProperty->DynAddProperty(AVariant(temp), "是否为新品",NULL,NULL,flag);
	m_pProperty->DynAddProperty(AVariant(temp), "是否为推荐品",NULL,NULL,flag);
	m_pProperty->DynAddProperty(AVariant(temp), "是否为促销品",NULL,NULL,flag);
	
	m_pProperty->DynAddProperty(AVariant(AString(m_Item.file_icon)), "图标路径",NULL,NULL,WAY_FILENAME | flag);
	m_pProperty->DynAddProperty(AVariant(AString(WC2AC((WCHAR*)m_Item.desc))), "物品描述(512字符)",(ASet*)m_pMultiLineFunc,NULL,WAY_CUSTOM | flag);

	//新加赠品
	m_pProperty->DynAddProperty(AVariant(m_Item.bHasPresent), "是否有赠品",NULL,NULL,flag);
	m_pProperty->DynAddProperty(AVariant(AString(m_Item.szPresentName)), "赠品名称(32字符)",NULL,NULL,flag);
	m_pProperty->DynAddProperty(AVariant(m_Item.Presentid), "赠品",(ASet*)m_pListIDSel_ForPresent->GetFuncsPtr(), NULL, WAY_CUSTOM | flag);
	m_pProperty->DynAddProperty(AVariant(m_Item.Presentid), "赠品ID",NULL, NULL, WAY_READONLY);
	m_pProperty->DynAddProperty(AVariant(m_Item.Presentcount), "赠品数量",NULL,NULL,flag);
	m_pProperty->DynAddProperty(AVariant(m_Item.Presenttime), "赠品存在的时限(秒)",NULL,NULL,flag);
	m_pProperty->DynAddProperty(AVariant(AString(m_Item.Present_file_icon)), "赠品图标路径",NULL,NULL,WAY_FILENAME | flag);
	m_pProperty->DynAddProperty(AVariant(m_Item.bPresentBind), "赠品是否绑定",NULL,NULL,flag);
	m_pProperty->DynAddProperty(AVariant(AString(WC2AC((WCHAR*)m_Item.Presentdesc))), "赠品描述(512字符)",(ASet*)m_pMultiLineFunc_ForPresent,NULL,WAY_CUSTOM | flag);

	//Added 2011-11-07.
	AString tbl = "";
	m_pProperty->DynAddProperty(AVariant(tbl), "生效时间",(ASet*)m_pValidTime, NULL, WAY_CUSTOM | flag);
	//Added end.

	//Added 2013-04-17 关键字，","分割	
	m_pProperty->DynAddProperty(AVariant(AString(WC2AC((WCHAR*)m_Item.SearchKey))), "关键字\",\"分割(共64字)",(ASet*)m_pMultiLineFunc_ForKey,NULL,WAY_CUSTOM | flag);
}

void CTreasureItemDlg::UpdateProperty(bool bGet)
{
	m_pList->UpdateData(true);
	if(bGet)
	{
		
		AString name = AString(m_pProperty->GetPropVal(0));
		strcpy(m_Item.szName, name);
		m_Item.id = m_pProperty->GetPropVal(1);	
		
		m_Item.count = m_pProperty->GetPropVal(3);	
		m_Item.buy_fashion[0].price = m_pProperty->GetPropVal(4);
		m_Item.buy_fashion[0].time = m_pProperty->GetPropVal(5);
		m_Item.bonus = m_pProperty->GetPropVal(6);
		/*
		m_Item.buy_fashion[1].price = m_pProperty->GetPropVal(5);
		m_Item.buy_fashion[1].time = m_pProperty->GetPropVal(6);

		m_Item.buy_fashion[2].price = m_pProperty->GetPropVal(7);
		m_Item.buy_fashion[2].time = m_pProperty->GetPropVal(8);

		m_Item.buy_fashion[3].price = m_pProperty->GetPropVal(9);
		m_Item.buy_fashion[3].time = m_pProperty->GetPropVal(10);
		*/
		DWORD by;
		m_Item.discount = m_pProperty->GetPropVal(7);
		by = m_pMaskDiscount->OnGetValue();
		m_Item.others = m_Item.others & 0x00ffffff | (by << 24);
		by = m_pMaskSale->OnGetValue();
		m_Item.others = m_Item.others & 0xff00ffff | (by << 16 & 0x00ff0000);
			
		DWORD flag = BIT_PRESENT;
		bool temp = (bool) m_pProperty->GetPropVal(10);
		if(temp) m_Item.others = m_Item.others | flag;
		else m_Item.others &= ~flag;

		flag = BIT_RECOMMEND;
		temp = (bool) m_pProperty->GetPropVal(11);
		if(temp) m_Item.others = m_Item.others | flag;
		else m_Item.others &= ~flag;

		flag = BIT_SALES_PROMOTION;
		temp = (bool) m_pProperty->GetPropVal(12);
		if(temp) m_Item.others = m_Item.others | flag;
		else m_Item.others &= ~flag;

		name = AString(m_pProperty->GetPropVal(13));
		if(!name.IsEmpty()) strcpy(m_Item.file_icon,name);
		wcscpy((WCHAR*)m_Item.desc,AC2WC(AString(m_pProperty->GetPropVal(14))));
		

		//新加赠品
		m_Item.bHasPresent	= (bool)m_pProperty->GetPropVal(15);
		AString Prentname = AString(m_pProperty->GetPropVal(16));
		strcpy(m_Item.szPresentName,Prentname);
		m_Item.Presentid	= (unsigned int)m_pProperty->GetPropVal(17);
		m_Item.Presentcount = (unsigned int)m_pProperty->GetPropVal(19);
		m_Item.Presenttime		= (unsigned int)m_pProperty->GetPropVal(20);
		Prentname = AString(m_pProperty->GetPropVal(21));
		if(!Prentname.IsEmpty()) 
			strcpy(m_Item.Present_file_icon,Prentname);
		m_Item.bPresentBind	= (bool)m_pProperty->GetPropVal(22);
		wcscpy((WCHAR*)m_Item.Presentdesc,AC2WC(AString(m_pProperty->GetPropVal(23))));

		//新增加商城上架有效时间 Added 2011-11-08.
		m_pValidTime->OnGetAllValues( m_Item.valid_time.type, m_Item.valid_time.start_time, m_Item.valid_time.end_time, m_Item.valid_time.param );
		//Added end.
		wcscpy((WCHAR*)m_Item.SearchKey,AC2WC(AString(m_pProperty->GetPropVal(25))));
	}
	else
	{
		m_pProperty->SetPropVal(0,AString(m_Item.szName));
		m_pProperty->SetPropVal(1,m_Item.id);
		m_pListIDSel->SetValue(AVariant(m_Item.id));
		m_pProperty->SetPropVal(3,m_Item.count);
		
		m_pProperty->SetPropVal(4,m_Item.buy_fashion[0].price);
		m_pProperty->SetPropVal(5,m_Item.buy_fashion[0].time);
		m_pProperty->SetPropVal(6,m_Item.bonus);

		BYTE by;
		m_pProperty->SetPropVal(7,m_Item.discount);
		by = m_Item.others >> 24;
		m_pMaskDiscount->OnSetValue(by);
		by = m_Item.others >> 16 & 0x000000ff;
		m_pMaskSale->OnSetValue(by);
		
		/*
		m_pProperty->SetPropVal(5,m_Item.buy_fashion[1].price);
		m_pProperty->SetPropVal(6,m_Item.buy_fashion[1].time);

		m_pProperty->SetPropVal(7,m_Item.buy_fashion[2].price);
		m_pProperty->SetPropVal(8,m_Item.buy_fashion[2].time);

		m_pProperty->SetPropVal(9,m_Item.buy_fashion[3].price);
		m_pProperty->SetPropVal(10,m_Item.buy_fashion[3].time);
		*/

		if(m_Item.others & BIT_PRESENT) 
			m_pProperty->SetPropVal(10,true);
		else m_pProperty->SetPropVal(10,false);

		if(m_Item.others & BIT_RECOMMEND) 
			m_pProperty->SetPropVal(11,true);
		else m_pProperty->SetPropVal(11,false);

		if(m_Item.others & BIT_SALES_PROMOTION) 
			m_pProperty->SetPropVal(12,true);
		else m_pProperty->SetPropVal(12,false);

		m_pProperty->SetPropVal(13,AString(m_Item.file_icon));
		//m_pProperty->SetPropVal(9,WC2AC(m_Item.desc));
		m_pMultiLineFunc->OnSetValue(AVariant(WC2AC((WCHAR*)m_Item.desc)));

		
		//新加赠品
		m_pProperty->SetPropVal(15,m_Item.bHasPresent);
		m_pProperty->SetPropVal(16,AString(m_Item.szPresentName));
		m_pProperty->SetPropVal(17,m_Item.Presentid);
		m_pListIDSel_ForPresent->SetValue(AVariant(m_Item.Presentid));
		m_pProperty->SetPropVal(19,m_Item.Presentcount);
		m_pProperty->SetPropVal(20,m_Item.Presenttime);
		m_pProperty->SetPropVal(21,AString(m_Item.Present_file_icon));
		m_pProperty->SetPropVal(22,m_Item.bPresentBind);
		m_pMultiLineFunc_ForPresent->OnSetValue(AVariant(WC2AC((WCHAR*)m_Item.Presentdesc)));

		//新增加商城上架有效时间 Added 2011-11-08.
		m_pValidTime->OnSetAllValues( m_Item.valid_time.type, m_Item.valid_time.start_time, m_Item.valid_time.end_time, m_Item.valid_time.param );
		//Added end.
		
		m_pMultiLineFunc_ForKey->OnSetValue(AVariant(WC2AC((WCHAR*)m_Item.SearchKey)));
	}
	m_pList->AttachDataObject(m_pProperty);
};

BOOL CTreasureItemDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_pList)
	{
		m_pList->CleanItems();
		m_pList->DestroyWindow();
		delete m_pList;
	}
	if(m_pProperty) delete m_pProperty;

	if(m_pListIDSel) delete m_pListIDSel; 
	if(m_pMultiLineFunc) delete m_pMultiLineFunc;
	if(m_pMaskDiscount) delete m_pMaskDiscount;
	if(m_pMaskSale) delete m_pMaskSale;
	if( m_pValidTime ) delete m_pValidTime;//Added 2011-11-08.

	if(m_pMultiLineFunc_ForPresent) delete m_pMultiLineFunc_ForPresent;
	if(m_pListIDSel_ForPresent) delete m_pListIDSel_ForPresent;
	if(m_pMultiLineFunc_ForKey) delete m_pMultiLineFunc_ForKey;

	return CDialog::DestroyWindow();
}

void CTreasureItemDlg::OnOK()
{
	UpdateData(true);
	UpdateProperty(true);
	CDialog::OnOK();
}

BOOL CALLBACK MULTILINE_TEXT_FUNCS::OnActivate(void)
{
	CMultiTextDlg dlg;
	dlg.m_strText = AString(m_var);
	if (dlg.DoModal() != IDOK) return FALSE;
	m_strText = dlg.m_strText;
	m_var = AString(m_strText);
	return TRUE;
}

LPCTSTR CALLBACK MULTILINE_TEXT_FUNCS::OnGetShowString(void) const
{
	return m_strText;
}

AVariant CALLBACK MULTILINE_TEXT_FUNCS::OnGetValue(void) const
{
	return m_var;
}

void CALLBACK MULTILINE_TEXT_FUNCS::OnSetValue(const AVariant& var)
{
	m_var = var;
	m_strText = AString(m_var);
}



/////////////////////////////////////////////////////////////////////////

BOOL CALLBACK MASK_SEL_FUNCS::OnActivate(void)
{
	CTreasureMskDlg dlg;
	dlg.m_byteMask = (BYTE)m_var;
	if (dlg.DoModal() != IDOK) return TRUE;
	m_var = dlg.m_byteMask;
	m_strText = "";
	for( int i = 0; i < 8; ++i)
	{
		if(dlg.m_byteMask & (1 << i))
		{
			CString temp;
			temp.Format("%d,",i+1);
			m_strText += temp;
		}
	}


	return TRUE;
}

LPCTSTR CALLBACK MASK_SEL_FUNCS::OnGetShowString(void) const
{
	return m_strText;
}

AVariant CALLBACK MASK_SEL_FUNCS::OnGetValue(void) const
{
	return m_var;
}

void CALLBACK MASK_SEL_FUNCS::OnSetValue(const AVariant& var)
{
	m_var = var;
	BYTE bt = m_var;
	m_strText = "";
	for( int i = 0; i < 8; ++i)
	{
		if(bt & (1 << i))
		{
			CString temp;
			temp.Format("%d,",i+1);
			m_strText += temp;
		}
	}
}


//=============================================================================
static const char ValueFormat[] = "type: %d; start_time: %d; end_time: %d; param: %d";
static const CString WeekDay[] = {"周日", "周一", "周二", "周三", "周四", "周五", "周六" };
//Added 2011-11-07.
BOOL CALLBACK VALID_TIME_SEL_FUNCS::OnActivate(void)
{
	//
	CTreasureValidTimeDlg dlg;
	int nType = 0, nStartTime = 0, nEndTime = 0, nParam = 0;
	OnGetAllValues( nType, nStartTime, nEndTime, nParam );
	dlg.SetAllValues( nType, nStartTime, nEndTime, nParam );
	if ( dlg.DoModal() != IDOK )
	{
		return TRUE;
	}

	//判断输入合法性
	if ( (dlg.m_nType == CTreasureValidTimeDlg::TYPE_TIMEINTERVAL && (dlg.m_nParam == 0)) )
	{
		AfxMessageBox( "非法输入：无任何开启时间或者结束时间！" );
		return FALSE;
	}
	if ( (dlg.m_nType == CTreasureValidTimeDlg::TYPE_TIMEINTERVAL && (dlg.m_nParam != 3)) ||
		(dlg.m_nStartTime < dlg.m_nEndTime) )
	{
		OnSetAllValues( dlg.m_nType, dlg.m_nStartTime, dlg.m_nEndTime, dlg.m_nParam );
	}
	else
	{
		AfxMessageBox( "非法输入：开启时间大于等于结束时间！" );
		return FALSE;
	}

	return TRUE;
}

LPCTSTR CALLBACK VALID_TIME_SEL_FUNCS::OnGetShowString(void) const
{
	return m_strText;
}

AVariant CALLBACK VALID_TIME_SEL_FUNCS::OnGetValue(void) const
{
	return m_var;
}

void CALLBACK VALID_TIME_SEL_FUNCS::OnSetValue(const AVariant& var)
{
	m_var = var;
	m_strText = "";
	int nType = 0, nStartTime = 0, nEndTime = 0, nParam = 0;
	if ( !OnGetAllValues(nType, nStartTime, nEndTime, nParam) )
	{
		return;
	}

	switch ( nType )	//处理方法类型
	{
		case 0:	//无效,也就是永久有效，没有时间限制
		{
			m_strText = "无效";
			break;
		}
		case 1: //按时间段处理
		{
			m_strText = "按时间段有效； ";
			if ( nParam & 0x00000001 )
			{
				CString str;
				str.Format( "开启时间：%d； ", nStartTime );
				m_strText += str;
			}
			if ( nParam & 0x00000002 )
			{
				CString str;
				str.Format( "结束时间：%d； ", nEndTime );
				m_strText += str;
			}
			break;
		}
		case 2: //按每周处理
		{
			m_strText = "按每周有效： ";
			for ( int i = 1; i <= 7; ++i )
			{
				if ( nParam & (1 << i) )
				{
					m_strText += WeekDay[i - 1];
					m_strText += "； ";
				}
			}
			CString str;
			int nHourBegin = nStartTime / 3600;
			int nMinBegin  = (nStartTime % 3600) / 60;
			int nHourEnd   = nEndTime / 3600;
			int nMinEnd	   = (nEndTime % 3600) / 60;
			str.Format( "开始时间：%d:%d； 结束时间：%d:%d", nHourBegin, nMinBegin, nHourEnd, nMinEnd );
			m_strText += str;
			break;
		}
		case 3: //按每月处理
		{
			m_strText = "按每月有效： ";
			CString str;
			for ( int i = 1; i <= 31; ++i )
			{
				if ( nParam & (1 << i) )
				{
					str.Format( "%d；", i );
					m_strText += str;
				}
			}
			int nHourBegin = nStartTime / 3600;
			int nMinBegin  = (nStartTime % 3600) / 60;
			int nHourEnd   = nEndTime / 3600;
			int nMinEnd	   = (nEndTime % 3600) / 60;
			str.Format( "开始时间：%d:%d； 结束时间：%d:%d", nHourBegin, nMinBegin, nHourEnd, nMinEnd );
			m_strText += str;
			break;
		}
		default:
			break;
	}
}

BOOL CALLBACK VALID_TIME_SEL_FUNCS::OnGetAllValues( int& nType, int& nStartTime, int& nEndTime, int& nParam )
{
	CString strValue = AString(m_var);
	if ( strValue.IsEmpty() )
	{
		return FALSE;
	}
	sscanf(strValue, ValueFormat, &nType, &nStartTime, &nEndTime, &nParam);
	
	//
	return TRUE;
}

void CALLBACK VALID_TIME_SEL_FUNCS::OnSetAllValues( int nType, int nStartTime, int nEndTime, int nParam )
{
	char szValue[1024];
	sprintf( szValue, ValueFormat, nType, nStartTime, nEndTime, nParam );
	OnSetValue( AString(szValue) );
}

//=============================================================================