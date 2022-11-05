#pragma once
#ifndef JCLIB_UNIQUE_H
#define JCLIB_UNIQUE_H

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

/**
 * @file
 * 
 * Provides some tooling for managing unique owned values that have a "cleanup" function that must be executed on destruction.
 * 
 * Essentially a revamp of the guard type.
*/

#include <jclib/config.h>
#include <jclib/feature.h>
#include <jclib/concepts.h>

#include <cstdio>

#define _JCLIB_UNIQUE_

namespace jc
{
#ifdef JCLIB_FEATURE_CONCEPTS

#endif


	/**
	 * @brief Generic owning handle to a uniquely owned value (think `unique_ptr` but can be used for non-pointer types).
	 * 
	 * @tparam T The type this will own.
	 * @tparam TraitsT The traits type implementing functionality for manipulating the owned value type `T`. See the example
	 * at the bottom of the file for more information (TODO: Add example to proper docs). 
	*/
	template <typename T, typename TraitsT>
	struct unique_value
	{
	public:

		/**
		 * @brief The type of value this owns.
		*/
		using value_type = T;

		/**
		 * @brief The traits type that implements manipulation functionality for the owned object.
		 *
		 * See the (commented out) example type `example_unique_value_traits` for an example of
		 * and documentation for how to write the traits type.
		*/
		using traits_type = TraitsT;

		/**
		 * @brief If not null, runs the cleanup function and sets to null; otherwise, this does nothing.
		*/
		constexpr void reset()
		{
			// Grab ownership of the owned value from ourselves.
			auto _value = this->release();

			// Invoke `traits_type::reset` if the value is not null.
			if (traits_type::good(_value))
			{
				traits_type::reset(std::move(_value));
			};
		};
		
		/**
		 * @brief Releases ownership of the held value, returning its original value to the caller.
		 * @return The (now no longer) owned value.
		*/
		constexpr JCLIB_NODISCARD("Owning and potentially non-null instance of `value_type`") value_type release()
		{
			// Move owned value to a temporary.
			auto o = std::move(this->value_);
			
			// Null out the "owned" value as we are transferring ownership to the caller.
			this->value_ = traits_type::null();

			// Return the copy of our no longer owned value.
			return o;
		};

		/**
		 * @brief Gets a copy of or handle to the held value.
		*/
		constexpr value_type get() const &
			noexcept(noexcept(value_type(std::declval<const value_type&>())))
		{
			return this->value_;
		};

		// Deleted to prevent accidental access to the owned value after move.
		// There is no situation where get() should be called on an rvalue so this should be never be hit unless the caller is incorrect.
		value_type get() && = delete;

		/**
		 * @brief Checks if this is NOT null.
		 * @return True if NOT null, false if null.
		*/
		constexpr bool good() const
			noexcept(noexcept(traits_type::good(std::declval<const value_type&>())))
		{
			return traits_type::good(this->value_);
		};

		/**
		 * @brief Checks if this is NOT null.
		 * @return True if NOT null, false if null.
		*/
		constexpr explicit operator bool() const
			noexcept(noexcept(std::declval<unique_value>().good()))
		{
			return this->good();
		};

		/**
		 * @brief Constructs a null unique value.
		*/
		constexpr unique_value()
			noexcept(noexcept(traits_type::null())) :
			value_(traits_type::null())
		{};

		/**
		 * @brief Obtains ownership of an already constructed value.
		 * @param _value The value to obtain ownership of, may be null.
		*/
		constexpr explicit unique_value(value_type&& _value)
			noexcept(noexcept(value_type(std::move(std::declval<value_type&&>())))) :
			value_(std::move(_value))
		{};

		/**
		 * @brief Obtains ownership from an already constructed `unique_value`.
		 * @param other The `unique_value` to transfer ownership from and into the constructed object.
		*/
		constexpr unique_value(unique_value&& other)
			noexcept(noexcept(std::declval<unique_value>().release())) :
			value_(other.release())
		{};

		/**
		 * @brief Resets the object and obtains ownership from an already constructed `unique_value`.
		 * @param other The `unique_value` to transfer ownership from and into this object.
		 * @return Reference to `this` (the constructed object).
		*/
		constexpr unique_value& operator=(unique_value&& other)
			noexcept(noexcept(std::declval<unique_value>().reset()) && noexcept(std::declval<unique_value>().release()))
		{
			// Only preform a move if the `other` object is NOT this object.
			if (this != &other)
			{
				this->reset();
				this->value_ = other.release();
			};
			return *this;
		};

		/**
		 * @brief If not null, runs the cleanup function and sets to null; otherwise, this does nothing.
		*/
		JCLIB_DESTRUCTOR_CONSTEXPR ~unique_value()
			noexcept(noexcept(std::declval<unique_value>().reset()))
		{
			this->reset();
		};

	private:
		
		/**
		 * @brief The actual owned value.
		*/
		value_type value_;

		// No copy as this is owning a unique value.
		unique_value(const unique_value& other) = delete;
		unique_value& operator=(const unique_value& other) = delete;
	};
};

#pragma region UNIQUE_VALUE_TRAITS_EXAMPLE
#if 0
/**
 * @brief An example type and more detailed documentation for the traits type used by the `unique_value` type's `TraitsT` template parameter.
*/
struct example_unique_value_traits
{
	/**
	 * @brief The actual type that is owned. In this case we are managing a C file handle.
	*/
	using value_type = FILE*;

	/**
	 * @brief Automatic cleanup function to be invoked on destruction.
	 *
	 * For the purpose of this example, we will invoke the `fclose` function to "close" the file handle.
	 *
	 * If this function can fail, throw an exception. In the future I may add support for returning error codes
	 * but this presents the problem of what to do if a call to `reset` fails during a `unique_value` object's destructor.
	 *
	 * @param _value Value to be reset. This will never be null.
	*/
	static void reset(value_type&& _value)
	{
		fclose(_value);
	};

	/**
	 * @brief Constructs a null instance of `value_type`.
	 *
	 * In this example we can just return `NULL`.
	 *
	 * @return Null value.
	*/
	constexpr static value_type null() noexcept
	{
		return NULL;
	};

	/**
	 * @brief Checks if an instance of `value_type` is NOT null.
	 *
	 * Here we can just check if the file handle is NOT equal to `NULL` as that is our null state.
	 *
	 * @param _value Instance of `value_type` to check.
	 * @return True if `_value` is NOT null, false if `_value` is null.
	*/
	constexpr static bool good(const value_type& _value) noexcept
	{
		return _value != NULL;
	};
};
#endif
#pragma endregion 

#endif