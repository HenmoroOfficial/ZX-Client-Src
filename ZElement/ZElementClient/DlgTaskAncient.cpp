#include "AFI.h"
#include "AUI\\AUICTranslate.h"
#include "DlgTaskAncient.h"
#include "EC_GameUIMan.h"
#include "EC_Game.h"
#include "EC_HostPlayer.h"
#include "EC_TaskInterface.h"
#include "AUI\\AUIDef.h"
#include "EC_IvtrItem.h"
#define new A_DEBUG_NEW
extern CECGame* g_pGame;
AUI_BEGIN_COMMAND_MAP(CDlgTaskAncient, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",					OnCommand_CANCEL)
AUI_ON_COMMAND("Btn_Get",					OnCommand_Accept)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgTaskAncient, CDlgBase)

AUI_ON_EVENT("*",		WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("*",		WM_MOUSEWHEEL,		OnEventMouseWheel)

AUI_END_EVENT_MAP()


CDlgTaskAncient::CDlgTaskAncient()
{
	m_pTemp = NULL;
	InitICON("Configs\\task_icon.cfg");
}

CDlgTaskAncient::~CDlgTaskAncient()
{
	m_TaskICONMap.clear();
}

bool CDlgTaskAncient::OnInitDialog()
{
	int i;
	char szItem[40];
	m_pTxt_Content = (AUITextArea *)GetDlgItem("Txt_Content");
	m_pTxt_Content->SetText(_AL(""));
	m_pTxt_Item = (AUITextArea *)GetDlgItem("Txt_Item");
	m_pTxt_Item->SetAlign(AUIFRAME_ALIGN_CENTER);
	m_pScl_Item = (PAUISCROLL)GetDlgItem("Sd_Item");
	m_pBtn_AcceptTask = (AUIStillImageButton *) GetDlgItem("Btn_Get");

	for(i = 0; i < 4; i++)
	{
		sprintf(szItem, "Img_%d", i + 1);
		m_pImgU[i] = (PAUIIMAGEPICTURE)GetDlgItem(szItem);
	}

	for(i = 0; i < CDlgTaskBase::CDLGTASK_AWARDITEM_MAX ; i++ )
	{
		sprintf(szItem, "Item_0%d", i + 1);
		m_pImg_Item[i] = (PAUIIMAGEPICTURE)GetDlgItem(szItem);
	}

	m_pImg_Checked = (PAUIIMAGEPICTURE)GetDlgItem("Img_Checked");
	AUIOBJECT_SETPROPERTY p;
	sprintf(p.fn, "");
	m_pImg_Checked->SetProperty("Image File", &p);	

	for(i = 0; i < 4; i++)
	{
		sprintf(szItem, "Shape_A0%d", i + 1);
		m_pEdit[i] = (PAUIEDITBOX)GetDlgItem(szItem);
	}
	m_pEditSelect = (PAUIEDITBOX)GetDlgItem("Shape_A05");
	
	sprintf(p.fn, "frame\\Quest_Wanted_ImgBg.tga");
	m_pEditSelect->SetProperty("Frame Image", &p);
	return true;
}

void CDlgTaskAncient::OnShowDialog()
{
	m_pBtn_AcceptTask->Enable(false);
}

void CDlgTaskAncient::OnCommand_Accept(const char * szCommand)
{

	if(m_pTemp)
	{
		GetGame()->GetTaskTemplateMan()->ManualTrigStorageTask(GetHostPlayer()->GetTaskInterface(), m_pTemp->GetID(), 9);
		m_pBtn_AcceptTask->Enable(false);
	}
}

void CDlgTaskAncient::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
}

bool CDlgTaskAncient::Tick()
{
	int i, j, iTaskNum, idTask, a_idtask[TASK_STORAGE_LEN];
	PAUIIMAGEPICTURE pObj;
	PAUIEDITBOX pEdit;
	CECTaskInterface *pTask = GetHostPlayer()->GetTaskInterface();
	const unsigned short* pStorage = pTask->GetStorageTasks(9);
	ATaskTempl *pTemp;
	ATaskTemplMan *pTempMan = GetGame()->GetTaskTemplateMan();
	AUIOBJECT_SETPROPERTY p;
	ACHAR szText[40];

	a_sprintf(szText, _AL("%d"), GetHostPlayer()->GetPack()->GetItemTotalNum(53490));
    m_pTxt_Item->SetText(szText);

	iTaskNum = 0;
	for(i = 0; i < TASK_STORAGE_LEN; i++)
	{
		idTask = pStorage[i];
		if(!pTask->HasTask(idTask) && pTask->CanShowTask(idTask))
		{
			if(iTaskNum == 0)
			{
				a_idtask[iTaskNum] = idTask;
				iTaskNum++;
			}
			else
			{
				for(j = iTaskNum - 1; j >= 0; j--)
				{
					if(pTempMan->GetTaskTemplByID(idTask)->GetType() > pTempMan->GetTaskTemplByID(a_idtask[j])->GetType())
						a_idtask[j + 1] = a_idtask[j];
					else 
						break;
				}
				a_idtask[j + 1] = idTask;
				iTaskNum++;
			}
		}
	}
	
	if(iTaskNum == 0)
		m_pScl_Item->Enable(false);
	else
		m_pScl_Item->Enable(true);
	
	for(i = iTaskNum; i < TASK_STORAGE_LEN; i++)
		a_idtask[i] = 0;

	m_pScl_Item->SetScrollRange(0, (iTaskNum - 1) / CECDLGSTASK_ANICIENT_LINEMAX - 
		CECDLGTASK_ANICIENT_NUMMAX / CECDLGSTASK_ANICIENT_LINEMAX + 1);
	m_pScl_Item->Show(true);
	m_pScl_Item->SetBarLength(-1);
	m_pScl_Item->SetScrollStep(1);

	if( m_nFirstLine != m_pScl_Item->GetBarLevel() )
		m_nFirstLine = m_pScl_Item->GetBarLevel();
	int nStart = m_nFirstLine *  2;

	for (i = 0; i < 4; i++)
	{
		pObj = m_pImgU[i];
		pEdit = m_pEdit[i];
		if(!pObj) continue;
			
		if(a_idtask[i + nStart] > 0 && i + nStart < iTaskNum)
		{
			pObj->Show(true);
			idTask = a_idtask[i + nStart];
			pTemp = NULL;
			{
				pTemp = pTempMan->GetTaskTemplByID(idTask);
			}

			if(pTemp)
			{
				sprintf(p.fn, m_TaskICONMap[idTask]);
				pObj->SetProperty("Image File", &p);
				pObj->SetDataPtr(pTemp, "ptr_ATaskTempl");
				ACString strText;
				strText = pTemp->GetName();
				pObj->SetText(pTemp->GetName());
				if(pTemp->GetType() == 20)
				{
					sprintf(p.fn, "frame\\Quest_Wanted_ImgBg2.tga");
					pEdit->SetProperty("Frame Image", &p);
				}
				else if(pTemp->GetType() == 19)
				{
					sprintf(p.fn, "frame\\Quest_Wanted_ImgBg3.tga");
					pEdit->SetProperty("Frame Image", &p);
				}
				else if(pTemp->GetType() == 18)
				{
					sprintf(p.fn, "frame\\Quest_Wanted_ImgBg.tga");
					pEdit->SetProperty("Frame Image", &p);
				}
			}
			else
			{
				pObj->SetDataPtr(NULL);
				pObj->SetText(_AL(""));
				pObj->ClearCover();
			}
		}
		else
		{
			pObj->Show(false);
			pObj->ClearCover();
			sprintf(p.fn, "frame\\Quest_Wanted_ImgBg.tga");
			pEdit->SetProperty("Frame Image", &p);
		}
	}

	for(i = 0; i < CDlgTaskBase::CDLGTASK_AWARDITEM_MAX; i++)
		m_pImg_Item[i]->Show(false);
	
	idTask = 0;
	if(m_pTemp)
		idTask = m_pTemp->GetID();
	Task_Award_Preview award;
	pTask->GetCanDeliverTaskAwardPreview(idTask, &award);
	ACString strText;
	strText = _AL("");
	if(award.m_bHasItem)
	{
		for(i = 0; i < CDlgTaskBase::CDLGTASK_AWARDITEM_MAX ; i++)
		{
			if(i < award.m_ulItemTypes)
			{
				CECIvtrItem *pItem = CECIvtrItem::CreateItem(award.m_ItemsId[i], 0, award.m_ItemsNum[i]);
				if(!pItem) continue;

				AString strFile;
				af_GetFileTitle(pItem->GetIconFile(), strFile);
				strFile.MakeLower();
				m_pImg_Item[i]->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
				GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
				AUICTranslate trans;
				pItem->GetDetailDataFromLocal();
				const ACHAR *pszDesc = pItem->GetDesc();
				if( pszDesc )
					m_pImg_Item[i]->SetHint(trans.Translate(pszDesc));
				else
					m_pImg_Item[i]->SetHint(pItem->GetName());
				delete pItem;
				strText.Format(_AL("%d"), award.m_ItemsNum[i]);
				m_pImg_Item[i]->SetText(strText);
				m_pImg_Item[i]->Show(true);
			}
		}
	}
//ÔÝÊ±×¢ÊÍµô ·½±ã²âÊÔ
//	if(m_pTemp)
//		m_pBtn_AcceptTask->Enable(GetHostPlayer()->GetTaskInterface()->CanDeliverTask(m_pTemp->GetID());
	return CDlgBase::Tick();
}

void CDlgTaskAncient::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if( !pObj->GetDataPtr("ptr_ATaskTempl") ) return;
	ACString strText;
	ATaskTempl* pTempRoot, *pTemp = (ATaskTempl *)pObj->GetDataPtr("ptr_ATaskTempl");
	CECTaskInterface *pTask = GetHostPlayer()->GetTaskInterface();

	if(pTemp)
	{
		pTempRoot = (ATaskTempl*)pTemp->GetTopTask();
		if(pTemp != pTempRoot)
		{
			strText += pTempRoot->GetName();
			strText += _AL("-");
		}
		strText += pTemp->GetName();
		strText += _AL(":  ");
		strText += pTemp->GetDescription();
		AUIOBJECT_SETPROPERTY p;
		sprintf(p.fn, m_TaskICONMap[pTemp->GetID()]);
		m_pImg_Checked->SetProperty("Image File", &p);
		m_pTemp = pTemp;
		m_pTxt_Content->SetText(FormatTaskText(strText));
		
		if(pTemp->GetType() == 20)
		{
			sprintf(p.fn, "frame\\Quest_Wanted_ImgBg2.tga");
			m_pEditSelect->SetProperty("Frame Image", &p);
		}
		else if(pTemp->GetType() == 19)
		{
			sprintf(p.fn, "frame\\Quest_Wanted_ImgBg3.tga");
			m_pEditSelect->SetProperty("Frame Image", &p);
		}
		else if(pTemp->GetType() == 18)
		{
			sprintf(p.fn, "frame\\Quest_Wanted_ImgBg.tga");
			m_pEditSelect->SetProperty("Frame Image", &p);
		}

		m_pBtn_AcceptTask->Enable(true);		
	}

}

void CDlgTaskAncient::OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);
	int zDelta = (short)HIWORD(wParam);
	if( zDelta > 0 )
		zDelta = 1;
	else
		zDelta = -1;
	if( m_pScl_Item->IsShow() )
		m_pScl_Item->SetBarLevel(m_pScl_Item->GetBarLevel() - zDelta);
}
 
bool CDlgTaskAncient::InitICON(const char* szFile)
{
	int iNum, idTask;
	AIniFile IniFile;

	if (!IniFile.Open(szFile))
	{
		a_LogOutput(1, "CDLGTaskAncient::InitICON, Failed to open file %s", szFile);
		return false;
	}

	AString szImage, strSect = "tasknum";
	iNum = IniFile.GetValueAsInt(strSect, "num", 0);
	char szItem[40];
	for(int i = 0; i < iNum; i++)
	{
		sprintf(szItem, "task_%d", i + 1);
		strSect = szItem;
		idTask = IniFile.GetValueAsInt(strSect, "id", 0);
		szImage = IniFile.GetValueAsString(strSect, "image", "");

		m_TaskICONMap[idTask] = szImage;

	}
	return true;
}