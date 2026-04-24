// Filename	: DlgGamePoker.h
// Creator	: Chen Zhixin
// Date		: 2009/03/30

#pragma once

#include "DlgBase.h"
#include "vector.h"
#include "EC_FGPDataType.h"

#define MAX_CARDS 20

class AUIScroll;
class AUIImagePicture;
class AUILabel;
class AUIStillImageButton;
class AUIListBox;
class AUITextArea;
class AUIProgress;

class CDlgGamePoker : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

	enum {
		STATE_WAIT_START = 0,
		STATE_INTURN_CALL,
		STATE_WAIT_CALL,
		STATE_INTURN_DISCARD,
		STATE_WAIT_DISCARD,	
		STATE_COOLING,
	};
	
	enum {
		PLAYERSTATE_ENTER = 0,
		PLAYERSTATE_READY,
		PLAYERSTATE_GAME,
		PLAYERSTATE_DROP,
	};

	struct room_player_info
	{
		int roleid;
		char direction;
		char playerstate;
		char state;
		char index;
		char cardsize;
		char gender;
		INT64 score;
		int win_count;
		int lose_count;
		int draw_count;
		int escape_count;
		ACString name;
		abase::vector<char> deskcard;
		abase::vector<char> hidecard;
	};
public:
	CDlgGamePoker();
	virtual ~CDlgGamePoker();
	
	void OnCommand_Ready(const char * szCommand);
	void OnCommand_Cancel(const char * szCommand);
	void OnCommand_Leave(const char * szCommand);
	void OnCommand_Discard(const char * szCommand);
	void OnCommand_Auction(const char * szCommand);
	void OnCommand_Pass(const char * szCommand);
	void OnCommand_Minimize(const char * szCommand);
	void OnCommand_Help(const char * szCommand);
	void OnCommand_ReturnHall(const char * szCommand);
	
	void OnEventLButtonUp_Card(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventLButtonUp_List(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	void PokerCmd_NotifyRoomInfo(const FG_S2C::INFO::room_info& fgci);															//得到本房间信息
	void PokerCmd_PlayerEnterRoom(const FG_S2C::INFO::player_base_info& fgci);													//其他玩家进入房间
 	void PokerCmd_PlayerLeaveRoom(int roleid);																					//玩家离开房间
	void PokerCmd_RoomIsReady(int roomid);
	void PokerCmd_RoomDispatchNormalCard(char cardsize, char* card, int next_roleid);											//发牌
	void PokerCmd_PlayerInturnNormalCard(int roleid);										
	void PokerCmd_PlayerThrowNormalCard(int roleid, int nextroleid, char card_size, char* cards);								//玩家出牌
	void PokerCmd_PlayerPickupNormalCard(char cardsize, char* card, int roleid);												//地主拿底牌
	void PokerCmd_PlayerGiveupNormalCard(int roleid, int nextroleid);															//放弃出牌或叫牌
	void PokerCmd_PlayerAuction(int roleid, int nextroleid, int score);															//玩家叫牌
	void PokerCmd_RoomGameEnd(int roleid, char reason);																			//牌局结束
	void PokerCmd_LandlordReonlineDataSend(const FG_S2C::INFO::landlord_reonline_info& fgci);									//断线重进
	
	void UpdateCards(int direction, int num = MAX_CARDS);
	void UpdatePlayState(char direction, char state);
	void UpdateRoomInfo(const FG_S2C::INFO::player_base_info& fgci);
	void SetPlayingState(int next_roleid, char state1, char state2);
	int  GetRoomId() { return m_RoomId;}
	bool IsInRoom(void);
	void QuitGame();
	void RefreshResult(const FG_S2C::CMD::fgcmd_landlord_result_broadcast& fgci);
protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();

	void ResetPlayerInfo(int index);
	void RefreshPlayerDetails(void);
	
	AUIImagePicture		*m_pImg_PlayPoker[3][MAX_CARDS];
	AUIImagePicture		*m_pImg_ShowPoker[3][MAX_CARDS];
	AUIImagePicture		*m_pImg_PlayerHead[3];
	AUIImagePicture		*m_pImg_PlayerHeadF[3];
	AUIImagePicture		*m_pImg_SelectPoker[3];
	AUIImagePicture		*m_pImg_HeadIcon;
	AUIImagePicture		*m_pImg_PlayerStatus[3];
	AUIImagePicture		*m_pImg_PlayerClock[3];
	AUIImagePicture		*m_pImg_PokerFinish;	

	AUIStillImageButton	*m_pBtn_Order[4];
	AUIStillImageButton	*m_pBtn_Play[3];
	AUIStillImageButton	*m_pBtn_Ready;

 	AUILabel			*m_pLab_PlayerStatus[3];
	AUILabel			*m_pLab_PlayerName[3];
	AUILabel			*m_pLab_Ready[3];
	AUILabel			*m_pLab_CallPoint[3];
	AUIProgress			*m_pProg_Time;
	AUIListBox			*m_pLst_Player;
	AUILabel			*m_pTxt_Name;
	AUILabel			*m_pTxt_Title;
	AUILabel			*m_pTxt_WinPercent;
	AUILabel			*m_pTxt_Score;
	bool				m_bChoose[MAX_CARDS];
	room_player_info	m_playerinfo[3];

	char				m_hostDir;
	abase::vector<char>		m_vecOwnCard;
	char				m_CurSel;
	DWORD				m_dwDispatchTime;				//发牌开始时间
	DWORD				m_dwShowTime;					//看底牌时间
	DWORD				m_dwResultTime;					//显示胜负时间
	DWORD				m_dwWaitingTime;				//时间滚动条
	int					m_RoomId;
	int					m_LandlordID;
	bool				m_bIsGaming;
};

