#pragma once
#ifndef JCLIB_EXCEPTION_H
#define JCLIB_EXCEPTION_H

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

#include <exception>
#include <new>
#include <string>

#define _JCLIB_EXCEPTION_

namespace jc
{
	/**
	 * @brief Tag type for non-throwing function overloads
	*/
	using std::nothrow_t;

	/**
	 * @brief Tag value type for non-throwing function overloads
	*/
	using std::nothrow;
	

	// TODO : Reduce the likely hood of an exception thrown on jc::exception construction. Maybe have a static buffer for the message?

	/**
	 * @brief Base exception type
	*/
	struct exception : public std::exception
	{
	public:

		/**
		 * @brief Returns the exception's message string
		 * @return Exception message or nullptr
		*/
		const char* what() const noexcept override
		{
			return this->what_.c_str();
		};

		exception(const char* _str, size_t _len) noexcept :
			std::exception{}, what_{ _str, _len }
		{};
		exception(const char* _str) noexcept :
			exception(_str, strlen(_str))
		{};
		
		exception() noexcept :
			std::exception{}
		{};

	private:
		std::string what_;
	};

};

#endif