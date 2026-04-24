/********************************************************************
	created:	2012/3/19
	file name:	CDlgGMTransferMap.h
	author:		ice
*********************************************************************/

#pragma once

#include "DlgBase.h"
#include "hashmap.h"
#include "vector.h"

class AUIComboBox;
class AUIListBox;

class CDlgGMTransferMap : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
public:

	struct MapInfo
	{
		int idMap;
		float x;
		float y;
		float z;

		ACString strName;
	};

	CDlgGMTransferMap();
	~CDlgGMTransferMap();

	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	
	void OnCommandChangeConfirm(const char * szCommand);
	void OnCommandCancel(const char * szCommand);

	
	void OnEventListMapDBClicked(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	typedef abase::vector<MapInfo> TransferMaps;

private:
	AUIListBox*  m_pLstMaps;

	TransferMaps m_TransferMaps;
	
	void LoadFromFile();
};