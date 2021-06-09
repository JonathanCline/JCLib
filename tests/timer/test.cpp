#include <jclib/timer.h>

#include <chrono>
#include <cassert>


template <typename T = typename jc::timer::clock_type>
int test_timer()
{
	using timer_clock = std::chrono::steady_clock;
	using timer_t = jc::basic_timer<timer_clock>;

	const auto _duration = std::chrono::microseconds{ 10 };
	jc::timer _t{ _duration };
	if (_t.get_duration() != _duration)
	{
		return 1;
	};

	_t.start();
	const auto _finishTime = _t.finished_at();

	jc::sleep_until(_t);
	if (!_t.finished())
	{
		return 2;
	};
	if (_finishTime > jc::timer::clock_type::now())
	{
		return 3;
	};
	if (_t.elapsed() < _duration)
	{
		return 4;
	};

	return 0;
};



int main()
{
	auto _result = 0;
	_result = test_timer<std::chrono::steady_clock>();
	if (_result != 0)
	{
		return _result;
	};

	_result = test_timer<>();
	if (_result != 0)
	{
		return _result;
	};

	return 0;
};
