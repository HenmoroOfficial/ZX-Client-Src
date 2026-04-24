#include "EC_Global.h"
#include "EC_AutoMove.h"

#include "AutoMove\MoveAgent.h"
#include "AutoMove\PfConstant.h"


#include "EC_World.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_FixedMsg.h"
#include "defence/stackchecker.h"

#include "TransferMap.h"

#include "EC_HostPlayer.h"
#include "terrain.h"
#include "EC_GameRecord.h"

extern game_record gr;

CECAutoMove g_ECAutoMove;




CECAutoMove::CECAutoMove(): 
m_iState(enumStateNone),
m_nCurStep(0),
m_NeedTransfer(false),
m_bAdjustEndLayer(false)
{
		m_TransMap = new AutoMove::CTransferMap;
}

CECAutoMove::~CECAutoMove()
{
	ClearPath(true);
	delete m_TransMap;
}


void CECAutoMove::LoadTransMap(const char * szFilePath)
{
	if (!m_TransMap->Load(szFilePath))
	{
		a_LogOutput(1,"CECAutoMove::LoadTransMap, fail to load %s", szFilePath);
	}
}

void CECAutoMove::ClearPath(bool cancel)
{

	m_a3DPath.clear();

	m_a3DPath.clear();
	m_nCurStep = 0;
	m_iState = enumStateNone;
	if (cancel)
	{
		//break, clear the needtransfer flag
		m_NeedTransfer = false;
	}
}

bool CECAutoMove::_LocalSearch(const A3DVECTOR3& start)
{
	using namespace AutoMove;

	m_CurId = g_pGame->GetGameRun()->GetWorld()->GetInstanceID();
	m_NeedTransfer = (m_CurId != m_DestId);
	A3DVECTOR3   temp_end(m_DestPos);
	if (m_NeedTransfer && !m_TransMap->GetTransferInfo(m_CurId, m_DestId, temp_end))
	{
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_AUTOMOVE_INVALID_POS);
		g_pGame->GetGameRun()->GetHostPlayer()->EndAutoMove();
		m_NeedTransfer = false;
		return false;
	}

	A3DVECTOR3  vDeltaStart(start), vDeltaGoal(temp_end);
	
	vDeltaStart.y -= g_Terrain.GetHeightAt(vDeltaStart.x, vDeltaStart.z);
	vDeltaGoal.y -= g_Terrain.GetHeightAt(vDeltaGoal.x, vDeltaGoal.z);

	A3DPOINT2 ptStart, ptGoal;
	ptStart = g_AutoMoveAgent.TransWld2Map(vDeltaStart);
	ptGoal  = g_AutoMoveAgent.TransWld2Map(vDeltaGoal);
	int iLayerStart, iLayerGoal;
	iLayerStart = g_AutoMoveAgent.WhichLayer(ptStart, vDeltaStart.y);
	if (m_bAdjustEndLayer)
	{
		iLayerGoal = g_AutoMoveAgent.WhichLayer(ptGoal, vDeltaGoal.y);
	}
	else
	{
		iLayerGoal = 0;
	}

	//if (!g_AutoMoveAgent.SetStartEnd(vDeltaStart, vDeltaGoal))
	if (!g_AutoMoveAgent.SetStartEnd(ptStart, iLayerStart, ptGoal, iLayerGoal))
	{
		int iMsg = (g_AutoMoveAgent.GetState() == PF_STATE_INVALIDSTART)? (FIXMSG_AUTOMOVE_INVALID_START ) :(FIXMSG_AUTOMOVE_INVALID_POS);
		g_pGame->GetGameRun()->AddFixedMessage(iMsg);
		g_pGame->GetGameRun()->GetHostPlayer()->EndAutoMove();
		m_NeedTransfer = false;
		return false;
	}

	g_AutoMoveAgent.Search();
	if (g_AutoMoveAgent.GetState() != PF_STATE_FOUND)
	{
		m_iState = enumStateFail;
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_AUTOMOVE_NO_PATH);
		g_pGame->GetGameRun()->GetHostPlayer()->EndAutoMove();
		m_NeedTransfer = false;
		return false;
	}

	g_AutoMoveAgent.Retrieve3DPath(m_a3DPath);


	m_iState = enumStateSuccess;
	g_pGame->GetGameRun()->GetHostPlayer()->BeginAutoMove();


	return true;
}


bool CECAutoMove::SearchPath(const A3DVECTOR3& vStart, const A3DVECTOR3& vEnd, int endId, bool bAdjustEndLayer)
{
	// save for record
	if(gr.get_state() == game_record::enum_gr_record)
	{
		frame_record* fr = gr.inc_frame();
		GNET::Marshal::OctetsStream os;
		os.reserve(128);
		int iTemp = (bAdjustEndLayer? 1:0);
		os << vStart.x;
		os << vStart.y;
		os << vStart.z;
		os << vEnd.x;
		os << vEnd.y;
		os << vEnd.z;
		os << endId;
		os << iTemp;
		fr->push_event(new event_record_data(RECORD_DATA_AUTOMOVE, os));
	}

	StackChecker::ACTrace(6);

	ClearPath(true); 
	
	m_bAdjustEndLayer = bAdjustEndLayer;

	if (!g_pGame->GetGameRun()->GetHostPlayer()->CanDo(CECHostPlayer::CANDO_AUTOMOVE))
	{
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_AUTOMOVE_FORBID);
		return false;
	}
	if (!g_AutoMoveAgent.IsReady())
	{
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_AUTOMOVE_INVALID_MAP);
		return false;
	}

	
	m_DestPos = vEnd;
	m_DestId =  endId;
	
	//m_DestPos = A3DVECTOR3(50, 0, 50);
	//m_DestId = 10;

	return _LocalSearch(vStart);
	
}


void CECAutoMove::CompleteSearch(const A3DVECTOR3& curPos)
{

	ClearPath(false);

	if (!m_NeedTransfer)
	{
		return;
	}

	if (!g_AutoMoveAgent.IsReady())
	{
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_AUTOMOVE_INVALID_MAP);
		m_NeedTransfer = false;
		return;
	}

	_LocalSearch(curPos);

}

void CECAutoMove::Tick(DWORD dwDeltaTime)
{

	if (m_NeedTransfer && m_CurId != g_pGame->GetGameRun()->GetWorld()->GetInstanceID())
	{
		CompleteSearch(g_pGame->GetGameRun()->GetHostPlayer()->GetPos());
	}


}

void CECAutoMove::Render(CECViewport* pViewport)
{

}

/*
//method 1
int CECAutoMove::MoveOn(const int lastIndex)
{
	A3DVECTOR3 cur_pos = g_pGame->GetGameRun()->GetHostPlayer()->GetPos();
	int   catch_count = g_AutoMoveAgent.GetOptimizeCatchCount();
	
	int i = 0;
	float min_dist2 = 99999999.0f;
	int   local_index = -1;
	
	while (i < catch_count && i < (int)m_a3DPath.size())
	{
		float dist2= (m_a3DPath[i].x - cur_pos.x) * (m_a3DPath[i].x - cur_pos.x) 
					+(m_a3DPath[i].z - cur_pos.z) * (m_a3DPath[i].z - cur_pos.z);
		if (dist2 < min_dist2)
		{
			min_dist2 = dist2;
			local_index = i;
		}
		++i;
	}

	if (local_index < 0)
	{
		return lastIndex;
	}

	local_index++;  //the ceiling
	
	AString  msg;
	msg.Format("local %d\n", local_index);
	OutputDebugStringA(msg);

	int global_index = lastIndex + local_index;

	if (!g_AutoMoveAgent.Optimize(global_index-1, &m_a3DPath))
	{
		m_a3DPath.erase(m_a3DPath.begin(), m_a3DPath.begin()+local_index);
	}

	
	return global_index;

}

*/


//method 2
int CECAutoMove::MoveOn(const int lastIndex)
{

	assert(m_nCurStep <= lastIndex);

	A3DVECTOR3 cur_pos = g_pGame->GetGameRun()->GetHostPlayer()->GetPos();
	int   catch_count = g_AutoMoveAgent.GetOptimizeCatchCount();
	
	int base = lastIndex - m_nCurStep;
	int i = base;
	float min_dist2 = 99999999.0f;
	int   local_index = -1;
	
	while (i < base+catch_count && i < (int)m_a3DPath.size())
	{
		float dist2= (m_a3DPath[i].x - cur_pos.x) * (m_a3DPath[i].x - cur_pos.x) 
					+(m_a3DPath[i].z - cur_pos.z) * (m_a3DPath[i].z - cur_pos.z);
		if (dist2 < min_dist2)
		{
			min_dist2 = dist2;
			local_index = i;
		}
		++i;
	}

	if (local_index < 0)
	{
		return lastIndex;
	}

	local_index++;  //the ceiling
	
// 	AString  msg;
// 	msg.Format("base %d local %d\n", base, local_index);
// 	OutputDebugStringA(msg);

	
	m_nCurStep += local_index;

	if (!g_AutoMoveAgent.Optimize(m_nCurStep-1, &m_a3DPath))
	{
		m_a3DPath.erase(m_a3DPath.begin(), m_a3DPath.begin()+local_index);
	}

	
	return m_nCurStep;

}


