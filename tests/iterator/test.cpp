#include <jclib/iterator.h>

#include <iostream>
#include <vector>
#include <list>

#ifdef JCLIB_FEATURE_INLINE_VARIABLES
#define JCLIB_TEST_TYPETRAIT(trait, type) (trait<type>::value && trait ## _v<type>)
#else
#define JCLIB_TEST_TYPETRAIT(trait, type) (trait<type>::value)
#endif

#define FAIL_TEST(result) { std::cout << "Failed at line " << __LINE__ << '\n'; return result; }

int main()
{

	{
		using container = std::vector<int>;
		using iterator = typename container::iterator;
		using const_iterator = typename container::const_iterator;

		if (!JCLIB_TEST_TYPETRAIT(jc::is_iterator, iterator) ||
			!JCLIB_TEST_TYPETRAIT(jc::is_iterator, const_iterator))
		{
			FAIL_TEST(1);
		};
		if (!JCLIB_TEST_TYPETRAIT(jc::is_forward_iterator, iterator) ||
			!JCLIB_TEST_TYPETRAIT(jc::is_forward_iterator, const_iterator))
		{
			FAIL_TEST(1);
		};
	};

	{
		using iterator = int*;
		using const_iterator = const int*;
		using inc = decltype(++std::declval<const_iterator&>());

		if (!JCLIB_TEST_TYPETRAIT(jc::is_iterator, iterator) ||
			!JCLIB_TEST_TYPETRAIT(jc::is_iterator, const_iterator))
		{
			FAIL_TEST(2);
		};
		if (!JCLIB_TEST_TYPETRAIT(jc::is_forward_iterator, iterator) ||
			!JCLIB_TEST_TYPETRAIT(jc::is_forward_iterator, const_iterator))
		{
			FAIL_TEST(2);
		};
	};

	{
		using iterator = int;
		if (JCLIB_TEST_TYPETRAIT(jc::is_iterator, iterator))
		{
			FAIL_TEST(3);
		};
		if (JCLIB_TEST_TYPETRAIT(jc::is_forward_iterator, iterator))
		{
			FAIL_TEST(3);
		};
	};


	{
		constexpr size_t _containerLen = 15;

		std::vector<int> _vec{};
		_vec.resize(_containerLen);
		std::fill_n(_vec.begin(), _containerLen, 0);

		std::list<int> _list{};
		_list.resize(_containerLen);
		std::fill_n(_list.begin(), _containerLen, 0);

		int _array[_containerLen]{};
		std::fill_n(&_array[0], _containerLen, 0);

		auto _vecDist = jc::distance(_vec.begin(), _vec.end());
		auto _listDist = jc::distance(_list.begin(), _list.end());
		auto _arrayDist = jc::distance(&_array[0], &_array[_containerLen]);

		if (!(_vecDist == _listDist && _listDist == _arrayDist))
		{
			FAIL_TEST(4);
		};
	};


	return 0;
};
