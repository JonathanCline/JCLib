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

#define _JCLIB_TYPE_TRAITS_

#include <type_traits>
#include <limits>
#include <utility>

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
	using std::disjunction;

#ifdef __cpp_inline_variables
	using std::is_same_v;
	using std::is_convertible_v;
	using std::disjunction_v;
#endif

	template <typename T, typename... Ts>
	struct is_any_of :
#ifdef __cpp_fold_expressions
		bool_constant<(std::is_same_v<T, Ts> || ...)>
#else
		disjunction<std::is_same<T, Ts>, Ts...>
#endif
	{};

#ifdef __cpp_inline_variables
	template <typename T, typename... Ts>
	JCLIB_CONSTANT static inline bool is_any_of_v = is_any_of<T, Ts...>::value;
#endif

	/*
		Character type traits
	*/


	template <typename T>
	struct is_character : public is_any_of<T,
		unsigned char,
		char,
		wchar_t,
#ifdef __cpp_char8_t
		char8_t,
#endif
		char16_t,
		char32_t
	> {};

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

#ifdef __cpp_inline_variables
	using std::is_fundamental_v;
	using std::is_integral_v;
	using std::is_arithmetic_v;
	using std::is_floating_point_v;
	using std::is_signed_v;
	using std::is_unsigned_v;
#endif

	template <typename T>
	struct is_integer : public bool_constant<is_integer<T>::value && std::numeric_limits<T>::is_integer> {};
	
#ifdef __cpp_inline_variables
	template <typename T>
	JCLIB_CONSTEXPR inline bool is_integer_v = is_integer<T>::value;
#endif

	/*
		Constructor related
	*/

	using std::is_constructible;
	using std::is_copy_constructible;
	using std::is_copy_assignable;
	using std::is_move_constructible;
	using std::is_move_assignable;
	using std::is_default_constructible;

#ifdef __cpp_inline_variables
	using std::is_constructible_v;
	using std::is_copy_constructible_v;
	using std::is_copy_assignable_v;
	using std::is_move_constructible_v;
	using std::is_move_assignable_v;
	using std::is_default_constructible_v;
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

#ifdef __cpp_inline_variables
	using std::is_trivially_default_constructible_v;
	using std::is_trivially_copy_constructible_v;
	using std::is_trivially_copy_assignable_v;
	using std::is_trivially_copyable_v;
	using std::is_trivially_move_constructible_v;
	using std::is_trivially_move_assignable_v;
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

	template <typename From, typename To>
	struct is_forwardable_to : public bool_constant<is_constructible<From, std::remove_reference_t<To>>::value> {};

#ifdef __cpp_inline_variables
	template <typename From, typename To>
	JCLIB_CONSTEXPR inline bool is_forwardable_to_v = is_forwardable_to<From, To>::value;
#endif
	

	/*
		Function related
	*/

	namespace impl
	{
		template <typename Op, typename Tup, typename = void>
		struct invocable_impl : false_type {};

		// generic function case
		template <typename Op, typename... ArgTs>
		struct invocable_impl <Op, std::tuple<ArgTs...>,
			void_t<decltype(std::declval<Op>()(std::declval<ArgTs>()...))>
		> : true_type {};

		// needed to handle case where the function has no arguements
		template <typename Op>
		struct invocable_impl <Op, std::tuple<void>,
			void_t<decltype(std::declval<Op>()())>
		> : true_type {};

		// free function case, should be handled by the general case but im leaving this in for now
		template <typename ReturnT, typename... ArgTs>
		struct invocable_impl<ReturnT(*)(ArgTs...), std::tuple<ArgTs...>,
			void_t<decltype(
				(*std::declval<ReturnT(*)(ArgTs...)>())(std::declval<ArgTs>()...)
				)>
		> : true_type {};

		// needed to handle case where the function has no arguements
		template <typename ReturnT>
		struct invocable_impl<ReturnT(*)(), std::tuple<void>,
			void_t<decltype((*std::declval<ReturnT(*)()>())())>
		> : true_type {};

		// member function case
		template <typename ReturnT, typename ClassT, typename... ArgTs>
		struct invocable_impl<ReturnT(ClassT::*)(ArgTs...), std::tuple<ArgTs...>,
			void_t<decltype(
				(std::declval<ClassT*>()->*std::declval<ReturnT(ClassT::*)(ArgTs...)>())(std::declval<ArgTs>()...)
				)>
		> : true_type {};

		// needed to handle case where the function has no arguements
		template <typename ReturnT, typename ClassT>
		struct invocable_impl<ReturnT(ClassT::*)(), std::tuple<void>,
			void_t<decltype((std::declval<ClassT*>()->*std::declval<ReturnT(ClassT::*)()>())())>
		> : true_type {};

	};

	template <typename Op, typename... Ts>
	struct is_invocable : impl::invocable_impl<Op, std::tuple<Ts...>> {};

#ifdef __cpp_inline_variables
	template <typename Op, typename... Ts>
	JCLIB_CONSTEXPR inline bool is_invocable_v = is_invocable<Op, Ts...>::value;
#endif

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

	using std::enable_if;
	using std::enable_if_t;

	template <typename IterT>
#ifdef __cpp_concepts
	requires requires (IterT& a) { *a; }
#endif
	struct iterator_to
	{
		using type = std::remove_reference_t<decltype(*std::declval<IterT&>())>;
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

};

#endif