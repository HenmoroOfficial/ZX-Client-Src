/*
 * FILE: EC_Customize.cpp
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

#include "FaceBoneController.h"
#include "EC_Global.h"
#include "EC_LoginUIMan.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_ShadowRender.h"
#include "EC_GameSession.h"
#include "EC_World.h"
#include "EC_Player.h"
#include "EC_LoginPlayer.h"
#include "EC_Face.h"

#include "A2DSprite.h"
#include "A2DSpriteItem.h"
#include "A3DFont.h"
#include "A3DSurfaceMan.h"
#include "A3DSurface.h"
#include "A3DCamera.h"
#include "A3DFuncs.h"

#include "AIniFile.h"

#include "elementdataman.h"

#define new A_DEBUG_NEW

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////
#define MSGBOX(sid) MessageBox("", GetStringFromTable(sid), MB_OK, A3DCOLORRGBA(255, 255, 255, 160))


///////////////////////////////////////////////////////////////////////////
//	
//	Reference to External variables and functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Local Types and Variables and Global variables
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////


bool CECCustomize::Tick_Customize()
{
	DWORD dwAnimTime = g_pGame->GetTickTime();

	RandomExpression();

	CustomizeExpressionTick();

	//移动摄像机
	Tick_MoveCamera();

	//放大缩小用户自定义图片
	Tick_WinUserPic();

	//禁止参考图片框拖动到窗口右侧的个性化对话框上
	if(m_pWinUserPic->GetPos().x + m_pWinUserPic->GetSize().cx >m_pWinChooseFace->GetPos().x)
	{
		// old : m_pWinUserPic->SetPos( m_pWinChooseFace->GetPos().x - m_pWinUserPic->GetSize().cx, m_pWinUserPic->GetPos().y);
		m_pWinUserPic->SetPosEx(m_pWinChooseFace->GetPos().x - m_pWinUserPic->GetSize().cx, m_pWinUserPic->GetPos().y);
	}
	
	return true;
}

//随机
void CECCustomize::RandomExpression(void)
{
	int nRand = rand()%(FACE_ANIMATION_MAX + 500);
	if( nRand < FACE_ANIMATION_MAX)
	{		
		m_pCurFace->PlayAnimation((FACE_ANIMATION)nRand);
	}
}

bool CECCustomize::Render_Customize()
{
	return true;
}

bool CECCustomize::Init_Customize(CustomizeInitParam_t cusInitParam)
{
	m_pAUIManager = cusInitParam.pAUImanager;
	m_pCurPlayer = cusInitParam.pCurPlayer;
	m_pCurFace = m_pCurPlayer->GetFaceModel();
	m_bCallForCreate = cusInitParam.bCallForCreate;

	//获取滚动条范围
	m_pSliderBound = g_CustomizeBoundMgr.GetCustomizeBound("Configs\\CustomizeBound.ini");
	assert(m_pSliderBound);
	
	//初始化个性化脸型的对话框的控件
	InitItem();

	//初始化成员变量
	InitVariable();

	// before customizing, we should store current data for later restore
	m_pCurPlayer->StoreCustomizeData();
	
	
	//获取元素数据管理器
	elementdataman* pElementDataMan;
	pElementDataMan= g_pGame->GetElementDataMan ();

	//数据空间ID
	ID_SPACE eIDSpace = ID_SPACE_FACE;		

	//获取脸型相关数据数量
	int nNumFaceData;
	nNumFaceData = pElementDataMan->get_data_num( eIDSpace);

	//脸型相关的数据类型
	DATA_TYPE dtFaceData;

	//脸型各纹理记数(初始化时只使用第一套纹理)
	unsigned int uFaceTexCount = 0;
	unsigned int uEyeBaseTexCount = 0;
	unsigned int uEyeBaseTexCount2 = 0;
	unsigned int uEyeBaseTexCount3 = 0;

	unsigned int uEyeHighTexCount = 0;
	unsigned int uBrowTexCount = 0;
	unsigned int uMouthTexCount = 0;
	unsigned int uNoseTexCount = 0;
	unsigned int uEyeBallTexCount = 0;
	unsigned int uMoustacheTexCount = 0;
	unsigned int uHairTexCount = 0;
	unsigned int uGoateeTexCount = 0;

	//形状相关数据
	unsigned int uFaceShapeCount = 0;//脸型数据计数
	unsigned int uEyeShapeCount1 = 0;//眼型计数1
	unsigned int uEyeShapeCount2 = 0;//眼型计数2
	unsigned int uEyeShapeCount3 = 0;//眼型计数3
	unsigned int uBrowShapeCount = 0;//眉型计数
	unsigned int uNoseShapeCount1 = 0;//鼻头计数
	unsigned int uNoseShapeCount2 = 0;//鼻梁计数
	unsigned int uUpLipShapeCount = 0;//上唇线计数
	unsigned int uMiddleLipShapeCount = 0;//唇沟计数
	unsigned int uDownLipShapeCount = 0;//下唇线计数
	unsigned int uEarShapeCount = 0;//耳型计数

	//头发模型相关计数
	unsigned int uHairModelCount = 0;

	//胡子模型计数
	unsigned int uGoateeModelCount = 0;

	//皮肤颜色图片相关数据
	unsigned int uColorPickerItemCount = 0;

	//预设数据计数
	unsigned int uPreCustomizeCount = 0;

	//法令计数
	unsigned int uFalingCount = 0;
	unsigned int uFalingTexCount = 0;

	CECFace::FACE_CUSTOMIZEDATA* pFaceData = m_pCurFace->GetFaceData();

	int nCharacterMask = 1 << m_pCurPlayer->GetProfession();

	for( int i = 0; i < nNumFaceData; i++)
	{
		//获取脸型数据id
		unsigned int uFaceDataID = pElementDataMan->get_data_id(eIDSpace, i, dtFaceData);
	
		//---------------------------------------------------
		//如果是脸型纹理数据
		//---------------------------------------------------
		if( dtFaceData == DT_FACE_TEXTURE_ESSENCE)
		{
			FACE_TEXTURE_ESSENCE* pFaceTextureEssence = (FACE_TEXTURE_ESSENCE*)pElementDataMan->get_data_ptr(uFaceDataID, eIDSpace,dtFaceData); 
			//当前人物id和性别id
			int uCurCharacterComboID = pFaceTextureEssence->character_combo_id ;
			int uCurGenderID = pFaceTextureEssence->gender_id ;

			//数据类型
			unsigned int uUserDataID = pFaceTextureEssence->user_data ;

			//设置当前人物的脸部纹理
			if( (nCharacterMask & uCurCharacterComboID) && (m_pCurPlayer->GetGender() == uCurGenderID))
			{

				unsigned int id = pFaceTextureEssence->id;
				
				//脸型纹理部分id
				unsigned int uCurTexPartID = pFaceTextureEssence->tex_part_id;
				
				//图标文件
				char* pszIconFile = pFaceTextureEssence->file_icon;

				//纹理的名字
				ACHAR* pszHintName = pFaceTextureEssence->name;



				//用于插入listbox 字符串
				ACHAR *pszName = _AL("");

				A2DSprite* pA2DIcon;

				if(*pszIconFile)
				{
				
					pszIconFile += 9;//跳过"surfaces\"			
					//变图标文件为A2DSprite文件
					pA2DIcon = new A2DSprite; //需要释放
					A3DDevice* pDevice = g_pGame->GetA3DDevice();
					if( !pA2DIcon->Init ( g_pGame->GetA3DDevice(), pszIconFile, 0))
					{
						delete[] pA2DIcon;

						#ifdef _DEBUG
						_asm int 3
						#endif

						return false;
					}

					//把A2DSprite*对象插入Vector
					m_vecA2DSprite.push_back(pA2DIcon);
				}				
				
				switch( uCurTexPartID )
				{
				case 0://脸

					uFaceTexCount++;
					break;
				case 1://眼皮

					if( uUserDataID == 0)
					{
						m_pListBoxEyeBaseTex1->InsertString(uEyeBaseTexCount , pszName);
						m_pListBoxEyeBaseTex1->SetItemImage(uEyeBaseTexCount , pA2DIcon,  0);
						m_pListBoxEyeBaseTex1->SetItemData(uEyeBaseTexCount , id);
						m_pListBoxEyeBaseTex1->SetItemHint(uEyeBaseTexCount, pszHintName);
						
						//设置选中
						if( id == int (pFaceData->idEyeBaseTex))
						{
							//设置listbox的当前选中
							m_pListBoxEyeBaseTex1->SetCurSel(uEyeBaseTexCount );
						}	

						uEyeBaseTexCount++;

					}
					else if( uUserDataID == 1)
					{	
						m_pListBoxEyeBaseTex2->InsertString(uEyeBaseTexCount2 , pszName);
						m_pListBoxEyeBaseTex2->SetItemImage(uEyeBaseTexCount2 , pA2DIcon,  0);
						m_pListBoxEyeBaseTex2->SetItemData(uEyeBaseTexCount2 , id);
						m_pListBoxEyeBaseTex2->SetItemHint(uEyeBaseTexCount2, pszHintName);

						//设置选中
						if( id == int (pFaceData->idEyeBaseTex))
						{
							//设置listbox的当前选中
							m_pListBoxEyeBaseTex2->SetCurSel(uEyeBaseTexCount2 );
						}
						
						uEyeBaseTexCount2++;

					}
					else if( uUserDataID == 2)
					{
						m_pListBoxEyeBaseTex3->InsertString(uEyeBaseTexCount3 , pszName);
						m_pListBoxEyeBaseTex3->SetItemImage(uEyeBaseTexCount3 , pA2DIcon,  0);
						m_pListBoxEyeBaseTex3->SetItemData(uEyeBaseTexCount3 , id);
						m_pListBoxEyeBaseTex3->SetItemHint(uEyeBaseTexCount3, pszHintName);

						//设置选中
						if( id == int (pFaceData->idEyeBaseTex))
						{
							//设置listbox的当前选中
							m_pListBoxEyeBaseTex3->SetCurSel(uEyeBaseTexCount3 );
						}
						uEyeBaseTexCount3++;
					}				

					break;
				case 2://眼影
					m_pListBoxEyeKohl->InsertString(uEyeHighTexCount , pszName);
					m_pListBoxEyeKohl->SetItemImage(uEyeHighTexCount , pA2DIcon,  0);
					m_pListBoxEyeKohl->SetItemData(uEyeHighTexCount , id);
					m_pListBoxEyeKohl->SetItemHint(uEyeHighTexCount, pszHintName);
				
				
					//设置选中
					if( id == int (pFaceData->idEyeHighTex))
					{
						//设置listbox的当前选中
						m_pListBoxEyeKohl->SetCurSel(uEyeHighTexCount );
					}

					uEyeHighTexCount++;

					break;

				case 3://眉毛
					
					m_pListBoxBrowTex->InsertString(uBrowTexCount, pszName);
					m_pListBoxBrowTex->SetItemImage(uBrowTexCount, pA2DIcon,  0);
					m_pListBoxBrowTex->SetItemData(uBrowTexCount, id);
					m_pListBoxBrowTex->SetItemHint(uBrowTexCount, pszHintName);
				
					//设置选中
					if( id == int (pFaceData->idBrowTex))
					{
						//设置listbox的当前选中
						m_pListBoxBrowTex->SetCurSel(uBrowTexCount);
					}
					uBrowTexCount++;

					break;

				case 4://嘴唇

					uMouthTexCount++;
					break;

				case 5://鼻子

					m_pListBoxNoseTex->InsertString(uNoseTexCount, pszName);
					m_pListBoxNoseTex->SetItemImage(uNoseTexCount, pA2DIcon,  0);
					m_pListBoxNoseTex->SetItemData(uNoseTexCount, id);
					m_pListBoxNoseTex->SetItemHint(uNoseTexCount, pszHintName);
				
				
					//设置选中
					if( id == int (pFaceData->idNoseTex))
					{
						//设置listbox的当前选中
						m_pListBoxNoseTex->SetCurSel(uNoseTexCount);
					}

					uNoseTexCount++;
					break;

				case 6://眼球

					uEyeBallTexCount++;
					break;
					
				case 7://胡子(性别为男时才有)
					
					m_pListBoxMoustache->InsertString(uMoustacheTexCount, pszName);
					m_pListBoxMoustache->SetItemImage(uMoustacheTexCount, pA2DIcon,  0);
					m_pListBoxMoustache->SetItemData(uMoustacheTexCount, id);
					m_pListBoxMoustache->SetItemHint(uMoustacheTexCount, pszHintName);
				
				
					//设置选中
					if( id == int (pFaceData->idMoustacheTex))
					{
						//设置listbox的当前选中
						m_pListBoxMoustache->SetCurSel(uMoustacheTexCount);
					}

					uMoustacheTexCount++;

					break;

				case 8: //头发纹理
					m_pListBoxHairTex->InsertString(uHairTexCount, pszName);
					m_pListBoxHairTex->SetItemImage(uHairTexCount, pA2DIcon, 0);
					m_pListBoxHairTex->SetItemData(uHairTexCount, id);
					m_pListBoxHairTex->SetItemHint(uHairTexCount, pszHintName);
					
					//设置选中
					if( id == int(pFaceData->idHairTex))
					{
						m_pListBoxHairTex->SetCurSel(uHairTexCount);
					}

					uHairTexCount ++;
					
					break;
					
				case 9: //大胡子纹理
					m_pListBoxGoateeTex->InsertString(uGoateeTexCount, pszName);
					m_pListBoxGoateeTex->SetItemImage(uGoateeTexCount, pA2DIcon, 0);
					m_pListBoxGoateeTex->SetItemData(uGoateeTexCount, id);
					m_pListBoxGoateeTex->SetItemHint(uGoateeTexCount, pszHintName);

					//设置选中
					if( id == int( pFaceData->idGoateeTex))
					{
						m_pListBoxGoateeTex->SetCurSel(uGoateeTexCount);
					}
					uGoateeTexCount++;

					break;
				case 10://法令纹理

					m_pListBoxFalingTex->InsertString(uFalingTexCount, pszName);
					m_pListBoxFalingTex->SetItemImage(uFalingTexCount, pA2DIcon, 0);
					m_pListBoxFalingTex->SetItemData(uFalingTexCount, id);
					m_pListBoxFalingTex->SetItemHint(uFalingTexCount, pszHintName);

					if( id == int(pFaceData->idFalingTex))
					{
						m_pListBoxFalingTex->SetCurSel(uFalingTexCount);
					}
					uFalingTexCount++;
					break;

				default:
					break;
	
				}//switch 脸各部分数据

			}//if 当前人物
						
		}//if 与脸型相关的纹理数据
		
		//------------------------------------------------------
		//如果是脸型相关的形状数据
		//------------------------------------------------------
		else if( dtFaceData == DT_FACE_SHAPE_ESSENCE)
		{
			FACE_SHAPE_ESSENCE* pFaceShapeEssence = (FACE_SHAPE_ESSENCE*)pElementDataMan->get_data_ptr(uFaceDataID, eIDSpace,dtFaceData);
			
			//当前人物id和性别id
			int uCurCharacterComboID = pFaceShapeEssence->character_combo_id ;
			int uCurGenderID = pFaceShapeEssence->gender_id ;

			//数据类型
			unsigned int uUserDataID = pFaceShapeEssence->user_data ;

			//当前人物的脸部形状
			if( (nCharacterMask & uCurCharacterComboID) && (m_pCurPlayer->GetGender() == uCurGenderID))
			{
				//数据id
				unsigned int id = pFaceShapeEssence->id ;

				//数据名称
				namechar* pszName = pFaceShapeEssence->name ;
				
				//脸部各部分的id
				unsigned int uFaceShapePartID = pFaceShapeEssence->shape_part_id ;

				//形状数据文件
				char* pszShapeFile = pFaceShapeEssence->file_shape;

				//图标文件
				char* pszIconFile = pFaceShapeEssence->file_icon;

				//
				ACHAR* pszHintName = pFaceShapeEssence->name;

				if( *pszIconFile)
				{
					pszIconFile += 9;//跳过"surfaces\"	
				}

				
				//变图标文件为A2DSprite文件
				A2DSprite* pA2DIcon = new A2DSprite; //需要释放
	
				A3DDevice* pDevice = g_pGame->GetA3DDevice();
				if( !pA2DIcon->Init ( g_pGame->GetA3DDevice(), pszIconFile, 0))
				{
					
					delete[] pA2DIcon;
					#ifdef _DEBUG
					_asm int 3
					#endif
					return false;
				}
				
				//把A2DSprite*对象插入Vector
 				m_vecA2DSprite.push_back(pA2DIcon);
				
				//控件名称
				char pszCtrlName[100];

				PAUIIMAGEPICTURE pImagePic;

		
				//判断是脸部各部分的形变数据
				switch( uFaceShapePartID)
				{
				case 0://脸型
					uFaceShapeCount ++;
					
					sprintf(pszCtrlName, "E_Face%d", uFaceShapeCount);
					pImagePic = (PAUIIMAGEPICTURE)m_pWinChooseFace->GetDlgItem(pszCtrlName);
					
 					pImagePic->SetCover(pA2DIcon, 0);
					pImagePic->SetData(id);//设置数据id
					pImagePic->SetHint(pszHintName);
					
					//设置选中
					if( id == pFaceData->idFaceShape1 )
					{
						pImagePic->SetColor(m_clrSel);
						m_pImagePicCurFaceShape1 = pImagePic;
						
					}
					if(id == pFaceData->idFaceShape2)
					{
						pImagePic->SetColor(m_clrSel);
						m_pImagePicCurFaceShape2 = pImagePic;
					}

					break;

				case 1://眼型
					if( uUserDataID == 0)
					{
						m_pListBoxEyeShape1->InsertString(uEyeShapeCount1, pszName);
						m_pListBoxEyeShape1->SetItemImage(uEyeShapeCount1, pA2DIcon,  0);
						m_pListBoxEyeShape1->SetItemData(uEyeShapeCount1, id);
						m_pListBoxEyeShape1->SetItemHint(uEyeShapeCount1, pszHintName);
								
						//设置选中
						if( id == int (pFaceData->idEyeShape))
						{
							//设置listbox的当前选中
							m_pListBoxEyeShape1->SetCurSel(uEyeShapeCount1);
						}

						uEyeShapeCount1++;
						
					}
					else if( uUserDataID == 1)
					{
						m_pListBoxEyeShape2->InsertString(uEyeShapeCount2, pszName);
						m_pListBoxEyeShape2->SetItemImage(uEyeShapeCount2, pA2DIcon,  0);
						m_pListBoxEyeShape2->SetItemData(uEyeShapeCount2, id);
						m_pListBoxEyeShape2->SetItemHint(uEyeShapeCount2, pszHintName);
								
						//设置选中
						if( id == int (pFaceData->idEyeShape))
						{
							//设置listbox的当前选中
							m_pListBoxEyeShape2->SetCurSel(uEyeShapeCount2);
						}

						uEyeShapeCount2++;
						
					}
					else if( uUserDataID == 2)
					{
						m_pListBoxEyeShape3->InsertString(uEyeShapeCount3, pszName);
						m_pListBoxEyeShape3->SetItemImage(uEyeShapeCount3, pA2DIcon,  0);
						m_pListBoxEyeShape3->SetItemData(uEyeShapeCount3, id);
						m_pListBoxEyeShape3->SetItemHint(uEyeShapeCount3, pszHintName);
								
						//设置选中
						if( id == int (pFaceData->idEyeShape))
						{
							//设置listbox的当前选中
							m_pListBoxEyeShape3->SetCurSel(uEyeShapeCount3);
						}

						uEyeShapeCount3++;
	
					}

					break;

				case 2://眉型

					m_pListBoxBrow->InsertString(uBrowShapeCount, pszName);
					m_pListBoxBrow->SetItemImage(uBrowShapeCount, pA2DIcon,  0);
					m_pListBoxBrow->SetItemData(uBrowShapeCount, id);
					m_pListBoxBrow->SetItemHint(uBrowShapeCount, pszHintName);
				
				
					//设置选中
					if( id == int (pFaceData->idBrowShape))
					{
						//设置listbox的当前选中
						m_pListBoxBrow->SetCurSel(uBrowShapeCount);
					}

					uBrowShapeCount++;

					break;

				case 3://鼻头

					m_pListBoxNoseTip->InsertString(uNoseShapeCount1, pszName);
					m_pListBoxNoseTip->SetItemImage(uNoseShapeCount1, pA2DIcon,  0);
					m_pListBoxNoseTip->SetItemData(uNoseShapeCount1, id);
					m_pListBoxNoseTip->SetItemHint(uNoseShapeCount1, pszHintName);
					
					
					//设置选中
					if( id == int (pFaceData->idNoseTipShape))
					{
						//设置listbox的当前选中
						m_pListBoxNoseTip->SetCurSel(uNoseShapeCount1);
					}
					uNoseShapeCount1++;

					break;

				case 4://鼻梁
					m_pListBoxNoseBridge->InsertString(uNoseShapeCount2, pszName);
					m_pListBoxNoseBridge->SetItemImage(uNoseShapeCount2, pA2DIcon,  0);
					m_pListBoxNoseBridge->SetItemData(uNoseShapeCount2, id);
					m_pListBoxNoseBridge->SetItemHint(uNoseShapeCount2, pszHintName);
					
					
					//设置选中
					if( id == int (pFaceData->idNoseBridgeShape))
					{
						//设置listbox的当前选中
						m_pListBoxNoseBridge->SetCurSel(uNoseShapeCount2);
					}
					uNoseShapeCount2++;

					break;

				case 5://上唇线
					
										
					m_pListBoxUpLipLine->InsertString(uUpLipShapeCount, pszName);
					m_pListBoxUpLipLine->SetItemImage(uUpLipShapeCount, pA2DIcon,  0);
					m_pListBoxUpLipLine->SetItemData(uUpLipShapeCount, id);
					m_pListBoxUpLipLine->SetItemHint(uUpLipShapeCount, pszHintName);
					
					
					//设置选中
					if( id == int (pFaceData->idMouthUpLipLine))
					{
						//设置listbox的当前选中
						m_pListBoxUpLipLine->SetCurSel(uUpLipShapeCount);
					}

					uUpLipShapeCount++;

					break;
					
				case 6://唇沟
					
					
					m_pListBoxMidLipLine->InsertString(uMiddleLipShapeCount , pszName);
					m_pListBoxMidLipLine->SetItemImage(uMiddleLipShapeCount , pA2DIcon,  0);
					m_pListBoxMidLipLine->SetItemData(uMiddleLipShapeCount , id);
					m_pListBoxMidLipLine->SetItemHint(uMiddleLipShapeCount, pszHintName);
					
					
					//设置选中
					if( id == int (pFaceData->idMouthMidLipLine))
					{
						//设置listbox的当前选中
						m_pListBoxMidLipLine->SetCurSel(uMiddleLipShapeCount );
					}
					
					uMiddleLipShapeCount++;

					break;
					
				case 7://下唇线
					
					
					m_pListBoxDownLipLine->InsertString(uDownLipShapeCount, pszName);
					m_pListBoxDownLipLine->SetItemImage(uDownLipShapeCount, pA2DIcon,  0);
					m_pListBoxDownLipLine->SetItemData(uDownLipShapeCount, id);
					m_pListBoxDownLipLine->SetItemHint(uDownLipShapeCount, pszHintName);
					
					
					//设置选中
					if( id == int (pFaceData->idMouthDownLipLine))
					{
						//设置listbox的当前选中
						m_pListBoxDownLipLine->SetCurSel(uDownLipShapeCount);
					}

					uDownLipShapeCount++;
					break;

				case 8://耳型
					

					m_pListBoxEar->InsertString(uEarShapeCount, pszName);
					m_pListBoxEar->SetItemImage(uEarShapeCount, pA2DIcon,  0);
					m_pListBoxEar->SetItemData(uEarShapeCount, id);
					m_pListBoxEar->SetItemHint(uEarShapeCount, pszHintName);
					
					
					//设置选中
					if( id == int (pFaceData->idHairModel))
					{
						//设置listbox的当前选中
						m_pListBoxHair->SetCurSel(uEarShapeCount);
					}

					uEarShapeCount ++;

					break;
					
				default:
					break;
				}
				
 			}//if为当前人物
 		}//if为脸部形状
		
		//-------------------------------------------
		//如果是与颜色贴图相关的数据
		//-------------------------------------------
		else if( dtFaceData == DT_COLORPICKER_ESSENCE)
		{
			COLORPICKER_ESSENCE* pColorPickerEssence = (COLORPICKER_ESSENCE*)pElementDataMan->get_data_ptr(uFaceDataID, eIDSpace,dtFaceData);
			
			//当前人物id和性别id
			int uCurCharacterComboID = pColorPickerEssence->character_combo_id ;
			int uCurGenderID = pColorPickerEssence->gender_id ;
			unsigned int uCurColorPartID = pColorPickerEssence->color_part_id;

			//当前人物的脸部形状
			if( (nCharacterMask & uCurCharacterComboID) && (m_pCurPlayer->GetGender() == uCurGenderID))
			{
				//颜色图片文件名
				char* pszPicFileName = pColorPickerEssence->file_colorpicker;
				pszPicFileName += 9;//去掉"surface/"

				//获取数据id
				int id = pColorPickerEssence->id;
				
				//A2DSprite文件
				A2DSprite* pA2DSpriteColorPic = new A2DSprite; //需要释放
	
				A3DDevice*  pDevice = g_pGame->GetA3DDevice();
				if( !pA2DSpriteColorPic->Init ( pDevice, pszPicFileName, 0))
				{
					
					delete[] pA2DSpriteColorPic;

					#ifdef _DEBUG
					_asm int 3
					#endif
						
					return false;
				}
				//把A2DSprite*对象插入Vector
 				m_vecA2DSprite.push_back(pA2DSpriteColorPic);

				A3DSurfaceMan* pA3DSurfaceMan = g_pGame->GetA3DEngine()->GetA3DSurfaceMan ();

				switch(uCurColorPartID)
				{
				case 0://脸			
					m_pListBoxFaceParts->AddString(_AL("皮肤颜色"));
					m_pListBoxFaceParts->SetItemDataPtr (uColorPickerItemCount, pA2DSpriteColorPic);
					m_pListBoxFaceParts->SetItemData(uColorPickerItemCount, id);
					m_pListBoxFaceParts->SetItemData(uColorPickerItemCount, 0, 1);

					uColorPickerItemCount ++;	
					break;
				case 1://眼影
					m_pListBoxFaceParts->AddString(_AL("眼影颜色"));
					
					m_pListBoxFaceParts->SetItemDataPtr (uColorPickerItemCount, pA2DSpriteColorPic);
					m_pListBoxFaceParts->SetItemData(uColorPickerItemCount, id);

					m_pListBoxFaceParts->SetItemData(uColorPickerItemCount, 1, 1);

					uColorPickerItemCount ++;
					break;
				case 2://眉毛
					m_pListBoxFaceParts->AddString(_AL("眉毛颜色"));
					
					m_pListBoxFaceParts->SetItemDataPtr (uColorPickerItemCount, pA2DSpriteColorPic);
					m_pListBoxFaceParts->SetItemData(uColorPickerItemCount, id);
					m_pListBoxFaceParts->SetItemData(uColorPickerItemCount, 2, 1);

					uColorPickerItemCount ++;
					break;
				case 3://嘴唇
					m_pListBoxFaceParts->AddString(_AL("嘴唇颜色"));
					
					m_pListBoxFaceParts->SetItemDataPtr (uColorPickerItemCount, pA2DSpriteColorPic);
					m_pListBoxFaceParts->SetItemData(uColorPickerItemCount, id);
					m_pListBoxFaceParts->SetItemData(uColorPickerItemCount, 3, 1);

					uColorPickerItemCount ++;
					break;
				case 4://头发
					m_pListBoxFaceParts->AddString(_AL("头发颜色"));
					m_pListBoxFaceParts->SetItemDataPtr (uColorPickerItemCount, pA2DSpriteColorPic);
					m_pListBoxFaceParts->SetItemData(uColorPickerItemCount, id);
					m_pListBoxFaceParts->SetItemData(uColorPickerItemCount, 4, 1);

					uColorPickerItemCount ++;
					break;
				case 5://眼珠
					m_pListBoxFaceParts->AddString(_AL("眼珠颜色"));
					m_pListBoxFaceParts->SetItemDataPtr (uColorPickerItemCount, pA2DSpriteColorPic);
					m_pListBoxFaceParts->SetItemData(uColorPickerItemCount, id);
					m_pListBoxFaceParts->SetItemData(uColorPickerItemCount, 5, 1);

					uColorPickerItemCount ++;
					break;
				case 6://胡子
					m_pListBoxFaceParts->AddString(_AL("胡子颜色"));
					m_pListBoxFaceParts->SetItemDataPtr (uColorPickerItemCount, pA2DSpriteColorPic);
					m_pListBoxFaceParts->SetItemData(uColorPickerItemCount, id);
					m_pListBoxFaceParts->SetItemData(uColorPickerItemCount, 6, 1);

					uColorPickerItemCount ++;
					break;
				}


			}//if为当前人物数据

		}//if 颜色贴图数据

		//------------------------------------------------
		//如果是与头发模型相关的数据
		//------------------------------------------------
		else if( dtFaceData == DT_FACE_HAIR_ESSENCE)
		{
			FACE_HAIR_ESSENCE* pFaceHairEssence = (FACE_HAIR_ESSENCE*)pElementDataMan->get_data_ptr(uFaceDataID, eIDSpace,dtFaceData);

			//当前人物id和性别id
			int uCurCharacterComboID = pFaceHairEssence->character_combo_id ;
			int uCurGenderID = pFaceHairEssence->gender_id ;

			int id = pFaceHairEssence->id;

			//图标文件
			char* pszIconFile = pFaceHairEssence->file_icon;

			char* pszHairModelFile = pFaceHairEssence->file_hair_model;
			char* pszHairSkinFile = pFaceHairEssence->file_hair_skin;
			
			ACHAR* pszHintName = pFaceHairEssence->name;

			A2DSprite* pA2DIcon;

			if(*pszIconFile)
			{
			
				pszIconFile += 9;//跳过"surfaces\"			
				//变图标文件为A2DSprite文件
				pA2DIcon = new A2DSprite; //需要释放
				A3DDevice* pDevice = g_pGame->GetA3DDevice();
				if( !pA2DIcon->Init ( g_pGame->GetA3DDevice(), pszIconFile, 0))
				{
					delete[] pA2DIcon;

					#ifdef _DEBUG
					_asm int 3
					#endif

					return false;
				}

				//把A2DSprite*对象插入Vector
				m_vecA2DSprite.push_back(pA2DIcon);
			}

			//当前人物的头发模型
			if( (nCharacterMask & uCurCharacterComboID) && (m_pCurPlayer->GetGender() == uCurGenderID))
			{
				
			
				//设置列表框
				ACHAR *pszName = _AL("");
				m_pListBoxHair->InsertString(uHairModelCount , pszName);
				m_pListBoxHair->SetItemImage(uHairModelCount , pA2DIcon,  0);
				m_pListBoxHair->SetItemData(uHairModelCount , id);
				m_pListBoxHair->SetItemHint(uHairModelCount, pszHintName);
				
				
				//设置选中
				if( id == int (pFaceData->idHairModel))
				{
					//设置listbox的当前选中
					m_pListBoxHair->SetCurSel(uHairModelCount );
				}

				uHairModelCount ++;
				
			}//if当前人物的头发模型

		}//if头发模型相关的数据

		//与胡子相关数据
		else if( dtFaceData == DT_FACE_MOUSTACHE_ESSENCE)
		{
			
			FACE_MOUSTACHE_ESSENCE* pFaceMoustancheEssence = (FACE_MOUSTACHE_ESSENCE*) pElementDataMan->get_data_ptr(uFaceDataID, eIDSpace,dtFaceData);

			//当前人物id和性别id
			int uCurCharacterComboID = pFaceMoustancheEssence->character_combo_id ;
			int uCurGenderID = pFaceMoustancheEssence->gender_id ;

			int id = pFaceMoustancheEssence->id;
			
			ACHAR* pszGoateeName = pFaceMoustancheEssence->name;

			//图标文件
			char* pszIconFile = pFaceMoustancheEssence->file_icon;

			char* pszGoateeSkinFile = pFaceMoustancheEssence->file_moustache_skin;
		
			ACHAR* pszHintName = pFaceMoustancheEssence->name;

			A2DSprite* pA2DIcon;
			if(*pszIconFile)
			{
			
				pszIconFile += 9;//跳过"surfaces\"			
				//变图标文件为A2DSprite文件
				pA2DIcon = new A2DSprite; //需要释放
				A3DDevice* pDevice = g_pGame->GetA3DDevice();
				if( !pA2DIcon->Init ( g_pGame->GetA3DDevice(), pszIconFile, 0))
				{
					delete[] pA2DIcon;

					#ifdef _DEBUG
					_asm int 3
					#endif

					return false;
				}

				//把A2DSprite*对象插入Vector
				m_vecA2DSprite.push_back(pA2DIcon);
			}
			
			//当前人物的胡子模型
			if( (nCharacterMask & uCurCharacterComboID) && (m_pCurPlayer->GetGender() == uCurGenderID))
			{	
				//设置列表框
				ACHAR *pszName = _AL("");
				m_pListBoxGoatee->InsertString(uGoateeModelCount , pszName);
				m_pListBoxGoatee->SetItemImage(uGoateeModelCount , pA2DIcon,  0);
				m_pListBoxGoatee->SetItemData(uGoateeModelCount , id);
				m_pListBoxGoatee->SetItemHint(uGoateeModelCount, pszHintName);
				
				
				//设置选中
				if( id == int (pFaceData->idMoustacheSkin))
				{
					//设置listbox的当前选中
					m_pListBoxGoatee->SetCurSel(uGoateeModelCount );
				}

				uGoateeModelCount ++;
				
			}//if当前人物的胡子模型

		}//if胡子模型相关数据

		//与预定义相关的数据
		else if( dtFaceData == DT_CUSTOMIZEDATA_ESSENCE)
		{
			CUSTOMIZEDATA_ESSENCE* pCustomizedDataEssence = (CUSTOMIZEDATA_ESSENCE*)pElementDataMan->get_data_ptr(uFaceDataID, eIDSpace,dtFaceData);
			
			//当前人物id和性别id
			int uCurCharacterComboID = pCustomizedDataEssence->character_combo_id ;
			int uCurGenderID = pCustomizedDataEssence->gender_id ;

			int id = pCustomizedDataEssence->id;
			
			ACHAR* pszName = pCustomizedDataEssence->name;
			char* pszFile = pCustomizedDataEssence->file_data;

			//当前人物的预设数据
			if( (nCharacterMask & uCurCharacterComboID) && (m_pCurPlayer->GetGender() == uCurGenderID))
			{
				m_pListBoxPreCustomize->InsertString(uPreCustomizeCount, pszName);
				m_pListBoxPreCustomize->SetItemDataPtr(uPreCustomizeCount,pszFile);
				m_pListBoxPreCustomize->SetItemHint(uPreCustomizeCount, pszName);
				
				uPreCustomizeCount++;
				m_nPreCustomsizeCount++;
			}

		}//if与预定义相关的数据

		//与法令相关
		else if ( dtFaceData == DT_FACE_FALING_ESSENCE)
		{
			FACE_FALING_ESSENCE* pFaceFalingEssence = (FACE_FALING_ESSENCE*)pElementDataMan->get_data_ptr(uFaceDataID, eIDSpace,dtFaceData);

			//当前人物id和性别id
			int uCurCharacterComboID = pFaceFalingEssence->character_combo_id;
			int uCurGenderID = pFaceFalingEssence->gender_id;

			int id = pFaceFalingEssence->id;

			ACHAR* pszName = pFaceFalingEssence->name;
			char* pszFile = pFaceFalingEssence->file_faling_skin;

			//当前人物的预设数据
			if( (nCharacterMask & uCurCharacterComboID) && (m_pCurPlayer->GetGender() == uCurGenderID))
			{
				if( uFalingCount == 0)
				{
//					CECFace::FACE_CUSTOMIZEDATA* pFaceData = m_pCurFace->GetFaceData();
//					pFaceData->idFalingSkin = id;
//					m_pCurFace->UpdateFalingModel();
				}
				uFalingCount++;
			}

		}

	}//for 所有与脸型相关数据
	


	//设置listbox的选中
	m_pListBoxFaceParts->SetCurSel(0);

	D3DXMATRIX matProj;
	D3DXMatrixOrthoOffCenterLH(&matProj, -0.08f, 0.08f, -0.12f, 0.12f, 0.0f, 1.0f);
	m_pCurFace->SetTattooProjMatrix(*(A3DMATRIX4 *)&matProj);


	//载入脸型相关的调节因子
	//LoadFaceShapeFactorFromIniFile("Configs\\CustomizeFactor.ini");
	//m_pCurFace->LoadFaceCusFactorFromIni();
	//m_pCurPlayer->LoadCustomizeFactorFromIni();

	//恢复滚动条的位置
	SetFaceAllSlider();
	
	//载入用户自定义数据
	LoadUserCustomizeData();

	//载入用户自定义图片
	LoadUserPic();

	//设置当前颜色
	m_pImagePictureCurColor->SetColor( pFaceData->colorFace);

	m_pBtmCurChoose = m_pBtmChoosePreCustomize;
	m_pBtmCurChoose->SetPushed(true);
	OnCommand_SwitchDlg("ChoosePreCustomize", m_pWinChoosePreCustomize);

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
	
	//设置全部个性化颜色位置
	SetAllColorPos();
		
	m_pAUIManager->GetDialog("Win_ChooseFacePainting")->SetNoFlush(true);

	//显示移动摄像机窗口
	m_pAUIManager->GetDialog("Win_MoveCamera")->Show(true);
	
	//显示用户参考图片窗口
	//GetDialog("Win_UserPic")->Show(true);

	//设置初始化颜色图片
	A2DSprite* pA2DSprite = (A2DSprite*)m_pListBoxFaceParts->GetItemDataPtr(0);
	m_pImagePictureColorPic->SetCover(pA2DSprite, 0);
	m_pListBoxFaceParts->SetCurSel(0);

	//获取元素数据管理器
	pElementDataMan= g_pGame->GetElementDataMan ();
	int id = m_pListBoxFaceParts->GetItemData ( 0);
	dtFaceData = DT_COLORPICKER_ESSENCE;
	COLORPICKER_ESSENCE* pColorPickerEssence = (COLORPICKER_ESSENCE*)pElementDataMan->get_data_ptr(id, ID_SPACE_FACE,dtFaceData);
	if( !pColorPickerEssence)
	{
		return true;
	}
	
	char* pszPicFileName = pColorPickerEssence->file_colorpicker;
	pszPicFileName +=9; //去掉"surface\\"
	A3DSurfaceMan* pA3DSurfaceMan = g_pGame->GetA3DEngine ()->GetA3DSurfaceMan ();				

	if( !pA3DSurfaceMan->LoadCursorSurfaceFromFile ( pszPicFileName, 0, &m_pA3DSurfaceColorPic))
	{
		m_pA3DSurfaceColorPic = NULL;
	}
	m_ColorPicFMT = m_pA3DSurfaceColorPic->GetFormat ();

	//-------------------用于测试自定义表情-----------------

	//载入自定义表情数据
 	LoadCustomizeExpression();

	return true;
}

bool CECCustomize::Release_Customize()
{
	//释放A2DSprite对象
 	abase::vector<A2DSprite*>::iterator iter;
	abase::vector<A2DSprite*>::iterator iterEnd;
	iter = m_vecA2DSprite.begin();
	iterEnd = m_vecA2DSprite.end();
	for( ;iter!=iterEnd; ++iter)
	{
		if( *iter != NULL)
		{
			(*iter)->Release();
			delete *iter;
			
		}

	}

	m_vecA2DSprite.clear();

	//释放自定义数据文件名向量
	abase::vector<char*>::iterator iterFileName;
	abase::vector<char*>::iterator iterFileNameEnd;
	iterFileName = m_vecCustomizeFileName.begin();
	iterFileNameEnd = m_vecCustomizeFileName.end();
	for( ; iterFileName != iterFileNameEnd; ++iterFileName)
	{
		if( *iterFileName != NULL)
		{
			delete[] (*iterFileName);
		}
	}
	m_vecCustomizeFileName.clear();
	


	if(m_pA3DSurfaceColorPic!= NULL)
	{
		m_pA3DSurfaceColorPic->Release();
		m_pA3DSurfaceColorPic = NULL;
	}

	//释放对话框中控件的数据
	ReleaseItem();
	
	//释放成员变量
	ReleaseVariable();

//	m_pCurPlayer->GetA3DSkinModel()->SetDirAndUp(m_vecDirBeforeDrag, m_vecUpBeforeDrag);

	
	//*******************用于测试自定义表情**********************
	//释放
	ReleaseExpressionCtrler();
	
	
	return true;
}

//初始化对话框控件
bool CECCustomize::InitItem( void)
{
	int i = 0;

	m_pImagePicCurFaceShape1 = NULL;
	m_pImagePicCurFaceShape2 = NULL;
	
	
	
	//------------------初始化脸型相关控件--------------------
	m_pWinChooseFace = m_pAUIManager->GetDialog("Win_ChooseFace");
	m_pWinChooseFace->SetCanMove(false);
	//3庭控件
	m_pSliderPartUp = (AUISlider*) m_pWinChooseFace->GetDlgItem("Slider_PartUp");
	m_pSliderPartMiddle = (AUISlider*) m_pWinChooseFace->GetDlgItem("Slider_PartMiddle");
	m_pSliderPartDown = (AUISlider*) m_pWinChooseFace->GetDlgItem("Slider_PartDown");
	


	
	m_pPicFace[0] = (AUIImagePicture*)m_pWinChooseFace->GetDlgItem("E_Face1");
	m_pPicFace[1] = (AUIImagePicture*)m_pWinChooseFace->GetDlgItem("E_Face2");
	m_pPicFace[2] = (AUIImagePicture*)m_pWinChooseFace->GetDlgItem("E_Face3");
	m_pPicFace[3] =(AUIImagePicture*)m_pWinChooseFace->GetDlgItem("E_Face4");
	m_pPicFace[4] =(AUIImagePicture*)m_pWinChooseFace->GetDlgItem("E_Face5");
	

	//初始化脸盘比例
	m_pSliderScaleFaceH = (AUISlider*) m_pWinChooseFace->GetDlgItem("Slider_ScaleFaceH");


	m_pSliderScaleFaceV = (AUISlider*)m_pWinChooseFace->GetDlgItem("Slider_ScaleFaceV");
	
	//-------------------初始化与法令相关的控件--------------------
	m_pListBoxFalingTex = (AUIListBox*)m_pWinChooseFace->GetDlgItem("ListBox_FalingTex");
	m_pListBoxFalingTex->SetViewMode(AUILISTBOX_VIEW_ICON);

	//-------------------初始化与眼型调整相关的控件-----------------

	m_pWinChooseEye = m_pAUIManager->GetDialog("Win_ChooseEye");

	m_pCheckBoxEyeAdjust = (AUICheckBox*)m_pWinChooseEye->GetDlgItem("CheckBox_EyeAdjust");
	m_pCheckBoxEyeAdjust->Check(true);
	
	m_pBtnLeftEye = (AUIStillImageButton*)m_pWinChooseEye->GetDlgItem("Btn_LeftEye");
	m_pBtnRightEye = (AUIStillImageButton*)m_pWinChooseEye->GetDlgItem("Btn_RightEye");
	m_pBtnLeftEye->SetPushLike(true);
	m_pBtnRightEye->SetPushLike(true);
	m_pBtnLeftEye->SetPushed(true);
	m_pBtnRightEye->SetPushed(false);
	
	m_pSliderScaleEyeH = (AUISlider*)m_pWinChooseEye->GetDlgItem("Slider_ScaleEyeH");
	m_pSliderScaleEyeV = (AUISlider*)m_pWinChooseEye->GetDlgItem("Slider_ScaleEyeV");
	m_pSliderRotateEye = (AUISlider*)m_pWinChooseEye->GetDlgItem("Slider_RotateEye");
	m_pSliderOffsetEyeH = (AUISlider*)m_pWinChooseEye->GetDlgItem("Slider_OffsetEyeH");
	m_pSliderOffsetEyeV = (AUISlider*)m_pWinChooseEye->GetDlgItem("Slider_OffsetEyeV");
	m_pSliderOffsetEyeZ = (AUISlider*)m_pWinChooseEye->GetDlgItem("Slider_OffsetEyeZ");
	m_pSliderScaleEyeBall = (AUISlider*)m_pWinChooseEye->GetDlgItem("Slider_ScaleEyeBall");



	char szTemp[100];

	for(i = 0;i < 7;i ++)
	{
		sprintf(szTemp,"Label_Eye%d",i+1);
		m_pLabelEye[i] = (AUILabel*)m_pWinChooseEye->GetDlgItem(szTemp);
	}

	m_pBtmChooseBaseEye1 = (AUIStillImageButton*)m_pWinChooseEye->GetDlgItem("Btm_ChooseBaseEye1");
	m_pBtmChooseBaseEye2 = (AUIStillImageButton*)m_pWinChooseEye->GetDlgItem("Btm_ChooseBaseEye2");
	m_pBtmChooseBaseEye3 = (AUIStillImageButton*)m_pWinChooseEye->GetDlgItem("Btm_ChooseBaseEye3");

	m_pBtmChooseBaseEye1->SetPushLike(true);
	m_pBtmChooseBaseEye2->SetPushLike(true);
	m_pBtmChooseBaseEye3->SetPushLike(true);
	m_pBtmChooseBaseEye1->SetPushed(true);
	

	m_pBtnChooseEyeBaseTex1 = (AUIStillImageButton*)m_pWinChooseEye->GetDlgItem("Btn_ChooseEyeBaseTex1");
	m_pBtnChooseEyeBaseTex2 = (AUIStillImageButton*)m_pWinChooseEye->GetDlgItem("Btn_ChooseEyeBaseTex2");
	m_pBtnChooseEyeBaseTex3 = (AUIStillImageButton*)m_pWinChooseEye->GetDlgItem("Btn_ChooseEyeBaseTex3");

	m_pBtnChooseEyeBaseTex1->SetPushLike(true);
	m_pBtnChooseEyeBaseTex2->SetPushLike(true);
	m_pBtnChooseEyeBaseTex3->SetPushLike(true);
	m_pBtnChooseEyeBaseTex1->SetPushed(true);


	m_pListBoxEyeBaseTex1 = (AUIListBox*)m_pWinChooseEye->GetDlgItem("ListBox_EyeBaseTex1");
	m_pListBoxEyeBaseTex1->SetViewMode(AUILISTBOX_VIEW_ICON);
	m_pListBoxEyeBaseTex1->Show(true);

	m_pListBoxEyeBaseTex2 = (AUIListBox*)m_pWinChooseEye->GetDlgItem("ListBox_EyeBaseTex2");
	m_pListBoxEyeBaseTex2->SetViewMode(AUILISTBOX_VIEW_ICON);
	m_pListBoxEyeBaseTex2->Show(false);

	m_pListBoxEyeBaseTex3 = (AUIListBox*)m_pWinChooseEye->GetDlgItem("ListBox_EyeBaseTex3");
	m_pListBoxEyeBaseTex3->SetViewMode(AUILISTBOX_VIEW_ICON);
	m_pListBoxEyeBaseTex3->Show(false);


	m_pListBoxEyeShape1 = (AUIListBox*)m_pWinChooseEye->GetDlgItem("ListBox_EyeShape1");
	m_pListBoxEyeShape1->SetViewMode(AUILISTBOX_VIEW_ICON);
	
	m_pListBoxEyeShape1->Show(true);

	m_pListBoxEyeShape2 = (AUIListBox*)m_pWinChooseEye->GetDlgItem("ListBox_EyeShape2");
	m_pListBoxEyeShape2->SetViewMode(AUILISTBOX_VIEW_ICON);

	m_pListBoxEyeShape2->Show(false);

	m_pListBoxEyeShape3 = (AUIListBox*)m_pWinChooseEye->GetDlgItem("ListBox_EyeShape3");
	m_pListBoxEyeShape3->SetViewMode(AUILISTBOX_VIEW_ICON);

	m_pListBoxEyeShape3->Show(false);

	//-------------------初始化与眉型调整相关的控件-----------------
	m_pWinChooseEyebrow = m_pAUIManager->GetDialog("Win_ChooseEyebrow");

	m_pSliderScaleBrowH = (AUISlider*)m_pWinChooseEyebrow->GetDlgItem("Slider_ScaleBrowH");
	m_pSliderScaleBrowV = (AUISlider*)m_pWinChooseEyebrow->GetDlgItem("Slider_ScaleBrowV");
	m_pSliderRotateBrow = (AUISlider*)m_pWinChooseEyebrow->GetDlgItem("Slider_RotateBrow");
	m_pSliderOffsetBrowH = (AUISlider*)m_pWinChooseEyebrow->GetDlgItem("Slider_OffsetBrowH");
	m_pSliderOffsetBrowV = (AUISlider*)m_pWinChooseEyebrow->GetDlgItem("Slider_OffsetBrowV");
	m_pSliderOffsetBrowZ = (AUISlider*)m_pWinChooseEyebrow->GetDlgItem("Slider_OffsetBrowZ");



	m_pCheckBoxBrowAdjust = (AUICheckBox*)m_pWinChooseEyebrow->GetDlgItem("CheckBox_BrowAdjust");;
	m_pCheckBoxBrowAdjust->Check(true);
	
	m_pBtnLeftBrow = (AUIStillImageButton*)m_pWinChooseEyebrow->GetDlgItem("Btn_LeftBrow");
	m_pBtnRightBrow = (AUIStillImageButton*)m_pWinChooseEyebrow->GetDlgItem("Btn_RightBrow");

	m_pBtnLeftBrow->SetPushLike(true);
	m_pBtnRightBrow->SetPushLike(true);
	m_pBtnLeftBrow->SetPushed(true);
	m_pBtnRightBrow->SetPushed(false);

	for( i = 0; i < 6; i ++)
	{
		char szTemp[100];
		sprintf( szTemp, "Label_Brow%d", i+1);
		m_pLabelBrow[i] = (AUILabel*)m_pWinChooseEyebrow->GetDlgItem(szTemp);
	}

	m_pBtmChooseBaseEyebrow1 = (AUIStillImageButton*)m_pWinChooseEyebrow->GetDlgItem("Btm_ChooseBaseEyebrow1");
	m_pBtmChooseBaseEyebrow1->SetPushLike(true);
	m_pBtmChooseBaseEyebrow1->SetPushed(true);

	m_pListBoxBrowTex = (AUIListBox*)m_pWinChooseEyebrow->GetDlgItem("ListBox_BrowTex");
	m_pListBoxBrowTex->SetViewMode(AUILISTBOX_VIEW_ICON);


	m_pListBoxBrow = (AUIListBox*)m_pWinChooseEyebrow->GetDlgItem("ListBox_Brow");
	m_pListBoxBrow->SetViewMode(AUILISTBOX_VIEW_ICON);
	m_pListBoxBrow->Show(true);


	//-------------------初始化与鼻型调整相关的控件-----------------

	m_pWinChooseNose = m_pAUIManager->GetDialog("Win_ChooseNose");
	
	m_pSliderScaleNoseTipH = (AUISlider*)m_pWinChooseNose->GetDlgItem("Slider_ScaleNoseTipH");
	m_pSliderScaleNoseTipV = (AUISlider*)m_pWinChooseNose->GetDlgItem("Slider_ScaleNoseTipV");
	m_pSliderScaleNoseTipZ = (AUISlider*)m_pWinChooseNose->GetDlgItem("Slider_ScaleNoseTipZ");
	m_pSliderOffsetNoseTipV = (AUISlider*)m_pWinChooseNose->GetDlgItem("Slider_OffsetNoseTipV");
	m_pSliderScaleBridgeTipH = (AUISlider*)m_pWinChooseNose->GetDlgItem("Slider_ScaleBridgeTipH");
	m_pSliderOffsetBridgeTipZ = (AUISlider*)m_pWinChooseNose->GetDlgItem("Slider_OffsetBridgeTipZ");
	



	m_pBtmChooseBaseNoseTip = (AUIStillImageButton*)m_pWinChooseNose->GetDlgItem("Btm_ChooseBaseNoseTip");
	m_pBtmChooseBaseNoseBridge = (AUIStillImageButton*)m_pWinChooseNose->GetDlgItem("Btm_ChooseBaseNoseBridge");
	m_pBtmChooseBaseNoseTip->SetPushLike(true);
	m_pBtmChooseBaseNoseBridge->SetPushLike(true);
	m_pBtmChooseBaseNoseTip->SetPushed(true);
	

	m_pListBoxNoseTex = (AUIListBox*)m_pWinChooseNose->GetDlgItem("ListBox_NoseTex");
	m_pListBoxNoseTex->SetViewMode(AUILISTBOX_VIEW_ICON);


	m_pListBoxNoseTip = (AUIListBox*)m_pWinChooseNose->GetDlgItem("ListBox_NoseTip");
	m_pListBoxNoseTip->SetViewMode(AUILISTBOX_VIEW_ICON);
	m_pListBoxNoseTip->Show(true);


	m_pListBoxNoseBridge = (AUIListBox*)m_pWinChooseNose->GetDlgItem("ListBox_NoseBridge");
	m_pListBoxNoseBridge->SetViewMode(AUILISTBOX_VIEW_ICON);
	m_pListBoxNoseBridge->Show(false);

	
	//-------------------初始化与嘴型调整相关的控件-----------------

	m_pWinChooseMouth = m_pAUIManager->GetDialog("Win_ChooseMouth");
	
	m_pSliderThickUpLip = (AUISlider*)m_pWinChooseMouth->GetDlgItem("Slider_ThickUpLip");
	m_pSliderThickDownLip = (AUISlider*)m_pWinChooseMouth->GetDlgItem( "Slider_ThickDownLip");
	m_pSliderScaleMouthH = (AUISlider*)m_pWinChooseMouth->GetDlgItem( "Slider_ScaleMouthH");
	m_pSliderOffsetMouthV = (AUISlider*)m_pWinChooseMouth->GetDlgItem( "Slider_OffsetMouthV");
	m_pSliderOffsetMouthZ = (AUISlider*)m_pWinChooseMouth->GetDlgItem( "Slider_OffsetMouthZ");
	m_pSliderOffsetCornerOfMouthSpecial = (AUISlider*)m_pWinChooseMouth->GetDlgItem("Slider_OffsetCornerOfMouthSpecial");

	m_pSliderScaleMouthH2 = (AUISlider* )m_pWinChooseMouth->GetDlgItem("Slider_ScaleMouthH2");
	
	m_pSliderOffsetCornerOfMouthSpecial2 = (AUISlider* )m_pWinChooseMouth->GetDlgItem("Slider_OffsetCornerOfMouthSpecial2");

	m_pCheckBoxScaleMouthH = (AUICheckBox*)m_pWinChooseMouth->GetDlgItem("CheckBox_ScaleMouthH");
	m_pCheckBoxOffsetCornerOfMouthSpecial = (AUICheckBox*)m_pWinChooseMouth->GetDlgItem("CheckBox_OffsetCornerOfMouthSpecial");
	
	m_pCheckBoxScaleMouthH->Check(true);
	m_pCheckBoxOffsetCornerOfMouthSpecial->Check(true);
	

	m_pBtmChooseMouthUpLipLine = (AUIStillImageButton* )m_pWinChooseMouth->GetDlgItem("Btm_ChooseMouthUpLipLine");
	m_pBtmChooseMouthMidLipLine = (AUIStillImageButton* )m_pWinChooseMouth->GetDlgItem("Btm_ChooseMouthMidLipLine");
	m_pBtmChooseMouthDownLipLine = (AUIStillImageButton* )m_pWinChooseMouth->GetDlgItem("Btm_ChooseMouthDownLipLine");
	
	m_pBtmChooseMouthUpLipLine->SetPushLike(true);
	m_pBtmChooseMouthMidLipLine->SetPushLike(true);
	m_pBtmChooseMouthDownLipLine->SetPushLike(true);

	m_pBtmChooseMouthUpLipLine->SetPushed(true);

	m_pListBoxUpLipLine = (AUIListBox*)m_pWinChooseMouth->GetDlgItem("ListBox_UpLipLine");
	m_pListBoxUpLipLine->SetViewMode(AUILISTBOX_VIEW_ICON);

	m_pListBoxUpLipLine->Show(true);

	m_pListBoxMidLipLine = (AUIListBox*)m_pWinChooseMouth->GetDlgItem("ListBox_MidLipLine");
	m_pListBoxMidLipLine->SetViewMode(AUILISTBOX_VIEW_ICON);

	m_pListBoxMidLipLine->Show(false);

	m_pListBoxDownLipLine = (AUIListBox*)m_pWinChooseMouth->GetDlgItem("ListBox_DownLipLine");
	m_pListBoxDownLipLine->SetViewMode(AUILISTBOX_VIEW_ICON);

	m_pListBoxDownLipLine->Show(false);

	//-------------------初始化与耳型调整相关的控件------------------
	m_pWinChooseEar = m_pAUIManager->GetDialog("Win_ChooseEar");

	m_pBtmChooseBaseEar1 = (AUIStillImageButton*)m_pWinChooseEar->GetDlgItem("Btm_ChooseBaseEar1");
	m_pBtmChooseBaseEar1->SetPushLike(true);
	m_pBtmChooseBaseEar1->SetPushed(true);

	m_pListBoxEar = (AUIListBox*)m_pWinChooseEar->GetDlgItem("ListBox_Ear");//列表控件
	m_pListBoxEar->SetViewMode(AUILISTBOX_VIEW_ICON);

	m_pListBoxEar->Show(true);

	m_pSliderScaleEar = (AUISlider*)m_pWinChooseEar->GetDlgItem("Slider_ScaleEar"); //耳朵的整体缩放
	m_pSliderOffsetEarV = (AUISlider*)m_pWinChooseEar->GetDlgItem("Slider_OffsetEarV");//耳朵的上下移动


	//-------------------初始化与高级操作相关的控件
	m_pWinAdvancedOption = m_pAUIManager->GetDialog( "Win_AdvancedOption");

	m_pSliderOffsetForeheadH = (AUISlider*)m_pWinAdvancedOption->GetDlgItem("Slider_OffsetForeheadH");
	m_pSliderOffsetForeheadV = (AUISlider*)m_pWinAdvancedOption->GetDlgItem("Slider_OffsetForeheadV");
	m_pSliderOffsetForeheadZ = (AUISlider*)m_pWinAdvancedOption->GetDlgItem("Slider_OffsetForeheadZ");
	m_pSliderRotateForehead = (AUISlider*)m_pWinAdvancedOption->GetDlgItem("Slider_RotateForehead");
	m_pSliderScaleForehead = (AUISlider*)m_pWinAdvancedOption->GetDlgItem("Slider_ScaleForehead");
	
	m_pSliderOffsetYokeBoneH = (AUISlider*)m_pWinAdvancedOption->GetDlgItem("Slider_OffsetYokeBoneH");
	m_pSliderOffsetYokeBoneV = (AUISlider*)m_pWinAdvancedOption->GetDlgItem("Slider_OffsetYokeBoneV");
	m_pSliderOffsetYokeBoneZ = (AUISlider*)m_pWinAdvancedOption->GetDlgItem("Slider_OffsetYokeBoneZ");
	m_pSliderRotateYokeBone = (AUISlider*)m_pWinAdvancedOption->GetDlgItem("Slider_RotateYokeBone");
	m_pSliderScaleYokeBone = (AUISlider*)m_pWinAdvancedOption->GetDlgItem("Slider_ScaleYokeBone");
	
	m_pSliderOffsetCheekH = (AUISlider*)m_pWinAdvancedOption->GetDlgItem("Slider_OffsetCheekH");
	m_pSliderOffsetCheekV = (AUISlider*)m_pWinAdvancedOption->GetDlgItem("Slider_OffsetCheekV");
	m_pSliderOffsetCheekZ = (AUISlider*)m_pWinAdvancedOption->GetDlgItem("Slider_OffsetCheekZ");
	m_pSliderScaleCheek = (AUISlider*)m_pWinAdvancedOption->GetDlgItem("Slider_ScaleCheek");

	m_pSliderOffsetChainV = (AUISlider*)m_pWinAdvancedOption->GetDlgItem("Slider_OffsetChainV");
	m_pSliderOffsetChainZ = (AUISlider*)m_pWinAdvancedOption->GetDlgItem("Slider_OffsetChainZ");
	m_pSliderRotateChain = (AUISlider*)m_pWinAdvancedOption->GetDlgItem("Slider_RotateChain");
	m_pSliderScaleChainH = (AUISlider*)m_pWinAdvancedOption->GetDlgItem("Slider_ScaleChainH");

	m_pSliderOffsetJawH = (AUISlider*)m_pWinAdvancedOption->GetDlgItem("Slider_OffsetJawH");
	m_pSliderOffsetJawV = (AUISlider*)m_pWinAdvancedOption->GetDlgItem("Slider_OffsetJawV");
	m_pSliderOffsetJawZ = (AUISlider*)m_pWinAdvancedOption->GetDlgItem("Slider_OffsetJawZ");
	m_pSliderScaleJawSpecial = (AUISlider*)m_pWinAdvancedOption->GetDlgItem("Slider_ScaleJawSpecial");
	m_pSliderScaleJawH = (AUISlider*)m_pWinAdvancedOption->GetDlgItem("Slider_ScaleJawH");
	m_pSliderScaleJawV = (AUISlider*)m_pWinAdvancedOption->GetDlgItem("Slider_ScaleJawV");


	//------------------------初始化与化妆有关的AUI控件
	m_pWinChooseFacePainting = m_pAUIManager->GetDialog( "Win_ChooseFacePainting");

	m_pListBoxFaceParts = (AUIListBox*)m_pWinChooseFacePainting->GetDlgItem("ListBox_FaceParts");

	m_pImagePictureCurColor = (AUIImagePicture*) m_pWinChooseFacePainting->GetDlgItem ("ImagePicture_CurColor");

	m_pImagePictureColorPic = (AUIImagePicture*) m_pWinChooseFacePainting->GetDlgItem ("ImagePicture_ColorPic");

	m_pListBoxEyeKohl = (AUIListBox*)m_pWinChooseFacePainting->GetDlgItem( "ListBox_EyeKohl");
	m_pListBoxEyeKohl->SetViewMode(AUILISTBOX_VIEW_ICON);
	
	m_pPicColorPos = (AUIImagePicture*)m_pWinChooseFacePainting->GetDlgItem("Pic_ColorPos");
	
	m_pA3DSurfaceColorPic = NULL;

	//----------------------初始化与毛发相关的AUI控件
	m_pWinChooseHair = m_pAUIManager->GetDialog("Win_ChooseHair");

	//头发列表框
	m_pListBoxHair = (AUIListBox*)m_pWinChooseHair->GetDlgItem("ListBox_Hair");
	m_pListBoxHair->SetViewMode(AUILISTBOX_VIEW_ICON);


	//小胡子列表框
	m_pListBoxMoustache = (AUIListBox*)m_pWinChooseHair->GetDlgItem("ListBox_Moustache");
	m_pListBoxMoustache->SetViewMode(AUILISTBOX_VIEW_ICON);

	
	//大胡子列表控件
	m_pListBoxGoatee = (AUIListBox*)m_pWinChooseHair->GetDlgItem("ListBox_Goatee");
	m_pListBoxGoatee->SetViewMode(AUILISTBOX_VIEW_ICON);

	//头发纹理列表框
	m_pListBoxHairTex = (AUIListBox*)m_pWinChooseHair->GetDlgItem("ListBox_HairTex");
	m_pListBoxHairTex->SetViewMode(AUILISTBOX_VIEW_ICON);

	//大胡子纹理列表框
	m_pListBoxGoateeTex = (AUIListBox*)m_pWinChooseHair->GetDlgItem("ListBox_GoateeTex");
	m_pListBoxGoateeTex->SetViewMode(AUILISTBOX_VIEW_ICON);


	//-----------------------初始化与身体选择相关的AUI
	m_pWinChooseStature = m_pAUIManager->GetDialog("Win_ChooseStature");

	//头大小
	m_pSliderHeadScale = (AUISlider*)m_pWinChooseStature->GetDlgItem("Slider_HeadScale");

	//上身健壮度
	m_pSliderUpScale = (AUISlider*)m_pWinChooseStature->GetDlgItem("Slider_UpScale");

	//腰部围度
	m_pSliderWaistScale = (AUISlider*)m_pWinChooseStature->GetDlgItem("Slider_WaistScale");

	//上肢围度
	m_pSliderArmWidth = (AUISlider*)m_pWinChooseStature->GetDlgItem("Slider_ArmWidth");

	//下肢围度
	m_pSliderLegWidth = (AUISlider*)m_pWinChooseStature->GetDlgItem("Slider_LegWidth");

	//胸部高低
	m_pSliderBreastScale = (AUISlider*)m_pWinChooseStature->GetDlgItem("Slider_BreastScale");

	m_pLabelBreastScale = (AUILabel*)m_pWinChooseStature->GetDlgItem("Label_BreastScale");

	m_pStillImageButtomBreastScale = (AUIStillImageButton*)m_pWinChooseStature->GetDlgItem("StillImageButton_BreastScaleReset");


	
	if( m_pCurPlayer->GetGender() == 0)
	{
		m_pSliderBreastScale->Show(false);
		m_pLabelBreastScale->Show(false);
		m_pStillImageButtomBreastScale->Show(false);
	}
	else
	{
		m_pSliderBreastScale->Show(true);
		m_pLabelBreastScale->Show(true);
		m_pStillImageButtomBreastScale->Show(true);
	}


	//------------------------与预定义相关
	m_pWinChoosePreCustomize = m_pAUIManager->GetDialog("Win_ChoosePreCustomize");
	m_pListBoxPreCustomize = (AUIListBox*)m_pWinChoosePreCustomize->GetDlgItem("ListBox_PreCustomize");
	m_pListBoxUserPic = (AUIListBox*)m_pWinChoosePreCustomize->GetDlgItem("ListBox_UserPic");
	
	m_pWinChoosePreCustomize->SetCanMove(false);

	m_pBtnUserPic = (AUIStillImageButton*)m_pWinChoosePreCustomize->GetDlgItem("Btn_UserPic");
	m_pBtnUserPic->SetPushLike(true);
	
	m_pBtnDeleteCustomizeData = (AUIStillImageButton*) m_pWinChoosePreCustomize->GetDlgItem("Btn_DeleteCustomizeData");

	//暂时屏蔽表情测试
	m_pWinChoosePreCustomize->GetDlgItem("Btm_TestExpression")->Show(false);

	
	//-------------------------各自定义部位的切换按钮
	m_pBtmChoosePreCustomize = (AUIStillImageButton*)m_pWinChoosePreCustomize->GetDlgItem("Btm_ChoosePreCustomize");
	m_pBtmChooseFace = (AUIStillImageButton*)m_pWinChooseFace->GetDlgItem("Btm_ChooseFace");
	m_pBtmChooseEye = (AUIStillImageButton*)m_pWinChooseEye->GetDlgItem("Btm_ChooseEye");
	m_pBtmChooseBrow = (AUIStillImageButton*)m_pWinChooseEyebrow->GetDlgItem("Btm_ChooseBrow");
	m_pBtmChooseNose = (AUIStillImageButton*)m_pWinChooseNose->GetDlgItem("Btm_ChooseNose");
	m_pBtmChooseMouth = (AUIStillImageButton*)m_pWinChooseMouth->GetDlgItem("Btm_ChooseMouth");
	m_pBtmChooseEar = (AUIStillImageButton*)m_pWinChooseEar->GetDlgItem("Btm_ChooseEar");
	m_pBtmChooseHair = (AUIStillImageButton*)m_pWinChooseHair->GetDlgItem("Btm_ChooseHair");
	m_pBtmChooseFacePainting = (AUIStillImageButton*)m_pWinChooseFacePainting->GetDlgItem("Btm_ChooseFacePainting");
	m_pBtmChooseStature = (AUIStillImageButton*)m_pWinChooseStature->GetDlgItem("Btm_ChooseStature");

	m_pBtmChoosePreCustomize->SetPushLike(true);
	m_pBtmChooseFace ->SetPushLike(true);
	m_pBtmChooseEye->SetPushLike(true); 
	m_pBtmChooseBrow->SetPushLike(true); 
	m_pBtmChooseNose->SetPushLike(true); 
	m_pBtmChooseMouth->SetPushLike(true); 
	m_pBtmChooseEar->SetPushLike(true); 
	m_pBtmChooseHair->SetPushLike(true); 
	m_pBtmChooseFacePainting->SetPushLike(true); 
	m_pBtmChooseStature->SetPushLike(true); 

	
	
	//-------------------------------
	//初始化移动摄像机对话框
	
	m_pWinMoveCamera = m_pAUIManager->GetDialog("Win_MoveCamera");
	
	m_pBtnCameraMoveFront = (AUIStillImageButton*)m_pWinMoveCamera->GetDlgItem("Btn_CameraMoveFront");
	m_pBtnCameraMoveBack = (AUIStillImageButton*)m_pWinMoveCamera->GetDlgItem("Btn_CameraMoveBack");
	m_pBtnCameraMoveLeft = (AUIStillImageButton*)m_pWinMoveCamera->GetDlgItem("Btn_CameraMoveLeft");
	m_pBtnCameraMoveRight = (AUIStillImageButton*)m_pWinMoveCamera->GetDlgItem("Btn_CameraMoveRight");
	
	m_pBtnCameraTurnLeft = (AUIStillImageButton*)m_pWinMoveCamera->GetDlgItem("Btn_CameraTurnLeft");
	m_pBtnCameraTurnRight = (AUIStillImageButton*)m_pWinMoveCamera->GetDlgItem("Btn_CameraTurnRight");
	
	m_pBtnCameraMoveUp = (AUIStillImageButton*)m_pWinMoveCamera->GetDlgItem("Btn_CameraMoveUp");
	m_pBtnCameraMoveDown = (AUIStillImageButton*)m_pWinMoveCamera->GetDlgItem("Btn_CameraMoveDown");

	//---------------------------------
	//与用户参考图片相关
	m_pWinUserPic = m_pAUIManager->GetDialog("Win_UserPic");
	m_pWinUserPic->Show(false);

	m_pPicUserPic = (AUIImagePicture*)m_pWinUserPic->GetDlgItem("Pic_UserPic");
	m_pPicUserPic->SetAcceptMouseMessage(true);
	
	m_pBtnUserPicDrag = (AUIStillImageButton*)m_pWinUserPic->GetDlgItem("Btn_UserPicDrag");
	m_pBtnUserPicZoomIn = (AUIStillImageButton*)m_pWinUserPic->GetDlgItem("Btn_UserPicZoomIn");
	m_pBtnUserPicZoomOut = (AUIStillImageButton*)m_pWinUserPic->GetDlgItem("Btn_UserPicZoomOut");


	
	m_pSliderUserPicAlpha = (AUISlider*)m_pWinUserPic->GetDlgItem("Slider_UserPicAlpha");
	m_pLabelUserPicAlpha = (AUILabel*)m_pWinUserPic->GetDlgItem("Label_UserPicAlpha");
	m_pLabelUserPic = (AUILabel*)m_pWinUserPic->GetDlgItem("Label_UserPic");
	
	m_pSliderUserPicAlpha->SetLevel(255);

	m_nA2DSPriteUserPicItemPosX = 0;//图片item的位置
	m_nA2DSPriteUserPicItemPosY = 0;

	//设置滚动条范围
	SetAllsliderBound();
	
	//*************用于测试自定义表情***************
 	InitExpressionCtrler();

	return true;
}


//更新ChooseFace对话框滚动条状态
void CECCustomize::UpdateChooseFaceSlider(void)
{	
	//骨骼控制器
	CFaceBoneController* pBoneCtrler;
	pBoneCtrler = m_pCurFace->GetBoneController();
	pBoneCtrler->RestoreBoneTM ();//恢复初始单位骨骼矩阵
	
	CalculateBlendFace();//计算混合脸型

	CalculateFace();//计算脸盘

	Calculate3Parts();//计算3庭

	//合并backup骨骼矩阵
	pBoneCtrler->ApplyBonesOnBakFace(true);

}



//复位脸型
void CECCustomize::ResetChooseFace(void)
{
	//复位脸型混合调解

	m_pSliderPartUp->SetLevel(m_nDefaultScaleUp);
	m_pSliderPartMiddle->SetLevel(m_nDefaultScaleMiddle);
	m_pSliderPartDown->SetLevel(m_nDefaultScaleDown);

	m_pSliderScaleFaceH->SetLevel(m_nDefaultScaleFaceH);
	m_pSliderScaleFaceV->SetLevel(m_nDefaultScaleFaceV);

	
	//更新脸型滚动条
	UpdateChooseFaceSlider();

}






//测试2个AUI控件是否相交
bool CECCustomize::Test2CtrlIntersect( AUIObject* pCtrl1, AUIObject* pCtrl2 )
{
	POINT ptPos1 = pCtrl1->GetPos();
	POINT ptPos2 = pCtrl2->GetPos();
	SIZE sizeCtrl2 = pCtrl2->GetSize();

	if( ptPos1.x < (ptPos2.x - sizeCtrl2.cx) )
		return false;
	if( ptPos1.y < (ptPos2.y - sizeCtrl2.cy))
		return false;
	if( ptPos1.x > (ptPos2.x + sizeCtrl2.cx))
		return false;
	if( ptPos1.y > (ptPos2.y + sizeCtrl2.cy))
		return false;

	return true;	
}


//释放对话框中的控件中的数据
bool CECCustomize::ReleaseItem( void)
{
	//清空选中
	if( m_pImagePicCurFaceShape1 )
		m_pImagePicCurFaceShape1->SetColor(m_clrUnSel);

	if( m_pImagePicCurFaceShape2 )
		m_pImagePicCurFaceShape2->SetColor(m_clrUnSel);

	int i;
	for( i = 0; i < 5; i++)
	{
		if(m_pPicFace[i])
			m_pPicFace[i]->ClearCover();
	}

	//释放脸部颜色listBox
	if( m_pListBoxFaceParts)
		m_pListBoxFaceParts->ResetContent();
	

	//释放大胡子ListBox的字符串和数据
	if(m_pListBoxGoatee)
		m_pListBoxGoatee->ResetContent();
	

	//释放眼影list box
	if(m_pListBoxGoatee)
		m_pListBoxEyeKohl->ResetContent();


	//释放小胡子list box
	if(m_pListBoxMoustache)
		m_pListBoxMoustache->ResetContent();
	
	
	//释放头发list box
	if(m_pListBoxHair)
		m_pListBoxHair->ResetContent();
	

	//释放耳型list box
	if(m_pListBoxHair)
		m_pListBoxEar->ResetContent();


	//释放上唇线list box
	if(m_pListBoxUpLipLine)
		m_pListBoxUpLipLine->ResetContent();
	
	
	//释放唇沟
	if(m_pListBoxMidLipLine)
		m_pListBoxMidLipLine->ResetContent();
	

	//释放下唇线
	if(m_pListBoxDownLipLine)
		m_pListBoxDownLipLine->ResetContent();
	

	//释放鼻子纹理
	if(m_pListBoxNoseTex)
		m_pListBoxNoseTex->ResetContent();
	

	//释放鼻头
	if(m_pListBoxNoseTip)
		m_pListBoxNoseTip->ResetContent();
	

	//释放鼻梁
	if(m_pListBoxNoseBridge)
		m_pListBoxNoseBridge->ResetContent();
	

	//释放眉毛纹理list box
	if(m_pListBoxBrowTex)
		m_pListBoxBrowTex->ResetContent();
	

	//释放眉毛形状
	if(m_pListBoxBrow)
		m_pListBoxBrow->ResetContent();
	

	//释放眼皮纹理
	if(m_pListBoxEyeBaseTex1)
		m_pListBoxEyeBaseTex1->ResetContent();
	if(m_pListBoxEyeBaseTex2)
		m_pListBoxEyeBaseTex2->ResetContent();
	if(m_pListBoxEyeBaseTex3)
		m_pListBoxEyeBaseTex3->ResetContent();
	

	//释放眼型
	if(m_pListBoxEyeShape1)
		m_pListBoxEyeShape1->ResetContent();
	
	if(m_pListBoxEyeShape2)
		m_pListBoxEyeShape2->ResetContent();
	
	if(m_pListBoxEyeShape3)
		m_pListBoxEyeShape3->ResetContent();
	

	//释放预设
	if(m_pListBoxPreCustomize)
		m_pListBoxPreCustomize->ResetContent();
	

	//释放头发纹理列表
	if(m_pListBoxHairTex)
		m_pListBoxHairTex->ResetContent();
	

	//释放大胡子纹理
	if(m_pListBoxGoateeTex)
		m_pListBoxGoateeTex->ResetContent();

	//释放用户参考图片
	if(m_pListBoxUserPic)
		m_pListBoxUserPic->ResetContent();

	//
	if(m_pImagePictureColorPic)
		m_pImagePictureColorPic->ClearCover();

	if(m_pPicUserPic)
		m_pPicUserPic->ClearCover();

	//法令
	if(m_pListBoxFalingTex)
		m_pListBoxFalingTex->ResetContent();
	
	return true;
}

//更新ChooseEye对话框的滚动条状态
void CECCustomize::UpdateChooseEyeSlider(void)
{
	//骨骼控制器
	CFaceBoneController* pBoneCtrler;
	pBoneCtrler = m_pCurFace->GetBoneController();
	pBoneCtrler->RestoreBoneTM ();//恢复初始单位骨骼矩阵
	
	CalculateEye();//计算眼型
	Calculate3Parts();//计算3庭

	//合并backup骨骼矩阵
	pBoneCtrler->ApplyBonesOnBakFace(true);
}

//复位眼型
void CECCustomize::ResetChooseEye(void)
{
		m_nScaleEyeH = m_nDefaultScaleEyeH;
		m_nScaleEyeV = m_nDefaultScaleEyeV;
		m_nRotateEye = m_nDefaultRotateEye;
		m_nOffsetEyeH = m_nDefaultOffsetEyeH;
		m_nOffsetEyeV = m_nDefaultOffsetEyeV;
		m_nOffsetEyeZ = m_nDefaultOffsetEyeZ;
		m_nScaleEyeBall = m_nDefaultScaleEyeBall;

		m_nScaleEyeH2 = m_nDefaultScaleEyeH2;
		m_nScaleEyeV2 = m_nDefaultScaleEyeV2;
		m_nRotateEye2 = m_nDefaultRotateEye2;
		m_nOffsetEyeH2 = m_nDefaultOffsetEyeH2;
		m_nOffsetEyeV2 = m_nDefaultOffsetEyeV2;
		m_nOffsetEyeZ2 = m_nDefaultOffsetEyeZ2;
		m_nScaleEyeBall2 = m_nDefaultScaleEyeBall2;

	
	if( m_pBtnLeftEye->IsPushed())
	{
		m_pSliderScaleEyeH->SetLevel(m_nDefaultScaleEyeH);
		m_pSliderScaleEyeV->SetLevel(m_nDefaultScaleEyeV);
		m_pSliderRotateEye->SetLevel(m_nDefaultRotateEye);
		m_pSliderOffsetEyeH->SetLevel(m_nDefaultOffsetEyeH);
		m_pSliderOffsetEyeV->SetLevel(m_nDefaultOffsetEyeV);
		m_pSliderOffsetEyeZ->SetLevel(m_nDefaultOffsetEyeZ);
		m_pSliderScaleEyeBall->SetLevel(m_nDefaultScaleEyeBall);
	}
	else if (m_pBtnRightEye->IsPushed())
	{
		m_pSliderScaleEyeH->SetLevel(m_nDefaultScaleEyeH2);
		m_pSliderScaleEyeV->SetLevel(m_nDefaultScaleEyeV2);
		m_pSliderRotateEye->SetLevel(m_nDefaultRotateEye2);
		m_pSliderOffsetEyeH->SetLevel(m_nDefaultOffsetEyeH2);
		m_pSliderOffsetEyeV->SetLevel(m_nDefaultOffsetEyeV2);
		m_pSliderOffsetEyeZ->SetLevel(m_nDefaultOffsetEyeZ2);
		m_pSliderScaleEyeBall->SetLevel(m_nDefaultScaleEyeBall2);
	}
	
	//更新眼睛滚动条
	UpdateChooseEyeSlider();

}

//更新ChooseEyebrow对话框的滚动条状态
void CECCustomize::UpdateChooseBrowSlider(void)
{
	//骨骼控制器
	CFaceBoneController* pBoneCtrler;
	pBoneCtrler = m_pCurFace->GetBoneController();
	pBoneCtrler->RestoreBoneTM ();//恢复初始单位骨骼矩阵
	
	CalculateBrow();//计算眉型
	Calculate3Parts();//计算3庭

	//合并backup骨骼矩阵
	pBoneCtrler->ApplyBonesOnBakFace(true);
}

//复位眉型
void CECCustomize::ResetChooseBrow(void)
{

		m_nScaleBrowH = m_nDefaultScaleBrowH;
		m_nScaleBrowV = m_nDefaultScaleBrowV;
		m_nRotateBrow = m_nDefaultRotateBrow;
		m_nOffsetBrowH = m_nDefaultOffsetBrowH;
		m_nOffsetBrowV = m_nDefaultOffsetBrowV;
		m_nOffsetBrowZ = m_nDefaultOffsetBrowZ;
		
		
		m_nScaleBrowH2 = m_nDefaultScaleBrowH2;
		m_nScaleBrowV2 = m_nDefaultScaleBrowV2;
		m_nRotateBrow2 = m_nDefaultRotateBrow2;
		m_nOffsetBrowH2 = m_nDefaultOffsetBrowH2;
		m_nOffsetBrowV2 = m_nDefaultOffsetBrowV2;
		m_nOffsetBrowZ2 = m_nDefaultOffsetBrowZ2;
		
	
	if( m_pBtnLeftBrow->IsPushed())
	{
		m_pSliderScaleBrowH->SetLevel(m_nDefaultScaleBrowH);
		m_pSliderScaleBrowV->SetLevel(m_nDefaultScaleBrowV);
		m_pSliderRotateBrow->SetLevel(m_nDefaultRotateBrow);
		m_pSliderOffsetBrowH->SetLevel(m_nDefaultOffsetBrowH);
		m_pSliderOffsetBrowV->SetLevel(m_nDefaultOffsetBrowV);
		m_pSliderOffsetBrowZ->SetLevel(m_nDefaultOffsetBrowZ);
	}
	else if (m_pBtnRightBrow->IsPushed())
	{
		m_pSliderScaleBrowH->SetLevel(m_nDefaultScaleBrowH2);
		m_pSliderScaleBrowV->SetLevel(m_nDefaultScaleBrowV2);
		m_pSliderRotateBrow->SetLevel(m_nDefaultRotateBrow2);
		m_pSliderOffsetBrowH->SetLevel(m_nDefaultOffsetBrowH2);
		m_pSliderOffsetBrowV->SetLevel(m_nDefaultOffsetBrowV2);
		m_pSliderOffsetBrowZ->SetLevel(m_nDefaultOffsetBrowZ2);
	}

	//更新所有相关的
	UpdateChooseBrowSlider();
}

//更新ChooseNose对话框的滚动条状态
void CECCustomize::UpdateChooseNoseSlider(void)
{
	//骨骼控制器
	CFaceBoneController* pBoneCtrler;
	pBoneCtrler = m_pCurFace->GetBoneController();
	pBoneCtrler->RestoreBoneTM ();//恢复初始单位骨骼矩阵
	
	CalculateNose();//计算鼻型
	Calculate3Parts();//计算3庭

	//合并backup骨骼矩阵
	pBoneCtrler->ApplyBonesOnBakFace(true);
	
}

//复位鼻型
void CECCustomize::ResetChooseNose(void)
{

	m_pSliderScaleNoseTipH->SetLevel(m_nDefaultScaleNoseTipH);
	m_pSliderScaleNoseTipV->SetLevel(m_nDefaultScaleNoseTipV);
	m_pSliderScaleNoseTipZ->SetLevel(m_nDefaultScaleNoseTipZ);
	m_pSliderOffsetNoseTipV->SetLevel(m_nDefaultOffsetNoseTipV);
	m_pSliderScaleBridgeTipH->SetLevel(m_nDefaultScaleBridgeTipH);
	m_pSliderOffsetBridgeTipZ->SetLevel(m_nDefaultOffsetBridegTipZ);


	//更新鼻型滚动条
	UpdateChooseNoseSlider();
}

//更新ChooseMouth对话框的滚动条状态
void CECCustomize::UpdateChooseMouthSlider(void)
{
	//骨骼控制器
	CFaceBoneController* pBoneCtrler;
	pBoneCtrler = m_pCurFace->GetBoneController();
	pBoneCtrler->RestoreBoneTM ();//恢复初始单位骨骼矩阵
	
	CalculateMouth();//计算嘴型型
	Calculate3Parts();//计算3庭

	//合并backup骨骼矩阵
	pBoneCtrler->ApplyBonesOnBakFace(true);
}

//复位嘴型
void CECCustomize::ResetChooseMouth(void)
{


	m_pSliderThickUpLip->SetLevel(m_nDefaultThickUpLip);
	m_pSliderThickDownLip->SetLevel(m_nDefaultThickDownLip);
	m_pSliderScaleMouthH->SetLevel(m_nDefaultScaleMouthH);
	m_pSliderScaleMouthH2->SetLevel(m_nDefaultScaleMouthH2);
	m_pSliderOffsetMouthV->SetLevel(m_nDefaultOffsetMouthV);
	m_pSliderOffsetMouthZ->SetLevel(m_nDefaultOffsetMouthZ);
	m_pSliderOffsetCornerOfMouthSpecial->SetLevel(m_nDefaultOffsetCornerOfMouthSpecial);
	m_pSliderOffsetCornerOfMouthSpecial2->SetLevel(m_nDefaultOffsetCornerOfMouthSpecial2);
	
	//更新嘴的滚动条
	UpdateChooseMouthSlider();
}

//更新ChooseEar对话框的滚动条状态
void CECCustomize::UpdateChooseEarSlider(void)
{
	//骨骼控制器
	CFaceBoneController* pBoneCtrler;
	pBoneCtrler = m_pCurFace->GetBoneController();
	pBoneCtrler->RestoreBoneTM ();//恢复初始单位骨骼矩阵
	
	CalculateEar();//计算耳型
	Calculate3Parts();//计算3庭

	//合并backup骨骼矩阵
	pBoneCtrler->ApplyBonesOnBakFace(true);
}

//复位耳型
void CECCustomize::ResetChooseEar(void)
{

	m_pSliderScaleEar->SetLevel(m_nDefaultScaleEar);
	m_pSliderOffsetEarV->SetLevel(m_nDefaultOffsetEarV);

	UpdateChooseEarSlider();
	
}


//复位额头
void CECCustomize::ResetForehead(void)
{

	m_pSliderOffsetForeheadH->SetLevel(m_nDefaultOffsetForeheadH);
	m_pSliderOffsetForeheadV->SetLevel(m_nDefaultOffsetForeheadV);
	m_pSliderOffsetForeheadZ->SetLevel(m_nDefaultOffsetForeheadZ);
	m_pSliderRotateForehead->SetLevel(m_nDefaultRotateForehead);
	m_pSliderScaleForehead->SetLevel(m_nDefaultScaleForehead);
	
	//更新高级选项滚动条
	UpdateAdvancedOptionSlider();
}

//复位颧骨
void CECCustomize::ResetYokeBone(void)
{
	m_pSliderOffsetYokeBoneH->SetLevel(m_nDefaultOffsetYokeBoneH);
	m_pSliderOffsetYokeBoneV->SetLevel(m_nDefaultOffsetYokeBoneV);
	m_pSliderOffsetYokeBoneZ->SetLevel(m_nDefaultOffsetYokeBoneZ);
	m_pSliderRotateYokeBone->SetLevel(m_nDefaultRotateYokeBone);
	m_pSliderScaleYokeBone->SetLevel(m_nDefaultScaleYokeBone);

	//更新高级选项滚动条
	UpdateAdvancedOptionSlider();

	
}

//复位脸颊
void CECCustomize::ResetCheek(void)
{
	m_pSliderOffsetCheekH->SetLevel(m_nDefaultOffsetCheekH);
	m_pSliderOffsetCheekV->SetLevel(m_nDefaultOffsetCheekV);
	m_pSliderOffsetCheekZ->SetLevel(m_nDefaultOffsetCheekZ);
	m_pSliderScaleCheek->SetLevel(m_nDefaultScaleCheek);
	

	//更新高级选项滚动条
	UpdateAdvancedOptionSlider();
}

//复位下巴
void CECCustomize::ResetChain(void)
{
	
	m_pSliderOffsetChainV->SetLevel(m_nDefaultOffsetChainV);
	m_pSliderOffsetChainZ->SetLevel(m_nDefaultOffsetChainZ);
	m_pSliderRotateChain->SetLevel(m_nDefaultRotateChain);
	m_pSliderScaleChainH->SetLevel(m_nDefaultScaleChainH);
	
	//更新高级选项滚动条
	UpdateAdvancedOptionSlider();
}

//复位颌骨
void CECCustomize::ResetJaw(void)
{

	m_pSliderOffsetJawH->SetLevel(m_nDefaultOffsetJawH);
	m_pSliderOffsetJawV->SetLevel(m_nDefaultOffsetJawV);
	m_pSliderOffsetJawZ->SetLevel(m_nDefaultOffsetJawZ);
	m_pSliderScaleJawSpecial->SetLevel(m_nDefaultScaleJawSpecial);
	m_pSliderScaleJawH->SetLevel(m_nDefaultScaleJawH);
	m_pSliderScaleJawV->SetLevel(m_nDefaultScaleJawV);

	//更新高级选项滚动条
	UpdateAdvancedOptionSlider();
}

//更新高级选项对话框滚动条控件
void CECCustomize::UpdateAdvancedOptionSlider(void)
{
	//骨骼控制器
	CFaceBoneController* pBoneCtrler;
	pBoneCtrler = m_pCurFace->GetBoneController();
	pBoneCtrler->RestoreBoneTM ();//恢复初始单位骨骼矩阵
	
	CalculateAdvaceOption();//计算高级选项
	Calculate3Parts();//计算3庭

	//合并backup骨骼矩阵
	pBoneCtrler->ApplyBonesOnBakFace(true);
	
}

//复位鼻头
void CECCustomize::ResetNoseTip()
{
	m_pSliderScaleNoseTipH->SetLevel(128);
	m_pSliderScaleNoseTipV->SetLevel(128);
	m_pSliderScaleNoseTipZ->SetLevel(128); 
	m_pSliderOffsetNoseTipV->SetLevel(128); 

	//更新鼻型滚动条
	UpdateChooseNoseSlider();
}

//复位鼻梁
void CECCustomize::ResetNoseBridge()
{
	m_pSliderScaleBridgeTipH->SetLevel(128);
	m_pSliderOffsetBridgeTipZ->SetLevel(128);

	//更新鼻型滚动条
	UpdateChooseNoseSlider();
}

//复位上嘴唇
void CECCustomize::ResetUpLip(void)
{
	m_pSliderThickUpLip->SetLevel(128);
	m_pSliderScaleMouthH->SetLevel (128);
	m_pSliderScaleMouthH2->SetLevel (128);
	m_pSliderOffsetMouthV->SetLevel(128);
	m_pSliderOffsetMouthZ->SetLevel(128);
	
	m_pSliderOffsetCornerOfMouthSpecial->SetLevel(128);
	m_pSliderOffsetCornerOfMouthSpecial2->SetLevel(128);

	//更新嘴的滚动条
	UpdateChooseMouthSlider();
}

//复位唇线
void CECCustomize::ResetMidLip(void)
{
	
	m_pSliderScaleMouthH->SetLevel (128);
	m_pSliderScaleMouthH2->SetLevel (128);
	m_pSliderOffsetMouthV->SetLevel(128);
	m_pSliderOffsetMouthZ->SetLevel(128);

	m_pSliderOffsetCornerOfMouthSpecial->SetLevel(128);
	m_pSliderOffsetCornerOfMouthSpecial2->SetLevel(128);

	//更新嘴的滚动条
	UpdateChooseMouthSlider();
}

//复位下嘴唇
void CECCustomize::ResetDownLip(void)
{
	m_pSliderThickDownLip->SetLevel (128);
	m_pSliderScaleMouthH->SetLevel (128);
	m_pSliderScaleMouthH2->SetLevel(128);
	m_pSliderOffsetMouthV->SetLevel(128);
	m_pSliderOffsetMouthZ->SetLevel(128);

	m_pSliderOffsetCornerOfMouthSpecial->SetLevel(128);
	m_pSliderOffsetCornerOfMouthSpecial2->SetLevel(128);

	//更新嘴的滚动条
	UpdateChooseMouthSlider();
}

//更新3庭
bool CECCustomize::Calculate3Parts(void)
{
	//获取当前滚动条的值
	int nPartUp = m_pSliderPartUp->GetLevel();
	int nPartMiddle = m_pSliderPartMiddle->GetLevel();
	int nPartDown = m_pSliderPartDown->GetLevel();
		

	
	//**获取FaceData指针
	CECFace::FACE_CUSTOMIZEDATA* pFaceData = m_pCurFace->GetFaceData();
	//---------设置与3庭相关的参数3--------------
	
	//**设置3庭比例

	pFaceData->scaleUp = nPartUp;
	pFaceData->scaleMiddle = nPartMiddle;
	pFaceData->scaleDown = nPartDown;
	
	//**计算3庭
	m_pCurFace->Calculate3Parts();
	
	return true;
}

/*
//载入脸型相关的调节系数文件
bool CECCustomize::LoadFaceShapeFactor(const char* szFile)
{
	AFile fileCFG;
	if( !fileCFG.Open(szFile, AFILE_TEXT|AFILE_OPENEXIST))
		return false;

	//AString strLine;
	//AString strFactor;
	char szLine[256];
	char szFactor[256];
	unsigned long nLineLength;
	int nCount;
	
	if(!fileCFG.ReadLine(szLine, 256, &nLineLength))
		return false;

	sscanf(szLine, "%d", &nCount);//获取形状因子的个数

	for( int i = 0; i< nCount; i++)
	{
		fileCFG.ReadLine(szLine, 256, &nLineLength);
		fileCFG.ReadLine(szFactor, 256, &nLineLength);
		
		if(strstr(szLine,"ScaleFaceHFactor"))
		{
			sscanf(szFactor, "%f", &m_fScaleFaceHFactor);
		}
		else if(strstr(szLine,"ScaleFaceVFactor"))
		{
			sscanf(szFactor, "%f", &m_fScaleFaceVFactor);
		}
		else if(strstr(szLine,"OffsetForeheadHFactor"))
		{
			sscanf(szFactor, "%f", &m_fOffsetForeheadHFactor);
		}
		else if(strstr(szLine,"OffsetForeheadVFactor"))
		{
			sscanf(szFactor, "%f", &m_fOffsetForeheadVFactor);
		}
		else if(strstr(szLine,"OffsetForeheadZFactor"))
		{
			sscanf(szFactor, "%f", &m_fOffsetForeheadZFactor);
		}
		else if(strstr(szLine,"RotateForeheadFactor"))
		{
			sscanf(szFactor, "%f", &m_fRotateForeheadFactor);
		}
		else if(strstr(szLine,"ScaleForeheadFactor"))
		{
			sscanf(szFactor, "%f", &m_fScaleForeheadFactor);
		}
		else if(strstr(szLine,"OffsetYokeBoneHFactor"))
		{
			sscanf(szFactor, "%f", &m_fOffsetYokeBoneHFactor);
		}
		else if(strstr(szLine,"OffsetYokeBoneVFactor"))
		{
			sscanf(szFactor, "%f", &m_fOffsetYokeBoneVFactor);
		}
		else if(strstr(szLine,"OffsetYokeBoneZFactor"))
		{
			sscanf(szFactor, "%f", &m_fOffsetYokeBoneZFactor);
		}
		else if(strstr(szLine,"RotateYokeBoneFactor"))
		{
			sscanf(szFactor, "%f", &m_fRotateYokeBoneFactor);
		}
		else if(strstr(szLine,"ScaleYokeBoneFactor"))
		{
			sscanf(szFactor, "%f", &m_fScaleYokeBoneFactor);
		}
		else if(strstr(szLine,"OffsetCheekHFactor"))
		{
			sscanf(szFactor, "%f", &m_fOffsetCheekHFactor);
		}
		else if(strstr(szLine,"OffsetCheekVFactor"))
		{
			sscanf(szFactor, "%f", &m_fOffsetCheekVFactor);
		}
		else if(strstr(szLine,"OffsetCheekZFactor"))
		{
			sscanf(szFactor, "%f", &m_fOffsetCheekZFactor);
		}
		else if(strstr(szLine,"ScaleCheekFactor"))
		{
			sscanf(szFactor, "%f", &m_fScaleCheekFactor);
		}
		else if(strstr(szLine,"OffsetChainVFactor"))
		{
			sscanf(szFactor, "%f", &m_fOffsetChainVFactor);
		}
		else if(strstr(szLine,"OffsetChainZFactor"))
		{
			sscanf(szFactor, "%f", &m_fOffsetChainZFactor);
		}
		else if(strstr(szLine,"RotateChainFactor"))
		{
			sscanf(szFactor, "%f", &m_fRotateChainFactor);
		}
		else if(strstr(szLine,"ScaleChainHFactor"))
		{
			sscanf(szFactor, "%f", &m_fScaleChainHFactor);
		}
		else if(strstr(szLine,"OffsetJawHFactor"))
		{
			sscanf(szFactor, "%f", &m_fOffsetJawHFactor);
		}
		else if(strstr(szLine,"OffsetJawVFactor"))
		{
			sscanf(szFactor, "%f", &m_fOffsetJawVFactor);
		}
		else if(strstr(szLine,"OffsetJawZFactor"))
		{
			sscanf(szFactor, "%f", &m_fOffsetJawZFactor);
		}
		else if(strstr(szLine,"ScaleJawSpecialFactor"))
		{
			sscanf(szFactor, "%f", &m_fScaleJawSpecialFactor);
		}
		else if(strstr(szLine,"ScaleJawHFactor"))
		{
			sscanf(szFactor, "%f", &m_fScaleJawHFactor);
		}
		else if(strstr(szLine,"ScaleJawVFactor"))
		{
			sscanf(szFactor, "%f", &m_fScaleJawVFactor);
		}
		else if(strstr(szLine,"ScaleEyeHFactor"))
		{
			sscanf(szFactor, "%f", &m_fScaleEyeHFactor);
		}
		else if(strstr(szLine,"ScaleEyeVFactor"))
		{
			sscanf(szFactor, "%f", &m_fScaleEyeVFactor);
		}
		else if(strstr(szLine,"RotateEyeFactor"))
		{
			sscanf(szFactor, "%f", &m_fRotateEyeFactor);
		}
		else if(strstr(szLine,"OffsetEyeHFactor"))
		{
			sscanf(szFactor, "%f", &m_fOffsetEyeHFactor);
		}
		else if(strstr(szLine,"OffsetEyeVFactor"))
		{
			sscanf(szFactor, "%f", &m_fOffsetEyeVFactor);
		}
		else if(strstr(szLine,"OffsetEyeZFactor"))
		{
			sscanf(szFactor, "%f", &m_fOffsetEyeZFactor);
		}
		else if(strstr(szLine,"ScaleEyeBallFactor"))
		{
			sscanf(szFactor, "%f", &m_fScaleEyeBallFactor);
		}
		else if(strstr(szLine,"ScaleBrowHFactor"))
		{
			sscanf(szFactor, "%f", &m_fScaleBrowHFactor);
		}
		else if(strstr(szLine,"ScaleBrowVFactor"))
		{
			sscanf(szFactor, "%f", &m_fScaleBrowVFactor);
		}
		else if(strstr(szLine,"RotateBrowFactor"))
		{
			sscanf(szFactor, "%f", &m_fRotateBrowFactor);
		}
		else if(strstr(szLine,"OffsetBrowHFactor"))
		{
			sscanf(szFactor, "%f", &m_fOffsetBrowHFactor);
		}
		else if(strstr(szLine,"OffsetBrowVFactor"))
		{
			sscanf(szFactor, "%f", &m_fOffsetBrowVFactor);
		}
		else if(strstr(szLine,"OffsetBrowZFactor"))
		{
			sscanf(szFactor, "%f", &m_fOffsetBrowZFactor);
		}
		else if(strstr(szLine,"ScaleNoseTipHFactor"))
		{
			sscanf(szFactor, "%f", &m_fScaleNoseTipHFactor);
		}
		else if(strstr(szLine,"ScaleNoseTipVFactor"))
		{
			sscanf(szFactor, "%f", &m_fScaleNoseTipVFactor);
		}
		else if(strstr(szLine,"OffsetNoseTipVFactor"))
		{
			sscanf(szFactor, "%f", &m_fOffsetNoseTipVFactor);
		}
		else if(strstr(szLine,"ScaleNoseTipZFactor"))
		{
			sscanf(szFactor, "%f", &m_fScaleNoseTipZFactor);
		}
		else if(strstr(szLine,"ScaleBridgeTipHFactor"))
		{
			sscanf(szFactor, "%f", &m_fScaleBridgeTipHFactor);
		}
		else if(strstr(szLine,"ThickUpLipFactor"))
		{
			sscanf(szFactor, "%f", &m_fThickUpLipFactor);
		}
		else if(strstr(szLine,"ThickDownLipFactor"))
		{
			sscanf(szFactor, "%f", &m_fThickDownLipFactor);
		}
		else if(strstr(szLine,"ScaleMouthHFactor"))
		{
			sscanf(szFactor, "%f", &m_fScaleMouthHFactor);
		}
		else if(strstr(szLine,"OffsetMouthVFactor"))
		{
			sscanf(szFactor, "%f", &m_fOffsetMouthVFactor);
		}
		else if(strstr(szLine,"OffsetMouthZFactor"))
		{
			sscanf(szFactor, "%f", &m_fOffsetMouthZFactor);
		}
		else if(strstr(szLine,"ScaleEarFactor"))
		{
			sscanf(szFactor, "%f", &m_fScaleEarFactor);
		}
		else if(strstr(szLine,"OffsetEarVFactor"))
		{
			sscanf(szFactor, "%f", &m_fOffsetEarVFactor);
		}
	}

	fileCFG.Close ();
	
	return true;
}
*/

//bool CECCustomize::DrawChooseColorPic()
//{
//	
//	if(m_pWinChooseFacePainting->IsShow() )
//	{	
//		POINT DlgPos = m_pWinChooseFacePainting->GetPos();
//	//	if( !m_pA2DSpriteColorPic->DrawToBack ( DlgPos.x + 20 ,DlgPos.y + 20 ))
//		if( !m_pA2DSpriteColorPic->DrawToBack ( 0 ,0 ))
//		{
//			#ifdef _DEBUG
//				_asm int 3 
//			#endif
//
//			return false;
//		}
//	}
//	
//	return true;
//}





//计算脸型
bool CECCustomize::CalculateBlendFace(void)
{



	//**获取FaceData指针
	CECFace::FACE_CUSTOMIZEDATA* pFaceData = m_pCurFace->GetFaceData();
	//---------设置与脸型融合相关的参数--------------

	//**计算脸型混合
	m_pCurFace->CalculateBlendFace();
	
	return true;
}

//计算脸盘缩放
bool CECCustomize::CalculateFace(void)
{
	//调整脸盘缩放比例
	int nScaleFaceH = m_pSliderScaleFaceH->GetLevel();
	int nScaleFaceV = m_pSliderScaleFaceV->GetLevel ();

	//**获取FaceData指针
	CECFace::FACE_CUSTOMIZEDATA* pFaceData = m_pCurFace->GetFaceData();
	
	//---------设置与脸盘相关的参数2--------------
	pFaceData->scaleFaceH = nScaleFaceH;
	pFaceData->scaleFaceV = nScaleFaceV;

	//**计算脸型
	m_pCurFace->CalculateFace();
	
	return true;
}

//计算眼型
bool CECCustomize::CalculateEye(void)
{
	//**获取FaceData指针
	CECFace::FACE_CUSTOMIZEDATA* pFaceData = m_pCurFace->GetFaceData();
	
	//左眼-----------------------------------------

	//横纵缩放调节
	int nScaleEyeH = m_nScaleEyeH;
	int nScaleEyeV = m_nScaleEyeV;
	
	//旋转调节
	int nRotateEye = m_nRotateEye;


	//平移调解
	int nOffsetEyeH = m_nOffsetEyeH;
	int nOffsetEyeV = m_nOffsetEyeV;
	int nOffsetEyeZ = m_nOffsetEyeZ;


	//眼球的缩放
	int nScaleEyeBall = m_nScaleEyeBall;	

	//------------设置左眼相关参数7---------------------------------

	//**设置眼型缩放比例参数
	pFaceData->scaleEyeH = nScaleEyeH;
	pFaceData->scaleEyeV = nScaleEyeV;
	//**设置眼型旋转
	pFaceData->rotateEye = nRotateEye;
	//**设置眼型的位移
	pFaceData->offsetEyeH = nOffsetEyeH;
	pFaceData->offsetEyeV = nOffsetEyeV;
	pFaceData->offsetEyeZ = nOffsetEyeZ;
	//**设置眼球的缩放
	pFaceData->scaleEyeBall= nScaleEyeBall;

	//左眼-----------------------------------------------------------------
		
	//横纵缩放调节
	int nScaleEyeH2 = m_nScaleEyeH2;
	int nScaleEyeV2 = m_nScaleEyeV2;

	
	//旋转调节
	int nRotateEye2 = m_nRotateEye2;


	//平移调解
	int nOffsetEyeH2 = m_nOffsetEyeH2;
	int nOffsetEyeV2 = m_nOffsetEyeV2;
	int nOffsetEyeZ2 = m_nOffsetEyeZ2;


	//眼球的缩放
	int nScaleEyeBall2 = m_nScaleEyeBall2;	

	//------------设置右眼相关参数7---------------------------------

	//**设置眼型缩放比例参数
	pFaceData->scaleEyeH2 = nScaleEyeH2;
	pFaceData->scaleEyeV2 = nScaleEyeV2;
	//**设置眼型旋转
	pFaceData->rotateEye2 = nRotateEye2;
	//**设置眼型的位移
	pFaceData->offsetEyeH2 = nOffsetEyeH2;
	pFaceData->offsetEyeV2 = nOffsetEyeV2;
	pFaceData->offsetEyeZ2 = nOffsetEyeZ2;
	//**设置眼球的缩放
	pFaceData->scaleEyeBall2= nScaleEyeBall2;


	//**计算眼型
	m_pCurFace->CalculateEye();
	
	return true;
}

//计算眉型
bool CECCustomize::CalculateBrow(void)
{

	//左眉------------------------------------------------------
	//横纵缩放调节
	int nScaleBrowH = m_nScaleBrowH;
	int nScaleBrowV = m_nScaleBrowV;


	//旋转调整
	int nRotateBrow = m_nRotateBrow;

	//平移调整
	int nOffsetBrowH =	m_nOffsetBrowH;
	int nOffsetBrowV =	m_nOffsetBrowV;
	int nOffsetBrowZ =	m_nOffsetBrowZ;

	//右眉------------------------------------------------------
	//横纵缩放调节
	int nScaleBrowH2 = m_nScaleBrowH2;
	int nScaleBrowV2 = m_nScaleBrowV2;


	//旋转调整
	int nRotateBrow2 = m_nRotateBrow2;

	//平移调整
	int nOffsetBrowH2 =	m_nOffsetBrowH2;
	int nOffsetBrowV2 =	m_nOffsetBrowV2;
	int nOffsetBrowZ2 =	m_nOffsetBrowZ2;


	//**获取FaceData指针
	CECFace::FACE_CUSTOMIZEDATA* pFaceData = m_pCurFace->GetFaceData();

	//------------设置眉型相关参数7---------------------------------


	//左眉----------------------------------------------------------
	//**设置眉型缩放
	pFaceData->scaleBrowH = nScaleBrowH;
	pFaceData->scaleBrowV = nScaleBrowV;
	//**设置眉型旋转
	pFaceData->rotateBrow = nRotateBrow;
	//**设置眉型平移
	pFaceData->offsetBrowH = nOffsetBrowH;
	pFaceData->offsetBrowV = nOffsetBrowV;
	pFaceData->offsetBrowZ = nOffsetBrowZ;

	//右眉----------------------------------------------------------

	//**设置眉型缩放
	pFaceData->scaleBrowH2 = nScaleBrowH2;
	pFaceData->scaleBrowV2 = nScaleBrowV2;
	//**设置眉型旋转
	pFaceData->rotateBrow2 = nRotateBrow2;
	//**设置眉型平移
	pFaceData->offsetBrowH2 = nOffsetBrowH2;
	pFaceData->offsetBrowV2 = nOffsetBrowV2;
	pFaceData->offsetBrowZ2 = nOffsetBrowZ2;
	
	//**计算眉型
	m_pCurFace->CalculateBrow();
	
	return true;
}

//计算鼻型
bool CECCustomize::CalculateNose(void)
{
	//鼻头衡纵缩放调节
	int nScaleNoseTipH = m_pSliderScaleNoseTipH->GetLevel();
	int nScaleNoseTipV = m_pSliderScaleNoseTipV->GetLevel();
	int nScaleNoseTipZ = m_pSliderScaleNoseTipZ->GetLevel();

	

	//鼻头的垂直位移
	int nOffsetNoseTipV = m_pSliderOffsetNoseTipV->GetLevel();

	//鼻梁水平比例调节
	int nScaleBridgeTipH = m_pSliderScaleBridgeTipH->GetLevel();

	//鼻梁前后位移调解
	int nOffsetBridgeTipZ = m_pSliderOffsetBridgeTipZ->GetLevel();
		
	//**获取FaceData指针
	CECFace::FACE_CUSTOMIZEDATA* pFaceData = m_pCurFace->GetFaceData();

	//---------设置与鼻型相关的参数7--------------

	//**设置鼻头缩放
	pFaceData->scaleNoseTipH = nScaleNoseTipH;
	pFaceData->scaleNoseTipV = nScaleNoseTipV;
	pFaceData->scaleNoseTipZ = nScaleNoseTipZ;
	//**设置鼻头位移
	pFaceData->offsetNoseTipV = nOffsetNoseTipV;
	//**设置鼻梁缩放
	pFaceData->scaleBridgeTipH = nScaleBridgeTipH;

	//**设置鼻梁前后位移
	pFaceData->offsetBridgeTipZ = nOffsetBridgeTipZ;

	//**计算鼻型
	m_pCurFace->CalculateNose();

	return true;
}

//计算嘴型
bool CECCustomize::CalculateMouth(void)
{

	//上嘴唇的厚度
	int nThickUpLip = m_pSliderThickUpLip->GetLevel();

	//下嘴唇的厚度
	int nThickDownLip = m_pSliderThickDownLip->GetLevel();


	//嘴部整体水平比例
	int nScaleMouthH = m_pSliderScaleMouthH->GetLevel();

	int nScaleMouthH2 = m_pSliderScaleMouthH2->GetLevel();


	//嘴部位移
	int nOffsetMouthV = m_pSliderOffsetMouthV->GetLevel();

	int nOffsetMouthZ = m_pSliderOffsetMouthZ->GetLevel();

	//嘴角特殊位移
	int nOffsetCornerOfMouthSpecial = m_pSliderOffsetCornerOfMouthSpecial->GetLevel();
	
	int nOffsetCornerOfMouthSpecial2 = m_pSliderOffsetCornerOfMouthSpecial2->GetLevel();

	//**获取FaceData指针
	CECFace::FACE_CUSTOMIZEDATA* pFaceData = m_pCurFace->GetFaceData();
	//---------设置与嘴型相关的参数8--------------

	//**设置嘴型比例
	pFaceData->thickUpLip = nThickUpLip;
	pFaceData->thickDownLip = nThickDownLip;
	pFaceData->scaleMouthH = nScaleMouthH;
	pFaceData->scaleMouthH2 = nScaleMouthH2;

	//**设置位移
	pFaceData->offsetMouthV = nOffsetMouthV;
	pFaceData->offsetMouthZ = nOffsetMouthZ;

	//嘴角的特殊位移
	pFaceData->offsetCornerOfMouthSpecial = nOffsetCornerOfMouthSpecial;
	pFaceData->offsetCornerOfMouthSpecial2 = nOffsetCornerOfMouthSpecial2;
	
	//**计算嘴型
	m_pCurFace->CalculateMouth();

	return true;
}

//计算耳型
bool CECCustomize::CalculateEar(void)
{
	int nScaleEar = m_pSliderScaleEar->GetLevel();

	int nOffsetEarV = m_pSliderOffsetEarV->GetLevel();


	//**获取FaceData指针
	CECFace::FACE_CUSTOMIZEDATA* pFaceData = m_pCurFace->GetFaceData();
	//---------设置与耳型相关的参数3--------------

	//**设置耳型缩放
	pFaceData->scaleEar = nScaleEar;
	//**设置耳型位移
	pFaceData->offsetEarV = nOffsetEarV;
	
	//**计算耳型
	m_pCurFace->CalculateEar();
	
	return true;
}

//计算额头
bool CECCustomize::CalculateForehead(void)
{
	//获取位移
	int nOffsetForeheadH = m_pSliderOffsetForeheadH->GetLevel();
	int nOffsetForeheadV = m_pSliderOffsetForeheadV->GetLevel();
	int nOffsetForeheadZ = m_pSliderOffsetForeheadZ->GetLevel();



	//获取旋转
	int nRotateForehead = m_pSliderRotateForehead->GetLevel();

	//获取比例
	int nScaleForehead = m_pSliderScaleForehead->GetLevel();

	//**获取FaceData指针
	CECFace::FACE_CUSTOMIZEDATA* pFaceData = m_pCurFace->GetFaceData();
	//---------设置与额头相关的参数5--------------

	//**设置额头的平移
	pFaceData->offsetForeheadH = nOffsetForeheadH;
	pFaceData->offsetForeheadV = nOffsetForeheadV;
	pFaceData->offsetForeheadZ = nOffsetForeheadZ;
	//**设置额头的旋转
	pFaceData->rotateForehead = nRotateForehead;
	//**设置额头的比例
	pFaceData->scaleForehead = nScaleForehead;
	
	//**计算额头
	m_pCurFace->CalculateForehead();

	return true;
}

//计算颧骨
bool CECCustomize::CalculateYokeBone(void)
{

	//获取平移
	int nOffsetYokeBoneH = m_pSliderOffsetYokeBoneH->GetLevel();
	int nOffsetYokeBoneV = m_pSliderOffsetYokeBoneV->GetLevel();
	int nOffsetYokeBoneZ = m_pSliderOffsetYokeBoneZ->GetLevel();

	//获取旋转
	int nRotateYokeBone = m_pSliderRotateYokeBone->GetLevel();


	//获取比例
	int nScaleYokeBone = m_pSliderScaleYokeBone->GetLevel();
	
	//**获取FaceData指针
	CECFace::FACE_CUSTOMIZEDATA* pFaceData = m_pCurFace->GetFaceData();
	//---------设置与颧骨相关的参数5--------------

	//**设置颧骨位移
	pFaceData->offsetYokeBoneH = nOffsetYokeBoneH;
	pFaceData->offsetYokeBoneV = nOffsetYokeBoneV;
	pFaceData->offsetYokeBoneZ = nOffsetYokeBoneZ;
	//**设置颧骨旋转
	pFaceData->rotateYokeBone = nRotateYokeBone;
	//**设置颧骨比例
	pFaceData->scaleYokeBone = nScaleYokeBone;

	//**计算颧骨
	m_pCurFace->CalculateYokeBone();

	return true;
}

//计算脸颊
bool CECCustomize::CalculateCheek(void)
{

	//获取平移
	int nOffsetCheekH = m_pSliderOffsetCheekH->GetLevel();
	int nOffsetCheekV = m_pSliderOffsetCheekV->GetLevel();
	int nOffsetCheekZ = m_pSliderOffsetCheekZ->GetLevel ();


	//获取缩放
	int nScaleCheek = m_pSliderScaleCheek->GetLevel();

	//**获取FaceData指针
	CECFace::FACE_CUSTOMIZEDATA* pFaceData = m_pCurFace->GetFaceData();
	//---------设置与脸颊相关的参数4--------------
	//**设置脸颊位移
	pFaceData->offsetCheekH = nOffsetCheekH;
	pFaceData->offsetCheekV = nOffsetCheekV;
	pFaceData->offsetCheekZ = nOffsetCheekZ;
	//**设置脸颊缩放
	pFaceData->scaleCheek = nScaleCheek;

	//**计算脸颊
	m_pCurFace->CalculateCheek();

	return true;
}

//计算下巴
bool CECCustomize::CalculateChain(void)
{

	//获取位移
	int nOffsetChainV = m_pSliderOffsetChainV->GetLevel();
	int nOffsetChainZ = m_pSliderOffsetChainZ->GetLevel();
	


	//获取旋转

	int nRotateChain = m_pSliderRotateChain->GetLevel();
	

	//获取缩放
	int nScaleChainH = m_pSliderScaleChainH->GetLevel();

	//**获取FaceData指针
	CECFace::FACE_CUSTOMIZEDATA* pFaceData = m_pCurFace->GetFaceData();
	//--------------设置与下巴相关的参数4--------------
	//**设置下巴平移
	pFaceData->offsetChainV = nOffsetChainV;
	pFaceData->offsetChainZ = nOffsetChainZ;
	//**设置下巴旋转
	pFaceData->rotateChain = nRotateChain;
	//**设置下巴缩放
	pFaceData->scaleChainH = nScaleChainH;

	//**计算下巴
	m_pCurFace->CalculateChain();

	return true;
}

//计算颌骨
bool CECCustomize::CalculateJaw(void)
{
	//获取位移
	int nOffsetJawH = m_pSliderOffsetJawH->GetLevel ();
	int nOffsetJawV = m_pSliderOffsetJawV->GetLevel();
	int nOffsetJawZ = m_pSliderOffsetJawZ->GetLevel();


	//获取特殊缩放
	int nScaleJawSpecial = m_pSliderScaleJawSpecial->GetLevel();

	//获取缩放
	int nScaleJawH = m_pSliderScaleJawH->GetLevel ();
	int nScaleJawV = m_pSliderScaleJawV->GetLevel();


	//**获取FaceData指针
	CECFace::FACE_CUSTOMIZEDATA* pFaceData = m_pCurFace->GetFaceData();
	//--------------设置与颌骨相关的参数6--------------
	//**设置颌骨平移
	pFaceData->offsetJawH = nOffsetJawH;
	pFaceData->offsetJawV = nOffsetJawV;
	pFaceData->offsetJawZ = nOffsetJawZ;
	//**设置颌骨特殊缩放
	pFaceData->scaleJawSpecial = nScaleJawSpecial;
	//**设置颌骨缩放
	pFaceData->scaleJawH = nScaleJawH;
	pFaceData->scaleJawV = nScaleJawV;

	//**计算颌骨
	m_pCurFace->CalculateJaw();

	return true;
}

//计算高级选型
bool CECCustomize::CalculateAdvaceOption(void)
{
	CalculateForehead();//计算额头
	
	CalculateYokeBone();//计算颧骨

	CalculateCheek();//计算脸颊
	
	CalculateChain();//计算下巴

	CalculateJaw();//计算颌骨
	
	return true;
}

/*
//载入脸型相关的调解参数(从ini文件)
bool CECCustomize::LoadFaceShapeFactorFromIniFile(const char* szFile)
{
	AIniFile IniFile;

	if (!IniFile.Open(szFile))
	{
		a_LogOutput(1, "CECCustomize::LoadFaceShapeFactorFromINIFile, Failed to open file %s", szFile);
		return false;
	}

	AString strSection= "Face";

	m_fScaleFaceHFactor	= IniFile.GetValueAsFloat(strSection, "ScaleFaceHFactor", m_fScaleFaceHFactor);
	m_fScaleFaceVFactor	= IniFile.GetValueAsFloat(strSection, "ScaleFaceVFactor", m_fScaleFaceVFactor);
	
	strSection = "3Parts";
	m_fScaleUpPartFactor = IniFile.GetValueAsFloat(strSection, "ScaleUpPartFactor", m_fScaleUpPartFactor);
	m_fScaleMiddlePartFactor = IniFile.GetValueAsFloat(strSection, "ScaleMiddlePartFactor", m_fScaleMiddlePartFactor);
	m_fScaleDownPartFactor = IniFile.GetValueAsFloat(strSection, "ScaleDownPartFactor", m_fScaleDownPartFactor);

	strSection = "Forehead";

	m_fOffsetForeheadHFactor = IniFile.GetValueAsFloat(strSection, "OffsetForeheadHFactor", m_fOffsetForeheadHFactor);
	m_fOffsetForeheadVFactor = IniFile.GetValueAsFloat(strSection, "OffsetForeheadVFactor", m_fOffsetForeheadVFactor);
	m_fOffsetForeheadZFactor = IniFile.GetValueAsFloat(strSection, "OffsetForeheadZFactor", m_fOffsetForeheadZFactor);
	m_fRotateForeheadFactor = IniFile.GetValueAsFloat(strSection, "RotateForeheadFactor", m_fRotateForeheadFactor);
	m_fScaleForeheadFactor = IniFile.GetValueAsFloat(strSection, "ScaleForeheadFactor", m_fScaleForeheadFactor);

	strSection = "YokeBone";
	
	m_fOffsetYokeBoneHFactor = IniFile.GetValueAsFloat(strSection, "OffsetYokeBoneHFactor", m_fOffsetYokeBoneHFactor);
	m_fOffsetYokeBoneVFactor = IniFile.GetValueAsFloat(strSection, "OffsetYokeBoneVFactor", m_fOffsetYokeBoneVFactor);
	m_fOffsetYokeBoneZFactor = IniFile.GetValueAsFloat(strSection, "OffsetYokeBoneZFactor", m_fOffsetYokeBoneZFactor);
	m_fRotateYokeBoneFactor = IniFile.GetValueAsFloat(strSection, "RotateYokeBoneFactor", m_fRotateYokeBoneFactor);
	m_fScaleYokeBoneFactor = IniFile.GetValueAsFloat(strSection, "ScaleYokeBoneFactor", m_fScaleYokeBoneFactor);

	strSection = "Cheek";

	m_fOffsetCheekHFactor = IniFile.GetValueAsFloat(strSection, "OffsetCheekHFactor", m_fOffsetCheekHFactor);
	m_fOffsetCheekVFactor = IniFile.GetValueAsFloat(strSection, "OffsetCheekVFactor", m_fOffsetCheekVFactor);
	m_fOffsetCheekZFactor = IniFile.GetValueAsFloat(strSection, "OffsetCheekZFactor", m_fOffsetCheekZFactor);
	m_fScaleCheekFactor = IniFile.GetValueAsFloat(strSection, "ScaleCheekFactor", m_fScaleCheekFactor);

	strSection = "Chain";

	m_fOffsetChainVFactor = IniFile.GetValueAsFloat(strSection, "OffsetChainVFactor", m_fOffsetChainVFactor);
	m_fOffsetChainZFactor = IniFile.GetValueAsFloat(strSection, "OffsetChainZFactor", m_fOffsetChainZFactor);
	m_fRotateChainFactor = IniFile.GetValueAsFloat(strSection, "RotateChainFactor", m_fRotateChainFactor);
	m_fScaleChainHFactor = IniFile.GetValueAsFloat(strSection, "ScaleChainHFactor", m_fScaleChainHFactor);

	strSection = "Jaw";
	m_fOffsetJawHFactor = IniFile.GetValueAsFloat(strSection, "OffsetJawHFactor", m_fOffsetJawHFactor);
	m_fOffsetJawVFactor = IniFile.GetValueAsFloat(strSection, "OffsetJawVFactor", m_fOffsetJawVFactor);
	m_fOffsetJawZFactor = IniFile.GetValueAsFloat(strSection, "OffsetJawZFactor", m_fOffsetJawZFactor);
	m_fScaleJawSpecialFactor = IniFile.GetValueAsFloat(strSection, "ScaleJawSpecialFactor", m_fScaleJawSpecialFactor);
	m_fScaleJawHFactor = IniFile.GetValueAsFloat(strSection, "ScaleJawHFactor", m_fScaleJawHFactor);
	m_fScaleJawVFactor = IniFile.GetValueAsFloat(strSection, "ScaleJawVFactor", m_fScaleJawVFactor);

	strSection ="Eye";
	m_fScaleEyeHFactor = IniFile.GetValueAsFloat(strSection, "ScaleEyeHFactor", m_fScaleEyeHFactor);
	m_fScaleEyeVFactor = IniFile.GetValueAsFloat(strSection, "ScaleEyeVFactor", m_fScaleEyeVFactor);
	m_fRotateEyeFactor = IniFile.GetValueAsFloat(strSection, "RotateEyeFactor", m_fRotateEyeFactor);
	m_fOffsetEyeHFactor = IniFile.GetValueAsFloat(strSection, "OffsetEyeHFactor", m_fOffsetEyeHFactor);
	m_fOffsetEyeVFactor = IniFile.GetValueAsFloat(strSection, "OffsetEyeVFactor", m_fOffsetEyeVFactor);
	m_fOffsetEyeZFactor = IniFile.GetValueAsFloat(strSection, "OffsetEyeZFactor", m_fOffsetEyeZFactor);
	m_fScaleEyeBallFactor = IniFile.GetValueAsFloat(strSection, "ScaleEyeBallFactor", m_fScaleEyeBallFactor);

	m_fScaleEyeHFactor2 = IniFile.GetValueAsFloat(strSection, "ScaleEyeHFactor2", m_fScaleEyeHFactor2);
	m_fScaleEyeVFactor2 = IniFile.GetValueAsFloat(strSection, "ScaleEyeVFactor2", m_fScaleEyeVFactor2);
	m_fRotateEyeFactor2 = IniFile.GetValueAsFloat(strSection, "RotateEyeFactor2", m_fRotateEyeFactor2);
	m_fOffsetEyeHFactor2 = IniFile.GetValueAsFloat(strSection, "OffsetEyeHFactor2", m_fOffsetEyeHFactor2);
	m_fOffsetEyeVFactor2 = IniFile.GetValueAsFloat(strSection, "OffsetEyeVFactor2", m_fOffsetEyeVFactor2);
	m_fOffsetEyeZFactor2 = IniFile.GetValueAsFloat(strSection, "OffsetEyeZFactor2", m_fOffsetEyeZFactor2);
	m_fScaleEyeBallFactor2 = IniFile.GetValueAsFloat(strSection, "ScaleEyeBallFactor2", m_fScaleEyeBallFactor2);

	strSection = "Brow";
	m_fScaleBrowHFactor = IniFile.GetValueAsFloat(strSection, "ScaleBrowHFactor", m_fScaleBrowHFactor);
	m_fScaleBrowVFactor = IniFile.GetValueAsFloat(strSection, "ScaleBrowVFactor", m_fScaleBrowVFactor);
	m_fRotateBrowFactor = IniFile.GetValueAsFloat(strSection, "RotateBrowFactor", m_fRotateBrowFactor);
	m_fOffsetBrowHFactor = IniFile.GetValueAsFloat(strSection, "OffsetBrowHFactor", m_fOffsetBrowHFactor);
	m_fOffsetBrowVFactor = IniFile.GetValueAsFloat(strSection, "OffsetBrowVFactor", m_fOffsetBrowVFactor);
	m_fOffsetBrowZFactor = IniFile.GetValueAsFloat(strSection, "OffsetBrowZFactor", m_fOffsetBrowZFactor);

	m_fScaleBrowHFactor2 = IniFile.GetValueAsFloat(strSection, "ScaleBrowHFactor2", m_fScaleBrowHFactor2);
	m_fScaleBrowVFactor2 = IniFile.GetValueAsFloat(strSection, "ScaleBrowVFactor2", m_fScaleBrowVFactor2);
	m_fRotateBrowFactor2 = IniFile.GetValueAsFloat(strSection, "RotateBrowFactor2", m_fRotateBrowFactor2);
	m_fOffsetBrowHFactor2 = IniFile.GetValueAsFloat(strSection, "OffsetBrowHFactor2", m_fOffsetBrowHFactor2);
	m_fOffsetBrowVFactor2 = IniFile.GetValueAsFloat(strSection, "OffsetBrowVFactor2", m_fOffsetBrowVFactor2);
	m_fOffsetBrowZFactor2 = IniFile.GetValueAsFloat(strSection, "OffsetBrowZFactor2", m_fOffsetBrowZFactor2);

	strSection = "Nose";
	m_fScaleNoseTipHFactor = IniFile.GetValueAsFloat(strSection, "ScaleNoseTipHFactor", m_fScaleNoseTipHFactor);
	m_fScaleNoseTipVFactor = IniFile.GetValueAsFloat(strSection, "ScaleNoseTipVFactor", m_fScaleNoseTipVFactor);
	m_fOffsetNoseTipVFactor = IniFile.GetValueAsFloat(strSection, "OffsetNoseTipVFactor", m_fOffsetNoseTipVFactor);
	m_fScaleNoseTipZFactor = IniFile.GetValueAsFloat(strSection, "ScaleNoseTipZFactor", m_fScaleNoseTipZFactor);
	m_fScaleBridgeTipHFactor = IniFile.GetValueAsFloat(strSection, "ScaleBridgeTipHFactor", m_fScaleBridgeTipHFactor);
	m_fOffsetBridgeTipZFactor = IniFile.GetValueAsFloat(strSection, "OffsetBridgeTipZFactor", m_fOffsetBridgeTipZFactor);
	
	strSection = "Mouth";
	m_fThickUpLipFactor = IniFile.GetValueAsFloat(strSection, "ThickUpLipFactor", m_fThickUpLipFactor);
	m_fThickDownLipFactor = IniFile.GetValueAsFloat(strSection, "ThickDownLipFactor", m_fThickDownLipFactor);
	m_fScaleMouthHFactor = IniFile.GetValueAsFloat(strSection, "ScaleMouthHFactor", m_fScaleMouthHFactor);
	m_fOffsetMouthVFactor = IniFile.GetValueAsFloat(strSection, "OffsetMouthVFactor", m_fOffsetMouthVFactor);
	m_fOffsetMouthZFactor = IniFile.GetValueAsFloat(strSection, "OffsetMouthZFactor", m_fOffsetMouthZFactor);
	m_fOffsetCornerOfMouthSpecialFactor = IniFile.GetValueAsFloat(strSection, "OffsetCornerOfMouthSpecialFactor", m_fOffsetCornerOfMouthSpecialFactor); 
	m_fScaleMouthHFactor2 = IniFile.GetValueAsFloat(strSection, "ScaleMouthHFactor2", m_fScaleMouthHFactor2);
	m_fOffsetCornerOfMouthSpecialFactor2 = IniFile.GetValueAsFloat(strSection, "OffsetCornerOfMouthSpecialFactor2", m_fOffsetCornerOfMouthSpecialFactor2); 

	strSection = "Ear";
	m_fScaleEarFactor = IniFile.GetValueAsFloat(strSection, "ScaleEarFactor", m_fScaleEarFactor);
	m_fOffsetEarVFactor = IniFile.GetValueAsFloat(strSection, "OffsetEarVFactor", m_fOffsetEarVFactor);

	strSection = "Stature";
	m_fHeadScaleFactor = IniFile.GetValueAsFloat(strSection, "HeadScaleFactor", m_fHeadScaleFactor);
	m_fUpScaleFactor = IniFile.GetValueAsFloat(strSection, "UpScaleFactor", m_fUpScaleFactor);
	m_fWaistScaleFactor = IniFile.GetValueAsFloat(strSection, "WaistScaleFactor", m_fWaistScaleFactor);
	m_fArmWidthFactor = IniFile.GetValueAsFloat(strSection, "ArmWidthFactor", m_fArmWidthFactor);
	m_fLegWidthFactor = IniFile.GetValueAsFloat(strSection, "LegWidthFactor", m_fLegWidthFactor);
	m_fBreastScaleFactor = IniFile.GetValueAsFloat(strSection, "BreastScaleFactor", m_fBreastScaleFactor);

	
	IniFile.Close();

	return true;
}
*/

//根据载入FaceData数据设置滚动条
bool CECCustomize::SetFaceAllSlider(void)
{
	CECFace::FACE_CUSTOMIZEDATA* pFaceData = m_pCurFace->GetFaceData();
	
	//脸盘融合

	//3庭3
	m_nDefaultScaleUp = MakeSureSlider(pFaceData->scaleUp, m_pSliderBound->m_nScaleUpPartMax, m_pSliderBound->m_nScaleUpPartMin);
	m_nDefaultScaleMiddle = MakeSureSlider(pFaceData->scaleMiddle, m_pSliderBound->m_nScaleMiddlePartMax, m_pSliderBound->m_nScaleMiddlePartMin);
	m_nDefaultScaleDown = MakeSureSlider(pFaceData->scaleDown, m_pSliderBound->m_nScaleDownPartMax, m_pSliderBound->m_nScaleDownPartMin);

	m_pSliderPartUp->SetLevel(m_nDefaultScaleUp);
	m_pSliderPartMiddle->SetLevel(m_nDefaultScaleMiddle);
	m_pSliderPartDown->SetLevel(m_nDefaultScaleDown);

	//脸盘缩放2
	m_nDefaultScaleFaceH = MakeSureSlider(pFaceData->scaleFaceH, m_pSliderBound->m_nScaleFaceHMax, m_pSliderBound->m_nScaleFaceHMin);
	m_nDefaultScaleFaceV = MakeSureSlider(pFaceData->scaleFaceV, m_pSliderBound->m_nScaleFaceVMax, m_pSliderBound->m_nScaleFaceVMin);
	m_pSliderScaleFaceH->SetLevel(m_nDefaultScaleFaceH);
	m_pSliderScaleFaceV->SetLevel(m_nDefaultScaleFaceV);

	//眼睛14
	m_nDefaultScaleEyeH = MakeSureSlider(pFaceData->scaleEyeH, m_pSliderBound->m_nScaleEyeHMax, m_pSliderBound->m_nScaleEyeHMin);
	m_nDefaultScaleEyeV = MakeSureSlider(pFaceData->scaleEyeV, m_pSliderBound->m_nScaleEyeVMax, m_pSliderBound->m_nScaleEyeVMin);
	m_nDefaultRotateEye = MakeSureSlider(pFaceData->rotateEye, m_pSliderBound->m_nRotateEyeMax, m_pSliderBound->m_nRotateEyeMin);
	m_nDefaultOffsetEyeH = MakeSureSlider(pFaceData->offsetEyeH, m_pSliderBound->m_nOffsetEyeHMax, m_pSliderBound->m_nOffsetEyeHMin);
	m_nDefaultOffsetEyeV = MakeSureSlider(pFaceData->offsetEyeV, m_pSliderBound->m_nOffsetEyeVMax, m_pSliderBound->m_nOffsetEyeVMin);
	m_nDefaultOffsetEyeZ = MakeSureSlider(pFaceData->offsetEyeZ, m_pSliderBound->m_nOffsetEyeZMax, m_pSliderBound->m_nOffsetEyeZMin);
	m_nDefaultScaleEyeBall = MakeSureSlider(pFaceData->scaleEyeBall, m_pSliderBound->m_nScaleEyeBallMax, m_pSliderBound->m_nScaleEyeBallMin);

	m_nDefaultScaleEyeH2 = MakeSureSlider(pFaceData->scaleEyeH2, m_pSliderBound->m_nScaleEyeHMax, m_pSliderBound->m_nScaleEyeHMin);
	m_nDefaultScaleEyeV2 = MakeSureSlider(pFaceData->scaleEyeV2, m_pSliderBound->m_nScaleEyeVMax, m_pSliderBound->m_nScaleEyeVMin);
	m_nDefaultRotateEye2 = MakeSureSlider(pFaceData->rotateEye2, m_pSliderBound->m_nRotateEyeMax, m_pSliderBound->m_nRotateEyeMin);
	m_nDefaultOffsetEyeH2 = MakeSureSlider(pFaceData->offsetEyeH2, m_pSliderBound->m_nOffsetEyeHMax, m_pSliderBound->m_nOffsetEyeHMin);
	m_nDefaultOffsetEyeV2 = MakeSureSlider(pFaceData->offsetEyeV2, m_pSliderBound->m_nOffsetEyeVMax, m_pSliderBound->m_nOffsetEyeVMin);
	m_nDefaultOffsetEyeZ2 = MakeSureSlider(pFaceData->offsetEyeZ2, m_pSliderBound->m_nOffsetEyeZMax, m_pSliderBound->m_nOffsetEyeZMin);
	m_nDefaultScaleEyeBall2 = MakeSureSlider(pFaceData->scaleEyeBall2, m_pSliderBound->m_nScaleEyeBallMax, m_pSliderBound->m_nScaleEyeBallMin);


	
	m_nScaleEyeH = m_nDefaultScaleEyeH;
	m_nScaleEyeV = m_nDefaultScaleEyeV;
	m_nRotateEye = m_nDefaultRotateEye;
	m_nOffsetEyeH = m_nDefaultOffsetEyeH;
	m_nOffsetEyeV = m_nDefaultOffsetEyeV;
	m_nOffsetEyeZ = m_nDefaultOffsetEyeZ;
	m_nScaleEyeBall = m_nDefaultScaleEyeBall;	
	
	m_nScaleEyeH2 = m_nDefaultScaleEyeH2;
	m_nScaleEyeV2 = m_nDefaultScaleEyeV2;
	m_nRotateEye2 = m_nDefaultRotateEye2;
	m_nOffsetEyeH2 = m_nDefaultOffsetEyeH2;
	m_nOffsetEyeV2 = m_nDefaultOffsetEyeV2;
	m_nOffsetEyeZ2 = m_nDefaultOffsetEyeZ2;
	m_nScaleEyeBall2 = m_nDefaultScaleEyeBall2;	

	m_pSliderScaleEyeH->SetLevel(m_nDefaultScaleEyeH);
	m_pSliderScaleEyeV->SetLevel(m_nDefaultScaleEyeV);
	m_pSliderRotateEye->SetLevel(m_nDefaultRotateEye);
	m_pSliderOffsetEyeH->SetLevel(m_nDefaultOffsetEyeH);
	m_pSliderOffsetEyeV->SetLevel(m_nDefaultOffsetEyeV);
	m_pSliderOffsetEyeZ->SetLevel(m_nDefaultOffsetEyeZ);
	m_pSliderScaleEyeBall->SetLevel(m_nDefaultScaleEyeBall);

	//眉毛6
	m_nDefaultScaleBrowH = MakeSureSlider(pFaceData->scaleBrowH, m_pSliderBound->m_nScaleBrowHMax, m_pSliderBound->m_nScaleBrowHMin);
	m_nDefaultScaleBrowV = MakeSureSlider(pFaceData->scaleBrowV, m_pSliderBound->m_nScaleBrowVMax, m_pSliderBound->m_nScaleBrowVMin);
	m_nDefaultRotateBrow = MakeSureSlider(pFaceData->rotateBrow, m_pSliderBound->m_nRotateBrowMax, m_pSliderBound->m_nRotateBrowMin);
	m_nDefaultOffsetBrowH = MakeSureSlider(pFaceData->offsetBrowH, m_pSliderBound->m_nOffsetBrowHMax, m_pSliderBound->m_nOffsetBrowHMin);
	m_nDefaultOffsetBrowV = MakeSureSlider(pFaceData->offsetBrowV, m_pSliderBound->m_nOffsetBrowVMax, m_pSliderBound->m_nOffsetBrowVMin);
	m_nDefaultOffsetBrowZ = MakeSureSlider(pFaceData->offsetBrowZ, m_pSliderBound->m_nOffsetBrowZMax, m_pSliderBound->m_nOffsetBrowZMin);
	

	m_nDefaultScaleBrowH2 = MakeSureSlider(pFaceData->scaleBrowH2, m_pSliderBound->m_nScaleBrowHMax, m_pSliderBound->m_nScaleBrowHMin);
	m_nDefaultScaleBrowV2 = MakeSureSlider(pFaceData->scaleBrowV2, m_pSliderBound->m_nScaleBrowVMax, m_pSliderBound->m_nScaleBrowVMin);
	m_nDefaultRotateBrow2 = MakeSureSlider(pFaceData->rotateBrow2, m_pSliderBound->m_nRotateBrowMax, m_pSliderBound->m_nRotateBrowMin);
	m_nDefaultOffsetBrowH2 = MakeSureSlider(pFaceData->offsetBrowH2, m_pSliderBound->m_nOffsetBrowHMax, m_pSliderBound->m_nOffsetBrowHMin);
	m_nDefaultOffsetBrowV2 = MakeSureSlider(pFaceData->offsetBrowV2, m_pSliderBound->m_nOffsetBrowVMax, m_pSliderBound->m_nOffsetBrowVMin);
	m_nDefaultOffsetBrowZ2 = MakeSureSlider(pFaceData->offsetBrowZ2, m_pSliderBound->m_nOffsetBrowZMax, m_pSliderBound->m_nOffsetBrowZMin);
	
	m_nScaleBrowH = m_nDefaultScaleBrowH;
	m_nScaleBrowV = m_nDefaultScaleBrowV;
	m_nRotateBrow = m_nDefaultRotateBrow;
	m_nOffsetBrowH = m_nDefaultOffsetBrowH;
	m_nOffsetBrowV = m_nDefaultOffsetBrowV;
	m_nOffsetBrowZ = m_nDefaultOffsetBrowZ;

	m_nScaleBrowH2 = m_nDefaultScaleBrowH2;
	m_nScaleBrowV2 = m_nDefaultScaleBrowV2;
	m_nRotateBrow2 = m_nDefaultRotateBrow2;
	m_nOffsetBrowH2 = m_nDefaultOffsetBrowH2;
	m_nOffsetBrowV2 = m_nDefaultOffsetBrowV2;
	m_nOffsetBrowZ2 = m_nDefaultOffsetBrowZ2;

	m_pSliderScaleBrowH->SetLevel(m_nDefaultScaleBrowH);
	m_pSliderScaleBrowV->SetLevel(m_nDefaultScaleBrowV);
	m_pSliderRotateBrow->SetLevel(m_nDefaultRotateBrow);
	m_pSliderOffsetBrowH->SetLevel(m_nDefaultOffsetBrowH);
	m_pSliderOffsetBrowV->SetLevel(m_nDefaultOffsetBrowV);
	m_pSliderOffsetBrowZ->SetLevel(m_nDefaultOffsetBrowZ);
	
	//鼻子5
	m_nDefaultScaleNoseTipH = MakeSureSlider(pFaceData->scaleNoseTipH, m_pSliderBound->m_nScaleNoseTipHMax, m_pSliderBound->m_nScaleNoseTipHMin);
	m_nDefaultScaleNoseTipV = MakeSureSlider(pFaceData->scaleNoseTipV, m_pSliderBound->m_nScaleNoseTipVMax, m_pSliderBound->m_nScaleNoseTipVMin);
	m_nDefaultScaleNoseTipZ = MakeSureSlider(pFaceData->scaleNoseTipZ, m_pSliderBound->m_nScaleNoseTipZMax, m_pSliderBound->m_nScaleNoseTipZMin);
	m_nDefaultOffsetNoseTipV = MakeSureSlider(pFaceData->offsetNoseTipV, m_pSliderBound->m_nOffsetNoseTipVMax, m_pSliderBound->m_nOffsetNoseTipVMin);
	m_nDefaultScaleBridgeTipH = MakeSureSlider(pFaceData->scaleBridgeTipH, m_pSliderBound->m_nScaleBridgeTipHMax, m_pSliderBound->m_nScaleBridgeTipHMin);
	m_nDefaultOffsetBridegTipZ = MakeSureSlider(pFaceData->offsetBridgeTipZ, m_pSliderBound->m_nOffsetBridgeTipZMax, m_pSliderBound->m_nOffsetBridgeTipZMin);
	
	m_pSliderScaleNoseTipH->SetLevel(m_nDefaultScaleNoseTipH);
	m_pSliderScaleNoseTipV->SetLevel(m_nDefaultScaleNoseTipV);
	m_pSliderScaleNoseTipZ->SetLevel(m_nDefaultScaleNoseTipZ);
	m_pSliderOffsetNoseTipV->SetLevel(m_nDefaultOffsetNoseTipV);
	m_pSliderScaleBridgeTipH->SetLevel(m_nDefaultScaleBridgeTipH);
	m_pSliderOffsetBridgeTipZ->SetLevel(m_nDefaultOffsetBridegTipZ);
	
	//嘴7
	m_nDefaultThickUpLip = MakeSureSlider(pFaceData->thickUpLip, m_pSliderBound->m_nThickUpLipMax, m_pSliderBound->m_nThickUpLipMin);
	m_nDefaultThickDownLip = MakeSureSlider(pFaceData->thickDownLip, m_pSliderBound->m_nThickDownLipMax, m_pSliderBound->m_nThickDownLipMin);
	m_nDefaultScaleMouthH = MakeSureSlider(pFaceData->scaleMouthH, m_pSliderBound->m_nScaleMouthHMax, m_pSliderBound->m_nScaleMouthHMin);
	m_nDefaultOffsetMouthV = MakeSureSlider(pFaceData->offsetMouthV, m_pSliderBound->m_nOffsetMouthVMax, m_pSliderBound->m_nOffsetMouthVMin);
	m_nDefaultOffsetMouthZ = MakeSureSlider(pFaceData->offsetMouthZ, m_pSliderBound->m_nOffsetMouthZMax, m_pSliderBound->m_nOffsetMouthZMin);
	m_nDefaultOffsetCornerOfMouthSpecial = MakeSureSlider(pFaceData->offsetCornerOfMouthSpecial, m_pSliderBound->m_nOffsetCornerOfMouthSpecialMax, m_pSliderBound->m_nOffsetCornerOfMouthSpecialMin);

	m_nDefaultScaleMouthH2 = MakeSureSlider(pFaceData->scaleMouthH2, m_pSliderBound->m_nScaleMouthH2Max, m_pSliderBound->m_nScaleMouthH2Min);
	m_nDefaultOffsetCornerOfMouthSpecial2 = MakeSureSlider(pFaceData->offsetCornerOfMouthSpecial2, m_pSliderBound->m_nOffsetCornerOfMouthSpecial2Max, m_pSliderBound->m_nOffsetCornerOfMouthSpecial2Min);


	m_pSliderThickUpLip->SetLevel(m_nDefaultThickUpLip);
	m_pSliderThickDownLip->SetLevel(m_nDefaultThickDownLip);
	m_pSliderScaleMouthH->SetLevel(m_nDefaultScaleMouthH);
	m_pSliderOffsetMouthV->SetLevel(m_nDefaultOffsetMouthV);
	m_pSliderOffsetMouthZ->SetLevel(m_nDefaultOffsetMouthZ);
	m_pSliderOffsetCornerOfMouthSpecial->SetLevel(m_nDefaultOffsetCornerOfMouthSpecial);
	
	m_pSliderScaleMouthH2->SetLevel(m_nDefaultScaleMouthH2);
	m_pSliderOffsetCornerOfMouthSpecial2->SetLevel(m_nDefaultOffsetCornerOfMouthSpecial2);

	//耳朵2
	m_nDefaultScaleEar = MakeSureSlider(pFaceData->scaleEar, m_pSliderBound->m_nScaleEarMax, m_pSliderBound->m_nScaleEarMin);
	m_nDefaultOffsetEarV = MakeSureSlider(pFaceData->offsetEarV, m_pSliderBound->m_nOffsetEarVMax, m_pSliderBound->m_nOffsetEarVMin);
	m_pSliderScaleEar->SetLevel(m_nDefaultScaleEar);
	m_pSliderOffsetEarV->SetLevel(m_nDefaultOffsetEarV);
	
	//额头5
	m_nDefaultOffsetForeheadH = MakeSureSlider(pFaceData->offsetForeheadH, m_pSliderBound->m_nOffsetForeheadHMax, m_pSliderBound->m_nOffsetForeheadHMin);
	m_nDefaultOffsetForeheadV = MakeSureSlider(pFaceData->offsetForeheadV, m_pSliderBound->m_nOffsetForeheadVMax, m_pSliderBound->m_nOffsetForeheadVMin);
	m_nDefaultOffsetForeheadZ = MakeSureSlider(pFaceData->offsetForeheadZ, m_pSliderBound->m_nOffsetForeheadZMax, m_pSliderBound->m_nOffsetForeheadZMin);
	m_nDefaultRotateForehead = MakeSureSlider(pFaceData->rotateForehead, m_pSliderBound->m_nRotateForeheadMax, m_pSliderBound->m_nRotateForeheadMin);
	m_nDefaultScaleForehead = MakeSureSlider(pFaceData->scaleForehead, m_pSliderBound->m_nScaleForeheadMax, m_pSliderBound->m_nScaleForeheadMin);

	m_pSliderOffsetForeheadH->SetLevel(m_nDefaultOffsetForeheadH);
	m_pSliderOffsetForeheadV->SetLevel(m_nDefaultOffsetForeheadV);
	m_pSliderOffsetForeheadZ->SetLevel(m_nDefaultOffsetForeheadZ);
	m_pSliderRotateForehead->SetLevel(m_nDefaultRotateForehead);
	m_pSliderScaleForehead->SetLevel(m_nDefaultScaleForehead);

	//颧骨5
	m_nDefaultOffsetYokeBoneH = MakeSureSlider(pFaceData->offsetYokeBoneH, m_pSliderBound->m_nOffsetYokeBoneHMax, m_pSliderBound->m_nOffsetYokeBoneHMin);
	m_nDefaultOffsetYokeBoneV = MakeSureSlider(pFaceData->offsetYokeBoneV, m_pSliderBound->m_nOffsetYokeBoneVMax, m_pSliderBound->m_nOffsetYokeBoneVMin);
	m_nDefaultOffsetYokeBoneZ = MakeSureSlider(pFaceData->offsetYokeBoneZ, m_pSliderBound->m_nOffsetYokeBoneZMax, m_pSliderBound->m_nOffsetYokeBoneZMin);
	m_nDefaultRotateYokeBone = MakeSureSlider(pFaceData->rotateYokeBone, m_pSliderBound->m_nRotateYokeBoneMax, m_pSliderBound->m_nRotateYokeBoneMin);
	m_nDefaultScaleYokeBone = MakeSureSlider(pFaceData->scaleYokeBone, m_pSliderBound->m_nScaleYokeBoneMax, m_pSliderBound->m_nScaleYokeBoneMin);

	m_pSliderOffsetYokeBoneH->SetLevel(m_nDefaultOffsetYokeBoneH);
	m_pSliderOffsetYokeBoneV->SetLevel(m_nDefaultOffsetYokeBoneV);
	m_pSliderOffsetYokeBoneZ->SetLevel(m_nDefaultOffsetYokeBoneZ);
	m_pSliderRotateYokeBone->SetLevel(m_nDefaultRotateYokeBone);
	m_pSliderScaleYokeBone->SetLevel(m_nDefaultScaleYokeBone);

	//脸颊4
	m_nDefaultOffsetCheekH = MakeSureSlider(pFaceData->offsetCheekH, m_pSliderBound->m_nOffsetCheekHMax, m_pSliderBound->m_nOffsetCheekHMin);
	m_nDefaultOffsetCheekV = MakeSureSlider(pFaceData->offsetCheekV, m_pSliderBound->m_nOffsetCheekVMax, m_pSliderBound->m_nOffsetCheekVMin);
	m_nDefaultOffsetCheekZ = MakeSureSlider(pFaceData->offsetCheekZ, m_pSliderBound->m_nOffsetCheekZMax, m_pSliderBound->m_nOffsetCheekZMin);
	m_nDefaultScaleCheek = MakeSureSlider(pFaceData->scaleCheek, m_pSliderBound->m_nScaleCheekMax, m_pSliderBound->m_nScaleCheekMin);

	m_pSliderOffsetCheekH->SetLevel(m_nDefaultOffsetCheekH);
	m_pSliderOffsetCheekV->SetLevel(m_nDefaultOffsetCheekV);
	m_pSliderOffsetCheekZ->SetLevel(m_nDefaultOffsetCheekZ);
	m_pSliderScaleCheek->SetLevel(m_nDefaultScaleCheek);

	//下巴4
	m_nDefaultOffsetChainV = MakeSureSlider(pFaceData->offsetChainV, m_pSliderBound->m_nOffsetChainVMax, m_pSliderBound->m_nOffsetChainVMin);
	m_nDefaultOffsetChainZ = MakeSureSlider(pFaceData->offsetChainZ, m_pSliderBound->m_nOffsetChainZMax, m_pSliderBound->m_nOffsetChainZMin);
	m_nDefaultRotateChain = MakeSureSlider(pFaceData->rotateChain, m_pSliderBound->m_nRotateBrowMax, m_pSliderBound->m_nRotateBrowMin);
	m_nDefaultScaleChainH = MakeSureSlider(pFaceData->scaleChainH, m_pSliderBound->m_nScaleChainHMax, m_pSliderBound->m_nScaleChainHMin);

	m_pSliderOffsetChainV->SetLevel(m_nDefaultOffsetChainV);
	m_pSliderOffsetChainZ->SetLevel(m_nDefaultOffsetChainZ);
	m_pSliderRotateChain->SetLevel(m_nDefaultRotateChain);
	m_pSliderScaleChainH->SetLevel(m_nDefaultScaleChainH);

	//颌骨6
	m_nDefaultOffsetJawH = MakeSureSlider(pFaceData->offsetJawH, m_pSliderBound->m_nOffsetJawHMax, m_pSliderBound->m_nOffsetJawHMin);
	m_nDefaultOffsetJawV = MakeSureSlider(pFaceData->offsetJawV, m_pSliderBound->m_nOffsetJawVMax, m_pSliderBound->m_nOffsetJawVMin);
	m_nDefaultOffsetJawZ = MakeSureSlider(pFaceData->offsetJawZ, m_pSliderBound->m_nOffsetJawZMax, m_pSliderBound->m_nOffsetJawZMin);
	m_nDefaultScaleJawSpecial = MakeSureSlider(pFaceData->scaleJawSpecial, m_pSliderBound->m_nScaleJawSpecialMax, m_pSliderBound->m_nScaleJawSpecialMin);
	m_nDefaultScaleJawH = MakeSureSlider(pFaceData->scaleJawH, m_pSliderBound->m_nScaleJawHMax, m_pSliderBound->m_nScaleJawHMin);
	m_nDefaultScaleJawV = MakeSureSlider(pFaceData->scaleJawV, m_pSliderBound->m_nScaleJawVMax, m_pSliderBound->m_nScaleJawVMin);
	
	m_pSliderOffsetJawH->SetLevel(m_nDefaultOffsetJawH);
	m_pSliderOffsetJawV->SetLevel(m_nDefaultOffsetJawV);
	m_pSliderOffsetJawZ->SetLevel(m_nDefaultOffsetJawZ);
	m_pSliderScaleJawSpecial->SetLevel(m_nDefaultScaleJawSpecial);
	m_pSliderScaleJawH->SetLevel(m_nDefaultScaleJawH);
	m_pSliderScaleJawV->SetLevel(m_nDefaultScaleJawV);

	CECPlayer::PLAYER_CUSTOMIZEDATA pCustomizeData = m_pCurPlayer->GetCustomizeData();

	//身材
	m_nDefaultHeadScale =MakeSureSlider(pCustomizeData.headScale, m_pSliderBound->m_nHeadScaleMax, m_pSliderBound->m_nHeadScaleMin);
	m_nDefaultUpScale = MakeSureSlider(pCustomizeData.upScale, m_pSliderBound->m_nUpScaleMax, m_pSliderBound->m_nUpScaleMin);
	m_nDefaultWaistScale = MakeSureSlider(pCustomizeData.waistScale, m_pSliderBound->m_nWaistScaleMax, m_pSliderBound->m_nWaistScaleMin);
	m_nDefaultArmWidth =MakeSureSlider(pCustomizeData.armWidth, m_pSliderBound->m_nArmWidthMax, m_pSliderBound->m_nArmWidthMin);
	m_nDefaultLegWidth = MakeSureSlider(pCustomizeData.legWidth, m_pSliderBound->m_nLegWidthMax, m_pSliderBound->m_nLegWidthMin);
	m_nDefaultBreastScale = MakeSureSlider(pCustomizeData.breastScale, m_pSliderBound->m_nBreastScaleMax, m_pSliderBound->m_nBreastScaleMin);

	m_pSliderHeadScale->SetLevel(m_nDefaultHeadScale);
	m_pSliderUpScale->SetLevel(m_nDefaultUpScale);
	m_pSliderWaistScale->SetLevel(m_nDefaultWaistScale);
	m_pSliderArmWidth->SetLevel(m_nDefaultArmWidth);
	m_pSliderLegWidth->SetLevel(m_nDefaultLegWidth);
	m_pSliderBreastScale->SetLevel(m_nDefaultBreastScale);

	return true;
}

//复原BlendFace系数
int CECCustomize::ResetBlendFaceFactorToInt(float fFactor)
{
	int nReturn = int(fFactor * 100.0f);

	if( nReturn < 0)
		return 0;
	else if( nReturn > 100)
		return 100;

	return nReturn;
}

//复原Scale
int CECCustomize::ResetScaleToInt(float fScale, float fScaleFactor)
{
	int nReturn = int ((fScale -1.0f)* (1.0f/ fScaleFactor) * 128.0f + 128.0f);
	if( nReturn < 0)
		return 0;
	else if( nReturn > 255)
		return 255;
	return nReturn;
}

//复原offset
int CECCustomize::ResetOffsetToInt(float fOffset, float fOffsetFactor)
{	
	int nReturn = int (fOffset* (1.0f/ fOffsetFactor) * 128.0f + 128.0f);
	if( nReturn < 0)
		return 0;
	else if( nReturn > 255)
		return 255;
	return nReturn;
}

//复原rotate
int CECCustomize::ResetRotateToInt( float fRotate, float fRotateFactor)
{
	int nReturn = int (fRotate* (1.0f/ fRotateFactor) * 128.0f + 128.0f);
	if( nReturn < 0)
		return 0;
	else if( nReturn > 255)
		return 255;
	return nReturn;
}

//复位脸部颜色
void CECCustomize::ResetFaceAllColor()
{
	//**获取FaceData指针
	CECFace::FACE_CUSTOMIZEDATA* pFaceData = m_pCurFace->GetFaceData();

	pFaceData->colorBrow = 0xffffffff;
	pFaceData->colorEye = 0xffffffff;
	pFaceData->colorEyeBall = 0xffffffff;
	pFaceData->colorFace = 0xffffffff;
	pFaceData->colorHair = 0xffffffff;
	pFaceData->colorMoustache = 0xffffffff;
	pFaceData->colorMouth = 0xffffffff;

	m_pCurFace->UpdateAllFaceColor();
	m_pImagePictureCurColor->SetColor(0xffffffff);

	m_pCurPlayer->SetBodyColor(0xffffffff);
}


//把比例从int 转换到float
float CECCustomize::TransformScaleFromIntToFloat(int nScale, float fScaleFactor)
{
	if(fScaleFactor >=  1.0)
		fScaleFactor = 0.99f;

	else if ( fScaleFactor < 0)
		fScaleFactor = 0;

	float fScale  = (nScale - 50.0f)/50.0f;

	fScale = fScale * fScaleFactor + 1.0f;

	return  fScale;
}



//更新体形
bool CECCustomize::UpdateStature(void)
{
	int nHeadScale = m_pSliderHeadScale->GetLevel();

	int nUpScale = m_pSliderUpScale->GetLevel();

	int nWaistScale = m_pSliderWaistScale->GetLevel();

	int nArmWidth = m_pSliderArmWidth->GetLevel();

	int nLegWidth = m_pSliderLegWidth->GetLevel();

	int nBreastScale = m_pSliderBreastScale->GetLevel();
	

	m_pCurPlayer->SetBodyHeadScale (nHeadScale);
	m_pCurPlayer->SetBodyUpScale(nUpScale);
	m_pCurPlayer->SetBodyWaistScale(nWaistScale);
	m_pCurPlayer->SetBodyArmWidth(nArmWidth);
	m_pCurPlayer->SetBodyLegWidth(nLegWidth);
	m_pCurPlayer->SetBodyBreastScale(nBreastScale);

	return true;
}



// 载入形象的个性化数据
bool CECCustomize::LoadCustomizeData(char* pszFile)
{
	CECPlayer::PLAYER_CUSTOMIZEDATA data;
	memset(&data, 0, sizeof(data));
	data.dwVersion = CUSTOMIZE_DATA_VERSION;

	AIniFile IniFile;
	
	if(!IniFile.Open(pszFile))
	{
		a_LogOutput(1, "CECCustomize::LoadCustomizeData, Failed to open file %s", pszFile);
		return false;
	}

	//3庭3
	AString strSection = "3Parts";
	data.faceData.scaleUp = IniFile.GetValueAsInt(strSection, "scaleUp", data.faceData.scaleUp);
	data.faceData.scaleMiddle = IniFile.GetValueAsInt(strSection, "scaleMiddle", data.faceData.scaleMiddle);
	data.faceData.scaleDown = IniFile.GetValueAsInt(strSection, "scaleDown", data.faceData.scaleDown);

	//脸型融合3
	strSection = "BlendFace";
	data.faceData.idFaceShape1 = IniFile.GetValueAsInt(strSection, "idFaceShape1", data.faceData.idFaceShape1);
	data.faceData.idFaceShape2 = IniFile.GetValueAsInt(strSection, "idFaceShape2", data.faceData.idFaceShape2);
	data.faceData.blendFaceShape = IniFile.GetValueAsInt(strSection, "blendFaceShape", data.faceData.blendFaceShape);

	
	if(!MakeSureCustomizeData(DT_FACE_SHAPE_ESSENCE, data.faceData.idFaceShape1, 0))
	{
		IniFile.Close();
		return false;
	}

	if(!MakeSureCustomizeData(DT_FACE_SHAPE_ESSENCE, data.faceData.idFaceShape2, 0))
	{
		IniFile.Close();
		return false;
	}

		

	//脸盘调整3
	strSection = "Face";
	data.faceData.scaleFaceH = IniFile.GetValueAsInt(strSection, "scaleFaceH", data.faceData.scaleFaceH);
	data.faceData.scaleFaceV = IniFile.GetValueAsInt(strSection, "scaleFaceV", data.faceData.scaleFaceV);
	data.faceData.idFaceTex = IniFile.GetValueAsInt(strSection, "idFaceTex", data.faceData.idFaceTex);


	if(!MakeSureCustomizeData(DT_FACE_TEXTURE_ESSENCE, data.faceData.idFaceTex, 0))
	{
		IniFile.Close();
		return false;
	}

	//法令
	strSection = "Faling";
	data.faceData.idFalingSkin = IniFile.GetValueAsInt(strSection, "idFalingSkin", data.faceData.idFalingSkin);
	data.faceData.idFalingTex = IniFile.GetValueAsInt(strSection, "idFalingTex", data.faceData.idFalingTex);

	if(!MakeSureCustomizeData(DT_FACE_FALING_ESSENCE , data.faceData.idFalingSkin))
	{
		IniFile.Close();
		return false;
	}
	
	if(!MakeSureCustomizeData(DT_FACE_TEXTURE_ESSENCE, data.faceData.idFalingTex, 10))
	{
		IniFile.Close();
		return false;
	}

	//额头5
	strSection = "Forehead";
	data.faceData.offsetForeheadH = IniFile.GetValueAsInt(strSection, "offsetForeheadH", data.faceData.offsetForeheadH);
	data.faceData.offsetForeheadV = IniFile.GetValueAsInt(strSection, "offsetForeheadV", data.faceData.offsetForeheadV);
	data.faceData.offsetForeheadZ = IniFile.GetValueAsInt(strSection, "offsetForeheadZ", data.faceData.offsetForeheadZ);
	data.faceData.rotateForehead = IniFile.GetValueAsInt(strSection, "rotateForehead", data.faceData.rotateForehead);
	data.faceData.scaleForehead = IniFile.GetValueAsInt(strSection, "scaleForehead", data.faceData.scaleForehead);

	//颧骨5
	strSection = "YokeBone";
	data.faceData.offsetYokeBoneH = IniFile.GetValueAsInt(strSection, "offsetYokeBoneH", data.faceData.offsetYokeBoneH);
	data.faceData.offsetYokeBoneV = IniFile.GetValueAsInt(strSection, "offsetYokeBoneV", data.faceData.offsetYokeBoneV);
	data.faceData.offsetYokeBoneZ = IniFile.GetValueAsInt(strSection, "offsetYokeBoneZ", data.faceData.offsetYokeBoneZ);
	data.faceData.rotateYokeBone = IniFile.GetValueAsInt(strSection, "rotateYokeBone", data.faceData.rotateYokeBone);
	data.faceData.scaleYokeBone = IniFile.GetValueAsInt(strSection, "scaleYokeBone", data.faceData.scaleYokeBone);
	
	//脸颊4
	strSection = "Cheek";
	data.faceData.offsetCheekH = IniFile.GetValueAsInt(strSection, "offsetCheekH", data.faceData.offsetCheekH);
	data.faceData.offsetCheekV = IniFile.GetValueAsInt(strSection, "offsetCheekV", data.faceData.offsetCheekV);
	data.faceData.offsetCheekZ = IniFile.GetValueAsInt(strSection, "offsetCheekZ", data.faceData.offsetCheekZ);
	data.faceData.scaleCheek = IniFile.GetValueAsInt(strSection, "scaleCheek", data.faceData.scaleCheek);

	//下巴4
	strSection = "Chain";
	data.faceData.offsetChainV = IniFile.GetValueAsInt(strSection, "offsetChainV", data.faceData.offsetChainV);
	data.faceData.offsetChainZ = IniFile.GetValueAsInt(strSection, "offsetChainZ", data.faceData.offsetChainZ);
	data.faceData.rotateChain = IniFile.GetValueAsInt(strSection, "rotateChain", data.faceData.rotateChain);
	data.faceData.scaleChainH = IniFile.GetValueAsInt(strSection, "scaleChainH", data.faceData.scaleChainH);

	//颌骨6
	strSection = "Jaw";
	data.faceData.offsetJawH = IniFile.GetValueAsInt(strSection, "offsetJawH", data.faceData.offsetJawH);
	data.faceData.offsetJawV = IniFile.GetValueAsInt(strSection, "offsetJawV", data.faceData.offsetJawV);
	data.faceData.offsetJawZ = IniFile.GetValueAsInt(strSection, "offsetJawZ", data.faceData.offsetJawZ);
	data.faceData.scaleJawSpecial = IniFile.GetValueAsInt(strSection, "scaleJawSpecial", data.faceData.scaleJawSpecial);
	data.faceData.scaleJawH = IniFile.GetValueAsInt(strSection, "scaleJawH", data.faceData.scaleJawH);
	data.faceData.scaleJawV = IniFile.GetValueAsInt(strSection, "scaleJawV", data.faceData.scaleJawV);

	//眼睛18
	strSection = "Eye";
	data.faceData.idEyeBaseTex = IniFile.GetValueAsInt(strSection, "idEyeBaseTex", data.faceData.idEyeBaseTex);
	data.faceData.idEyeHighTex = IniFile.GetValueAsInt(strSection, "idEyeHighTex", data.faceData.idEyeHighTex);
	data.faceData.idEyeBallTex = IniFile.GetValueAsInt(strSection, "idEyeBallTex", data.faceData.idEyeBallTex);
	data.faceData.idEyeShape = IniFile.GetValueAsInt(strSection, "idEyeShape", data.faceData.idEyeShape);
	data.faceData.scaleEyeH = IniFile.GetValueAsInt(strSection, "scaleEyeH", data.faceData.scaleEyeH);
	data.faceData.scaleEyeV = IniFile.GetValueAsInt(strSection, "scaleEyeV", data.faceData.scaleEyeV);
	data.faceData.rotateEye = IniFile.GetValueAsInt(strSection, "rotateEye", data.faceData.rotateEye);
	data.faceData.offsetEyeH = IniFile.GetValueAsInt(strSection, "offsetEyeH", data.faceData.offsetEyeH);
	data.faceData.offsetEyeV = IniFile.GetValueAsInt(strSection, "offsetEyeV", data.faceData.offsetEyeV);
	data.faceData.offsetEyeZ = IniFile.GetValueAsInt(strSection, "offseteyeZ", data.faceData.offsetEyeZ);
	data.faceData.scaleEyeBall = IniFile.GetValueAsInt(strSection, "scaleEyeBall", data.faceData.scaleEyeBall);

	data.faceData.scaleEyeH2 = IniFile.GetValueAsInt(strSection, "scaleEyeH2", data.faceData.scaleEyeH2);
	data.faceData.scaleEyeV2 = IniFile.GetValueAsInt(strSection, "scaleEyeV2", data.faceData.scaleEyeV2);
	data.faceData.rotateEye2= IniFile.GetValueAsInt(strSection, "rotateEye2", data.faceData.rotateEye2);
	data.faceData.offsetEyeH2 = IniFile.GetValueAsInt(strSection, "offsetEyeH2", data.faceData.offsetEyeH2);
	data.faceData.offsetEyeV2 = IniFile.GetValueAsInt(strSection, "offsetEyeV2", data.faceData.offsetEyeV2);
	data.faceData.offsetEyeZ2 = IniFile.GetValueAsInt(strSection, "offseteyeZ2", data.faceData.offsetEyeZ2);
	data.faceData.scaleEyeBall2 = IniFile.GetValueAsInt(strSection, "scaleEyeBall2", data.faceData.scaleEyeBall2);
	
	if(!MakeSureCustomizeData(DT_FACE_SHAPE_ESSENCE, data.faceData.idEyeShape, 1))
	{
		IniFile.Close();
		return false;
	}
	if(!MakeSureCustomizeData(DT_FACE_TEXTURE_ESSENCE, data.faceData.idEyeBaseTex, 1))
	{
		IniFile.Close();
		return false;
	}
	
	if(!MakeSureCustomizeData(DT_FACE_TEXTURE_ESSENCE, data.faceData.idEyeHighTex, 2))
	{
		IniFile.Close();
		return false;
	}
	if(!MakeSureCustomizeData(DT_FACE_TEXTURE_ESSENCE, data.faceData.idEyeBallTex, 6))
	{
		IniFile.Close();
		return false;
	}


	//眉毛14
	strSection = "Brow";
	data.faceData.idBrowTex = IniFile.GetValueAsInt(strSection, "idBrowTex", data.faceData.idBrowTex);
	data.faceData.idBrowShape = IniFile.GetValueAsInt(strSection, "idBrowShape", data.faceData.idBrowShape);
	data.faceData.scaleBrowH = IniFile.GetValueAsInt(strSection, "scaleBrowH", data.faceData.scaleBrowH);
	data.faceData.scaleBrowV = IniFile.GetValueAsInt(strSection, "scaleBrowV", data.faceData.scaleBrowV);
	data.faceData.rotateBrow = IniFile.GetValueAsInt(strSection, "rotateBrow", data.faceData.rotateBrow);
	data.faceData.offsetBrowH = IniFile.GetValueAsInt(strSection, "offsetBrowH", data.faceData.offsetBrowH);
	data.faceData.offsetBrowV = IniFile.GetValueAsInt(strSection, "offsetBrowV", data.faceData.offsetBrowV);
	data.faceData.offsetBrowZ = IniFile.GetValueAsInt(strSection, "offsetBrowZ", data.faceData.offsetBrowZ);
	
	data.faceData.scaleBrowH2 = IniFile.GetValueAsInt(strSection, "scaleBrowH2", data.faceData.scaleBrowH2);
	data.faceData.scaleBrowV2 = IniFile.GetValueAsInt(strSection, "scaleBrowV2", data.faceData.scaleBrowV2);
	data.faceData.rotateBrow2 = IniFile.GetValueAsInt(strSection, "rotateBrow2", data.faceData.rotateBrow2);
	data.faceData.offsetBrowH2 = IniFile.GetValueAsInt(strSection, "offsetBrowH2", data.faceData.offsetBrowH2);
	data.faceData.offsetBrowV2 = IniFile.GetValueAsInt(strSection, "offsetBrowV2", data.faceData.offsetBrowV2);
	data.faceData.offsetBrowZ2 = IniFile.GetValueAsInt(strSection, "offsetBrowZ2", data.faceData.offsetBrowZ2);

	if(!MakeSureCustomizeData(DT_FACE_SHAPE_ESSENCE,data.faceData.idBrowShape, 2))
	{
		IniFile.Close();
		return false;
	}
	if(!MakeSureCustomizeData(DT_FACE_TEXTURE_ESSENCE, data.faceData.idBrowTex, 3))
	{
		IniFile.Close();
		return false;
	}

	//鼻子9
	strSection = "Nose";
	data.faceData.idNoseTex = IniFile.GetValueAsInt(strSection, "idNoseTex", data.faceData.idNoseTex);
	data.faceData.idNoseTipShape = IniFile.GetValueAsInt(strSection, "idNoseTipShape", data.faceData.idNoseTipShape);
	data.faceData.scaleNoseTipH = IniFile.GetValueAsInt(strSection, "scaleNoseTipH", data.faceData.scaleNoseTipH);
	data.faceData.scaleNoseTipV = IniFile.GetValueAsInt(strSection, "scaleNoseTipV", data.faceData.scaleNoseTipV);
	data.faceData.scaleNoseTipZ = IniFile.GetValueAsInt(strSection, "scaleNoseTipZ", data.faceData.scaleNoseTipZ);
	data.faceData.offsetNoseTipV = IniFile.GetValueAsInt(strSection, "offsetNoseTipV", data.faceData.offsetNoseTipV);
	data.faceData.idNoseBridgeShape = IniFile.GetValueAsInt(strSection, "idNoseBridgeShape", data.faceData.idNoseBridgeShape);
	data.faceData.scaleBridgeTipH = IniFile.GetValueAsInt(strSection, "scaleBridgeTipH", data.faceData.scaleBridgeTipH);
	data.faceData.offsetBridgeTipZ = IniFile.GetValueAsInt(strSection, "OffsetBridgeTipZ", data.faceData.offsetBridgeTipZ);

	if(!MakeSureCustomizeData(DT_FACE_SHAPE_ESSENCE,data.faceData.idNoseTipShape, 3))
	{
		IniFile.Close();
		return false;
	}

	if(!MakeSureCustomizeData(DT_FACE_SHAPE_ESSENCE,data.faceData.idNoseBridgeShape, 4))
	{
		IniFile.Close();
		return false;
	}

	if(!MakeSureCustomizeData(DT_FACE_TEXTURE_ESSENCE, data.faceData.idNoseTex, 5))
	{
		IniFile.Close();
		return false;
	}
	

	//嘴11
	strSection = "Mouth";
	data.faceData.idMouthUpLipLine = IniFile.GetValueAsInt (strSection, "idMouthUpLipLine", data.faceData.idMouthUpLipLine);
	data.faceData.idMouthMidLipLine = IniFile.GetValueAsInt(strSection, "idMouthMidLipLine", data.faceData.idMouthMidLipLine);
	data.faceData.idMouthDownLipLine = IniFile.GetValueAsInt(strSection, "idMouthDownLipLine", data.faceData.idMouthDownLipLine);
	data.faceData.thickUpLip = IniFile.GetValueAsInt(strSection, "thickUpLip", data.faceData.thickUpLip);
	data.faceData.thickDownLip = IniFile.GetValueAsInt(strSection, "thickDownLip", data.faceData.thickDownLip);
	data.faceData.scaleMouthH = IniFile.GetValueAsInt(strSection, "scaleMouthH", data.faceData.scaleMouthH);
	data.faceData.offsetMouthV = IniFile.GetValueAsInt(strSection, "offsetMouthV", data.faceData.offsetMouthV);
	data.faceData.offsetMouthZ = IniFile.GetValueAsInt(strSection, "offsetMOuthZ", data.faceData.offsetMouthZ);
	data.faceData.idMouthTex = IniFile.GetValueAsInt(strSection, "idMouthTex", data.faceData.idMouthTex);
	data.faceData.offsetCornerOfMouthSpecial = IniFile.GetValueAsInt(strSection, "offsetCornerOfMouthSpecial", data.faceData.offsetCornerOfMouthSpecial);

	data.faceData.scaleMouthH2 = IniFile.GetValueAsInt(strSection, "scaleMouthH2", data.faceData.scaleMouthH2);
	data.faceData.offsetCornerOfMouthSpecial2 = IniFile.GetValueAsInt(strSection, "offsetCornerOfMouthSpecial2", data.faceData.offsetCornerOfMouthSpecial2);

	if(!MakeSureCustomizeData(DT_FACE_SHAPE_ESSENCE,data.faceData.idMouthUpLipLine, 5))
	{
		IniFile.Close();
		return false;
	}

	if(!MakeSureCustomizeData(DT_FACE_SHAPE_ESSENCE,data.faceData.idMouthMidLipLine, 6))
	{
		IniFile.Close();
		return false;
	}

	if(!MakeSureCustomizeData(DT_FACE_SHAPE_ESSENCE,data.faceData.idMouthDownLipLine, 7))
	{
		IniFile.Close();
		return false;
	}

	if(!MakeSureCustomizeData(DT_FACE_TEXTURE_ESSENCE, data.faceData.idMouthTex, 4))
	{
		IniFile.Close();
		return false;
	}
	
	//耳朵3
	strSection = "Ear";
	data.faceData.idEarShape = IniFile.GetValueAsInt(strSection, "idEarShape", data.faceData.idEarShape);
	data.faceData.scaleEar = IniFile.GetValueAsInt(strSection, "scaleEar", data.faceData.scaleEar);
	data.faceData.offsetEarV = IniFile.GetValueAsInt(strSection, "offsetEarV", data.faceData.offsetEarV);

	if(!MakeSureCustomizeData(DT_FACE_SHAPE_ESSENCE,data.faceData.idEarShape, 8))
	{
		IniFile.Close();
		return false;
	}


	//头发1
	strSection = "Hair";
	data.faceData.idHairModel = IniFile.GetValueAsInt(strSection, "idHairModel", data.faceData.idHairModel);
	data.faceData.idHairTex = IniFile.GetValueAsInt(strSection, "idHairTex", data.faceData.idHairTex);

	if(!MakeSureCustomizeData(DT_FACE_TEXTURE_ESSENCE,data.faceData.idHairTex, 8))
	{
		IniFile.Close();
		return false;
	}

	if(!MakeSureCustomizeData(DT_FACE_HAIR_ESSENCE,data.faceData.idHairModel))
	{
		IniFile.Close();
		return false;
	}

	//胡子2
	strSection = "Moustache";
	data.faceData.idMoustacheTex = IniFile.GetValueAsInt(strSection, "idMoustacheTex", data.faceData.idMoustacheTex);
	data.faceData.idMoustacheSkin = IniFile.GetValueAsInt(strSection, "idMoustacheSkin", data.faceData.idMoustacheSkin);
	data.faceData.idGoateeTex = IniFile.GetValueAsInt(strSection, "idGoateeTex", data.faceData.idGoateeTex);

	//检查男性胡须
	if( m_pCurPlayer->GetGender() == 0)
	{

		if(!MakeSureCustomizeData(DT_FACE_TEXTURE_ESSENCE,data.faceData.idMoustacheTex, 7))
		{
			IniFile.Close();
			return false;
		}

		if(!MakeSureCustomizeData(DT_FACE_TEXTURE_ESSENCE,data.faceData.idGoateeTex, 9))
		{
			IniFile.Close();
			return false;
		}
		
		if(!MakeSureCustomizeData(DT_FACE_MOUSTACHE_ESSENCE,data.faceData.idMoustacheSkin))
		{
			IniFile.Close();
			return false;
		}
	
	}
	
	//各部分颜色7
	strSection = "Color";
	data.faceData.colorHair = IniFile.GetValueAsInt(strSection, "colorHair", data.faceData.colorHair);
	data.faceData.colorFace = IniFile.GetValueAsInt(strSection, "colorFace", data.faceData.colorFace);
	data.faceData.colorEye = IniFile.GetValueAsInt(strSection, "colorEye", data.faceData.colorEye);
	data.faceData.colorBrow = IniFile.GetValueAsInt(strSection, "colorBrow", data.faceData.colorBrow);
	data.faceData.colorMouth = IniFile.GetValueAsInt(strSection, "colorMouth", data.faceData.colorMouth);
	data.faceData.colorEyeBall = IniFile.GetValueAsInt(strSection, "colorEyeBall", data.faceData.colorEyeBall);
	data.faceData.colorMoustache = IniFile.GetValueAsInt(strSection, "colorMoustache", data.faceData.colorMoustache);

	// 身体参数
	strSection = "Body";
	data.bodyID = IniFile.GetValueAsInt(strSection, "bodyID", data.bodyID);
	data.colorBody = IniFile.GetValueAsInt(strSection, "colorBody", data.colorBody);
	data.headScale = IniFile.GetValueAsInt(strSection, "headScale", data.headScale);
	data.upScale = IniFile.GetValueAsInt(strSection, "upScale", data.upScale);
	data.waistScale = IniFile.GetValueAsInt(strSection, "waistScale", data.waistScale);
	data.armWidth = IniFile.GetValueAsInt(strSection, "armWidth", data.armWidth);
	data.legWidth = IniFile.GetValueAsInt(strSection, "legWidth", data.legWidth);
	data.breastScale = IniFile.GetValueAsInt(strSection, "breastScale", data.breastScale);
         
	IniFile.Close();
	
	m_pCurPlayer->ChangeCustomizeData(data);
	m_pCurFace = m_pCurPlayer->GetFaceModel();
	
	m_pCurFace->UpdateAll();
	return true;
}


//把眼睛重置为通常状态
void CECCustomize::ResetEyeToNormal(void)
{
	m_pSliderScaleEyeH->SetLevel (128);
	m_pSliderScaleEyeV->SetLevel (128);
	m_pSliderRotateEye->SetLevel (128);
	m_pSliderOffsetEyeH->SetLevel (128);
	m_pSliderOffsetEyeV->SetLevel (128);
	m_pSliderOffsetEyeZ->SetLevel (128);
	m_pSliderScaleEyeBall->SetLevel (128);

	m_nScaleEyeH = 128;
	m_nScaleEyeV = 128;
	m_nRotateEye = 128;
	m_nOffsetEyeH = 128;
	m_nOffsetEyeV = 128;
	m_nOffsetEyeZ = 128;
	m_nScaleEyeBall = 128;

	m_nScaleEyeH2 = 128;
	m_nScaleEyeV2 = 128;
	m_nRotateEye2 = 128;
	m_nOffsetEyeH2 = 128;
	m_nOffsetEyeV2 = 128;
	m_nOffsetEyeZ2 = 128;
	m_nScaleEyeBall2 = 128;

	//更新眼睛滚动条
	UpdateChooseEyeSlider();
}
	
//把眼睛重置为通常状态
void CECCustomize::ResetBrowToNormal(void)
{
	m_pSliderScaleBrowH->SetLevel(128); 
	m_pSliderScaleBrowV->SetLevel(128); 
	m_pSliderRotateBrow->SetLevel(128); 
	m_pSliderOffsetBrowH->SetLevel(128); 
	m_pSliderOffsetBrowV->SetLevel(128);  
	m_pSliderOffsetBrowZ->SetLevel(128); 
	
	m_nScaleBrowH = 128;
	m_nScaleBrowV = 128;
	m_nRotateBrow = 128;
	m_nOffsetBrowH = 128;
	m_nOffsetBrowV = 128;
	m_nOffsetBrowZ = 128;

	m_nScaleBrowH2 = 128;
	m_nScaleBrowV2 = 128;
	m_nRotateBrow2 = 128;
	m_nOffsetBrowH2 = 128;
	m_nOffsetBrowV2 = 128;
	m_nOffsetBrowZ2 = 128;

	UpdateChooseBrowSlider();

}
//把眼睛重置为通常状态
void CECCustomize::ResetEarToNormal(void)
{
	m_pSliderScaleEar->SetLevel(128);
	m_pSliderOffsetEarV->SetLevel(128);

	UpdateChooseEarSlider();
}


//设置个性化所有纹理
bool CECCustomize::SetFaceAllTexture(void)
{
	CECFace::FACE_CUSTOMIZEDATA* pFaceData = m_pCurFace->GetFaceData();
	int i;

	//设置当前眼睛基本纹理
	for( i = 0; i < m_pListBoxEyeBaseTex1->GetCount(); i++)
	{
		if( pFaceData->idEyeBaseTex == m_pListBoxEyeBaseTex1->GetItemData(i))
			m_pListBoxEyeBaseTex1->SetCurSel(i);
	}
	//设置当前眼睛基本纹理
	for( i = 0; i < m_pListBoxEyeBaseTex2->GetCount(); i++)
	{
		if( pFaceData->idEyeBaseTex == m_pListBoxEyeBaseTex2->GetItemData(i))
			m_pListBoxEyeBaseTex2->SetCurSel(i);
	}
	//设置当前眼睛基本纹理
	for( i = 0; i < m_pListBoxEyeBaseTex3->GetCount(); i++)
	{
		if( pFaceData->idEyeBaseTex == m_pListBoxEyeBaseTex3->GetItemData(i))
			m_pListBoxEyeBaseTex3->SetCurSel(i);
	}

	//设置当前眉毛纹理
	for( i = 0; i< m_pListBoxBrowTex->GetCount(); i++)
	{
		if( pFaceData->idBrowTex == m_pListBoxBrowTex->GetItemData(i))
			m_pListBoxBrowTex->SetCurSel(i);
	}

	//设置当前鼻子纹理
	for( i = 0; i< m_pListBoxBrowTex->GetCount(); i++)
	{
		if( pFaceData->idNoseTex == m_pListBoxNoseTex->GetItemData(i))
			m_pListBoxNoseTex->SetCurSel(i);
	}

	//设置当前眼睛高光纹理
	for( i = 0; i< m_pListBoxEyeKohl->GetCount(); i++)
	{
		if( pFaceData->idEyeHighTex == m_pListBoxEyeKohl->GetItemData(i))
			m_pListBoxEyeKohl->SetCurSel(i);
	}
	
	//设置当前小胡子纹理
	for( i = 0; i < m_pListBoxMoustache->GetCount(); i++)
	{
		if( pFaceData->idMoustacheTex == m_pListBoxMoustache->GetItemData(i))
			m_pListBoxMoustache->SetCurSel(i);
	}

	//设置当前头发纹理
	for( i = 0; i < m_pListBoxHairTex->GetCount(); i++)
	{
		if( pFaceData->idHairTex == m_pListBoxHairTex->GetItemData(i))
			m_pListBoxHairTex->SetCurSel(i);
	}

	//设置当前胡子纹理
	for( i = 0; i < m_pListBoxGoateeTex->GetCount(); i++)
	{
		if( pFaceData->idGoateeTex == m_pListBoxGoateeTex->GetItemData(i))
			m_pListBoxGoateeTex->SetCurSel(i);
	}

	return true;
}

//个性化所有形状
bool CECCustomize::SetFaceAllShape(void)
{
	CECFace::FACE_CUSTOMIZEDATA* pFaceData = m_pCurFace->GetFaceData();
	int i;
	

	//基本脸型控件的名称
	char pszCtrlName[100];
	PAUIIMAGEPICTURE pImagePic;
	
	//有5个基本脸型
	for( i = 1;i <= 5;i++)
	{
		sprintf(pszCtrlName, "E_Face%d", i);
		
		pImagePic = (PAUIIMAGEPICTURE)m_pWinChooseFace->GetDlgItem(pszCtrlName);
		
 		pImagePic->SetColor(m_clrUnSel);//设置为不选中
		if(pImagePic->GetData() == pFaceData->idFaceShape1 )
		{
			m_pImagePicCurFaceShape1 = pImagePic;//指定当前选中的脸型

			pImagePic->SetColor(m_clrSel);
		}
		if( pImagePic->GetData() == pFaceData->idFaceShape2)
		{
			m_pImagePicCurFaceShape2 = pImagePic;//指定当前选中的脸型

			pImagePic->SetColor(m_clrSel);
		}
	}
	//设置当前眼睛形状
	for( i = 0; i < m_pListBoxEyeShape1->GetCount(); i++)
	{
		if( pFaceData->idEyeShape == m_pListBoxEyeShape1->GetItemData(i))
			m_pListBoxEyeShape1->SetCurSel(i);
	}
	
	for( i = 0; i < m_pListBoxEyeShape2->GetCount(); i++)
	{
		if( pFaceData->idEyeShape == m_pListBoxEyeShape2->GetItemData(i))
			m_pListBoxEyeShape2->SetCurSel(i);
	}

	for( i = 0; i < m_pListBoxEyeShape3->GetCount(); i++)
	{
		if( pFaceData->idEyeShape == m_pListBoxEyeShape3->GetItemData(i))
			m_pListBoxEyeShape3->SetCurSel(i);
	}

	//设置当前眉型
	for( i = 0; i < m_pListBoxBrow->GetCount(); i++)
	{
		if( pFaceData->idBrowShape == m_pListBoxBrow->GetItemData(i))
			m_pListBoxBrow->SetCurSel(i);
	}

	//设置当前鼻型
	for( i = 0; i < m_pListBoxNoseTip->GetCount(); i++)
	{
		if( pFaceData->idNoseTipShape == m_pListBoxNoseTip->GetItemData(i))
			m_pListBoxNoseTip->SetCurSel(i);
	}
	
	for( i = 0; i < m_pListBoxNoseBridge->GetCount(); i++)
	{
		if( pFaceData->idNoseBridgeShape == m_pListBoxNoseBridge->GetItemData(i))
			m_pListBoxNoseTip->SetCurSel(i);
	}

	//设置当前嘴型
	for( i = 0; i < m_pListBoxUpLipLine->GetCount(); i++)
	{
		if( pFaceData->idMouthUpLipLine == m_pListBoxUpLipLine->GetItemData(i))
			m_pListBoxUpLipLine->SetCurSel(i);
	}
	for( i = 0; i < m_pListBoxMidLipLine->GetCount(); i++)
	{
		if( pFaceData->idMouthMidLipLine == m_pListBoxMidLipLine->GetItemData(i))
			m_pListBoxMidLipLine->SetCurSel(i);
	}
	for( i = 0; i < m_pListBoxDownLipLine->GetCount(); i++)
	{
		if( pFaceData->idMouthDownLipLine == m_pListBoxDownLipLine->GetItemData(i))
			m_pListBoxDownLipLine->SetCurSel(i);
	}

	//设置当前耳型
	for( i = 0; i < m_pListBoxEar->GetCount(); i ++)
	{
		if( pFaceData->idEarShape == m_pListBoxEar->GetItemData(i))
			m_pListBoxEar->SetCurSel(i);
	}

	return true;
}

//设置个性化所有模型
bool CECCustomize::SetFaceAllModel(void)
{
	CECFace::FACE_CUSTOMIZEDATA* pFaceData = m_pCurFace->GetFaceData();
	int i;
	
	//设置头发模型
	for( i = 0; i < m_pListBoxHair->GetCount(); i++)
	{
		if( pFaceData->idHairModel == m_pListBoxHair->GetItemData(i))
			m_pListBoxHair->SetCurSel(i);
	}
	
	//设置胡子模型
	for( i = 0; i < m_pListBoxGoatee->GetCount(); i++)
	{
		if( pFaceData->idMoustacheSkin == m_pListBoxGoatee->GetItemData(i))
			m_pListBoxGoatee->SetCurSel(i);
	}
	
	return true;
}

//设置个性化所有颜色
bool CECCustomize::SetFaceAllColor(void)
{
	CECFace::FACE_CUSTOMIZEDATA* pFaceData = m_pCurFace->GetFaceData();

	int nCurSeleted = m_pListBoxFaceParts->GetCurSel ();
	if( nCurSeleted >= 0 && nCurSeleted <m_pListBoxFaceParts->GetCount () )
	{	
		A3DCOLOR clrCur;
		switch( nCurSeleted)
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
	}
	
	return true;
}

//设置所有颜色位置
bool CECCustomize::SetAllColorPos(void)
{
	CECFace::FACE_CUSTOMIZEDATA* pFaceData = m_pCurFace->GetFaceData();

	if( m_pCurPlayer->GetProfession() == 4)//如果是妖兽则跳过
	{
		CalculateColorPos(0, pFaceData->colorEyeBall, m_EyeBallColorPos);
	}
	else
	{
		CalculateColorPos(0, pFaceData->colorFace, m_FaceColorPos);
		CalculateColorPos(1, pFaceData->colorEye, m_EyeColorPos);
		CalculateColorPos(2, pFaceData->colorBrow, m_BrowColorPos);
		CalculateColorPos(3, pFaceData->colorMouth, m_MouthColorPos);
		CalculateColorPos(4, pFaceData->colorHair, m_HairColorPos);
		CalculateColorPos(5, pFaceData->colorEyeBall, m_EyeBallColorPos);
		if( m_pCurPlayer->GetGender() == 0)//男性
		{
			CalculateColorPos(6, pFaceData->colorMoustache, m_MoustacheColorPos);
		}
	}
	
	return true;
}

//计算颜色所在位置
bool CECCustomize::CalculateColorPos(int nID, A3DCOLOR clrPart, POINT& colorPoint)
{

	A3DSurface* pA3DSurface;

	elementdataman* pElementDataMan;
	pElementDataMan= g_pGame->GetElementDataMan ();

	DATA_TYPE dtFaceData = DT_COLORPICKER_ESSENCE;
	int id = m_pListBoxFaceParts->GetItemData ( nID);
	COLORPICKER_ESSENCE* pColorPickerEssence = (COLORPICKER_ESSENCE*)pElementDataMan->get_data_ptr(id, ID_SPACE_FACE,dtFaceData);

	if( !pColorPickerEssence)
	{
		colorPoint.x = 0;
		colorPoint.y = 0;

		return false;
	}
	
	char* pszPicFileName = pColorPickerEssence->file_colorpicker;
	pszPicFileName +=9; //去掉"surface\\"
	A3DSurfaceMan* pA3DSurfaceMan = g_pGame->GetA3DEngine ()->GetA3DSurfaceMan ();

	if( !pA3DSurfaceMan->LoadCursorSurfaceFromFile ( pszPicFileName, 0, &pA3DSurface))
	{
		colorPoint.x = 0;
		colorPoint.y = 0;
		
		return false;
	}
	
	A3DFORMAT ColorFMT = pA3DSurface->GetFormat ();

	if(A3DFMT_A8R8G8B8 == ColorFMT)
	{
		int nPitch;
		unsigned int* pData = NULL;

		if(!pA3DSurface->LockRect ( NULL, (void**)&pData, &nPitch, 0))
		{
			colorPoint.x = 0;
			colorPoint.y = 0;
			return false;
		}
					
		pA3DSurface->UnlockRect();

		

		int i, j;
		for( j = 0; j< pA3DSurface->GetHeight(); j++ )
		{
			for( i = 0; i < pA3DSurface->GetWidth();i++)
			{
				if( pData[j*(nPitch/4) + i] == clrPart)
				{
					colorPoint.x = i;
					colorPoint.y = j;

					pA3DSurface->Release();

					return true;
				}
			}
		}

		
	}

	pA3DSurface->Release();
	colorPoint.x = 0;
	colorPoint.y = 0;
	return false;
}


//画颜色所在位置
bool CECCustomize::DrawColorPos(void)
{

	int nCurSeleted = m_pListBoxFaceParts->GetCurSel ();

	//当前颜色位置
	POINT ptColor;

	if( nCurSeleted >= 0 && nCurSeleted <m_pListBoxFaceParts->GetCount () )
	{	
	
		switch( m_pListBoxFaceParts->GetItemData(nCurSeleted, 1))
		{
		case 0://脸	
			
			ptColor = m_FaceColorPos;

			break;

		case 1://眼影
			
			ptColor = m_EyeColorPos;

			break;

		case 2://眉毛
			ptColor = m_BrowColorPos;

			break;

		case 3://嘴唇
			ptColor = m_MouthColorPos;

			break;

		case 4://头发
			ptColor = m_HairColorPos;

			break;

		case 5://眼球(瞳孔)
			ptColor = m_EyeBallColorPos;

			break;

		case 6://胡子
			ptColor = m_MouthColorPos;

			break;

		default:
			break;

		}
	}
	

	//画当前颜色位置
	POINT ptDlg = m_pWinChooseFacePainting->GetPos();
	POINT ColorPicPos = m_pImagePictureColorPic->GetPos();
	m_pPicColorPos->SetPos(ColorPicPos.x + ptColor.x - ptDlg.x,
	ColorPicPos.y + ptColor.y - ptDlg.y);
	//pDlg->ChangeFocus(m_pPicColorPos);
	return true;
}

//保存用户自定义数据
bool CECCustomize::SaveUserCustomizeData(void)
{
	CECPlayer::PLAYER_CUSTOMIZEDATA playerCustomizeData = m_pCurPlayer->GetCustomizeData();
	playerCustomizeData.faceData = *m_pCurFace->GetFaceData();

	m_pCurPlayer->ChangeCustomizeData(playerCustomizeData);
	const CECFace::FACE_CUSTOMIZEDATA& faceData = playerCustomizeData.faceData;

	//获取时间
	WORD wYear,wMonth, wDay, wDate, wHour, wMinute, wSecond;
	a_GetSystemTime(&wYear, &wMonth, &wDay, &wDate, &wHour, &wMinute, &wSecond);

	AIniFile IniFile;
	char szFile[256];
	sprintf(szFile, "userdata\\character\\Customize\\%d%d\\%d_%d_%d_%d_%d_%d.ini",m_pCurPlayer->GetProfession(),m_pCurPlayer->GetGender(), wYear, wMonth, wDay, wHour, wMinute, wSecond);
	char* szCustiomizeFileName = new char[256];
	strcpy(szCustiomizeFileName,szFile);

	//压入向量
	m_vecCustomizeFileName.push_back(szCustiomizeFileName);

	//设置工作路径
	ACHAR szWorkDir[MAX_PATH];
	GetModuleFileNameW(NULL, szWorkDir, MAX_PATH);
	ACHAR* pTemp = wcsrchr(szWorkDir, '\\');
	*(pTemp+1) = '\0';

	ACHAR szPath[256];
	wcscpy(szPath, szWorkDir);
	wcscat(szPath, _AL("userdata\\character\\Customize"));
	
	_wmkdir(szPath);

	
	ACHAR szPath2[256];
	wcscpy(szPath2, szWorkDir);
	swprintf(szPath,_AL("userdata\\character\\Customize\\%d%d"), m_pCurPlayer->GetProfession(),m_pCurPlayer->GetGender());
	wcscat(szPath2,szPath);
	
	_wmkdir(szPath2);
	
	IniFile.Open(szFile);

	//3庭3
	AString strSection = "3Parts";
	IniFile.WriteIntValue(strSection, "scaleUp", faceData.scaleUp);
	IniFile.WriteIntValue(strSection, "scaleMiddle", faceData.scaleMiddle);
	IniFile.WriteIntValue(strSection, "scaleDown", faceData.scaleDown);

	//脸型融合3
	strSection = "BlendFace";
	IniFile.WriteIntValue(strSection, "idFaceShape1", faceData.idFaceShape1);
	IniFile.WriteIntValue(strSection, "idFaceShape2", faceData.idFaceShape2);
	IniFile.WriteIntValue(strSection, "blendFaceShape", faceData.blendFaceShape);

	//脸盘调整3
	strSection = "Face";
	IniFile.WriteIntValue(strSection, "scaleFaceH", faceData.scaleFaceH);
	IniFile.WriteIntValue(strSection, "scaleFaceV", faceData.scaleFaceV);
	IniFile.WriteIntValue(strSection, "idFaceTex", faceData.idFaceTex);

	//法令
	strSection = "Faling";
	IniFile.WriteIntValue(strSection, "idFalingSkin", faceData.idFalingSkin);
	IniFile.WriteIntValue(strSection, "idFalingTex", faceData.idFalingTex);

	//额头5
	strSection = "Forehead";
	IniFile.WriteIntValue(strSection, "offsetForeheadH", faceData.offsetForeheadH);
	IniFile.WriteIntValue(strSection, "offsetForeheadV", faceData.offsetForeheadV);
	IniFile.WriteIntValue(strSection, "offsetForeheadZ", faceData.offsetForeheadZ);
	IniFile.WriteIntValue(strSection, "rotateForehead", faceData.rotateForehead);
	IniFile.WriteIntValue(strSection, "scaleForehead", faceData.scaleForehead);

	//颧骨5
	strSection = "YokeBone";
	IniFile.WriteIntValue(strSection, "offsetYokeBoneH", faceData.offsetYokeBoneH);
	IniFile.WriteIntValue(strSection, "offsetYokeBoneV", faceData.offsetYokeBoneV);
	IniFile.WriteIntValue(strSection, "offsetYokeBoneZ", faceData.offsetYokeBoneZ);
	IniFile.WriteIntValue(strSection, "rotateYokeBone", faceData.rotateYokeBone);
	IniFile.WriteIntValue(strSection, "scaleYokeBone", faceData.scaleYokeBone);
	
	//脸颊4
	strSection = "Cheek";
	IniFile.WriteIntValue(strSection, "offsetCheekH", faceData.offsetCheekH);
	IniFile.WriteIntValue(strSection, "offsetCheekV", faceData.offsetCheekV);
	IniFile.WriteIntValue(strSection, "offsetCheekZ", faceData.offsetCheekZ);
	IniFile.WriteIntValue(strSection, "scaleCheek", faceData.scaleCheek);

	//下巴4
	strSection = "Chain";
	IniFile.WriteIntValue(strSection, "offsetChainV", faceData.offsetChainV);
	IniFile.WriteIntValue(strSection, "offsetChainZ", faceData.offsetChainZ);
	IniFile.WriteIntValue(strSection, "rotateChain", faceData.rotateChain);
	IniFile.WriteIntValue(strSection, "scaleChainH", faceData.scaleChainH);

	//颌骨6
	strSection = "Jaw";
	IniFile.WriteIntValue(strSection, "offsetJawH", faceData.offsetJawH);
	IniFile.WriteIntValue(strSection, "offsetJawV", faceData.offsetJawV);
	IniFile.WriteIntValue(strSection, "offsetJawZ", faceData.offsetJawZ);
	IniFile.WriteIntValue(strSection, "scaleJawSpecial", faceData.scaleJawSpecial);
	IniFile.WriteIntValue(strSection, "scaleJawH", faceData.scaleJawH);
	IniFile.WriteIntValue(strSection, "scaleJawV", faceData.scaleJawV);

	//眼睛18
	strSection = "Eye";
	IniFile.WriteIntValue(strSection, "idEyeBaseTex", faceData.idEyeBaseTex);
	IniFile.WriteIntValue(strSection, "idEyeHighTex", faceData.idEyeHighTex);
	IniFile.WriteIntValue(strSection, "idEyeBallTex", faceData.idEyeBallTex);
	IniFile.WriteIntValue(strSection, "idEyeShape", faceData.idEyeShape);
	IniFile.WriteIntValue(strSection, "scaleEyeH", faceData.scaleEyeH);
	IniFile.WriteIntValue(strSection, "scaleEyeV", faceData.scaleEyeV);
	IniFile.WriteIntValue(strSection, "rotateEye", faceData.rotateEye);
	IniFile.WriteIntValue(strSection, "offsetEyeH", faceData.offsetEyeH);
	IniFile.WriteIntValue(strSection, "offsetEyeV", faceData.offsetEyeV);
	IniFile.WriteIntValue(strSection, "offseteyeZ", faceData.offsetEyeZ);
	IniFile.WriteIntValue(strSection, "scaleEyeBall", faceData.scaleEyeBall);

	IniFile.WriteIntValue(strSection, "scaleEyeH2", faceData.scaleEyeH2);
	IniFile.WriteIntValue(strSection, "scaleEyeV2", faceData.scaleEyeV2);
	IniFile.WriteIntValue(strSection, "rotateEye2", faceData.rotateEye2);
	IniFile.WriteIntValue(strSection, "offsetEyeH2", faceData.offsetEyeH2);
	IniFile.WriteIntValue(strSection, "offsetEyeV2", faceData.offsetEyeV2);
	IniFile.WriteIntValue(strSection, "offseteyeZ2", faceData.offsetEyeZ2);
	IniFile.WriteIntValue(strSection, "scaleEyeBall2", faceData.scaleEyeBall2);
	
	//眉毛8
	strSection = "Brow";
	IniFile.WriteIntValue(strSection, "idBrowTex", faceData.idBrowTex);
	IniFile.WriteIntValue(strSection, "idBrowShape", faceData.idBrowShape);
	IniFile.WriteIntValue(strSection, "scaleBrowH", faceData.scaleBrowH);
	IniFile.WriteIntValue(strSection, "scaleBrowV", faceData.scaleBrowV);
	IniFile.WriteIntValue(strSection, "rotateBrow", faceData.rotateBrow);
	IniFile.WriteIntValue(strSection, "offsetBrowH", faceData.offsetBrowH);
	IniFile.WriteIntValue(strSection, "offsetBrowV", faceData.offsetBrowV);
	IniFile.WriteIntValue(strSection, "offsetBrowZ", faceData.offsetBrowZ);

	IniFile.WriteIntValue(strSection, "scaleBrowH2", faceData.scaleBrowH2);
	IniFile.WriteIntValue(strSection, "scaleBrowV2", faceData.scaleBrowV2);
	IniFile.WriteIntValue(strSection, "rotateBrow2", faceData.rotateBrow2);
	IniFile.WriteIntValue(strSection, "offsetBrowH2", faceData.offsetBrowH2);
	IniFile.WriteIntValue(strSection, "offsetBrowV2", faceData.offsetBrowV2);
	IniFile.WriteIntValue(strSection, "offsetBrowZ2", faceData.offsetBrowZ2);

	//鼻子8
	strSection = "Nose";
	IniFile.WriteIntValue(strSection, "idNoseTex", faceData.idNoseTex);
	IniFile.WriteIntValue(strSection, "idNoseTipShape", faceData.idNoseTipShape);
	IniFile.WriteIntValue(strSection, "scaleNoseTipH", faceData.scaleNoseTipH);
	IniFile.WriteIntValue(strSection, "scaleNoseTipV", faceData.scaleNoseTipV);
	IniFile.WriteIntValue(strSection, "scaleNoseTipZ", faceData.scaleNoseTipZ);
	IniFile.WriteIntValue(strSection, "offsetNoseTipV", faceData.offsetNoseTipV);
	IniFile.WriteIntValue(strSection, "idNoseBridgeShape", faceData.idNoseBridgeShape);
	IniFile.WriteIntValue(strSection, "scaleBridgeTipH", faceData.scaleBridgeTipH);
	IniFile.WriteIntValue(strSection, "offsetBridgeTipZ", faceData.offsetBridgeTipZ);

	//嘴9
	strSection = "Mouth";
	IniFile.WriteIntValue (strSection, "idMouthUpLipLine", faceData.idMouthUpLipLine);
	IniFile.WriteIntValue(strSection, "idMouthMidLipLine", faceData.idMouthMidLipLine);
	IniFile.WriteIntValue(strSection, "idMouthDownLipLine", faceData.idMouthDownLipLine);
	IniFile.WriteIntValue(strSection, "thickUpLip", faceData.thickUpLip);
	IniFile.WriteIntValue(strSection, "thickDownLip", faceData.thickDownLip);
	IniFile.WriteIntValue(strSection, "scaleMouthH", faceData.scaleMouthH);
	IniFile.WriteIntValue(strSection, "offsetMouthV", faceData.offsetMouthV);
	IniFile.WriteIntValue(strSection, "offsetMOuthZ", faceData.offsetMouthZ);
	IniFile.WriteIntValue(strSection, "idMouthTex", faceData.idMouthTex);
	IniFile.WriteIntValue(strSection, "offsetCornerOfMouthSpecial", faceData.offsetCornerOfMouthSpecial);
	IniFile.WriteIntValue(strSection, "scaleMouthH2", faceData.scaleMouthH2);
	IniFile.WriteIntValue(strSection, "offsetCornerOfMouthSpecial2", faceData.offsetCornerOfMouthSpecial2);

	//耳朵3
	strSection = "Ear";
	IniFile.WriteIntValue(strSection, "idEarShape", faceData.idEarShape);
	IniFile.WriteIntValue(strSection, "scaleEar", faceData.scaleEar);
	IniFile.WriteIntValue(strSection, "offsetEarV", faceData.offsetEarV);
	
	//头发1
	strSection = "Hair";
	IniFile.WriteIntValue(strSection, "idHairModel", faceData.idHairModel);
	IniFile.WriteIntValue(strSection, "idHairTex", faceData.idHairTex);

	//胡子2
	strSection = "Moustache";
	IniFile.WriteIntValue(strSection, "idMoustacheTex", faceData.idMoustacheTex);
	IniFile.WriteIntValue(strSection, "idMoustacheSkin", faceData.idMoustacheSkin);
	IniFile.WriteIntValue(strSection, "idGoateeTex", faceData.idGoateeTex);

	//各部分颜色7
	strSection = "Color";
	IniFile.WriteIntValue(strSection, "colorHair", faceData.colorHair);
	IniFile.WriteIntValue(strSection, "colorFace", faceData.colorFace);
	IniFile.WriteIntValue(strSection, "colorEye", faceData.colorEye);
	IniFile.WriteIntValue(strSection, "colorBrow", faceData.colorBrow);
	IniFile.WriteIntValue(strSection, "colorMouth", faceData.colorMouth);
	IniFile.WriteIntValue(strSection, "colorEyeBall", faceData.colorEyeBall);
	IniFile.WriteIntValue(strSection, "colorMoustache", faceData.colorMoustache);

	//身体参数
	strSection = "Body";
	IniFile.WriteIntValue(strSection, "bodyID", playerCustomizeData.bodyID);
	IniFile.WriteIntValue(strSection, "colorBody", playerCustomizeData.colorBody);
	IniFile.WriteIntValue(strSection, "headScale", playerCustomizeData.headScale);
	IniFile.WriteIntValue(strSection, "upScale", playerCustomizeData.upScale);
	IniFile.WriteIntValue(strSection, "waistScale", playerCustomizeData.waistScale);
	IniFile.WriteIntValue(strSection, "armWidth", playerCustomizeData.armWidth);
	IniFile.WriteIntValue(strSection, "legWidth", playerCustomizeData.legWidth);
	IniFile.WriteIntValue(strSection, "breastScale", playerCustomizeData.breastScale);

	IniFile.Save(szFile);
	IniFile.Close();

	//把文件名插入预设数组
	ACHAR szCustomizeName[256];
	swprintf(szCustomizeName, _AL("%d_%d_%d_%d_%d_%d"),wYear, wMonth, wDay, wHour, wMinute, wSecond);

	//在列表结尾加入
	//int nIndex = m_pListBoxPreCustomize->GetCount();
	//m_pListBoxPreCustomize->InsertString(nIndex, szCustomizeName);
	//m_pListBoxPreCustomize->SetItemDataPtr(nIndex,szCustiomizeFileName);
	//m_pListBoxPreCustomize->SetCurSel(nIndex);

	//在列表开头加入
	m_pListBoxPreCustomize->InsertString(0, szCustomizeName);
	m_pListBoxPreCustomize->SetItemDataPtr(0,szCustiomizeFileName);
	m_pListBoxPreCustomize->SetCurSel(0);

	return true;
}

//载入用户自定义数据
bool CECCustomize::LoadUserCustomizeData(void)
{
	
	char szUserCustomizeDataDir[256];
	sprintf(szUserCustomizeDataDir, "userdata\\character\\Customize\\%d%d\\",m_pCurPlayer->GetProfession(),m_pCurPlayer->GetGender());

	char szUserCustomizeDataFilePath[256];//当前人物自定义数据
	if( szUserCustomizeDataDir[strlen(szUserCustomizeDataDir)-1] == '\\')
		sprintf(szUserCustomizeDataFilePath, "%s*.ini", szUserCustomizeDataDir);

	_finddata_t fdIni;//文件属性
	long lFileHandle = _findfirst( szUserCustomizeDataFilePath, &fdIni);//获得第一个文件的句柄
	if( lFileHandle == -1)
	{
		return true;
	}

	int nResult = 0;//保存findnext的返回值
	while( nResult != -1)
	{
		//得到完整的用户自定义数据文件名
		if( szUserCustomizeDataDir[strlen(szUserCustomizeDataDir) -1] =='\\')
			sprintf( szUserCustomizeDataFilePath, "%s%s", szUserCustomizeDataDir, fdIni.name );
		char szTempName[256];
		sprintf(szTempName, "%s", fdIni.name);

		char*pEnd = strstr(szTempName, ".ini");
		(*pEnd) = '\0';

		ACHAR szUserCustomizeDataName[256];

		
		int nLength = strlen(szTempName);
		int nWLength = MultiByteToWideChar(CP_ACP, 0, szTempName, nLength, szUserCustomizeDataName,nLength);
		szUserCustomizeDataName[nWLength]= _AL('\0');



		char* szCustiomizeFileName = new char[256];
		strcpy(szCustiomizeFileName,szUserCustomizeDataFilePath);
		m_vecCustomizeFileName.push_back(szCustiomizeFileName);
		
		//载入listbox 末尾插入
//		int nIndex = m_pListBoxPreCustomize->GetCount();
//		m_pListBoxPreCustomize->InsertString(nIndex, szUserCustomizeDataName);
//		m_pListBoxPreCustomize->SetItemDataPtr(nIndex, szCustiomizeFileName);//设置listbox数据文件指针
		
		//在listbox 开头插入
		m_pListBoxPreCustomize->InsertString(0, szUserCustomizeDataName);
		m_pListBoxPreCustomize->SetItemDataPtr(0, szCustiomizeFileName);//设置listbox数据文件指针
		
		//获取下一个用户自定义数据文件的句柄
		nResult = _findnext( lFileHandle, &fdIni);
	}
	
	//关闭句柄
	_findclose( lFileHandle);
	
	return true;
}

//移动摄像机
bool CECCustomize::Tick_MoveCamera()
{
	static float fDeltaDist = 0.02f;
	static float fDeltaDist2 = 0.002f;

	A3DCamera * pCamera = g_pGame->GetViewport()->GetA3DCamera();

	static float s_fMoveZDist = 0.0f;
	static float s_fMoveXDist = 0.0f;
	static float s_fMoveYDist = 0.0f;
	
	if( m_pBtnCameraMoveFront->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK)
	{
	

		if( s_fMoveZDist < 1.0f)
		{
			s_fMoveZDist += fDeltaDist;
			pCamera->MoveFront(fDeltaDist);
		}

	}
	
	if( m_pBtnCameraMoveBack->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK)
	{
	
		if( s_fMoveZDist > -1.0f)
		{
			s_fMoveZDist -= fDeltaDist;
			pCamera->MoveBack(fDeltaDist);
		}
			
	}

	
	if( m_pBtnCameraMoveLeft->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK)
	{
		if( m_pBtmChoosePreCustomize->IsPushed() || m_pBtmChooseStature->IsPushed())
		{
			if( s_fMoveXDist < 1.0f)
			{
				s_fMoveXDist += fDeltaDist;
				pCamera->MoveLeft(fDeltaDist);
			}
		}
		else
		{
			if( s_fMoveXDist < 1.0f)
			{
				s_fMoveXDist += fDeltaDist2;
				pCamera->MoveLeft(fDeltaDist2);
			}
		}
	}

	if( m_pBtnCameraMoveRight->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK)
	{
		if( m_pBtmChoosePreCustomize->IsPushed() || m_pBtmChooseStature->IsPushed())
		{
			if( s_fMoveXDist > -1.0f)
			{
				s_fMoveXDist -= fDeltaDist;
				pCamera->MoveRight(fDeltaDist);
			}
		}
		else
		{
			if( s_fMoveXDist > -1.0f)
			{
				s_fMoveXDist -= fDeltaDist2;
				pCamera->MoveRight(fDeltaDist2);
			}
		}
		
	}
	if( m_pBtnCameraMoveUp->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK)
	{
		if( m_pBtmChoosePreCustomize->IsPushed() || m_pBtmChooseStature->IsPushed())
		{
			if( s_fMoveYDist < 1.0f)
			{
				s_fMoveYDist += fDeltaDist;
				pCamera->Move(A3DVECTOR3(0.0f, fDeltaDist, 0.0f));
					
			}
		}
		else
		{
			if( s_fMoveYDist < 1.0f)
			{
				s_fMoveYDist += fDeltaDist2;
				pCamera->Move(A3DVECTOR3(0.0f, fDeltaDist2, 0.0f));
					
			}

		}
	}
	if( m_pBtnCameraMoveDown->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK)
	{
		if( m_pBtmChoosePreCustomize->IsPushed() || m_pBtmChooseStature->IsPushed())
		{
			if( s_fMoveYDist > -0.3f)
			{
				s_fMoveYDist -= fDeltaDist;
				pCamera->Move(A3DVECTOR3(0.0f, -fDeltaDist, 0.0f));
			}
		}
		else
		{
			if( s_fMoveYDist > -0.3f)
			{
				s_fMoveYDist -= fDeltaDist2;
				pCamera->Move(A3DVECTOR3(0.0f, -fDeltaDist2, 0.0f));
			}
		}
	}
	if( m_pBtnCameraTurnLeft->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK)
	{
		A3DMATRIX4 matRotate = RotateY(0.015f);

		A3DSkinModel * pSkinModel = m_pCurPlayer->GetA3DSkinModel();
		A3DVECTOR3 vecDir = pSkinModel->GetAbsoluteTM().GetRow(2);
		A3DVECTOR3 vecUp = pSkinModel->GetAbsoluteTM().GetRow(1);
		vecDir = a3d_VectorMatrix3x3(vecDir, matRotate);
		vecUp = a3d_VectorMatrix3x3(vecUp, matRotate);

		pSkinModel->SetDirAndUp(vecDir, vecUp);
	}

	if( m_pBtnCameraTurnRight->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK)
	{
		A3DMATRIX4 matRotate = RotateY(-0.015f);

		A3DSkinModel * pSkinModel = m_pCurPlayer->GetA3DSkinModel();
		A3DVECTOR3 vecDir = pSkinModel->GetAbsoluteTM().GetRow(2);
		A3DVECTOR3 vecUp = pSkinModel->GetAbsoluteTM().GetRow(1);
		vecDir = a3d_VectorMatrix3x3(vecDir, matRotate);
		vecUp = a3d_VectorMatrix3x3(vecUp, matRotate);

		pSkinModel->SetDirAndUp(vecDir, vecUp);
	}
		
	return true;
}

//载入用户自定义图片
bool CECCustomize::LoadUserPic(void)
{
	char szUserPicDir[256];
	sprintf(szUserPicDir, "userdata\\userpic\\*.*");

	_finddata_t fdIni;//文件属性
	long lFileHandle = _findfirst( szUserPicDir, &fdIni);//获得第一个文件的句柄
	if( lFileHandle == -1)
	{
		return true;
	}

	int nResult = 0;//保存findnext的返回值

	while( nResult != -1)
	{		
		char* szUserPicFileName = new char[256];
		sprintf(szUserPicFileName, "%s", fdIni.name);
		
		//如果是当前是目录文件,则跳过
		if( fdIni.attrib & 0x10)//16表示目录(0x10)
		{
			nResult = _findnext( lFileHandle, &fdIni);
			continue;
		}
	

		ACHAR szUserCustomizeDataName[256];

		int nIndex = m_pListBoxUserPic->GetCount();

		int nLength = strlen(szUserPicFileName);
		int nWLength = MultiByteToWideChar(CP_ACP, 0, szUserPicFileName, nLength, szUserCustomizeDataName,nLength);
		szUserCustomizeDataName[nWLength]= _AL('\0');	

		//载入图片
		A2DSprite* pA2DSprite = NULL;

		if(szUserPicFileName)
		{	
			g_pGame->GetA3DEngine()->GetA3DSurfaceMan()->SetFolderName("userdata/userpic");

			//变图标文件为A2DSprite文件
			pA2DSprite = new A2DSprite; //需要释放

			if( !pA2DSprite->Init ( g_pGame->GetA3DDevice(), szUserPicFileName, 0))
			{
				delete pA2DSprite;


				//获取下一个用户自定义数据文件的句柄
				nResult = _findnext( lFileHandle, &fdIni);

				g_pGame->GetA3DEngine()->GetA3DSurfaceMan()->SetFolderName("surfaces");
				
				continue;
			}
			//如果图片的高度乘上控件宽度与图片宽度比例之后依然大于屏幕高度则跳过该图片
			HWND hWnd = GetActiveWindow();
			RECT rectClient;
			GetClientRect(hWnd, & rectClient);
		
			m_nPicUserPicOriWidth = m_pPicUserPic->GetSize().cx;
			if( pA2DSprite->GetHeight()*( m_nPicUserPicOriWidth/ pA2DSprite->GetWidth()) > rectClient.bottom)
			{
				delete pA2DSprite;
				
				//获取下一个用户自定义数据文件的句柄
				nResult = _findnext( lFileHandle, &fdIni);

				g_pGame->GetA3DEngine()->GetA3DSurfaceMan()->SetFolderName("surfaces");
				
				continue;
			}

			pA2DSprite->Release();
			delete pA2DSprite;//删除A2DSprite对象


			m_pListBoxUserPic->InsertString(nIndex, szUserCustomizeDataName);

			g_pGame->GetA3DEngine()->GetA3DSurfaceMan()->SetFolderName("surfaces");

			m_pListBoxUserPic->SetItemDataPtr(nIndex, szUserPicFileName);//设置listbox数据文件指针
		}		
		
		//获取下一个用户自定义数据文件的句柄
		nResult = _findnext( lFileHandle, &fdIni);
	}
	
	//关闭句柄
	_findclose( lFileHandle);

	//显示第一张图片
	if( m_pListBoxUserPic->GetCount() > 0)
	{
		char* szUserPicFileName =(char*)m_pListBoxUserPic->GetItemDataPtr(0);

		g_pGame->GetA3DEngine()->GetA3DSurfaceMan()->SetFolderName("userdata/userpic");
		if( m_pA2DSpriteUserPic->Init(g_pGame->GetA3DDevice(), szUserPicFileName, 0))
		{
			m_pA2DSpriteUserPic->SetLinearFilter(true);//设置线性过滤

			
			
			//m_fA2DSpriteUserPicScale = m_fA2DSpriteUserPicOriScale = (float)sizeUserPicCtrler.cx / (float)m_pA2DSpriteUserPic->GetWidth();
			m_fA2DSpriteUserPicScale = m_fA2DSpriteUserPicOriScale = 250.0f / (float)m_pA2DSpriteUserPic->GetWidth();

			m_pA2DSpriteUserPic->SetScaleX(m_fA2DSpriteUserPicOriScale);
			m_pA2DSpriteUserPic->SetScaleY(m_fA2DSpriteUserPicOriScale);
			
			SIZE sizeUserPicCtrler;
			sizeUserPicCtrler.cx = 250;//控件原始大小
			sizeUserPicCtrler.cy =(int)(m_fA2DSpriteUserPicOriScale * m_pA2DSpriteUserPic->GetHeight()); 
			
			A3DRECT rectUserPic(0,0, m_pA2DSpriteUserPic->GetWidth(), m_pA2DSpriteUserPic->GetHeight());

			m_pA2DSpriteUserPic->ResetItems(1, &rectUserPic);

			m_nA2DSPriteUserPicItemPosX = 0;
			m_nA2DSPriteUserPicItemPosY =0;
			
			//设置AUIImagePicture尺寸
			
			m_pPicUserPic->SetSize(sizeUserPicCtrler.cx, sizeUserPicCtrler.cy);
			m_nPicUserPicOriWidth =  sizeUserPicCtrler.cx;
			m_nPicUserPicOriHeight = sizeUserPicCtrler.cy;

			SIZE dialogFrameSize = m_pWinUserPic->GetFrame()->GetSizeLimit();
			
			//设置控件的位置
			m_pPicUserPic->SetPos(dialogFrameSize.cx/2, dialogFrameSize.cy/2 + m_pLabelUserPic->GetSize().cy);

			m_pPicUserPic->SetCover( m_pA2DSpriteUserPic, 0);

			//设置图片窗口大小
			m_pWinUserPic->SetSize(sizeUserPicCtrler.cx + dialogFrameSize.cx, 
				sizeUserPicCtrler.cy + dialogFrameSize.cy + m_pBtnUserPicDrag->GetSize().cy + m_pLabelUserPic->GetSize().cy);


			//设置4个按钮的位置
			m_pBtnUserPicDrag->SetPos(m_pWinUserPic->GetSize().cx 
									  - m_pBtnUserPicDrag->GetSize().cx
									  - dialogFrameSize.cx/2, 
										m_pWinUserPic->GetSize().cy 
										- m_pBtnUserPicDrag->GetSize().cy
										- dialogFrameSize.cy/2);

			m_pBtnUserPicZoomIn->SetPos(m_pWinUserPic->GetSize().cx	- m_pBtnUserPicDrag->GetSize().cx 
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
	
	return true;	
}

//初始化成员变量
bool CECCustomize::InitVariable(void)
{
	//创建用户参考图片的A2DSprite的对象
	m_pA2DSpriteUserPic = new A2DSprite;

	//-------------------------------
	//初始化预设个性化计数
	m_nPreCustomsizeCount = 0;
	
	//-------------------------------
	//初始化
	m_fA2DSpriteUserPicScale = 0.0f;

	m_nPicUserPicOriWidth = 0;
	m_nPicUserPicOriHeight = 0;

	m_fPicUserPicScale = 1.0f;

	//鼠标在item上面的位置
	m_nMouseOnItemX = 0;
	m_nMouseOnItemY = 0;

	//鼠标在A2DSprite上面的位置
	m_nMouseOnA2DSpriteX = 0;
	m_nMouseOnA2DSpriteY = 0;

	//------------------------
	m_nScaleEyeH = 0;
	m_nScaleEyeV = 0;
	m_nRotateEye = 0;
	m_nOffsetEyeH = 0;
	m_nOffsetEyeV = 0;
	m_nOffsetEyeZ = 0;
	m_nScaleEyeBall = 0;

	m_nScaleEyeH2 = 0;
	m_nScaleEyeV2 = 0;
	m_nRotateEye2 = 0;
	m_nOffsetEyeH2 = 0;
	m_nOffsetEyeV2 = 0;
	m_nOffsetEyeZ2 = 0;
	m_nScaleEyeBall2 = 0;

	//----------------------
	m_nScaleBrowH = 0;
	m_nScaleBrowV = 0;
	m_nRotateBrow = 0;
	m_nOffsetBrowH = 0;
	m_nOffsetBrowV = 0;
	m_nOffsetBrowZ = 0;
	
	m_nScaleBrowH2 = 0;
	m_nScaleBrowV2 = 0;
	m_nRotateBrow2 = 0;
	m_nOffsetBrowH2 = 0;
	m_nOffsetBrowV2 = 0;
	m_nOffsetBrowZ2 = 0;
	

	return true;
}

//释放成员变量
bool CECCustomize::ReleaseVariable(void)
{
	//释放用户参考图片的A2DSprite的对象
	if( m_pA2DSpriteUserPic != NULL)
	{
		m_pA2DSpriteUserPic->Release();
		delete m_pA2DSpriteUserPic;
		m_pA2DSpriteUserPic = NULL;

	}

	return true;
}




//主要用于放大缩小图片
bool CECCustomize::Tick_WinUserPic()
{
	//缩小
	if( m_pBtnUserPicZoomIn->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK)
	{
		ZoomInUserPic();
	}

	//放大
	if( m_pBtnUserPicZoomOut->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK)
	{
		ZoomOutUserPic();
	}
	return true;
}

//放大图片
void CECCustomize::ZoomOutUserPic()
{
	if( m_fA2DSpriteUserPicScale < 2.0f)
	{
		
		m_fPicUserPicScale += 0.05f;//控件比例
		m_fA2DSpriteUserPicScale += (0.05f * m_fA2DSpriteUserPicOriScale);//图片比例
	
		m_pA2DSpriteUserPic->SetScaleX(m_fA2DSpriteUserPicScale);
		m_pA2DSpriteUserPic->SetScaleY(m_fA2DSpriteUserPicScale);

		int nPicUserPicWidth = (int)(m_nPicUserPicOriWidth * m_fPicUserPicScale);
		int nPicUserPicHeight = (int)(m_nPicUserPicOriHeight * m_fPicUserPicScale);
		int nPicUserPicX,nPicUserPicY;

		SIZE dialogFrameSize = m_pWinUserPic->GetFrame()->GetSizeLimit();

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
	}
}

//缩小图片
void CECCustomize::ZoomInUserPic()
{
	if( m_fA2DSpriteUserPicScale > 0.1f)
	{
		
		m_fPicUserPicScale -= 0.05f;//控件比例
		m_fA2DSpriteUserPicScale -= (0.05f * m_fA2DSpriteUserPicOriScale);//图片比例
	
		m_pA2DSpriteUserPic->SetScaleX(m_fA2DSpriteUserPicScale);
		m_pA2DSpriteUserPic->SetScaleY(m_fA2DSpriteUserPicScale);

		int nPicUserPicWidth = (int)(m_nPicUserPicOriWidth * m_fPicUserPicScale);
		int nPicUserPicHeight = (int)(m_nPicUserPicOriHeight * m_fPicUserPicScale);
		int nPicUserPicX,nPicUserPicY;


		SIZE dialogFrameSize = m_pWinUserPic->GetFrame()->GetSizeLimit();
		
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
	}
}

CECCustomize::CECCustomize()
{
	int i;
	m_pAUIManager = NULL;

	m_pCurPlayer = NULL;	
	m_pCurFace = NULL;

	m_pImagePicCurFaceShape1 = NULL;
	m_pImagePicCurFaceShape2 = NULL;

	for(i = 0;i < 5;i++)
	{
		m_pPicFace[i] = NULL;
	}

	//法令
	m_pListBoxFalingTex = NULL;

	m_pWinChooseFace = NULL;
	m_pWinChooseEye = NULL;
	m_pWinChooseEyebrow = NULL;
	m_pWinChooseNose = NULL;
	m_pWinChooseMouth = NULL;
	m_pWinChooseEar = NULL;
	m_pWinChooseHair = NULL;
	m_pWinChooseFacePainting = NULL;
	m_pWinAdvancedOption = NULL;
	m_pWinChooseStature = NULL;
	m_pWinChoosePreCustomize = NULL;

	m_clrUnSel = 0xffffffff;
	m_clrSel = 0xffffff10;

	m_pListBoxEyeBaseTex1 = NULL;
	m_pListBoxEyeBaseTex2  = NULL;
	m_pListBoxEyeBaseTex3 = NULL;

	m_pBtnChooseEyeBaseTex1 = NULL;
	m_pBtnChooseEyeBaseTex2 = NULL;
	m_pBtnChooseEyeBaseTex3 = NULL;

	m_pListBoxEyeShape1 = NULL;
	m_pListBoxEyeShape2 = NULL;
	m_pListBoxEyeShape3 = NULL;

	
	m_pCheckBoxEyeAdjust = NULL;
	m_pBtnLeftEye = NULL;
	m_pBtnRightEye = NULL;

	m_pSliderScaleEyeH = NULL;		//左眼水平比例调节
	m_pSliderScaleEyeV = NULL;		//左眼竖直比例调节
	m_pSliderRotateEye = NULL;		//左眼旋转调解，绕Z轴旋转
	m_pSliderOffsetEyeH = NULL;		//左眼水平移动
	m_pSliderOffsetEyeV = NULL;		//左眼垂直移动
	m_pSliderOffsetEyeZ = NULL;		//左眼上下移动
	m_pSliderScaleEyeBall = NULL;	//左眼球缩放

	m_pBtmChooseBaseEye1 = NULL;
	m_pBtmChooseBaseEye2 = NULL;
	m_pBtmChooseBaseEye3 = NULL;

	//用于保存左右眼的滚动条当前值
	m_nScaleEyeH = 0;
	m_nScaleEyeV = 0;
	m_nRotateEye = 0;
	m_nOffsetEyeH = 0;
	m_nOffsetEyeV = 0;
	m_nOffsetEyeZ = 0;
	m_nScaleEyeBall = 0;

	m_nScaleEyeH2 = 0;
	m_nScaleEyeV2 = 0;
	m_nRotateEye2 = 0;
	m_nOffsetEyeH2 = 0;
	m_nOffsetEyeV2 = 0;
	m_nOffsetEyeZ2 = 0;
	m_nScaleEyeBall2;
	
	
	for( i = 0; i< 7;i++)
	{
		m_pLabelEye[i] = NULL;
	}
	

	m_pListBoxBrowTex = NULL;//眉毛纹理列表框
	m_pListBoxBrow = NULL;//眉毛形状列表框
	
	m_pSliderScaleBrowH = NULL;	//眉毛的水平比例调节
	m_pSliderScaleBrowV = NULL; //眉毛的树枝比例调节
	m_pSliderRotateBrow = NULL; //眉毛旋转调节,绕Z轴旋转
	m_pSliderOffsetBrowH = NULL; //眉毛的水平位移
	m_pSliderOffsetBrowV = NULL; //眉毛的垂直位移
	m_pSliderOffsetBrowZ = NULL; //眉毛的前后位移

	//用于保存当前眉毛滚动条值
	m_nScaleBrowH =0;
	m_nScaleBrowV = 0;
	m_nRotateBrow = 0;
	m_nOffsetBrowH =0;
	m_nOffsetBrowV = 0;
	m_nOffsetBrowZ = 0;

	m_nScaleBrowH2 = 0;
	m_nScaleBrowV2 = 0;
	m_nRotateBrow2 = 0;
	m_nOffsetBrowH2 = 0;
	m_nOffsetBrowV2 = 0;
	m_nOffsetBrowZ2 = 0;

	m_pCheckBoxBrowAdjust= NULL;
	m_pBtnLeftBrow= NULL;
	m_pBtnRightBrow= NULL;

	for(i = 0; i< 6; i++)
	{
		m_pLabelBrow[i]= NULL;
	}

	m_pBtmChooseBaseEyebrow1= NULL;


	//--------------与鼻子调整相关的滚动条控件

	m_pListBoxNoseTex= NULL;//鼻子纹理list box
	m_pListBoxNoseTip= NULL;//鼻头形状list box
	m_pListBoxNoseBridge= NULL;//鼻梁形状list box
	
	m_pSliderScaleNoseTipH= NULL;//鼻头水平比例调节
	m_pSliderScaleNoseTipV= NULL;//鼻头垂直比例调节
	m_pSliderScaleNoseTipZ= NULL;//鼻头前后比例调节
	m_pSliderOffsetNoseTipV= NULL;//鼻头垂直位移调节
	m_pSliderScaleBridgeTipH= NULL;//鼻梁水平比例调节
	m_pSliderOffsetBridgeTipZ= NULL;//鼻梁前后移动调节
	
	m_pBtmChooseBaseNoseTip= NULL;
	m_pBtmChooseBaseNoseBridge= NULL;

	//--------------与嘴型调整相关的滚动条控件
	m_pListBoxUpLipLine= NULL;//上唇线list box
	m_pListBoxMidLipLine= NULL;//唇沟list box
	m_pListBoxDownLipLine= NULL;//下唇线list box

	m_pBtmChooseMouthUpLipLine= NULL;
	m_pBtmChooseMouthMidLipLine= NULL;
	m_pBtmChooseMouthDownLipLine= NULL;
	
	m_pSliderThickUpLip= NULL; //上嘴唇厚度调节
	m_pSliderThickDownLip= NULL;//下嘴唇厚度调节
	m_pSliderScaleMouthH= NULL;//嘴部整体水平比例调节
	m_pSliderOffsetMouthV= NULL;//嘴部整体垂直位移调节
	m_pSliderOffsetMouthZ= NULL;//嘴部整体沿Z轴的位移调节
	m_pSliderOffsetCornerOfMouthSpecial= NULL;//嘴角的调节
	m_pSliderScaleMouthH2= NULL;
	m_pSliderOffsetCornerOfMouthSpecial2= NULL;

	m_pCheckBoxScaleMouthH = NULL;
	m_pCheckBoxOffsetCornerOfMouthSpecial = NULL;

	//---------------与耳朵调节相关的滚动条控件

	m_pListBoxEar= NULL;//耳朵形状列表
	m_pBtmChooseBaseEar1= NULL;

	m_pSliderScaleEar= NULL; //耳朵的整体缩放
	m_pSliderOffsetEarV= NULL;//耳朵的上下移动

	//---------------与胡子头发调节相关的控件

	m_pListBoxHair= NULL;//头发列表框
	
	m_pListBoxMoustache= NULL;//小胡子列表控件
	
	m_pListBoxGoatee= NULL;//大胡子的列表控件
	
	m_uCurItem= 0;//当前item索引

	m_pListBoxHairTex= NULL;//头发纹理列表框

	m_pListBoxGoateeTex= NULL;//大胡子纹理列表框

	//----------------与脸部颜色相关的控件
	m_pListBoxFaceParts= NULL;
	m_pImagePictureColorPic= NULL;//用于显示当前颜色图片
	m_pImagePictureCurColor= NULL;

	m_pSliderOffsetForeheadH= NULL;	//额头的横移
	m_pSliderOffsetForeheadV= NULL;	//额头的纵移
	m_pSliderOffsetForeheadZ= NULL;	//额头的前后移
	m_pSliderRotateForehead= NULL;		//额头的旋转
	m_pSliderScaleForehead= NULL;		//额头的比例缩放
	
	m_pSliderOffsetYokeBoneH= NULL;	//颧骨的横移
	m_pSliderOffsetYokeBoneV= NULL;	//颧骨的纵移
	m_pSliderOffsetYokeBoneZ= NULL;	//颧骨的前后移动
	m_pSliderRotateYokeBone= NULL;		//颧骨的旋转
	m_pSliderScaleYokeBone= NULL;		//颧骨的比例缩放
	
	m_pSliderOffsetCheekH= NULL;		//脸颊的横移
	m_pSliderOffsetCheekV= NULL;		//脸颊的纵移
	m_pSliderOffsetCheekZ= NULL;		//脸颊的前后移动
	m_pSliderScaleCheek= NULL;			//脸颊的比例缩放

	m_pSliderOffsetChainV= NULL;		//下巴的纵移
	m_pSliderOffsetChainZ= NULL;		//下巴的前后移动
	m_pSliderRotateChain= NULL;		//下巴的旋?
	m_pSliderScaleChainH= NULL;		//下巴的左右比例缩放

	m_pSliderOffsetJawH= NULL;			//颌骨的横移
	m_pSliderOffsetJawV= NULL;			//颌骨的纵移
	m_pSliderOffsetJawZ= NULL;			//颌骨的前后移动
	m_pSliderScaleJawSpecial= NULL;	//颌骨的特殊缩放
	m_pSliderScaleJawH= NULL;			//颌骨的左右比例缩放
	m_pSliderScaleJawV= NULL;			//颌骨的上下比例缩放
	
	//---------------身材调节相关的滚动条控件
	m_pSliderHeadScale= NULL;			//头大小
	m_pSliderUpScale= NULL;			//上身健壮度
	m_pSliderWaistScale= NULL;			//腰部为度
	m_pSliderArmWidth= NULL;			//下肢围度
	m_pSliderLegWidth= NULL;			//下肢围度
	m_pSliderBreastScale= NULL;		//胸部高低
	m_pLabelBreastScale= NULL;			//胸部高低标签
	m_pStillImageButtomBreastScale= NULL;//胸部高低复位按钮

	//---------------与预定义相关的滚动条控件
	m_pListBoxPreCustomize= NULL;
	m_pListBoxUserPic= NULL;

	m_pBtnUserPic= NULL;

	m_pBtmChoosePreCustomize= NULL;
	m_pBtmChooseFace= NULL;
	m_pBtmChooseEye= NULL;
	m_pBtmChooseBrow= NULL;
	m_pBtmChooseNose= NULL;
	m_pBtmChooseMouth= NULL;
	m_pBtmChooseEar= NULL;
	m_pBtmChooseHair= NULL;
	m_pBtmChooseFacePainting= NULL;
	m_pBtmChooseStature= NULL;

	m_pBtmCurChoose= NULL;//当前按钮
	
	

		//------------------与移动摄像机相关-------------
	m_pWinMoveCamera = NULL;
	m_pBtnCameraMoveFront= NULL;
	m_pBtnCameraMoveBack= NULL;
	m_pBtnCameraMoveLeft= NULL;
	m_pBtnCameraMoveRight= NULL;
	
	m_pBtnCameraTurnLeft= NULL;
	m_pBtnCameraTurnRight= NULL;

	m_pBtnCameraMoveUp = NULL;
	m_pBtnCameraMoveDown = NULL;

	//------------------与用户参考图片相关------------
	m_pWinUserPic= NULL;
	m_pPicUserPic= NULL;
	m_pA2DSpriteUserPic= NULL;

	m_pBtnUserPicDrag= NULL;
	m_pBtnUserPicZoomIn= NULL;
	m_pBtnUserPicZoomOut= NULL;

	m_pSliderUserPicAlpha= NULL;
	m_pLabelUserPicAlpha= NULL;

	m_pLabelUserPic= NULL;

	m_pWinChooseExpression= NULL;

	m_pListBoxExp= NULL;

	for(i = 0; i< EBE_NUMBER; i++)
	{
		m_pPicBlendExp[i] = NULL;
	}

	for(i = 0;i< EAP_NUMBER;i++)
	{
		//表情参数滚动条
		m_pSliderBlendExp[i] = NULL;
	}

	//表情参数标签
	for(i = 0;i <EAP_NUMBER;i++)
	{
		m_pLabelBlendExp[i] = NULL;
	}

	
	m_pWinExpressionList = NULL;
	

	m_pListBoxExpressionList = NULL;

	

}



//确保每个slider的值都是可用的
int CECCustomize::MakeSureSlider(unsigned char &level, int nMax, int nMin)
{
	if( level < nMin)
	{
		level = nMin;
		
	}
	if( level > nMax)
	{
		level = nMax;
	
	}
	
	return level;
}



bool CECCustomize::SetAllsliderBound()
{
	m_pSliderScaleFaceH->SetTotal( m_pSliderBound->m_nScaleFaceHMax, m_pSliderBound->m_nScaleFaceHMin);
	m_pSliderScaleFaceV->SetTotal(m_pSliderBound->m_nScaleFaceVMax, m_pSliderBound->m_nScaleFaceVMin);
		
	m_pSliderPartUp->SetTotal(m_pSliderBound->m_nScaleUpPartMax, m_pSliderBound->m_nScaleUpPartMin);
	m_pSliderPartMiddle->SetTotal(m_pSliderBound->m_nScaleMiddlePartMax, m_pSliderBound->m_nScaleMiddlePartMin);
	m_pSliderPartDown->SetTotal(m_pSliderBound->m_nScaleDownPartMax, m_pSliderBound->m_nScaleDownPartMin);

	m_pSliderOffsetForeheadH->SetTotal(m_pSliderBound->m_nOffsetForeheadHMax, m_pSliderBound->m_nOffsetForeheadHMin);
	m_pSliderOffsetForeheadV->SetTotal(m_pSliderBound->m_nOffsetForeheadVMax, m_pSliderBound->m_nOffsetForeheadVMin);
	m_pSliderOffsetForeheadZ->SetTotal(m_pSliderBound->m_nOffsetForeheadZMax, m_pSliderBound->m_nOffsetForeheadZMin);
	m_pSliderRotateForehead->SetTotal(m_pSliderBound->m_nRotateForeheadMax, m_pSliderBound->m_nRotateForeheadMin);
	m_pSliderScaleForehead->SetTotal(m_pSliderBound->m_nScaleForeheadMax, m_pSliderBound->m_nScaleForeheadMin);

	m_pSliderOffsetYokeBoneH->SetTotal(m_pSliderBound->m_nOffsetYokeBoneHMax, m_pSliderBound->m_nOffsetYokeBoneHMin);
	m_pSliderOffsetYokeBoneV->SetTotal(m_pSliderBound->m_nOffsetYokeBoneVMax, m_pSliderBound->m_nOffsetYokeBoneVMin);
	m_pSliderOffsetYokeBoneZ->SetTotal(m_pSliderBound->m_nOffsetYokeBoneZMax, m_pSliderBound->m_nOffsetYokeBoneZMin);
	m_pSliderRotateYokeBone->SetTotal(m_pSliderBound->m_nRotateYokeBoneMax, m_pSliderBound->m_nRotateYokeBoneMin);
	m_pSliderScaleYokeBone->SetTotal(m_pSliderBound->m_nScaleYokeBoneMax, m_pSliderBound->m_nScaleYokeBoneMin);

	m_pSliderOffsetCheekH->SetTotal(m_pSliderBound->m_nOffsetCheekHMax, m_pSliderBound->m_nOffsetCheekHMin);
	m_pSliderOffsetCheekV->SetTotal(m_pSliderBound->m_nOffsetCheekVMax, m_pSliderBound->m_nOffsetCheekVMin);
	m_pSliderOffsetCheekZ->SetTotal(m_pSliderBound->m_nOffsetCheekZMax, m_pSliderBound->m_nOffsetCheekZMin);
	m_pSliderScaleCheek->SetTotal(m_pSliderBound->m_nScaleCheekMax, m_pSliderBound->m_nScaleCheekMin);

	m_pSliderOffsetChainZ->SetTotal(m_pSliderBound->m_nOffsetChainZMax, m_pSliderBound->m_nOffsetChainZMin);
	m_pSliderOffsetChainV->SetTotal(m_pSliderBound->m_nOffsetChainVMax, m_pSliderBound->m_nOffsetChainVMin);
	m_pSliderRotateChain->SetTotal(m_pSliderBound->m_nRotateChainMax, m_pSliderBound->m_nRotateChainMin);
	m_pSliderScaleChainH->SetTotal(m_pSliderBound->m_nScaleChainHMax, m_pSliderBound->m_nScaleChainHMin);
	
	m_pSliderOffsetJawH->SetTotal(m_pSliderBound->m_nOffsetJawHMax, m_pSliderBound->m_nOffsetJawHMin);
	m_pSliderOffsetJawV->SetTotal(m_pSliderBound->m_nOffsetJawVMax, m_pSliderBound->m_nOffsetJawVMin);
	m_pSliderOffsetJawZ->SetTotal(m_pSliderBound->m_nOffsetJawZMax, m_pSliderBound->m_nOffsetJawZMin);
	m_pSliderScaleJawSpecial->SetTotal(m_pSliderBound->m_nScaleJawSpecialMax, m_pSliderBound->m_nScaleJawSpecialMin);
	m_pSliderScaleJawH->SetTotal(m_pSliderBound->m_nScaleJawHMax, m_pSliderBound->m_nScaleJawHMin);
	m_pSliderScaleJawV->SetTotal(m_pSliderBound->m_nScaleJawVMax, m_pSliderBound->m_nScaleJawVMin);
	
	m_pSliderScaleEyeH->SetTotal(m_pSliderBound->m_nScaleEyeHMax, m_pSliderBound->m_nScaleEyeHMin);
	m_pSliderScaleEyeV->SetTotal(m_pSliderBound->m_nScaleEyeVMax, m_pSliderBound->m_nScaleEyeVMin);
	m_pSliderRotateEye->SetTotal(m_pSliderBound->m_nRotateEyeMax, m_pSliderBound->m_nRotateEyeMin);
	m_pSliderOffsetEyeH->SetTotal(m_pSliderBound->m_nOffsetEyeHMax, m_pSliderBound->m_nOffsetEyeHMin);
	m_pSliderOffsetEyeV->SetTotal(m_pSliderBound->m_nOffsetEyeVMax, m_pSliderBound->m_nOffsetEyeVMin);
	m_pSliderOffsetEyeZ->SetTotal(m_pSliderBound->m_nOffsetEyeZMax, m_pSliderBound->m_nOffsetEyeZMin);
	m_pSliderScaleEyeBall->SetTotal(m_pSliderBound->m_nScaleEyeBallMax, m_pSliderBound->m_nScaleEyeBallMin);

	m_pSliderScaleBrowH->SetTotal(m_pSliderBound->m_nScaleBrowHMax, m_pSliderBound->m_nScaleBrowHMin);
	m_pSliderScaleBrowV->SetTotal(m_pSliderBound->m_nScaleBrowVMax, m_pSliderBound->m_nScaleBrowVMin);
	m_pSliderRotateBrow->SetTotal(m_pSliderBound->m_nRotateBrowMax, m_pSliderBound->m_nRotateBrowMin);
	m_pSliderOffsetBrowH->SetTotal(m_pSliderBound->m_nOffsetBrowHMax, m_pSliderBound->m_nOffsetBrowHMin);
	m_pSliderOffsetBrowV->SetTotal(m_pSliderBound->m_nOffsetBrowVMax, m_pSliderBound->m_nOffsetBrowVMin);
	m_pSliderOffsetBrowZ->SetTotal(m_pSliderBound->m_nOffsetBrowZMax, m_pSliderBound->m_nOffsetBrowZMin);

	m_pSliderScaleNoseTipH->SetTotal(m_pSliderBound->m_nScaleNoseTipHMax, m_pSliderBound->m_nScaleNoseTipHMin);
	m_pSliderScaleNoseTipV->SetTotal(m_pSliderBound->m_nScaleNoseTipVMax, m_pSliderBound->m_nScaleNoseTipVMin);
	m_pSliderOffsetNoseTipV->SetTotal(m_pSliderBound->m_nOffsetNoseTipVMax, m_pSliderBound->m_nOffsetNoseTipVMin);
	m_pSliderScaleNoseTipZ->SetTotal(m_pSliderBound->m_nScaleNoseTipZMax, m_pSliderBound->m_nScaleNoseTipZMin);
	m_pSliderScaleBridgeTipH->SetTotal(m_pSliderBound->m_nScaleBridgeTipHMax, m_pSliderBound->m_nScaleBridgeTipHMin);
	m_pSliderOffsetBridgeTipZ->SetTotal(m_pSliderBound->m_nOffsetBridgeTipZMax, m_pSliderBound->m_nOffsetBridgeTipZMin);

	m_pSliderThickUpLip->SetTotal(m_pSliderBound->m_nThickUpLipMax, m_pSliderBound->m_nThickUpLipMin);
	m_pSliderThickDownLip->SetTotal(m_pSliderBound->m_nThickDownLipMax, m_pSliderBound->m_nThickDownLipMin);
	m_pSliderScaleMouthH->SetTotal(m_pSliderBound->m_nScaleMouthHMax, m_pSliderBound->m_nScaleMouthHMin);
	m_pSliderOffsetMouthV->SetTotal(m_pSliderBound->m_nOffsetMouthVMax, m_pSliderBound->m_nOffsetMouthVMin);
	m_pSliderOffsetMouthZ->SetTotal(m_pSliderBound->m_nOffsetMouthZMax, m_pSliderBound->m_nOffsetMouthZMin);
	m_pSliderOffsetCornerOfMouthSpecial->SetTotal(m_pSliderBound->m_nOffsetCornerOfMouthSpecialMax, m_pSliderBound->m_nOffsetCornerOfMouthSpecialMin);
	m_pSliderScaleMouthH2->SetTotal(m_pSliderBound->m_nScaleMouthH2Max, m_pSliderBound->m_nScaleMouthH2Min);
	m_pSliderOffsetCornerOfMouthSpecial2->SetTotal(m_pSliderBound->m_nOffsetCornerOfMouthSpecial2Max, m_pSliderBound->m_nOffsetCornerOfMouthSpecial2Min);

	m_pSliderScaleEar->SetTotal(m_pSliderBound->m_nScaleEarMax, m_pSliderBound->m_nScaleEarMin);
	m_pSliderOffsetEarV->SetTotal(m_pSliderBound->m_nOffsetEarVMax, m_pSliderBound->m_nOffsetEarVMin);

	m_pSliderHeadScale->SetTotal(m_pSliderBound->m_nHeadScaleMax, m_pSliderBound->m_nHeadScaleMin);
	m_pSliderUpScale->SetTotal(m_pSliderBound->m_nUpScaleMax, m_pSliderBound->m_nUpScaleMin);
	m_pSliderWaistScale->SetTotal(m_pSliderBound->m_nWaistScaleMax, m_pSliderBound->m_nWaistScaleMin);
	m_pSliderArmWidth->SetTotal(m_pSliderBound->m_nArmWidthMax, m_pSliderBound->m_nArmWidthMin);
	m_pSliderLegWidth->SetTotal(m_pSliderBound->m_nLegWidthMax, m_pSliderBound->m_nLegWidthMin);
	m_pSliderBreastScale->SetTotal(m_pSliderBound->m_nBreastScaleMax, m_pSliderBound->m_nBreastScaleMin);

	return true;
}


//确认个性化数据是否合法
//形状part:0-脸型、1-眼型、2-眉型、3-鼻头、4-鼻梁、5-上唇线、6-唇沟、7-下唇线、8-耳型
//纹理part: 0-脸、1-眼皮、2-眼影、3-眉、4-唇、5-鼻、6-眼珠、7-小胡子、8-头发、9-大胡子、10-法令
bool CECCustomize::MakeSureCustomizeData( DATA_TYPE dt, unsigned int id, unsigned int part)
{
	//妖兽不判断
	if( m_pCurPlayer->GetProfession() == 4)
		return true;

	elementdataman* pElementDataMan;
	pElementDataMan= g_pGame->GetElementDataMan ();
	DATA_TYPE dtFaceData;

	//脸部形状
	if( dt == DT_FACE_SHAPE_ESSENCE)
	{
		FACE_SHAPE_ESSENCE* pEssence = (FACE_SHAPE_ESSENCE*)pElementDataMan->get_data_ptr(id, ID_SPACE_FACE,dtFaceData);

		if( dtFaceData !=dt)
		{
			return false;
		}

//		if((m_pCurPlayer->GetProfession() != pEssence->character_id)|| (m_pCurPlayer->GetGender() != pEssence->gender_id))
//		{
		if( m_pCurPlayer->GetGender() != (int)pEssence->gender_id)
		{
			return false;
		}
		if( part != pEssence->shape_part_id)
		{
			return false;
		}
	}

	//脸部纹理
	if( dt == DT_FACE_TEXTURE_ESSENCE)
	{
		if( id == 0)
			return true;

		FACE_TEXTURE_ESSENCE* pEssence = (FACE_TEXTURE_ESSENCE*)pElementDataMan->get_data_ptr(id, ID_SPACE_FACE,dtFaceData);

		if( dtFaceData !=dt)
		{
			return false;
		}

//		if((m_pCurPlayer->GetProfession()!= pEssence->character_id)|| (m_pCurPlayer->GetGender() != pEssence->gender_id))
//		{
		if( m_pCurPlayer->GetGender() != (int)pEssence->gender_id)
		{
			return false;
		}
		
		if( part != pEssence->tex_part_id)
		{
			return false;
		}

	}
	
	//头发模型
	if( dt == DT_FACE_HAIR_ESSENCE)
	{
		FACE_HAIR_ESSENCE* pEssence = (FACE_HAIR_ESSENCE*)pElementDataMan->get_data_ptr(id, ID_SPACE_FACE,dtFaceData);

		if( dtFaceData !=dt)
		{
			return false;
		}

//		if((m_pCurPlayer->GetProfession()!= pEssence->character_id)|| (m_pCurPlayer->GetGender() != pEssence->gender_id))
//		{
		if( m_pCurPlayer->GetGender() != (int)pEssence->gender_id)
		{
			return false;
		}
		
	}

	//胡子
	if( dt == DT_FACE_MOUSTACHE_ESSENCE)
	{
		FACE_MOUSTACHE_ESSENCE* pEssence = (FACE_MOUSTACHE_ESSENCE*)pElementDataMan->get_data_ptr(id, ID_SPACE_FACE,dtFaceData);

		if( dtFaceData !=dt)
		{
			return false;
		}

//		if((m_pCurPlayer->GetProfession()!= pEssence->character_id)|| (m_pCurPlayer->GetGender() != pEssence->gender_id))
//		{
		if( m_pCurPlayer->GetGender() != (int)pEssence->gender_id)
		{
			return false;
		}
		
	}

	//法令
	if( dt == DT_FACE_FALING_ESSENCE)
	{
		FACE_FALING_ESSENCE* pEssence = (FACE_FALING_ESSENCE*)pElementDataMan->get_data_ptr(id, ID_SPACE_FACE,dtFaceData);

		if( dtFaceData !=dt)
		{
			return false;
		}

//		if((m_pCurPlayer->GetProfession()!= pEssence->character_id)|| (m_pCurPlayer->GetGender() != pEssence->gender_id))
//		{
		if( m_pCurPlayer->GetGender() != (int) pEssence->gender_id)
		{
			return false;
		}
		
	}


	return true;
}