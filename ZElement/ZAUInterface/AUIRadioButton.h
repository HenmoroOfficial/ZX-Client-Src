// Filename	: AUIRadioButton.h
// Creator	: Tom Zhou
// Date		: May 13, 2004
// Desc		: AUIRadioButton is the class of radio button control.

#ifndef _AUIRADIOBUTTON_H_
#define _AUIRADIOBUTTON_H_

#include "AUIObject.h"

class A2DSprite;
class A3DGFXEx;

enum
{
	AUIRADIOBUTTON_ITEM_MIN = 0,
	AUIRADIOBUTTON_ITEM_NORMAL = AUIRADIOBUTTON_ITEM_MIN,
	AUIRADIOBUTTON_ITEM_CHECKED,
	AUIRADIOBUTTON_ITEM_ONHOVER,
	AUIRADIOBUTTON_ITEM_CHECKEDONHOVER,
	AUIRADIOBUTTON_ITEM_MAX
};
#define AUIRADIOBUTTON_NUMITEMS		AUIRADIOBUTTON_ITEM_MAX

class AUIRadioButton : public AUIObject
{
public:
	AUIRadioButton();
	virtual ~AUIRadioButton();

	virtual bool Init(A3DEngine *pA3DEngine, A3DDevice *pA3DDevice, AScriptFile *pASF);
	virtual bool InitXML(A3DEngine *pA3DEngine, A3DDevice *pA3DDevice, AXMLItem *pItem);
	virtual bool InitIndirect(const char *pszFileName, int idType);
	virtual bool Save(FILE *file);
	virtual bool SaveXML(AXMLItem *pXMLItemDialog);
	virtual bool Release(void);
	virtual bool Tick(void);
	virtual bool Render(void);
	virtual bool OnDlgItemMessage(UINT msg, WPARAM wParam, LPARAM lParam);
	/* support properties
	"Align"							Property->i
	"Normal Image File"				Property->fn
	"Checked Image File"			Property->fn
	"OnHover Image File"			Property->fn
	"CheckedOnHover Image File"		Property->fn
	"Checked Gfx File"				Property->fn
	"OnHover Gfx File"				Property->fn
	"Group ID"						Property->i
	"Button ID"						Property->i 
	"Text Offset X"					Property->i
	"Text Offset Y"					Property->i
	"Vertical Text"					Property->b
	AUIObject.Properties
	*/
	virtual bool GetProperty(const char *pszPropertyName, PAUIOBJECT_SETPROPERTY Property);
	virtual bool SetProperty(const char *pszPropertyName, PAUIOBJECT_SETPROPERTY Property);
	static bool RegisterPropertyType();
	virtual bool IsInHitArea(int x, int y, UINT msg = WM_LBUTTONDOWN);
	virtual void Show(bool bShow);
	virtual void OptimizeSpriteResource(bool bLoad);

	bool IsChecked(void);
	void Check(bool bCheck);
	int GetGroupID(void);
	void SetGroupID(int idGroup);
	int GetButtonID(void);
	void SetButtonID(int idButton);
	bool IsVerticalText() const;
	void SetVerticalText(bool bVertical);
	bool SetGfxChecked(const char* szGfx);
	bool SetGfxOnHover(const char* szGfx);
	A3DGFXEx* GetGfxChecked();
	A3DGFXEx* GetGfxOnHover();
	void StartOnHoverGfx();
	void SetHover(bool bHover);
protected:
	bool m_bHover;
	bool m_bCheck;
	bool m_bVerticalText;
	int	m_nTextOffsetX, m_nTextOffsetY;
	A2DSprite *m_pA2DSpriteNormal, *m_pA2DSpriteChecked, *m_pA2DSpriteOnHover, *m_pA2DSpriteCheckedOnHover;
	A3DGFXEx *m_pGfxChecked, *m_pGfxOnHover;
	AString m_strGfxChecked, m_strGfxOnHover;
	int m_idGroup, m_idButton;

	void CalcAlignPos(A2DSprite* pA2DSprite, POINT& ptText, POINT& ptSprite);
	A2DSprite* GetRenderSprite();
	A3DCOLOR GetRenderColor();
};

typedef AUIRadioButton * PAUIRADIOBUTTON;

#endif //_AUIRADIOBUTTON_H_