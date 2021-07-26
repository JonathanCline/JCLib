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





int add(int a, int b) { return a + b; };


int test_piping()
{
	NEWTEST();

	constexpr auto add_2 = jc::plus & 2;
	constexpr auto n = 2 | add_2;


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

int main()
{
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

	SUBTEST(test_piping);

	return 0;
};