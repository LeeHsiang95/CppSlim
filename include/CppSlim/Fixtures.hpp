#pragma once

#include "StatementExecutor.hpp"

#define SLIM_QUOTES(x)  #x
#define SLIM_FIXTURE(fixture) \
extern void fixture##_Register(std::shared_ptr<StatementExecutor>);\
executor->AddFixture(fixture##_Register);

#define SLIM_FIXTURES void AddFixtures(std::shared_ptr<StatementExecutor> executor) \
{

#define SLIM_END }

#define PRIVATE_REGISTER_FIXTURE(name) executor->RegisterFixture(#name, &name::Create)

#define SLIM_CREATE_EMPTY_FIXTURE(name)  \
	void name##_Register(std::shared_ptr<StatementExecutor> executor) \
	{ \
		PRIVATE_REGISTER_FIXTURE(name)\
	}


#define SLIM_CREATE_FIXTURE(name) static std::string fixtureName = #name; \
void name##_Register(std::shared_ptr<StatementExecutor> executor) \
{ \
	PRIVATE_REGISTER_FIXTURE(name);


#define SLIM_ABORT(reason) SLIM_QUOTES(__EXCEPTION__:ABORT_SLIM_TEST:message:<<reason.>>)

#define SLIM_EXCEPTION(reason) SLIM_QUOTES(__EXCEPTION__:message:<<reason.>>)

#define SLIM_CONSTRUCTOR_ERROR(errorHandler, reason) 		errorHandler->ConstructorError(reason);


#define BEGIN_REGISTER_METHOD() std::function<std::string(std::shared_ptr<SlimList>)> GetMethodNode(const std::string& method_name) override {
#define END_REGISTER_METHOD() return nullptr;\
	}

#define REGISTER(fixture_name, name) if (method_name == #name)\
	return std::bind(&fixture_name::name, this, std::placeholders::_1);\
