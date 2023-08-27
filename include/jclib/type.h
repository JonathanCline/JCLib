#pragma once
#ifndef JCLIB_TYPE_H
#define JCLIB_TYPE_H

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

#define _JCLIB_TYPE_

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
	
	
	
	/**
	 * @brief Empty type containing a using alias for the type being "rafted"
	 * This can be used to simplify some meta-programming code.
	 * @tparam T Type to be rafted
	*/
	template <typename T>
	struct raft
	{
		/**
		 * @brief Type held by the raft
		*/
		using type = T;
	};

	/**
	 * @brief Gets the type held within a type raft
	 * @tparam T Raft type
	*/
	template <typename T>
	struct raft_type;

	/**
	 * @brief Gets the type held within a type raft
	 * @tparam T Type held by the raft
	*/
	template <typename T>
	struct raft_type<raft<T>>
	{
		/**
		 * @brief Type held by the raft
		*/
		using type = typename raft<T>::type;
	};

	/**
	 * @brief Inline version of raft_type, gets the type held within a type raft
	 * @tparam T Type raft type
	*/
	template <typename T>
	using raft_type_t = typename raft_type<T>::type;





	template <typename T>
	struct null_ftor;

	/**
	 * @brief Define pointers as nullable types
	 * @tparam T Type being pointed to.
	*/
	template <typename T>
	struct null_ftor<T*>
	{
		using type = T*;

		constexpr static type make_null() noexcept
		{
			return nullptr;
		};
		constexpr static bool is_null(const type& _value) noexcept
		{
			return _value == nullptr;
		};
	};






	template <typename T, typename Enable = void>
	struct is_null_comparable : jc::false_type
	{};

	template <typename T>
	struct is_null_comparable <T, 
		jc::enable_if_t<jc::is_same<decltype(null_ftor<T>::is_null(std::declval<const T&>())), bool>::value>
	> : jc::true_type
	{};

#if	JCLIB_FEATURE_INLINE_VARIABLES_V
	template <typename T>
	constexpr inline bool is_null_comparable_v = is_null_comparable<T>::value;
#endif

#if	JCLIB_FEATURE_CONCEPTS_V
	template <typename T>
	concept cx_null_comparable = is_null_comparable_v<T>;
#endif



	template <typename T, typename Enable = void>
	struct is_nullable : jc::false_type
	{};

	template <typename T>
	struct is_nullable<T,
		jc::enable_if_t
		<
			jc::is_constructible<T, decltype(null_ftor<T>::make_null())>::value &&
			is_null_comparable<T>::value
		>
	> : jc::true_type
	{};

#if	JCLIB_FEATURE_INLINE_VARIABLES_V
	template <typename T>
	constexpr inline bool is_nullable_v = is_nullable<T>::value;
#endif

#if	JCLIB_FEATURE_CONCEPTS_V
	template <typename T>
	concept cx_nullable = is_nullable_v<T>;
#endif

	struct null_t
	{
		template <typename T>
		constexpr friend inline auto operator==(const T& lhs, null_t) noexcept ->
			decltype(null_ftor<T>::is_null(lhs))
		{
			return null_ftor<T>::is_null(lhs);
		};
		template <typename T>
		constexpr friend inline auto operator==(null_t, const T& rhs) noexcept ->
			decltype(null_ftor<T>::is_null(rhs))
		{
			return null_ftor<T>::is_null(rhs);
		};

		template <typename T>
		constexpr friend inline auto operator!=(const T& lhs, null_t) noexcept ->
			decltype(!null_ftor<T>::is_null(lhs))
		{
			return !null_ftor<T>::is_null(lhs);
		};
		template <typename T>
		constexpr friend inline auto operator!=(null_t, const T& rhs) noexcept ->
			decltype(!null_ftor<T>::is_null(rhs))
		{
			return !null_ftor<T>::is_null(rhs);
		};
		
		template <typename T, typename Enable = jc::enable_if_t<is_nullable<T>::value>>
		constexpr operator T() const
		{
			return null_ftor<T>::make_null();
		};

		constexpr explicit null_t() noexcept = default;
	};

	constexpr static null_t null{};

};

namespace jc
{
	template <>
	struct null_ftor<std::nullptr_t>
	{
		constexpr static auto make_null() noexcept
		{
			return nullptr;
		};
		constexpr static bool is_null(std::nullptr_t) noexcept
		{
			return true;
		};
	};
};


namespace jc
{
	/**
	 * @brief Type that has its copy constructor deleted.
	 * 
	 * Inherit from this to easily delete the copy constructor.
	*/
	struct no_copy_construct
	{
		constexpr no_copy_construct() noexcept = default;
		constexpr no_copy_construct(const no_copy_construct&) noexcept = delete;
		constexpr no_copy_construct& operator=(const no_copy_construct&) noexcept = default;
		constexpr no_copy_construct(no_copy_construct&&) noexcept = default;
		constexpr no_copy_construct& operator=(no_copy_construct&&) noexcept = default;
	};

	/**
	 * @brief Type that has its copy assignment operator deleted.
	 *
	 * Inherit from this to easily delete the copy assignment operator.
	*/
	struct no_copy_assign
	{
		constexpr no_copy_assign() noexcept = default;
		constexpr no_copy_assign(const no_copy_assign&) noexcept = default;
		constexpr no_copy_assign& operator=(const no_copy_assign&) noexcept = delete;
		constexpr no_copy_assign(no_copy_assign&&) noexcept = default;
		constexpr no_copy_assign& operator=(no_copy_assign&&) noexcept = default;
	};

	/**
	 * @brief Type that has its copy constructor and copy assignment operator deleted.
	 *
	 * Inherit from this to easily make a type non-copyable.
	*/
	struct no_copy : no_copy_construct, no_copy_assign {};

	/**
	 * @brief Type that has its move constructor deleted.
	 *
	 * Inherit from this to easily delete the move constructor.
	*/
	struct no_move_construct
	{
		constexpr no_move_construct() noexcept = default;
		constexpr no_move_construct(const no_move_construct&) noexcept = default;
		constexpr no_move_construct& operator=(const no_move_construct&) noexcept = default;
		constexpr no_move_construct(no_move_construct&&) noexcept = delete;
		constexpr no_move_construct& operator=(no_move_construct&&) noexcept = default;
	};
}

#endif