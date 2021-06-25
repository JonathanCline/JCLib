#pragma once
#ifndef JCLIB_OPTIONAL_H
#define JCLIB_OPTIONAL_H

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

#include <jclib/config.h>

#define _JCLIB_OPTIONAL_

namespace jc
{
	/**
	 * @brief Value type held by an empty optional
	*/
	struct nullopt_t {};

	/**
	 * @brief Value held by an empty optional
	*/
	constexpr static nullopt_t nullopt{};

	/**
	 * @brief Contains either the specified type or nullopt, should be nearly identical to std::optional
	 * @tparam T Optionally held value type
	*/
	template <typename T>
	struct optional
	{
	public:
		using value_type = T;

		constexpr bool has_value() const noexcept
		{
			return this->hasval_;
		};
		constexpr explicit operator bool() const noexcept
		{
			return this->has_value();
		};
		
		constexpr value_type& value()
		{
			JCLIB_ASSERT(this->has_value());
			return this->val_;
		};
		constexpr const value_type& value() const
		{
			JCLIB_ASSERT(this->has_value());
			return this->val_;
		};

		constexpr value_type& operator*()
		{
			return this->value();
		};
		constexpr const value_type& operator*() const
		{
			return this->value();
		};

		constexpr value_type* operator->()
		{
			return &this->value();
		};
		constexpr const value_type* operator->() const
		{
			return &this->value();
		};

	private:
		union
		{
			T val_;
			nullopt_t nullval_{ nullopt };
		};
		bool hasval_ = false;
	};

};

#endif