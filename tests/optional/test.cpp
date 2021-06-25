#include <jclib/optional.h>


int main()
{
	{
		jc::optional<int> _opt{};
		if (_opt.has_value())
		{
			// should be nullopt by default
			return 1;
		};
	};




	return 0;
};