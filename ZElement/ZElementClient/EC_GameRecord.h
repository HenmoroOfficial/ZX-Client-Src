#ifndef _EC_GAMERECORD_H_
#define _EC_GAMERECORD_H_

#include "vector.h"
#include "hashmap.h"

#include "EC_Global.h"
#include "gameclient.h"
#include "EC_MsgDef.h"

#define EVENT_TYPE_PROTOCOL			1
#define EVENT_TYPE_MSG				2
#define EVENT_TYPE_DATA				3

#define RECORD_NAME_LEN				30
#define RECORD_COMPRESS_SIZE		8192
#define RECORD_TITLE_LEN			30
#define RECORD_MAX_KP_COUNT			1000
#define	RECORD_KEY_POINT_INTERVAL	60000
#define CHECK_RECORD_INTERVAL		30000
#define	RECORD_MAX_EDIT_COUNT		50

#define RECORD_DATA_PLAYER_CACHE	0
#define RECORD_DATA_TEAM			1
#define RECORD_DATA_SKILL_ADDON		2
#define RECORD_DATA_FACTION			3
#define RECORD_DATA_EXTRA_DATA		4
#define RECORD_DATA_CHAT_MSG		5
#define RECORD_DATA_AUTOMOVE		6

// info to mask
#define REPLAY_PRIVATE_CHAT			0
#define REPLAY_WORLD_CHAT			1
#define REPLAY_COMMON_CHAT			2
#define REPLAY_INVENTORY			3
#define	REPLAY_SKILL				4
#define REPLAY_SELF_INFO			5


template<class T>
inline void add_to_octets(GNET::Octets& o, T d)
{
	o.insert(o.end(), &d, sizeof(T));
}

#pragma pack(1)

union game_record_header
{
	char				data[512];

	struct
	{
		// size 114
		int				record_version;
		int				game_version;
		int				char_id;
		unsigned char	gender;
		unsigned char	faceid;
		unsigned char	hairid;
		unsigned char   earid;
		unsigned char	tailid;
		unsigned char	defaultskin;
		unsigned char   occupation;
		unsigned char	name_len;
		char			name[RECORD_NAME_LEN];
		unsigned int	replay_setting;
		wchar_t			title[RECORD_TITLE_LEN];
		long			save_time;
		// size 201
		unsigned char	edit_point_count;
		unsigned short	edit_point_indice[RECORD_MAX_EDIT_COUNT];
		unsigned char	edit_point_prev_offset[RECORD_MAX_EDIT_COUNT];
		unsigned char	edit_point_post_offset[RECORD_MAX_EDIT_COUNT];
	};
};

union game_record_tracker
{
	char				data[4096];

	struct
	{
		unsigned int	total_time;
		unsigned char	kp_interval;
		unsigned int	kp_count;
		unsigned int	kp_offsets[RECORD_MAX_KP_COUNT];
	};
};

#pragma pack()

class event_record
{
protected:

	event_record(char t) : type(t) {}

public:

	virtual ~event_record() {}

protected:

	char type;

protected:

	virtual bool _marshal(GNET::Marshal::OctetsStream& os) = 0;
	virtual bool _unmarshal(GNET::Marshal::OctetsStream& os) = 0;

public:

	char get_type() const { return type; }

	static event_record* unmarshal(GNET::Marshal::OctetsStream& os);
	bool marshal(GNET::Marshal::OctetsStream& os);
};

typedef abase::vector<event_record*> event_vec;

class frame_record
{
public:

	frame_record() : delta_time(0), flag(0) {}
	~frame_record()
	{
		for (size_t i = 0; i < events.size(); i++)
			delete events[i];
	}

protected:

	enum
	{
		fr_host_pos = 0,
		fr_host_dir,
		fr_break,
		fr_key_point,
		fr_key_point2,
	};

	unsigned char	delta_time;
	unsigned char	flag;
	int				instance;
	int				record_stamp;
	A3DVECTOR3		host_pos;
	unsigned short	host_dir;
	event_vec		events;

public:

	bool unmarshal(GNET::Marshal::OctetsStream& os);
	bool marshal(GNET::Marshal::OctetsStream& os);
	void push_event(event_record* rec) { events.push_back(rec); }
	size_t evnet_count() const { return events.size(); }
	event_vec& get_events() { return events; }
	unsigned char get_delta_time() const { return delta_time; }
	void set_delta_time(unsigned char t) { delta_time = t; }
	void set_host_pos(const A3DVECTOR3& v) { flag |= (1 << fr_host_pos); host_pos = v; }
	const A3DVECTOR3& get_host_pos() { return host_pos;	}
	int get_instance() const { return instance; }
	int get_record_stamp() const { return record_stamp; }
	bool get_has_host_pos() const { return (flag & (1 << fr_host_pos)) != 0; }
	void set_host_dir(const A3DVECTOR3& v);
	void set_host_dir(unsigned short d) { flag |= (1 << fr_host_dir); host_dir = d; }
	bool get_has_host_dir() const { return (flag & (1 << fr_host_dir)) != 0; }
	unsigned short get_host_dir() { return host_dir; }
	void set_break() { flag |= (1 << fr_break); }
	bool get_break() const { return (flag & (1 << fr_break)) != 0; }
	void set_key_point(int _instance, const A3DVECTOR3& pos, int stamp)
	{
		flag |= (1 << fr_key_point);
		instance = _instance;
		host_pos = pos;
		record_stamp = stamp;
	}
	bool get_key_point() const { return (flag & (1 << fr_key_point)) != 0; }
	void set_key_point2() { flag |= (1 << fr_key_point2); }
	bool get_key_point2() const { return (flag & (1 << fr_key_point2)) != 0; }
	bool is_empty() const { return events.size() == 0; }
};

typedef abase::vector<frame_record*> frame_vec;

struct record_edit_info
{
	unsigned int start_second;
	unsigned int end_second;
};

typedef abase::vector<record_edit_info> record_edit_info_vec;

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

	enum
	{
		enum_seek_none,
		enum_seek_start,
		enum_seek_second,
		enum_seek_end,
	};

	game_record();
	~game_record() { release(); }

protected:

	typedef abase::hash_map<int, int> filter_protocol_map;

	int cur_version;
	frame_vec frames;
	FILE* fp_data;
	frame_record* cur_frame;
	frame_record* last_empty_frame;
	int state;
	float play_speed;
	bool paused;
	int char_id;
	int instance;
	A3DVECTOR3 host_pos;
	A3DVECTOR3 host_dir;
	unsigned char gender;
	unsigned char faceid;
	unsigned char hairid;
	unsigned char earid;
	unsigned char tailid;
	unsigned char defaultskin;
	unsigned char occupation;
	char name[RECORD_NAME_LEN];
	wchar_t title[RECORD_TITLE_LEN];
	unsigned char name_len;
	filter_protocol_map filter_map;
	long save_time;
	unsigned char kp_interval;

	unsigned char record_data[RECORD_COMPRESS_SIZE];
	size_t data_size;

	unsigned int real_time;
	unsigned int record_time;
	unsigned int replay_time;
	unsigned int prev_record_time;
	unsigned int check_time;
	unsigned int kp_save_time;
	unsigned int kp_time;
	unsigned int kp_skip_time;
	unsigned int kp_cur_time;
	A3DVECTOR3 host_prev_frame_pos;
	A3DVECTOR3 host_cur_frame_pos;
	unsigned int total_time;
	unsigned int kp_count;
	unsigned int kp_cur_count;
	unsigned int kp_offsets[RECORD_MAX_KP_COUNT];
	bool start_record_flag;
	bool end_record_flag;
	bool start_replay_flag;
	bool end_replay_flag;
	unsigned int replay_setting;
	int record_stamp;
	int replay_valid;
	char file_to_open[MAX_PATH];
	bool kp_init;
	bool seek_flag;
	unsigned int seek_time;
	unsigned int seek_remain_time;
	bool seeking;
	bool inner_seek;
	bool replay_ended;
	bool edit_mode;
	record_edit_info_vec edit_info;
	abase::hash_map<int, int> edit_point_map;
	unsigned char edit_point_prev_offset[RECORD_MAX_EDIT_COUNT];
	unsigned char edit_point_post_offset[RECORD_MAX_EDIT_COUNT];
	int fade_state;
	int seek_adjust;
	
protected:

	void prepare_seek();

public:

	int get_cur_version() const { return cur_version; }

	int get_state() const { return state; }
	void set_state(int s) { state = s; }

	float get_play_speed() const { return play_speed; }
	void set_play_speed(float speed) { play_speed = speed; }

	bool is_paused() const { return paused; }
	void set_paused(bool b) { paused = b; }

	bool filter_protocol(int type)
	{
		return filter_map.find(type) != filter_map.end();
	}

	void set_host_info(
		int c_id,
		unsigned char g,	// gender
		unsigned char f,	// face
		unsigned char h,	// head
		unsigned char e,	// ear
		unsigned char t,	// tail
		unsigned char ds,
		unsigned char o,	// occupation
		const void* n,
		size_t len)
	{
		assert(len <= RECORD_NAME_LEN);
		char_id		= c_id;
		gender		= g;
		faceid		= f;
		hairid		= h;
		earid		= e;
		tailid		= t;
		defaultskin	= ds;
		occupation	= o;
		name_len = (unsigned char)len;
		memset(name, 0, sizeof(name));
		memcpy(name, n, len);
	}

	int get_char_id() const { return char_id; }
	int get_instance() const { return instance; }
	const A3DVECTOR3& get_host_pos() const { return host_pos; }
	const A3DVECTOR3& get_host_dir() const { return host_dir; }
	unsigned char get_gender() const { return gender; }
	unsigned char get_face() const { return faceid; }
	unsigned char get_hair() const { return hairid; }
	unsigned char get_ear() const { return earid; }
	unsigned char get_tail() const { return tailid; }
	unsigned char get_skin() const { return defaultskin; }
	unsigned char get_occupation() const { return occupation; }
	const char* get_name() const { return name; }
	unsigned char get_name_len() const { return name_len; }
	const wchar_t* get_title() const { return title; }
	void set_title(const wchar_t* t)
	{ 
		wcsncpy(title, t, RECORD_TITLE_LEN-1);
		title[RECORD_TITLE_LEN-1] = 0;
	}
	long get_save_time() const { return save_time; }

	frame_record* inc_frame()
	{
		if (!cur_frame)
			cur_frame = new frame_record;

		return cur_frame;
	}

	void push_cur_frame(bool force_save = false);

	frame_record* get_cur_frame()
	{
		if (!cur_frame)
			cur_frame = new frame_record;
		return cur_frame;
	}

	bool begin_save(const char* file_name, int game_version);
	void end_save();

	bool open_record(const char* file_name);

	void close_record()
	{
		if (fp_data)
		{
			fclose(fp_data);
			fp_data = 0;
		}
	}

	void save_one_frame(frame_record* frame);
	frame_record* read_one_frame(bool seek_read);

	void reset()
	{
		for (size_t i = 0; i < frames.size(); i++)
			delete frames[i];

		frames.clear();
		delete cur_frame;
		cur_frame = 0;
		delete last_empty_frame;
		last_empty_frame = 0;
		kp_init = false;
		replay_ended = false;
		replay_valid = 1;
		end_replay_flag = false;
		kp_time = 0;
		kp_skip_time = 0;
		kp_cur_time = 0;
		inner_seek = false;
	}

	void release()
	{
		reset();
		data_size = 0;
		play_speed = 1.0f;
		paused = false;
		real_time = 0;
		record_time = 0;
		replay_time = 0;
		prev_record_time = 0;
		total_time = 0;
		check_time = 0;
		kp_save_time = 0;
		start_record_flag = false;
		end_record_flag = false;
		start_replay_flag = false;
		end_replay_flag = false;
		file_to_open[0] = 0;
		kp_count = 0;
		kp_cur_count = 0;
		kp_interval = 0;
		memset(kp_offsets, 0, sizeof(kp_offsets));
		seek_flag = false;
		seek_time = 0;
		seek_remain_time = 0;
		seeking = false;
		edit_mode = false;
		edit_info.clear();
		edit_point_map.clear();
		fade_state = enum_fade_none;

		if (fp_data)
		{
			fclose(fp_data);
			fp_data = 0;
		}

		char_id			= 0;
		instance		= 0;
		host_pos.Clear();
		host_dir.Set(1.0f, 0, 0);
	}

	bool seek();
	unsigned int get_real_time() const { return real_time; }
	void inc_real_time(unsigned int delta) { real_time += delta; }
	unsigned int get_record_time() const { return record_time; }
	unsigned int get_replay_time() const { return replay_time; }
	void inc_record_time(unsigned int delta) { prev_record_time = record_time; record_time += delta; }
	void calc_host_pos();
	unsigned int get_total_time() const { return total_time; }
	void set_start_record_flag() { start_record_flag = true; }
	void clear_start_record_flag() { start_record_flag = false; }
	bool get_start_record_flag() const { return start_record_flag; }
	void set_end_record_flag() { end_record_flag = true; }
	void clear_end_record_flag() { end_record_flag = false; }
	bool get_end_record_flag() const { return end_record_flag; }
	bool get_start_replay_flag() const { return start_replay_flag; }
	void set_start_replay_flag() { start_replay_flag = true; }
	void clear_start_replay_flag() { start_replay_flag = false; }
	void set_end_replay_flag() { end_replay_flag = true; }
	bool get_end_replay_flag() const { return end_replay_flag; }
	void set_replay_setting(unsigned int setting, bool enable);
	bool get_replay_setting(unsigned int setting) const { return (replay_setting & (1 << setting)) != 0; }
	void apply_cur_setting();
	void set_record_stamp(int stamp) { record_stamp = stamp; }
	int get_record_stamp() const { return record_stamp; }
	void inc_record_stamp() { record_stamp++; }
	void set_check_time(unsigned int t) { check_time = t; }
	unsigned int get_check_time() const { return check_time; }
	void set_kp_save_time(unsigned int t) { kp_save_time = t; }
	unsigned int get_kp_save_time() const { return kp_save_time; }
	bool replay_is_valid() const { return replay_valid > 0; }
	void inc_replay_valid() { replay_valid++; }
	void dec_replay_valid() { replay_valid--; }
	void set_file_to_open(const char* path) { strcpy(file_to_open, path); }
	const char* get_file_to_open() const { return file_to_open; }
	void set_seek_time(unsigned int t) { seek_flag = true; seek_time = t; }
	bool get_seek_flag() const { return seek_flag; }
	unsigned int get_seek_remain_time() const { return seek_remain_time; }
	void reset_seek_remain_time() { seek_remain_time = 0; }
	void clear_seek_flag() { seek_flag = false; }
	bool is_seeking() const { return seeking; }
	void reset_seeking() { seeking = false; }
	bool is_replay_ended() const { return replay_ended; }
	void set_edit_mode() { edit_mode = true; }
	void clear_edit_mode() { edit_mode = false; }
	bool is_edit_mode() const { return edit_mode; }
	bool save_edited_record(const char* path, const wchar_t* _title);
	void set_edit_info(const record_edit_info_vec& v) { edit_info = v; }
	bool is_edited_record() { return edit_point_map.size() != 0; }
	int get_fade_state() const { return fade_state; }
	void set_fade_state(int s) { fade_state = s; }
	bool is_inner_seek() const { return inner_seek; }

	void reset_host_pos(const A3DVECTOR3& pos) 
	{ 
		host_pos = pos; 
		host_prev_frame_pos = pos; 
		host_cur_frame_pos = pos; 
	} 
	//for restore host player work(avoid work invalidation) after progress bar be adjusted.
	void set_seek_adjust(int s) { seek_adjust = s; }
	int get_seek_adjust() const  { return seek_adjust; }
	const A3DVECTOR3& get_pre_frame_host_pos() const { return host_prev_frame_pos; }
	const A3DVECTOR3& get_cur_frame_host_pos() const { return host_cur_frame_pos; }
};

class event_record_protocol : public event_record
{
public:

	event_record_protocol() : event_record(EVENT_TYPE_PROTOCOL), protocol(0) {}
	event_record_protocol(GNET::Protocol* p) : event_record(EVENT_TYPE_PROTOCOL) { protocol = p->Clone(); }
	~event_record_protocol()
	{
		if (protocol)
			protocol->Destroy();
	}

protected:

	GNET::Protocol* protocol;

public:

	virtual bool _unmarshal(GNET::Marshal::OctetsStream& os);
	virtual bool _marshal(GNET::Marshal::OctetsStream& os);
	GNET::Protocol* get_protocol() { return protocol; }
};

class event_record_msg : public event_record
{
public:

	event_record_msg() : event_record(EVENT_TYPE_MSG) { memset(&msg, 0, sizeof(msg)); }
	event_record_msg(const ECMSG& m) : event_record(EVENT_TYPE_MSG), msg(m) {}
	~event_record_msg() {}

protected:

	ECMSG msg;

public:

	virtual bool _unmarshal(GNET::Marshal::OctetsStream& os);
	virtual bool _marshal(GNET::Marshal::OctetsStream& os);
	ECMSG& get_msg() { return msg; }
};

class event_record_data : public event_record
{
public:

	event_record_data() : event_record(EVENT_TYPE_DATA), data_type(0) {}
	event_record_data(unsigned char _type, const GNET::Octets& o) : event_record(EVENT_TYPE_DATA), data_type(_type), data(o) {}
	~event_record_data() {}

protected:

	unsigned char data_type;
	GNET::Octets data;

public:

	virtual bool _unmarshal(GNET::Marshal::OctetsStream& os);
	virtual bool _marshal(GNET::Marshal::OctetsStream& os);
	unsigned char get_data_type() { return data_type; }
	GNET::Octets& get_data() { return data; }
};

struct record_info
{
	char path[MAX_PATH];
	wchar_t title[RECORD_TITLE_LEN];
	long save_time;
	long total_time;
};

typedef abase::vector<record_info> record_info_vec;

void EnumRecordFiles(const char* szDir, record_info_vec& Infos);

#endif
