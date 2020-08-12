#pragma once
#include "SlimList.hpp"
#include "Fixtures.hpp"

void TestSlim_Register(std::shared_ptr<StatementExecutor>);

class TestSlim : public InstanceNode
{
public:
	int noArgsCalled;
	std::string arg;
	std::string constructionArg;
	std::string echoBuf;

	static std::shared_ptr<TestSlim> Create(std::shared_ptr<SlimList> args) {
		if (args->GetLength() > 1) {
			return nullptr;
		}

		auto result = std::make_shared<TestSlim>(args);
		if (args->GetLength() == 1)
			result->constructionArg = args->GetStringAt(0);

		return result;
	}

	TestSlim(std::shared_ptr<SlimList> args)
	{
		if (args->GetLength() == 1)
			constructionArg = args->GetStringAt(0);
	}

	int TestSlim_noArgsCalled() {
		return noArgsCalled;
	}

	std::string noArgs(std::shared_ptr<SlimList> args) {
		noArgsCalled = 1;
		return "/__VOID__/";
	}

	std::string returnValue(std::shared_ptr<SlimList> args) {
		return "value";
	}

	std::string oneArg(std::shared_ptr<SlimList> args) {
		auto value = args->GetStringAt(0);
		return value;
	}

	std::string add(std::shared_ptr<SlimList> args) {
		static char buf[50];
		snprintf(buf, 50, "%s%s", args->GetStringAt(0), args->GetStringAt(1));
		return buf;
	}

	std::string Null(std::shared_ptr<SlimList> args) {
		return "";
	}

	std::string setArg(std::shared_ptr<SlimList> args) {
		arg = args->GetStringAt(0);
		return "/__VOID__/";
	}

	std::string getArg(std::shared_ptr<SlimList> args) {
		return arg;
	}

	std::string getArg_From_Function_With_Underscores(std::shared_ptr<SlimList> args) {
		return arg;
	}

	std::string getConstructionArg(std::shared_ptr<SlimList> args) {
		return constructionArg;
	}

	std::string returnError(std::shared_ptr<SlimList> args) {
		return StatementExecutor_FixtureError("my exception");
	}

	BEGIN_REGISTER_METHOD()
		REGISTER(TestSlim, returnValue)
		REGISTER(TestSlim, noArgs)
		REGISTER(TestSlim, oneArg)
		REGISTER(TestSlim, add)
		REGISTER(TestSlim, Null)
		REGISTER(TestSlim, setArg)
		REGISTER(TestSlim, getArg)
		REGISTER(TestSlim, getArg_From_Function_With_Underscores)
		REGISTER(TestSlim, getConstructionArg)
		REGISTER(TestSlim, returnError)
		REGISTER(TestSlim, Null)
		REGISTER(TestSlim, setArg)
		END_REGISTER_METHOD()
};