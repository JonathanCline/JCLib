#pragma once
#ifndef JCLIB_ITERATOR_H
#define JCLIB_ITERATOR_H

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
#include "jclib/feature.h"
#include "jclib/concepts.h"

#define _JCLIB_ITERATOR_

#include <numeric>

namespace jc
{

	namespace impl
	{
		template <typename T, typename = void>
		struct difference_type;

		template <typename T>
		struct difference_type<T, void_t<typename T::difference_type>>
		{
			using type = typename T::difference_type;
		};

		template <typename T>
		struct difference_type<T*, enable_if_t<is_pointer<T>::value>>
		{
			using type = std::ptrdiff_t;
		};

		template <typename T>
		using difference_type_t = typename difference_type<T>::type;



		template <typename T, typename = void>
		struct next_ftor;

		template <typename T>
		struct next_ftor<T, enable_if_t<is_pointer<T>::value>>
		{
			constexpr T operator()(T _value, std::ptrdiff_t _count) const noexcept
			{
				return _value + _count;
			};
		};
		template <typename T>
		struct next_ftor<T, enable_if_t<!is_pointer<T>::value>>
		{
			constexpr T operator()(T _value, difference_type_t<T> _count) const noexcept
			{
				return std::next(_value, _count);
			};
		};
	};

	using impl::difference_type;
	using impl::difference_type_t;

	template <typename T>
	constexpr inline auto next(T _at, difference_type_t<remove_cvref_t<T>> _count = 1) noexcept
	{
		return impl::next_ftor<T>{}(_at, _count);
	};
	
	







#ifdef JCLIB_FEATURE_CONCEPTS

	template <typename T>
	concept cx_iterator = requires (T& a)
	{
		*a;
	}
	&& !cx_same_as<void, decltype(*std::declval<T>())>;

	template <typename T>
	struct is_iterator : bool_constant<cx_iterator<T>> {};

#else

	template <typename T, typename = void>
	struct is_iterator : false_type {};

	template <typename T>
	struct is_iterator<T, enable_if_t<!is_same<void, decltype(*std::declval<T&>())>::value>> : true_type {};

#endif

#ifdef JCLIB_FEATURE_INLINE_VARIABLES
	template <typename T>
	constexpr inline bool is_iterator_v = is_iterator<T>::value;
#endif

	
#ifdef JCLIB_FEATURE_CONCEPTS

	template <typename T>
	concept cx_forward_iterator = cx_iterator<T> && requires (T& a)
	{
		{ ++a } -> cx_same_as<T&>;
	};

	template <typename T>
	struct is_forward_iterator : bool_constant<cx_forward_iterator<T>> {};

#else

	template <typename T, typename = void>
	struct is_forward_iterator : false_type {};

	template <typename T>
	struct is_forward_iterator < T, enable_if_t<
			is_iterator<T>::value && is_same<T&, decltype(++std::declval<T&>())>::value
		>> : true_type {};

#endif

#ifdef JCLIB_FEATURE_INLINE_VARIABLES
	template <typename T>
	constexpr inline bool is_forward_iterator_v = is_forward_iterator<T>::value;
#endif





	namespace impl
	{
		template <typename T>
		struct distance_ftor
		{
			using result_type = decltype(std::distance(std::declval<T>(), std::declval<T>()));
			constexpr inline auto operator()(T _begin, T _end) const noexcept -> result_type
			{
				return std::distance(_begin, _end);
			};
		};

		template <typename T>
		struct distance_ftor<T*>
		{
			using result_type = std::ptrdiff_t;
			constexpr inline result_type operator()(T* _begin, T* _end) const noexcept
			{
				return _end - _begin;
			};
		};
	};

	/**
	 * @brief Calls std::distance unless T is a pointer
	 * @tparam T 
	 * @param _begin 
	 * @param _end 
	 * @return 
	*/
	template <typename T>
	constexpr inline auto distance(T _begin, T _end) noexcept ->
		typename impl::distance_ftor<T>::result_type
	{
		return impl::distance_ftor<T>{}(_begin, _end);
	};

};

#endif