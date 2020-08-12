#include "SymbolTable.hpp"
#include <stdlib.h>
#include <string.h>
#include "SlimUtil.hpp"
#include "SlimList.hpp"


SymbolTable::SymbolTable()
{
}

SymbolTable::~SymbolTable()
{
}

std::string SymbolTable::FindSymbol(const std::string& name) {
	if (symbols_.find(name) == symbols_.end())
		return {};

	return symbols_[name];
}

void SymbolTable::SetSymbol(const std::string& symbol, const std::string& value) {
	symbols_[symbol] = value;
}

int SymbolTable::GetSymbolLength(const std::string& symbol) {
	auto symbolValue = FindSymbol(symbol);
	if (symbolValue.empty())
		return -1;

	return symbolValue.size();
}

void SymbolTable::replaceSymbols(std::shared_ptr<SlimList> list) {
	std::shared_ptr<Node> iterator = list->CreateIterator();
	while (iterator != nullptr) {
		auto string = iterator->GetString();
		std::shared_ptr<SlimList> embeddedList = SlimList::IsValid(string) ? std::make_shared<SlimList>(string) : nullptr;
		if (embeddedList == nullptr) {
			const auto replacedString = replaceString(string);
			iterator->Replace(replacedString);
		}
		else {
			replaceSymbols(embeddedList);
			auto serializedReplacedList = embeddedList->Serialize();
			iterator->Replace(serializedReplacedList);
		}
		iterator = list->Advance(iterator);
	}
}


std::string SymbolTable::replaceString(const std::string& str) {
	return replaceStringFrom(str.c_str(), str.c_str());
}

std::string SymbolTable::replaceStringFrom(const char* string, const char* from) {
	const char* dollarSign = strpbrk(from, "$");
	if (dollarSign)
	{
		int length = lengthOfSymbol(dollarSign + 1);
		auto symbolValue = FindSymbol(std::string(dollarSign + 1, length));
		if (!symbolValue.empty())
		{
			int valueLength = symbolValue.size();
			int bufferLength = strlen(string) + valueLength - (length + 1) + 1;

			std::string new_str;
			new_str += std::string(string, dollarSign - string);
			new_str += symbolValue;
			new_str += dollarSign + 1 + length;

			auto recursedString = replaceStringFrom(new_str.c_str(), new_str.c_str());
			return recursedString;
		}
		else
		{
			if (*(dollarSign + 1) == 0)
				return std::string(string);

			return replaceStringFrom(string, dollarSign + 1);
		}
	}
	return std::string(string);
}

int SymbolTable::lengthOfSymbol(const char* start)
{
	int length = 0;
	while (isalnum(*start))
	{
		start++;
		length++;
	}
	return length;
}
