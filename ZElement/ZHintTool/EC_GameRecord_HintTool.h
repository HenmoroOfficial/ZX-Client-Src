//
//
//
#ifndef _EC_GAMERECORD_H_
#define _EC_GAMERECORD_H_


//===============================================
//BE CAREFUL. It just used for compile, no other 
//usage.
void VerifyDefenceThread()
{
};
//===============================================

class game_record
{
public:

	enum
	{
		enum_gr_disable,
		enum_gr_record,
		enum_gr_replay,
	};

	enum
	{
		enum_fade_none,
		enum_fade_in,
		enum_fade_out,
	};

	game_record():state(0)
	{

	};

	~game_record() { release(); }

	int get_state() const { return state; }	

	void release()
	{
		state = 0;
	};

protected:
	int state;
};

#endif //(_EC_GAMERECORD_H_)