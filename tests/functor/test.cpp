
#include <jclib/functor.h>

int foo(int _a, int _b)
{
	return _a + _b;
};
int foo_noexcept(int _a, int _b) noexcept
{
	return _a + _b;
};

struct Bar
{
	int foobar(int _a, int _b)
	{
		return _a + _b;
	};
	int foobar_noexcept(int _a, int _b) noexcept
	{
		return _a + _b;
	};

	int foobar_const(int _a, int _b) const
	{
		return _a + _b;
	};
	int foobar_const_noexcept(int _a, int _b) const noexcept
	{
		return _a + _b;
	};
};

void another_freefunction(Bar& _b)
{

};

int main()
{
	jc::functor<int(int, int)> _f{};

	{
		jc::functor<void(Bar&)> a{ &another_freefunction };
	};

	if (_f.good() || _f)
		return -1;

	_f = &foo;

	if (!_f.good() || !_f)
		return -1;

	if (_f.invoke(2, 2) != _f(2, 2))
		return -1;


	{

		Bar _b{};

		jc::functor<int(int, int)> _f{ &Bar::foobar, &_b };
		jc::functor<int(int, int)> _fn{ &Bar::foobar_noexcept, &_b };

		const Bar _constb{};
		jc::functor<int(int, int)> _fc{ &Bar::foobar_const, &_constb };
		jc::functor<int(int, int)> _fcn{ &Bar::foobar_const_noexcept, &_constb };

	}



	Bar _b{};
#ifdef __cpp_deduction_guides
	_f = std::pair{ &Bar::foobar, &_b };
#else
	_f = std::pair<int(Bar::*)(int,int), Bar*>{ &Bar::foobar, &_b };
#endif

	if (!_f.good() || !_f)
		return -1;

	if (_f.invoke(2, 2) != _f(2, 2))
		return -1;

	// test copy constructor

	const auto _fconstcopy = _f;
	jc::functor<int(int, int)> _fcopy{ _fconstcopy };

	if (!_fcopy.good())
		return -1;

	if (_fcopy.invoke(2, 2) != _f.invoke(2, 2))
		return -1;

	
	// test move constructor

	jc::functor<int(int, int)> _fmoved{ std::move(_fcopy) };

	if (_fcopy.good())
		return -1;

	if (!_fmoved.good())
		return -1;

	if (_fmoved.invoke(2, 2) != _f.invoke(2, 2))
		return -1;


	{
		// Test const member function
		struct ConstFoo
		{
			int id() const
			{
				return 2;
			};
		};
		ConstFoo _foo{};
	};






	return 0;
};
