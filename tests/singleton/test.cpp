#include <jclib/timer.h>

#include <jclib/guard.h>
#include <jclib/functional.h>

#include <iostream>
#include <functional>

using stdclock = std::chrono::steady_clock;

int main()
{
	// Check is_clock
	if (!jc::is_clock<stdclock>::value)
	{
		return 1;
	};
#ifdef __cpp_inline_variables
	// Check that the inline version matches the type trait
	if (jc::is_clock<stdclock>::value ^ jc::is_clock_v<stdclock>)
	{
		return 1;
	};
#endif
#ifdef __cpp_concepts
	// Check that the concept matches the type trait
	if (jc::is_clock<stdclock>::value ^ jc::cx_clock<stdclock>)
	{
		return 1;
	};
#endif

	return 0;
};