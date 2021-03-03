cmake_minimum_required (VERSION 3.8)

option(JCLIB_BUILD_TESTS ON)

project(JCLib 
	VERSION 0.0.1
	DESCRIPTION "A collection of useful things and improvements for c++"
	HOMEPAGE_URL "https://github.com/JonathanCline/JCLib")

include(CTest)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE "include")

enable_testing()

add_subdirectory("tests")
