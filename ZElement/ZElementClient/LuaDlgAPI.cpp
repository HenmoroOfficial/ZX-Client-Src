/********************************************************************
  created:	   9/5/2006  
  filename:	   LuaDlgAPI.cpp
  author:      Wangkuiwu  
  description: 
  Copyright (c) , All Rights Reserved.
*********************************************************************/
#ifdef LUA_UI


#include "LuaDlgAPI.h"
#include "LuaUtil.h"
#include "LuaDlg.h"
#include "LuaManager.h"
#include "LuaScript.h"
#include "LuaState.h"
#include "AUI\\AUICheckBox.h"
#include "AUI\\AUITreeView.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUIProgress.h"
#include "AUI\\AUIComboBox.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIEditBox.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIModelPicture.h"
#include "AUI\\AUIRadioButton.h"
#include "AUI\\AUIScroll.h"
#include "AUI\\AUIManager.h"
#include "EC_Global.h"
#include "EC_LoginPlayer.h"
#include "EC_Game.h"
#include "EC_UIPlayer.h"

namespace LuaBind
{

int MessageBox(lua_State *L)
{
	AUIManager * pUIMan = static_cast<AUIManager *>(luaL_checklightudata(L, 1));
	const char * szName = luaL_checkstring(L, 2);
	const char * szText = luaL_checkstring(L, 3);
	DWORD dwType        = luaL_checkint(L, 4);
	int    r            = luaL_checkint(L, 5);
	int    g            = luaL_checkint(L, 6);
	int    b            = luaL_checkint(L, 7);
	int    a            = luaL_checkint(L, 8);

	bool   ret = pUIMan->MessageBox(szName, AS2AC(szText), dwType, A3DCOLORRGBA(r,g,b,a));

	lua_pushboolean(L, ret);
	return 1;
}


/**< aui object    */
int SetDlgItemText(lua_State * L)
{
	PAUIOBJECT pAObj = static_cast<PAUIOBJECT> (luaL_checklightudata(L, 1));
	assert(pAObj != NULL);
	const char * szText = luaL_checkstring(L, 2);
	pAObj->SetText(AS2AC(szText));
	return 0;
}

int GetDlgItemText(lua_State * L)
{
	PAUIOBJECT pAObj = static_cast<PAUIOBJECT> (luaL_checklightudata(L, 1));
	assert(pAObj != NULL);
	lua_pushstring(L, AC2AS(pAObj->GetText()));
	return 1;
}

int SetDlgItemColor(lua_State * L)
{
	PAUIOBJECT pAObj = static_cast<PAUIOBJECT> (luaL_checklightudata(L, 1));
	assert(pAObj != NULL);
	DWORD dwColor = static_cast<DWORD>(luaL_checkint(L, 2));
	pAObj->SetColor(dwColor);
	return 0;
}

int SetDlgItemAlign(lua_State * L)
{
	PAUIOBJECT pAObj = static_cast<PAUIOBJECT> (luaL_checklightudata(L, 1));
	assert(pAObj != NULL);
	int nAlign = luaL_checkint(L, 2);
	pAObj->SetAlign(nAlign);
	return 0;
}

/**
 * \brief get dlgitem state(checkbox, etc)
 * \param[in]
 * \param[out]
 * \return
 * \note  use rtti to check the given item
 * \warning
 * \todo   
 * \author kuiwu 
 * \date 21/4/2006
 * \see 
 */

int GetDlgItemState(lua_State * L)
{
	PAUIOBJECT pAObj  = static_cast<PAUIOBJECT> (luaL_checklightudata(L, 1));
	assert(pAObj != NULL);
	if (PAUICHECKBOX pChk = dynamic_cast<PAUICHECKBOX>(pAObj))
	{
		lua_pushboolean(L, pChk->IsChecked());
	}
	else
	{
		lua_pushinteger(L, pAObj->GetState());
	}

	return 1;	
}
/**
 * \brief set dlgitem state(checkbox, etc)
 * \param[in]
 * \param[out]
 * \return
 * \note  use rtti to check the given item
 * \warning
 * \todo   
 * \author kuiwu 
 * \date 21/4/2006
 * \see 
 */
int SetDlgItemState(lua_State * L)
{
	PAUIOBJECT  pAObj = static_cast<PAUIOBJECT > (luaL_checklightudata(L, 1));
	assert(pAObj != NULL);
	if (PAUICHECKBOX pChk = dynamic_cast<PAUICHECKBOX>(pAObj))
	{
		bool state = luaL_checkbool(L, 2);
		pChk->Check(state);
		return 0;
	}

	//char msg[200];
	//sprintf(msg, "SetDlgItemState: %s fail\n", pAObj->GetName());
	AString msg;
	msg.Format("SetDlgItemState: %s fail\n", pAObj->GetName());
	LUA_DEBUG_INFO(msg);
	return 0;
}


int SetItemProperty(lua_State * L)
{
	PAUIOBJECT  pAObj = static_cast<PAUIOBJECT > (luaL_checklightudata(L, 1));
	assert(pAObj != NULL);
	const char * szName = luaL_checkstring(L, 2);
	AUIObjectPropertyValue  type = GetAUIObjectPropertyType(szName);
	AUIOBJECT_SETPROPERTY   value;
	switch(type) 
	{
	case AUIOBJECT_PROPERTY_VALUE_BOOL:
		value.b = luaL_checkbool(L, 3);
		break;
	case AUIOBJECT_PROPERTY_VALUE_INT:
		value.i = luaL_checkint(L, 3);
		break;
	case AUIOBJECT_PROPERTY_VALUE_FLOAT:
		value.f = static_cast<float>(luaL_checknumber(L, 3));
		break;
	case AUIOBJECT_PROPERTY_VALUE_CHAR:
		strcpy(value.c, luaL_checkstring(L, 3));
		break;
	case AUIOBJECT_PROPERTY_VALUE_ACHAR:
		a_strcpy(value.s, AS2AC(luaL_checkstring(L, 3)));
		break;
	default:
		{
			//char msg[200];
			//sprintf(msg, "SetItemProperty %s: %s unkown type\n", pAObj->GetName(), szName);
			AString msg;
			msg.Format("SetItemProperty %s: %s unkown type\n", pAObj->GetName(), szName);
			LUA_DEBUG_INFO(msg);
			return 0;
		}
		
	}
	pAObj->SetProperty(szName, &value);
	return 0;
}

int EnableDlgItem(lua_State * L)
{
	PAUIOBJECT  pAObj = static_cast<PAUIOBJECT > (luaL_checklightudata(L, 1));
	assert(pAObj != NULL);
	bool state = luaL_checkbool(L, 2);
	pAObj->Enable(state);
	return 0;
}


int SetItemProgress(lua_State * L)
{
	PAUIOBJECT pAObj = static_cast<PAUIOBJECT> (luaL_checklightudata(L, 1));
	assert(pAObj != NULL);
	if (PAUIPROGRESS pProg = dynamic_cast<PAUIPROGRESS>(pAObj))
	{
		int nProg = luaL_checkint(L, 2);
		pProg->SetProgress(nProg);
	}

	return 0;
}

/**< dialog    */
int ShowDlg(lua_State * L)
{
	CLuaDlg * pDlg = static_cast<CLuaDlg *> (luaL_checklightudata(L, 1));
	assert(pDlg != NULL);
	bool bShow = luaL_checkbool(L, 2);
	pDlg->Show(bShow);

	return 0;
}

int IsDlgShow(lua_State * L)
{
	CLuaDlg * pDlg = static_cast<CLuaDlg *> (luaL_checklightudata(L, 1));
	assert(pDlg != NULL);
	bool bShow = pDlg->IsShow();
	lua_pushboolean(L, bShow);

	return 1;
}



int GetDlgItem(lua_State * L)
{
	CLuaDlg * pDlg = static_cast<CLuaDlg *> (luaL_checklightudata(L, 1));
	assert(pDlg != NULL);
	const char * szItem = luaL_checkstring(L, 2);
	PAUIOBJECT pAObj = pDlg->GetDlgItem(szItem);

	if (pAObj)
	{
		lua_pushlightuserdata(L, pAObj);
		return 1;
	}

	//char msg[200];
	//sprintf(msg, "GetDlgItem: %s fail\n", szItem);
	AString msg;
	msg.Format("GetDlgItem: %s fail\n", szItem);
	LUA_DEBUG_INFO(msg);
	return 0;

}

int AlignTo(lua_State * L)
{
	CLuaDlg * pDlg = static_cast<CLuaDlg *> (luaL_checklightudata(L, 1));
	assert(pDlg != NULL);
	int nInsideX = luaL_checkint(L, 2);
	int nAlignX =  luaL_checkint(L, 3);
	int nOffsetX = luaL_checkint(L, 4);
	int nInsideY = luaL_checkint(L, 5);
	int nAlignY =  luaL_checkint(L, 6);
	int nOffsetY = luaL_checkint(L, 7);
	AUIDialog * pBase = NULL;
	//if (Match(TypeWrapper<LuaLightUserData>(), L, 8))
	if (Match(LUA_TLIGHTUSERDATA, L, 8))
	{
		pBase = static_cast<AUIDialog *>(luaL_checklightudata(L, 8));
	}

	pDlg->AlignTo(pBase, nInsideX, nAlignX, nInsideY, nAlignY, nOffsetX, nOffsetY);

	return 0;
}

/**
 * \brief get the const string from current dlg
 * \param[in]  id, the string id
 * \param[out]
 * \return  the string
 * \note
 * \warning
 * \todo   
 * \author kuiwu 
 * \date 12/5/2006
 * \see 
 */
int GetStringFromTable(lua_State * L)
{
	CLuaDlg * pDlg = static_cast<CLuaDlg *>(luaL_checklightudata(L, 1));
	assert(pDlg != NULL);
	int id = luaL_checkint(L, 2);
	lua_pushstring(L, AC2AS(pDlg->GetStringFromTable(id)));
	return 1;
}


int SetCanOutOfWindow(lua_State * L)
{
	CLuaDlg * pDlg = static_cast<CLuaDlg *>(luaL_checklightudata(L, 1));
	assert(pDlg != NULL);
	bool bVal = luaL_checkbool(L, 2);
	pDlg->SetCanOutOfWindow(bVal);
	return 0;
}

int SetDlgPos(lua_State * L)
{
	CLuaDlg * pDlg = static_cast<CLuaDlg *>(luaL_checklightudata(L, 1));
	assert(pDlg != NULL);
	int x = luaL_checkint(L, 2);
	int y = luaL_checkint(L, 3);
	pDlg->SetPosEx(x, y);
	return 0;
}


/**< tree view    */
/**
 * \brief tree view insert item
 * \param[in]
 * \param[out]
 * \return
 * \note
 * \warning
 * \todo   
 * \author kuiwu 
 * \date 21/4/2006
 * \see 
 */
int TVInsertItem(lua_State * L)
{
	PAUITREEVIEW 	pTV = static_cast<PAUITREEVIEW>(luaL_checklightudata(L, 1));
	assert(pTV);
	const char * szText = luaL_checkstring(L, 2);
	P_AUITREEVIEW_ITEM   pParent = AUITREEVIEW_ROOT;
	if (lua_type(L,3) == LUA_TLIGHTUSERDATA )
	{
		 pParent =  static_cast<P_AUITREEVIEW_ITEM>(lua_touserdata(L, 3));
	}
	P_AUITREEVIEW_ITEM   pAfter = AUITREEVIEW_LAST;
	if (lua_type(L, 4) == LUA_TLIGHTUSERDATA)
	{
		pAfter = static_cast<P_AUITREEVIEW_ITEM>(lua_touserdata(L, 4));
	}
	P_AUITREEVIEW_ITEM pRT = pTV->InsertItem(AS2AC(szText), pParent, pAfter);
	
	lua_pushlightuserdata(L, pRT);
	
	return 1;
}

int TVGetSelItem(lua_State *L)
{
	PAUITREEVIEW 	pTV = static_cast<PAUITREEVIEW>(luaL_checklightudata(L, 1));
	assert(pTV);
	P_AUITREEVIEW_ITEM pItem = pTV->GetSelectedItem();
	if (pItem)
	{
		lua_pushlightuserdata(L, pItem);
		return 1;
	}

	return 0;
}

int TVGetItemText(lua_State * L)
{
	PAUITREEVIEW 	pTV = static_cast<PAUITREEVIEW>(luaL_checklightudata(L, 1));
	assert(pTV);
	P_AUITREEVIEW_ITEM pItem = static_cast<P_AUITREEVIEW_ITEM> (luaL_checklightudata(L, 2));
	assert(pItem);
	const ACHAR * szText = pTV->GetItemText(pItem);
	lua_pushstring(L, AC2AS(szText));
	return 1;
}

/**< list box    */
int LstResetContent(lua_State * L)
{
	PAUILISTBOX pLst = static_cast<PAUILISTBOX> (luaL_checklightudata(L, 1));
	assert(pLst);
	pLst->ResetContent();

	return 0;
}

int LstAddString(lua_State * L)
{
	PAUILISTBOX pLst = static_cast<PAUILISTBOX> (luaL_checklightudata(L, 1));
	assert(pLst);
	const char * szText = luaL_checkstring(L, 2);
	int index = pLst->AddString(AS2AC(szText));
	lua_pushinteger(L, index);

	return 1;
}

int LstSetItemData(lua_State * L)
{
	PAUILISTBOX pLst = static_cast<PAUILISTBOX> (luaL_checklightudata(L, 1));
	assert(pLst);
	
	int index = luaL_checkint(L,2);
	int dwData = static_cast<int>(luaL_checkint(L, 3));
	int subIndex = 0;
	//if (Match(TypeWrapper<int>(), L, 4))
	if (Match(LUA_TNUMBER, L, 4))
	{
		subIndex = static_cast<int>(lua_tonumber(L, 4));
	}

	const char * szName = "";
	//if (Match(TypeWrapper<const char *>(), L, 5))
	if (Match(LUA_TSTRING, L, 5))
	{
		szName = lua_tostring(L, 5);
	}
	pLst->SetItemData(index, dwData, subIndex, szName);

	return 0;
}

int LstGetItemData(lua_State * L)
{
	PAUILISTBOX pLst = static_cast<PAUILISTBOX> (luaL_checklightudata(L, 1));
	assert(pLst);

	int index = luaL_checkint(L,2);
	int subIndex = 0;
	//if (Match(TypeWrapper<int>(), L, 3))
	if (Match(LUA_TNUMBER, L, 3))
	{
		subIndex = static_cast<int>(lua_tonumber(L, 3));
	}

	const char * szName = "";
	//if (Match(TypeWrapper<const char *>(), L, 4))
	if (Match(LUA_TSTRING, L, 4))
	{
		szName = lua_tostring(L, 4);
	}

	DWORD dwData = pLst->GetItemData(index, subIndex, szName);

	lua_pushinteger(L, dwData);

	return 1;

}

int LstGetCurSel(lua_State * L)
{
	PAUILISTBOX pLst = static_cast<PAUILISTBOX> (luaL_checklightudata(L, 1));
	assert(pLst);
	int sel = pLst->GetCurSel();
	if (sel < 0 || sel >= pLst->GetCount())
	{
		return 0;
	}
	//++sel; //lua start from 1
	lua_pushinteger(L, sel);
	return 1;
}

int LstSetCurSel(lua_State * L)
{
	PAUILISTBOX pLst = static_cast<PAUILISTBOX> (luaL_checklightudata(L, 1));
	assert(pLst);
	int index = luaL_checkint(L, 2);
	if (index < 0 || index >= pLst->GetCount())
	{
		index = 0;
	}
	pLst->SetCurSel(index);
	return 0;
}

/**< combo box    */
int ComboAddString(lua_State * L)
{
	PAUICOMBOBOX pCombo = 	static_cast<PAUICOMBOBOX> (luaL_checklightudata(L, 1));
	assert(pCombo);
	const char * text = luaL_checkstring(L, 2);
	int size =	pCombo->AddString(AS2AC(text));
	lua_pushinteger(L, size);
	return 1;
}

int ComboGetCurSel(lua_State * L)
{
	PAUICOMBOBOX pCombo = 	static_cast<PAUICOMBOBOX> (luaL_checklightudata(L, 1));
	assert(pCombo);

	int sel = pCombo->GetCurSel();
	lua_pushinteger(L, sel);

	return 1;
}


/**< still image button    */
int BtnSetPushed(lua_State * L)
{
	PAUISTILLIMAGEBUTTON pBtn = static_cast<PAUISTILLIMAGEBUTTON> (luaL_checklightudata(L, 1));
	assert(pBtn != NULL);
	bool bPush = luaL_checkbool(L, 2);
	pBtn->SetPushed( bPush);
	return 0;
}

/**< edit box    */
int EdtSetNumberOnly(lua_State * L)
{
	PAUIEDITBOX pEdt = static_cast<PAUIEDITBOX> (luaL_checklightudata(L, 1));
	assert(pEdt != NULL);
	bool bNumber = luaL_checkbool(L, 2);
	pEdt->SetIsNumberOnly(bNumber);

	return 0;
}

/**< image picture    */
/**
 * \brief 
 * \param[in] pic, the picture control
 * \param[in] pPlayer, the demonstration player
 * \param[in] angle, the demonstration player  angle
 * \param[out]
 * \return
 * \note
 * \warning
 * \todo   
 * \author kuiwu 
 * \date 18/5/2006
 * \see 
 */
int PicRenderPlayerDemonstration(lua_State * L)
{
	PAUIIMAGEPICTURE pPic = static_cast<PAUIIMAGEPICTURE> (luaL_checklightudata(L, 1));
	assert(pPic != NULL);
	CECLoginPlayer * pPlayer = static_cast<CECLoginPlayer * > (luaL_checklightudata(L, 2));
	pPlayer->Stand();
	pPlayer->Tick(g_pGame->GetTickTime());
	int angle = luaL_checkint(L, 3);

	pPic->SetRenderCallback(PlayerRenderDemonstration, (DWORD)pPlayer, angle);

	return 0;

}

/**< auiplayer    */
int UIPLSetTemplate(lua_State *L)
{
	CECUIPlayer * pUIPlayer = static_cast<CECUIPlayer *> (luaL_checklightudata(L, 1));
	assert(pUIPlayer);
	int tmpId = luaL_checkint(L, 2);
	pUIPlayer->CreateDemoPlayer(tmpId);
	
	return 0;
}

int UIPLSetAngle(lua_State * L)
{
	CECUIPlayer * pUIPlayer = static_cast<CECUIPlayer *> (luaL_checklightudata(L, 1));
	assert(pUIPlayer);
	int angle = luaL_checkint(L, 2);
	pUIPlayer->SetAngle(angle);
	return 0;
}

int UIPLUpdateEquips(lua_State * L)
{
	CECUIPlayer * pUIPlayer = static_cast<CECUIPlayer *> (luaL_checklightudata(L, 1));
	assert(pUIPlayer);
	pUIPlayer->UpdateEquips();
	return 0;
	
}


void SetDlgConstant(lua_State * L)
{
	//win message
	SET_GLOBAL_CONST(L, WM_MOUSEMOVE);
	SET_GLOBAL_CONST(L,WM_LBUTTONDOWN);
	SET_GLOBAL_CONST(L,WM_LBUTTONUP);
	SET_GLOBAL_CONST(L,WM_LBUTTONDBLCLK);
	SET_GLOBAL_CONST(L,WM_RBUTTONDOWN);
	SET_GLOBAL_CONST(L,WM_RBUTTONUP);
	SET_GLOBAL_CONST(L,WM_RBUTTONDBLCLK);
	SET_GLOBAL_CONST(L,WM_MBUTTONDOWN);
	SET_GLOBAL_CONST(L,WM_MBUTTONUP);
	SET_GLOBAL_CONST(L,WM_MBUTTONDBLCLK);
	SET_GLOBAL_CONST(L,WM_MOUSEWHEEL);
	SET_GLOBAL_CONST(L,WM_KEYDOWN);
	SET_GLOBAL_CONST(L,WM_KEYUP);
	SET_GLOBAL_CONST(L,WM_CHAR);
	SET_GLOBAL_CONST(L,WM_SYSCHAR);
	//align
	SET_GLOBAL_CONST(L,AUIFRAME_ALIGN_LEFT);
	SET_GLOBAL_CONST(L,AUIFRAME_ALIGN_CENTER);
	SET_GLOBAL_CONST(L,AUIFRAME_ALIGN_RIGHT);
	SET_GLOBAL_CONST(L,AUIDIALOG_ALIGN_INSIDE);
	SET_GLOBAL_CONST(L,AUIDIALOG_ALIGN_OUTSIDE);
	SET_GLOBAL_CONST(L,AUIDIALOG_ALIGN_LEFT);
	SET_GLOBAL_CONST(L,AUIDIALOG_ALIGN_RIGHT);
	SET_GLOBAL_CONST(L,AUIDIALOG_ALIGN_TOP);
	SET_GLOBAL_CONST(L,AUIDIALOG_ALIGN_BOTTOM);
	SET_GLOBAL_CONST(L,AUIDIALOG_ALIGN_CENTER);

	//messagebox
	SET_GLOBAL_CONST(L,MB_OK);
	SET_GLOBAL_CONST(L,MB_OKCANCEL);
	SET_GLOBAL_CONST(L,MB_YESNO);
	SET_GLOBAL_CONST(L,IDYES);
	SET_GLOBAL_CONST(L,IDOK);
	//progress
	SET_GLOBAL_CONST(L,AUIPROGRESS_MAX);
	//imagebutton
	SET_GLOBAL_CONST(L,AUISTILLIMAGEBUTTON_STATE_CLICK);
}

////////////// dlg api table///////////////////////
const struct luaL_reg DlgApi[]	= 
{
	REG_API(MessageBox),
	REG_API(SetDlgItemText),
	REG_API(GetDlgItemText),
	REG_API(SetDlgItemAlign),
	REG_API(SetDlgItemColor),
	REG_API(EnableDlgItem),
	REG_API(ShowDlg),
	REG_API(AlignTo),
	REG_API(IsDlgShow),
	REG_API(GetDlgItemState),
	REG_API(SetDlgItemState),
	REG_API(GetDlgItem),
	REG_API(SetItemProperty),

	REG_API(TVInsertItem),
	REG_API(TVGetSelItem),
	REG_API(TVGetItemText),

	REG_API(LstResetContent),
	REG_API(LstAddString),
	REG_API(LstSetItemData),
	REG_API(LstGetItemData),
	REG_API(LstGetCurSel),
	REG_API(LstSetCurSel),
	
	REG_API(SetItemProgress),
	REG_API(ComboAddString),
	REG_API(ComboGetCurSel),
	REG_API(GetStringFromTable),
	REG_API(SetCanOutOfWindow),
	REG_API(BtnSetPushed),
	REG_API(EdtSetNumberOnly),
	REG_API(PicRenderPlayerDemonstration),
	
	REG_API(UIPLSetTemplate),
	REG_API(UIPLSetAngle),
	REG_API(UIPLUpdateEquips),
	{NULL, NULL}
};

void InitDlgApi(lua_State * L)
{
  //register dlg api
  luaL_register(L, "DlgApi", DlgApi);
  lua_pop(L, 1);   //pop the table DlgApi
  
  SetDlgConstant(L);

  AUIButton::RegisterPropertyType();
  AUICheckBox::RegisterPropertyType();
  AUIComboBox::RegisterPropertyType();
  AUIDialog::RegisterPropertyType();
  AUIEditBox::RegisterPropertyType();
  AUIImagePicture::RegisterPropertyType();
  AUILabel::RegisterPropertyType();
  AUIListBox::RegisterPropertyType();
  AUIModelPicture::RegisterPropertyType();
  AUIObject::RegisterPropertyType();
  AUIPicture::RegisterPropertyType();
  AUIProgress::RegisterPropertyType();
  AUIRadioButton::RegisterPropertyType();
  AUIScroll::RegisterPropertyType();
  AUISlider::RegisterPropertyType();
  AUIStillImageButton::RegisterPropertyType();
  AUITextArea::RegisterPropertyType();
  AUITreeView::RegisterPropertyType();

  CECUIPlayer::RegisterPropertyType();


}

bool PreLoadUI(CLuaState * pUIState)
{
	//preload

	// pay attention to the order  [7/11/2006 kuiwu]
	char *   preload_scripts[] = 
	{
		"Interfaces\\script\\Widget.lua",
		"Interfaces\\script\\DlgTemplate.lua",
		"Interfaces\\script\\Win_Target.lua",
		"Interfaces\\script\\AUIPlayer.lua",
		NULL
	};

	TLuaFile   * pLuaFile;
	int i = 0;
	while (preload_scripts[i])
	{
		CLuaScript * pScript = new CLuaScript(pUIState);
		pLuaFile = new TLuaFile(preload_scripts[i], pScript, true);
		if (!pLuaFile->Update(true))
		{
			assert(0);
			delete pLuaFile;
			//char msg[200];
			//sprintf(msg, "preload %s fail\n", preload_scripts[i]);
			AString msg;
			msg.Format("preload %s fail\n", preload_scripts[i]);
			LUA_DEBUG_INFO(msg);
			return false;
		}
		g_LuaFileMan.AddScript(pLuaFile);
		++i;
	}

/*
	/////////////////////////////////////////////////////////////////////////////
	//test
	CLuaScript * pTestScript = new CLuaScript(pUIState);
	pLuaFile = new TLuaFile("Interfaces\\script\\test_utf8_nb.lua", pTestScript);
	pLuaFile->Update(true);
	
	WCHAR szWStr[256];
	int len;
//  char * msg = "ÄãºÃ";
// 	len = MultiByteToWideChar(CP_ACP, 0, msg, -1,  szWStr, 256 );
// 	char utf8[256];
// 	len = WideCharToMultiByte(CP_UTF8, 0, szWStr, -1,  utf8, 256, NULL, NULL );
// 
// 
// 	lua_pushstring(pUIState->GetState(), utf8);

	Push(TypeWrapper<WCHAR*>(), pUIState->GetState(), L"ÄãºÃhello world!");

	Call(pUIState->GetState(), "test_utf8", "hello", 1, 1);
	
	len = luaL_checkstringW(pUIState->GetState(), -1, szWStr, 256);
	Pop(pUIState->GetState(), 1);

	delete pTestScript;
	delete pLuaFile;
	//////////////////////////////////////////////////////////////////////////
*/

	return  true;
}

}

#endif