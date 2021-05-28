#include "jclib/array_view.h"

#include <numeric>
#include <cassert>

int main()
{
	int _ca[] = { 0, 0, 0, 0, 0 };
	jc::array_view<int> _ax{ _ca };
	std::iota(_ax.begin(), _ax.end(), 1);

	assert(_ca[2] == 3);

	return 0;
};
