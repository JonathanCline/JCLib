#pragma once
#ifndef JCLIB_TO_H
#define JCLIB_TO_H

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

#include <jclib/type_traits.h>
#include <jclib/concepts.h>

#include <string>

#define _JCLIB_TO_

namespace jc
{
	template <typename ToT, typename FromT, typename Enable = void>
	struct to_t;

	template <typename ToT, typename FromT>
	struct to_t<ToT, FromT, jc::enable_if_t<jc::is_convertible<ToT, FromT>::value>>
	{
		constexpr inline ToT operator()(FromT&& _from)
		{
			return _from;
		};
	};

	template <typename FromT>
	struct to_t<std::string, FromT, jc::void_t<decltype(std::to_string(std::declval<FromT&&>()))>>
	{
		inline auto operator()(FromT&& _from)
		{
			return std::to_string(std::forward<FromT>(_from));
		};
	};

	template <typename ToT, typename FromT>
	constexpr inline auto to(FromT&& _from) ->
		decltype(std::declval<to_t<ToT, FromT>>(std::declval<FromT&&>()))
	{
		return to_t<ToT, FromT>{}(std::forward<FromT>(_from));
	};

};

#endif