#pragma once
#ifndef JCLIB_MEMORY_H
#define JCLIB_MEMORY_H

#define _JCLIB_MEMORY_

#include "jclib/config.h"
#include "jclib/type_traits.h"

#ifdef _cpp_concepts
#include "jclib/concepts.h"
#endif

namespace jc
{
	/**
	 * @brief Calls the destructor of the element passed in
	 * @tparam T Object type
	 * @param _ref Object to call destructor on
	*/
	template <typename T>
#ifdef __cpp_lib_constexpr_dynamic_alloc
	JCLIB_CONSTEXPR inline void destroy_at(T& _ref)
#else
	inline void destroy_at(T& _ref)
#endif
	{
		_ref.~T();
	};

	/**
	 * @brief Calls the destructor of the element passed in
	 * @tparam T Object type
	 * @param _ref Object to call destructor on
	*/
	template <typename T>
#ifdef __cpp_lib_constexpr_dynamic_alloc
	JCLIB_CONSTEXPR inline void destroy_at(T* _ptr)
#else
	inline void destroy_at(T* _ptr)
#endif
	{
		destroy_at(*_ptr);
	};

};

#endif