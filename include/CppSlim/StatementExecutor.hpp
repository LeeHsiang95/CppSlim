#pragma once

#include <string>
#include <functional>
#include "SlimList.hpp"
#include "SymbolTable.hpp"

class InstanceNode;
class StatementExecutor;

typedef void(*Fixture)(std::shared_ptr<StatementExecutor>);


class FixtureNode : public std::enable_shared_from_this<FixtureNode> {
public:
	FixtureNode() = default;

	std::shared_ptr<InstanceNode> CreateInstance(std::shared_ptr<SlimList> args);

	std::string name_;
	std::function<std::shared_ptr<InstanceNode>(std::shared_ptr<SlimList>)> creator_;
};

class InstanceNode : public std::enable_shared_from_this<InstanceNode> {
public:
	bool HasMethod(const std::string& methodName);
	std::string InstanceCall(const std::string& methodName, std::shared_ptr<SlimList> args);

	virtual std::function<std::string(std::shared_ptr<SlimList>)> GetMethodNode(const std::string& method_name) = 0;

	std::string name_;
	//std::shared_ptr<InstanceNode> instance_{ nullptr };
	//std::shared_ptr<FixtureNode> fixture_{ nullptr };
};

class StatementExecutor : public std::enable_shared_from_this<StatementExecutor> {
public:
	StatementExecutor();
	~StatementExecutor();

	void AddFixture(Fixture);
	void RegisterFixture(const std::string& className, std::function<std::shared_ptr<InstanceNode>(std::shared_ptr<SlimList>)> creator);

	std::string Make(const std::string& instanceName, const std::string& className, std::shared_ptr<SlimList> args);
	std::string Call(const std::string& instanceName, const std::string& methodName, std::shared_ptr<SlimList>);
	std::shared_ptr<InstanceNode> Instance(const std::string& instanceName);
	void SetSymbol(const std::string& symbol, const std::string& value);

	void ConstructorError(char const* message);

protected:
	std::shared_ptr<InstanceNode> GetInstanceNode(const std::string& instanceName);
	std::shared_ptr<FixtureNode> findFixture(const std::string& classNameWithSymbols);
	std::shared_ptr<FixtureNode> findFixtureByName(const std::string& className);
	void pushLibraryInstance(std::shared_ptr<InstanceNode> instanceNode);
	void pushInstance(std::shared_ptr<InstanceNode> instanceNode);

private:
	std::list<std::shared_ptr<FixtureNode>> fixtures_;
	std::list<std::shared_ptr<InstanceNode>> instances_;
	std::list<std::shared_ptr<InstanceNode>> library_instances_;

	std::shared_ptr<SymbolTable> symbol_table_{ nullptr };
	char message[120];
	char const* userMessage;
};

std::string StatementExecutor_FixtureError(const std::string& message);
