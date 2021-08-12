#pragma once
#ifndef JCLIB_FMTSTREAM_H
#define JCLIB_FMTSTREAM_H

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
#include <jclib/feature.h>


#define _JCLIB_FMTSTREAM_

#ifdef __cpp_lib_format

#include <format>
#include <string_view>
#include <ostream>

namespace jc
{
	/**
	 * @brief Provides a convenience interface for writing a formatted string to an output stream
	*/
	struct formatted_ostream
	{
	public:

		void write(std::string_view _fmt, const auto&... _val)
		{
			this->ostr_ << std::format(_fmt, _val...);
		};
		auto operator()(std::string_view _fmt, const auto&... _val)
		{
			return this->write(_fmt, _val...);
		};

		formatted_ostream() = default;
		formatted_ostream(std::streambuf* _buff) :
			ostr_{ _buff }
		{};
		formatted_ostream(std::ostream& _ostr) :
			formatted_ostream{ _ostr.rdbuf() }
		{};

	private:
		std::ostream ostr_;
	};
	
};

#endif

#endif