/*
 * FILE: EC_CustomizeEvent.cpp
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
#include "EC_UIManager.h"
#include "EC_LoginUIMan.h"
#include "EC_GameUIMan.h"
#include "A3DViewport.h"

//响应事件
bool CECCustomize::OnEvent_Customize( UINT uMsg, WPARAM wParam, LPARAM lParam, AUIDialog* pDlg, AUIObject* pObj)
{

	
	//使用选择脸型窗口响应事件
	if( 0 == strcmpi(pDlg->GetName(), "Win_ChooseFace"))
		return OnEvent_ChooseFace( uMsg, wParam, lParam, pDlg, pObj);

	if( 0 == strcmpi(pDlg->GetName(), "Win_ChooseFacePainting"))
		return OnEvent_WinChooseFacePainting( uMsg, wParam, lParam, pDlg, pObj);

	if( 0 == strcmpi(pDlg->GetName(), "Win_ChooseHair"))
		return OnEvent_WinChooseHair(uMsg, wParam, lParam, pDlg, pObj);

	if( 0 == strcmpi(pDlg->GetName(), "Win_ChooseEar"))
		return OnEvent_WinChooseEar(uMsg, wParam, lParam, pDlg, pObj);

	if( 0 == strcmpi(pDlg->GetName(), "Win_ChooseMouth"))

		return OnEvent_WinChooseMouth(uMsg, wParam, lParam, pDlg, pObj);

	if( 0 == strcmpi(pDlg->GetName(), "Win_ChooseNose"))
		return OnEvent_WinChooseNose(uMsg, wParam, lParam, pDlg, pObj);

	if( 0 == strcmpi(pDlg->GetName(), "Win_ChooseEyeBrow"))
		return OnEvent_WinChooseBrow(uMsg, wParam, lParam, pDlg, pObj);

	if( 0 == strcmpi(pDlg->GetName(), "Win_ChooseEye"))
		return OnEvent_WinChooseEye(uMsg, wParam, lParam, pDlg, pObj);
	
	if( 0 == strcmpi(pDlg->GetName(), "Win_ChoosePreCustomize"))
		return OnEvent_WinChoosePreCustomize(uMsg, wParam,lParam, pDlg, pObj);

	if( 0 == strcmpi(pDlg->GetName(), "Win_UserPic"))
		return OnEvent_WinUserPic(uMsg, wParam,lParam, pDlg, pObj);

	
	
	return m_pAUIManager->AUIManager::OnEvent(uMsg, wParam, lParam, pDlg, pObj);
}

//选择脸型窗口响应事件
bool CECCustomize::OnEvent_ChooseFace(UINT uMsg, WPARAM wParam, LPARAM lParam, AUIDialog* pDlg, AUIObject* pObj)
{
	CECFace::FACE_CUSTOMIZEDATA* pFaceData= m_pCurFace->GetFaceData();
			
	if(uMsg == WM_LBUTTONUP || uMsg == WM_KEYDOWN)
	{
		if( !pObj)
			return true;

		if( uMsg == WM_KEYDOWN)
		{
			if( wParam != VK_UP && wParam != VK_DOWN)
			{
				return true;
			}
				
		}

		if( strstr(pObj->GetName(), "E_") )
		{
			CFaceBoneController* pBoneCtrler = m_pCurFace->GetBoneController();
			
			if(pBoneCtrler->GetReady())
			{
				//当前图标不存在
				if( !m_pImagePicCurFaceShape1 || !m_pImagePicCurFaceShape2)
					return true;

				m_pImagePicCurFaceShape1->SetColor(m_clrUnSel);
				m_pImagePicCurFaceShape2->SetColor(m_clrUnSel);

				pObj->SetColor(m_clrSel);
				m_pImagePicCurFaceShape1 = (AUIImagePicture*)pObj;
				m_pImagePicCurFaceShape2 = (AUIImagePicture*)pObj;

				//设置脸型数据
				pFaceData->idFaceShape1 = (unsigned short)pObj->GetData();
				pFaceData->idFaceShape2 = (unsigned short)pObj->GetData();
			
//				m_pPicLeft->SetData(pObj->GetData());
//				m_pPicRight->SetData(pObj->GetData());
				
				A2DSprite* pA2DSprite;
				int nItem;
				m_pImagePicCurFaceShape1->GetCover(&pA2DSprite, &nItem);

//				m_pPicLeft->SetCover(pA2DSprite, nItem);
//				m_pPicRight->SetCover(pA2DSprite, nItem);

				UpdateChooseFaceSlider();
			}
			return true;
		}

		//点选法令纹理list box
		if( 0 == strcmpi(pObj->GetName(), "ListBox_FalingTex"))
		{
			if( uMsg == WM_LBUTTONUP)
			{
				POINT ptPos = pObj->GetPos();

				A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
				int nMouseX = GET_X_LPARAM(lParam) - ptPos.x - p->X;
				int nMouseY = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;
				
				if(m_pListBoxFalingTex->GetHitArea(nMouseX, nMouseY)!= AUILISTBOX_RECT_TEXT)
					return true;		
			}

			int nCurSel = m_pListBoxFalingTex->GetCurSel();

			if( nCurSel >= 0 && nCurSel < m_pListBoxFalingTex->GetCount())
			{				
				
				pFaceData->idFalingTex = (unsigned short)m_pListBoxFalingTex->GetItemData(nCurSel);

				m_pCurFace->UpdateFalingTexture();

				return true;
			}
		}
		
	}
 	return true;
}


//化妆对话框响应事件
bool CECCustomize::OnEvent_WinChooseFacePainting(UINT uMsg, WPARAM wParam, LPARAM lParam, AUIDialog* pDlg, AUIObject* pObj)
{
	//**获取FaceData指针
	CECFace::FACE_CUSTOMIZEDATA* pFaceData = m_pCurFace->GetFaceData();
	
	POINT posColor;
	
	if(uMsg == WM_LBUTTONUP || uMsg == WM_KEYDOWN)
	{
		if( !pObj)
			return true;
		
		if( uMsg == WM_KEYDOWN)
		{
			if( wParam != VK_UP && wParam != VK_DOWN)
			{
				return true;
			}
				
		}
		
		const char* pszObjName = pObj->GetName();
		unsigned  int id = pObj->GetData ();
		
		//点选眼影list box
		if( 0 == strcmpi(pObj->GetName(), "ListBox_EyeKohl"))
		{
			if( uMsg == WM_LBUTTONUP)
			{
				POINT ptPos = pObj->GetPos();

				A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
				int nMouseX = GET_X_LPARAM(lParam) - ptPos.x - p->X;
				int nMouseY = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;
				
				if(m_pListBoxEyeKohl->GetHitArea(nMouseX, nMouseY)!= AUILISTBOX_RECT_TEXT)
					return true;		
			}

			int nCurSel = m_pListBoxEyeKohl->GetCurSel();
			if( nCurSel >= 0 && nCurSel < m_pListBoxEyeKohl->GetCount())
			{				
			
				//**设置当前的眼影
				pFaceData->idEyeHighTex = (unsigned short)m_pListBoxEyeKohl->GetItemData(nCurSel);
				
				//**更新当前眼影
				m_pCurFace->UpdateEyeTexture();
			
			
			}
		}

		//点中的是脸型列表框
		else if( 0 == strcmpi(pszObjName, "ListBox_FaceParts") )
		{
			if( uMsg == WM_LBUTTONUP)
			{
				POINT ptPos = pObj->GetPos();

				A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
				int nMouseX = GET_X_LPARAM(lParam) - ptPos.x - p->X;
				int nMouseY = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;

				if(m_pListBoxFaceParts->GetHitArea(nMouseX, nMouseY)!= AUILISTBOX_RECT_TEXT)
					return true;		
			}

			int nCurSeleted = m_pListBoxFaceParts->GetCurSel ();
			if( nCurSeleted >= 0 && nCurSeleted <m_pListBoxFaceParts->GetCount () )
			{	
				A3DCOLOR clrCur;
				switch( m_pListBoxFaceParts->GetItemData( nCurSeleted,1))
				{
				case 0://脸	
					clrCur = pFaceData->colorFace;
					m_pImagePictureCurColor->SetColor(clrCur);

					break;

				case 1://眼影
					clrCur = pFaceData->colorEye ;					
					m_pImagePictureCurColor->SetColor(clrCur);

					

					break;

				case 2://眉毛
					clrCur = pFaceData->colorBrow ;
				
					m_pImagePictureCurColor->SetColor(clrCur);

					
					break;

				case 3://嘴唇
					clrCur = pFaceData->colorMouth;
					
					m_pImagePictureCurColor->SetColor(clrCur);

					
					break;

				case 4://头发
					clrCur = pFaceData->colorHair;
					
					m_pImagePictureCurColor->SetColor(clrCur);

					
					break;

				case 5://眼球(瞳孔)
					clrCur = pFaceData->colorEyeBall;
					m_pImagePictureCurColor->SetColor(clrCur);

				

					break;

				case 6://胡子
					clrCur = pFaceData->colorMoustache;
					m_pImagePictureCurColor->SetColor(clrCur);

					
					break;

				default:
					break;

				}
				
				DrawColorPos();//画当前颜色位置

				A2DSprite* p2DSprite = (A2DSprite*)m_pListBoxFaceParts->GetItemDataPtr ( nCurSeleted);
				m_pImagePictureColorPic->SetCover (p2DSprite,0);

				//获取元素数据管理器
				elementdataman* pElementDataMan;
				pElementDataMan= g_pGame->GetElementDataMan ();
				int id = m_pListBoxFaceParts->GetItemData ( nCurSeleted);
				DATA_TYPE dtFaceData = DT_COLORPICKER_ESSENCE;
				COLORPICKER_ESSENCE* pColorPickerEssence = (COLORPICKER_ESSENCE*)pElementDataMan->get_data_ptr(id, ID_SPACE_FACE,dtFaceData);
				char* pszPicFileName = pColorPickerEssence->file_colorpicker;
				pszPicFileName +=9; //去掉"surface\\"
				A3DSurfaceMan* pA3DSurfaceMan = g_pGame->GetA3DEngine ()->GetA3DSurfaceMan ();				

				if( !pA3DSurfaceMan->LoadCursorSurfaceFromFile ( pszPicFileName, 0, &m_pA3DSurfaceColorPic))
				{
					return false;
				}
				m_ColorPicFMT = m_pA3DSurfaceColorPic->GetFormat ();
			}
		}
	}
	
	if (uMsg == WM_MOUSEMOVE || uMsg == WM_LBUTTONDOWN )//当鼠标移动
	{
		if( !(wParam & MK_LBUTTON))//并且左键按下时
			return true;
		
		if( !pObj)					//且在控件上
			return true;
		
		const char* pszObjName = pObj->GetName();
		unsigned  int id = pObj->GetData ();

			
		if( 0 == strcmpi(pszObjName, "ImagePicture_ColorPic")) //在当前颜色图片控件上显示颜色
		{
			POINT MousePos ;

//			MousePos.x = LOWORD(lParam);
//			MousePos.y = HIWORD(lParam); 

			A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
			MousePos.x = GET_X_LPARAM(lParam)  - p->X;
			MousePos.y = GET_Y_LPARAM(lParam)  - p->Y;

			POINT ColorPicPos = m_pImagePictureColorPic->GetPos();

			RECT rect;
			rect.left = MousePos.x - ColorPicPos.x;
			rect.top = MousePos.y - ColorPicPos.y;

			//去掉图片边框
			if( rect.left  == 0)
				rect.left = 1;
			if( rect.top == 0)
				rect.top = 1;
			
			if( !m_pA3DSurfaceColorPic)
				return true;
			
			if( rect.left >= m_pA3DSurfaceColorPic->GetWidth () -1)
				rect.left = m_pA3DSurfaceColorPic->GetWidth () -2;
			if( rect.top >= m_pA3DSurfaceColorPic->GetHeight() -1)
				rect.top = m_pA3DSurfaceColorPic->GetHeight() -2;
	
			//
			rect.right = rect.left + 1;
			rect.bottom = rect.top + 1;

			
			//当前位置
			posColor.x = rect.left -2;
			posColor.y = rect.top - 2;
					
			if(A3DFMT_A8R8G8B8 == m_ColorPicFMT)
			{
				int* pData = NULL;

				int nPicPitch;

				if(!m_pA3DSurfaceColorPic->LockRect ( &rect, (void**)&pData, &nPicPitch, 0))
					return false;
				
				m_pA3DSurfaceColorPic->UnlockRect();
				A3DCOLOR clrCur = (A3DCOLOR)(*pData);
				m_pImagePictureCurColor->SetColor(clrCur);

				int nCurSeleted = m_pListBoxFaceParts->GetCurSel ();
				switch( m_pListBoxFaceParts->GetItemData( nCurSeleted,1))
				{
				case 0://脸
					pFaceData->colorFace = clrCur;
					
					m_pCurPlayer->SetBodyColor(clrCur);
					m_pCurFace->UpdateFaceColor();

					m_FaceColorPos = posColor;

					break;

				case 1://眼影
					pFaceData->colorEye = clrCur;
					
					m_pCurFace->UpdateEyeColor();

					m_EyeColorPos = posColor;

					break;

				case 2://眉毛
					pFaceData->colorBrow = clrCur;
				
					m_pCurFace->UpdateBrowColor();

					m_BrowColorPos = posColor;

					break;

				case 3://嘴唇
					pFaceData->colorMouth = clrCur;
					
					m_pCurFace->UpdateMouthColor();

					m_MouthColorPos = posColor;

					break;

				case 4://头发
					pFaceData->colorHair = clrCur;
					
					m_pCurFace->UpdateHairColor();

					m_HairColorPos = posColor;

					break;

				case 5://眼球(瞳孔)
					pFaceData->colorEyeBall = clrCur;
					m_pCurFace->UpdateEyeBallColor();

					m_EyeBallColorPos = posColor;

					break;

				case 6://胡子
					pFaceData->colorMoustache = clrCur;
					m_pCurFace->UpdateGoateeColor();

					m_MouthColorPos = posColor;
					
					break;

				default:
					break;

				}
			}
			int nColorPicWidth = m_pA3DSurfaceColorPic->GetWidth ();
			int nColorPicHeight = m_pA3DSurfaceColorPic->GetHeight();
			int nColorPosWidth = m_pPicColorPos->GetSize().cx;
			int nColorPosHeight = m_pPicColorPos->GetSize().cy;
			
			//限制一下posColor的范围
			if( posColor.x < 0)
				posColor.x = 0;
			if( posColor.x > nColorPicWidth - nColorPosWidth)
				posColor.x = nColorPicWidth - nColorPosWidth;
			if( posColor.y < 0)
				posColor.y = 0;
			if( posColor.y > nColorPicHeight - nColorPosHeight)
				posColor.y = nColorPicHeight - nColorPosHeight;
			
			//画当前颜色位置
			POINT ptDlg = pDlg->GetPos();
			m_pPicColorPos->SetPos(ColorPicPos.x + posColor.x - ptDlg.x,
				ColorPicPos.y + posColor.y - ptDlg.y);
			pDlg->ChangeFocus(m_pPicColorPos);
		}
	}	


	return true;
}

//毛发选择对话框相应消息
bool CECCustomize::OnEvent_WinChooseHair(UINT uMsg, WPARAM wParam, LPARAM lParam, AUIDialog* pDlg, AUIObject* pObj)
{
	//**获取FaceData指针
	CECFace::FACE_CUSTOMIZEDATA* pFaceData = m_pCurFace->GetFaceData();

	if(uMsg == WM_LBUTTONUP || uMsg == WM_KEYDOWN)
	{
		if( !pObj)
			return true;
		
		if( uMsg == WM_KEYDOWN)
		{
			if( wParam != VK_UP && wParam != VK_DOWN)
			{
				return true;
			}
				
		}
		
		//点选大胡子模型
		if( 0 == strcmpi(pObj->GetName(), "ListBox_Goatee"))
		{
			if( uMsg == WM_LBUTTONUP)
			{
				POINT ptPos = pObj->GetPos();

				A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
				int nMouseX = GET_X_LPARAM(lParam) - ptPos.x - p->X;
				int nMouseY = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;
				
				if(m_pListBoxGoatee->GetHitArea(nMouseX, nMouseY)!= AUILISTBOX_RECT_TEXT)
					return true;		
			}

			int nCurSel = m_pListBoxGoatee->GetCurSel();
			if( nCurSel >= 0 && nCurSel < m_pListBoxGoatee->GetCount())
			{
				//**设置当前的胡子模型
				pFaceData->idMoustacheSkin = (unsigned short)m_pListBoxGoatee->GetItemData(nCurSel);
			
				
				//**更新当前胡子模型
				m_pCurFace->UpdateGoateeModel();
			}
		}

		//点选小胡子纹理
		else if( 0 == strcmpi(pObj->GetName(), "ListBox_Moustache"))
		{
			if( uMsg == WM_LBUTTONUP)
			{
				POINT ptPos = pObj->GetPos();

				A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
				int nMouseX = GET_X_LPARAM(lParam) - ptPos.x - p->X;
				int nMouseY = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;
				
				if(m_pListBoxMoustache->GetHitArea(nMouseX, nMouseY)!= AUILISTBOX_RECT_TEXT)
					return true;		
			}

			int nCurSel = m_pListBoxMoustache->GetCurSel();
			if( nCurSel >= 0 && nCurSel < m_pListBoxMoustache->GetCount())
			{
				//**设置当前胡子纹理
				pFaceData->idMoustacheTex = (unsigned short)m_pListBoxMoustache->GetItemData(nCurSel);


				//**跟新当前胡子纹理
				m_pCurFace->UpdateMoustacheTexture();
				
			}
		}

		//点选头发模型
		else if( 0 == strcmpi(pObj->GetName(), "ListBox_Hair"))
		{
			if( uMsg == WM_LBUTTONUP)
			{
				POINT ptPos = pObj->GetPos();

				A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
				int nMouseX = GET_X_LPARAM(lParam) - ptPos.x - p->X;
				int nMouseY = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;

				if(m_pListBoxHair->GetHitArea(nMouseX, nMouseY)!= AUILISTBOX_RECT_TEXT)
					return true;		
			}

			int nCurSel = m_pListBoxHair->GetCurSel();
			if( nCurSel >= 0 && nCurSel < m_pListBoxHair->GetCount())
			{
		
				//**设置当前胡子纹理
				pFaceData->idHairModel = (unsigned short)m_pListBoxHair->GetItemData(nCurSel);

				//**更新当前头发模型
				m_pCurFace->UpdateHairModel();
				
			}
		}

		//点选是头发纹理
		else if( 0 == strcmpi(pObj->GetName(), "ListBox_HairTex"))
		{
			if( uMsg == WM_LBUTTONUP)
			{
				POINT ptPos = pObj->GetPos();
				A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
				int nMouseX = GET_X_LPARAM(lParam) - ptPos.x - p->X;
				int nMouseY = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;

				if(m_pListBoxHairTex->GetHitArea(nMouseX, nMouseY)!= AUILISTBOX_RECT_TEXT)
					return true;		
			}

			int nCurSel = m_pListBoxHairTex->GetCurSel();
			if( nCurSel >= 0 && nCurSel < m_pListBoxHairTex->GetCount())
			{
				
				//**设置当前头发纹理
				pFaceData->idHairTex = (unsigned short)m_pListBoxHairTex->GetItemData(nCurSel);

				//**更新当前头发模型
				m_pCurFace->UpdateHairTexture();
			}
		}

		//点选是大胡子纹理
		else if( 0 == strcmpi(pObj->GetName(), "ListBox_GoateeTex"))
		{
			if( uMsg == WM_LBUTTONUP)
			{
				POINT ptPos = pObj->GetPos();
				A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
				int nMouseX = GET_X_LPARAM(lParam) - ptPos.x - p->X;
				int nMouseY = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;

				if(m_pListBoxGoateeTex->GetHitArea(nMouseX, nMouseY)!= AUILISTBOX_RECT_TEXT)
					return true;		
			}

			int nCurSel = m_pListBoxGoateeTex->GetCurSel();
			if( nCurSel >= 0 && nCurSel < m_pListBoxGoateeTex->GetCount())
			{
				
				
				//**设置当前头发纹理
				pFaceData->idGoateeTex = (unsigned short)m_pListBoxGoateeTex->GetItemData(nCurSel);

				//**更新当前头发模型
				m_pCurFace->UpdateGoateeTexture();
			}
		}
	}
	return true;
}

//耳型选择对话框响应事件
bool CECCustomize::OnEvent_WinChooseEar(UINT uMsg, WPARAM wParam, LPARAM lParam, AUIDialog* pDlg, AUIObject* pObj)
{
	//**获取FaceData指针
	CECFace::FACE_CUSTOMIZEDATA* pFaceData = m_pCurFace->GetFaceData();

	if(uMsg == WM_LBUTTONUP || uMsg == WM_KEYDOWN)
	{
		if( !pObj)
			return true;
		
		if( uMsg == WM_KEYDOWN)
		{
			if( wParam != VK_UP && wParam != VK_DOWN)
			{
				return true;
			}
				
		}

		//点选耳型list box
		if( 0 == strcmpi(pObj->GetName(), "ListBox_Ear"))
		{
			if( uMsg == WM_LBUTTONUP)
			{
				POINT ptPos = pObj->GetPos();

				A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
				int nMouseX = GET_X_LPARAM(lParam) - ptPos.x - p->X;
				int nMouseY = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;

				if(m_pListBoxEar->GetHitArea(nMouseX, nMouseY)!= AUILISTBOX_RECT_TEXT)
					return true;		
			}

			int nCurSel = m_pListBoxEar->GetCurSel();

			if( nCurSel >= 0 && nCurSel < m_pListBoxEar->GetCount())
			{

				pFaceData->idEarShape = (unsigned short)m_pListBoxEar->GetItemData(nCurSel);

				ResetEarToNormal();//重置耳朵
			}
		}

	}
	
	return true;
}

//嘴型选择对话框项响应事件
bool CECCustomize::OnEvent_WinChooseMouth(UINT uMsg, WPARAM wParam, LPARAM lParam, AUIDialog* pDlg, AUIObject* pObj)
{
	//**获取FaceData指针
	CECFace::FACE_CUSTOMIZEDATA* pFaceData = m_pCurFace->GetFaceData();

	if(uMsg == WM_LBUTTONUP || uMsg == WM_KEYDOWN)
	{
		if( !pObj)
			return true;
		
		if( uMsg == WM_KEYDOWN)
		{
			if( wParam != VK_UP && wParam != VK_DOWN)
			{
				return true;
			}
				
		}

		//点选上唇list box
		if( 0 == strcmpi(pObj->GetName(), "ListBox_UpLipLine"))
		{
			if( uMsg == WM_LBUTTONUP)
			{
				POINT ptPos = pObj->GetPos();

				A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
				int nMouseX = GET_X_LPARAM(lParam) - ptPos.x - p->X;
				int nMouseY = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;

				if(m_pListBoxUpLipLine->GetHitArea(nMouseX, nMouseY)!= AUILISTBOX_RECT_TEXT)
					return true;		
			}

			int nCurSel = m_pListBoxUpLipLine->GetCurSel();

			if( nCurSel >= 0 && nCurSel < m_pListBoxUpLipLine->GetCount())
			{

				pFaceData->idMouthUpLipLine = (unsigned short)m_pListBoxUpLipLine->GetItemData(nCurSel);

				ResetUpLip();//重置上唇
			}
		}
		
		//点选唇沟list box
		else if( 0 == strcmpi(pObj->GetName(), "ListBox_MidLipLine"))
		{
			if( uMsg == WM_LBUTTONUP)
			{
				POINT ptPos = pObj->GetPos();

				A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
				int nMouseX = GET_X_LPARAM(lParam) - ptPos.x - p->X;
				int nMouseY = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;

				if(m_pListBoxMidLipLine->GetHitArea(nMouseX, nMouseY)!= AUILISTBOX_RECT_TEXT)
					return true;		
			}

			int nCurSel = m_pListBoxMidLipLine->GetCurSel();
			if( nCurSel >= 0 && nCurSel < m_pListBoxMidLipLine->GetCount())
			{
				pFaceData->idMouthMidLipLine = (unsigned short)m_pListBoxMidLipLine->GetItemData(nCurSel);

				ResetMidLip();//重置唇沟
				
			}
		}

		//点选下唇线
		else if( 0 == strcmpi(pObj->GetName(), "ListBox_DownLipLine"))
		{
			if( uMsg == WM_LBUTTONUP)
			{
				POINT ptPos = pObj->GetPos();

				A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
				int nMouseX = GET_X_LPARAM(lParam) - ptPos.x - p->X;
				int nMouseY = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;

				if(m_pListBoxDownLipLine->GetHitArea(nMouseX, nMouseY)!= AUILISTBOX_RECT_TEXT)
					return true;		
			}

			int nCurSel = m_pListBoxDownLipLine->GetCurSel();
			if( nCurSel >= 0 && nCurSel < m_pListBoxDownLipLine->GetCount())
			{
				pFaceData->idMouthDownLipLine = (unsigned short)m_pListBoxDownLipLine->GetItemData(nCurSel);

				ResetDownLip();//重置下唇线
			}
		}

	}	
	return true;
}

//鼻子选择对话框响应事件
bool CECCustomize::OnEvent_WinChooseNose(UINT uMsg, WPARAM wParam, LPARAM lParam, AUIDialog* pDlg, AUIObject* pObj)
{
	//**获取FaceData指针
	CECFace::FACE_CUSTOMIZEDATA* pFaceData = m_pCurFace->GetFaceData();

	if(uMsg == WM_LBUTTONUP || uMsg == WM_KEYDOWN)
	{
		if( !pObj)
			return true;
		
		if( uMsg == WM_KEYDOWN)
		{
			if( wParam != VK_UP && wParam != VK_DOWN)
			{
				return true;
			}
				
		}
		
		//点选鼻子纹理list box
		if( 0 == strcmpi(pObj->GetName(), "ListBox_NoseTex"))
		{
			if( uMsg == WM_LBUTTONUP)
			{
				POINT ptPos = pObj->GetPos();

				A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
				int nMouseX = GET_X_LPARAM(lParam) - ptPos.x - p->X;
				int nMouseY = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;

				if(m_pListBoxNoseTex->GetHitArea(nMouseX, nMouseY)!= AUILISTBOX_RECT_TEXT)
					return true;		
			}

			int nCurSel = m_pListBoxNoseTex->GetCurSel();

			if( nCurSel >= 0 && nCurSel < m_pListBoxNoseTex->GetCount())
			{
								
				pFaceData->idNoseTex = (unsigned short)m_pListBoxNoseTex->GetItemData(nCurSel);

				//更新鼻子纹理
				m_pCurFace->UpdateNoseTexture();
			}
		}
		//如果选择的是鼻头形状list box
		else if(0 == strcmpi(pObj->GetName(), "ListBox_NoseTip"))
		{
			if( uMsg == WM_LBUTTONUP)
			{
				POINT ptPos = pObj->GetPos();

				A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
				int nMouseX = GET_X_LPARAM(lParam) - ptPos.x - p->X;
				int nMouseY = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;
				
				if(m_pListBoxNoseTip->GetHitArea(nMouseX, nMouseY)!= AUILISTBOX_RECT_TEXT)
					return true;		
			}

			int nCurSel = m_pListBoxNoseTip->GetCurSel();
			if( nCurSel >= 0 && nCurSel < m_pListBoxNoseTip->GetCount())
			{
			
				pFaceData->idNoseTipShape = (unsigned short)m_pListBoxNoseTip->GetItemData(nCurSel);

				//重置鼻头
				ResetNoseTip();
			}
		}
		//如果选择的是鼻梁形状list box
		else if( 0 == strcmpi(pObj->GetName(), "ListBox_NoseBridge"))
		{
			if( uMsg == WM_LBUTTONUP)
			{
				POINT ptPos = pObj->GetPos();

				A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
				int nMouseX = GET_X_LPARAM(lParam) - ptPos.x - p->X;
				int nMouseY = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;

				if(m_pListBoxNoseBridge->GetHitArea(nMouseX, nMouseY)!= AUILISTBOX_RECT_TEXT)
					return true;		
			}

			int nCurSel = m_pListBoxNoseBridge->GetCurSel();
			if( nCurSel >= 0 && nCurSel < m_pListBoxNoseTip->GetCount())
			{
				pFaceData->idNoseBridgeShape = (unsigned short)m_pListBoxNoseBridge->GetItemData(nCurSel);

				//重置鼻梁
				ResetNoseBridge();
			}
		}
	}
	return true;
}

//眉毛选择对话框响应事件
bool CECCustomize::OnEvent_WinChooseBrow(UINT uMsg, WPARAM wParam, LPARAM lParam, AUIDialog* pDlg, AUIObject* pObj)
{
	//**获取FaceData指针
	CECFace::FACE_CUSTOMIZEDATA* pFaceData = m_pCurFace->GetFaceData();

	if(uMsg == WM_LBUTTONUP || uMsg == WM_KEYDOWN)
	{
		if( !pObj)
			return true;
		
		if( uMsg == WM_KEYDOWN)
		{
			if( wParam != VK_UP && wParam != VK_DOWN)
			{
				return true;
			}
				
		}

		//点选眉毛纹理list box
		if( 0 == strcmpi(pObj->GetName(), "ListBox_BrowTex"))
		{
			if( uMsg == WM_LBUTTONUP)
			{
				POINT ptPos = pObj->GetPos();

				A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
				int nMouseX = GET_X_LPARAM(lParam) - ptPos.x - p->X;
				int nMouseY = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;

				if(m_pListBoxBrowTex->GetHitArea(nMouseX, nMouseY)!= AUILISTBOX_RECT_TEXT)
					return true;		
			}

			int nCurSel = m_pListBoxBrowTex->GetCurSel();

			if( nCurSel >= 0 && nCurSel < m_pListBoxBrowTex->GetCount())
			{				
				
				pFaceData->idBrowTex = (unsigned short)m_pListBoxBrowTex->GetItemData(nCurSel);

				//更新眉毛纹理
				m_pCurFace->UpdateBrowTexture();
				return true;
			}
		}
		//点选眉毛形状list box
		else if( 0 == strcmpi(pObj->GetName(), "ListBox_Brow"))
		{
			if( uMsg == WM_LBUTTONUP)
			{
				POINT ptPos = pObj->GetPos();

				A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
				int nMouseX = GET_X_LPARAM(lParam) - ptPos.x - p->X;
				int nMouseY = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;

				if(m_pListBoxBrow->GetHitArea(nMouseX, nMouseY)!= AUILISTBOX_RECT_TEXT)
					return true;		
			}

			int nCurSel = m_pListBoxBrow->GetCurSel();
			if( nCurSel >= 0 && nCurSel < m_pListBoxBrow->GetCount())
			{
				pFaceData->idBrowShape = (unsigned short)m_pListBoxBrow->GetItemData(nCurSel);

				//重置眉毛
				ResetBrowToNormal();
				return true;
			}
		}
	}
	
	return true;
}

//眼睛选择对话框响应事件
bool CECCustomize::OnEvent_WinChooseEye(UINT uMsg, WPARAM wParam, LPARAM lParam, AUIDialog* pDlg, AUIObject* pObj)
{
	//**获取FaceData指针
	CECFace::FACE_CUSTOMIZEDATA* pFaceData = m_pCurFace->GetFaceData();

	if(uMsg == WM_LBUTTONUP || uMsg == WM_KEYDOWN)
	{
		if( !pObj)
			return true;
		
		if( uMsg == WM_KEYDOWN)
		{
			if( wParam != VK_UP && wParam != VK_DOWN)
			{
				return true;
			}
				
		}

		//点选眼皮纹理list box1
		if( 0 == strcmpi(pObj->GetName(), "ListBox_EyeBaseTex1"))
		{
			if( uMsg == WM_LBUTTONUP)
			{
				POINT ptPos = pObj->GetPos();

				A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
				int nMouseX = GET_X_LPARAM(lParam) - ptPos.x - p->X;
				int nMouseY = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;

				if(m_pListBoxEyeBaseTex1->GetHitArea(nMouseX, nMouseY)!= AUILISTBOX_RECT_TEXT)
					return true;		
			}

			int nCurSel = m_pListBoxEyeBaseTex1->GetCurSel();
			
			if( nCurSel >= 0 && nCurSel < m_pListBoxEyeBaseTex1->GetCount())
			{
				
				pFaceData->idEyeBaseTex = (unsigned short)m_pListBoxEyeBaseTex1->GetItemData(nCurSel);

				//更新眼皮纹理
				m_pCurFace->UpdateEyeTexture();
				return true;
			}
		}

		//点选眼皮纹理list box2
		else if( 0 == strcmpi(pObj->GetName(), "ListBox_EyeBaseTex2"))
		{
			if( uMsg == WM_LBUTTONUP)
			{
				POINT ptPos = pObj->GetPos();

				A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
				int nMouseX = GET_X_LPARAM(lParam) - ptPos.x - p->X;
				int nMouseY = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;

				if(m_pListBoxEyeBaseTex2->GetHitArea(nMouseX, nMouseY)!= AUILISTBOX_RECT_TEXT)
					return true;		
			}

			int nCurSel = m_pListBoxEyeBaseTex2->GetCurSel();
			
			if( nCurSel >= 0 && nCurSel < m_pListBoxEyeBaseTex2->GetCount())
			{
				
				pFaceData->idEyeBaseTex = (unsigned short)m_pListBoxEyeBaseTex2->GetItemData(nCurSel);

				//更新眼皮纹理
				m_pCurFace->UpdateEyeTexture();
				return true;
			}
		}

		//点选眼皮纹理list box3
		else if( 0 == strcmpi(pObj->GetName(), "ListBox_EyeBaseTex3"))
		{
			if( uMsg == WM_LBUTTONUP)
			{
				POINT ptPos = pObj->GetPos();

				A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
				int nMouseX = GET_X_LPARAM(lParam) - ptPos.x - p->X;
				int nMouseY = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;

				if(m_pListBoxEyeBaseTex3->GetHitArea(nMouseX, nMouseY)!= AUILISTBOX_RECT_TEXT)
					return true;		
			}

			int nCurSel = m_pListBoxEyeBaseTex3->GetCurSel();
			
			if( nCurSel >= 0 && nCurSel < m_pListBoxEyeBaseTex3->GetCount())
			{
				
				pFaceData->idEyeBaseTex = (unsigned short)m_pListBoxEyeBaseTex3->GetItemData(nCurSel);

				//更新眼皮纹理
				m_pCurFace->UpdateEyeTexture();
				return true;
			}
		}

		//点选的是第一类眼型
		else if ( 0 == strcmpi(pObj->GetName(), "ListBox_EyeShape1"))
		{
			if( uMsg == WM_LBUTTONUP)
			{
				POINT ptPos = pObj->GetPos();

				A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
				int nMouseX = GET_X_LPARAM(lParam) - ptPos.x - p->X;
				int nMouseY = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;

				if(m_pListBoxEyeShape1->GetHitArea(nMouseX, nMouseY)!= AUILISTBOX_RECT_TEXT)
					return true;		
			}

			int nCurSel = m_pListBoxEyeShape1->GetCurSel();

			if( nCurSel >= 0 && nCurSel < m_pListBoxEyeShape1->GetCount())
			{

				pFaceData->idEyeShape = (unsigned short)m_pListBoxEyeShape1->GetItemData(nCurSel);

				//重置眼睛
				ResetEyeToNormal();
				return true;
			}
		}

		//点选的是第二类眼型
		else if ( 0 == strcmpi(pObj->GetName(), "ListBox_EyeShape2"))
		{
			if( uMsg == WM_LBUTTONUP)
			{
				POINT ptPos = pObj->GetPos();

				A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
				int nMouseX = GET_X_LPARAM(lParam) - ptPos.x - p->X;
				int nMouseY = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;
				
				if(m_pListBoxEyeShape2->GetHitArea(nMouseX, nMouseY)!= AUILISTBOX_RECT_TEXT)
					return true;		
			}
			
			int nCurSel = m_pListBoxEyeShape2->GetCurSel();

			if( nCurSel >= 0 && nCurSel < m_pListBoxEyeShape2->GetCount())
			{
				
				pFaceData->idEyeShape = (unsigned short)m_pListBoxEyeShape2->GetItemData(nCurSel);

				//重置眼睛
				ResetEyeToNormal();
				return true;
			}
		}

		//点选的是第三类眼型
		else if ( 0 == strcmpi(pObj->GetName(), "ListBox_EyeShape3"))
		{
			if( uMsg == WM_LBUTTONUP)
			{
				POINT ptPos = pObj->GetPos();

				A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
				int nMouseX = GET_X_LPARAM(lParam) - ptPos.x - p->X;
				int nMouseY = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;
				
				if(m_pListBoxEyeShape3->GetHitArea(nMouseX, nMouseY)!= AUILISTBOX_RECT_TEXT)
					return true;		
			}

			int nCurSel = m_pListBoxEyeShape3->GetCurSel();

			if( nCurSel >= 0 && nCurSel < m_pListBoxEyeShape3->GetCount())
			{
			
				pFaceData->idEyeShape = (unsigned short)m_pListBoxEyeShape3->GetItemData(nCurSel);

				//重置眼睛
				ResetEyeToNormal();
				return true;
			}
		}
	}

	return true;	
}

//选择预设对话框响应事件
bool CECCustomize::OnEvent_WinChoosePreCustomize(UINT uMsg, WPARAM wParam, LPARAM lParam, AUIDialog* pDlg, AUIObject* pObj)
{
	if(uMsg == WM_LBUTTONUP || uMsg == WM_KEYDOWN)
	{
		if( !pObj)
			return true;
		
		if( uMsg == WM_KEYDOWN)
		{
			if( wParam != VK_UP && wParam != VK_DOWN)
			{
				return true;
			}
				
		}
	

		//点选预设list box
		if( 0 == strcmpi(pObj->GetName(), "ListBox_PreCustomize"))
		{
			if( uMsg == WM_LBUTTONUP)
			{
				POINT ptPos = pObj->GetPos();

				A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
				int nMouseX = GET_X_LPARAM(lParam) - ptPos.x - p->X;
				int nMouseY = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;

				if(m_pListBoxPreCustomize->GetHitArea(nMouseX, nMouseY)!= AUILISTBOX_RECT_TEXT)
					return true;		
			}
			
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

				char* pszFile = (char*)m_pListBoxPreCustomize->GetItemDataPtr(nCurSel);
				

				if(!LoadCustomizeData(pszFile))
				{
					int nCurUIMan = g_pGame->GetGameRun()->GetUIManager()->GetCurrentUIMan();

					if(nCurUIMan == CECUIManager::UIMAN_LOGIN)
					{
						((CECLoginUIMan*)m_pAUIManager)->MessageBox("错误", _AL("个性化数据有错"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
					}
					else if( nCurUIMan == CECUIManager::UIMAN_INGAME)
					{
						((CECGameUIMan*)m_pAUIManager)->MessageBox("错误", _AL("个性化数据有错"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
					}
				
				}
				
				SetFaceAllShape();
				SetFaceAllSlider();
				SetFaceAllTexture();
				SetFaceAllModel();
				SetFaceAllColor();
				SetAllColorPos();

				return true;
			}
		}
		
		//点选的是参考图片list box
		if (0 == strcmpi(pObj->GetName(), "ListBox_UserPic"))
		{
			if( uMsg == WM_LBUTTONUP)
			{
				POINT ptPos = pObj->GetPos();

				A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
				int nMouseX = GET_X_LPARAM(lParam) - ptPos.x - p->X;
				int nMouseY = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;

				if(m_pListBoxUserPic->GetHitArea(nMouseX, nMouseY)!= AUILISTBOX_RECT_TEXT)
					return true;		
			}
			
			int nCurSel = m_pListBoxUserPic->GetCurSel();
			if( nCurSel >= 0 && nCurSel < m_pListBoxUserPic->GetCount())
			{
				char* szUserPicFileName =(char*)m_pListBoxUserPic->GetItemDataPtr(nCurSel);

				
				
				if( m_pA2DSpriteUserPic != NULL)
				{
					m_pA2DSpriteUserPic->Release();
				}
				
			

				g_pGame->GetA3DEngine()->GetA3DSurfaceMan()->SetFolderName("userdata/userpic");
				if( m_pA2DSpriteUserPic->Init(g_pGame->GetA3DDevice(), szUserPicFileName, 0))
				{
					m_pA2DSpriteUserPic->SetLinearFilter(true);//设置线性过滤
					
				
					m_fA2DSpriteUserPicScale=m_fA2DSpriteUserPicOriScale = (float)m_nPicUserPicOriWidth / (float)m_pA2DSpriteUserPic->GetWidth();
					m_fPicUserPicScale = 1.0f;

					m_pA2DSpriteUserPic->SetScaleX(m_fA2DSpriteUserPicOriScale);
					m_pA2DSpriteUserPic->SetScaleY(m_fA2DSpriteUserPicOriScale);
					 
					m_nPicUserPicOriHeight =(int)(m_fA2DSpriteUserPicOriScale * m_pA2DSpriteUserPic->GetHeight()); 

					A3DRECT rectUserPic(0,0, m_pA2DSpriteUserPic->GetWidth(), m_pA2DSpriteUserPic->GetHeight());

					m_pA2DSpriteUserPic->ResetItems(1, &rectUserPic);

					m_nA2DSPriteUserPicItemPosX = 0;
					m_nA2DSPriteUserPicItemPosY =0;

					SIZE dialogFrameSize = m_pWinUserPic->GetFrame()->GetSizeLimit();
					
					//设置AUIImagePicture尺寸
					m_pPicUserPic->SetPos(dialogFrameSize.cx/2, dialogFrameSize.cy/2 + m_pLabelUserPic->GetSize().cy);
					
					m_pPicUserPic->SetSize(m_nPicUserPicOriWidth, m_nPicUserPicOriHeight);
					
					m_pPicUserPic->SetCover( m_pA2DSpriteUserPic, 0);

					//设置图片窗口大小
					m_pWinUserPic->SetSize(m_nPicUserPicOriWidth + dialogFrameSize.cx, m_nPicUserPicOriHeight + m_pBtnUserPicDrag->GetSize().cy + m_pLabelUserPic->GetSize().cy + dialogFrameSize.cy);

					//设置图片窗口不能大于游戏窗口
					HWND hWnd = GetActiveWindow();
					RECT rectClient;
					GetClientRect(hWnd, & rectClient);

//					A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
//					rectClient.left = p->X;
//					rectClient.top = p->Y;
//					rectClient.right = p->X + p->Width;
//					rectClient.bottom = p->Y + p->Height;
					
					if( m_pWinUserPic->GetPos().y + m_pWinUserPic->GetSize().cy > rectClient.bottom)
					{
						// old : m_pWinUserPic->SetPos(m_pWinUserPic->GetPos().x, rectClient.bottom - m_pWinUserPic->GetSize().cy);
						m_pWinUserPic->SetPosEx(m_pWinUserPic->GetPos().x, rectClient.bottom - m_pWinUserPic->GetSize().cy);
					}
					
					//设置4个按钮的位置
					m_pBtnUserPicDrag->SetPos(m_pWinUserPic->GetSize().cx - m_pBtnUserPicDrag->GetSize().cx - dialogFrameSize.cx/2, 
												m_pWinUserPic->GetSize().cy - m_pBtnUserPicDrag->GetSize().cy - dialogFrameSize.cy/2);

					m_pBtnUserPicZoomIn->SetPos(m_pWinUserPic->GetSize().cx	
												- m_pBtnUserPicDrag->GetSize().cx 
												- m_pBtnUserPicZoomIn->GetSize().cx
												- dialogFrameSize.cx/2,
												m_pWinUserPic->GetSize().cy 
												- m_pBtnUserPicZoomIn->GetSize().cy
												- dialogFrameSize.cy/2);

					m_pBtnUserPicZoomOut->SetPos(m_pWinUserPic->GetSize().cx 
												- m_pBtnUserPicDrag->GetSize().cx \
												- m_pBtnUserPicZoomIn->GetSize().cx \
												- m_pBtnUserPicZoomOut->GetSize().cx
												- dialogFrameSize.cx/2, 
												m_pWinUserPic->GetSize().cy 
												- m_pBtnUserPicZoomOut->GetSize().cy
												- dialogFrameSize.cy/2);



					//设置滚动条和标签的位置
					m_pLabelUserPicAlpha->SetPos(dialogFrameSize.cx/2, m_pWinUserPic->GetSize().cy - m_pLabelUserPicAlpha->GetSize().cy - dialogFrameSize.cy/2);
					m_pSliderUserPicAlpha->SetPos(50, m_pWinUserPic->GetSize().cy - m_pSliderUserPicAlpha->GetSize().cy - dialogFrameSize.cy/2);
					m_pLabelUserPic->SetPos((m_pWinUserPic->GetSize().cx - m_pLabelUserPic->GetSize().cx)/2, dialogFrameSize.cy/2);

				}
				g_pGame->GetA3DEngine()->GetA3DSurfaceMan()->SetFolderName("surfaces");
			}
		}
	}
	
	return true;
}


//用户参考对话框响应事件
bool CECCustomize::OnEvent_WinUserPic(UINT uMsg, WPARAM wParam, LPARAM lParam, AUIDialog* pDlg, AUIObject* pObj)
{
	if(pObj == NULL)
	{
		return true;
	}
	
	//用鼠标轮缩放图片大小
 	if( uMsg == WM_MOUSEWHEEL)
	{
		if( 0 == strcmpi(pObj->GetName(), "Pic_UserPic"))
		{
			if((short)HIWORD(wParam) > 0)
			{
				ZoomInUserPic();
			}
			else
			{
				ZoomOutUserPic();
			}
		}
	}

	if( uMsg == WM_LBUTTONDOWN)
	{
		if( !pObj)
			return true;

		if( 0 == strcmpi(pObj->GetName(), "Btn_UserPicDrag"))
		{
			pDlg->SetCaptureObject(pObj);
		}

		if( 0 == strcmpi(pObj->GetName(), "Pic_UserPic"))
		{
			//禁止对话框移动
//			pDlg->SetCanMove(false);

			//pDlg->SetCaptureObject(pObj);

			int nMouseX, nMouseY;
//			nMouseX = LOWORD(lParam);
//			nMouseY = HIWORD(lParam);
			
			A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
			nMouseX = GET_X_LPARAM(lParam)  - p->X;
			nMouseY = GET_Y_LPARAM(lParam)  - p->Y;


			//鼠标在ItemX上的位置(拖动窗口)
//			m_nMouseOnItemX = nMouseX - m_pWinUserPic->GetPos().x - m_pPicUserPic->GetPos().x;
//			m_nMouseOnItemY = nMouseY - m_pWinUserPic->GetPos().y - m_pPicUserPic->GetPos().y; 

			//(拖动图片)
			A3DRECT rectItem = m_pA2DSpriteUserPic->GetItem(0)->GetRect();
			//鼠标在图片上的位置
			m_nMouseOnA2DSpriteX = (int)((nMouseX - m_pWinUserPic->GetPos().x - m_pPicUserPic->GetPos().x)* (1.0f/m_fA2DSpriteUserPicScale)) + rectItem.left;
			m_nMouseOnA2DSpriteY = (int)((nMouseY - m_pWinUserPic->GetPos().y - m_pPicUserPic->GetPos().y)* (1.0f/m_fA2DSpriteUserPicScale))+ rectItem.top;

		}
	}

	//拖动按钮处于下压状态
	//缩放窗口
	if( m_pBtnUserPicDrag->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK)
	{
		
		if( uMsg == WM_MOUSEMOVE)
		{
			
			//左键按下
			if(wParam & MK_LBUTTON)
			{		
				int nMouseX, nMouseY;
//				nMouseX = LOWORD(lParam);
//				nMouseY = HIWORD(lParam);

				A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
				nMouseX = GET_X_LPARAM(lParam)  - p->X;
				nMouseY = GET_Y_LPARAM(lParam)  - p->Y;

				//获取客户区大小
				HWND hWnd = GetActiveWindow();
				RECT rectClient;
				GetClientRect(hWnd, & rectClient);

//				rectClient.left = p->X;
//				rectClient.top = p->Y;
//				rectClient.right = p->X + p->Width;
//				rectClient.bottom = p->Y + p->Height;

				SIZE dialogFrameSize = m_pWinUserPic->GetFrame()->GetSizeLimit();

				if( nMouseX < (m_pWinUserPic->GetPos().x +250 + dialogFrameSize.cx/2 - m_pBtnUserPicDrag->GetSize().cx / 2))
				{
					nMouseX = m_pWinUserPic->GetPos().x + 250 + dialogFrameSize.cx/2- m_pBtnUserPicDrag->GetSize().cx / 2;
				}

				//设置图片框在窗口左侧，不能覆盖右侧的个性化对话框
				if( nMouseX > (m_pWinChooseFace->GetPos().x - dialogFrameSize.cx/2 - m_pBtnUserPicDrag->GetSize().cx / 2))
				{
					nMouseX = m_pWinChooseFace->GetPos().x - dialogFrameSize.cx/2 - m_pBtnUserPicDrag->GetSize().cx / 2;
				}

				if( nMouseY < (m_pWinUserPic->GetPos().y + 100 + m_pLabelUserPic->GetSize().cy + dialogFrameSize.cy/2 + m_pBtnUserPicDrag->GetSize().cy / 2))
				{
					nMouseY = m_pWinUserPic->GetPos().y + 100 + m_pLabelUserPic->GetSize().cy + dialogFrameSize.cy/2 + m_pBtnUserPicDrag->GetSize().cy / 2;
				}
				if( nMouseY > ( rectClient.bottom - dialogFrameSize.cy/2 - m_pBtnUserPicDrag->GetSize().cy / 2))
				{
					nMouseY = rectClient.bottom - dialogFrameSize.cy/2 - m_pBtnUserPicDrag->GetSize().cy / 2;
				}
				

				int nWinWidth = nMouseX - m_pWinUserPic->GetPos().x + m_pBtnUserPicDrag->GetSize().cx/2 + dialogFrameSize.cx/2;
				int nWinHeight = nMouseY - m_pWinUserPic->GetPos().y + m_pBtnUserPicDrag->GetSize().cy/2 + dialogFrameSize.cy/2;
			
			

				//设置窗口尺寸
				m_pWinUserPic->SetSize(nWinWidth, nWinHeight);



			int nPicUserPicWidth = (int)(m_nPicUserPicOriWidth * m_fPicUserPicScale);
			int nPicUserPicHeight = (int)(m_nPicUserPicOriHeight * m_fPicUserPicScale);
			int nPicUserPicX,nPicUserPicY;

		
			if( nPicUserPicWidth > m_pWinUserPic->GetSize().cx - dialogFrameSize.cx)
			{
				nPicUserPicX = dialogFrameSize.cx/2;
				nPicUserPicWidth = m_pWinUserPic->GetSize().cx - dialogFrameSize.cx;
			}
			else
			{
				nPicUserPicX = (m_pWinUserPic->GetSize().cx - nPicUserPicWidth)/2;
				
				
			}
			
			if( nPicUserPicHeight > m_pWinUserPic->GetSize().cy -m_pLabelUserPic->GetSize().cy - dialogFrameSize.cy - m_pBtnUserPicDrag->GetSize().cy)
			{
				nPicUserPicY = m_pLabelUserPic->GetSize().cy + dialogFrameSize.cy/2;
				nPicUserPicHeight = m_pWinUserPic->GetSize().cy -m_pLabelUserPic->GetSize().cy - dialogFrameSize.cy - m_pBtnUserPicDrag->GetSize().cy;
			}
			else
			{
				nPicUserPicY = (m_pWinUserPic->GetSize().cy - m_pBtnUserPicDrag->GetSize().cy -m_pLabelUserPic->GetSize().cy - dialogFrameSize.cy - nPicUserPicHeight)/2 + m_pLabelUserPic->GetSize().cy + dialogFrameSize.cy/2;
			}

			//设置控件尺寸
			m_pPicUserPic->SetSize(nPicUserPicWidth, nPicUserPicHeight);

			//设置控件位置
			m_pPicUserPic->SetPos(nPicUserPicX, nPicUserPicY);

			//nItemWidth大小
			int nItemWidth = (int)(nPicUserPicWidth * (1.0f/m_fA2DSpriteUserPicScale));
			int nItemHeight = (int) (nPicUserPicHeight * (1.0f/m_fA2DSpriteUserPicScale));			

			

			if( m_nA2DSPriteUserPicItemPosX + nItemWidth > m_pA2DSpriteUserPic->GetWidth())
			{
				m_nA2DSPriteUserPicItemPosX = m_pA2DSpriteUserPic->GetWidth() - nItemWidth;
			}
			if( m_nA2DSPriteUserPicItemPosY + nItemHeight > m_pA2DSpriteUserPic->GetHeight())
			{
				m_nA2DSPriteUserPicItemPosY = m_pA2DSpriteUserPic->GetHeight() - nItemHeight;
			}
			
			A3DRECT rectItem(m_nA2DSPriteUserPicItemPosX, m_nA2DSPriteUserPicItemPosY, m_nA2DSPriteUserPicItemPosX + nItemWidth, m_nA2DSPriteUserPicItemPosY + nItemHeight);


			m_pA2DSpriteUserPic->ResetItems(1, &rectItem);
			

			m_pPicUserPic->SetCover( m_pA2DSpriteUserPic, 0);

				//设置4个按钮位置
				m_pBtnUserPicDrag->SetPos(nWinWidth - m_pBtnUserPicDrag->GetSize().cx
										  -dialogFrameSize.cx/2, 
											nWinHeight - m_pBtnUserPicDrag->GetSize().cy
											-dialogFrameSize.cy/2);

				m_pBtnUserPicZoomIn->SetPos(nWinWidth - m_pBtnUserPicDrag->GetSize().cx 
											- m_pBtnUserPicZoomIn->GetSize().cx
											-dialogFrameSize.cx/2,
											nWinHeight- m_pBtnUserPicZoomIn->GetSize().cy
											-dialogFrameSize.cy/2);

				m_pBtnUserPicZoomOut->SetPos(nWinWidth - m_pBtnUserPicDrag->GetSize().cx 
											- m_pBtnUserPicZoomIn->GetSize().cx 
											- m_pBtnUserPicZoomOut->GetSize().cx
											-dialogFrameSize.cx/2,
											nWinHeight - m_pBtnUserPicZoomOut->GetSize().cy
											-dialogFrameSize.cy/2);


				//设置滚动条和标签的位置
				m_pLabelUserPicAlpha->SetPos(dialogFrameSize.cx/2, m_pWinUserPic->GetSize().cy - m_pLabelUserPicAlpha->GetSize().cy -dialogFrameSize.cy/2);
				m_pSliderUserPicAlpha->SetPos(50, m_pWinUserPic->GetSize().cy - m_pSliderUserPicAlpha->GetSize().cy -dialogFrameSize.cy/2);
				m_pLabelUserPic->SetPos((m_pWinUserPic->GetSize().cx - m_pLabelUserPic->GetSize().cx)/2, dialogFrameSize.cy/2);
			}
		}

	}

	//拖动图片
	if( uMsg == WM_MOUSEMOVE)
	{	
		//左键按下
		if(wParam & MK_LBUTTON)
		{	
			//是图片控件
			if( 0 == strcmpi(pObj->GetName(), "Pic_UserPic"))
			{

				
				int nMouseX, nMouseY;
//				nMouseX = LOWORD(lParam);
//				nMouseY = HIWORD(lParam);

				A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
				nMouseX = GET_X_LPARAM(lParam)  - p->X;
				nMouseY = GET_Y_LPARAM(lParam)  - p->Y;

//				//鼠标在图片上的位置(拖动窗口)
//				int nMouseOnPicX,nMouseOnPicY;
//				nMouseOnPicX = nMouseX - m_pWinUserPic->GetPos().x - m_pPicUserPic->GetPos().x + m_nA2DSPriteUserPicItemPosX;
//				nMouseOnPicY = nMouseY - m_pWinUserPic->GetPos().y - m_pPicUserPic->GetPos().y + m_nA2DSPriteUserPicItemPosY; 
//						
//				A3DRECT rectItem;
//				
//				rectItem.left = nMouseOnPicX - m_nMouseOnItemX;
//				rectItem.top = nMouseOnPicY - m_nMouseOnItemY;

				//(拖动图片)
				int nMouseOnItemX, nMouseOnItemY;
				nMouseOnItemX = (int)((nMouseX - m_pWinUserPic->GetPos().x - m_pPicUserPic->GetPos().x)* (1.0f/m_fA2DSpriteUserPicScale));
				nMouseOnItemY = (int)((nMouseY - m_pWinUserPic->GetPos().y - m_pPicUserPic->GetPos().y)* (1.0f/m_fA2DSpriteUserPicScale)) ;

				A3DRECT rectItem;
				rectItem.left = m_nMouseOnA2DSpriteX - nMouseOnItemX;
				rectItem.top = m_nMouseOnA2DSpriteY - nMouseOnItemY;

				if( rectItem.left < 0)
				{
					rectItem.left = 0;
				}
				if( rectItem.top < 0)
				{
					rectItem.top = 0;
				}
								
				if(rectItem.left + m_pPicUserPic->GetSize().cx* (1.0f/m_fA2DSpriteUserPicScale) > m_pA2DSpriteUserPic->GetWidth())
				{
					
					rectItem.left = (int)(m_pA2DSpriteUserPic->GetWidth() - m_pPicUserPic->GetSize().cx * (1.0f/m_fA2DSpriteUserPicScale));
				}
				if(rectItem.top + m_pPicUserPic->GetSize().cy * (1.0f/m_fA2DSpriteUserPicScale) > m_pA2DSpriteUserPic->GetHeight())
				{
					
					rectItem.top = (int)(m_pA2DSpriteUserPic->GetHeight() - m_pPicUserPic->GetSize().cy * (1.0f/m_fA2DSpriteUserPicScale));
				}
				


				m_nA2DSPriteUserPicItemPosX = rectItem.left;
				m_nA2DSPriteUserPicItemPosY = rectItem.top;
				
				rectItem.right = rectItem.left + (int)(m_pPicUserPic->GetSize().cx * (1.0f/m_fA2DSpriteUserPicScale));
				rectItem.bottom = rectItem.top + (int)(m_pPicUserPic->GetSize().cy * (1.0f/m_fA2DSpriteUserPicScale));
						
				m_pA2DSpriteUserPic->ResetItems(1, &rectItem);
				
				m_pPicUserPic->SetCover( m_pA2DSpriteUserPic, 0);

			}
		}
	}

	if( uMsg == WM_LBUTTONUP)
	{	
		if( 0 == strcmpi(pObj->GetName(), "Btn_UserPicDrag"))
		{
			pDlg->SetCaptureObject(NULL);	
		}
		
//		if( 0 == strcmpi(pObj->GetName(), "Pic_UserPic"))
//		{
//			//pDlg->SetCaptureObject(NULL);
//
//			//解除禁止移动
//			pDlg->SetCanMove(true);
//		}
	}

	return true;
}


