#
#	Finds and creates the tests for jclib
#

# Add test support cmake
include("jclibtest.cmake")


# Path to the root directory containing test folders
set(test_root "${CMAKE_CURRENT_LIST_DIR}")


# Get list of subdirectories
set(_testDirectoryList )
SUBDIRLIST(_testDirectoryList ${test_root})

# Create tests from sources within the test folders
foreach(_testDirectory IN LISTS _testDirectoryList)
	
	# Complete test directory path prefixed with the test root
	set(_testDirectoryPath "${test_root}/${_testDirectory}")

	# Find all .cmake test "driver" files
	set(_testDriverList )
	MATCH_DIRECTORY_CONTENTS(_testDriverList ${_testDirectoryPath} "^.+\.cmake")

	# Get number of test drivers found
	set(_testDriverListSize )
	list(LENGTH _testDriverList _testDriverListSize)

	# Check that there were test drivers found
	if ("${_testDriverListSize}" EQUAL "0")
		
	
		# Check for a test.cpp file
		if (EXISTS "${_testDirectoryPath}/test.cpp")
			# This is probably just an old test that hasn't been updated to the new system	
			#message(FATAL_ERROR "${_testDirectory}")

			# Add the new test, using the directory name a the test name
			JCLIB_ADD_TEST_FN(${_testDirectory} "${_testDirectoryPath}/test.cpp")
		
		else()

			# Print a debug message that this test directory is being ignored
			message(FATAL_ERROR "ignored invalid test directory ${_testDirectoryPath}")

		endif()

	else()

		# Include all of the test drivers
		foreach (_testDriver IN LISTS _testDriverList)
			include("${_testDirectory}/${_testDriver}")
		endforeach()

	endif()
endforeach()
