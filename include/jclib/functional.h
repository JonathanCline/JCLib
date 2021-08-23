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
			template <typename U>
			constexpr auto operator()(const U& rhs) const ->
				jc::invoke_result_t<OpT, T, U>
			{
				return jc::invoke(this->op, this->value, rhs);
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
			template <typename U>
			constexpr auto operator()(const U& lhs) const ->
				jc::invoke_result_t<OpT, U, T>
			{
				return jc::invoke(this->op, lhs, this->value);
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
	 * @brief Unary dereference "*" function object type (ie. dereference(int*) -> int&)
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
	 * @brief Unary dereference "*" function object (ie. dereference(int*) -> int&)
	*/
	constexpr static dereference_t dereference{};





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
					jc::is_invocable_with_count<const LT, sizeof...(WildCards)>::value,
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

};

#endif