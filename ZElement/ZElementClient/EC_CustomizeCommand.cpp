/*
 * FILE: EC_CustomizeCommand.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Liyi, 2005/4/25
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Customize.h"
#include "EC_LoginUIMan.h"
#include "EC_UIManager.h"

bool CECCustomize::OnCommand_Customize(const char* szCommand, AUIDialog* pDlg)
{
	int nCurUIMan = g_pGame->GetGameRun()->GetUIManager()->GetCurrentUIMan();

	if(nCurUIMan == CECUIManager::UIMAN_LOGIN)
	{

		if( 0 == stricmp(szCommand, "IDCANCEL") )
		{
			m_pAUIManager->ShowAllDialogs(false);

			m_pCurPlayer->RestoreCustomizeData();
			
			Release_Customize();//释放自定义对话框的资源

			if( m_bCallForCreate )
			{
				m_pAUIManager->GetDialog("Win_Create")->Show(true);

				char szName[40];
				sprintf(szName,"Win_CreateInfo%d", m_pCurPlayer->GetProfession());
				m_pAUIManager->GetDialog(szName)->Show(true);
			
				((CECLoginUIMan*)m_pAUIManager)->ChangeScene((CECLoginUIMan::LOGIN_SCENE)(CECLoginUIMan::LOGIN_SCENE_CREATE1 + m_pCurPlayer->GetProfession()));	
				
				m_pCurPlayer->SetDirAndUp(-g_pGame->GetViewport()->GetA3DCamera()->GetDirH(), g_vAxisY);
			}
			else
			{
				// move this player back to its role position
				
				m_pCurPlayer->SetPos(((CECLoginUIMan*)m_pAUIManager)->GetTargetPos());
				m_pCurPlayer->SetDirAndUp(-g_pGame->GetViewport()->GetA3DCamera()->GetDirH(), g_vAxisY);

				m_pAUIManager->GetDialog("Win_Customize")->Show(false);
				m_pAUIManager->GetDialog("Win_Select")->Show(true);
				m_pAUIManager->GetDialog("Win_Manage")->Show(true);
				m_pAUIManager->GetDialog("Win_Manage2")->Show(true);

				int nCurUIMan = g_pGame->GetGameRun()->GetUIManager()->GetCurrentUIMan();

			
				((CECLoginUIMan*)m_pAUIManager)->ChangeScene(CECLoginUIMan::LOGIN_SCENE_SELCHAR);
			

				char szCommand[40] = "";
				sprintf(szCommand, "char%d", ((CECLoginUIMan*)m_pAUIManager)->GetIDCurRole()+ 1);

				((CECLoginUIMan*)m_pAUIManager)->SetIDCurRole(-1);

				((CECLoginUIMan*)m_pAUIManager)->OnCommand_SelChar(szCommand, pDlg);
			}
			return true;
		}

		if( 0 == stricmp(szCommand, "IDOK") )
		{
			m_pAUIManager->ShowAllDialogs(false);

			Release_Customize();//释放自定义对话框的资源

			if( m_bCallForCreate )
			{
				m_pAUIManager->GetDialog("Win_Create")->Show(true);
				
				char szName[40];
				sprintf(szName,"Win_CreateInfo%d", m_pCurPlayer->GetProfession());
				m_pAUIManager->GetDialog(szName)->Show(true);
				

				// set the customize data to current player
				CECPlayer::PLAYER_CUSTOMIZEDATA data = m_pCurPlayer->GetCustomizeData();
				data.faceData = *m_pCurFace->GetFaceData();
				m_pCurPlayer->ChangeCustomizeData(data);
				
				// 保存CustomizeData
				m_pCurPlayer->SaveAsDefaultCustomizeData();

				((CECLoginUIMan*)m_pAUIManager)->ChangeScene((CECLoginUIMan::LOGIN_SCENE)(CECLoginUIMan::LOGIN_SCENE_CREATE1 + m_pCurPlayer->GetProfession()));
				
				m_pCurPlayer->SetDirAndUp(-g_pGame->GetViewport()->GetA3DCamera()->GetDirH(), g_vAxisY);
			}
			else
			{
				// set the customize data to current player
				CECPlayer::PLAYER_CUSTOMIZEDATA data = m_pCurPlayer->GetCustomizeData();
				data.faceData = *m_pCurFace->GetFaceData();
				m_pCurPlayer->ChangeCustomizeData(data);

				// 保存CustomizeData
				m_pCurPlayer->SaveAsDefaultCustomizeData();

				// now we send this data to the server to store it on the net
				g_pGame->GetGameSession()->SetRoleCustomizeData(((CECLoginUIMan*)m_pAUIManager)->GetVecRoleInfo()[((CECLoginUIMan*)m_pAUIManager)->GetIDCurRole()].roleid, (void *)&m_pCurPlayer->GetCustomizeData(), sizeof(CECPlayer::PLAYER_CUSTOMIZEDATA));

				// move this player back to its role position
				m_pCurPlayer->SetPos(((CECLoginUIMan*)m_pAUIManager)->GetTargetPos());
				m_pCurPlayer->SetDirAndUp(-g_pGame->GetViewport()->GetA3DCamera()->GetDirH(), g_vAxisY);

				m_pAUIManager->GetDialog("Win_Customize")->Show(false);
				m_pAUIManager->GetDialog("Win_Select")->Show(true);
				m_pAUIManager->GetDialog("Win_Manage")->Show(true);
				m_pAUIManager->GetDialog("Win_Manage2")->Show(true);
				((CECLoginUIMan*)m_pAUIManager)->ChangeScene(CECLoginUIMan::LOGIN_SCENE_SELCHAR);

				char szCommand[40] = "";
				sprintf(szCommand, "char%d", ((CECLoginUIMan*)m_pAUIManager)->GetIDCurRole() + 1);
				((CECLoginUIMan*)m_pAUIManager)->SetIDCurRole(-1);
				((CECLoginUIMan*)m_pAUIManager)->OnCommand_SelChar(szCommand, pDlg);
			}
			return true;
		}
	}

	else if ( nCurUIMan == CECUIManager::UIMAN_INGAME)//游戏内部
	{
	//	assert(0);

		if( 0 == stricmp(szCommand, "IDCANCEL") )
		{
			m_pAUIManager->ShowAllDialogs(false);

			m_pCurPlayer->RestoreCustomizeData();
			
			Release_Customize();//释放自定义对话框的资源

		
			// move this player back to its role position
			
//			m_pCurPlayer->SetPos(((CECLoginUIMan*)m_pAUIManager)->GetTargetPos());
//			m_pCurPlayer->SetDirAndUp(-g_pGame->GetViewport()->GetA3DCamera()->GetDirH(), g_vAxisY);
//
//			m_pAUIManager->GetDialog("Win_Customize")->Show(false);
//			m_pAUIManager->GetDialog("Win_Select")->Show(true);
//			m_pAUIManager->GetDialog("Win_Manage")->Show(true);
//			m_pAUIManager->GetDialog("Win_Manage2")->Show(true);

			return true;
		}

		if( 0 == stricmp(szCommand, "IDOK") )
		{
			m_pAUIManager->ShowAllDialogs(false);

			Release_Customize();//释放自定义对话框的资源


			// set the customize data to current player
			CECPlayer::PLAYER_CUSTOMIZEDATA data = m_pCurPlayer->GetCustomizeData();
			data.faceData = *m_pCurFace->GetFaceData();
			m_pCurPlayer->ChangeCustomizeData(data);

			// 保存CustomizeData
			m_pCurPlayer->SaveAsDefaultCustomizeData();

			int nRoleID = g_pGame->GetGameRun()->GetSelectedRoleInfo().roleid;
			g_pGame->GetGameSession()->SetRoleCustomizeData(nRoleID, (void *)&m_pCurPlayer->GetCustomizeData(), sizeof(CECPlayer::PLAYER_CUSTOMIZEDATA));

			// now we send this data to the server to store it on the net
			//g_pGame->GetGameSession()->SetRoleCustomizeData(((CECLoginUIMan*)m_pAUIManager)->GetVecRoleInfo()[((CECLoginUIMan*)m_pAUIManager)->GetIDCurRole()].roleid, (void *)&m_pCurPlayer->GetCustomizeData(), sizeof(CECPlayer::PLAYER_CUSTOMIZEDATA));

			// move this player back to its role position
//			m_pCurPlayer->SetPos(((CECLoginUIMan*)m_pAUIManager)->GetTargetPos());
//			m_pCurPlayer->SetDirAndUp(-g_pGame->GetViewport()->GetA3DCamera()->GetDirH(), g_vAxisY);
//
//			m_pAUIManager->GetDialog("Win_Customize")->Show(false);
//			m_pAUIManager->GetDialog("Win_Select")->Show(true);
//			m_pAUIManager->GetDialog("Win_Manage")->Show(true);
//			m_pAUIManager->GetDialog("Win_Manage2")->Show(true);
//			((CECLoginUIMan*)m_pAUIManager)->ChangeScene(CECLoginUIMan::LOGIN_SCENE_SELCHAR);
			
			return true;
		}
	}

	//截获切换窗口的消息
	const char *pszCommand = szCommand;
	
	if( OnCommand_SwitchDlg(pszCommand, pDlg))
		return true;

	if( 0 == strcmpi(pDlg->GetName(), "Win_ChooseFace") )
		return OnCommand_WinChooseFace(pszCommand, pDlg);

	if( 0 == strcmpi(pDlg->GetName(), "Win_ChooseEye") )
		return OnCommand_WinChooseEye(pszCommand, pDlg);

	if( 0 == strcmpi(pDlg->GetName(), "Win_ChooseEyebrow"))
		return OnCommand_WinChooseEyebrow(pszCommand, pDlg);
	
	
	if( 0 == strcmpi(pDlg->GetName(), "Win_ChooseNose"))
		return OnCommand_WinChooseNose(pszCommand, pDlg);

	

	if( 0 == strcmpi(pDlg->GetName(), "Win_ChooseMouth"))
		return OnCommand_WinChooseMouth(pszCommand,pDlg);

	
	if( 0 == strcmpi(pDlg->GetName(), "Win_ChooseEar"))
		return OnCommand_WinChooseEar(pszCommand,pDlg);


	if( 0 == strcmpi(pDlg->GetName(), "Win_AdvancedOption"))
		
		return OnCommand_AdvancedOption(pszCommand, pDlg);

	if( 0 == strcmpi(pDlg->GetName(), "Win_ChooseFacePainting"))
	
		return OnCommand_WinChooseFacePainting(pszCommand, pDlg);
	
	if( 0 == strcmpi(pDlg->GetName(), "Win_ChooseStature"))
		return OnCommand_WinChooseStature(pszCommand, pDlg);

	if( 0 == strcmpi(pDlg->GetName(), "Win_ChoosePreCustomize"))
		return OnCommand_WinChoosePreCustomize(pszCommand, pDlg);
	
	if( 0 == strcmpi(pDlg->GetName(), "Win_UserPic"))
		return OnCommand_WinUserPic(pszCommand, pDlg);

	//**********************用于测试***************************
	
	if( 0 == strcmpi(pDlg->GetName(), "Win_ChooseExpression"))
		return OnCommand_WinChooseExpression(pszCommand, pDlg);

	if( 0 == strcmpi(pDlg->GetName(), "Win_ExpressionList"))
		return OnCommand_WinExpressionList(pszCommand, pDlg);

	return true;
}


//响应ChooseFace 对话框命令
bool CECCustomize::OnCommand_WinChooseFace(const char *pszCommand, PAUIDIALOG pDlg)
{
	//调整滚动条
	if( 0 == strcmpi(pszCommand, "MoveSlider"))
	{
		static DWORD nLastTime = 0;

		DWORD nThisTime = (DWORD)(ACounter::GetMicroSecondNow()/1000);
		if(  nThisTime - nLastTime > 30 )
		{
			//更新滚动条控件
			UpdateChooseFaceSlider();
			nLastTime = nThisTime;

			return true;
		}

	}

	//复位
	else if( 0 == strcmpi( pszCommand, "Reset"))
	{
		//复位当前脸型
		ResetChooseFace();
		return true;
	}
	
	
	return true;
}

//选择眼型对话框
bool CECCustomize::OnCommand_WinChooseEye(const char *pszCommand, PAUIDIALOG pDlg)
{
	//重置眼型
	if( 0 == strcmpi(pszCommand,"Reset"))
	{
		ResetChooseEye();

		return true;
	}
	
	else if( 0 == strcmpi(pszCommand, "MoveSlider_ScaleEyeH"))
	{
		static DWORD nLastTime = 0;

		DWORD nThisTime = (DWORD)(ACounter::GetMicroSecondNow()/1000);
		if(  nThisTime - nLastTime > 30 )
		{
			if( m_pCheckBoxEyeAdjust->IsChecked())
			{
				m_nScaleEyeH = m_nScaleEyeH2 = m_pSliderScaleEyeH->GetLevel();
			}
			else
			{
				if( m_pBtnLeftEye->IsPushed())
				{
					m_nScaleEyeH = m_pSliderScaleEyeH->GetLevel();
				}
				else if( m_pBtnRightEye->IsPushed())
				{
					m_nScaleEyeH2 = m_pSliderScaleEyeH->GetLevel();
				}
			}
			
			//更新选择眼型的滚动条
			UpdateChooseEyeSlider();
			nLastTime = nThisTime;
			return true;
		}

	}

	else if( 0 == strcmpi(pszCommand, "MoveSlider_ScaleEyeV"))
	{
		static DWORD nLastTime = 0;

		DWORD nThisTime = (DWORD)(ACounter::GetMicroSecondNow()/1000);
		if(  nThisTime - nLastTime > 30 )
		{
			if( m_pCheckBoxEyeAdjust->IsChecked())
			{
				m_nScaleEyeV = m_nScaleEyeV2 = m_pSliderScaleEyeV->GetLevel();
			}
			else
			{
				if( m_pBtnLeftEye->IsPushed())
				{
					m_nScaleEyeV = m_pSliderScaleEyeV->GetLevel();
				}
				else if( m_pBtnRightEye->IsPushed())
				{
					m_nScaleEyeV2 = m_pSliderScaleEyeV->GetLevel();
				}
			}
			
			//更新选择眼型的滚动条
			UpdateChooseEyeSlider();
			nLastTime = nThisTime;
			return true;
		}

	}

	else if( 0 == strcmpi(pszCommand, "MoveSlider_RotateEye"))
	{
		static DWORD nLastTime = 0;

		DWORD nThisTime = (DWORD)(ACounter::GetMicroSecondNow()/1000);
		if(  nThisTime - nLastTime > 30 )
		{
			if( m_pCheckBoxEyeAdjust->IsChecked())
			{
				m_nRotateEye = m_nRotateEye2 = m_pSliderRotateEye->GetLevel();
			}
			else
			{
				if( m_pBtnLeftEye->IsPushed())
				{
					m_nRotateEye = m_pSliderRotateEye->GetLevel();
				}
				else if( m_pBtnRightEye->IsPushed())
				{
					m_nRotateEye2 = m_pSliderRotateEye->GetLevel();
				}
			}
			
			//更新选择眼型的滚动条
			UpdateChooseEyeSlider();
			nLastTime = nThisTime;
			return true;
		}

	}

	else if( 0 == strcmpi(pszCommand, "MoveSlider_OffsetEyeH"))
	{
		static DWORD nLastTime = 0;

		DWORD nThisTime = (DWORD)(ACounter::GetMicroSecondNow()/1000);
		if(  nThisTime - nLastTime > 30 )
		{
			if( m_pCheckBoxEyeAdjust->IsChecked())
			{
				m_nOffsetEyeH = m_nOffsetEyeH2 = m_pSliderOffsetEyeH->GetLevel();
			}
			else
			{
				if( m_pBtnLeftEye->IsPushed())
				{
					m_nOffsetEyeH = m_pSliderOffsetEyeH->GetLevel();
				}
				else if( m_pBtnRightEye->IsPushed())
				{
					m_nOffsetEyeH2 = m_pSliderOffsetEyeH->GetLevel();
				}
			}
			
			//更新选择眼型的滚动条
			UpdateChooseEyeSlider();
			nLastTime = nThisTime;
			return true;
		}

	}

	else if( 0 == strcmpi(pszCommand, "MoveSlider_OffsetEyeV"))
	{
		static DWORD nLastTime = 0;

		DWORD nThisTime = (DWORD)(ACounter::GetMicroSecondNow()/1000);
		if(  nThisTime - nLastTime > 30 )
		{
			if( m_pCheckBoxEyeAdjust->IsChecked())
			{
				m_nOffsetEyeV = m_nOffsetEyeV2 = m_pSliderOffsetEyeV->GetLevel();
			}
			else
			{
				if( m_pBtnLeftEye->IsPushed())
				{
					m_nOffsetEyeV = m_pSliderOffsetEyeV->GetLevel();
				}
				else if( m_pBtnRightEye->IsPushed())
				{
					m_nOffsetEyeV2 = m_pSliderOffsetEyeV->GetLevel();
				}
			}
			
			//更新选择眼型的滚动条
			UpdateChooseEyeSlider();
			nLastTime = nThisTime;
			return true;
		}

	}

	else if( 0 == strcmpi(pszCommand, "MoveSlider_OffsetEyeZ"))
	{
		static DWORD nLastTime = 0;

		DWORD nThisTime = (DWORD)(ACounter::GetMicroSecondNow()/1000);
		if(  nThisTime - nLastTime > 30 )
		{
			if( m_pCheckBoxEyeAdjust->IsChecked())
			{
				m_nOffsetEyeZ = m_nOffsetEyeZ2 = m_pSliderOffsetEyeZ->GetLevel();
			}
			else
			{
				if( m_pBtnLeftEye->IsPushed())
				{
					m_nOffsetEyeZ = m_pSliderOffsetEyeZ->GetLevel();
				}
				else if( m_pBtnRightEye->IsPushed())
				{
					m_nOffsetEyeZ2 = m_pSliderOffsetEyeZ->GetLevel();
				}
			}
			
			//更新选择眼型的滚动条
			UpdateChooseEyeSlider();
			nLastTime = nThisTime;
			return true;
		}

	}
	else if( 0 == strcmpi(pszCommand, "MoveSlider_ScaleEyeBall"))
	{
		static DWORD nLastTime = 0;

		DWORD nThisTime = (DWORD)(ACounter::GetMicroSecondNow()/1000);
		if(  nThisTime - nLastTime > 30 )
		{
			if( m_pCheckBoxEyeAdjust->IsChecked())
			{
				m_nScaleEyeBall = m_nScaleEyeBall2 = m_pSliderScaleEyeBall->GetLevel();
			}
			else
			{
				if( m_pBtnLeftEye->IsPushed())
				{
					m_nScaleEyeBall = m_pSliderScaleEyeBall->GetLevel();
				}
				else if( m_pBtnRightEye->IsPushed())
				{
					m_nScaleEyeBall2 = m_pSliderScaleEyeBall->GetLevel();
				}
			}
			
			//更新选择眼型的滚动条
			UpdateChooseEyeSlider();
			nLastTime = nThisTime;
			return true;
		}

	}

	else if( 0 == strcmpi(pszCommand, "ChooseBaseEye1") )
	{	
		bool bShow = m_pListBoxEyeShape1->IsShow();
		m_pListBoxEyeShape1->Show(!bShow);

		m_pListBoxEyeShape2->Show(false);
		m_pListBoxEyeShape3->Show(false);

		m_pBtmChooseBaseEye1->SetPushed(!bShow);
		m_pBtmChooseBaseEye2->SetPushed(false);
		m_pBtmChooseBaseEye3->SetPushed(false);

		return true;
		
	}
	else if( 0 == strcmpi(pszCommand, "ChooseBaseEye2") )
	{	
		bool bShow = m_pListBoxEyeShape2->IsShow();
		m_pListBoxEyeShape2->Show(!bShow);

		m_pListBoxEyeShape1->Show(false);
		m_pListBoxEyeShape3->Show(false);

		m_pBtmChooseBaseEye1->SetPushed(false);
		m_pBtmChooseBaseEye2->SetPushed(!bShow);
		m_pBtmChooseBaseEye3->SetPushed(false);
		return true;
		
	}
	else if( 0 == strcmpi(pszCommand, "ChooseBaseEye3") )
	{	
		bool bShow = m_pListBoxEyeShape3->IsShow();
		m_pListBoxEyeShape3->Show(!bShow);

		m_pListBoxEyeShape1->Show(false);
		m_pListBoxEyeShape2->Show(false);

		m_pBtmChooseBaseEye1->SetPushed(false);
		m_pBtmChooseBaseEye2->SetPushed(false);
		m_pBtmChooseBaseEye3->SetPushed(!bShow);

		return true;
		
	}
	else if( 0 == strcmpi(pszCommand, "ChooseBaseEyeTex1"))
	{
		bool bShow = m_pListBoxEyeBaseTex1->IsShow();
		m_pListBoxEyeBaseTex1->Show(!bShow);

		m_pListBoxEyeBaseTex2->Show(false);
		m_pListBoxEyeBaseTex3->Show(false);

		m_pBtnChooseEyeBaseTex1->SetPushed(!bShow);
		m_pBtnChooseEyeBaseTex2->SetPushed(false);
		m_pBtnChooseEyeBaseTex3->SetPushed(false);
		return true;
	}
	else if( 0 == strcmpi(pszCommand, "ChooseBaseEyeTex2"))
	{
		bool bShow = m_pListBoxEyeBaseTex2->IsShow();
		m_pListBoxEyeBaseTex2->Show(!bShow);

		m_pListBoxEyeBaseTex1->Show(false);
		m_pListBoxEyeBaseTex3->Show(false);

		m_pBtnChooseEyeBaseTex1->SetPushed(false);
		m_pBtnChooseEyeBaseTex2->SetPushed(!bShow);
		m_pBtnChooseEyeBaseTex3->SetPushed(false);
		return true;
	}
	else if( 0 == strcmpi(pszCommand, "ChooseBaseEyeTex3"))
	{
		bool bShow = m_pListBoxEyeBaseTex3->IsShow();
		m_pListBoxEyeBaseTex3->Show(!bShow);

		m_pListBoxEyeBaseTex1->Show(false);
		m_pListBoxEyeBaseTex2->Show(false);

		m_pBtnChooseEyeBaseTex1->SetPushed(false);
		m_pBtnChooseEyeBaseTex2->SetPushed(false);
		m_pBtnChooseEyeBaseTex3->SetPushed(!bShow);
		return true;
	}

	else if( 0 == strcmpi(pszCommand, "OnBtnLeftEye"))
	{
		for( int i = 0; i < 7; i++)
		{
			
			const ACHAR* pszName = m_pAUIManager->GetStringFromTable(401+i);
			m_pLabelEye[i]->SetText(pszName);
		}

		m_pSliderScaleEyeH->SetLevel(m_nScaleEyeH);
		m_pSliderScaleEyeV->SetLevel(m_nScaleEyeV );
		m_pSliderRotateEye->SetLevel(m_nRotateEye );
		m_pSliderOffsetEyeH->SetLevel(m_nOffsetEyeH );
		m_pSliderOffsetEyeV->SetLevel(m_nOffsetEyeV );
		m_pSliderOffsetEyeZ->SetLevel(m_nOffsetEyeZ );
		m_pSliderScaleEyeBall->SetLevel(m_nScaleEyeBall );
		
		m_pBtnLeftEye->SetPushed(true);
		m_pBtnRightEye->SetPushed(false);

		return true;
	}
	
	else if(0 == strcmpi(pszCommand, "OnBtnRightEye"))
	{
		for( int i = 0; i < 7; i++)
		{
			
			const ACHAR* pszName = m_pAUIManager->GetStringFromTable(501+i);
			m_pLabelEye[i]->SetText(pszName);
		}

		m_pSliderScaleEyeH->SetLevel(m_nScaleEyeH2);
		m_pSliderScaleEyeV->SetLevel(m_nScaleEyeV2);
		m_pSliderRotateEye->SetLevel(m_nRotateEye2);
		m_pSliderOffsetEyeH->SetLevel(m_nOffsetEyeH2);
		m_pSliderOffsetEyeV->SetLevel(m_nOffsetEyeV2);
		m_pSliderOffsetEyeZ->SetLevel(m_nOffsetEyeZ2);
		m_pSliderScaleEyeBall->SetLevel(m_nScaleEyeBall2);

		m_pBtnRightEye->SetPushed(true);
		m_pBtnLeftEye->SetPushed(false);
		return true;
	}

	return true;
}

//选择眉型对话框
bool CECCustomize::OnCommand_WinChooseEyebrow(const char *pszCommand, PAUIDIALOG pDlg)
{
	//重置脸型
	if( 0 == strcmpi(pszCommand,"Reset"))
	{
		ResetChooseBrow();
		return true;
	}

	//拖动滚动条
	else if(0 == strcmpi(pszCommand,"MoveSlider_ScaleBrowH"))
	{
		static DWORD nLastTime = 0;

		DWORD nThisTime = (DWORD)(ACounter::GetMicroSecondNow()/1000);
		if(  nThisTime - nLastTime > 30 )
		{
			if( m_pCheckBoxBrowAdjust->IsChecked())
			{
				m_nScaleBrowH = m_nScaleBrowH2 = m_pSliderScaleBrowH->GetLevel();
			}
			else
			{
				if( m_pBtnLeftBrow->IsPushed())
				{
					m_nScaleBrowH = m_pSliderScaleBrowH->GetLevel();
				}
				else if( m_pBtnRightBrow->IsPushed())
				{
					m_nScaleBrowH2 = m_pSliderScaleBrowH->GetLevel();
				}
			}
			
			//更新眉型滚动条
			UpdateChooseBrowSlider();
			nLastTime = nThisTime;
			return true;
		}
	
	}

	//拖动滚动条
	else if(0 == strcmpi(pszCommand,"MoveSlider_ScaleBrowV"))
	{
		static DWORD nLastTime = 0;

		DWORD nThisTime = (DWORD)(ACounter::GetMicroSecondNow()/1000);
		if(  nThisTime - nLastTime > 30 )
		{
			if( m_pCheckBoxBrowAdjust->IsChecked())
			{
				m_nScaleBrowV = m_nScaleBrowV2 = m_pSliderScaleBrowV->GetLevel();
			}
			else
			{
				if( m_pBtnLeftBrow->IsPushed())
				{
					m_nScaleBrowV = m_pSliderScaleBrowV->GetLevel();
				}
				else if( m_pBtnRightBrow->IsPushed())
				{
					m_nScaleBrowV2 = m_pSliderScaleBrowV->GetLevel();
				}
			}
			
			//更新眉型滚动条
			UpdateChooseBrowSlider();
			nLastTime = nThisTime;
			return true;
		}
	
	}

	//拖动滚动条
	else if(0 == strcmpi(pszCommand,"MoveSlider_RotateBrow"))
	{
		static DWORD nLastTime = 0;

		DWORD nThisTime = (DWORD)(ACounter::GetMicroSecondNow()/1000);
		if(  nThisTime - nLastTime > 30 )
		{
			if( m_pCheckBoxBrowAdjust->IsChecked())
			{
				m_nRotateBrow = m_nRotateBrow2 = m_pSliderRotateBrow->GetLevel();
			}
			else
			{
				if( m_pBtnLeftBrow->IsPushed())
				{
					m_nRotateBrow = m_pSliderRotateBrow->GetLevel();
				}
				else if( m_pBtnRightBrow->IsPushed())
				{
					m_nRotateBrow2 = m_pSliderRotateBrow->GetLevel();
				}
			}
			
			//更新眉型滚动条
			UpdateChooseBrowSlider();
			nLastTime = nThisTime;
			return true;
		}
	
	}

	//拖动滚动条
	else if(0 == strcmpi(pszCommand,"MoveSlider_OffsetBrowH"))
	{
		static DWORD nLastTime = 0;

		DWORD nThisTime = (DWORD)(ACounter::GetMicroSecondNow()/1000);
		if(  nThisTime - nLastTime > 30 )
		{
			if( m_pCheckBoxBrowAdjust->IsChecked())
			{
				m_nOffsetBrowH= m_nOffsetBrowH2 = m_pSliderOffsetBrowH->GetLevel();
			}
			else
			{
				if( m_pBtnLeftBrow->IsPushed())
				{
					m_nOffsetBrowH = m_pSliderOffsetBrowH->GetLevel();
				}
				else if( m_pBtnRightBrow->IsPushed())
				{
					m_nOffsetBrowH2 = m_pSliderOffsetBrowH->GetLevel();
				}
			}
			
			//更新眉型滚动条
			UpdateChooseBrowSlider();
			nLastTime = nThisTime;
			return true;
		}
	
	}

	//拖动滚动条
	else if(0 == strcmpi(pszCommand,"MoveSlider_OffsetBrowV"))
	{
		static DWORD nLastTime = 0;

		DWORD nThisTime = (DWORD)(ACounter::GetMicroSecondNow()/1000);
		if(  nThisTime - nLastTime > 30 )
		{
			if( m_pCheckBoxBrowAdjust->IsChecked())
			{
				m_nOffsetBrowV= m_nOffsetBrowV2 = m_pSliderOffsetBrowV->GetLevel();
			}
			else
			{
				if( m_pBtnLeftBrow->IsPushed())
				{
					m_nOffsetBrowV = m_pSliderOffsetBrowV->GetLevel();
				}
				else if( m_pBtnRightBrow->IsPushed())
				{
					m_nOffsetBrowV2 = m_pSliderOffsetBrowV->GetLevel();
				}
			}
			
			//更新眉型滚动条
			UpdateChooseBrowSlider();
			nLastTime = nThisTime;
			return true;
		}
	
	}
		
	//拖动滚动条
	else if(0 == strcmpi(pszCommand,"MoveSlider_OffsetBrowZ"))
	{
		static DWORD nLastTime = 0;

		DWORD nThisTime = (DWORD)(ACounter::GetMicroSecondNow()/1000);
		if(  nThisTime - nLastTime > 30 )
		{
			if( m_pCheckBoxBrowAdjust->IsChecked())
			{
				m_nOffsetBrowZ= m_nOffsetBrowZ2 = m_pSliderOffsetBrowZ->GetLevel();
			}
			else
			{
				if( m_pBtnLeftBrow->IsPushed())
				{
					m_nOffsetBrowZ = m_pSliderOffsetBrowZ->GetLevel();
				}
				else if( m_pBtnRightBrow->IsPushed())
				{
					m_nOffsetBrowZ = m_pSliderOffsetBrowZ->GetLevel();
				}
			}
			
			//更新眉型滚动条
			UpdateChooseBrowSlider();
			nLastTime = nThisTime;
			return true;
		}
	
	}

	//显示选择1号基本眉型对话框
	else if( 0 == strcmpi(pszCommand, "ChooseBaseEyebrow1") )
	{	
		bool bShow = m_pListBoxBrow->IsShow();
		m_pListBoxBrow->Show(!bShow);
		m_pBtmChooseBaseEyebrow1->SetPushed(!bShow);
		
		return true;
		
	}

	else if( 0 == strcmpi(pszCommand, "OnBtnLeftBrow"))
	{
		for( int i = 0; i < 6; i++)
		{
			
			const ACHAR* pszName = m_pAUIManager->GetStringFromTable(601+i);
			m_pLabelBrow[i]->SetText(pszName);
		}
		m_pSliderScaleBrowH->SetLevel(m_nScaleBrowH);
		m_pSliderScaleBrowV->SetLevel(m_nScaleBrowV);
		m_pSliderRotateBrow->SetLevel(m_nRotateBrow);
		m_pSliderOffsetBrowH->SetLevel(m_nOffsetBrowH);
		m_pSliderOffsetBrowV->SetLevel(m_nOffsetBrowV);
		m_pSliderOffsetBrowZ->SetLevel(m_nOffsetBrowZ);

		m_pBtnLeftBrow->SetPushed(true);
		m_pBtnRightBrow->SetPushed(false);
		
		return true;
	}

	else if ( 0 == strcmpi(pszCommand, "OnBtnRightBrow"))
	{
		for( int i = 0; i < 6; i++)
		{
			
			const ACHAR* pszName = m_pAUIManager->GetStringFromTable(701+i);
			m_pLabelBrow[i]->SetText(pszName);
		}

		m_pSliderScaleBrowH->SetLevel(m_nScaleBrowH2);
		m_pSliderScaleBrowV->SetLevel(m_nScaleBrowV2);
		m_pSliderRotateBrow->SetLevel(m_nRotateBrow2);
		m_pSliderOffsetBrowH->SetLevel(m_nOffsetBrowH2);
		m_pSliderOffsetBrowV->SetLevel(m_nOffsetBrowV2);
		m_pSliderOffsetBrowZ->SetLevel(m_nOffsetBrowZ2);
		
		m_pBtnRightBrow->SetPushed(true);
		m_pBtnLeftBrow->SetPushed(false);

		return true;
	}
	
	return true;
}

//选择鼻型对话框
bool CECCustomize::OnCommand_WinChooseNose(const char *pszCommand, PAUIDIALOG pDlg)
{
	//重置鼻型型
	if( 0 == strcmpi(pszCommand,"Reset"))
	{
		ResetChooseNose();
		return true;
	}
	
	//拖动滚动条
	else if(0 == strcmpi(pszCommand,"MoveSlider"))
	{
		static DWORD nLastTime = 0;

		DWORD nThisTime =  (DWORD)(ACounter::GetMicroSecondNow()/1000);
		if(  nThisTime - nLastTime > 30 )
		{
			//更新鼻子滚动条
			UpdateChooseNoseSlider();

			nLastTime = nThisTime;
			
		}

		
		return true;
		
	}

	//显示选择基本鼻头对话框
	else if( 0 == strcmpi(pszCommand, "ChooseBaseNoseTip") )
	{	
		bool bShow = m_pListBoxNoseTip->IsShow();
		m_pListBoxNoseTip->Show(!bShow);
		m_pListBoxNoseBridge->Show(false);

		m_pBtmChooseBaseNoseTip->SetPushed(!bShow);
		m_pBtmChooseBaseNoseBridge->SetPushed(false);
		return true;
		
	}

	//显示选择基本鼻梁对话框
	else if( 0 == strcmpi(pszCommand, "ChooseBaseNoseBridge") )
	{	
		bool bShow = m_pListBoxNoseBridge->IsShow();
		m_pListBoxNoseBridge->Show(!bShow);
		m_pListBoxNoseTip->Show(false);

		m_pBtmChooseBaseNoseTip->SetPushed(false);
		m_pBtmChooseBaseNoseBridge->SetPushed(!bShow);
		
		return true;
		
	}
	return true;
}

//选择嘴型对话框
bool CECCustomize::OnCommand_WinChooseMouth(const char *pszCommand, PAUIDIALOG pDlg)
{

	//重置嘴型
	if( 0 == strcmpi(pszCommand,"Reset"))
	{
		ResetChooseMouth();
		return true;
	}
	//重置嘴型
	else if( 0 == strcmpi(pszCommand,"MoveSlider"))
	{
		static DWORD nLastTime = 0;

		DWORD nThisTime =  (DWORD)(ACounter::GetMicroSecondNow()/1000);
		if(  nThisTime - nLastTime > 30 )
		{
			//更新嘴型滚动条
			UpdateChooseMouthSlider();
			nLastTime = nThisTime;
		}
		return true;
	}
	else if( 0 == strcmpi(pszCommand, "MoveSlider_ScaleMouthH"))
	{
		static DWORD nLastTime = 0;

		DWORD nThisTime =  (DWORD)(ACounter::GetMicroSecondNow()/1000);
		if(  nThisTime - nLastTime > 30 )
		{
			if( m_pCheckBoxScaleMouthH->IsChecked())
			{
				int nLevel = m_pSliderScaleMouthH->GetLevel();
				m_pSliderScaleMouthH2->SetLevel(nLevel);
			}

			//更新嘴型滚动条
			UpdateChooseMouthSlider();
			nLastTime = nThisTime;
		}
		return true;
	}
	else if( 0 == strcmpi(pszCommand, "MoveSlider_ScaleMouthH2"))
	{
		static DWORD nLastTime = 0;

		DWORD nThisTime =  (DWORD)(ACounter::GetMicroSecondNow()/1000);
		if(  nThisTime - nLastTime > 30 )
		{
			if( m_pCheckBoxScaleMouthH->IsChecked())
			{
				int nLevel = m_pSliderScaleMouthH2->GetLevel();
				m_pSliderScaleMouthH->SetLevel(nLevel);
			}

			//更新嘴型滚动条
			UpdateChooseMouthSlider();
			nLastTime = nThisTime;
		}
		return true;
	}
	else if( 0 == strcmpi(pszCommand, "MoveSlider_OffsetCornerOfMouthSpecial"))
	{
		static DWORD nLastTime = 0;

		DWORD nThisTime =  (DWORD)(ACounter::GetMicroSecondNow()/1000);
		if(  nThisTime - nLastTime > 30 )
		{
			if( m_pCheckBoxOffsetCornerOfMouthSpecial->IsChecked())
			{
				int nLevel = m_pSliderOffsetCornerOfMouthSpecial->GetLevel();
				m_pSliderOffsetCornerOfMouthSpecial2->SetLevel(nLevel);
			}

			//更新嘴型滚动条
			UpdateChooseMouthSlider();
			nLastTime = nThisTime;
		}
		return true;
	}

	else if( 0 == strcmpi(pszCommand, "MoveSlider_OffsetCornerOfMouthSpecial2"))
	{
		static DWORD nLastTime = 0;

		DWORD nThisTime =  (DWORD)(ACounter::GetMicroSecondNow()/1000);
		if(  nThisTime - nLastTime > 30 )
		{
			if( m_pCheckBoxOffsetCornerOfMouthSpecial->IsChecked())
			{
				int nLevel = m_pSliderOffsetCornerOfMouthSpecial2->GetLevel();
				m_pSliderOffsetCornerOfMouthSpecial->SetLevel(nLevel);
			}

			//更新嘴型滚动条
			UpdateChooseMouthSlider();
			nLastTime = nThisTime;
		}
		return true;
	}
	
	//显示选择基本上唇线
	else if( 0 == strcmpi(pszCommand, "ChooseBaseUpLipLine") )
	{	
		bool bShow = m_pListBoxUpLipLine->IsShow();
		m_pListBoxUpLipLine->Show(!bShow);

		m_pListBoxMidLipLine->Show(false);
		m_pListBoxDownLipLine->Show(false);

		m_pBtmChooseMouthUpLipLine->SetPushed(!bShow);
		m_pBtmChooseMouthMidLipLine->SetPushed(false);
		m_pBtmChooseMouthDownLipLine->SetPushed(false);

		return true;
	}
	
	//显示选择基本唇沟
	else if( 0 == strcmpi(pszCommand, "ChooseBaseMiddleLipLine") )
	{	
		bool bShow = m_pListBoxMidLipLine->IsShow();
		m_pListBoxMidLipLine->Show(!bShow);

		m_pListBoxUpLipLine->Show(false);
		m_pListBoxDownLipLine->Show(false);

		m_pBtmChooseMouthUpLipLine->SetPushed(false);
		m_pBtmChooseMouthMidLipLine->SetPushed(!bShow);
		m_pBtmChooseMouthDownLipLine->SetPushed(false);


		return true;
	}
	//显示选择基本下唇线
	else if( 0 == strcmpi(pszCommand, "ChooseBaseDownLipLine") )
	{	
		bool bShow = m_pListBoxDownLipLine->IsShow();
		m_pListBoxDownLipLine->Show(!bShow);

		m_pListBoxUpLipLine->Show(false);
		m_pListBoxMidLipLine->Show(false);

		m_pBtmChooseMouthUpLipLine->SetPushed(false);
		m_pBtmChooseMouthMidLipLine->SetPushed(false);
		m_pBtmChooseMouthDownLipLine->SetPushed(!bShow);

		return true;
	}
	return true;
}

//选择耳型对话框
bool CECCustomize::OnCommand_WinChooseEar(const char *pszCommand, PAUIDIALOG pDlg)
{
	//重置耳型
	if( 0 == strcmpi(pszCommand,"Reset"))
	{
		ResetChooseEar();
		return true;
	}
	
	//滚动条耳型
	else if( 0 == strcmpi(pszCommand,"MoveSlider"))
	{
		static DWORD nLastTime = 0;

		DWORD nThisTime =  (DWORD)(ACounter::GetMicroSecondNow()/1000);
		if(  nThisTime - nLastTime > 30 )
		{
			//更新滚动条控件
			UpdateChooseEarSlider();
			nLastTime = nThisTime;

			return true;
		}
		
	}

	//显示选择1号基本耳型对话框
	else if( 0 == strcmpi(pszCommand, "ChooseBaseEar1") )
	{	
		bool bShow = m_pListBoxEar->IsShow();

		m_pListBoxEar->Show(!bShow);

		m_pBtmChooseBaseEar1->SetPushed(!bShow);

		return true;
	}
	return true;
}

bool CECCustomize::OnCommand_SwitchDlg(const char *pszCommand, PAUIDIALOG pDlg)
{
	
	//获取骨骼控制器
	CFaceBoneController* pBoneCtrler = m_pCurFace->GetBoneController();
	

	if( 0 == strcmpi(pszCommand, "ChooseFace") )
	{
		//关闭对话框
		pDlg->Show(false);
		POINT DlgPos = pDlg->GetPos();
		PAUIDIALOG pOtherDlg = m_pAUIManager->GetDialog("Win_ChooseFace");

		// old : pOtherDlg->SetPos(DlgPos.x, DlgPos.y);
		pOtherDlg->SetPosEx(DlgPos.x, DlgPos.y);

		pOtherDlg->Show(true);
		pOtherDlg->SetCanMove(false);
		
		
		//------------------------------------
		//预先计算除了脸型和3庭以外的数据
		//------------------------------------

		//初始化骨骼
		pBoneCtrler->RestoreBoneTM();

		CalculateEye();//计算眼型
		CalculateBrow();//计算眉型
		CalculateNose();//计算鼻型
		CalculateMouth();//计算嘴型
		CalculateEar();//计算耳型
		CalculateAdvaceOption();//计算高级选项

		pBoneCtrler->BackupBoneTM ();//备份之前的数据

		CalculateBlendFace();
		CalculateFace();
		Calculate3Parts();
		
		int nCurUIMan = g_pGame->GetGameRun()->GetUIManager()->GetCurrentUIMan();

		if(nCurUIMan == CECUIManager::UIMAN_LOGIN)
		{
			((CECLoginUIMan*)m_pAUIManager)->ChangeScene((CECLoginUIMan::LOGIN_SCENE)(CECLoginUIMan::LOGIN_SCENE_FACECUSTOMIZE1 + m_pCurPlayer->GetProfession()));
		}
		else if( nCurUIMan == CECUIManager::UIMAN_INGAME)
		{
		//	assert(0);
		}

		m_pBtmCurChoose->SetPushed(false);
		m_pBtmCurChoose = m_pBtmChooseFace;
		m_pBtmCurChoose->SetPushed(true);

		return true;
	}
	else if( 0 == strcmpi(pszCommand, "ChooseEye") )
	{
		//关闭对话框
		pDlg->Show(false);
		POINT DlgPos = pDlg->GetPos();
		PAUIDIALOG pOtherDlg = m_pAUIManager->GetDialog("Win_ChooseEye");

		// old : pOtherDlg->SetPos(DlgPos.x, DlgPos.y);
		pOtherDlg->SetPosEx(DlgPos.x, DlgPos.y);

		pOtherDlg->Show(true);
		pOtherDlg->SetCanMove(false);
		

		//------------------------------------
		//预先计算除了眼睛和3庭以外的数据
		//------------------------------------

		//初始化骨骼
		pBoneCtrler->RestoreBoneTM();

		CalculateBrow();//计算眉型
		CalculateNose();//计算鼻型
		CalculateMouth();//计算嘴型
		CalculateEar();//计算耳型
		CalculateAdvaceOption();//计算高级选项

		CalculateBlendFace ();//计算混合脸型
		CalculateFace();//计算脸盘
		
		pBoneCtrler->BackupBoneTM ();//备份之前的数据
		
		CalculateEye();//计算眼型
		Calculate3Parts();//计算3庭
		
		int nCurUIMan = g_pGame->GetGameRun()->GetUIManager()->GetCurrentUIMan();

		if(nCurUIMan == CECUIManager::UIMAN_LOGIN)
		{
			((CECLoginUIMan*)m_pAUIManager)->ChangeScene((CECLoginUIMan::LOGIN_SCENE)(CECLoginUIMan::LOGIN_SCENE_FACECUSTOMIZE1 +  m_pCurPlayer->GetProfession()));
		}
		else if( nCurUIMan == CECUIManager::UIMAN_INGAME)
		{
	//		assert(0);
		}


		m_pBtmCurChoose->SetPushed(false);
		m_pBtmCurChoose = m_pBtmChooseEye;
		m_pBtmCurChoose->SetPushed(true);

		return true;


		
	}
	else if( 0 == strcmpi(pszCommand, "ChooseEyebrow") )
	{
		//关闭对话框
		pDlg->Show(false);
		POINT DlgPos = pDlg->GetPos();
		PAUIDIALOG pOtherDlg = m_pAUIManager->GetDialog("Win_ChooseEyebrow");

		// old : pOtherDlg->SetPos(DlgPos.x, DlgPos.y);
		pOtherDlg->SetPosEx(DlgPos.x, DlgPos.y);
		
		pOtherDlg->Show(true);
		pOtherDlg->SetCanMove(false);

		//------------------------------------
		//预先计算除了眉型和3庭以外的数据
		//------------------------------------

		//初始化骨骼
		pBoneCtrler->RestoreBoneTM();
		
		CalculateEye();//计算眼型
		CalculateNose();//计算鼻型
		CalculateMouth();//计算嘴型
		CalculateEar();//计算耳型
		CalculateAdvaceOption();//计算高级选项
		
		CalculateBlendFace ();//计算混合脸型
		CalculateFace();//计算脸盘
		
		pBoneCtrler->BackupBoneTM ();//备份之前的数据
		
		CalculateBrow();//计算眉型
		Calculate3Parts();//计算3庭

		int nCurUIMan = g_pGame->GetGameRun()->GetUIManager()->GetCurrentUIMan();


		if(nCurUIMan == CECUIManager::UIMAN_LOGIN)
		{
			((CECLoginUIMan*)m_pAUIManager)->ChangeScene((CECLoginUIMan::LOGIN_SCENE)(CECLoginUIMan::LOGIN_SCENE_FACECUSTOMIZE1 +  m_pCurPlayer->GetProfession()));
		}
		else if( nCurUIMan == CECUIManager::UIMAN_INGAME)
		{
	//		assert(0);
		}

		m_pBtmCurChoose->SetPushed(false);
		m_pBtmCurChoose = m_pBtmChooseBrow;
		m_pBtmCurChoose->SetPushed(true);

		return true;
		
	}
	else if( 0 == strcmpi(pszCommand, "ChooseNose") )
	{
		//关闭对话框
		pDlg->Show(false);
		POINT DlgPos = pDlg->GetPos();
		PAUIDIALOG pOtherDlg = m_pAUIManager->GetDialog("Win_ChooseNose");

		// old : pOtherDlg->SetPos(DlgPos.x, DlgPos.y);
		pOtherDlg->SetPosEx(DlgPos.x, DlgPos.y);
		
		pOtherDlg->Show(true);
		pOtherDlg->SetCanMove(false);

		//------------------------------------
		//预先计算除了鼻型和3庭以外的数据
		//------------------------------------

		//初始化骨骼
		pBoneCtrler->RestoreBoneTM();
		
		CalculateEye();//计算眼型
		CalculateBrow();//计算鼻型
		CalculateMouth();//计算嘴型
		CalculateEar();//计算耳型
		CalculateAdvaceOption();//计算高级选项

		CalculateBlendFace ();//计算混合脸型
		CalculateFace();//计算脸盘
		
		pBoneCtrler->BackupBoneTM ();//备份之前的数据
		
		CalculateNose();//计算鼻型
		Calculate3Parts();//计算3庭

		int nCurUIMan = g_pGame->GetGameRun()->GetUIManager()->GetCurrentUIMan();

		if(nCurUIMan == CECUIManager::UIMAN_LOGIN)
		{
			((CECLoginUIMan*)m_pAUIManager)->ChangeScene((CECLoginUIMan::LOGIN_SCENE)(CECLoginUIMan::LOGIN_SCENE_FACECUSTOMIZE1 + m_pCurPlayer->GetProfession()));
		}
		else if( nCurUIMan == CECUIManager::UIMAN_INGAME)
		{
	//		assert(0);
		}


		m_pBtmCurChoose->SetPushed(false);
		m_pBtmCurChoose = m_pBtmChooseNose;
		m_pBtmCurChoose->SetPushed(true);

		return true;
		
	}
	else if( 0 == strcmpi(pszCommand, "ChooseMouth") )
	{
		//关闭对话框
		pDlg->Show(false);
		POINT DlgPos = pDlg->GetPos();
		PAUIDIALOG pOtherDlg = m_pAUIManager->GetDialog("Win_ChooseMouth");
		
		// old : pOtherDlg->SetPos(DlgPos.x, DlgPos.y);
		pOtherDlg->SetPosEx(DlgPos.x, DlgPos.y);
		
		pOtherDlg->Show(true);
		pOtherDlg->SetCanMove(false);
		
		//------------------------------------
		//预先计算除了嘴型和3庭以外的数据
		//------------------------------------

		//初始化骨骼
		pBoneCtrler->RestoreBoneTM();
		
		CalculateEye();//计算眼型
		CalculateBrow();//计算眉型
		CalculateNose();//计算鼻型
		CalculateEar();//计算耳型
		CalculateAdvaceOption();//计算高级选项
		
		CalculateBlendFace ();//计算混合脸型
		CalculateFace();//计算脸盘
		
		pBoneCtrler->BackupBoneTM ();//备份之前的数据
		
		CalculateMouth();//计算嘴型
		Calculate3Parts();//计算3庭

		int nCurUIMan = g_pGame->GetGameRun()->GetUIManager()->GetCurrentUIMan();

		if(nCurUIMan == CECUIManager::UIMAN_LOGIN)
		{
			((CECLoginUIMan*)m_pAUIManager)->ChangeScene((CECLoginUIMan::LOGIN_SCENE)(CECLoginUIMan::LOGIN_SCENE_FACECUSTOMIZE1 +  m_pCurPlayer->GetProfession()));
		}
		else if( nCurUIMan == CECUIManager::UIMAN_INGAME)
		{
	//		assert(0);
		}
		

		m_pBtmCurChoose->SetPushed(false);
		m_pBtmCurChoose = m_pBtmChooseMouth;
		m_pBtmCurChoose->SetPushed(true);

		return true;
		
	}
	else if( 0 == strcmpi(pszCommand, "ChooseEar") )
	{
		//关闭对话框
		pDlg->Show(false);
		POINT DlgPos = pDlg->GetPos();
		PAUIDIALOG pOtherDlg = m_pAUIManager->GetDialog("Win_ChooseEar");

		// old : pOtherDlg->SetPos(DlgPos.x, DlgPos.y);
		pOtherDlg->SetPosEx(DlgPos.x, DlgPos.y);
		
		pOtherDlg->Show(true);
		pOtherDlg->SetCanMove(false);
		
		//------------------------------------
		//预先计算除了嘴型和3庭以外的数据
		//------------------------------------

		//初始化骨骼
		pBoneCtrler->RestoreBoneTM();
		
		CalculateEye();//计算眼型
		CalculateBrow();//计算眉型
		CalculateNose();//计算鼻型
		CalculateMouth();//计算嘴型
		CalculateAdvaceOption();//计算高级选项
		
		CalculateBlendFace ();//计算混合脸型
		CalculateFace();//计算脸盘
		
		pBoneCtrler->BackupBoneTM ();//备份之前的数据
		
		CalculateEar();//计算耳型
		Calculate3Parts();//计算3庭

		int nCurUIMan = g_pGame->GetGameRun()->GetUIManager()->GetCurrentUIMan();

		if(nCurUIMan == CECUIManager::UIMAN_LOGIN)
		{
			((CECLoginUIMan*)m_pAUIManager)->ChangeScene((CECLoginUIMan::LOGIN_SCENE)(CECLoginUIMan::LOGIN_SCENE_FACECUSTOMIZE1 +  m_pCurPlayer->GetProfession()));
		}
		else if( nCurUIMan == CECUIManager::UIMAN_INGAME)
		{
//			assert(0);
		}
	
		m_pBtmCurChoose->SetPushed(false);
		m_pBtmCurChoose = m_pBtmChooseEar;
		m_pBtmCurChoose->SetPushed(true);
		return true;
		
	}
	else if( 0 == strcmpi(pszCommand, "ChooseHair") )
	{
		//关闭对话框
		pDlg->Show(false);
		POINT DlgPos = pDlg->GetPos();
		PAUIDIALOG pOtherDlg = m_pAUIManager->GetDialog("Win_ChooseHair");
		
		// old : pOtherDlg->SetPos(DlgPos.x, DlgPos.y);
		pOtherDlg->SetPosEx(DlgPos.x, DlgPos.y);
		
		pOtherDlg->Show(true);
		pOtherDlg->SetCanMove(false);

		int nCurUIMan = g_pGame->GetGameRun()->GetUIManager()->GetCurrentUIMan();

		if(nCurUIMan == CECUIManager::UIMAN_LOGIN)
		{
			((CECLoginUIMan*)m_pAUIManager)->ChangeScene((CECLoginUIMan::LOGIN_SCENE)(CECLoginUIMan::LOGIN_SCENE_FACECUSTOMIZE1 +  m_pCurPlayer->GetProfession()));
		}
		else if( nCurUIMan == CECUIManager::UIMAN_INGAME)
		{
	//		assert(0);
		}

		m_pBtmCurChoose->SetPushed(false);
		m_pBtmCurChoose = m_pBtmChooseHair;
		m_pBtmCurChoose->SetPushed(true);
		return true;
		
	}
	else if( 0 == strcmpi(pszCommand, "ChooseFacePainting") )
	{
		//关闭对话框
		pDlg->Show(false);
		POINT DlgPos = pDlg->GetPos();
		PAUIDIALOG pOtherDlg = m_pAUIManager->GetDialog("Win_ChooseFacePainting");

		// old : pOtherDlg->SetPos(DlgPos.x, DlgPos.y);
		pOtherDlg->SetPosEx(DlgPos.x, DlgPos.y);
		
		pOtherDlg->Show(true);
		pOtherDlg->SetCanMove(false);

		DrawColorPos();//显示当前颜色的位置

		int nCurUIMan = g_pGame->GetGameRun()->GetUIManager()->GetCurrentUIMan();

		if(nCurUIMan == CECUIManager::UIMAN_LOGIN)
		{
			((CECLoginUIMan*)m_pAUIManager)->ChangeScene((CECLoginUIMan::LOGIN_SCENE)(CECLoginUIMan::LOGIN_SCENE_FACECUSTOMIZE1 +  m_pCurPlayer->GetProfession()));
		}
		else if( nCurUIMan == CECUIManager::UIMAN_INGAME)
		{
	//		assert(0);
		}

		m_pBtmCurChoose->SetPushed(false);
		m_pBtmCurChoose = m_pBtmChooseFacePainting;
		m_pBtmCurChoose->SetPushed(true);
		return true;
		
	}

	//打开高级选项对话框
	else if( 0 == strcmpi(pszCommand, "AdvancedOption"))
	{
		//关闭对话框
		pDlg->Show(false);
		POINT DlgPos = pDlg->GetPos();
		PAUIDIALOG pOtherDlg = m_pAUIManager->GetDialog("Win_AdvancedOption");

		// old : pOtherDlg->SetPos(DlgPos.x, DlgPos.y);
		pOtherDlg->SetPosEx(DlgPos.x, DlgPos.y);
		
		pOtherDlg->Show(true);
		pOtherDlg->SetCanMove(false);

		//------------------------------------
		//预先计算除了高级和3庭以外的数据
		//------------------------------------

		//初始化骨骼
		pBoneCtrler->RestoreBoneTM();
		
		CalculateEye();//计算眼型
		CalculateBrow();//计算眉型
		CalculateNose();//计算鼻型
		CalculateMouth();//计算嘴型
		CalculateEar();//计算耳型
		
		CalculateBlendFace ();//计算混合脸型
		CalculateFace();//计算脸盘
		
		pBoneCtrler->BackupBoneTM ();//备份之前的数据
			
		CalculateAdvaceOption();//计算高级选项
		Calculate3Parts();//计算3庭
		
		int nCurUIMan = g_pGame->GetGameRun()->GetUIManager()->GetCurrentUIMan();

		if(nCurUIMan == CECUIManager::UIMAN_LOGIN)
		{
			((CECLoginUIMan*)m_pAUIManager)->ChangeScene((CECLoginUIMan::LOGIN_SCENE)(CECLoginUIMan::LOGIN_SCENE_FACECUSTOMIZE1 +  m_pCurPlayer->GetProfession()));
		}
		else if( nCurUIMan == CECUIManager::UIMAN_INGAME)
		{
	//		assert(0);
		}
		
		return true;
	}

	else if( 0 == strcmpi(pszCommand, "ChooseStature") )
	{
		//关闭对话框
		pDlg->Show(false);
		POINT DlgPos = pDlg->GetPos();
		PAUIDIALOG pOtherDlg = m_pAUIManager->GetDialog("Win_ChooseStature");

		// old : pOtherDlg->SetPos(DlgPos.x, DlgPos.y);
		pOtherDlg->SetPosEx(DlgPos.x, DlgPos.y);
		
		pOtherDlg->Show(true);
		pOtherDlg->SetCanMove(false);

		int nCurUIMan = g_pGame->GetGameRun()->GetUIManager()->GetCurrentUIMan();

		if(nCurUIMan == CECUIManager::UIMAN_LOGIN)
		{
			((CECLoginUIMan*)m_pAUIManager)->ChangeScene((CECLoginUIMan::LOGIN_SCENE)(CECLoginUIMan::LOGIN_SCENE_BODYCUSTOMIZE1 +  m_pCurPlayer->GetProfession()));
		}
		else if( nCurUIMan == CECUIManager::UIMAN_INGAME)
		{
	//		assert(0);
		}

		m_pBtmCurChoose->SetPushed(false);
		m_pBtmCurChoose = m_pBtmChooseStature;
		m_pBtmCurChoose->SetPushed(true);
		return true;
	}
	else if( 0 == strcmpi(pszCommand, "ChoosePreCustomize"))
	{
		//关闭对话框
		pDlg->Show(false);
		POINT DlgPos = pDlg->GetPos();
		PAUIDIALOG pOtherDlg = m_pAUIManager->GetDialog("Win_ChoosePreCustomize");

		// old : pOtherDlg->SetPos(DlgPos.x, DlgPos.y);
		pOtherDlg->SetPosEx(DlgPos.x, DlgPos.y);
		
		pOtherDlg->Show(true);
		pOtherDlg->SetCanMove(false);
		
		//设置当前删除个性化按钮是否可用
		int nCurSel = m_pListBoxPreCustomize->GetCurSel();
		if( nCurSel >= 0 && nCurSel < m_pListBoxPreCustomize->GetCount())
		{
			if( nCurSel >= m_pListBoxPreCustomize->GetCount() - m_nPreCustomsizeCount)
			{
				m_pBtnDeleteCustomizeData->Enable(false);
			}
			else
			{
				m_pBtnDeleteCustomizeData->Enable(true);
			}
		}

		int nCurUIMan = g_pGame->GetGameRun()->GetUIManager()->GetCurrentUIMan();

		if(nCurUIMan == CECUIManager::UIMAN_LOGIN)
		{
			((CECLoginUIMan*)m_pAUIManager)->ChangeScene((CECLoginUIMan::LOGIN_SCENE)(CECLoginUIMan::LOGIN_SCENE_BODYCUSTOMIZE1 + m_pCurPlayer->GetProfession()));
		}
		else if( nCurUIMan == CECUIManager::UIMAN_INGAME)
		{
			//assert(0);
		}

		m_pBtmCurChoose->SetPushed(false);
		m_pBtmCurChoose = m_pBtmChoosePreCustomize;
		m_pBtmCurChoose->SetPushed(true);
		return true;
	}
	return false;
}

//高级选项对话框
bool CECCustomize::OnCommand_AdvancedOption(const char *pszCommand, PAUIDIALOG pDlg)
{
	if( 0 == strcmpi(pszCommand, "ResetForehead") )
	{
		ResetForehead();
		return true;
	}

	else if( 0 == strcmpi(pszCommand, "ResetYokeBone") )
	{
		ResetYokeBone();
		return true;
	}
	
	else if( 0 == strcmpi(pszCommand, "ResetCheek") )
	{
		ResetCheek();
		return true;
	}
	else if( 0 == strcmpi(pszCommand, "ResetChain") )
	{
		ResetChain();
		return true;
	}

	else if( 0 == strcmpi(pszCommand, "ResetJaw") )
	{
		ResetJaw();
		return true;
	}

	else if( 0 == strcmpi( pszCommand, "MoveSlider"))
	{
		static DWORD nLastTime = 0;

		DWORD nThisTime =  (DWORD)(ACounter::GetMicroSecondNow()/1000);
		if(  nThisTime - nLastTime > 30 )
		{
			//更新滚动条控件
			UpdateAdvancedOptionSlider();
			nLastTime = nThisTime;
		}

		return true;
	}
	

	return true;
}

//体形对话框
bool CECCustomize::OnCommand_WinChooseStature(const char *pszCommand, PAUIDIALOG pDlg)
{
	//重置
	if( 0 == strcmpi(pszCommand,"ResetHeadScale"))
	{
		m_pSliderHeadScale->SetLevel(m_nDefaultHeadScale);
		UpdateStature();
		return true;
	}

	else if( 0 == strcmpi(pszCommand,"ResetUpScale"))
	{
		m_pSliderUpScale->SetLevel(m_nDefaultUpScale);
		UpdateStature();
		true;
	}
	
	else if( 0 == strcmpi(pszCommand,"ResetWaistScale"))
	{
		m_pSliderWaistScale->SetLevel(m_nDefaultWaistScale);
		UpdateStature();
		return true;
	}

	else if( 0 == strcmpi(pszCommand,"ResetArmWidth"))
	{
		m_pSliderArmWidth->SetLevel(m_nDefaultArmWidth);
		UpdateStature();

		return true;
	}
	
	else if( 0 == strcmpi(pszCommand,"ResetLegWidth"))
	{
		m_pSliderLegWidth->SetLevel(m_nDefaultLegWidth);
		UpdateStature();

		return true;
	}

	else if( 0 == strcmpi(pszCommand,"ResetBreastScale"))
	{
		m_pSliderBreastScale->SetLevel(m_nDefaultBreastScale);
		UpdateStature();

		return true;
	}

	//拖动滚动条
	else if(0 == strcmpi(pszCommand,"MoveSlider"))
	{
		static DWORD nLastTime = 0;

		DWORD nThisTime =  (DWORD)(ACounter::GetMicroSecondNow()/1000);
		if(  nThisTime - nLastTime > 30 )
		{
			//更新选择体形滚动条
			UpdateStature();
			nLastTime = nThisTime;

			return true;
		}
		
	}
	return true;
}


//预先自定义对话框
bool CECCustomize::OnCommand_WinChoosePreCustomize(const char* pszCommand, PAUIDIALOG pDlg)
{

	if( 0 == strcmpi(pszCommand, "SaveCustomizeData"))
	{
		static DWORD nLastTime = 0;

		DWORD nThisTime = (DWORD)(ACounter::GetMicroSecondNow()/1000);
		if(  nThisTime - nLastTime > 1000)
		{
			// set the customize data to current player
//			CECPlayer::PLAYER_CUSTOMIZEDATA data = m_aModel[m_nCurProfession][m_nCurGender]->GetCustomizeData();
//			data.faceData = *m_pCurFace->GetFaceData();
//			m_aModel[m_nCurProfession][m_nCurGender]->ChangeCustomizeData(data);
			
			//保存当前用户自定义数据
			SaveUserCustomizeData();
			nLastTime = nThisTime;
		}
	}
	//删除自定义文件
	else if( 0 == strcmpi(pszCommand, "DeleteCustomizeData"))
	{
		int nCurSel = m_pListBoxPreCustomize->GetCurSel();
		
		//判断是否为用户自定义文件(缺省文件不允许删除)

//		if( nCurSel >= m_nPreCustomsizeCount && nCurSel < m_pListBoxPreCustomize->GetCount())
//		{

		if( nCurSel >= 0 && nCurSel < m_pListBoxPreCustomize->GetCount() - m_nPreCustomsizeCount)
		{

			char* pFileName = (char*)m_pListBoxPreCustomize->GetItemDataPtr(nCurSel);

			//设置工作路径
			char szWorkDir[MAX_PATH];
			GetModuleFileNameA(NULL, szWorkDir, MAX_PATH);
			char* pTemp = strrchr(szWorkDir, '\\');
			*(pTemp+1) = '\0';

			char szFile[256];
			strcpy(szFile, szWorkDir);
			strcat(szFile, pFileName);

			ACHAR szFilePath[256];//宽字符文件路径

			int nLength = strlen(szFile);
			int nWLength = MultiByteToWideChar(CP_ACP, 0, szFile, nLength, szFilePath,nLength);
			szFilePath[nWLength]= _AL('\0');

			//删除当前文件
			if( szFilePath)
			{
				DeleteFile(szFilePath);
			}

			//删除当前listbox选项
			m_pListBoxPreCustomize->DeleteString(nCurSel);
			
			//设置当前listbox选中
			if(nCurSel < m_pListBoxPreCustomize->GetCount())
				m_pListBoxPreCustomize->SetCurSel(nCurSel);
			else
				m_pListBoxPreCustomize->SetCurSel(m_pListBoxPreCustomize->GetCount() -1);
		}

	}

	//用于是否显示用户参考图片窗口
	else if( 0 == strcmpi(pszCommand, "OnBtnUserPic"))
	{
		bool bShow = m_pWinUserPic->IsShow();
		
		m_pWinUserPic->Show(!bShow);
		m_pBtnUserPic->SetPushed(!bShow);
	}
	

	//*********************用于测试表情***********************
	
	else if( 0 == strcmpi(pszCommand,"TestChooseExpression"))
	{

		
		pDlg->Show(false);
		m_pWinChooseExpression->Show(true);
		m_pWinChooseExpression->SetCanMove(false);
		
		m_pWinExpressionList->Show(true);
		m_pWinExpressionList->SetCanMove(false);
		
		int nCurUIMan = g_pGame->GetGameRun()->GetUIManager()->GetCurrentUIMan();

		if(nCurUIMan == CECUIManager::UIMAN_LOGIN)
		{
			((CECLoginUIMan*)m_pAUIManager)->ChangeScene((CECLoginUIMan::LOGIN_SCENE)(CECLoginUIMan::LOGIN_SCENE_FACECUSTOMIZE1 + ((CECLoginUIMan*)m_pAUIManager)->GetCurPlayer()->GetProfession()));
		}
		else if( nCurUIMan == CECUIManager::UIMAN_INGAME)
		{
		//	assert(0);
		}
		
		//初始化融合表情列表
		m_nCustomizeBlendExpressionID = 0;
	}

	return true;
}

//用户参考对话框响应消息
bool CECCustomize::OnCommand_WinUserPic(const char *pszCommand, PAUIDIALOG pDlg)
{
	if(0 == strcmpi(pszCommand,"MoveSlider"))
	{
		int nAlpha = m_pSliderUserPicAlpha->GetLevel();
		m_pPicUserPic->SetAlpha(nAlpha);
	}

	if( 0 == strcmpi(pszCommand, "ZoomInUserPic"))
	{
		ZoomInUserPic();
	}

	if( 0 == strcmpi(pszCommand, "ZoomOutUserPic"))
	{
		ZoomOutUserPic();
	}
	

	return true;
}

//化妆对话框
bool CECCustomize::OnCommand_WinChooseFacePainting(const char *pszCommand, PAUIDIALOG pDlg)
{
	//复位脸型颜色
	if( 0 == strcmpi(pszCommand,"Reset"))
	{
		ResetFaceAllColor();
		return true;
	}

	return true;
}

