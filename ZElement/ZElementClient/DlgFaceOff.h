// Filename	: DlgFaceOff.h
// Creator	: Fu Chonggang
// Date		: 2009/05/13

#pragma once

#include "DlgBase.h"

class AUIComboBox;
class AUIImagePicture;
class AUIStillImageButton;
class CECLoginPlayer;

class CDlgFaceOff : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
public:
	CDlgFaceOff();
	virtual ~CDlgFaceOff();
	virtual bool Render(void);

	void OnCommandConfirm(const char* szCommand);
	void OnCommandHairLeft(const char* szCommand);
	void OnCommandHairRight(const char* szCommand);
	void OnCommandHairChange(const char* szCommand);
	void OnCommandFaceLeft(const char* szCommand);
	void OnCommandFaceRight(const char* szCommand);
	void OnCommandFaceChange(const char* szCommand);
	void OnCommandClothLeft(const char* szCommand);
	void OnCommandClothRight(const char* szCommand);
	void OnCommandClothChange(const char* szCommand);
	void OnCommandEarLeft(const char* szCommand);
	void OnCommandEarRight(const char* szCommand);
	void OnCommandEarChange(const char* szCommand);
	void OnCommandTailLeft(const char* szCommand);
	void OnCommandTailRight(const char* szCommand);
	void OnCommandTailChange(const char* szCommand);
	void OnCommandZoom(const char* szCommand);
	void OnCommandType(const char* szCommand);
	void OnCommandChange(const char* szCommand);
	
	void OnCommand_CANCEL(const char* szCommand);

	void OnEventMouseMove(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonDown_Size(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonUp_Size(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void ReLoadModel();
protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
private:
	AUIComboBox *m_pComboFace;
	AUIComboBox *m_pComboHair;
	AUIComboBox *m_pComboCloth;
	AUIComboBox *m_pComboEar;
	AUIComboBox *m_pComboTail;
	AUIImagePicture *m_pImg_Head;
	
	AUIStillImageButton	*m_pBtn_LeftTurn;
	AUIStillImageButton	*m_pBtn_RightTurn;
	int m_nMouseLastX;
	int m_nMouseOffset;
	int m_nMouseOffsetThis;

	CECLoginPlayer*		m_pPlayer;
	int					m_nAngle;
	bool				m_bEarTail;

	int					m_iCameraWeight;

	void InitStrings();
};
