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

#define _JCLIB_ALGORITHM_

#include <algorithm>

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

	template <typename IterT, typename T, typename PredT>
	JCLIB_CONSTEXPR inline auto binary_search(IterT _begin, const IterT _trueEnd, const T& _val, PredT&& _pred) -> enable_if_t<
			is_invocable<PredT, iterator_to<IterT>, T>::value && is_forward_iterator<IterT>::value,
		IterT>
	{
		auto _end = _trueEnd;
		auto _middle = jc::midpoint(_begin, _end);
		for (_middle; jc::distance(_begin, _end) > 1;)
		{
			_middle = jc::midpoint(_begin, _end);
			if (!jc::invoke(_pred, *_middle, _val))
			{
				_end = _middle;
			}
			else
			{
				_begin = _middle;
			};
		};
		if (_middle != _trueEnd && *_middle != _val)
		{
			_middle = _trueEnd;
		};
		return _middle;
	};

	struct less_than_t
	{
		template <typename T, typename U>
		JCLIB_CONSTEXPR bool operator()(T&& a, U&& b) const noexcept
		{
			return a < b;
		};
	};

	template <typename IterT, typename T>
	JCLIB_CONSTEXPR inline auto binary_search(IterT _begin, IterT _end, const T& _val)
	{
		return jc::binary_search(_begin, _end, _val, less_than_t{});
	};

	namespace ranges
	{
		template <typename RangeT, typename T = ranges::value_t<remove_reference_t<RangeT>>>
		JCLIB_CONSTEXPR inline auto binary_search(RangeT&& _range, const T& _value)
		{
			return jc::binary_search(jc::begin(_range), jc::end(_range), _value);
		};
	};

};

#endif