#include <jclib/functional.h>

#include <vector>
#include <numeric>
#include <string>
#include <functional>
#include <array>

#include <iostream>

#define FAIL_I(_code, _msg) \
{	\
	std::vector<std::string> _parts	\
	{ "Failed test at '", std::to_string(__LINE__), "': ", _msg }; \
	for(auto& v : _parts) { std::cout << v; }; std::cout << '\n';	 \
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




// jc::plus test
int test_op_plus()
{
	NEWTEST();

	constexpr auto operator_v = jc::plus;
	using value_type = int;

	static_assert(jc::has_operator<decltype(operator_v), value_type, value_type>::value, "missing operator");
	static_assert(jc::has_operator<decltype(operator_v), value_type>::value, "missing self-applied operator");


	constexpr value_type operand_a_v = 2;
	constexpr value_type operand_b_v = 2;
	constexpr value_type expected_v = operand_a_v + operand_b_v;


	const value_type a = operand_a_v;
	const value_type b = operand_b_v;
	
	{
		const value_type q = operator_v(a, b);
		ASSERT(q == expected_v, "plus operator failed");
	};

	{
		const value_type q = b | (a & operator_v);
		ASSERT(q == expected_v, "bound and piped plus operator failed");
	};

	PASS();
};

// jc::minus test
int test_op_minus()
{
	NEWTEST();

	constexpr auto operator_v = jc::minus;

	using value_type = int;

	static_assert(jc::has_operator<decltype(operator_v), value_type, value_type>::value, "missing operator");
	static_assert(jc::has_operator<decltype(operator_v), value_type>::value, "missing self-applied operator");


	constexpr value_type operand_a_v = 12;
	constexpr value_type operand_b_v = 2;
	constexpr value_type expected_v = operand_a_v - operand_b_v;


	const value_type a = operand_a_v;
	const value_type b = operand_b_v;

	{
		const value_type q = operator_v(a, b);
		ASSERT(q == expected_v, "minus operator failed");
	};

	{
		const value_type q = b | (a & operator_v);
		ASSERT(q == expected_v, "bound and piped minus operator failed");
	};

	PASS();
};

// jc::times test
int test_op_times()
{
	NEWTEST();

	constexpr auto operator_v = jc::times;

	using value_type = int;

	static_assert(jc::has_operator<decltype(operator_v), value_type, value_type>::value, "missing operator");
	static_assert(jc::has_operator<decltype(operator_v), value_type>::value, "missing self-applied operator");


	constexpr value_type operand_a_v = 12;
	constexpr value_type operand_b_v = 2;
	constexpr value_type expected_v = operand_a_v * operand_b_v;



	const value_type a = operand_a_v;
	const value_type b = operand_b_v;

	{
		const value_type q = operator_v(a, b);
		ASSERT(q == expected_v, "times operator failed");
	};

	{
		const value_type q = b | (a & operator_v);
		ASSERT(q == expected_v, "bound and piped times operator failed");
	};

	PASS();
};

// jc::divide test
int test_op_divide()
{
	NEWTEST();

	constexpr auto operator_v = jc::divide;

	using value_type = int;

	static_assert(jc::has_operator<decltype(operator_v), value_type, value_type>::value, "missing operator");
	static_assert(jc::has_operator<decltype(operator_v), value_type>::value, "missing self-applied operator");


	constexpr value_type operand_a_v = 12;
	constexpr value_type operand_b_v = 2;
	constexpr value_type expected_v = operand_a_v / operand_b_v;



	const value_type a = operand_a_v;
	const value_type b = operand_b_v;

	{
		const value_type q = operator_v(a, b);
		ASSERT(q == expected_v, "divide operator failed");
	};

	{
		const value_type q = b | (a & operator_v);
		ASSERT(q == expected_v, "bound and piped divide operator failed");
	};

	PASS();
};



// jc::conjunct test
int test_op_conjuct()
{
	NEWTEST();

	constexpr auto operator_v = jc::conjunct;
	using value_type = bool;

	static_assert(jc::has_operator<decltype(operator_v), value_type, value_type>::value, "missing operator");
	static_assert(jc::has_operator<decltype(operator_v), value_type>::value, "missing self-applied operator");

	{
		constexpr value_type operand_a_v = false;
		constexpr value_type operand_b_v = false;
		constexpr value_type expected_v = operand_a_v && operand_b_v;

		const value_type a = operand_a_v;
		const value_type b = operand_b_v;

		{
			const value_type q = operator_v(a, b);
			ASSERT(q == expected_v, "conjunct operator failed");
		};

		{
			const value_type q = operator_v(b, a);
			ASSERT(q == expected_v, "conjunct operator failed");
		};

		{
			const value_type q = b | (a & operator_v);
			ASSERT(q == expected_v, "bound and piped conjunct operator failed");
		};

		{
			const value_type q = a | (b & operator_v);
			ASSERT(q == expected_v, "bound and piped conjunct operator failed");
		};
	};

	{
		constexpr value_type operand_a_v = false;
		constexpr value_type operand_b_v = true;
		constexpr value_type expected_v = operand_a_v && operand_b_v;

		const value_type a = operand_a_v;
		const value_type b = operand_b_v;

		{
			const value_type q = operator_v(a, b);
			ASSERT(q == expected_v, "conjunct operator failed");
		};

		{
			const value_type q = operator_v(b, a);
			ASSERT(q == expected_v, "conjunct operator failed");
		};

		{
			const value_type q = b | (a & operator_v);
			ASSERT(q == expected_v, "bound and piped conjunct operator failed");
		};

		{
			const value_type q = a | (b & operator_v);
			ASSERT(q == expected_v, "bound and piped conjunct operator failed");
		};
	};

	{
		constexpr value_type operand_a_v = true;
		constexpr value_type operand_b_v = true;
		constexpr value_type expected_v = operand_a_v && operand_b_v;

		const value_type a = operand_a_v;
		const value_type b = operand_b_v;

		{
			const value_type q = operator_v(a, b);
			ASSERT(q == expected_v, "conjunct operator failed");
		};

		{
			const value_type q = operator_v(b, a);
			ASSERT(q == expected_v, "conjunct operator failed");
		};

		{
			const value_type q = b | (a & operator_v);
			ASSERT(q == expected_v, "bound and piped conjunct operator failed");
		};

		{
			const value_type q = a | (b & operator_v);
			ASSERT(q == expected_v, "bound and piped conjunct operator failed");
		};
	};

	PASS();
};

// jc::disjunct test
int test_op_disjunct()
{
	NEWTEST();

	constexpr auto operator_v = jc::disjunct;
	using value_type = bool;

	static_assert(jc::has_operator<decltype(operator_v), value_type, value_type>::value, "missing operator");
	static_assert(jc::has_operator<decltype(operator_v), value_type>::value, "missing self-applied operator");

	{
		constexpr value_type operand_a_v = false;
		constexpr value_type operand_b_v = false;
		constexpr value_type expected_v = operand_a_v || operand_b_v;

		const value_type a = operand_a_v;
		const value_type b = operand_b_v;

		{
			const value_type q = operator_v(a, b);
			ASSERT(q == expected_v, "disjunct operator failed");
		};

		{
			const value_type q = operator_v(b, a);
			ASSERT(q == expected_v, "disjunct operator failed");
		};

		{
			const value_type q = b | (a & operator_v);
			ASSERT(q == expected_v, "bound and piped disjunct operator failed");
		};
		
		{
			const value_type q = a | (b & operator_v);
			ASSERT(q == expected_v, "bound and piped disjunct operator failed");
		};
	};

	{
		constexpr value_type operand_a_v = false;
		constexpr value_type operand_b_v = true;
		constexpr value_type expected_v = operand_a_v || operand_b_v;

		const value_type a = operand_a_v;
		const value_type b = operand_b_v;

		{
			const value_type q = operator_v(a, b);
			ASSERT(q == expected_v, "disjunct operator failed");
		};

		{
			const value_type q = operator_v(b, a);
			ASSERT(q == expected_v, "disjunct operator failed");
		};

		{
			const value_type q = b | (a & operator_v);
			ASSERT(q == expected_v, "bound and piped disjunct operator failed");
		};

		{
			const value_type q = a | (b & operator_v);
			ASSERT(q == expected_v, "bound and piped disjunct operator failed");
		};
	};

	{
		constexpr value_type operand_a_v = true;
		constexpr value_type operand_b_v = true;
		constexpr value_type expected_v = operand_a_v || operand_b_v;

		const value_type a = operand_a_v;
		const value_type b = operand_b_v;

		{
			const value_type q = operator_v(a, b);
			ASSERT(q == expected_v, "disjunct operator failed");
		};

		{
			const value_type q = operator_v(b, a);
			ASSERT(q == expected_v, "disjunct operator failed");
		};

		{
			const value_type q = b | (a & operator_v);
			ASSERT(q == expected_v, "bound and piped disjunct operator failed");
		};

		{
			const value_type q = a | (b & operator_v);
			ASSERT(q == expected_v, "bound and piped disjunct operator failed");
		};
	};

	PASS();
};



// jc::dereference test
int test_op_dereference()
{
	NEWTEST();

	constexpr auto operator_v = jc::dereference;

	// Test with non-const value pointer
	{
		using value_type = int;

		static_assert(jc::has_operator<decltype(operator_v), value_type*>::value, "missing operator");
		static_assert(jc::is_invocable_with_count<decltype(operator_v), 1>::value, "failed function probing");
		
		value_type n = 125;
		value_type* nptr = &n;

		// Test invocation
		{
			auto q = operator_v(nptr);
			ASSERT(q == n, "dereference operator failed");
		};
		
		// Test piped invocation
		{
			auto q = nptr | operator_v;
			ASSERT(q == n, "piped dereference operator failed");
		};

		// Test packed piped invocation
		{
			auto q = jc::pack(nptr) | operator_v;
			ASSERT(q == n, "packed and piped dereference operator failed");
		};

	};

	// Test with const value pointer
	{
		using value_type = const int;

		static_assert(jc::has_operator<decltype(operator_v), const value_type*>::value, "missing operator");

		value_type n = 125;
		value_type* nptr = &n;

		// Test invocation
		{
			auto q = operator_v(nptr);
			ASSERT(q == n, "dereference operator with const value type failed");
		};

		// Test piped invocation
		{
			auto q = nptr | operator_v;
			ASSERT(q == n, "piped dereference operator with const value type failed");
		};

		// Test packed piped invocation
		{
			auto q = jc::pack(nptr) | operator_v;
			ASSERT(q == n, "packed and piped dereference operator with const value type failed");
		};
	};

	PASS();
};

// jc::invert test
int test_op_invert()
{
	NEWTEST();
	constexpr auto operator_v = jc::invert;

	// Test operator
	{
		using value_type = bool;

		static_assert(jc::has_operator<decltype(operator_v), value_type>::value, "missing operator");

		const value_type initial_v = false;
		const value_type new_v = true;

		value_type i = initial_v;

		ASSERT(i == initial_v, "invalid invert test condition");

		i = operator_v(i);
		ASSERT(i == new_v, "invert  operator failed");

		i = (i | operator_v);
		ASSERT(i == initial_v, "piped invert operator failed");
	};

	PASS();
};

// jc::negate test
int test_op_negate()
{
	NEWTEST();
	constexpr auto operator_v = jc::negate;

	// Test operator
	{
		using value_type = int;

		static_assert(jc::has_operator<decltype(operator_v), value_type>::value, "missing operator");

		const value_type initial_v = 5;
		const value_type new_v = -5;

		value_type i = initial_v;

		ASSERT(i == initial_v, "invalid negate test condition");

		i = operator_v(i);
		ASSERT(i == new_v, "negate operator failed");

		i = (i | operator_v);
		ASSERT(i == initial_v, "piped negate operator failed");
	};

	PASS();
};

// jc::modulo test
int test_op_modulo()
{
	NEWTEST();
	constexpr auto operator_v = jc::modulo;

	// Test operator
	{
		using value_type = int;

		static_assert(jc::has_operator<decltype(operator_v), value_type>::value, "missing operator");

		const value_type lhs_v = 10;
		const value_type rhs_v = 4;
		const value_type result_v = 2;

		value_type i = operator_v(lhs_v, rhs_v);
		ASSERT(i == result_v, "modulo operator failed");

		i = (lhs_v | operator_v & rhs_v);
		ASSERT(i == result_v, "piped modulo operator failed");
	};

	// Test operator
	{
		using value_type = int;

		static_assert(jc::has_operator<decltype(operator_v), value_type>::value, "missing operator");

		const value_type lhs_v = 11;
		const value_type rhs_v = 4;
		const value_type result_v = 3;

		value_type i = operator_v(lhs_v, rhs_v);
		ASSERT(i == result_v, "modulo operator failed");

		i = (lhs_v | operator_v & rhs_v);
		ASSERT(i == result_v, "piped modulo operator failed");
	};

	// Test operator
	{
		using value_type = int;

		static_assert(jc::has_operator<decltype(operator_v), value_type>::value, "missing operator");

		const value_type lhs_v = 12;
		const value_type rhs_v = 4;
		const value_type result_v = 0;

		value_type i = operator_v(lhs_v, rhs_v);
		ASSERT(i == result_v, "modulo operator failed");

		i = (lhs_v | operator_v & rhs_v);
		ASSERT(i == result_v, "piped modulo operator failed");
	};

	PASS();
};



// jc::bnot test
int test_op_bnot()
{
	NEWTEST();
	constexpr auto operator_v = jc::bnot;

	// Test operator
	{
		using value_type = int;

		static_assert(jc::has_operator<decltype(operator_v), value_type>::value, "missing operator");
		static_assert(jc::is_invocable_with_count<decltype(operator_v), 1>::value, "failed function probing");

		const value_type initial_v = 0b00001111;
		const value_type new_v = ~initial_v;

		value_type i = initial_v;

		ASSERT(i == initial_v, "invalid bnot test condition");

		i = operator_v(i);
		ASSERT(i == new_v, "bnot operator failed");

		i = (i | operator_v);
		ASSERT(i == initial_v, "piped bnot operator failed");

		i = jc::pack(i) | operator_v;
		ASSERT(i == new_v, "packed and piped bnot operator failed");
	};

	PASS();
};

// jc::band test
int test_op_band()
{
	NEWTEST();

	constexpr auto operator_v = jc::band;
	using value_type = int;

	static_assert(jc::has_operator<decltype(operator_v), value_type, value_type>::value, "missing operator");
	static_assert(jc::has_operator<decltype(operator_v), value_type>::value, "missing self-applied operator");
	static_assert(jc::is_invocable_with_count<decltype(operator_v), 2>::value, "failed function probing");

	constexpr value_type operand_a_v = 0b00011011;
	constexpr value_type operand_b_v = 0b00000111;
	constexpr value_type expected_v = operand_a_v & operand_b_v;

	const value_type a = operand_a_v;
	const value_type b = operand_b_v;

	{
		const value_type q = operator_v(a, b);
		ASSERT(q == expected_v, "band operator failed");
	};

	{
		const value_type q = b | (a & operator_v);
		ASSERT(q == expected_v, "bound and piped band operator failed");
	};

	{
		const value_type q = jc::pack(a, b) | operator_v;
		ASSERT(q == expected_v, "packed and piped band operator failed");
	};

	PASS();
};

// jc::bor test
int test_op_bor()
{
	NEWTEST();

	constexpr auto operator_v = jc::bor;
	using value_type = int;

	static_assert(jc::has_operator<decltype(operator_v), value_type, value_type>::value, "missing operator");
	static_assert(jc::has_operator<decltype(operator_v), value_type>::value, "missing self-applied operator");
	static_assert(jc::is_invocable_with_count<decltype(operator_v), 2>::value, "failed function probing");

	constexpr value_type operand_a_v = 0b00011011;
	constexpr value_type operand_b_v = 0b00000111;
	constexpr value_type expected_v = operand_a_v | operand_b_v;

	const value_type a = operand_a_v;
	const value_type b = operand_b_v;

	{
		const value_type q = operator_v(a, b);
		ASSERT(q == expected_v, "bor operator failed");
	};

	{
		const value_type q = b | (a & operator_v);
		ASSERT(q == expected_v, "bound and piped bor operator failed");
	};

	{
		const value_type q = jc::pack(a, b) | operator_v;
		ASSERT(q == expected_v, "packed and piped bor operator failed");
	};

	PASS();
};

// jc::bxor test
int test_op_bxor()
{
	NEWTEST();

	constexpr auto operator_v = jc::bxor;
	using value_type = int;

	static_assert(jc::has_operator<decltype(operator_v), value_type, value_type>::value, "missing operator");
	static_assert(jc::has_operator<decltype(operator_v), value_type>::value, "missing self-applied operator");
	static_assert(jc::is_invocable_with_count<decltype(operator_v), 2>::value, "failed function probing");

	constexpr value_type operand_a_v = 0b00011011;
	constexpr value_type operand_b_v = 0b00000111;
	constexpr value_type expected_v = operand_a_v ^ operand_b_v;

	const value_type a = operand_a_v;
	const value_type b = operand_b_v;

	{
		const value_type q = operator_v(a, b);
		ASSERT(q == expected_v, "bxor operator failed");
	};

	{
		const value_type q = b | (a & operator_v);
		ASSERT(q == expected_v, "bound and piped bxor operator failed");
	};

	{
	const value_type q = jc::pack(a, b) | operator_v;
	ASSERT(q == expected_v, "packed and piped bxor operator failed");
	};

	PASS();
};



// jc::address_of test
int test_op_address_of()
{
	NEWTEST();

	constexpr auto operator_v = jc::address_of;

	// Test with non-const value
	{
		using value_type = int;

		static_assert(jc::has_operator<decltype(operator_v), value_type&>::value, "missing operator");
		static_assert(jc::is_invocable_with_count<decltype(operator_v), 1>::value, "failed function probing");

		value_type n = 125;
		value_type* nptr = &n;

		// Test invocation
		{
			auto q = operator_v(n);
			ASSERT(q == nptr, "address_of operator failed");
		};

		// Test piped invocation
		{
			auto q = n | operator_v;
			ASSERT(q == nptr, "piped address_of operator failed");
		};

#if JCLIB_VERSION_MAJOR >= 0 && JCLIB_VERSION_MINOR >= 2 && JCLIB_VERSION_PATCH >= 3
		// Test packed piped invocation
		{
			auto q = jc::pack(n) | operator_v;
			ASSERT(q == nptr, "packed and piped address_of operator failed");
		};
#endif

	};

	// Test with const value pointer
	{
		using value_type = const int;

		static_assert(jc::has_operator<decltype(operator_v), value_type&>::value, "missing operator");

		value_type n = 125;
		value_type* nptr = &n;

		// Test invocation
		{
			auto q = operator_v(n);
			ASSERT(q == nptr, "address_of operator with const value type failed");
		};

		// Test piped invocation
		{
			auto q = n | operator_v;
			ASSERT(q == nptr, "piped address_of operator with const value type failed");
		};

#if JCLIB_VERSION_MAJOR >= 0 && JCLIB_VERSION_MINOR >= 2 && JCLIB_VERSION_PATCH >= 3
		// Test packed piped invocation
		{
			auto q = jc::pack(n) | operator_v;
			ASSERT(q == nptr, "packed and piped address_of operator with const value type failed");
		};
#endif
	};

	PASS();
};


int test_op_member()
{
	NEWTEST();

	// Testing operator value alias
	constexpr auto operator_v = jc::member;

	// Test arguement binding
	{
		using value_type = std::pair<int, int>;
		
		constexpr auto first_fn = operator_v & (&value_type::first);
		constexpr auto second_fn = operator_v & (&value_type::second);

		value_type v{ 1, 12 };
		auto& first = v.first;
		auto& second = v.second;

		// Test invocation access
		{
			auto& q = first_fn(v);
			ASSERT(q == first, "bad member access operator (first)");
		};
		{
			auto& q = second_fn(v);
			ASSERT(q == second, "bad member access operator (second)");
		};

		// Test piped access
		{
			auto& q = v | first_fn;
			ASSERT(q == first, "bad piped member access operator (first)");
		};
		{
			auto& q = v | second_fn;
			ASSERT(q == second, "bad piped member access operator (second)");
		};

#if JCLIB_VERSION_MAJOR >= 0 && JCLIB_VERSION_MINOR >= 2 && JCLIB_VERSION_PATCH >= 3
		// Test packed piped access
		{
			auto q = jc::pack(v) | first_fn;
			ASSERT(q == first, "bad piped member access operator (first)");
		};
		{
			auto& q = jc::pack(v) | second_fn;
			ASSERT(q == second, "bad piped member access operator (second)");
		};
#endif
	};

	PASS();
};



// Runs the tests for the various operators defined by jclib/functional.h
int test_operators()
{
	NEWTEST();
	
	// Test arithmatic operators

	SUBTEST(test_op_plus);
	SUBTEST(test_op_minus);
	SUBTEST(test_op_times);
	SUBTEST(test_op_divide);

	SUBTEST(test_op_modulo);
	SUBTEST(test_op_negate);


	// Test binary arithmetic operators

	SUBTEST(test_op_bnot);
	SUBTEST(test_op_band);
	SUBTEST(test_op_bor);
	SUBTEST(test_op_bxor);



	// Test logical operatorss

	SUBTEST(test_op_invert);

	SUBTEST(test_op_conjuct);
	SUBTEST(test_op_disjunct);



	// Test other operators

	SUBTEST(test_op_dereference);
	SUBTEST(test_op_address_of);
	SUBTEST(test_op_member);
	

	PASS();
};



// Tests the arguement pack piping behavior
int test_arguement_pack_piping()
{
	NEWTEST();
	
	// Composed function used for the static assertions
	constexpr auto test_fn = jc::plus | jc::equals & 4;
	
	// Extra testing to ensure function composition maintains the required traits
	static_assert(jc::is_operator<decltype(test_fn)>::value, "composed test function failed is_opertor trait check");
	static_assert(jc::is_invocable_with_count<decltype(test_fn), 2>::value, "composed test function failed is_invocable_with_count check");

	// Test piping of immediate pack
	static_assert(jc::pack(2, 2) | test_fn,
		"failed expansion of piped arguement pack");

	// Ensure that re-repacking doesn't break the piping
	static_assert(jc::pack(2, 2) | jc::repack | test_fn,
		"failed repacking and piping of expanded arguement pack");

	// Test piping of repacked immediate pack
	static_assert(std::pair<int, int>{2, 2} | jc::repack | test_fn,
		"failed repacking and piping of expanded arguement pack");

	// Ensure that re-repacking doesn't break the piping
	static_assert(std::pair<int, int>{ 2, 2} | jc::repack | jc::repack | test_fn,
		"failed repacking and piping of expanded arguement pack");


	
	// Testing for piping of existing pack

	constexpr std::pair<int, int> args_v{ 2, 2 };
	constexpr auto packed_args_v = jc::pack(2, 2);


	// Test piping of existing pack
	static_assert(packed_args_v | test_fn,
		"failed expansion of piped arguement pack");

	// Ensure that re-repacking doesn't break the piping
	static_assert(packed_args_v | jc::repack | test_fn,
		"failed repacking and piping of expanded arguement pack");

	// Test piping of repacked non-pack value
	static_assert(args_v | jc::repack | test_fn,
		"failed repacking and piping of expanded arguement pack");

	// Ensure that re-repacking doesn't break the piping
	static_assert(args_v | jc::repack | jc::repack | test_fn,
		"failed repacking and piping of expanded arguement pack");





	PASS();
};


// Main subtest for all jclib functional piping
int test_piping()
{
	NEWTEST();




	// Test arguement pack piping
	SUBTEST(test_arguement_pack_piping);


	PASS();
};




struct Foo
{
	constexpr auto operator+(int a) const noexcept
	{
		return Foo{ a + this->n };
	};
	constexpr auto operator-(Foo b) const noexcept
	{
		return Foo{ this->n - b.n };
	};
	int n = 0;
};

constexpr auto add(int a, int b) { return a + b; };

#include <jclib/ranges.h>
#include <jclib/algorithm.h>

int main()
{
	

	SUBTEST(test_operators);
	SUBTEST(test_piping);

	if (!jc::has_operator<jc::plus_t, Foo, int>::value)
	{
		NEWTEST();
		FAIL("Failed to detect plus operator");
	};
	if (!jc::has_operator<jc::minus_t, Foo>::value)
	{
		NEWTEST();
		FAIL("Failed to detect minus operator");
	};
	if (jc::has_operator<jc::minus_t, Foo, int>::value)
	{
		NEWTEST();
		FAIL("Detected minus(with other type = int) operator when there wasn't one defined");
	};

	{
		std::tuple<int, int> _args{ 2, 3 };
		if (jc::apply(add, _args) != 5)
		{
			NEWTEST();
			FAIL("Apply did not return the correct value");
		};
	};

	PASS();
};