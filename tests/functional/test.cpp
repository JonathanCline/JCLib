#include <jclib/functional.h>

#include <iostream>

#define FAIL(_code, _msg) { std::cout << "Failed test at '" <<  __LINE__ << "': " <<  _msg << '\n'; return _code; }
#define SUBTEST(_fn, ...) { const auto _res = _fn( __VA_ARGS__ ); if (_res != 0) { return _res; };  }

int add(int a, int b) { return a + b; };



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
		FAIL(1, "Failed to detect plus operator");
	};
	if (!jc::has_operator<jc::minus_t, Foo>::value)
	{
		FAIL(1, "Failed to detect minus operator");
	};
	if (jc::has_operator<jc::minus_t, Foo, int>::value)
	{
		FAIL(1, "Detected minus (with other type = int) operator when there wasn't one defined");
	};



	{
		std::tuple<int, int> _args{ 2, 3 };
		if (jc::apply(add, _args) != 5)
		{
			FAIL(2, "Apply did not return the correct value");
		};
	};

	return 0;
};