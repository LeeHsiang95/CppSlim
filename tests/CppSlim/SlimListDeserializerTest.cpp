#include <stdlib.h>
#include <string.h>
#include <iostream>

#include "SlimList.hpp"

#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"

TEST_GROUP(SlimListDeserializer)
{
    std::shared_ptr<SlimList> slimList;
	std::shared_ptr<SlimList> deserializedList;
	std::string serializedList;

    void setup()
    {
		slimList = std::make_shared<SlimList>();
    }

    void teardown()
    {
    }

	void check_lists_equal(std::shared_ptr<SlimList> expected, std::shared_ptr<SlimList> actual) {
		CHECK(expected->Equals(actual));
	}

};


TEST(SlimListDeserializer, deserializeEmptyList)
{
	deserializedList = std::make_shared<SlimList>("[000000:]");
	CHECK(deserializedList != 0);
	LONGS_EQUAL(0, deserializedList->GetLength());
}

TEST(SlimListDeserializer, canDeserializeCanonicalListWithOneElement)
{
	char const* canonicalList = "[000001:000008:Hi doug.:]";
	std::shared_ptr<SlimList> deserializedList = std::make_shared<SlimList>(canonicalList);
	CHECK(deserializedList != nullptr);
	LONGS_EQUAL(1, deserializedList->GetLength());
	CHECK_EQUAL("Hi doug.", deserializedList->GetStringAt(0));
}

TEST(SlimListDeserializer, canDeSerializeListWithOneElement)
{
	slimList->AddString("hello");
	serializedList = slimList->Serialize();
	deserializedList = std::make_shared<SlimList>(serializedList);
	CHECK(deserializedList != 0);
	check_lists_equal(slimList, deserializedList);
}

TEST(SlimListDeserializer, canDeSerializeListWithTwoElements)
{
	slimList->AddString("hello");
	slimList->AddString("bob");
	serializedList = slimList->Serialize();
	deserializedList = std::make_shared<SlimList>(serializedList);
	CHECK(deserializedList != 0);
	check_lists_equal(slimList, deserializedList);
}

TEST(SlimListDeserializer, canAddSubList)
{
	auto embeddedList = std::make_shared<SlimList>();
	embeddedList->AddString("element");
	slimList->AddList(embeddedList);
	serializedList = slimList->Serialize();
	deserializedList = std::make_shared<SlimList>(serializedList);
	auto subList = deserializedList->GetListAt(0);
	subList = deserializedList->GetListAt(0);
	check_lists_equal(embeddedList, subList);
}

TEST(SlimListDeserializer, getStringWhereThereIsAList)
{
	auto embeddedList = std::make_shared<SlimList>();
	embeddedList->AddString("element");
	slimList->AddList(embeddedList);
	serializedList = slimList->Serialize();
	deserializedList = std::make_shared<SlimList>(serializedList);
	auto string = deserializedList->GetStringAt(0);

	CHECK_EQUAL("[000001:000007:element:]", string);
	// POINTERS_EQUAL(0, string); ?????????????????????????????????????
}
