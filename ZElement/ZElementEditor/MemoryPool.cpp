#include "Stdafx.h"
#include "MemoryPool.h"
#include <assert.h>

#define  MEMPOOL_BLOCK_SIZE  1024*1024*10		//10M 
#define	 MEMPOOL_FLAG		 "#$#$"

int	CMemoryPool::ms_nFlag		= (('#'<<24) | ('$'<<16) | ('#'<<8) | ('$'));
int	CMemoryPool::ms_nFixedSize	= 8;

template<> CMemoryPool*	Singleton<CMemoryPool>::ms_Singleton = 0;
CMemoryPool&	CMemoryPool::GetSingleton()
{
	assert(ms_Singleton);
	return (*ms_Singleton);
}

CMemoryPool*	CMemoryPool::GetSingletonPtr()
{
	return ms_Singleton;
}

CMemoryPool::CMemoryPool()
	: m_bInit(FALSE)
	, m_hHeap(NULL)
	, m_pMPC_Big1(NULL)
	, m_pMPC_Big2(NULL)
	, m_pMPC_Normal(NULL)
{

}

CMemoryPool::~CMemoryPool()
{
	
}

BOOL   	CMemoryPool::Initialize()
{
	if(m_bInit)
		return FALSE;

	Destroy();   

	m_hHeap = GetProcessHeap();    

	m_pMPC_Big1   = (LPMEMORY_BLOCK)HeapAlloc(m_hHeap,0,sizeof(MEMORY_BLOCK)); 
	if(!m_pMPC_Big1)
		return FALSE;

	m_pMPC_Big2   = (LPMEMORY_BLOCK)HeapAlloc(m_hHeap,0,sizeof(MEMORY_BLOCK)); 
	if(!m_pMPC_Big2)
		return FALSE;

	m_pMPC_Normal = (LPMEMORY_BLOCK)HeapAlloc(m_hHeap,0,sizeof(MEMORY_BLOCK));
	if(!m_pMPC_Normal)
		return FALSE;
	
	BOOL bRet = _reserveMemory();   
	if(!bRet)
	{
		Destroy();
	}

	return bRet; 
}

void    CMemoryPool::Destroy()
{
	//MPC_BIG1
	if(m_pMPC_Big1)
	{
		VirtualFree(m_pMPC_Big1->lpMemory,MEMPOOL_BLOCK_SIZE*4,MEM_DECOMMIT| MEM_RELEASE);
		HeapFree(m_hHeap,0,m_pMPC_Big1);
	}
	m_pMPC_Big1 = NULL;

	//MPC_BIG2
	if(m_pMPC_Big2)
	{
		VirtualFree(m_pMPC_Big2->lpMemory,MEMPOOL_BLOCK_SIZE*2,MEM_DECOMMIT| MEM_RELEASE);
		HeapFree(m_hHeap,0,m_pMPC_Big2);
	}
	m_pMPC_Big2 = NULL;

	//MPC_NORMAL
	if(m_pMPC_Normal)   
	{   
		LPMEMORY_BLOCK  pNextBlock = NULL;   

		LPMEMORY_BLOCK  pBlockToFree = m_pMPC_Normal;    
		while (pBlockToFree)   
		{   
			pNextBlock = pBlockToFree->lpNext;
			HeapFree(m_hHeap,0,pBlockToFree->lpMemory);
			HeapFree(m_hHeap,0,pBlockToFree);   
			pBlockToFree = pNextBlock;   
		}   
		m_pMPC_Normal = NULL;   
	}   

	m_bInit = FALSE;
}

void*	CMemoryPool::Alloc(DWORD dwSize)
{
	if(dwSize > DWORD(MEMPOOL_BLOCK_SIZE*4-ms_nFixedSize))
	{
		assert("This MemoryPool can not alloc memory > 40M");
		return NULL;
	}
	else if(dwSize > DWORD(MEMPOOL_BLOCK_SIZE*2-ms_nFixedSize))
	{
		if(m_pMPC_Big1->dwIndex != 0 || m_pMPC_Big1->bIsUsed)
		{
			assert("MPC_BIG1 40M memory cannot be used before dealloc");
			return NULL;
		}

		if(!m_pMPC_Big1->bIsInit)
		{
			m_pMPC_Big1->lpMemory = (LPBYTE)VirtualAlloc(m_pMPC_Big1->lpMemory,dwSize,MEM_COMMIT,PAGE_READWRITE);
			if(!m_pMPC_Big1->lpMemory)
				return NULL;
			m_pMPC_Big1->bIsInit  = TRUE;
		}

		m_pMPC_Big1->dwIndex  = dwSize;
		m_pMPC_Big1->bIsUsed  = TRUE;
		int* pFlag = (int*)m_pMPC_Big1->lpMemory;
		pFlag[0]   = ms_nFlag;
		pFlag[1]   = int(dwSize);
		return m_pMPC_Big1->lpMemory+ms_nFixedSize;
	}
	else if(dwSize > (DWORD)MEMPOOL_BLOCK_SIZE && dwSize <= DWORD(MEMPOOL_BLOCK_SIZE*2-ms_nFixedSize))
	{
		if(m_pMPC_Big2->dwIndex != 0 || m_pMPC_Big2->bIsUsed)
		{
			assert("MPC_BIG2 20M memory cannot be used before dealloc");
			return NULL;
		}
		
		if(!m_pMPC_Big2->bIsInit)
		{
			m_pMPC_Big2->lpMemory = (LPBYTE)VirtualAlloc(m_pMPC_Big2->lpMemory,dwSize,MEM_COMMIT,PAGE_READWRITE);
			if(!m_pMPC_Big2->lpMemory)
				return NULL;
			m_pMPC_Big2->bIsInit  = TRUE;
		}
		
		m_pMPC_Big2->dwIndex  = dwSize;
		m_pMPC_Big2->bIsUsed  = TRUE;
		int* pFlag = (int*)m_pMPC_Big2->lpMemory;
		pFlag[0]   = ms_nFlag;
		pFlag[1]   = int(dwSize);
		return m_pMPC_Big2->lpMemory+ms_nFixedSize;
	} 

	return _getMemory(dwSize);
}   

void	CMemoryPool::DeAlloc(void* p)
{
	int* pFlag = (int*)((LPBYTE)p-ms_nFixedSize);
	if(pFlag[0] != ms_nFlag)
	{
		assert("This pointer is not alloc by MemoryPool");
		return;
	}

	int nSize = pFlag[1];
	if(nSize > MEMPOOL_BLOCK_SIZE*2-ms_nFixedSize)
	{
		assert(m_pMPC_Big1->bIsInit && m_pMPC_Big1->dwIndex != 0 && m_pMPC_Big1->bIsUsed);
		m_pMPC_Big1->dwIndex = 0;
		m_pMPC_Big1->bIsUsed = FALSE;

		return;
	}
	else if(nSize > MEMPOOL_BLOCK_SIZE && nSize <= MEMPOOL_BLOCK_SIZE*2-ms_nFixedSize)
	{
		assert(m_pMPC_Big2->bIsInit && m_pMPC_Big2->dwIndex != 0 && m_pMPC_Big2->bIsUsed);
		m_pMPC_Big2->dwIndex = 0;
		m_pMPC_Big2->bIsUsed = FALSE;

		return;
	}

	_delMemory(p);
}

void*	CMemoryPool::_getMemory(DWORD dwSize)
{
	LPMEMORY_BLOCK  pCurrentBlock	  = m_pMPC_Normal;   
	DWORD           dwSizeNeeded	  = dwSize + ms_nFixedSize;
	assert(dwSizeNeeded <= MEMPOOL_BLOCK_SIZE);

	LPMEMORY_BLOCK pLastBlock = pCurrentBlock;
	while (pCurrentBlock)
	{
		if(!pCurrentBlock->bIsUsed)
		{
			LPBYTE pStart = pCurrentBlock->lpMemory;
			int* pFlag = (int*)pStart;
			pFlag[0]   = ms_nFlag;
			pFlag[1]   = dwSize;
			pCurrentBlock->dwIndex = dwSizeNeeded;
			pCurrentBlock->bIsUsed = TRUE;

			return (void*)(pStart+ms_nFixedSize);
		}
		
		pLastBlock    = pCurrentBlock;
		pCurrentBlock = pCurrentBlock->lpNext;
	}

	LPMEMORY_BLOCK pNext = (LPMEMORY_BLOCK)HeapAlloc(m_hHeap,0,sizeof(MEMORY_BLOCK));
	if(!pNext)
		return NULL;
	pNext->dwIndex		 = dwSizeNeeded;
	pNext->dwSize		 = dwSizeNeeded;
	pNext->lpPrev		 = m_pMPC_Normal;
	pNext->lpNext		 = NULL;
	pNext->lpMemory		 = (LPBYTE)HeapAlloc(m_hHeap,0,dwSizeNeeded);
	if(!pNext->lpMemory)
	{
		HeapFree(pNext,0,pNext);
		return NULL;
	}
	memset(pNext->lpMemory,0,dwSizeNeeded);
	LPBYTE pStart = pNext->lpMemory;
	int* pFlag = (int*)pStart;
	pFlag[0]   = ms_nFlag;
	pFlag[1]   = dwSize;
	pNext->bIsInit	= TRUE;
	pNext->bIsUsed  = TRUE;
	pLastBlock->lpNext = pNext;
	pNext->lpPrev      = pLastBlock;

	return (void*)(pNext->lpMemory+ms_nFixedSize);
}

void	CMemoryPool::_delMemory(void* p)
{
	LPMEMORY_BLOCK  pCurrentBlock = m_pMPC_Normal;
	while (pCurrentBlock)
	{
		if(p > pCurrentBlock->lpMemory && p < pCurrentBlock->lpMemory + MEMPOOL_BLOCK_SIZE)
		{
			pCurrentBlock->dwIndex = 0;
			pCurrentBlock->bIsUsed = FALSE;
			break;
		}

		pCurrentBlock = pCurrentBlock->lpNext;
	}

	if(pCurrentBlock != m_pMPC_Normal &&
		pCurrentBlock != m_pMPC_Normal->lpNext)
	{
		LPMEMORY_BLOCK pPrev = pCurrentBlock->lpPrev;
		LPMEMORY_BLOCK pNext = pCurrentBlock->lpNext;
		pPrev->lpNext = pNext;
		if(pNext)
			pNext->lpPrev = pPrev;

		HeapFree(m_hHeap,0,pCurrentBlock->lpMemory);
		HeapFree(m_hHeap,0,pCurrentBlock);
	}
}

BOOL	CMemoryPool::_reserveMemory()
{
	//MPC_BIG1
	m_pMPC_Big1->dwIndex  = 0;
	m_pMPC_Big1->dwSize   = MEMPOOL_BLOCK_SIZE*4;
	m_pMPC_Big1->lpNext   = NULL;
	m_pMPC_Big1->lpPrev   = NULL;
	m_pMPC_Big1->lpMemory = (LPBYTE)VirtualAlloc(NULL,MEMPOOL_BLOCK_SIZE*4,MEM_RESERVE,PAGE_NOACCESS);
	m_pMPC_Big1->bIsInit  = FALSE;
	m_pMPC_Big1->bIsUsed  = FALSE;

	//MPC_BIG2
	m_pMPC_Big2->dwIndex  = 0;
	m_pMPC_Big2->dwSize   = MEMPOOL_BLOCK_SIZE*2;
	m_pMPC_Big2->lpNext   = NULL;
	m_pMPC_Big2->lpPrev   = NULL;
	m_pMPC_Big2->lpMemory = (LPBYTE)VirtualAlloc(NULL,MEMPOOL_BLOCK_SIZE*2,MEM_RESERVE,PAGE_NOACCESS);
	m_pMPC_Big2->bIsInit  = FALSE;
	m_pMPC_Big2->bIsUsed  = FALSE;

	//MPC_NORMAL
	m_pMPC_Normal->dwIndex  = 0;
	m_pMPC_Normal->dwSize   = MEMPOOL_BLOCK_SIZE;
	m_pMPC_Normal->lpPrev   = NULL;
	m_pMPC_Normal->lpMemory = (LPBYTE)HeapAlloc(m_hHeap,0,MEMPOOL_BLOCK_SIZE);
	memset(m_pMPC_Normal->lpMemory,0,MEMPOOL_BLOCK_SIZE);
	if(!m_pMPC_Normal->lpMemory)
		return FALSE;
	m_pMPC_Normal->bIsInit	= TRUE;
	m_pMPC_Normal->bIsUsed  = FALSE;

	LPMEMORY_BLOCK pNext = (LPMEMORY_BLOCK)HeapAlloc(m_hHeap,0,sizeof(MEMORY_BLOCK));
	if(!pNext)
		return FALSE;
	pNext->dwIndex		 = 0;
	pNext->dwSize		 = MEMPOOL_BLOCK_SIZE;
	pNext->lpPrev		 = m_pMPC_Normal;
	pNext->lpNext		 = NULL;
	pNext->lpMemory		 = (LPBYTE)HeapAlloc(m_hHeap,0,MEMPOOL_BLOCK_SIZE);
	if(!pNext->lpMemory)
		return FALSE;
	memset(pNext->lpMemory,0,MEMPOOL_BLOCK_SIZE);
	pNext->bIsInit	= TRUE;
	pNext->bIsUsed  = FALSE;

	m_pMPC_Normal->lpNext = pNext;
	m_bInit = TRUE;
	return TRUE;
}