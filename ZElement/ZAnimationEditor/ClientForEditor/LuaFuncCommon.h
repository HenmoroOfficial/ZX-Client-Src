/********************************************************************
	created:	2008/11/24
	author:		liudong
	
	purpose:   common functions about lua.
	Copyright (C) 2008 - All Rights Reserved
*********************************************************************/
#pragma once

#include <vector>
#include <vector.h>
#include <ABaseDef.h>
#include <AWString.h>
#include <APoint.h>
#include <A3DVector.h>

#include "LuaAPI.h"
#include "LuaUtil.h"
#include "rpcdefs.h"

//////////////////////////////////////////////////////////////////////////
// common functions
#define SetKVPaires(key_array, val_array, key, value)	\
	do {\
		CScriptValue sv1;\
		(sv1).SetVal(key);\
		(key_array).push_back(sv1);\
		CScriptValue sv2;\
		(sv2).SetVal(value);\
		(val_array).push_back(sv2);\
	} while(0);

// value is table
#define SetKVPaires_Table(key_array, val_array, key, value)	\
do {\
	CScriptValue sv1;\
	(sv1).SetVal(key);\
	(key_array).push_back(sv1);\
	CScriptValue sv2;\
	(sv2).SetArray(value);\
	(val_array).push_back(sv2);\
} while(0);

namespace luaf
{

	//辅助函数，将数字列表放到 CScriptValue 数组中
	bool SetKVPaires_Vector(abase::vector<CScriptValue>& keys,abase::vector<CScriptValue>& vals,const char* pKey,const std::vector<int>& list);

	//辅助函数，将数字列表放到 CScriptValue 数组中
	bool SetKVPaires_Vector(abase::vector<CScriptValue>& keys,abase::vector<CScriptValue>& vals,const char* pKey,const RpcDataVector<int>& list);

	//从table中获取变量。
	const CScriptValue* GetValue(const CScriptValue& varTable, const wchar_t* pKey);

	//从table中获取简单变量。
	bool GetValue(float& value,			const CScriptValue& varTable, const wchar_t* pKey);
	bool GetValue(double& value,		const CScriptValue& varTable, const wchar_t* pKey);
	bool GetValue(int& value,			const CScriptValue& varTable, const wchar_t* pKey);
	bool GetValue(unsigned int& value,	const CScriptValue& varTable, const wchar_t* pKey);
	bool GetValue(bool& value,			const CScriptValue& varTable, const wchar_t* pKey);
	bool GetValue(AString& value,		const CScriptValue& varTable, const wchar_t* pKey);
	bool GetValue(AWString& value,		const CScriptValue& varTable, const wchar_t* pKey);

	//从var中取出一个数组，只能是lua中key从1开始、连续没有空洞的数组，否则取不出来。
	bool GetValue(abase::vector<double>& value,				const CScriptValue& var);
	bool GetValue(abase::vector<float>& value,				const CScriptValue& var);
	bool GetValue(abase::vector<int>& value,				const CScriptValue& var);
	bool GetValue(abase::vector<unsigned int>& value,		const CScriptValue& var);
	bool GetValue(abase::vector<const CScriptValue*>& value,const CScriptValue& var);

	//从var中查找key为pKey的子table，从子table中取出一个数组，只能是lua中key从1开始、连续没有空洞的数组，否则取不出来。
	bool GetValue(abase::vector<double>& value,				const CScriptValue& var, const wchar_t* pKey);
	bool GetValue(abase::vector<float>& value,				const CScriptValue& var, const wchar_t* pKey);
	bool GetValue(abase::vector<int>& value,				const CScriptValue& var, const wchar_t* pKey);
	bool GetValue(abase::vector<unsigned int>& value,		const CScriptValue& var, const wchar_t* pKey);
	bool GetValue(abase::vector<const CScriptValue*>& value,const CScriptValue& var, const wchar_t* pKey);

	bool GetValue(A3DVECTOR3& value ,const CScriptValue& var);
	bool GetValue(A3DVECTOR3& value ,const CScriptValue& var, const wchar_t* pKey);

	template <class T>
	bool GetValue(APoint<T>& value ,const CScriptValue& var)
	{
		abase::vector<T> vecTemp;
		if (!GetValue(vecTemp, var) || vecTemp.size()<2)
		{
			ASSERT(0);
			memset(&value,0,sizeof(value));
			return false;
		}
		
		value.x = vecTemp[0];
		value.y = vecTemp[1];
		return true;
	}

	template <class T>
	bool GetValue(APoint<T>& value ,const CScriptValue& var, const wchar_t* pKey)
	{
		const CScriptValue* pVar = GetValue(var,pKey);
		if (!pVar)
		{
			return false;
		}
		return GetValue(value, *pVar);
	}
};


