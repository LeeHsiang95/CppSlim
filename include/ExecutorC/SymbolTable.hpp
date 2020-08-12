#pragma once
#include <string>
#include <memory>
#include <list>
#include <map>

class SlimList;

class SymbolTable {
public:
	SymbolTable();
	~SymbolTable();

	std::string FindSymbol(const std::string& name);
	void SetSymbol(const std::string& symbol, const std::string& value);
	int GetSymbolLength(const std::string& symbol);

	void replaceSymbols(std::shared_ptr<SlimList>);

	std::string replaceString(const std::string& str);
	std::string replaceStringFrom(const char* string, const char* from);
	int SymbolTable::lengthOfSymbol(const char* start);

	std::map<std::string, std::string> symbols_;
};
