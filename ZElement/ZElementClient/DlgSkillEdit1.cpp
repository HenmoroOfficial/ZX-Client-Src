// File		: DlgSkillEdit1.cpp
// Creator	: Fu Chonggang
// Date		: 2010/3/29

#include "AFI.h"
#include "DlgSkillEdit1.h"
#include "DlgSkill.h"
#include "DlgSkillMan.h"
#include "EC_GameUIMan.h"
#include "EC_Skill.h"
#include "EC_Game.h"
#include "EC_Configs.h"
#include "EC_HostPlayer.h"
#include "DlgSettingVideo.h"
#include "LuaEvent.h"
#include "EC_GameSession.h"
#include "elementdataman.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgSkillComboEdit, CDlgBase)

AUI_ON_COMMAND("confirm",		OnCommandConfirm)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgSkillComboEdit, CDlgBase)

AUI_ON_EVENT("Item_*",		WM_LBUTTONDOWN,		OnEventLButtonDown_Item)
AUI_ON_EVENT("Img_Icon*",	WM_LBUTTONDOWN,		OnEventLButtonDown_Icon)
AUI_ON_EVENT("Img_Icon*",	WM_LBUTTONDBLCLK,	OnEventLButtonDown_Icon)

AUI_END_EVENT_MAP()

CDlgSkillComboEdit::CDlgSkillComboEdit()
{
	int i;
	for (i = 0; i<COMBOEDIT_ELEMENT; i++)
	{
		m_pImgSkillElement[i] = NULL;
	}
	for (i = 0; i<COMBOEDIT_NUM; i++)
	{
		m_pImgSkillEdit[i] = NULL;
	}
}

CDlgSkillComboEdit::~CDlgSkillComboEdit()
{
}

bool CDlgSkillComboEdit::OnInitDialog()
{
	int i;
	for (i = 0; i<COMBOEDIT_ELEMENT; i++)
	{
		AString strName;
		strName.Format("Item_%d", i + 1);
		DDX_Control(strName, m_pImgSkillElement[i]);
	}
	for (i = 0; i<COMBOEDIT_NUM; i++)
	{
		AString strName;
		strName.Format("Img_Icon%02d", i + 1);
		DDX_Control(strName, m_pImgSkillEdit[i]);
	}
	
	m_pLabTime = (PAUILABEL)GetDlgItem("Txt_Time");
	m_pBtnConfirm = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Confirm");
	m_pSkill = NULL;
	m_nEleNum = 0;
	m_nEditNum = 0;
	m_nTime = 0;

	return true;
}

void CDlgSkillComboEdit::OnTick()
{
	if (m_nTime>0)
	{
		ACString strText;
		strText.Format(GetGameUIMan()->GetStringFromTable(1609), m_nTime/60000, (m_nTime/1000)%60);
		strText = GetGameUIMan()->GetStringFromTable(1512) + strText;
		m_pLabTime->SetText(strText);
		m_pBtnConfirm->Enable(false);
		m_nTime -= GetGame()->GetTickTime();
	}
	else
	{
		m_pLabTime->SetText(_AL(""));
		m_pBtnConfirm->Enable(true);
	}
}

void CDlgSkillComboEdit::OnCommandConfirm(const char *szCommand)
{
	int i;
	int id = m_pSkill->GetSkillID();
	elementdataman* pDB = GetGame()->GetElementDataMan();
	DATA_TYPE DataType;
	const COMBINE_SKILL_EDIT_CONFIG* csec	= (COMBINE_SKILL_EDIT_CONFIG*)pDB->get_data_ptr(270, ID_SPACE_CONFIG, DataType);
	if (DataType==DT_COMBINE_SKILL_EDIT_CONFIG)
	{
		for (i=0; i<30; i++)
		{
			if (csec->edit_config[i].skill_id == id)
			{
				m_nTime = GetHostPlayer()->GetSkillEditCoolTime(csec->edit_config[i].cool_index-GP_CT_COMBINE_EDIT_START);
				if (m_nTime)
				{
					GetGameUIMan()->AddChatMessage(GetGameUIMan()->GetStringFromTable(8738), GP_CHAT_MISC);
					return;
				}
			}
		}
	}
	int elements[COMBOEDIT_NUM];
	for (i=0; i<COMBOEDIT_NUM; i++)
	{
		elements[i] = 0;
		if (m_pImgSkillEdit[i]->GetDataPtr())
		{
			CECSkill* pSkill = (CECSkill*)m_pImgSkillEdit[i]->GetDataPtr();
			elements[i] = pSkill->GetSkillID();
		}
	}
	GetGameSession()->c2s_CmdUpdateCombineSkill(m_pSkill->GetSkillID(), m_nEditNum, elements);
	GetHostPlayer()->UpdateCombineSkillElems(m_pSkill->GetSkillID(), elements, m_nEditNum);
	Show(false);
}

void CDlgSkillComboEdit::OnEventLButtonDown_Item(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	POINT pt = 
	{
		GET_X_LPARAM(lParam) - p->X,
		GET_Y_LPARAM(lParam) - p->Y,
	};

	GetGameUIMan()->InvokeDragDrop(this, pObj, pt);
}

void CDlgSkillComboEdit::OnEventLButtonDown_Icon(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	int iSlot = atoi(pObj->GetName() + strlen("Img_Icon"));
	PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)pObj;
	if (pImg->GetDataPtr())
	{
		pImg->SetDataPtr(NULL);
		pImg->ClearCover();
		pImg->SetHint(_AL(""));
		pImg->SetData(0);
	}
}

void CDlgSkillComboEdit::EditSkill(int id, int level)
{
	if (m_pSkill)
	{
		delete m_pSkill;
	}

	m_pSkill = new CECSkill(id, level);
	m_nEleNum = m_pSkill->GetSkillElemsNum();
	m_nEditNum = m_pSkill->GetComboSkillCapacity(level);
	int i=0;
	for (i=0; i<m_nEleNum; i++)
	{
		m_pImgSkillElement[i]->Show(true);
		GetGameUIMan()->SetSkillInfo(m_pImgSkillElement[i], m_pSkill->GetSkillElem(i+1));
	}
	for (; i<COMBOEDIT_ELEMENT; i++)
	{
		m_pImgSkillElement[i]->Show(false);
	}
	
	int num;
	unsigned short * skillEdit = GetHostPlayer()->GetCombineSkillElems(id, num);
	for (i=0; i<m_nEditNum; i++)
	{
		m_pImgSkillEdit[i]->Show(true);
		if (i<num && skillEdit[i]>0)
		{
			int j;
			for (j =0; j< m_nEleNum; ++j)
			{
				CECSkill* pSkill = (CECSkill*)m_pImgSkillElement[j]->GetDataPtr();
				if (pSkill && (pSkill->GetSkillID() == skillEdit[i]))
				{
					AString strFile;
					af_GetFileTitle(pSkill->GetIconFile(), strFile);
					strFile.MakeLower();
					int ItemIndex;
					A2DSprite* pSprite = GetGameUIMan()->GetSpriteIcon(CECGameUIMan::ICONS_SKILL, strFile.GetBuffer(0), &ItemIndex);
					m_pImgSkillEdit[i]->SetDataPtr(pSkill);
					m_pImgSkillEdit[i]->SetCover(pSprite, ItemIndex);
					m_pImgSkillEdit[i]->SetHint(pSkill->GetDesc());
					break;
				}
			}
			if (j == m_nEleNum)
			{
				m_pImgSkillEdit[i]->SetDataPtr(NULL);
				m_pImgSkillEdit[i]->SetCover(NULL, -1);
				m_pImgSkillEdit[i]->SetHint(_AL(""));		
			}
		}
		else
		{
			m_pImgSkillEdit[i]->SetDataPtr(NULL);
			m_pImgSkillEdit[i]->SetCover(NULL, -1);
			m_pImgSkillEdit[i]->SetHint(_AL(""));
		}
		
	}
	for (; i<COMBOEDIT_NUM; i++)
	{
		m_pImgSkillEdit[i]->Show(false);
	}
	if (!IsShow())
	{
		Show(true);
	}
	
	m_nTime = 0;
	id = m_pSkill->GetSkillID();
	elementdataman* pDB = GetGame()->GetElementDataMan();
	DATA_TYPE DataType;
	const COMBINE_SKILL_EDIT_CONFIG* csec	= (COMBINE_SKILL_EDIT_CONFIG*)pDB->get_data_ptr(270, ID_SPACE_CONFIG, DataType);
	if (DataType==DT_COMBINE_SKILL_EDIT_CONFIG)
	{
		for (i=0; i<30; i++)
		{
			if (csec->edit_config[i].skill_id == id)
			{
				m_nTime = GetHostPlayer()->GetSkillEditCoolTime(csec->edit_config[i].cool_index-GP_CT_COMBINE_EDIT_START);
				return;
			}
		}
	}
}

void CDlgSkillComboEdit::DragDropItem(PAUIDIALOG pDlgSrc, PAUIOBJECT pObjSrc, PAUIOBJECT pObjOver)
{
	if (pDlgSrc==this && strstr(pObjSrc->GetName(), "Item_") && strstr(pObjOver->GetName(), "Img_Icon"))
	{
		int srcIndex = atoi(pObjSrc->GetName() + strlen("Item_"));
		PAUIIMAGEPICTURE pImgSrc = (PAUIIMAGEPICTURE)pObjSrc;
		PAUIIMAGEPICTURE pImgDst = (PAUIIMAGEPICTURE)pObjOver;
		pImgDst->SetDataPtr(pImgSrc->GetDataPtr());
		A2DSprite* pA2DSprite;
		int nItem;
		pImgSrc->GetCover(&pA2DSprite, &nItem);
		pImgDst->SetCover(pA2DSprite, nItem);
		pImgDst->SetHint(pImgSrc->GetHint());
		pImgDst->SetData(srcIndex);
	}
}

bool CDlgSkillComboEdit::Release()
{
	if (m_pSkill)
	{
		delete m_pSkill;
	}
	for (int i=0; i<COMBOEDIT_ELEMENT; i++)
	{
		if (m_pImgSkillElement[i]->GetDataPtr())
		{
			CECSkill* pSkill = (CECSkill*)m_pImgSkillElement[i]->GetDataPtr();
			delete pSkill;
		}
	}
	return CDlgBase::Release();
}