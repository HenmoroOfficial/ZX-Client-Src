// Filename	: AUIObject.h
// Creator	: Tom Zhou
// Date		: May 12, 2004
// Desc		: AUIObject is the base class of all other AUI controls.

#ifndef _AUIOBJECT_H_
#define _AUIOBJECT_H_

#include "AFile.h"
#include "AXMLFile.h"
#include "A3DTypes.h"
#include "A3DMacros.h"

class AScriptFile;

#define AUIOBJECT_STATE_UNKNOWN		0
#define AUIOBJECT_BLINK_INTERVAL	500
#define AUIOBJECT_FLASH_INTERVAL	500

#define AUI_COLORKEY	A3DCOLORRGB(255, 0, 255)

class A3DEngine;
class A3DDevice;
class A3DFTFont;
class AUIDialog;
class A3DViewport;

namespace AudioEngine
{
	class EventInstance;
	class Event;
}

enum
{
	AUIOBJECT_TYPE_NULL = 0,
	AUIOBJECT_TYPE_MIN,
	AUIOBJECT_TYPE_CHECKBOX = AUIOBJECT_TYPE_MIN,
	AUIOBJECT_TYPE_COMBOBOX,
	AUIOBJECT_TYPE_CONSOLE,
	AUIOBJECT_TYPE_CUSTOMIZE,
	AUIOBJECT_TYPE_EDITBOX,
	AUIOBJECT_TYPE_IMAGEPICTURE,
	AUIOBJECT_TYPE_LABEL,
	AUIOBJECT_TYPE_LISTBOX,
	AUIOBJECT_TYPE_MODELPICTURE,
	AUIOBJECT_TYPE_PROGRESS,
	AUIOBJECT_TYPE_RADIOBUTTON,
	AUIOBJECT_TYPE_SCROLL,
	AUIOBJECT_TYPE_SLIDER,
	AUIOBJECT_TYPE_STILLIMAGEBUTTON,
	AUIOBJECT_TYPE_SUBDIALOG,
	AUIOBJECT_TYPE_TEXTAREA,
	AUIOBJECT_TYPE_TREEVIEW,
	AUIOBJECT_TYPE_VERTICALTEXT,
	AUIOBJECT_TYPE_WINDOWPICTURE,
	AUIOBJECT_TYPE_MAX
};

#define AUIOBJECT_TYPE_NUM	(AUIOBJECT_TYPE_MAX - AUIOBJECT_TYPE_MIN)

typedef struct _AUIOBJECT_FONT
{
	ACHAR szName[AFILE_LINEMAXLEN];
	int nHeight;
	bool bBold;
	bool bItalic;
} AUIOBJECT_FONT, * PAUIOBJECT_FONT;

typedef union _AUIOBJECT_SETPROPERTY
{
	int i;
	bool b;
	float f;
	DWORD dw;
	AUIOBJECT_FONT font;
	char c[AFILE_LINEMAXLEN];
	char fn[AFILE_LINEMAXLEN];
	ACHAR s[AFILE_LINEMAXLEN];
	char cmd[AFILE_LINEMAXLEN];
} AUIOBJECT_SETPROPERTY, * PAUIOBJECT_SETPROPERTY;

enum
{
	AUIOBJECT_PROPERTY_ALIGN,
	AUIOBJECT_PROPERTY_ALIGNY,
	AUIOBJECT_PROPERTY_FRAMEMODE,
	AUIOBJECT_PROPERTY_UNDERLINEMODE,
	AUIOBJECT_PROPERTY_ENUMMAX,
	AUIOBJECT_PROPERTY_INT = AUIOBJECT_PROPERTY_ENUMMAX,
	AUIOBJECT_PROPERTY_BOOL,
	AUIOBJECT_PROPERTY_FLOAT,
	AUIOBJECT_PROPERTY_COLOR,
	AUIOBJECT_PROPERTY_FONT,
	AUIOBJECT_PROPERTY_CHAR,
	AUIOBJECT_PROPERTY_STRING,
	AUIOBJECT_PROPERTY_COMMAND,
	AUIOBJECT_PROPERTY_FILENAME,
	AUIOBJECT_PROPERTY_INTERFACE_FILENAME,
	AUIOBJECT_PROPERTY_SURFACE_FILENAME,
	AUIOBJECT_PROPERTY_GFX_FILENAME,
	AUIOBJECT_PROPERTY_SFX_FILENAME,
	AUIOBJECT_PROPERTY_AUDIOEVENT,
};

#define AUIOBJECT_MAXPROPERTYNUM		48
#define AUIOBJECT_MAXPROPERTYNAMELEN	64
typedef struct _AUIOBJECT_PROPERTY
{
	char szName[AUIOBJECT_MAXPROPERTYNAMELEN];
	int nType;
	char szReserved[AFILE_LINEMAXLEN];
} AUIOBJECT_PROPERTY, * PAUIOBJECT_PROPERTY;

#define AUI_TEMPLATE_TOK ','

extern bool AUI_ReportError(int nLine, const char *pszFile);

class AUIObject
{
public:
	typedef bool (* LPFRENDERHINTCALLBACK)(const POINT &pt, DWORD param1, DWORD param2, DWORD param3);

public:
	AUIObject();
	virtual ~AUIObject();

	virtual bool Init(A3DEngine *pA3DEngine, A3DDevice *pA3DDevice, AScriptFile *pASF);
	virtual bool InitXML(A3DEngine *pA3DEngine, A3DDevice *pA3DDevice, AXMLItem *pItem);
	virtual bool InitIndirect(const char *pszFileName, int idType);
	virtual bool Release(void);
	virtual bool Save(FILE *file);
	virtual bool SaveXML(AXMLItem *pXMLItemDialog);
	virtual bool Tick(void);
	virtual bool Render(void);
	virtual bool OnDlgItemMessage(UINT msg, WPARAM wParam, LPARAM lParam);
	virtual A3DRECT GetRect(void);
	/* support properties
	"Name"					Property->c
	"Hint"					Property->s
	"Command"				Property->cmd
	"Sound Effect"			Property->fn
	"Hover Sound Effect"	Property->fn
	"X"						Property->i
	"Y"						Property->i
	"Width"					Property->i
	"Height"				Property->i
	"Text Color"			Property->dw
	"Outline Color"			Property->dw
	"Inner Text Color"		Property->dw
	"Text Upper Color"		Property->dw
	"Text Lower Color"		Property->dw
	"Hover Color"			Property->b
	"Text Hover Color"		Property->dw
	"Check Color"			Property->b
	"Text Check Color"		Property->dw
	"Font"					Property->font
	"Shadow"				Property->dw
	"Outline"				Property->i
	"Fade Color"			Property->b
	"FreeType"				Property->b
	"Text Content"			Property->s
	"Text Align"			Property->i
	"Anomaly"				Property->b
	"Line Space"			Property->i
	"Force Dynamic Render"	Property->b
	"Template"				Property->c
	*/
	virtual bool GetProperty(const	char *pszPropertyName, PAUIOBJECT_SETPROPERTY Property);
	virtual bool SetProperty(const	char *pszPropertyName, PAUIOBJECT_SETPROPERTY Property);
	static bool RegisterPropertyType();
	virtual void SetColor(A3DCOLOR color);
	virtual void Show(bool bShow);
	virtual void OptimizeSpriteResource(bool bLoad) {}

	const char * GetCommand(void);
	void SetCommand(const char *pszCommand);
	virtual const ACHAR * GetText(void);
	virtual void SetText(const ACHAR *pszText);
	const ACHAR * GetHint();
	int GetHintMaxWidth();
	void SetHint(const ACHAR *pszHint);
	void SetHintMaxWidth(int nMaxWidth);
	bool IsHintFollow();
	void SetHintFollow(bool bHintFollow);
	int GetType(void) const;
	bool IsEnabled(void);
	void Enable(bool bEnable);
	int GetState(void);
	void SetState(int nState);
	POINT GetPos(bool bLocal = false) const;
	virtual void SetPos(int x, int y);
	SIZE GetSize(void);
	virtual void SetSize(int W, int H);
	const char * GetName(void);
	void SetName(const char *pszName);
	bool IsShow(void);
	bool IsFocus(void);
	virtual void SetFocus(bool bFocus);
	bool IsTransparent();
	void SetTransparent(bool bTransparent);
	bool AcceptMouseMessage();
	void SetAcceptMouseMessage(bool bAccept);
	AUIDialog * GetParent(void);
	void SetParent(AUIDialog * pParent);
	int RetrievePropertyList(PAUIOBJECT_PROPERTY a_Property);
	A3DFTFont * GetFont();
	int GetFontHeight();
	A3DCOLOR GetColor(void);
	void ForceGetData(DWORD &dwData, AString &strName);
	DWORD GetData(AString strName = "");
	void SetData(DWORD dwData, AString strName = "");
	void ForceGetDataPtr(void *&pvData, AString &strName);
	void * GetDataPtr(AString strName = "");
	void SetDataPtr(void *pvData, AString strName = "");
	void ForceGetData64(UINT64 &uiData64, AString &strName);
	UINT64 GetData64(AString strName = "");
	void SetData64(UINT64 uiData64, AString strName = "");
	void ForceGetDataPtr64(UINT64 &uiDataPtr64, AString &strName);
	void SetDataPtr64(UINT64 uiDataPtr64, AString strName = "");
	UINT64 GetDataPtr64(AString strName = "");
	void BlinkText(bool bBlink);
	int GetAlpha();
	void SetAlpha(int nAlpha);
	int GetAlign();
	void SetAlign(int nAlign);
	void SetFlash(bool bFlash);
	bool GetFlash();
	void SetAnomaly(bool bAnomaly);
	bool GetAnomaly();
	void SetAutoIndent(bool bAutoIndent);
	bool GetAutoIndent() const;
	virtual void SetLineSpace(int nSpace);
	int GetLineSpace();
	void SetDynamicRender(bool bDynamic);
	void SetForceDynamicRender(bool bDynamic);
	bool NeedDynamicRender();
	void UpdateRenderTarget();
	AString GetHoverSound();
	virtual bool SetFontAndColor(const ACHAR *pszFontName, int nFontHeight,
		A3DCOLOR dwColor, int nShadow = 0, int nOutline = 0,
		bool bBold = false, bool bItalic = false, bool bFreeType = true,
		A3DCOLOR clOutline = 0,
		A3DCOLOR clInnerText = A3DCOLORRGB(255, 255, 255),
		bool bInterpColor = false,
		A3DCOLOR clUpper = 0,
		A3DCOLOR clLower = 0);

	A3DViewport * GetClipViewport();
	void SetClipViewport(A3DViewport *pViewport);
	void SetViewportParam(int x, int y, int W, int H);
	void SetRenderHintCallback(LPFRENDERHINTCALLBACK pCallback,	DWORD param1, DWORD param2 = 0, DWORD param3 = 0);
	bool ExecuteRenderHintCallback(POINT ptCursor);
	virtual int GetRequiredHeight();
	virtual bool IsInHitArea(int x, int y, UINT msg = WM_LBUTTONDOWN);
	virtual bool Resize();

	virtual void SetDefaultPos(int x, int y);
	POINT GetDefaultPos() const;
	virtual void SetDefaultSize(int W, int H);
	SIZE GetDefaultSize() const;
	
	void FitFont();
	void FitFont(const int nCreateMode);

	void SetTemplateName(const char* szTemplate);
	void AddTemplate(const char* szTemplate);
	const char* GetTemplateName() const;
	bool ApplyTemplate();
	bool ExportToXml(AXMLItem& xmlItem);

protected:
	//  helper functions
	bool LoadAudioInstance(const char* szGUID, AudioEngine::Event*& pInst);

protected:
	A3DEngine *m_pA3DEngine;
	A3DDevice *m_pA3DDevice;
	AUIDialog *m_pParent;
	A3DViewport *m_pA3DViewport;

	AString m_szName;
	AString m_strCommand;
	AString m_strSound;
	AString m_strSoundHover;

	ACString m_szFontName;
	int m_nFontHeight;
	float m_fDisplayHeight;
	int m_nShadow; // now, use it as color.
	int m_nOutline;
	bool m_bBold;
	bool m_bItalic;
	bool m_bFreeType;
	bool m_bFreeTypeInit;
	bool m_bAutoIndent;
	A3DCOLOR m_clOutline;
	A3DCOLOR m_clInnerText;
	bool m_bInterpColor;
	A3DCOLOR m_clUpper;
	A3DCOLOR m_clLower;
	bool m_bHoverColor;
	A3DCOLOR m_clHover;
	bool m_bCheckColor;
	A3DCOLOR m_clCheck;
	A3DFTFont *m_pFont;
	A3DCOLOR m_color;
	int m_nLineSpace;

	int m_nType;
	int m_nState;
	bool m_bShow;
	bool m_bFocus;
	bool m_bEnabled;
	bool m_bTransparent;
	bool m_bAcceptMouseMsg;
	bool m_bFlash;
	bool m_bAnomaly;
	int m_nFlashTime;
	int m_x, m_y;
	int m_nWidth, m_nHeight;
	int m_nDefaultX, m_nDefaultY, m_nDefaultWidth, m_nDefaultHeight;
	int m_nOffX, m_nOffY, m_nVPW, m_nVPH;
	int m_nAlign;
	ACString m_szText;
	ACString m_szHint;
	int m_nHintMaxWidth;
	bool m_bHintFollow;
	int m_nTime;
	bool m_bBlink;
	bool m_bBlinkText;
	int m_nAlpha;
	int m_nOldAlpha;
	bool m_bDynamicRender;
	bool m_bForceDynamicRender;
	int m_nOptimizeResourceState;

	DWORD m_dwParamCallback1;
	DWORD m_dwParamCallback2;
	DWORD m_dwParamCallback3;
	LPFRENDERHINTCALLBACK m_pRenderHintCallback;

	DWORD m_dwData;
	void *m_pvData;
	UINT64 m_uiData64;
	UINT64 m_uiDataPtr64;

	AString m_strDataName;
	AString m_strDataPtrName;
	AString m_strData64Name;
	AString m_strDataPtr64Name;

	AudioEngine::Event*	m_pAudio;
	AudioEngine::Event*	m_pAudioHover;
	
	AString m_strTemplate;
};

typedef AUIObject * PAUIOBJECT;

#endif //_AUIOBJECT_H_