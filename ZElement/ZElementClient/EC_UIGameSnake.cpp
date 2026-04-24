/*******************************************************************
  FILE: EC_UIGameSnake.cpp
 
  CREATED BY: Ma Yuanzheng , 2011.12.13
 
  HISTORY: 
 
  DESCRIPTION: 
 
  Copyright (c) 2011 ZhuXian Studio, All Rights Reserved.
*******************************************************************/
#include "EC_UIGameSnake.h"

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////


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


///////////////////////////////////////////////////////////////////////////
//	
//	Implement
//	
///////////////////////////////////////////////////////////////////////////
CECGameSnake::CECGameSnake(){
	// 游戏属性------------------------------------------
	m_ucStatus = STATUS_ERROR;

	// 迷宫----------------------------------------------
	m_iHeight = 10;
	m_iWidth  = 10;
	m_iFoodNum = 0;

	m_ucPtrMatrix = NULL;


	// 闯关者--------------------------------------------
	m_iStartingPnt = 0;
	m_iOrgnlSize = 1;
	m_iOrgnlLife = 0; // 无限生命值
	m_ucOrgnlDir = DIR_RIGHT;
	m_iLifeTime = 0;  // 无限生存期
	m_iSpeed = 1;

	// 实时属性---------------------
	Reset();

	m_ucDirCache = -1;
}

CECGameSnake::~CECGameSnake(){
	if(m_ucPtrMatrix){
		delete[] m_ucPtrMatrix;
		m_ucPtrMatrix = NULL;
	}
}


void CECGameSnake::Init(int height, int width, int* wallPos, int wallNum, int foodNum, int orgnl,unsigned char dir, int speed,int life, int time){
	int i;
	m_iHeight = height;
	m_iWidth = width;

	if(m_ucPtrMatrix) 
		delete[] m_ucPtrMatrix;

	m_ucPtrMatrix = new unsigned char[m_iWidth * m_iHeight];

	for(i = m_iWidth * m_iHeight - 1; i >= 0;i--){
		m_ucPtrMatrix[i] = MDT_NONE;
	}

	int pos;
	for(i = 0 ; i < wallNum; i++){
		pos = wallPos[i];
		if(pos >= 0 && pos <  m_iWidth * m_iHeight)
			m_ucPtrMatrix[pos] = MDT_WALL;
	}

	m_iFoodNum = foodNum;
	m_iStartingPnt = orgnl;
	m_ucPtrMatrix[m_iStartingPnt] = MDT_BODY;
	m_ucOrgnlDir = dir;
	m_iSpeed = speed;
	m_iOrgnlLife = life;
	m_iLifeTime = time * 1000;
	m_ucStatus = STATUS_STOP;
	Reset();
}
void CECGameSnake::SetDir(unsigned char dir){
	if(dir < DIR_UP || dir > DIR_RIGHT) return;

	if(m_iPrcnt < 76 && m_iPrcnt > 0){
		m_ucDirCache = -1;
		switch(m_ucDir){
			case DIR_UP:
			case DIR_UP_TO_LEFT:
			case DIR_UP_TO_RIGHT:
				if(dir == DIR_RIGHT) m_ucDir = DIR_UP_TO_RIGHT;
				else if(dir == DIR_LEFT) m_ucDir = DIR_UP_TO_LEFT;
				else if(dir == DIR_UP) m_ucDir = DIR_UP;
				break;
			case DIR_DOWN:
			case DIR_DOWN_TO_LEFT:
			case DIR_DOWN_TO_RIGHT:
				if(dir == DIR_RIGHT) m_ucDir = DIR_DOWN_TO_RIGHT;
				else if(dir == DIR_LEFT) m_ucDir = DIR_DOWN_TO_LEFT;
				else if(dir == DIR_DOWN) m_ucDir = DIR_DOWN;
				break;
			case DIR_LEFT:
			case DIR_LEFT_TO_UP:
			case DIR_LEFT_TO_DOWN:
				if(dir == DIR_LEFT) m_ucDir = DIR_LEFT;
				else if(dir == DIR_UP) m_ucDir = DIR_LEFT_TO_UP;
				else if(dir == DIR_DOWN) m_ucDir = DIR_LEFT_TO_DOWN;
				break;
			case DIR_RIGHT:
			case DIR_RIGHT_TO_UP:
			case DIR_RIGHT_TO_DOWN:
				if(dir == DIR_RIGHT) m_ucDir = DIR_RIGHT;
				else if(dir == DIR_UP) m_ucDir = DIR_RIGHT_TO_UP;
				else if(dir == DIR_DOWN) m_ucDir = DIR_RIGHT_TO_DOWN;
				break;
			default:
				m_ucDir = DIR_UP;
				break;
		};
	}
	else 
	{
		m_ucDirCache = dir;
	}
	/*else if(m_iPrcnt == 100 || m_iPrcnt == 0){
		switch(m_ucDir){
			case DIR_UP:
			case DIR_LEFT_TO_UP:
			case DIR_RIGHT_TO_UP:
				if(dir == DIR_RIGHT) m_ucDir = DIR_UP_TO_RIGHT;
				else if(dir == DIR_LEFT) m_ucDir = DIR_UP_TO_LEFT;
				else if(dir == DIR_UP) m_ucDir = DIR_UP;
				break;
			case DIR_DOWN:
			case DIR_LEFT_TO_DOWN:
			case DIR_RIGHT_TO_DOWN:
				if(dir == DIR_RIGHT) m_ucDir = DIR_DOWN_TO_RIGHT;
				else if(dir == DIR_LEFT) m_ucDir = DIR_DOWN_TO_LEFT;
				else if(dir == DIR_DOWN) m_ucDir = DIR_DOWN;
				break;
			case DIR_LEFT:
			case DIR_UP_TO_LEFT:
			case DIR_DOWN_TO_LEFT:
				if(dir == DIR_LEFT) m_ucDir = DIR_LEFT;
				else if(dir == DIR_UP) m_ucDir = DIR_LEFT_TO_UP;
				else if(dir == DIR_DOWN) m_ucDir = DIR_LEFT_TO_DOWN;
				break;
			case DIR_RIGHT:
			case DIR_UP_TO_RIGHT:
			case DIR_DOWN_TO_RIGHT:
				if(dir == DIR_RIGHT) m_ucDir = DIR_RIGHT;
				else if(dir == DIR_UP) m_ucDir = DIR_RIGHT_TO_UP;
				else if(dir == DIR_DOWN) m_ucDir = DIR_RIGHT_TO_DOWN;
				break;
			default:
				m_ucDir = DIR_UP;
				break;
		};
	}*/
}
bool CECGameSnake::Start(){
	if(m_ucStatus == STATUS_ERROR) return false;
	m_ucStatus = STATUS_RUN;
	return true;
}

bool CECGameSnake::Pause(){
	if(m_ucStatus == STATUS_ERROR) return false;
	if(m_ucStatus == STATUS_RUN){
		m_ucStatus = STATUS_PAUSE;
	}
	return true;

}
bool CECGameSnake::Stop(){
	if(m_ucStatus == STATUS_ERROR) return false;

	m_ucStatus = STATUS_STOP;
	Reset();
	return true;
}
int CECGameSnake::Tick(unsigned long dwDeltaTime){
	if(m_ucStatus == STATUS_RUN){
		return AutoMove(dwDeltaTime);
	}
	return LITTLE_GAME_EVENT_PAUSE;
}
const unsigned char * CECGameSnake::GetMaze(int& iWidth, int& iHeight,unsigned char& ucDir){
	iWidth = m_iWidth;
	iHeight = m_iHeight;
	ucDir = m_ucDir;
	return m_ucPtrMatrix;
}
int CECGameSnake::AutoMove(unsigned long dwDeltaTime){

	// 判断是否超时
	if(m_iLifeTime > 0){
		m_iTime += dwDeltaTime;
		if(m_iTime >= m_iLifeTime) return LITTLE_GAME_EVENT_OVERTIME;
	}

	// 向前移动
	m_iPrcnt += m_iSpeed * dwDeltaTime * 100 / 1000 ;

	// 没有进入下一格
	if(m_iPrcnt < 100){
		return LITTLE_GAME_EVENT_NONE;
	}


	// 进入下一格，重置为下一格的百分数
	m_iPrcnt-=100;
	// 方向打直
	DirToStraight();

	// 下一格的位置
	int iNext = GetNextStep();

	// 下一格是非法值
	if(iNext == -1){
		// 无法复活
		if(m_iLife && --m_iLife <= 0) return LITTLE_GAME_EVENT_OUTLIFE;

		return LITTLE_GAME_EVENT_OUTLINE;
	}

	// 下一格什么都没有撞到
	if(m_ucPtrMatrix[iNext] == MDT_NONE){	
		// 头记录新的位置
		m_iHeadPos = iNext;
		m_iBodyPos.push_front(iNext);
		m_ucPtrMatrix[iNext] = MDT_BODY;
		// 记录刚刚弹出的尾部位置
		m_iTailPos = m_iBodyPos.back();
		m_ucPtrMatrix[m_iTailPos] = MDT_NONE;
		m_iBodyPos.pop_back();

		return LITTLE_GAME_EVENT_STEP;
	}

	// 下一格撞到了食物
	if(m_ucPtrMatrix[iNext] == MDT_FOOD){

		// 头记录新的位置
		m_iHeadPos = iNext;
		m_iBodyPos.push_front(iNext);
		m_ucPtrMatrix[iNext] = MDT_BODY;

		if(GetCurFoodNum() == 0) return LITTLE_GAME_EVENT_WIN;

		return LITTLE_GAME_EVENT_GAIN;
	}

	// 下一格撞到了墙 或 自己
	if(m_ucPtrMatrix[iNext] == MDT_WALL || m_ucPtrMatrix[iNext] == MDT_BODY){
		// 无法复活
		if(m_iLife > 0 && --m_iLife <= 0) return LITTLE_GAME_EVENT_OUTLIFE;

		return LITTLE_GAME_EVENT_HURT;
	}
	return LITTLE_GAME_EVENT_NONE;
}

int CECGameSnake::GetNextStep(){
	int iNext = -1;
	switch(m_ucDir){
		case DIR_UP:
		case DIR_LEFT_TO_UP:
		case DIR_RIGHT_TO_UP:
			iNext = m_iBodyPos.front() - m_iWidth;
			break;
		case DIR_DOWN:
		case DIR_LEFT_TO_DOWN:
		case DIR_RIGHT_TO_DOWN:
			iNext = m_iBodyPos.front() + m_iWidth;
			break;
		case DIR_LEFT:
		case DIR_UP_TO_LEFT:
		case DIR_DOWN_TO_LEFT:
			if(m_iBodyPos.front() % m_iWidth == 0) iNext = -1;
			else iNext = m_iBodyPos.front() - 1;
			break;
		case DIR_RIGHT:
		case DIR_UP_TO_RIGHT:
		case DIR_DOWN_TO_RIGHT:
			iNext = m_iBodyPos.front() + 1;
			if(iNext % m_iWidth == 0) iNext = -1;
			break;
		default:
			iNext = -1;
	};
	if(iNext < 0 || iNext >= m_iWidth * m_iHeight) iNext = -1;

	return iNext;	
}
void CECGameSnake::Reset(){
	// 迷宫
	if(m_ucPtrMatrix){
		ResetMaze();
		ResetPlayer();
	}
}
void CECGameSnake::ResetPlayer(){
	//闯关者
	m_iLife = m_iOrgnlLife;
	m_iTime = 0;
	PlayerBack();


}

void CECGameSnake::PlayerBack(){
	int i;
	m_iSize = m_iOrgnlSize;
	m_ucDir = m_ucOrgnlDir;

	for(i = 0;i<m_iHeight * m_iWidth;i++){
		if(m_ucPtrMatrix[i] == MDT_BODY) m_ucPtrMatrix[i] = MDT_NONE;
	}
	m_ucPtrMatrix[m_iStartingPnt] = MDT_BODY;
	m_iPrcnt = 100;

	m_iBodyPos.clear();
	for(i = 0; i<m_iSize;i++){
		m_iBodyPos.push_back(m_iStartingPnt);
	}
	m_iTailPos = -1;
	m_iHeadPos = m_iStartingPnt;
}
void CECGameSnake::ResetMaze(){
	for(int i = 0;i<m_iHeight * m_iWidth;i++){
		if(m_ucPtrMatrix[i] != MDT_WALL) m_ucPtrMatrix[i] = MDT_NONE;
	}
	RandomFood();
}
void CECGameSnake::RandomFood(){

	//食物数不能多于空位数
	int num = 0;
	for(int i = 0;i<m_iHeight * m_iWidth;i++){
		if(m_ucPtrMatrix[i] == MDT_FOOD) m_ucPtrMatrix[i] = MDT_NONE;
		if(m_ucPtrMatrix[i] == MDT_NONE) num++;
	}
	if(m_iFoodNum > num) m_iFoodNum = num;


	// 布置食物
	for(int k = 0; k<m_iFoodNum;k++){
		// 获得一个随机数，表示第几个空位置放食物
		int iIndex = rand()%(num - k)+1; 
		int i = -1;
		//数到这个空位置
		while(iIndex>0){
			if(++i >= m_iHeight * m_iWidth) i=0;
			if(m_ucPtrMatrix[i] == MDT_NONE){
				iIndex--;
			}	
		}
		//放置食物
		m_ucPtrMatrix[i] = MDT_FOOD;
	}
}
int CECGameSnake::GetCurFoodNum(){
	int num = 0;
	for(int i = m_iHeight * m_iWidth - 1; i>=0;i--){
		if(m_ucPtrMatrix[i] == MDT_FOOD) num++;
	}
	return num;
}
void CECGameSnake::DirToStraight(){
	switch(m_ucDir){
			case DIR_UP:
			case DIR_LEFT_TO_UP:
			case DIR_RIGHT_TO_UP:
				m_ucDir = DIR_UP;
				break;
			case DIR_DOWN:
			case DIR_LEFT_TO_DOWN:
			case DIR_RIGHT_TO_DOWN:
				m_ucDir = DIR_DOWN;
				break;
			case DIR_LEFT:
			case DIR_UP_TO_LEFT:
			case DIR_DOWN_TO_LEFT:
				m_ucDir = DIR_LEFT;
				break;
			case DIR_RIGHT:
			case DIR_UP_TO_RIGHT:
			case DIR_DOWN_TO_RIGHT:
				m_ucDir = DIR_RIGHT;
				break;
			default:
				m_ucDir = DIR_UP;
				break;
		};
}
