/********************************************************************
	created:	2005/09/03
	created:	3:9:2005   11:48
	file name:	ECHelpDialog.h
	author:		yaojun
	
	purpose:	interface for help dialog
*********************************************************************/

#pragma once


#include <AString.h>

class AUIObject;
class CECScriptContext;

class IECScriptUI
{
public:
	virtual ~IECScriptUI() {}
	virtual void ShowHelpDialog() = 0;
	virtual void CloseHelpDialog() = 0;
	virtual void SetHasNextTip(bool bHasNext) = 0;
	virtual void SetHelpDialogContent(AString strContent) = 0;
	virtual void AddToSpirit(int nScriptID, AString strName) = 0;
};