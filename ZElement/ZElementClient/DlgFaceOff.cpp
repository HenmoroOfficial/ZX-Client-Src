// Filename	: DlgFaceOff.cpp
// Creator	: Fu Chonggang
// Date		: 2009/05/13

#include "AFI.h"
#include "AUI\\CSplit.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIComboBox.h"
#include "AUI\\AUIStillImageButton.h"
#include "DlgFaceOff.h"
#include "EC_HostPlayer.h"
#include "EC_GameUIMan.h"
#include "EC_Shortcut.h"
#include "EC_ShortcutSet.h"
#include "EC_GameSession.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_Inventory.h"
#include "EC_LoginPlayer.h"
#include "EC_World.h"
#include "EC_ManPlayer.h"
#include "EC_LoginPlayer.h"
#include "EC_Model.h"
#include "roleinfo"
#include "EC_GameUIMisc.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgFaceOff, CDlgBase)

AUI_ON_COMMAND("confirm",			OnCommandConfirm)
AUI_ON_COMMAND("Btn_LeftHair",		OnCommandHairLeft)
AUI_ON_COMMAND("Btn_RightHair",		OnCommandHairRight)
AUI_ON_COMMAND("Combo_Hair",		OnCommandHairChange)
AUI_ON_COMMAND("Btn_LeftFace",		OnCommandFaceLeft)
AUI_ON_COMMAND("Btn_RightFace",		OnCommandFaceRight)
AUI_ON_COMMAND("Combo_Face",		OnCommandFaceChange)
AUI_ON_COMMAND("Btn_RightCloth",	OnCommandClothRight)
AUI_ON_COMMAND("Btn_LeftCloth",		OnCommandClothLeft)
AUI_ON_COMMAND("Combo_Cloth",		OnCommandClothChange)
AUI_ON_COMMAND("Btn_LeftEar",		OnCommandEarLeft)
AUI_ON_COMMAND("Btn_RightEar",		OnCommandEarRight)
AUI_ON_COMMAND("Combo_Ear",			OnCommandEarChange)
AUI_ON_COMMAND("Btn_LeftTail",		OnCommandTailLeft)
AUI_ON_COMMAND("Btn_RightTail",		OnCommandTailRight)
AUI_ON_COMMAND("Combo_Tail",		OnCommandTailChange)
AUI_ON_COMMAND("Rdo_ZoomIn",		OnCommandZoom)
AUI_ON_COMMAND("Rdo_ZoomOut",		OnCommandZoom)
AUI_ON_COMMAND("Rdo_*",		OnCommandType)
AUI_ON_COMMAND("change",	OnCommandChange)
AUI_ON_COMMAND("IDCANCEL",	OnCommand_CANCEL)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgFaceOff, CDlgBase)

AUI_ON_EVENT("Img_Face",		WM_MOUSEMOVE,		OnEventMouseMove)
AUI_ON_EVENT("Img_Face",		WM_LBUTTONDOWN,		OnEventLButtonDown_Size)
AUI_ON_EVENT("Img_Face",		WM_LBUTTONUP,		OnEventLButtonUp_Size)

AUI_END_EVENT_MAP()

CDlgFaceOff::CDlgFaceOff()
{
	m_nMouseOffset = 0;
	m_nMouseOffsetThis = 0;
	m_pPlayer = NULL;
}

CDlgFaceOff::~CDlgFaceOff()
{
	A3DRELEASE(m_pPlayer);
}

bool CDlgFaceOff::OnInitDialog()
{
	m_pComboHair = (PAUICOMBOBOX)GetDlgItem("Combo_Hair");
	m_pComboFace = (PAUICOMBOBOX)GetDlgItem("Combo_Face");
	m_pComboCloth = (PAUICOMBOBOX)GetDlgItem("Combo_Cloth");
	m_pComboEar = (PAUICOMBOBOX)GetDlgItem("Combo_Ear");
	m_pComboTail = (PAUICOMBOBOX)GetDlgItem("Combo_Tail");
	m_pImg_Head = (PAUIIMAGEPICTURE)GetDlgItem("Img_Face");
	m_pImg_Head->SetAcceptMouseMessage(true);
	m_pBtn_LeftTurn = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_LeftTurn");
	m_pBtn_RightTurn = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_RightTurn");

	CheckRadioButton(3, 2);
	m_iCameraWeight = 0;
	return true;
}

void CDlgFaceOff::InitStrings()
{
	CECHostPlayer* pHost = GetHostPlayer();
	if (!pHost)
		return;

	int iGender = pHost->GetGender();
	int iStrFace, iStrHair, iCloth;
	switch(pHost->GetProfRace())
	{
	case PROFRACE_JIULI:
		iStrFace = 8103;
		iStrHair = 8101;
		iCloth = 8117;
		break;
	case PROFRACE_LIESHAN:
		iStrFace = 8111;
		iStrHair = 8109;
		iCloth = 8117;
		break;
	case PROFRACE_TIANHUA:
		iStrFace = 8115;
		iStrHair = 8113;
		iCloth = 8117;
		break;
	case PROFRACE_HUAIGUANG:
		iStrFace = 8107;
		iStrHair = 8105;
		iCloth = 8117;
		break;
	case PROFRACE_TAIHAO:
		iStrFace = 8103;
		iStrHair = 8101;
		iCloth = 8117;
		break;
	case PROFRACE_CHENHUANG:
		iStrFace = 8121;
		iStrHair = 8119;
		iCloth = 8117;
		break;
	case PROFRACE_QIANJI:
		iStrFace = 8129;
		iStrHair = 8127;
		iCloth = 8416;
		break;
	case PROFRACE_YINGZHAO:
		iStrFace = 8125;
		iStrHair = 8123;
		iCloth = 8416;
		break;
	default:
		iStrFace = 8401;
		iStrHair = 8403;
		iCloth = 8416;
	}
	unsigned int i;
	CSPLIT_STRING_VECTOR vec;
	CSplit s(_AL(""));

	m_pComboFace->ResetContent();
	s = GetStringFromTable(iStrFace+iGender);
	vec = s.Split(_AL("\t"));
	for( i = 0; i < vec.size(); i++ )
		m_pComboFace->AddString(vec[i]);
	m_pComboFace->SetCurSel(pHost->FaceHairId2Index(pHost->GetFaceid()));
	
	m_pComboHair->ResetContent();
	s = GetStringFromTable(iStrHair+iGender);
	vec = s.Split(_AL("\t"));
	for( i = 0; i < vec.size(); i++ )
		m_pComboHair->AddString(vec[i]);
	m_pComboHair->SetCurSel(pHost->FaceHairId2Index(pHost->GetHairid()));
	
	m_pComboCloth->ResetContent();
	s = GetStringFromTable(iCloth+iGender);
	vec = s.Split(_AL("\t"));
	for( i = 0; i < vec.size(); i++ )
		m_pComboCloth->AddString(vec[i]);
	m_pComboCloth->SetCurSel(pHost->GetDefaultSkinid());
	
	int idProf = pHost->GetProfession();
	m_bEarTail = (idProf>=39 && idProf<=44);
	GetDlgItem("Rdo_1")->Show(m_bEarTail);
	GetDlgItem("Rdo_2")->Show(m_bEarTail);
	GetDlgItem("Rdo_3")->Show(m_bEarTail);
	GetDlgItem("Rdo_4")->Show(m_bEarTail);
	GetDlgItem("Btn_LeftEar")->Show(m_bEarTail);
	GetDlgItem("Btn_RightEar")->Show(m_bEarTail);
	GetDlgItem("Btn_LeftTail")->Show(m_bEarTail);
	GetDlgItem("Btn_RightTail")->Show(m_bEarTail);
	GetDlgItem("Edt_2")->Show(m_bEarTail);
	m_pComboEar->Show(m_bEarTail);
	m_pComboTail->Show(m_bEarTail);

	if (m_bEarTail)
	{
		CheckRadioButton(1, 1 + pHost->IsAdvancedEar());
		int iId;
		if (iGender==0)
			if (GetCheckedRadioButton(1)==1)
				iId = 8407;
			else
				iId = 8411;
		else
			if (GetCheckedRadioButton(1)==1)
				iId = 8406;
			else
				iId = 8410;
		s = GetStringFromTable(iId);
		vec = s.Split(_AL("\t"));
		m_pComboEar->ResetContent();
		for( i = 0; i < vec.size(); i++ )
			m_pComboEar->AddString(vec[i]);
		m_pComboEar->SetCurSel(pHost->GetEarid());
		
		CheckRadioButton(2, 1 + pHost->IsAdvancedTail());
		if (iGender==0)
			if (GetCheckedRadioButton(2)==1)
				iId = 8409;
			else
				iId = 8413;
		else
			if (GetCheckedRadioButton(2)==1)
				iId = 8408;
			else
				iId = 8412;
		s = GetStringFromTable(iId);
		vec = s.Split(_AL("\t"));
		m_pComboTail->ResetContent();
		for( i = 0; i < vec.size(); i++ )
			m_pComboTail->AddString(vec[i]);
		m_pComboTail->SetCurSel(pHost->GetTailid());
	}
}

void CDlgFaceOff::OnShowDialog()
{	
	CECHostPlayer* pHost = GetHostPlayer();	
	pHost->LoadHeadPortraitModel();
	InitStrings();
	ReLoadModel();

	pHost->SyncDemonstrationPlayer(m_pPlayer);
	if (m_bEarTail)
	{
		int iEar = (pHost->IsAdvancedEar()) ? (pHost->GetEarid()|0x00000080) : pHost->GetEarid();
		m_pPlayer->ChangeEar(iEar);
		int iTail = (pHost->IsAdvancedTail()) ? (pHost->GetTailid()|0x00000080) : pHost->GetTailid();
		m_pPlayer->ChangeTail(iTail);
	}
	m_nAngle = 0;

	int aEquips[SIZE_EQUIPIVTR];
	memset(aEquips, 0, sizeof(aEquips));
	m_pPlayer->ShowEquipments(aEquips, false, true);
	m_pPlayer->SetFashionMode(false);
}


void CDlgFaceOff::OnTick()
{
	if( m_pBtn_LeftTurn->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK )
		m_nAngle = (m_nAngle + 360 - GetGame()->GetTickTime() / 6 ) % 360;
	else if( m_pBtn_RightTurn->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK )
		m_nAngle = (m_nAngle + GetGame()->GetTickTime() / 6) % 360;
	
	if (GetCheckedRadioButton(3)==2 && m_iCameraWeight>0)
		m_iCameraWeight -= 30;
	else if (GetCheckedRadioButton(3)==1 && m_iCameraWeight<100)
		m_iCameraWeight += 30;

	if (m_pPlayer)
	{
		if (GetCheckedRadioButton(3)==2)
			m_pPlayer->Stand();
		else
			m_pPlayer->StandForCustomize();
		m_pPlayer->Tick(g_pGame->GetTickTime());

		m_pImg_Head->SetRenderCallback(	PlayerRenderDemonstration,
		(DWORD)m_pPlayer, m_nAngle, m_iCameraWeight);
	}
	else
		m_pImg_Head->SetRenderCallback(NULL, 0);

}

bool CDlgFaceOff::Render()
{
	return CDlgBase::Render();
}

void CDlgFaceOff::OnEventMouseMove(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if( (wParam & MK_LBUTTON) )
	{
		m_nMouseOffsetThis = GET_X_LPARAM(lParam) - m_nMouseLastX;
		if( m_nMouseOffset + m_nMouseOffsetThis > 70 )
			m_nMouseOffsetThis = 70 - m_nMouseOffset;
		else if( m_nMouseOffset + m_nMouseOffsetThis < -70 )
			m_nMouseOffsetThis = -70 - m_nMouseOffset;
	}
}


void CDlgFaceOff::OnEventLButtonDown_Size(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	SetCaptureObject(pObj);
	m_nMouseLastX = GET_X_LPARAM(lParam);
	m_nMouseOffset += m_nMouseOffsetThis;
	m_nMouseOffsetThis = 0;
}

void CDlgFaceOff::OnEventLButtonUp_Size(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	SetCaptureObject(NULL);
}

void CDlgFaceOff::OnCommandConfirm(const char* szCommand)
{
	const SPECIAL_ID_CONFIG& cfg = g_pGame->GetGameRun()->GetSpecialIDConfig();
	if (GetHostPlayer()->GetPack()->GetItemTotalNum(cfg.id_change_face_item) < 1)
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(8418),MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	}
	else
	{
		int iFace = GetHostPlayer()->FaceHairIndex2Id(m_pComboFace->GetCurSel());
		int iHair = GetHostPlayer()->FaceHairIndex2Id(m_pComboHair->GetCurSel());
		int iEar = 0;
		int iTail = 0;
		if (m_bEarTail)
		{
			iEar = m_pComboEar->GetCurSel();
			iEar = (GetCheckedRadioButton(1)==1) ? (iEar&0xFFFFFF7F) : (iEar|0x00000080);
			iTail = m_pComboTail->GetCurSel();
			iTail = (GetCheckedRadioButton(2)==1) ? (iTail&0xFFFFFF7F) : (iTail|0x00000080);
		}
		GetGameSession()->c2s_CmdNPCSevChangeFace(iFace, iHair,	iEar, iTail, m_pComboCloth->GetCurSel(), GetHostPlayer()->GetPack()->FindItem(cfg.id_change_face_item));
	}
}

void CDlgFaceOff::OnCommandHairLeft(const char* szCommand)
{
	int iCur = m_pComboHair->GetCurSel();
	iCur--;
	if (iCur<0)
	{
		iCur = m_pComboHair->GetCount() -1;
	}
	m_pComboHair->SetCurSel(iCur);
	m_pPlayer->ChangeHair(m_pPlayer->FaceHairIndex2Id(iCur));
}

void CDlgFaceOff::OnCommandHairRight(const char* szCommand)
{
	int iCur = m_pComboHair->GetCurSel();
	iCur++;
	if (iCur>=m_pComboHair->GetCount())
	{
		iCur = 0;
	}
	m_pComboHair->SetCurSel(iCur);
	m_pPlayer->ChangeHair(m_pPlayer->FaceHairIndex2Id(iCur));
}

void CDlgFaceOff::OnCommandHairChange(const char* szCommand)
{
	int iCur = m_pComboHair->GetCurSel();
	m_pPlayer->ChangeHair(m_pPlayer->FaceHairIndex2Id(iCur));
}

void CDlgFaceOff::OnCommandFaceLeft(const char* szCommand)
{
	int iCur = m_pComboFace->GetCurSel();
	iCur--;
	if (iCur<0)
	{
		iCur = m_pComboFace->GetCount() -1;
	}
	m_pComboFace->SetCurSel(iCur);
	m_pPlayer->ChangeFace(m_pPlayer->FaceHairIndex2Id(iCur));
}

void CDlgFaceOff::OnCommandFaceRight(const char* szCommand)
{
	int iCur = m_pComboFace->GetCurSel();
	iCur++;
	if (iCur>=m_pComboFace->GetCount())
	{
		iCur = 0;
	}
	m_pComboFace->SetCurSel(iCur);
	m_pPlayer->ChangeFace(m_pPlayer->FaceHairIndex2Id(iCur));
}

void CDlgFaceOff::OnCommandFaceChange(const char* szCommand)
{
	int iCur = m_pComboFace->GetCurSel();
	m_pPlayer->ChangeFace(m_pPlayer->FaceHairIndex2Id(iCur));
}

void CDlgFaceOff::OnCommandClothLeft(const char* szCommand)
{
	int iCur = m_pComboCloth->GetCurSel();
	iCur--;
	if (iCur<0)
	{
		iCur = m_pComboCloth->GetCount() -1;
	}
	m_pComboCloth->SetCurSel(iCur);
	m_pPlayer->ChangeDefaultSkin(iCur);
}

void CDlgFaceOff::OnCommandClothRight(const char* szCommand)
{
	int iCur = m_pComboCloth->GetCurSel();
	iCur++;
	if (iCur>=m_pComboCloth->GetCount())
	{
		iCur = 0;
	}
	m_pComboCloth->SetCurSel(iCur);
	m_pPlayer->ChangeDefaultSkin(iCur);
}

void CDlgFaceOff::OnCommandClothChange(const char* szCommand)
{
	int iCur = m_pComboCloth->GetCurSel();
	m_pPlayer->ChangeDefaultSkin(iCur);
}

void CDlgFaceOff::OnCommandEarLeft(const char* szCommand)
{
	int iCur = m_pComboEar->GetCurSel();
	iCur--;
	if (iCur<0)
	{
		iCur = m_pComboEar->GetCount() -1;
	}
	m_pComboEar->SetCurSel(iCur);
	int iEar = (GetCheckedRadioButton(1)==1) ? iCur : (iCur|0x00000080);
	m_pPlayer->ChangeEar(iEar);
}

void CDlgFaceOff::OnCommandEarRight(const char* szCommand)
{
	int iCur = m_pComboEar->GetCurSel();
	iCur++;
	if (iCur>=m_pComboEar->GetCount())
	{
		iCur = 0;
	}
	m_pComboEar->SetCurSel(iCur);
	int iEar = (GetCheckedRadioButton(1)==1) ? iCur : (iCur|0x00000080);
	m_pPlayer->ChangeEar(iEar);
}

void CDlgFaceOff::OnCommandEarChange(const char* szCommand)
{
	int iCur = m_pComboEar->GetCurSel();
	m_pPlayer->ChangeEar(iCur);
}

void CDlgFaceOff::OnCommandTailLeft(const char* szCommand)
{
	int iCur = m_pComboTail->GetCurSel();
	iCur--;
	if (iCur<0)
	{
		iCur = m_pComboTail->GetCount() -1;
	}
	m_pComboTail->SetCurSel(iCur);
	int iTail = (GetCheckedRadioButton(2)==1) ? iCur : (iCur|0x00000080);
	m_pPlayer->ChangeTail(iTail);
}

void CDlgFaceOff::OnCommandTailRight(const char* szCommand)
{
	int iCur = m_pComboTail->GetCurSel();
	iCur++;
	if (iCur>=m_pComboTail->GetCount())
	{
		iCur = 0;
	}
	m_pComboTail->SetCurSel(iCur);
	int iTail = (GetCheckedRadioButton(2)==1) ? iCur : (iCur|0x00000080);
	m_pPlayer->ChangeTail(iTail);
}

void CDlgFaceOff::OnCommandTailChange(const char* szCommand)
{
	int iCur = m_pComboTail->GetCurSel();
	m_pPlayer->ChangeTail(iCur);
}

void CDlgFaceOff::OnCommandZoom(const char* szCommand)
{
	if (GetCheckedRadioButton(3)==2)
	{
		m_pPlayer->Stand(true);
	}
}

void CDlgFaceOff::OnCommandType(const char* szCommand)
{
	int iEar = m_pComboEar->GetCurSel();
	int iTail = m_pComboTail->GetCurSel();
	int bGender = GetHostPlayer()->GetGender();
	int iId;
	DWORD i;
	
	if (bGender)
		if (GetCheckedRadioButton(1)==1)
			iId = 8407;
		else
			iId = 8411;
	else
		if (GetCheckedRadioButton(1)==1)
			iId = 8406;
		else
			iId = 8410;
	CSplit s = GetStringFromTable(iId);
	CSPLIT_STRING_VECTOR vec = s.Split(_AL("\t"));
	m_pComboEar->ResetContent();
	for( i = 0; i < vec.size(); i++ )
	{
		m_pComboEar->AddString(vec[i]);
	}
	if (bGender)
		if (GetCheckedRadioButton(2)==1)
			iId = 8409;
		else
			iId = 8413;
	else
		if (GetCheckedRadioButton(2)==1)
			iId = 8408;
		else
			iId = 8412;
	s = GetStringFromTable(iId);
	vec = s.Split(_AL("\t"));
	m_pComboTail->ResetContent();
	for( i = 0; i < vec.size(); i++ )
	{
		m_pComboTail->AddString(vec[i]);
	}
	CECHostPlayer* pHost = GetHostPlayer();
	m_pComboEar->SetCurSel(iEar);
	m_pComboTail->SetCurSel(iTail);
	iEar = (GetCheckedRadioButton(1)==1) ? iEar : (iEar|0x00000080);
	m_pPlayer->ChangeEar(iEar);
	iTail = (GetCheckedRadioButton(2)==1) ? iTail : (iTail|0x00000080);
	m_pPlayer->ChangeTail(iTail);
}

void CDlgFaceOff::OnCommandChange(const char* szCommand)
{
	GetHostPlayer()->ChangeHeadPortraitModel(m_pComboFace->GetCurSel(), m_pComboHair->GetCurSel());
}

void CDlgFaceOff::OnCommand_CANCEL(const char* szCommand)
{
	Show(false);
	GetHostPlayer()->EndNPCService();
}

void CDlgFaceOff::ReLoadModel()
{
	A3DRELEASE(m_pPlayer);
	m_pPlayer = new CECLoginPlayer(g_pGame->GetGameRun()->GetWorld()->GetPlayerMan());
	const GNET::RoleInfo& info = g_pGame->GetGameRun()->GetSelectedRoleInfo();
	
	if (!m_pPlayer->Load(CECPlayer::GetPhysiqueByProf(info.occupation), info.occupation, info.faceid, info.hairid, info.gender, NULL))
	{
		m_pPlayer->Release();
		delete m_pPlayer;
		m_pPlayer = NULL;
		AUI_ReportError(__LINE__, "face off player model load error");
		return;
	}
	else
	{
		m_pPlayer->SetPos(A3DVECTOR3(0));
		m_pPlayer->SetDirAndUp(A3DVECTOR3(0, 0, -1.0f), A3DVECTOR3(0, 1.0f, 0));
		m_pPlayer->GetPlayerModel()->SetAutoUpdateFlag(false);
	}
}