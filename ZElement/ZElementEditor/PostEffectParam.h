#ifndef _POSTEFFECT_PARAM_H_
#define _POSTEFFECT_PARAM_H_

#include <AIniFile.h>

class CPostEffectParam
{
public:
	CPostEffectParam();
	~CPostEffectParam();

//²Ù×÷
public:
	bool	SavePostParams();
	bool	LoadPostParams();

protected:
	void 	LoadBoolFromIni(AIniFile* pFileIni,const char* szSect,const char* szKey,bool& bValue);
	void 	LoadIntFromIni(AIniFile* pFileIni,const char* szSect,const char* szKey,int& nValue);
	void 	LoadFloatFromIni(AIniFile* pFileIni,const char* szSect,const char* szKey,float& fValue);
	void 	LoadStringFromIni(AIniFile* pFileIni,const char* szSect,const char* szKey,AString& strValue);
};


#endif