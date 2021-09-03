#include <jclib/type_traits.h>

template <typename T>
struct is_int : public jc::is_same<T, int> {};


template <typename... Ts>
constexpr bool test_foo()
{
	return jc::disjunction<is_int<Ts>...>::value;
};

template <typename... Ts>
constexpr bool test_nfoo()
{
	return jc::conjunction<is_int<Ts>...>::value;
};








int main()
{
	constexpr auto b = test_foo<char, bool, int, char>();
	static_assert(b, "");

	constexpr auto n = test_nfoo<char, bool, int, char>();
	static_assert(!n, "");

	return 0;
};