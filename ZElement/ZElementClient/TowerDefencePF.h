/***************************************************
	file   : TowerDefencePF.h
	created: 2008/10/31
	author : liudong
	purpose: 游戏 "Tower Defence" 中寻径有关的算法. 
	         地图中位置的下标都是从0开始计算,遵循C的规则。

 ****************************************************/
#pragma once

#include <vector.h>
#include <APoint.h>
#include "hashmap.h"

class CTowerDefencePF 
{
public:
	//NPC 每条逃跑路线都要设置起始点和终止点，该结构体用来保存这些信息。
	struct EscapeInfo
	{
		abase::vector<APointI> entry;
		abase::vector<APointI> exit;
	};

	struct TDPosEx 
	{
		APointI pos;
		int     iEscape;
		TDPosEx(const int x, const int y, const int escape)
		{
			pos.x = x;
			pos.y = y;
			iEscape = escape;
		}
		TDPosEx()
		{
			pos.x   = 0;
			pos.y   = 0;
			iEscape = 0;
		}
	};

	//地图中每个点的类型。
	//目前地图仅仅支持两种类型：可行走区域和不可行走区域，如果将来加新类型，需要改动代码。
	enum PosType
	{
		PosRoad = 0,//路面
		PosBlock,   //障碍物
		PosNum,
	};

protected:
	CTowerDefencePF();

public:
	~CTowerDefencePF();
	
	//单例模式
	static CTowerDefencePF* GetInstance()
	{
		return &m_instance;
	}

	//remark       : 初始化.
	//nX,nY        : [in] 地图在水平和垂直方向上的点的数目。
	//vecEscapeInfo: [in] 所有逃跑路线。
	//return       : 成功返回true，失败返回false。
	bool Init(const int nX, const int nY, const abase::vector<EscapeInfo>& vecEscapeInfo);

	//remark       : 清空地图中所有的障碍物。
	//return       : 成功返回true，失败返回false。
	bool ClearMap();

	//remark       : 设置地图中某个点的类型（路面,障碍物 ... ...）。
	//pos          : [in] 点的坐标。
	//posType      : [in] 点的类型。(PosRoad, PosBlock ... ... )
	//pVecNPC      : [in] 当前所有NPC的位置，设置障碍物不应该把NPC的路堵住。如果是取消障碍物，传入空列表即可。
	//return       : 成功返回true，失败返回false。
	bool SetPosType(const APointI& pos, const CTowerDefencePF::PosType posType, const abase::vector<TDPosEx>* pVecNPC = NULL);

	//remark       : 一次性设置地图中多个点的类型（路面,障碍物 ... ...）,比如一个障碍物可能占4个点的位置。
    //vecPos       : [in] 点列表。
	//posType      : [in] 点的类型。(PosRoad, PosBlock ... ... )
	//pVecNPC      : [in] 当前所有NPC的位置，设置障碍物不应该把NPC的路堵住。如果是取消障碍物，传入空列表即可。
	//return       : 成功返回true，失败返回false。
	bool SetPosType(const abase::vector<APointI>& vecPos, const CTowerDefencePF::PosType posType, const abase::vector<TDPosEx>* pVecNPC = NULL);

	//remark       : 获得一个点的类型 （路面,障碍物 ... ...）。
	//pos          : [in] 点的坐标。
	//return       : 点的类型(PosRoad, PosBlock ... ... )。
	CTowerDefencePF::PosType GetPosType(const APointI& pos);

	//remark       : 是否可以在指定位置设定一个障碍物，如果位置不合法或者设置障碍物后会阻塞某些NPC的路径，则不允许该点设置为障碍物。
	//pos          : [in] 点的坐标。
	//pVecNPC      : [in] 当前所有NPC的位置，设置障碍物不应该把NPC的路堵住。
	//return       : 允许设置该障碍物返回true，否则返回false。
	bool CanBlockPos(const APointI& pos, const abase::vector<TDPosEx>* pVecNPC = NULL);

	//remark       : 是否可以在指定的多个点设置障碍物，如果位置不合法或者设置障碍物后会阻塞某些NPC的路径，则不允许该点设置为障碍物。
	//vecPos       : [in] 点列表。
	//pVecNPC      : [in] 当前所有NPC的位置，设置障碍物不应该把NPC的路堵住。
	//return       : 允许设置该障碍物返回true，否则返回false。
	bool CanBlockPos(const abase::vector<APointI>& vecPos, const abase::vector<TDPosEx>* pVecNPC = NULL);

	//remark       : 根据NPC当前位置，查找行走路径中的下一个位置。如果返回true代表寻路成功；false代表找不到路径，说明已经到达最终目标点了。
	//nextPos      : [out]传出NPC行走路径的下一个位置,只有返回值是true的时候该值才有效。
	//pos          : [in] 传入NPC 当前位置。
	//iEscapeMap   : [in] 传入逃跑路线，就是该路径在vecEscapeInfo(由Init函数传入)中的位置。zero based.
	//return       : 成功返回true，失败返回false。
	bool GetNextEscapePos(APointI& nextPos, const APointI& pos, const int iEscapeMap);

#ifdef _TD_EDITOR
	//temporary. just for test.
	int  GetDist(const APointI& pos, const int iEscapeMap);
#endif

protected:
    void _Release();
	bool _CreateAllEscapeMap();//为所有的逃跑路径创建权值图。
	bool _CreateEscapeMap(const int iEscapeMap);//为某一条逃跑路径创建权值图.
	void _CheckHeuristicAroundPos(const APointI& pos, int* pEscapeMap, abase::vector<APointI>& vecOpen);//创建一张权值图时，根据当前已经确定权值的点设置周边点的权值。
	void _StoreCurMap();//缓存当前地图的信息，以备将来恢复时用。简单存储，仅能存储一个状态，不像堆栈。
	void _RestoreMap();//恢复到最近缓存的地图数据。
	bool _CheckNPCParams(const abase::vector<TDPosEx>* pVecNPC);//检查传入的NPC位置是否合法。
	bool _CheckNPCDist(const abase::vector<TDPosEx>* pVecNPC);//判断是否某个NPC会无路可走。
	
	inline bool _IsLineToNeighbor(const APointI& pos,const int dir);  //从当前点到指定方向是否有可以直接走过去的点。
	inline bool _IsPosInMap(const APointI& pos);                      //判断一个点是否在地图中。
	inline int  _GetPosIndex(const APointI& pos);                     //根据一个点的x，y坐标判断它在地图中的索引。
	inline void _GetPosFromIndex(APointI& pos, const int index);      //根据一个点在地图中的索引获取它的x，y坐标。
	inline APointI _GetNeighbor(const APointI& pos,const int dir);    //获取一个点在指定方向的邻居。
	inline int _GetDistOffset(const int dir);                         //获取一个点与指定方向的邻居间的偏移值。
	inline bool _IsPosEntryExit(const APointI& pos);//一个点是否出口、入口。

protected:
	static CTowerDefencePF    m_instance;
	int                       m_nX;            //地图宽度。
	int                       m_nY;            //地图高度。
	CTowerDefencePF::PosType* m_pMap;          //地图文件。
	abase::vector<int*>       m_vecEscapeMap;  //每一条套盘路径对应的权值地图。
	abase::vector<EscapeInfo> m_vecEscapeInfo; //所有逃跑路径。

	//一些缓存数据，为了提高程序执行的效率.
	int*                      m_pOpenMap;        
	bool*                     m_pCloseMap;       
	abase::vector<int*>       m_vecEscapeMapTemp;
	CTowerDefencePF::PosType* m_pMapTemp;
	bool*                     m_pMapEntryExit;//判断一个点是否是入口点、出口点（这些点不可以设置障碍物）。
};

//////////////////////////////////////////////////////////////////////////
//A*算法的实现，用于小游戏中的寻径
//2009.5.21
//by lihengju 

class CAStarPF
{
public: // types
	struct PathNode
	{
		APointI pos;
		PathNode* pParent;
		int evaluate_g;   // 从起点到该点的评估
		int evaluate_h;   // 从该点到终点的评估
		int	evaluate_f;   // f = g+h
		PathNode(int x, int y)
		{
			pos.x = x;
			pos.y = y;
			evaluate_g = evaluate_h = evaluate_f = 0;
			pParent = NULL;
		}
		PathNode(const APointI& p)
		{
			pos = p;
			evaluate_g = evaluate_h = evaluate_f = 0;
			pParent = NULL;
		}
	};
	typedef abase::hash_map<int, PathNode*> PathSet;

public: // constructor and destructor

public: // attributes

public: // operations
	static CAStarPF* GetInstance()
	{
		return &m_instance;
	}
	~CAStarPF();
	bool Init(const int row, const int col, const abase::vector<APointI>& vecBlocks);
	bool ClearMap();
	// 将某些点设置为阻塞状态
	bool SetBlockPos(const abase::vector<APointI>& vecBlocks);
	// 将某些点从阻塞状态设置为清除状态
	bool SetClearPos(const abase::vector<APointI>& vecClears);
	// 输入要寻径的起点和终点，进行寻径，结果放在vecPath中，无路径返回false
	bool FindPath(const APointI& start, const APointI& end, abase::vector<APointI>& vecPath);

protected: // attributes
	static CAStarPF m_instance;
	int			m_iRow;			// 多少行 高度
	int			m_iCol;			// 多少列 宽度
	APointI		m_start;
	APointI		m_end;
	int*		m_pMap;			// 对地图的存储 1：block；0：empty
	PathSet		m_openSet;		// 打开节点集合 
	PathSet		m_closeSet;		// 关闭节点集合

protected: // operations
	CAStarPF();
	bool Release();
	inline int GetPosIndex(const APointI& pos);
	inline int GetPosIndex(int x, int y);
	bool IsBlockPos(const APointI& pos);
	void ClearPathSet();
	int	GetEvaluateH(int x, int y);
	int GetEvaluateH(const APointI& pos);
	void Finish(abase::vector<APointI>& vecPath);
	PathNode* GetLeastNode();
	bool DealNextNode(PathNode* pNode);
	bool _DealNextNode(int x, int y, PathNode* pFrom);
	bool IsPosInMap(int x, int y);
private:


};