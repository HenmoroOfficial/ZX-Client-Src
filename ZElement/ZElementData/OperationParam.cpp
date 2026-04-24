#include "stdafx.h"
#include "Global.h"
#include "operationparam.h"
#include "PolicyTriggerDlg.h"

BOOL CALLBACK TRIGGER_ID_FUNCS::OnActivate(void)
{
	COperationParam_Run_Trigger *pTrigger = (COperationParam_Run_Trigger*)m_pParent;
	if(pTrigger->m_Data.uID==0)
	{//新搞一个触发器
		CPolicyTriggerDlg dlg;
		dlg.m_pCurrentPolicy = pTrigger->m_pPolicyData;
		if(IDOK==dlg.DoModal())
		{
			if(dlg.m_pTriggerData)
			{
				dlg.m_pTriggerData->SetRunStatus(true);
				pTrigger->m_pPolicyData->AddTriggerPtr(dlg.m_pTriggerData);
				pTrigger->m_Data.uID = dlg.m_pTriggerData->GetID();
				m_strText = dlg.m_pTriggerData->GetName();
				g_bPolicyModifyed = true;
			}
		}
	}else
	{//打开一个旧的触发器
		for( int i = 0; i < pTrigger->m_pPolicyData->GetTriggerPtrNum(); i++)
		{
			CTriggerData *pData =  pTrigger->m_pPolicyData->GetTriggerPtr(i);
			if(pData->IsRun())
			{
				if(pData->GetID() ==  pTrigger->m_Data.uID)
				{
					CPolicyTriggerDlg dlg;
					dlg.m_bModifyed = true;
					dlg.m_pTriggerData = pData;
					dlg.m_pCurrentPolicy = pTrigger->m_pPolicyData;
					if(IDOK==dlg.DoModal())
					{
						g_bPolicyModifyed = true;
					}
					m_strText = pData->GetName();
					break;
				}
			}
		}
	}
	return TRUE;
}

LPCTSTR CALLBACK TRIGGER_ID_FUNCS::OnGetShowString(void) const
{
	return m_strText;
}

AVariant CALLBACK TRIGGER_ID_FUNCS::OnGetValue(void) const
{
	return m_var;
}

void CALLBACK TRIGGER_ID_FUNCS::OnSetValue(const AVariant& var)
{
	COperationParam_Run_Trigger *pTrigger = (COperationParam_Run_Trigger *)m_pParent;
	//打开一个旧的触发器
	for( int i = 0; i <  pTrigger->m_pPolicyData->GetTriggerPtrNum(); i++)
	{
		CTriggerData *pData = pTrigger->m_pPolicyData->GetTriggerPtr(i);
		if(pData->IsRun())
		{
			if(pData->GetID() == pTrigger->m_Data.uID)
			{
				m_strText = pData->GetName();
				break;
			}
		}
	}
}
