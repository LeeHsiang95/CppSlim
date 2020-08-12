#include "CppUTest/TestHarness.h"

#include "SymbolTable.hpp"

#include <stdio.h>

TEST_GROUP(SymbolTable)
{
    std::shared_ptr<SymbolTable> symbolTable;

    void setup()
    {
      symbolTable = std::make_shared<SymbolTable>();
    }
    
    void teardown()
    {
    }
};

TEST(SymbolTable, findNonExistentSymbolShouldReturnNullptr)
{
	CHECK_TRUE(symbolTable->FindSymbol("Hey").empty());
}

TEST(SymbolTable, findSymbolShouldReturnSymbol)
{
	symbolTable->SetSymbol("Hey", "You");
	CHECK_EQUAL("You", symbolTable->FindSymbol("Hey"));
}

TEST(SymbolTable, CanGetLengthOfSymbol)
{
	symbolTable->SetSymbol("Hey", "1234567890");
	LONGS_EQUAL(10, symbolTable->GetSymbolLength("Hey"));
}

TEST(SymbolTable, CanGetLengthOfNonExistentSymbol)
{
	LONGS_EQUAL(-1, symbolTable->GetSymbolLength("Hey"));
}

