
add_executable(JCLib_TimerTesting "timer_test.cpp")
target_link_libraries(JCLib_TimerTesting PRIVATE JCLib)
add_test("JCLib_Timer_test" JCLib_TimerTesting)

set_target_properties(JCLib_TimerTesting PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED True)
