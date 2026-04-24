/*
* FILE: EC_Configs.h
*
* DESCRIPTION: 
*
* CREATED BY: Duyuxin, 2004/8/27
*
* HISTORY: 
*
* Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
*/

#pragma once

#include "AString.h"
#include "A3DVector.h"
#include "ABaseDef.h"
#include <bitset>

/************************************************************************
小改动不需要增加版本，如增加一个bool值
增加版本需要做的修改
CECConfigsDataBase 派生一个CECConfigsData*
新派生的类必须包含DefaultUserSettings()
修改下面的enum
修改bool CECConfigsDataLoader::Load(const void* pDataBuf, int iDataSize)中对应位置
************************************************************************/
enum
{
	VER_23 =0,
	VER_24,
	VER_NUM,
	VER_BASE = 23,
	EC_CONFIG_VERSION = VER_BASE+VER_24,
};


///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////

#pragma pack(1)

class CECConfigsDataBase
{
public:
	CECConfigsDataBase(){	m_nVersion = 23; }
	virtual ~CECConfigsDataBase(){}
	virtual bool LoadData(const void* pDataBuf, int iDataSize) = 0;
	int  GetVersion() { return m_nVersion; }
	virtual CECConfigsDataBase& operator = (CECConfigsDataBase* data) = 0;
protected:
	int		m_nVersion;
};

class CECConfigsData23 : public CECConfigsDataBase
{
public:
	CECConfigsData23(){	m_nVersion = 23; }
	~CECConfigsData23(){}
	bool LoadData(const void* pDataBuf, int iDataSize);
	CECConfigsDataBase& operator = (CECConfigsDataBase* data23);
	enum
	{
		EC_AUTOREPLY_LEN		= 64,
		EC_BLACKLIST_LEN		= 20,
		EC_AUTOTEAM_LEN			= 5,
		EC_USERCHANNEL_NUM		= 6,
		EC_ACTUALCHANNEL_NUM	= 19,
		EC_COMBOSKILL_NUM		= 7,
		EC_COMBOSKILL_LEN		= 12,
		EC_COMBOSKILL_ICONNUM	= 18,
	};
	
	struct EC_COMBOSKILL
	{
		BYTE	nIcon;
		short	idSkill[EC_COMBOSKILL_LEN];
	};
	
	struct EC_VIDEO_SETTING
	{
		bool bPlayerHP;
		bool bPlayerMP;
		bool bPlayerEXP;
		bool bPlayerHeadText;
		bool bPlayerName;
		bool bPlayerFaction;
		bool bPlayerFamily;
		bool bPlayerTitle;
		bool bPlayerShop;
		bool bPlayerSelfName;
		bool bPlayerTalk;
		bool bPlayerBubble;
		bool bMonsterName;
		bool bNPCName;
		bool bGoodsName;
		bool bModelLimit;
		BYTE nDistance;
		bool bMapMonster;
		bool bMapNPC;
		bool bMapPlayer;
		BYTE nEffect;
		bool bShowCustomize;
		BYTE nQuickbarNum;
		EC_COMBOSKILL comboSkill[EC_COMBOSKILL_NUM];
		bool bPlayerCircle;
		char cNameSize;
		char cBossKey; //最高位表示是否用boss key，后面的位表示哪个key
		char reserve2;
		char reserve3;
	} m_vs;
	
	struct EC_GAME_SETTING
	{
		bool bNoTeamRequest;
		bool bNoTradeRequest;
		bool bTurnaround;
		bool bReverseWheel;
		bool bChannel[EC_USERCHANNEL_NUM][EC_ACTUALCHANNEL_NUM];
		bool bAutoReply;
		ACHAR szAutoReply[EC_AUTOREPLY_LEN+1];
		float fCamTurnSpeed;
		float fCamZoomSpeed;
		BYTE nFontSize;
		bool bAtk_NoWhite;
		bool bAtk_NoRed;
		bool bAtk_NoBlue;
		bool bAtk_NoMafia;
		BYTE nPrintScreenType;
		bool bShowTalismanDlg;
		bool bEquipCompare;
		bool bHideSecretary;
	} m_gs;
	
	struct EC_BLACKLIST_SETTING
	{
		int idPlayer[EC_BLACKLIST_LEN];
	} m_bs;
	
	struct EC_AUTOTEAM_SETTING
	{
		bool bUse;
		bool bAutoAll;
		bool bAutoFriend;
		bool bAutoFamily;
		bool bAutoGuild;
		ACHAR szName[EC_AUTOTEAM_LEN][8];
	} m_ts;
	
	struct EC_GAME_CLIENTDATA
	{
		unsigned int uSize;
		int  nCutsceneData[4];
		int  nReserve1;
		int  nReserve2;
		int  nReserve3;
		int  nReserve4;
	} m_gcd;
	
protected:
private:
};

// 23-24 自动入队由5->10，增加同服务器频道，增加一些标志：头顶圈子显示
class CECConfigsData24 : public CECConfigsDataBase
{
public:
	CECConfigsData24()
	{
		m_nVersion = 24; 
		memset(&m_vs, 0, sizeof(m_vs));
		memset(&m_gs, 0, sizeof(m_gs));
		memset(&m_bs, 0, sizeof(m_bs));
		memset(&m_ts, 0, sizeof(m_ts));
		memset(&m_gcd, 0, sizeof(m_gcd));
	}
	~CECConfigsData24(){}
	bool LoadData(const void* pDataBuf, int iDataSize);
	CECConfigsDataBase& operator = (CECConfigsDataBase* data23);
	
	enum
	{
		EC_AUTOREPLY_LEN		= 64,
		EC_BLACKLIST_LEN		= 20,
		EC_AUTOTEAM_LEN			= 10,
		EC_USERCHANNEL_NUM		= 7,
		EC_ACTUALCHANNEL_NUM	= 20,
		EC_COMBOSKILL_NUM		= 7,
		EC_COMBOSKILL_LEN		= 12,
		EC_COMBOSKILL_ICONNUM	= 18,
	};
	
	struct EC_COMBOSKILL
	{
		BYTE	nIcon;
		short	idSkill[EC_COMBOSKILL_LEN];
	};
	
	struct EC_VIDEO_SETTING
	{
		bool bPlayerHP;
		bool bPlayerMP:1;
		bool bPlayerDP:7;
		bool bPlayerEXP;
		bool bPlayerHeadText;
		bool bPlayerName;
		bool bPlayerFaction;
		bool bPlayerFamily;
		bool bPlayerTitle;
		bool bPlayerShop;
		bool bPlayerSelfName;
		bool bPlayerTalk;
		bool bPlayerBubble;
		bool bMonsterName;
		bool bNPCName;
		bool bGoodsName;
		bool bModelLimit;
		BYTE nDistance;
		bool bMapMonster;
		bool bMapNPC;
		bool bMapPlayer;
		BYTE nEffect;
		bool bShowCustomize;
		BYTE nQuickbarNum;
		EC_COMBOSKILL comboSkill[EC_COMBOSKILL_NUM];
		bool bPlayerCircle;
		char cNameSize;
		char cBossKey; //最高位表示是否用boss key，后面的位表示哪个key
		bool bPlayerServer;
		char cUpdateVersion;		// 每次更新弹一次相应界面
	} m_vs;
	
	struct EC_GAME_SETTING
	{
		bool bNoTeamRequest;
		bool bNoTradeRequest;
		bool bTurnaround;
		bool bReverseWheel;
		std::bitset<32> bChannel[EC_USERCHANNEL_NUM];
		bool bChannelReserve[86];
		bool bAutoReply;
		ACHAR szAutoReply[EC_AUTOREPLY_LEN+1];
		float fCamTurnSpeed;
		float fCamZoomSpeed;
		BYTE nFontSize;
		bool bAtk_NoWhite:1;
		bool bAtk_NoZone:1;
		bool bAtk_Reserve:6;
		bool bAtk_NoRed;
		bool bAtk_NoBlue;
		bool bAtk_NoMafia;
		BYTE nPrintScreenType;
		bool bShowTalismanDlg;
		bool bEquipCompare;
		bool bHideSecretary;
	} m_gs;
	
	struct EC_BLACKLIST_SETTING
	{
		int idPlayer[EC_BLACKLIST_LEN];
	} m_bs;
	
	struct EC_AUTOTEAM_SETTING
	{
		bool bUse:1;
		bool bAutoAll:1;
		bool bAutoFriend:1;
		bool bAutoFamily:1;
		bool bAutoGuild:1;
		bool bAutoServer:1;
		bool bAutoReserve:2;
		ACHAR szName[EC_AUTOTEAM_LEN][8];
	} m_ts;
	
	struct EC_GAME_CLIENTDATA
	{
		unsigned int uSize;
		int  nCutsceneData[4];
		int  nHideDailyPrompLoginHint:1;
		int  nTaskTrace:1;
		int  nReserve1:30;
		int  nDigTreaId[3];
	} m_gcd;
	
	void DefaultUserSettings(EC_VIDEO_SETTING *pvs, EC_GAME_SETTING *pgs);
	protected:
	private:
};

typedef CECConfigsData24 CECConfigsCur;

class CECConfigsDataLoader
{
public:
	CECConfigsDataLoader(CECConfigsCur* pData):m_pData(pData) {}
	~CECConfigsDataLoader(){}

	bool Load(const void* pDataBuf, int iDataSize);
	void Default();
private:
	CECConfigsCur* m_pData;

};

#pragma pack()