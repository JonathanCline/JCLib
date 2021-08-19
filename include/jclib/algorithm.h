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

/*
	Provides range based std-algorithms using jclib ranges and provides a few other small algorithms
*/

#include <jclib/ranges.h>
#include <jclib/functional.h>
#include <jclib/feature.h>

#define _JCLIB_ALGORITHM_

#include <algorithm>

namespace jc
{
	/**
	 * @brief Returns an iterator the value specified if it exists
	 * @tparam RangeT Range object type to look in
	 * @tparam T Value type
	 * @param _range Range to look in
	 * @param _val Value to search for
	 * @return Iterator to the found element or jc::end(_range) if it wasn't found
	*/
	template <typename RangeT, typename T>
	JCLIB_CONSTEXPR static auto find(RangeT&& _range, const T& _val) ->
			ranges::iterator_t<remove_reference_t<RangeT>>
	{
		return std::find(jc::begin(_range), jc::end(_range), _val);
	};

	/**
	 * @brief Returns an iterator the value specified if a predicate returns true
	 * @tparam RangeT Range object type to look in
	 * @tparam Op Predicate function object type
	 * @param _range Range to look in
	 * @param _pred Comparison function, an element is "found" if the predicate returns true when passed it
	 * @return Iterator to the found element or jc::end(_range) if it wasn't found
	*/
	template <typename RangeT, typename Op>
	JCLIB_CONSTEXPR static auto find_if(RangeT&& _range, Op&& _pred) ->
		ranges::iterator_t<remove_reference_t<RangeT>>
	{
		return std::find_if(jc::begin(_range), jc::end(_range), std::forward<Op>(_pred));
	};



	template <typename RangeT, typename ValT = jc::ranges::value_t<RangeT>>
	constexpr inline RangeT& fill(RangeT& _range, const ValT& _value)
	{
		std::fill(jc::begin(_range), jc::end(_range), _value);
		return _range;
	};


	template <typename RangeT, typename DestIterT>
	JCLIB_REQUIRES((jc::cx_range<RangeT> && jc::is_assignable_v<jc::iterator_to_t<DestIterT>, const jc::ranges::value_t<RangeT>&>))
	constexpr inline auto copy(const RangeT& _source, DestIterT _destBegin) ->
#ifdef JCLIB_FEATURE_CONCEPTS
		DestIterT
#else
		jc::enable_if_t<
			jc::is_assignable<
				/* to   */ jc::iterator_to_t<DestIterT>, // *DestIterT
				/* from */ std::add_lvalue_reference_t<std::add_const_t<jc::ranges::value_t<RangeT>>> // const RangeT::value_type&
			>::value,
		DestIterT>
#endif
	{
		return std::copy(jc::begin(_source), jc::end(_source), _destBegin);
	};


};

#endif