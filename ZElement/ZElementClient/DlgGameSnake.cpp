/*******************************************************************
  FILE: .cpp
 
  CREATED BY: Ma Yuanzheng , 2011.12.15
 
  HISTORY: 
 
  DESCRIPTION: 
 
  Copyright (c) 2011 ZhuXian Studio, All Rights Reserved.
*******************************************************************/
#include "AIniFile.h"
#include "afi.h"
#include "DlgGameSnake.h"
#include "EC_UIGameSnake.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameUIMan.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIProgress.h"
#include "EC_TaskInterface.h"
#include "TaskTemplMan.h"
#include "EC_HostPlayer.h"

#define new A_DEBUG_NEW

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////
#define MazeLeft 326
#define MazeTop  105
#define MAZE_CELL_SIZE 16


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
// 
static char* szMovePath[6][2] = {
	"Game\\Snake\\Up.tga",			"Game\\Snake\\Down.tga",	//  正序：向上； 逆序：向下
	"Game\\Snake\\U_T_R.tga",		"Game\\Snake\\L_T_D.tga",	//  正序：向上转右；逆序：向左转下
	"Game\\Snake\\U_T_L.tga",		"Game\\Snake\\R_T_D.tga",	//  正序：向上转左；逆序：向右转下
	"Game\\Snake\\D_T_R.tga",		"Game\\Snake\\L_T_U.tga",	//  正序：向下转右；逆序：向左转上
	"Game\\Snake\\D_T_L.tga",		"Game\\Snake\\R_T_U.tga",	//  正序：向下转左；逆序：向右转上
	"Game\\Snake\\Left.tga",		"Game\\Snake\\Right.tga",	//  正序：向左；逆序：向右
};

enum{
	LAZY_CMD_START,
	LAZY_CMD_STOP,
	LAZY_CMD_PLAYER_BACK,
	LAZY_CMD_WIN,
};

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
AUI_BEGIN_COMMAND_MAP(CDlgGameSnake,CDlgBase)
AUI_ON_COMMAND("Btn_Start",OnCommand_Start)
AUI_ON_COMMAND("Btn_Pause",OnCommand_Pause)
AUI_ON_COMMAND("Btn_Stop",OnCommand_Stop)
AUI_ON_COMMAND("Btn_Close",OnCommand_Close)
AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgGameSnake,CDlgBase)
AUI_ON_EVENT(NULL,WM_KEYDOWN,OnEvent_KeyDown)
AUI_ON_EVENT("*",WM_KEYDOWN,OnEvent_KeyDown)
AUI_END_EVENT_MAP()

CDlgGameSnake::CDlgGameSnake(){
	m_pGameSnake = NULL;
	m_ucPtrMatrix = NULL;
	m_LstDynamicObj.clear();
	m_iWidth = 0;
	m_iHeight = 0;
	m_ucDir = CECGameSnake::DIR_UP;
	m_iHead = -1;
	m_iTail = -1;
	m_iGameEvent = LITTLE_GAME_EVENT_NONE;
	m_iTimeCnt = -1;
	m_iCmd = -1;
	m_pProg = NULL;
	m_iGameId = 1;
	m_iMazeLeft = 326;
	m_iMazeTop = 105;
	m_iMazeCellSize = 16;
}
CDlgGameSnake::~CDlgGameSnake(){
	if(m_pGameSnake){
		delete m_pGameSnake;
		m_pGameSnake = NULL;
	}
	if(m_ucPtrMatrix){
		delete[] m_ucPtrMatrix;
		m_ucPtrMatrix;
	}
	m_LstDynamicObj.clear();
}

void CDlgGameSnake::OnShowDialog(){
	_AUIOBJECT_SETPROPERTY p;
	POINT pos = GetDlgItem("Img_Wall")->GetPos(true);
	m_iMazeLeft = pos.x;
	m_iMazeTop = pos.y;
	GetDlgItem("Img_Wall")->GetProperty("Width", &p);
	m_iMazeCellSize = p.i;
	LoadMazeConfig(m_iGameId);
	GetDlgItem("Img_Lost")->Show(false);
	GetDlgItem("Img_Win")->Show(false);
	GetDlgItem("Img_start")->Show(false);
}

void CDlgGameSnake::OnTick(){ 
	DWORD deltaTime = GetGame()->GetTickTime();
	if(!m_pGameSnake) return;

	if(m_iTimeCnt >0){
		m_iTimeCnt-=deltaTime;
		if(m_iCmd == LAZY_CMD_START){
			if(m_iTimeCnt > 2500){
				((AUIImagePicture*)GetDlgItem("Img_start"))->FixFrame(0);
			}else if(m_iTimeCnt > 1500){
				((AUIImagePicture*)GetDlgItem("Img_start"))->FixFrame(1);
			}else if(m_iTimeCnt > 500){
				((AUIImagePicture*)GetDlgItem("Img_start"))->FixFrame(2);
			}else{
				((AUIImagePicture*)GetDlgItem("Img_start"))->FixFrame(3);
			}
		}
		if(m_iTimeCnt<=0){
			if(m_iCmd == LAZY_CMD_START){
				m_iTimeCnt = -1;
				LazyStart();
			}
			else if(m_iCmd == LAZY_CMD_STOP){
				m_iTimeCnt = -1;
				LazyStop();
			}
			else if(m_iCmd == LAZY_CMD_PLAYER_BACK){
				m_iTimeCnt = -1;
				LazyPlayerBack();
				OnCommand_Start("");
			}else if(m_iCmd == LAZY_CMD_WIN){
				LazyWin();
			}
			GetDlgItem("Img_BumpHorizon")->Show(false);
			GetDlgItem("Img_BumpErect")->Show(false);
		}
	}
 
	int gameEvent = m_pGameSnake->Tick(deltaTime);
	switch(gameEvent){
		case LITTLE_GAME_EVENT_NONE:
			break;
		case LITTLE_GAME_EVENT_GAIN:
		//	GetGameUIMan()->AddChatMessage(GetStringFromTable(102201),GP_CHAT_MISC);
			break;
		case LITTLE_GAME_EVENT_HURT:
		//	GetGameUIMan()->AddChatMessage(GetStringFromTable(102202),GP_CHAT_MISC);
			m_pGameSnake->Pause();
			PlayCollision();
			SetTimeAndCmd(2000,LAZY_CMD_PLAYER_BACK);
			UpdateLife();
			return;
		case LITTLE_GAME_EVENT_OUTLINE:
		//	GetGameUIMan()->AddChatMessage(GetStringFromTable(102203),GP_CHAT_MISC);
			m_pGameSnake->Pause();
			PlayCollision();
			UpdateLife();
			SetTimeAndCmd(2000,LAZY_CMD_PLAYER_BACK);
			UpdateLife();
			return;
		case LITTLE_GAME_EVENT_OVERTIME:
		//	GetGameUIMan()->AddChatMessage(GetStringFromTable(102204),GP_CHAT_MISC);
			m_pGameSnake->Pause();
			UpdateLife();
			GetDlgItem("Img_Lost")->Show(true);
			SetTimeAndCmd(2000,LAZY_CMD_STOP);
			return;
		case LITTLE_GAME_EVENT_OUTLIFE:
		//	GetGameUIMan()->AddChatMessage(GetStringFromTable(102205),GP_CHAT_MISC);
			m_pGameSnake->Pause();
			PlayCollision();
			GetDlgItem("Img_Lost")->Show(true);
			SetTimeAndCmd(2000,LAZY_CMD_STOP);
			return;
		case LITTLE_GAME_EVENT_WIN:
			SetTimeAndCmd(500,LAZY_CMD_WIN);
			return;
		case LITTLE_GAME_EVENT_PAUSE:
			return;
		default :
			break;
	};

	if(gameEvent != LITTLE_GAME_EVENT_NONE)m_iGameEvent = gameEvent;

	if(!m_pProg) m_pProg = (AUIProgress*) GetDlgItem("Prgs_Cruel");
	else m_pProg->SetProgress(m_pGameSnake->GetTimeProcess() * 100);

	int width,height;
	unsigned char dir;
	const unsigned char* maze = m_pGameSnake->GetMaze(width,height,dir);
	UpdateMaze(maze,width,height,dir);
}

void CDlgGameSnake::OnCommand_Start(const char* szCommand){
	if((m_iCmd  == LAZY_CMD_PLAYER_BACK && m_iTimeCnt > 0) || m_pGameSnake->GetState() == 0) return;
	SetTimeAndCmd(3500,LAZY_CMD_START);
	((AUIImagePicture*)GetDlgItem("Img_start"))->FixFrame(0);
	GetDlgItem("Img_start")->Show(true);
}

void CDlgGameSnake::OnCommand_Pause(const char* szCommand){
	if(m_pGameSnake)
		m_pGameSnake->Pause();
//	else
//		GetGameUIMan()->AddChatMessage(GetStringFromTable(102200),GP_CHAT_MISC); //snake游戏没有加载
}
void CDlgGameSnake::OnCommand_Stop(const char* szCommand){
	LazyStop();
}

void CDlgGameSnake::OnCommand_Close(const char* szCommand)
{
	Show(false);
}
void CDlgGameSnake::OnEvent_KeyDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj){
	if(!m_pGameSnake) return;
	int key = wParam;
	switch (key){
		case VK_UP:
			m_pGameSnake->SetDir(CECGameSnake::DIR_UP);
			break;
		case VK_DOWN:
			m_pGameSnake->SetDir(CECGameSnake::DIR_DOWN);
			break;
		case VK_LEFT:
			m_pGameSnake->SetDir(CECGameSnake::DIR_LEFT);
			break;
		case VK_RIGHT:
			m_pGameSnake->SetDir(CECGameSnake::DIR_RIGHT);
			break;
		default:
			break;
	};
}

// 加载迷宫数据
bool CDlgGameSnake::LoadMazeConfig(int iNo){
	// Load Snake Maze Data from Cfg
	AIniFile theIni;
	char szIniFile[MAX_PATH];
	sprintf(szIniFile, "%s\\Configs\\GameSnakeCfg.ini", af_GetBaseDir());
	if (!theIni.Open(szIniFile))
		return false;

	char szSect[32];
	sprintf(szSect, "maze%d", iNo);
	int height = theIni.GetValueAsInt(szSect,"height",10);
	int width = theIni.GetValueAsInt(szSect,"width",10);
	int foodNum = theIni.GetValueAsInt(szSect, "foodnum",5);
	int initPos = theIni.GetValueAsInt(szSect, "initpos",50);
	int initSize= theIni.GetValueAsInt(szSect, "initsize",1);
	unsigned char initDir = theIni.GetValueAsInt(szSect, "initdir",0);
	int speed = theIni.GetValueAsInt(szSect, "speed",1);
	int life = theIni.GetValueAsInt(szSect, "life",0);
	int time = theIni.GetValueAsInt(szSect, "time",0);
	int wallNum = 0;

	AString strPos;
	strPos = theIni.GetValueAsString(szSect, "wall");
	if (!strPos.IsEmpty())
	{
		int idx = -1, iStart = 0;
		do
		{
			iStart = idx + 1;
			int pos = atoi(&strPos[iStart]);
			if(pos>=0)wallNum++;
			idx = strPos.Find(",", iStart);
		}
		while (idx >= 0);
	}
	int* wallPos = new int[wallNum];
	theIni.GetValueAsIntArray(szSect, "wall",wallNum,wallPos);



	if(!m_pGameSnake)
		m_pGameSnake = new CECGameSnake();

	m_pGameSnake->Init(height,width,wallPos,wallNum,foodNum,initPos,initDir,speed,life,time);

	delete[] wallPos;

	// Close INI file
	theIni.Close();

	unsigned char dir;
	const unsigned char* maze = m_pGameSnake->GetMaze(width,height,dir);
	SetMaze(maze,width,height,dir);
	m_pGameSnake->GetHeadAndTail(m_iHead,m_iTail);
	UpdateLife();

	return true;
}

void CDlgGameSnake::UpdateMaze(const unsigned char* mazeMatrix, int width, int height,unsigned char ucDir){
	if(width * height <= 0) return;

	if(m_iWidth != width || m_iHeight != height){
//		GetGameUIMan()->AddChatMessage(GetStringFromTable(102207),GP_CHAT_MISC);
		return;
	}
	if(m_pGameSnake->GetDirCache() != -1)
		m_pGameSnake->SetDir(m_pGameSnake->GetDirCache());
	PAUIOBJECT pObj;
	const char* szTemplName = NULL;
	char szCellName[16];
	int iHead,iTail,iPercentage;
	m_pGameSnake->GetHeadAndTail(iHead,iTail);
	iPercentage = m_pGameSnake->GetPercentage();

	for(int i = width * height - 1; i>=0 ;i--){
		if(m_ucPtrMatrix[i] != mazeMatrix[i]){
			m_ucPtrMatrix[i] = mazeMatrix[i];
			if(m_ucPtrMatrix[i] == CECGameSnake::MDT_WALL){
				szTemplName = "Img_Block";
			}else if(m_ucPtrMatrix[i] == CECGameSnake::MDT_FOOD){
				szTemplName = "Img_Cruel";
			}else if(m_ucPtrMatrix[i] == CECGameSnake::MDT_BODY){
				szTemplName = "Img_Snake";	
			}
			sprintf(szCellName,"Img_Cell%d",i);
			pObj = GetDlgItem(szCellName);
			if(pObj && i!=iTail){
				m_LstDynamicObj.remove(GetDlgItem(szCellName));
				DeleteControl(GetDlgItem(szCellName));
			}

			if(szTemplName){
				pObj = CloneControl(GetDlgItem(szTemplName));	
				pObj->SetName(szCellName);
				pObj->SetPos(m_iMazeLeft + m_iMazeCellSize * (i % width),m_iMazeTop + m_iMazeCellSize * (i / width));	
				m_LstDynamicObj.push_back(pObj);
			}
			szTemplName = NULL;
			//ACString strObj;
			//strObj.Format(_AL("%d"),m_nListSize);
			//GetGameUIMan()->AddChatMessage(strObj,GP_CHAT_MISC);
		}
	}
	
	// 更新头
	if(m_iHead !=iHead || m_ucDir!=ucDir){
		AUIOBJECT_SETPROPERTY prop;
		if(m_iHead !=iHead){
			//ACString strObj;
			//strObj.Format(_AL("%d , 3"),m_iHead);
			//GetGameUIMan()->AddChatMessage(strObj,GP_CHAT_MISC);
			sprintf(szCellName,"Img_Cell%d",m_iHead);
			pObj = GetDlgItem(szCellName);
			if(pObj){
				if(pObj->GetData() == CECGameSnake::DIR_LEFT || pObj->GetData() == CECGameSnake::DIR_RIGHT){
					strcpy(prop.fn, "Game\\Snake\\BodyHorizon.tga");
					pObj->SetProperty("Image File",&prop);
					((AUIImagePicture*)pObj)->SetFrameNumber(1);
					((AUIImagePicture*)pObj)->FixFrame(0);
					//ACString strObj;
					//strObj.Format(_AL("%d , BodyHorizon.tga"),m_iHead);
					//GetGameUIMan()->AddChatMessage(strObj,GP_CHAT_MISC);
				}
				else if(pObj->GetData() == CECGameSnake::DIR_UP || pObj->GetData() == CECGameSnake::DIR_DOWN){
					strcpy(prop.fn, "Game\\Snake\\BodyErect.tga");
					pObj->SetProperty("Image File",&prop);
					((AUIImagePicture*)pObj)->SetFrameNumber(1);
					((AUIImagePicture*)pObj)->FixFrame(0);
					//ACString strObj;
					//strObj.Format(_AL("%d , BodyErect.tga"),m_iHead);
					//GetGameUIMan()->AddChatMessage(strObj,GP_CHAT_MISC);
				}else{
					((AUIImagePicture*)pObj)->FixFrame(3);
				}
			}
			m_iHead = iHead;
		}
		m_ucDir = ucDir;
		sprintf(szCellName,"Img_Cell%d",m_iHead);
		pObj = GetDlgItem(szCellName);	
		strcpy(prop.fn, GetPic(ucDir,iPercentage,true));
		if(pObj){
			pObj->SetProperty("Image File",&prop);
			((AUIImagePicture*)pObj)->SetFrameNumber(4);
		}
		//ACString strObj;
		//strObj.Format(_AL("%d"),m_ucDir);
		//GetGameUIMan()->AddChatMessage(strObj,GP_CHAT_MISC);
	}
	int frame = GetFrame(iPercentage,true);
	sprintf(szCellName,"Img_Cell%d",m_iHead);
	pObj = GetDlgItem(szCellName);	
	if(pObj){
		if(frame>=0)((AUIImagePicture*)pObj)->FixFrame(frame);
		pObj->Show(frame>=0);
		pObj->SetData(m_ucDir);
		//ACString strObj;
		//strObj.Format(_AL("%d,%d"),m_iHead,frame);
		//GetGameUIMan()->AddChatMessage(strObj,GP_CHAT_MISC);
	}

	// 更新尾
	if(m_iTail!=iTail){
		if(m_iTail != m_iHead){ // 头追到尾上，就不要删
			sprintf(szCellName,"Img_Cell%d",m_iTail);
			pObj = GetDlgItem(szCellName);
			m_LstDynamicObj.remove(pObj);
			DeleteControl(pObj);
		}
		m_iTail = iTail;
		sprintf(szCellName,"Img_Cell%d",m_iTail);
		pObj = GetDlgItem(szCellName);
		if(pObj){
			AUIOBJECT_SETPROPERTY prop;
			strcpy(prop.fn, GetPic(pObj->GetData(),iPercentage,false));
			pObj->SetProperty("Image File",&prop);
			((AUIImagePicture*)pObj)->SetFrameNumber(4);
			//ACString strObj;
			//strObj.Format(_AL("%d"),pObj->GetData());
			//GetGameUIMan()->AddChatMessage(strObj,GP_CHAT_MISC);
		}
	}
	frame = GetFrame(iPercentage,false);
	sprintf(szCellName,"Img_Cell%d",m_iTail);
	pObj = GetDlgItem(szCellName);
	if(pObj && m_iGameEvent!=LITTLE_GAME_EVENT_GAIN){ //　成长的过程不需要尾部突然变长然后慢慢缩回
		if(frame>=0)((AUIImagePicture*)pObj)->FixFrame(frame);
		pObj->Show(frame>=0);
	}

}
void CDlgGameSnake::SetMaze(const unsigned char* mazeMatrix, int width, int height,unsigned char ucDir){
	if(width * height <= 0) return;

	m_iWidth = width;
	m_iHeight = height;
	m_ucDir = ucDir;

	if(m_ucPtrMatrix){
		delete[] m_ucPtrMatrix;
		m_ucPtrMatrix =NULL;
	}
	m_ucPtrMatrix = new unsigned char[width*height];

	if(m_LstDynamicObj.size() > 0){
		for(std::list<AUIObject*>::const_iterator it = m_LstDynamicObj.begin();it!=m_LstDynamicObj.end();it++){
			(*it)->Show(false);
			if(!DeleteControl(*it)){
				AUI_ReportError(__LINE__, 1, "CDlgHint::Reset(), failed to delete Control");
			}
		}
		m_LstDynamicObj.clear();
	}

	// 建造四周围墙
	BuildBar();

	PAUIOBJECT pObj;
	const char* szTemplName = NULL;
	char szCellName[16];

	for(int i = width * height - 1; i>=0 ;i--){
		m_ucPtrMatrix[i] = mazeMatrix[i];
		if(m_ucPtrMatrix[i] == CECGameSnake::MDT_WALL){
			szTemplName = "Img_Block";
		}else if(m_ucPtrMatrix[i] == CECGameSnake::MDT_FOOD){
			szTemplName = "Img_Cruel";
		}else if(m_ucPtrMatrix[i] == CECGameSnake::MDT_BODY){
			szTemplName = "Img_Snake";	
		}
		if(szTemplName){
			pObj = CloneControl(GetDlgItem(szTemplName));
			sprintf(szCellName,"Img_Cell%d",i);
			pObj->SetName(szCellName);
			pObj->SetPos(m_iMazeLeft + m_iMazeCellSize * (i % width),m_iMazeTop + m_iMazeCellSize * (i / width));
			m_LstDynamicObj.push_back(pObj);
		}
		szTemplName = NULL;
	}

	if(!m_pGameSnake) return;	
	m_pGameSnake->GetHeadAndTail(m_iHead,m_iTail);
	AUIOBJECT_SETPROPERTY prop;
		
	// 更新头
	sprintf(szCellName,"Img_Cell%d",m_iHead);
	pObj = GetDlgItem(szCellName);
	if(pObj){
		if(m_ucDir == CECGameSnake::DIR_LEFT || m_ucDir == CECGameSnake::DIR_RIGHT){
			strcpy(prop.fn, "Game\\Snake\\Horizon.tga");
			pObj->SetProperty("Image File",&prop);
			((AUIImagePicture*)pObj)->SetFrameNumber(1);
			((AUIImagePicture*)pObj)->FixFrame(0);
			//ACString strObj;
			//strObj.Format(_AL("%d , BodyHorizon.tga"),m_iHead);
			//GetGameUIMan()->AddChatMessage(strObj,GP_CHAT_MISC);
		}
		else if(m_ucDir == CECGameSnake::DIR_UP || m_ucDir == CECGameSnake::DIR_DOWN){
			strcpy(prop.fn, "Game\\Snake\\Erect.tga");
			pObj->SetProperty("Image File",&prop);
			((AUIImagePicture*)pObj)->SetFrameNumber(1);
			((AUIImagePicture*)pObj)->FixFrame(0);
			//ACString strObj;
			//strObj.Format(_AL("%d , BodyErect.tga"),m_iHead);
			//GetGameUIMan()->AddChatMessage(strObj,GP_CHAT_MISC);
		}
		pObj->SetData(m_ucDir);
	}

	// 更新尾
	sprintf(szCellName,"Img_Cell%d",m_iTail);
	pObj = GetDlgItem(szCellName);
	if(pObj && m_iTail!= m_iHead){
		AUIOBJECT_SETPROPERTY prop;
		strcpy(prop.fn, GetPic(ucDir,m_pGameSnake->GetPercentage(),false));
		pObj->SetProperty("Image File",&prop);
		((AUIImagePicture*)pObj)->SetFrameNumber(4);
	}

	int frame = GetFrame(m_pGameSnake->GetPercentage(),false);
	if(pObj && m_iTail!= m_iHead){
		if(frame>=0)((AUIImagePicture*)pObj)->FixFrame(frame);
		pObj->Show(frame>=0);
	}
}
void CDlgGameSnake::UpdateLife(){
	AUIImagePicture* pObj = NULL;
	char szName[16];
	for(int i = 1;i<=3;i++){
		sprintf(szName,"Img_HP%d",i);
		pObj = (AUIImagePicture*)GetDlgItem(szName);
		if(pObj && m_pGameSnake){
			pObj->FixFrame(i <= m_pGameSnake->GetLife() ? 0:1);
			pObj = NULL;
		}
	}
}
inline const char* CDlgGameSnake::GetPic(unsigned char dir,int percentage,bool bHead){
	int pos,path;
	if(dir == CECGameSnake::DIR_UP || dir == CECGameSnake::DIR_DOWN) path = 0;
		else if(dir == CECGameSnake::DIR_UP_TO_RIGHT || dir == CECGameSnake::DIR_LEFT_TO_DOWN) path = 1;
		else if(dir == CECGameSnake::DIR_UP_TO_LEFT || dir == CECGameSnake::DIR_RIGHT_TO_DOWN) path = 2;
		else if(dir == CECGameSnake::DIR_DOWN_TO_RIGHT || dir == CECGameSnake::DIR_LEFT_TO_UP) path = 3;
		else if(dir == CECGameSnake::DIR_DOWN_TO_LEFT || dir == CECGameSnake::DIR_RIGHT_TO_UP) path = 4;
		else if(dir == CECGameSnake::DIR_LEFT || dir == CECGameSnake::DIR_RIGHT) path = 5;

	if(bHead){
		//if(percentage < 25) return "";
		pos = 0;
	}else{
		//if(percentage >= 100) return "";
		pos = 1;
	}

	if(dir == CECGameSnake::DIR_DOWN || dir == CECGameSnake::DIR_LEFT_TO_DOWN
		|| dir == CECGameSnake::DIR_RIGHT_TO_DOWN || dir == CECGameSnake::DIR_LEFT_TO_UP
		|| dir == CECGameSnake::DIR_RIGHT_TO_UP || dir == CECGameSnake::DIR_RIGHT) pos = 1 - pos;

	return szMovePath[path][pos];
}

inline int CDlgGameSnake::GetFrame(int percentage,bool bHead){
	if(bHead) return percentage/25;
	else return 3 - percentage/25;
}
void CDlgGameSnake::BuildBar(){
	int i;
	AUIObject* pObj = NULL;
	// 上
	for(i = 0; i < m_iWidth + 2 ;i++){	
		if(i == 0){
			pObj = GetDlgItem("Img_Wall"); // 左上
			((AUIImagePicture*)pObj)->FixFrame(0);
		}else if(i == m_iWidth + 1){
			pObj = CloneControl(GetDlgItem("Img_Wall")); // 右上
			((AUIImagePicture*)pObj)->FixFrame(2);
			pObj->SetPos(m_iMazeLeft - m_iMazeCellSize + m_iMazeCellSize * i,m_iMazeTop - m_iMazeCellSize);
			m_LstDynamicObj.push_back(pObj);
		}else{ 
			pObj = CloneControl(GetDlgItem("Img_Wall")); // 正上
			((AUIImagePicture*)pObj)->FixFrame(1);
			pObj->SetPos(m_iMazeLeft - m_iMazeCellSize + m_iMazeCellSize * i,m_iMazeTop - m_iMazeCellSize);
			m_LstDynamicObj.push_back(pObj);
		}
		
	}
	// 左 右
	for(i = 1; i < m_iHeight + 1 ;i++){
		pObj = CloneControl(GetDlgItem("Img_Wall")); // 左
		((AUIImagePicture*)pObj)->FixFrame(7);
		pObj->SetPos(m_iMazeLeft - m_iMazeCellSize,m_iMazeTop - m_iMazeCellSize + i * m_iMazeCellSize);
		m_LstDynamicObj.push_back(pObj);
		pObj = CloneControl(GetDlgItem("Img_Wall")); // 右
		((AUIImagePicture*)pObj)->FixFrame(3);
		pObj->SetPos(m_iMazeLeft + m_iWidth * m_iMazeCellSize,m_iMazeTop - m_iMazeCellSize + i * m_iMazeCellSize);
		m_LstDynamicObj.push_back(pObj);
	}
	// 下
	for(i = 0; i < m_iWidth + 2 ;i++){
		pObj = CloneControl(GetDlgItem("Img_Wall"));
		if(i == 0){
			((AUIImagePicture*)pObj)->FixFrame(6); // 左下
		}else if(i == m_iWidth + 1){
			((AUIImagePicture*)pObj)->FixFrame(4); // 右下
		}else{
			((AUIImagePicture*)pObj)->FixFrame(5); // 正下
		}
		pObj->SetPos(m_iMazeLeft - m_iMazeCellSize + m_iMazeCellSize * i,m_iMazeTop + m_iMazeCellSize * m_iHeight);
		m_LstDynamicObj.push_back(pObj);
	}
}
void CDlgGameSnake::PlayCollision(){
	AUIImagePicture* pObj = NULL;
	int x = m_iMazeLeft + m_iMazeCellSize * (m_iHead % m_iWidth);
	int y = m_iMazeTop + m_iMazeCellSize * (m_iHead / m_iWidth);
	switch(m_ucDir){ 
		case CECGameSnake::DIR_LEFT:
		case CECGameSnake::DIR_UP_TO_LEFT:
		case CECGameSnake::DIR_DOWN_TO_LEFT:
			x -= 16; 
			// pass down
		case CECGameSnake::DIR_RIGHT:
		case CECGameSnake::DIR_UP_TO_RIGHT:
		case CECGameSnake::DIR_DOWN_TO_RIGHT:
			y -= 8;
			pObj = (AUIImagePicture*)GetDlgItem("Img_BumpErect");
			break;
		case CECGameSnake::DIR_UP:
		case CECGameSnake::DIR_LEFT_TO_UP:
		case CECGameSnake::DIR_RIGHT_TO_UP:
			y -= 16;
			// pass down
		case CECGameSnake::DIR_DOWN:
		case CECGameSnake::DIR_LEFT_TO_DOWN:
		case CECGameSnake::DIR_RIGHT_TO_DOWN:
			x -= 8;
			pObj = (AUIImagePicture*)GetDlgItem("Img_BumpHorizon");
			break;
	};
	pObj->SetPos(x,y);
	pObj->Show(true);
}
void CDlgGameSnake::LazyStart(){
	if(m_pGameSnake){
		//int width,height;
		//unsigned char dir;
		//const unsigned char* maze = m_pGameSnake->GetMaze(width,height,dir);
		//SetMaze(maze,width,height,dir);
		m_pGameSnake->Start();
		GetDlgItem("Img_start")->Show(false);
		
	}
//	else
//		GetGameUIMan()->AddChatMessage(GetStringFromTable(102200),GP_CHAT_MISC); //snake游戏没有加载
}
void CDlgGameSnake::LazyStop(){
	if(m_pGameSnake){
		m_pGameSnake->Stop();
		int width,height;
		unsigned char dir;
		const unsigned char* maze = m_pGameSnake->GetMaze(width,height,dir);
		SetMaze(maze,width,height,dir);
	}
//	else
//		GetGameUIMan()->AddChatMessage(GetStringFromTable(102200),GP_CHAT_MISC); //snake游戏没有加载
	GetDlgItem("Img_Lost")->Show(false);
	GetDlgItem("Img_Win")->Show(false);
	UpdateLife();
	if(!m_pProg) m_pProg = (AUIProgress*) GetDlgItem("Prgs_Cruel");
	else m_pProg->SetProgress(m_pGameSnake->GetTimeProcess() * 100);
}
void CDlgGameSnake::LazyPlayerBack(){
	if(m_pGameSnake){
		m_pGameSnake->PlayerBack();
		int width,height;
		unsigned char dir;
		const unsigned char* maze = m_pGameSnake->GetMaze(width,height,dir);
		SetMaze(maze,width,height,dir);
	}
}
void CDlgGameSnake::LazyWin(){
//	GetGameUIMan()->AddChatMessage(GetStringFromTable(102206),GP_CHAT_MISC);
	m_pGameSnake->Pause();
	GetDlgItem("Img_Win")->Show(true);
	SetTimeAndCmd(2000,LAZY_CMD_STOP);

	//如果玩家身上有小游戏对应的任务，则直接完成
	GetHostPlayer()->FinishTinyGameTask(m_iGameId);
	
}

void CDlgGameSnake::StartStage(int iStage)
{
	m_iGameId = iStage;
	Show(true);
}

CDlgWindowsGfx::CDlgWindowsGfx()
{
}

CDlgWindowsGfx::~CDlgWindowsGfx()
{
}

void CDlgWindowsGfx::OnShowDialog()
{
	m_dwRefreshTime = GetTickCount();
}

void CDlgWindowsGfx::OnTick()
{
	DWORD dwTimeNow = GetTickCount();
	if(dwTimeNow - m_dwRefreshTime > 5000)
		Show(false);
}


AUI_BEGIN_COMMAND_MAP(CDlgSpring2012, CDlgBase)

AUI_ON_COMMAND("Btn_GetQuest",		OnCommandGet)

AUI_END_COMMAND_MAP()

CDlgSpring2012::CDlgSpring2012()
{
}

CDlgSpring2012::~CDlgSpring2012()
{
}

//点击直接完成
void CDlgSpring2012::OnCommandGet(const char * szCommand)
{
	GetHostPlayer()->FinishTinyGameTask(4);
	Show(false);
}

