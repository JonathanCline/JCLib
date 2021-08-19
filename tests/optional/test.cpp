#include <jclib/optional.h>

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

#define SUBTEST_NEW(name, ...) int name ( __VA_ARGS__ ) \
{ \
	NEWTEST()

#define SUBTEST_PASS() PASS(); \
}




SUBTEST_NEW(subtest_has_value)
{
	jc::optional<int> opt{};

	if (opt.has_value())
	{
		FAIL("optional's default constructor should give it jc::nullopt, not the parameter type T");
	};
	if ((bool)opt != opt.has_value())
	{
		FAIL("optional's bool conversion does not match its has_value() function");
	};

	// Set value
	opt = 2;

	if (!opt.has_value())
	{
		FAIL("optional's value assignment did not give it a held value");
	};
	if ((bool)opt != opt.has_value())
	{
		FAIL("optional's bool conversion does not match its has_value() function");
	};
}
SUBTEST_PASS()



SUBTEST_NEW(subtest_reset)
{
	jc::optional<int> opt{ 2 };
	if (!opt.has_value())
	{
		FAIL("optional's reset function did not give it a held value");
	};
	if ((bool)opt != opt.has_value())
	{
		FAIL("optional's bool conversion does not match its has_value() function");
	};


	// Remove value
	opt.reset();

	if (opt.has_value())
	{
		FAIL("optional's reset function did not give it a held value");
	};
	if ((bool)opt != opt.has_value())
	{
		FAIL("optional's bool conversion does not match its has_value() function");
	};
};
SUBTEST_PASS()



SUBTEST_NEW(subtest_trivial_destructor_optimization)
{
#if JCLIB_VERSION_MAJOR >= 0 && JCLIB_VERSION_MINOR >= 3
	using trivial_type = jc::optional<int>;
	if (!std::is_trivially_destructible<trivial_type>::value)
	{
		FAIL("optional should be trivially destructible")
	};

	struct Foo
	{
		~Foo()
		{
			this->i = 1;
		};
		int i = 0;
	};

	using non_trivial_type = jc::optional<Foo>;
	if (std::is_trivially_destructible<non_trivial_type>::value)
	{
		FAIL("optional should NOT be trivially destructible")
	};

#endif
};
SUBTEST_PASS()


int main()
{
	NEWTEST();
	SUBTEST(subtest_has_value);
	SUBTEST(subtest_reset);
	SUBTEST(subtest_trivial_destructor_optimization);
	PASS();
};