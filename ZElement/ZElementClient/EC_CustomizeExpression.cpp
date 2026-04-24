/*
 * FILE: EC_CustomizeExpression.cpp
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

bool CECCustomize::InitExpressionCtrler(void)
{
	//表情融合窗口
	m_pWinChooseExpression = m_pAUIManager->GetDialog("Win_ChooseExpression");
	
	m_pListBoxExp = (AUIListBox*)m_pWinChooseExpression->GetDlgItem("ListBox_Expression");
	m_pListBoxExp->SetViewMode(AUILISTBOX_VIEW_ICON);
	
	int i ;
	for(i = 0 ; i < EBE_NUMBER; i++)
	{
		char szTemp[256];
		sprintf(szTemp, "Pic_BlendExp%d", i+1);
		m_pPicBlendExp[i] = (AUIImagePicture*)m_pWinChooseExpression->GetDlgItem(szTemp);

	}
	
	

	//获取表情动画参数滚动条 和 标签
	char* pszAniParam[EAP_NUMBER];
	
	pszAniParam[EAP_APPEARTIME] =  "AppearTime";
	pszAniParam[EAP_KEEPTIME] = "KeepTime";
	pszAniParam[EAP_DISAPPEARTIME] = "DisappearTime";
	pszAniParam[EAP_RESTTIME] = "RestTime";
	pszAniParam[EAP_REPEAT] = "Repeat";
	
	char szTempSliderName[256];
	char szTempLableName[256];
	
	
	for( i = 0; i < EAP_NUMBER; i++)
	{
		sprintf(szTempSliderName, "Slider_BlendExp%s", pszAniParam[i]);
		m_pSliderBlendExp[i] = (AUISlider*)m_pWinChooseExpression->GetDlgItem(szTempSliderName);

		sprintf(szTempLableName, "Label_BlendExp%s",  pszAniParam[i]);
		m_pLabelBlendExp[i] = (AUILabel*)m_pWinChooseExpression->GetDlgItem(szTempLableName);
	}
	

	//表情列表窗口
	m_pWinExpressionList = m_pAUIManager->GetDialog("Win_ExpressionList");
	m_pListBoxExpressionList = (AUIListBox*)m_pWinExpressionList->GetDlgItem("ListBox_ExpressionList");
	

	//初始化表情列表播放状态
	m_Status = ES_STOP;

	m_nPassedTime = 0;
	m_nCurBlendExpTime = 0;
	m_nPlayBlendExpListCount = 0;

	return true;
}


bool CECCustomize::OnCommand_WinExpressionList(const char* pszCommand, PAUIDIALOG pDlg)
{
	//返回
	if(0 == strcmpi(pszCommand, "CloseExpressionDlg"))
	{
		m_pPicBlendExp[EBE_NUM1]->ClearCover();
		m_pPicBlendExp[EBE_NUM2]->ClearCover();
		m_pPicBlendExp[EBE_NUM3]->ClearCover();

		pDlg->Show(false);
		m_pWinChooseExpression->Show(false);

		m_pWinChoosePreCustomize->Show(true);
		m_pWinChoosePreCustomize->SetCanMove(false);

		//释放
		//表情列表窗口
		
		m_pListBoxExpressionList->ResetContent();

		//释放
		for( int i = 0; i < EBE_NUMBER; i++)
		{
			m_pPicBlendExp[i]->SetDataPtr(NULL);
		}

		//释放表情列表
		ReleaseExpList();
	}

	//播放表情列表
	else if( 0 == strcmpi(pszCommand, "PlayBlendExpList"))
	{
		PlayExpList();
	}

	//停止播放表情列表
	else if( 0 == strcmpi(pszCommand, "StopBlendExpList"))
	{
		StopExpList();
	}
	
	//删除当前混合表情
	else if( 0 == strcmpi(pszCommand, "DeleteCurBlendExp"))
	{
		StopExpList();
		int nIndex = m_pListBoxExpressionList->GetCurSel();
		if( nIndex >= 0 && nIndex < m_pListBoxExpressionList->GetCount())
		{
			DeleteCurBlendExp(nIndex);
			m_pListBoxExpressionList->DeleteString(nIndex);
		}
	}

	return true;
}

bool CECCustomize::ReleaseExpressionCtrler(void)
{
	int i;
	if(m_pListBoxExp)
		m_pListBoxExp ->ResetContent();


	//释放表情文件名向量
	abase::vector<char*>::iterator iterFileName;
	abase::vector<char*>::iterator iterFileNameEnd;
	iterFileName = m_vecExpFileName.begin();
	iterFileNameEnd = m_vecExpFileName.end();
	for( ; iterFileName != iterFileNameEnd; ++iterFileName)
	{
		if( *iterFileName != NULL)
		{
			delete[] (*iterFileName);
		}
	}
	m_vecExpFileName.clear();

	for( i = 0; i < EBE_NUMBER; i++)
	{
		if(m_pPicBlendExp[i])
		{
			if( m_pPicBlendExp[i]->GetDataPtr() != NULL)
			{
				m_pPicBlendExp[i]->ClearCover();
			}
		}
	}
	
	
	//表情列表窗口
	if(m_pListBoxExpressionList)
		m_pListBoxExpressionList->ResetContent();


	//释放表情列表
	ReleaseExpList();
	
	return true;

}

bool CECCustomize::LoadCustomizeExpression(void)
{
	char szFile[256];//图标文件
	char* pszExpFileName;//表情文件

	ACHAR szString[256];//listbox插入字符串
	int i = 0;

	A2DSprite* pA2DIcon;

	//载入表情
	for(i = 0; i < EXP_NUMBER; i++)
	{
		//图表文件名
		sprintf(szFile, "Expression\\Exp%d.tga",i+1);

		pszExpFileName = new char[256];

		sprintf(pszExpFileName, "ExpressionData\\Exp%d.eface",i+1);

		m_vecExpFileName.push_back(pszExpFileName);
		
		swprintf(szString,_AL("Exp%d"), i+1);
			
		//变图标文件为A2DSprite文件
		pA2DIcon = new A2DSprite; //需要释放
		A3DDevice* pDevice = g_pGame->GetA3DDevice();
		if( !pA2DIcon->Init ( g_pGame->GetA3DDevice(), szFile, 0))
		{
			delete[] pA2DIcon;

			#ifdef _DEBUG
			_asm int 3
			#endif

			return false;
		}

		//把A2DSprite*对象插入Vector
		m_vecA2DSprite.push_back(pA2DIcon);
			
		m_pListBoxExp->InsertString(i, szString);
		m_pListBoxExp->SetItemImage(i, pA2DIcon, 0);
		m_pListBoxExp->SetItemDataPtr(i, pszExpFileName);
	}

		
	//设置表情
	//获取骨骼控制器
	CFaceBoneController* pBoneCtrler = m_pCurFace->GetBoneController();
	m_Animation.SetFaceBoneController(pBoneCtrler);
	
	return true;
}

bool CECCustomize::OnCommand_WinChooseExpression(const char *pszCommand, PAUIDIALOG pDlg)
{	
	//设置表情
	if(0 == strcmpi(pszCommand, "SetExpression"))
	{
		
		int nCurSel = m_pListBoxExp->GetCurSel();
		
		if( nCurSel >= 0 && nCurSel< m_pListBoxExp->GetCount())
		{	
			int nItem;
			A2DSprite* pA2DIcon;
			m_pListBoxExp->GetItemImage(nCurSel,&pA2DIcon,&nItem);

			char* pszExpFile = (char*)m_pListBoxExp->GetItemDataPtr(nCurSel);

			for( int i = 0; i < EBE_NUMBER; i++)
			{
				if( m_pPicBlendExp[i]->GetDataPtr() == NULL)
				{
					m_pPicBlendExp[i]->SetCover(pA2DIcon, 0);
					m_pPicBlendExp[i]->SetDataPtr(pszExpFile);

					return true;;
				}
			}
		}
	}


	//清除混合表情1
	else if(0 == strcmpi(pszCommand, "ClearBlendExp1"))
	{
		m_pPicBlendExp[EBE_NUM1]->ClearCover();
		m_pPicBlendExp[EBE_NUM1]->SetDataPtr(NULL);
	}

	//清除混合表情2
	else if( 0 == strcmpi(pszCommand, "ClearBlendExp2"))
	{
		m_pPicBlendExp[EBE_NUM2]->ClearCover();
		m_pPicBlendExp[EBE_NUM2]->SetDataPtr(NULL);
	}
	
	//清除混合表情3
	else if( 0 == strcmpi(pszCommand,"ClearBlendExp3"))
	{
		m_pPicBlendExp[EBE_NUM3]->ClearCover();
		m_pPicBlendExp[EBE_NUM3]->SetDataPtr(NULL);
	}

	
	//播放表情
	else if( 0 == strcmpi(pszCommand,"PlayExpression"))
	{
		
		int nCurSel = m_pListBoxExp->GetCurSel();
		if( nCurSel >= 0 && nCurSel < m_pListBoxExp->GetCount())
		{
			char* pszExpFileName = (char*)m_pListBoxExp->GetItemDataPtr(nCurSel);
		
			m_Expression.LoadExpression(pszExpFileName);

			m_AniParam.nAppearTime = 70;
			m_AniParam.nKeepTime = 70;
			m_AniParam.nDisapperTime = 70;
			m_AniParam.nRestTime = 70;
			m_AniParam.nRepeat = 10;

			float fWeight = 1.0f;
			m_Animation.Init(1, &m_Expression, &fWeight);
			m_Animation.Play(	m_AniParam.nAppearTime,\
									m_AniParam.nKeepTime,\
									m_AniParam.nDisapperTime,\
									m_AniParam.nRestTime, \
									m_AniParam.nRepeat);
			return true;
		}
	}
	
	//播放混合表情
	else if(0 == strcmpi(pszCommand,"PlayBlendExp") )
	{
		float fWeight[EBE_NUMBER];

		int nExpNum = 0;//混合表情数量
		CExpression expBlend[EBE_NUMBER];//混合表情
		int i;
		
		for( i = 0; i < EBE_NUMBER; i++)
		{
			fWeight[i] = 1.0f;
		}

		for( i = 0; i < EBE_NUMBER; i++)
		{
			if( m_pPicBlendExp[i]->GetDataPtr()!=NULL)
			{
				char* pszExpFileName = (char*)m_pPicBlendExp[i]->GetDataPtr();
				expBlend[nExpNum].LoadExpression(pszExpFileName);
				nExpNum++;
			}		
		}

		//没有混合表情
		if( nExpNum == 0)
			return true;

		m_Animation.Init(nExpNum, expBlend, fWeight);
		
		int nFactor = 5;
		
		m_AniParam.nAppearTime = m_pSliderBlendExp[EAP_APPEARTIME]->GetLevel()* nFactor;
		m_AniParam.nKeepTime = m_pSliderBlendExp[EAP_KEEPTIME]->GetLevel()* nFactor;
		m_AniParam.nDisapperTime = m_pSliderBlendExp[EAP_DISAPPEARTIME]->GetLevel()* nFactor;
		m_AniParam.nRestTime = m_pSliderBlendExp[EAP_RESTTIME]->GetLevel()* nFactor + 4;
		m_AniParam.nRepeat = m_pSliderBlendExp[EAP_REPEAT]->GetLevel() + 1;
		
		m_Animation.Play(	m_AniParam.nAppearTime,\
							m_AniParam.nKeepTime,\
							m_AniParam.nDisapperTime,\
							m_AniParam.nRestTime, \
							m_AniParam.nRepeat);
	}
	
	//停止混合表情播放
	else if ( 0 == strcmpi(pszCommand, "StopBlendExp"))
	{
		
		m_Animation.Stop();
	
	}

	//将当前混合表情加入到表情列表中
	else if( 0 == strcmpi(pszCommand, "PushBackCurBlendExpression"))
	{	
		//如果3个表情都为null,则返回
		if(	!(m_pPicBlendExp[EBE_NUM1]->GetDataPtr()) && \
			!(m_pPicBlendExp[EBE_NUM2]->GetDataPtr()) && \
			!(m_pPicBlendExp[EBE_NUM3]->GetDataPtr()))

			return true;

		m_nCustomizeBlendExpressionID++ ;

		ACHAR szString[256];
		swprintf(szString,_AL("自定义混合表情%d"), m_nCustomizeBlendExpressionID);

		int nIndex = m_pListBoxExpressionList->GetCount();
		m_pListBoxExpressionList->InsertString(nIndex, szString);

		int nFactor = 5;

		//填充混合表情数据
		S_BLEND_ANIMATION_PARAM* pSBAP = new S_BLEND_ANIMATION_PARAM;
		for( int i = 0; i < EBE_NUMBER; i++)
		{
			if( !(m_pPicBlendExp[i]->GetDataPtr()))
			{
				pSBAP->pszBlendExpFile[i] = NULL;
			}
			else
			{

				pSBAP->pszBlendExpFile[i] = (char*)m_pPicBlendExp[i]->GetDataPtr();
			}
		}
		pSBAP->nAppearTime = m_pSliderBlendExp[EAP_APPEARTIME]->GetLevel()* nFactor;
		pSBAP->nKeepTime = m_pSliderBlendExp[EAP_KEEPTIME]->GetLevel()* nFactor;
		pSBAP->nDisapperTime = m_pSliderBlendExp[EAP_DISAPPEARTIME]->GetLevel()* nFactor;
		pSBAP->nRestTime = m_pSliderBlendExp[EAP_RESTTIME]->GetLevel()* nFactor + 4;
		pSBAP->nRepeat = m_pSliderBlendExp[EAP_REPEAT]->GetLevel() + 1;

			
		

		//把混合表情数据插入自定义表情队列
 		InsertCurBlendExp(pSBAP);

	}

	
	else if(0 == strcmpi(pszCommand, "MoveSlider"))
	{
		int nFactor = 5;
		ACHAR szNum[10];
		
		for( int i = 0; i < EAP_NUMBER; i++)
		{
			if( i == EAP_RESTTIME)
			{
				swprintf(szNum, _AL("%d"), m_pSliderBlendExp[i]->GetLevel() * nFactor + 4);
				m_pLabelBlendExp[i]->SetText(szNum);	
			}
			else if(i == EAP_REPEAT)
			{
				swprintf(szNum, _AL("%d"), m_pSliderBlendExp[i]->GetLevel() + 1);
				m_pLabelBlendExp[i]->SetText(szNum);	
			}
			else
			{
				swprintf(szNum, _AL("%d"), m_pSliderBlendExp[i]->GetLevel() * nFactor);
				m_pLabelBlendExp[i]->SetText(szNum);	
			}
		}

		return true;
	}
	
	return true;
}

void CECCustomize::CustomizeExpressionTick()
{
	DWORD dwTickTime = g_pGame->GetTickTime();

	if( m_Animation.HasInited())
		m_Animation.Tick(dwTickTime);




	//用于控制自定义表情列表播放
	switch ( m_Status)
	{

	case ES_STOP:
		break;

	case ES_PLAY:
		if( m_nPassedTime >= m_nCurBlendExpTime)
		{
			//播放表情列表(播放播放下一个混合表情)
			PlayExpList();
		}
		else 
		{
			m_nPassedTime += dwTickTime;
		}

		break;
	}

	
}

//以后要写成一个播放自定义表情表情列表的类
void CECCustomize::PlayExpList()
{
	if(m_CustomizeExpList.size() == 0 )
		return ;

	int i;

	//计算当前混合表情所需时间
	if( m_nPlayBlendExpListCount <= (m_CustomizeExpList.size() - 1))
	{
		//当前播放时间
		m_nPassedTime = 0;

		m_Status = ES_PLAY;

		abase::vector<S_BLEND_ANIMATION_PARAM*>::iterator iter;
		iter = m_CustomizeExpList.begin();
		iter += m_nPlayBlendExpListCount;

		//计算当前混合表情所需时间
		CalculateCurBlendExpTime(*iter);
		float fWeight[3];

		for( i = 0; i <EBE_NUMBER; i++)
		{
			fWeight[i] = 1.0f;
		}

		
		CExpression expBlend[EBE_NUMBER];//混合表情
		int nExpNum = 0;//混合表情数量
		for( i = 0; i < EBE_NUMBER; i++)
		{
			if( (*iter)->pszBlendExpFile[i] != NULL)
			{
				expBlend[nExpNum].LoadExpression((*iter)->pszBlendExpFile[i]);
				nExpNum ++;
			}
		}

		m_Animation.Init(nExpNum,expBlend,fWeight);
		
		m_Animation.Play((*iter)->nAppearTime, (*iter)->nKeepTime,\
						(*iter)->nDisapperTime, (*iter)->nRestTime,\
						(*iter)->nRepeat);
		
		
		m_nPlayBlendExpListCount ++;
	}
	else
	{
		m_nPlayBlendExpListCount = 0;

		//当前播放时间
		m_nPassedTime = 0;

		m_Status = ES_STOP;
	}	
}

void CECCustomize::StopExpList()
{

	m_Status = ES_STOP;
	
	m_nCurBlendExpTime = 0;

	m_nPlayBlendExpListCount = 0;

	//当前播放时间
	m_nPassedTime = 0;
	
	m_Animation.Stop();

}

void CECCustomize::InsertCurBlendExp(S_BLEND_ANIMATION_PARAM* blendExp)
{
	m_CustomizeExpList.push_back(blendExp);
}

void CECCustomize::DeleteCurBlendExp(unsigned int nIndex)
{
	if(m_CustomizeExpList.size() == 0 )
		return ;

	if( nIndex >= 0 && nIndex < m_CustomizeExpList.size())
	{
		//删除当前自定义表情
 		abase::vector<S_BLEND_ANIMATION_PARAM*>::iterator iter;
		iter = m_CustomizeExpList.begin();
		iter += nIndex;

		if( *iter != NULL)
		{	
			delete (*iter);		
			
		}

		m_CustomizeExpList.erase(iter);
	}

}


void CECCustomize::ReleaseExpList()
{
	int nExpListCount = m_CustomizeExpList.size();

	//释放表情列表向量
 	abase::vector<S_BLEND_ANIMATION_PARAM*>::iterator iter;
	abase::vector<S_BLEND_ANIMATION_PARAM*>::iterator iterEnd;
	
	iter = m_CustomizeExpList.begin();
	iterEnd = m_CustomizeExpList.end();
	for( ;iter!=iterEnd; ++iter)
	{
		if( *iter != NULL)
		{
			delete (*iter);
		}

	}

	m_CustomizeExpList.clear();
}


//计算当前混合表情所需时间
void CECCustomize::CalculateCurBlendExpTime(S_BLEND_ANIMATION_PARAM* pSBAP)
{	
	m_nCurBlendExpTime = 0;	
	m_nCurBlendExpTime = (pSBAP->nAppearTime + \
						pSBAP->nKeepTime + \
						pSBAP->nDisapperTime + \
						pSBAP->nRestTime)* pSBAP->nRepeat;		
}