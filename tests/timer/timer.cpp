#include <jclib/timer.h>

using namespace std::chrono_literals;


int main()
{
	using clock = std::chrono::steady_clock;

	auto _duration = 100us;
	jc::timer _tm{ _duration };
	
	_tm.start();
	jc::sleep(_duration);
	if (!_tm.finished())
	{
		return 1;
	};

	return 0;
};
