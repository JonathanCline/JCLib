#pragma once
#ifndef JCLIB_RANGES_H
#define JCLIB_RANGES_H

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

#include "jclib/type_traits.h"

#define _JCLIB_RANGES_

namespace jc
{
	namespace ranges
	{

		template <typename T, typename = void>
		struct range_ftor;

		template <typename T, size_t N>
		struct range_ftor<T[N], void>
		{
			constexpr auto begin(T(&_val)[N]) const noexcept
			{
				return &_val[0];
			};
			constexpr auto end(T(&_val)[N]) const noexcept
			{
				return &_val[N];
			};
		};

		template <typename T>
		struct range_ftor < T, void_t<
			decltype(std::declval<remove_reference_t<T>>().begin()),
			decltype(std::declval<remove_reference_t<T>>().end())
			>>
		{
			constexpr auto begin(remove_reference_t<T>& _val) const noexcept
			{
				return _val.begin();
			};
			constexpr auto end(remove_reference_t<T>& _val) const noexcept
			{
				return _val.end();
			};
		};

		template <typename T, typename = void>
		struct is_range : false_type {};

		template <typename T>
		struct is_range<T,
			void_t<decltype(std::declval<range_ftor<T>>().begin(std::declval<T&>()))>
		> : true_type {};

#ifdef __cpp_inline_variables
		template <typename T>
		constexpr inline bool is_range_v = is_range<T>::value;
#endif


		template <typename T, typename = void>
		struct iterator;
		template <typename T>
		struct iterator<T, enable_if_t<is_range<T>::value>>
		{
			using type = decltype(std::declval<range_ftor<T>>().begin(std::declval<T&>()));
		};

		template <typename T>
		using iterator_t = typename iterator<T>::type;

		template <typename T, typename = void>
		struct value;
		template <typename T>
		struct value <T, enable_if_t<is_range<T>::value>>
		{
			using type = remove_reference_t<decltype(*std::declval<iterator_t<T>>())>;
		};

		template <typename T>
		using value_t = typename value<T>::type;





		template <typename T>
		constexpr auto begin(T& _value) -> iterator_t<T>
		{
			return range_ftor<T>{}.begin(_value);
		};

		template <typename T>
		constexpr auto end(T& _value) -> iterator_t<T>
		{
			return range_ftor<T>{}.end(_value);
		};

	};

	using ranges::begin;
	using ranges::end;

};

#endif