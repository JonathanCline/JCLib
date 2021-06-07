#pragma once
#ifndef JCLIB_TIME_H
#define JCLIB_TIME_H

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
	Time support header for jclib
*/

#include "jclib/config.h"
#include "jclib/type.h"

#define _JCLIB_TIME_

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

#ifdef __cpp_concepts
	/**
	 * @brief Check if a type fufills the standard clock type requirements
	 * see https://en.cppreference.com/w/cpp/named_req/Clock
	 * @tparam T Type to check
	*/
	template <typename T>
	struct is_clock : bool_constant<cx_clock<T>> {};
#else
	/**
	 * @brief Check if a type fufills the standard clock type requirements
	 * see https://en.cppreference.com/w/cpp/named_req/Clock
	 * @tparam T Type to check
	*/
	template <typename T, typename = void>
	struct is_clock : false_type {};

	template <typename T>
	struct is_clock <T, void_t<
		typename T::rep,
		typename T::period,
		typename T::duration,
		typename T::time_point,
		decltype(T::is_steady),
		decltype(T::now())
		>> : true_type {};
#endif

#ifdef __cpp_inline_variables
	/**
	 * @brief Check if a type fufills the standard clock type requirements
	 * see https://en.cppreference.com/w/cpp/named_req/Clock
	 * @tparam T Type to check
	*/
	template <typename T>
	JCLIB_CONSTEXPR static inline bool is_clock_v = is_clock<T>::value;
#endif
};

#endif