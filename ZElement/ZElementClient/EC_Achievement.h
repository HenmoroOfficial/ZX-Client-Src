/********************************************************************
	created:	3/6/2009
	author:		liudong
	
	purpose:   成就系统
	Copyright (C) All Rights Reserved
*********************************************************************/
#pragma once

#include <hashmap.h>
#include <AWString.h>
#include "ScriptValue.h"

class  CECAchievementMan;
struct sAchievement;

namespace S2C
{
	struct achievement;
	struct player_achievement;
	struct player_achievement_finish;
	struct player_achievement_active;
	struct player_premiss_data;
	struct player_premiss_finish;	
	struct player_achievement_map;
	struct player_obtain_achieve_award;
}

//成就条件的公用参数，从本地脚本中读取。
struct sAchievementCondCommon
{
	sAchievementCondCommon();

	unsigned int    id;			//条件id
	unsigned int	dataSize;	//条件自定义数据大小（字节）
};

//成就条件。
struct sAchievementCond
{
    sAchievementCond(const unsigned int _idAchievement,const int _index);
	sAchievementCond(const sAchievementCond& src);
	sAchievementCond& operator=(const sAchievementCond& rhs);
	~sAchievementCond();

	void GetStrDesc(AWString& str);			//获取条件的简要描述
	void GetStrInfo(AWString& str);			//获取条件详细信息
	void SetData(const char* data, const unsigned int size);
	bool GetDataInt(int& data) const;		//从条件的自定义数据中获取一个整数
	bool GetProgress(int &current, int &max);	//获取条件的完成进度

	unsigned int				id;			//条件id
	bool						bFinish;	//该条件是否已经完成
	int                         index;		//这个条件在所属成就中的索引(第几个条件)，0 based.
	char*                       pData;		//条件自定义数据，该数据是从服务器获得的。

protected:
	unsigned int				idAchievement;//条件所属成就的id。
};

//成就
struct sAchievement
{
	sAchievement();
	void GetStrDesc(AWString& str);					//获取成就的描述文字。
	void SetFinish(const bool finish);

	unsigned int					id;				//成就id
	AWString						name;			//成就名字
	AWString						strDesc;		//成就描述
	AWString						strPreCondDesc;	//成就条件列表的前缀文字
	AWString						strMsg;			//成就达成时对应的消息文字
	unsigned int                    iClass;			//成就所属类别
	unsigned int					iSubClass;		//成就所属子类别
	unsigned int					awardScore;		//完成该成就奖励的成就点
	unsigned int					awardItem;		//完成该成就奖励的道具
	unsigned int					awardMoney;		//完成该奖励获取的金钱
	bool                            bFinish;		//成就是否完成
	bool                            bActive;		//成就是否激活
	bool							bMarkFinish;	//成就是否有完成时间
	int								iFinishTime;	//成就完成时间
	abase::vector<unsigned int>		vecPremissID;	//前提成就列表
	abase::vector<sAchievementCond> vecCond;		//条件列表
	bool							bGetAward;		//是否领取了成就奖励
	AWString						strOwnerName;	//成就获得者姓名
};

struct sAchievementSubClass
{
	sAchievementSubClass();
	unsigned int id;	// 成就类别
	AWString     name;  // 成就类别名字
	unsigned int awardNum;	//未领取的奖励数
};

//成就类别
struct sAchievementClass
{
	sAchievementClass();
	unsigned int id;	//成就类别
	AWString     name;	//成就类别名字
	abase::vector<sAchievementSubClass> vecSubClass;  // 成就子类
};

//一个玩家所有成就的集合
class CECAchievementMan
{
public:
	CECAchievementMan();
	~CECAchievementMan();
	

	//初始化，从脚本中加载数据。
	bool Init();
	void Release();
	
	bool UpdateData(const S2C::player_achievement* p,const unsigned int buffSize);			// 全部成就数据	
    bool UpdateData(const S2C::player_achievement_finish* p,const unsigned int buffSize);	// 完成成就 	
	bool UpdateData(const S2C::player_achievement_active* p,const unsigned int buffSize);	// 激活成就(前提成就都完成了)	
	bool UpdateData(const S2C::player_premiss_data* p,const unsigned int buffSize);			//成就条件变化	
	bool UpdateData(const S2C::player_premiss_finish* p,const unsigned int buffSize);		// 完成成就条件
	bool UpdateData(const S2C::player_achievement_map* p,const unsigned int buffSize);		// 其他玩家成就完成状况。
	bool UpdateData(const S2C::player_obtain_achieve_award* p, const unsigned int buffSize);// 获得成就奖励
	
    sAchievement*			GetAchievement(const unsigned int idAchievement);				//获取成就
	int                     GetAchievementPoint(){ return m_AchievePoint; }					//获取玩家的总的成就点
	int						GetCurAchievementPoint() { return m_CurAchievePoint; }			//获取玩家的当前成就点
	abase::hash_map<unsigned int, sAchievement>& GetAchievementSet() { return m_mapAchievement; } // 获取成就集合
	abase::hash_map<unsigned int, sAchievementClass>& GetAchievementClassSet() { return s_mapAchievementClass; } // 获取成就集合
	
	static const sAchievementCondCommon*	GetCondCommon(const unsigned int idCond);		//获取成就条件的公用参数
	static const sAchievementClass*			GetAchievementClass(const unsigned int idClass);//获取成就类别

	static bool PreLoadDataFromScript();

protected:
	//update active achievement.
	bool _UpdateData(unsigned int& sz, const S2C::achievement* p,const unsigned int buffSize);

protected:
	abase::hash_map<unsigned int,sAchievement>					m_mapAchievement;		//所有成就
	int															m_AchievePoint;			//玩家总的成就点
	int															m_CurAchievePoint;		//当前的成就点

	static abase::hash_map<unsigned int,sAchievementCondCommon> s_mapCondCommon;		//成就条件的公用参数
	static abase::hash_map<unsigned int,sAchievementClass>		s_mapAchievementClass;	//所有的成就类别

	static CECAchievementMan									s_DefaultData;			//默认数据，从脚本中读取的配置文件信息。
};






