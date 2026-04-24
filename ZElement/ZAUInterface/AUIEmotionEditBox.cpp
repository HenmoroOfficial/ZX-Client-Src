// File		: AUIEmotionEditBox.cpp
// Creator	: Xiao Zhou
// Date		: 2005/11/12

#include "AUIEmotionEditBox.h"
#include "AUIDialog.h"
#include "AUICommon.h"
#include "AUIManager.h"
#include "A3DFTFont.h"
#include "A2DSprite.h"
#include "A2DSpriteItem.h"

AUIEmotionEditBox::AUIEmotionEditBox()
{
	m_nEmotionWidth = 0;
	m_nEmotionHeight = 0;
	m_pA2DSpriteEmotion = NULL;
	m_nType = AUIOBJECT_TYPE_EMOTIONEDITBOX;
}

AUIEmotionEditBox::~AUIEmotionEditBox()
{
}

bool AUIEmotionEditBox::Save(FILE *file)
{
	fprintf(file, "EMOTIONEDIT");

	if( !AUIObject::Save(file) ) return false;

	ACHAR szFontName[256];
	a_sprintf(szFontName, _AL("%s###%d###%d###%d###%d"),
		m_szFontName, m_nShadow, m_nOutline, m_bBold, m_bItalic);
	int idString = m_pParent->AddStringToTable(szFontName);
	if( idString < 0 ) return false;

	fprintf(file, " \"%s\" %d %d %d %d %d \"%s\" %d %d %d %d\n",
		m_pAUIFrame ? m_pAUIFrame->GetA2DSprite()->GetName() : "",
		idString, m_nFontHeight, A3DCOLOR_GETRED(m_color),
		A3DCOLOR_GETGREEN(m_color),	A3DCOLOR_GETBLUE(m_color),
		m_bPassword ? "PASSWORD" : "TEXT", m_bReadOnly, m_bMultiline, m_nMaxLen, m_nFrameMode);

	return true;
}

void AUIEmotionEditBox::SetEmotionList(A2DSprite *pA2DSpriteEmotion,
	abase::vector<AUITEXTAREA_EMOTION> *pvecEmotion)
{
	m_pA2DSpriteEmotion = pA2DSpriteEmotion;
	m_vecEmotion = *pvecEmotion;
	A3DRECT rc = pA2DSpriteEmotion->GetItem(0)->GetRect();

	m_nEmotionHeight = rc.Height();
	m_nEmotionWidth = rc.Width();
	float p = ((float)m_CharHeight) / m_nEmotionHeight;
	m_nEmotionHeight *= p;
	m_nEmotionWidth *= p;
}

void AUIEmotionEditBox::CalcTexWidthBuf()
{
	float fWindowScale = m_pParent->GetAUIManager()->GetWindowScale();

	if( m_pFont )
		m_pFont->Resize(m_nFontHeight * fWindowScale);

	A3DFTFont* pFont = m_pFont ? m_pFont : m_pParent->GetAUIManager()->GetDefaultFont();

	int nLen = wcslen(m_szBuffer);
	m_CharWidthBuf[0] = 0;
	m_CharHeightBuf[0] = 0;

	if (m_bPassword)
	{
		WORD wWidth = pFont->GetTextExtent(L"*").x;

		for (int i = 0; i < nLen; i++)
			m_CharWidthBuf[i+1] = m_CharWidthBuf[i] + wWidth;
	}
	else
	{
		if(!m_bAutoReturn)
		{
			for (int i = 0; i < nLen; i++)
			{
				if(m_bMultiline && m_szBuffer[i]=='\r')
				{
					m_CharWidthBuf[i+1] = 0;
					m_CharHeightBuf[i+1] = m_CharHeightBuf[i] + m_CharHeight;
				}
				else
				{
					if( m_szBuffer[i] >= AUICOMMON_EMOTIONCODE_START &&
						m_szBuffer[i] <= AUICOMMON_EMOTIONCODE_END)
						m_CharWidthBuf[i+1] = m_CharWidthBuf[i] + m_nEmotionWidth * fWindowScale;
					else
						m_CharWidthBuf[i+1] = m_CharWidthBuf[i] + pFont->GetTextExtent(&m_szBuffer[i], 1).x;
					m_CharHeightBuf[i+1] = m_CharHeightBuf[i];
				}
			}
		}
		else
		{
			int i=0;
			int j=0;
			int len=m_szBuffer.GetLength();
			ACHAR tempBuffer[CHAR_WIDTH_BUF_SIZE];
			int xInsert=m_xInsertPos;
			int xCaret=m_xCaretPos;
			while(i<len)
			{
				if(m_szBuffer[i]==L'\r')
				{
					if(i+1==m_szBuffer.GetLength() || m_szBuffer[i+1]!=L'\n')
					{
						m_CharWidthBuf[j+1] = 0;
						m_CharHeightBuf[j+1] = m_CharHeightBuf[j] + m_CharHeight;
						tempBuffer[j] = L'\r';
						j++;
					}
					else
					{
						i++;
						if(i<xInsert)
							m_xInsertPos-=2;
						if(i<xCaret)
							m_xCaretPos-=2;
						m_xEndPos-=2;
					}
				}
				else
				{
					int width;
					if( m_szBuffer[i] >= AUICOMMON_EMOTIONCODE_START &&
						m_szBuffer[i] <= AUICOMMON_EMOTIONCODE_END)
						width = m_nEmotionWidth * fWindowScale;
					else
						width = pFont->GetTextExtent(&m_szBuffer[i], 1).x;
					if(m_CharWidthBuf[j] + width > m_rcText.Width())
					{
						m_xEndPos+=2;
						if(i<=xInsert)
							m_xInsertPos+=2;
						if(i<=xCaret)
							m_xCaretPos+=2;
						m_CharWidthBuf[j+2] = m_CharWidthBuf[j+1] = 0;
						m_CharHeightBuf[j+2] = m_CharHeightBuf[j+1] = m_CharHeightBuf[j] + m_CharHeight;
						tempBuffer[j++] = L'\r';
						tempBuffer[j++] = L'\n';
					}
					m_CharWidthBuf[j+1] = m_CharWidthBuf[j] + width;
					m_CharHeightBuf[j+1] = m_CharHeightBuf[j];
					tempBuffer[j] = m_szBuffer[i];
					j++;
				}
				i++;
			}
			tempBuffer[j] = L'\0';
			m_szBuffer = tempBuffer;
			len = j;
			j = 0;
			for(i=0;i<m_xInsertPos;i++)
				if(m_szBuffer[i]!=L'\r')
					tempBuffer[j++] = m_szBuffer[i];
				else if(i+1<m_szBuffer.GetLength()&&m_szBuffer[i+1]==L'\n')
					i++;
				else
					tempBuffer[j++] = m_szBuffer[i];
			for(i=m_xCaretPos;i<len;i++)
				if(m_szBuffer[i]!=L'\r')
					tempBuffer[j++] = m_szBuffer[i];
				else if(i+1<m_szBuffer.GetLength()&&m_szBuffer[i+1]==L'\n')
					i++;
				else
					tempBuffer[j++] = m_szBuffer[i];
			tempBuffer[j] = L'\0';
			m_szText = tempBuffer;
		}
	}
}

void AUIEmotionEditBox::RenderEditText(A3DFTFont* pFont, const ACHAR* szText)
{
	if (wcslen(szText) == 0) return;

	float fWindowScale = m_pParent->GetAUIManager()->GetWindowScale();

	int nMaxRow = m_rcText.Height() / m_CharHeight;
	if (nMaxRow == 0) nMaxRow++;

	int nLine = 0;
	const wchar_t *p, *pHead;
	p = pHead = szText;

	A3DRECT rc = m_rcText;
	rc.Offset(GetPos().x + m_nOffX, GetPos().y + m_nOffY);
	A3DRECT newrc = rc;
	newrc.left *= fWindowScale;
	newrc.right *= fWindowScale;
	newrc.top *= fWindowScale;
	newrc.bottom *= fWindowScale;

	while (true)
	{
		if (*p == L'\r' || *p == L'\0')
		{
			wchar_t temp = *p;
			(wchar_t)*p = L'\0';
			if( (nLine + 1) * m_CharHeight + m_ptViewOrg.y >= 0 &&
				nLine * m_CharHeight + m_ptViewOrg.y <= rc.Height() )
			{
				ACHAR szText[AUIOBJECT_MAXTEXTLEN + 1];
				AUI_ConvertChatString(pHead, szText);
				AUI_TextOutFormat( pFont, (m_ptViewOrg.x + rc.left) * fWindowScale, 
					(rc.top + nLine * m_CharHeight + m_ptViewOrg.y) * fWindowScale,
					szText, &newrc, 0, NULL, 0, false, NULL,
					m_pA2DSpriteEmotion, &m_vecEmotion,
					NULL, true, m_nShadow, true );
			}
			(wchar_t)*p = temp;

			if (*p == L'\0')
				break;

			nLine++;
			pHead = p + 1;
			if(*pHead == L'\n')
				pHead++;
		}

		p++;
	}
}
