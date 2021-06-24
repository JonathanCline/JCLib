#include "jclib/ranges.h"
#include "jclib/functional.h"

#include <vector>
#include <numeric>

#include <functional>






constexpr bool is_even(int i)
{
	return (i % 2) == 0;
};

int main()
{
	{
		constexpr size_t _len = 4;
		int _arr[_len]{ 1, 2, 3, 4 };

		static_assert(jc::ranges::is_range<decltype(_arr)>::value, "");
		static_assert(jc::is_same<jc::ranges::value_t<decltype(_arr)>, int>::value, "");
		static_assert(jc::is_same<jc::ranges::iterator_t<decltype(_arr)>, int*>::value, "");

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
		std::vector<int> _ivec{ 1, 2, 3, 4 };
		static_assert(jc::ranges::is_range<decltype(_ivec)>::value, "");
		static_assert(jc::is_same<jc::ranges::value_t<decltype(_ivec)>, int>::value, "");
		static_assert(jc::is_same<jc::ranges::iterator_t<decltype(_ivec)>, typename decltype(_ivec)::iterator>::value, "");

		if (_ivec.begin() != jc::begin(_ivec))
		{
			return 2;
		};
		if (_ivec.end() != jc::end(_ivec))
		{
			return 2;
		};
	
	};


	{
		const std::vector<int> _ivec{ 1, 2, 3, 4 };
		using ree = jc::ranges::value_t<const std::vector<int>>;

		static_assert(jc::is_same<jc::ranges::value_t<decltype(_ivec)>, const int>::value, "");
		static_assert(jc::is_same<jc::ranges::iterator_t<decltype(_ivec)>, typename decltype(_ivec)::const_iterator>::value, "");

		if (_ivec.begin() != jc::begin(_ivec))
		{
			return 2;
		};
		if (_ivec.end() != jc::end(_ivec))
		{
			return 2;
		};
	};
	
	{
		std::vector<int> _ivec{};
		_ivec.resize(10);
		std::iota(_ivec.begin(), _ivec.end(), 1);

		for (auto v : _ivec | jc::views::filter(&is_even))
		{
			if (!is_even(v))
			{
				return 3;
			};
		};
	};

	{
		constexpr int _completeCount = 10;
		constexpr int _evenCount = _completeCount / 2;
		using iotav = decltype(jc::views::iota(1, _completeCount));

		if (!jc::ranges::is_view<iotav>::value || !jc::ranges::is_range<iotav>::value)
		{
			return 4;
		};

		int _count = 0;
		for (auto v : jc::views::iota(0, _completeCount) | jc::views::filter(&is_even))
		{
			++_count;
		};

		if (_count != _evenCount)
		{
			return 4;
		};
	};



	return 0;
};