set(TARGET_CppSlim CppSlim)
project(${TARGET_CppSlim})

if (UNIX)
    set(cppslim_com_sources
        Com/SocketServer.cpp
        Com/TcpComLink.cpp
        )
else()
    set(cppslim_com_sources
        ComWin32/SocketServer.cpp
        ComWin32/TcpComLink.cpp
        )
endif()

set(cppslim_sources
    CppSlim/Slim.cpp
    CppSlim/SlimConnectionHandler.cpp
    CppSlim/SlimList.cpp
    CppSlim/SlimUtil.cpp
    ExecutorCpp/StatementExecutor.cpp
    ExecutorCpp/SymbolTable.cpp
    )

if (CPPSLIM_BUILD_AS_CPP)
    set_source_files_properties(${cppslim_com_sources} PROPERTIES LANGUAGE "CXX")
    set_source_files_properties(${cppslim_sources} PROPERTIES LANGUAGE "CXX")
endif()

add_library(${TARGET_CppSlim} STATIC
    ${cppslim_sources}
    ${cppslim_com_sources}
    )

target_include_directories(${TARGET_CppSlim} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/CppSlim>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/ExecutorC>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/Com>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>  # <prefix>/include/CppSlim
    )
if (WIN32)
    target_link_libraries(${TARGET_CppSlim} PUBLIC WS2_32)
endif()

install(DIRECTORY "${CMAKE_SOURCE_DIR}/include/CppSlim" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY "${CMAKE_SOURCE_DIR}/include/ExecutorCpp" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY "${CMAKE_SOURCE_DIR}/include/Com" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS "${TARGET_CppSlim}"
        ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
        )

set(TARGET_CppSlim_main CppSlim_main)
set(cppslim_main_sources CppSlim_main/Main.cpp)
if (CPPSLIM_BUILD_AS_CPP)
    set_source_files_properties(${cppslim_main_sources} PROPERTIES LANGUAGE "CXX")
endif()
add_library(${TARGET_CppSlim_main} STATIC ${cppslim_main_sources})
target_link_libraries(${TARGET_CppSlim_main} PUBLIC ${TARGET_CppSlim})
install(TARGETS "${TARGET_CppSlim_main}"
        ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(CMAKE_COMPILER_IS_MSVC)
    # TARGET_PDB_FILE is not supported with static libraries
    # https://gitlab.kitware.com/cmake/cmake/issues/16932
    #install(FILES $<TARGET_PDB_FILE:${TARGET_CppSlim}> DESTINATION ${CMAKE_INSTALL_BINDIR} OPTIONAL)
    #install(FILES $<TARGET_PDB_FILE:${TARGET_CppSlim_main}> DESTINATION ${CMAKE_INSTALL_BINDIR} OPTIONAL)

    # workaround
    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_CppSlim}.dir/$\{CMAKE_INSTALL_CONFIG_NAME\}/${TARGET_CppSlim}.pdb"
        "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_CppSlim_main}.dir/$\{CMAKE_INSTALL_CONFIG_NAME\}/${TARGET_CppSlim_main}.pdb"
        DESTINATION ${CMAKE_INSTALL_LIBDIR} OPTIONAL)
endif()

