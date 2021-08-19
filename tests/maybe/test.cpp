#include <jclib/maybe.h>

#include <iostream>

#define FAIL(_code, _msg) { std::cout << "Failed test at '" <<  __LINE__ << "': " <<  _msg << '\n'; return _code; }
#define SUBTEST(_fn, ...) { const auto _res = _fn( __VA_ARGS__ ); if (_res != 0) { return _res; };  }

int test_construction()
{
	constexpr int _code = 2;

	struct Foo
	{

	};
	Foo _f{};

	using ambig_maybe = jc::maybe<int, bool>;
	using diff_maybe = jc::maybe<int, Foo>;

	ambig_maybe _val{};
	if (_val.has_value())
	{
		FAIL(_code, "ambigious default constructor did not construct first value");
	};
	
	ambig_maybe _val2{ 12 };
	if (!_val2.has_value())
	{
		FAIL(_code, "ambigious maybe constructed second value when first was intended");
	};
	
	ambig_maybe _val3{ jc::alternate, false };
	if (_val3.has_value())
	{
		FAIL(_code, "ambigious maybe constructed first value when second was intended");
	};

	diff_maybe _val4{ _f };
	if (_val4.has_value())
	{
		FAIL(_code, "unambigious maybe constructed first value when second was intended");
	};

	diff_maybe _val5{ jc::alternate, _f };
	if (_val5.has_value())
	{
		FAIL(_code, "unambigious maybe constructed first value when second was intended");
	};

	diff_maybe _val6{ 12 };
	if (!_val6.has_value())
	{
		FAIL(_code, "unambigious maybe constructed second value when first was intended");
	};

	return 0;
};
int test_copy()
{
	constexpr int _code = 3;
	
	struct Foo
	{

	};
	using maybe_type = jc::maybe<int, Foo>;

	maybe_type _src{ 12 };
	const auto _srcVal = _src.has_value();
	maybe_type _dest{ _src };
	const auto _destVal = _dest.has_value();

	if (_srcVal != _destVal)
	{
		FAIL(_code, "bad maybe copy")
	};

	return 0;
};
int test_move()
{
	constexpr int _code = 4;

	struct Foo
	{

	};
	using maybe_type = jc::maybe<int, Foo>;

	maybe_type _src{ 12 };
	const auto _srcVal = _src.has_value();
	maybe_type _dest{ std::move(_src) };
	const auto _destVal = _dest.has_value();

	if (_srcVal != _destVal)
	{
		FAIL(_code, "bad maybe move")
	};

	return 0;
};
int test_trivial()
{
	constexpr int _code = 5;

	struct Foo
	{
		~Foo()
		{

		};
	};


	// Disabled while jc::maybe('s) triviality optimization is broken
#if JCLIB_VERSION_MAJOR >= 0 && JCLIB_VERSION_MINOR >= 3
	using trivial_type = jc::maybe<int, bool>;
	using non_trivial_type = jc::maybe<int, Foo>;

	if (!jc::is_trivially_destructible<trivial_type>::value)
	{
		FAIL(_code, "trivial maybe should be trivially destructible");
	};
	if (jc::is_trivially_destructible<non_trivial_type>::value)
	{
		FAIL(_code, "non-trivial maybe should not be trivially destructible");
	};
#endif

	return 0;
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
	for (auto t : tests)
	{
		SUBTEST(t);
	};
	return 0;
};
