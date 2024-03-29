#include <jclib/guard.h>





struct BoolLate
{
	static bool wasCleared;
	
	void reset()
	{
		this->wasCleared = true;
	};
};
bool BoolLate::wasCleared = false;

int main()
{
	{
		BoolLate::wasCleared = false;
		{
			jc::guard<BoolLate> _guard{};
			if (!_guard.good() || !_guard)
			{
				return 1;
			};
		};
		if (!BoolLate::wasCleared)
		{
			return 1;
		};
	};

	{
		BoolLate::wasCleared = false;
		jc::guard<BoolLate> _guard{};
		_guard.reset();
		if (!BoolLate::wasCleared)
		{
			return 2;
		};
	};

	{
		BoolLate::wasCleared = false;
		{
			jc::guard<BoolLate> _guard{};
			_guard.release();
		};
		if (BoolLate::wasCleared)
		{
			return 3;
		};
	};


	return 0;
};