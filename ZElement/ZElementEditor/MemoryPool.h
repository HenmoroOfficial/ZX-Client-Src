/*************************************************************************************************************
*  FILE: MemoryPool.h
*
*  DESCRIPTION: 简单内存池类,用于大内存临时分配空间,尽量使用完就释放,尽量在一个函数中Alloc,DeAlloc
*				MPC_BIG1	: 40M,用于临时分配大内存时使用
*				MPC_BIG2	: 20M,用于临时分配大内存时使用
*				MPC_NORMAL	: 10M,初始分配2块,可以动态分配
*								
*  CREATED BY: doujianwei, 2012/6/27
*************************************************************************************************************/
#ifndef _MEMORY_POOL_H_
#define _MEMORY_POOL_H_

#include "Singleton.h"

enum MEMORY_POOL_CHUNK
{
	MPC_BIG1	= 0,	//40M,初始化时仅保留虚拟地址,使用时才分配
	MPC_BIG2	= 1,	//20M,初始化时仅保留虚拟地址,使用时才分配
	MPC_NORMAL	= 2		//10M,初始化时直接分配2个Block,使用时如果不够可以动态申请
};

typedef struct tagMEMORY_BLOCK  MEMORY_BLOCK, *LPMEMORY_BLOCK;
struct tagMEMORY_BLOCK   
{   
	DWORD                 dwIndex;   	
	DWORD                 dwSize;	 	
	LPMEMORY_BLOCK        lpNext;	 	
	LPMEMORY_BLOCK        lpPrev;	 	
	LPBYTE                lpMemory;  	
	BOOL				  bIsInit;		
	BOOL				  bIsUsed;		
}; 

class CMemoryPool : public Singleton<CMemoryPool>
{
public:
	CMemoryPool();
	~CMemoryPool();

//数据
public:
protected:
	BOOL			m_bInit;		
	HANDLE			m_hHeap;		

	LPMEMORY_BLOCK	m_pMPC_Big1;	
	LPMEMORY_BLOCK	m_pMPC_Big2;	
	LPMEMORY_BLOCK	m_pMPC_Normal;  

	static int		ms_nFlag;
	static int		ms_nFixedSize;

//操作
public:
	static CMemoryPool&	GetSingleton();
	static CMemoryPool*	GetSingletonPtr();

public:
	BOOL   	Initialize();											
	void   	Destroy();													
	
	void*	Alloc(DWORD dwSize);									
	void	DeAlloc(void* p);										

private:
	void*	_getMemory(DWORD dwSize);
	void	_delMemory(void* p);

	BOOL	_reserveMemory();
};

#endif