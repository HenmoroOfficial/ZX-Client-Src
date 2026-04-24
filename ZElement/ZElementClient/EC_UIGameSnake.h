/*******************************************************************
  FILE: EC_UIGameSnake.h
 
  CREATED BY: Ma Yuanzheng , 2011.12.13
 
  HISTORY: 
 
  DESCRIPTION: 
 
  Copyright (c) 2011 ZhuXian Studio, All Rights Reserved.
*******************************************************************/
#include <deque>

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
enum{
	LITTLE_GAME_EVENT_NONE,      // 没有值得注意的事件发生
	LITTLE_GAME_EVENT_STEP,		 // 下一步
	LITTLE_GAME_EVENT_OVERTIME,  // 超时
	LITTLE_GAME_EVENT_HURT,		 // 受伤
	LITTLE_GAME_EVENT_GAIN,		 // 成长
	LITTLE_GAME_EVENT_OUTLINE,	 // 出界
	LITTLE_GAME_EVENT_OUTLIFE,	 // 无法复活
	LITTLE_GAME_EVENT_WIN,		 // 胜利
	LITTLE_GAME_EVENT_PAUSE,	 // 暂停的
};

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class
//	
//	NAME:  CECGameSnake
//
//	DESCRIPTION: 贪吃蛇风格的游戏
//				 迷宫有 h * w 个格，每个格里存在某种物体类型
//				 闯关者只能在格内走
//               闯关者正在进入的格为它当前唯一存在的格
//
///////////////////////////////////////////////////////////////////////////
class CECGameSnake{
public:
	CECGameSnake();
	~CECGameSnake();

	// 初始化一个迷宫游戏
	// 高度 宽度 墙的位置 墙的数量 食物数量，起点，方向，速度，生命值，时间 
	void Init(int height, int width, int* wallPos, int wallNum, int foodNum, int orgnl,unsigned char dir, int speed,int life = 0, int time = 0);

	// 开始
	bool Start();

	// 游戏tick，并返回发生的事件
	int Tick(unsigned long dwDeltaTime);

	// 设置方向，只接受上下左右
	void SetDir(unsigned char dir);

	// 暂停
	bool Pause();

	// 停止
	bool Stop();

	// 闯关者重置初始状态，迷宫重新布局
	void Reset();

	//　回到起始位置
	void PlayerBack();
	

	// 取得迷宫阵数据
	const unsigned char * GetMaze(int& iWidth, int& iHeight,unsigned char& ucDir);
	// 可复活次数
	int GetLife();
	// 游戏的时间进度 1.0 ----- 0
	float GetTimeProcess();
	// 取的头尾位置
	void GetHeadAndTail(int& iHead, int& iTail);
	// 取的当前过度的百分数
	int  GetPercentage();

	short GetDirCache();
	int GetState();

	// 游戏状态
	enum{
		STATUS_RUN,
		STATUS_PAUSE,
		STATUS_STOP,
		STATUS_ERROR,
	};

	// 迷宫阵列里的物体类型-------------------
	enum MatrixDataType{
		MDT_NONE = 0, // 空
		MDT_WALL,     // 墙
		MDT_FOOD,     // 食物
		MDT_BODY,	  // 闯关者的身体
		MDT_MAX = 255,
	};

	// 方向值---------------------------------
	enum{
		DIR_UP,              //向上
		DIR_DOWN,            //向下
		DIR_LEFT,			 //向左
		DIR_RIGHT,			 //向右
		DIR_UP_TO_LEFT,		 //向上转左
		DIR_UP_TO_RIGHT,	 //向上转右
		DIR_DOWN_TO_LEFT,	 //向下转左
		DIR_DOWN_TO_RIGHT,	 //向下转右
		DIR_LEFT_TO_UP,		 //向左转上
		DIR_LEFT_TO_DOWN,	 //向左转下
		DIR_RIGHT_TO_UP,	 //向右转上
		DIR_RIGHT_TO_DOWN,	 //向右转下
	};


private:
	// dwDeltaTime时间内发生的移动，并返回发生的事件
	int AutoMove(unsigned long dwDeltaTime);
	// 方向转直
	void DirToStraight();
	// 下一步的位置，非法值为-1
	int GetNextStep();
	// 迷宫属性设为初始值
	void ResetMaze();
	// 随机食物
	void RandomFood();
	// 闯关者重置初始状态
	void ResetPlayer();
	// 清点场上食物数量
	int GetCurFoodNum();

	// 游戏属性------------------------------------------
	unsigned char m_ucStatus;	// 游戏状态

	// 迷宫----------------------------------------------
	// 初始属性---------------------
	int m_iHeight;	// 迷宫高度
	int m_iWidth;	// 迷宫宽度
	int m_iFoodNum;	// 食物数量
	
	// 实时属性---------------------
	unsigned char * m_ucPtrMatrix;  // 迷宫阵列数据
	//---------------------------------------------------


	// 闯关者--------------------------------------------
	// 初始属性---------------------
	int m_iStartingPnt;				// 起点
	int m_iOrgnlSize;				// 初始长度
	int m_iOrgnlLife;				// 初始生命值
	unsigned char m_ucOrgnlDir;		// 初始朝向
	int m_iLifeTime;				// 生存期 毫秒

	// 实时属性---------------------
	int m_iSize;					// 长度
	int m_iLife;					// 生命值
	int m_iTime;					// 已过的时间 毫秒
	unsigned char m_ucDir;			// 朝向
	std::deque<int> m_iBodyPos;		// 每个身体单元所处的位置
	int m_iSpeed;					// 每秒钟移动多少格
	unsigned int m_iPrcnt;			// 身体正在进入当前格的百分数
	int m_iHeadPos;					// “蛇”头的位置，逻辑上被“蛇”占据
	int m_iTailPos;					// “蛇”尾部的位置，逻辑上不被“蛇”占据，展示即将消失的效果
	//---------------------------------------------------
	
	short m_ucDirCache;

};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////
inline int CECGameSnake::GetLife(){return m_iLife;}
inline float CECGameSnake::GetTimeProcess(){return m_iLifeTime>0 ? ((m_iLifeTime - m_iTime) / (float)m_iLifeTime):1.0;}
inline void CECGameSnake::GetHeadAndTail(int& iHead,int& iTail){iHead = m_iHeadPos;iTail = m_iTailPos;}
inline int CECGameSnake::GetPercentage(){return m_iPrcnt;}
inline short CECGameSnake::GetDirCache(){return m_ucDirCache;}
inline int CECGameSnake::GetState(){return m_ucStatus;}
