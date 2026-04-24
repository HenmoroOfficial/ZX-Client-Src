/*
 * FILE: EC_PateText.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/4/7
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_PateText.h"
#include "EC_Decal.h"
#include "EC_Game.h"
#include "EC_Resource.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "EC_GameRun.h"
#include "EC_World.h"
#include "EC_ManDecal.h"
#include "EC_Achievement.h"
#include "A3DGDI.h"
#include "A3DEngine.h"
#include "EC_IvtrItem.h"

#include "AUI\\A3DFTFont.h"
#include "AUI\\A3DFTFontMan.h"
#include "AUI\\AUITextArea.h"
#include "AUI\\AUIFrame.h"
#include "A2DSprite.h"
#include "DlgChat.h"

#define new A_DEBUG_NEW

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Reference to External variables and functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Local Types and Variables and Global variables
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECPateText
//	
///////////////////////////////////////////////////////////////////////////

CECPateText::CECPateText()
{
	m_iExtX			= 0;
	m_iExtY			= 0;
	m_iLines		= 0;
	m_iMaxLineLen	= 30;
	m_iMaxLines		= 5;
	m_bBorder		= false;
	m_bBackground	= false;
	m_iFontType	= TYPE_TEXT;
	
	m_pFramePopChat = new AUIFrame;
	if( !m_pFramePopChat ) { AUI_ReportError(__LINE__, 1, "AUIManager::Init(), no enough memory"); return;}
	
	bool bval = m_pFramePopChat->Init(g_pGame->GetA3DEngine(), g_pGame->GetA3DDevice(), "Frame\\ChatPop.tga");
	if( !bval ) A3DRELEASE(m_pFramePopChat);
}

CECPateText::~CECPateText()
{
	A3DRELEASE(m_pFramePopChat);
	CECPateText::Clear();
}

/*	Set text string

	Return the number of characters in szText which was added into pate text.

	bIncEmotion: should parse emotion keyword
	bEllipsis: true, add ellipsis at the end of pate text if szText is too long
*/
int CECPateText::SetText(const ACHAR* szText, bool bIncEmotion, bool bEllipsis/* true */, CECIvtrItem* pItem, sAchievement* pAchiv)
{
	//	Clear old content
	Clear();
	
	if (!szText || !szText[0])
	{
		delete pItem;
		delete pAchiv;
		return true;
	}

	ACString strName;
	A3DCOLOR color;
	if (pItem)
		CDlgChat::TransformItemColorName(pItem, strName, color);
	else
		CDlgChat::TransformAchievementColorName(pAchiv, strName, color);

	ACString str = UnmarshalEditBoxText(szText, m_ItemsSet, 0, strName, color, 0xffffffff);
	szText = str;

	// Set achievement editbox item color.
	if (pItem == NULL && pAchiv != NULL)
	{
		EditBoxItemsSet* pItemSet = &m_ItemsSet;
		EditBoxItemMap::iterator iter = pItemSet->GetItemIterator();

		while(!iter.is_eof())
		{
			EditBoxItemBase* pItem = iter->second;
			if (pItem->GetType() == enumEITask)
			{
				pItem->SetName(strName);
				pItem->SetColor(color);

				break;
			}

			++iter;
		}
	}

	int iAddedChar = 0;

	if (!bIncEmotion)
	{
		int iLen = a_strlen(szText);
		if (iLen > m_iMaxLineLen)
		{
			ACString str(szText, m_iMaxLineLen);

			//	Add '...' at string ending
			if (bEllipsis)
				str += _AL("...");

			CreateTextItem(str, -1, 0);
			iAddedChar = m_iMaxLineLen;
		}
		else
		{
			CreateTextItem(szText, -1, 0);
			iAddedChar = iLen;
		}
	}
	else	//	Should parse emotion keyword
	{
		int i=0, iStart=0, iEnd=0, iSkip=0, iLenCnt=0;
		bool bTooLong = false;
		int iLine = 0;

		while (szText[i])
		{
			ACHAR ch = szText[i];

			if (IsEditboxItemCode(ch))
			{
				if (iEnd > iStart)
					CreateTextItem(&szText[iStart], iEnd-iStart, iLine);

				EditBoxItemBase* pItem = m_ItemsSet.GetItemByChar(ch);

				if (pItem)
				{
					if (pItem->GetType() == enumEIEmotion)
					{
						int nSet = 0, nIndex = 0;
						UnmarshalEmotionInfo(pItem->GetInfo(), nSet, nIndex);
						CreateEmotionItem(nSet, nIndex, iLine);
						iLenCnt += 2;	//	a emotion take two-character place
					}
					else if(pItem->GetType() == enumEICoord)
					{
						const ACHAR* szName = pItem->GetName();
						CreateTextItem(szName, -1, iLine, pItem->GetColor());
						iLenCnt += wcslen(szName);						
					}
					else
					{
						const ACHAR* szName = pItem->GetName();
						CreateTextItem(szName, -1, iLine, pItem->GetColor());
						iLenCnt += wcslen(szName);
					}
				}

				i ++;
				iStart = i;
				iEnd = i;
				goto CheckLength;
			}

			iEnd++;
			i++;
			iLenCnt++;

		CheckLength:

			if (iLenCnt > m_iMaxLineLen)
			{
				if (iLine + 1 >= m_iMaxLines)
				{
					bTooLong = true;
					break;
				}

				if (iEnd > iStart)
					CreateTextItem(&szText[iStart], iEnd-iStart, iLine);

				iStart = i;
				iLine++;
				iLenCnt = 0;
			}
		}

		iAddedChar = i;

		if (iEnd > iStart)
		{
			if (bTooLong)
			{
				ACString strEnd(&szText[iStart], iEnd-iStart);

				//	Add '...' at string ending
				if (bEllipsis)
					strEnd += _AL("...");

				CreateTextItem(strEnd, -1, iLine);
			}
			else
				CreateTextItem(&szText[iStart], iEnd-iStart, iLine);
		}
		else if (bTooLong)
		{
			//	Add '...' at string ending
			if (bEllipsis)
				CreateTextItem(_AL("..."), -1, iLine);
		}
	}

	//	Calculate whole extent
	m_iExtX = 0;
	m_iExtY = 0;

	int iCurLine = 0;
	int iLineExtX = 0;
	int iLastLine = 0;

	for (int i=0; i < m_aItems.GetSize(); i++)
	{
		const ITEM& Item = m_aItems[i];

		if (Item.iLine != iLastLine)
		{
			iLastLine = Item.iLine;

			if (m_iExtX < iLineExtX)
				m_iExtX = iLineExtX;

			iLineExtX = Item.iExtX;
		}
		else
			iLineExtX += Item.iExtX;

		if (m_iExtY < Item.iExtY)
			m_iExtY = Item.iExtY;
	}

	m_iLines = iLastLine + 1;

	if (m_iExtX < iLineExtX)
		m_iExtX = iLineExtX;

	delete pItem;
	delete pAchiv;
	return iAddedChar;
}

//	Create a text item
bool CECPateText::CreateTextItem(const ACHAR* szText, int iNumChar, int iLine, A3DCOLOR cl)
{
	AUIManager* pUIMan = g_pGame->GetGameRun()->GetUIManager()->GetCurrentUIManPtr();
	assert (pUIMan);
	A3DFTFont* pFont = pUIMan->GetPateFont();
	assert(pFont);

	ASSERT(szText && szText[0]);

	ACString* pstr = NULL;
	if (iNumChar <= 0)
		pstr = new ACString(szText);
	else
		pstr = new ACString(szText, iNumChar);

	int iIndex = m_aTextStrs.Add(pstr);

	ITEM Item;
	Item.iType	= TYPE_TEXT;
	Item.iIndex	= iIndex;
	A3DPOINT2 sz = pFont->GetTextExtent(*pstr);
	// 计算当scale＝1时大小
	sz.x = int(sz.x / pUIMan->GetWindowScale());
	sz.y = int(sz.y / pUIMan->GetWindowScale());
	Item.iExtX  = sz.x;
	Item.iExtY	= sz.y;
	Item.iLine	= iLine;
	Item.clItem = cl;
	m_aItems.Add(Item);

	return true;
}

//	Get face index
int CECPateText::GetFaceIndex(const ACHAR* szText, int& iSkipChar)
{
/*	ASSERT(szText[0] == '#');

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	if (!pGameUI)
		return -1;

	A2DSprite* pFaceImg = NULL;
	abase::vector<AUITEXTAREA_EMOTION>* pAreaList = NULL;
	pGameUI->GetEmotionList(&pFaceImg, &pAreaList);
	if (!pFaceImg || !pAreaList)
		return -1;

	ACHAR szNum[64];

	int i = 1;
	iSkipChar = 1;

	while (szText[i])
	{
		if (szText[i] >= '0' && szText[i] <= '9')
		{
			szNum[i-1] = szText[i];
			i++;
			iSkipChar++;
		}
		else
			break;
	}

	if (i == 1 || i >= 64)
		return -1;

	szNum[i-1] = '\0';
	int iFace = a_atoi(szNum) - 1;
	if (iFace < 0 || iFace >= (int)pAreaList->size())
		return -1;
*/
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	if (!pGameUI)
		return -1;

	A2DSprite* pFaceImg = NULL;
	abase::vector<AUITEXTAREA_EMOTION>* pAreaList = NULL;
	pGameUI->GetEmotionList(&pFaceImg, &pAreaList);
	if (!pFaceImg || !pAreaList)
		return -1;

	int iFace = -1;// szText[0] - AUICOMMON_EMOTIONCODE_START;
	if (iFace < 0 || iFace >= (int)pAreaList->size())
		return -1;

	iSkipChar = 1;

	return iFace;
}

//	Create a emotion item
bool CECPateText::CreateEmotionItem(int nEmotionSet, int iFace, int iLine)
{
	CECFaceDecal* pDecal = new CECFaceDecal;
	if (!pDecal->Init(nEmotionSet, iFace))
	{
		delete pDecal;
		return false;
	}

	//	Add to emotion list
	int iIndex = m_aEmotions.Add(pDecal);

	ITEM Item;
	Item.iType	= TYPE_EMOTION;
	Item.iIndex	= iIndex;
	Item.iLine	= iLine;

	//	Get decal extent
	pDecal->GetExtent(&Item.iExtX, &Item.iExtY);
	m_aItems.Add(Item);

	return true;
}

//	Clear whole text string
void CECPateText::Clear()
{
	int i;

	for (i=0; i < m_aTextStrs.GetSize(); i++)
		delete m_aTextStrs[i];

	m_aTextStrs.RemoveAll(false);

	for (i=0; i < m_aEmotions.GetSize(); i++)
		delete m_aEmotions[i];

	m_aEmotions.RemoveAll(false);
	m_aItems.RemoveAll(false);
	m_ItemsSet.Release();
}

//	Tick routine
bool CECPateText::Tick(DWORD dwDeltaTime)
{
	//	Tick emotion animation
	for (int i=0; i < m_aEmotions.GetSize(); i++)
		m_aEmotions[i]->Tick(dwDeltaTime);

	return true;
}

//	Render routine
bool CECPateText::Render(CECViewport* pViewport, int sx, int sy, DWORD dwCol, float z, float rhw)
{
	CECGameUIMan* pUIMan = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetCurrentUIManPtr();
	assert(pUIMan);
	float fScale = pUIMan->GetWindowScale();
	A3DFTFont* pFont = pUIMan->GetPateFont();
	A3DFTFont* pFontName = pUIMan->GetPateFontName();
	assert(pFont);

	int i, x=sx, y=sy;
	int iLine = 0;

	if (m_bBackground && m_aItems.GetSize())
	{
		int nWidth = int(m_iExtX * fScale);
		int nHeight = int(m_iExtY * m_iLines * fScale);
		A3DRECT rect;
		rect.left = sx - nHeight / 3;
		rect.top = sy - 2;
		rect.right = sx + nWidth + nHeight / 3 + 1;
		rect.bottom = sy + nHeight + 4;
		bool bval;
		m_pFramePopChat->GetA2DSprite()->SetZPos(z+0.000001f);
		bval = m_pFramePopChat->Render(sx - nHeight/m_aItems.GetSize() / 3-3, sy - 4, nWidth + nHeight/m_aItems.GetSize() * 2 / 3 + 6, nHeight+nHeight/m_aItems.GetSize() / 3 + 9,
			pFont, 0, NULL, AUIFRAME_ALIGN_LEFT, 0, 0, 255, true);
		//g_pA3DGDI->Draw2DZRectWithEdge(rect, z+0.00001, A3DCOLORRGB(128, 150, 157), A3DCOLORRGB(79, 110, 165));
	}
	
	for (i=0; i < m_aItems.GetSize(); i++)
	{
		const ITEM& Item = m_aItems[i];

		if (Item.iLine != iLine)
		{
			x = sx;
			y += int(m_iExtY * fScale);
			iLine = Item.iLine;
		}

		if (Item.iType == TYPE_TEXT)
		{
			ACString* pstr = m_aTextStrs[Item.iIndex];

			if(Item.iIndex<0 || Item.iIndex>=m_aTextStrs.GetSize())
				a_LogOutput(1, "CECPateText::Render dmp error found!!!!!!!!!!");

			else
			{
				// draw normal text here
				if (m_iFontType == TYPE_TEXT)
					pFont->TextOut(x, y, *pstr, Item.clItem ? Item.clItem : dwCol, true, A3DCOLORRGBA(0, 0, 0, 255), z, -1, rhw);
				else
					pFontName->TextOut(x, y, *pstr, Item.clItem ? Item.clItem : dwCol, true, A3DCOLORRGBA(0, 0, 0, 255), z, -1, rhw);
			}
		}
		else	//	TYPE_EMOTION
		{
			CECFaceDecal* pDecal = m_aEmotions[Item.iIndex];
			pDecal->SetScreenPos(x, y);
			pDecal->SetZValue(z, rhw);
			pDecal->SetScaleX(fScale);
			pDecal->SetScaleY(fScale);
			pDecal->Render(pViewport);
		}

		x += int(Item.iExtX * fScale);
	}

	return true;
}

//	Register render
bool CECPateText::RegisterRender(int sx, int sy, DWORD dwCol, float z, float rhw, int fontType)
{
	m_RegRender.x	= sx;
	m_RegRender.y	= sy;
	m_RegRender.z	= z;
	m_RegRender.rhw = rhw;
	m_RegRender.col	= dwCol;
	m_iFontType	= fontType;
	if (m_bBackground)
	{
		m_RegRender.y -= m_pFramePopChat->GetSizeLimit().cy/2;
	}
	g_pGame->GetGameRun()->GetWorld()->GetDecalMan()->RegisterPateText(this);
	return true;
}

