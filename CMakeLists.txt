cmake_minimum_required (VERSION 3.8)

option(JCLIB_BUILD_TESTS ON)
option(JCLIB_ALLOW_DEPRECATED OFF)

project(JCLib 
	VERSION 0.1.1
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

#
#	Takes an optional additional arguement for the test's source file
#
macro(JCLIB_ADD_TEST testName)
	if (${ARGC} EQUAL "2")
		add_executable(JCLib_${testName}Testing "${ARGV1}")
	else()
		add_executable(JCLib_${testName}Testing "${testName}.cpp")
	endif()
	set(testSourceFile )
	target_link_libraries(JCLib_${testName}Testing PRIVATE JCLib)
	add_test("JCLib_${testName}_test" JCLib_${testName}Testing)
	set_target_properties(JCLib_${testName}Testing PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED True)
endmacro(JCLIB_ADD_TEST )

if(JCLIB_BUILD_TESTS)
	enable_testing()
	add_subdirectory("tests")
endif()
