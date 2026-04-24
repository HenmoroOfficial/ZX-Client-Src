/*
 * FILE: EC_BossKey.cpp
 *
 * DESCRIPTION: boss key. 
 *
 * CREATED BY: liudong, 2009/2/11
 *
 * HISTORY: 
 *
 */

#include "EC_BossKey.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"

#include <A3DDevice.h>

#include "resource.h"

#include <stdio.h>

#ifndef BACK_VERSION
#define _L(c)	L##c
#endif

#define WM_BOSS_KEY_SHOW_GAME        (WM_USER + 200)
#define WM_BOSS_KEY_ADD_ONE_KEY      (WM_USER + 201)
#define WM_BOSS_KEY_REMOVE_ALL_KEY   (WM_USER + 202)

static const int MIN_ID = 0x0000;
static const int MAX_ID = 0xBFFF;

CECBossKey CECBossKey::s_instance;

CECBossKey::CECBossKey()
{	
	m_lastError = ERR_SUCCESS;
}

CECBossKey::~CECBossKey()
{
	RemoveAllBossKeys();
	m_aHotKey.clear();
	m_lastError = ERR_SUCCESS;
}

bool CECBossKey::AddBossKey(const HWND hWnd, const unsigned int key,  const unsigned int fsModifiers)
{
	m_lastError = ERR_SUCCESS;

	if(GetWindowThreadProcessId(hWnd,NULL)!=GetCurrentThreadId())
	{
		PostMessage(hWnd,WM_BOSS_KEY_ADD_ONE_KEY,key,fsModifiers);
		m_lastError = ERR_THREAD_ILLEGAL;
		return false;
	}
	
	int i;
	for (i=0; i<(int)m_aHotKey.size(); ++i)
	{
		const sHotKey& hotKey = m_aHotKey[i];
		if (hWnd==hotKey.hWnd && key==hotKey.key && fsModifiers==hotKey.fsModifiers)
		{
			//already register it.
			return true;
		}
	}

	int id = MIN_ID;
	if (m_aHotKey.size()>0 && m_aHotKey.back().id<MAX_ID)
	{
		id = m_aHotKey.back().id+1;
	}
	const int MAX_TRY = 100;//max try times.
	for (i=0;i<MAX_TRY;i++)
	{
		if(_CheckID(id))
		{
		    id++;
			if (id>MAX_ID)
			{
				id = MIN_ID;
			}
		}else
		{
			break;
		}
	}
	if (i==MAX_TRY)
	{
		//no id to use.
		return false;
	}

	BOOL bResult = ::RegisterHotKey(hWnd,id,fsModifiers,key);
	if (bResult)
	{
		sHotKey hotKey;
		hotKey.hWnd        = hWnd;
		hotKey.key         = key;
		hotKey.fsModifiers = fsModifiers;
		hotKey.id          = id;
		m_aHotKey.push_back(hotKey);
	}else
	{
        unsigned int errorCode = ::GetLastError();
		switch(errorCode) {
		case 1409:
			m_lastError = ERR_REG_BY_OTHER;
			break;
		default:
			break;
		}
	}
	return (bResult!=0);
}

void CECBossKey::RemoveAllBossKeys()
{
	m_lastError = ERR_SUCCESS;

	for (int i=0; i<(int)m_aHotKey.size(); ++i)
	{
		sHotKey& hotKey = m_aHotKey[i];

		if(GetWindowThreadProcessId(hotKey.hWnd,NULL)!=GetCurrentThreadId())
		{
			PostMessage(hotKey.hWnd,WM_BOSS_KEY_REMOVE_ALL_KEY,0,0);
			m_lastError = ERR_THREAD_ILLEGAL;
			return ;
		}

	    BOOL bResult = ::UnregisterHotKey(hotKey.hWnd,hotKey.id);
		if(bResult)
	    {
			if (i!=(int)m_aHotKey.size())
			{
				m_aHotKey[i] = m_aHotKey.back();
			}
			m_aHotKey.pop_back();
			i--;
	    }else
		{
			//m_lastError = ...;
		}
	}
}

CECBossKey* CECBossKey::GetInstance()
{
	return &s_instance;
}

bool CECBossKey::_CheckID(const int id)
{
	for (int i=0; i<(int)m_aHotKey.size(); ++i)
	{
		const sHotKey& hotKey = m_aHotKey[i];
		if (hotKey.id == id)
		{
            return true;
		}
	}
	return false;
}

bool CECBossKey::DealWindowsMessage(const HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam)
{
	bool bDealMsg = true;

	switch(message) {
	case WM_HOTKEY:
		{
			if(_CheckID(wParam) && IsWindowVisible(hWnd))
			{
				m_Hwnd = hWnd;
				AddTaskbarIcon();
				if (!g_pGame->GetA3DDevice()->GetDevFormat().bWindowed)
				{
					ShowWindow(hWnd,SW_HIDE);
				}
				ShowWindow(hWnd,SW_HIDE);
//				g_pGame->GetGameRun()->SetTrayFlag(true);
			}
		}
		break;

	case WM_BOSS_KEY_SHOW_GAME:
		{
			if (IDI_MAINICON==wParam && WM_LBUTTONDOWN==lParam)
			{
				ShowWindow(hWnd,SW_SHOW);
				if (IsIconic(hWnd))//IsMinimized()
				{
					ShowWindow(hWnd,SW_RESTORE);
				}
				NOTIFYICONDATA nid; 
				nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA); 
				nid.hWnd = hWnd;
				nid.uID=IDI_MAINICON; 
				nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP ; 
				nid.uCallbackMessage=WM_BOSS_KEY_SHOW_GAME;
				nid.hIcon=LoadIcon((HINSTANCE)GetWindowLong(hWnd,GWL_HINSTANCE),MAKEINTRESOURCE(IDI_MAINICON)); 
				wchar_t titleName[MAX_PATH];
				int num = GetWindowText(hWnd,titleName,MAX_PATH);
				if (num>0 && num<sizeof(nid.szTip)/sizeof(wchar_t))
				{
					titleName[num] = _L('\0');
					wcscpy(nid.szTip,titleName);
				}else
				{
					wcscpy(nid.szTip,_L("Game"));
				}
				Shell_NotifyIcon(NIM_DELETE,&nid);
//				g_pGame->GetGameRun()->SetTrayFlag(false);
			}
		}
		break;

	case WM_BOSS_KEY_ADD_ONE_KEY:
		AddBossKey(hWnd, wParam, lParam);
		break;

	case WM_BOSS_KEY_REMOVE_ALL_KEY:
		RemoveAllBossKeys();
		break;

	default:
		bDealMsg = false;
		break;
	}
	
	return bDealMsg;
}

void CECBossKey::AddTaskbarIcon()
{
	if (!IsWindow(m_Hwnd))
	{
		return;
	}
	NOTIFYICONDATA nid; 
	nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA); 
	nid.hWnd = m_Hwnd;
	nid.uID=IDI_MAINICON; 
	nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP ; 
	nid.uCallbackMessage=WM_BOSS_KEY_SHOW_GAME;
	nid.hIcon=LoadIcon((HINSTANCE)GetWindowLong(m_Hwnd,GWL_HINSTANCE),MAKEINTRESOURCE(IDI_MAINICON));
	wchar_t titleName[MAX_PATH];
	int num = GetWindowText(m_Hwnd,titleName,MAX_PATH);
	if (num>0 && num<sizeof(nid.szTip)/sizeof(wchar_t))
	{
		titleName[num] = _L('\0');
		wcscpy(nid.szTip,titleName);
	}else
	{
		wcscpy(nid.szTip,_L("Game"));
	}
	Shell_NotifyIcon(NIM_ADD,&nid);

}