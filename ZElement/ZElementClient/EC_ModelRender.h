// Filename	:	DlgDailyPromp.h
// Creator	:	Fu Chonggang
// Date		:	Sep 2, 2010

#pragma once

#include "A3DTypes.h"

class CECLoginPlayer;

// ModelRenderType
enum
{
	MRT_FENGSHEN,
	MRT_DAILYPROMP,
	MRT_FENGSHEN_EP,
	MRT_LITTLEPET,
	MRT_PET,
	MRT_NUM,
};
// 动态根据modelAABB调整摄像机距离
// param1 model
// param2 ModelRenderType
// param3 angle
void DlgModelRender(const A3DRECT &rc, DWORD param1, DWORD param2, DWORD param3);
// 通过参数传递model大小，考虑到模型的AABB大小可能随动作改变
// param1 model
// param2 模型高度
// param3 angle
void DlgModelRenderFixDist(const A3DRECT &rc, DWORD param1, DWORD param2, DWORD param3);
void DlgDeityModelRender(const A3DRECT &rc, DWORD param1, DWORD param2, DWORD param3);
void DlgModelMountWingRender(const A3DRECT &rc, DWORD param1, DWORD param2, DWORD param3);
void ReLoadModel(CECLoginPlayer*& pPlayer);
