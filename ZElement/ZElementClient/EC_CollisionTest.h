// Filename	: EC_CollisionText.h
// Creator	: Fu Chonggang
// Date		: 2012.10.18
// Åö×²¸±±¾²âÊÔ´úÂë

#pragma once

#include "a3dvector.h"
#include "vector.h"

class DispColliInfo  
{
public:
	DispColliInfo() : m_iIndex(-1) {}
	~DispColliInfo() {}

	struct ColliInfo
	{
		ColliInfo(A3DVECTOR3 _p1,
			A3DVECTOR3 _p2,
			A3DVECTOR3 _v1,
			A3DVECTOR3 _v2,
			A3DVECTOR3 _v12,
		A3DVECTOR3 _v22)
		{
			p1=_p1; p2=_p2; v1=_v1; v2=_v2; v12=_v12; v22=_v22;
		}
		A3DVECTOR3 p1;
		A3DVECTOR3 p2;
		A3DVECTOR3 v1;
		A3DVECTOR3 v2;
		A3DVECTOR3 v12;
		A3DVECTOR3 v22;
	};

	void Add(A3DVECTOR3 p1,
		A3DVECTOR3 p2,
		A3DVECTOR3 v1,
		A3DVECTOR3 v2,
		A3DVECTOR3 v12,
		A3DVECTOR3 v22);
	void Clear() { m_vecInfos.clear(); }

	void DispIndex(int index) { m_iIndex = index; }
	void Render();
	void RenderInfo(ColliInfo &dispInfo);
	

protected:
	abase::vector<ColliInfo> m_vecInfos;
	int m_iIndex;
};

extern DispColliInfo g_DispColliInfo;