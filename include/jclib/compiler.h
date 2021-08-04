#pragma once
#ifndef JCLIB_COMPILER_H
#define JCLIB_COMPILER_H

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

#define _JCLIB_COMPILER_



// Compiler ID value for unknown compiler
#define JCLIB_COMPILER_VALUE_NULL  0

// Compiler ID value for GCC
#define JCLIB_COMPILER_VALUE_GCC   1

// Compiler ID value for MSVC
#define JCLIB_COMPILER_VALUE_MSVC  2

// Compiler ID value for Clang
#define JCLIB_COMPILER_VALUE_CLANG 3

// Compiler ID value for Mingw
#define JCLIB_COMPILER_VALUE_MINGW 4

// Compiler ID value for Intel
#define JCLIB_COMPILER_VALUE_INTEL 5



// If the compiler was not already specified, specify it here
#ifndef JCLIB_COMPILER

	/*
		Compiler detection section, this will probably end up being generated
	*/

	#if defined(_MSC_VER)
		// Detected compiler "MSVC"
		#define JCLIB_COMPILER JCLIB_COMPILER_VALUE_MSVC
	#elif defined(__GNUC__)
		// Detected compiler "gcc"
		#define JCLIB_COMPILER JCLIB_COMPILER_VALUE_GCC
	#elif defined(__clang__)
		// Detected compiler "Clang"
		#define JCLIB_COMPILER JCLIB_COMPILER_VALUE_CLANG
	#elif defined(__MINGW32__) || defined(__MINGW64__)
		// Detected compiler "MinGW"
		#define JCLIB_COMPILER JCLIB_COMPILER_VALUE_MINGW
	#elif defined(__INTEL_COMPILER)
		// Detected compiler "Intel"
		#define JCLIB_COMPILER JCLIB_COMPILER_VALUE_INTEL
	#endif

	// Set the detected compiler to null if it wasn't found in the above detection code
	#ifndef JCLIB_COMPILER
		// Failed to determine compiler
		#define JCLIB_COMPILER JCLIB_COMPILER_VALUE_NULL
	#endif

#endif






#endif