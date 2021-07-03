#include <jclib/functional.h>



struct Foo
{
	constexpr auto operator+(int a) const noexcept
	{
		return Foo{ a + this->n };
	};
	int n = 0;
};

int add(int a, int b) { return a + b; };

int main()
{
	if (!jc::has_operator<jc::plus_t, Foo>::value)
	{
		return 1;
	};
	if (jc::has_operator<jc::minus_t, Foo>::value)
	{
		return 1;
	};

	{
		std::tuple<int, int> _args{ 2, 3 };
		if (jc::apply(add, _args) != 5)
		{
			return 2;
		};
	};

	return 0;
};