
add_executable(JCLib_FunctorTesting "test.cpp")
set_target_properties(JCLib_FunctorTesting PROPERTIES CXX_STANDARD 14)

target_compile_definitions(JCLib_FunctorTesting 
	INTERFACE JCLIB_CXX_STANDARD=${CMAKE_CXX_STANDARD})

target_link_libraries(JCLib_FunctorTesting PRIVATE SAELib)
add_test("JCLib_FunctorTesting" JCLib_FunctorTesting)
