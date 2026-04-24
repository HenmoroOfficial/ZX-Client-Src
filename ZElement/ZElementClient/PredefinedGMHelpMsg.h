#pragma once

#include <vector.h>
#include <ABaseDef.h>
#include <A3DTypes.h>
#include "SingletonHolder.h"

struct GMHelpMsg
{
	ACHAR type;
	A3DCOLOR color;
	int msg;
};

typedef abase::vector<GMHelpMsg> CGMHelpMsgArrayBase;
class CGMHelpMsgArray : public CGMHelpMsgArrayBase
{
public:
	CGMHelpMsgArray();
	bool FindByType(ACHAR type, GMHelpMsg * pMsg) const;
};
typedef SingletonHolder<const CGMHelpMsgArray> TheGMHelpMsgArray;

#ifdef UNICODE
	#define GM_HELP_MSG_TYPE_BASE 0xfff0
#else
	#define GM_HELP_MSG_TYPE_BASE 0xf0
#endif

