/*
 * FILE: EC_BossKey.h
 *
 * DESCRIPTION: boss key. All functions must be called from the thread that creat the window.
 *
 * CREATED BY: liudong, 2009/2/11
 *
 * HISTORY: 
 *
 */
#pragma once

#include <vector.h>
#include <windows.h>

class CECBossKey
{
protected:
	CECBossKey();
	struct sHotKey 
	{
		HWND hWnd;
		unsigned int key;
		unsigned int fsModifiers;
		int id;
	};
public:
    ~CECBossKey();
	
	enum ErrorType 
	{
		ERR_SUCCESS= 0,
		ERR_REG_BY_OTHER,//热键已注册。（有可能被别的应用程序）.
		ERR_THREAD_ILLEGAL,//当前线程不是创建制定窗口的线程。 
		ERR_NUM,
	};

	static CECBossKey* GetInstance();
	ErrorType GetLastError(){ return m_lastError; }

    bool AddBossKey(const HWND hWnd, const unsigned int key,  const unsigned int fsModifiers);
	void RemoveAllBossKeys();

	bool DealWindowsMessage(const HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam);

	void AddTaskbarIcon();
	
protected:
	bool _CheckID(const int id);//return true if "id" is in array "m_aHotKey".

protected:
	static CECBossKey s_instance;
	abase::vector<sHotKey> m_aHotKey;
	ErrorType m_lastError;

	HWND	m_Hwnd;
};