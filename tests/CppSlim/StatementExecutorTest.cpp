#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "StatementExecutor.hpp"
#include "TestSlim.hpp"
#include "SlimList.hpp"
#include "Fixtures.hpp"

#include <string.h>
#include <stdio.h>

TEST_GROUP(StatementExecutor)
{
	std::shared_ptr<StatementExecutor> statementExecutor;
	std::shared_ptr<SlimList> args;
	std::shared_ptr<SlimList> empty;

	void setup()
	{
		args = std::make_shared<SlimList>();
		empty = std::make_shared<SlimList>();
		statementExecutor = std::make_shared<StatementExecutor>();
		statementExecutor->AddFixture(TestSlim_Register);
		statementExecutor->Make("test_slim", "TestSlim", empty);
	}
    
    void teardown()
    {
    }
};

TEST(StatementExecutor, canCallFunctionWithNoArguments)
{
	statementExecutor->Call("test_slim", "noArgs", args);
	auto testSlim = std::dynamic_pointer_cast<TestSlim>(statementExecutor->Instance("test_slim"));
	CHECK(testSlim->TestSlim_noArgsCalled());
}

TEST(StatementExecutor, cantCallFunctionThatDoesNotExist)
{
	auto result = statementExecutor->Call("test_slim", "noSuchMethod", args);
	CHECK_EQUAL("__EXCEPTION__:message:<<NO_METHOD_IN_CLASS noSuchMethod[0]>>", result);

	result = statementExecutor->Call("test_slim", "noOtherSuchMethod", args);
	CHECK_EQUAL("__EXCEPTION__:message:<<NO_METHOD_IN_CLASS noOtherSuchMethod[0]>>", result);
}

TEST(StatementExecutor, shouldTruncateReallyLongNamedFunctionThatDoesNotExistTo32)
{
	auto result = statementExecutor->Call("test_slim", "noOtherSuchMethod123456789022345678903234567890423456789052345678906234567890", args);
	CHECK(result.size() < 120);
	CHECK_EQUAL("__EXCEPTION__:message:<<NO_METHOD_IN_CLASS noOtherSuchMethod123456789022345[0]>>", result);	
}

TEST(StatementExecutor, shouldKnowNumberofArgumentsforNonExistantFunction)
{
	args->AddString("BlahBlah");
	auto result = statementExecutor->Call("test_slim", "noSuchMethod", args);
	CHECK_EQUAL("__EXCEPTION__:message:<<NO_METHOD_IN_CLASS noSuchMethod[1]>>", result);
}

TEST(StatementExecutor, shouldNotAllowACallToaNonexistentInstance)
{
	auto result = statementExecutor->Call("noSuchInstance", "noArgs", args);
	CHECK_EQUAL("__EXCEPTION__:message:<<NO_INSTANCE noSuchInstance.>>", result);	
}

TEST(StatementExecutor, shouldNotAllowAMakeOnANonexistentClass)
{
	auto result = statementExecutor->Make("instanceName", "NoSuchClass", empty);
	CHECK_EQUAL("__EXCEPTION__:message:<<NO_CLASS NoSuchClass.>>", result);	
}

TEST(StatementExecutor, canCallaMethodThatReturnsAValue) {
	auto result = statementExecutor->Call("test_slim", "returnValue", args);
	CHECK_EQUAL("value", result);
}

TEST(StatementExecutor, canCallaMethodThatTakesASlimList)
{
	args->AddString("hello world");
	
	auto result = statementExecutor->Call("test_slim", "oneArg", args);
	CHECK_EQUAL("hello world", result);
}

TEST(StatementExecutor, WhereCalledFunctionHasUnderscoresSeparatingNameParts)
{

	args->AddString("hello world");

	statementExecutor->Call("test_slim", "setArg", args);
	auto result = statementExecutor->Call("test_slim", "getArg_From_Function_With_Underscores", empty);
	CHECK_EQUAL("hello world", result);
}

TEST(StatementExecutor, canCallTwoInstancesOfTheSameFixture)
{
	auto args2 = std::make_shared<SlimList>();
	args->AddString("one");
	args2->AddString("two");
	
	statementExecutor->Make("test_slim2", "TestSlim", empty);
	statementExecutor->Call("test_slim", "setArg", args);
	statementExecutor->Call("test_slim2", "setArg", args2);
	auto one = statementExecutor->Call("test_slim", "getArg", empty);
	auto two = statementExecutor->Call("test_slim2", "getArg", empty);
	CHECK_EQUAL("one", one);
	CHECK_EQUAL("two", two);
}

TEST(StatementExecutor, canCreateTwoDifferentFixtures)
{
	auto args2 = std::make_shared<SlimList>();
	args->AddString("one");
	args2->AddString("two");
		
	statementExecutor->Make("test_slim2", "TestSlimAgain", empty);	
	statementExecutor->Call("test_slim", "setArg", args);	
	statementExecutor->Call("test_slim2", "setArg", args2);
	auto one = statementExecutor->Call("test_slim", "getArg", empty);
	auto two = statementExecutor->Call("test_slim2", "getArg", empty);
	CHECK_EQUAL("one", one);
	CHECK_EQUAL("two", two);
}

TEST(StatementExecutor, canReplaceSymbolsWithTheirValue)
{
	statementExecutor->SetSymbol("v", "bob");
	args->AddString("hi $v.");
	auto result = statementExecutor->Call("test_slim", "oneArg", args);
	LONGS_EQUAL(strlen("hi bob."), result.size());
	CHECK_EQUAL("hi bob.", result);
}

TEST(StatementExecutor, canReplaceSymbolsInTheMiddle)
{
	statementExecutor->SetSymbol("v", "bob");
	args->AddString("hi $v whats up.");
	CHECK_EQUAL("hi bob whats up.", statementExecutor->Call("test_slim", "oneArg", args));
}

TEST(StatementExecutor, canReplaceSymbolsWithOtherNonAlphaNumeric)
{
	statementExecutor->SetSymbol("v2", "bob");
	args->AddString("$v2=why");
	CHECK_EQUAL("bob=why", statementExecutor->Call("test_slim", "oneArg", args));
}

TEST(StatementExecutor, canReplaceMultipleSymbolsWithTheirValue)
{
	statementExecutor->SetSymbol("v", "bob");
	statementExecutor->SetSymbol("e", "doug");
	args->AddString("hi $v. Cost:  $12.32 from $e.");
	CHECK_EQUAL("hi bob. Cost:  $12.32 from doug.", statementExecutor->Call("test_slim", "oneArg", args));
}

TEST(StatementExecutor, canHandlestringWithJustADollarSign)
{
	statementExecutor->SetSymbol("v2", "bob");
	args->AddString("$");
	CHECK_EQUAL("$", statementExecutor->Call("test_slim", "oneArg", args));
}

TEST(StatementExecutor, canHandleDollarSignAtTheEndOfTheString)
{
	statementExecutor->SetSymbol("v2", "doug");
	args->AddString("hi $v2$");
	CHECK_EQUAL("hi doug$", statementExecutor->Call("test_slim", "oneArg", args));
}

TEST(StatementExecutor, canReplaceSymbolsInSubLists) 
{
	statementExecutor->SetSymbol("v2", "doug");
	auto subList = std::make_shared<SlimList>();
	subList->AddString("Hi $v2.");
	args->AddList(subList);
	auto result = statementExecutor->Call("test_slim", "oneArg", args);
	CHECK_FALSE(result.empty());
	auto returnedList = std::make_shared<SlimList>(result);
	CHECK(nullptr != returnedList);
	LONGS_EQUAL(1, returnedList->GetLength());
	auto element = returnedList->GetStringAt(0);
	CHECK_EQUAL("Hi doug.", element);
}

TEST(StatementExecutor, canReplaceSymbolsInSubSubLists) 
{
	statementExecutor->SetSymbol("v2", "doug");
	auto subList = std::make_shared<SlimList>();
	auto subSubList = std::make_shared<SlimList>();
	subSubList->AddString("Hi $v2.");
	subList->AddList(subSubList);
	args->AddList(subList);
	auto result = statementExecutor->Call("test_slim", "oneArg", args);
	CHECK_FALSE(result.empty());
	auto returnedSubList = std::make_shared<SlimList>(result);
	CHECK(nullptr != returnedSubList);
	LONGS_EQUAL(1, returnedSubList->GetLength());
	auto returnedSubSubList = returnedSubList->GetListAt(0);
	CHECK(nullptr != returnedSubSubList);
	LONGS_EQUAL(1, returnedSubSubList->GetLength());
	auto element = returnedSubSubList->GetStringAt(0);
	CHECK_FALSE(element.empty());
	CHECK_EQUAL("Hi doug.", element);
}

TEST(StatementExecutor, canCreateFixtureWithSymbolAsClassName)
{
	statementExecutor->SetSymbol("fixtureName", "Test_Slim");
	auto makeResponse = statementExecutor->Make("instanceName", "$fixtureName", empty);
	CHECK_EQUAL("OK", makeResponse);
}

TEST(StatementExecutor, shouldNotAllowAMakeOnANonexistentClassReferencedBySymbol)
{
	statementExecutor->SetSymbol("fixtureName", "NoSuchClass");
	auto makeResponse = statementExecutor->Make("instanceName", "$fixtureName", empty);
	CHECK_EQUAL("__EXCEPTION__:message:<<NO_CLASS $fixtureName.>>", makeResponse);
}

TEST(StatementExecutor, canCreateFixtureWithSymbolInClassName)
{
	statementExecutor->SetSymbol("test", "Test");
	auto makeResponse = statementExecutor->Make("instanceName", "$test_Slim", empty);
	CHECK_EQUAL("OK", makeResponse);
}

TEST(StatementExecutor, canCreateFixtureWithMultipleSymbolsInClassName)
{
	statementExecutor->SetSymbol("test", "Test");
	statementExecutor->SetSymbol("slim", "Slim");
	auto makeResponse = statementExecutor->Make("instanceName", "$test_$slim", empty);
	CHECK_EQUAL("OK", makeResponse);
}

TEST(StatementExecutor, canCreateFixtureWithArguments) 
{
	auto constructionArgs = std::make_shared<SlimList>();
	constructionArgs->AddString("hi");
	statementExecutor->Make("test_slim_with_arg", "TestSlim", constructionArgs);
	auto result = statementExecutor->Call("test_slim_with_arg", "getConstructionArg", empty);
	CHECK_EQUAL("hi", result);
}

TEST(StatementExecutor, canCreateFixtureWithArgumentsThatHaveSymbols) 
{
	statementExecutor->SetSymbol("name", "doug");
	auto constructionArgs = std::make_shared<SlimList>();
	constructionArgs->AddString("hi $name");
	statementExecutor->Make("test_slim_symbols", "TestSlim", constructionArgs);
	auto result = statementExecutor->Call("test_slim_symbols", "getConstructionArg", empty);
	CHECK_EQUAL("hi doug", result);
}

/*TEST(StatementExecutor, canCreateFixtureWithArgumentsThatHaveMultipleSymbols) 
{
	statementExecutor->SetSymbol("fname", "doug");
	statementExecutor->SetSymbol("lname", "bradbury");
	
	auto constructionArgs = std::make_shared<SlimList>();
	constructionArgs->AddString("hi $fname $lname");
	statementExecutor->Make("test_slim", "TestSlim", constructionArgs);
	auto result = statementExecutor->Call("test_slim", "getConstructionArg", empty);
	CHECK_EQUAL("hi doug bradbury", result);
}*/

TEST(StatementExecutor, fixtureConstructionFailsWithUserErrorMessage) 
{
	auto constructionArgs = std::make_shared<SlimList>();
	constructionArgs->AddString("hi doug");
	constructionArgs->AddString("ho doug");
	
	auto result = statementExecutor->Make("test_slim", "TestSlim", constructionArgs);
	CHECK_EQUAL("__EXCEPTION__:message:<<COULD_NOT_INVOKE_CONSTRUCTOR TestSlim .>>", result);
}

TEST(StatementExecutor, fixtureReferencedBySymbolConstructionFailsWithUserErrorMessage)
{
	statementExecutor->SetSymbol("fixtureName", "Test_Slim");
	args->AddString("arg0");
	args->AddString("arg1");
	auto makeResponse = statementExecutor->Make("instanceName", "$fixtureName", args);
	CHECK_EQUAL("__EXCEPTION__:message:<<COULD_NOT_INVOKE_CONSTRUCTOR $fixtureName .>>", makeResponse);
}

TEST(StatementExecutor, fixtureCanReturnError) 
{
	auto result = statementExecutor->Call("test_slim", "returnError", args);
	CHECK_EQUAL("__EXCEPTION__:message:<<my exception.>>", result);	
}

TEST(StatementExecutor, canCallFixtureDeclaredBackwards) 
{
	statementExecutor->Make("backwardsTestSlim", "TestSlimDeclaredLate", empty);
	args->AddString("hi doug");
	auto result = statementExecutor->Call("backwardsTestSlim", "oneArg", args);
	CHECK_EQUAL("hi doug", result);
}

TEST(StatementExecutor, canCallFixtureNotDeclared) 
{
	statementExecutor->Make("undeclaredTestSlim", "TestSlimUndeclared", empty);
	args->AddString("hi doug");
	auto result = statementExecutor->Call("undeclaredTestSlim", "oneArg", args);
	CHECK_EQUAL("__EXCEPTION__:message:<<NO_INSTANCE undeclaredTestSlim.>>", result);
}

TEST(StatementExecutor, canHavenullptrResult)
{
	auto result = statementExecutor->Call("test_slim", "Null", args);
	CHECK(result.empty());  
}

TEST_GROUP(StatementExecutorWithLibraryInstances)
{
    std::shared_ptr<StatementExecutor> statementExecutor;
    std::shared_ptr<SlimList> noArgs;

    void setup()
    {
        statementExecutor = std::make_shared<StatementExecutor>();
        noArgs = std::make_shared<SlimList>();
        statementExecutor->AddFixture(&RegisterMockFixtureWith1Method);
        statementExecutor->AddFixture(&RegisterMockFixtureWith2Methods);
        statementExecutor->AddFixture(&RegisterMockFixtureWith3Methods);
    }

    void teardown()
    {
        mock().clear();
    }

    class MockFixture : public InstanceNode
    {
	public:
		static std::shared_ptr<MockFixture> Create(std::shared_ptr<SlimList> args) {
			return *reinterpret_cast<std::shared_ptr<MockFixture>*>((mock().actualCall("createMockFixture")
				.returnPointerValue()));
		}

		std::string method1(SlimList* args)
		{
			return mock().actualCall("method1")
				.onObject(this)
				.withParameter("args", args)
				.returnStringValue();
		}

		std::string method2(SlimList* args)
		{
			return mock().actualCall("method2")
				.onObject(this)
				.withParameter("args", args)
				.returnStringValue();
		}

		std::string method3(SlimList* args)
		{
			return mock().actualCall("method3")
				.onObject(this)
				.withParameter("args", args)
				.returnStringValue();
		}

		MockFixture(std::shared_ptr<SlimList>)
		{
		}

		MockFixture()
		{
		}

		std::string invokeMethod1(std::shared_ptr<SlimList> args)
		{
			return method1(args.get());
		}

		std::string invokeMethod2(std::shared_ptr<SlimList> args)
		{
			return method2(args.get());
		}

		std::string invokeMethod3(std::shared_ptr<SlimList> args)
		{
			return method3(args.get());
		}

		BEGIN_REGISTER_METHOD()
			REGISTER(MockFixture, invokeMethod1)
			REGISTER(MockFixture, invokeMethod2)
			REGISTER(MockFixture, invokeMethod3)
			END_REGISTER_METHOD()
    };


    static void RegisterMockFixtureWith1Method(std::shared_ptr<StatementExecutor> statementExecutor)
    {
        statementExecutor->RegisterFixture("MockFixtureWith1Method", &MockFixture::Create);
    }

    static void RegisterMockFixtureWith2Methods(std::shared_ptr<StatementExecutor> statementExecutor)
    {
        statementExecutor->RegisterFixture("MockFixtureWith2Methods", &MockFixture::Create);
    }

    static void RegisterMockFixtureWith3Methods(std::shared_ptr<StatementExecutor> statementExecutor)
    {
        statementExecutor->RegisterFixture("MockFixtureWith3Methods", &MockFixture::Create);
    }
};

TEST(StatementExecutorWithLibraryInstances, callsMethodOnInstanceFirst)
{
    auto standardInstance = std::make_shared<MockFixture>();
    auto libraryInstance = std::make_shared<MockFixture>();

    mock().expectOneCall("createMockFixture")
          .andReturnValue(&standardInstance);
    mock().expectOneCall("createMockFixture")
          .andReturnValue(&libraryInstance);
    mock().expectOneCall("method1")
          .onObject(standardInstance.get())
          .withParameter("args", noArgs.get())
          .andReturnValue("OK");

    statementExecutor->Make("standardInstance", "MockFixtureWith1Method", noArgs);
    statementExecutor->Make("libraryInstance", "MockFixtureWith2Methods", noArgs);
    auto result = statementExecutor->Call("standardInstance", "invokeMethod1", noArgs);

    CHECK_EQUAL("OK", result);
    mock().checkExpectations();
}

TEST(StatementExecutorWithLibraryInstances, callMethodThatDoesNotExistOnGivenInstanceOrLibraryInstancesReturnsException)
{
	auto standardInstance = std::make_shared<MockFixture>();
	auto libraryInstanceA = std::make_shared<MockFixture>();
	auto libraryInstanceB = std::make_shared<MockFixture>();

    mock().expectOneCall("createMockFixture")
          .andReturnValue(&standardInstance);
    mock().expectOneCall("createMockFixture")
          .andReturnValue(&libraryInstanceA);
    mock().expectOneCall("createMockFixture")
          .andReturnValue(&libraryInstanceB);

    statementExecutor->Make("standardInstance", "MockFixtureWith1Method", noArgs);
    statementExecutor->Make("libraryInstanceA", "MockFixtureWith3Methods", noArgs);
    statementExecutor->Make("libraryInstanceB", "MockFixtureWith2Methods", noArgs);
    auto result = statementExecutor->Call("standardInstance", "invokeMethod4", noArgs);

    CHECK_EQUAL("__EXCEPTION__:message:<<NO_METHOD_IN_CLASS invokeMethod4[0]>>", result);
    mock().checkExpectations();
}
