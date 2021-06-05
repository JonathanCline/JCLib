#include "jclib/algorithm.h"



int main()
{
	int _vals[] = { 0, 1, 2, 3, 4 };
	auto _it = jc::find(_vals, 2);
	if (_it != &_vals[0] + 2)
	{
		return 1;
	};
	
	return 0;
};
