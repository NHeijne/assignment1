cmake_minimum_required( VERSION 2.6 FATAL_ERROR )

project(project1n)
set (project1n_VERSION_MAJOR 1)
set (project1n_VERSION_MINOR 0)

set(CMAKE_BUILD_TYPE release) 

include (CheckIncludeFiles)

add_definitions(-std=c++0x)

find_package( Boost REQUIRED COMPONENTS serialization )
include_directories( ${Boost_INCLUDE_DIRS} )

add_executable(project1n Main.cpp Grammar.cpp CYKParser.cpp)

target_link_libraries( project1n ${Boost_SERIALIZATION_LIBRARY} )

include (InstallRequiredSystemLibraries)

set (CPACK_PACKAGE_VERSION_MAJOR "${project1n_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${project1n_VERSION_MINOR}")

set (CPACK_PACKAGE_EXECUTABLES "project1n" "project1n")
include (CPack)


