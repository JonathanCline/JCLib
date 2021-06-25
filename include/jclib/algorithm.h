#pragma once
#ifndef JCLIB_ALGORITHM_H
#define JCLIB_ALGORITHM_H

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

#include "jclib/ranges.h"

#include <algorithm>

#define _JCLIB_ALGORITHM_

namespace jc
{
	template <typename RangeT, typename T>
	JCLIB_CONSTEXPR static auto find(RangeT&& _range, const T& _val) ->
		ranges::iterator_t<remove_reference_t<RangeT>>
	{
		return std::find(jc::begin(_range), jc::end(_range), _val);
	};
	template <typename RangeT, typename Op>
	JCLIB_CONSTEXPR static auto find_if(RangeT&& _range, Op&& _pred) ->
		ranges::iterator_t<remove_reference_t<RangeT>>
	{
		return std::find_if(jc::begin(_range), jc::end(_range), std::forward<Op>(_pred));
	};
};

#endif