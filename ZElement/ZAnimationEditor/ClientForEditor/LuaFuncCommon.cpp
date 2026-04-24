/********************************************************************
created:	2008/11/24
author:		liudong

  purpose:   common functions about lua.
  Copyright (C) 2008 - All Rights Reserved
*********************************************************************/
#include "LuaFuncCommon.h"

//////////////////////////////////////////////////////////////////////////
// macros

#define _IMPLEMENT_GET_VAR_DOUBLE_LIKE(DOUBLE_LIKE_TYPE) \
	bool GetValue(DOUBLE_LIKE_TYPE& value, const CScriptValue& varTable, const wchar_t* pKey)\
	{\
		const CScriptValue* pVar = GetValue(varTable,pKey);\
		if (!pVar || CScriptValue::SVT_NUMBER!=pVar->m_Type)\
		{\
			value = (DOUBLE_LIKE_TYPE)0;\
			return false;\
		}\
		value = (DOUBLE_LIKE_TYPE)pVar->GetDouble();\
		return true;\
	}\
	bool GetValue(abase::vector<DOUBLE_LIKE_TYPE>& value, const CScriptValue& var)\
	{\
		value.clear();\
		if (var.m_Type!=CScriptValue::SVT_ARRAY)\
		{\
			ASSERT(0);\
			return false;\
		}\
		const unsigned int ARRAY_SIZE = var.m_ArrVal.size();\
		unsigned int i;\
		for (i=0; i<ARRAY_SIZE; ++i)\
		{\
			value.push_back((DOUBLE_LIKE_TYPE)0);\
		}\
		for (i=0; i<ARRAY_SIZE; ++i)\
		{\
			if (var.m_ArrKey[i].m_Type!=CScriptValue::SVT_NUMBER ||\
				var.m_ArrVal[i].m_Type!=CScriptValue::SVT_NUMBER)\
			{\
				ASSERT(0);\
				value.clear();\
				return false;\
			}\
			unsigned int iKey = (unsigned int)var.m_ArrKey[i].GetDouble()-1;\
			if (iKey>=ARRAY_SIZE)\
			{\
				ASSERT(0);\
				value.clear();\
				return false;\
			}\
			value[iKey] = (DOUBLE_LIKE_TYPE)var.m_ArrVal[i].GetDouble();\
		}\
		return true;\
	}\
	bool GetValue(abase::vector<DOUBLE_LIKE_TYPE>& value, const CScriptValue& var, const wchar_t* pKey)\
	{\
		const CScriptValue* pVar = GetValue(var,pKey);\
		if (!pVar)\
		{\
			return false;\
		}\
		return GetValue(value, *pVar);\
	}


//////////////////////////////////////////////////////////////////////////
//implementation

namespace luaf
{

	_IMPLEMENT_GET_VAR_DOUBLE_LIKE(double);
	_IMPLEMENT_GET_VAR_DOUBLE_LIKE(float);
	_IMPLEMENT_GET_VAR_DOUBLE_LIKE(int);
	_IMPLEMENT_GET_VAR_DOUBLE_LIKE(unsigned int);

	//辅助函数，将std中的数字列表放到 CScriptValue 数组中
	bool SetKVPaires_Vector(abase::vector<CScriptValue>& keys,abase::vector<CScriptValue>& vals,const char* pKey, const std::vector<int>& list)
	{
		if (list.size()<=0 || !pKey)
		{
			return false;
		}
		
		abase::vector<CScriptValue> keysTemp, valsTemp;
		std::vector<int>::iterator it;
		std::vector<int>& wList = const_cast<std::vector<int>&>(list);
		int i=0;
		for (it=(wList.begin());it!=(wList.end());it++)
		{
			SetKVPaires(keysTemp,valsTemp,(double)(i+1), (double)(*it));
			i++;
		}
		
		CScriptValue varValue;
		varValue.SetArray(valsTemp, keysTemp);
		vals.push_back(varValue);
		
		CScriptValue varKey;
		varKey.SetVal(pKey);
		keys.push_back(varKey);
		return true;
	}

	//辅助函数，将数字列表放到 CScriptValue 数组中
	bool SetKVPaires_Vector(abase::vector<CScriptValue>& keys, abase::vector<CScriptValue>& vals, const char* pKey,  const RpcDataVector<int>& list)
	{
		std::vector<int> stdList;
		RpcDataVector<int>::iterator it;
		RpcDataVector<int>& wList = const_cast<RpcDataVector<int>&>(list);
		for (it = (wList.begin());it!=(wList.end());it++)
		{
			stdList.push_back((int)(*it));
		}
		return SetKVPaires_Vector(keys,vals,pKey,stdList);
	}

	const CScriptValue* GetValue(const CScriptValue& varTable, const wchar_t* pKey)
	{
		if (!pKey || CScriptValue::SVT_ARRAY != varTable.m_Type)
		{
			return NULL;
		}

		for (int i=0; i<(int)varTable.m_ArrKey.size(); ++i)
		{
			CScriptValue* pVar = const_cast<CScriptValue*>(&(varTable.m_ArrKey[i]));
			if (CScriptValue::SVT_STRING==pVar->m_Type)
			{
				AWString str;
 				pVar->RetrieveAWString(str);
				if (str==pKey)
				{
					return &(varTable.m_ArrVal[i]);
				}
			}
 		}
		return NULL;
	}
	
	bool GetValue(bool& value, const CScriptValue& varTable, const wchar_t* pKey)
	{
		const CScriptValue* pVar = GetValue(varTable,pKey);
		if (!pVar || CScriptValue::SVT_BOOL!=pVar->m_Type)
		{
			value = false;
			return false;
		}
		value = pVar->GetBool();
		return true;
	}

	bool GetValue(AString& value, const CScriptValue& varTable, const wchar_t* pKey)
	{
		const CScriptValue* pVar = GetValue(varTable,pKey);
		if (!pVar || CScriptValue::SVT_STRING!=pVar->m_Type)
		{
			value = "";
			return false;
		}
		const_cast<CScriptValue*>(pVar)->RetrieveAString(value);
		return true;
	}
	
	bool GetValue(AWString& value, const CScriptValue& varTable, const wchar_t* pKey)
	{
		const CScriptValue* pVar = GetValue(varTable,pKey);
		if (!pVar || CScriptValue::SVT_STRING!=pVar->m_Type)
		{
			value = _AL("");
			return false;
		}
		const_cast<CScriptValue*>(pVar)->RetrieveAWString(value);
		return true;
	}

	bool GetValue(abase::vector<const CScriptValue*>& value, const CScriptValue& var)
	{
		value.clear();
		if (var.m_Type!=CScriptValue::SVT_ARRAY)
		{
			ASSERT(0);
			return false;
		}
		const unsigned int ARRAY_SIZE = var.m_ArrVal.size();
		unsigned int i;
		for (i=0; i<ARRAY_SIZE; ++i)
		{
			value.push_back(NULL);
		}
		for (i=0; i<ARRAY_SIZE; ++i)
		{
			if (var.m_ArrKey[i].m_Type!=CScriptValue::SVT_NUMBER)
			{
				ASSERT(0);
				value.clear();
				return false;
			}
			unsigned int iKey = (unsigned int)var.m_ArrKey[i].GetDouble()-1;
			if (iKey>=ARRAY_SIZE)
			{
				ASSERT(0);
				value.clear();
				return false;
			}
			value[iKey] = &(var.m_ArrVal[i]);
		}

		return true;
	}

	bool GetValue(abase::vector<const CScriptValue*>& value, const CScriptValue& var, const wchar_t* pKey)
	{
		const CScriptValue* pVar = GetValue(var,pKey);
		if (!pVar)
		{
			return false;
		}
		return GetValue(value, *pVar);
	}


	bool GetValue(A3DVECTOR3& value ,const CScriptValue& var)
	{
		abase::vector<float> vecTemp;
		if (!GetValue(vecTemp, var) || vecTemp.size()<3)
		{
			ASSERT(0);
			memset(&value,0,sizeof(value));
			return false;
		}

		value.x = vecTemp[0];
		value.y = vecTemp[1];
		value.z = vecTemp[2];
		return true;
	}

	bool GetValue(A3DVECTOR3& value ,const CScriptValue& var, const wchar_t* pKey)
	{
		const CScriptValue* pVar = GetValue(var,pKey);
		if (!pVar)
		{
			return false;
		}
		return GetValue(value, *pVar);
	}

}
