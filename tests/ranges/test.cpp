#include "jclib/ranges.h"
#include "jclib/functional.h"

#include <vector>

int main()
{
	{
		constexpr size_t _len = 4;
		int _arr[_len]{ 1, 2, 3, 4 };
		
		static_assert(jc::is_same<jc::range_value_t<decltype(_arr)>, int>::value, "");
		static_assert(jc::is_same<jc::range_iterator_t<decltype(_arr)>, int*>::value, "");

		auto _arrBegin = &_arr[0];

		if (_arrBegin != jc::begin(_arr))
		{
			return 1;
		};
		if (_arrBegin + _len != jc::end(_arr))
		{
			return 1;
		};
	};

	{
		const std::vector<int> _ivec{ 1, 2, 3, 4 };
		using ree = jc::range_value_t<const std::vector<int>>;

		static_assert(jc::is_same<jc::range_value_t<decltype(_ivec)>, const int>::value, "");
		static_assert(jc::is_same<jc::range_iterator_t<decltype(_ivec)>, typename decltype(_ivec)::const_iterator>::value, "");

		if (_ivec.begin() != jc::begin(_ivec))
		{
			return 2;
		};
		if (_ivec.end() != jc::end(_ivec))
		{
			return 2;
		};
	};







	return 0;
};