#pragma once
#ifndef JCLIB_TYPE_TRAITS_H
#define JCLIB_TYPE_TRAITS_H

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
	Contains any using directives for standard library type traits used in SAELib along with SAELib specific type traits
*/

#include "jclib/config.h"
#include "jclib/feature.h"


#define _JCLIB_TYPE_TRAITS_

#include <type_traits>
#include <limits>
#include <utility>
#include <cstdint>

namespace jc
{

#ifdef __cpp_lib_bool_constant
	using std::bool_constant;
	using std::true_type;
	using std::false_type;
#else
	template <bool B>
	struct bool_constant : public std::integral_constant<bool, B> {};
	using true_type = bool_constant<true>;
	using false_type = bool_constant<false>;
#endif

	using std::enable_if;
	using std::enable_if_t;

	/**
	 * @brief A fancy way of saying void
	 * @tparam ...Ts 
	*/
	template <typename... Ts>
	using void_t = void;

	/*
		Type comparison
	*/

	using std::is_same;
	using std::is_convertible;







	// Shamelessly copied from https://en.cppreference.com/w/cpp/types/disjunction
	
	template<typename...>
	struct disjunction :
		jc::false_type
	{};

	template<typename B1>
	struct disjunction<B1> : B1
	{};

	template<typename B1, typename... Bn>
	struct disjunction<B1, Bn...> :
#ifdef __cpp_fold_expressions
		jc::bool_constant<bool(B1::value) || (bool(Bn::value) || ...)>
#else
		std::conditional_t<bool(B1::value), B1, jc::disjunction<Bn...>>
#endif
	{};



	// Shamelessly copied from https://en.cppreference.com/w/cpp/types/conjunction

	template<typename...>
	struct conjunction :
		jc::true_type
	{};

	template<typename B1>
	struct conjunction<B1> : B1
	{};

	template<typename B1, typename... Bn>
	struct conjunction<B1, Bn...> :
#ifdef __cpp_fold_expressions
		jc::bool_constant<bool(B1::value) && (bool(Bn::value) && ...)>
#else
		std::conditional_t<bool(B1::value), jc::conjunction<Bn...>, B1>
#endif
	{};




	template <typename B, typename Enable = void>
	struct negation;

	template<class B>
	struct negation<B, void_t<decltype(B::value)>> :
		jc::bool_constant<!bool(B::value)>
	{};



#ifdef __cpp_inline_variables
	using std::is_same_v;
	using std::is_convertible_v;

	template <typename... Bs>
	constexpr inline auto conjunction_v = conjunction<Bs...>::value;

	template <typename... Bs>
	constexpr inline auto disjunction_v = disjunction<Bs...>::value;

	template <typename B>
	constexpr inline auto negation_v = negation<B>::value;
#endif

	template <typename T, typename... Ts>
	struct is_any_of :
#ifdef __cpp_fold_expressions
		bool_constant<(std::is_same_v<T, Ts> || ...)>
#else
		disjunction<std::is_same<T, Ts>...>
#endif
	{};

#ifdef __cpp_inline_variables
	template <typename T, typename... Ts>
	JCLIB_CONSTEXPR inline bool is_any_of_v = is_any_of<T, Ts...>::value;
#endif

	template <typename T, typename U>
	struct is_element_of;

	template <typename T, template <typename... Ts> typename U, typename... Ts>
	struct is_element_of<T, U<Ts...>> : bool_constant<is_any_of<T, Ts...>::value> {};

#if __cpp_inline_variables
	template <typename T, typename U>
	constexpr inline auto is_element_of_v = is_element_of<T, U>::value;
#endif




	/*
		Character type traits
	*/

	namespace impl
	{
		/**
		 * @brief Character types tuple
		*/
		using character_typelist = std::tuple
		<
			char,
			wchar_t,
#ifdef __cpp_char8_t
			char8_t,
#endif
			char16_t,
			char32_t
		>;
	};

	/**
	 * @brief Is true if the given type is a character type
	*/
	template <typename T>
	struct is_character :
		public bool_constant<is_element_of<T, impl::character_typelist>::value>
	{};

#ifdef __cpp_inline_variables
	template <typename T>
	JCLIB_CONSTEXPR inline bool is_character_v = is_character<T>::value;
#endif

	/*
		Basic type catagory
	*/

	using std::is_fundamental;
	using std::is_integral;
	using std::is_arithmetic;
	using std::is_floating_point;
	using std::is_signed;
	using std::is_unsigned;
	using std::is_pointer;

#ifdef __cpp_inline_variables
	using std::is_fundamental_v;
	using std::is_integral_v;
	using std::is_arithmetic_v;
	using std::is_floating_point_v;
	using std::is_signed_v;
	using std::is_unsigned_v;
	using std::is_pointer_v;
#endif

	template <typename T>
	struct is_integer : public bool_constant<is_integral<T>::value && std::numeric_limits<T>::is_integer>
	{};
	
#ifdef __cpp_inline_variables
	template <typename T>
	JCLIB_CONSTEXPR inline bool is_integer_v = is_integer<T>::value;
#endif

	/*
		Constructor / Assignment related
	*/

	using std::is_constructible;
	using std::is_copy_constructible;
	using std::is_copy_assignable;
	using std::is_move_constructible;
	using std::is_move_assignable;
	using std::is_default_constructible;
	using std::is_assignable;

#ifdef __cpp_inline_variables
	using std::is_constructible_v;
	using std::is_copy_constructible_v;
	using std::is_copy_assignable_v;
	using std::is_move_constructible_v;
	using std::is_move_assignable_v;
	using std::is_default_constructible_v;
	using std::is_assignable_v;
#endif


	/*
		Trivial type
	*/

	using std::is_trivially_default_constructible;
	using std::is_trivially_copy_constructible;
	using std::is_trivially_copy_assignable;
	using std::is_trivially_copyable;
	using std::is_trivially_move_constructible;
	using std::is_trivially_move_assignable;
	using std::is_trivially_destructible;

#ifdef __cpp_inline_variables
	using std::is_trivially_default_constructible_v;
	using std::is_trivially_copy_constructible_v;
	using std::is_trivially_copy_assignable_v;
	using std::is_trivially_copyable_v;
	using std::is_trivially_move_constructible_v;
	using std::is_trivially_move_assignable_v;
	using std::is_trivially_destructible_v;
#endif

	template <typename T>
	struct is_trivially_moveable : public bool_constant<
		is_trivially_move_assignable<T>::value && is_trivially_move_constructible<T>::value
	> {};

#ifdef __cpp_inline_variables
	template <typename T>
	JCLIB_CONSTEXPR inline bool is_trivially_moveable_v = is_trivially_moveable<T>::value;
#endif

	using std::is_trivially_assignable;
	using std::is_trivially_constructible;
	using std::is_trivially_default_constructible;
	using std::is_trivial;

#ifdef __cpp_inline_variables
	using std::is_trivially_assignable_v;
	using std::is_trivially_constructible_v;
	using std::is_trivially_default_constructible_v;
	using std::is_trivial_v;
#endif


#if JCLIB_FEATURE_CONCEPTS_V
	namespace impl
	{
		template <typename From, typename To>
		concept cx_is_forwardable_to_impl = requires (From&& _from)
		{
			To{ std::forward<From>(_from) };
		};
	};
#endif


	/**
	 * @brief Tests if a type can be constructed from a forwarded value
	 * @tparam From Type being forwarded
	 * @tparam To Type being constructed
	*/
	template <typename From, typename To>
	struct is_forwardable_to : public bool_constant <
#if JCLIB_FEATURE_CONCEPTS_V
			impl::cx_is_forwardable_to_impl<From, To>
#else
			is_constructible<decltype(std::forward<From>(std::declval<From&&>())), To>::value
#endif
		>
	{};

#if JCLIB_FEATURE_INLINE_VARIABLES_V
	/**
	 * @brief Tests if a type can be constructed from a forwarded value
	 * @tparam From Type being forwarded
	 * @tparam To Type being constructed
	*/
	template <typename From, typename To>
	JCLIB_CONSTEXPR inline bool is_forwardable_to_v = is_forwardable_to<From, To>::value;
#endif
	

	/**
	 * @brief Tests if a type can be constructed from a forwarded value and would be noexcept
	 * @tparam From Type being forwarded
	 * @tparam To Type being constructed
	*/
	template <typename From, typename To>
	struct is_noexcept_forwardable_to : public
		bool_constant
		<
			is_forwardable_to<From, To>::value &&
			noexcept(noexcept(To{ std::forward<From>(std::declval<From&&>()) }))
		>
	{};

#if JCLIB_FEATURE_INLINE_VARIABLES_V
	/**
	 * @brief Tests if a type can be constructed from a forwarded value and would be noexcept
	 * @tparam From Type being forwarded
	 * @tparam To Type being constructed
	*/
	template <typename From, typename To>
	JCLIB_CONSTEXPR inline bool is_noexcept_forwardable_to_v = is_noexcept_forwardable_to<From, To>::value;
#endif



	/*
		Misc. Type Traits
	*/

	using std::is_base_of;
	using std::is_abstract;

#ifdef JCLIB_FEATURE_INLINE_VARIABLES
	using std::is_base_of_v;
	using std::is_abstract_v;
#endif






	/*
		Function related
	*/

	namespace impl
	{
		struct noexcept_function_tag {};
		struct const_function_tag {};


		/**
		 * @brief Determines function properties based on specialization selection
		 * @tparam OpT Function or other invocable type
		*/
		template <typename OpT>
		struct function_traits_impl;


		// function signature type
		template <typename ReturnT, typename... Ts>
		struct function_traits_impl<ReturnT(Ts...)>
		{
			using return_type = ReturnT;
			using arguement_typelist = std::tuple<Ts...>;
		};

		// free function type
		template <typename ReturnT, typename... Ts>
		struct function_traits_impl<ReturnT(*)(Ts...)>
		{
			using return_type = ReturnT;
			using arguement_typelist = std::tuple<Ts...>;
		};

#if __cpp_noexcept_function_type
		// noexcept free function type
		template <typename ReturnT, typename... Ts>
		struct function_traits_impl<ReturnT(*)(Ts...) noexcept> : noexcept_function_tag
		{
			using return_type = ReturnT;
			using arguement_typelist = std::tuple<Ts...>;
		};
#endif

		// member function type
		template <typename ReturnT, typename ClassT, typename... Ts>
		struct function_traits_impl<ReturnT(ClassT::*)(Ts...)>
		{
			using class_type = ClassT;
			using return_type = ReturnT;
			using arguement_typelist = std::tuple<Ts...>;
		};

#if __cpp_noexcept_function_type
		// noexcept member function type
		template <typename ReturnT, typename ClassT, typename... Ts>
		struct function_traits_impl<ReturnT(ClassT::*)(Ts...) noexcept> : noexcept_function_tag
		{
			using class_type = ClassT;
			using return_type = ReturnT;
			using arguement_typelist = std::tuple<Ts...>;
		};
#endif

		// const member function type
		template <typename ReturnT, typename ClassT, typename... Ts>
		struct function_traits_impl<ReturnT(ClassT::*)(Ts...) const> : const_function_tag
		{
			using class_type = const ClassT;
			using return_type = ReturnT;
			using arguement_typelist = std::tuple<Ts...>;
		};

#if __cpp_noexcept_function_type
		// const noexcept member function type
		template <typename ReturnT, typename ClassT, typename... Ts>
		struct function_traits_impl<ReturnT(ClassT::*)(Ts...) const noexcept> : const_function_tag, noexcept_function_tag
		{
			using class_type = const ClassT;
			using return_type = ReturnT;
			using arguement_typelist = std::tuple<Ts...>;
		};
#endif
	};


	template <typename OpT, typename Enable = void>
	struct function_arguements;

	template <typename OpT>
	struct function_arguements<OpT,
		void_t<typename impl::function_traits_impl<OpT>::arguement_typelist>
	>
	{
		using type = typename impl::function_traits_impl<OpT>::arguement_typelist;
	};

	template <typename OpT>
	using function_arguements_t = typename function_arguements<OpT>::type;





	namespace impl
	{
		template <typename RetT, typename T>
		struct function_signature_impl;

		template <typename RetT, template <typename... Ts> typename T, typename... Ts>
		struct function_signature_impl<RetT, T<Ts...>>
		{
			using type = RetT(Ts...);
		};
	};

	template <typename OpT, typename Enable = void>
	struct function_signature;

	template <typename OpT>
	struct function_signature<OpT, void_t
		<
			typename impl::function_traits_impl<OpT>::return_type,
			typename impl::function_traits_impl<OpT>::arguement_typelist
		>>
	{
		using type = typename impl::function_signature_impl
			<
				typename impl::function_traits_impl<OpT>::return_type,
				typename impl::function_traits_impl<OpT>::arguement_typelist
			>::type;
	};

	template <typename OpT>
	using function_signature_t = typename function_signature<OpT>::type;






	template <typename OpT, typename Enable = void>
	struct function_arguement_count;

	template <typename OpT>
	struct function_arguement_count<OpT,
		jc::void_t<typename impl::function_traits_impl<OpT>::arguement_typelist>
	> :
		public std::integral_constant
		<
			size_t,
			std::tuple_size<typename impl::function_traits_impl<OpT>::arguement_typelist>::value
		>
	{};

#if JCLIB_FEATURE_INLINE_VARIABLES_V
	template <typename OpT>
	constexpr inline auto function_arguement_count_v = function_arguement_count<OpT>::value;
#endif



	namespace impl
	{
		/**
		 * @brief Type used to fake any value
		*/
		struct wildcard
		{
			constexpr auto operator==(wildcard rhs) const noexcept { return true; };
			constexpr auto operator!=(wildcard rhs) const noexcept { return true; };

			constexpr auto operator>(wildcard rhs) const noexcept { return true; };
			constexpr auto operator<(wildcard rhs) const noexcept { return true; };
			constexpr auto operator>=(wildcard rhs) const noexcept { return true; };
			constexpr auto operator<=(wildcard rhs) const noexcept { return true; };

			constexpr auto operator+(wildcard rhs) const noexcept { return *this; };
			constexpr auto operator-(wildcard rhs) const noexcept { return *this; };
			constexpr auto operator*(wildcard rhs) const noexcept { return *this; };
			constexpr auto operator/(wildcard rhs) const noexcept { return *this; };

			constexpr auto& operator+=(wildcard rhs) noexcept { return *this; };
			constexpr auto& operator-=(wildcard rhs) noexcept { return *this; };
			constexpr auto& operator*=(wildcard rhs) noexcept { return *this; };
			constexpr auto& operator/=(wildcard rhs) noexcept { return *this; };

			constexpr auto& operator++() noexcept { return *this; };
			constexpr auto operator++(int) noexcept { return *this; };

			constexpr auto& operator--() noexcept { return *this; };
			constexpr auto operator--(int) noexcept { return *this; };

			constexpr auto operator~() const noexcept { return *this; };
			constexpr auto operator!() const noexcept { return *this; };

			constexpr auto operator&(wildcard rhs) const noexcept { return *this; };
			constexpr auto operator|(wildcard rhs) const noexcept { return *this; };
			constexpr auto operator^(wildcard rhs) const noexcept { return *this; };
			constexpr auto operator&=(wildcard rhs) const noexcept { return *this; };
			constexpr auto operator|=(wildcard rhs) const noexcept { return *this; };
			constexpr auto operator^=(wildcard rhs) const noexcept { return *this; };

			constexpr auto operator>>(wildcard rhs) const noexcept { return *this; };
			constexpr auto operator<<(wildcard rhs) const noexcept { return *this; };
			constexpr auto& operator>>=(wildcard rhs) noexcept { return *this; };
			constexpr auto& operator<<=(wildcard rhs) noexcept { return *this; };

			constexpr auto operator*() noexcept { return *this; };
			constexpr auto operator*() const noexcept { return *this; };
			constexpr auto operator->() noexcept { return *this; };
			constexpr auto operator->() const noexcept { return *this; };





			template <typename T>
			constexpr operator T& () const noexcept;

			template <typename T>
			constexpr operator T && () const noexcept;


			constexpr wildcard() = delete;
		};


		// Used to turn pack expansion into the wildcard type
		template <size_t N>
		struct wildcard_pos_void
		{
			using type = wildcard;
		};


		// Turns an index sequence into a wildcard tuple
		template <typename T>
		struct make_wildcard_tuple_impl;

		template <size_t... Ns>
		struct make_wildcard_tuple_impl<std::index_sequence<Ns...>>
		{
			// pack expansion abuse
			using type = std::tuple<typename wildcard_pos_void<Ns>::type...>;
		};


		template <size_t N>
		using make_wildcard_tuple_t = typename make_wildcard_tuple_impl<std::make_index_sequence<N>>::type;



		


		template <typename OpT, typename T, typename Enable = void>
		struct invocable_with_arg_count_impl : jc::false_type {};

		template <typename OpT, template <typename... Ts> typename TupT, typename... Ts>
		struct invocable_with_arg_count_impl<OpT, TupT<Ts...>, void_t
			<
			decltype(std::declval<OpT>()(std::declval<Ts>()...))
			>> : jc::true_type
		{};
	};



	/**
	 * @brief Tests if a type is invocable given 'N' number of arguements
	 * @tparam OpT Type to test
	 * @tparam N Number of arguements
	*/
	template <typename OpT, size_t N>
	struct is_invocable_with_count :
		public impl::invocable_with_arg_count_impl<OpT, impl::make_wildcard_tuple_t<N>>
	{};

#if JCLIB_FEATURE_INLINE_VARIABLES_V

	/**
	 * @brief Tests if a type is invocable given 'N' number of arguements
	 * @tparam OpT Type to test
	 * @tparam N Number of arguements
	*/
	template <typename OpT, size_t N>
	constexpr inline auto is_invocable_with_count_v = is_invocable_with_count<OpT, N>::value;

#endif





	namespace impl
	{
		template <typename Op, typename Tup, typename = void>
		struct invocable_impl : false_type {};

		// generic function case
		template <typename Op, typename... ArgTs>
		struct invocable_impl <Op, std::tuple<ArgTs...>,
			void_t<decltype(std::declval<Op>()(std::declval<ArgTs>()...))>
		> : true_type
		{
			using arguement_typelist = std::tuple<ArgTs...>;
			using return_type = decltype(std::declval<Op>()(std::declval<ArgTs>()...));
		};

		// needed to handle case where the function has no arguements
		template <typename Op>
		struct invocable_impl <Op, std::tuple<void>,
			void_t<decltype(std::declval<Op>()())>
		> : true_type
		{
			using arguement_typelist = std::tuple<void>;
			using return_type = decltype(std::declval<Op>()());
		};

		// free function case, should be handled by the general case but im leaving this in for now
		template <typename ReturnT, typename... ArgTs>
		struct invocable_impl<ReturnT(*)(ArgTs...), std::tuple<ArgTs...>,
			void_t<decltype(
				(*std::declval<ReturnT(*)(ArgTs...)>())(std::declval<ArgTs>()...)
				)>
		> : true_type
		{
			using arguement_typelist = std::tuple<ArgTs...>;
			using return_type = ReturnT;
		};

		// needed to handle case where the function has no arguements
		template <typename ReturnT>
		struct invocable_impl<ReturnT(*)(), std::tuple<void>,
			void_t<decltype((*std::declval<ReturnT(*)()>())())>
		> : true_type
		{
			using arguement_typelist = std::tuple<void>;
			using return_type = ReturnT;
		};

		// member function case
		template <typename ReturnT, typename ClassT, typename... ArgTs>
		struct invocable_impl<ReturnT(ClassT::*)(ArgTs...), std::tuple<ArgTs...>,
			void_t<decltype(
				(std::declval<ClassT*>()->*std::declval<ReturnT(ClassT::*)(ArgTs...)>())(std::declval<ArgTs>()...)
				)>
		> : true_type
		{
			using arguement_typelist = std::tuple<ArgTs...>;
			using return_type = ReturnT;
		};

		// needed to handle case where the function has no arguements
		template <typename ReturnT, typename ClassT>
		struct invocable_impl<ReturnT(ClassT::*)(), std::tuple<void>,
			void_t<decltype((std::declval<ClassT*>()->*std::declval<ReturnT(ClassT::*)()>())())>
		> : true_type
		{
			using arguement_typelist = std::tuple<void>;
			using return_type = ReturnT;
		};

		// const member function case
		template <typename ReturnT, typename ClassT, typename... ArgTs>
		struct invocable_impl<ReturnT(ClassT::*)(ArgTs...) const, std::tuple<ArgTs...>,
			void_t<decltype(
				(std::declval<const ClassT*>()->*std::declval<ReturnT(ClassT::*)(ArgTs...) const>())(std::declval<ArgTs>()...)
				)>
		> : true_type
		{
			using arguement_typelist = std::tuple<ArgTs...>;
			using return_type = ReturnT;
		};

		// const member function with no args case
		template <typename ReturnT, typename ClassT>
		struct invocable_impl<ReturnT(ClassT::*)() const, std::tuple<void>,
			void_t<decltype(
				(std::declval<const ClassT*>()->*std::declval<ReturnT(ClassT::*)() const>())()
				)>
		> : true_type
		{
			using arguement_typelist = std::tuple<void>;
			using return_type = ReturnT;
		};
	};

	/**
	 * @brief Is true is if Op is invocable given a list of arguements (backport of C++17 std::is_invocable)
	 * @tparam Op Function or other callable type to check
	 * @tparam ...Ts Arguement types
	*/
	template <typename Op, typename... Ts>
	using is_invocable = impl::invocable_impl<Op, std::tuple<Ts...>>;

#ifdef __cpp_inline_variables
	template <typename Op, typename... Ts>
	JCLIB_CONSTEXPR inline bool is_invocable_v = is_invocable<Op, Ts...>::value;
#endif







	namespace impl
	{
		template <typename Op, typename Args, typename = void>
		struct invoke_result_impl;
		
		template <typename Op, typename... Args>
		struct invoke_result_impl <Op, std::tuple<Args...>, enable_if_t<is_invocable<Op, Args...>::value>>
		{
			using type = typename invocable_impl<Op, std::tuple<Args...>>::return_type;
		};

		template <typename Op>
		struct invoke_result_impl <Op, std::tuple<void>, enable_if_t<is_invocable<Op>::value>>
		{
			using type = typename invocable_impl<Op, std::tuple<void>>::return_type;
		};
	};

	template <typename Op, typename... Ts>
	using invoke_result = impl::invoke_result_impl<Op, std::tuple<Ts...>>;
	
	template <typename Op, typename... Ts>
	using invoke_result_t = typename invoke_result<Op, Ts...>::type;






	using std::is_function;

#ifdef __cpp_inline_variables
	using std::is_function_v;
#endif

	template <typename T, typename = void>
	struct is_function_pointer : public false_type {};

	template <typename RetT, typename... Args>
	struct is_function_pointer<RetT(*)(Args...), void> : public true_type {};

	template <typename RetT, typename ScopeT, typename... Args>
	struct is_function_pointer<RetT(ScopeT::*)(Args...), void> : public true_type {};

#ifdef __cpp_inline_variables
	template <typename T>
	JCLIB_CONSTEXPR static bool is_function_pointer_v = is_function_pointer<T>::value;
#endif


	/*
		Member function pointer
	*/

	using std::is_member_function_pointer;
#ifdef __cpp_inline_variables
	using std::is_member_function_pointer_v;
#endif


	/*
		Free function pointer
	*/

	template <typename T>
	struct is_free_function_pointer : bool_constant<is_function_pointer<T>::value && !is_member_function_pointer<T>::value> {};

#ifdef __cpp_inline_variables
	template <typename T>
	JCLIB_CONSTEXPR static bool is_free_function_pointer_v = is_free_function_pointer<T>::value;
#endif

#ifdef __cpp_noexcept_function_type
	template <typename T>
	struct is_noexcept_function : false_type {};
	template <typename RetT, typename... Vals>
	struct is_noexcept_function<RetT(Vals...) noexcept> : true_type {};
#else
	template <typename T>
	struct is_noexcept_function : false_type {};
#endif

#ifdef __cpp_noexcept_function_type
	template <typename T>
	struct add_noexcept;

	template <typename ReturnT, typename... ArgTs>
	struct add_noexcept<ReturnT(ArgTs...)>
	{
		using type = ReturnT(ArgTs...) noexcept;
	};

	template <typename ReturnT, typename... ArgTs>
	struct add_noexcept<ReturnT(*)(ArgTs...)>
	{
		using type = ReturnT(*)(ArgTs...) noexcept;
	};
	template <typename ReturnT, typename ClassT, typename... ArgTs>
	struct add_noexcept<ReturnT(ClassT::*)(ArgTs...)>
	{
		using type = ReturnT(ClassT::*)(ArgTs...) noexcept;
	};

	template <typename T>
	using add_noexcept_t = typename add_noexcept<T>::type;

#else

#endif

#ifdef __cpp_inline_variables
	template <typename T>
	JCLIB_CONSTEXPR static bool is_noexcept_function_v = is_noexcept_function<T>::value;
#endif




	/**
	 * @brief Gets the type returned when dereferencing a given type
	 * @tparam IterT Iterator type or type with derefencing operator
	 * @tparam Enable SFINAE specialization point
	*/
	template <typename IterT, typename Enable = void>
	struct iterator_to;

	/**
	 * @brief Gets the type returned when dereferencing a given type
	 * @tparam IterT Iterator type or type with derefencing operator
	*/
	template <typename IterT> JCLIB_REQUIRES(requires (IterT a) { *a; } )
	struct iterator_to<IterT,
#ifdef __cpp_concepts
		void
#else
		jc::void_t<decltype(*std::declval<IterT>())>
#endif
		>
	{
		using type = std::remove_reference_t<decltype(*std::declval<IterT>())>;
	};
	template <typename IterT>
#ifdef __cpp_concepts
	requires requires () { iterator_to<IterT>{}; }
#endif
	using iterator_to_t = typename iterator_to<IterT>::type;




	template <typename IterT, typename T, typename = void>
	struct is_iterator_to : false_type {};

	template <typename IterT, typename T>
#ifdef __cpp_concepts
	requires requires () { is_iterator_to<IterT, T>{}; }
#endif
	struct is_iterator_to<IterT, T, jc::enable_if_t<is_same<T, iterator_to_t<IterT>>::value, int>> : true_type {};

#ifdef __cpp_inline_variables
	template <typename IterT, typename T>
#ifdef __cpp_concepts
	requires requires () { is_iterator_to<IterT, T>{}; }
#endif
	JCLIB_CONSTEXPR static bool is_iterator_to_v = is_iterator_to<IterT, T>::value;
#endif

	using std::remove_reference;
	using std::remove_reference_t;

	using std::remove_cv;
	using std::remove_cv_t;

	using std::remove_const;
	using std::remove_const_t;

	template <typename T>
	struct remove_cvref
	{
		using type = remove_cv_t<remove_reference_t<T>>;
	};

	template <typename T>
	using remove_cvref_t = typename remove_cvref<T>::type;

	using std::remove_pointer;
	using std::remove_pointer_t;



	/*
		Type conversion related
	*/

	/**
	 * @brief Associates a size with the signed and unsigned integer types with matching size
	*/
	template <size_t Size>
	struct integers_with_size;

	/**
	 * @brief Signed/Unsigned 8 bit integer types
	*/
	template <>
	struct integers_with_size<1>
	{
		using signed_type = int8_t;
		using unsigned_type = uint8_t;

		static_assert(sizeof(signed_type) == 1 && sizeof(unsigned_type) == 1, "");
	};

	/**
	 * @brief Signed/Unsigned 16 bit integer types
	*/
	template <>
	struct integers_with_size<2>
	{
		using signed_type = int16_t;
		using unsigned_type = uint16_t;

		static_assert(sizeof(signed_type) == 2 && sizeof(unsigned_type) == 2, "");
	};

	/**
	 * @brief Signed/Unsigned 16 bit integer types
	*/
	template <>
	struct integers_with_size<4>
	{
		using signed_type = int32_t;
		using unsigned_type = uint32_t;

		static_assert(sizeof(signed_type) == 4 && sizeof(unsigned_type) == 4, "");
	};

	/**
	 * @brief Signed/Unsigned 16 bit integer types
	*/
	template <>
	struct integers_with_size<8>
	{
		using signed_type = int64_t;
		using unsigned_type = uint64_t;

		static_assert(sizeof(signed_type) == 8 && sizeof(unsigned_type) == 8, "");
	};


	template <typename T, typename Enable = void>
	struct signed_equivalent;

	/**
	 * @brief Provides an equivalent sized integer type
	 * @tparam T Type to find equivalent sized integer
	*/
	template <typename T>
	struct signed_equivalent<T,
		void_t<typename integers_with_size<sizeof(T)>::signed_type>>
	{
		using type = typename integers_with_size<sizeof(T)>::signed_type;
	};

	/**
	 * @brief Provides an equivalent sized integer type
	 * @tparam T Type to find equivalent sized integer
	*/
	template <typename T>
	using signed_equivalent_t = typename signed_equivalent<T>::type;



	template <typename T, typename Enable = void>
	struct unsigned_equivalent;

	/**
	 * @brief Provides an equivalent unsized integer type
	 * @tparam T Type to find equivalent unsized integer
	*/
	template <typename T>
	struct unsigned_equivalent<T,
		void_t<typename integers_with_size<sizeof(T)>::unsigned_type>>
	{
		using type = typename integers_with_size<sizeof(T)>::unsigned_type;
	};

	/**
	 * @brief Provides an equivalent unsized integer type
	 * @tparam T Type to find equivalent unsized integer
	*/
	template <typename T>
	using unsigned_equivalent_t = typename unsigned_equivalent<T>::type;


	/**
	 * @brief Converts an enumerator value into its underlying type equivalent
	 * @tparam EnumT Enumerator type
	 * @param _val Enumerator value
	 * @return Underling equivalent
	*/
	template <typename EnumT>
	constexpr inline auto to_underlying(EnumT _val) noexcept ->
		std::underlying_type_t<EnumT>
	{
		return static_cast<std::underlying_type_t<EnumT>>(_val);
	};
};

#endif