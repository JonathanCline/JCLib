#include <jclib/type_traits.h>


template <typename... Ts>
constexpr bool test_foo()
{
	return jc::conjunction<jc::is_integer<Ts>...>::value;
};

int main()
{
	constexpr auto b = test_foo<int, bool, int>();
	static_assert(b, "");

	return 0;
};