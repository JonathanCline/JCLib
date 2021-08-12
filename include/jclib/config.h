#pragma once
#ifndef JCLIB_CONFIG_H
#define JCLIB_CONFIG_H

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

#include <jclib/config/version.h>

#define _JCLIB_CONFIG_

#ifdef __cpp_constexpr
#define JCLIB_CONSTEXPR constexpr
#else
#define JCLIB_CONSTEXPR 
#endif


#ifdef __cpp_constexpr
#define JCLIB_CONSTANT constexpr
#else
#define JCLIB_CONSTANT const
#endif

namespace jc
{
	namespace impl
	{
		/**
		 * @brief Represents a version as a set of major, minor, and patch numbers
		*/
		struct version
		{
		private:
			JCLIB_CONSTEXPR static unsigned fold(const version& _v) noexcept
			{
				return ((unsigned)_v.major << 16u) | ((unsigned)_v.minor << 8u) | ((unsigned)_v.patch);
			};

		public:
			friend JCLIB_CONSTEXPR inline bool operator==(const version& lhs, const version& rhs) noexcept
			{
				return (lhs.major == rhs.major && lhs.minor == rhs.minor && lhs.patch == rhs.patch);
			};
			friend JCLIB_CONSTEXPR inline bool operator>(const version& lhs, const version& rhs) noexcept
			{
				return fold(lhs) > fold(rhs);
			};

			unsigned char major = 0;
			unsigned char minor = 0;
			unsigned char patch = 0;
		};

		/**
		 * @brief jclib version
		*/
		constexpr static version LIBRARY_VERSION{ JCLIB_VERSION_MAJOR, JCLIB_VERSION_MINOR, JCLIB_VERSION_PATCH };
	};
};

#ifndef JCLIB_ABORT
#include <exception>
#define JCLIB_ABORT(...) std::terminate()
#endif

#if defined(JCLIB_DEBUG_MODE) || !defined(NDEBUG)
#define JCLIB_DEBUG true
#endif

#ifndef JCLIB_ASSERT
#ifdef JCLIB_DEBUG
#define JCLIB_ASSERT(cond) if ( ! ( cond ) ) { JCLIB_ABORT(); }
#else
#define JCLIB_ASSERT(cond) {}
#endif
#endif

#ifndef JCLIB_NODISCARD
#if __has_cpp_attribute(nodiscard)
#define JCLIB_NODISCARD(reason) [[nodiscard(reason)]]
#else
#define JCLIB_NODISCARD()
#endif
#endif

#ifndef JCLIB_EMPTY
#if __has_cpp_attribute(no_unique_address)
#define JCLIB_EMPTY [[no_unique_address]]
#else
#define JCLIB_EMPTY
#endif
#endif

// Used to annotate that a function is now deprecated and will shortly be removed
#ifndef JCLIB_DEPRECATED
#if __has_cpp_attribute(deprecated)
#define JCLIB_DEPRECATED(reason) [[deprecated(reason)]]
#else
#define JCLIB_DEPRECATED(reason) 
#endif
#endif

// Marks a region as deprecated and prevents it from being compiled unless JCLIB_ALLOW_DEPRECATED is defined
#ifndef JCLIB_DEPRECATE_BLOCK
#if defined(JCLIB_ALLOW_DEPRECATED)
#define JCLIB_DEPRECATE_BLOCK(code) code
#else
#define JCLIB_DEPRECATE_BLOCK(code) static_assert(true, "")
#endif
#endif

// Check that versioning info is present
#if !(defined(JCLIB_VERSION_MAJOR) && defined(JCLIB_VERSION_MINOR) && defined(JCLIB_VERSION_PATCH)) && !defined(JCLIB_VERSION)
#error missing versioning info
#endif

// Full 3-part library version as "MAJOR MINOR PATCH" (seperated by spaces)
#ifndef JCLIB_VERSION
#define JCLIB_VERSION JCLIB_VERSION_MAJOR JCLIB_VERSION_MINOR JCLIB_VERSION_PATCH
#endif

// Number containing the library version, should be fine for use in comparisons
#ifndef JCLIB_VERSION_N
#define JCLIB_VERSION_N ((JCLIB_VERSION_MAJOR << 16) | (JCLIB_VERSION_MINOR << 8) | (JCLIB_VERSION_PATCH))
#endif

// Convenience macro for c++20 requires clauses
#ifndef JCLIB_REQUIRES
#ifdef __cpp_concepts
#define JCLIB_REQUIRES(x) requires x
#else
#define JCLIB_REQUIRES(x)
#endif
#endif

// Likely attribute for C++20 (+?)
#ifndef JCLIB_LIKELY
#if __has_cpp_attribute(likely)
#define JCLIB_LIKELY [[likely]]
#else
#define JCLIB_LIKELY
#endif
#endif

// Unlikely attribute for C++20 (+?)
#ifndef JCLIB_UNLIKELY
#if __has_cpp_attribute(unlikely)
#define JCLIB_UNLIKELY [[unlikely]]
#else
#define JCLIB_UNLIKELY
#endif
#endif

// consteval qualifier, defaults to constexpr if consteval isnt available
#ifndef JCLIB_CONSTEVAL
#if __has_cpp_attribute(likely)
#define JCLIB_CONSTEVAL consteval
#else
#define JCLIB_CONSTEVAL JCLIB_CONSTEXPR
#endif
#endif

#ifdef JCLIB_NO_EXCEPTIONS
// True/False depending on if exceptions are allowed
#define JCLIB_EXCEPTIONS false
#else
// True/False depending on if exceptions are allowed
#define JCLIB_EXCEPTIONS true
#endif

namespace jc
{
	/**
	 * @brief True if exceptions are enabled, false otherwise
	*/
	constexpr extern bool exceptions_v = JCLIB_EXCEPTIONS;
};

#endif