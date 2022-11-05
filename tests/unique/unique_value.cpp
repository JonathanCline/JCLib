#include <jclib-test.hpp>
#include <jclib/unique.h>

struct modified_int_uval_traits
{
	using value_type = int;
	static void reset(value_type&& v) {};
	static value_type null() { return 1; };
	static bool good(const value_type& v) { return v != null(); };
};
using modified_int_uval = jc::unique_value<int, modified_int_uval_traits>;

// Tests that null-related behavior works using a non-zero null value to ensure no default "null = 0" behavior occurs.
int subtest_null()
{
	NEWTEST();

	using uval = modified_int_uval;
	using traits = modified_int_uval::traits_type;

	// Test default constructor is null construction
	{
		auto uv = uval();
		ASSERT(uv.get() == traits::null(), "unique_value default constructor should construct its value using the traits' null function");
		ASSERT(!traits::good(uv.get()), "unique_value default constructor should construct its value using the traits' null function");
		ASSERT(!uv.good(), "unique_value good() should check if the held value is NOT null, got `true` when `false` was expected");
		ASSERT(!uv, "unique_value operator bool() should check if held value is NOT null and behave the same as the .good() function, got `true` when `false` was expected");
	};

	// Test value constructor obtains the given value
	{
		const auto v = 2;
		auto uv = uval((int)v);

		ASSERT(uv.get() == v, "unique_value assigning constructor did not obtain ownership of the given value");
		ASSERT(uv.good(), "unique_value good() should check if held value is NOT null, got `false` when `true` was expected");
		ASSERT(uv, "unique_value operator bool() should check if held value is NOT null and behave the same as the .good() function, got `false` when `true` was expected");
	};

	// Test the move constructor sets the source unique_value to null.
	{
		const auto v = 2;
		auto uv = uval((int)v);
		ASSERT(uv.good(), "failed test precondition");

		auto uv2 = uval(std::move(uv));
		ASSERT(uv2.good(), "move constructor should transfer ownership of the owned value to the constructed object");
		ASSERT(!uv.good(), "move constructor should release ownership of the owned value from the source object");
	};

	// Test the move assignment sets the source unique_value to null.
	{
		const auto v = 2;
		auto uv = uval((int)v);
		ASSERT(uv.good(), "failed test precondition");

		auto uv2 = uval();
		ASSERT(!uv2.good(), "failed test precondition");

		uv2 = std::move(uv);
		ASSERT(uv2.good(), "move assignment should transfer ownership of the owned value to the destination object");
		ASSERT(!uv.good(), "move assignment should release ownership of the owned value from the source object");
	};

	// Test the reset() function sets the unique_value to null.
	{
		const auto v = 2;
		auto uv = uval((int)v);
		ASSERT(uv.good(), "failed test precondition");

		uv.reset();
		ASSERT(!uv.good(), "unique_value reset() should set its held value to null");

		uv.reset();
		ASSERT(!uv.good(), "unique_value reset() should set its held value to null, this showed non-null after 2 sequential calls to reset, very bad news");
	};

	// Test the release() function returning it value and setting itself to null.  
	{
		const auto v = 2;
		auto uv = uval((int)v);
		ASSERT(uv.good(), "failed test precondition");

		const auto rv = uv.release();
		ASSERT(!uv.good(), "unique_value release() should set its held value to null");
	};

	PASS();
};

int main()
{
	NEWTEST();

	// Test null behavior
	SUBTEST(subtest_null);
	PASS();
};