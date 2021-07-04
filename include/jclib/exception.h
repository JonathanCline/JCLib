#pragma once
#ifndef JCLIB_EXCEPTION_H
#define JCLIB_EXCEPTION_H

#include "jclib/config.h"

#include <exception>

#define _JCLIB_EXCEPTION_

namespace jc
{
	/**
	 * @brief Tag type for non-throwing function overloads
	*/
	struct nothrow_t {};

	/**
	 * @brief Tag value type for non-throwing function overloads
	*/
	constexpr static nothrow_t nothrow{};
	
	/**
	 * @brief Base exception type alias
	*/
	using std::exception;

};

#endif