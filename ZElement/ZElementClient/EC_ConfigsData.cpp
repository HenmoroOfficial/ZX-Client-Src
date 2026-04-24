#include "EC_ConfigsData.h"
#include "EC_RTDebug.h"
#include "EC_NetDef.h"


/************************************************************************/
/*						CECConfigsData23                                */
/************************************************************************/
bool CECConfigsData23::LoadData(const void* pDataBuf, int iDataSize)
{
	CECDataReader dr((void*)pDataBuf, iDataSize);
	m_vs = *(EC_VIDEO_SETTING*)dr.Read_Data(sizeof (EC_VIDEO_SETTING));
	m_gs = *(EC_GAME_SETTING*)dr.Read_Data(sizeof (EC_GAME_SETTING));
	m_bs = *(EC_BLACKLIST_SETTING*)dr.Read_Data(sizeof (EC_BLACKLIST_SETTING));
	m_ts = *(EC_AUTOTEAM_SETTING*)dr.Read_Data(sizeof (EC_AUTOTEAM_SETTING));
	m_gcd = *(EC_GAME_CLIENTDATA*)dr.Read_Data(sizeof (EC_GAME_CLIENTDATA));
	
	for(int i = 0; i < EC_COMBOSKILL_NUM; i++ )
		if( m_vs.comboSkill[i].nIcon > EC_COMBOSKILL_ICONNUM )
		{
			for(int j = 0; j < EC_COMBOSKILL_NUM; j++ )
				m_vs.comboSkill[j].nIcon = 0;
			break;
		}
	m_gs.fCamTurnSpeed = 10.0f;
	m_gs.fCamZoomSpeed = 1.0f;
	return true;
}

CECConfigsDataBase& CECConfigsData23::operator = (CECConfigsDataBase* data23)
{
	if(data23->GetVersion()==m_nVersion)
	{
		memcpy(this, data23, sizeof(CECConfigsData23));
		return *this;
	}
	else
		throw 0;

	return *this;
}

/************************************************************************/
/*						CECConfigsData24                                */
/************************************************************************/
bool CECConfigsData24::LoadData(const void* pDataBuf, int iDataSize)
{
	CECDataReader dr((void*)pDataBuf, iDataSize);
	m_vs = *(EC_VIDEO_SETTING*)dr.Read_Data(sizeof (EC_VIDEO_SETTING));
	m_gs = *(EC_GAME_SETTING*)dr.Read_Data(sizeof (EC_GAME_SETTING));
	m_bs = *(EC_BLACKLIST_SETTING*)dr.Read_Data(sizeof (EC_BLACKLIST_SETTING));
	m_ts = *(EC_AUTOTEAM_SETTING*)dr.Read_Data(sizeof (EC_AUTOTEAM_SETTING));
	m_gcd = *(EC_GAME_CLIENTDATA*)dr.Read_Data(sizeof (EC_GAME_CLIENTDATA));
	m_gs.fCamTurnSpeed = 10.0f;
	m_gs.fCamZoomSpeed = 1.0f;
	int i;
	for (i=0; i<EC_USERCHANNEL_NUM; ++i)
	{
		m_gs.bChannel[i][GP_CHAT_SPEAKER] = m_gs.bChannel[i][GP_CHAT_FARCRY];
		m_gs.bChannel[i][GP_CHAT_RUMOR] = m_gs.bChannel[i][GP_CHAT_FARCRY];
	}
	
	for(i = 0; i < EC_COMBOSKILL_NUM; i++ )
		if( m_vs.comboSkill[i].nIcon > EC_COMBOSKILL_ICONNUM )
		{
			for(int j = 0; j < EC_COMBOSKILL_NUM; j++ )
				m_vs.comboSkill[j].nIcon = 0;
			break;
		}
	return true;
}

CECConfigsDataBase& CECConfigsData24::operator = (CECConfigsDataBase* data23)
{
	if(data23->GetVersion()==m_nVersion)
	{
		memcpy(this, data23, sizeof(CECConfigsData24));
		return *this;
	}
	else if(data23->GetVersion()!=(m_nVersion-1))
		throw 0;

	CECConfigsData23* pData23 = (CECConfigsData23*)data23;
	memcpy(&m_vs, &pData23->m_vs, sizeof(pData23->m_vs));
	m_vs.bPlayerCircle = true;
	memcpy(&m_gs, &pData23->m_gs, sizeof(pData23->m_gs));
	m_gs.bAtk_NoZone = true;
	memcpy(&m_bs, &pData23->m_bs, sizeof(pData23->m_bs));
	m_ts.bUse = m_ts.bUse;
	m_ts.bAutoAll = pData23->m_ts.bAutoAll;
	m_ts.bAutoFriend = pData23->m_ts.bAutoFriend;
	m_ts.bAutoFamily = pData23->m_ts.bAutoFamily;
	m_ts.bAutoGuild = pData23->m_ts.bAutoGuild;
	m_ts.bAutoServer = 0;
	m_ts.bAutoReserve = 0;
	memcpy(&m_ts.szName[0][0], &pData23->m_ts.szName[0][0], 5*8*sizeof(ACHAR));
	memcpy(&m_ts, &pData23->m_ts, sizeof(pData23->m_ts));
	memcpy(&m_gcd, &pData23->m_gcd, sizeof(pData23->m_gcd));
	
	std::bitset<32> a_bChannel[EC_USERCHANNEL_NUM] =
	{
		// 本地 呐喊 组队 帮派 密语 伤害 战斗 交易 系统 广播 其它 家族 圈子	同服 预留1 预留2 金钱 经验 拾取 杀人 预留3
		// 0、1组合从右往左读
		std::bitset<32>(std::string("011110111111100011111")) ,// 普通
		std::bitset<32>(std::string("011110100011100010111")) ,// 组队
		std::bitset<32>(std::string("000000100011100010010")) ,// 密语
		std::bitset<32>(std::string("010000001111100011000")) ,// 帮派
		std::bitset<32>(std::string("011110100011100010011")) ,// 系统
		std::bitset<32>(std::string("000000100011100010010")) ,// 世界
		std::bitset<32>(std::string("010000011011100010000")) ,// 同服
	};
	memcpy(m_gs.bChannel, a_bChannel, sizeof(std::bitset<32>) * EC_USERCHANNEL_NUM);

	return *this;
}

void CECConfigsData24::DefaultUserSettings(	EC_VIDEO_SETTING *pvs, EC_GAME_SETTING *pgs)
{
	if( pvs )
	{
		EC_VIDEO_SETTING vs;
		vs = *pvs;
		vs.bPlayerHP = false;
		vs.bPlayerMP = false;
		vs.bPlayerDP = false;
		vs.bPlayerEnergy = true;
		vs.bPlayerEXP = false;
		vs.bPlayerHeadText = true;
		vs.bPlayerName = true;
		vs.bPlayerFaction = true;
		vs.bPlayerCircle = false;
		vs.bPlayerFamily = true;
		vs.bPlayerTitle = true;
		vs.bPlayerShop = true;
		vs.bPlayerSelfName = true;
		vs.bPlayerTalk = true;
		vs.bPlayerBubble = false;
		vs.bMonsterName = true;
		vs.bNPCName = true;
		vs.bGoodsName = true;
		vs.bModelLimit = false;
		vs.nDistance = 50;
		vs.bMapMonster = false;
		vs.bMapNPC = true;
		vs.bMapPlayer = true;
		vs.nEffect = 4;
		vs.bShowCustomize = true;
		vs.nQuickbarNum = 1;
		vs.cNameSize = 1;
		vs.cBossKey = 0;
		vs.bPlayerServer = true;
		*pvs = vs;
	}

	if( pgs ) 
	{
		EC_GAME_SETTING gs;
		gs = *pgs;
		gs.bNoTeamRequest = false;
		gs.bNoTradeRequest = false;
		gs.bTurnaround = false;
		gs.bReverseWheel = false;
		gs.bAutoReply = false;
		gs.szAutoReply[0] = '\0';
		gs.fCamTurnSpeed = 10.0f;
		gs.fCamZoomSpeed = 1.0f;
		gs.nFontSize = 2;
		gs.bAtk_NoMafia = true;
		gs.bAtk_NoZone = true;
		gs.bAtk_NoWhite = true;
		gs.bAtk_NoBlue  = true;
		gs.bAtk_NoRed	= true;
		gs.nPrintScreenType = 0;
		gs.bShowTalismanDlg = true;
		gs.bEquipCompare = true;
		gs.bHideSecretary = true;
		
		std::bitset<32> a_bChannel[EC_USERCHANNEL_NUM] =
		{	// 1	2	3		4	5	6		7	8	9	10		11	12	13	14		15	16	17		18	19	20		21	
			// 本地 呐喊 组队 帮派 密语 伤害 战斗 交易 系统 广播 其它 家族 圈子	同服 预留1 预留2 金钱 经验 拾取 杀人 预留3
			// 01组合从右往左读
			std::bitset<32>(std::string("011110111111100011111")) ,// 普通
			std::bitset<32>(std::string("011110100011100010111")) ,// 组队
			std::bitset<32>(std::string("000000100011100010010")) ,// 密语
			std::bitset<32>(std::string("010000001111100011000")) ,// 帮派
			std::bitset<32>(std::string("011110100011100010011")) ,// 系统
			std::bitset<32>(std::string("000000100011100010010")) ,// 世界
			std::bitset<32>(std::string("010000011011100010000")) ,// 同服
		};
		memcpy(gs.bChannel, a_bChannel, sizeof(std::bitset<32>) * EC_USERCHANNEL_NUM);
		*pgs = gs;
	}
}
/************************************************************************/
/*						CECConfigsData25                                */
/************************************************************************/
bool CECConfigsData25::LoadData(const void* pDataBuf, int iDataSize)
{
	CECDataReader dr((void*)pDataBuf, iDataSize);
	m_vs = *(EC_VIDEO_SETTING*)dr.Read_Data(sizeof (EC_VIDEO_SETTING));
	m_gs = *(EC_GAME_SETTING*)dr.Read_Data(sizeof (EC_GAME_SETTING));
	m_bs = *(EC_BLACKLIST_SETTING*)dr.Read_Data(sizeof (EC_BLACKLIST_SETTING));
	m_ts = *(EC_AUTOTEAM_SETTING*)dr.Read_Data(sizeof (EC_AUTOTEAM_SETTING));
	m_gcd = *(EC_GAME_CLIENTDATA*)dr.Read_Data(sizeof (EC_GAME_CLIENTDATA));
	m_gs.fCamTurnSpeed = 10.0f;
	m_gs.fCamZoomSpeed = 1.0f;
	int i;
	for (i=0; i<EC_USERCHANNEL_NUM; ++i)
	{
		m_gs.bChannel[i][GP_CHAT_SPEAKER] = m_gs.bChannel[i][GP_CHAT_FARCRY];
		m_gs.bChannel[i][GP_CHAT_RUMOR] = m_gs.bChannel[i][GP_CHAT_FARCRY];
	}

	for(i = 0; i < EC_COMBOSKILL_NUM; i++ )
		if( m_vs.comboSkill[i].nIcon > EC_COMBOSKILL_ICONNUM )
		{
			for(int j = 0; j < EC_COMBOSKILL_NUM; j++ )
				m_vs.comboSkill[j].nIcon = 0;
			break;
		}
		return true;
}

CECConfigsDataBase& CECConfigsData25::operator = (CECConfigsDataBase* pData)
{
	if(pData->GetVersion()==m_nVersion)
	{
		memcpy(this, pData, sizeof(CECConfigsData25));
		return *this;
	}
	else if(pData->GetVersion()!=(m_nVersion-1) && pData->GetVersion()!=(m_nVersion-2))
		throw 0;

	std::bitset<32> a_bChannel[EC_USERCHANNEL_NUM] =
	{
		// 本地 呐喊 组队 帮派 密语 伤害 战斗 交易 系统 广播 其它 家族 圈子	同服 预留1 预留2 金钱 经验 拾取 杀人 预留3
		// 0、1组合从右往左读
		std::bitset<32>(std::string("011110111111100011111")) ,// 普通
		std::bitset<32>(std::string("011110100011100010111")) ,// 组队
		std::bitset<32>(std::string("000000100011100010010")) ,// 密语
		std::bitset<32>(std::string("010000001111100011000")) ,// 帮派
		std::bitset<32>(std::string("011110100011100010011")) ,// 系统
		std::bitset<32>(std::string("000000100011100010010")) ,// 世界
		std::bitset<32>(std::string("010000011011100010000")) ,// 同服
		std::bitset<32>(std::string("011110111111100011111")) ,//自定义1
		std::bitset<32>(std::string("011110111111100011111")) ,//自定义2
	};
    
	if(pData->GetVersion() == (m_nVersion-2))
	{
		CECConfigsData23* pData23 = (CECConfigsData23*)pData;
		memcpy(&m_vs, &pData23->m_vs, sizeof(pData23->m_vs));
		m_vs.bPlayerCircle = true;
		memcpy(&m_gs, &pData23->m_gs, sizeof(pData23->m_gs));
		m_gs.bAtk_NoZone = true;
		memcpy(&m_bs, &pData23->m_bs, sizeof(pData23->m_bs));
		m_ts.bUse = m_ts.bUse;
		m_ts.bAutoAll = pData23->m_ts.bAutoAll;
		m_ts.bAutoFriend = pData23->m_ts.bAutoFriend;
		m_ts.bAutoFamily = pData23->m_ts.bAutoFamily;
		m_ts.bAutoGuild = pData23->m_ts.bAutoGuild;
		m_ts.bAutoServer = 0;
		m_ts.bAutoReserve = 0;
		memcpy(&m_ts.szName[0][0], &pData23->m_ts.szName[0][0], 5*8*sizeof(ACHAR));
		memcpy(&m_ts, &pData23->m_ts, sizeof(pData23->m_ts));
		memcpy(&m_gcd, &pData23->m_gcd, sizeof(pData23->m_gcd));
	    memcpy(m_gs.bChannel, a_bChannel, sizeof(std::bitset<32>) * (EC_USERCHANNEL_NUM-2));
	}
	else if(pData->GetVersion() == (m_nVersion-1))
	{
		CECConfigsData24* pData24 = (CECConfigsData24*)pData;
		memcpy(&m_vs,&pData24->m_vs,sizeof(pData24->m_vs));
		memcpy(&m_bs,&pData24->m_bs,sizeof(pData24->m_bs));
		memcpy(&m_ts,&pData24->m_ts,sizeof(pData24->m_ts));
		memcpy(&m_gcd,&pData24->m_gcd,sizeof(pData24->m_gcd));

		m_gs.bNoTeamRequest = pData24->m_gs.bNoTeamRequest;
		m_gs.bNoTradeRequest = pData24->m_gs.bNoTradeRequest;
		m_gs.bTurnaround = pData24->m_gs.bTurnaround;
		m_gs.bReverseWheel = pData24->m_gs.bReverseWheel;
		m_gs.bAutoReply = pData24->m_gs.bAutoReply;
		m_gs.fCamTurnSpeed = pData24->m_gs.fCamTurnSpeed;
		m_gs.fCamZoomSpeed = pData24->m_gs.fCamZoomSpeed; 
		m_gs.nFontSize = pData24->m_gs.nFontSize;
		m_gs.bAtk_NoMafia = pData24->m_gs.bAtk_NoMafia;
		m_gs.bAtk_NoZone = pData24->m_gs.bAtk_NoZone;
		m_gs.bAtk_NoWhite = pData24->m_gs.bAtk_NoWhite;
		m_gs.bAtk_NoBlue  = pData24->m_gs.bAtk_NoBlue;
		m_gs.bAtk_NoRed	= pData24->m_gs.bAtk_NoRed;
		m_gs.bAtk_Reserve = pData24->m_gs.bAtk_Reserve;
		m_gs.nPrintScreenType = pData24->m_gs.nPrintScreenType;
		m_gs.bShowTalismanDlg = pData24->m_gs.bShowTalismanDlg;
		m_gs.bEquipCompare = pData24->m_gs.bEquipCompare;
		m_gs.bHideSecretary = pData24->m_gs.bHideSecretary;
		m_gs.szAutoReply[0] = '\0';
		memcpy(&m_gs.bChannelReserve,&pData24->m_gs.bChannelReserve,sizeof(pData24->m_gs.bChannelReserve));
		memcpy(&m_gs.szAutoReply,&pData24->m_gs.szAutoReply,sizeof(pData24->m_gs.szAutoReply));
		memcpy(&m_gs.bChannel,&pData24->m_gs.bChannel,sizeof(std::bitset<32>)*(EC_USERCHANNEL_NUM-2));
		m_gs.bChannel[EC_USERCHANNEL_NUM-1] = a_bChannel[EC_USERCHANNEL_NUM-1];
		m_gs.bChannel[EC_USERCHANNEL_NUM-2] = a_bChannel[EC_USERCHANNEL_NUM-2];

	}


	return *this;
}

void CECConfigsData25::DefaultUserSettings(	EC_VIDEO_SETTING *pvs, EC_GAME_SETTING *pgs)
{
	if( pvs )
	{
		EC_VIDEO_SETTING vs;
		vs = *pvs;
		vs.bPlayerHP = false;
		vs.bPlayerMP = false;
		vs.bPlayerDP = false;
		vs.bPlayerEnergy = true;
		vs.bPlayerEXP = false;
		vs.bPlayerHeadText = true;
		vs.bPlayerName = true;
		vs.bPlayerFaction = true;
		vs.bPlayerCircle = false;
		vs.bPlayerFamily = true;
		vs.bPlayerTitle = true;
		vs.bPlayerShop = true;
		vs.bPlayerSelfName = true;
		vs.bPlayerTalk = true;
		vs.bPlayerBubble = false;
		vs.bMonsterName = true;
		vs.bNPCName = true;
		vs.bGoodsName = true;
		vs.bModelLimit = false;
		vs.nDistance = 50;
		vs.bMapMonster = false;
		vs.bMapNPC = true;
		vs.bMapPlayer = true;
		vs.nEffect = 4;
		vs.bShowCustomize = true;
		vs.nQuickbarNum = 1;
		vs.cNameSize = 1;
		vs.cBossKey = 0;
		vs.bPlayerServer = true;
		*pvs = vs;
	}

	if( pgs ) 
	{
		EC_GAME_SETTING gs;
		gs = *pgs;
		gs.bNoTeamRequest = false;
		gs.bNoTradeRequest = false;
		gs.bTurnaround = false;
		gs.bReverseWheel = false;
		gs.bAutoReply = false;
		gs.szAutoReply[0] = '\0';
		gs.fCamTurnSpeed = 10.0f;
		gs.fCamZoomSpeed = 1.0f;
		gs.nFontSize = 2;
		gs.bAtk_NoMafia = true;
		gs.bAtk_NoZone = true;
		gs.bAtk_NoWhite = true;
		gs.bAtk_NoBlue  = true;
		gs.bAtk_NoRed	= true;
		gs.nPrintScreenType = 0;
		gs.bShowTalismanDlg = true;
		gs.bEquipCompare = true;
		gs.bHideSecretary = true;

		std::bitset<32> a_bChannel[EC_USERCHANNEL_NUM] =
		{	// 1	2	3		4	5	6		7	8	9	10		11	12	13	14		15	16	17		18	19	20		21	
			// 本地 呐喊 组队 帮派 密语 伤害 战斗 交易 系统 广播 其它 家族 圈子	同服 预留1 预留2 金钱 经验 拾取 杀人 预留3
			// 01组合从右往左读
			std::bitset<32>(std::string("011110111111100011111")) ,// 普通
			std::bitset<32>(std::string("011110100011100010111")) ,// 组队
			std::bitset<32>(std::string("000000100011100010010")) ,// 密语
			std::bitset<32>(std::string("010000001111100011000")) ,// 帮派
			std::bitset<32>(std::string("011110100011100010011")) ,// 系统
			std::bitset<32>(std::string("000000100011100010010")) ,// 世界
			std::bitset<32>(std::string("010000011011100010000")) ,// 同服
			std::bitset<32>(std::string("011110111111100011111")) ,//自定义1
			std::bitset<32>(std::string("011110111111100011111")) ,//自定义2
		};
		memcpy(gs.bChannel, a_bChannel, sizeof(std::bitset<32>) * EC_USERCHANNEL_NUM);
		*pgs = gs;
	}
}

/************************************************************************/
/*						CECConfigsData26                                */
/************************************************************************/
bool CECConfigsData26::LoadData(const void* pDataBuf, int iDataSize)
{
	CECDataReader dr((void*)pDataBuf, iDataSize);
	m_vs = *(EC_VIDEO_SETTING*)dr.Read_Data(sizeof (EC_VIDEO_SETTING));
	m_gs = *(EC_GAME_SETTING*)dr.Read_Data(sizeof (EC_GAME_SETTING));
	m_bs = *(EC_BLACKLIST_SETTING*)dr.Read_Data(sizeof (EC_BLACKLIST_SETTING));
	m_ts = *(EC_AUTOTEAM_SETTING*)dr.Read_Data(sizeof (EC_AUTOTEAM_SETTING));
	m_gcd = *(EC_GAME_CLIENTDATA*)dr.Read_Data(sizeof (EC_GAME_CLIENTDATA));
	m_frs = *(EC_FRIENDSREMARKS*)dr.Read_Data(sizeof(EC_FRIENDSREMARKS));
	m_gs.fCamTurnSpeed = 10.0f;
	m_gs.fCamZoomSpeed = 1.0f;
	int i;
	for (i=0; i<EC_USERCHANNEL_NUM; ++i)
	{
		m_gs.bChannel[i][GP_CHAT_SPEAKER] = m_gs.bChannel[i][GP_CHAT_FARCRY];
		m_gs.bChannel[i][GP_CHAT_RUMOR] = m_gs.bChannel[i][GP_CHAT_FARCRY];
	}

	for(i = 0; i < EC_COMBOSKILL_NUM; i++ )
	  if( m_vs.comboSkill[i].nIcon > EC_COMBOSKILL_ICONNUM )
		{
			for(int j = 0; j < EC_COMBOSKILL_NUM; j++ )
				m_vs.comboSkill[j].nIcon = 0;
			break;
		}
		return true;
}

CECConfigsDataBase& CECConfigsData26::operator = (CECConfigsDataBase* pData)
{
	if(pData->GetVersion()==m_nVersion)
	{
		memcpy(this, pData, sizeof(CECConfigsData26));
		return *this;
	}
	else if(pData->GetVersion()!=(m_nVersion-1) && pData->GetVersion()!=(m_nVersion-2) && pData->GetVersion()!=(m_nVersion-3))
		throw 0;

	std::bitset<32> a_bChannel[EC_USERCHANNEL_NUM] =
	{
		// 本地 呐喊 组队 帮派 密语 伤害 战斗 交易 系统 广播 其它 家族 圈子	同服 预留1 预留2 金钱 经验 拾取 杀人 预留3
		// 0、1组合从右往左读
		std::bitset<32>(std::string("011110111111100011111")) ,// 普通
		std::bitset<32>(std::string("011110100011100010111")) ,// 组队
		std::bitset<32>(std::string("000000100011100010010")) ,// 密语
		std::bitset<32>(std::string("010000001111100011000")) ,// 帮派
		std::bitset<32>(std::string("011110100011100010011")) ,// 系统
		std::bitset<32>(std::string("000000100011100010010")) ,// 世界
		std::bitset<32>(std::string("010000011011100010000")) ,// 同服
		std::bitset<32>(std::string("011110111111100011111")) ,//自定义1
		std::bitset<32>(std::string("011110111111100011111")) ,//自定义2
	};

	if(pData->GetVersion() == (m_nVersion-3))
	{
		CECConfigsData23* pData23 = (CECConfigsData23*)pData;
		memcpy(&m_vs, &pData23->m_vs, sizeof(pData23->m_vs));
		m_vs.bPlayerCircle = true;
		memcpy(&m_gs, &pData23->m_gs, sizeof(pData23->m_gs));
		m_gs.bAtk_NoZone = true;
		memcpy(&m_bs, &pData23->m_bs, sizeof(pData23->m_bs));
		m_ts.bUse = m_ts.bUse;
		m_ts.bAutoAll = pData23->m_ts.bAutoAll;
		m_ts.bAutoFriend = pData23->m_ts.bAutoFriend;
		m_ts.bAutoFamily = pData23->m_ts.bAutoFamily;
		m_ts.bAutoGuild = pData23->m_ts.bAutoGuild;
		m_ts.bAutoServer = 0;
		m_ts.bAutoReserve = 0;
		memcpy(&m_ts.szName[0][0], &pData23->m_ts.szName[0][0], 5*8*sizeof(ACHAR));
		memcpy(&m_ts, &pData23->m_ts, sizeof(pData23->m_ts));
		memcpy(&m_gcd, &pData23->m_gcd, sizeof(pData23->m_gcd));
		memcpy(m_gs.bChannel, a_bChannel, sizeof(std::bitset<32>) * (EC_USERCHANNEL_NUM-2));
	}
	else if(pData->GetVersion() == (m_nVersion-2))
	{
		CECConfigsData24* pData24 = (CECConfigsData24*)pData;
		memcpy(&m_vs,&pData24->m_vs,sizeof(pData24->m_vs));
		memcpy(&m_bs,&pData24->m_bs,sizeof(pData24->m_bs));
		memcpy(&m_ts,&pData24->m_ts,sizeof(pData24->m_ts));
		memcpy(&m_gcd,&pData24->m_gcd,sizeof(pData24->m_gcd));

		m_gs.bNoTeamRequest = pData24->m_gs.bNoTeamRequest;
		m_gs.bNoTradeRequest = pData24->m_gs.bNoTradeRequest;
		m_gs.bTurnaround = pData24->m_gs.bTurnaround;
		m_gs.bReverseWheel = pData24->m_gs.bReverseWheel;
		m_gs.bAutoReply = pData24->m_gs.bAutoReply;
		m_gs.fCamTurnSpeed = pData24->m_gs.fCamTurnSpeed;
		m_gs.fCamZoomSpeed = pData24->m_gs.fCamZoomSpeed; 
		m_gs.nFontSize = pData24->m_gs.nFontSize;
		m_gs.bAtk_NoMafia = pData24->m_gs.bAtk_NoMafia;
		m_gs.bAtk_NoZone = pData24->m_gs.bAtk_NoZone;
		m_gs.bAtk_NoWhite = pData24->m_gs.bAtk_NoWhite;
		m_gs.bAtk_NoBlue  = pData24->m_gs.bAtk_NoBlue;
		m_gs.bAtk_NoRed	= pData24->m_gs.bAtk_NoRed;
		m_gs.bAtk_Reserve = pData24->m_gs.bAtk_Reserve;
		m_gs.nPrintScreenType = pData24->m_gs.nPrintScreenType;
		m_gs.bShowTalismanDlg = pData24->m_gs.bShowTalismanDlg;
		m_gs.bEquipCompare = pData24->m_gs.bEquipCompare;
		m_gs.bHideSecretary = pData24->m_gs.bHideSecretary;
		m_gs.szAutoReply[0] = '\0';
		memcpy(&m_gs.bChannelReserve,&pData24->m_gs.bChannelReserve,sizeof(pData24->m_gs.bChannelReserve));
		memcpy(&m_gs.szAutoReply,&pData24->m_gs.szAutoReply,sizeof(pData24->m_gs.szAutoReply));
		memcpy(&m_gs.bChannel,&pData24->m_gs.bChannel,sizeof(std::bitset<32>)*(EC_USERCHANNEL_NUM-2));
		m_gs.bChannel[EC_USERCHANNEL_NUM-1] = a_bChannel[EC_USERCHANNEL_NUM-1];
		m_gs.bChannel[EC_USERCHANNEL_NUM-2] = a_bChannel[EC_USERCHANNEL_NUM-2];

	}
	else if(pData->GetVersion() == (m_nVersion-1))
	{
		CECConfigsData25* pData25 = (CECConfigsData25*)pData;
		memcpy(&m_vs,&pData25->m_vs,sizeof(pData25->m_vs));
		memcpy(&m_bs,&pData25->m_bs,sizeof(pData25->m_bs));
		memcpy(&m_ts,&pData25->m_ts,sizeof(pData25->m_ts));
		memcpy(&m_gcd,&pData25->m_gcd,sizeof(pData25->m_gcd));
		memcpy(&m_gs,&pData25->m_gs,sizeof(pData25->m_gs));
	}


	return *this;
}

void CECConfigsData26::DefaultUserSettings(	EC_VIDEO_SETTING *pvs, EC_GAME_SETTING *pgs)
{
	if( pvs )
	{
		EC_VIDEO_SETTING vs;
		vs = *pvs;
		vs.bPlayerHP = false;
		vs.bPlayerMP = false;
		vs.bPlayerDP = false;
		vs.bPlayerEnergy = true;
		vs.bPlayerEXP = false;
		vs.bPlayerHeadText = true;
		vs.bPlayerName = true;
		vs.bPlayerFaction = true;
		vs.bPlayerCircle = false;
		vs.bPlayerFamily = true;
		vs.bPlayerTitle = true;
		vs.bPlayerShop = true;
		vs.bPlayerSelfName = true;
		vs.bPlayerTalk = true;
		vs.bPlayerBubble = false;
		vs.bMonsterName = true;
		vs.bNPCName = true;
		vs.bGoodsName = true;
		vs.bModelLimit = false;
		vs.nDistance = 50;
		vs.bMapMonster = false;
		vs.bMapNPC = true;
		vs.bMapPlayer = true;
		vs.nEffect = 4;
		vs.bShowCustomize = true;
		vs.nQuickbarNum = 1;
		vs.cNameSize = 1;
		vs.cBossKey = 0;
		vs.bPlayerServer = true;
		*pvs = vs;
	}

	if( pgs ) 
	{
		EC_GAME_SETTING gs;
		gs = *pgs;
		gs.bNoTeamRequest = false;
		gs.bNoTradeRequest = false;
		gs.bTurnaround = false;
		gs.bReverseWheel = false;
		gs.bAutoReply = false;
		gs.szAutoReply[0] = '\0';
		gs.fCamTurnSpeed = 10.0f;
		gs.fCamZoomSpeed = 1.0f;
		gs.nFontSize = 2;
		gs.bAtk_NoMafia = true;
		gs.bAtk_NoZone = true;
		gs.bAtk_NoWhite = true;
		gs.bAtk_NoBlue  = true;
		gs.bAtk_NoRed	= true;
		gs.nPrintScreenType = 0;
		gs.bShowTalismanDlg = true;
		gs.bEquipCompare = true;
		gs.bHideSecretary = true;

		std::bitset<32> a_bChannel[EC_USERCHANNEL_NUM] =
		{	// 1	2	3		4	5	6		7	8	9	10		11	12	13	14		15	16	17		18	19	20		21	
			// 本地 呐喊 组队 帮派 密语 伤害 战斗 交易 系统 广播 其它 家族 圈子	同服 预留1 预留2 金钱 经验 拾取 杀人 预留3
			// 01组合从右往左读
			std::bitset<32>(std::string("011110111111100011111")) ,// 普通
			std::bitset<32>(std::string("011110100011100010111")) ,// 组队
			std::bitset<32>(std::string("000000100011100010010")) ,// 密语
			std::bitset<32>(std::string("010000001111100011000")) ,// 帮派
			std::bitset<32>(std::string("011110100011100010011")) ,// 系统
			std::bitset<32>(std::string("000000100011100010010")) ,// 世界
			std::bitset<32>(std::string("010000011011100010000")) ,// 同服
			std::bitset<32>(std::string("011110111111100011111")) ,//自定义1
			std::bitset<32>(std::string("011110111111100011111")) ,//自定义2
		};
		memcpy(gs.bChannel, a_bChannel, sizeof(std::bitset<32>) * EC_USERCHANNEL_NUM);
		*pgs = gs;
	}
}
bool CECConfigsDataLoader::Load(const void* pDataBuf, int iDataSize)
{
	CECDataReader dr((void*)pDataBuf, iDataSize);
	DWORD dwVer = dr.Read_DWORD();
	
	if (dwVer > EC_CONFIG_VERSION)
	{
		ASSERT(dwVer > EC_CONFIG_VERSION);
		throw CECException(CECException::TYPE_DATAERR);
	}
	
	if (dwVer < VER_BASE)
	{
		Default();
		return true;
	}
	CECConfigsDataBase* pDatas[VER_NUM];
	pDatas[VER_23] = new CECConfigsData23;
	pDatas[VER_24] = new CECConfigsData24;
	pDatas[VER_25] = new CECConfigsData25;
	pDatas[VER_26] = new CECConfigsData26;
	
	int index = dwVer-VER_BASE;
	pDatas[index]->LoadData((BYTE*)pDataBuf+sizeof(DWORD), iDataSize-=sizeof(DWORD));
	while (index<(VER_NUM-1))
	{
		*pDatas[index+1] = pDatas[index];
		index++;
	}
	*m_pData = ((CECConfigsCur*)pDatas[index]);
	
	for (int i=0; i<VER_NUM; i++)
	{
		delete pDatas[i];
	}
	return true;
}

void CECConfigsDataLoader::Default()
{
	m_pData->DefaultUserSettings(&m_pData->m_vs, &m_pData->m_gs);
	memset(&m_pData->m_bs, 0, sizeof (m_pData->m_bs));
	memset(&m_pData->m_ts, 0, sizeof (m_pData->m_ts));
	memset(&m_pData->m_gcd, 0, sizeof (m_pData->m_gcd));
	
	/** 版本VER_24的时候加入第一次进游戏播放动画功能
	 *  VER_24之前版本需要播动画 nCutsceneData置0
	 *  之后版本进入不需要播动画
	if (EC_CONFIG_VERSION>=(VER_BASE+VER_24))
	{
		m_pData->m_gcd.nCutsceneData[0] = 10;	// 人族2，神族8
	}
	*/
	
	//	This fix a bug in some version
	m_pData->m_gs.fCamTurnSpeed = 10.0f;
	m_pData->m_gs.fCamZoomSpeed = 1.0f;
}