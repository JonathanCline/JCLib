#pragma once
#ifndef JCLIB_TIME_H
#define JCLIB_TIME_H

#define _JCLIB_TIME_

/*
	Time support header for jclib
*/

#include "jclib/config.h"

#include <chrono>

namespace jc
{
#ifdef __cpp_concepts
	/**
	 * @brief Concept for enforcing the standard library clock type named requirements
	 * see https://en.cppreference.com/w/cpp/named_req/Clock.
	*/
	template <typename T>
	concept cx_clock = requires
	{
		typename T::rep;
		typename T::period;
		typename T::duration;
		typename T::time_point;
		T::is_steady;
		T::now();
	};
#endif

};

#endif