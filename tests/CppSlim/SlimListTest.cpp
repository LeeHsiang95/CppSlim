#include <stdlib.h>
#include <string.h>
#include <iostream>

#include "SlimList.hpp"
#include "SlimUtil.hpp"

#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"

TEST_GROUP(SlimList)
{
    std::shared_ptr<SlimList> slimList;
	std::shared_ptr<SlimList> deserializedList;
	std::string serializedList;

    void setup()
    {
		slimList  = std::make_shared<SlimList>();
    }

    void teardown()
    {
    }

	void check_lists_equal(std::shared_ptr<SlimList> expected, std::shared_ptr<SlimList> actual) {
		CHECK(expected->Equals(actual));
	}

};

TEST(SlimList, twoEmptyListsAreEqual)
{
	auto list = std::make_shared<SlimList>();
	check_lists_equal(slimList, list);
}

TEST(SlimList, twoDifferentLenghtListsAreNotEqual)
{
	auto list = std::make_shared< SlimList>();
	slimList->AddString("hello");
	CHECK(!slimList->Equals(list));
}

TEST(SlimList, twoSingleElementListsWithDifferentElmementsAreNotEqual)
{
	auto list = std::make_shared< SlimList>();
	slimList->AddString("hello");
	list->AddString("goodbye");
	CHECK(!slimList->Equals(list));
}


TEST(SlimList, twoIdenticalMultipleElementListsElmementsAreEqual)
{
	auto list = std::make_shared< SlimList>();
	slimList->AddString("hello");
	slimList->AddString("goodbye");
	list->AddString("hello");
	list->AddString("goodbye");
	CHECK(slimList->Equals(list));
}


TEST(SlimList, twoNonIdenticalMultipleElementListsElmementsAreNotEqual)
{
	auto list = std::make_shared< SlimList>();
	slimList->AddString("hello");
	slimList->AddString("hello");
	list->AddString("hello");
	list->AddString("goodbye");
	CHECK(!slimList->Equals(list));
}


TEST(SlimList, canGetElements)
{
	slimList->AddString("element1");
	slimList->AddString("element2");
	CHECK_EQUAL("element1", slimList->GetStringAt(0));
	CHECK_EQUAL("element2", slimList->GetStringAt(1));
}

TEST(SlimList, canGetHashWithOneElement)
{
	slimList->AddString("<table><tr><td>name</td><td>bob</td></tr></table>");

	auto hash = slimList->GetHashAt(0);
	auto twoElementList = hash->GetListAt(0);

	CHECK_EQUAL("name", twoElementList->GetStringAt(0));
	CHECK_EQUAL("bob", twoElementList->GetStringAt(1));
}

TEST(SlimList, canGetHashWithMultipleElements)
{
     slimList->AddString("<table><tr><td>name</td><td>dough</td></tr><tr><td>addr</td><td>here</td></tr></table>");

     auto hash = slimList->GetHashAt(0);
     auto twoElementList = hash->GetListAt(1);

     CHECK_EQUAL("addr", twoElementList->GetStringAt(0));
     CHECK_EQUAL("here", twoElementList->GetStringAt(1));
}

TEST(SlimList, cannotGetElementThatAreNotThere)
{
	slimList->AddString("element1");
	slimList->AddString("element2");
	CHECK(slimList->GetStringAt(3).empty());
}

TEST(SlimList, canReplaceString)
{
	slimList->AddString("replaceMe");
	slimList->ReplaceAt(0, "WithMe");

	CHECK_EQUAL("WithMe", slimList->GetStringAt(0));
}

TEST(SlimList, canGetTail)
{
	slimList->AddString("1");
	slimList->AddString("2");
	slimList->AddString("3");
	slimList->AddString("4");

	auto expected = std::make_shared<SlimList>();
	expected->AddString("3");
	expected->AddString("4");

	auto tail = slimList->GetTailAt(2);
	CHECK(expected->Equals(tail));
}

TEST(SlimList, getDouble)
{
	slimList->AddString("2.3");
	DOUBLES_EQUAL(2.3, slimList->GetDoubleAt(0), 0.1);
}

TEST(SlimList, ToStringForEmptyList)
{
	auto listAsAString = slimList->ToString();
	CHECK_EQUAL("[]", listAsAString);
}

TEST(SlimList, toStringForSimpleList)
{
	slimList->AddString("a");
	slimList->AddString("b");
	auto stringVersionOfList = slimList->ToString();

	CHECK_EQUAL("[\"a\", \"b\"]", stringVersionOfList);
}

TEST(SlimList, toStringDoesNotHaveASideEffectWhichChangesResultsFromPriorCalls)
{
	auto priorString = slimList->ToString();

	slimList->AddString("a");
	auto listWithAnElementAsASting = slimList->ToString();
	CHECK_TRUE(priorString != listWithAnElementAsASting);
}

TEST(SlimList, recursiveToString)
{
	slimList->AddString("a");
	slimList->AddString("b");

	auto sublist = std::make_shared<SlimList>();
	sublist->AddString("3");
	sublist->AddString("4");

	slimList->AddList(sublist);

	auto listAsAString = slimList->ToString();
	CHECK_EQUAL("[\"a\", \"b\", [\"3\", \"4\"]]", listAsAString);
}

TEST(SlimList, toStringForLongList)
{
	const int entries = 128; //TODO: consider updating the size given its no longer as relevant
	int i;

	for (i = 0; i < entries; i++)
	{
		slimList->AddString("a");
	}

	auto listAsAString = slimList->ToString();
}

TEST(SlimList, CanPopHeadOnListWithOneEntry)
{
	slimList->AddString("a");
	slimList->PopHead();
	CHECK_EQUAL(0, slimList->GetLength());
}

TEST(SlimList, CanInsertAfterPoppingListWithEntries)
{
	slimList->AddString("a");
	slimList->AddString("a");

	slimList->PopHead();
	slimList->PopHead();

	slimList->AddString("a");
	slimList->AddString("a");
	CHECK_EQUAL(2, slimList->GetLength());
}

TEST(SlimList, iteratorDoesNotHaveAnItemWhenEmpty)
{
	auto iterator = slimList->CreateIterator();
	CHECK_TRUE(iterator == nullptr);
}

TEST(SlimList, iteratorHasItem)
{
	slimList->AddString("a");
	auto iterator = slimList->CreateIterator();

	CHECK_TRUE(iterator != nullptr);
}

TEST(SlimList, iteratorNext)
{
	slimList->AddString("a");
	auto iterator = slimList->CreateIterator();
	iterator = slimList->Advance(iterator);

	CHECK_TRUE(iterator == nullptr);
}


TEST(SlimList, iteratorGetString)
{
	const std::string contents = "a";
	slimList->AddString(contents);
	auto iterator = slimList->CreateIterator();

	auto actual =  iterator->GetString();
	CHECK_EQUAL(contents, actual);
}

