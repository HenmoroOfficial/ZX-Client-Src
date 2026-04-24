#ifndef __GNET_FACTIONDATA_H
#define __GNET_FACTIONDATA_H
#include "gnmarshal.h"
namespace GNET
{	
	struct testsync_param_ct
	{
		int padding;
		testsync_param_ct(int _padding=0) : padding(_padding) { }
		testsync_param_ct& Create(const Marshal::OctetsStream& os) 
		{
			os >> padding;
			return *this;
		}
		Marshal::OctetsStream marshal()
		{
			return Marshal::OctetsStream()<<padding;
		}
	};
	struct testsync_param_st
	{
		int padding;
		testsync_param_st(int _padding=0) : padding(_padding) { }
		testsync_param_st(const testsync_param_ct& _ct){
			padding=_ct.padding;
		}
		void Create(const Marshal::OctetsStream& os) 
		{
			os >> padding;
		}
		Marshal::OctetsStream marshal()
		{
			return Marshal::OctetsStream()<<padding;
		}
	};
	struct create_param_ct
	{
		Octets faction_name;
		Octets proclaim;
		create_param_ct(const Octets& _faction_name=Octets(),const Octets& _proclaim=Octets()) : faction_name(_faction_name),proclaim(_proclaim) { }
		create_param_ct& Create(const Marshal::OctetsStream& os) 
		{
			os >> faction_name;
			os >> proclaim;
			return *this;
		}
		Marshal::OctetsStream marshal()
		{
			return Marshal::OctetsStream()<<faction_name<<proclaim;
		}
	};
	struct create_param_st
	{
		int level;
		unsigned int money;
		int sp;
		Octets faction_name;
		Octets proclaim;
		create_param_st(int _level=0,unsigned int _money=0,int _sp=0,const Octets& _faction_name=Octets(),const Octets& _proclaim=Octets()) : level(_level),money(_money),sp(_sp),faction_name(_faction_name),proclaim(_proclaim) { }
		create_param_st(int _level,unsigned int _money,int _sp,const create_param_ct& _ct) : level(_level),money(_money),sp(_sp){
			faction_name=_ct.faction_name;
			proclaim=_ct.proclaim;
		}
		void Create(const Marshal::OctetsStream& os) 
		{
			os >> level;
			os >> money;
			os >> sp;
			os >> faction_name;
			os >> proclaim;
		}
		Marshal::OctetsStream marshal()
		{
			return Marshal::OctetsStream()<<level<<money<<sp<<faction_name<<proclaim;
		}
	};
	struct changeproclaim_param_ct
	{
		Octets proclaim;
		changeproclaim_param_ct(const Octets& _proclaim=Octets()) : proclaim(_proclaim) { }
		changeproclaim_param_ct& Create(const Marshal::OctetsStream& os) 
		{
			os >> proclaim;
			return *this;
		}
		Marshal::OctetsStream marshal()
		{
			return Marshal::OctetsStream()<<proclaim;
		}
	};
	struct changeproclaim_param_st
	{
		Octets proclaim;
		changeproclaim_param_st(const Octets& _proclaim=Octets()) : proclaim(_proclaim) { }
		changeproclaim_param_st(const changeproclaim_param_ct& _ct){
			proclaim=_ct.proclaim;
		}
		void Create(const Marshal::OctetsStream& os) 
		{
			os >> proclaim;
		}
		Marshal::OctetsStream marshal()
		{
			return Marshal::OctetsStream()<<proclaim;
		}
	};
	struct acceptjoin_param_ct
	{
		int applicant;
		char blAgree;
		acceptjoin_param_ct(int _applicant=-1,char _blAgree=0) : applicant(_applicant),blAgree(_blAgree) { }
		acceptjoin_param_ct& Create(const Marshal::OctetsStream& os) 
		{
			os >> applicant;
			os >> blAgree;
			return *this;
		}
		Marshal::OctetsStream marshal()
		{
			return Marshal::OctetsStream()<<applicant<<blAgree;
		}
	};
	struct acceptjoin_param_st
	{
		int applicant;
		char blAgree;
		acceptjoin_param_st(int _applicant=-1,char _blAgree=0) : applicant(_applicant),blAgree(_blAgree) { }
		acceptjoin_param_st(const acceptjoin_param_ct& _ct){
			applicant=_ct.applicant;
			blAgree=_ct.blAgree;
		}
		void Create(const Marshal::OctetsStream& os) 
		{
			os >> applicant;
			os >> blAgree;
		}
		Marshal::OctetsStream marshal()
		{
			return Marshal::OctetsStream()<<applicant<<blAgree;
		}
	};
	struct expelmember_param_ct
	{
		int memberid;
		expelmember_param_ct(int _memberid=-1) : memberid(_memberid) { }
		expelmember_param_ct& Create(const Marshal::OctetsStream& os) 
		{
			os >> memberid;
			return *this;
		}
		Marshal::OctetsStream marshal()
		{
			return Marshal::OctetsStream()<<memberid;
		}
	};
	struct expelmember_param_st
	{
		int memberid;
		expelmember_param_st(int _memberid=-1) : memberid(_memberid) { }
		expelmember_param_st(const expelmember_param_ct& _ct){
			memberid=_ct.memberid;
		}
		void Create(const Marshal::OctetsStream& os) 
		{
			os >> memberid;
		}
		Marshal::OctetsStream marshal()
		{
			return Marshal::OctetsStream()<<memberid;
		}
	};
	struct appoint_param_ct
	{
		int memberid;
		char newoccup;
		appoint_param_ct(int _memberid=-1,char _newoccup=-1) : memberid(_memberid),newoccup(_newoccup) { }
		appoint_param_ct& Create(const Marshal::OctetsStream& os) 
		{
			os >> memberid;
			os >> newoccup;
			return *this;
		}
		Marshal::OctetsStream marshal()
		{
			return Marshal::OctetsStream()<<memberid<<newoccup;
		}
	};
	struct appoint_param_st
	{
		int memberid;
		char newoccup;
		appoint_param_st(int _memberid=-1,char _newoccup=-1) : memberid(_memberid),newoccup(_newoccup) { }
		appoint_param_st(const appoint_param_ct& _ct){
			memberid=_ct.memberid;
			newoccup=_ct.newoccup;
		}
		void Create(const Marshal::OctetsStream& os) 
		{
			os >> memberid;
			os >> newoccup;
		}
		Marshal::OctetsStream marshal()
		{
			return Marshal::OctetsStream()<<memberid<<newoccup;
		}
	};
	struct masterresign_param_ct
	{
		int newmaster;
		masterresign_param_ct(int _newmaster=-1) : newmaster(_newmaster) { }
		masterresign_param_ct& Create(const Marshal::OctetsStream& os) 
		{
			os >> newmaster;
			return *this;
		}
		Marshal::OctetsStream marshal()
		{
			return Marshal::OctetsStream()<<newmaster;
		}
	};
	struct masterresign_param_st
	{
		int newmaster;
		masterresign_param_st(int _newmaster=-1) : newmaster(_newmaster) { }
		masterresign_param_st(const masterresign_param_ct& _ct){
			newmaster=_ct.newmaster;
		}
		void Create(const Marshal::OctetsStream& os) 
		{
			os >> newmaster;
		}
		Marshal::OctetsStream marshal()
		{
			return Marshal::OctetsStream()<<newmaster;
		}
	};
	struct resign_param_ct
	{
		int padding;
		resign_param_ct(int _padding=0) : padding(_padding) { }
		resign_param_ct& Create(const Marshal::OctetsStream& os) 
		{
			os >> padding;
			return *this;
		}
		Marshal::OctetsStream marshal()
		{
			return Marshal::OctetsStream()<<padding;
		}
	};
	struct resign_param_st
	{
		int padding;
		resign_param_st(int _padding=0) : padding(_padding) { }
		resign_param_st(const resign_param_ct& _ct){
			padding=_ct.padding;
		}
		void Create(const Marshal::OctetsStream& os) 
		{
			os >> padding;
		}
		Marshal::OctetsStream marshal()
		{
			return Marshal::OctetsStream()<<padding;
		}
	};
	struct leave_param_ct
	{
		char paddings;
		leave_param_ct(char _paddings=0) : paddings(_paddings) { }
		leave_param_ct& Create(const Marshal::OctetsStream& os) 
		{
			os >> paddings;
			return *this;
		}
		Marshal::OctetsStream marshal()
		{
			return Marshal::OctetsStream()<<paddings;
		}
	};
	struct leave_param_st
	{
		char paddings;
		leave_param_st(char _paddings=0) : paddings(_paddings) { }
		leave_param_st(const leave_param_ct& _ct){
			paddings=_ct.paddings;
		}
		void Create(const Marshal::OctetsStream& os) 
		{
			os >> paddings;
		}
		Marshal::OctetsStream marshal()
		{
			return Marshal::OctetsStream()<<paddings;
		}
	};
	struct broadcast_param_ct
	{
		Octets msg;
		broadcast_param_ct(const Octets& _msg=Octets()) : msg(_msg) { }
		broadcast_param_ct& Create(const Marshal::OctetsStream& os) 
		{
			os >> msg;
			return *this;
		}
		Marshal::OctetsStream marshal()
		{
			return Marshal::OctetsStream()<<msg;
		}
	};
	struct broadcast_param_st
	{
		Octets msg;
		broadcast_param_st(const Octets& _msg=Octets()) : msg(_msg) { }
		broadcast_param_st(const broadcast_param_ct& _ct){
			msg=_ct.msg;
		}
		void Create(const Marshal::OctetsStream& os) 
		{
			os >> msg;
		}
		Marshal::OctetsStream marshal()
		{
			return Marshal::OctetsStream()<<msg;
		}
	};
	struct dismiss_param_ct
	{
		int padding;
		dismiss_param_ct(int _padding=0) : padding(_padding) { }
		dismiss_param_ct& Create(const Marshal::OctetsStream& os) 
		{
			os >> padding;
			return *this;
		}
		Marshal::OctetsStream marshal()
		{
			return Marshal::OctetsStream()<<padding;
		}
	};
	struct dismiss_param_st
	{
		int padding;
		dismiss_param_st(int _padding=0) : padding(_padding) { }
		dismiss_param_st(const dismiss_param_ct& _ct){
			padding=_ct.padding;
		}
		void Create(const Marshal::OctetsStream& os) 
		{
			os >> padding;
		}
		Marshal::OctetsStream marshal()
		{
			return Marshal::OctetsStream()<<padding;
		}
	};
	struct upgrade_param_ct
	{
		int padding;
		upgrade_param_ct(int _padding=0) : padding(_padding) { }
		upgrade_param_ct& Create(const Marshal::OctetsStream& os) 
		{
			os >> padding;
			return *this;
		}
		Marshal::OctetsStream marshal()
		{
			return Marshal::OctetsStream()<<padding;
		}
	};
	struct upgrade_param_st
	{
		int padding;
		upgrade_param_st(int _padding=0) : padding(_padding) { }
		upgrade_param_st(const upgrade_param_ct& _ct){
			padding=_ct.padding;
		}
		void Create(const Marshal::OctetsStream& os) 
		{
			os >> padding;
		}
		Marshal::OctetsStream marshal()
		{
			return Marshal::OctetsStream()<<padding;
		}
	};
	struct degrade_param_ct
	{
		int padding;
		degrade_param_ct(int _padding=0) : padding(_padding) { }
		degrade_param_ct& Create(const Marshal::OctetsStream& os) 
		{
			os >> padding;
			return *this;
		}
		Marshal::OctetsStream marshal()
		{
			return Marshal::OctetsStream()<<padding;
		}
	};
	struct degrade_param_st
	{
		int padding;
		degrade_param_st(int _padding=0) : padding(_padding) { }
		degrade_param_st(const degrade_param_ct& _ct){
			padding=_ct.padding;
		}
		void Create(const Marshal::OctetsStream& os) 
		{
			os >> padding;
		}
		Marshal::OctetsStream marshal()
		{
			return Marshal::OctetsStream()<<padding;
		}
	};
    struct listmember_param_ct
    {       
        int handle;
        listmember_param_ct(int _handle=-1) : handle(_handle) { }
        listmember_param_ct& Create(const Marshal::OctetsStream& os)
        {   
            os >> handle;
            return *this;
        }
        Marshal::OctetsStream marshal()
        {
            return Marshal::OctetsStream()<<handle;
        }
    };      
    struct rename_param_ct
    {   
        int dst_roleid;
        Octets new_name;
        rename_param_ct(int _dst_roleid=-1,Octets _new_name=Octets()) : dst_roleid(_dst_roleid),new_name(_new_name) { }
        rename_param_ct& Create(const Marshal::OctetsStream& os)
        {
            os >> dst_roleid;
            os >> new_name;
            return *this;
        }   
        Marshal::OctetsStream marshal()
        {
            return Marshal::OctetsStream()<<dst_roleid<<new_name;
        }
    };  
}; //end of namespace
#endif	
