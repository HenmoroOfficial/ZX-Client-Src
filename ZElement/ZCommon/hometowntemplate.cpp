#include "hometowntemplate.h"

static const int HOMETOWN_TEMPLATE_VERSION=1;
using namespace HOMETOWN;
/************************************************************************ 
 template structure storing and loading related routines
*************************************************************************/
int save_vector(const std::vector<int>&, FILE *);
int load_vector(std::vector<int>&, FILE *);
int save_vector(const std::vector<__int64>&, FILE *);
int load_vector(std::vector<__int64>&, FILE *);
int save_vector(const std::vector<LEVEL_CONFIG>&, FILE *);
int load_vector(std::vector<LEVEL_CONFIG>&, FILE *);
int save_vector(const std::vector<CROP_ESSENCE>&, FILE *);
int load_vector(std::vector<CROP_ESSENCE>&, FILE *);
int save_vector(const std::vector<FRUIT_ESSENCE>&, FILE *);
int load_vector(std::vector<FRUIT_ESSENCE>&, FILE *);
int save_vector(const std::vector<TOOL_FUNC>&, FILE *);
int load_vector(std::vector<TOOL_FUNC>&, FILE *);
int save_vector(const std::vector<TOOL_ESSENCE>&, FILE *);
int load_vector(std::vector<TOOL_ESSENCE>&, FILE *);

#define CHECK_SAVE(t) \
do\
{\
	if (fwrite(&(t), sizeof(t), 1, file)!=1)\
		return -1;\
}\
while(0)

#define CHECK_LOAD(t) \
do\
{\
	if (fread(&(t), sizeof(t), 1, file)!=1)\
		return -1;\
}\
while(0)

#define CHECK_SAVE_POINTER(p, count) \
do\
{\
	if (fwrite(p, sizeof(*(p)), (count), file)!=(count))\
		return -1;\
}\
while(0)

#define CHECK_LOAD_POINTER(p, count) \
do\
{\
	if (fread(p, sizeof(*(p)), (count), file)!=(count))\
		return -1;\
}\
while(0)

#define CHECK_SAVE_ELEMENT(elem) \
do\
{\
	if (save_element((elem), file) == -1)\
		return -1;\
}\
while(0)

#define CHECK_LOAD_ELEMENT(elem) \
do\
{\
	if (load_element((elem), file) == -1)\
		return -1;\
}\
while(0)

#define CHECK_SAVE_VECTOR(arr)  \
do\
{\
	if (save_vector((arr), file) == -1)\
		return -1;\
}\
while(0)

#define CHECK_LOAD_VECTOR(arr)  \
do\
{\
	if (load_vector((arr), file) == -1)\
		return -1;\
}\
while(0)

template <typename T>
static int save_element(const T &elem, FILE *file)
{
	CHECK_SAVE(elem);
	return 0;
}

template <typename T>
static int load_element(T &elem, FILE *file)
{
	CHECK_LOAD(elem);
	return 0;
}

static int save_element(const FARM_CONFIG &elem, FILE *file)
{
	CHECK_SAVE(elem.id);
	CHECK_SAVE_POINTER(elem.name, 32);
	CHECK_SAVE(elem.max_level);
	CHECK_SAVE(elem.max_field_count);
	CHECK_SAVE(elem.init_pocket_caps);
	CHECK_SAVE_VECTOR(elem.level_config);
	CHECK_SAVE_VECTOR(elem.upgrade_config);
	CHECK_SAVE_VECTOR(elem.crop_list);
	CHECK_SAVE_VECTOR(elem.tool_list);
	CHECK_SAVE(elem.money_local);
	CHECK_SAVE(elem.money_universal);
	return 0;
}

static int load_element(FARM_CONFIG &elem, FILE *file)
{
	CHECK_LOAD(elem.id);
	CHECK_LOAD_POINTER(elem.name, 32);
	CHECK_LOAD(elem.max_level);
	CHECK_LOAD(elem.max_field_count);
	CHECK_LOAD(elem.init_pocket_caps);
	CHECK_LOAD_VECTOR(elem.level_config);
	CHECK_LOAD_VECTOR(elem.upgrade_config);
	CHECK_LOAD_VECTOR(elem.crop_list);
	CHECK_LOAD_VECTOR(elem.tool_list);
	CHECK_LOAD(elem.money_local);
	CHECK_LOAD(elem.money_universal);
	return 0;
}

static int save_element(const HOMETOWN_CONFIG &elem, FILE *file)
{
	CHECK_SAVE(elem.id);
	CHECK_SAVE_POINTER(elem.name, 32);
	CHECK_SAVE_VECTOR(elem.compo_list);
	return 0;
}

static int load_element(HOMETOWN_CONFIG &elem, FILE *file)
{
	CHECK_LOAD(elem.id);
	CHECK_LOAD_POINTER(elem.name, 32);
	CHECK_LOAD_VECTOR(elem.compo_list);
	return 0;
}

int save_vector(const std::vector<int> &arr, FILE *file)
{
	size_t ts = sizeof(int);
	CHECK_SAVE(ts);
	size_t s = arr.size();
	CHECK_SAVE(s);
	if(s>0) 
	{
		std::vector<int>::const_iterator it, ie = arr.end();
		for (it = arr.begin(); it != ie; ++it)
		{
			CHECK_SAVE_ELEMENT(*it);
		}
	}
	return 0;
}

int load_vector(std::vector<int> &arr, FILE *file)
{
	arr.clear();
	size_t ts;
	CHECK_LOAD(ts);
	if(ts != sizeof(int)) return -1;
	size_t s;
	CHECK_LOAD(s);
	if(s>0) 
	{       
		arr.resize(s);
		for(size_t i=0; i<s; i++)
		{ 
			CHECK_LOAD_ELEMENT(arr[i]);
		}       
	} 
	return 0;
}

int save_vector(const std::vector<__int64> &arr, FILE *file)
{
	size_t ts = sizeof(__int64);
	CHECK_SAVE(ts);
	size_t s = arr.size();
	CHECK_SAVE(s);
	if(s>0) 
	{
		std::vector<__int64>::const_iterator it, ie = arr.end();
		for (it = arr.begin(); it != ie; ++it)
		{
			CHECK_SAVE_ELEMENT(*it);
		}
	}
	return 0;
}

int load_vector(std::vector<__int64> &arr, FILE *file)
{
	arr.clear();
	size_t ts;
	CHECK_LOAD(ts);
	if(ts != sizeof(__int64)) return -1;
	size_t s;
	CHECK_LOAD(s);
	if(s>0) 
	{       
		arr.resize(s);
		for(size_t i=0; i<s; i++)
		{ 
			CHECK_LOAD_ELEMENT(arr[i]);
		}       
	} 
	return 0;
}

int save_vector(const std::vector<LEVEL_CONFIG> &arr, FILE *file)
{
	size_t ts = sizeof(LEVEL_CONFIG);
	CHECK_SAVE(ts);
	size_t s = arr.size();
	CHECK_SAVE(s);
	if(s>0) 
	{
		std::vector<LEVEL_CONFIG>::const_iterator it, ie = arr.end();
		for (it = arr.begin(); it != ie; ++it)
		{
			CHECK_SAVE_ELEMENT(*it);
		}
	}
	return 0;
}

int load_vector(std::vector<LEVEL_CONFIG> &arr, FILE *file)
{
	arr.clear();
	size_t ts;
	CHECK_LOAD(ts);
	if(ts != sizeof(LEVEL_CONFIG)) return -1;
	size_t s;
	CHECK_LOAD(s);
	if(s>0) 
	{       
		arr.resize(s);
		for(size_t i=0; i<s; i++)
		{ 
			CHECK_LOAD_ELEMENT(arr[i]);
		}       
	} 
	return 0;
}

int save_vector(const std::vector<CROP_ESSENCE> &arr, FILE *file)
{
	size_t ts = sizeof(CROP_ESSENCE);
	CHECK_SAVE(ts);
	size_t s = arr.size();
	CHECK_SAVE(s);
	if(s>0) 
	{
		std::vector<CROP_ESSENCE>::const_iterator it, ie = arr.end();
		for (it = arr.begin(); it != ie; ++it)
		{
			CHECK_SAVE_ELEMENT(*it);
		}
	}
	return 0;
}

int load_vector(std::vector<CROP_ESSENCE> &arr, FILE *file)
{
	arr.clear();
	size_t ts;
	CHECK_LOAD(ts);
	if(ts != sizeof(CROP_ESSENCE)) return -1;
	size_t s;
	CHECK_LOAD(s);
	if(s>0) 
	{       
		arr.resize(s);
		for(size_t i=0; i<s; i++)
		{ 
			CHECK_LOAD_ELEMENT(arr[i]);
		}       
	} 
	return 0;
}

int save_vector(const std::vector<FRUIT_ESSENCE> &arr, FILE *file)
{
	size_t ts = sizeof(FRUIT_ESSENCE);
	CHECK_SAVE(ts);
	size_t s = arr.size();
	CHECK_SAVE(s);
	if(s>0) 
	{
		std::vector<FRUIT_ESSENCE>::const_iterator it, ie = arr.end();
		for (it = arr.begin(); it != ie; ++it)
		{
			CHECK_SAVE_ELEMENT(*it);
		}
	}
	return 0;
}

int load_vector(std::vector<FRUIT_ESSENCE> &arr, FILE *file)
{
	arr.clear();
	size_t ts;
	CHECK_LOAD(ts);
	if(ts != sizeof(FRUIT_ESSENCE)) return -1;
	size_t s;
	CHECK_LOAD(s);
	if(s>0) 
	{       
		arr.resize(s);
		for(size_t i=0; i<s; i++)
		{ 
			CHECK_LOAD_ELEMENT(arr[i]);
		}       
	} 
	return 0;
}

int save_vector(const std::vector<TOOL_FUNC> &arr, FILE *file)
{
	size_t ts = sizeof(TOOL_FUNC);
	CHECK_SAVE(ts);
	size_t s = arr.size();
	CHECK_SAVE(s);
	if(s>0) 
	{
		std::vector<TOOL_FUNC>::const_iterator it, ie = arr.end();
		for (it = arr.begin(); it != ie; ++it)
		{
			CHECK_SAVE_ELEMENT(*it);
		}
	}
	return 0;
}

int load_vector(std::vector<TOOL_FUNC> &arr, FILE *file)
{
	arr.clear();
	size_t ts;
	CHECK_LOAD(ts);
	if(ts != sizeof(TOOL_FUNC)) return -1;
	size_t s;
	CHECK_LOAD(s);
	if(s>0) 
	{       
		arr.resize(s);
		for(size_t i=0; i<s; i++)
		{ 
			CHECK_LOAD_ELEMENT(arr[i]);
		}       
	} 
	return 0;
}

int save_vector(const std::vector<TOOL_ESSENCE> &arr, FILE *file)
{
	size_t ts = sizeof(TOOL_ESSENCE);
	CHECK_SAVE(ts);
	size_t s = arr.size();
	CHECK_SAVE(s);
	if(s>0) 
	{
		std::vector<TOOL_ESSENCE>::const_iterator it, ie = arr.end();
		for (it = arr.begin(); it != ie; ++it)
		{
			CHECK_SAVE_ELEMENT(*it);
		}
	}
	return 0;
}


int load_vector(std::vector<TOOL_ESSENCE> &arr, FILE *file)
{
	arr.clear();
	size_t ts;
	CHECK_LOAD(ts);
	if(ts != sizeof(TOOL_ESSENCE)) return -1;
	size_t s;
	CHECK_LOAD(s);
	if(s>0) 
	{       
		arr.resize(s);
		for(size_t i=0; i<s; i++)
		{ 
			CHECK_LOAD_ELEMENT(arr[i]);
		}       
	} 
	return 0;
}

int HometownTemplate::_save_data(FILE *file)
{
	CHECK_SAVE(HOMETOWN_TEMPLATE_VERSION);

	CHECK_SAVE_VECTOR(_CROP_ESSENCE_vector);
	CHECK_SAVE_VECTOR(_FRUIT_ESSENCE_vector);
	CHECK_SAVE_VECTOR(_TOOL_FUNC_vector);
	CHECK_SAVE_VECTOR(_TOOL_ESSENCE_vector);
	CHECK_SAVE_ELEMENT(_FARM_CONFIG);
	CHECK_SAVE_ELEMENT(_HOMETOWN_CONFIG);
	return 0;
}

int HometownTemplate::_load_data(FILE *file)
{
	int version;
	CHECK_LOAD(version);
	if (version != HOMETOWN_TEMPLATE_VERSION) return -1;

	CHECK_LOAD_VECTOR(_CROP_ESSENCE_vector);
	CHECK_LOAD_VECTOR(_FRUIT_ESSENCE_vector);
	CHECK_LOAD_VECTOR(_TOOL_FUNC_vector);
	CHECK_LOAD_VECTOR(_TOOL_ESSENCE_vector);
	CHECK_LOAD_ELEMENT(_FARM_CONFIG);
	CHECK_LOAD_ELEMENT(_HOMETOWN_CONFIG);
	return 0;
}

void HometownTemplate::_setup_location()
{
	for (int i = 0; i < ID_SPACE_MAX; ++i)
		_id_loc_map[i].clear();

#define SETUP(SPACE, TYPE) \
do\
{\
	LOCATION loc;\
	loc.type = DT_##TYPE;\
	for (size_t i = 0; i < _##TYPE##_vector.size(); ++i)\
	{\
		loc.pos = i;\
		TYPE &dt = _##TYPE##_vector[i];\
		_id_loc_map[SPACE][dt.id] = loc;\
	}\
	_vec_start_ptr[DT_##TYPE] = &_##TYPE##_vector;\
}\
while(0)

	SETUP(ID_SPACE_ESSENCE, CROP_ESSENCE);
	SETUP(ID_SPACE_ESSENCE, FRUIT_ESSENCE);
	SETUP(ID_SPACE_FUNC, TOOL_FUNC);
	SETUP(ID_SPACE_ESSENCE, TOOL_ESSENCE);

#undef SETUP
}

void HometownTemplate::_setup_start()
{
	_vec_start_ptr.clear();
	_vec_start_ptr.resize(DT_MAX);
#define SETUP(TYPE) _vec_start_ptr[DT_##TYPE] = &_##TYPE##_vector
	SETUP(CROP_ESSENCE);
	SETUP(FRUIT_ESSENCE);
	SETUP(TOOL_FUNC);
	SETUP(TOOL_ESSENCE);
#undef SETUP
}

int HometownTemplate::save_data(const char *pathname)
{
	FILE *file = fopen(pathname, "wb+");
	if (file == NULL) return -1;
	int res = _save_data(file);
	fclose(file);
	return res;
}

int HometownTemplate::load_data(const char *pathname)
{
	FILE *file = fopen(pathname, "rb");
	if (file == NULL) return -1;
	int res = _load_data(file);
	fclose(file);
	if (res != -1)
		_setup_location();
	return res;
}

unsigned int HometownTemplate::get_first_data_id(ID_SPACE idspace, DATA_TYPE & datatype)
{
	if (idspace >= ID_SPACE_MAX)
	{
		datatype = DT_INVALID;
		return 0;
	}
	IDToLOCATIONMap &id_loc_map = _id_loc_map[idspace];
	IDToLOCATIONMap::iterator &id_loc_itr = _id_loc_itr[idspace];
	id_loc_itr = id_loc_map.begin();
	if (id_loc_itr == id_loc_map.end())
	{
		datatype = DT_INVALID;
		return 0;
	}
	else
	{
		datatype = id_loc_itr->second.type;
		return id_loc_itr->first;
	}
}

unsigned int HometownTemplate::get_next_data_id(ID_SPACE idspace, DATA_TYPE & datatype)
{
	if (idspace >= ID_SPACE_MAX)
	{
		datatype = DT_INVALID;
		return 0;
	}
	IDToLOCATIONMap &id_loc_map = _id_loc_map[idspace];
	IDToLOCATIONMap::iterator &id_loc_itr = _id_loc_itr[idspace];
	++id_loc_itr;
	if (id_loc_itr == id_loc_map.end())
	{
		datatype = DT_INVALID;
		return 0;
	}
	else
	{
		datatype = id_loc_itr->second.type;
		return id_loc_itr->first;
	}
}

unsigned int HometownTemplate::get_data_num(ID_SPACE idspace)
{
	if (idspace >= ID_SPACE_MAX) return 0;
	return (unsigned int)_id_loc_map[idspace].size();
}

DATA_TYPE HometownTemplate::get_data_type(unsigned int id, ID_SPACE idspace)
{
	if (idspace >= ID_SPACE_MAX) return DT_INVALID;
	IDToLOCATIONMap &id_loc_map = _id_loc_map[idspace];
	IDToLOCATIONMap::iterator it = id_loc_map.find(id);
	if (it != id_loc_map.end())
		return it->second.type;
	else
		return DT_INVALID;
}
