// Filename	: EC_GameUIMisc.cpp
// Creator	: Fuchonggang
// Date		: 2010.6.23
// Desc		: 界面相关公用类型


#include "EC_GameUIMisc.h"
#include "AUI\\AUICommon.h"
#include "AUI\\AUITextArea.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUILabel.h"
#include "AUI\\CSplit.h"
#include "AUI\\AUICommon.h"
#include "AUI\\A3DFTFont.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_Resource.h"
#include "EC_GameRun.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "A3DEngine.h"
#include "A3DViewport.h"

const unsigned int idFeitian				= 8079;
const unsigned int CECGAMEUIMAN_MAX_MSGS	= 1000;
const unsigned int idFlyItem				= 8079;

void FilterTextColor(ACString& str)
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

void ConvertChatString(ACString& str)
{
	ACHAR szText[512];
	AUI_ConvertChatString(str, szText);
	str = szText;
}

int GetLengthExpItem(const ACHAR * const szText)
{
	const ACHAR* szTemp = szText;
	const ACHAR* szStart = szText;
	int nLen = 0;
	int nCurScriptIndex = 0;
	
	while (*szTemp)
	{
		wchar_t ch = *szTemp;
		
		if (IsEditboxItemCode(ch))
		{
			if (szTemp > szStart)
				nLen += szTemp - szStart;
			
			szTemp++;
			EditBoxItemBase* pItem = EditBoxItemBase::Unserialize(szTemp);
			szStart = szTemp;
 			if (pItem)
			{
				delete pItem;
//item是否算多少字符？ 			
// 				nLen++;
			}
		}
		else
			szTemp++;
	}
	
	if (szTemp > szStart)
		nLen += szTemp - szStart;
	return nLen;
}

bool GetItemLinkOnText(int x, int y, PAUITEXTAREA pText, AUITEXTAREA_EDITBOX_ITEM *pLink)
{
	bool bInFrame = false;
	A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
	POINT ptPos = pText->GetPos();
	
	g_pGame->ChangeCursor(RES_CUR_NORMAL);
	if( pText->GetHitArea(x - ptPos.x, y - ptPos.y) == AUITEXTAREA_RECT_FRAME )
	{
		int i;
		abase::vector<AUITEXTAREA_EDITBOX_ITEM> &vecItemLink = pText->GetEditBoxItems();
		
		x += p->X;
		y += p->Y;
		for( i = 0; i < (int)vecItemLink.size(); i++ )
		{
			if( (vecItemLink[i].m_pItem->GetType() == enumEIIvtrlItem ||
				vecItemLink[i].m_pItem->GetType() == enumEICoord ||
				vecItemLink[i].m_pItem->GetType() == enumEIImage) &&
				vecItemLink[i].rc.PtInRect(x, y) )
			{
				g_pGame->ChangeCursor(RES_CUR_HAND);
				*pLink = vecItemLink[i];
				break;
			}
		}
		bInFrame = true;
	}
	
	return bInFrame;
}

bool GetItemLinkOnList(int x, int y, AUIListBox* pLst)
{
	POINT ptPos = pLst->GetPos();
	int hitArea = pLst->GetHitArea(x-ptPos.x, y-ptPos.y);
	g_pGame->ChangeCursor(RES_CUR_NORMAL);
	if (hitArea==AUILISTBOX_RECT_TEXT)
	{
		g_pGame->ChangeCursor(RES_CUR_HAND);
		return true;
	}
	return false;
}

bool SetImageFileProperty(AUIImagePicture* pImg, const char* path, int size)
{
	AUIOBJECT_SETPROPERTY p;
	strcpy(p.fn, path);
	pImg->SetProperty("Image File", &p);
	return true;
}

ACString GetCashText(int nCash, bool bFullText, bool bPoint)
{
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	ACString strText;
	if( bFullText )
		strText.Format(pGameUI->GetStringFromTable(1403), nCash / 100, nCash % 100);
	else if( bPoint )
	{
		if (nCash%100 == 0)
		{
			strText.Format(pGameUI->GetStringFromTable(1401), nCash/100);
		}
		else
		{
			if (nCash%10 == 0)
			{
				strText.Format(pGameUI->GetStringFromTable(1401), (nCash%100)/10);
			}
			else
			{
				strText.Format(pGameUI->GetStringFromTable(1401), nCash%100);
			}
			strText = _AL(".") + strText;
			ACString strInt;
			strInt.Format(_AL("%d"), nCash / 100);
			strText = strInt + strText;
		}
	}
	else if( nCash < 100 )
		strText.Format(pGameUI->GetStringFromTable(1402), nCash);
	else if( nCash % 100 == 0 )
		strText.Format(pGameUI->GetStringFromTable(1401), nCash / 100);
	else
		strText.Format(pGameUI->GetStringFromTable(1403), nCash / 100, nCash % 100);
	return strText;
}

ACString GetTimeText(int t)
{	
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	ACString strTime;

	if (t > 24*3600)
		strTime.Format(pGameUI->GetStringFromTable(1610), t/(24*3600), (t%(24*3600))/3600);
	else if (t > 3600)
		strTime.Format(pGameUI->GetStringFromTable(1611), t/3600, (t%3600)/60);
	else if (t > 60)
		strTime.Format(pGameUI->GetStringFromTable(1609), t/60, (t%60));
	else if (t > 0)
		strTime.Format(pGameUI->GetStringFromTable(1601), t);
	else
		strTime.Format(pGameUI->GetStringFromTable(1601), 0);
	
	return strTime;
}

ACString GetTimeText1(int t)
{	
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	ACString strTime;
	
	if (t > 24*3600)
		strTime.Format(pGameUI->GetStringFromTable(1604), t/(24*3600));
	else if (t > 3600)
		strTime.Format(pGameUI->GetStringFromTable(1603), t/3600);
	else if (t > 60)
		strTime.Format(pGameUI->GetStringFromTable(1602), t/60);
	else if (t > 0)
		strTime.Format(pGameUI->GetStringFromTable(1601), t);
	else
		strTime = pGameUI->GetStringFromTable(1600);
	
	return strTime;
}

ACString GetTimeTextNumber(int t)
{	
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	ACString strTime;
	
	if (t > 24*3600)
		strTime.Format(pGameUI->GetStringFromTable(1604), t/(24*3600));
	else if (t > 3600)
		strTime.Format(pGameUI->GetStringFromTable(1603), t/3600);
	else if (t > 60)
		strTime.Format(pGameUI->GetStringFromTable(1602), t/60);
	else if (t > 0)
		strTime.Format(pGameUI->GetStringFromTable(1601), t);
	else
		strTime.Format(pGameUI->GetStringFromTable(1601), 0);
	
	return strTime;
}

void FormatTextFixedWidth(ACString &strText, int width, A3DFTFont *pFont)
{
	if (!pFont)
	{
		CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		pFont = pGameUI->GetDefaultFont();
	}

	int i=0;
	int iCurLen = 0;
	ACString strRet;
	for (i=0; i<strText.GetLength(); ++i)
	{
		if (strText[i]==_AL('^'))
		{
			strRet += strText.Mid(i, 7);
			i+=6;
			continue;
		}
		else if (strText[i]==_AL('\r') || strText[i]==_AL('\n'))
		{
			iCurLen = 0;
		}
		int iLength = pFont->GetTextExtent(ACString(strText[i], 1), 1).x;
		iCurLen += iLength;
		if (iCurLen>width)
		{
			strRet += _AL("\r");
			iCurLen = iLength;
		}

		strRet += strText[i];
	}

	strText = strRet;
}