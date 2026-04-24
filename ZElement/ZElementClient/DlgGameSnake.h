/*******************************************************************
  FILE: .h
 
  CREATED BY: Ma Yuanzheng , 2011.12.15
 
  HISTORY: 
 
  DESCRIPTION: 
 
  Copyright (c) 2011 ZhuXian Studio, All Rights Reserved.
*******************************************************************/
#pragma once

#include "DlgBase.h"
#include <list>
///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////
class CECGameSnake;
class AUIProgress;

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class
//	
//	NAME:  CDlgGameSnake
//
//	RESOUSE: 
//
//	DESCRIPTION: 
//
///////////////////////////////////////////////////////////////////////////
class CDlgGameSnake : public CDlgBase{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
public:
	CDlgGameSnake();
	~CDlgGameSnake();


	virtual void OnTick();
	virtual void OnShowDialog();


	void OnCommand_Start(const char* szCommand);
	void OnCommand_Pause(const char* szCommand);
	void OnCommand_Stop(const char* szCommand);
	void OnCommand_Close(const char* szCommand);
	void OnEvent_KeyDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	// 加载迷宫数据
	bool LoadMazeConfig(int iNo);

	// 更新界面上的迷宫数据 （替换改变值）
	void UpdateMaze(const unsigned char * mazeMatrix, int width, int height,unsigned char ucDir);

	// 迷宫数据显示在界面上 （全部替换）
	void SetMaze(const unsigned char * mazeMatrix, int width, int height,unsigned char ucDir);

	// 更新界面上的生命显示
	void UpdateLife();
	
	void StartStage(int iStage);

private:
	CECGameSnake* m_pGameSnake;
	unsigned char * m_ucPtrMatrix;  // 迷宫阵列数据
	int m_iWidth;
	int m_iHeight;
	unsigned char m_ucDir;      // 闯关者的方向
	int			  m_iHead;		// 闯关者头部
	int           m_iTail;		// 闯关者尾部
	int           m_iGameEvent;	// 游戏事件
	int           m_iTimeCnt;	// 倒计时使用
	int			  m_iCmd;		// 当前命令
	int           m_iGameId;	// 游戏ID 任务用
	long		  m_iMazeLeft;
	long		  m_iMazeTop;
	int			  m_iMazeCellSize;

	AUIProgress* m_pProg;		// 时间进度
	std::list<AUIObject*> m_LstDynamicObj;			// 所有动态添加的控件

	// 根据方向、进度、是 头/尾 返回显示的图片
	inline const char* GetPic(unsigned char dir,int percentage,bool bHead);
	// 根据进度，是 头/尾 返回 头/尾该显示第几帧
	inline int GetFrame(int percentage,bool bHead);

	// 设置一段时间之后开始一个动作 用于倒计时
	inline void SetTimeAndCmd(int iTime , int iCmd);
	void LazyStart();
	void LazyStop();
	void LazyPlayerBack();
	void LazyWin();

	//　建立四周围墙
	void BuildBar();

	// 显示碰撞
	void PlayCollision();

};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////
inline void CDlgGameSnake::SetTimeAndCmd(int iTime , int iCmd){m_iTimeCnt = iTime; m_iCmd = iCmd;}

class CDlgWindowsGfx : public CDlgBase
{
public:
	CDlgWindowsGfx();
	~CDlgWindowsGfx();
protected:
	virtual void OnShowDialog();
	virtual void OnTick();

	DWORD m_dwRefreshTime;
};

//春节守岁界面
class CDlgSpring2012 : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgSpring2012();
	virtual ~CDlgSpring2012();

	void OnCommandGet(const char * szCommand);
};