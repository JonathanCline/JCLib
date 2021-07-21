#pragma once
#ifndef JCLIB_MEMORY_H
#define JCLIB_MEMORY_H

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
#include "jclib/type_traits.h"
#include "jclib/concepts.h"
#include "jclib/feature.h"

#include <memory>

#ifdef JCLIB_FEATURE_THREE_WAY_COMPARISON
#include <compare>
#endif

#define _JCLIB_MEMORY_

namespace jc
{
	/**
	 * @brief Calls the destructor of the element passed in
	 * @tparam T Object type
	 * @param _ref Object to call destructor on
	*/
	template <typename T> JCLIB_REQUIRES( requires (T& a) { a.~T(); } )
	JCLIB_CONSTEXPR inline void destroy_at(T& _ref)
		noexcept(noexcept(std::declval<T&>().~T()))
	{
		_ref.~T();
	};
	
	/**
	 * @brief Calls the destructor of the element passed in
	 * @tparam T Object type
	 * @param _ref Object to call destructor on
	*/
	template <typename T> JCLIB_REQUIRES(requires (T* a) { destroy_at(*a); })
	JCLIB_CONSTEXPR inline void destroy_at(T* _ptr)
		noexcept(noexcept(jc::destroy_at(*std::declval<T*>())))
	{
		destroy_at(*_ptr);
	};


	/**
	 * @brief Version of std::make_unique but adds SFINAE or concepts support to improve error messages (at least on MSVC)
	 * @tparam T Type for the unique_ptr
	 * @tparam ...Ts Constructor arguement types
	 * @param ..._cargs Constructor arguement values
	 * @return New std::unique_ptr<T>
	*/
	template <typename T, typename... Ts>
#ifdef JCLIB_FEATURE_CONCEPTS
	requires jc::cx_constructible<T, decltype(std::forward<Ts>(std::declval<Ts&&>()))...>
#endif
	inline auto make_unique(Ts&&... _cargs)
		noexcept(noexcept(std::unique_ptr<T>{ new T{ std::forward<Ts>(std::declval<Ts&&>())... } }))
#ifndef JCLIB_FEATURE_CONCEPTS
		-> jc::enable_if_t<
				jc::is_constructible<T, decltype(std::forward<Ts>(std::declval<Ts&&>()))...>::value,
			std::unique_ptr<T>>
#endif
	{
		return std::unique_ptr<T>{ new T{ std::forward<Ts>(_cargs)... }  };
	};



	namespace impl
	{
#ifndef __cpp_conditional_explicit
		// CRTP type for the optional implicit conversion operator for borrow_ptr
		template <typename T, typename ValT, bool Enable>
		struct borrow_ptr_implicit_conversion_impl;

		// Implements the IMPLICIT conversion operator
		template <typename T, typename ValT>
		struct borrow_ptr_implicit_conversion_impl<T, ValT, true>
		{
			constexpr operator ValT*() const noexcept
			{
				return static_cast<const T*>(this)->get();
			};
		};

		// Implements the EXPLICIT conversion operator
		template <typename T, typename ValT>
		struct borrow_ptr_implicit_conversion_impl<T, ValT, false>
		{
			constexpr explicit operator ValT*() const noexcept
			{
				return static_cast<const T*>(this)->get();
			};
		};
#endif
	};

	/**
	 * @brief Small wrapper around a pointer to indicate a borrowing relationship
	 * @tparam T Type to point to
	*/
	template <typename T, bool AllowImplicitConversion = false>
	struct borrow_ptr
#ifndef __cpp_conditional_explicit
		: public impl::borrow_ptr_implicit_conversion_impl<borrow_ptr<T, AllowImplicitConversion>, T, AllowImplicitConversion>
#endif
	{
	public:
		using value_type = T;
		using pointer = value_type*;
		using reference = value_type&;

		using difference_type = std::ptrdiff_t;


		constexpr pointer get() const noexcept { return this->ptr_; };


#ifdef __cpp_conditional_explicit
		constexpr explicit(!AllowImplicitConversion) operator pointer() const noexcept
		{
			return this->get();
		};
#endif

		constexpr pointer operator->() const noexcept { return this->get(); };

		constexpr auto& operator*() noexcept { return *this->get(); };
		constexpr const auto& operator*() const noexcept { return *this->get(); };

		constexpr bool good() const noexcept
		{
			return this->get() != nullptr;
		};
		constexpr explicit operator bool() const noexcept
		{
			return this->good();
		};

		/**
		 * @brief Releases the underlying ptr by setting it to null
		*/
		constexpr void release() noexcept
		{
			this->ptr_ = nullptr;
		};

		/**
		 * @brief Releases the underlying ptr by setting it to null and return the original pointer
		*/
		constexpr pointer extract() noexcept
		{
			auto _out = this->get();
			this->release();
			return _out;
		};

#ifdef JCLIB_FEATURE_THREE_WAY_COMPARISON
		constexpr auto operator<=>(pointer _rhs) const noexcept
		{
			return this->get() <=> _rhs;
		};
		constexpr auto operator<=>(const borrow_ptr& _other) const noexcept = default;
#endif

		friend inline constexpr bool operator==(const borrow_ptr& _lhs, pointer _rhs) noexcept
		{
			return _lhs.get() == _rhs;
		};
		friend inline constexpr bool operator==(pointer _lhs, const borrow_ptr& _rhs) noexcept
		{
			return _rhs == _lhs;
		};

		friend inline constexpr bool operator!=(const borrow_ptr& _lhs, pointer _rhs) noexcept
		{
			return _lhs.get() != _rhs;
		};
		friend inline constexpr bool operator!=(pointer _lhs, const borrow_ptr& _rhs) noexcept
		{
			return _rhs != _lhs;
		};

		constexpr bool operator==(const borrow_ptr& _other) const noexcept
		{
			return this->get() == _other.get();
		};
		constexpr bool operator!=(const borrow_ptr& _other) const noexcept
		{
			return this->get() != _other.get();
		};

		constexpr difference_type operator-(const borrow_ptr& _other) const noexcept
		{
			return this->get() - _other.get();
		};

		friend inline constexpr difference_type operator-(const borrow_ptr& _lhs, pointer _rhs) noexcept
		{
			return _lhs.get() - _rhs;
		};
		friend inline constexpr difference_type operator-(pointer _lhs, const borrow_ptr& _rhs) noexcept
		{
			return _lhs - _rhs.get();
		};

		constexpr borrow_ptr operator+(difference_type _count) const noexcept
		{
			return borrow_ptr{ this->get() + _count };
		};
		constexpr borrow_ptr& operator+=(difference_type _count) noexcept
		{
			this->ptr_ += _count;
			return *this;
		};

		constexpr borrow_ptr operator-(difference_type _count) const noexcept
		{
			return borrow_ptr{ this->get() - _count };
		};
		constexpr borrow_ptr& operator-=(difference_type _count) noexcept
		{
			this->ptr_ -= _count;
			return *this;
		};

		constexpr borrow_ptr() noexcept = default;
		constexpr borrow_ptr(pointer _ptr) noexcept :
			ptr_{ _ptr }
		{};

		constexpr borrow_ptr& operator=(pointer _ptr) noexcept
		{
			this->ptr_ = _ptr;
			return *this;
		};

	private:
		pointer ptr_ = nullptr;
	};

	/**
	 * @brief Same as a borrow_ptr but may never be null
	 * @tparam T Type to point to
	*/
	template <typename T>
	struct reference_ptr : public borrow_ptr<T>
	{
	public:
		using value_type = T;
		using pointer = value_type*;
		using reference = value_type&;

		constexpr bool good() const noexcept
		{
			JCLIB_ASSERT(borrow_ptr<value_type>::good());
			return true;
		};
		constexpr explicit operator bool() const noexcept
		{
			return this->good();
		};

		constexpr reference_ptr() = delete;
		
		constexpr reference_ptr(reference _ref) noexcept :
			borrow_ptr<value_type>{ &_ref }
		{};
		constexpr reference_ptr& operator=(reference _ref) noexcept
		{
			borrow_ptr<value_type>::operator=(&_ref);
			return *this;
		};

	};









};

#endif