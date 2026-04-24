#include "EC_GameRecord.h"
#include "AFilePackage.h"
#include "A3DMacros.h"
#include <shlwapi.h>

#define GAME_RECORD_CUR_VERSION		16
#define GAME_RECORD_MIN_VERSION		15
#define	EMPTY_FRAME_MAX_DELTA		150
#define RECORD_MAX_FRAME_SIZE		(1024 * 1024)

static const wchar_t* _default_title = L"诛仙录像";

inline unsigned short _gen_angle_from_dir(const A3DVECTOR3& v)
{
	float a = atan2f(v.z, v.x);
	a = (a + A3D_PI) * 65535.f / A3D_2PI;
	return (unsigned short)a;
}

inline A3DVECTOR3 _gen_dir_from_angle(unsigned short a)
{
	float d = a * A3D_2PI / 65535.f - A3D_PI;
	A3DVECTOR3 v;
	v.y = 0.f;
	v.x = cosf(d);
	v.z = sinf(d);
	v.Normalize();
	return v;
}

void find_files(const char* dir, const char* ext, abase::vector<char*>& paths)
{
	char path[MAX_PATH];
	strcpy(path, dir);
	int off = strlen(dir);
	strcat(path, "\\*.*");
	_finddata_t c_file;
    long hFile = _findfirst(path, &c_file);

	if (hFile == -1)
		return;

	while (true)
	{
		if (c_file.attrib & _A_SUBDIR)
		{
			if (strcmp(c_file.name, ".") != 0
			 && strcmp(c_file.name, "..") != 0)
			{
				strcpy(path+off+1, c_file.name);
				find_files(path, ext, paths);
			}
		}
		else
		{
			if (stricmp(::PathFindExtensionA(c_file.name), ext) == 0)
			{
				char* full_path = new char[MAX_PATH];
				strcpy(full_path, dir);
				strcat(full_path, "\\");
				strcat(full_path, c_file.name);
				paths.push_back(full_path);
			}
		}

		if (_findnext(hFile, &c_file) != 0)
			break;
	}

	_findclose(hFile);
}

using namespace GNET;

game_record::game_record() :
cur_version(0),
fp_data(0),
cur_frame(0),
last_empty_frame(0),
state(enum_gr_disable),
play_speed(1.f),
paused(false),
data_size(0),
real_time(0),
record_time(0),
replay_time(0),
prev_record_time(0),
check_time(0),
kp_save_time(0),
kp_time(0),
kp_skip_time(0),
kp_cur_time(0),
total_time(0),
start_record_flag(false),
end_record_flag(false),
start_replay_flag(false),
end_replay_flag(false),
replay_setting(0),
record_stamp(0),
replay_valid(1),
kp_init(false),
seek_flag(false),
inner_seek(false),
seek_time(0),
seek_remain_time(0),
seeking(false),
replay_ended(false),
edit_mode(false),
fade_state(enum_fade_none),
seek_adjust(enum_seek_none)
{
	file_to_open[0] = 0;
	host_pos.Clear();
	host_dir.Set(1.0f, 0, 0);
	host_prev_frame_pos.Clear();
	host_cur_frame_pos.Clear();

	filter_map[PROTOCOL_ACREPORT]		= 1;
	filter_map[PROTOCOL_ACREMOTECODE]	= 1;
	filter_map[PROTOCOL_GETFRIENDS_RE]	= 1;
	filter_map[PROTOCOL_FACTIONLIST_RE]	= 1;
	filter_map[PROTOCOL_PLAYERLOGOUT]	= 1;
}

event_record* event_record::unmarshal(GNET::Marshal::OctetsStream& os)
{
	try
	{
		event_record* rec;
		char type;
		GNET::Marshal::OctetsStream data;

		os >> type;
		os >> data;

		switch (type)
		{
		case EVENT_TYPE_PROTOCOL:
			rec = new event_record_protocol;
			break;
		case EVENT_TYPE_MSG:
			rec = new event_record_msg;
			break;
		case EVENT_TYPE_DATA:
			rec = new event_record_data;
			break;
		default:
			return 0;
		}

		if (!rec->_unmarshal(data))
		{
			delete rec;
			return 0;
		}

		return rec;
	}
	catch (Marshal::Exception &)
	{
		return 0;
	}
}

bool event_record::marshal(GNET::Marshal::OctetsStream& os)
{
	try
	{
		GNET::Marshal::OctetsStream data;

		if (!_marshal(data))
			return false;

		os << type;
		os << data;
		return true;
	}
	catch (Marshal::Exception &)
	{
		return 0;
	}
}

void frame_record::set_host_dir(const A3DVECTOR3& v)
{
	flag |= (1 << fr_host_dir);
	host_dir = _gen_angle_from_dir(v);
}

bool frame_record::unmarshal(GNET::Marshal::OctetsStream& os)
{
	try
	{
		unsigned short event_count;

		os >> delta_time;
		os >> flag;

		if (get_has_host_pos())
		{
			os >> host_pos.x;
			os >> host_pos.y;
			os >> host_pos.z;
		}

		if (get_has_host_dir())
			os >> host_dir;

		if (get_key_point())
		{
			os >> instance;
			os >> host_pos.x;
			os >> host_pos.y;
			os >> host_pos.z;
			os >> record_stamp;
		}

		os >> event_count;

		for (int i = 0; i < event_count; i++)
		{
			event_record* event = event_record::unmarshal(os);

			if (event)
				events.push_back(event);
			else
			{
				assert(false);
				return false;
			}
		}

		return true;
	}
	catch (Marshal::Exception &)
	{
		return false;
	}
}

bool frame_record::marshal(GNET::Marshal::OctetsStream& os)
{
	try
	{
		os << delta_time;
		os << flag;

		if (get_has_host_pos())
		{
			os << host_pos.x;
			os << host_pos.y;
			os << host_pos.z;
		}

		if (get_has_host_dir())
			os << host_dir;

		if (get_key_point())
		{
			os << instance;
			os << host_pos.x;
			os << host_pos.y;
			os << host_pos.z;
			os << record_stamp;
		}

		unsigned short count = (unsigned short)events.size();
		os << count;

		for (size_t i = 0; i < count; i++)
			events[i]->marshal(os);

		return true;
	}
	catch (Marshal::Exception &)
	{
		return false;
	}
}

inline void once_compress_save(
	FILE* fp_data,
	unsigned char* record_data,
	size_t data_size)
{
	if (data_size == 0)
		return;

	unsigned char* buf = new unsigned char[data_size];
	size_t size = data_size;
	int ret = AFilePackage::Compress(record_data, data_size, buf, (DWORD*)&size);

	if (ret == 0 && size < data_size)
	{
		fwrite(&data_size, sizeof(data_size), 1, fp_data);
		fwrite(&size, sizeof(size), 1, fp_data);
		fwrite(buf, size, 1, fp_data);
	}
	else
	{
		size = 0;
		fwrite(&data_size, sizeof(data_size), 1, fp_data);
		fwrite(&size, sizeof(size), 1, fp_data);
		fwrite(record_data, data_size, 1, fp_data);
	}

	fflush(fp_data);
	delete[] buf;
}

void game_record::save_one_frame(frame_record* frame)
{
	Marshal::OctetsStream os;

	if (!frame->marshal(os))
	{
		assert(false);
		os.clear();
	}

	size_t total_size = data_size + os.size() + sizeof(size_t);

	if (total_size > RECORD_COMPRESS_SIZE)
	{
		unsigned char* buf = new unsigned char[total_size];
		memcpy(buf, record_data, data_size);
		unsigned char* offset = buf + data_size;
		*(size_t*)offset = os.size();
		memcpy(offset + sizeof(size_t), os.begin(), os.size());
		once_compress_save(fp_data, buf, total_size);
		data_size = 0;
		delete[] buf;
	}
	else
	{
		unsigned char* offset = record_data + data_size;
		*(size_t*)offset = os.size();
		memcpy(offset + sizeof(size_t), os.begin(), os.size());
		data_size = total_size;
	}
}

void game_record::push_cur_frame(bool force_save)
{
	if (!fp_data)
	{
		delete cur_frame;
		cur_frame = 0;
		return;
	}

	if (!cur_frame)
		return;

	total_time += cur_frame->get_delta_time();

	if (force_save || !cur_frame->is_empty() || cur_frame->get_key_point())
	{
		if (last_empty_frame)
		{
			save_one_frame(last_empty_frame);
			delete last_empty_frame;
			last_empty_frame = 0;
		}

		if (cur_frame->get_key_point())
		{
			// force to flush and save
			once_compress_save(fp_data, record_data, data_size);
			data_size = 0;

			if (kp_count < RECORD_MAX_KP_COUNT)
			{
				unsigned int cur_count = kp_count;
				kp_offsets[cur_count] = ftell(fp_data);
				kp_count++;
				fseek(fp_data, sizeof(game_record_header), SEEK_SET);
				fwrite(&total_time, sizeof(total_time), 1, fp_data);
				fseek(fp_data, 1, SEEK_CUR);
				fwrite(&kp_count, sizeof(kp_count), 1, fp_data);
				fseek(fp_data, sizeof(int) * cur_count, SEEK_CUR);
				fwrite(&kp_offsets[cur_count], sizeof(int), 1, fp_data);
				fflush(fp_data);
				fseek(fp_data, 0, SEEK_END);
			}
		}

		save_one_frame(cur_frame);
		delete cur_frame;
		cur_frame = 0;
	}
	else // cur_frame is empty
	{
		if (last_empty_frame)
		{
			unsigned int t = (unsigned int)last_empty_frame->get_delta_time() + cur_frame->get_delta_time();

			if (t >= EMPTY_FRAME_MAX_DELTA)
			{
				save_one_frame(last_empty_frame);
				delete last_empty_frame;
				last_empty_frame = cur_frame;
			}
			else
			{
				last_empty_frame->set_delta_time((unsigned char)t);

				if (cur_frame->get_has_host_pos())
					last_empty_frame->set_host_pos(cur_frame->get_host_pos());

				if (cur_frame->get_has_host_dir())
					last_empty_frame->set_host_dir(cur_frame->get_host_dir());

				delete cur_frame;
			}
		}
		else
			last_empty_frame = cur_frame;

		cur_frame = 0;
	}
}

bool game_record::begin_save(const char* file_name, int game_version)
{
	start_record_flag = false;
	fp_data = fopen(file_name, "wb");

	if (!fp_data)
		return false;

	game_record_header head;
	memset(&head, 0, sizeof(head));
	head.game_version	= game_version;
	head.record_version	= GAME_RECORD_CUR_VERSION;
	head.char_id		= char_id;
	head.gender			= gender;
	head.faceid			= faceid;
	head.hairid			= hairid;
	head.earid			= earid;
	head.tailid			= tailid;
	head.occupation		= occupation;
	memcpy(head.name, name, sizeof(name));
	head.name_len		= name_len;
	head.replay_setting	= replay_setting;
	wcsncpy(head.title, title, RECORD_TITLE_LEN-1);
	head.title[RECORD_TITLE_LEN-1] = 0;
	head.save_time		= time(0);
	fwrite(&head, sizeof(head), 1, fp_data);

	// save and clear tracker part
	game_record_tracker tracker;
	memset(&tracker, 0, sizeof(tracker));
	fwrite(&tracker, sizeof(tracker), 1, fp_data);

	fflush(fp_data);
	return true;
}

void game_record::end_save()
{
	end_record_flag = false;
	push_cur_frame(true);

	if (fp_data)
	{
		// force to flush
		once_compress_save(fp_data, record_data, data_size);
		data_size = 0;
		game_record_tracker tracker;
		memset(&tracker, 0, sizeof(tracker));
		tracker.total_time	= total_time;
		tracker.kp_interval	= RECORD_KEY_POINT_INTERVAL / 1000;
		tracker.kp_count	= kp_count;
		memcpy(tracker.kp_offsets, kp_offsets, sizeof(kp_offsets));
		fseek(fp_data, sizeof(game_record_header), SEEK_SET);
		fwrite(&tracker, sizeof(tracker), 1, fp_data);
		fclose(fp_data);
		fp_data = 0;
	}
}

bool game_record::save_edited_record(const char* path, const wchar_t* _title)
{
	if (get_state() != enum_gr_replay || fp_data == 0 || edit_info.size() == 0 || edit_info.size() > RECORD_MAX_EDIT_COUNT || is_edited_record())
		return false;

	FILE* fp_edit = fopen(path, "wb");

	if (!fp_edit)
		return false;

	long old_pos = ftell(fp_data);
	fseek(fp_data, 0, SEEK_SET);
	game_record_header head;
	fread(&head, sizeof(head), 1, fp_data);
	wcsncpy(head.title, _title, RECORD_TITLE_LEN-1);
	head.title[RECORD_TITLE_LEN-1] = 0;
	fseek(fp_edit, sizeof(game_record_header)+sizeof(game_record_tracker), SEEK_SET);

	game_record_tracker tracker;
	memset(&tracker, 0, sizeof(tracker));
	tracker.kp_interval = RECORD_KEY_POINT_INTERVAL / 1000;

	for (size_t i = 0; i < edit_info.size(); i++)
	{
		record_edit_info* info = &edit_info[i];

		if (info->start_second >= info->end_second)
		{
			assert(false);
			continue;
		}

		unsigned int start_kp = info->start_second / (RECORD_KEY_POINT_INTERVAL / 1000);
		unsigned int end_kp = info->end_second / (RECORD_KEY_POINT_INTERVAL / 1000) + 1;

		if (start_kp >= kp_count || end_kp > kp_count)
		{
			assert(false);
			continue;
		}

		long sz;

		if (end_kp == kp_count)
		{
			fseek(fp_data, 0, SEEK_END);
			sz = ftell(fp_data) - kp_offsets[start_kp];
		}
		else
			sz = kp_offsets[end_kp] - kp_offsets[start_kp];

		fseek(fp_data, kp_offsets[start_kp], SEEK_SET);
		char* buf = new char[sz];

		if (fread(buf, sz, 1, fp_data) != 1)
		{
			delete[] buf;
			continue;
		}

		head.edit_point_indice[head.edit_point_count] = tracker.kp_count;
		head.edit_point_post_offset[head.edit_point_count] = info->start_second - start_kp * (RECORD_KEY_POINT_INTERVAL / 1000);
		head.edit_point_count++;
		tracker.kp_offsets[tracker.kp_count] = ftell(fp_edit);

		if (head.edit_point_count < RECORD_MAX_EDIT_COUNT)
			head.edit_point_prev_offset[head.edit_point_count] = (RECORD_KEY_POINT_INTERVAL / 1000) - (info->end_second - (end_kp - 1) * (RECORD_KEY_POINT_INTERVAL / 1000));

		for (unsigned int j = 1; j < end_kp - start_kp; j++)
			tracker.kp_offsets[tracker.kp_count+j] = tracker.kp_offsets[tracker.kp_count] + (kp_offsets[start_kp+j] - kp_offsets[start_kp]);

		tracker.kp_count += end_kp - start_kp;
		tracker.total_time += (info->end_second - info->start_second) * 1000;
		fwrite(buf, sz, 1, fp_edit);
		delete[] buf;
	}

	fseek(fp_edit, 0, SEEK_SET);
	fwrite(&head, sizeof(head), 1, fp_edit);
	fwrite(&tracker, sizeof(tracker), 1, fp_edit);
	fclose(fp_edit);
	fseek(fp_data, old_pos, SEEK_SET);
	return true;
}

bool game_record::open_record(const char* file_name)
{
	try
	{
		size_t len;
		fp_data = fopen(file_name, "rb");

		if (!fp_data)
			throw "record: can't open file";

		game_record_header head;
		len = fread(&head, sizeof(head), 1, fp_data);

		if (len != 1 || head.record_version < GAME_RECORD_MIN_VERSION || head.record_version > GAME_RECORD_CUR_VERSION)
			throw "record: wrong header version";

		cur_version		= head.record_version;
		char_id			= head.char_id;
		gender			= head.gender;
		faceid			= head.faceid;
		hairid			= head.hairid;
		earid			= head.earid;
		tailid			= head.tailid;
		occupation		= head.occupation;
		name_len		= head.name_len;

		if (name_len > RECORD_NAME_LEN)
			throw "record: wrong name len";

		memset(name, 0, sizeof(name));
		memcpy(name, head.name, name_len);
		replay_setting	= head.replay_setting;
		memcpy(title, head.title, sizeof(title));
		title[RECORD_TITLE_LEN-1] = 0;
		save_time		= head.save_time;

		if (head.edit_point_count > RECORD_MAX_EDIT_COUNT)
			throw "record: wrong edit count";

		game_record_tracker tracker;
		len = fread(&tracker, sizeof(tracker), 1, fp_data);

		if (len != 1)
			throw "record: wrong tracker";

		total_time		= tracker.total_time;
		kp_interval		= tracker.kp_interval;
		kp_count		= tracker.kp_count;
		memcpy(kp_offsets, tracker.kp_offsets, sizeof(kp_offsets));
		memcpy(edit_point_prev_offset, head.edit_point_prev_offset, sizeof(edit_point_prev_offset));
		memcpy(edit_point_post_offset, head.edit_point_post_offset, sizeof(edit_point_post_offset));

		if (kp_count > RECORD_MAX_KP_COUNT)
			throw "record: wrong tracker count";

		for (unsigned char i = 0; i < head.edit_point_count; i++)
		{
			if (head.edit_point_indice[i] >= kp_count)
				throw "record: wrong edit index";

			if (edit_point_prev_offset[i] > (RECORD_KEY_POINT_INTERVAL / 1000) || edit_point_post_offset[i] > (RECORD_KEY_POINT_INTERVAL))
				throw "record: wrong edit index";

			edit_point_map[head.edit_point_indice[i]] = i;
		}

		frame_record* fr = read_one_frame(true);

		if (!fr || !fr->get_key_point())
			throw "record; wrong keypoint";

		instance = fr->get_instance();
		host_pos = fr->get_host_pos();
		record_stamp = fr->get_record_stamp();
		apply_cur_setting();
	}
	catch (char*)
	{
		release();
		return false;
	}

	return true;
}

frame_record* game_record::read_one_frame(bool seek_read)
{
	try
	{
		delete cur_frame;
		cur_frame = 0;

		if (frames.size())
		{
			cur_frame = frames.front();
			frames.erase(frames.begin());
			kp_cur_time += cur_frame->get_delta_time();

			if (kp_cur_time > kp_skip_time)
			{
				replay_time += cur_frame->get_delta_time();

				if (replay_time > total_time)
				{
					replay_ended = true;
					delete cur_frame;
					cur_frame = 0;
					return 0;
				}
			}

			if (kp_time && kp_cur_time >= kp_time)
			{
				prepare_seek();
				set_fade_state(enum_fade_out);
				delete cur_frame;
				cur_frame = 0;
				return 0;
			}

			host_prev_frame_pos = host_cur_frame_pos;

			if (cur_frame->get_has_host_pos())
				host_cur_frame_pos = cur_frame->get_host_pos();

			if (cur_frame->get_has_host_dir())
				host_dir = _gen_dir_from_angle(cur_frame->get_host_dir());

			return cur_frame;
		}

		if (!fp_data || feof(fp_data))
		{
			replay_ended = true;
			return 0;
		}

		size_t frame_size, compress_size;
		int len = fread(&frame_size, sizeof(frame_size), 1, fp_data);

		if (len != 1)
			throw "record: wrong frame size";

		len = fread(&compress_size, sizeof(compress_size), 1, fp_data);

		if (len != 1)
			throw "record: wrong frame size";

		if (frame_size > RECORD_MAX_FRAME_SIZE || compress_size > RECORD_MAX_FRAME_SIZE)
			throw "record: wrong frame size";

		unsigned char* buf = new unsigned char[frame_size];

		if (compress_size == 0)
		{
			len = fread(buf, frame_size, 1, fp_data);

			if (len != 1)
			{
				delete[] buf;
				throw "record: wrong frame size";
			}
		}
		else
		{
			unsigned char* buf_c = new unsigned char[compress_size];
			len = fread(buf_c, compress_size, 1, fp_data);

			if (len != 1)
			{
				delete[] buf;
				delete[] buf_c;
				throw "record: wrong frame size";
			}

			size_t size_read = frame_size;

			if (AFilePackage::Uncompress(buf_c, compress_size, buf, (DWORD*)&size_read) != 0
			|| size_read != frame_size)
			{
				delete[] buf;
				delete[] buf_c;
				throw "record: wrong frame size";
			}

			delete[] buf_c;
		}

		size_t i = 0;
		unsigned char* frame_buf;

		while (i < frame_size)
		{
			frame_buf = buf + i;
			i += sizeof(size_t);

			if (i > frame_size)
				break;

			size_t one_frame_size = *(size_t*)frame_buf;
			frame_buf += sizeof(size_t);
			i += one_frame_size;

			if (i > frame_size)
				break;

			Octets o(frame_buf, one_frame_size);
			frame_record* fr = new frame_record;

			if (fr->unmarshal(GNET::Marshal::OctetsStream(o)))
			{
				if (fr->get_key_point())
				{
					kp_cur_time = 0;
					kp_skip_time = 0;
					kp_cur_count++;

					abase::hash_map<int, int>::iterator it;
					it = edit_point_map.find(kp_cur_count);

					if (it != edit_point_map.end())
						kp_time = RECORD_KEY_POINT_INTERVAL - edit_point_prev_offset[it->second] * 1000;
					else
						kp_time = 0;

					it = edit_point_map.find(kp_cur_count-1);

					if (it != edit_point_map.end())
					{
						seek_remain_time = edit_point_post_offset[it->second] * 1000;
						kp_skip_time = seek_remain_time;

						if (!seek_read)
						{
							prepare_seek();
							set_fade_state(enum_fade_out);
							delete fr;
							break;
						}
					}
				}

				if (fr->get_key_point() || fr->get_key_point2())
				{
					// 当关键点初始化完成后，忽略以后的关键点
					if (kp_init)
					{
						delete fr;
						continue;
					}
				}
				else
					kp_init = true;

				if (cur_frame)
					frames.push_back(fr);
				else
				{
					kp_cur_time += fr->get_delta_time();

					if (kp_cur_time > kp_skip_time)
					{
						replay_time += fr->get_delta_time();

						if (replay_time > total_time)
						{
							replay_ended = true;
							delete fr;
							break;
						}
					}

					if (kp_time && kp_cur_time >= kp_time)
					{
						prepare_seek();
						set_fade_state(enum_fade_out);
						delete fr;
						break;
					}

					cur_frame = fr;
				}
			}
			else
			{
				assert(false);
				delete fr;
			}
		}

		delete[] buf;

		if (cur_frame)
		{
			host_prev_frame_pos = host_cur_frame_pos;

			if (cur_frame->get_has_host_pos())
				host_cur_frame_pos = cur_frame->get_host_pos();

			if (cur_frame->get_has_host_dir())
				host_dir = _gen_dir_from_angle(cur_frame->get_host_dir());
		}

		return cur_frame;
	}
	catch (char*)
	{
		replay_ended = true;
		return 0;
	}
}

void game_record::prepare_seek()
{
	seek_flag = true;
	inner_seek = true;
	seek_time = replay_time;
}

bool game_record::seek()
{
	if (!fp_data)
		return false;

	if (seek_time > total_time)
		return false;

	unsigned int outer_delay = 0;

	if (!inner_seek)
	{
		int t = 0;
		unsigned int _kp = 0;

		while (true)
		{
			if (_kp >= kp_count)
				return false;

			unsigned int prev_t = t;
			t += RECORD_KEY_POINT_INTERVAL;
			abase::hash_map<int, int>::iterator it = edit_point_map.find(_kp);

			if (it != edit_point_map.end())
				t -= edit_point_post_offset[it->second] * 1000;

			it = edit_point_map.find(_kp+1);

			if (it != edit_point_map.end())
				t -= edit_point_prev_offset[it->second] * 1000;

			if (t >= (int)seek_time)
			{
				outer_delay = seek_time - prev_t;
				kp_cur_count = _kp;
				replay_time = seek_time;
				record_time = _kp * RECORD_KEY_POINT_INTERVAL;
				break;
			}

			_kp++;
		}
	}

	if (kp_cur_count >= kp_count)
		return false;

	reset();
	fseek(fp_data, kp_offsets[kp_cur_count], SEEK_SET);
	frame_record* fr = read_one_frame(true);

	if (!fr || !fr->get_key_point())
	{
		replay_ended = true;
		return false;
	}

	seek_remain_time += outer_delay;
	kp_skip_time = seek_remain_time;
	instance = fr->get_instance();
	host_pos = fr->get_host_pos();
	host_prev_frame_pos	= host_pos;
	host_cur_frame_pos	= host_pos;
	record_stamp = fr->get_record_stamp();
	prev_record_time = record_time;
	real_time = record_time;
	check_time = record_time;
	paused = false;
	seeking = true;
	return true;
}

void game_record::set_replay_setting(unsigned int setting, bool enable)
{
	setting = (1 << setting);

	if (enable)
		replay_setting |= setting;
	else
		replay_setting &= ~setting;
}

void game_record::apply_cur_setting()
{
	if (replay_setting & (1 << REPLAY_PRIVATE_CHAT))
		filter_map.erase(PROTOCOL_PRIVATECHAT);
	else
		filter_map[PROTOCOL_PRIVATECHAT] = 1;

	if (replay_setting & (1 << REPLAY_WORLD_CHAT))
		filter_map.erase(PROTOCOL_WORLDCHAT);
	else
		filter_map[PROTOCOL_WORLDCHAT] = 1;

	if (replay_setting & (1 << REPLAY_COMMON_CHAT))
		filter_map.erase(PROTOCOL_CHATMESSAGE);
	else
		filter_map[PROTOCOL_CHATMESSAGE] = 1;
}

void game_record::calc_host_pos()
{
	if (real_time > record_time || real_time < prev_record_time || prev_record_time >= record_time)
		return;

	float ratio = (float)(real_time - prev_record_time) / (record_time - prev_record_time);
	A3DVECTOR3 diff = host_cur_frame_pos - host_prev_frame_pos;

	if (diff.IsZero() || diff.SquaredMagnitude() > 100.0f)
		return;

	host_pos = host_prev_frame_pos + diff * ratio;
}

bool event_record_protocol::_unmarshal(GNET::Marshal::OctetsStream& os)
{
	try
	{
		Protocol::Type type;
		os >> type;

		if ((protocol = Protocol::Create(type)))
		{
			os >> *protocol;
			return true;
		}

		return false;
	}
	catch (Marshal::Exception &)
	{
		if (protocol)
		{
			protocol->Destroy();
			protocol = 0;
		}

		return false;
	}
}

bool event_record_protocol::_marshal(GNET::Marshal::OctetsStream& os)
{
	try
	{
		os << protocol->GetType();
		os << *protocol;
		return true;
	}
	catch (Marshal::Exception &)
	{
		return false;
	}
}

bool event_record_msg::_unmarshal(GNET::Marshal::OctetsStream& os)
{
	try
	{
		GNET::Octets o;
		os >> o;

		if (o.size() != sizeof(msg))
			return false;

		memcpy(&msg, o.begin(), sizeof(msg));
		return true;
	}
	catch (Marshal::Exception &)
	{
		return false;
	}
}

bool event_record_msg::_marshal(GNET::Marshal::OctetsStream& os)
{
	try
	{
		os << Octets(&msg, sizeof(msg));
		return true;
	}
	catch (Marshal::Exception &)
	{
		return false;
	}
}

bool event_record_data::_unmarshal(GNET::Marshal::OctetsStream& os)
{
	try
	{
		os >> data_type;
		os >> data;
		return true;
	}
	catch (Marshal::Exception &)
	{
		return false;
	}
}

bool event_record_data::_marshal(GNET::Marshal::OctetsStream& os)
{
	try
	{
		os << data_type;
		os << data;
		return true;
	}
	catch (Marshal::Exception &)
	{
		return false;
	}
}

void EnumRecordFiles(const char* szDir, record_info_vec& Infos)
{
	Infos.reserve(64);
	abase::vector<char*> paths;
	find_files(szDir, ".zgr", paths);

	for (size_t i = 0; i < paths.size(); i++)
	{
		game_record _gr;

		if (_gr.open_record(paths[i]))
		{
			record_info info;
			strcpy(info.path, paths[i]);
			wcscpy(info.title, _gr.get_title());
			info.save_time = _gr.get_save_time();
			info.total_time = _gr.get_total_time();
			_gr.close_record();
			Infos.push_back(info);
		}

		delete[] paths[i];
	}
}
