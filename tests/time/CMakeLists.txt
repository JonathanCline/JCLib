
add_executable(JCLib_TimeTesting "time_test.cpp")
target_link_libraries(JCLib_TimeTesting PRIVATE JCLib)
add_test("JCLib_Time_test" JCLib_TimeTesting)

set_target_properties(JCLib_TimeTesting PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED True)
