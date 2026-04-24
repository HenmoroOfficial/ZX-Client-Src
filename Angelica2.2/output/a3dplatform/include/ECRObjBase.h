#ifndef _ELEMENT_CLIENT_RENDER_OBJECT_BASE_H_
#define _ELEMENT_CLIENT_RENDER_OBJECT_BASE_H_

#include "A3DTypes.h"

struct ELEMENT_RENDER_PARAM;
class ECViewport;

struct ECRENDERPARAM
{
	ECViewport*				pViewport;
	A3DLIGHTPARAM			paramLight;
	float					fDNFactor;

	unsigned int nWaterEffect   : 6;
	unsigned int bUnderWater    : 1;
	unsigned int bModelLightMap : 1;

	ECRENDERPARAM(const ELEMENT_RENDER_PARAM& ERParam, ECViewport* pECViewport);
};

class ECRObjBase
{
public:
	virtual bool Render(const ECRENDERPARAM* pRenderParam) = NULL;
};

#endif // _ELEMENT_CLIENT_RENDER_OBJECT_BASE_H_