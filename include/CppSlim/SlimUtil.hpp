#pragma once
#include <string>

int CSlim_StringStartsWith(const char* string, const char* prefix);

typedef struct MapStringInt
{
	const char* string;
	int n;
} MapStringInt;

int CSlim_MapToIntFrom(MapStringInt*, const char*);
const char* CSlim_MapToStringFrom(MapStringInt*, int);
bool CSlim_IsCharacter(unsigned char byte);


class SlimString {
public:
	static bool IsSlimString(const std::string& data);
	
	SlimString(const std::string& raw_data);

	int Length() const;
	std::string Content() const;
	
private:
	std::string raw_str_;
};