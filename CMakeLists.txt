cmake_minimum_required (VERSION 3.8)

#
#	Options related to which parts of the source get built
#

option(JCLIB_BUILD_TESTS "Build test executables" OFF)
option(JCLIB_BUILD_SOURCE_GENERATOR "Enables the jclib source code generator to be built" OFF)

#
#	Options related to enabling / disabling / tweaking features within jclib
#

option(JCLIB_ALLOW_DEPRECATED "Allows deprecated features to be used" OFF)
option(JCLIB_NO_EXCEPTIONS "Uses the nothrow versions by default for functions that provide one" OFF)



#
#	jclib project declaration
#
project(jclib
	VERSION 0.2.0
	DESCRIPTION "A collection of utilities and tools for c++"
	HOMEPAGE_URL "https://github.com/JonathanCline/JCLib")

include(CTest)

add_library(${PROJECT_NAME} INTERFACE)

#	Provide an alias for the JCLib target so that "jclib" is a valid target, in the future this will be the only name.
add_library(JCLib ALIAS ${PROJECT_NAME})

#	Add the configuration file subdirectory
add_subdirectory("config")


target_include_directories(${PROJECT_NAME} INTERFACE "include" "config/include")



#
#	Provide version information
#
# TODO : use a cmake configure file instead

if(JCLIB_ALLOW_DEPRECATED)
	target_compile_definitions(${PROJECT_NAME} INTERFACE
		JCLIB_ALLOW_DEPRECATED=true)
endif()
if (JCLIB_NO_EXCEPTIONS)
	target_compile_definitions(${PROJECT_NAME} INTERFACE
		JCLIB_NO_EXCEPTIONS=true)
endif()




#
#	Add test helper cmake file
#
include(jclibtest.cmake)

if(JCLIB_BUILD_TESTS)
	enable_testing()
	add_subdirectory("tests")
endif()



if (JCLIB_BUILD_GENERATOR)

	# Include the code generation cmake
	add_subdirectory("generation")

endif()


