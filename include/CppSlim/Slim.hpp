#pragma once

#include "SlimConnectionHandler.hpp"
#include "StatementExecutor.hpp"

class ISlim : public std::enable_shared_from_this<ISlim>
{
public:
	virtual std::string HandleMessage(const std::string& message) = 0;
};

class Slim : public ISlim
{
public:
	std::shared_ptr<StatementExecutor> statementExecutor;

public:
	Slim();
	~Slim();

	int HandleConnection(std::shared_ptr<ComLink> comLink);

	std::string HandleMessage(const std::string& message) override;
};
