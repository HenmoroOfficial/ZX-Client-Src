/*
 * FILE: EC_InputFilter.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/8/30
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_InputFilter.h"
#include "EC_InputCtrl.h"
#include "EC_Bosskey.h"

#define new A_DEBUG_NEW

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Reference to External variables and functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Local Types and Variables and Global variables
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECInputFilter
//	
///////////////////////////////////////////////////////////////////////////

DWORD CECInputFilter::m_dwPressMask[CECInputFilter::NUM_CKCOMBO] =
{
	EC_KSF_SHIFT | EC_KSF_CONTROL | EC_KSF_ALT,
	EC_KSF_CONTROL | EC_KSF_ALT,
	EC_KSF_SHIFT | EC_KSF_ALT,
	EC_KSF_SHIFT | EC_KSF_CONTROL,
	EC_KSF_SHIFT,
	EC_KSF_CONTROL,
	EC_KSF_ALT,
	0,
};

CECInputFilter::CECInputFilter(CECInputCtrl* pInputCtrl)
{
	m_pInputCtrl = pInputCtrl;
	memset(m_VK2LKMap, 0, sizeof (m_VK2LKMap));
}

//	Translate key input to logic-key
int CECInputFilter::ConvertVKToLK(int vk, DWORD dwCtrlKeys)
{
	if (!vk)
		return 0;

	int lk = FindLogicKey(vk, dwCtrlKeys);

	//	If no logic key is got with any control key pressed, try the logic key
	//	without control keys. For example: if no logic key match to shift+F1, then
	//	try to find the logic key which match to F1.
	if (!lk)
		lk = m_VK2LKMap[vk][CKCOMBO_NONE];

	return lk;
}

//	Check whether specified logic key is pressed
bool CECInputFilter::IsLogicKeyPressed(int iLogicKey)
{
	LK2VKMap::iterator it = m_LK2VKMap.find(iLogicKey);
	if (it != m_LK2VKMap.end())
	{
		//	Get current control key states
		DWORD dwCurCtrlKeys = m_pInputCtrl->GetCtrlKeyStates();
		dwCurCtrlKeys &= EC_KSF_CTRLKEYS;

		int vk;
		DWORD dwCtrlKeys;
		const VK_GROUP& vk_group = it->second;

		DecompressVK(vk_group.dwVKey1, vk, dwCtrlKeys);
		if (vk && (m_pInputCtrl->KeyIsBeingPressed(vk) && (dwCtrlKeys & dwCurCtrlKeys) == dwCtrlKeys))
			return true;

		DecompressVK(vk_group.dwVKey2, vk, dwCtrlKeys);
		if (vk && (m_pInputCtrl->KeyIsBeingPressed(vk) && (dwCtrlKeys & dwCurCtrlKeys) == dwCtrlKeys))
			return true;
	}

	return false;
}

//	Check whether specified logic key is toggled
bool CECInputFilter::IsLogicKeyToggled(int iLogicKey)
{
	LK2VKMap::iterator it = m_LK2VKMap.find(iLogicKey);
	if (it != m_LK2VKMap.end())
	{
		int vk;
		DWORD dwCtrlKeys;
		const VK_GROUP& vk_group = it->second;

		DecompressVK(vk_group.dwVKey1, vk, dwCtrlKeys);
		if (vk && m_pInputCtrl->KeyIsBeingToggled(vk))
			return true;

		DecompressVK(vk_group.dwVKey2, vk, dwCtrlKeys);
		if (vk && m_pInputCtrl->KeyIsBeingToggled(vk))
			return true;
	}

	return false;
}

//	Compress virtual key and control key states
DWORD CECInputFilter::CompressVK(int vk, DWORD dwCtrlKeys)
{
	return (vk << 16) | (dwCtrlKeys & EC_KSF_CTRLKEYS);
}

//	Decompress virtual key and control key states
void CECInputFilter::DecompressVK(DWORD dwSrc, int& vk, DWORD& dwCtrlKeys)
{
	dwCtrlKeys = dwSrc & EC_KSF_CTRLKEYS;
	vk = (int)(dwSrc >> 16);
}

//	Register logic key
void CECInputFilter::RegisterLogicKey(int lk, int vk1, int iCKCombo1, int vk2, int iCKCombo2)
{
	DWORD dwCtrlKeys1 = m_dwPressMask[iCKCombo1];
	DWORD dwCtrlKeys2 = m_dwPressMask[iCKCombo2];

	if (vk1) m_VK2LKMap[vk1][iCKCombo1] = lk;
	if (vk2) m_VK2LKMap[vk2][iCKCombo2] = lk;

	VK_GROUP vk_group;
	vk_group.dwVKey1 = vk1 ? CompressVK(vk1, dwCtrlKeys1) : 0;
	vk_group.dwVKey2 = vk2 ? CompressVK(vk2, dwCtrlKeys2) : 0;
	m_LK2VKMap[lk] = vk_group;
}

//	Comvert control key states to combo index
int CECInputFilter::GetCKComboIndex(DWORD dwCtrlKeys)
{
	dwCtrlKeys &= EC_KSF_CTRLKEYS;

	int j;

	//	Translate to logic keys
	for (j=0; j < NUM_CKCOMBO; j++)
	{
		if (dwCtrlKeys == m_dwPressMask[j])
			break;
	}

	ASSERT(j < NUM_CKCOMBO);
	return j;
}

//	Get logic key that is coresponding to specified vk
int CECInputFilter::FindLogicKey(int vk, DWORD dwCtrlKeys)
{
	if (!vk)
		return 0;

	int n = GetCKComboIndex(dwCtrlKeys);
	return m_VK2LKMap[vk][n];
}

//	Remap logic key
bool CECInputFilter::RemapLogicKey(int lk, int vk1, DWORD dwCtrlKeys1, int vk2, DWORD dwCtrlKeys2)
{
	//	The logic key should have been registered
	LK2VKMap::iterator it = m_LK2VKMap.find(lk);
	if (it == m_LK2VKMap.end())
	{
		ASSERT(0);
		return false;
	}

	//	Clear old logic key map
	ClearLogicKeyMap(lk, CLEAR_ALL);

	//	Re-register new lk-vk map
	int iCKCombo1 = GetCKComboIndex(dwCtrlKeys1);
	int iCKCombo2 = GetCKComboIndex(dwCtrlKeys2);
	RegisterLogicKey(lk, vk1, iCKCombo1, vk2, iCKCombo2);

	return true;
}

//	Clear specified lk-vk map if it exists
void CECInputFilter::ClearLogicKeyMap(int lk, int iClearFlag)
{
	if (!lk)
		return;

	int vk;
	DWORD dwCtrlKeys;
	VK_GROUP& vk_group = m_LK2VKMap[lk];

	if (iClearFlag & CLEAR_MAP1)
	{
		DecompressVK(vk_group.dwVKey1, vk, dwCtrlKeys);
		int iCKCombo = GetCKComboIndex(dwCtrlKeys);
		if (vk && m_VK2LKMap[vk][iCKCombo] == lk)
			m_VK2LKMap[vk][iCKCombo] = 0;

		vk_group.dwVKey1 = 0;
	}

	if (iClearFlag & CLEAR_MAP2)
	{
		DecompressVK(vk_group.dwVKey2, vk, dwCtrlKeys);
		int iCKCombo = GetCKComboIndex(dwCtrlKeys);
		if (vk && m_VK2LKMap[vk][iCKCombo] == lk)
			m_VK2LKMap[vk][iCKCombo] = 0;

		vk_group.dwVKey2 = 0;
	}
}

//	Get logic key info
bool CECInputFilter::GetLogicKeyInfo(int lk, int& vk1, DWORD& dwCtrlKeys1, int& vk2, DWORD& dwCtrlKeys2)
{
	vk1 = 0;
	dwCtrlKeys1 = 0;
	vk2 = 0;
	dwCtrlKeys2 = 0;

	//	The logic key should have been registered
	LK2VKMap::iterator it = m_LK2VKMap.find(lk);
	if (it != m_LK2VKMap.end())
	{
		const VK_GROUP& vk_group = it->second;
		DecompressVK(vk_group.dwVKey1, vk1, dwCtrlKeys1);
		DecompressVK(vk_group.dwVKey2, vk2, dwCtrlKeys2);
		return true;
	}
	else
		return false;
}

//	Clear all logic keys
void CECInputFilter::ClearAllLogicKeys()
{
	memset(m_VK2LKMap, 0, sizeof (m_VK2LKMap));
	m_LK2VKMap.clear();
}


