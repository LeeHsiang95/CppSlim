#include "SlimList.hpp"
#include "SlimUtil.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "StatementExecutor.hpp"

//static local variables
typedef struct Node Node;


Node::Node(const std::string& str):str_data_(str){
}

Node::~Node() {
}

SlimList::~SlimList()
{
}

std::shared_ptr<Node> SlimList::CreateIterator() {
	return GetHead();
}

std::shared_ptr<Node> SlimList::Advance(std::shared_ptr<Node> iterator) {
	if (iterator)
		return iterator->next_;

	return iterator;
}

void SlimList::AddBuffer(const std::string& str)
{
	auto newNode = std::make_shared<Node>(str);
	newNode->next_ = nullptr;
	newNode->list_ = nullptr;

	insertNode(newNode);
}


void SlimList::AddString(const std::string& str)
{
	AddBuffer(str);
}

void SlimList::AddList(std::shared_ptr<SlimList> element)
{
	auto embedded = element->Serialize();
	AddString(embedded);
}

void SlimList::PopHead()
{
	assert(head != nullptr);
	nodes_.erase(nodes_.begin());
}

int SlimList::GetLength()
{
	return nodes_.size();
}

bool SlimList::Equals(std::shared_ptr<SlimList> other) {
	if (GetLength() != other->GetLength())
		return false;

	std::list<std::shared_ptr<Node>>::iterator p;
	std::list<std::shared_ptr<Node>>::iterator q;
	for (p = GetBegin(), q = other->GetBegin(); p != nodes_.end(); ++p, ++q)
	{
		if ((*p)->str_data_ != (*q)->str_data_)
			return false;
	}

	return true;
}

std::shared_ptr<Node> SlimList::GetNodeAt(int index)
{
	if (index >= GetLength())
		return 0;

	auto iter = nodes_.begin();
	for (int i = 0; i < index; ++i) {
		++iter;
	}

	return *iter;
}

std::shared_ptr<SlimList> SlimList::GetListAt(int index)
{
	std::shared_ptr<Node> node = GetNodeAt(index);
	return node->GetList();
}

std::shared_ptr<SlimList> Node::GetList()
{
	if (list_ == nullptr)
		list_ = SlimList::IsValid(str_data_) ? std::make_shared<SlimList>(str_data_) : nullptr;

	return list_;
}

std::string SlimList::GetStringAt(int index)
{
	std::shared_ptr<Node> node = GetNodeAt(index);
	if(node == nullptr)
		return {};

	return node->GetString();
}

std::string Node::GetString()
{
	return str_data_;
}

double SlimList::GetDoubleAt(int index)
{
  auto speed_s = GetStringAt(index);
  return stod(speed_s);
}

static std::string parseHashCell(const char ** cellStart)
{
	const char * cellValue = *cellStart + strlen("<td>");
	const char * cellStop = strstr(cellValue, "</td>");

	int length = (int)(cellStop - cellValue);
	std::string buf(cellValue, length);
	
	*cellStart = strstr(cellStop + strlen("<td>"), "<td>");
	
	return buf;
}

static std::shared_ptr<SlimList> parseHashEntry(const char* row) {
	auto element = std::make_shared<SlimList>();

	const char* cellStart = strstr(row, "<td>");

	const auto hashKey = parseHashCell(&cellStart);
	element->AddString(hashKey);

	const auto hashValue = parseHashCell(&cellStart);
	element->AddString(hashValue);

	return element;
}

static std::shared_ptr<SlimList> SlimList_deserializeHash(std::string& serializedHash)
{
	auto hash = std::make_shared<SlimList>();

	const char* row = strstr(serializedHash.c_str(), "<tr>");
	while (row != nullptr) {
		auto element = parseHashEntry(row);
		hash->AddList(element);
		row = strstr(row + strlen("<tr>"), "<tr>");
	}

	return hash;
}

std::shared_ptr<SlimList> SlimList::GetHashAt(int index)
{
	return SlimList_deserializeHash(GetStringAt(0));
}

void SlimList::ReplaceAt(int index, char const * replacementString)
{
	std::shared_ptr<Node> node = GetNodeAt(index);
	node->Replace(replacementString);
}

void Node::Replace(const std::string& replacementString) {
	if (list_ != nullptr) {
		list_ = nullptr;
	}
	auto newString = replacementString;
	str_data_ = newString;
}

void SlimList::insertNode(std::shared_ptr<Node> node) {
	if (GetLength() == 0)
	{
	}
	else
	{
		GetTail()->next_ = node;
	}

	nodes_.push_back(node);
}

std::shared_ptr<Node> SlimList::AdvanceBy(std::shared_ptr<Node> iterator, int amount) {
	std::shared_ptr<Node> result = iterator;
	for (int i = 0; i < amount; i++) {
		iterator = Advance(iterator);
	}

	return iterator;
}

std::shared_ptr<SlimList> SlimList::GetTailAt(int index)
{
	auto tail = std::make_shared<SlimList>();

	std::shared_ptr<Node> iterator = CreateIterator();
	iterator = AdvanceBy(iterator, index);

	while (iterator != nullptr)
	{
		tail->AddString(iterator->GetString());
		iterator = Advance(iterator);
	}

	return tail;
}

std::string SlimList::ToString() {
	std::string result;
	result += "[";

	std::shared_ptr<Node> iterator = CreateIterator();
	while (iterator != nullptr) {
		auto sublist = iterator->GetList();

		if (sublist != nullptr) {
			result += sublist->ToString();
		} else {
			result += "\"";
			result += iterator->GetString();
			result += "\"";
		}

		if (iterator->next_ != nullptr) {
			result += ", ";
		}
		iterator = Advance(iterator);
	}

	result += "]";

	return result;
}


int SlimList::readLength(char const** readPtr)
{
	int length = atoi(*readPtr);
	*readPtr += 6;
	return length;
}

int SlimList::ByteLength(int characterLength, char const* current)
{
	unsigned char const* p;
	int chars = 0;
	int bytes = 0;
	for (p = (unsigned char const*)current; chars <= characterLength; p++)
	{
		bytes++;
		if (CSlim_IsCharacter(*p) == 1)
			chars++;
	}
	if (chars > characterLength)
		bytes--;
	return bytes;
}

SlimList::SlimList(const std::string& raw_data) {
	if (raw_data.empty())
		return;

	auto tmp = raw_data;
	auto current = tmp.c_str();

	if (*current != ('['))
	{
		nodes_.clear();
		return;
	}
	current++;

	SlimString slim_str(current);
	auto listLength = slim_str.Length();
	current += 6;

	if (*current != (':'))
	{
		nodes_.clear();
		return;
	}
	current++;

	while (listLength--)
	{
		SlimString slim_str(current);
		auto characterLength = slim_str.Length();
		current += 6;

		if (*current != (':'))
		{
			nodes_.clear();
			return;
		}
		current++;

		std::string tmp(current, characterLength);
		AddBuffer(tmp);
		current += characterLength;
		if (*current != (':'))
		{
			nodes_.clear();
			return;
		}
		current++;
	}

	if (*current != (']'))
	{
		nodes_.clear();
		return;
	}
	current++;
}

bool SlimList::IsValid(const std::string& raw_data) {
	int listLength;
	char const* current = 0;

	if (raw_data.empty())
		return false;

	current = raw_data.c_str();

	if (*current != '[')
		return false;

	current++;

	listLength = readLength(&current);

	if (*current != ':')
		return false;

	current++;

	while (listLength--) {
		int characterLength = readLength(&current);
		if (*current != ':')
			return false;

		current++;

		int byteLength = ByteLength(characterLength, current);
		current += byteLength;
		if (*current != ':')
			return false;

		current++;
	}

	if (*current != (']'))
		return false;

	return true;
}

static const char* stringForNullNode = "Null";

enum { LIST_OVERHEAD = 9, ELEMENT_OVERHEAD = 8 };

std::string SlimList::GetStringWithNullAsANormalString(std::shared_ptr<Node> iterator)
{
	auto nodeString = iterator->GetString();

	if (nodeString.empty())
		nodeString = stringForNullNode;

	return nodeString;
}

long SlimList::FieldLength(const std::string& nodeString)
{
	long fieldlength = 0;
	for (int i = 0; i< nodeString.size(); ++i)
		if (CSlim_IsCharacter(nodeString[i]) == 1)
			fieldlength++;
		
	return fieldlength;
}

int SlimList::SerializedLength()
{
	int length = LIST_OVERHEAD;

	std::shared_ptr<Node> iterator = CreateIterator();
	while (iterator != nullptr) {
		length += GetStringWithNullAsANormalString(iterator).size() + ELEMENT_OVERHEAD;

		iterator = Advance(iterator);
	}

	return length;
}

std::string SlimList::Serialize()
{
	std::string buf;
	char tmp[128];
	int listLength = GetLength();

	sprintf(tmp, "[%06d:", listLength);
	buf += tmp;

	std::shared_ptr<Node> iterator = CreateIterator();
	while (iterator != nullptr) {
		auto nodeString = GetStringWithNullAsANormalString(iterator);
		sprintf(tmp, "%06ld:%s:", FieldLength(nodeString), nodeString.c_str());
		buf += tmp;

		iterator = Advance(iterator);
	}
	buf += "]";
	return buf;
}

void SlimList::AddResult(const std::string& id, const std::string& result)
{
	auto pair = std::make_shared<SlimList>();
	pair->AddString(id);
	pair->AddString(result);
	AddList(pair);
}

std::shared_ptr<SlimList> SlimList::Execute(std::shared_ptr<StatementExecutor> executor)
{
	auto results = std::make_shared<SlimList>();

	std::shared_ptr<Node> iterator = CreateIterator();
	while (iterator != nullptr) {
		auto instruction = iterator->GetList();
		auto id = instruction->GetStringAt(0);
		auto result = instruction->Dispatch(executor);
		results->AddResult(id, result);

		iterator = Advance(iterator);
	}

	return results;
}

std::string SlimList::InvalidCommand() {
	auto id = GetStringAt(0);
	auto command = GetStringAt(1);
	static char msg[128];
	snprintf(msg, (size_t)128, "__EXCEPTION__:message:<<INVALID_STATEMENT: [\"%s\", \"%s\"].>>", id.c_str(), command.c_str());
	return std::string(msg);
}

std::string SlimList::MalformedInstruction() {
	char msg[128];

	auto listAsAString = ToString();
	snprintf(msg, (size_t)128, "__EXCEPTION__:message:<<MALFORMED_INSTRUCTION %s.>>", listAsAString.c_str());

	return std::string(msg);
}

std::string SlimList::Import() {
	return std::string("OK");
}

std::string SlimList::Make(std::shared_ptr<StatementExecutor> executor) {
	auto instanceName = GetStringAt(2);
	auto className = GetStringAt(3);
	auto args = GetTailAt(4);
	auto result = executor->Make(instanceName, className, args);
	return result;
}

std::string SlimList::Call(std::shared_ptr<StatementExecutor> executor) {
	if (GetLength() < 4)
		return MalformedInstruction();

	auto instanceName = GetStringAt(2);
	auto methodName = GetStringAt(3);
	auto args = GetTailAt(4);
	auto result = executor->Call(instanceName, methodName, args);
	return result;
}

std::string SlimList::CallAndAssign(std::shared_ptr<StatementExecutor> executor) {
	if (GetLength() < 5)
		return MalformedInstruction();

	auto symbolName = GetStringAt(2);
	auto instanceName = GetStringAt(3);
	auto methodName = GetStringAt(4);
	auto args = GetTailAt(5);
	auto result = executor->Call(instanceName, methodName, args);
	executor->SetSymbol(symbolName, result);
	return result;
}

std::string SlimList::Dispatch(std::shared_ptr<StatementExecutor> executor) {
	auto command = GetStringAt(1);
	if (command == "import")
		return Import();
	else if (command == "make")
		return Make(executor);
	else if (command == "call")
		return Call(executor);
	else if (command == "callAndAssign")
		return CallAndAssign(executor);
	else
		return InvalidCommand();
}
