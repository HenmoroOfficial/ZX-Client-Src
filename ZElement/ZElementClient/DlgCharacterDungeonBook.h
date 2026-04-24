// Filename	: DlgCharacterDungeonBook.h
// Creator	: Fu Chonggang
// Date		: 2012.4.19
// Desc		: ÅÀËþ¸±±¾Í¼¼ø

#pragma once

#include "DlgBase.h"

class AUIImagePicture;
#define MONSTER_PERPAGE	20

class CDlgCharacterDungeonBook : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();

public:
	CDlgCharacterDungeonBook();
	virtual ~CDlgCharacterDungeonBook();
	
	void OnCommandLast(const char * szCommand);
	void OnCommandNext(const char * szCommand);
	void OnCommandHead(const char * szCommand);
	void OnCommandTail(const char * szCommand);

	void SetBookIcon(AUIImagePicture *pImg, int id);
	void SetMonsterIcon(AUIImagePicture *pImg, int id);


protected:
	virtual bool OnInitDialog();
	virtual bool Release();
	virtual void OnShowDialog();
	virtual void OnTick();
	void UpdatePage();

private:
	int					m_iCurHintIndex;

	AUIImagePicture *m_pImgMonster[MONSTER_PERPAGE];
	int m_iCurPage;
	static const int ms_iNumTotal;
};
