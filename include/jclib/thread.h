#pragma once
#ifndef JCLIB_THREAD_H
#define JCLIB_THREAD_H

/*
	Copyright 2021 Jonathan Cline
	Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files
	(the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge,
	publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do
	so, subject to the following conditions:
	The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
	WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
	COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
	OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
	Support header for thread utilies
	Also provides some convenience functions for standard library functionality
*/

#include "jclib/time.h"

#define _JCLIB_THREAD_

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



	/**
	 * @brief Tag type for providing non-locking function overloads
	*/
	struct nolock_t
	{
		// Explicit to prevent accidental construction
		constexpr explicit nolock_t() noexcept = default;
	};

	/**
	 * @brief Tag type value for invoking non-locking function overloads
	*/
	constexpr nolock_t nolock{};

};

#endif 