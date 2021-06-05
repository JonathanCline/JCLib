#include "jclib/ranges.h"

#include <vector>

int main()
{
	int _arr[4]{ 1, 2, 3, 4 };
	if (&_arr[0] != jc::begin(_arr))
	{
		return 1;
	};

	const std::vector<int> _ivec{ 1, 2, 3, 4 };
	using ree = jc::range_value_t<const std::vector<int>>;

	if (_ivec.begin() != jc::begin(_ivec))
	{
		return 2;
	};

	return 0;
};