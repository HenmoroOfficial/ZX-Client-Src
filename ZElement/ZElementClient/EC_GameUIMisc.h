// Filename	: EC_GameUIMisc.h
// Creator	: Fuchonggang
// Date		: 2010.6.23
// Desc		: 界面相关公用函数

#include "DlgBase.h"

class A3DFTFont;
class AUITextArea;
class AUIImagePicture;
class AUIListBox;
class AUILabel;
struct AUITEXTAREA_EDITBOX_ITEM;
extern const unsigned int idFeitian;
extern const unsigned int CECGAMEUIMAN_MAX_MSGS;
extern const unsigned int idFlyItem;					//飞天符

void FilterTextColor(ACString& str);
void ConvertChatString(ACString& str);
int GetLengthExpItem(const ACHAR * const szText);
bool GetItemLinkOnText(int x, int y, AUITextArea* pText, AUITEXTAREA_EDITBOX_ITEM *pLink);
bool GetItemLinkOnList(int x, int y, AUIListBox* pLst);
bool SetImageFileProperty(AUIImagePicture* pImg, const char* path, int size = 0);
ACString GetCashText(int nCash, bool bFullText=false, bool bPoint=false);
ACString GetTimeText(int t);
ACString GetTimeText1(int t);		//商城专用
ACString GetTimeTextNumber(int t);

/*
悬浮显示中太宽不好看，在中间适当的地方加上换行符
*/
void FormatTextFixedWidth(ACString &strText, int width, A3DFTFont *pFont=0);

inline void ChangeDlgShowState(PAUIDIALOG pDlg) { if (pDlg) pDlg->Show(!pDlg->IsShow()); }
