// Filename	: DlgRuneMain.h
// Creator	: Fu Chonggang
// Date		: 2012.4.21
// Desc		: 元魂装备属性显示主界面

#pragma once

#include "DlgBase.h"

class CECIvtrRune;
class AUIStillImageButton;
class AUIImagePicture;
class AUIComboBox;
class AUIProgress;
class AUILabel;

class CDlgRuneMain : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();

public:
	CDlgRuneMain();
	virtual ~CDlgRuneMain();
	
	void OnCommandDefault(const char* szCommand);
	void OnCommandUpgrade(const char* szCommand);
	void OnCommandDecompose(const char* szCommand);
	void OnCommandRefine(const char* szCommand);
	void OnCommandInsert(const char* szCommand);
	void OnCommandChange(const char* szCommand);
	void OnCommandErase(const char* szCommand);
	void OnCommandLevelUp(const char* szCommand);
	void OnCommandHelp(const char* szCommand);
	void OnCommandHelp1(const char* szCommand);
	void OnCommandCustomizeRune1(const char* szCommand);
	void OnCommandCustomizeRune2(const char* szCommand);
	void OnCommandCustomizeRune3(const char* szCommand);
	void OnCommandCustomizeRune4(const char* szCommand);

	void DisplayOneProp(int id, int value, AString strSuffix, AUIDialog* pDlg=NULL);

	bool IsInService();
protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();

	void UpdateDisplay();

private:
	CECIvtrRune	*m_pRune;
	int			m_iIndex;

	AUIStillImageButton	*m_pLvUp;
	AUIImagePicture		*m_pImgIcon;
	AUIProgress			*m_pPrgRuneValue;
	AUILabel			*m_pLabRuneValue;
	PAUIDIALOG			m_pDlgInService;
	AUIImagePicture     *m_pCustomImage1;
	AUIImagePicture     *m_pCustomImage2;
	AUIImagePicture     *m_pCustomImage3;
	AUIImagePicture     *m_pCustomImage4;

public:
	static const int	ms_idTmpConfig;
	static const int	ms_iDefaultPropValue[50];
	static const int	ms_iRuneValueLevel[6];
};

class CDlgRuneCustom : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	
public:
	
	enum
	{
		CUSTOMIZE_TYPE1 = 1,
		CUSTOMIZE_TYPE2,
		CUSTOMIZE_TYPE3,
		CUSTOMIZE_TYPE4,

	};

	CDlgRuneCustom();
	virtual ~CDlgRuneCustom();

	void OnCommandCancel(const char * szCommand);
	void OnCommandConfirm(const char * szCommand);
	void OnCommandProduce(const char * szCommand);
	void SetCustomType(int iType);	

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	void InitStrings();

	AUIStillImageButton	*m_pBtn_Confirm;
	AUILabel			*m_pTxt_Prompt;
	AUIComboBox			*m_pCombo_Property1;
	AUIComboBox			*m_pCombo_Property2;
	AUIComboBox			*m_pCombo_Property3;
	AUIComboBox			*m_pCombo_Property4;
	AUIComboBox			*m_pCombo_ProPerty5;
	int					m_iType;
	int					m_iProperty[5];
};