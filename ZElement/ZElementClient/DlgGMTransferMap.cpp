#include "AFI.h"
#include "DlgGMTransferMap.h"
#include "AUI\\AUIListBox.h"
#include "AScriptFile.h"
#include "EC_GameSession.h"

AUI_BEGIN_COMMAND_MAP(CDlgGMTransferMap, CDlgBase)

AUI_ON_COMMAND("Btn_Confirm",		OnCommandChangeConfirm)
AUI_ON_COMMAND("Btn_Cancel",		OnCommandCancel)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgGMTransferMap, CDlgBase)

AUI_ON_EVENT("Lst_Map",	WM_LBUTTONDBLCLK,		OnEventListMapDBClicked)

AUI_END_EVENT_MAP()

CDlgGMTransferMap::CDlgGMTransferMap()
{

}

CDlgGMTransferMap::~CDlgGMTransferMap()
{
	
}

bool CDlgGMTransferMap::OnInitDialog()
{
	m_pLstMaps = (PAUILISTBOX)GetDlgItem("Lst_Map");

	LoadFromFile();

	for (TransferMaps::iterator itMap=m_TransferMaps.begin(); itMap!=m_TransferMaps.end(); ++itMap)
	{
		m_pLstMaps->AddString((*itMap).strName);
		m_pLstMaps->SetItemDataPtr(m_pLstMaps->GetCount()-1, (void*)itMap);
	}

	return true;
}

void CDlgGMTransferMap::OnShowDialog()
{
	m_pLstMaps->SetCurSel(0);
}

void CDlgGMTransferMap::OnTick()
{

}

void CDlgGMTransferMap::OnCommandChangeConfirm(const char * szCommand)
{
	int iCurSel = m_pLstMaps->GetCurSel();
	if (iCurSel>=0 && iCurSel<m_pLstMaps->GetCount())
	{
		MapInfo* pMapInfo = (MapInfo*)m_pLstMaps->GetItemDataPtr(iCurSel);

		if(!pMapInfo)
			return;

		GetGameSession()->gm_TransferMap(pMapInfo->idMap, pMapInfo->x, pMapInfo->y, pMapInfo->z);
	}	
}

void CDlgGMTransferMap::OnCommandCancel(const char * szCommand)
{
	Show(false);
}

void CDlgGMTransferMap::LoadFromFile()
{
	AScriptFile sfile;
	if (!sfile.Open("gm_maps.txt"))
		return;

	while (sfile.PeekNextToken(true))
	{
		MapInfo mapInfo;
		ACString strMap, strValue;
		
		sfile.GetNextToken(true);
		strMap = AS2AC(sfile.m_szToken);

		mapInfo.strName = strMap;

		mapInfo.idMap = sfile.GetNextTokenAsInt(true);
		mapInfo.x = sfile.GetNextTokenAsFloat(true);
		mapInfo.y = sfile.GetNextTokenAsFloat(true);
		mapInfo.z = sfile.GetNextTokenAsFloat(false);
		
		m_TransferMaps.push_back(mapInfo);
	}

	sfile.Close();
	return;
}

void CDlgGMTransferMap::OnEventListMapDBClicked(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	int iCurSel = m_pLstMaps->GetCurSel();
	if (iCurSel>=0 && iCurSel<m_pLstMaps->GetCount())
	{
		MapInfo* pMapInfo = (MapInfo*)m_pLstMaps->GetItemDataPtr(iCurSel);

		if(!pMapInfo)
			return;

		GetGameSession()->gm_TransferMap(pMapInfo->idMap, pMapInfo->x, pMapInfo->y, pMapInfo->z);
	}
}