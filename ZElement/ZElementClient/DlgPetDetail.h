// File		: DlgPetDetail.h
// Creator	: Xiao Zhou
// Date		: 2008/4/3

#pragma once

#include "DlgBase.h"
#include "EC_GPDataType.h"
#include "EC_IvtrTypes.h"

class CLuaState;
class AUIProgress;
class CECPetBedge;
class CECIvtrItem;
class CECModel;
class AUIImagePicture;
class AUIStillImageButton;
class AUIObject;

#define DLGPETDETAIL_MAX_HOROSCOPE	10
#define DLGPETDETAIL_MAX_TYPE		13

class CDlgPetDetail : public CDlgBase  
{
	friend class CDlgPetEquip;

public:
	CDlgPetDetail();
	virtual ~CDlgPetDetail();
	
	void SetPropAdded(void *pData);
	void SetInfo(int index = -1, CECPetBedge *pPet = NULL);

protected:
	virtual	bool OnInitDialog();
	virtual bool Release();
	virtual void OnTick();

	void UpdateDisplay(CECPetBedge *pPet);

	S2C::player_pet_prop_added m_PetPropadded[IVTRSIZE_PETPACK];
	A2DSprite* m_pImageStar;
	CLuaState * m_pState;
	int			m_nLevelValue[100];
	int			m_nLevels[DLGPETDETAIL_MAX_HOROSCOPE][DLGPETDETAIL_MAX_TYPE];
	int			m_nLevelsIndex[DLGPETDETAIL_MAX_HOROSCOPE][DLGPETDETAIL_MAX_TYPE];
	AUIProgress *m_pPrgLevel[DLGPETDETAIL_MAX_TYPE][2];
	int			m_nLength;
};

class CDlgPetDetailOperate : public CDlgPetDetail
{	
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgPetDetailOperate() : m_pPet(0),m_iAngle(0),m_fModelSize(0) {}
	~CDlgPetDetailOperate() {}

	void OnCommandChangeName(const char * szCommand);
	void OnCommandChangeBone(const char * szCommand);
	void OnCommandChangeFight(const char * szCommand);
	void OnCommandChangeHoroscope(const char * szCommand);
	void OnCommandChangeImage(const char * szCommand);
	void OnCommandChangeLife(const char * szCommand);
	void OnCommandChangeNature(const char * szCommand);
	void OnCommandChangePreLife(const char * szCommand);
	void OnCommandChangeSkill(const char * szCommand);
	void OnCommandExpandSkill(const char * szCommand);
	void OnCommandUpNum(const char * szCommand);
	void OnCommandChangeBasicType(const char * szCommand);
	void OnCommandFlyAttrOpt(const char * szCommand);
	void OnCommandCancel(const char * szCommand);
	
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	void SetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg=NULL);

	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);

protected:
	virtual	bool OnInitDialog();
	virtual bool Release();
	virtual void OnTick();
	bool PrepareOpt();
	void ClearDisplay();

	// 切换属性显示区域：常规属性展示与飞升属性提升
	void SwitchFlyAttr(bool bFly);
	void DoSetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg=NULL);
		
	AUIStillImageButton *m_pBtnLeftTurn;
	AUIStillImageButton *m_pBtnRightTurn;
	AUIImagePicture		*m_pImgPet;
	AUIObject	*m_pObjFlyAttrButton;
	
	CECModel	*m_pPet;
	int			m_iAngle;
	bool		m_bLoaded;
	float		m_fModelSize;
};
