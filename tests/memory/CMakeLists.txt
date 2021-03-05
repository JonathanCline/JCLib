
add_executable(JCLib_MemoryTesting "memory_test.cpp")
target_link_libraries(JCLib_MemoryTesting PRIVATE JCLib)
add_test("JCLib_Memory_test" JCLib_MemoryTesting)

set_target_properties(JCLib_MemoryTesting PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED True)
