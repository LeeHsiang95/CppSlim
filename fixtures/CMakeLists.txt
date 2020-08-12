set(TARGET examples)
project(${TARGET})

set(fixtures_sources
    Main.cpp
    DecisionTableExample.cpp
    ExceptionsExample.cpp
    FixtureInCpp.cpp
    Fixtures.cpp
    QueryTableExample.cpp
    ScriptTableExample.cpp
    ScriptTableExampleEcho.cpp
    ScriptTableExampleEchoLoudly.cpp
    )
if (CPPSLIM_BUILD_AS_CPP)
    set_source_files_properties(${fixtures_sources} PROPERTIES LANGUAGE "CXX")
endif()

add_executable(${TARGET} ${fixtures_sources})

target_link_libraries(${TARGET} PUBLIC CppSlim)
