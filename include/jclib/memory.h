#pragma once
#ifndef JCLIB_MEMORY_H
#define JCLIB_MEMORY_H

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

#include "jclib/config.h"
#include "jclib/type_traits.h"
#include "jclib/concepts.h"

#define _JCLIB_MEMORY_

namespace jc
{
	/**
	 * @brief Calls the destructor of the element passed in
	 * @tparam T Object type
	 * @param _ref Object to call destructor on
	*/
	template <typename T>
#ifdef __cpp_lib_constexpr_dynamic_alloc
	JCLIB_CONSTEXPR inline void destroy_at(T& _ref)
#else
	inline void destroy_at(T& _ref)
#endif
	{
		_ref.~T();
	};

	/**
	 * @brief Calls the destructor of the element passed in
	 * @tparam T Object type
	 * @param _ref Object to call destructor on
	*/
	template <typename T>
#ifdef __cpp_lib_constexpr_dynamic_alloc
	JCLIB_CONSTEXPR inline void destroy_at(T* _ptr)
#else
	inline void destroy_at(T* _ptr)
#endif
	{
		destroy_at(*_ptr);
	};

};

#endif