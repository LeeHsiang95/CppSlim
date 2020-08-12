#include <string.h>
#include <stdlib.h>
#include "SlimUtil.hpp"
#include <assert.h>


int CSlim_StringStartsWith(const char* string, const char* prefix)
{
	return strncmp(string, prefix, strlen(prefix)) == 0;
}

int CSlim_MapToIntFrom(MapStringInt* map, const char* name)
{
	MapStringInt* p = map;
	while (p->string != nullptr && 0 != strcmp(name, p->string))
	{
		p++;
	}

	return p->n;
}

const char* CSlim_MapToStringFrom(MapStringInt* map, int n)
{
	MapStringInt* p = map;
	while (p->string != nullptr && n != p->n)
	{
		p++;
	}

	return p->string;
}

bool CSlim_IsCharacter(unsigned char byte)
{
	return ((byte < (unsigned char const)0x80) || (byte > (unsigned char const)0xBF));
}


SlimString::SlimString(const std::string& raw_data) :
	raw_str_(raw_data) {
}

int SlimString::Length() const {
	auto tmp = raw_str_.substr(0, 6);
	return std::stoi(tmp);
}

std::string SlimString::Content() const {
	return raw_str_.substr(7, Length());
}

bool SlimString::IsSlimString(const std::string& data) {
	try {
		if (data.size() <= 6)
			return false;

		if (data[6] != ':')
			return false;

		if (std::stoi(data.substr(0, 6)) < 0)
			return false;

		if (data.size() - 7 != std::stoi(data.substr(0, 6)))
			return false;
	}
	catch (...) {
		return false;
	}

	return true;
}