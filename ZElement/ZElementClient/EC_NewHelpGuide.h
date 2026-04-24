// Filename	: EC_NewHelpGuide.h
// Creator	: Wang Dongliang
// Date		: 2013/07/10
// Desc		: 指引系统


#pragma  once

#include "AWString.h"
#include "AString.h"
#include "hashmap.h"
#include "vector.h"

class CLuaState;
class _AUITREEVIEW_ITEM;
struct tagPOINT;
struct tagSIZE;

class CECNewHelpGuide{

public:

	enum
	{
		AIMGUIDE_TASKID_MAX             = 200,   //任务ID个数
		AIMGUIDE_STARTLEVEL_MAX         = 11,    //星级个数
		AIMGUIDE_TYPE_MAX               = 6,     //类型个数
		AIMGUIDE_TYPE_GETMONEY          = 0x1,   //赚钱
		AIMGUIDE_TYPE_LEVELUP           = 0x2,   //升级
		AIMGUIDE_TYPE_MAKEFRIENDS       = 0x4,   //泡妞
		AIMGUIDE_TYPE_ENHANCESTRENGTH   = 0x8,   //提升实力
		AIMGUIDE_TYPE_BORING            = 0x10,  //无聊
		AIMGUIDE_TYPE_WASTEMONEY        = 0x20,  //花钱
	};


	enum
	{
		EAIMGUIDE_GETMONEY,
		EAIMGUIDE_LEVELUP,
		EAIMGUIDE_MAKEFRIENDS,
		EAIMGUIDE_ENHANCESTRENTH,
		EAIMGUIDE_SOBORING,
		EAIMGUIDE_WASTEMONEY,
		EAIMGUIDE_NUM,
	};


	enum SortType
	{
		ST_REQLEVEL,   //按需求的最低等级排序
		ST_STARTLEVEL, //按星级排序
	};

	typedef struct _SAimGuideInfo 
	{
		int iId;										//ID
		unsigned short   uiType;						//类型掩码
		AWString		strName;						//名称
		AString			strIcon;						//图标名字
		AWString		strDay;							//开启日期
		AWString		strTime;						//开启时间
		unsigned long	ulMinLevel;						//等级下限
		unsigned long	ulMaxLevel;						//等级上限
		AWString		strLevelInfo;					//等级信息
		bool			bReborn;						//是否飞升
		int				iPermitCount;					//次数
		AWString		strCondition;					//条件

		UINT			uiNumID;						//任务ID数量
		UINT			uiTaskID[AIMGUIDE_TASKID_MAX];	//任务ID
		
		AWString		strNPC;							//NPC名称
		bool			bTaskNpc;						//是否为任务NPC
		int				iNpcMapid;						//目标地图ID
		AWString		strNpcPos;						//目标位置
		bool			bIsTransport;					//是否可以传送

		BYTE            istarLevel[AIMGUIDE_STARTLEVEL_MAX]; //星级信息（1,15,30,45...150）
		AWString		strDesc;							 //描述

		AWString        strBtnName;                     //按钮名称（打开界面指引）
		AWString        strDlgName;                     //界面名称（打开界面指引）

		_SAimGuideInfo()
			:iId(0),uiType(0),ulMinLevel(1),ulMaxLevel(1024),
			bReborn(false),iPermitCount(-1),uiNumID(0),
			bTaskNpc(false),iNpcMapid(0),bIsTransport(false)
		{
			memset(uiTaskID,0,sizeof(uiTaskID));
			memset(istarLevel,0,sizeof(istarLevel));
		}
		bool IsType_GetMoney(){return (uiType & AIMGUIDE_TYPE_GETMONEY) != 0;}
		bool IsType_LevelUp(){return (uiType & AIMGUIDE_TYPE_LEVELUP) != 0;}
		bool IsType_MakeFriends(){return (uiType & AIMGUIDE_TYPE_MAKEFRIENDS) != 0;}
		bool IsType_EnhanceStrength(){return (uiType & AIMGUIDE_TYPE_ENHANCESTRENGTH) != 0;}
		bool IsType_Boring(){return (uiType & AIMGUIDE_TYPE_BORING) != 0;}
		bool IsType_WasteMoney(){return (uiType & AIMGUIDE_TYPE_WASTEMONEY) != 0;}
		bool IsType(int type)
		{
			switch(type)
			{
                case EAIMGUIDE_GETMONEY:
					return IsType_GetMoney();
				case EAIMGUIDE_LEVELUP:
					return IsType_LevelUp();
				case EAIMGUIDE_MAKEFRIENDS:
					return IsType_MakeFriends();
				case EAIMGUIDE_ENHANCESTRENTH:
                    return IsType_EnhanceStrength();
				case EAIMGUIDE_SOBORING:
					return IsType_Boring();
				case EAIMGUIDE_WASTEMONEY:
					return IsType_WasteMoney();
			}
			return false;
		}
  
		int GetStarLevel();
	}SAimGuideInfo;

	//指引步骤
	typedef struct _SGuideStep
	{
		_SGuideStep():stepId(0),type(0){}
		//步骤ID
	    int      stepId;
		//箭头类型 0(上)、1(下)、2(左)、3(右)
		BYTE     type;
		//指引提示
		AWString strDesc;
		//界面名称
		AWString strDlg;
		//控件名称
		AWString strControl;
	}SGuideStep;
	//指引
    typedef struct _SGuideContent 
    {
		_SGuideContent():guideid(0),level_min(1),level_max(1024),isrepeate(false){}
		~_SGuideContent(){guideid = 0;ids.clear();}
		//指引ID
		int guideid;
		//等级下限
		int level_min;
		//等级上限
		int level_max;
		//失败可重复触发
		bool isrepeate;
		//指引步骤列表
		abase::vector<int> ids;
    }SGuideContent;

	//指引步骤表
    typedef abase::hash_map<int,SGuideStep*> GuideStepMap;
	//指引数据表
	typedef abase::hash_map<int,SGuideContent*> GuideContentMap;
     //目的引导数据表
	typedef abase::vector<SAimGuideInfo>  AimGuideInfoVec;

	

	CECNewHelpGuide();
	virtual ~CECNewHelpGuide();
	bool Init();
	void Release();
	void Reset();
	void Tick(unsigned long dwDeltaTime);
	
	//开始新的指引
	bool StartGuide(int iGuide);
	//停止指引的当前步骤，进行下一个步骤
	bool NextGuide(int iGuide,int iStep);
	//完成指引
	bool FinishGuide(int iGuide);
	//停止指引的当前步骤，但不立刻进行下一个步骤
	bool PauseGuide(int iGuide,int iStep);
	//强制停止当前的指引
	void StopGuide();
    //开启/关闭指引
	//void Enable(bool isEnable);
	bool IsEnable();


private:
	CLuaState * m_pState;
	
	GuideContentMap  m_GuideContent;
	
	GuideStepMap  m_GuideStep;

	AimGuideInfoVec  m_AimGuideInfo;

	int           m_iCurGuide;  //当前指引ID
	int           m_iCurStep;   //当前指引步骤（第X步（1,2,3...），不是步骤ID）
	int           m_iCurType;   //界面类型(0,1,2,3)

protected:
	bool LoadGuideStep();
	bool LoadGuideContent();
	bool LoadAimGuideInfo();
	bool LoadAimGuideInfo2();
	bool LoadGuideData();
	void ReleaseGuideData();

	void SetGuideState(int iGuide);
	void ReSetGuideState(int iGuide);

	bool StartStep(int iStep);
	void StopStep();

public:
	bool CheckGuideState(int iGuide);

public:
	SGuideContent* FindGuideContentById(int iGuide);
	SGuideStep* FindGuideStepById(int iStep);

    void SortAimGuideInfo(SortType st,bool bInc = true);
	AimGuideInfoVec& GetAimGuideInfoVec(){return m_AimGuideInfo;}
	SAimGuideInfo* GetAimGuideInfo(int index);
	//获取特定类型的目的指引的索引集合
	void GetAimGuideIndexs(int type,abase::vector<int>& vecIndex);

protected:
	//轮询任务触发的指引
	bool TickHelpGuideForGettingTask();
	//轮询获取物品触发的指引
	bool TickHelpGuideForGettingItem();
    //轮询升级触发的指引
	bool TickHelpGuideForHostLevelUp();
	//轮询之前，检查特定指引的完成状态
	bool CheckGetTaskGuideState();
	bool CheckGetGetItemGuideState();
	bool CheckHostLevelUpGuideState();

};

//指引ID与配置脚本中的ID一一对应
enum
{
	//GUIDE_HELP_ENABLE = 0,                  //开启/关闭指引位
	//GUIDE_HELP_WELCOM_1,					//欢迎词       ---使用单独界面来表现
	GUIDE_HELP_MOVE_1 = 0,						//移动教学     ---使用单独界面来表现
    GUIDE_HELP_PATHFIND_1,					//寻径
	GUIDE_HELP_NPCCHAT_2,					//NPC对话
	GUIDE_HELP_FIGHT_2,						//战斗         
	GUIDE_HELP_EQUIPWEAPON_3,				//装备武器
	GUIDE_HELP_SKILLADDPOINT_3,				//技能加点(人族)
	GUIDE_HELP_USESKILL_1,					//使用技能
	GUIDE_HELP_USEGIFT_2,					//使用礼包
	GUIDE_HELP_USEMEDICINE_1,				//使用药品
	GUIDE_HELP_EQUIPTALISMAN_4,				//装备法宝
	GUIDE_HELP_USEGAMEAUTOMATIC_2,			//使用天人合一
    GUIDE_HELP_REFINING_5,					//炼器教学
	GUIDE_HELP_PET_11,						//宠物

    GUIDE_HELP_SKILLADDPOINT_JIULI_3,		//技能加点(九黎)
	GUIDE_HELP_SKILLADDPOINT_LIESHAN_3,		//技能加点(烈山)
	GUIDE_HELP_SKILLADDPOINT_HUAIGUANG_3,	//技能加点(怀光)
	GUIDE_HELP_SKILLADDPOINT_TIANHUA_3,		//技能加点(天华)
	GUIDE_HELP_SKILLADDPOINT_CHENHUANG_3,	//技能加点(辰皇)
	GUIDE_HELP_SKILLADDPOINT_TAIHAO_3,		//技能加点(太昊)
	GUIDE_HELP_SKILLADDPOINT_SIZU_3,		//技能加点(牵机)
	GUIDE_HELP_SKILLADDPOINT_LUOLI_3,		//技能加点(英招)

    GUIDE_HELP_NUM ,                 //
};


//指引步骤ID与配置脚本中的ID一一对应

//需要特殊任务ID的步骤ID
enum
{
	ESTEP_TASK_PATHFIND = 3,  //寻径指引
	ESTEP_TASK_FIGHTWAR = 5,  //战斗指引
	ESTEP_TASK_REFINING = 21,//炼器教学
	ESTEP_TASK_PET      = 26, //宠物
};
//需要特殊物品ID的步骤ID
enum
{
	ESTEP_ITEM_EQUIPWEAPON		 = 7,        //装备武器（武器）
	ESTEP_ITEM_USEGIFT_2_2		 = 14,       //使用礼包（礼包）
	ESTEP_ITEM_EQUIPTALISMAN_4_2 = 17,       //装备法宝（法宝）
	ESTEP_ITEM_EQUIPTALISMAN_4_3 = 18,       //装备法宝（寒月之辉）
	ESTEP_ITEM_REFINING_5_3      = 23,       //炼器教学（装备）
    ESTEP_ITEM_REFINING_5_4      = 24,       //炼器教学（炼器符）
	ESTEP_ITEM_PET_11_3          = 28,       //宠物（宠物）
	ESTEP_ITEM_PET_11_10          = 35,       //宠物（宠物）
};
//需要特殊NPC ID的步骤ID
enum
{
	ESTEP_NPC_NPCTALK = 4,       //NPC对话
	ESTEP_NPC_NPCREFINING = 22,      //炼器教学
};
//需要进行特殊条件判断的步骤ID
enum
{
    ESTEP_SPECIAL_USESKILL = 12,     //使用技能
	ESTEP_SPECIAL_USEMEDICINE = 15,  //使用药品
	ESTEP_SPECIAL_FIGHT = 46,        //战斗指引
};
namespace NewHelpGuide
{
  //单独界面
  void StartGuide_Welcome();
  void FinishGuide_Welcome();
  //单独界面
  void StartGuide_Move();
  void FinishGuide_Move();


  //技能加点
  bool StartGuide_SkillAddPoint_1();
  bool StartGuide_SkillAddPoint_2();
  bool StartGuide_SkillAddPoint_3();
  bool FinishGuide_SkillAddPoint();

  bool CheckTaskTraceId(int taskid,int stepid);
  bool CheckInventoryItemId(int itemid,int stepid);
  bool CheckNpcTalkId(int npcid,int stepid);

  bool StartGuide_Step_1(int iGuide);
  bool StartGuide_Step_2(int iGuide);
  bool StartGuide_Step_3(int iGuide);
  bool StartGuide_Step_4(int iGuide);
  bool StartGuide_Step_5(int iGuide);
  bool StartGuide_Step_6(int iGuide);
  bool StartGuide_Step_7(int iGuide);
  bool StartGuide_Step_8(int iGuide);
  bool StartGuide_Step_9(int iGuide);
  bool StartGuide_Step_10(int iGuide);
  bool StartGuide_Step_11(int iGuide);
  bool FinishGuide(int iGuide);

  //
  bool PauseGuide_Step_1(int iGuide);
  bool PauseGuide_Step_2(int iGuide);
  bool PauseGuide_Step_3(int iGuide);
  bool PauseGuide_Step_4(int iGuide);
  bool PauseGuide_Step_5(int iGuide);
  bool PauseGuide_Step_6(int iGuide);
  bool PauseGuide_Step_7(int iGuide);
  bool PauseGuide_Step_8(int iGuide);
  bool PauseGuide_Step_9(int iGuide);
  bool PauseGuide_Step_10(int iGuide);
  bool PauseGuide_Step_11(int iGuide);

  void StopGuide();

  //双击或右键点击包裹界面物品图标
  bool TriggerClickInventoryItem(int itemid);
  //单击任务追踪面板上的NPC名字
  bool TriggerClickTaskTraceNpcName(int taskid);
  //NPC对话界面打开
  bool TriggerOpeningNpcTalk(int npcid);
  //点击NPC列表项
  bool TriggerClickNpcListItem(int index);

 //接到新任务
 bool TriggerGetTask();
 //得到新物品
 bool TriggerGetItem();
 //升级
 bool TriggerHostLevelUp();

}
//获取特殊界面控件位置的辅助函数

//获取“任务追踪界面”指定任务ID的树控件结构
_AUITREEVIEW_ITEM* GetTaskTraceItemByTaskId(int taskid);
//将“任务追踪界面”中指定任务ID的树控件展开
void  ExpandTaskTraceItemByTaskId(int taskId);
//获取“包裹界面”中指定物品ID的图标编号（00-47(CECDLGSHOP_PACKMAX-1)）
int GetInventoryItemByItemId(int itemid);
//强制显示“包裹界面”指定物品ID的图标
void ExpandInventoryItemByItemId(int itemid);
//获取“NPC对话界面”中列表框指定条目的位置信息
bool GetNpcTalkListItemRect(int nline,tagPOINT& point,tagSIZE& size1,bool isMain = false);
//获取特定步骤中的任务ID
int GetTaskIdForStep(int stepid);
//获取特定步骤中的物品ID
int GetItemIdForStep(int stepid);
//获取特定步骤中的NPCID
int GetNpcIdForStep(int stepid);

