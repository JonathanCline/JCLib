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

	Constexpr versions of standard library algorithms will also be made available for C++14/17 if the
	relevant CMake option is set.
*/

#include <jclib/ranges.h>
#include <jclib/functional.h>
#include <jclib/feature.h>

// Algorithm config file
#include <jclib/config/algorithm.h>

#include <algorithm>
#include <numeric>

#define _JCLIB_ALGORITHM_


/*
	 Determine if the standard library has constexpr algorithms and add a macro for ease of implementation custom backports
*/
#if JCLIB_FEATURE_CPP_CONSTEXPR_ALGORITHMS_V

	// Use standard library algorithms
	#define JCLIB_ALGORITHM_H_USE_STD_ALGORITHMS
	
	// jclib/algorithm.h specific constexpr macro
	#define JCLIB_ALGORITHM_H_CONSTEXPR constexpr

#elif JCLIB_CONSTEXPR_ALGORITHMS_V

	// Use custom algorithm backports
	#define JCLIB_ALGORITHM_H_USE_CUSTOM_ALGORITHMS
	
	// jclib/algorithm.h specific constexpr macro
	#define JCLIB_ALGORITHM_H_CONSTEXPR constexpr

#else

	// Use standard library algorithms
	#define JCLIB_ALGORITHM_H_USE_STD_ALGORITHMS
	
	// jclib/algorithm.h specific constexpr macro
	#define JCLIB_ALGORITHM_H_CONSTEXPR

#endif


namespace jc
{
	// Contains the type constraints used with algorithm functions regardless of implementation selected
	namespace impl_algorithms_constraints
	{
#pragma region ACCUMULATE_CONSTRAINTS
#if JCLIB_FEATURE_CONCEPTS_V
		template <typename IterT, typename OpT, typename T>
		concept cx_accumulate_constraints =
			jc::cx_iterator<IterT> &&
			jc::cx_invocable<OpT, T&&, jc::iterator_to_t<IterT>> &&
			jc::cx_same_as<T, jc::invoke_result_t<OpT, T&&, jc::iterator_to_t<IterT>>>;
	
		template <typename IterT, typename OpT, typename T>
		struct accumulate_constraints : 
			jc::bool_constant<cx_accumulate_constraints<IterT, OpT, T>>
		{};
#else
		template <typename IterT, typename OpT, typename T>
		struct accumulate_constraints : jc::bool_constant
			<
				jc::is_iterator<IterT>::value&&
				jc::is_invocable<OpT, T&&, jc::iterator_to_t<IterT>>::value&&
				jc::is_same<jc::invoke_result_t<OpT, T&&, jc::iterator_to_t<IterT>>, T>::value
			>
		{};
#endif
#pragma endregion ACCUMULATE_CONSTRAINTS
	};

	// Iterator based accumulate
	template <typename IterT, typename OpT = jc::plus_t, typename T = jc::remove_cvref_t<jc::iterator_to_t<IterT>>>
	JCLIB_REQUIRES((impl_algorithms_constraints::cx_accumulate_constraints<IterT, OpT, T>))
	JCLIB_ALGORITHM_H_CONSTEXPR inline auto accumulate(IterT _begin, const IterT _end, const OpT& _op = jc::plus, T _init = T{})
		noexcept(noexcept(
			jc::invoke(std::declval<const OpT>(), std::declval<T&&>(), std::declval<jc::iterator_to_t<IterT>>())
		))
		-> JCLIB_RET_SFINAE_CXSWITCH(T, impl_algorithms_constraints::accumulate_constraints<IterT, OpT, T>::value)
	{
#if defined(JCLIB_ALGORITHM_H_USE_STD_ALGORITHMS)
		return ::std::accumulate(_begin, _end, std::move(_init), _op);
#else
		// Exit early if 0 length range
		if (_begin == _end)
		{
			return _init;
		};

		// Preform accumulate
		for (auto _at = _begin; _at != _end; _at = jc::next(_at, 1))
		{
			_init = jc::invoke(_op, std::move(_init), *_at);
		};

		// Return result
		return _init;
#endif
	};

	// Range based accumulate
	template <typename RangeT, typename OpT = jc::plus_t, typename T = jc::remove_const_t<jc::ranges::value_t<RangeT>>>
	JCLIB_REQUIRES((jc::cx_range<RangeT>))
	JCLIB_ALGORITHM_H_CONSTEXPR inline auto accumulate(RangeT&& _range, const OpT& _op = jc::plus, T _init = T{})
		noexcept(noexcept(
			jc::accumulate
			(
				std::declval<jc::ranges::iterator_t<RangeT>>(),
				std::declval<jc::ranges::iterator_t<RangeT>>(),
				std::declval<const OpT&>(),
				std::declval<T&&>()
			)
		))
		-> JCLIB_RET_SFINAE_CXSWITCH
		(
			decltype(jc::accumulate
			(
				std::declval<jc::ranges::iterator_t<RangeT>>(),
				std::declval<jc::ranges::iterator_t<RangeT>>(),
				std::declval<const OpT&>(),
				std::declval<T&&>()
			)),
			jc::ranges::is_range<RangeT>::value
		)
	{
		return jc::accumulate(jc::begin(_range), jc::end(_range), _op, std::move(_init));
	};








	/**
	 * @brief Returns an iterator the value specified if it exists
	 * @tparam RangeT Range object type to look in
	 * @tparam T Value type
	 * @param _range Range to look in
	 * @param _val Value to search for
	 * @return Iterator to the found element or jc::end(_range) if it wasn't found
	*/
	template <typename RangeT, typename T>
	JCLIB_REQUIRES
	((
		jc::cx_range<RangeT> &&
		std::equality_comparable_with<jc::ranges::const_reference_t<RangeT>, const T&>
	))
	JCLIB_CONSTEXPR inline auto find(RangeT&& _range, const T& _val) ->
#if !JCLIB_FEATURE_CONCEPTS_V
		jc::enable_if_t
		<
			jc::ranges::is_range<RangeT>::value &&
			jc::has_operator<jc::equals_t, jc::ranges::const_reference_t<RangeT>, const T&>::value,
#endif
		ranges::iterator_t<remove_reference_t<RangeT>>
#if !JCLIB_FEATURE_CONCEPTS_V
		>
#endif
	{
		return std::find(jc::begin(_range), jc::end(_range), _val);
	};

	/**
	 * @brief Returns an iterator the value specified if a predicate returns true
	 * @tparam RangeT Range object type to look in
	 * @tparam OpT Predicate function object type
	 * @param _range Range to look in
	 * @param _pred Comparison function, an element is "found" if the predicate returns true when passed it
	 * @return Iterator to the found element or jc::end(_range) if it wasn't found
	*/
	template <typename RangeT, typename OpT>
	JCLIB_REQUIRES
	((
		jc::cx_range<RangeT> &&
		std::is_invocable_r_v<bool, OpT, jc::ranges::const_reference_t<RangeT>>
	))
	JCLIB_CONSTEXPR inline auto find_if(RangeT&& _range, OpT&& _pred) ->
#if !JCLIB_FEATURE_CONCEPTS_V
		jc::enable_if_t
		<
			jc::ranges::is_range<RangeT>::value &&
			jc::is_same<bool, jc::invoke_result_t<OpT, jc::ranges::const_reference_t<RangeT>>>::value,
#endif
			ranges::iterator_t<remove_reference_t<RangeT>>
#if !JCLIB_FEATURE_CONCEPTS_V
		>
#endif
	{
		return std::find_if(jc::begin(_range), jc::end(_range), std::forward<OpT>(_pred));
	};



	template <typename RangeT, typename ValT = jc::ranges::value_t<RangeT>>
	JCLIB_REQUIRES
	((
		jc::cx_range<RangeT> &&
		!std::is_const_v<RangeT> &&
		std::assignable_from<jc::ranges::reference_t<RangeT>, const ValT&>
	))
	constexpr inline auto fill(RangeT& _range, const ValT& _value) ->
#if !JCLIB_FEATURE_CONCEPTS_V
		jc::enable_if_t
		<
			!std::is_const<RangeT>::value &&
			jc::ranges::is_range<RangeT>::value &&
			jc::is_assignable<jc::ranges::reference_t<RangeT>, const ValT&>::value,
#endif
			RangeT&
#if !JCLIB_FEATURE_CONCEPTS_V
		>
#endif
	{
		std::fill(jc::begin(_range), jc::end(_range), _value);
		return _range;
	};


	template <typename RangeT, typename DestIterT>
	JCLIB_REQUIRES
	((
		jc::cx_range<RangeT> &&
		jc::is_assignable_v<jc::iterator_to_t<DestIterT>, jc::ranges::const_reference_t<RangeT>>
	))
	constexpr inline auto copy(const RangeT& _source, DestIterT _destBegin) ->
#if !JCLIB_FEATURE_CONCEPTS_V
		jc::enable_if_t<
			jc::is_assignable<
				/* to   */ jc::iterator_to_t<DestIterT>, // *DestIterT
				/* from */ std::add_lvalue_reference_t<std::add_const_t<jc::ranges::value_t<RangeT>>> // const RangeT::value_type&
			>::value &&
			jc::ranges::is_range<RangeT>::value,
#endif
		DestIterT
#if !JCLIB_FEATURE_CONCEPTS_V
		>
#endif
	{
		return std::copy(jc::begin(_source), jc::end(_source), _destBegin);
	};



	template <typename RangeT, typename ValueT>
	JCLIB_REQUIRES
	((
		jc::cx_range<RangeT> &&
		std::equality_comparable_with<jc::ranges::const_reference_t<RangeT>, const ValueT&>
	))
	JCLIB_CONSTEXPR inline auto contains(const RangeT& _source, const ValueT& _value) ->
#if !JCLIB_FEATURE_CONCEPTS_V	
		jc::enable_if_t
		<
			jc::ranges::is_range<RangeT>::value &&
			jc::has_operator<jc::equals_t, jc::ranges::const_reference_t<RangeT>, const ValueT&>::value,
#endif
			bool
#if !JCLIB_FEATURE_CONCEPTS_V
		>
#endif
	{
		return jc::find(_source, _value) != jc::end(_source);
	};



	template <typename RangeT, typename PredT>
	JCLIB_REQUIRES
	((
		jc::cx_range<RangeT> &&
		std::is_invocable_r_v<bool, PredT, jc::ranges::const_reference_t<RangeT>>
	))
	JCLIB_CONSTEXPR inline auto contains_if(const RangeT& _source, PredT&& _pred) ->
#if !JCLIB_FEATURE_CONCEPTS_V
		jc::enable_if_t
		<
			jc::ranges::is_range<RangeT>::value &&
			jc::is_same<jc::invoke_result_t<PredT, jc::ranges::const_reference_t<RangeT>>, bool>::value,
#endif
			bool
#if !JCLIB_FEATURE_CONCEPTS_V
		>
#endif
	{
		return jc::find_if(_source, std::forward<PredT>(_pred)) != jc::end(_source);
	};


};

#endif