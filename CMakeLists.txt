cmake_minimum_required (VERSION 3.8)

option(JCLIB_BUILD_TESTS ON)
option(JCLIB_ALLOW_DEPRECATED OFF)

project(JCLib 
	VERSION 0.0.2
	DESCRIPTION "A collection of useful things and improvements for c++"
	HOMEPAGE_URL "https://github.com/JonathanCline/JCLib")

include(CTest)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE "include")


target_compile_definitions(${PROJECT_NAME} INTERFACE
	JCLIB_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
	JCLIB_VERSION_MINOR=${PROJECT_VERSION_MINOR}
	JCLIB_VERSION_PATCH=${PROJECT_VERSION_PATCH})

if(JCLIB_ALLOW_DEPRECATED)
	target_compile_definitions(${PROJECT_NAME} INTERFACE
		JCLIB_ALLOW_DEPRECATED=true)
endif()




enable_testing()

add_subdirectory("tests")
