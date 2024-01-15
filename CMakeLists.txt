#Project parent cmake file#
cmake_minimum_required(VERSION 3.26.0)

project(SNAKE_AI LANGUAGES C ASM)

include(CTest)

#___________________________________________________________________________#

#TODO add custom debug directory
set(TEST_ENV "${PROJECT_SOURCE_DIR}/.test")
make_directory(${TEST_ENV})

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	add_compile_definitions(DEBUG)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	add_compile_definitions(RELEASE)
endif()

if(APPLE)
	include_directories("/opt/homebrew/include")
endif()

add_custom_target(clean-all
	COMMAND rm -rf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/*
	COMMAND rm -rf ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/*
)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/src/error)

add_library(global_interface INTERFACE)
target_include_directories(global_interface INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/include)
target_link_libraries(global_interface INTERFACE sn_error)
#target_precompile_headers(global_interface INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/include/pch.h)

#define sub directories
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/src/snake)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/test)
set(CMAKE_C_STANDARD 99)

