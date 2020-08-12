#include <stdlib.h>
#include <string.h>

#include "Slim.hpp"
#include "SlimList.hpp"

void AddFixtures(std::shared_ptr<StatementExecutor>);

	Slim::Slim() {
		statementExecutor = std::make_shared<StatementExecutor>();
		AddFixtures(statementExecutor);
	}

	Slim::~Slim() {
	}

	int Slim::HandleConnection(std::shared_ptr<ComLink> comLink)
	{
		int result = 0;
		auto connection = std::make_shared<SlimConnectionHandler>(comLink);
		connection->RegisterSlimMessageHandler(shared_from_this());
		result = connection->Run();
		return result;
	}

	std::string Slim::HandleMessage(const std::string& message)
	{
		auto instructions = std::make_shared<SlimList>(message);
		auto results = instructions->Execute(statementExecutor);
		auto response = results->Serialize();
		return response;
	}
