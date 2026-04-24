#include "gt_overlay.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_HostPlayer.h"

//#include <atlbase.h>
//#include <shlobj.h>
//#include <shlwapi.h>
//
//#pragma warning( disable : 4996 )

namespace overlay {

void GTOverlay::OnLogin() {
  // TODO(game client developer):
  // 登录成功. 游戏可以根据这个消息enable界面上的进入组队/帮派的按钮.
	g_pGame->GetGameRun()->GTLogin(true);

  // GT sample code: (发布前请删除以下代码)
  //EnterTeam(123456);
  //SetGTWindowPosition(150, 200);
}

void GTOverlay::OnLogout() {
	// 登出. 游戏可以根据这个消息disable界面上的进入组队/帮派的按钮.
	g_pGame->GetGameRun()->GTLogin(false);
}

void GTOverlay::OnEnterTeam() {
  // 进入组队语音. 游戏可以根据这个消息更新界面上的组队语音按钮.
	if(g_pGame->GetGameRun()->GetHostPlayer())
		g_pGame->GetGameRun()->GetHostPlayer()->SetGTTeamState(true);
}

void GTOverlay::OnLeaveTeam() {
	// 离开组队语音. 游戏可以根据这个消息更新界面上的组队语音按钮.
	if(g_pGame->GetGameRun()->GetHostPlayer())
		g_pGame->GetGameRun()->GetHostPlayer()->SetGTTeamState(false);
}

void GTOverlay::OnEnterFaction() {
	// 进入帮派语音. 游戏可以根据这个消息更新界面上的帮派语音按钮.
	if(g_pGame->GetGameRun()->GetHostPlayer())
		g_pGame->GetGameRun()->GetHostPlayer()->SetGTGuildState(true);
}

void GTOverlay::OnLeaveFaction() {
	// 离开帮派语音. 游戏可以根据这个消息更新界面上的帮派语音按钮.
	if(g_pGame->GetGameRun()->GetHostPlayer())
		g_pGame->GetGameRun()->GetHostPlayer()->SetGTGuildState(false);
}

}  // namespace overlay
