// Treasure.h: interface for the CTreasure class.
//
//////////////////////////////////////////////////////////////////////


#ifndef __TREASURE_H__
#define __TREASURE_H__

#include "vector.h"

#define BIT_PRESENT			(0x1 << 0) //新品
#define BIT_RECOMMEND		(0x1 << 1) //推荐品
#define BIT_SALES_PROMOTION (0x1 << 2) //促销品

extern const char *szMainType[];

struct  TREASURE_ITEM_VERSION1
{
	unsigned int id;	//物品ID
	char         szName[128]; //物品名字
	unsigned int count; //物品个数
	
	struct  
	{
		unsigned int price; //价格
		unsigned int time; //租借时间（秒，0表示无期）
		
	}buy_fashion[4];//购买方式
	
	DWORD        others;//标志
};

struct  TREASURE_ITEM_VERSION3
{
	unsigned int id;	//物品ID
	char         szName[128]; //物品名字
	unsigned int count; //物品个数
	
	struct  
	{
		unsigned int price; //价格
		unsigned int time; //租借时间（秒，0表示无期）
		
	}buy_fashion[1];//购买方式
	
	DWORD        others;//标志
	
	unsigned int tid;//
	unsigned int main_type;//0 - 6
	unsigned int sub_type;// 0 - 8
};

struct  TREASURE_ITEM_VERSION6
{
	unsigned int id;	//物品ID
	char         szName[128]; //物品名字
	unsigned int count; //物品个数
	
	struct  
	{
		unsigned int price; //价格
		unsigned int time; //租借时间（秒，0表示无期）
		
	}buy_fashion[1];//购买方式
	
	DWORD        others;//标志
	
	unsigned int tid;//
	unsigned int main_type;//0 - 6
	unsigned int sub_type;// 0 - 8

	char		 file_icon[128];//图标文件路径名
	WORD         desc[512];//文字描述原长64字，2006.8.7 改为512字
};

struct  TREASURE_ITEM_VERSION7
{
	unsigned int id;	//物品ID
	char         szName[128]; //物品名字
	unsigned int count; //物品个数
	
	struct  
	{
		unsigned int price; //价格
		unsigned int time; //租借时间（秒，0表示无期）
		
	}buy_fashion[1];//购买方式

	DWORD        others;//标志
	
	unsigned int tid;//
	unsigned int main_type;//0 - 6
	unsigned int sub_type;// 0 - 8

	char		 file_icon[128];//图标文件路径名
	WORD         desc[512];//文字描述原长64字，2006.8.7 改为512字

	int			 bonus; //返还红利的比例
};

struct  TREASURE_ITEM
{
	unsigned int id;	//物品ID
	char         szName[128]; //物品名字
	unsigned int count; //物品个数
	
	struct  
	{
		unsigned int price; //价格
		unsigned int time; //租借时间（秒，0表示无期）
		
	}buy_fashion[1];//购买方式

	DWORD        others;//标志
	
	unsigned int tid;//
	unsigned int main_type;//0 - 6
	unsigned int sub_type;// 0 - 8

	char		 file_icon[128];//图标文件路径名
	WORD         desc[512];//文字描述原长64字，2006.8.7 改为512字

	int			 bonus; //返还红利的比例
	int			 discount; //打折比例
};

struct	  TREASURE_ITEM_VERSION9
{
	TREASURE_ITEM_VERSION9()
	{
		bHasPresent = false;
		memset(szPresentName,0,128);
		Presentid = 0;
		Presentcount = 0;
		Presenttime = 0;
		memset(Present_file_icon,0,128);
		bPresentBind = false;
		memset(Presentdesc,0,sizeof(WORD)*512);
	}

	unsigned int id;						//物品ID
	char         szName[128];				//物品名字
	unsigned int count;						//物品个数
	
	struct  
	{
		unsigned int price;					//价格
		unsigned int time;					//租借时间（秒，0表示无期）
		
	}buy_fashion[1];						//购买方式
	
	DWORD        others;					//标志
	
	unsigned int tid;						//
	unsigned int main_type;					//0 - 6
	unsigned int sub_type;					//0 - 8
	
	char		 file_icon[128];			//图标文件路径名
	WORD         desc[512];					//文字描述原长64字，2006.8.7 改为512字
	
	int			 bonus;						//返还红利的比例
	int			 discount;					//打折比例
	
	//新加赠品
	bool		 bHasPresent;				//是否有赠品	
	char		 szPresentName[128];		//赠品名字
	unsigned int Presentid;					//赠品ID
	unsigned int Presentcount;				//赠品个数
	unsigned int Presenttime;				//赠品存在的时限(秒)
	char		 Present_file_icon[128];	//赠品图标路径
	bool		 bPresentBind;				//赠品是否绑定
	WORD         Presentdesc[512];			//赠品文字描述
};

//Added 2011-11-07.
struct	  TREASURE_ITEM_VERSION10
{
	TREASURE_ITEM_VERSION10()
	{
		bHasPresent = false;
		memset(szPresentName,0,128);
		Presentid = 0;
		Presentcount = 0;
		Presenttime = 0;
		memset(Present_file_icon,0,128);
		bPresentBind = false;
		memset(Presentdesc,0,sizeof(WORD)*512);
		memset(&valid_time, 0, sizeof(valid_time));
	}

	unsigned int id;						//物品ID
	char         szName[128];				//物品名字
	unsigned int count;						//物品个数
	
	struct  
	{
		unsigned int price;					//价格
		unsigned int time;					//租借时间（秒，0表示无期）
		
	}buy_fashion[1];						//购买方式
	
	DWORD        others;					//标志
	
	unsigned int tid;						//
	unsigned int main_type;					//0 - 6
	unsigned int sub_type;					//0 - 8
	
	char		 file_icon[128];			//图标文件路径名
	WORD         desc[512];					//文字描述原长64字，2006.8.7 改为512字
	
	int			 bonus;						//返还红利的比例
	int			 discount;					//打折比例
	
	//新加赠品
	bool		 bHasPresent;				//是否有赠品	
	char		 szPresentName[128];		//赠品名字
	unsigned int Presentid;					//赠品ID
	unsigned int Presentcount;				//赠品个数
	unsigned int Presenttime;				//赠品存在的时限(秒)
	char		 Present_file_icon[128];	//赠品图标路径
	bool		 bPresentBind;				//赠品是否绑定
	WORD         Presentdesc[512];			//赠品文字描述

	//商城物品的上架有效时间Added 2011-11-07.
	struct
	{
		int		type;						//有效时间的类别 0:永久有效，默认类别；1：按时间段；2：每周；3：每月
		int		start_time;					//开始时间：按时间段时：存储UTC的秒数，按每月和每周时：存储（小时+分+秒）的秒数
		int		end_time;					//终止时间：存储方案同上
		int		param;						//按时间段时：1：表示开启开始时间；2：表示开启终止时间
											//按每周时：（掩码）0-6位分别表示：周日 - 周六
											//按每月时：（掩码）1-31位分别表示：1-31日
	}valid_time;
	//Added end.
};
//Added end.

//Added 2013-04-17.
struct	  TREASURE_ITEM_VERSION11
{
	TREASURE_ITEM_VERSION11()
	{
		bHasPresent = false;
		memset(szPresentName,0,128);
		Presentid = 0;
		Presentcount = 0;
		Presenttime = 0;
		memset(Present_file_icon,0,128);
		bPresentBind = false;
		memset(Presentdesc,0,sizeof(WORD)*512);
		memset(&valid_time, 0, sizeof(valid_time));
		memset(&SearchKey, 0, sizeof(SearchKey));
	}
	
	unsigned int id;						//物品ID
	char         szName[128];				//物品名字
	unsigned int count;						//物品个数
	
	struct  
	{
		unsigned int price;					//价格
		unsigned int time;					//租借时间（秒，0表示无期）
		
	}buy_fashion[1];						//购买方式
	
	DWORD        others;					//标志
	
	unsigned int tid;						//
	unsigned int main_type;					//0 - 6
	unsigned int sub_type;					//0 - 8
	
	char		 file_icon[128];			//图标文件路径名
	WORD         desc[512];					//文字描述原长64字，2006.8.7 改为512字
	
	int			 bonus;						//返还红利的比例
	int			 discount;					//打折比例
	
	//新加赠品
	bool		 bHasPresent;				//是否有赠品	
	char		 szPresentName[128];		//赠品名字
	unsigned int Presentid;					//赠品ID
	unsigned int Presentcount;				//赠品个数
	unsigned int Presenttime;				//赠品存在的时限(秒)
	char		 Present_file_icon[128];	//赠品图标路径
	bool		 bPresentBind;				//赠品是否绑定
	WORD         Presentdesc[512];			//赠品文字描述
	
	struct
	{
		int		type;						//有效时间的类别 0:永久有效，默认类别；1：按时间段；2：每周；3：每月
		int		start_time;					//开始时间：按时间段时：存储UTC的秒数，按每月和每周时：存储（小时+分+秒）的秒数
		int		end_time;					//终止时间：存储方案同上
		int		param;						//按时间段时：1：表示开启开始时间；2：表示开启终止时间
		//按每周时：（掩码）0-6位分别表示：周日 - 周六
		//按每月时：（掩码）1-31位分别表示：1-31日
	}valid_time;

	// 关键字搜索商城物品Added 2013-04-17.
	WORD         SearchKey[64];			//关键字，","分割
	//Added end.
};
//Added end.


struct SUB_TYPE
{
	unsigned int id;
	char szName[64];
};


struct MAIN_TYPE
{
	char szName[64];
	unsigned int id;
	abase::vector<SUB_TYPE> listSubType;
};



class CTreasureMan
{
	enum
	{
		TREASURE_VERSION = 11,
	};
	
public:
	CTreasureMan()
	{
		m_nSubCurID = 0;
		m_nCurTID = 0;
		m_bChanged = false;
		m_nCurNpcMainID = 20;
		for( int i = 0; i < 8; ++i)
		{
			sprintf(listMainType[i].szName,szMainType[i]);
			listMainType[i].id = i;
		}
	}
	~CTreasureMan(){}

	bool Load(const char * szName);
	bool Save(const char * szName);
	bool Export(const char* szPath, bool bFremdness);
	void Delete( int idx );
	void DeleteByID( int id)
	{
		for( int i = 0; i < listTreasure.size(); ++i )
		{
			if(listTreasure[i]->tid == id) 
				Delete(i);				
		}
	}

	void DeleteSubType( int id);
	void Add(TREASURE_ITEM_VERSION11* pNewItem);
	void Release();
	int  GetTreasureCount(){ return listTreasure.size(); }
	TREASURE_ITEM_VERSION11 * GetTreasure( int idx){ return listTreasure[idx]; }
	TREASURE_ITEM_VERSION11 * GetTreasureByID( int id)
	{
		for( int i = 0; i < listTreasure.size(); ++i )
			if(listTreasure[i]->tid == id) return listTreasure[i];
		return NULL;
	}
	
	MAIN_TYPE * GetMainTypeList(){ return listMainType; }
	abase::vector<MAIN_TYPE*>* GetNpcMainTypeList(){ return &listNpcMainType;}
	MAIN_TYPE* AddNpcMainType(const char *szNpcMainType);
	void DeleteNpcMainType( int mid);
	
	abase::vector<TREASURE_ITEM_VERSION11*>* GetTreasureList(){ return &listTreasure; }
	int GenSubID(){ return ++m_nSubCurID; }
	int GenTID(){ return ++m_nCurTID; }
	int GenNpcMainID(){ return ++m_nCurNpcMainID; }

	bool IsChanged(){ return m_bChanged; }
	
private:
	int GetSubIndex( int mid, int id_sub );
	int GetMainIndex( int mid);
	
	abase::vector<TREASURE_ITEM_VERSION11*> listTreasure;
	MAIN_TYPE listMainType[8];
	
	abase::vector<MAIN_TYPE*> listNpcMainType;//new
	
	int   m_nSubCurID;
	int   m_nCurTID;
	int   m_nCurNpcMainID;//Lower = 20;
	bool  m_bChanged;
};

#endif
