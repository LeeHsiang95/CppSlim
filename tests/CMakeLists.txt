include(CTest)

set(TARGET CppSlim-tests)
project(${TARGET})

find_package(CppUTest REQUIRED)

if (CPPSLIM_BUILD_AS_CPP)
    set_source_files_properties(CppSlim/TestSlim.cpp PROPERTIES LANGUAGE "CXX")
endif()

add_executable(${TARGET}
    AllTests.cpp
    CppSlim/SlimListDeserializerTest.cpp
    CppSlim/ListExecutorTest.cpp
    CppSlim/SymbolTableTest.cpp
    CppSlim/SlimConnectionHandlerTest.cpp
    CppSlim/SlimListTest.cpp
    CppSlim/SlimListSerializerTest.cpp
    CppSlim/StatementExecutorTest.cpp
    CppSlim/SlimUtilTest.cpp
    CppSlim/TestSlim.cpp
    )
target_include_directories(${TARGET} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/CppSlim")
target_link_libraries(${TARGET} PUBLIC CppSlim CppUTest::CppUTest CppUTest::CppUTestExt)
#target_compile_options(${TARGET} PUBLIC
#    -Dmalloc=cpputest_malloc
#    -Dfree=cpputest_free
#    -Drealloc=cpputest_realloc
#    )
add_test(NAME ${TARGET} COMMAND ${TARGET} -v)
