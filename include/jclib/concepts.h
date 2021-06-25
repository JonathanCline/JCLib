#pragma once
#ifndef JCLIB_CONCEPTS_H
#define JCLIB_CONCEPTS_H

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
		This is a semi-standin concepts library that was created before MSVC (or any other c++ implementation)
	had a standard concepts library. Also contains various concepts that are frequently useful
*/

#include "jclib/config.h"

#if defined(__cpp_concepts) && defined(__cpp_lib_concepts)
#include "jclib/type_traits.h"

#define _JCLIB_CONCEPTS_

#include <concepts>

namespace jc
{
	/*
		Type comparison
	*/

	template< class T, class U >
	concept cx_same_as = is_same_v<T, U>;

	template <class From, class To>
	concept cx_convertible_to = is_convertible_v<From, To>;

	template <typename T, typename... Ts>
	concept cx_any_of = is_any_of_v<T, Ts...>;

	/*
		Character concept
	*/

	template <typename T>
	concept cx_character = is_character_v<T>;



	/*
		Basic type catagory concepts
	*/

	template <typename T>
	concept cx_fundamental = is_fundamental_v<T>;

	template <typename T>
	concept cx_integral = is_integral_v<T>;
	
	template <typename T>
	concept cx_arithmetic = is_arithmetic_v<T>;
	
	template <typename T>
	concept cx_floating_point = is_floating_point_v<T>;

	template <typename T>
	concept cx_signed = is_signed_v<T>;

	template <typename T>
	concept cx_unsigned = is_unsigned_v<T>;

	template <typename T>
	concept cx_integer = is_integer_v<T>;










	/*
		Constructor related concepts
	*/

	template <typename T, typename... Args>
	concept cx_constructible = is_constructible_v<T, Args...>;

	template <typename T>
	concept cx_copy_constructible = is_copy_constructible_v<T>;

	template <typename T>
	concept cx_copy_assignable = is_copy_assignable_v<T>;

	template <typename T>
	concept cx_move_constructible = is_move_constructible_v<T>;

	template <typename T>
	concept cx_move_assignable = is_move_assignable_v<T>;

	template <typename T>
	concept cx_default_constructible = is_default_constructible_v<T>;

	template <typename From, typename To>
	concept cx_forwardable_to = is_forwardable_to_v<From, To>;



	/*
		Trivial type concepts
	*/

	template <typename T>
	concept cx_trivially_default_constructible = is_trivially_default_constructible_v<T>;

	template <typename T>
	concept cx_trivially_copy_constructible = is_trivially_copy_constructible_v<T>;

	template <typename T>
	concept cx_trivially_copy_assignable = is_trivially_copy_assignable_v<T>;

	template <typename T>
	concept cx_trivially_copyable = is_trivially_copyable_v<T>;

	template <typename T>
	concept cx_trivially_move_constructible = is_trivially_move_constructible_v<T>;

	template <typename T>
	concept cx_trivially_move_assignable = is_trivially_move_assignable_v<T>;

	template <typename T>
	concept cx_trivially_moveable = is_trivially_moveable_v<T>;

	template <typename From, typename To>
	concept cx_trivially_assignable = is_trivially_assignable_v<From, To>;

	template <typename T>
	concept cx_trivially_constructible = is_trivially_constructible_v<T>;

	template <typename T>
	concept cx_trivially_destructible = is_trivially_default_constructible_v<T>;

	template <typename T>
	concept cx_trivial = is_trivial_v<T>;



	/*
		Function related concepts
	*/

	template <typename T>
	concept cx_function = is_function_v<T>;

	template <typename T>
	concept cx_function_pointer = is_function_pointer_v<T>;

	template <typename FunctionT, typename... ArgTs>
	concept cx_invocable = is_invocable_v<FunctionT, ArgTs...>;



	/*
		Member function pointer concepts
	*/

	template <typename T>
	concept cx_member_function_pointer = is_member_function_pointer_v<T>;


	/*
		Free function pointer concepts
	*/

	template <typename T>
	concept cx_free_function_pointer = is_free_function_pointer_v<T>;


	/*
		Iterator related concepts
	*/
	
	template <typename T, typename U>
	concept cx_iterator_to = requires (T _iter)
	{
		{ *_iter } -> std::convertible_to<U>;
	};

};

#endif

#endif
