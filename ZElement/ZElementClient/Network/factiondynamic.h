#ifndef _GNET_FACTION_DYNAMIC_H_
#define _GNET_FACTION_DYNAMIC_H_

#include <vector>
/*
#include "hometownpacket.h"
#include "hometownprotocol.h"
#include <time.h>
*/

//帮派动态


//帮派动态

namespace FAC_DYNAMIC
{
#pragma pack(1)
	enum ACTION_TYPE
	{
		SIEGE_READY = 1,
		TERRI_READY,
		MEMBER_CHANGE,
		FAC_PK,
		CHSH_DONATE,// 捐献帮派元宝
		PUT_AUCTION_ITEM, //拍卖品上架
		WIN_AUCTION_ITEM, //拍卖成功
		BUY_AUCTION_ITEM, //购买无匹上架拍卖行
		BUILDING_UPGRADE,// 建设或者升级建筑
		BUILDING_COMPLETE, // 建筑完成
	};
	struct dynamic_header
	{
		int time;
		int action;
	};
	//城战系统公告
	struct siege_ready
	{
		enum
		{
			DEFENDER = 1,
			ATTACKER,
			ASSISTANT,
		};
		int battletime;
		char self_role; //攻城帮 守城帮 or 协助帮
		char defendernamesize;
		char defendername[20];
		char attackernamesize;
		char attackername[20];
		char assistnamesize;
		char assistname[20];
	};
	//社稷（领土）战系统公告
	struct terri_ready
	{
		int tid;
		int battletime;
	};
	//成员变更
	struct member_change
	{
		enum
		{
			MEMBER_JOIN = 1,
			MEMBER_LEAVE,
			FAMILY_JOIN,
			FAMILY_LEAVE,
		};
		char type;
		char rolenamesize;
		char rolename[20];
		char familynamesize;
		char familyname[20];
	};
	//帮战
	struct fac_pk
	{
		int battletime;
		char nType;				// 0 发起挑战 1 被挑战
		char enemynamesize;
		char enemyname[20];
	};
	struct cash_donate
	{
		int cash;
		char rolenamesize;
		char rolename[20];
	};
	struct put_auction_item
	{
		int itemid;
		int last_time; // 拍卖持续时间
		int task_id; //玩家完成哪个任务获得的物品
		char rolenamesize;
		char rolename[20];
	};
	struct win_auction_item
	{
		int itemid;
		int cost;
		char rolenamesize;
		char rolename[20];
	};
	struct buy_auction_item
	{
		int itemid;
		int last_time; // 拍卖持续时间
		char rolenamesize;
		char rolename[20];
	};
	struct building_upgrade
	{
		int tid;
		int level;
		int cost_grass;
		int cost_mine;
		int cost_monster_food;
		int cost_monster_core;
		char rolenamesize;
		char rolename[20];
	};
	struct building_complete
	{
		int tid;
		int level;
	};
#pragma pack()
};


namespace GNET
{
//	class FactionDynamic
//	{
//	private:
//		enum
//		{
//			MAX_SIZE = 50,
//			PAGE_SIZE = 5,
//		};
////		const size_t _max_num;
//		std::vector<Octets> _dynamics;
//		bool dirty;
//
//		void Push(const Octets & dynamic)
//		{
//			_dynamics.push_back(dynamic);
//			if (_dynamics.size() > MAX_SIZE)
//				_dynamics.erase(_dynamics.begin());
//		}
//		void Record(int time, int action, const void *data, size_t size)
//		{
//			FUNGAME::WRAPPER wrapper;
//			wrapper << time;
//			wrapper << action;
//			wrapper.push_back(data, size);
//			Push(wrapper.get_buf());
//			SetDirty(true);
//		}
//		template <typename T>
//		void RecordDynamic(int time, int action, const T & dynamic)
//		{
//			Record(time, action, &dynamic, sizeof(T));
//		}
//	public:
//		FactionDynamic() : dirty(false) {}
//		bool IsDirty() const { return dirty; }
//		void SetDirty(bool b) { dirty = b; }
//		void GetData(int pageid, int & totalsize, std::vector<Octets> & data) const
//		{
//			if (pageid < 0 || (int)(pageid*PAGE_SIZE) < 0 ||
//					pageid*PAGE_SIZE >= (int)_dynamics.size())
//				return;
//			totalsize = _dynamics.size();
//			std::vector<Octets>::const_iterator it = _dynamics.begin(), ie = _dynamics.end();
//			std::advance(it, pageid*PAGE_SIZE);
//			for (int i = 0; i < PAGE_SIZE && it != ie; i++,++it)
//				data.push_back(*it);
//		}
//		std::vector<Octets> & GetData() const
//		{
//			return _dynamics;
//		}
//		void LoadData(const std::vector<Octets> & data)
//		{
//			_dynamics = data;
//			int now = Timer::GetTime();
//			std::vector<Octets>::iterator it;
//			if (it = _dynamics.begin(); it != _dynamics.end();)
//			{
//				if (it->size() < FAC_DYNAMIC::dynamic_header)
//				{
//					Log::log(LOG_ERR, "factiondynamic size %d error", it->size());
//					it++;
//				}
//				else
//				{
//					FAC_DYNAMIC::dynamic_header * header = (FAC_DYNAMIC::dynamic_header *)it->begin();
//					if (now - header->time < WEEK_SECONDS)
//						break;
//					else
//					{
//						LOG_TRACE("obsolete faction dynamic, time %d type %d", header->time, header->action);
//						it = _dynamics.erase(it);
//						SetDirty(true);
//					}
//				}
//			}
//		}
//		/*
//		Marshal::OctetsStream& Save(Marshal::OctetsStream &os)
//		{
//			os << _dynamics;
//			return os;
//		}
//
//		const Marshal::OctetsStream& Load(const Marshal::OctetsStream &os)
//		{
//			_dynamics.clear();
//			os >> _dynamics;
//			return os;
//		}
//		*/
//
//		void RecordDynamic(int time, int action, const FAC_DYNAMIC::member_change & dynamic)
//		{
//			RecordDynamic(time, action, dynamic);
//		}
//	};
};

#endif
