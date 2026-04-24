// Filename	: DlgReplayEdit.h
// Creator	: Xiao Zhou
// Date		: 2006/10/12

#pragma once

#include "DlgBase.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIFrame.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIEditBox.h"
#include "AUI\\AUIScroll.h"
#include "AUI\\AUIRadioButton.h"

struct SECTION
{
	int start;
	int	end;
};

#define VECTOR_SECTION abase::vector<SECTION>

class CDlgReplayEdit : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP();

public:
	CDlgReplayEdit();
	virtual ~CDlgReplayEdit();

	void OnCommandExport(const char * szCommand);
	void OnCommandPreview(const char * szCommand);
	void OnCommandUndo(const char * szCommand);
	void OnCommandRedo(const char * szCommand);

	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventMouseMove(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventLButtonUp(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventKeyDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void InitEdit();

	void Export(ACString strRecordName);

	static bool m_bPreview;
	
protected:
	virtual void OnTick();
	virtual void OnShowDialog();
	virtual bool OnInitDialog();
	virtual bool Render();

	bool GetRecordPos(int x, int &sec, int &pos, bool cut = false);
	static abase::vector<VECTOR_SECTION> m_vecSteps;
	static int m_nCurStep;
	static int m_nSelStart, m_nSelEnd, m_nClick;
	static int m_nPreviewSection;
	static int m_nPreviewSecTime;
	static float m_fScale;

	PAUIIMAGEPICTURE		m_pImg_Record;
	PAUISTILLIMAGEBUTTON	m_pBtn_Undo;
	PAUISTILLIMAGEBUTTON	m_pBtn_Redo;
	PAUISTILLIMAGEBUTTON	m_pBtn_Preview;
	PAUISTILLIMAGEBUTTON	m_pBtn_StopPreview;
	PAUIRADIOBUTTON			m_pRdo[3];
	PAUISCROLL				m_pScl_Record;
	AUIFrame				m_Frame;
	bool					m_bDrag;
	int						m_nDragStartX;
	int						m_nDragCurX;
};
