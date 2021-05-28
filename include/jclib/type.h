#pragma once
#ifndef JCLIB_TYPE_H
#define JCLIB_TYPE_H

#define _JCLIB_TYPE_

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
	Contains various type meta programming tools
*/

#include "jclib/config.h"
#include "jclib/type_traits.h"

#include <tuple>

namespace jc
{

	template <int Index, typename... Ts>
	struct type_selector
	{
#ifdef __cpp_lib_tuple_element_t
		using type = typename std::tuple_element<Index, Ts...>::type;
#endif
	};

	template <int Index, typename... Ts>
	using type_selector_t = typename type_selector<Index, Ts...>::type;


	// if UseAlternate is false, type_switch::type = T, otherwise type_switch::type = AlternateT
	template <bool UseAlternate, typename T, typename AlternateT>
	struct type_switch;

	template <typename T, typename AlternateT>
	struct type_switch<true, T, AlternateT>
	{
		using type = AlternateT;
	};
	template <typename T, typename AlternateT>
	struct type_switch<false, T, AlternateT>
	{
		using type = T;
	};

	// if UseAlternate is false this is T, otherwise this is AlternateT
	template <bool UseAlternate, typename T, typename AlternateT>
	using type_switch_t = typename type_switch<UseAlternate, T, AlternateT>::type;
	
};

#endif