#pragma once
#ifndef JCLIB_FUNCTIONAL_H
#define JCLIB_FUNCTIONAL_H

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

#include "jclib/config.h"
#include "jclib/type_traits.h"

#include <tuple>

#define _JCLIB_FUNCTIONAL_

namespace jc
{
	/**
	 * @brief Invokes a function or invocable object and returns its result
	 * @param _op Function or invocable object to invoke
	 * @param _args... Arguements to pass as the parameters
	*/
	template <typename OpT, typename... ArgTs>
	JCLIB_CONSTEXPR inline auto invoke(OpT&& _op, ArgTs&&... _args)
		-> invoke_result_t<remove_reference_t<OpT>, decltype(std::forward<ArgTs>(std::declval<ArgTs>()))...>
	{
		return _op(std::forward<ArgTs>(_args)...);
	};
	template <typename ReturnT, typename... ArgTs>
	JCLIB_CONSTEXPR inline auto invoke(ReturnT(*_fn)(ArgTs...), ArgTs&&... _args)
		-> ReturnT
	{
		return (*_fn)(std::forward<ArgTs>(_args)...);
	};
#ifdef __cpp_noexcept_function_type
	template <typename ReturnT, typename... ArgTs>
	JCLIB_CONSTEXPR inline auto invoke(ReturnT(*_fn)(ArgTs...) noexcept, ArgTs&&... _args) noexcept
		-> ReturnT
	{
		return (*_fn)(std::forward<ArgTs>(_args)...);
	};
#endif

	template <typename ReturnT, typename ClassT, typename... ArgTs>
	JCLIB_CONSTEXPR inline auto invoke(ReturnT(ClassT::*_op)(ArgTs...), ClassT* _class, ArgTs&&... _args)
		-> ReturnT
	{
		return (_class->*_op)(std::forward<ArgTs>(_args)...);
	};
	template <typename ReturnT, typename ClassT, typename... ArgTs>
	JCLIB_CONSTEXPR inline auto invoke(ReturnT(ClassT::* _op)(ArgTs...) const, const ClassT* _class, ArgTs&&... _args)
		-> ReturnT
	{
		return (_class->*_op)(std::forward<ArgTs>(_args)...);
	};
#ifdef __cpp_noexcept_function_type
	template <typename ReturnT, typename ClassT, typename... ArgTs>
	JCLIB_CONSTEXPR inline auto invoke(ReturnT(ClassT::* _op)(ArgTs...) noexcept, ClassT* _class, ArgTs&&... _args)
		noexcept-> ReturnT
	{
		return (_class->*_op)(std::forward<ArgTs>(_args)...);
	};
	template <typename ReturnT, typename ClassT, typename... ArgTs>
	JCLIB_CONSTEXPR inline auto invoke(ReturnT(ClassT::* _op)(ArgTs...) const noexcept, const ClassT* _class, ArgTs&&... _args)
		noexcept-> ReturnT
	{
		return (_class->*_op)(std::forward<ArgTs>(_args)...);
	};
#endif

	namespace impl
	{
		template <typename Op, template <typename... ArgTs> typename T, typename... ArgTs, size_t... Idxs>
		constexpr inline auto apply_impl(Op _op, const T<ArgTs...>& _args, std::index_sequence<Idxs...> _isq)
		{
			return jc::invoke(_op, std::get<Idxs>(_args)...);
		};
		template <typename Op, template <typename... ArgTs> typename T, typename... ArgTs, size_t... Idxs>
		constexpr inline auto apply_impl(Op _op, T<ArgTs...>&& _args, std::index_sequence<Idxs...> _isq)
		{
			return jc::invoke(_op, std::move(std::get<Idxs>(_args))...);
		};
	};

	template <typename Op, template <typename... ArgTs> typename T, typename... ArgTs>
	constexpr inline auto apply(Op _op, const T<ArgTs...>& _args) ->
		decltype(jc::invoke(std::declval<Op>(), std::declval<ArgTs>()...))
	{
		return impl::apply_impl(std::move(_op), _args, std::make_index_sequence<sizeof...(ArgTs)>());
	};
	template <typename Op, template <typename... ArgTs> typename T, typename... ArgTs>
	constexpr inline auto apply(Op _op, T<ArgTs...>&& _args) ->
		decltype(jc::invoke(std::declval<Op>(), std::declval<ArgTs>()...))
	{
		return impl::apply_impl(std::move(_op), std::move(_args), std::make_index_sequence<sizeof...(ArgTs)>());
	};






	/**
	 * @brief Tag type for implementing operators
	*/
	struct operator_tag {};

	template <typename T>
	struct is_operator : bool_constant<is_base_of<operator_tag, T>::value> {};
#ifdef JCLIB_FEATURE_INLINE_VARIABLES
	template <typename T>
	constexpr inline bool is_operator_v = is_operator<T>::value;
#endif

#ifdef JCLIB_FEATURE_CONCEPTS
	template <typename T>
	concept cx_operator = is_operator_v<T>;
#endif

	/**
	 * @brief Tag type for implementing unary operators
	*/
	struct unary_operator_tag : operator_tag {};

	template <typename T>
	struct is_unary_operator : bool_constant<is_base_of<unary_operator_tag, T>::value> {};
#ifdef JCLIB_FEATURE_INLINE_VARIABLES
	template <typename T>
	constexpr inline bool is_unary_operator_v = is_unary_operator<T>::value;
#endif

#ifdef JCLIB_FEATURE_CONCEPTS
	template <typename T>
	concept cx_unary_operator = is_unary_operator_v<T>;
#endif

	/**
	 * @brief Tag type for implementing binary operators
	*/
	struct binary_operator_tag : operator_tag {};

	template <typename T>
	struct is_binary_operator : bool_constant<is_base_of<binary_operator_tag, remove_cvref_t<T>>::value> {};
#ifdef JCLIB_FEATURE_INLINE_VARIABLES
	template <typename T>
	constexpr inline bool is_binary_operator_v = is_binary_operator<T>::value;
#endif

#ifdef JCLIB_FEATURE_CONCEPTS
	template <typename T>
	concept cx_binary_operator = is_binary_operator_v<T>;
#endif

	namespace impl
	{
		struct bind_first_t {};
		struct bind_second_t {};

		/**
		 * @brief This is an even more impl-ish impl namespace type, please do not use this as it is
		 * a placeholder until I implement a proper closure system
		*/
		template <typename WhichEndT, typename OpT, typename T>
		struct bound_op;

		template <typename OpT, typename T>
		struct bound_op<bind_first_t, OpT, T> : unary_operator_tag
		{
			template <typename U>
			constexpr auto operator()(U&& rhs) const ->
				jc::invoke_result_t<OpT, T, U>
			{
				return jc::invoke(this->op, this->value, rhs);
			};

			OpT op;
			T value;
		};

		template <typename OpT, typename T>
		struct bound_op<bind_second_t, OpT, T> : unary_operator_tag
		{
			template <typename U>
			constexpr auto operator()(U&& lhs) const ->
				jc::invoke_result_t<OpT, U, T>
			{
				return jc::invoke(this->op, lhs, this->value);
			};

			OpT op;
			T value;
		};
	};

	struct placeholder_t {};
	constexpr static placeholder_t placeholder{};
	
	template <typename OpT, typename T>
	constexpr inline auto bind(OpT&& _op, placeholder_t, T&& _value) -> enable_if_t<is_binary_operator<OpT>::value,
		impl::bound_op<impl::bind_second_t, remove_reference_t<OpT>, remove_reference_t<T>>>
	{
		using output_type = impl::bound_op<impl::bind_second_t, remove_reference_t<OpT>, remove_reference_t<T>>;
		return output_type{ std::forward<OpT>(_op), std::forward<T>(_value) };
	};

	template <typename OpT, typename T>
	constexpr inline auto bind(OpT&& _op, T&& _value, placeholder_t) -> enable_if_t<is_binary_operator<OpT>::value,
		impl::bound_op<impl::bind_first_t, remove_reference_t<OpT>, remove_reference_t<T>>>
	{
		using output_type = impl::bound_op<impl::bind_first_t, remove_reference_t<OpT>, remove_reference_t<T>>;
		return output_type{ std::forward<OpT>(_op), std::forward<T>(_value) };
	};


	struct less_t : binary_operator_tag
	{
		template <typename T, typename U>
		constexpr inline auto operator()(T&& lhs, U&& rhs) const noexcept ->
			decltype(std::declval<T&&>() < std::declval<U&&>())
		{
			return lhs < rhs;
		};
	};
	constexpr static less_t less{};

	struct greater_t : binary_operator_tag
	{
		template <typename T, typename U>
		constexpr inline auto operator()(T&& lhs, U&& rhs) const noexcept ->
			decltype(std::declval<T&&>() > std::declval<U&&>())
		{
			return lhs > rhs;
		};
	};
	constexpr static greater_t greater{};

	struct equals_t : binary_operator_tag
	{
		template <typename T, typename U>
		constexpr inline auto operator()(T&& lhs, U&& rhs) const noexcept ->
			decltype(std::declval<T&&>() == std::declval<U&&>())
		{
			return lhs == rhs;
		};
	};
	constexpr static equals_t equals{};

	struct unequals_t : binary_operator_tag
	{
		template <typename T, typename U>
		constexpr inline auto operator()(T&& lhs, U&& rhs) const noexcept ->
			decltype(std::declval<T&&>() != std::declval<U&&>())
		{
			return lhs != rhs;
		};
	};
	constexpr static unequals_t unequals{};

	struct plus_t : binary_operator_tag
	{
		template <typename T, typename U>
		constexpr inline auto operator()(T&& lhs, U&& rhs) const noexcept ->
			decltype(std::declval<T&&>() + std::declval<U&&>())
		{
			return lhs + rhs;
		};
	};
	constexpr static plus_t plus{};

	struct minus_t : binary_operator_tag
	{
		template <typename T, typename U>
		constexpr inline auto operator()(T&& lhs, U&& rhs) const noexcept ->
			decltype(std::declval<T&&>() - std::declval<U&&>())
		{
			return lhs - rhs;
		};
	};
	constexpr static minus_t minus{};

	struct times_t : binary_operator_tag
	{
		template <typename T, typename U>
		constexpr inline auto operator()(T&& lhs, U&& rhs) const noexcept ->
			decltype(std::declval<T&&>()* std::declval<U&&>())
		{
			return lhs * rhs;
		};
	};
	constexpr static times_t times{};

	struct divide_t : binary_operator_tag
	{
		template <typename T, typename U>
		constexpr inline auto operator()(T&& lhs, U&& rhs) const noexcept ->
			decltype(std::declval<T&&>() / std::declval<U&&>())
		{
			return lhs / rhs;
		};
	};
	constexpr static divide_t divide{};

	struct invert_t : unary_operator_tag
	{
		template <typename T>
		constexpr auto operator()(T&& _value) const noexcept
		{
			return !_value;
		};
	};
	constexpr static invert_t invert{};

	struct conjunct_t : binary_operator_tag
	{
		template <typename T, typename U>
		constexpr auto operator()(T&& lhs, U&& rhs) const noexcept
		{
			return lhs && rhs;
		};
	};
	constexpr static conjunct_t conjunct{};





	namespace impl
	{
		template <typename Op, typename T, typename = void>
		struct has_unary_operator : false_type {};

		template <typename Op, typename T>
		struct has_unary_operator<Op, T,
			void_t<decltype(std::declval<Op>()(std::declval<T&&>()))>
		>: true_type {};

		template <typename Op, typename T, typename U, typename = void>
		struct has_binary_operator : false_type {};

		template <typename Op, typename T, typename U>
		struct has_binary_operator<Op, T, U,
			void_t<decltype(std::declval<Op>()(std::declval<T&&>(), std::declval<U&&>()))>
		> : true_type {};
	};

	template <typename Op, typename T, typename U = T, typename = void>
	struct has_operator : false_type {};

	template <typename Op, typename T>
	struct has_operator<Op, T, T, enable_if_t<
		is_unary_operator<Op>::value && impl::has_unary_operator<Op, T>::value
		>> : true_type{};

	template <typename Op, typename T>
	struct has_operator<Op, T, T, enable_if_t<
		is_binary_operator<Op>::value && impl::has_binary_operator<Op, T, T>::value
		>> : true_type{};

	template <typename Op, typename T, typename U>
	struct has_operator<Op, T, U, enable_if_t<
		is_binary_operator<Op>::value&& impl::has_binary_operator<Op, T, U>::value
		>> : true_type{};

#ifdef JCLIB_FEATURE_INLINE_VARIABLES
	template <typename Op, typename T, typename U = T>
	constexpr inline bool has_operator_v = has_operator<Op, T, U>::value;
#endif


	/**
	 * @brief Namespace contains piping operations. Add a using namespace directive
	 to enable usage in a particular scope.
	*/
	namespace piping
	{
		template <typename T, typename OpT>
		constexpr auto inline operator|(T&& _value, OpT&& _op) ->
			jc::enable_if_t<is_unary_operator<remove_cvref_t<OpT>>::value,
			jc::invoke_result_t<OpT, remove_reference_t<T>>>
		{
			return jc::invoke(std::forward<OpT>(_op), std::forward<T>(_value));
		};

		namespace impl
		{
			enum : bool
			{
				bindFront,
				bindBack
			};

			template <bool Where>
			struct bound_pos;
			template <>
			struct bound_pos<bindFront>
			{
				using type = jc::impl::bind_first_t;
			};
			template <>
			struct bound_pos<bindBack>
			{
				using type = jc::impl::bind_second_t;
			};

			template <bool Where>
			using bound_pos_t = typename bound_pos<Where>::type;

			template <typename OpT, typename T, bool Where>
			using bound_op_t = typename jc::impl::bound_op<bound_pos_t<Where>, OpT, T>;
		};

		template <typename T, typename OpT>
		constexpr auto inline operator&(T&& _value, OpT&& _op) ->
			enable_if_t<is_binary_operator<OpT>::value,
				impl::bound_op_t<remove_cvref_t<OpT>, remove_cvref_t<T>, impl::bindFront>>
		{
			using output_type = impl::bound_op_t<remove_cvref_t<OpT>, remove_cvref_t<T>, impl::bindFront>;
			return output_type{ _op, std::forward<T>(_value) };
		};

		template <typename T, typename OpT>
		constexpr auto inline operator&(OpT&& _op, T&& _value) ->
			enable_if_t<is_binary_operator<remove_cvref_t<OpT>>::value,
			impl::bound_op_t<remove_cvref_t<OpT>, remove_cvref_t<T>, impl::bindBack>>
		{
			using output_type = typename impl::bound_op_t<remove_cvref_t<OpT>, remove_cvref_t<T>, impl::bindBack>;
			return output_type{ _op, std::forward<T>(_value) };
		};


	};

	


};

#endif