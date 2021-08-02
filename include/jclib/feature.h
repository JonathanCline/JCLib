#pragma once
#ifndef JCLIB_FEATURE_H
#define JCLIB_FEATURE_H

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
		JCLib feature c++ testing encapsulation header. This exists partially because certain feature c++ testing macros
	are imply different features available depending on their value.
*/

#define _JCLIB_FEATURE_

#define JCLIB_CPP __cplusplus

// c++14 = 201402L
// c++17 = 201703L
// c++20 = 202002L



/*
	Test for __cpp_inline_variables
*/

#define JCLIB_FEATURE_VALUE_INLINE_VARIABLES 201606L
#if defined(__cpp_inline_variables) && JCLIB_FEATURE_VALUE_INLINE_VARIABLES != __cpp_inline_variables
#error Feature testing value mismatch
#endif
#if JCLIB_CPP >= JCLIB_FEATURE_VALUE_INLINE_VARIABLES || __cpp_inline_variables >= JCLIB_FEATURE_VALUE_INLINE_VARIABLES
#define JCLIB_FEATURE_INLINE_VARIABLES
#else
#ifdef JCLIB_FEATURE_INLINE_VARIABLES
#error Feature testing macro was defined when it shouldn't be
#endif
#endif

#ifdef JCLIB_FEATURE_INLINE_VARIABLES
#define JCLIB_FEATURE_INLINE_VARIABLES_V true
#else
#define JCLIB_FEATURE_INLINE_VARIABLES_V false
#endif



/*
	Test for __cpp_concepts
*/

#define JCLIB_FEATURE_VALUE_CONCEPTS 201907L
#if defined(__cpp_concepts) && JCLIB_FEATURE_VALUE_CONCEPTS != __cpp_concepts
#error Feature testing value mismatch
#endif
#if JCLIB_CPP >= JCLIB_FEATURE_VALUE_CONCEPTS || __cpp_concepts >= JCLIB_FEATURE_VALUE_CONCEPTS
#define JCLIB_FEATURE_CONCEPTS
#else
#ifdef JCLIB_FEATURE_CONCEPTS
#error Feature testing macro was defined when it shouldn't be
#endif
#endif

#ifdef JCLIB_FEATURE_CONCEPTS
#define JCLIB_FEATURE_CONCEPTS_V true
#else
#define JCLIB_FEATURE_CONCEPTS_V false
#endif



/*
	Test for __cpp_lib_constexpr_dynamic_alloc
*/

#define JCLIB_FEATURE_VALUE_CONSTEXPR_DYNAMIC_ALLOC 201907L
#if defined(__cpp_lib_constexpr_dynamic_alloc) && JCLIB_FEATURE_VALUE_CONSTEXPR_DYNAMIC_ALLOC != __cpp_lib_constexpr_dynamic_alloc
#error Feature testing value mismatch
#endif
#if JCLIB_CPP >= JCLIB_FEATURE_VALUE_CONSTEXPR_DYNAMIC_ALLOC || __cpp_lib_constexpr_dynamic_alloc >= JCLIB_FEATURE_VALUE_CONSTEXPR_DYNAMIC_ALLOC
#define JCLIB_FEATURE_CONSTEXPR_DYNAMIC_ALLOC
#else
#ifdef JCLIB_FEATURE_CONSTEXPR_DYNAMIC_ALLOC
#error Feature testing macro was defined when it shouldn't be
#endif
#endif

#ifdef JCLIB_FEATURE_CONSTEXPR_DYNAMIC_ALLOC
#define JCLIB_FEATURE_CONSTEXPR_DYNAMIC_ALLOC_V true
#else
#define JCLIB_FEATURE_CONSTEXPR_DYNAMIC_ALLOC_V false
#endif



/*
	Test for __cpp_lib_three_way_comparison
*/

#define JCLIB_FEATURE_VALUE_THREE_WAY_COMPARISON 201907L
#if defined(__cpp_lib_three_way_comparison) && JCLIB_FEATURE_VALUE_THREE_WAY_COMPARISON != __cpp_lib_three_way_comparison
#error Feature testing value mismatch
#endif
#if JCLIB_CPP >= JCLIB_FEATURE_VALUE_THREE_WAY_COMPARISON || __cpp_lib_three_way_comparison >= JCLIB_FEATURE_VALUE_THREE_WAY_COMPARISON
#define JCLIB_FEATURE_THREE_WAY_COMPARISON
#else
#ifdef JCLIB_FEATURE_THREE_WAY_COMPARISON
#error Feature testing macro was defined when it shouldn't be
#endif
#endif

#ifdef JCLIB_FEATURE_THREE_WAY_COMPARISON
#define JCLIB_FEATURE_THREE_WAY_COMPARISON_V true
#else
#define JCLIB_FEATURE_THREE_WAY_COMPARISON_V false
#endif




/*
	Test for __cpp_consteval
*/

#define JCLIB_FEATURE_VALUE_CONSTEVAL 201811L
#if defined(__cpp_consteval) && JCLIB_FEATURE_VALUE_CONSTEVAL != __cpp_consteval
#error Feature testing value mismatch
#endif
#if JCLIB_CPP >= JCLIB_FEATURE_VALUE_CONSTEVAL || __cpp_consteval >= JCLIB_FEATURE_VALUE_CONSTEVAL
#define JCLIB_FEATURE_CONSTEVAL
#else
#ifdef JCLIB_FEATURE_CONSTEVAL
#error Feature testing macro was defined when it shouldn't be
#endif
#endif

#ifdef JCLIB_FEATURE_CONSTEVAL
#define JCLIB_FEATURE_CONSTEVAL_V true
#else
#define JCLIB_FEATURE_CONSTEVAL_V false
#endif




/*
	Test for __cpp_if_constexpr
*/

#define JCLIB_FEATURE_VALUE_CONSTEVAL 201811L
#if defined(__cpp_consteval) && JCLIB_FEATURE_VALUE_CONSTEVAL != __cpp_consteval
#error Feature testing value mismatch
#endif
#if JCLIB_CPP >= JCLIB_FEATURE_VALUE_CONSTEVAL || __cpp_consteval >= JCLIB_FEATURE_VALUE_CONSTEVAL
#define JCLIB_FEATURE_CONSTEVAL
#else
#ifdef JCLIB_FEATURE_CONSTEVAL
#error Feature testing macro was defined when it shouldn't be
#endif
#endif

#ifdef JCLIB_FEATURE_CONSTEVAL
#define JCLIB_FEATURE_CONSTEVAL_V true
#else
#define JCLIB_FEATURE_CONSTEVAL_V false
#endif



#endif