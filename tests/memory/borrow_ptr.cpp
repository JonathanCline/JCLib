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
int runcase_add( TESTCODE_VAR, jc::borrow_ptr<T> lhs, DT rhs)
{
	const auto _ptr = lhs.get();
	const auto _expected = _ptr + rhs;
	const auto _result = lhs + rhs;
	
};


// Tests borrow_ptr arithmetic behavior
int subtest_distance()
{
	NEWTEST();

	using type = int;

	type _val{};
	type* _ptr = &_val;
	jc::borrow_ptr<type> _bptr{ &_val };

	{
		// Test "+" operator
		const auto _operand = 12;
		
		// Test lhs
		{
			const auto _expected = _ptr + _operand;
			const auto _result = _val + _operand;
			ASSERT(_bptr.get() == _expected);
		};
	};

	PASS();
};



int main()
{
	NEWTEST();
	SUBTEST(subtest_construction);
	SUBTEST(subtest_distance);
	PASS();
};