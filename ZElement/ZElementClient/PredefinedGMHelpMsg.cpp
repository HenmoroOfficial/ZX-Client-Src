#include "PredefinedGMHelpMsg.h"
#include <A3DMacros.h>

static GMHelpMsg _msgs[] = 
{
	{ 0,	A3DCOLORRGB(0xff, 0xff, 0xff),	802 },
	{ 0,	A3DCOLORRGB(0x00, 0xff, 0xff),	803 },
	{ 0,	A3DCOLORRGB(0xff, 0x00, 0xff),	804 },
};
static const int _msgCount = sizeof(_msgs) / sizeof(_msgs[0]);


CGMHelpMsgArray::CGMHelpMsgArray()
{
	for (int nIndex = 0; nIndex < _msgCount; ++nIndex)
	{
		GMHelpMsg &msg = _msgs[nIndex];
		msg.type = nIndex + GM_HELP_MSG_TYPE_BASE;
		push_back(msg);
	}
}

bool CGMHelpMsgArray::FindByType(ACHAR type, GMHelpMsg * pMsg) const
{
	int nIndex = type - GM_HELP_MSG_TYPE_BASE;
	if (nIndex >= 0 && nIndex < static_cast<int>(size()))
	{
		if (pMsg) *pMsg = at(nIndex);
		return true;
	}
	else
	{
		return false;
	}
}