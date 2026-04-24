// SceneAIGenerator.h: interface for the CSceneAIGenerator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCENEAIGENERATOR_H__D446CAD6_1B77_4360_8FEF_560CEEDF2FF1__INCLUDED_)
#define AFX_SCENEAIGENERATOR_H__D446CAD6_1B77_4360_8FEF_560CEEDF2FF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SceneObject.h"
#include "AObject.h"
#include <A3DVertex.h>
#include "aabbcd.h"

#define EXP_AI_VERSION 0x0


class CPtrList;
class CECModel;

#define STEP_LEN 4.0f

struct AI_DATA
{
	DWORD dwAreaType;	//区域的类型(贴地，平面)
	float fEdgeLen;		//长
	float fEdgeWth;		//宽
	float fEdgeHei;		//盒子的高
	int   nMonsterType; //0,表示普通群怪; 1,表示群怪类型1; 2,表示群怪类型2
	bool  bPoint;       //06-13是否是怪物产生点(还没有写存盘和输出)
	
	int   nControllerId;
	int   nMounsterLife; // 生成对象的寿命（仅限于怪物）int　 单位秒,0表示无限
	int   nMounsterNum;  // 能够生成的最大数目
	int   iReviveType;//区域怪物是否自动重生
	
	//下面的数据已经废除，但是为了提示用户修改，暂时保留
	bool  bGenMounsterInit;//怪物是否起始时生成
	bool  bValidAtOneTime;//区域是否同时生效一次
	DWORD dwGenID;//区域制定编号
	//del 06.7.4
};

struct NPC_PROPERTY
{
	AString strNpcName;		//怪物名字
	DWORD dwID;				//怪物ID
	DWORD dwNum;			//怪物数
	DWORD dwRefurbish;		//刷新率
	DWORD dwDiedTimes;		//产生新属性需死的次数
	DWORD dwInitiative;		//主动被动攻击选择
	float fOffsetWater;		//相对于水的高度
	float fOffsetTerrain;	//相对与地的高度
	
	//new 
	DWORD dwMounsterFaction;//怪物自己的类别
	DWORD dwMounsterFactionHelper;//怪物寻求帮助的类别
	DWORD dwMounsterFactionAccept;//怪物响应帮助的类别
	DWORD dwDisapearTime;//怪物死亡后的消失时间（0-100S）
	bool  bNeedHelp;//是否要求救
	
	bool  bUseDefautFaction;//用怪物默认类别
	bool  bUseDefautFactionHelper;//用怪物默认的帮助者
	bool  bUseDefautFactionAccept;//怪物响应默认的求助者

	int   nPathID;
	int   nLoopType;       //循环方式
	AString strPathName;   //帮定路径名
	int     nSpeedFlags;     //0,慢速，1,快速
};

struct FIXED_NPC_DATA
{
	AString strNpcName;		//固定位置NPC的名字
	DWORD dwID;				//NPC的ID
	DWORD dwRefurbish;		//NPC的刷新频率
	float fRadius;			//随机半径
	DWORD dwNpcType;	    //类型(贴地，平面)
	char  cNpcInfo;			//0:NPC  1:怪物   2:可互动物品

	//绑定路径
	int		nPathID;         //路径ID
	int     nLoopType;       //循环方式
	AString strPathName;     //路径名字
	int     nSpeedFlags;     //0,慢速，1,快速
	DWORD   dwDisapearTime;  //怪物死亡后的消失时间（0-100S）

	int   nControllerId;
	int   nMounsterLife; // 生成对象的寿命（仅限于怪物）int　 单位秒,0表示无限
	int   nMounsterNum;  // 能够生成的最大数目

	int   iReviveType;	//区域怪物是否自动重生
	bool  bPatrol;//怪物自己巡逻
	short sPhase;		// 相位功能，13.7.8

	bool  bGenMounsterInit;//怪物是否起始时生成
	bool  bValidAtOneTime;//区域是否同时生效一次
	DWORD dwGenID;//区域制定编号
};

void		init_ai_data( AI_DATA &data);
void        Init_npc_property( NPC_PROPERTY * data);
void		init_fixed_npc_data(FIXED_NPC_DATA &data);
AString     GetModelPath(DWORD dwId);


struct AI_ID_FUNCS : public CUSTOM_FUNCS
{
	virtual BOOL CALLBACK OnActivate(void);
	virtual LPCTSTR CALLBACK OnGetShowString(void) const;
	virtual AVariant CALLBACK OnGetValue(void) const;
	virtual void CALLBACK OnSetValue(const AVariant& var);

	AVariant	m_var;
	CString		m_strText;
};

struct AI_EXTRA_ID_FUNCS : public CUSTOM_FUNCS
{
	virtual BOOL CALLBACK OnActivate(void);
	virtual LPCTSTR CALLBACK OnGetShowString(void) const;
	virtual AVariant CALLBACK OnGetValue(void) const;
	virtual void CALLBACK OnSetValue(const AVariant& var);

	AVariant	m_var;
	CString		m_strText;
};

struct NPC_EXTRA_ID_FUNCS : public CUSTOM_FUNCS
{
	virtual BOOL CALLBACK OnActivate(void);
	virtual LPCTSTR CALLBACK OnGetShowString(void) const;
	virtual AVariant CALLBACK OnGetValue(void) const;
	virtual void CALLBACK OnSetValue(const AVariant& var);

	AVariant	m_var;
	CString		m_strText;
};

struct FIXED_NPC_ID_FUNCS : public CUSTOM_FUNCS
{
	virtual BOOL CALLBACK OnActivate(void);
	virtual LPCTSTR CALLBACK OnGetShowString(void) const;
	virtual AVariant CALLBACK OnGetValue(void) const;
	virtual void CALLBACK OnSetValue(const AVariant& var);

	AVariant	m_var;
	CString		m_strText;
};

struct NPC_BEZIER_SEL_ID_FUNCS : public CUSTOM_FUNCS
{
	virtual BOOL CALLBACK OnActivate(void);
	virtual LPCTSTR CALLBACK OnGetShowString(void) const;
	virtual AVariant CALLBACK OnGetValue(void) const;
	virtual void CALLBACK OnSetValue(const AVariant& var);

	AVariant	m_var;
	CString		m_strText;
};

struct CONTROLLER_ID_FUNCS : public CUSTOM_FUNCS
{
	virtual BOOL CALLBACK OnActivate(void);
	virtual LPCTSTR CALLBACK OnGetShowString(void) const;
	virtual AVariant CALLBACK OnGetValue(void) const;
	virtual void CALLBACK OnSetValue(const AVariant& var);

	AVariant	m_var;
	CString		m_strText;
};

struct CONTROLLER_NPC_ID_FUNCS : public CUSTOM_FUNCS
{
	virtual BOOL CALLBACK OnActivate(void);
	virtual LPCTSTR CALLBACK OnGetShowString(void) const;
	virtual AVariant CALLBACK OnGetValue(void) const;
	virtual void CALLBACK OnSetValue(const AVariant& var);

	AVariant	m_var;
	CString		m_strText;
};

enum LOOP_TYPE
{
	LOOP_NO = 0,//不循环
	LOOP_RETURN,//原路返回
	LOOP_CIRCLE,//始终循环
};

class CSceneAIGenerator :public CSceneObject 
{
public:
	enum AREA_TYPE
	{
		TYPE_TERRAIN_FOLLOW = 0,//跟随地形起伏
		TYPE_PLANE,//在某一平面上
		TYPE_BOX,//盒子内部
	};

	enum 
	{
		INDIVIDUALITY_NORMAL = 0,
		INDIVIDUALITY_INITIATIVE,	
		INDIVIDUALITY_PASSIVITY,
	};


	CSceneAIGenerator();
	virtual ~CSceneAIGenerator();

	virtual void Translate(const A3DVECTOR3& vDelta);
	virtual void Render(A3DViewport* pA3DViewport);
	virtual void PostRender(A3DViewport* pA3DViewport);
	virtual void Tick(DWORD timeDelta);
	virtual void UpdateProperty(bool bGet);
	virtual void Release();
	virtual void EntryScene();
	virtual void LeaveScene();
	//Load data
	virtual bool Load(CELArchive& ar,DWORD dwVersion,int iLoadFlags);
	//Save data
	virtual bool Save(CELArchive& ar,DWORD dwVersion);
	
	//Exp data
	bool ExpData(AFile *pFile,DWORD dwVersion);
	bool ImpData(AFile *pFile,DWORD dwVersion);
	//  Copy object
	virtual CSceneObject*		CopyObject();
	AI_DATA						GetProperty(){ return m_Data; };
	void						SetProperty(const AI_DATA& data){ m_Data = data; };

	DWORD						GetNPCNum();
	AString                     GetNPCModelPath(int index);
	CECModel*                   GetNpcModel(int index);
	NPC_PROPERTY*				GetNPCProperty(int index); 
	NPC_PROPERTY*				GetNPCProperty(const AString& name); 
	void                        AddNPC(NPC_PROPERTY* npc);
	void                        DeleteNPC(int index);
	void                        ReleaseModels();
	void                        ReloadModels();
	int                         GetCopyFlags(){ return m_nCopy; }
	void                        SetCopyFlags( int nCopy){ m_nCopy = nCopy; }
	
	//附属区域相关操作
	bool                        IsExtraArea(){ return m_bExtraArea; }
	int                         GetExtraAreaNum(){ return m_listExtraArea.size(); }
	int                         GetExtraAreaID( int idx ){ return m_listExtraArea[idx]; } 
	void                        AddExtra( int id){ m_listExtraArea.push_back(id); }
	void                        DelExtra( int id);
    void                        SetExtraArea( bool bExtra ){ m_bExtraArea = bExtra; }

private:
	AString                     GetModelPath(DWORD dwId);
	void                        DrawLine(A3DVECTOR3 *pVertices,DWORD dwNum,DWORD clr);
	void						DrawRect();
	void                        DrawMounsterName();
	void						BuildProperty();

	void                        RenderModels(A3DViewport *pView);
	
	ASetTemplate<int>          area_type;
	ASetTemplate<int>          group_type;
	ASetTemplate<int>          loop_type;
	ASetTemplate<int>          revive_type;
	AI_DATA			m_Data;				//区域的数据
	CPtrList        m_listNPC;			//区域中的NPC列表
	CPtrList        m_listModels;        //这儿是模型的列表，用于预览模型
	AI_ID_FUNCS*    m_pIDSelFuncs;	
	CONTROLLER_ID_FUNCS *m_pControllerSelFuncs;
	AI_EXTRA_ID_FUNCS* m_pExtraIDSelFuncs;
	AString         m_strNPCList;
	float           m_fArea;//temp
	int             m_nCopy;//数据套数，决定是第几套数据(0,1)

	bool				m_bExtraArea;//是否是附加区域
	abase::vector<int>	m_listExtraArea;//自己的附加区域列表，如果本身是附加属性这个列表为空
};




class CSceneFixedNpcGenerator :public CSceneObject 
{
public:
	enum NPC_TYPE
	{
		TYPE_TERRAIN_FOLLOW = 0,//跟随地形起伏
		TYPE_PLANE,//在某一平面上
	};
	
	CSceneFixedNpcGenerator();
	virtual ~CSceneFixedNpcGenerator();

	virtual void	Render(A3DViewport* pA3DViewport);
	virtual void	PostRender(A3DViewport* pA3DViewport);
	virtual void	Tick(DWORD timeDelta);
	virtual void	UpdateProperty(bool bGet);
	virtual void    Release();
	virtual void	Rotate(const A3DVECTOR3& axis,float fRad);
	virtual void    Translate(const A3DVECTOR3& vDelta);
	virtual void SetDirAndUp(const A3DVECTOR3& vDir, const A3DVECTOR3& vUp);
		
	//Load data
	virtual bool Load(CELArchive& ar,DWORD dwVersion,int iLoadFlags);
	//Save data
	virtual bool Save(CELArchive& ar,DWORD dwVersion);

	//Exp data
	bool ExpData(AFile *pFile,DWORD dwVersion);
	bool ImpData(AFile *pFile,DWORD dwVersion);

	//  Copy object
	virtual CSceneObject*		CopyObject();

	void			BuildProperty();

	inline FIXED_NPC_DATA  GetProperty(){ return m_Data; };
	inline void            SetProperty( const FIXED_NPC_DATA & data){ m_Data = data; };
	inline CECModel*       GetECModel(){ return m_pModel; };
	inline AString         GetECModelPath(){ return GetModelPath(m_Data.dwID); };
	void                   ReloadModel();
	AString                GetNPCName(DWORD dwId);
	int                    GetCopyFlags(){ return m_nCopy; }
	void                   SetCopyFlags( int nCopy){ m_nCopy = nCopy; }
	bool					AABBCollideWithMe(BrushTraceInfo * pInfo);

	//附属区域相关操作
	bool                        IsExtraArea(){ return m_bExtraArea; }
	int                         GetExtraAreaNum(){ return m_listExtraArea.size(); }
	int                         GetExtraAreaID( int idx ){ return m_listExtraArea[idx]; } 
	void                        AddExtra( int id){ m_listExtraArea.push_back(id); }
	void                        DelExtra( int id);
	void                        SetExtraArea( bool bExtra ){ m_bExtraArea = bExtra; }
private:
	
	AString         GetModelPath(DWORD dwId);
	int             GetSightRange(DWORD dwId);
	
	FIXED_NPC_DATA m_Data;
	CECModel*      m_pModel; //Preview model
	FIXED_NPC_ID_FUNCS* m_pIDSelFuncs;
	CONTROLLER_NPC_ID_FUNCS *m_pControllerSelFuncs;
	NPC_BEZIER_SEL_ID_FUNCS *m_pBezierSelFuncs;
	NPC_EXTRA_ID_FUNCS* m_pExtraIDSelFuncs;

	ASetTemplate<int>   npc_type;
	ASetTemplate<int>   loop_type;
	ASetTemplate<int>   speed_type;
	ASetTemplate<int>   revive_type;
	AString m_strText;
	int m_nSight;
	int m_nCopy;//数据套数，决定是第几套数据(0,1)

	bool				m_bExtraArea;//是否是附加区域
	abase::vector<int>	m_listExtraArea;//自己的附加区域列表，如果本身是附加属性这个列表为空
};

#endif // !defined(AFX_SCENEAIGENERATOR_H__D446CAD6_1B77_4360_8FEF_560CEEDF2FF1__INCLUDED_)
