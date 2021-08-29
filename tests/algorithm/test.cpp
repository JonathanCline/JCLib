#include "jclib/algorithm.h"

#include <vector>
#include <numeric>
#include <string>
#include <functional>
#include <array>

#include <iostream>

void on_fail()
{
	return;
};

#define FAIL_I(_code, _msg) \
{	\
	std::vector<std::string> _parts	\
	{ "Failed test at '", std::to_string(__LINE__), "': ", _msg }; \
	for(auto& v : _parts) { std::cout << v; }; std::cout << '\n';	 \
	on_fail();\
	return _code; \
}

#define SUBTEST(_fn, ...) { const auto _res = _fn( __VA_ARGS__ ); if (_res != 0) { return _res; };  }

inline int newtest()
{
	static int count = 0;
	return ++count;
};

#define NEWTEST() const auto _testCode = ::newtest(); {}
#define FAIL(_msg) FAIL_I(_testCode, _msg)
#define PASS() { return 0; }

#define TIMPL_FRONT(_first, ...) _first

#define ASSERT(_condition, ...) if(!( _condition )) { FAIL(TIMPL_FRONT(__VA_ARGS__)); }

#define TEST_ALGORITHM_FN(algofn, expected, ...) ASSERT( expected == jc::algofn( __VA_ARGS__ ), #algofn " failed! (" #__VA_ARGS__ ")"  )



#define C_ARRAY_NAME _c_array
#define CPP_ARRAY_NAME _cpp_array
#define CONST_C_ARRAY_NAME _const_c_array
#define CONST_CPP_ARRAY_NAME _const_cpp_array



#define MAKE_ALGORITHM_TEST(name, algo, expcfn) \
template <typename RT, typename... Ts> \
int test_##algo##_fn_##name(RT& _range, Ts&&... _vals) \
{\
	const auto _expected = expcfn ;\
	NEWTEST(); \
	TEST_ALGORITHM_FN(algo, _expected, _range, _vals...); \
	PASS(); \
}; static_assert(true,"")



#define SUBTEST_ALGORITHM_CONST(name, algo, ...) \
SUBTEST(test_##algo##_fn_##name, CONST_C_ARRAY_NAME, __VA_ARGS__); \
SUBTEST(test_##algo##_fn_##name, CONST_CPP_ARRAY_NAME, __VA_ARGS__); \
static_assert(true, "")

#define SUBTEST_ALGORITHM_NON_CONST(name, algo, ...) \
SUBTEST(test_##algo##_fn_##name, C_ARRAY_NAME, __VA_ARGS__); \
SUBTEST(test_##algo##_fn_##name, CPP_ARRAY_NAME, __VA_ARGS__); \
static_assert(true, "")

#define SUBTEST_ALGORITHM(name, algo, ...) \
SUBTEST_ALGORITHM_NON_CONST(name, algo, __VA_ARGS__); \
SUBTEST_ALGORITHM_CONST(name, algo, __VA_ARGS__)





#define make_const_arrays(T, N, ...) \
const T CONST_C_ARRAY_NAME[N]  { __VA_ARGS__ }; \
const std::array<T, N> CONST_CPP_ARRAY_NAME { __VA_ARGS__ }; static_assert(true, "")

#define make_non_const_arrays(T, N, ...) \
T C_ARRAY_NAME[N] { __VA_ARGS__ } ; \
std::array<T, N> CPP_ARRAY_NAME { __VA_ARGS__ }; static_assert(true, "")

#define makearrays(T, N, ...) \
make_non_const_arrays(T, N, __VA_ARGS__); \
make_const_arrays(T, N, __VA_ARGS__)










// jc::find test
MAKE_ALGORITHM_TEST(a, find, (jc::next(jc::begin(_range), 5)));
int test_find()
{
	NEWTEST();
	makearrays(int, 8, 0, 1, 2, 3, 4, 5, 6, 7);
	SUBTEST_ALGORITHM(a, find, 5);
	PASS();
};

// jc::find_if test
MAKE_ALGORITHM_TEST(a, find_if, (jc::next(jc::begin(_range), 5)));
int test_find_if()
{
	NEWTEST();
	makearrays(int, 8, 0, 1, 2, 3, 4, 5, 6, 7);
	SUBTEST_ALGORITHM(a, find_if, jc::equals & 5);
	PASS();
};


// jc::fill test
int test_fill()
{
	NEWTEST();
	make_non_const_arrays(int, 8, 0, 1, 2, 2, 4, 5, 6, 7);

	{
		if (jc::contains(CPP_ARRAY_NAME, 3))
		{
			FAIL("fill algorithm had bad preconditions for c-array");
		};

		jc::fill(C_ARRAY_NAME, 3);
		for (auto& v : C_ARRAY_NAME)
		{
			if (v != 3)
			{
				FAIL("fill algorithm did not fill a c-array");
			};
		};
	};

	{
		if (jc::contains(CPP_ARRAY_NAME, 3))
		{
			FAIL("fill algorithm had bad preconditions for cpp-array");
		};

		jc::fill(CPP_ARRAY_NAME, 3);
		for (auto& v : CPP_ARRAY_NAME)
		{
			if (v != 3)
			{
				FAIL("fill algorithm did not fill a cpp-array");
			};
		}; 
	};

	PASS();
};


// jc::contains test

MAKE_ALGORITHM_TEST(good, contains, true);
MAKE_ALGORITHM_TEST(bad, contains, false);

int test_contains()
{
	NEWTEST();
	makearrays(int, 8, 0, 1, 2, 3, 4, 5, 6, 7);

	// Find a value not contained in the test ranges
	bool _keepLooking = true;
	int  _findVal = 0;
	while (_keepLooking)
	{
		// Reset flag
		_keepLooking = false;
		
		// Check if the value is contained
		for (auto& v : CPP_ARRAY_NAME)
		{
			if (_findVal == v)
			{
				_keepLooking = true;
				break;
			};
		};

		// Check flag and increment find val if we are going to continue
		if (_keepLooking)
		{
			++_findVal;
		};
	};

	// Value that should be contained
	const auto _findContainedVal = CPP_ARRAY_NAME.front();

	// Value that shouldn't be contained
	const auto _findNotContainedVal = _findVal;

	// Run tests
	SUBTEST_ALGORITHM(good, contains, _findContainedVal);
	SUBTEST_ALGORITHM(bad,  contains, _findNotContainedVal);

	PASS();
};



// jc::contains_if test

MAKE_ALGORITHM_TEST(good, contains_if, true);
MAKE_ALGORITHM_TEST(bad, contains_if, false);

int test_contains_if()
{
	NEWTEST();
	makearrays(int, 8, 0, 1, 2, 3, 4, 5, 6, 7);

	// Find a value not contained in the test ranges
	bool _keepLooking = true;
	int  _findVal = 0;
	while (_keepLooking)
	{
		// Reset flag
		_keepLooking = false;

		// Check if the value is contained
		for (auto& v : CPP_ARRAY_NAME)
		{
			if (_findVal == v)
			{
				_keepLooking = true;
				break;
			};
		};

		// Check flag and increment find val if we are going to continue
		if (_keepLooking)
		{
			++_findVal;
		};
	};

	// Value that should be contained
	const auto _findContainedVal = CPP_ARRAY_NAME.front();

	// Value that shouldn't be contained
	const auto _findNotContainedVal = _findVal;

	// Run tests
	SUBTEST_ALGORITHM(good, contains_if, jc::equals & _findContainedVal);
	SUBTEST_ALGORITHM(bad, contains_if, jc::equals & _findNotContainedVal);

	PASS();
};







int main()
{
	NEWTEST();

	SUBTEST(test_find);
	SUBTEST(test_find_if);
	
	SUBTEST(test_fill);

	SUBTEST(test_contains);
	SUBTEST(test_contains_if);


	PASS();
};
