/*
	Tests the borrow_ptr type
*/

#include <jclib/memory.h>

#include <jclib-test.hpp>

struct BaseType {};
struct DerivedType : public BaseType {};



int subtest_construction_nullptr()
{
	NEWTEST();
	
	using type = int;
	
	jc::borrow_ptr<type> _ptr{ nullptr };
	ASSERT(_ptr.get() == nullptr, "borrow_ptr null construction should hold nullptr");

	PASS();
};
int subtest_construction_integral()
{
	NEWTEST();
	using type = int;
	
	type i{};
	
	auto _iptr = &i;
	jc::borrow_ptr<type> _bptr = &i;
	ASSERT(_bptr.get() == _iptr);

	PASS();
};
int subtest_construction_const_integral()
{
	NEWTEST();
	using type = int;
	using const_type = const type;

	// Test construction of const value pointer
	{
		const_type i{};
		auto _iptr = &i;
		jc::borrow_ptr<const_type> _bptr = &i;
		ASSERT(_bptr.get() == _iptr, "const value pointer mismatch");
	};

	// Test construction of const value pointer from non-const value pointer
	{
		type i{};
		auto _iptr = &i;
		jc::borrow_ptr<const_type> _bptr = &i;
		ASSERT(_bptr.get() == _iptr, "const value pointer from non const value pointer mismatch");
	};

	PASS();
};

// Tests borrow_ptr construction behavior
int subtest_construction()
{
	NEWTEST();
	SUBTEST(subtest_construction_nullptr);
	SUBTEST(subtest_construction_integral);
	SUBTEST(subtest_construction_const_integral);
	PASS();
};

// Tests borrow_ptr subtraction operator behavior
int subtest_distance()
{
	NEWTEST();

	// Test with regular pointers
	{
		using type = int;

		type lhs_val{};
		type rhs_val{};

		const auto _expected = &lhs_val - &rhs_val;
	
		jc::borrow_ptr<type> lhs = &lhs_val;
		jc::borrow_ptr<type> rhs = &rhs_val;
	
		const auto _result = lhs - rhs;
		ASSERT(_result == _expected, "distance result mismatch");
	};

	PASS();
};


template <typename T, typename DT>
int runcase_add(TESTCODE_VARDECL, jc::borrow_ptr<T> lhs, DT rhs, int _caseNum)
{
	const auto _ptr = lhs.get();
	const auto _expected = _ptr + rhs;
	const auto _result = lhs + rhs;
	ASSERT(_expected == _result.get(), "failed borrow_ptr + case ", std::to_string(_caseNum));
	PASS();
};
template <typename T, typename DT>
int runcase_addeq(TESTCODE_VARDECL, jc::borrow_ptr<T> lhs, DT rhs, int _caseNum)
{
	auto _ptr = lhs.get();
	auto _expected = (_ptr += rhs);
	auto _result = (lhs += rhs);
	ASSERT(_expected == _result.get(), "failed borrow_ptr += case ", std::to_string(_caseNum));
	PASS();
};

template <typename T, typename DT>
int runcase_sub(TESTCODE_VARDECL, jc::borrow_ptr<T> lhs, DT rhs, int _caseNum)
{
	const auto _ptr = lhs.get();
	const auto _expected = _ptr - rhs;
	const auto _result = lhs - rhs;
	ASSERT(_expected == _result.get(), "failed borrow_ptr - case ", std::to_string(_caseNum));
	PASS();
};
template <typename T, typename DT>
int runcase_subeq(TESTCODE_VARDECL, jc::borrow_ptr<T> lhs, DT rhs, int _caseNum)
{
	auto _ptr = lhs.get();
	auto _expected = (_ptr -= rhs);
	auto _result = (lhs -= rhs);
	ASSERT(_expected == _result.get(), "failed borrow_ptr -= case ", std::to_string(_caseNum));
	PASS();
};



// Tests borrow_ptr arithmetic behavior
int subtest_arithmetic()
{
	NEWTEST();

	using type = int;

	type _val{};
	type* _ptr = &_val;
	jc::borrow_ptr<type> _bptr{ &_val };
	
	// Testing out using a set of test cases to run
	using Case = int;
	const Case _testCases[] =
	{
		12,
		25,
		69,
		12551,
		-12,
		0
	};
	constexpr int _testCaseCount = sizeof(_testCases) / sizeof(Case);

	// Test "+" operator
	for (int n = 0; n != _testCaseCount; ++n)
	{
		SUBTEST(runcase_add, TESTCODE_VAR, _bptr, _testCases[n], n);
	};

	// Test "+=" operator
	for (int n = 0; n != _testCaseCount; ++n)
	{
		SUBTEST(runcase_addeq, TESTCODE_VAR, _bptr, _testCases[n], n);
	};

	// Test "-" operator
	for (int n = 0; n != _testCaseCount; ++n)
	{
		SUBTEST(runcase_sub, TESTCODE_VAR, _bptr, _testCases[n], n);
	};

	// Test "-=" operator
	for (int n = 0; n != _testCaseCount; ++n)
	{
		SUBTEST(runcase_subeq, TESTCODE_VAR, _bptr, _testCases[n], n);
	};


	PASS();
};





// Tests borrow_ptr comparison behavior
int subtest_comparison()
{
	NEWTEST();

	using type = int;

	type lhsval{}, rhsval{};

	type* lhsptr = &lhsval, *rhsptr = &rhsval;
	jc::borrow_ptr<type> lhs = &lhsval, rhs = &rhsval;

	// Test borrow_ptr == borrow_ptr
	{
		const auto _expected = (lhsptr == rhsptr);
		const auto _result = (lhs == rhs);
		ASSERT(_expected == _result, "equality comparison mismatch");
	};
	// Test borrow_ptr<T> == T*
	{
		const auto _expected = (lhsptr == lhsptr);
		const auto _result = (lhs == lhsptr);
		ASSERT(_expected == _result, "equality (borrow_ptr<T> == T*) comparison mismatch");
	};
	// Test T* == borrow_ptr<T> 
	{
		const auto _expected = (lhsptr == lhsptr);
		const auto _result = (lhsptr == lhs);
		ASSERT(_expected == _result, "equality (T* == borrow_ptr<T>) comparison mismatch");
	};

	// Test borrow_ptr != borrow_ptr
	{
		const auto _expected = (lhsptr != rhsptr);
		const auto _result = (lhs != rhs);
		ASSERT(_expected == _result, "!= comparison mismatch");
	};
	// Test borrow_ptr<T> != T*
	{
		const auto _expected = (lhsptr != lhsptr);
		const auto _result = (lhs != lhsptr);
		ASSERT(_expected == _result, "(borrow_ptr<T> != T*) comparison mismatch");
	};
	// Test T* != borrow_ptr<T> 
	{
		const auto _expected = (lhsptr != lhsptr);
		const auto _result = (lhsptr != lhs);
		ASSERT(_expected == _result, "(T* != borrow_ptr<T>) comparison mismatch");
	};

	// Test borrow_ptr > borrow_ptr
	{
		const auto _expected = (lhsptr > rhsptr);
		const auto _result = (lhs > rhs);
		ASSERT(_expected == _result, "> comparison mismatch");
	};
	// Test borrow_ptr<T> > T*
	{
		const auto _expected = (lhsptr > lhsptr);
		const auto _result = (lhs > lhsptr);
		ASSERT(_expected == _result, "(borrow_ptr<T> > T*) comparison mismatch");
	};
	// Test T* > borrow_ptr<T> 
	{
		const auto _expected = (lhsptr > lhsptr);
		const auto _result = (lhsptr > lhs);
		ASSERT(_expected == _result, "(T* > borrow_ptr<T>) comparison mismatch");
	};

	// Test borrow_ptr >= borrow_ptr
	{
		const auto _expected = (lhsptr >= rhsptr);
		const auto _result = (lhs >= rhs);
		ASSERT(_expected == _result, ">= comparison mismatch");
	};
	// Test borrow_ptr<T> >= T*
	{
		const auto _expected = (lhsptr >= lhsptr);
		const auto _result = (lhs >= lhsptr);
		ASSERT(_expected == _result, "(borrow_ptr<T> >= T*) comparison mismatch");
	};
	// Test T* >= borrow_ptr<T> 
	{
		const auto _expected = (lhsptr >= lhsptr);
		const auto _result = (lhsptr >= lhs);
		ASSERT(_expected == _result, "(T* >= borrow_ptr<T>) comparison mismatch");
	};

	// Test borrow_ptr > borrow_ptr
	{
		const auto _expected = (lhsptr < rhsptr);
		const auto _result = (lhs < rhs);
		ASSERT(_expected == _result, "< comparison mismatch");
	};
	// Test borrow_ptr<T> > T*
	{
		const auto _expected = (lhsptr < lhsptr);
		const auto _result = (lhs < lhsptr);
		ASSERT(_expected == _result, "(borrow_ptr<T> < T*) comparison mismatch");
	};
	// Test T* > borrow_ptr<T> 
	{
		const auto _expected = (lhsptr < lhsptr);
		const auto _result = (lhsptr < lhs);
		ASSERT(_expected == _result, "(T* < borrow_ptr<T>) comparison mismatch");
	};

	// Test borrow_ptr <= borrow_ptr
	{
		const auto _expected = (lhsptr <= rhsptr);
		const auto _result = (lhs <= rhs);
		ASSERT(_expected == _result, "<= comparison mismatch");
	};
	// Test borrow_ptr<T> <= T*
	{
		const auto _expected = (lhsptr <= lhsptr);
		const auto _result = (lhs <= lhsptr);
		ASSERT(_expected == _result, "(borrow_ptr<T> <= T*) comparison mismatch");
	};
	// Test T* <= borrow_ptr<T> 
	{
		const auto _expected = (lhsptr <= lhsptr);
		const auto _result = (lhsptr <= lhs);
		ASSERT(_expected == _result, "(T* <= borrow_ptr<T>) comparison mismatch");
	};

	PASS();
};






int main()
{
	NEWTEST();
	SUBTEST(subtest_construction);
	SUBTEST(subtest_distance);
	SUBTEST(subtest_arithmetic);
	SUBTEST(subtest_comparison);
	PASS();
};