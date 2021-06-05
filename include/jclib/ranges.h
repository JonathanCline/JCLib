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
	/**
	 * @brief jc::begin() function customization point
	*/
	template <typename T, typename = void>
	struct begin_ftor;

	template <typename T, size_t N>
	struct begin_ftor<T[N]>
	{
		using type = T[N];
		JCLIB_CONSTEXPR static auto begin(type& _t) noexcept
		{
			return &_t[0];
		};
	};
	template <typename T>
	struct begin_ftor<T, void_t<decltype(std::declval<T&>().begin())>>
	{
		using type = T;
		JCLIB_CONSTEXPR static auto begin(type& _t) noexcept
		{
			return _t.begin();
		};
	};

	/**
	 * @brief Returns iterator or equivalent to the beginning of the range
	*/
	template <typename T>
	JCLIB_CONSTEXPR auto begin(T& _range) noexcept -> decltype(begin_ftor<T>::begin(std::declval<T&>()))
	{
		return begin_ftor<T>::begin(_range);
	};

	/**
	 * @brief Returns const_iterator or equivalent to the beginning of the range
	*/
	template <typename T>
	JCLIB_CONSTEXPR auto cbegin(const T& _range) noexcept -> decltype(begin_ftor<T>::begin(std::declval<const T&>()))
	{
		return begin_ftor<T>::begin(_range);
	};


	/**
	 * @brief jc::end() function customization point
	*/
	template <typename T, typename = void>
	struct end_ftor;

	template <typename T, size_t N>
	struct end_ftor<T[N]>
	{
		using type = T[N];
		JCLIB_CONSTEXPR static auto end(type& _t) noexcept
		{
			return &_t[N];
		};
	};
	template <typename T>
	struct end_ftor<T, void_t<decltype(std::declval<T&>().end())>>
	{
		using type = T;
		JCLIB_CONSTEXPR static auto end(type& _t) noexcept
		{
			return _t.end();
		};
	};

	/**
	 * @brief Returns iterator or equivalent to the end of the range
	*/
	template <typename T>
	JCLIB_CONSTEXPR auto end(T& _range) noexcept -> decltype(end_ftor<T>::end(std::declval<T&>()))
	{
		return end_ftor<T>::end(_range);
	};

	/**
	 * @brief Returns const_iterator or equivalent to the end of the range
	*/
	template <typename T>
	JCLIB_CONSTEXPR auto end(const T& _range) noexcept -> decltype(end_ftor<T>::end(std::declval<const T&>()))
	{
		return end_ftor<T>::end(_range);
	};

#ifdef __cpp_concepts
	template <typename T>
	concept cx_range = requires(T& _range)
	{
		begin(_range);
		end(_range);
	};

	template <typename T>
	struct is_range : bool_constant<cx_range<T>> {};
#else
	template <typename T, typename = void>
	struct is_range : false_type {};

	template <typename T>
	struct is_range<T, void_t<
		decltype(begin(std::declval<T&>())),
		decltype(end(std::declval<T&>()))
		>> : true_type{};
#endif

#ifdef __cpp_inline_variables
	template <typename T>
	JCLIB_CONSTEXPR inline bool is_range_v = is_range<T>::value;
#endif

	template <typename T> JCLIB_REQUIRES(cx_range<T>)
	using range_iterator_t = decltype(begin_ftor<T>::begin(std::declval<T&>()));

	template <typename T> JCLIB_REQUIRES(cx_range<T>)
	using range_value_t = std::remove_reference_t<decltype(*std::declval<range_iterator_t<T>>())>;





};

#endif