// PartDlg.cpp : implementation file
//


#include <set>
#include "Global.h"
#include "ModelView.h"
#include "PartDlg.h"
#include "Player.h"
//#include "..\ZElementClient\EC_IvtrTypes.h"
#include "..\ZElementData\DataPathMan.h"
#include "MODELVIEWDataMan.h"
#include "..\ZElementClient\EC_RoleTypes.h"
#include "a3d.h"



#define new A_DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// CPartDlg dialog


CPartDlg::CPartDlg(CWnd* pParent /*=NULL*/)
: CDialog(CPartDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPartDlg)
	m_strFilter = _T("");
	//}}AFX_DATA_INIT
	m_nFilter = 0;
}


void CPartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPartDlg)
	DDX_Control(pDX, IDC_LIST_PART, m_listPart);
	DDX_Text(pDX, IDC_EDIT_FILTER, m_strFilter);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPartDlg, CDialog)
//{{AFX_MSG_MAP(CPartDlg)
ON_LBN_SELCHANGE(IDC_LIST_PART, OnSelchangeListPart)
ON_BN_CLICKED(IDC_BUTTON_FILTER, OnButtonFilter)
ON_BN_CLICKED(IDC_CHECK_MAN, OnCheckMan)
ON_BN_CLICKED(IDC_CHECK_WOMAN, OnCheckWoman)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPartDlg message handlers

void CPartDlg::OnOK()
{
	ShowWindow(SW_HIDE);
}

void CPartDlg::OnCancel()
{
	ShowWindow(SW_HIDE);
}

void CPartDlg::ClearAllItem()
{
	int n = m_listPart.GetCount();
	for( int i = 0; i < n; ++i)
		m_listPart.DeleteString(0);
}

void CPartDlg::SetPartType( int nPart , CString strOldSkin , bool bFilter)
{ 
	ClearAllItem();
	m_nPart = nPart; 
	m_strOldSkin = strOldSkin;
	if(!bFilter) InitFilter();
	switch(m_nPart)
	{
	case PART_HEAD:			     // 头
		InitHead(true);
		break;
	case PART_HAIR:				// 头发
		InitHair(true);
		break;
	case PART_BODY:				// 普通衣服
		InitBody(true);
		break;
	case PART_FOOT:				// 普通鞋子
		InitFoot(true);
		break;
	case PART_HELM:				// 普通头盔
		InitHelm(true);
		break;
	case PART_EAR:
		InitEar(true);
		break;
	case PART_TAIL:
		InitTail(true);
		break;
		//case PART_GLASSES:			// 眼镜
		//InitGlasses(true);
		//break;
		//case PART_NOSE:				// 鼻子
		//InitNose(true);
		//break;
		//case PART_MUSTACHE:			// 胡子
		//InitMustache(true);
		//break;
		//case PART_CLOAK	:			// 披风
		//InitCloak(true);
		//break;
	case PART_FASHION_UPPER_BODY:// 时装上衣
		InitFashionUpper(true);
		break;
		//case PART_FASHION_LOWER:	// 时装裤子
		//InitFashionLower(true);
		//break;
	case PART_FASHION_FOOT:		// 时装鞋子
		InitFashionFoot(true);
		break;
	case PART_FASHION_HEADWEAR:	// 时装头饰
		InitFashionHeadwear(true);
		break;
	case PART_FASHION_EAR:
		InitFashionEar(true);
		break;
	case PART_FASHION_TAIL:
		InitFashionTail(true);
		break;
	case PART_WEAPON:
		InitWeapon(true);
		break;
	case PART_RIDE:
		InitRide(true);
		break;
	}
	SetSkinSel(strOldSkin);
}

void CPartDlg::SetSkinSel(CString sel)
{
	int n = m_listPart.GetCount();
	for( int i = 0; i < n; ++i)
	{
		CString txt;
		
		if(g_bUsedPath && m_nPart!= PART_HEAD && m_nPart != PART_HAIR)
		{
			if(m_nPart==PART_RIDE)
			{
				VEHICLE_ESSENCE* pData = (VEHICLE_ESSENCE*)m_listPart.GetItemData(i);;
				if(pData) txt = WC2AS(pData->name);
			}else
			{
				EQUIPMENT_ESSENCE* pData = (EQUIPMENT_ESSENCE*)m_listPart.GetItemData(i);;
				if(pData) txt = WC2AS(pData->name);
			}
		}else m_listPart.GetText(i,txt);
		
		if(sel==txt) 
		{
			m_listPart.SetCurSel(i);
			return;
		}
	}
	if(n>0) 
	{
		if(g_bUsedPath) m_listPart.SetCurSel(n-1);
		else m_listPart.SetCurSel(0);
	}
}

void CPartDlg::InitHead( bool bFilter)
{
	SetWindowText("头");
	m_listPart.AddString("---None---");
	
	
	CString strName;
	for( int i = 1; i < HEAD_INDEX_NUM+1; i++)
	{
		strName.Format("头%d",i);
		m_listPart.AddString(strName);
	}
}

void CPartDlg::InitHair(bool bFilter)
{
	SetWindowText("头发");
	m_listPart.AddString("---None---");
	
	CString strName;
	for( int i = 1; i < HEAD_INDEX_NUM+1; i++)
	{
		strName.Format("头发%d",i);
		m_listPart.AddString(strName);
	}
}

void CPartDlg::InitEar(bool bFilter)
{
	SetWindowText("耳朵");
	m_listPart.AddString("---None---");
	
	CString strName;
	for( int i = 1; i < EAR_INDEX_NUM+1; i++)
	{
		strName.Format("耳朵1%d",i);
		m_listPart.AddString(strName);
	}
	for( i = 1; i < EAR_INDEX_NUM+1; i++)
	{
		strName.Format("耳朵2%d",i);
		m_listPart.AddString(strName);
	}
	for( i = 1; i < EAR_INDEX_NUM+1; i++)
	{
		strName.Format("耳朵3%d",i);
		m_listPart.AddString(strName);
	}
	for( i = 1; i < EAR_INDEX_NUM+1; i++)
	{
		strName.Format("耳朵4%d",i);
		m_listPart.AddString(strName);
	}
}

void CPartDlg::InitTail(bool bFilter)
{
	SetWindowText("尾巴");
	m_listPart.AddString("---None---");
	
	CString strName;
	for( int i = 1; i < TAIL_INDEX_NUM+1; i++)
	{
		strName.Format("尾巴1%d",i);
		m_listPart.AddString(strName);
	}
	for( i = 1; i < TAIL_INDEX_NUM+1; i++)
	{
		strName.Format("尾巴2%d",i);
		m_listPart.AddString(strName);
	}
	for( i = 1; i < TAIL_INDEX_NUM+1; i++)
	{
		strName.Format("尾巴3%d",i);
		m_listPart.AddString(strName);
	}
	for( i = 1; i < TAIL_INDEX_NUM+1; i++)
	{
		strName.Format("尾巴4%d",i);
		m_listPart.AddString(strName);
	}
}

void CPartDlg::InitFashionEar(bool bFilter)
{
	SetWindowText("时装耳朵");
	m_listPart.AddString("---None---");
	
	CString strName;
	for (int i = 1; i<=FASHION_EAR_TAIL_INDEX_NUM;i++) {
		strName.Format("时装耳朵%d",i);
		m_listPart.AddString(strName);
	}
}

void CPartDlg::InitFashionTail(bool bFilter)
{
	SetWindowText("时装尾巴");
	m_listPart.AddString("---None---");
	
	CString strName;
	for (int i = 1; i<=FASHION_EAR_TAIL_INDEX_NUM;i++) {
		strName.Format("时装尾巴%d",i);
		m_listPart.AddString(strName);
	}
}

void CPartDlg::InitBody(bool bFilter)
{
	SetWindowText("上衣");
	int item = m_listPart.AddString("---None---");
	m_listPart.SetItemData(item,0);
	DWORD dwNum = g_DataMan.GetEquipEssenceNum();//get_data_num(ID_SPACE_ESSENCE);
	if(dwNum == 0) return;
	for(int i = 0; i < dwNum; i++)
	{
		//DATA_TYPE type;
		//DWORD dwid = g_DataMan.get_data_id(ID_SPACE_ESSENCE,i,type);
		//if(type == DT_EQUIPMENT_ESSENCE)
		{
			EQUIPMENT_ESSENCE* pData;
			pData = (EQUIPMENT_ESSENCE*)g_DataMan.GetEquipEssencePtrByIndex(i);//get_data_ptr(dwid,ID_SPACE_ESSENCE,type);
			
			CString strName = "";
			
			if(pData->equip_type != 1 || !(pData->equip_mask & EQUIP_MASK_BODY))
				continue;
			
			GetEquipNameAndSetListItemData(pData,strName,bFilter);	
		}
	}
}

void CPartDlg::InitFoot(bool bFilter)
{
	SetWindowText("鞋子");
	int item = m_listPart.AddString("---None---");
	m_listPart.SetItemData(item,0);
	DWORD dwNum = g_DataMan.GetEquipEssenceNum();//get_data_num(ID_SPACE_ESSENCE);
	if(dwNum == 0) return;
	for(int i = 0; i < dwNum; i++)
	{
		//DATA_TYPE type;
		//DWORD dwid = g_DataMan.get_data_id(ID_SPACE_ESSENCE,i,type);
		//if(type == DT_EQUIPMENT_ESSENCE)
		{
			EQUIPMENT_ESSENCE* pData;
			pData = (EQUIPMENT_ESSENCE*)g_DataMan.GetEquipEssencePtrByIndex(i);//get_data_ptr(dwid,ID_SPACE_ESSENCE,type);
			
			CString strName = "";
			if(pData->equip_type != 1 || !(pData->equip_mask & EQUIP_MASK_FOOT))
				continue;	
			GetEquipNameAndSetListItemData(pData,strName,bFilter);
		}
	}
}

void CPartDlg::InitHelm(bool bFilter)
{
	SetWindowText("头盔");
	int item = m_listPart.AddString("---None---");
	m_listPart.SetItemData(item,0);
	DWORD dwNum = g_DataMan.GetEquipEssenceNum();//get_data_num(ID_SPACE_ESSENCE);
	if(dwNum == 0) return;
	for(int i = 0; i < dwNum; i++)
	{
		//DATA_TYPE type;
		//DWORD dwid = g_DataMan.get_data_id(ID_SPACE_ESSENCE,i,type);
		//if(type == DT_EQUIPMENT_ESSENCE)
		{
			EQUIPMENT_ESSENCE* pData;
			pData = (EQUIPMENT_ESSENCE*)g_DataMan.GetEquipEssencePtrByIndex(i);//get_data_ptr(dwid,ID_SPACE_ESSENCE,type);
			
			CString strName = "";
			
			if(pData->equip_type != 1 || !(pData->equip_mask & EQUIP_MASK_HEAD))
				continue;	
			GetEquipNameAndSetListItemData(pData,strName,bFilter);
		}
	}
}
//
//void CPartDlg::InitGlasses(bool bFilter)
//{
//	SetWindowText("眼镜");
//	m_listPart.AddString("---None---");
//	DWORD dwNum = g_DataMan.get_data_num(ID_SPACE_ESSENCE);
//	if(dwNum == 0) return;
//	for(int i = 0; i < dwNum; i++)
//	{
//		DATA_TYPE type;
//		DWORD dwid = g_DataMan.get_data_id(ID_SPACE_ESSENCE,i,type);
//		if(type == DT_EQUIPMENT_ESSENCE)
//		{
//			EQUIPMENT_ESSENCE* pData;
//			pData = (EQUIPMENT_ESSENCE*)g_DataMan.get_data_ptr(dwid,ID_SPACE_ESSENCE,type);
//			CString strName;
//			if(g_bUsedPath)
//			{
//				unsigned int nPath;
//				if( pData->require_gender==0 ) nPath = pData->file_model_male;
//				else nPath = pData->file_model_female;
//				if(nPath == 0) nPath = nPath = pData->file_model_male;
//
//				const char *szPath = g_DataPathMan.GetPathByID(nPath);
//				strName = szPath;
//			}else strName = WC2AS(pData->name);
//			if(pData->equip_mask & EQUIP_MASK_FASHION_BODY && pData->equip_type == 3  && pData->equip_location==6)
//			{
//				if(bFilter)
//				{
//					if(strName.Find(m_strFilter)==-1) continue;
//					if(Filter(pData->require_gender)) continue;
//				}
//				
//				int item = m_listPart.AddString(strName);
//				m_listPart.SetItemData(item,(DWORD)pData);
//			}
//		}
//	}
//}
//
//void CPartDlg::InitNose(bool bFilter)
//{
//	SetWindowText("鼻子");
//	m_listPart.AddString("---None---");
//	DWORD dwNum = g_DataMan.get_data_num(ID_SPACE_ESSENCE);
//	if(dwNum == 0) return;
//	for(int i = 0; i < dwNum; i++)
//	{
//		DATA_TYPE type;
//		DWORD dwid = g_DataMan.get_data_id(ID_SPACE_ESSENCE,i,type);
//		if(type == DT_EQUIPMENT_ESSENCE)
//		{
//			EQUIPMENT_ESSENCE* pData;
//			pData = (EQUIPMENT_ESSENCE*)g_DataMan.get_data_ptr(dwid,ID_SPACE_ESSENCE,type);
//			CString strName;
//			if(g_bUsedPath)
//			{
//				unsigned int nPath;
//				if( pData->require_gender==0 ) nPath = pData->file_model_male;
//				else nPath = pData->file_model_female;
//				if(nPath == 0) nPath = nPath = pData->file_model_male;
//
//				const char *szPath = g_DataPathMan.GetPathByID(nPath);
//				strName = szPath;
//			}else strName = WC2AS(pData->name);
//			if(pData->equip_mask & 0x0100 && pData->equip_type == 3 && pData->equip_location==7)
//			{
//				if(bFilter)
//				{
//					if(strName.Find(m_strFilter)==-1) continue;
//					if(Filter(pData->require_gender)) continue;
//				}
//				
//				int item = m_listPart.AddString(strName);
//				m_listPart.SetItemData(item,(DWORD)pData);
//			}
//		}
//	}
//}
//
//void CPartDlg::InitMustache(bool bFilter)
//{
//	SetWindowText("胡子");
//	m_listPart.AddString("---None---");
//	DWORD dwNum = g_DataMan.get_data_num(ID_SPACE_ESSENCE);
//	if(dwNum == 0) return;
//	for(int i = 0; i < dwNum; i++)
//	{
//		DATA_TYPE type;
//		DWORD dwid = g_DataMan.get_data_id(ID_SPACE_ESSENCE,i,type);
//		if(type == DT_EQUIPMENT_ESSENCE)
//		{
//			EQUIPMENT_ESSENCE* pData;
//			pData = (EQUIPMENT_ESSENCE*)g_DataMan.get_data_ptr(dwid,ID_SPACE_ESSENCE,type);
//			CString strName;
//			if(g_bUsedPath)
//			{
//				unsigned int nPath;
//				if( pData->require_gender==0 ) nPath = pData->file_model_male;
//				else nPath = pData->file_model_female;
//				if(nPath == 0) nPath = nPath = pData->file_model_male;
//				
//				const char *szPath = g_DataPathMan.GetPathByID(nPath);
//				strName = szPath;
//			}else strName = WC2AS(pData->name);
//			if(pData->equip_mask & 0x0200 && pData->equip_type == 3 && pData->equip_location==8)
//			{
//				if(bFilter)
//				{
//					if(strName.Find(m_strFilter)==-1) continue;
//					if(Filter(pData->require_gender)) continue;
//				}
//				
//				int item = m_listPart.AddString(strName);
//				m_listPart.SetItemData(item,(DWORD)pData);
//			}
//		}
//	}
//}
//
//void CPartDlg::InitCloak(bool bFilter)
//{
//	SetWindowText("背部");
//	m_listPart.AddString("---None---");
//	DWORD dwNum = g_DataMan.get_data_num(ID_SPACE_ESSENCE);
//	if(dwNum == 0) return;
//	for(int i = 0; i < dwNum; i++)
//	{
//		DATA_TYPE type;
//		DWORD dwid = g_DataMan.get_data_id(ID_SPACE_ESSENCE,i,type);
//		if(type == DT_EQUIPMENT_ESSENCE)
//		{
//			EQUIPMENT_ESSENCE* pData;
//			pData = (EQUIPMENT_ESSENCE*)g_DataMan.get_data_ptr(dwid,ID_SPACE_ESSENCE,type);
//			CString strName;
//			if(g_bUsedPath)
//			{
//				unsigned int nPath;
//				if( pData->require_gender==0 ) nPath = pData->file_model_male;
//				else nPath = pData->file_model_female;
//				if(nPath == 0) nPath = nPath = pData->file_model_male;
//				
//				const char *szPath = g_DataPathMan.GetPathByID(nPath);
//				strName = szPath;
//			}else strName = WC2AS(pData->name);
//			if(pData->equip_mask & 0x4000 && pData->equip_type == 3 && pData->equip_location==9)
//			{
//				if(bFilter)
//				{
//					if(strName.Find(m_strFilter)==-1) continue;
//					if(Filter(pData->require_gender)) continue;
//				}
//				
//				int item = m_listPart.AddString(strName);
//				m_listPart.SetItemData(item,(DWORD)pData);
//			}
//		}
//	}
//}

void CPartDlg::InitFashionUpper(bool bFilter)
{
	SetWindowText("时装上衣");
	int item = m_listPart.AddString("---None---");
	m_listPart.SetItemData(item,0);
	DWORD dwNum = g_DataMan.GetEquipEssenceNum();//get_data_num(ID_SPACE_ESSENCE);
	if(dwNum == 0) return;
	for(int i = 0; i < dwNum; i++)
	{
		//DATA_TYPE type;
		//DWORD dwid = g_DataMan.get_data_id(ID_SPACE_ESSENCE,i,type);
		//if(type == DT_EQUIPMENT_ESSENCE)
		EQUIPMENT_ESSENCE* pData;
		pData = (EQUIPMENT_ESSENCE*)g_DataMan.GetEquipEssencePtrByIndex(i);//get_data_ptr(dwid,ID_SPACE_ESSENCE,type);
		
		CString strName = "";
		if(pData->equip_type != 3 || !(pData->equip_mask & EQUIP_MASK_FASHION_BODY) || !(pData->equip_location==4 || pData->equip_location == 2))
			continue;
		GetEquipNameAndSetListItemData(pData,strName,bFilter);
		
	}
}
//
//void CPartDlg::InitFashionLower(bool bFilter)
//{
//	SetWindowText("时装裤子");
//	m_listPart.AddString("---None---");
//	DWORD dwNum = g_DataMan.get_data_num(ID_SPACE_ESSENCE);
//	if(dwNum == 0) return;
//	for(int i = 0; i < dwNum; i++)
//	{
//		DATA_TYPE type;
//		DWORD dwid = g_DataMan.get_data_id(ID_SPACE_ESSENCE,i,type);
//		if(type == DT_EQUIPMENT_ESSENCE)
//		{
//			EQUIPMENT_ESSENCE* pData;
//			pData = (EQUIPMENT_ESSENCE*)g_DataMan.get_data_ptr(dwid,ID_SPACE_ESSENCE,type);
//			CString strName;
//			if(g_bUsedPath)
//			{
//				unsigned int nPath;
//				if( pData->require_gender==0 ) nPath = pData->file_model_male;
//				else nPath = pData->file_model_female;
//				if(nPath == 0) nPath = nPath = pData->file_model_male;
//				
//				const char *szPath = g_DataPathMan.GetPathByID(nPath);
//				strName = szPath;
//			}else strName = WC2AS(pData->name);
//			if(pData->equip_mask & 0x1000 && pData->equip_type == 3 && pData->equip_location==3)
//			{
//				if(bFilter)
//				{
//					if(strName.Find(m_strFilter)==-1) continue;
//					if(Filter(pData->require_gender)) continue;
//				}
//				
//				int item = m_listPart.AddString(strName);
//				m_listPart.SetItemData(item,(DWORD)pData);
//			}
//		}
//	}
//}

void CPartDlg::InitFashionFoot(bool bFilter)
{
	SetWindowText("时装鞋子");
	int item = m_listPart.AddString("---None---");
	m_listPart.SetItemData(item,0);
	DWORD dwNum = g_DataMan.GetEquipEssenceNum();//get_data_num(ID_SPACE_ESSENCE);
	if(dwNum == 0) return;
	for(int i = 0; i < dwNum; i++)
	{
		//DATA_TYPE type;
		//DWORD dwid = g_DataMan.get_data_id(ID_SPACE_ESSENCE,i,type);
		//if(type == DT_EQUIPMENT_ESSENCE)
		{
			EQUIPMENT_ESSENCE* pData;
			pData = (EQUIPMENT_ESSENCE*)g_DataMan.GetEquipEssencePtrByIndex(i);//get_data_ptr(dwid,ID_SPACE_ESSENCE,type);
			
			CString strName = "";
			
			if(pData->equip_type != 3 ||!(pData->equip_mask & EQUIP_MASK_FASHION_FOOT) || pData->equip_location!=5)
				continue;
			GetEquipNameAndSetListItemData(pData,strName,bFilter);
		}
	}
}

void CPartDlg::InitFashionHeadwear(bool bFilter)
{
	SetWindowText("时装头饰");
	int item = m_listPart.AddString("---None---");
	m_listPart.SetItemData(item,0);
	DWORD dwNum = g_DataMan.GetEquipEssenceNum();//get_data_num(ID_SPACE_ESSENCE);
	if(dwNum == 0) return;
	for(int i = 0; i < dwNum; i++)
	{
		//DATA_TYPE type;
		//DWORD dwid = g_DataMan.get_data_id(ID_SPACE_ESSENCE,i,type);
		//if(type == DT_EQUIPMENT_ESSENCE)
		{
			EQUIPMENT_ESSENCE* pData;
			pData = (EQUIPMENT_ESSENCE*)g_DataMan.GetEquipEssencePtrByIndex(i);//get_data_ptr(dwid,ID_SPACE_ESSENCE,type);
			
			CString strName = "";
			
			if(pData->equip_type != 3 || !(pData->equip_mask & EQUIP_MASK_FASHION_HEAD) || pData->equip_location!=1)
				continue;
			GetEquipNameAndSetListItemData(pData,strName,bFilter);
			
		}
	}
}

void CPartDlg::InitWeapon(bool bFilter)
{
	SetWindowText("武器");
	int item = m_listPart.AddString("---None---");
	m_listPart.SetItemData(item,0);
	DWORD dwNum = g_DataMan.GetEquipEssenceNum();//get_data_num(ID_SPACE_ESSENCE);
	if(dwNum == 0) return;
	for(int i = 0; i < dwNum; i++)
	{
		//DATA_TYPE type;
		//DWORD dwid = g_DataMan.get_data_id(ID_SPACE_ESSENCE,i,type);
		//if(type == DT_EQUIPMENT_ESSENCE)
		EQUIPMENT_ESSENCE* pData;
		pData = (EQUIPMENT_ESSENCE*)g_DataMan.GetEquipEssencePtrByIndex(i);//get_data_ptr(dwid,ID_SPACE_ESSENCE,type);
		CString strName = "";
		
		if(pData->equip_type != 0||!(pData->equip_mask & EQUIP_MASK_WEAPON))//不是武器则跳过
			continue;
		GetEquipNameAndSetListItemData(pData,strName,bFilter);
	}
}

void CPartDlg::GetEquipNameAndSetListItemData(void* p, CString& strName,bool bFilter){
	
	EQUIPMENT_ESSENCE* pData =(EQUIPMENT_ESSENCE*) p;
	if( pData->require_gender!=2 && pData->require_gender != g_CPlayer->GetGender())//性别不符则跳过
		return;
	
	if((pData->sect_mask & ((1 << NUM_HUMAN_SECT) - 1)) && g_CPlayer->GetSect() == SECT_1//中等体型 人
		||(pData->sect_mask & (1 << SECT_15) )&& g_CPlayer->GetSect() == SECT_15)//中等体型 天华
	{
		if(g_bUsedPath)
		{
			unsigned int nPath = 0;
			switch(g_CPlayer->GetGender()) {
			case 0:
				nPath = pData->file_model_male; break;
			case 1:
				nPath = pData->file_model_female; break;
			default:
				nPath = pData->file_model_male; break;
			}
			if(nPath == 0) nPath = pData->file_model_male;
			const char *szPath = g_DataPathMan.GetPathByID(nPath);
			strName = szPath;
		}else strName = WC2AS(pData->name);
	}else if((pData->sect_mask & (1 << SECT_10) )&& g_CPlayer->GetSect() == SECT_10)//大体型
	{
		if(g_bUsedPath)
		{
			unsigned int nPath = 0;
			switch(g_CPlayer->GetGender()) {
			case 0:
				nPath = pData->file_model_big_male; break;
			case 1:
				nPath = pData->file_model_big_female; break;
			default:
				nPath = pData->file_model_big_male; break;
			}
			if(nPath == 0) nPath = pData->file_model_big_male;
			
			const char *szPath = g_DataPathMan.GetPathByID(nPath);
			strName = szPath;
		}else strName = WC2AS(pData->name);
	}else if((pData->sect_mask & (1 << SECT_11) )&& g_CPlayer->GetSect() == SECT_11//小体型 烈山
		||(pData->sect_mask & (1 << SECT_13) )&& g_CPlayer->GetSect() == SECT_13)//小体型 怀光
	{	
		if(g_bUsedPath)
		{
			unsigned int nPath = 0;
			switch(g_CPlayer->GetGender()) {
			case 0:
				nPath = pData->file_model_small_male; break;
			case 1:
				nPath = pData->file_model_small_female; break;
			default:
				nPath = pData->file_model_small_male; break;
			}
			if(nPath == 0) nPath = pData->file_model_small_male;
			
			const char *szPath = g_DataPathMan.GetPathByID(nPath);
			strName = szPath;
		}else strName = WC2AS(pData->name);
	}
	if(strName!="")
	{
		if(bFilter)
		{
			if(strName.Find(m_strFilter)==-1) return;
			if(Filter(pData->require_gender)) return;
		}
		
		int item = m_listPart.AddString(strName);
		m_listPart.SetItemData(item,(DWORD)pData);	
	}
	
}




void CPartDlg::InitRide(bool bFilter)
{
	SetWindowText("骑乘");
	int item = m_listPart.AddString("---None---");
	m_listPart.SetItemData(item,0);
	DWORD dwNum = g_DataMan.GetVehicleEssenceNum();//get_data_num(ID_SPACE_ESSENCE);
	if(dwNum == 0) return;
	std::set<CString> rideNamePath;
	rideNamePath.clear();
	for(int i = 0; i < dwNum; i++)
	{
		//DATA_TYPE type;
		//DWORD dwid = g_DataMan.get_data_id(ID_SPACE_ESSENCE,i,type);
		//if(type == DT_VEHICLE_ESSENCE)
		{
			VEHICLE_ESSENCE* pData;
			pData = (VEHICLE_ESSENCE*)g_DataMan.GetVehicleEssencePtrByIndex(i);//get_data_ptr(dwid,ID_SPACE_ESSENCE,type);
			CString strName;
			
			unsigned int nPath;
			nPath = pData->file_model;
			const char *szPath = g_DataPathMan.GetPathByID(nPath);		
			strName = szPath;
			strName.MakeLower();
			if(rideNamePath.find(strName)!=rideNamePath.end())continue;
			rideNamePath.insert(strName);
			if(!g_bUsedPath) strName = WC2AS(pData->name);
			
			if(bFilter)
			{
				if(strName.Find(m_strFilter)==-1) continue;
			}
			
			int item = m_listPart.AddString(strName);
			m_listPart.SetItemData(item,(DWORD)pData);	
		}
	}
}

void CPartDlg::OnSelchangeListPart() 
{
	
	int sel = m_listPart.GetCurSel();
	if(sel==-1) return;
	CString rString;
	m_listPart.GetText(sel,rString);
	
	if(g_pRenderableObject)
	{
		int type = g_pRenderableObject->GetObjectType();
		switch(type) 
		{
		case RENDERABLE_OBJECT_PLAYER:
			{	
				if(m_nPart==PART_HEAD)
				{
					rString.Replace("头","");
					int nSkin;
					if(rString=="---None---") nSkin = 1;
					else	nSkin = (int)atof(rString);	
					int nGender = ((CPlayer *)g_pRenderableObject)->GetGender();
					((CPlayer *)g_pRenderableObject)->ReplaceFixedSkin(SKIN_HEAD_INDEX,nGender,nSkin,nSkin/10);
				}else if(m_nPart==PART_HAIR)
				{
					rString.Replace("头发","");
					int nSkin;
					if(rString=="---None---") nSkin = 1;
					else	nSkin = (int)atof(rString);
					int nGender = ((CPlayer *)g_pRenderableObject)->GetGender();
					((CPlayer *)g_pRenderableObject)->ReplaceFixedSkin(SKIN_HAIR_INDEX,nGender,nSkin,nSkin/10);
				}else if(m_nPart==PART_EAR)
				{
					rString.Replace("耳朵","");
					int nSkin;
					if(rString=="---None---") nSkin = 1;
					else nSkin = (int)atof(rString);
					int nGender = ((CPlayer *)g_pRenderableObject)->GetGender();
					((CPlayer *)g_pRenderableObject)->ReplaceFixedSkin(m_nPart,nGender,nSkin,nSkin/10);
				}else if(m_nPart==PART_TAIL)
				{
					rString.Replace("尾巴","");
					int nSkin;
					if(rString=="---None---") nSkin = 1;
					else nSkin = (int)atof(rString);
					int nGender = ((CPlayer *)g_pRenderableObject)->GetGender();
					((CPlayer *)g_pRenderableObject)->ReplaceFixedSkin(m_nPart,nGender,nSkin,nSkin/10);
				}else if(m_nPart <= PART_FASHION_HEADWEAR && m_nPart > PART_HAIR )
				{
					DWORD dwPtr = (DWORD)m_listPart.GetItemDataPtr(sel);				
					((CPlayer *)g_pRenderableObject)->ReplaceSkin(m_nPart,dwPtr);
				}else if(m_nPart==PART_FASHION_EAR)
				{
					rString.Replace("时装耳朵","");
					int nSkin;
					if(rString=="---None---") nSkin = 1;
					else nSkin = (int)atof(rString);
					int nGender = ((CPlayer *)g_pRenderableObject)->GetGender();
					((CPlayer *)g_pRenderableObject)->ReplaceFixedSkin(m_nPart,nGender,nSkin,nSkin/10);
				}else if(m_nPart==PART_FASHION_TAIL)
				{
					rString.Replace("时装尾巴","");
					int nSkin;
					if(rString=="---None---") nSkin = 1;
					else nSkin = (int)atof(rString);
					int nGender = ((CPlayer *)g_pRenderableObject)->GetGender();
					((CPlayer *)g_pRenderableObject)->ReplaceFixedSkin(m_nPart,nGender,nSkin,nSkin/10);
				}else if(m_nPart == PART_WEAPON)
				{
					DWORD dwPtr = (DWORD)m_listPart.GetItemDataPtr(sel);
					((CPlayer *)g_pRenderableObject)->MountModel("Weapon",dwPtr);	
				}else if(m_nPart == PART_RIDE)
				{
					DWORD dwPtr = (DWORD)m_listPart.GetItemDataPtr(sel);
					((CPlayer *)g_pRenderableObject)->MountModel("Rider",dwPtr);	
				}		
				break;
			}
		case RENDERABLE_OBJECT_NPC:
			break;
		}
	}
}

void CPartDlg::InitFilter()
{
	bool bMan = ((CPlayer*)g_pRenderableObject)->IsMan();
	if(bMan) 
	{
		m_bFilterMan = false;
		m_bFilterWoman = true;
	}else
	{
		m_bFilterMan = true;
		m_bFilterWoman = false;
	}
	
	if(FILTER_GENDER == m_nFilter)
	{
		CButton *pWnd = (CButton *)GetDlgItem(IDC_CHECK_MAN);
		if(pWnd) 
		{
			pWnd->EnableWindow(TRUE);
			pWnd->SetCheck(!m_bFilterMan);
		}
		pWnd = (CButton *)GetDlgItem(IDC_CHECK_WOMAN);
		if(pWnd) 
		{
			pWnd->EnableWindow(TRUE);
			pWnd->SetCheck(!m_bFilterWoman);
		}
	}
}

void CPartDlg::OnButtonFilter() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	SetPartType(m_nPart,m_strOldSkin,true);
}

bool CPartDlg::Filter(WORD data)
{
	if(FILTER_GENDER == m_nFilter)
	{
		//0-男，1-女，2-男女都可
		if( 2 == data ) 
		{
			if(!m_bFilterWoman ||  (!m_bFilterMan))
				return false;
			else return true;
		}
		else if( 1 == data ) 
		{
			if(m_bFilterWoman) return true;
			else return false;
		}else if( 0 == data )
		{
			if(m_bFilterMan) return true;
			else return false;
		}
		
	}
	return false;
}

void CPartDlg::OnCheckMan() 
{
	// TODO: Add your control notification handler code here
	m_bFilterMan = !m_bFilterMan;
	SetPartType(m_nPart,m_strOldSkin,true);
	
}

void CPartDlg::OnCheckWoman() 
{
	// TODO: Add your control notification handler code here
	m_bFilterWoman = !m_bFilterWoman;
	SetPartType(m_nPart,m_strOldSkin,true);
}

