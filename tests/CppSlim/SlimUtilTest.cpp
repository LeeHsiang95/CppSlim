#include <stdlib.h>
#include <string.h>
#include <iostream>

#include "SlimUtil.hpp"

#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"

TEST_GROUP(SlimUtil)
{

};

TEST(SlimUtil, CanCreateEmptyString)
{
    const std::string actual;
    CHECK_EQUAL("", actual);
}

TEST(SlimUtil, StringStartsWith)
{
    CHECK(CSlim_StringStartsWith("", ""));
    CHECK_FALSE(CSlim_StringStartsWith("", "a"));
    CHECK_FALSE(CSlim_StringStartsWith("a", "ab"));
    CHECK(CSlim_StringStartsWith("a", ""));
    CHECK(CSlim_StringStartsWith("a", "a"));
    CHECK(CSlim_StringStartsWith("ab", "a"));
    CHECK_FALSE(CSlim_StringStartsWith("a", "b"));
    CHECK(CSlim_StringStartsWith("abc", "ab"));
    CHECK_FALSE(CSlim_StringStartsWith("abc", "ac"));
}


TEST_GROUP(SlimString) {

};

TEST(SlimString, TestIsSlimString) {
    CHECK_TRUE(SlimString::IsSlimString("000005:hello"));
    CHECK_FALSE(SlimString::IsSlimString("000005-hello"));
    CHECK_FALSE(SlimString::IsSlimString("a00005:hello"));
    CHECK_FALSE(SlimString::IsSlimString("a00005:"));
    CHECK_FALSE(SlimString::IsSlimString("000005:helloworld"));
    CHECK_FALSE(SlimString::IsSlimString("00005:"));
}

TEST(SlimString, TestContructor) {
    SlimString str("000005:hello");
    CHECK_EQUAL(str.Length(), 5);
    CHECK_EQUAL(str.Content(), "hello");
}