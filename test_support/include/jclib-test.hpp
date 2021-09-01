#pragma once
#ifndef JCLIB_TEST_HPP
#define JCLIB_TEST_HPP

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
	Test support header
*/

#include <vector>
#include <numeric>
#include <string>
#include <functional>
#include <array>

#include <iostream>

// MSVC error disable
#pragma warning(disable:4003)

// Fails the subtest function and prints a message to the output log
#define FAIL_I(_code, _msg) \
{	\
	std::vector<std::string> _parts	\
	{ "Failed test at '", std::to_string(__LINE__), "': ", _msg }; \
	for(auto& v : _parts) { std::cout << v; }; std::cout << '\n';	 \
	return _code; \
}

// Runs a subtest function passing in any arguements
#define SUBTEST(_fn, ...) { const auto _res = _fn( __VA_ARGS__ ); if (_res != 0) { return _res; };  }

// Helper function
namespace jctest
{
	// Type used to hold test codes
	using testcode_t = int;

	namespace impl
	{
		inline testcode_t newtest()
		{
			static testcode_t count = 0;
			return ++count;
		};
	};
};


// The name of the test code variable
#define TESTCODE_VAR _jclib_testCode

// The full test code variable declaration
#define TESTCODE_VARDECL const ::jctest::impl::testcode_t TESTCODE_VAR

// Sets the subtest function
#define NEWTEST() TESTCODE_VARDECL = ::jctest::impl::newtest(); {}

// Marks subtest function as failed (returns the test code and prints the message to the output stream)
#define FAIL(_msg) FAIL_I(_testCode, _msg)

// Marks subtest function as passing and returns (returns 0)
#define PASS() { return 0; }

#define TIMPL_FRONT(_first, ...) _first

// Checks subtest condition is true and calls FAIL with the additional arguements
#define ASSERT(_condition, ...) if(!( _condition )) { FAIL(TIMPL_FRONT(__VA_ARGS__)); }



namespace jctest
{

};

#endif