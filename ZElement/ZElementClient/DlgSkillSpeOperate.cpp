
#include "DlgSkillSpeOperate.h"
#include "EC_GameSession.h"
#include "EC_NPC.h"
#include "EC_World.h"
#include "EC_ManNPC.h"
#include "EC_HostPlayer.h"

#define new A_DEBUG_NEW


AUI_BEGIN_COMMAND_MAP(CDlgSkillTeleport, CDlgBase)

AUI_ON_COMMAND("Btn_Cancel",			OnCommandCancel)
AUI_ON_COMMAND("Btn_Transfer",			OnCommandConfirm)

AUI_END_COMMAND_MAP()


CDlgSkillTeleport::CDlgSkillTeleport()
{
	m_idNPC = 0;
}

CDlgSkillTeleport::~CDlgSkillTeleport()
{

}

void CDlgSkillTeleport::SetNpcId(int id)
{
	m_idNPC = id;
}

void CDlgSkillTeleport::OnCommandCancel(const char * szCommand)
{
	Show(false);
}

void CDlgSkillTeleport::OnCommandConfirm(const char * szCommand)
{
	GetGameSession()->c2s_CmdSummonTeleport(m_idNPC);
	Show(false);
}


void CDlgSkillTeleport::OnTick()
{
	float fDist = 0.0f;
	A3DVECTOR3 vPosNPC, vPosPlayer;
	CECNPC* pNPC = GetWorld()->GetNPCMan()->GetNPC(m_idNPC);
	if (!pNPC)
	{
		Show(false);
	}
	else
	{
		vPosNPC = pNPC->GetPos();
		vPosPlayer = GetHostPlayer()->GetPos();
		fDist = a3d_MagnitudeH(vPosNPC - vPosPlayer);
		if(fDist > 6.0f)
			Show(false);
	}
}


AUI_BEGIN_COMMAND_MAP(CDlgSkillRemote, CDlgBase)

AUI_ON_COMMAND("Btn_Remote",			OnCommandConfirm)

AUI_END_COMMAND_MAP()

CDlgSkillRemote::CDlgSkillRemote()
{
	m_iTime = 0;
}

CDlgSkillRemote::~CDlgSkillRemote()
{

}

bool CDlgSkillRemote::OnInitDialog()
{
	m_iTime = 0;
	m_dwStartTime = 0;
	m_iTrapId = 0;
	m_iTrapTid = 0;
	m_pLabTime = (PAUILABEL)GetDlgItem("Txt_Time");

	return true;
}

void CDlgSkillRemote::OnShowDialog()
{
	m_dwStartTime = GetTickCount();
}

void CDlgSkillRemote::OnCommandConfirm(const char * szCommand)
{
	GetGameSession()->c2s_CmdControlTrap(m_iTrapId, m_iTrapTid);
	Show(false);
}

void CDlgSkillRemote::SetTrapInfo(int iTrapId, int iTrapTid, int iTime)
{
	m_iTrapId = iTrapId;
	m_iTrapTid = iTrapTid;
	m_iTime = iTime * 1000;
}

void CDlgSkillRemote::OnTick()
{
	ACString strText;
	int iLeftTime = m_iTime - (GetTickCount() - m_dwStartTime);
	if(iLeftTime < 0)
		Show(false);
	else
	{
		strText.Format(_AL("%02d:%02d"), iLeftTime/ 1000 / 60, iLeftTime / 1000 % 60);
		m_pLabTime->SetText(strText);	
	}
}