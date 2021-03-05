
add_executable(JCLib_EitherTesting "test.cpp")
set_target_properties(JCLib_EitherTesting PROPERTIES CXX_STANDARD 14)

target_compile_definitions(JCLib_EitherTesting 
	INTERFACE JCLIB_CXX_STANDARD=${CMAKE_CXX_STANDARD})

target_link_libraries(JCLib_EitherTesting PRIVATE JCLib)
add_test("JCLib_EitherTesting_T" JCLib_EitherTesting)
