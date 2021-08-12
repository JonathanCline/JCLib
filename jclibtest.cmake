#
#	C++ versions to check
#
set(cxxversions 14 17 20)

#
#	Defines a new test executable target for use with CTest
#
#	@param testName Target name for the new test
#   @param testSource C++ source file path, must be absolute
#	@param cxxVersion C++ standard version for the test
#
function(JCLIB_ADD_TEST_CXX_FN testName testSource cxxVersion)

	# Test name
	set(tname jclib_test_${testName}_${cxxVersion})
	#set(ctname jclib_test_${testName}_${cxxVersion}_t)

	# Define the target
	add_executable(${tname} ${testSource})
	
	# Configure target
	target_link_libraries(${tname} PRIVATE jclib)
	#target_compile_features(${tname} PRIVATE cxx_std_${cxxVersion})
	set_target_properties(${tname} PROPERTIES CXX_STANDARD ${cxxVersion})

	# Tell CTest that we made a present for it
	add_test("${tname}" ${tname})

endfunction()

#
#	Creates a test for each C++ version jclib supports
#
#	@param testName Target name of the new tests
#   @param testSource Test C++ source file path, must be absolute
#
function(JCLIB_ADD_TEST_FN testName testSource)

	# Loop cxx targets
	foreach(cxv IN LISTS cxxversions)
		# Add test
		JCLIB_ADD_TEST_CXX_FN(${testName} ${testSource} ${cxv})

	endforeach()

endfunction()


#
#	Creates a test for each C++ version jclib supports
#
#	@param testName Target name of the new tests
#   @param testSource Test C++ source file path
#
macro(JCLIB_ADD_TEST testName testSource)

	# Make file path absolute
	set(__jclibaddtest_realpath )
	file(REAL_PATH ${testSource} __jclibaddtest_realpath)

	# Invoke function
	JCLIB_ADD_TEST_FN(${testName} ${__jclibaddtest_realpath})

endmacro()
