cmake_minimum_required (VERSION 3.8)

option(JCLIB_BUILD_TESTS "Build test executables" ON)
option(JCLIB_ALLOW_DEPRECATED "Allows deprecated features to be used" OFF)
option(JCLIB_NO_EXCEPTIONS "Uses the nothrow versions by default for functions that provide one" OFF)

project(JCLib 
	VERSION 0.1.2
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
if (JCLIB_NO_EXCEPTIONS)
	target_compile_definitions(${PROJECT_NAME} INTERFACE
		JCLIB_NO_EXCEPTIONS=true)
endif()

#
#	Takes an optional additional arguement for the test's source file
#
macro(JCLIB_ADD_TEST_CXX testName testSource cxxVersion)

	set(tname JCLib_test_${testName}_${cxxVersion})
	set(ctname JCLib_test_${testName}_${cxxVersion}_t)

	add_executable(${tname} ${testSource})
	
	target_link_libraries(${tname} PRIVATE JCLib)
	add_test("${ctname}" ${tname})
	target_compile_features(${tname} PRIVATE cxx_std_${cxxVersion})

endmacro(JCLIB_ADD_TEST_CXX )

#
#	C++ versions to check
#
set(cxxversions 14 17 20)

macro(JCLIB_ADD_TEST testName testSource)
	foreach(cxv IN LISTS cxxversions)
		JCLIB_ADD_TEST_CXX(${testName} ${testSource} ${cxv})
	endforeach()
endmacro(JCLIB_ADD_TEST)


if(JCLIB_BUILD_TESTS)
	enable_testing()
	add_subdirectory("tests")
endif()
