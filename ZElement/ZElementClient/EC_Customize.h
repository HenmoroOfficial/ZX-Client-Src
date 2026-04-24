/*
 * FILE: EC_Customize.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Liyi, 2005/4/25
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#ifndef EC_CUSTOMIZE_H
#define EC_CUSTOMIZE_H

#ifdef _WIN32
#pragma once
#endif

#include "AUI\\AUI.h"
#include "roleinfo"
#include "Expression.h"
#include "FaceAnimation.h"

#include "FaceBoneController.h"
#include "EC_Global.h"

#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_ShadowRender.h"
#include "EC_GameSession.h"

#ifdef BACK_VERSION
#include "EC_Viewport.h"
#else
#include "ECViewport.h"
#endif
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


#include "EC_CustomizeBound.h"


struct CustomizeInitParam_t
{
	AUIManager* pAUImanager;
	CECPlayer* pCurPlayer;
	bool bCallForCreate;
};



class CECCustomize
{
	
public:
	//构造，析构
	CECCustomize();
	~CECCustomize(){};

	//响应命令
	bool OnCommand_Customize(const char *pszCommand, PAUIDIALOG pDlg);

	//选择脸型对话框响应
	bool OnCommand_WinChooseFace(const char *pszCommand, PAUIDIALOG pDlg);
	
	//选择眼型对话框
	bool OnCommand_WinChooseEye(const char *pszCommand, PAUIDIALOG pDlg);	

	//选择眉型对话框
	bool OnCommand_WinChooseEyebrow(const char *pszCommand, PAUIDIALOG pDlg);
	

	//选择鼻型对话框
	bool OnCommand_WinChooseNose(const char *pszCommand, PAUIDIALOG pDlg);

	
	//选择嘴型对话框
	bool OnCommand_WinChooseMouth(const char *pszCommand, PAUIDIALOG pDlg);
	

	//选择耳型对话框
	bool OnCommand_WinChooseEar(const char *pszCommand, PAUIDIALOG pDlg);
	

	//用于响应切换窗口
	bool OnCommand_SwitchDlg(const char *pszCommand, PAUIDIALOG pDlg);

	//高级选项对话框
	bool OnCommand_AdvancedOption(const char *pszCommand, PAUIDIALOG pDlg);

	//体形对话框
	bool OnCommand_WinChooseStature(const char *pszCommand, PAUIDIALOG pDlg);

	//预先自定义对话框
	bool OnCommand_WinChoosePreCustomize(const char* pszCommand, PAUIDIALOG pDlg);

	//响应事件
	virtual bool OnEvent_Customize( UINT uMsg, WPARAM wParam, LPARAM lParam, AUIDialog* pDlg, AUIObject* pObj);

	//选择脸型窗口响应事件
	bool OnEvent_ChooseFace(UINT uMsg, WPARAM wParam, LPARAM lParam, AUIDialog* pDlg, AUIObject* pObj);

	
	//测试2个AUI控件是否相交
	bool Test2CtrlIntersect( AUIObject* pCtrl1, AUIObject* pCtrl2 );

	//化妆对话框响应消息
	bool OnCommand_WinChooseFacePainting(const char *pszCommand, PAUIDIALOG pDlg);

	//用户参考对话框响应消息
	bool OnCommand_WinUserPic(const char *pszCommand, PAUIDIALOG pDlg);

	//化妆对话框响应事件
	bool OnEvent_WinChooseFacePainting(UINT uMsg, WPARAM wParam, LPARAM lParam, AUIDialog* pDlg, AUIObject* pObj);

	//毛发选择对话框响应事件
	bool OnEvent_WinChooseHair(UINT uMsg, WPARAM wParam, LPARAM lParam, AUIDialog* pDlg, AUIObject* pObj);

	//耳型选择对话框响应事件
	bool OnEvent_WinChooseEar(UINT uMsg, WPARAM wParam, LPARAM lParam, AUIDialog* pDlg, AUIObject* pObj);

	//嘴型选择对话框项响应事件
	bool OnEvent_WinChooseMouth(UINT uMsg, WPARAM wParam, LPARAM lParam, AUIDialog* pDlg, AUIObject* pObj);

	//鼻子选择对话框响应事件
	bool OnEvent_WinChooseNose(UINT uMsg, WPARAM wParam, LPARAM lParam, AUIDialog* pDlg, AUIObject* pObj);

	//眉毛选择对话框响应事件
	bool OnEvent_WinChooseBrow(UINT uMsg, WPARAM wParam, LPARAM lParam, AUIDialog* pDlg, AUIObject* pObj);

	//眼睛选择对话框响应事件
	bool OnEvent_WinChooseEye(UINT uMsg, WPARAM wParam, LPARAM lParam, AUIDialog* pDlg, AUIObject* pObj);

	//选择预设对话框响应事件
	bool OnEvent_WinChoosePreCustomize(UINT uMsg, WPARAM wParam, LPARAM lParam, AUIDialog* pDlg, AUIObject* pObj);

	//用户参考对话框响应事件
	bool OnEvent_WinUserPic(UINT uMsg, WPARAM wParam, LPARAM lParam, AUIDialog* pDlg, AUIObject* pObj);

	//移动摄像机
	bool Tick_MoveCamera();

	bool Init_Customize(CustomizeInitParam_t cusInitParam);

	bool Release_Customize();
	bool Tick_Customize();
	bool Render_Customize();

	
protected:
	


	bool SetAllsliderBound();
	
	//初始化对话框中的控件
	bool InitItem( void);

	//释放对话框中的控件中的数据
	bool ReleaseItem( void);

	//初始化成员变量
	bool InitVariable(void);

	//释放成员变量
	bool ReleaseVariable(void);

	//载入脸型相关的调节系数文件
	bool LoadFaceShapeFactor(const char* szFile);

	//载入脸型相关的调解参数(从ini文件)
	bool LoadFaceShapeFactorFromIniFile(const char* szFile);


	//更新ChooseFace对话框滚动条状态
	void UpdateChooseFaceSlider(void);

	//复位脸型
	void ResetChooseFace(void);

	//更新ChooseEye对话框的滚动条状态
	void UpdateChooseEyeSlider(void);

	//复位眼型
	void ResetChooseEye(void);

	//更新ChooseEyebrow对话框的滚动条状态
	void UpdateChooseBrowSlider(void);

	//复位眉型
	void ResetChooseBrow(void);

	//更新ChooseNose对话框的滚动条状态
	void UpdateChooseNoseSlider(void);

	//复位鼻型
	void ResetChooseNose(void);

	//复位鼻头
	void ResetNoseTip(void);

	//复位鼻梁
	void ResetNoseBridge(void);

	//更新ChooseMouth对话框的滚动条状态
	void UpdateChooseMouthSlider(void);

	//复位嘴型
	void ResetChooseMouth(void);

	//复位上嘴唇
	void ResetUpLip(void);

	//复位唇线
	void ResetMidLip(void);

	//复位下嘴唇
	void ResetDownLip(void);

	//更新ChooseEar对话框的滚动条状态
	void UpdateChooseEarSlider(void);

	//复位耳型
	void ResetChooseEar(void);

	//更新高级选项对话框的滚动条控件
	void UpdateAdvancedOptionSlider(void);

	//复位额头
	void ResetForehead(void);

	//复位颧骨
	void ResetYokeBone(void);

	//复位脸颊
	void ResetCheek(void);


	//复位下巴
	void ResetChain(void);


	//复位颌骨
	void ResetJaw(void);


	//复位脸部颜色
	void ResetFaceAllColor();
	
	//更新体形
	bool UpdateStature(void);


	//实际脸型个部分调解函数----------------------------------

	//计算眼型
	bool CalculateEye(void);

	//计算眉型
	bool CalculateBrow(void);

	//计算鼻型
	bool CalculateNose(void);

	//计算嘴型
	bool CalculateMouth(void);

	//计算耳型
	bool CalculateEar(void);

	//计算额头
	bool CalculateForehead(void);

	//计算颧骨
	bool CalculateYokeBone(void);

	//计算脸颊
	bool CalculateCheek(void);

	//计算下巴
	bool CalculateChain(void);

	//计算颌骨
	bool CalculateJaw(void);

	//计算高级选型
	bool CalculateAdvaceOption(void);

	//计算脸型融合
	bool CalculateBlendFace(void);

	//计算脸盘缩放
	bool CalculateFace(void);

	//计算3庭
	bool Calculate3Parts(void);

	//复原BlendFace系数
	int ResetBlendFaceFactorToInt(float fFactor);

	//复原Scale
	int ResetScaleToInt(float fScale, float fScaleFactor);

	//复原offset
	int ResetOffsetToInt(float fOffset, float fOffsetFactor);

	//复原rotate
	int ResetRotateToInt( float fRotate, float fRotateFactor);

	//把比例从int 转换到float
	float TransformScaleFromIntToFloat(int nScale, float fScaleFactor);

	//载入预定义数据
	bool LoadCustomizeData(char* pszFile);

	//确认个性化数据是否合法
	bool MakeSureCustomizeData( DATA_TYPE dt, unsigned int id, unsigned int part = 0);

	//随机表情
	void RandomExpression(void);

	//把眼睛重置为通常状态
	void ResetEyeToNormal(void);
	
	//把眼睛重置为通常状态
	void ResetBrowToNormal();

	//把眼睛重置为通常状态
	void ResetEarToNormal(void);

	//确保slider的值是可用的
	int MakeSureSlider(unsigned char& level, int nMax, int nMin);

	//根据载入FaceData数据设置滚动条
	bool SetFaceAllSlider(void);

	//设置个性化所有纹理
	bool SetFaceAllTexture(void);

	//设置个性化所有形状
	bool SetFaceAllShape(void);

	//设置个性化所有模型
	bool SetFaceAllModel(void);

	//设置个性化所有颜色
	bool SetFaceAllColor(void);

	//设置所有颜色位置
	bool SetAllColorPos(void);

	//计算颜色所在位置
	bool CalculateColorPos(int nID, A3DCOLOR clrPart, POINT& colorPoint);

	//画颜色所在位置
	bool DrawColorPos(void);

	//保存自定义数据
	bool SaveUserCustomizeData(void);

	//载入用户自定义数据
	bool LoadUserCustomizeData(void);

	//载入用户自定义图片
	bool LoadUserPic(void);

protected:
	AUIManager* m_pAUIManager;

	CECPlayer *	m_pCurPlayer;	// current player to be customized
	CECFace *	m_pCurFace;		// current face to be customized

	//是否从创建按钮调用
	bool m_bCallForCreate;

	PAUIDIALOG m_pWinChooseFace;
	PAUIDIALOG m_pWinChooseEye;
	PAUIDIALOG m_pWinChooseEyebrow;
	PAUIDIALOG m_pWinChooseNose;
	PAUIDIALOG m_pWinChooseMouth;
	PAUIDIALOG m_pWinChooseEar;
	PAUIDIALOG m_pWinChooseHair;
	PAUIDIALOG m_pWinChooseFacePainting;
	PAUIDIALOG m_pWinAdvancedOption;
	PAUIDIALOG m_pWinChooseStature;
	PAUIDIALOG m_pWinChoosePreCustomize;

	//选中与未选中颜色
	A3DCOLOR m_clrSel;
	A3DCOLOR m_clrUnSel;

	//-------------与脸部调整相关的滚动条控件-----------------
	AUIImagePicture* m_pImagePicCurFaceShape1;
	AUIImagePicture* m_pImagePicCurFaceShape2;

	//调整3庭的滚动条控件
	AUISlider* m_pSliderPartUp;
	AUISlider* m_pSliderPartMiddle;
	AUISlider* m_pSliderPartDown;

	PAUIIMAGEPICTURE m_pPicFace[5];
	
	//脸盘比例调节
	AUISlider* m_pSliderScaleFaceH;
	AUISlider* m_pSliderScaleFaceV;

	//与法令相关
	AUIListBox* m_pListBoxFalingTex;//法令纹理列表框

	//-------------与眼部调整相关的滚动条控件----------------
	AUIListBox* m_pListBoxEyeBaseTex1;
	AUIListBox* m_pListBoxEyeBaseTex2;
	AUIListBox* m_pListBoxEyeBaseTex3;

	AUIStillImageButton* m_pBtnChooseEyeBaseTex1;
	AUIStillImageButton* m_pBtnChooseEyeBaseTex2;
	AUIStillImageButton* m_pBtnChooseEyeBaseTex3;

	AUIListBox* m_pListBoxEyeShape1;
	AUIListBox* m_pListBoxEyeShape2;
	AUIListBox* m_pListBoxEyeShape3;

	
	AUICheckBox* m_pCheckBoxEyeAdjust;
	AUIStillImageButton* m_pBtnLeftEye;
	AUIStillImageButton* m_pBtnRightEye;

	AUISlider* m_pSliderScaleEyeH;		//左眼水平比例调节
	AUISlider* m_pSliderScaleEyeV;		//左眼竖直比例调节
	AUISlider* m_pSliderRotateEye;		//左眼旋转调解，绕Z轴旋转
	AUISlider* m_pSliderOffsetEyeH;		//左眼水平移动
	AUISlider* m_pSliderOffsetEyeV;		//左眼垂直移动
	AUISlider* m_pSliderOffsetEyeZ;		//左眼上下移动
	AUISlider* m_pSliderScaleEyeBall;	//左眼球缩放

	AUIStillImageButton* m_pBtmChooseBaseEye1;
	AUIStillImageButton* m_pBtmChooseBaseEye2;
	AUIStillImageButton* m_pBtmChooseBaseEye3;

	//用于保存左右眼的滚动条当前值
	int m_nScaleEyeH;
	int m_nScaleEyeV;
	int m_nRotateEye;
	int m_nOffsetEyeH;
	int m_nOffsetEyeV;
	int m_nOffsetEyeZ;
	int m_nScaleEyeBall;

	int m_nScaleEyeH2;
	int m_nScaleEyeV2;
	int m_nRotateEye2;
	int m_nOffsetEyeH2;
	int m_nOffsetEyeV2;
	int m_nOffsetEyeZ2;
	int m_nScaleEyeBall2;

	AUILabel* m_pLabelEye[7];

	//--------------与眉毛调整相关的滚动条控件
	AUIListBox* m_pListBoxBrowTex;//眉毛纹理列表框
	AUIListBox* m_pListBoxBrow;//眉毛形状列表框
	
	AUISlider* m_pSliderScaleBrowH;	//眉毛的水平比例调节
	AUISlider* m_pSliderScaleBrowV; //眉毛的树枝比例调节
	AUISlider* m_pSliderRotateBrow; //眉毛旋转调节,绕Z轴旋转
	AUISlider* m_pSliderOffsetBrowH; //眉毛的水平位移
	AUISlider* m_pSliderOffsetBrowV; //眉毛的垂直位移
	AUISlider* m_pSliderOffsetBrowZ; //眉毛的前后位移

	//用于保存当前眉毛滚动条值
	int m_nScaleBrowH;
	int m_nScaleBrowV;
	int m_nRotateBrow;
	int m_nOffsetBrowH;
	int m_nOffsetBrowV;
	int m_nOffsetBrowZ;

	int m_nScaleBrowH2;
	int m_nScaleBrowV2;
	int m_nRotateBrow2;
	int m_nOffsetBrowH2;
	int m_nOffsetBrowV2;
	int m_nOffsetBrowZ2;

	AUICheckBox* m_pCheckBoxBrowAdjust;
	AUIStillImageButton* m_pBtnLeftBrow;
	AUIStillImageButton* m_pBtnRightBrow;

	AUILabel* m_pLabelBrow[6];

	AUIStillImageButton* m_pBtmChooseBaseEyebrow1;

	//--------------与鼻子调整相关的滚动条控件

	AUIListBox* m_pListBoxNoseTex;//鼻子纹理list box
	AUIListBox* m_pListBoxNoseTip;//鼻头形状list box
	AUIListBox* m_pListBoxNoseBridge;//鼻梁形状list box
	
	AUISlider* m_pSliderScaleNoseTipH;//鼻头水平比例调节
	AUISlider* m_pSliderScaleNoseTipV;//鼻头垂直比例调节
	AUISlider* m_pSliderScaleNoseTipZ;//鼻头前后比例调节
	AUISlider* m_pSliderOffsetNoseTipV;//鼻头垂直位移调节
	AUISlider* m_pSliderScaleBridgeTipH;//鼻梁水平比例调节
	AUISlider* m_pSliderOffsetBridgeTipZ;//鼻梁前后移动调节
	
	AUIStillImageButton* m_pBtmChooseBaseNoseTip;
	AUIStillImageButton* m_pBtmChooseBaseNoseBridge;

	//--------------与嘴型调整相关的滚动条控件
	AUIListBox* m_pListBoxUpLipLine;//上唇线list box
	AUIListBox* m_pListBoxMidLipLine;//唇沟list box
	AUIListBox* m_pListBoxDownLipLine;//下唇线list box

	AUIStillImageButton* m_pBtmChooseMouthUpLipLine;
	AUIStillImageButton* m_pBtmChooseMouthMidLipLine;
	AUIStillImageButton* m_pBtmChooseMouthDownLipLine;
	
	AUISlider* m_pSliderThickUpLip; //上嘴唇厚度调节
	AUISlider* m_pSliderThickDownLip;//下嘴唇厚度调节
	AUISlider* m_pSliderScaleMouthH;//嘴部整体水平比例调节
	AUISlider* m_pSliderOffsetMouthV;//嘴部整体垂直位移调节
	AUISlider* m_pSliderOffsetMouthZ;//嘴部整体沿Z轴的位移调节
	AUISlider* m_pSliderOffsetCornerOfMouthSpecial;//嘴角的调节
	AUISlider* m_pSliderScaleMouthH2;
	AUISlider* m_pSliderOffsetCornerOfMouthSpecial2;

	AUICheckBox* m_pCheckBoxScaleMouthH;
	AUICheckBox* m_pCheckBoxOffsetCornerOfMouthSpecial;


	//---------------与耳朵调节相关的滚动条控件

	AUIListBox* m_pListBoxEar;//耳朵形状列表
	AUIStillImageButton* m_pBtmChooseBaseEar1;

	AUISlider* m_pSliderScaleEar; //耳朵的整体缩放
	AUISlider* m_pSliderOffsetEarV;//耳朵的上下移动

	//---------------与胡子头发调节相关的控件

	AUIListBox* m_pListBoxHair;//头发列表框
	
	AUIListBox* m_pListBoxMoustache;//小胡子列表控件
	
	AUIListBox* m_pListBoxGoatee;//大胡子的列表控件
	
	unsigned int *m_uCurItem;//当前item索引

	AUIListBox* m_pListBoxHairTex;//头发纹理列表框

	AUIListBox* m_pListBoxGoateeTex;//大胡子纹理列表框

	//----------------与脸部颜色相关的控件
	AUIListBox* m_pListBoxFaceParts;
	AUIImagePicture* m_pImagePictureColorPic;//用于显示当前颜色图片
	AUIImagePicture* m_pImagePictureCurColor;

	A3DSurface* m_pA3DSurfaceColorPic;//当前颜色图
	A3DFORMAT m_ColorPicFMT;//颜色图的格式

	AUIListBox* m_pListBoxEyeKohl;//眼影列表控件
	
	//---------------与高级操作相关的滚动条控件
	AUISlider* m_pSliderOffsetForeheadH;	//额头的横移
	AUISlider* m_pSliderOffsetForeheadV;	//额头的纵移
	AUISlider* m_pSliderOffsetForeheadZ;	//额头的前后移
	AUISlider* m_pSliderRotateForehead;		//额头的旋转
	AUISlider* m_pSliderScaleForehead;		//额头的比例缩放
	
	AUISlider* m_pSliderOffsetYokeBoneH;	//颧骨的横移
	AUISlider* m_pSliderOffsetYokeBoneV;	//颧骨的纵移
	AUISlider* m_pSliderOffsetYokeBoneZ;	//颧骨的前后移动
	AUISlider* m_pSliderRotateYokeBone;		//颧骨的旋转
	AUISlider* m_pSliderScaleYokeBone;		//颧骨的比例缩放
	
	AUISlider* m_pSliderOffsetCheekH;		//脸颊的横移
	AUISlider* m_pSliderOffsetCheekV;		//脸颊的纵移
	AUISlider* m_pSliderOffsetCheekZ;		//脸颊的前后移动
	AUISlider* m_pSliderScaleCheek;			//脸颊的比例缩放

	AUISlider* m_pSliderOffsetChainV;		//下巴的纵移
	AUISlider* m_pSliderOffsetChainZ;		//下巴的前后移动
	AUISlider* m_pSliderRotateChain;		//下巴的旋?
	AUISlider* m_pSliderScaleChainH;		//下巴的左右比例缩放

	AUISlider* m_pSliderOffsetJawH;			//颌骨的横移
	AUISlider* m_pSliderOffsetJawV;			//颌骨的纵移
	AUISlider* m_pSliderOffsetJawZ;			//颌骨的前后移动
	AUISlider* m_pSliderScaleJawSpecial;	//颌骨的特殊缩放
	AUISlider* m_pSliderScaleJawH;			//颌骨的左右比例缩放
	AUISlider* m_pSliderScaleJawV;			//颌骨的上下比例缩放
	
	//---------------身材调节相关的滚动条控件
	AUISlider* m_pSliderHeadScale;			//头大小
	AUISlider* m_pSliderUpScale;			//上身健壮度
	AUISlider* m_pSliderWaistScale;			//腰部为度
	AUISlider* m_pSliderArmWidth;			//下肢围度
	AUISlider* m_pSliderLegWidth;			//下肢围度
	AUISlider* m_pSliderBreastScale;		//胸部高低
	AUILabel* m_pLabelBreastScale;			//胸部高低标签
	AUIStillImageButton* m_pStillImageButtomBreastScale;//胸部高低复位按钮

	//---------------与预定义相关的滚动条控件
	AUIListBox* m_pListBoxPreCustomize;
	AUIListBox* m_pListBoxUserPic;

	AUIStillImageButton* m_pBtnUserPic;
	AUIStillImageButton* m_pBtnDeleteCustomizeData;
	
	//----------------脸型个部分的调节因子,总共51个

	//脸型融合1
	int m_nDefaultBlendFace;

	/*
	//脸 2
	float m_fScaleFaceHFactor;
	float m_fScaleFaceVFactor;

	//3庭
	float m_fScaleUpPartFactor;
	float m_fScaleMiddlePartFactor;
	float m_fScaleDownPartFactor;

	//额头 5
	float m_fOffsetForeheadHFactor;
	float m_fOffsetForeheadVFactor;
	float m_fOffsetForeheadZFactor;
	float m_fRotateForeheadFactor;
	float m_fScaleForeheadFactor;

	//颧骨 5
	float m_fOffsetYokeBoneHFactor;
	float m_fOffsetYokeBoneVFactor;
	float m_fOffsetYokeBoneZFactor;
	float m_fRotateYokeBoneFactor;
	float m_fScaleYokeBoneFactor;
	

	//脸颊 4
	float m_fOffsetCheekHFactor;
	float m_fOffsetCheekVFactor;
	float m_fOffsetCheekZFactor;
	float m_fScaleCheekFactor;

	//下巴 4
	float m_fOffsetChainVFactor;
	float m_fOffsetChainZFactor;
	float m_fRotateChainFactor;
	float m_fScaleChainHFactor;

	//颌骨 6
	float m_fOffsetJawHFactor;
	float m_fOffsetJawVFactor;
	float m_fOffsetJawZFactor;
	float m_fScaleJawSpecialFactor;
	float m_fScaleJawHFactor;
	float m_fScaleJawVFactor;

	//眼睛 14
	float m_fScaleEyeHFactor;
	float m_fScaleEyeVFactor;
	float m_fRotateEyeFactor;
	float m_fOffsetEyeHFactor;
	float m_fOffsetEyeVFactor;
	float m_fOffsetEyeZFactor;
	float m_fScaleEyeBallFactor;
	float m_fScaleEyeHFactor2;
	float m_fScaleEyeVFactor2;
	float m_fRotateEyeFactor2;
	float m_fOffsetEyeHFactor2;
	float m_fOffsetEyeVFactor2;
	float m_fOffsetEyeZFactor2;
	float m_fScaleEyeBallFactor2;


	//眉毛 12
	float m_fScaleBrowHFactor;
	float m_fScaleBrowVFactor;
	float m_fRotateBrowFactor;
	float m_fOffsetBrowHFactor;
	float m_fOffsetBrowVFactor;
	float m_fOffsetBrowZFactor;

	float m_fScaleBrowHFactor2;
	float m_fScaleBrowVFactor2;
	float m_fRotateBrowFactor2;
	float m_fOffsetBrowHFactor2;
	float m_fOffsetBrowVFactor2;
	float m_fOffsetBrowZFactor2;

	//鼻子 5
	float m_fScaleNoseTipHFactor;
	float m_fScaleNoseTipVFactor;
	float m_fOffsetNoseTipVFactor;
	float m_fScaleNoseTipZFactor;
	float m_fScaleBridgeTipHFactor;
	float m_fOffsetBridgeTipZFactor;

	//嘴 5
	float m_fThickUpLipFactor;
	float m_fThickDownLipFactor;
	float m_fScaleMouthHFactor;
	float m_fOffsetMouthVFactor;
	float m_fOffsetMouthZFactor;
	float m_fOffsetCornerOfMouthSpecialFactor;
	float m_fScaleMouthHFactor2;
	float m_fOffsetCornerOfMouthSpecialFactor2;

	//耳朵 2
	float m_fScaleEarFactor;
	float m_fOffsetEarVFactor;

	//身材
	float m_fHeadScaleFactor;
	float m_fUpScaleFactor;
	float m_fWaistScaleFactor;
	float m_fArmWidthFactor;
	float m_fLegWidthFactor;
	float m_fBreastScaleFactor;
*/
	
	CECCustomizeBound *m_pSliderBound;
	
	//------------定义所有与自定义相关的滚动条初始位置

	//3庭3
	int m_nDefaultScaleUp;
	int m_nDefaultScaleMiddle;
	int m_nDefaultScaleDown ;

	//脸盘缩放2
	int m_nDefaultScaleFaceH;
	int m_nDefaultScaleFaceV;
	

	//眼睛14
	int m_nDefaultScaleEyeH;
	int m_nDefaultScaleEyeV;
	int m_nDefaultRotateEye;
	int m_nDefaultOffsetEyeH;
	int m_nDefaultOffsetEyeV;
	int m_nDefaultOffsetEyeZ;
	int m_nDefaultScaleEyeBall;	

	int m_nDefaultScaleEyeH2;
	int m_nDefaultScaleEyeV2;
	int m_nDefaultRotateEye2;
	int m_nDefaultOffsetEyeH2;
	int m_nDefaultOffsetEyeV2;
	int m_nDefaultOffsetEyeZ2;
	int m_nDefaultScaleEyeBall2;	


	//眉毛6
	int m_nDefaultScaleBrowH;
	int m_nDefaultScaleBrowV;
	int m_nDefaultRotateBrow;
	int m_nDefaultOffsetBrowH;
	int m_nDefaultOffsetBrowV;
	int m_nDefaultOffsetBrowZ;

	int m_nDefaultScaleBrowH2;
	int m_nDefaultScaleBrowV2;
	int m_nDefaultRotateBrow2;
	int m_nDefaultOffsetBrowH2;
	int m_nDefaultOffsetBrowV2;
	int m_nDefaultOffsetBrowZ2;
		
	//鼻子5
	int m_nDefaultScaleNoseTipH;
	int m_nDefaultScaleNoseTipV;
	int m_nDefaultScaleNoseTipZ;
	int m_nDefaultOffsetNoseTipV;
	int m_nDefaultScaleBridgeTipH;
	int m_nDefaultOffsetBridegTipZ;
	
	
	//嘴6
	int m_nDefaultThickUpLip;
	int m_nDefaultThickDownLip;
	int m_nDefaultScaleMouthH;
	int m_nDefaultOffsetMouthV ;
	int m_nDefaultOffsetMouthZ ;
	int m_nDefaultOffsetCornerOfMouthSpecial;
	int m_nDefaultScaleMouthH2;
	int m_nDefaultOffsetCornerOfMouthSpecial2;

	//耳朵2
	int m_nDefaultScaleEar;
	int m_nDefaultOffsetEarV;
	
	//额头5
	int m_nDefaultOffsetForeheadH;
	int m_nDefaultOffsetForeheadV;
	int m_nDefaultOffsetForeheadZ;
	int m_nDefaultRotateForehead;
	int m_nDefaultScaleForehead;

	//颧骨5
	int m_nDefaultOffsetYokeBoneH ;
	int m_nDefaultOffsetYokeBoneV;
	int m_nDefaultOffsetYokeBoneZ ;
	int m_nDefaultRotateYokeBone;
	int m_nDefaultScaleYokeBone;

	//脸颊4
	int m_nDefaultOffsetCheekH ;
	int m_nDefaultOffsetCheekV;
	int m_nDefaultOffsetCheekZ;
	int m_nDefaultScaleCheek ;


	//下巴4
	int m_nDefaultOffsetChainV;
	int m_nDefaultOffsetChainZ;
	int m_nDefaultRotateChain;
	int m_nDefaultScaleChainH;


	//颌骨6
	int m_nDefaultOffsetJawH;
	int m_nDefaultOffsetJawV;
	int m_nDefaultOffsetJawZ;
	int m_nDefaultScaleJawSpecial;
	int m_nDefaultScaleJawH;
	int m_nDefaultScaleJawV;

	//身材
	int m_nDefaultHeadScale;
	int m_nDefaultUpScale;
	int m_nDefaultWaistScale;
	int m_nDefaultArmWidth;
	int m_nDefaultLegWidth;
	int m_nDefaultBreastScale;

	//----------------设置按钮控件---------------------
	AUIStillImageButton* m_pBtmChoosePreCustomize;
	AUIStillImageButton* m_pBtmChooseFace;
	AUIStillImageButton* m_pBtmChooseEye;
	AUIStillImageButton* m_pBtmChooseBrow;
	AUIStillImageButton* m_pBtmChooseNose;
	AUIStillImageButton* m_pBtmChooseMouth;
	AUIStillImageButton* m_pBtmChooseEar;
	AUIStillImageButton* m_pBtmChooseHair;
	AUIStillImageButton* m_pBtmChooseFacePainting;
	AUIStillImageButton* m_pBtmChooseStature;

	AUIStillImageButton* m_pBtmCurChoose;//当前按钮

	//------------------当前颜色位置---------------
	POINT m_FaceColorPos;
	POINT m_EyeColorPos;
	POINT m_BrowColorPos;
	POINT m_MouthColorPos;
	POINT m_HairColorPos;
	POINT m_EyeBallColorPos;
	POINT m_MoustacheColorPos;
	AUIImagePicture* m_pPicColorPos;

	//预定义个性化的数量
	int m_nPreCustomsizeCount;

	

	//------------------与移动摄像机相关-------------
	AUIDialog* m_pWinMoveCamera;
	AUIStillImageButton* m_pBtnCameraMoveFront;
	AUIStillImageButton* m_pBtnCameraMoveBack;
	AUIStillImageButton* m_pBtnCameraMoveLeft;
	AUIStillImageButton* m_pBtnCameraMoveRight;
	
	AUIStillImageButton* m_pBtnCameraTurnLeft;
	AUIStillImageButton* m_pBtnCameraTurnRight;

	AUIStillImageButton* m_pBtnCameraMoveUp;
	AUIStillImageButton* m_pBtnCameraMoveDown;

	//------------------与用户参考图片相关------------
	AUIDialog* m_pWinUserPic;
	AUIImagePicture* m_pPicUserPic;
	A2DSprite* m_pA2DSpriteUserPic;

	float m_fA2DSpriteUserPicScale;//A2DSprite的比例
	float m_fA2DSpriteUserPicOriScale;//A2DSprite的原始比例

	float m_fPicUserPicScale;//控件的比例

	int m_nPicUserPicOriWidth;//控件原始尺寸
	int m_nPicUserPicOriHeight;

	int m_nA2DSPriteUserPicItemPosX;//图片item的位置
	int m_nA2DSPriteUserPicItemPosY;

	//鼠标在item上面的位置
	int m_nMouseOnItemX, m_nMouseOnItemY;

	//鼠标在图片上面的位置
	int m_nMouseOnA2DSpriteX,m_nMouseOnA2DSpriteY;

	AUIStillImageButton* m_pBtnUserPicDrag;
	AUIStillImageButton* m_pBtnUserPicZoomIn;
	AUIStillImageButton* m_pBtnUserPicZoomOut;

	AUISlider* m_pSliderUserPicAlpha;
	AUILabel* m_pLabelUserPicAlpha;

	AUILabel* m_pLabelUserPic;

	//主要用于点击按钮后自动放大缩小图片
	bool Tick_WinUserPic();

	//放大图片
	void ZoomOutUserPic();

	//缩小图片
	void ZoomInUserPic();

	//动态数组保存对话框中的A2DSprite对象
	abase::vector<A2DSprite*> m_vecA2DSprite;
	abase::vector<char*> m_vecCustomizeFileName;
		

	//!*************用于测试自定义表情***************!//
	
public:


	AUIDialog* m_pWinChooseExpression;
	


	AUIListBox* m_pListBoxExp;

	//表情数量
	#define EXP_NUMBER 9

	//融合表情数量
	enum E_BLEND_EXP
	{
		EBE_NUM1 = 0,
		EBE_NUM2 = 1,
		EBE_NUM3 = 2,
		EBE_NUMBER
	};

	//融合表情图片控件
	AUIImagePicture* m_pPicBlendExp[EBE_NUMBER];

	CExpression m_Expression;
	CFaceAnimation m_Animation;

	struct S_ANIMATION_PARAM
	{
		int nAppearTime;
		int nKeepTime;
		int nDisapperTime;
		int nRestTime;
		int nRepeat;
	};

	//表情参数
	S_ANIMATION_PARAM m_AniParam;
	
	enum E_ANIMATION_PARAM
	{
		EAP_APPEARTIME = 0,
		EAP_KEEPTIME = 1,
		EAP_DISAPPEARTIME = 2,
		EAP_RESTTIME = 3,
		EAP_REPEAT = 4,
		EAP_NUMBER = 5
	};
	
	struct S_BLEND_ANIMATION_PARAM
	{
		char* pszBlendExpFile[EAP_NUMBER];
		int nAppearTime;
		int nKeepTime;
		int nDisapperTime;
		int nRestTime;
		int nRepeat;
	};
	
	//混合表情参数
	S_BLEND_ANIMATION_PARAM m_BlendAniParam;

	
	//表情参数滚动条
	AUISlider* m_pSliderBlendExp[EAP_NUMBER];

	//表情参数标签
	AUILabel* m_pLabelBlendExp[EAP_NUMBER];

	
	AUIDialog* m_pWinExpressionList;
	

	AUIListBox* m_pListBoxExpressionList;
	
	abase::vector<char*> m_vecExpFileName;//存储表情文件名

	int m_nCustomizeBlendExpressionID;//混合表情ID

	//初始化表情控件
	bool InitExpressionCtrler(void);

	//释放表情控件
	bool ReleaseExpressionCtrler(void);

	//载入自定义表情
	bool LoadCustomizeExpression(void);

	
	bool OnCommand_WinChooseExpression(const char* pszCommand, PAUIDIALOG pDlg);
	
	void CustomizeExpressionTick();

	bool OnCommand_WinExpressionList(const char* pszCommand, PAUIDIALOG pDlg);


	void PlayExpList();
	void StopExpList();

	void InsertCurBlendExp(S_BLEND_ANIMATION_PARAM* blendExp);
	void DeleteCurBlendExp(unsigned int nIndex);

	void ReleaseExpList();


	abase::vector<S_BLEND_ANIMATION_PARAM*> m_CustomizeExpList;


	void CalculateCurBlendExpTime(S_BLEND_ANIMATION_PARAM* pSBAP);

	

	enum E_STATUS
	{
		ES_STOP,
		ES_PLAY	
	};
	
	E_STATUS m_Status;
	
	//当前表情播放时间
	int m_nPassedTime;

	//当前混合表情所需时间(当前3个表情中时间最长的值)
	int m_nCurBlendExpTime;

	//当前表情列表播放的表情记数
	unsigned int m_nPlayBlendExpListCount;

};



#endif
