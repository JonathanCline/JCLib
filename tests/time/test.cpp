#include <jclib/time.h>

#include <ctime>
#include <iostream>



#include <jclib/ranges.h>






template <typename... Ts>
std::string format()












int main()
{










	{
		using clock = std::chrono::steady_clock;
		if (!jc::is_clock<clock>::value)
		{
			return 1;
		};
	};
	
	{
		using clock = std::chrono::system_clock;
		if (!jc::is_clock<clock>::value)
		{
			return 1;
		};

		const auto _nowCTime = clock::to_time_t(clock::now());
		std::tm _nowTM = jc::impl::localtime(_nowCTime);
		std::cout << _nowTM.tm_hour << ':' << _nowTM.tm_min;
	};


	return 0;
};