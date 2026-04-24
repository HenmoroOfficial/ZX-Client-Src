// Filename	: DlgGardenMan.h
// Creator	: Fu Chonggang
// Date		: 2009/8/12

#pragma once

#include "DlgBase.h"

class AUILabel;
class AUIImagePicture;
class A2DSprite;

namespace HOMETOWN
{
	struct CROP_ESSENCE;
}

class CDlgGardenMan : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP();
	AUI_DECLARE_COMMAND_MAP();

	// From server
	enum HOMETOWN_FARM_STATE_CROP //注意状态是有优先级的，ID小的状态会先检查是否有变迁
    {
        //states for crop
        HTF_STATE_CROP_BEGIN        = 0,    
		HTF_STATE_DRYFREE           = HTF_STATE_CROP_BEGIN,
		HTF_STATE_DRY               = HTF_STATE_CROP_BEGIN+1,
		HTF_STATE_PESTFREE          = HTF_STATE_CROP_BEGIN+2,
		HTF_STATE_PEST              = HTF_STATE_CROP_BEGIN+3,
		HTF_STATE_WEEDFREE          = HTF_STATE_CROP_BEGIN+4,
		HTF_STATE_WEED              = HTF_STATE_CROP_BEGIN+5,
		HTF_STATE_WILT              = HTF_STATE_CROP_BEGIN+6,		//枯萎
		HTF_STATE_HARVEST			= HTF_STATE_CROP_BEGIN+7,
		HTF_STATE_SEED              = HTF_STATE_CROP_BEGIN+8,
		HTF_STATE_SPROUT            = HTF_STATE_CROP_BEGIN+9,		//幼苗
		HTF_STATE_IMMATURE          = HTF_STATE_CROP_BEGIN+10,
		HTF_STATE_PREMATURE         = HTF_STATE_CROP_BEGIN+11,
		HTF_STATE_MATURE            = HTF_STATE_CROP_BEGIN+12,
        HTF_STATE_ALLSTOLEN         = HTF_STATE_CROP_BEGIN+13,      //被偷光
        HTF_STATE_CROPPROTECTBEGIN  = HTF_STATE_CROP_BEGIN+14,      //防偷状态起始
        HTF_STATE_CROPPROTECTEND    = HTF_STATE_CROP_BEGIN+19,      //防偷状态结束
		HTF_STATE_SPEEDGROWBEGIN    = HTF_STATE_CROP_BEGIN+20,      //加速生长状态起始
        HTF_STATE_SPEEDGROWEND      = HTF_STATE_CROP_BEGIN+25,      //加速生长状态结束

		HTF_STATE_CROP_END,
	};
	enum HOMETOWN_FARM_STATE_FIELD
	{
		//states for field
		HTF_STATE_FIELD_BEGIN,
		
		HTF_STATE_INCULT            = HTF_STATE_FIELD_BEGIN,		//未开垦
		HTF_STATE_CULT              = HTF_STATE_FIELD_BEGIN+1,
		HTF_STATE_PLOW              = HTF_STATE_FIELD_BEGIN+2,
		HTF_STATE_PLANT             = HTF_STATE_FIELD_BEGIN+3,
		HTF_STATE_FIELD_END			= HTF_STATE_PLANT,
	};
	enum HOMETOWN_FARM_STATE_FARM
	{
		//states for farm
		HTF_STATE_FARM_BEGIN        = 0,
		HTF_STATE_FARMPROTECTBEGIN  = HTF_STATE_FARM_BEGIN,         //整个农场防偷状态起始
		HTF_STATE_FARMPROTECTEND    = HTF_STATE_FARM_BEGIN+5,
		HTF_STATE_FARMSPEEDGROWBEGIN= HTF_STATE_FARM_BEGIN+6,       //整个农场加速状态起始
		HTF_STATE_FARMSPEEDGROWEND  = HTF_STATE_FARM_BEGIN+11,
		HTF_STATE_FARM_END          = HTF_STATE_FARM_BEGIN+1023,

	};
	enum HOMETOWN_FARM_STATE_PLAYER
	{
		//states for farm player
		HTF_STATE_PLAYER_BEGIN,
		HTF_STATE_HIDE              = HTF_STATE_PLAYER_BEGIN,
		HTF_STATE_PLAYER_END,
    };

	// For local farm info
	enum FARM_TYPE
	{
		FARMTYPE_INCULT,
		FARMTYPE_OWN,
		FARMTYPE_LOVE,
		FARMTYPE_MAX,
	};
	// For local dynamic info
	enum
	{
		DYNAMICTYPE_GARDEN	= 0,
		DYNAMICTYPE_ACTION,
		DYNAMICTYPE_LOVEGARDEN,		// own garden only
		DYNAMICTYPE_FRUITS,			// own garden only
		DYNAMICTYPE_NUM,
	};

	// const variables
	enum
	{
		HOMETOWNFARMPLANTNUM = 18,
	};
	enum
	{
		HOMETOWNFARMPLANTOWNNUM = 12,
	};
	enum
	{
		HOMETOWNFARMPLANTLOVENUM = 6,
	};
	enum
	{
		GARDENSEEDITEM_NUM = 6,
	};
	enum
	{
		IMAGESTATE_CROP = 0,
		IMAGESTATE_PLOT,
		IMAGESTATE_WATER,
		IMAGESTATE_DROUGHT,
		IMAGESTATE_GRASS,
		IMAGESTATE_WORM,
		IMAGESTATE_LESSMATURE,
		IMAGESTATE_TRAP,
		IMAGESTATE_GEMMULE,
		IMAGESTATE_BIGGER,
		IMAGESTATE_MATURE,
		IMAGESTATE_DIE,
		IMAGESTATE_HARVEST,
		IMAGESTATE_NUM,
	};
	enum
	{
		OPERTYPE_NONE	= 0,
		OPERTYPE_PLOT,
		OPERTYPE_SEED,
		OPERTYPE_WATER,
		OPERTYPE_PEST,
		OPERTYPE_WEED,
		OPERTYPE_HARVEST,
		OPERTYPE_STEAL,
		OPERTYPE_NUM,
	};

	enum
	{
		BEE_NUM = 3,
	};

	struct dlg_field_info							//地块信息
	{
		int field_id;							// from server
		char field_type;						//0:未开垦, 1:普通地，2：爱心地
		int state;								//地块当前的状态					1<<HOMETOWN_FARM_STATE_FIELD
		int friend_id;							//爱心地播种者角色id,只有爱心地才会发这个和下个字段
		ACString strFriendName;
		int id;									//庄稼模板id
		int seed_time;							//播种时间
		int ripe_time;							//还有多少时间成熟,只有在未成熟之前才有效,但都会发
		int protect_period;
		int crop_state;							//状态		1<<HOMETOWN_FARM_STATE_CROP
		int fruit_left;							//当前剩余果实数 只在成熟状态下才有效,但都会发
		int fruit_total;						//被偷的果实数, 只在成熟或被偷光状态下才有效,但都会发
	};


public:
	CDlgGardenMan();
	virtual ~CDlgGardenMan();

	// garden main
	void OnCommandGardenOpenBarn(const char* szCommand);
	void OnCommandGardenOpenProp(const char* szCommand);
	void OnCommandGardenCheckStatus(const char* szCommand);
	void OnCommandGardenExchange(const char* szCommand);
	void OnCommandGardenCool(const char* szCommand);
	void OnCommandGardenLevelUp(const char* szCommand);
	void OnCommandGardenSetting(const char* szCommand);
	void OnCommandGardenFriendList(const char* szCommand);
	void OnCommandGardenFriendGarden(const char* szCommand);
	void OnCommandGardenMinimize(const char* szCommand);
	void OnCommandClose(const char* szCommand);
	void OnCommandCancel(const char* szCommand);

	void OnCommandGardenOperate(const char* szCommand);
	void OnCommandGardenSeedSelect(const char* szCommand);
	void OnCommandGardenOperType(const char* szCommand);
	// dynamic
	void OnCommandGardenStatusCheck(const char* szCommand);
	void OnCommandGardenCheckLove(const char* szCommand);
	void OnCommandGardenCheckMature(const char* szCommand);

	void OnEventLButtonDownPlant(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonUpPlant(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonDownItem(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonDownFriendGarden(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	void OnPrtc_LevelUp(void*);
	void OnPrtc_ViewFarm(void*);
	void OnPrtc_ViewFarmField(void*);
	void OnPrtc_SowField(void*);
	void OnPrtc_PestField(void*);
	void OnPrtc_WaterField(void*);
	void OnPrtc_PlowField(void*);
	void OnPrtc_WeedField(void*);
	void OnPrtc_HarvestField(void*);
	void OnPrtc_StealField(void*);
	void OnPrtc_ListLoveFields(void*);
	void OnPrtc_ListMatureFields(void*);
	void DealFarmInfo(void*);
	
	static int GetOwnerId() { return m_iOwnerId; }
	static int GetActionType() { return m_iActionType; }
	void ResetState();
	inline void ResetOneField(int index) { for(int i=0; i<IMAGESTATE_NUM; i++) m_pImgPlants[index][i]->Show(false); }
	void UpdatePlantState(int index);
	void UpdatePlantState(int index, int fState, int cState);
	int GetFieldId(UINT index);
	
	static void SetGardenMoney(INT64 money) { m_iOwnerMoney = money;}
	static void SetGardenCharm(INT64 charm) { m_iOwnerCharm = charm;}
	static INT64 GetGardenMoney() { return m_iOwnerMoney;}
	static INT64 GetGardenCharm() { return m_iOwnerCharm;}

protected:
	virtual void OnTick();
	virtual	bool OnInitDialog();
	virtual bool Release();
	virtual void OnShowDialog();
	void TickState();
	void UpdateHint();
	void UpdateSeed();
	ACString GetFormatTime(int nTime);
	int BeastState();

private:
	static abase::vector<int>				m_vecLoveGardenIds;				// For Dynamic
	static abase::vector<int>				m_vecMatureGardenIds;			// For Dynamic
	static abase::vector<dlg_field_info>	m_vecFarmInfo[FARMTYPE_MAX];
	static abase::hash_map<int, int>		m_mapId2Index;					// For QuickSearh
	static abase::vector<ACString>			m_vecDynamicString[DYNAMICTYPE_NUM];
	static abase::vector<const HOMETOWN::CROP_ESSENCE*>		m_vecCropEss;
	static abase::vector<INT64>		m_vecLevelUpCharm;
	static int m_iOwnerId;
	static INT64 m_iOwnerMoney;
	static INT64 m_iOwnerCharm;
	static int m_iLevel;
	static int m_iOperType;
	static int m_iActionType;
	static int m_iSeedType;
	static bool m_bOwn;
	static int m_iFarmState;
	static int m_iFarmProtectPeriod;
	AUILabel *m_pLabTitle;
	AUILabel *m_pLabCharm;
	AUILabel *m_pLabLevel;
	AUIImagePicture *m_pImgPlants[HOMETOWNFARMPLANTNUM][IMAGESTATE_NUM];
	AUIImagePicture *m_pImgGuardBeast;
	AUIImagePicture *m_pImgBee[BEE_NUM];
	AUIImagePicture *m_pImgButterfly[BEE_NUM];
	int m_iGuardFrameIndex;
	int m_iBeeFrameIndex[BEE_NUM];
	int m_iBfFrameIndex[BEE_NUM];
	int m_iBarLevel;

	int m_iFirstLine;

};
