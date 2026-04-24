#pragma once

#include <deque>
#include <Windows.h>

class CAnmObject;
class CAnmNPC;
class CAnmPlayer;
class CAnmGFX;

class CAnmObjLoader
{
public:

	typedef void ( * BeginLoadFunc )( CAnmObject* pObj );

	typedef void ( * EndLoadFunc )( CAnmObject* pObj );

	typedef std::deque< CAnmObject* >	ObjectQueue;

public:

	CAnmObjLoader();

	virtual ~CAnmObjLoader();

	virtual bool Init();

	virtual void Release();

	virtual bool LoadObject( CAnmObject* pObj );

	virtual void UnloadObject( CAnmObject* pObj );

	virtual void SetCallback( BeginLoadFunc pBeginCB, EndLoadFunc pEndCB ) { m_pBeginCB = pBeginCB, m_pEndCB = m_pEndCB; }

	static unsigned int __stdcall ObjLoadLoop( void* pArg );

	void Lock() { EnterCriticalSection( & m_csThread ); }

	void Unlock() { LeaveCriticalSection( & m_csThread ); }

	void AddEvent() { ReleaseSemaphore( m_hEvent, 1, 0 ); }

	void WaitEvent() { WaitForSingleObject( m_hEvent, INFINITE ); }

	CAnmObject* SafePopObject();

	BeginLoadFunc GetBeginLoadFunc() const { return m_pBeginCB; }

	EndLoadFunc GetEndLoadFunc() const { return m_pEndCB; }

	int GetObjQueueSize() const { return m_qObject.size(); }

protected:

	bool Load( CAnmObject* pObj );
	void Unload( CAnmObject* pObj );

	bool LoadNPC( CAnmNPC* pNPC );
	void UnloadNPC( CAnmNPC* pNPC );

	bool LoadPlayer( CAnmPlayer* pPlayer );
	void UnloadPlayer( CAnmPlayer* pPlayer );

	bool LoadGFX( CAnmGFX* pGFX );
	void UnloadGFX( CAnmGFX* pGFX );

protected:

	static bool			m_bRun;
	HANDLE				m_hThread;
	DWORD				m_dwThread;
	HANDLE				m_hEvent;
	BeginLoadFunc		m_pBeginCB;
	EndLoadFunc			m_pEndCB;
	CRITICAL_SECTION	m_csThread;
	ObjectQueue			m_qObject;
};