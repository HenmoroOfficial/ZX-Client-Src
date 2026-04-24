// Filename	: DlgPetFlyShow.cpp
// Creator	: Fu Chonggang
// Date		: 2013.8.16

#include "AUI\\AUIImagePicture.h"
#include "DlgPetFlyShow.h"
#include "EC_GameUIMan.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_Model.h"
#include "EC_IvtrConsume.h"
#include "EC_HostPlayer.h"
#include "EC_ModelRender.h"
#include "EC_NPC.h"
#include "EC_ManPlayer.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgPetFlyShow, CDlgBase)
AUI_ON_COMMAND("Btn_Help",			OnCommand_Help)
AUI_END_COMMAND_MAP()


CDlgPetFlyShow::CDlgPetFlyShow() : 
m_pPet(0),
m_pIvtrPet(0),
m_fModelSize(0)
{
}

CDlgPetFlyShow::~CDlgPetFlyShow()
{
}

bool CDlgPetFlyShow::OnInitDialog()
{
	m_pImgPet = (PAUIIMAGEPICTURE)GetDlgItem("Img_Show");

	return true;
}

bool CDlgPetFlyShow::Release()
{
	A3DRELEASE(m_pPet);

	return CDlgBase::Release();
}

void CDlgPetFlyShow::OnTick()
{
	if(m_iShape!=m_pIvtrPet->GetEssence().shape)
		LoadPetModel();
	
	if (!m_pPet)
	{
		m_pImgPet->SetRenderCallback(NULL,	(DWORD)m_pPet, MRT_PET, 0);
		return;
	}
	
	m_pPet->Tick(GetGame()->GetTickTime());
	m_pImgPet->SetRenderCallback(DlgModelRenderFixDist,	(DWORD)m_pPet, (DWORD)(&m_fModelSize), 0);
}

void CDlgPetFlyShow::OnHideDialog()
{
	if(m_pIvtrPet)
		m_pIvtrPet->Freeze(false);
}

int CDlgPetFlyShow::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if ( strMsg=="set_pet" || strMsg=="fly_finish" )
	{
		m_pIvtrPet = (CECPetBedge*)GetHostPlayer()->GetPack()->GetItem(wParam);
		m_pIvtrPet->Freeze(true);
		LoadPetModel();

		Show(true);
	}

	return 0;
}

void CDlgPetFlyShow::LoadPetModel()
{
	if (m_pPet)
		A3DRELEASE(m_pPet);

	m_iShape = m_pIvtrPet->GetEssence().shape;
	AString strPath = GetGame()->GetDataPath(m_pIvtrPet->GetDBEssence()->file_to_shown[m_iShape-1]);
	EC_PLAYERLOADRESULT loadResult;
	if (!CECPlayer::LoadPetModel(strPath, loadResult))
	{
		a_LogOutput(1, "CDlgPetFlyShow::LoadPetModel load %s failed", strPath);
		return;
	}

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
}
void CDlgPetFlyShow::OnCommand_Help(const char * szCommand)
{
	AString strDlg = "Win_PetLevelUp_Help";
	GetGameUIMan()->CommonMessage("Win_HelpInfo","open",(WPARAM)(&strDlg),0);
}
