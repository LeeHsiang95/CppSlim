#include "TestSlim.hpp"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "compatibility.hpp"
#include "Fixtures.hpp"


void TestSlim_Register(std::shared_ptr<StatementExecutor> executor)
{
	executor->RegisterFixture("TestSlim", &TestSlim::Create);
	executor->RegisterFixture("TestSlimAgain", &TestSlim::Create);
	executor->RegisterFixture("TestSlimDeclaredLate", &TestSlim::Create);
}

