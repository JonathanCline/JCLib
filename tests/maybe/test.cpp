#include <jclib/maybe.h>
#include <jclib-test.hpp>

#include <iostream>

int test_construction()
{
	NEWTEST();

	struct Foo
	{

	};
	Foo _f{};

	using ambig_maybe = jc::maybe<int, bool>;
	using diff_maybe = jc::maybe<int, Foo>;

	static_assert(!jc::is_constructible<ambig_maybe>::value, "should not be default constructible");
	static_assert(!jc::is_constructible<diff_maybe>::value, "should not be default constructible");
	
	ambig_maybe _val2{ 12 };
	ASSERT(_val2.has_value(), "ambigious maybe constructed second value when first was intended");
	
	ambig_maybe _val3{ jc::alternate, false };
	ASSERT(!_val3.has_value(), "ambigious maybe constructed first value when second was intended");

	diff_maybe _val4{ _f };
	ASSERT(!_val4.has_value(), "unambigious maybe constructed first value when second was intended");
	
	diff_maybe _val5{ jc::alternate, _f };
	ASSERT(!_val5.has_value(), "unambigious maybe constructed first value when second was intended");
	
	diff_maybe _val6{ 12 };
	ASSERT(_val6.has_value(), "unambigious maybe constructed second value when first was intended");

	return 0;
};
int test_copy()
{
	NEWTEST();

	struct Foo
	{

	};
	using maybe_type = jc::maybe<int, Foo>;

	maybe_type _src{ 12 };
	const auto _srcVal = _src.has_value();
	maybe_type _dest{ _src };
	const auto _destVal = _dest.has_value();

	ASSERT(_srcVal == _destVal, "bad maybe copy");

	PASS();
};
int test_move()
{
	NEWTEST();

	constexpr int _code = 4;

	struct Foo
	{

	};
	using maybe_type = jc::maybe<int, Foo>;

	maybe_type _src{ 12 };
	const auto _srcVal = _src.has_value();
	maybe_type _dest{ std::move(_src) };
	const auto _destVal = _dest.has_value();

	ASSERT(_srcVal == _destVal, "bad maybe move");

	PASS();
};
int test_trivial()
{
	NEWTEST();

	struct Foo
	{
		~Foo()
		{

		};
	};



	// Disabled while jc::maybe('s) triviality optimization is broken
#if JCLIB_VERSION_MAJOR >= 0 && JCLIB_VERSION_MINOR >= 3
	using trivial_type = jc::maybe<int, bool>;
	using non_trivial_type = jc::impl::maybe_data<jc::impl::maybe_union<int, Foo>>;

	non_trivial_type _f{ 2 };

	static_assert(jc::is_trivially_destructible<trivial_type>::value, "trivial maybe should be trivially destructible");
	static_assert(!jc::is_trivially_destructible<non_trivial_type>::value, "non-trivial maybe should not be trivially destructible");
#endif

	PASS();
};



using test_t = int(*)();

test_t tests[] =
{
	test_construction,
	test_copy,
	test_move,
	test_trivial
};



int main()
{
	NEWTEST();
	for (auto t : tests)
	{
		SUBTEST(t);
	};
	PASS();
};
