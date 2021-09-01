#
#	JCLib test support library
#

# jclib test support library
add_library(${PROJECT_NAME}-testsupport INTERFACE)

# Set include paths
target_include_directories(${PROJECT_NAME}-testsupport
	INTERFACE "include")


# jclib test support library alias
add_library(${PROJECT_NAME}::test ALIAS ${PROJECT_NAME}-testsupport)

