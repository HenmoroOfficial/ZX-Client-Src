// Filename	: DlgTab3D.h
// Creator	: Fu Chonggang
// Date		: 2012-6-12
/* Desc		: 
*/

#pragma once

#include "DlgBase.h"
#include "vector.h"

class A3DStream;
class A3DTexture;
class AUIStillImageButton;

class TabInfo
{
public:
	TabInfo() {}
	TabInfo(unsigned int a, float ltx, float lty, float rtx, float rty, float lbx, float lby, float rbx, float rby) 
	{
		alpha = a;
		pos[0][0] = ltx;
		pos[0][1] = lty;
		pos[1][0] = rtx;
		pos[1][1] = rty;
		pos[2][0] = lbx;
		pos[2][1] = lby;
		pos[3][0] = rbx;
		pos[3][1] = rby;
		
	}
	unsigned int alpha;		// 0-255
	float pos[4][2];	// lefttop righttop leftbot rightbot
	
	
	friend TabInfo operator + (const TabInfo& v1, const TabInfo& v2);
	friend TabInfo operator * (const TabInfo& v, float f);
};

class CDlgTab3D : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();


public:
	CDlgTab3D();
	virtual ~CDlgTab3D();
	
	void OnCommand_Prev(const char * szCommand);
	void OnCommand_Next(const char * szCommand);

	bool AddTextrure(AString strPath);
	void ReleaseSprites();
	
	virtual void AddTextrures();
	virtual void OnSelOneItem() {}
protected:
	virtual bool OnInitDialog();
	virtual bool Release();
	virtual void OnTick();
	virtual bool Render(void);
	virtual void Resize(A3DRECT rcOld, A3DRECT rcNew);

	void RenderSprites();
	void RenderSprite(TabInfo info, int index, int deltaX=0, int deltaY=0);

	int	m_iCurPos;
	int	m_iStage;		// 0-1000平滑过度
	bool m_bNext;		// true:选择下一个， false:选择上一个

	AUIStillImageButton	*m_pBtnLeft;
	AUIStillImageButton	*m_pBtnRight;

	A3DStream *m_pTab3DStream;
	abase::vector<A3DTexture*>	m_vecTextures;
	float			m_fDeltaX;
	float			m_fDeltaY;
	TabInfo			ms_TabInfo[7];
	static int		ms_iSpeed;
};
