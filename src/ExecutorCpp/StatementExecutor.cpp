#include "StatementExecutor.hpp"
#include "SlimList.hpp"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "SlimUtil.hpp"
#include "assert.h"
#include <ctype.h>
#include "compatibility.hpp"


static int isLibraryInstanceName(const std::string& instanceName);
static int lengthOfSymbol(const char *);
static void nullptr_Destroy(void* self);
static void* nullptr_Create(std::shared_ptr<StatementExecutor> self, std::shared_ptr<SlimList> args);


StatementExecutor::StatementExecutor() {
	symbol_table_ = std::make_shared<SymbolTable>();
}

static int compareNamesIgnoreUnderScores(const char * name1, const char * name2)
{
	while (*name1 && *name2)
	{
		if (*name1 == *name2) { name1++; name2++;}
		else if ((*name1) == '_') name1++;
		else if (*name2 == '_') name2++;
		else return 0;
	}
	if (*name1 != *name2)
		return 0;
	return 1;
}

std::shared_ptr<InstanceNode> StatementExecutor::GetInstanceNode(const std::string& instanceName)
{
	for (auto iter = instances_.begin(); iter != instances_.end(); ++iter) {
		if (compareNamesIgnoreUnderScores((*iter)->name_.c_str(), instanceName.c_str())) {
			return *iter;
		}
	}
	return nullptr;
}

StatementExecutor::~StatementExecutor() {
}

std::string InstanceNode::InstanceCall(const std::string& methodName, std::shared_ptr<SlimList> args) {
	auto methodNode = GetMethodNode(methodName);
	if (methodNode) {
		return methodNode(args);
	}

	return "";
}

bool InstanceNode::HasMethod(const std::string& methodName) {
	auto methodNode = GetMethodNode(methodName);
	return bool(methodNode);
}

std::shared_ptr<InstanceNode> FixtureNode::CreateInstance(std::shared_ptr<SlimList> args) {
	return creator_(args);
}


std::string StatementExecutor::Make(const std::string& instanceName, const std::string& className, std::shared_ptr<SlimList> args){
	std::shared_ptr<FixtureNode> fixtureNode = findFixture(className);
	if (fixtureNode) {
		symbol_table_->replaceSymbols(args);
		auto instance = fixtureNode->creator_(args);
		if (isLibraryInstanceName(instanceName)) {
			library_instances_.push_front(instance);
		}
		else {
			instances_.push_front(instance);
		}

		if (instance != nullptr) {
			instance->name_ = instanceName;
			
			userMessage = nullptr;
			return "OK";
		}
		else {
			const char* formatString = "__EXCEPTION__:message:<<COULD_NOT_INVOKE_CONSTRUCTOR %.32s %.32s.>>";
			snprintf(message, 120, formatString, className, userMessage ? userMessage : "");
			return message;
		}
	}
	const char * formatString = "__EXCEPTION__:message:<<NO_CLASS %.32s.>>";
	snprintf(message, 120, formatString, className);
	return message;
}

std::string StatementExecutor::Call(const std::string& instanceName, const std::string& methodName, std::shared_ptr<SlimList> args) {
	std::shared_ptr<InstanceNode> instanceNode = GetInstanceNode(instanceName);
	if (instanceNode)
	{
		if (instanceNode->HasMethod(methodName)) {
			symbol_table_->replaceSymbols(args);
			return instanceNode->InstanceCall(methodName, args);
		}

		for (auto iter = library_instances_.begin(); iter != library_instances_.end(); ++iter) {
			if ((*iter)->HasMethod(methodName)) {
				symbol_table_->replaceSymbols(args);
				return (*iter)->InstanceCall(methodName, args);
			}
		}

		const char* formatString = "__EXCEPTION__:message:<<NO_METHOD_IN_CLASS %.32s[%d]>>";
		snprintf(message, 120, formatString, methodName.c_str(), args->GetLength());
		return message;
	}

	const char* formatString = "__EXCEPTION__:message:<<NO_INSTANCE %.32s.>>";
	snprintf(message, 120, formatString, instanceName.c_str());
	return message;
}

std::shared_ptr<FixtureNode> StatementExecutor::findFixture(const std::string& classNameWithSymbols)
{
	const auto className = symbol_table_->replaceString(classNameWithSymbols);
	std::shared_ptr<FixtureNode> fixtureNode = findFixtureByName(className);
	return fixtureNode;
}

static int isLibraryInstanceName(const std::string& instanceName)
{
	return CSlim_StringStartsWith(instanceName.c_str(), "library");
}

void StatementExecutor::pushLibraryInstance(std::shared_ptr<InstanceNode> instanceNode)
{
	library_instances_.push_front(instanceNode);
}

void StatementExecutor::pushInstance(std::shared_ptr<InstanceNode> instanceNode)
{
	instances_.push_front(instanceNode);
}

static int lengthOfSymbol(const char * start)
{
	int length = 0;
	while(isalnum(*start))
	{
		start++;
		length ++;
	}
	return length;
}

std::shared_ptr<InstanceNode> StatementExecutor::Instance(const std::string& instanceName){
	return GetInstanceNode(instanceName);
}

void StatementExecutor::AddFixture(Fixture fixture) {
	fixture(shared_from_this());
}

void StatementExecutor::RegisterFixture(const std::string& className, std::function<std::shared_ptr<InstanceNode>(std::shared_ptr<SlimList>)> creator)
{
	auto fixture = std::make_shared<FixtureNode>();
	fixture->name_ = className;
	fixture->creator_ = creator;
	fixtures_.push_back(fixture);
}

std::shared_ptr<FixtureNode> StatementExecutor::findFixtureByName(const std::string& className)
{
	for (auto iter = fixtures_.begin(); iter != fixtures_.end(); ++iter) {
		if (compareNamesIgnoreUnderScores((*iter)->name_.c_str(), className.c_str())) {
			return *iter;
		}
	}
	return nullptr;
}

void StatementExecutor::SetSymbol(const std::string& symbol, const std::string& value) {
	symbol_table_->SetSymbol(symbol, value);
}

void StatementExecutor::ConstructorError(char const* message) {
	userMessage = message;
}

std::string StatementExecutor_FixtureError(const std::string& message) {
	static char buffer[128];
	const char * formatString = "__EXCEPTION__:message:<<%.100s.>>";
	snprintf(buffer, sizeof(buffer), formatString, message);
	return buffer;
}

static void* nullptr_Create(std::shared_ptr<StatementExecutor> self, std::shared_ptr<SlimList> args)
{
	return nullptr;
}

static void nullptr_Destroy(void* self)
{
}


