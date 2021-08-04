#pragma once
#ifndef JCLIB_COMPILER_H
#define JCLIB_COMPILER_H

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