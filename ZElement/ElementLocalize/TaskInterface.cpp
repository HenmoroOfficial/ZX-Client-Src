#include "TaskInterface.h"
#include "StdAfx.h"
#include "..\ZElementClient\Network\IOLib\gnsecure.h"


unsigned long TaskInterface::GetCurTime()
{
	return 0;
}

void TaskInterface::WriteLog(int,int,int,char const *)
{

}
int TaskInterface::GetTimeZoneBias()
{
	return 0;
}

bool TaskInterface::CalcFileMD5(const char* szFile, unsigned char md5[16])
{ 
	FILE* fp = fopen(szFile, "rb");

	if (!fp)
		return false;

	fseek(fp, 0, SEEK_END);
	size_t sz = ftell(fp);

	if (sz == 0)
	{
		fclose(fp);
		return false;
	}

	fseek(fp, 0, SEEK_SET);
	char* buf = new char[sz];
	fread(buf, sz, 1, fp);
	fclose(fp);
	GNET::Octets input(buf, sz);
	GNET::Octets output = GNET::MD5Hash::Digest(input);
	memcpy(md5, output.begin(), 16);
	delete[] buf;
	return true;
}