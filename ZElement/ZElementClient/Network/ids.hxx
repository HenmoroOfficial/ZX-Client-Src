#ifndef __GNET_IDS_H
#define	__GNET_IDS_H

namespace GNET
{
	
	enum Roles {
		_R_UNMEMBER=0,
		_R_SYSTEM=1,
		_R_MASTER=2,
		_R_VICEMASTER=3,
		_R_BODYGUARD=4,
		_R_HEADER=5,
		_R_MEMBER=6,
	}; //end of Roles
	
	enum Operations {
		_O_FACTION_TESTSYNC=0,
		_O_FACTION_CREATE=1,
		_O_FACTION_CHANGEPROCLAIM=2,
		_O_FACTION_ACCEPTJOIN=3,
		_O_FACTION_EXPELMEMBER=4,
		_O_FACTION_APPOINT=5,
		_O_FACTION_MASTERRESIGN=6,
		_O_FACTION_RESIGN=7,
		_O_FACTION_LEAVE=8,
		_O_FACTION_BROADCAST=9,
		_O_FACTION_DISMISS=10,
		_O_FACTION_UPGRADE=11,
		_O_FACTION_DEGRADE=12,
        _O_FACTION_LISTMEMBER = 13,
        _O_FACTION_RENAME = 14,
	}; //end of Operations
	
}; //end of namespace	

#endif
