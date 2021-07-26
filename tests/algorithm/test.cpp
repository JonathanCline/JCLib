#include "jclib/algorithm.h"

#include <array>
#include <vector>

#include <iostream>

#define BAD_TEST(num, msg) { std::cout << msg << '\n'; return num; }

int main()
{
	{
		int _vals[] = { 0, 1, 2, 3, 4 };
		auto _it = jc::find(_vals, 2);
		if (_it != &_vals[0] + 2)
		{
			return 1;
		};
	};

	{
		std::array<int, 8> _arrayVals{ 0, 1, 2, 3, 4, 5, 6, 7 };
		auto _it = jc::find(_arrayVals, 2);
		if (_it != _arrayVals.begin() + 2)
		{
			return 2;
		};
	};

	{
		std::vector<int> _arrayVals{ 0, 1, 2, 3, 4, 5, 6, 7 };
		auto _it = jc::find(_arrayVals, 2);
		if (_it != _arrayVals.begin() + 2)
		{
			return 3;
		};
	};

	{
		std::vector<int> _ints{ 1, 2, 3, 4 };
		{
			constexpr int _findval = 2;
			auto _expected = jc::find(_ints, _findval);
			auto _bsearchIters = jc::binary_search(_ints.begin(), _ints.end(), _findval);
			auto _bsearchRange = jc::ranges::binary_search(_ints, _findval);
			if (_bsearchIters != _bsearchRange)
			{
				BAD_TEST(4, "binary search returned different results for ranged and iterator pair versions");
			};
			if (_bsearchIters != _expected)
			{
				BAD_TEST(4, "binary search returned unexpected result");
			};
			auto _tooLowIter = jc::ranges::binary_search(_ints, 0);
			auto _tooHighIter = jc::ranges::binary_search(_ints, 5);
			if (_tooLowIter != _tooHighIter)
			{
				std::terminate();
			};
		};
	};

	return 0;
};
