
add_executable(JCLib_ConceptsTesting "concepts_test.cpp")
target_link_libraries(JCLib_ConceptsTesting PRIVATE JCLib)
add_test("JCLib_Concepts_test" JCLib_ConceptsTesting)

set_target_properties(JCLib_ConceptsTesting PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED True)
