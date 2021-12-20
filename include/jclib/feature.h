// generated using codegen/feature_gen.lua
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


/*
    Test for __cpp_inline_variables
*/

#define JCLIB_FEATURE_VALUE_INLINE_VARIABLES 201606L
#if JCLIB_CPP >= JCLIB_FEATURE_VALUE_INLINE_VARIABLES || __cpp_inline_variables >= JCLIB_FEATURE_VALUE_INLINE_VARIABLES
    #define JCLIB_FEATURE_INLINE_VARIABLES
#else
    #ifdef JCLIB_FEATURE_INLINE_VARIABLES
        #error "Feature testing macro was defined when it shouldn't be"
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
#if JCLIB_CPP >= JCLIB_FEATURE_VALUE_CONCEPTS || __cpp_concepts >= JCLIB_FEATURE_VALUE_CONCEPTS
    #define JCLIB_FEATURE_CONCEPTS
#else
    #ifdef JCLIB_FEATURE_CONCEPTS
        #error "Feature testing macro was defined when it shouldn't be"
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
#if JCLIB_CPP >= JCLIB_FEATURE_VALUE_CONSTEXPR_DYNAMIC_ALLOC || __cpp_lib_constexpr_dynamic_alloc >= JCLIB_FEATURE_VALUE_CONSTEXPR_DYNAMIC_ALLOC
    #define JCLIB_FEATURE_CONSTEXPR_DYNAMIC_ALLOC
#else
    #ifdef JCLIB_FEATURE_CONSTEXPR_DYNAMIC_ALLOC
        #error "Feature testing macro was defined when it shouldn't be"
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
#if JCLIB_CPP >= JCLIB_FEATURE_VALUE_THREE_WAY_COMPARISON || __cpp_lib_three_way_comparison >= JCLIB_FEATURE_VALUE_THREE_WAY_COMPARISON
    #define JCLIB_FEATURE_THREE_WAY_COMPARISON
#else
    #ifdef JCLIB_FEATURE_THREE_WAY_COMPARISON
        #error "Feature testing macro was defined when it shouldn't be"
    #endif
#endif

#ifdef JCLIB_FEATURE_THREE_WAY_COMPARISON
    #define JCLIB_FEATURE_THREE_WAY_COMPARISON_V true
#else
    #define JCLIB_FEATURE_THREE_WAY_COMPARISON_V false
#endif


/*
    Test for __cpp_conteval
*/

#define JCLIB_FEATURE_VALUE_CONSTEVAL 201811L
#if JCLIB_CPP >= JCLIB_FEATURE_VALUE_CONSTEVAL || __cpp_conteval >= JCLIB_FEATURE_VALUE_CONSTEVAL
    #define JCLIB_FEATURE_CONSTEVAL
#else
    #ifdef JCLIB_FEATURE_CONSTEVAL
        #error "Feature testing macro was defined when it shouldn't be"
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

#define JCLIB_FEATURE_VALUE_IF_CONSTEXPR 201606L
#if JCLIB_CPP >= JCLIB_FEATURE_VALUE_IF_CONSTEXPR || __cpp_if_constexpr >= JCLIB_FEATURE_VALUE_IF_CONSTEXPR
    #define JCLIB_FEATURE_IF_CONSTEXPR
#else
    #ifdef JCLIB_FEATURE_IF_CONSTEXPR
        #error "Feature testing macro was defined when it shouldn't be"
    #endif
#endif

#ifdef JCLIB_FEATURE_IF_CONSTEXPR
    #define JCLIB_FEATURE_IF_CONSTEXPR_V true
#else
    #define JCLIB_FEATURE_IF_CONSTEXPR_V false
#endif


/*
    Test for __cpp_if_consteval
*/

#define JCLIB_FEATURE_VALUE_IF_CONSTEVAL 202106L
#if JCLIB_CPP >= JCLIB_FEATURE_VALUE_IF_CONSTEVAL || __cpp_if_consteval >= JCLIB_FEATURE_VALUE_IF_CONSTEVAL
    #define JCLIB_FEATURE_IF_CONSTEVAL
#else
    #ifdef JCLIB_FEATURE_IF_CONSTEVAL
        #error "Feature testing macro was defined when it shouldn't be"
    #endif
#endif

#ifdef JCLIB_FEATURE_IF_CONSTEVAL
    #define JCLIB_FEATURE_IF_CONSTEVAL_V true
#else
    #define JCLIB_FEATURE_IF_CONSTEVAL_V false
#endif


/*
    Test for __cpp_lib_constexpr_algorithms
*/

#define JCLIB_FEATURE_VALUE_CPP_CONSTEXPR_ALGORITHMS 201806L
#if JCLIB_CPP >= JCLIB_FEATURE_VALUE_CPP_CONSTEXPR_ALGORITHMS || __cpp_lib_constexpr_algorithms >= JCLIB_FEATURE_VALUE_CPP_CONSTEXPR_ALGORITHMS
    #define JCLIB_FEATURE_CPP_CONSTEXPR_ALGORITHMS
#else
    #ifdef JCLIB_FEATURE_CPP_CONSTEXPR_ALGORITHMS
        #error "Feature testing macro was defined when it shouldn't be"
    #endif
#endif

#ifdef JCLIB_FEATURE_CPP_CONSTEXPR_ALGORITHMS
    #define JCLIB_FEATURE_CPP_CONSTEXPR_ALGORITHMS_V true
#else
    #define JCLIB_FEATURE_CPP_CONSTEXPR_ALGORITHMS_V false
#endif


/*
    Test for __cpp_lib_span
*/

#define JCLIB_FEATURE_VALUE_CPP_SPAN 202002L
#if JCLIB_CPP >= JCLIB_FEATURE_VALUE_CPP_SPAN || __cpp_lib_span >= JCLIB_FEATURE_VALUE_CPP_SPAN
    #define JCLIB_FEATURE_CPP_SPAN
#else
    #ifdef JCLIB_FEATURE_CPP_SPAN
        #error "Feature testing macro was defined when it shouldn't be"
    #endif
#endif

#ifdef JCLIB_FEATURE_CPP_SPAN
    #define JCLIB_FEATURE_CPP_SPAN_V true
#else
    #define JCLIB_FEATURE_CPP_SPAN_V false
#endif


/*
    Test for __cpp_noexcept_function_type
*/

#define JCLIB_FEATURE_VALUE_NOEXCEPT_FUNCTION_TYPE 201510L
#if JCLIB_CPP >= JCLIB_FEATURE_VALUE_NOEXCEPT_FUNCTION_TYPE || __cpp_noexcept_function_type >= JCLIB_FEATURE_VALUE_NOEXCEPT_FUNCTION_TYPE
    #define JCLIB_FEATURE_NOEXCEPT_FUNCTION_TYPE
#else
    #ifdef JCLIB_FEATURE_NOEXCEPT_FUNCTION_TYPE
        #error "Feature testing macro was defined when it shouldn't be"
    #endif
#endif

#ifdef JCLIB_FEATURE_NOEXCEPT_FUNCTION_TYPE
    #define JCLIB_FEATURE_NOEXCEPT_FUNCTION_TYPE_V true
#else
    #define JCLIB_FEATURE_NOEXCEPT_FUNCTION_TYPE_V false
#endif


/*
    Test for __cpp_char8_t
*/

#define JCLIB_FEATURE_VALUE_CHAR8 201811L
#if JCLIB_CPP >= JCLIB_FEATURE_VALUE_CHAR8 || __cpp_char8_t >= JCLIB_FEATURE_VALUE_CHAR8
    #define JCLIB_FEATURE_CHAR8
#else
    #ifdef JCLIB_FEATURE_CHAR8
        #error "Feature testing macro was defined when it shouldn't be"
    #endif
#endif

#ifdef JCLIB_FEATURE_CHAR8
    #define JCLIB_FEATURE_CHAR8_V true
#else
    #define JCLIB_FEATURE_CHAR8_V false
#endif


/*
    Test for __cpp_fold_expressions
*/

#define JCLIB_FEATURE_VALUE_FOLD_EXPRESSIONS 201603L
#if JCLIB_CPP >= JCLIB_FEATURE_VALUE_FOLD_EXPRESSIONS || __cpp_fold_expressions >= JCLIB_FEATURE_VALUE_FOLD_EXPRESSIONS
    #define JCLIB_FEATURE_FOLD_EXPRESSIONS
#else
    #ifdef JCLIB_FEATURE_FOLD_EXPRESSIONS
        #error "Feature testing macro was defined when it shouldn't be"
    #endif
#endif

#ifdef JCLIB_FEATURE_FOLD_EXPRESSIONS
    #define JCLIB_FEATURE_FOLD_EXPRESSIONS_V true
#else
    #define JCLIB_FEATURE_FOLD_EXPRESSIONS_V false
#endif


/*
    Test for __cpp_lib_bool_constant
*/

#define JCLIB_FEATURE_VALUE_BOOL_CONSTANT 201505L
#if JCLIB_CPP >= JCLIB_FEATURE_VALUE_BOOL_CONSTANT || __cpp_lib_bool_constant >= JCLIB_FEATURE_VALUE_BOOL_CONSTANT
    #define JCLIB_FEATURE_BOOL_CONSTANT
#else
    #ifdef JCLIB_FEATURE_BOOL_CONSTANT
        #error "Feature testing macro was defined when it shouldn't be"
    #endif
#endif

#ifdef JCLIB_FEATURE_BOOL_CONSTANT
    #define JCLIB_FEATURE_BOOL_CONSTANT_V true
#else
    #define JCLIB_FEATURE_BOOL_CONSTANT_V false
#endif


/*
    Test for __cpp_lib_optional
*/

#define JCLIB_FEATURE_VALUE_OPTIONAL 201606L
#if JCLIB_CPP >= JCLIB_FEATURE_VALUE_OPTIONAL || __cpp_lib_optional >= JCLIB_FEATURE_VALUE_OPTIONAL
    #define JCLIB_FEATURE_OPTIONAL
#else
    #ifdef JCLIB_FEATURE_OPTIONAL
        #error "Feature testing macro was defined when it shouldn't be"
    #endif
#endif

#ifdef JCLIB_FEATURE_OPTIONAL
    #define JCLIB_FEATURE_OPTIONAL_V true
#else
    #define JCLIB_FEATURE_OPTIONAL_V false
#endif


/*
    Test for __cpp_lib_span
*/

#define JCLIB_FEATURE_VALUE_SPAN 202002L
#if JCLIB_CPP >= JCLIB_FEATURE_VALUE_SPAN || __cpp_lib_span >= JCLIB_FEATURE_VALUE_SPAN
    #define JCLIB_FEATURE_SPAN
#else
    #ifdef JCLIB_FEATURE_SPAN
        #error "Feature testing macro was defined when it shouldn't be"
    #endif
#endif

#ifdef JCLIB_FEATURE_SPAN
    #define JCLIB_FEATURE_SPAN_V true
#else
    #define JCLIB_FEATURE_SPAN_V false
#endif


/*
    Test for __cpp_lib_byte
*/

#define JCLIB_FEATURE_VALUE_BYTE 201603L
#if JCLIB_CPP >= JCLIB_FEATURE_VALUE_BYTE || __cpp_lib_byte >= JCLIB_FEATURE_VALUE_BYTE
    #define JCLIB_FEATURE_BYTE
#else
    #ifdef JCLIB_FEATURE_BYTE
        #error "Feature testing macro was defined when it shouldn't be"
    #endif
#endif

#ifdef JCLIB_FEATURE_BYTE
    #define JCLIB_FEATURE_BYTE_V true
#else
    #define JCLIB_FEATURE_BYTE_V false
#endif

    
#endif
