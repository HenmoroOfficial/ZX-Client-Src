// Filename	: CDlgTaskBase.h
// Creator	: Fu Chonggang
// Date		: 2011/4/02

#pragma once
#include "DlgBase.h"
#include "DlgTabContainer.h"
#include "vector.h"

class A2DSprite;

class CDlgTaskBase : public CDlgBase, public CDlgTabContainer<CDlgTaskBase*>
{
public:
	enum
	{
		CDLGTASK_AWARDITEM_MAX = 6,
	};

public:
	CDlgTaskBase();
	virtual ~CDlgTaskBase();
	
	ACString GetTaskItem(int idTask);
	ACString FormatTaskText(const ACHAR* szText, bool bFormatImg=true);	// 与策划定义符号

protected:

	int			m_iFixSpriteNum;
	abase::vector<A2DSprite*> m_vecImageList;			// 动态生成需要在任务描述中显示的图片
	
	virtual bool Release();
	virtual void Resize(A3DRECT rcOld, A3DRECT rcNew);
	void ClearImages();
};
