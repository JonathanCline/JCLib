#pragma once
#ifndef JCLIB_CONFIG_ALGORITHM_H
#define JCLIB_CONFIG_ALGORITHM_H

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
 *	@brief Enables custom std algorithm implementations for backporting constexpr support when using C++14/17
*/
#cmakedefine JCLIB_CONSTEXPR_ALGORIHMS

#ifdef JCLIB_CONSTEXPR_ALGORIHMS
	// Enables custom std algorithm implementations for backporting constexpr support when using C++14/17
	#define JCLIB_CONSTEXPR_ALGORIHMS_V true
#else
	// Enables custom std algorithm implementations for backporting constexpr support when using C++14/17
	#define JCLIB_CONSTEXPR_ALGORIHMS_V false
#endif

#endif