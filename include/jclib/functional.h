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
#include <functional>

#if JCLIB_FEATURE_STRING_VIEW_V
	#include <string_view>
#else
	#include <string>
#endif

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

	/**
	 * @brief Invokes a function or invocable object and returns its result
	 * @param _op Function or invocable object to invoke
	 * @param _args... Arguements to pass as the parameters
	*/
	template <typename ReturnT, typename... ArgTs>
	JCLIB_CONSTEXPR inline auto invoke(ReturnT(*_fn)(ArgTs...), ArgTs&&... _args)
		-> ReturnT
	{
		return (*_fn)(std::forward<ArgTs>(_args)...);
	};
#ifdef __cpp_noexcept_function_type

	/**
	 * @brief Invokes a function or invocable object and returns its result
	 * @param _op Function or invocable object to invoke
	 * @param _args... Arguements to pass as the parameters
	*/
	template <typename ReturnT, typename... ArgTs>
	JCLIB_CONSTEXPR inline auto invoke(ReturnT(*_fn)(ArgTs...) noexcept, ArgTs&&... _args) noexcept
		-> ReturnT
	{
		return (*_fn)(std::forward<ArgTs>(_args)...);
	};
#endif

	/**
	 * @brief Invokes a function or invocable object and returns its result
	 * @param _op Function or invocable object to invoke
	 * @param _args... Arguements to pass as the parameters
	*/
	template <typename ReturnT, typename ClassT, typename... ArgTs>
	JCLIB_CONSTEXPR inline auto invoke(ReturnT(ClassT::* _op)(ArgTs...), ClassT* _class, ArgTs&&... _args)
		-> ReturnT
	{
		return (_class->*_op)(std::forward<ArgTs>(_args)...);
	};

	/**
	 * @brief Invokes a function or invocable object and returns its result
	 * @param _op Function or invocable object to invoke
	 * @param _args... Arguements to pass as the parameters
	*/
	template <typename ReturnT, typename ClassT, typename... ArgTs>
	JCLIB_CONSTEXPR inline auto invoke(ReturnT(ClassT::* _op)(ArgTs...) const, const ClassT* _class, ArgTs&&... _args)
		-> ReturnT
	{
		return (_class->*_op)(std::forward<ArgTs>(_args)...);
	};
#ifdef __cpp_noexcept_function_type
	/**
	 * @brief Invokes a function or invocable object and returns its result
	 * @param _op Function or invocable object to invoke
	 * @param _args... Arguements to pass as the parameters
	*/
	template <typename ReturnT, typename ClassT, typename... ArgTs>
	JCLIB_CONSTEXPR inline auto invoke(ReturnT(ClassT::* _op)(ArgTs...) noexcept, ClassT* _class, ArgTs&&... _args)
		noexcept-> ReturnT
	{
		return (_class->*_op)(std::forward<ArgTs>(_args)...);
	};

	/**
	 * @brief Invokes a function or invocable object and returns its result
	 * @param _op Function or invocable object to invoke
	 * @param _args... Arguements to pass as the parameters
	*/
	template <typename ReturnT, typename ClassT, typename... ArgTs>
	JCLIB_CONSTEXPR inline auto invoke(ReturnT(ClassT::* _op)(ArgTs...) const noexcept, const ClassT* _class, ArgTs&&... _args)
		noexcept-> ReturnT
	{
		return (_class->*_op)(std::forward<ArgTs>(_args)...);
	};
#endif

	namespace impl
	{
		template <typename Op, template <typename... ArgTs> class T, typename... ArgTs, size_t... Idxs>
		constexpr inline auto apply_impl(Op _op, const T<ArgTs...>& _args, std::index_sequence<Idxs...> _isq)
		{
			return jc::invoke(_op, std::get<Idxs>(_args)...);
		};
		template <typename Op, template <typename... ArgTs> class T, typename... ArgTs, size_t... Idxs>
		constexpr inline auto apply_impl(Op _op, T<ArgTs...>&& _args, std::index_sequence<Idxs...> _isq)
		{
			return jc::invoke(_op, std::move(std::get<Idxs>(_args))...);
		};
	};


	template <typename Op, template <typename... ArgTs> class T, typename... ArgTs>
	constexpr inline auto apply(Op _op, const T<ArgTs...>& _args) ->
		decltype(jc::invoke(std::declval<Op>(), std::declval<ArgTs>()...))
	{
		return impl::apply_impl(std::move(_op), _args, std::make_index_sequence<sizeof...(ArgTs)>());
	};


	template <typename Op, template <typename... ArgTs> class T, typename... ArgTs>
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

	// Deprecated as of v0.3.0
	JCLIB_DEPRECATE_BLOCK
	((
		/**
		 * @brief Tag type for implementing unary operators, deprecated
		*/
		struct JCLIB_DEPRECATED("no longer needed for denoting unary operator types, just use operator_tag instead")
		unary_operator_tag : operator_tag {};
	));

	/**
	 * @brief Tests if an operator type is invocable with 1 arguement
	 * @tparam T Type to test
	*/
	template <typename T>
	struct is_unary_operator : bool_constant
		<
			is_base_of<operator_tag, remove_cvref_t<T>>::value &&
			is_invocable_with_count<T, 1>::value
		>
	{};

#ifdef JCLIB_FEATURE_INLINE_VARIABLES
	/**
	 * @brief Tests if an operator type is invocable with 1 arguement
	 * @tparam T Type to test
	*/
	template <typename T>
	constexpr inline bool is_unary_operator_v = is_unary_operator<T>::value;
#endif

#ifdef JCLIB_FEATURE_CONCEPTS
	template <typename T>
	concept cx_unary_operator = is_unary_operator_v<T>;
#endif

	// Deprecated as of v0.3.0
	JCLIB_DEPRECATE_BLOCK
	((
		/**
		 * @brief CRTP type for adding piping semantics alongside other common unary function features
		 * @tparam OperatorT Operator type being CRTPd (child type)
		*/
		template <typename OperatorT>
		struct JCLIB_DEPRECATED("no longer needed, use non-crtp operator tag instead") unary_operator : public operator_tag
		{
		public:

		};
	));


	template <typename LT, typename OpT>
	constexpr inline auto operator|(LT&& lhs, const OpT& rhs) ->
		jc::enable_if_t
		<
			jc::is_invocable<OpT, LT>::value,
			jc::invoke_result_t<OpT, LT>
		>
	{
		return rhs(lhs);
	};

	// Deprecated as of v0.3.0
	JCLIB_DEPRECATE_BLOCK
	((
		/**
		 * @brief Tag type for implementing binary operators
		*/
		struct JCLIB_DEPRECATED("no longer needed for denoting binary operator types, just use operator_tag instead")
		binary_operator_tag : operator_tag {};
	));

	/**
	 * @brief Tests if an operator type is invocable with 2 arguements
	 * @tparam T Type to test
	*/
	template <typename T>
	struct is_binary_operator : bool_constant
		<
			is_base_of<operator_tag, remove_cvref_t<T>>::value &&
			is_invocable_with_count<T, 2>::value
		>
	{};

#ifdef JCLIB_FEATURE_INLINE_VARIABLES
	/**
	 * @brief Tests if an operator type is invocable with 2 arguements
	 * @tparam T Type to test
	*/
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
		struct bound_op<bind_first_t, OpT, T> :
			public operator_tag
		{
			template <typename... Ts>
			constexpr auto operator()(Ts&&... rhs) const ->
				decltype(jc::invoke(std::declval<const OpT>(),
					std::declval<const T&>(),
					std::forward<Ts>(std::declval<Ts&&>())...
				))
			{
				return jc::invoke(this->op, this->value, std::forward<Ts>(rhs)...);
			};

			constexpr bound_op(OpT _op, T _value) :
				op{ _op }, value{ _value }
			{};

			OpT op;
			T value;
		};

		template <typename OpT, typename T>
		struct bound_op<bind_second_t, OpT, T> :
			public operator_tag
		{
			template <typename... Ts>
			constexpr auto operator()(Ts&&... rhs) const ->
				decltype(jc::invoke(std::declval<const OpT>(),
					std::forward<Ts>(std::declval<Ts&&>())...,
					std::declval<const T&>()
				))
			{
				return jc::invoke(this->op, std::forward<Ts>(rhs)..., this->value);
			};

			constexpr bound_op(OpT _op, T _value) :
				op{ _op }, value{ _value }
			{};

			OpT op;
			T value;
		};
	};

	// Deprecated as of v0.3.0
	JCLIB_DEPRECATE_BLOCK
	((
		/**
		 * @brief CRTP type for allowing arguement binding
		 * @tparam OperatorT Operator type being CRTPd
		*/
		template <typename OperatorT>
		struct JCLIB_DEPRECATED("no longer needed, use non-crtp operator_tag instead") binary_operator : operator_tag
		{

		};
	));

	template <typename LT, typename RT>
	inline constexpr auto operator&(const LT& _value, const RT& _op) ->
		jc::enable_if_t
		<
			jc::is_binary_operator<RT>::value,
			impl::bound_op<impl::bind_first_t, RT, LT>
		>
	{
		return impl::bound_op<impl::bind_first_t, RT, LT>
		{
			_op, _value
		};
	};

	template <typename LT, typename RT>
	inline constexpr auto operator&(const LT& _op, const RT& _value) ->
		jc::enable_if_t
		<
			jc::is_binary_operator<LT>::value,
			impl::bound_op<impl::bind_second_t, LT, RT>
		>
	{
		return impl::bound_op<impl::bind_second_t, LT, RT>
		{
			_op, _value
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

	/**
	 * @brief Less than "<" comparison function object type
	*/
	struct less_t : operator_tag
	{
		template <typename T, typename U>
		constexpr inline auto operator()(T&& lhs, U&& rhs) const noexcept ->
			decltype(std::declval<T&&>() < std::declval<U&&>())
		{
			return lhs < rhs;
		};
	};

	/**
	 * @brief Less than "<" comparison function object
	*/
	constexpr static less_t less{};

	/**
	 * @brief Greater than ">" comparison function object type
	*/
	struct greater_t : operator_tag
	{
		template <typename T, typename U>
		constexpr inline auto operator()(T&& lhs, U&& rhs) const noexcept ->
			decltype(std::declval<T&&>() > std::declval<U&&>())
		{
			return lhs > rhs;
		};
	};

	/**
	 * @brief Greater than ">" comparison function object
	*/
	constexpr static greater_t greater{};

	/**
	 * @brief Equality "==" comparison function object
	*/
	struct equals_t : operator_tag
	{
		template <typename T, typename U>
		constexpr inline auto operator()(T&& lhs, U&& rhs) const noexcept ->
			decltype(std::declval<T&&>() == std::declval<U&&>())
		{
			return lhs == rhs;
		};
	};
	constexpr static equals_t equals{};

	/**
	 * @brief Inequality "!=" comparison function object type
	*/
	struct unequals_t : operator_tag
	{
		template <typename T, typename U>
		constexpr inline auto operator()(T&& lhs, U&& rhs) const noexcept ->
			decltype(std::declval<T&&>() != std::declval<U&&>())
		{
			return lhs != rhs;
		};
	};

	/**
	 * @brief Inequality "!=" comparison function object
	*/
	constexpr static unequals_t unequals{};

	/**
	 * @brief Addition "+" arithmatic function object type
	*/
	struct plus_t : operator_tag
	{
		template <typename T, typename U>
		constexpr inline auto operator()(T&& lhs, U&& rhs) const noexcept ->
			decltype(std::declval<T&&>() + std::declval<U&&>())
		{
			return lhs + rhs;
		};
	};

	/**
	 * @brief Addition "+" arithmatic function object
	*/
	constexpr static plus_t plus{};

	/**
	 * @brief Subtraction "-" arithmatic function object type
	*/
	struct minus_t : operator_tag
	{
		template <typename T, typename U>
		constexpr inline auto operator()(T&& lhs, U&& rhs) const noexcept ->
			decltype(std::declval<T&&>() - std::declval<U&&>())
		{
			return lhs - rhs;
		};
	};

	/**
	 * @brief Subtraction "-" arithmatic function object
	*/
	constexpr static minus_t minus{};

	/**
	 * @brief Multiplcation "*" arithmatic function object type
	*/
	struct times_t : operator_tag
	{
		template <typename T, typename U>
		constexpr inline auto operator()(T&& lhs, U&& rhs) const noexcept ->
			decltype(std::declval<T&&>()* std::declval<U&&>())
		{
			return lhs * rhs;
		};
	};

	/**
	 * @brief Multiplcation "*" arithmatic function object
	*/
	constexpr static times_t times{};

	/**
	 * @brief Division "/" arithmatic function object type
	*/
	struct divide_t : operator_tag
	{
		template <typename T, typename U>
		constexpr inline auto operator()(T&& lhs, U&& rhs) const noexcept ->
			decltype(std::declval<T&&>() / std::declval<U&&>())
		{
			return lhs / rhs;
		};
	};

	/**
	 * @brief Division "/" arithmatic function object
	*/
	constexpr static divide_t divide{};

	/**
	 * @brief Inversion "!" arithmatic function object type
	*/
	struct invert_t : operator_tag
	{
		template <typename T>
		constexpr auto operator()(T&& _value) const noexcept ->
			decltype(!std::declval<T&&>())
		{
			return !_value;
		};
	};

	/**
	 * @brief Inversion "!" arithmatic function object
	*/
	constexpr static invert_t invert{};



	/**
	 * @brief Binary AND "&&" logical function object type
	*/
	struct conjunct_t : operator_tag
	{
		template <typename T, typename U>
		constexpr auto operator()(T&& lhs, U&& rhs) const
			noexcept(noexcept(std::declval<T&&>() && std::declval<U&&>()))
			->
			decltype(std::declval<T&&>() && std::declval<U&&>())
		{
			return lhs && rhs;
		};
	};

	/**
	 * @brief Binary AND "&&" logical function object
	*/
	constexpr static conjunct_t conjunct{};



	/**
	 * @brief Binary OR "||" logical function object type
	*/
	struct disjunct_t : operator_tag
	{
		template <typename T, typename U>
		constexpr auto operator()(T&& lhs, U&& rhs) const
			noexcept(noexcept(std::declval<T&&>() || std::declval<U&&>()))
			->
			decltype(std::declval<T&&>() || std::declval<U&&>())
		{
			return lhs || rhs;
		};
	};

	/**
	 * @brief Binary OR "||" logical function object
	*/
	constexpr static disjunct_t disjunct{};


	
	/**
	 * @brief Binary modulus "%" function object type (ie. modulo(3, 2) -> 1)
	*/
	struct modulo_t : public operator_tag
	{
	public:
		template <typename T, typename U>
		constexpr auto operator()(T&& lhs, U&& rhs) const noexcept ->
			decltype(std::declval<T&&>() % std::declval<U&&>())
		{
			return lhs % rhs;
		};
	};

	/**
	 * @brief Binary modulus "%" function object (ie. modulo(3, 2) -> 1)
	*/
	constexpr static modulo_t modulo{};



	/**
	 * @brief Arithmatic negation "-n" function object type (ie. -(5) -> (-5), -(-5) -> (5))
	*/
	struct negate_t : public operator_tag
	{
	public:
		template <typename T>
		constexpr auto operator()(T&& lhs) const noexcept ->
			decltype(-std::declval<T&&>())
		{
			return -lhs;
		};
	};

	/**
	 * @brief Arithmatic negation "-n" function object (ie. -(5) -> (-5), -(-5) -> (5))
	*/
	constexpr static negate_t negate{};


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

	/**
	 * @brief Type trait for checking if an operator can be used with a type or types
	 * @tparam Op Operator function object type
	 * @tparam T Left hand side type
	 * @tparam U Right hand side type
	 * @tparam Enable SFINAE specialization point
	*/
	template <typename Op, typename T, typename U = T, typename Enable = void>
	struct has_operator : false_type {};
	
	/**
	* @brief Type trait for checking if a unary operator can be used with a type
	* @tparam Op Unary operator function object type
	* @tparam T Type to use with the operator
	*/
	template <typename Op, typename T>
	struct has_operator<Op, T, T, enable_if_t<
		is_unary_operator<Op>::value && impl::has_unary_operator<Op, T>::value
		>> : true_type{};

	/**
	* @brief Type trait for checking if a binary operator can be used with a type
	* @tparam Op Binary operator function object type
	* @tparam T Type to use on both sides of the operator
	*/
	template <typename Op, typename T>
	struct has_operator<Op, T, T, enable_if_t<
		is_binary_operator<Op>::value && impl::has_binary_operator<Op, T, T>::value
		>> : true_type{};

	/**
	* @brief Type trait for checking if a binary operator can be used with two types
	* @tparam Op Binary operator function object type
	* @tparam T Left hand side type
	* @tparam U Right hand side type
	*/
	template <typename Op, typename T, typename U>
	struct has_operator<Op, T, U, enable_if_t<
		is_binary_operator<Op>::value&& impl::has_binary_operator<Op, T, U>::value
		>> : true_type{};

#ifdef JCLIB_FEATURE_INLINE_VARIABLES
	/**
	 * @brief Convenience variable for the has_operator type trait
	 * @tparam Op Operator function object type
	 * @tparam T Left hand side type
	 * @tparam U Right hand side type
	*/
	template <typename Op, typename T, typename U = T>
	constexpr inline bool has_operator_v = has_operator<Op, T, U>::value;
#endif

	
	namespace impl
	{
#if JCLIB_FEATURE_CONCEPTS_V
		template <typename LT, typename LTIn, typename RT, typename RTIn>
		concept cx_both_forwardable_to = cx_is_forwardable_to_impl<LTIn, LT> && cx_is_forwardable_to_impl<RTIn, RT>;
#endif
		
		// Used to compose two function objects into a single function object
		template <typename LT, typename RT>
		struct piped_function : public jc::operator_tag
		{
		public:

			template <typename... WildCards>
			constexpr auto operator()(const WildCards&...) const ->
				jc::enable_if_t
				<
					jc::conjunction<jc::is_same<jc::impl::wildcard, WildCards>...>::value &&
					jc::is_invocable_with_count<const LT, sizeof...(WildCards)>::value &&
					(
						!jc::is_invocable_with_count<const LT, 0>::value
					),
					void
				>
			{};

			template <typename... ArgTs>
			constexpr auto operator()(ArgTs&&... _args) const ->
				decltype(std::declval<LT>()(std::declval<ArgTs>()...) | std::declval<RT>())
			{
				return this->lhs_(std::forward<ArgTs>(_args)...) | this->rhs_;
			};

			template <typename _LT, typename _RT
#if !JCLIB_FEATURE_CONCEPTS_V
				, typename = jc::enable_if_t<jc::is_forwardable_to<_LT, LT>::value&& jc::is_forwardable_to<_RT, RT>::value>
#endif
			>
				JCLIB_REQUIRES((cx_both_forwardable_to<LT, _LT, RT, _RT>))
				constexpr piped_function(_LT&& _lhs, _RT&& _rhs)
				noexcept(jc::is_noexcept_forwardable_to<_LT, LT>::value&& jc::is_noexcept_forwardable_to<_RT, RT>::value)
				: lhs_{ std::forward<_LT>(_lhs) }, rhs_{ std::forward<_RT>(_rhs) }
			{};

		private:
			LT lhs_;
			RT rhs_;
		};

	};


	template <typename LT, typename RT>
	constexpr inline auto operator|(const LT& _lhs, const RT& _rhs) ->
		jc::enable_if_t
		<
			jc::is_operator<RT>::value && jc::is_operator<LT>::value,
			impl::piped_function<LT, RT>
		>
	{
		return impl::piped_function<LT, RT>(_lhs, _rhs);
	};



	namespace impl
	{
		// Allows for regular functions and lamdas to be used like typical operator objects
		template <typename OpT>
		struct callwrap : public operator_tag
		{
		public:
			
			template <typename... Ts>
			constexpr auto operator()(Ts&&... _args) const
				-> decltype
				(
					jc::invoke(std::declval<const OpT&>(), std::forward<Ts>(std::declval<Ts&&>())...)
				)
			{
				return jc::invoke(this->op_, std::forward<Ts>(_args)...);
			};

			template <typename = jc::enable_if_t<jc::is_copy_constructible<OpT>::value>>
			constexpr callwrap(const OpT& _op) :
				op_{ _op }
			{};

			template <typename = jc::enable_if_t<jc::is_move_constructible<OpT>::value>>
			constexpr callwrap(OpT&& _op) :
				op_{ std::move(_op) }
			{};

		private:
			OpT op_;
		};
	};

	/**
	 * @brief Allows lamdas and functions to be used in function object composition, returns new function object
	*/
	struct call_t: public operator_tag
	{
	public:
		constexpr auto operator()(const jc::impl::wildcard& _wc) const noexcept {};

		template <typename OpT> 
		constexpr auto operator()(OpT&& _op) const noexcept
		{
			return impl::callwrap<jc::remove_cvref_t<OpT>>(std::forward<OpT>(_op));
		};
	};

	/**
	 * @brief Allows lamdas and functions to be used in function object composition, returns new function object
	*/
	constexpr static call_t call{};

};


namespace jc
{
	namespace impl
	{
		template <typename T>
		struct packed_arg
		{
		public:

			constexpr T&& unpack() noexcept
			{
				return std::move(this->val);
			};
			constexpr const T& unpack() const noexcept
			{
				return this->val;
			};

			constexpr operator T && () noexcept { return std::move(this->unpack()); };
			constexpr operator const T& () const noexcept { return this->unpack(); };



			constexpr packed_arg(T _value) :
				val{ std::move(_value) }
			{};

		private:
			T val;
		};

		template <typename T>
		struct packed_arg<T&>
		{
		public:
			constexpr T& unpack() noexcept { return this->val; };
			constexpr const T& unpack() const noexcept { return this->val; };

			constexpr operator T& () noexcept { return this->unpack(); };
			constexpr operator const T& () const noexcept { return this->unpack(); };

			constexpr packed_arg(T& v) : val{ v } {};

		private:
			std::reference_wrapper<T> val;
		};

		template <typename... Ts>
		struct argpack
		{
			using tuple = std::tuple<packed_arg<Ts>...>;

			constexpr argpack(Ts&&... _vals) :
				args{ std::forward<Ts>(_vals)... }
			{};

			tuple args{};
		};

	};

	struct pack_t
	{
		template <typename... Ts>
		constexpr auto operator()(Ts&&... _vals) const noexcept
		{
			return impl::argpack<Ts...>{ std::forward<Ts>(_vals)... };
		};
	};
	constexpr static pack_t pack{};



	template <typename OpT, typename... Ts, size_t... Idxs>
	constexpr inline auto apply(OpT&& _op, impl::argpack<Ts...> _args, std::index_sequence<Idxs...>) ->
		decltype(jc::invoke(std::declval<OpT&&>(), std::declval<Ts>()...))
	{
		return jc::invoke(_op, std::get<Idxs>(_args.args)...);
	};

	template <typename OpT, typename... Ts>
	constexpr inline auto apply(OpT&& _op, impl::argpack<Ts...> _args) ->
		decltype(jc::invoke(std::declval<OpT&&>(), std::declval<Ts>()...))
	{
		return apply(_op, std::move(_args), std::make_index_sequence<sizeof...(Ts)>{});
	};


	/**
	 * @brief Implements arguement pack piping allowing a function object to be invoked with multiple arguements
	 * @tparam ...Ts Packed arguement types
	 * @tparam OpT Function object type
	 * @tparam Enable SFINAE point for ensuring this is a valid pipe expression
	 * @param _args Arguement pack object
	 * @param _op Function object to invoke
	 * @return Result of applying the arguement pack to the function object
	*/
	template <typename... Ts, typename OpT,
		typename Enable = jc::enable_if_t<jc::is_operator<OpT>::value&& jc::is_invocable_with_count<OpT, sizeof...(Ts)>::value>
	>
		constexpr inline auto operator|(jc::impl::argpack<Ts...> _args, const OpT& _op)
	{
		return jc::apply(_op, std::move(_args));
	};

};

namespace jc
{
	/**
	 * @brief Provides a type based wrapper for get_ftor specializations that use a non-type template parameter as the index
	 * @tparam T Index value type
	 * @tparam Value Index value
	*/
	template <typename T, T Value>
	struct get_value_idx : public std::integral_constant<T, Value> {};

	/**
	 * @brief Customization point for "overloading" std::get
	 * @tparam T Type to get from
	 * @tparam IdxT Index type (ie. std::get<IdxT>(T&))
	 * @tparam Enable SFINAE specialization point
	*/
	template <typename T, typename IdxT, typename Enable = void>
	struct get_ftor;

	// std::get where index is a template type parameter
	template <typename T, typename IdxT>
	struct get_ftor<T, IdxT, void_t<decltype(std::get<IdxT>(std::declval<T&>()))>>
	{
		constexpr auto operator()(T& _from) const -> decltype(std::get<IdxT>(std::declval<T&>()))
		{
			return std::get<IdxT>(_from);
		};
		constexpr auto operator()(const T& _from) const -> decltype(std::get<IdxT>(std::declval<const T&>()))
		{
			return std::get<IdxT>(_from);
		};
	};

	// std::get where index is a non-type template parameter
	template <typename T, typename IdxT, IdxT Value>
	struct get_ftor<T, get_value_idx<IdxT, Value>, void_t<decltype(std::get<Value>(std::declval<T&>()))>>
	{
		constexpr auto operator()(T& _from) const -> decltype(std::get<Value>(std::declval<T&>()))
		{
			return std::get<Value>(_from);
		};
		constexpr auto operator()(const T& _from) const -> decltype(std::get<Value>(std::declval<const T&>()))
		{
			return std::get<Value>(_from);
		};
	};




	template <typename IdxT, typename FromT>
	constexpr inline auto get(FromT& _from) ->
		decltype(std::declval<get_ftor<std::remove_const_t<FromT>, IdxT>>()(_from))
	{
		return get_ftor<std::remove_const_t<FromT>, IdxT>{}(_from);
	};

	template <size_t N, typename FromT>
	constexpr inline auto get(FromT& _from) ->
		decltype(std::declval<get_ftor<std::remove_const_t<FromT>, get_value_idx<size_t, N>>>()(_from))
	{
		return get_ftor<std::remove_const_t<FromT>, get_value_idx<size_t, N>>{}(_from);
	};

};

namespace jc
{

	template <size_t N, typename... Ts>
	struct get_ftor<impl::argpack<Ts...>, jc::get_value_idx<size_t, N>, void>
	{
		constexpr auto operator()(impl::argpack<Ts...>& _args) const ->
			decltype(jc::get<N>(std::declval<impl::argpack<Ts...>&>().args).unpack())
		{
			return jc::get<N>(_args.args).unpack();
		};
		constexpr auto operator()(const impl::argpack<Ts...>& _args) const ->
			decltype(jc::get<N>(std::declval<const impl::argpack<Ts...>&>().args).unpack())
		{
			return jc::get<N>(_args.args).unpack();
		};
	};


};

namespace jc
{
	namespace impl
	{
		// Implementation for generic arguement packing
		template <typename T>
		struct repack_impl;

		/*
		{
			constexpr auto operator()(T& _value) const noexcept
			{
				return jc::pack(_value);
			};
		};

		template <typename T>
		struct repack_impl<const T>
		{
			constexpr auto operator()(const T& _value) const noexcept
			{
				return jc::pack(_value);
			};
		};
		*/


		template <template <typename... Ts> class T, typename... Ts>
		struct repack_impl<T<Ts...>>
		{
			template <size_t... Idxs>
			constexpr auto operator()(T<Ts...>& _pack, std::index_sequence<Idxs...>) const
			{
				return jc::pack(std::forward<Ts>(jc::get<Idxs>(_pack))...);
			};

			constexpr auto operator()(T<Ts...>& _pack) const
			{
				return (*this)(_pack, std::make_index_sequence<sizeof...(Ts)>{});
			};
		};

		template <template <typename... Ts> class T, typename... Ts>
		struct repack_impl<const T<Ts...>>
		{
			template <size_t... Idxs>
			constexpr auto operator()(T<Ts...> _pack, std::index_sequence<Idxs...>) const
			{
				return jc::pack(std::forward<Ts>(jc::get<Idxs>(_pack))...);
			};
			constexpr auto operator()(const T<Ts...>& _pack) const
			{
				return (*this)(T<Ts...>(_pack), std::make_index_sequence<sizeof...(Ts)>{});
			};
		};

		template <>
		struct repack_impl<wildcard>
		{
			constexpr auto& operator()(wildcard& w) const noexcept { return w; };
		};

	};

	struct repack_t : public jc::operator_tag
	{
		template <typename T>
		constexpr auto operator()(T&& _val) const
		{
			return impl::repack_impl<jc::remove_reference_t<T>>{}(_val);
		};
	};
	constexpr repack_t repack{};
};


#pragma region BINARY_OPERATORS

namespace jc
{
	/**
	 * @brief Binary NOT operator - "~" function object type
	*/
	struct bnot_t : public operator_tag
	{
		template <typename T>
		constexpr auto operator()(T&& val) const
			noexcept(noexcept(~std::declval<T&&>()))
			-> decltype(~std::declval<T&&>())
		{
			return ~val;
		};
	};

	/**
	 * @brief Binary NOT operator - "~" function object
	*/
	constexpr static bnot_t bnot{};

	/**
	 * @brief Binary AND operator - "&" function object type
	*/
	struct band_t : public operator_tag
	{
		template <typename LT, typename RT>
		constexpr auto operator()(LT&& lhs, RT&& rhs) const
			noexcept(noexcept(std::declval<LT&&>() & std::declval<RT&&>()))
			-> decltype(std::declval<LT&&>() & std::declval<RT&&>())
		{
			return lhs & rhs;
		};
	};

	/**
	 * @brief Binary AND operator - "&" function object
	*/
	constexpr static band_t band{};

	/**
	 * @brief Binary OR operator - "|" function object type
	*/
	struct bor_t : public operator_tag
	{
		template <typename LT, typename RT>
		constexpr auto operator()(LT&& lhs, RT&& rhs) const
			noexcept(noexcept(std::declval<LT&&>() | std::declval<RT&&>()))
			-> decltype(std::declval<LT&&>() | std::declval<RT&&>())
		{
			return lhs | rhs;
		};
	};

	/**
	 * @brief Binary OR operator - "|" function object
	*/
	constexpr static bor_t bor{};

	/**
	 * @brief Binary XOR operator - "^" function object type
	*/
	struct bxor_t : public operator_tag
	{
		template <typename LT, typename RT>
		constexpr auto operator()(LT&& lhs, RT&& rhs) const
			noexcept(noexcept(std::declval<LT&&>() ^ std::declval<RT&&>()))
			-> decltype(std::declval<LT&&>() ^ std::declval<RT&&>())
		{
			return lhs ^ rhs;
		};
	};

	/**
	 * @brief Binary XOR operator - "^" function object
	*/
	constexpr static bxor_t bxor{};

};

#pragma endregion BINARY_OPERATORS



/*
	Member access operators and other value access operators
*/
#pragma region ACCESSOR_OPERATORS

namespace jc
{
	/**
	 * @brief Unary dereference operator - "*" function object type (ie. dereference(int*) -> int&)
	*/
	struct dereference_t : public operator_tag
	{
	public:
		template <typename T>
		constexpr auto operator()(T&& _value) const noexcept ->
			decltype(*std::declval<T&&>())
		{
			return *_value;
		};
	};

	/**
	 * @brief Unary dereference operator - "*" function object type (ie. dereference(int*) -> int&)
	*/
	constexpr static dereference_t dereference{};



	/**
	 * @brief Unary address_of operator - "&" function object type (ie. address_of(int) -> int*)
	*/
	struct address_of_t : public operator_tag
	{
		template <typename T>
		constexpr auto operator()(T&& val) const
			noexcept(noexcept(&std::declval<T&&>()))
			-> decltype(&std::declval<T&&>())
		{
			return &val;
		};
		
		// Wildcard overload for function probing
		constexpr auto operator()(wildcard w) const
		{
			return w;
		};
	};

	/**
	 * @brief Unary address_of operator - "&" function object (ie. address_of(int) -> int*)
	*/
	constexpr static address_of_t address_of{};



	/**
	 * @brief Class member variable access operator - function object type
	*/
	struct member_t : public operator_tag
	{
		template <typename ClassT, typename VarT>
		constexpr auto operator()(ClassT& _obj, VarT jc::remove_const_t<ClassT>::* _member) const
			noexcept(noexcept( std::declval<ClassT&>().*(std::declval<VarT jc::remove_const_t<ClassT>::*>()) ))
			-> decltype(std::declval<ClassT&>().*(std::declval<VarT jc::remove_const_t<ClassT>::*>()))
		{
			return _obj.*_member;
		};



		// Wildcard overloads for function probing
		constexpr auto operator()(wildcard w) const
		{
			return w;
		};
		constexpr auto operator()(wildcard w, wildcard) const
		{
			return w;
		};
	};

	/**
	 * @brief Class member variable access operator - "&" object
	*/
	constexpr static member_t member{};

};

#pragma endregion ACCESSOR_OPERATORS



/*
	Standard library functionality related operators
*/
#pragma region STD_OPERATORS

namespace jc
{
	/**
	 * @brief Adds an "is_transparent" tag to an operator by inheriting from it.
	 * @tparam T Operator type to add tag too.
	 * @param Enable SFINAE specialization point.
	*/
	template <typename T, typename Enable = void>
	struct transparent;

	/**
	 * @brief Adds an "is_transparent" tag to an operator by inheriting from it.
	 * @tparam T Operator type to add tag too.
	*/
	template <typename T> JCLIB_REQUIRES(jc::cx_operator<T>)
	struct transparent<T, JCLIB_ENABLE_IF_CXSWITCH(void, jc::is_operator<T>::value)> : public T
	{
		/**
		 * @brief Marks the type as transparent mostly for use with std::unordered_map
		*/
		using is_transparent = void;

		// Allow the parent's constructor
		using T::T;
	};

	/**
	 * @brief Hash operator type using std::hash for the actual implementation.
	*/
	struct hash_t : jc::operator_tag
	{
		template <typename T>
		constexpr auto operator()(const T& _value) const
			noexcept(noexcept(std::hash<T>{}(std::declval<const T&>()))) ->
			decltype(std::hash<T>{}(std::declval<const T&>()))
		{
			return std::hash<T>{}(_value);
		};

		template <size_t N>
		constexpr size_t operator()(const char(&_value)[N]) const noexcept
		{
#if JCLIB_FEATURE_STRING_VIEW_V
			return std::hash<std::string_view>{}(std::string_view{ _value, N });
#else
			return std::hash<std::string>{}(std::string{ _value, N });
#endif
		};

		template <size_t N>
		constexpr size_t operator()(const wchar_t(&_value)[N]) const noexcept
		{
#if JCLIB_FEATURE_STRING_VIEW_V
			return std::hash<std::wstring_view>{}(std::wstring_view{ _value, N });
#else
			return std::hash<std::wstring>{}(std::wstring{ _value, N });
#endif
		};
		
		template <size_t N>
		constexpr size_t operator()(const char16_t(&_value)[N]) const noexcept
		{
#if JCLIB_FEATURE_STRING_VIEW_V
			return std::hash<std::u16string_view>{}(std::u16string_view{ _value, N });
#else
			return std::hash<std::u16string>{}(std::u16string{ _value, N });
#endif
		};
		
		template <size_t N>
		constexpr size_t operator()(const char32_t(&_value)[N]) const noexcept
		{
#if JCLIB_FEATURE_STRING_VIEW_V
			return std::hash<std::u32string_view>{}(std::u32string_view{ _value, N });
#else
			return std::hash<std::u32string>{}(std::u32string{ _value, N });
#endif
		};

#if JCLIB_FEATURE_CHAR8_V
		template <size_t N>
		constexpr size_t operator()(const char8_t(&_value)[N]) const noexcept
		{
#if JCLIB_FEATURE_STRING_VIEW_V
			return std::hash<std::u8string_view>{}(std::u8string_view{ _value, N });
#else
			return std::hash<std::u8string>{}(std::u8string{ _value, N });
#endif
		};
#endif
	};

	/**
	 * @brief Hash operator using std::hash for the actual implementation.
	*/
	constexpr static hash_t hash{};

};

// Required to prevent issues with jc::wildcard operator argument probing.
namespace std
{
	template <>
	struct hash<jc::wildcard>
	{
		constexpr const size_t operator()(const jc::wildcard& wc) const noexcept
		{
			return 0;
		};
	};
};

#pragma endregion STD_OPERATORS

#endif