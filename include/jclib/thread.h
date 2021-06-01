#pragma once
#ifndef JCLIB_THREAD_H
#define JCLIB_THREAD_H

#define _JCLIB_THREAD_

/*
	Support header for thread utilies
	Also provides some convenience functions for standard library functionality
*/

#include "jclib/time.h"

#include <thread>

namespace jc
{
	/**
	 * @brief Convenience function for std::this_thread::sleep_for()
	 * @param _dt Duration to sleep for
	*/
	template <typename Rep, typename Period>
	static void sleep(const std::chrono::duration<Rep, Period>& _dt)
	{
		std::this_thread::sleep_for(std::move(_dt));
	};

	/**
	 * @brief Convenience function for std::this_thread::sleep_for()
	 * @param _seconds duration in seconds
	*/
	template <typename T>
	static void sleep(T _seconds)
	{
		const T _nanoseconds = (_seconds / (T)std::nano::num) * (T)std::nano::den;
		return sleep(std::chrono::duration<T, std::nano>{ _nanoseconds });
	};

	/**
	 * @brief Convenience function for std::this_thread::sleep_until()
	 * @param _timepoint Time to sleep until
	*/
	template <typename ClockT, typename DurationT>
	static void sleep_until(const std::chrono::time_point<ClockT, DurationT>& _timepoint)
	{
		std::this_thread::sleep_until(_timepoint);
	};

};

#endif 