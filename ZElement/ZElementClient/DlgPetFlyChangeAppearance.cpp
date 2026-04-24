// Filename	: DlgPetFlyChangeAppearance.cpp
// Creator	: Fu Chonggang
// Date		: 2013.8.16

#include "AUI\\AUIImagePicture.h"
#include "DlgPetFlyChangeAppearance.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_IvtrConsume.h"
#include "EC_GameSession.h"
#include "EC_ModelRender.h"
#include "EC_Model.h"
#include "EC_Game.h"
#include "EC_NPC.h"
#include "EC_ManPlayer.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgPetFlyChangeAppearance, CDlgBase)

AUI_ON_COMMAND("Rdo*",			OnCommandSelect)
AUI_ON_COMMAND("Btn_Save",		OnCommandSave)

AUI_END_COMMAND_MAP()


CDlgPetFlyChangeAppearance::CDlgPetFlyChangeAppearance() : 
m_pPet(0),
m_bLoaded(false),
m_fModelSize(0)
{
}

CDlgPetFlyChangeAppearance::~CDlgPetFlyChangeAppearance()
{
}

bool CDlgPetFlyChangeAppearance::OnInitDialog()
{
	m_pImgPet = (PAUIIMAGEPICTURE)GetDlgItem("Img_Show");
	return true;
}

bool CDlgPetFlyChangeAppearance::Release()
{
	A3DRELEASE(m_pPet);
	return CDlgBase::Release();
}

void CDlgPetFlyChangeAppearance::OnTick()
{
	if (!m_pPet || !m_bLoaded)
	{
		m_pImgPet->SetRenderCallback(NULL,	(DWORD)m_pPet, MRT_PET, 0);
		return;
	}

	m_pPet->Tick(GetGame()->GetTickTime());
	m_pImgPet->SetRenderCallback(DlgModelRenderFixDist,	(DWORD)m_pPet, (DWORD)(&m_fModelSize), 0);
}

void CDlgPetFlyChangeAppearance::OnCommandSelect(const char * szCommand)
{
	int iIndex = GetCheckedRadioButton(1);
	ChangeShape(2*(iIndex-1));
}

void CDlgPetFlyChangeAppearance::OnCommandSave(const char * szCommand)
{
	GetGameSession()->c2s_CmdPetChangeShape(GetData("pet_index"), GetCheckedRadioButton(1));
}

int CDlgPetFlyChangeAppearance::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="set_pet")
	{
		SetData(wParam, "pet_index");
		CECPetBedge *pIvtrPet = (CECPetBedge*)GetHostPlayer()->GetPetPack()->GetItem(wParam);
		if (!pIvtrPet)
			return 0;

		if (!m_pPet)
			m_pPet = new CECModel();
		m_pIvtrPet = pIvtrPet;
		int iIndex = (pIvtrPet->GetEssence().shape+1)/2;
		CheckRadioButton(1, iIndex);
		OnCommandSelect("");
		Show(true);
	}

	return 0;
}

bool CDlgPetFlyChangeAppearance::ChangeShape(int index)
{
	m_bLoaded = false;
	AString strPath = GetGame()->GetDataPath(m_pIvtrPet->GetDBEssence()->file_to_shown[index]);
	EC_PLAYERLOADRESULT loadResult;
	if (!CECPlayer::LoadPetModel(strPath, loadResult))
		return false;

	A3DRELEASE(m_pPet);
	m_pPet = loadResult.pPetModel;

	const A3DAABB& chAABB = m_pPet->HasCHAABB() ? m_pPet->GetCHAABB() : m_pPet->GetModelAABB();
	m_fModelSize = chAABB.Extents.y;
	m_pPet->SetGfxUseLOD(false);
	m_pPet->SetPos(-chAABB.Center);
	m_pPet->SetDirAndUp(A3DVECTOR3(0, 0, -1.0f), A3DVECTOR3(0, 1.0f, 0));
	m_pPet->SetAutoUpdateFlag(false);
	m_pPet->Tick(1);
	m_pPet->SetAutoUpdateFlag(true);
	m_pPet->PlayActionByName(CECNPC::GetActionName(CECNPC::ACT_IDLE), 1.0f, false);
	m_pPet->QueueAction(CECNPC::GetActionName(CECNPC::ACT_STAND));
	m_bLoaded = true;

	return true;
}
