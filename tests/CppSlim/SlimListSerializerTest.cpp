#include <stdlib.h>
#include <string.h>
#include <iostream>

#include "SlimList.hpp"

#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"

TEST_GROUP(SlimListSerializer)
{
  std::shared_ptr<SlimList> slimList;
  std::string serializedList;

  void setup()
  {
    slimList  = std::make_shared<SlimList>();
  }

  void teardown()
  {
  }
};

TEST(SlimListSerializer, SerializeAListWithNoElements)
{
	serializedList = slimList->Serialize();
	CHECK_EQUAL("[000000:]", serializedList);
}

TEST(SlimListSerializer, SerializeAListWithOneElements)
{
	slimList->AddString("hello");
	serializedList = slimList->Serialize();
	CHECK_EQUAL("[000001:000005:hello:]", serializedList);
}

TEST(SlimListSerializer, SerializeAListWithTwoElements)
{
	slimList->AddString("hello");
	slimList->AddString("world");

	serializedList = slimList->Serialize();

	CHECK_EQUAL("[000002:000005:hello:000005:world:]", serializedList);
}

TEST(SlimListSerializer, ListCopysItsString)
{
	char string[12] = "Hello";
	slimList->AddString(string);
	strcpy(string, "Goodbye");
	serializedList = slimList->Serialize();
	CHECK_EQUAL("[000001:000005:Hello:]", serializedList);
}

TEST(SlimListSerializer, canCopyAList)
{
	slimList->AddString("123456");
	slimList->AddString("987654");

	std::shared_ptr<SlimList> copy = std::make_shared<SlimList>();
	int i;
	for (i = 0; i < slimList->GetLength(); i++) {
		const std::string string = slimList->GetStringAt(i);
		copy->AddString(string);
	}
	auto serialCopy = copy->Serialize();
	auto serialSlimList = slimList->Serialize();
	CHECK_EQUAL(serialCopy, serialSlimList);
}

TEST(SlimListSerializer, SerializeNestedList)
{
	auto embeddedList = std::make_shared<SlimList>();
	embeddedList->AddString("element");
	slimList->AddList(embeddedList);
	serializedList = slimList->Serialize();
	CHECK_EQUAL("[000001:000024:[000001:000007:element:]:]", serializedList);
}

TEST(SlimListSerializer, serializedLength)
{
	slimList->AddString("12345");
	LONGS_EQUAL(5 + 17, slimList->SerializedLength());
	slimList->AddString("123456");
	LONGS_EQUAL(9 + (5 + 8) + (6 + 8), slimList->SerializedLength());
	serializedList = slimList->Serialize();
	LONGS_EQUAL(9 + (5 + 8) + (6 + 8), serializedList.size());
}

TEST(SlimListSerializer, serializeNull)
{
	slimList->AddString("");
	serializedList = slimList->Serialize();
	CHECK_EQUAL("[000001:000004:Null:]", serializedList);
}
