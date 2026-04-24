// File		: AUIEmotionEditBox.h
// Creator	: Xiao Zhou
// Date		: 2005/11/12

#ifndef _AUIEmotionEditBox_H_
#define _AUIEmotionEditBox_H_

#include "AUIEditBox.h"
#include "AUITextArea.h"
#include "vector.h"

class AUIEmotionEditBox : public AUIEditBox
{
public:
	AUIEmotionEditBox();
	virtual ~AUIEmotionEditBox();

	void SetEmotionList(A2DSprite *pA2DSpriteEmotion,
		abase::vector<AUITEXTAREA_EMOTION> *pvecEmotion);

protected:
	A2DSprite *m_pA2DSpriteEmotion;
	abase::vector<AUITEXTAREA_EMOTION> m_vecEmotion;
	int	m_nEmotionHeight;
	int	m_nEmotionWidth;

	virtual bool Save(FILE *file);
	virtual void CalcTexWidthBuf();
	virtual void RenderEditText(A3DFTFont* pFont, const ACHAR* szText);
};

typedef AUIEmotionEditBox * PAUIEMOTIONEDITBOX;

#endif //_AUIEmotionEditBox_H_