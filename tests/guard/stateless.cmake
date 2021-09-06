# guard-stateless Test Driver


JCLIB_ADD_TEST("guard-stateless" "${CMAKE_CURRENT_LIST_DIR}/stateless.cpp")

# Old stateless guard testing, I'm leaving it as it may catch a case I miss later
JCLIB_ADD_TEST("guard-stateless_old" "${CMAKE_CURRENT_LIST_DIR}/stateless_old.cpp")
