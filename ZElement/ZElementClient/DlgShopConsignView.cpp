// Filename	:	DlgShopConsignView.cpp
// Creator	:	Lei Dongyu
// Date		:	2010/10/29
// Description:	寻宝网游戏内商品浏览

#include "AUI\\AUIListBox.h"
#include "AUI\\AUITreeView.h"
#include "AUI\\AUICTranslate.h"
#include "AUI\\AUILabel.h"
#include "AFI.h"
#include "AString.h"
#include "AFileImage.h"
#include "A2DSprite.h"

#include "DlgShopConsignView.h"
#include "DlgShopConsignAction.h"
#include "DlgShopConsign.h"
#include "EC_GameUIMan.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrTalismanMain.h"
#include "EC_Game.h"
#include "EC_Global.h"
#include "EC_GameSession.h"
#include "EC_GameRun.h"
#include "EC_HostPlayer.h"
#include "EC_Algorithm.h"

#include "DlgDataConsignViewChar.h"

#include "consignlistall.hpp"
#include "consignlistall_re.hpp"
#include "consigngetitem.hpp"
#include "consigngetitem_re.hpp"
#include "consignlistlargecategory.hpp"
#include "consignlistlargecategory_re.hpp"
#include "GRoleInventory"
#include "GConsignListRoleNode"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgShopConsignView, CDlgBase)

AUI_ON_COMMAND("Rdo_AllList",			OnCommand_AllList)
AUI_ON_COMMAND("Rdo_CharAllList",		OnCommand_CharList)
AUI_ON_COMMAND("Rdo_MyList",			OnCommand_MyList)
AUI_ON_COMMAND("Btn_Close",				OnCommand_CANCEL)
AUI_ON_COMMAND("Btn_WebSite",			OnCommand_WebSite)
AUI_ON_COMMAND("Btn_PrevPage",			OnCommand_PrevPage)
AUI_ON_COMMAND("Btn_NextPage",			OnCommand_NextPage)
AUI_ON_COMMAND("Btn_FirstPage",			OnCommand_FirstPage)
AUI_ON_COMMAND("Btn_LastPage",			OnCommand_LastPage)
AUI_ON_COMMAND("Btn_Refresh",			OnCommand_Refresh)
AUI_ON_COMMAND("Btn_CharProf",			OnCommand_SortByProf)
AUI_ON_COMMAND("Btn_CharLv",			OnCommand_SortByLevel)
AUI_ON_COMMAND("Btn_CharName",			OnCommand_SortByName)
AUI_ON_COMMAND("Btn_Seller",			OnCommand_SortBySeller)
AUI_ON_COMMAND("Btn_ListName",			OnCommand_SortByName)
AUI_ON_COMMAND("Btn_ListPrice",			OnCommand_SortByPrice)
AUI_ON_COMMAND("Btn_ListTime",			OnCommand_SortByTime)
AUI_ON_COMMAND("Btn_Prev",				OnCommand_PrevPage)
AUI_ON_COMMAND("Btn_Next",				OnCommand_NextPage)
AUI_ON_COMMAND("Btn_Reset",				OnCommand_Reset)
AUI_ON_COMMAND("Btn_Help",				OnCommand_Help)
AUI_ON_COMMAND("IDCANCEL",				OnCommand_CANCEL)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgShopConsignView , CDlgBase)

AUI_ON_EVENT("Tree_List",	WM_LBUTTONDOWN,		OnEventLButtonDownTree)
AUI_ON_EVENT("Lst_AllGoods",WM_RBUTTONDOWN,		OnEventRButtonUpGoods)
AUI_ON_EVENT("Lst_AllChar", WM_RBUTTONDOWN,		OnEventRButtonUpChars)

AUI_END_EVENT_MAP()

CachePage::CachePage(int t, int p, std::vector<GConsignListNode> node)
{
	iPage = p;
	iType = t;
	vList.clear();
	dwRefreshTime = GetTickCount();
	std::vector<GConsignListNode>::iterator it = node.begin();
	if(node.size() > 0)
	{
		startIndex = it->info.sn;
		while(it != node.end())
		{
			ACString vendorName = ACString((const ACHAR *)it->seller_name.begin(), it->seller_name.size() / sizeof(ACHAR));
			ItemData item(it->info , vendorName , it->commodityid);

//			long createTime = it->info.sell_endtime - g_pGame->GetTimeZoneBias() * 60;
//			tm* tmCreate = gmtime(&createTime);
//			if( tmCreate )
//			{
//				ACString strText;
//				strText.Format(_AL("%d年%d月%d日%d时%d分"),tmCreate->tm_year+1900,tmCreate->tm_mon+1,tmCreate->tm_mday,tmCreate->tm_hour,tmCreate->tm_min);
//			}
//
			vList.push_back(item);
			//vendorName.Empty();
			it++;
		}
		it--;
		endIndex = it->info.sn;
	}
}

CachePage::CachePage(int t, int p)
{
	iPage = p;
	iType = t;
	dwRefreshTime = GetTickCount();
}

CachePage::CachePage()
{
}

CachePage::~CachePage()
{
}

ItemData::ItemData(GConsignItem d , ACString v, int id)
{
	data = d;
	vendor = v;
	commodityid = id;
}

ItemData::~ItemData()
{
//	delete vendor;
}

int CachePage::getType()
{
	return iType;
}

int CachePage::getPage()
{
	return iPage;
}

void CachePage::clear()
{
	startIndex = 0;
	endIndex = 0;
	iPage = 0;
	iType = 0;
	dwRefreshTime = 0;
	vList.clear();
}

ItemList* CachePage::getList()
{
	return &vList;
}

DWORD CachePage::getTime()
{
	return dwRefreshTime;
}

void CacheCategory::clear()
{
	abase::hash_map<int , CachePage* >::iterator it = cache.begin();
	while(it != cache.end())
	{
		CachePage* cp = it->second;
		abase::hash_map<int , CachePage* >::iterator ittemp = it;
		++it;
		cp->clear();
		delete cp;
		cache.erase(ittemp);			
	}
	lastPage = -1;
}


//ItemForRButton::ItemForRButton(int64_t s, ACString v)
//{
//	sn = s;
//	vendor = v;
//}

bool CDlgShopConsignView::Release()
{
	ClearCache(GetTickCount(), true);
	A2DSprite* moneyPic = vecItemPic.at(1);
	A3DRELEASE(moneyPic);
	delete m_DataViewChar;
	return CDlgBase::Release();
}

bool CDlgShopConsignView::OnInitDialog()
{
	m_pTree_ItemTree = (AUITreeView*)GetDlgItem("Tree_List");
	m_pList_Item = (AUIListBox*)GetDlgItem("Lst_AllGoods");
	m_pList_Char = (AUIListBox*)GetDlgItem("Lst_AllChar");
	m_pLabel = (AUILabel*)GetDlgItem("Lab_NoGood");
	m_pLabelStart = (AUILabel*)GetDlgItem("Lab_Start");
	m_pLabelSub = (AUILabel*)GetDlgItem("Lab_Sub");
	m_pLabel->Show(false);
	m_pLabelSub->Show(false);
	m_bLoaded = false;
	m_iLastPageCurType = -1;
	m_iCurPageItems = 0;
	iCurrentType = 0;
	m_pTree_ItemTree->SelectItem(m_pTree_ItemTree->GetRootItem());
	//pCurTreeItem = 0;

	_InsertSellTypes();

	vecItemPic.push_back(GetGameUIMan()->GetIconSet(CECGameUIMan::ICONS_INVENTORY));

	A2DSprite* moneyPic = new A2DSprite();
	char szFile[MAX_PATH];
	sprintf(szFile, "%s\\surfaces\\ingame\\寻宝金币.dds",af_GetBaseDir());
	//szFile.Format(_AL() , af_GetBaseDir());
	moneyPic->SetDynamicTex(true);
	bool bval = moneyPic->Init(m_pA3DDevice, szFile, AUI_COLORKEY);
	if(!bval)
	{
		a_LogOutput(0, "CDlgShopConsignView::Init(), Error while loading Money Icon");
	}
	vecItemPic.push_back(moneyPic);
	m_pList_Item->SetImageList(&vecItemPic);
	m_pList_Item->SetItemMask(0xffffffff);
	m_pList_Item->SetAlign(1);

	cacheAll.lastPage = -1;

	m_DataViewChar = new CDlgDataConsignViewChar();
	
	return true;
}

void CDlgShopConsignView::_InsertSellTypes()
{
	m_pTree_ItemTree->DeleteAllItems();
	AString pTreeFileName("Configs\\typeDef_initial.xml");
	AFileImage AFI;
	
	if(!AFI.Open(pTreeFileName , AFILE_NOHEAD | AFILE_OPENEXIST | AFILE_TEXT | AFILE_TEMPMEMORY)) 
	{
		a_LogOutput(1, "CDlgShopConsignView::_InsertSellTypes(), Failed to load %s", pTreeFileName);
		pTreeFileName.Format("Interfaces\\typeDef_initial.xml");
		if(!AFI.Open(pTreeFileName , AFILE_NOHEAD | AFILE_OPENEXIST | AFILE_TEXT | AFILE_TEMPMEMORY)) 
		{
			a_LogOutput(1, "CDlgShopConsignView::_InsertSellTypes(), Failed to load %s", pTreeFileName);
			return;
		}
	}
	char *buf = new char[AFI.GetFileLength() + 2];
	DWORD dwLen;
	bool bRet = true;
	if( AFI.Read(buf, AFI.GetFileLength(), &dwLen) )
	{
		buf[dwLen] = '\0';
		buf[dwLen + 1] = '\0';
		AWString tempBuf = AS2WC(buf);
		TreeFile.LoadFromText(tempBuf.GetBuffer(0));
	}
	delete []buf;
	AFI.Close();
	
	AXMLItem* pRoot = TreeFile.GetRootItem();
	AXMLItem* pItem = pRoot->GetFirstChildItem();
	if( !pItem)
	{
		a_LogOutput(1, "CDlgShopConsignView::_InsertSellTypes(), Error while loading TreeList");
		return;
	}
	
	P_AUITREEVIEW_ITEM pTreeRoot = m_pTree_ItemTree->GetRootItem();
	while(pItem)
	{
		if(a_strcmp(pItem->GetName() , _AL("object-array")) == 0) pItem = pItem->GetFirstChildItem();
		else if (a_strcmp(pItem->GetName() , _AL("ItemType")) == 0)
		{
			AWString group,group1;
			int value;
			XMLGET_INT_VALUE(pItem , _AL("id") , value);
			XMLGET_WSTRING_VALUE(pItem ,  _AL("group0") , group);
			XMLGET_WSTRING_VALUE(pItem ,  _AL("group1") , group1);
			
			P_AUITREEVIEW_ITEM pTempItem;
			if(!m_pTree_ItemTree->ItemHasChildren(pTreeRoot))
			{
				m_pTree_ItemTree->InsertItem(group);
				pTempItem = m_pTree_ItemTree->GetFirstChildItem(pTreeRoot);
			}
			else
			{
				pTempItem = m_pTree_ItemTree->GetFirstChildItem(pTreeRoot);
				while(pTempItem)
				{
					if(a_strcmp(m_pTree_ItemTree->GetItemText(pTempItem) , group) == 0)
						break;
					pTempItem = m_pTree_ItemTree->GetNextSiblingItem(pTempItem);
				}
				if(!pTempItem)
				{
					pTempItem = m_pTree_ItemTree->InsertItem(group,pTreeRoot);
				}
			}	
			P_AUITREEVIEW_ITEM pTempItem2 = m_pTree_ItemTree->InsertItem(group1 , pTempItem );
			m_pTree_ItemTree->SetItemData(pTempItem2, value);
			pItem = pItem->GetNextItem();
		}
		else pItem = pItem->GetFirstChildItem();
	}
	
	//	if(!TreeFile.LoadFromFile(pTreeFileName))
	//		return AUI_ReportError(__LINE__, 1, "AUIDialog::Init(), Failed to load %s", pTreeFileName);
	//A3DRELEASE(pTreeFile);
	TreeFile.Release();
	
	m_pTree_ItemTree->SetItemData(m_pTree_ItemTree->GetRootItem() , ALL_CATEGORY);
}

void CDlgShopConsignView::OnShowDialog()
{
//	OnCommand_Refresh("");
	m_pLabelSub->Show(false);
	m_pLabel->Show(false);
	m_pLabelStart->Show(true);
	m_iCurPageItems = 0;
	//iCurrentType = 0;
	iCurrentType = ALL_CATEGORY;
	m_pAction = (CDlgShopConsignAction*)GetGameUIMan()->GetDialog("Win_ShopConsignAction");
	m_pTree_ItemTree->SelectItem(m_pTree_ItemTree->GetRootItem());

	P_AUITREEVIEW_ITEM pTempItem = m_pTree_ItemTree->GetRootItem();
	pTempItem = m_pTree_ItemTree->GetFirstChildItem(pTempItem);
	while(pTempItem)
	{
		m_pTree_ItemTree->Expand(pTempItem,1);
		pTempItem = m_pTree_ItemTree->GetNextSiblingItem(pTempItem);
	}
	//SendData(ALL_CATEGORY , 0 , 0 ,0);
	ClearCacheCategory(ALL_CATEGORY);
	curCache.clear();
	if (GetCheckedRadioButton(3)==2)
	{
		OnCommand_AllList("");
		Refresh(0 , ALL_CATEGORY , 2);
	}
	else //if (GetCheckedRadioButton(3)==3)
	{
		OnCommand_CharList("");
		m_DataViewChar->Refresh();
	}
}

void CDlgShopConsignView::OnTick()
{
	if (GetCheckedRadioButton(3)==2)
	{
		_TickItems();
	}
	else //if (GetCheckedRadioButton(3)==3)
	{
		_TickCharList();
	}
}

void CDlgShopConsignView::_TickItems()
{
	P_AUITREEVIEW_ITEM pItem = m_pTree_ItemTree->GetSelectedItem();
	if(pItem)
	{
		int newType = m_pTree_ItemTree->GetItemData(pItem);
		if(iCurrentType != newType)
		{
			//iCurrentType = newType;
			//m_iCurPageItems = 0;
			curCache.clear();
			Refresh(0,newType);
		}
		ASSERT((iCurrentType ==0) || (iCurrentType >= MIN_CATEGORY && iCurrentType <= MAX_CATEGORY) || (iCurrentType == ALL_CATEGORY));
	}

	if(m_iCurPageItems < -1)
	{
		a_LogOutput(0, "CDlgShopConsignView::Tick(), CurrentPage Error: %i", m_iCurPageItems);
		m_iCurPageItems = 0;
	}
	if(m_iLastPageCurType > -1 && m_iCurPageItems > m_iLastPageCurType)
		m_iCurPageItems = m_iLastPageCurType;
	if(m_iLastPageCurType != -1 && m_iCurPageItems == -1)
		m_iCurPageItems = m_iLastPageCurType;
//	if(iCurrentType < MIN_CATEGORY || iCurrentType > MAX_CATEGORY) return;
	BuildItemList();
	//if(pCurTreeItem)
	//	m_pTree_ItemTree->SelectItem(pCurTreeItem);
}

void CDlgShopConsignView::_TickCharList()
{
	m_pLabelStart->Show(false);
	CharListVector* pData =m_DataViewChar->UpdateView();
	if ( pData )
	{
		CharListVector &charList = *pData;
		m_pList_Char->ResetContent();
		unsigned int i;
		ACString strText, strProf, strName, strInfo, strMoney;
		for (i=0; i<charList.size(); ++i)
		{
			GConsignListRoleNode node = charList[i];
			GConsignItem item = node.info;
			strInfo = GetGameUIMan()->GetStringFromTable(10242+item.state);
			strProf = GetGameUIMan()->GetStringFromTable(2101+node.cls);
			if (strProf.GetLength()==3)		// 鬼王宗 要显示成 鬼王
				strProf.CutRight(1);

			strProf = GetGameUIMan()->GetStringFromTable(7000+node.cultivation)+strProf;
			strName = ACString((const ACHAR *)node.seller_name.begin(), node.seller_name.size() / sizeof(ACHAR));
			if (item.state==CDlgShopConsign::STATE_SELL)
			{
				int iRemainTime = item.sell_endtime - g_pGame->GetServerGMTTime();
				if (iRemainTime > 24*3600)
				{
					strInfo.Format(GetStringFromTable(1610),iRemainTime/(24*3600), (iRemainTime%(24*3600))/3600);
				}
				else if (iRemainTime > 3600)
				{
					strInfo.Format(GetStringFromTable(1611),iRemainTime/3600, (iRemainTime%3600)/60);
				}
				else if (iRemainTime > 60)
				{
					strInfo.Format(GetStringFromTable(1609),iRemainTime/60, (iRemainTime%60));
				}
				else if (iRemainTime > 0)
				{
					strInfo.Format(GetStringFromTable(1601),iRemainTime);
				}
				else
					strInfo = _AL("---");
			}
			if (item.state==CDlgShopConsign::STATE_SHOW || item.state==CDlgShopConsign::STATE_SELL)
			{
				ACString strTemp;
				if (item.price%100==0)
					strTemp.Format(_AL("%d"), item.price/100);
				else
					strTemp.Format(_AL("%d.%02d"), item.price/100, item.price%100); 
				strMoney.Format(GetGameUIMan()->GetStringFromTable(10241), strTemp);
			}
			else
			strMoney = _AL("---");
			strText.Format(_AL("%s\t%d\t%s\t%s\t%s\t%s"), strProf, node.level, strName, strInfo, strMoney, strName);
			m_pList_Char->AddString(strText);
			m_pList_Char->SetItemDataPtr(i, &((*pData)[i]), 0, "sell_char_info");
		}
	}
}

void CDlgShopConsignView::BuildItemList()
{
	int sel = m_pList_Item->GetCurSel();
	if(GetTickCount() - lastRequestTime > 500)
		m_pList_Item->ResetContent(); 
	if((iCurrentType < MIN_CATEGORY || iCurrentType > MAX_CATEGORY) && iCurrentType != ALL_CATEGORY)
	{
		m_pLabel->Show(false);
		if(m_pTree_ItemTree->GetSelectedItem() && m_pTree_ItemTree->GetSelectedItem() != m_pTree_ItemTree->GetRootItem())
		{
			m_pLabelSub->Show(true);
			m_pLabelStart->Show(false);
		}
		else
		{
			m_pLabelStart->Show(true);
			m_pLabelSub->Show(false);
		}
		return;
	}
	m_pLabelStart->Show(false);
	m_pLabelSub->Show(false);
	if(m_cache.empty() && iCurrentType != ALL_CATEGORY)
	{
		return;
	}
	CacheCategory* cacheC;
	if(iCurrentType == ALL_CATEGORY)
	{
		cacheC = &cacheAll;
	}
	else if(iCurrentType >= MIN_CATEGORY && iCurrentType <= MAX_CATEGORY)
	{
		cacheMap::iterator it = m_cache.find(iCurrentType);
		if(it != m_cache.end())
		{
			cacheC = m_cache[iCurrentType];
		}
		else
		{
			DWORD now = GetTickCount();
			if(now - lastRequestTime > 500)
				Refresh(m_iCurPageItems, iCurrentType ,2);
			m_iLastPageCurType = -1;
			return;
		}
	}
	else
	{
		m_iLastPageCurType = -1;
		return;
	}
	m_iLastPageCurType = cacheC->lastPage;
	if(m_iCurPageItems == -1)
	{
		if(m_iLastPageCurType != -1) 
			m_iCurPageItems = m_iLastPageCurType;
		else
			return;
	}
	abase::hash_map<int , CachePage* >& cacheList = cacheC->cache;
	abase::hash_map<int , CachePage* >::iterator pos = cacheList.find(m_iCurPageItems);
	if(pos != cacheList.end())
	{
		if(!m_bLoaded || iCurrentType == ALL_CATEGORY)
			curCache = *cacheList[m_iCurPageItems];
		m_pList_Item->ResetContent();
	}
	else
	{
		DWORD now = GetTickCount();
		if(now - lastRequestTime > 500)
			Refresh(m_iCurPageItems, iCurrentType ,2);
		return;
	}
	if(curCache.getType() != iCurrentType || curCache.getPage() != m_iCurPageItems)
		return;
	ACString strTemp;
	//abase::vector<ItemData*>::iterator it = cache->vList.begin();
	int size = curCache.getList()->size();
	if(size == 0)
	{
		m_pLabel->Show(true);
		return;
	}
	else
	{
		m_pLabel->Show(false);
	}
	startIndex = curCache.startIndex;
	endIndex = curCache.endIndex;
	for(int i =0;i< size;i++)
	{
		ItemData itemData = curCache.getList()->at(i);
		GConsignItem item = itemData.data;
		ACString strVendor = itemData.vendor;
		ACString strItem , strInfo, strImage , strMoney , strText;
//		CECIvtrItem* tempItem = CECIvtrItem::CreateItem(item->data.item_id,0,1);
//		strTemp = tempItem->GetName();
//		delete tempItem;
//		ACString strPrice;
		strInfo = GetStringFromTable(10242+item.state);
		if (item.state==CDlgShopConsign::STATE_SELL)
		{
			int iRemainTime = item.sell_endtime - g_pGame->GetServerGMTTime();
			if (iRemainTime > 24*3600)
			{
				strInfo.Format(GetStringFromTable(1610),iRemainTime/(24*3600), (iRemainTime%(24*3600))/3600);
			}
			else if (iRemainTime > 3600)
			{
				strInfo.Format(GetStringFromTable(1611),iRemainTime/3600, (iRemainTime%3600)/60);
			}
			else if (iRemainTime > 60)
			{
				strInfo.Format(GetStringFromTable(1609),iRemainTime/60, (iRemainTime%60));
			}
			else if (iRemainTime > 0)
			{
				strInfo.Format(GetStringFromTable(1601),iRemainTime);
			}
			else
				strInfo = _AL("---");
		}
		if (item.state==CDlgShopConsign::STATE_SHOW || item.state==CDlgShopConsign::STATE_SELL)
		{
			ACString strTemp;
			if (item.price%100==0)
				strTemp.Format(_AL("%d"), item.price/100);
			else
				strTemp.Format(_AL("%d.%02d"), item.price/100, item.price%100);
			strMoney.Format(GetGameUIMan()->GetStringFromTable(10241), strTemp);
		}
		else
			strMoney = _AL("---");
		ACString strTemp;
			//GetGameUIMan()->Int64ToStr(item.sn , strTemp);
		strItem.Format(_AL(""));
		if (item.consign_type==2)	// 1:money	2:item
		{
			CECIvtrItem* pIvtrItem = CECIvtrItem::CreateItem(item.item_id, 0, 1);
			//strItem = pItem->GetName();
			AString strIndex;
			af_GetFileTitle(pIvtrItem->GetIconFile(), strIndex);
			strIndex.MakeLower();
			int index = GetGameUIMan()->GetIconId(CECGameUIMan::ICONS_INVENTORY , strIndex);
			EditBoxItemBase tempItem(enumEIImage);
			tempItem.SetImageIndex(0);
			tempItem.SetImageFrame(index);
			strImage  = (ACHAR)AUICOMMON_ITEM_CODE_START + tempItem.Serialize();
			strItem = /*strImage + strTemp + */pIvtrItem->GetName();
			delete pIvtrItem;
			if (item.item_cnt>1)
			{
				ACString strPostfix;
				strPostfix.Format(_AL(" x%d"), item.item_cnt);
				strItem += strPostfix;
			}
		}
		else
		{
			EditBoxItemBase tempItem(enumEIImage);
			tempItem.SetImageIndex(1);
			tempItem.SetImageFrame(0);
			strImage  = (ACHAR)AUICOMMON_ITEM_CODE_START + tempItem.Serialize();
			strItem.Format(GetGameUIMan()->GetStringFromTable(1704), item.money/10000);
			strItem = strTemp + strItem;
			//strImage = _AL( " ");
		}
		strText.Format(_AL("%s\t%s\t%s\t%s\t%s"), strImage,strItem, strInfo,strMoney,strVendor);
		m_pList_Item->AddString(strText);
		//ItemForRButton* tempData = new ItemForRButton(item.sn , strVendor);
		m_pList_Item->SetItemDataPtr(i, &(curCache.getList()->at(i)));
		if(item.consign_type == 2)
		{
			abase::hash_map<int64_t , ACString>::iterator pos = m_ItemDetail.find(item.sn);
			if(pos != m_ItemDetail.end())
			{
				m_pList_Item->SetItemHint(i , pos->second);
			}
		}
		m_bLoaded = true;
	}
	int itemIndex = m_pList_Item->GetHOverItem();
	if(itemIndex >=0 && itemIndex < m_pList_Item->GetCount())
	{
		if(iCurrentType != MONEY_CATEGORY)
		{
			GConsignItem data = ((ItemData*)m_pList_Item->GetItemDataPtr(itemIndex))->data;
			if(data.consign_type == 2)
			{
				ACString strHint(m_pList_Item->GetItemHint(itemIndex));
				if(strHint.GetLength() <= 0)
					requestDetail(data.sn);
			}
		}
	}
	
	m_pList_Item->SetCurSel(sel);
}

CDlgShopConsignView::CDlgShopConsignView()
{
}

CDlgShopConsignView::~CDlgShopConsignView()
{
}

int CDlgShopConsignView::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if(strMsg == "listall_re")	// wParam : Protocol pointer
	{
		ConsignListAll_Re* listAll = (ConsignListAll_Re*)wParam;
		int retCode = listAll->retcode;
		int iType = listAll->category;
		int iPage = listAll->page;
		
		if(iType > MAX_CATEGORY || iType < MIN_CATEGORY)
		{
			a_LogOutput(0,"CDlgShopConsignView::CommonMessage(), Error while receiving Category = %i" , iType);
		}
		if(iPage < 0)
		{
			a_LogOutput(0,"CDlgShopConsignView::CommonMessage(), Error while receiving Page = %i" , iPage);
		}
		if(retCode == 17) 
		{
			//pCacheCategory->lastPage = iPage;
		}
		else if(retCode == 0)
		{
			if(listAll->nodes.size() != NUMSTOREPERPAGE)
			{
				a_LogOutput(0,"CDlgShopConsignView::CommonMessage(), Error Page %i is not full" , iPage);
			}
		}
		else
		{
			GetGameUIMan()->MessageBox(10290+retCode);
			return 1;
		}
		
		cacheMap::iterator it = m_cache.find(iType);
		CacheCategory* pCacheCategory;
		if(it != m_cache.end())
		{
			pCacheCategory = it->second;
		}
		else
		{
			pCacheCategory = new CacheCategory;
			pCacheCategory->lastPage = -1;
			m_cache[iType] = pCacheCategory;
		}
		if(retCode == 17) 
		{
			pCacheCategory->lastPage = iPage;
		}

		abase::hash_map<int , CachePage* >& cacheList = pCacheCategory->cache;
		if(listAll->nodes.size() > 0) 
		{
			std::vector<GConsignListNode>::iterator it = listAll->nodes.begin();
			cacheList[iPage] = new CachePage(iType, iPage ,listAll->nodes);
		}
		else if(iPage == 0 && listAll->nodes.size() == 0)
		{ 
			cacheList[iPage] = new CachePage(iType, iPage ,listAll->nodes);
		}
		else
		{
			pCacheCategory->lastPage = iPage -1;
		}
	}
	
	else if(strMsg == "getitem_re")
	{
		ConsignGetItem_Re* itemDetail = (ConsignGetItem_Re*)wParam;
		int retCode = itemDetail->retcode;
		if(retCode == 1)
		{
			GetGameUIMan()->MessageBox(10290);
			return 1;
		}
		else if(retCode == 2)
		{
			//GetGameUIMan()->MessageBox(10308);
			ClearCacheCategory(iCurrentType);
			curCache.clear();
			Refresh(m_iCurPageItems , iCurrentType , 2);
			return 1;
		}
		int64_t sn = itemDetail->sn;
		GRoleInventory ivtrItem = itemDetail->obj;
		unsigned int id = ivtrItem.id;
		CECIvtrItem* pItem = CECIvtrItem::CreateItem(id , 0 ,1);
		if( ivtrItem.data.begin() && ivtrItem.data.size() )
		{			
			if (pItem->GetClassID()==CECIvtrItem::ICID_TALISMAN_MAINPART)
				((CECIvtrTalismanMain*)pItem)->SetItemFromMailInfo((BYTE*)ivtrItem.data.begin(), ivtrItem.data.size());
			else
				pItem->SetItemInfo((BYTE*)ivtrItem.data.begin(), ivtrItem.data.size());
		}
		pItem->SetNeedUpdate(false);
		pItem->UpdateInfoMD5();
		ACString strDesc = pItem->GetDesc(CECIvtrItem::DESC_NORMAL);
		if(strDesc)
		{
			//AUI_ReportError(__LINE__, 1, "CDlgShopConsignView::BuildItemList(), Failed to Read Cache %s ", strDesc);
			AUICTranslate trans;
			strDesc = trans.Translate(strDesc);
			m_ItemDetail[sn] = strDesc;
		}
		delete pItem;
		//m_bLoaded = false;
	}
	else if( strMsg == "getLargeCategory_re")
	{
		ConsignListLargeCategory_Re* largeCategory = (ConsignListLargeCategory_Re*)wParam;
		int retCode = largeCategory->retcode;
		int iPage = largeCategory->page;
		int tstamp = largeCategory->timestamp;
		if(tstamp != m_iTimestampItems)
		{
			cacheAll.clear();
		}
		m_iTimestampItems = tstamp;
		std::vector<GConsignListNode> nodes = largeCategory->nodes;
		if(retCode == 17) 
		{
			cacheAll.lastPage = iPage;
		}
		else if(retCode == 0)
		{
			ASSERT(largeCategory->nodes.size() == NUMSTOREPERPAGE);
		}
		else
		{
			GetGameUIMan()->MessageBox(10290+retCode);
			return 1;
		}
		abase::hash_map<int , CachePage* >& cacheList = cacheAll.cache;
		if(largeCategory->nodes.size() > 0) 
		{
			std::vector<GConsignListNode>::iterator it = largeCategory->nodes.begin();
			cacheList[iPage] = new CachePage(ALL_CATEGORY, iPage ,largeCategory->nodes);
		}
		else if(iPage == 0 && largeCategory->nodes.size() == 0)
		{ 
			cacheList[iPage] = new CachePage(ALL_CATEGORY, iPage ,largeCategory->nodes);
		}
		else
		{
			cacheAll.lastPage = iPage -1;
		}
		
	}	
	else if( strMsg == "role_list")
	{
		m_DataViewChar->LoadFromProtocol((void*)wParam);
	}
	return 1;
}

void CDlgShopConsignView::OnCommand_MyList(const char* szCommand)
{
	
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_ShopConsign");
	if( pDlg )
	{
		Show(false);
		pDlg->Show(true);
		pDlg->SetPosEx(GetPos().x, GetPos().y);
		GetGameUIMan()->GetDialog("Win_Inventory")->Show(true);
		pDlg->CheckRadioButton(3,1);
//		m_pTree_ItemTree->DeleteAllItems();
		curCache.clear();
	}
}

void CDlgShopConsignView::OnCommand_AllList(const char* szCommand)
{
	GetDlgItem("Btn_ListName")->Show(true);
	GetDlgItem("Btn_CharProf")->Show(false);
	GetDlgItem("Btn_CharLv")->Show(false);
	GetDlgItem("Btn_CharName")->Show(false);
	GetDlgItem("Btn_Reset")->Show(true);
	
	GetDlgItem("Lst_AllGoods")->Show(true);
	GetDlgItem("Lst_AllChar")->Show(false);
	
	_InsertSellTypes();
}

void CDlgShopConsignView::OnCommand_CharList(const char* szCommand)
{
	GetDlgItem("Btn_ListName")->Show(false);
	GetDlgItem("Btn_CharProf")->Show(true);
	GetDlgItem("Btn_CharLv")->Show(true);
	GetDlgItem("Btn_CharName")->Show(true);
	GetDlgItem("Btn_Reset")->Show(false);
	
	m_pLabelStart->Show(false);
	m_pLabelSub->Show(false);
	m_pLabel->Show(false);

	GetDlgItem("Lst_AllGoods")->Show(false);
	GetDlgItem("Lst_AllChar")->Show(true);

	m_pTree_ItemTree->DeleteAllItems();
	m_pTree_ItemTree->InsertItem(GetGame()->GetGameRun()->GetServerName());
}

void CDlgShopConsignView::OnCommand_WebSite(const char* szCommand)
{
	ShellExecuteA(NULL, "open", AC2AS(GetGameUIMan()->GetStringFromTable(10280)), NULL, NULL, SW_SHOWNORMAL);
}

void CDlgShopConsignView::OnCommand_Refresh(const char* szCommand)
{
	if (GetCheckedRadioButton(3)==3)
	{
		m_DataViewChar->Refresh();
	}
	else
	{
		ClearCacheCategory(iCurrentType);
		curCache.clear();
		Refresh(m_iCurPageItems,iCurrentType,2);
	}
}

void CDlgShopConsignView::OnCommand_FirstPage(const char* szCommand)
{
	if (GetCheckedRadioButton(3)==2)
		Refresh(0,iCurrentType);
	else
		m_DataViewChar->FirstPage();
}

void CDlgShopConsignView::OnCommand_PrevPage(const char* szCommand)
{
	if (GetCheckedRadioButton(3)==2)
	{
		if(m_iCurPageItems == 0) ;
		else 
			Refresh(m_iCurPageItems -1 ,iCurrentType , 1);
	}
	else
		m_DataViewChar->PrevPage();
}

void CDlgShopConsignView::OnCommand_NextPage(const char* szCommand)
{
	if (GetCheckedRadioButton(3)==2)
	{
		if(m_iCurPageItems != m_iLastPageCurType) Refresh(m_iCurPageItems +1 , iCurrentType , 0);
	}
	else
		m_DataViewChar->NextPage();
}

void CDlgShopConsignView::OnCommand_LastPage(const char* szCommand)
{
	if (GetCheckedRadioButton(3)==2)
		Refresh(m_iLastPageCurType , iCurrentType);
	else
		m_DataViewChar->LastPage();
}

void CDlgShopConsignView::OnCommand_SortByProf(const char* szCommand)
{
	m_DataViewChar->SortByProf();
}

void CDlgShopConsignView::OnCommand_SortByLevel(const char* szCommand)
{
	m_DataViewChar->SortByLevel();
}

void CDlgShopConsignView::OnCommand_SortByName(const char* szCommand)
{
	if (GetCheckedRadioButton(3)==3)
	{
		m_DataViewChar->SortByName();
		return;
	}
	if(iCurrentType == ALL_CATEGORY)
	{
		return;
	}
	static int bAscent = true;
	ItemList* list = curCache.getList();
	if (bAscent)
		BubbleSort(list->begin(), list->end(), CompareListByNameAscent());
	else
		BubbleSort(list->begin(), list->end(), CompareListByNameDescent());
	bAscent = !bAscent;
}

void CDlgShopConsignView::OnCommand_SortByPrice(const char* szCommand)
{
	if (GetCheckedRadioButton(3)==3)
	{
		m_DataViewChar->SortByPrice();
		return;
	}
	if(iCurrentType == ALL_CATEGORY)
	{
		return;
	}
	static int bAscent = true;
	ItemList* list = curCache.getList();
	if (bAscent)
		BubbleSort(list->begin(), list->end(), CompareListByPriceAscent());
	else
		BubbleSort(list->begin(), list->end(), CompareListByPriceDescent());
	bAscent = !bAscent;
}

void CDlgShopConsignView::OnCommand_SortByTime(const char* szCommand)
{
	if (GetCheckedRadioButton(3)==3)
	{
		m_DataViewChar->SortByTime();
		return;
	}
	if(iCurrentType == ALL_CATEGORY)
	{
		return;
	}
	static int bAscent = true;
	ItemList* list = curCache.getList();
	if (bAscent)
		BubbleSort(list->begin(), list->end(), CompareListByTimeAscent());
	else
		BubbleSort(list->begin(), list->end(), CompareListByTimeDescent());
	bAscent = !bAscent;
}

void CDlgShopConsignView::OnCommand_SortBySeller(const char* szCommand)
{
	if (GetCheckedRadioButton(3)==3)
	{
		m_DataViewChar->SortByName();
		return;
	}
	if(iCurrentType == ALL_CATEGORY)
	{
		return;
	}
	static int bAscent = true;
	ItemList* list = curCache.getList();
	if (bAscent)
		BubbleSort(list->begin(), list->end(), CompareListByVendorAscent());
	else
		BubbleSort(list->begin(), list->end(), CompareListByVendorDescent());
	bAscent = !bAscent;
}

void CDlgShopConsignView::OnCommand_CANCEL(const char* szCommand)
{
	CheckRadioButton(3,1);
	Show(false);
//	m_pTree_ItemTree->DeleteAllItems();
	curCache.clear();
	GetGameUIMan()->EndNPCService();
}

void CDlgShopConsignView::OnCommand_Reset(const char* szCommand)
{
	curCache.clear();
//	m_pLabel->Show(false);
	OnShowDialog();
	//SendData(iCurrentType , 100 , 0 , 0);
}

void CDlgShopConsignView::OnCommand_Help(const char * szCommand)
{
	//PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_ShopConsign_Help");
	//pDlg->Show(!pDlg->IsShow());
	AString strDlg = "Win_ShopConsign_Help";
	GetGameUIMan()->CommonMessage("Win_HelpInfo","open",(WPARAM)(&strDlg),0);
}

void CDlgShopConsignView::OnEventLButtonDownTree(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	POINT ptPos = pObj->GetPos();
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - ptPos.x - p->X;
	int y = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;
	PAUITREEVIEW pTree = (PAUITREEVIEW)pObj;
	bool bOnIcon;
	P_AUITREEVIEW_ITEM pItem = pTree->HitTest(x, y, &bOnIcon);
	if(!pItem)
	{
//		pTree->SelectItem(pOldItem);
		return;
	}
	DWORD state = pTree->GetItemState(pItem);
	int iType = pTree->GetItemData(pItem);
	if(iType == 0)
	{
		P_AUITREEVIEW_ITEM pTempItem = pTree->GetRootItem();
		pTempItem = pTree->GetFirstChildItem(pTempItem);
		while(pTempItem)
		{
			pTree->Expand(pTempItem,1);
			pTempItem = pTree->GetNextSiblingItem(pTempItem);
		}
		if(!bOnIcon || state & 0x0001)
			pTree->Expand(pItem,0);
	}
//	if(iType != 0)
//		pTree->Expand(pItem, 0);
//	if(iType == 0 && (state & 0x0001) )
//		return;
	curCache.clear();
	if(pItem != pTree->GetSelectedItem())
	{
		pTree->SelectItem(NULL);
	}
//	m_pLabel->Show(false);
	
	//if(iType >=MIN_CATEGORY && iType <= MAX_CATEGORY ) 
		//return;
//	if(iType == 0 && (state & 0x0001))
//	{
//		pTempItem = pTree->GetFirstChildItem(pItem);
//		while(pTempItem)
//		{
//			if((int)pTree->GetItemData(pTempItem) == iCurrentType)
//			{
//				pTree->SelectItem(pTempItem);
//				break;
//			}
//			pTempItem = pTree->GetNextSiblingItem(pTempItem);
//		}
//	}
//	pOldItem = pTree->GetSelectedItem();
	Refresh(0,iType);
}

void CDlgShopConsignView::OnEventRButtonUpGoods(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	POINT ptPos = pObj->GetPos();
	int nCurSel = m_pList_Item->GetCurSel();
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - ptPos.x - p->X;
	int y = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;
	
	if( nCurSel >= 0 && nCurSel < m_pList_Item->GetCount() &&
		m_pList_Item->GetHitArea(x, y) == AUILISTBOX_RECT_TEXT )
	{
		ItemData* item = (ItemData*)m_pList_Item->GetItemDataPtr(nCurSel);
		x = GET_X_LPARAM(lParam) - p->X;
		y = GET_Y_LPARAM(lParam) - p->Y;
		m_pAction->SetPosEx(x,y,alignNone,alignNone,this);
		if(item)
		{
			m_pAction->SetData(item->data.sn, item->vendor, item->commodityid);
			m_pAction->Show(true);
		}
		else
		{
			ASSERT(item);
		}

	}
}

void CDlgShopConsignView::OnEventRButtonUpChars(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	POINT ptPos = pObj->GetPos();
	int nCurSel = m_pList_Char->GetCurSel();
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - ptPos.x - p->X;
	int y = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;
	
	if( nCurSel >= 0 && nCurSel < m_pList_Char->GetCount() &&
		m_pList_Char->GetHitArea(x, y) == AUILISTBOX_RECT_TEXT )
	{
		ASSERT(m_pList_Char->GetItemDataPtr(nCurSel, 0, "sell_char_info"));
		GConsignListRoleNode &node = *((GConsignListRoleNode*)m_pList_Char->GetItemDataPtr(nCurSel, 0, "sell_char_info"));
		ACString strName = ACString((const ACHAR *)node.seller_name.begin(), node.seller_name.size() / sizeof(ACHAR));
		x = GET_X_LPARAM(lParam) - p->X;
		y = GET_Y_LPARAM(lParam) - p->Y;
		m_pAction->SetPosEx(x,y,alignNone,alignNone,this);
		m_pAction->SetData(0, strName, node.commodityid);
		m_pAction->Show(true);
	}
}

void CDlgShopConsignView::Refresh(int iPage , int iType , char dir)
{
	m_iCurPageItems = iPage;
	iCurrentType = iType;
	if((iType < MIN_CATEGORY || iType > MAX_CATEGORY) && (iType != ALL_CATEGORY))
	{
		return;
	}
	m_bLoaded = false;
	if(iType == ALL_CATEGORY)
	{
		CacheCategory* cacheC = &cacheAll;
		abase::hash_map<int , CachePage* >& cacheList = cacheC->cache;
		abase::hash_map<int , CachePage* >::iterator pos = cacheList.find(m_iCurPageItems);
		if(pos != cacheList.end())
		{
			CachePage* cPage = pos->second;
			if(GetTickCount() - cPage->getTime() >= CACHETIME * 1000)
				cPage->clear();
			else
			{
				return;
			}			
		}
	}
	else if(!m_cache.empty())
	{
		cacheMap::iterator it = m_cache.find(iCurrentType);
		if(it != m_cache.end())
		{
			CacheCategory* cacheC = m_cache[iCurrentType];
			abase::hash_map<int , CachePage* >& cacheList = cacheC->cache;
			//CachePage* cache;
			abase::hash_map<int , CachePage* >::iterator pos = cacheList.find(m_iCurPageItems);
			if(pos != cacheList.end())
			{
				CachePage* cPage = pos->second;
				if(GetTickCount() - cPage->getTime() >= CACHETIME * 1000)
					cPage->clear();
				else
				{
					return;
				}	
			}
		}
	}

	requestList(iType ,iPage , dir);
	//BuildItemList();
}

void CDlgShopConsignView::requestList(int c , int page , char direction)
{
	//m_bLoaded = false;
	if(direction == 2) //current Page
	{
		SendData(iCurrentType , m_iCurPageItems , 0 , 0);
		return ;
	}
	else if (direction == 1)	// prev Page
	{
		SendData(iCurrentType , m_iCurPageItems, startIndex , direction);
		return;
	}
	else	//prev page
	{
		SendData(iCurrentType , m_iCurPageItems , endIndex , direction);
		return ;
	}	
	ASSERT(0);
}

void CDlgShopConsignView::requestDetail(int64_t sn)
{
	int nowTime = GetTickCount();
	if(lastReqSN == sn && (nowTime - lastReqDetailTime) < 2000) return;
	lastReqSN = sn;
	lastReqDetailTime = GetTickCount();
	GetGameSession()->SendNetData(GNET::ConsignGetItem(g_pGame->GetGameRun()->GetHostPlayer()->GetCharacterID(),
		sn , 0 ));
}

void CDlgShopConsignView::SendData(int type , int page , int64_t index , char dir)
{
	lastRequestTime = GetTickCount();
	if(type == ALL_CATEGORY)
	{
		GetGameSession()->SendNetData(GNET::ConsignListLargeCategory(g_pGame->GetGameRun()->GetHostPlayer()->GetCharacterID(), 
		page ,  0));
	}
	else
	{
		GetGameSession()->SendNetData(GNET::ConsignListAll(g_pGame->GetGameRun()->GetHostPlayer()->GetCharacterID(), 
		type , page , index , dir , 0));
	}

}

bool CDlgShopConsignView::ClearCacheCategory(int type)
{
	if(type == ALL_CATEGORY)
	{
		cacheAll.clear();
		return true;
	}
	cacheMap::iterator it = m_cache.find(type);
	//CacheCategory* pCacheCategory;
	if(it != m_cache.end())
	{
		CacheCategory* cc = it->second;
		cc->clear();
	}
	return true;
}

bool CDlgShopConsignView::ClearCache(DWORD dwTime , bool clearAll)
{
	cacheMap::iterator it = m_cache.begin();
	while(it != m_cache.end())
	{
		CacheCategory* cc = it->second;
		cc->clear();
		++it;
		if(clearAll)
			delete cc;
	}
	cacheAll.clear();
	return true;
}
