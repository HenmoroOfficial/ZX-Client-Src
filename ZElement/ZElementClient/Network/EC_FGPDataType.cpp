/*
 * FILE: EC_FGPDataType.cpp
 *
 * DESCRIPTION: Fun game command types
 *
 * CREATED BY: Wangrui, 2009/3/11
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_FGPDataType.h"
#include "EC_RTDebug.h"

#define new A_DEBUG_NEW

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

#define CATCH_FILLDATA_EXP(fn) \
	catch (CECException& e) \
	{ \
		ASSERT(0); \
		a_LogOutput(1, "%s::FillData error (%d)", #fn, e.GetType()); \
		return false; \
	}

///////////////////////////////////////////////////////////////////////////
//	
//	Reference to External variables and functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Local Types and Variables and Global variables
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Implement
//	
///////////////////////////////////////////////////////////////////////////

bool FG_S2C::CMD::fgcmd_get_room_list::FillData(const void* pData, int iBufLen, int* piRealLen/* NULL */)
{
	using namespace FG_S2C::INFO;

	memset(this, 0, sizeof(*this));

	try
	{
		CECDataReader dr((void*)pData, iBufLen);
		
		room_size = dr.Read_char();

		int iRemainLen = iBufLen - dr.GetPos();

		if (room_size)
		{
			if (!(data = new room_info[room_size]))
				return false;

			int i, iReadLen;
			void* pReadData = dr.GetCurAddr();

			for (i=0; i<room_size; i++)
			{
				if (!data[i].FillData(pReadData, iRemainLen, &iReadLen))
				{
					delete [] data;
					data = NULL;
					return false;
				}

				pReadData = dr.Offset(iReadLen, SEEK_CUR);
				iRemainLen -= iReadLen;
			}
		}

		if (piRealLen)
			*piRealLen = dr.GetPos();
	}
	CATCH_FILLDATA_EXP(fgcmd_get_room_list)

	return true;
}

bool FG_S2C::CMD::fgcmd_notify_room_info::FillData(const void* pData, int iBufLen, int* piRealLen/* NULL */)
{
	return data.FillData(pData, iBufLen, piRealLen);
}

bool FG_S2C::CMD::fgcmd_player_info::FillData(const void* pData, int iBufLen, int* piRealLen/* =NULL */)
{
	return data.FillData(pData, iBufLen, piRealLen);
}

bool FG_S2C::CMD::fgcmd_player_enter_room::FillData(const void* pData, int iBufLen, int* piRealLen/* =NULL */)
{
	memset(this, 0, sizeof(*this));

	try
	{
		CECDataReader dr((void*)pData, iBufLen);

		roomid = dr.Read_short();

		int iRemainLen = iBufLen - dr.GetPos();
		void* pReadData = dr.GetCurAddr();
		int iReadLen;

		if (!data.FillData(pReadData, iRemainLen, &iReadLen))
			return false;

		if (piRealLen)
			*piRealLen = dr.GetPos();
	}
	CATCH_FILLDATA_EXP(fgcmd_player_enter_room)

	return true;
}

bool FG_S2C::CMD::fgcmd_room_dispatch_normal_card::FillData(const void* pData, int iBufLen, int* piRealLen/* NULL */)
{
	memset(this, 0, sizeof(*this));

	try
	{
		CECDataReader dr((void*)pData, iBufLen);

		roomid = dr.Read_short();
		roleid = dr.Read_int();
		next_roleid = dr.Read_int();

		int iRemainLen = iBufLen - dr.GetPos();
		void* pReadData = dr.GetCurAddr();
		int iReadLen;

		if (!cards.FillData(pReadData, iRemainLen, &iReadLen))
			return false;

		if (piRealLen)
			*piRealLen = dr.GetPos();
	}
	CATCH_FILLDATA_EXP(fgcmd_room_dispatch_normal_card)

	return true;
}

bool FG_S2C::CMD::fgcmd_player_throw_normal_card::FillData(const void* pData, int iBufLen, int* piRealLen/* NULL */)
{
	memset(this, 0, sizeof(*this));

	try
	{
		CECDataReader dr((void*)pData, iBufLen);

		roomid = dr.Read_short();
		roleid = dr.Read_int();
		next_roleid = dr.Read_int();

		int iRemainLen = iBufLen - dr.GetPos();
		void* pReadData = dr.GetCurAddr();
		int iReadLen;

		if (!cards.FillData(pReadData, iRemainLen, &iReadLen))
			return false;

		if (piRealLen)
			*piRealLen = dr.GetPos();
	}
	CATCH_FILLDATA_EXP(fgcmd_player_throw_normal_card)

	return true;
}

bool FG_S2C::CMD::fgcmd_player_pickup_normal_card::FillData(const void* pData, int iBufLen, int* piRealLen/* NULL */)
{
	memset(this, 0, sizeof(*this));

	try
	{
		CECDataReader dr((void*)pData, iBufLen);

		roomid = dr.Read_short();
		roleid = dr.Read_int();

		int iRemainLen = iBufLen - dr.GetPos();
		void* pReadData = dr.GetCurAddr();
		int iReadLen;

		if (!cards.FillData(pReadData, iRemainLen, &iReadLen))
			return false;

		if (piRealLen)
			*piRealLen = dr.GetPos();
	}
	CATCH_FILLDATA_EXP(fgcmd_player_throw_normal_card)

	return true;
}

bool FG_S2C::CMD::fgcmd_landlord_result_broadcast::FillData(const void* pData, int iBufLen, int* piRealLen/* =NULL */)
{
	using namespace FG_S2C::INFO;

	memset(this, 0, sizeof(*this));

	try
	{
		CECDataReader dr((void*)pData, iBufLen);
		
		roomid = dr.Read_short();
		size = dr.Read_BYTE();

		if (size)
		{
			if (!(results = new landlord_result_info[size]))
				return false;

			const landlord_result_info* pTmp = (landlord_result_info*)dr.Read_Data(size * sizeof(landlord_result_info));
			memcpy(results, pTmp, size * sizeof(landlord_result_info));
		}

		if (piRealLen)
			*piRealLen = dr.GetPos();
	}
	CATCH_FILLDATA_EXP(fgcmd_landlord_result_broadcast)

	return true;
}

bool FG_S2C::CMD::fgcmd_landlord_reonline_datasend::FillData(const void* pData, int iBufLen, int* piRealLen/* =NULL */)
{
	memset(this, 0, sizeof(*this));

	try
	{
		CECDataReader dr((void*)pData, iBufLen);

		roomid = dr.Read_short();
	
		int iRemainLen = iBufLen - dr.GetPos();
		void* pReadData = dr.GetCurAddr();
		int iReadLen;

		if (!info.FillData(pReadData, iRemainLen, &iReadLen))
			return false;

		if (piRealLen)
			*piRealLen = dr.GetPos();
	}
	CATCH_FILLDATA_EXP(fgcmd_landlord_reonline_datasend)

	return true;
}

bool FG_S2C::CMD::fgcmd_player_enter_hall::FillData(const void* pData, int iBufLen, int* piRealLen/* =NULL */)
{
	return data.FillData(pData, iBufLen, piRealLen);
}

bool FG_S2C::CMD::fgcmd_notify_hall_members::FillData(const void* pData, int iBufLen, int* piRealLen/* =NULL */)
{
	using namespace FG_S2C::INFO;

	memset(this, 0, sizeof(*this));
	
	try
	{
		CECDataReader dr((void*)pData, iBufLen);
		
		member_num = dr.Read_int();
		
		int iRemainLen = iBufLen - dr.GetPos();
		
		if (member_num)
		{
			if (!(players = new player_base_info[member_num]))
				return false;
			
			int i, iReadLen;
			void* pReadData = dr.GetCurAddr();
			
			for (i=0; i<member_num; i++)
			{
				if (!players[i].FillData(pReadData, iRemainLen, &iReadLen))
				{
					delete [] players;
					players = NULL;
					return false;
				}
				
				pReadData = dr.Offset(iReadLen, SEEK_CUR);
				iRemainLen -= iReadLen;
			}
		}
		
		if (piRealLen)
			*piRealLen = dr.GetPos();
	}
	CATCH_FILLDATA_EXP(fgcmd_notify_hall_members)
		
	return true;
}

bool FG_S2C::INFO::player_base_info::FillData(const void* pData, int iBufLen, int* piRealLen/* =NULL */)
{
	memset(this, 0, sizeof(*this));

	try
	{
		CECDataReader dr((void*)pData, iBufLen);
		
		roleid = dr.Read_int();
		direction = dr.Read_BYTE();
		state = dr.Read_BYTE();
		gender = dr.Read_BYTE();
		score = dr.Read_INT64();
		win_count = dr.Read_int();
		lose_count = dr.Read_int();
		draw_count = dr.Read_int();
		escape_count = dr.Read_int();
		name_size = dr.Read_BYTE();
		name = name_size ? (char*)dr.Read_Data(name_size * sizeof(char)) : NULL; 

		if (piRealLen)
			*piRealLen = dr.GetPos();
	}
	CATCH_FILLDATA_EXP(player_base_info)

	return true;
}

bool FG_S2C::INFO::room_info::FillData(const void* pData, int iBufLen, int* piRealLen/* NULL */)
{
	memset(this, 0, sizeof(*this));

	try
	{
		CECDataReader dr((void*)pData, iBufLen);

		roomid = dr.Read_short();
		player_size = dr.Read_char();

		int iRemainLen = iBufLen - dr.GetPos();

		if (player_size)
		{
			if (!(players = new player_base_info[player_size]))
				return false;

			int i, iReadLen;
			void* pReadData = dr.GetCurAddr();

			for (i=0; i<player_size; i++)
			{
				if (!players[i].FillData(pReadData, iRemainLen, &iReadLen))
				{
					delete [] players;
					players = NULL;
					return false;
				}

				pReadData = dr.Offset(iReadLen, SEEK_CUR);
				iRemainLen -= iReadLen;
			}
		}

		if (piRealLen)
			*piRealLen = dr.GetPos();
	}
	CATCH_FILLDATA_EXP(room_info)

	return true;
}

bool FG_S2C::INFO::normal_card_info::FillData(const void* pData, int iBufLen, int* piRealLen/* NULL */)
{
	memset(this, 0, sizeof(*this));

	try
	{
		CECDataReader dr((void*)pData, iBufLen);

		card_size = dr.Read_char();
		
		if (card_size)
		{
			if (!(cards = new char[card_size]))
				return false;

			const char* pTmp = (char*)dr.Read_Data(card_size * sizeof(char));
			memcpy(cards, pTmp, card_size * sizeof(char));
		}

		if (piRealLen)
			*piRealLen = dr.GetPos();
	}
	CATCH_FILLDATA_EXP(normal_card_info)

	return true;
}

bool FG_S2C::INFO::landlord_reonline_info::FillData(const void* pData, int iBufLen, int* piRealLen/* =NULL */)
{
	memset(this, 0, sizeof(*this));

	try
	{
		CECDataReader dr((void*)pData, iBufLen);
		
		int iRemineLen, iReadLen;
		if (!cur_room_info.FillData(pData, iBufLen, &iReadLen))
			return false;

		iRemineLen = iBufLen - dr.GetPos();

		score = dr.Read_int();
		lord_dir = dr.Read_BYTE();
		desk_dir = dr.Read_BYTE();
		next_dir = dr.Read_BYTE();
		cards[0].direct = dr.Read_BYTE();
		cards[0].card_size = dr.Read_BYTE();
		cards[1].direct = dr.Read_BYTE();
		cards[1].card_size = dr.Read_BYTE();
		
		self_card_size = dr.Read_BYTE();
		if (self_card_size)
		{
			if (!(self_cards = new char[self_card_size]))
				return false;

			const char* pTmp = (char*)dr.Read_Data(self_card_size * sizeof(char));
			memcpy(self_cards, pTmp, self_card_size * sizeof(char));
		}

		desk_card_size = dr.Read_BYTE();
		if (desk_card_size)
		{
			if (!(desk_cards = new char[desk_card_size]))
				return false;

			const char* pTmp = (char*)dr.Read_Data(desk_card_size * sizeof(char));
			memcpy(desk_cards, pTmp, desk_card_size * sizeof(char));
		}

		if (piRealLen)
			*piRealLen = dr.GetPos();
	}
	CATCH_FILLDATA_EXP(landlord_reonline_info)

	return true;	
}