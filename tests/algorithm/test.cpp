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

	return 0;
};
