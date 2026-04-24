#ifndef _ICON_3D_H_
#define _ICON_3D_H_

#include <map>
#include <set>
#include <vector>
#include <A3DVertex.h>
#include <A3DStream.h>

class A3DTexture;
class A3DDevice;

struct Icon3DItem
{
	A3DLVERTEX m_vVerts[4];
	A3DTexture* m_pTexture;
};

class Icon3DRender
{
public:
	Icon3DRender();
	~Icon3DRender();

public:

	bool Init(A3DDevice* pDevice);
	void AddIcon(int id, const Icon3DItem& item);
	void RenderFlush(A3DDevice* pDevice);

private:

	void preRender(A3DTexture* pTexture);
	void postRender(A3DTexture* pTexture);
	bool presetIndexBuf();

private:
	typedef std::vector<Icon3DItem> IconArray;
	typedef std::set<int> IconHashSet;
	typedef std::map<A3DTexture*, IconArray> IconMap;
	IconMap m_mapIcons;
	IconHashSet m_setHash;

	A3DStream m_stream;
	A3DDevice* m_pDevice;
};

#endif