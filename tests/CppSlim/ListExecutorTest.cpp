#include "CppUTest/TestHarness.h"

#include "SlimList.hpp"
#include "StatementExecutor.hpp"
#include "TestSlim.hpp"

TEST_GROUP(ListExecutor)
{
    std::shared_ptr<SlimList> instructions;
    std::shared_ptr<StatementExecutor> statementExecutor;
    void setup()
    {
      statementExecutor = std::make_shared<StatementExecutor>();
      statementExecutor->AddFixture(TestSlim_Register);
      instructions = std::make_shared<SlimList>();
      char const * import[] = {"i1", "import", "blah", 0};
      addStatementTo(instructions, import);
      char const * make[] = {"m1", "make", "test_slim", "TestSlim", 0};
      addStatementTo(instructions, make);
      
    }
    
    void teardown()
    {
    }
    
    void addStatementTo(std::shared_ptr<SlimList> instructions, char const** elements)
    {
      auto statement = std::make_shared<SlimList>();
      while (*elements) {
        statement->AddString(*elements++);
      }
      instructions->AddList(statement);
    }
};

TEST(ListExecutor, ImportShouldReturnOk)
{
  auto results = instructions->Execute(statementExecutor);
  auto importResult = results->GetListAt(0);
  CHECK_EQUAL("i1", importResult->GetStringAt(0));
  CHECK_EQUAL("OK", importResult->GetStringAt(1));
}

TEST(ListExecutor, CannotExecuteAnInvalidOperation)
{
    char const* invalid[] = { "inv1", "Invalid", 0 };
    addStatementTo(instructions, invalid);

    auto results = instructions->Execute(statementExecutor);
    LONGS_EQUAL(3, results->GetLength());
    auto invalidStatementResult = results->GetListAt(2);

    CHECK_EQUAL("inv1", invalidStatementResult->GetStringAt(0));
    CHECK_EQUAL("__EXCEPTION__:message:<<INVALID_STATEMENT: [\"inv1\", \"Invalid\"].>>", invalidStatementResult->GetStringAt(1));
}

TEST(ListExecutor, CanCallASimpleFunction)
{
    char const* call[] = { "call1", "call", "test_slim", "returnValue", 0 };
    addStatementTo(instructions, call);

    auto results = instructions->Execute(statementExecutor);
    LONGS_EQUAL(3, results->GetLength());

    auto makeResult = results->GetListAt(1);
    CHECK_EQUAL("m1", makeResult->GetStringAt(0));
    CHECK_EQUAL("OK", makeResult->GetStringAt(1));

    auto callResult = results->GetListAt(2);
    CHECK_EQUAL("call1", callResult->GetStringAt(0));
    CHECK_EQUAL("value", callResult->GetStringAt(1));
}

TEST(ListExecutor, CantExecuteMalformedInstruction)
{
    char const* call[] = { "call1", "call", "notEnoughArguments", 0 };
    addStatementTo(instructions, call);
    auto results = instructions->Execute(statementExecutor);
    auto invalidStatementResult = results->GetListAt(2);
    CHECK_EQUAL("__EXCEPTION__:message:<<MALFORMED_INSTRUCTION [\"call1\", \"call\", \"notEnoughArguments\"].>>", invalidStatementResult->GetStringAt(1));
}

TEST(ListExecutor, CantCallAmethodOnAnInstanceThatDoesntExist)
{
    char const* call[] = { "call1", "call", "noSuchInstance", "method", 0 };
    addStatementTo(instructions, call);
    auto results = instructions->Execute(statementExecutor);
    auto invalidStatementResult = results->GetListAt(2);
    CHECK_EQUAL("__EXCEPTION__:message:<<NO_INSTANCE noSuchInstance.>>", invalidStatementResult->GetStringAt(1));
}

TEST(ListExecutor, ShouldRespondToAnEmptySetOfInstructionsWithAnEmptySetOfResults)
{
    auto instructions = std::make_shared<SlimList>();
    auto results = instructions->Execute(statementExecutor);
    LONGS_EQUAL(0, results->GetLength());
}

TEST(ListExecutor, CanPassArgumentsToConstructor)
{
    char const* make2[] = { "make2", "make", "test_slim2", "TestSlim", "ConstructorArgument", 0 };
    char const* call[] = { "call1", "call", "test_slim2", "getConstructionArg", 0 };
    addStatementTo(instructions, make2);
    addStatementTo(instructions, call);
    auto results = instructions->Execute(statementExecutor);
    auto callResult = results->GetListAt(3);
    CHECK_EQUAL("ConstructorArgument", callResult->GetStringAt(1));
}

TEST(ListExecutor, CanCallAFunctionMoreThanOnce)
{
    char const* call[] = { "call1", "call", "test_slim", "oneArg", "Hello", 0 };
    addStatementTo(instructions, call);
    char const* call2[] = { "call2", "call", "test_slim", "oneArg", "Goodbye", 0 };
    addStatementTo(instructions, call2);

    auto results = instructions->Execute(statementExecutor);

    auto callResult = results->GetListAt(2);
    CHECK_EQUAL("Hello", callResult->GetStringAt(1));
    callResult = results->GetListAt(3);
    CHECK_EQUAL("Goodbye", callResult->GetStringAt(1));
}

TEST(ListExecutor, CanAssignTheReturnValueToASymbol)
{
    char const* call[] = { "id1", "callAndAssign", "v", "test_slim", "add", "x", "y", 0 };
    addStatementTo(instructions, call);

    char const* call2[] = { "id2", "call", "test_slim", "oneArg", "$v", 0 };
    addStatementTo(instructions, call2);

    auto results = instructions->Execute(statementExecutor);

    auto callResult = results->GetListAt(2);
    CHECK_EQUAL("xy", callResult->GetStringAt(1));
    callResult = results->GetListAt(3);
    CHECK_EQUAL("xy", callResult->GetStringAt(1));
}

TEST(ListExecutor, CanReplateMultipleSymbolsInASingleArgument)
{
    char const* call[] = { "id1", "callAndAssign", "v1", "test_slim", "oneArg", "Bob", 0 };
    addStatementTo(instructions, call);

    char const* call2[] = { "id2", "callAndAssign", "v2", "test_slim", "oneArg", "Martin", 0 };
    addStatementTo(instructions, call2);

    char const* call3[] = { "id2", "call", "test_slim", "oneArg", "name:  $v1 $v2 $12.23", 0 };
    addStatementTo(instructions, call3);

    auto results = instructions->Execute(statementExecutor);

    auto callResult = results->GetListAt(4);
    CHECK_EQUAL("name:  Bob Martin $12.23", callResult->GetStringAt(1));
}

TEST(ListExecutor, CanPassAndReturnAList)
{
    auto l = std::make_shared<SlimList>();
    l->AddString("1");
    l->AddString("2");

    auto statement = std::make_shared<SlimList>();
    statement->AddString("id1");
    statement->AddString("call");
    statement->AddString("test_slim");
    statement->AddString("oneArg");
    statement->AddList(l);

    instructions->AddList(statement);

    auto results = instructions->Execute(statementExecutor);

    auto callResult = results->GetListAt(2);
    auto resultList = callResult->GetListAt(1);
    CHECK(l->Equals(resultList));
}

TEST(ListExecutor, CanReturnnullptr)
{
    char const* call[] = { "id1", "call", "test_slim", "Null", 0 };
    addStatementTo(instructions, call);

    auto results = instructions->Execute(statementExecutor);

    auto callResult = results->GetListAt(2);

    CHECK_EQUAL("Null", callResult->GetStringAt(1));
}

TEST(ListExecutor, CanPassASymbolInAList)
{
    char const* call[] = { "id1", "callAndAssign", "v", "test_slim", "oneArg", "Bob", 0 };
    addStatementTo(instructions, call);

    auto statement = std::make_shared<SlimList>();
    statement->AddString("id2");
    statement->AddString("call");
    statement->AddString("test_slim");
    statement->AddString("oneArg");

    auto l = std::make_shared<SlimList>();
    l->AddString("$v");
    statement->AddList(l);

    instructions->AddList(statement);

    auto results = instructions->Execute(statementExecutor);

    auto callResult = results->GetListAt(3);
    auto resultList = callResult->GetListAt(1);

    std::shared_ptr<SlimList> expected = std::make_shared<SlimList>();
    expected->AddString("Bob");

    CHECK(expected->Equals(resultList));
}
