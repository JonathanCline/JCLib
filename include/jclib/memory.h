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
#include "jclib/type.h"

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
	template <typename T> JCLIB_REQUIRES(std::is_destructible_v<T>)
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
	template <typename T> JCLIB_REQUIRES(requires (T* a) { jc::destroy_at(*a); })
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



	/**
	 * @brief Small wrapper around a pointer to indicate a borrowing relationship
	 * @tparam T Type to point to
	*/
	template <typename T>
	struct borrow_ptr
	{
	public:

		/**
		 * @brief Type being pointed
		*/
		using value_type = T;

		/**
		 * @brief Pointer held by this type
		*/
		using pointer = value_type*;

		/**
		 * @brief Reference to the type being held
		*/
		using reference = value_type&;

		/**
		 * @brief Type returned from subtraction operations
		*/
		using difference_type = std::ptrdiff_t;

	private:

		// Returns a reference to the held pointer
		pointer& raw_get() noexcept
		{
			return this->ptr_;
		};

		// Returns a reference to the held pointer
		const pointer& raw_get() const noexcept
		{
			return this->ptr_;
		};

	public:

		/**
		 * @brief Returns the pointer being held
		 * @return Held pointer value
		*/
		constexpr pointer get() const noexcept
		{
			return this->raw_get();
		};

		/**
		 * @brief Returns the pointer being held
		 * @return Held pointer value
		*/
		constexpr pointer operator->() const noexcept
		{
			return this->get();
		};

		/**
		 * @brief Returns a reference to the value being pointed to
		 * @return Reference to the held value
		*/
		constexpr reference operator*() const noexcept(!jc::exceptions_v)
		{
			return *this->get();
		};

		/**
		 * @brief Checks if this is holding a valid pointer
		 * @return False if pointer is null, true otherwise
		*/
		constexpr bool good() const noexcept
		{
			return this->get() != nullptr;
		};

		/**
		 * @brief Checks if this is holding a valid pointer
		 * @return False if pointer is null, true otherwise
		*/
		constexpr explicit operator bool() const noexcept
		{
			return this->good();
		};

		/**
		 * @brief Releases the held pointer by setting it to null
		*/
		constexpr void release() noexcept
		{
			this->raw_get() = nullptr;
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


		/**
		 * @brief Returns the distance between the held pointer and another borrow_ptr's held pointer
		 * @param rhs Other borrow_ptr
		 * @return Distance between the pointers
		*/
		constexpr difference_type operator-(const borrow_ptr& rhs) const noexcept
		{
			return this->get() - rhs.get();
		};

		// Distance functions for held pointer type

		friend inline constexpr difference_type operator-(const borrow_ptr& _lhs, pointer _rhs) noexcept
		{
			return _lhs.get() - _rhs;
		};
		friend inline constexpr difference_type operator-(pointer _lhs, const borrow_ptr& _rhs) noexcept
		{
			return _lhs - _rhs.get();
		};


		// Pointer arithmetic functions

		constexpr borrow_ptr operator+(difference_type _count) const noexcept
		{
			return borrow_ptr{ this->get() + _count };
		};
		constexpr borrow_ptr& operator+=(difference_type _count) noexcept
		{
			this->raw_get() += _count;
			return *this;
		};

		constexpr borrow_ptr operator-(difference_type _count) const noexcept
		{
			return borrow_ptr{ this->get() - _count };
		};
		constexpr borrow_ptr& operator-=(difference_type _count) noexcept
		{
			this->raw_get() -= _count;
			return *this;
		};


		// Pointer comparisons

		constexpr bool operator==(const borrow_ptr& rhs) const noexcept
		{
			return this->get() == rhs.get();
		};
		friend inline constexpr bool operator==(const borrow_ptr& lhs, pointer rhs) noexcept
		{
			return lhs.get() == rhs;
		};
		friend inline constexpr bool operator==(pointer lhs, const borrow_ptr& rhs) noexcept
		{
			return rhs == lhs;
		};
		
		constexpr bool operator!=(const borrow_ptr& rhs) const noexcept
		{
			return this->get() != rhs.get();
		};
		friend inline constexpr bool operator!=(const borrow_ptr& lhs, pointer rhs) noexcept
		{
			return lhs.get() != rhs;
		};
		friend inline constexpr bool operator!=(pointer lhs, const borrow_ptr& rhs) noexcept
		{
			return rhs != lhs;
		};
		
		constexpr bool operator>(const borrow_ptr& rhs) const noexcept
		{
			return this->get() > rhs.get();
		};
		friend inline constexpr bool operator>(const borrow_ptr& lhs, pointer rhs) noexcept
		{
			return lhs.get() > rhs;
		};
		friend inline constexpr bool operator>(pointer lhs, const borrow_ptr& rhs) noexcept
		{
			return lhs > rhs.get();
		};
		
		constexpr bool operator>=(const borrow_ptr& rhs) const noexcept
		{
			return this->get() >= rhs.get();
		};
		friend inline constexpr bool operator>=(const borrow_ptr& lhs, pointer rhs) noexcept
		{
			return lhs.get() >= rhs;
		};
		friend inline constexpr bool operator>=(pointer lhs, const borrow_ptr& rhs) noexcept
		{
			return lhs >= rhs.get();
		};
		
		constexpr bool operator<(const borrow_ptr& rhs) const noexcept
		{
			return this->get() < rhs.get();
		};
		friend inline constexpr bool operator<(const borrow_ptr& lhs, pointer rhs) noexcept
		{
			return lhs.get() < rhs;
		};
		friend inline constexpr bool operator<(pointer lhs, const borrow_ptr& rhs) noexcept
		{
			return lhs < rhs.get();
		};
		
		constexpr bool operator<=(const borrow_ptr& rhs) const noexcept
		{
			return this->get() <= rhs.get();
		};
		friend inline constexpr bool operator<=(const borrow_ptr& lhs, pointer rhs) noexcept
		{
			return lhs.get() <= rhs;
		};
		friend inline constexpr bool operator<=(pointer lhs, const borrow_ptr& rhs) noexcept
		{
			return lhs <= rhs.get();
		};


		/*
		// Nullptr comparisons (may not be needed)

		friend inline constexpr bool operator==(const borrow_ptr& lhs, nullptr_t rhs) noexcept
		{
			return lhs.get() == rhs;
		};
		friend inline constexpr bool operator==(nullptr_t lhs, const borrow_ptr& rhs) noexcept
		{
			return rhs == lhs;
		};
		friend inline constexpr bool operator!=(const borrow_ptr& lhs, nullptr_t rhs) noexcept
		{
			return lhs.get() != rhs;
		};
		friend inline constexpr bool operator!=(nullptr_t lhs, const borrow_ptr& rhs) noexcept
		{
			return rhs != lhs;
		};
		friend inline constexpr bool operator>(const borrow_ptr& lhs, nullptr_t rhs) noexcept
		{
			return lhs.get() > rhs;
		};
		friend inline constexpr bool operator>(nullptr_t lhs, const borrow_ptr& rhs) noexcept
		{
			return lhs > rhs.get();
		};
		friend inline constexpr bool operator>=(const borrow_ptr& lhs, nullptr_t rhs) noexcept
		{
			return lhs.get() >= rhs;
		};
		friend inline constexpr bool operator>=(nullptr_t lhs, const borrow_ptr& rhs) noexcept
		{
			return lhs >= rhs.get();
		};
		friend inline constexpr bool operator<(const borrow_ptr& lhs, nullptr_t rhs) noexcept
		{
			return lhs.get() < rhs;
		};
		friend inline constexpr bool operator<(nullptr_t lhs, const borrow_ptr& rhs) noexcept
		{
			return lhs < rhs.get();
		};
		friend inline constexpr bool operator<=(const borrow_ptr& lhs, nullptr_t rhs) noexcept
		{
			return lhs.get() <= rhs;
		};
		friend inline constexpr bool operator<=(nullptr_t lhs, const borrow_ptr& rhs) noexcept
		{
			return lhs <= rhs.get();
		};
		*/


		/**
		 * @brief Does not initialize the held pointer! This allows for the type to be trivial.
		*/
		constexpr borrow_ptr() noexcept = default;

		/**
		 * @brief Sets the held pointer to a user-provided pointer
		 * @param _ptr Pointer to hold
		*/
		constexpr borrow_ptr(pointer _ptr) noexcept :
			ptr_{ _ptr }
		{};

		/**
		 * @brief Sets the held pointer to a user-provided pointer
		 * @param _ptr New pointer to hold
		 * @return Reference to this object
		*/
		constexpr borrow_ptr& operator=(pointer _ptr) noexcept
		{
			this->raw_get() = _ptr;
			return *this;
		};

		/**
		 * @brief Sets the held pointer to null
		 * @param _ptr Null ptr tag object
		*/
		constexpr borrow_ptr(std::nullptr_t _ptr) noexcept :
			ptr_{ nullptr }
		{};

		/**
		 * @brief Releases the held pointer by setting it to null
		 * @param _ptr Null pointer tag object
		 * @return Reference to this object
		*/
		constexpr borrow_ptr& operator=(std::nullptr_t _ptr) noexcept
		{
			this->release();
			return *this;
		};

		/**
		 * @brief Sets the held pointer to null
		 * @param nt Null value tag
		*/
		constexpr borrow_ptr(jc::null_t nt) noexcept :
			ptr_{ nullptr }
		{};

		/**
		 * @brief Releases the held pointer by setting it to null
		 * @param nt Null value tag
		 * @return Reference to this object
		*/
		constexpr borrow_ptr& operator=(jc::null_t nt) noexcept
		{
			this->release();
			return *this;
		};

	private:

		/**
		 * @brief The held pointer
		*/
		pointer ptr_;

	};

	// borrow_ptr
	// borrow_ref
	// 
	// unique_ptr
	// unique_ref
	// shared_ptr
	// shared_ref

	/**
	 * @brief Same as a borrow_ptr but may never be null
	 * @tparam T Type to point to
	*/
	template <typename T>
	struct reference_ptr
	{
	public:
		/**
		 * @brief Type being pointed
		*/
		using value_type = T;

		/**
		 * @brief Pointer held by this type
		*/
		using pointer = value_type*;

		/**
		 * @brief Reference to the type being held
		*/
		using reference = value_type&;

		/**
		 * @brief Type returned from subtraction operations
		*/
		using difference_type = std::ptrdiff_t;

	private:

		// Returns a reference to the held pointer
		pointer& raw_get() noexcept
		{
			return this->ptr_;
		};

		// Returns a reference to the held pointer
		const pointer& raw_get() const noexcept
		{
			return this->ptr_;
		};

	public:

		/**
		 * @brief Returns the pointer being held
		 * @return Held pointer value
		*/
		constexpr pointer get() const noexcept
		{
			return this->raw_get();
		};

		/**
		 * @brief Implicitly converts to a borrow_ptr<T> as this implements the same sort of relationship
		 * @return borrow_ptr containing the pointer this holds
		*/
		constexpr operator borrow_ptr<T>() const noexcept
		{
			return borrow_ptr<T>{ this->get() };
		};

		/**
		 * @brief Returns the pointer being held
		 * @return Held pointer value
		*/
		constexpr pointer operator->() const noexcept
		{
			return this->get();
		};

		/**
		 * @brief Returns a reference to the value being pointed to
		 * @return Reference to the held value
		*/
		constexpr reference operator*() const noexcept
		{
			return *this->get();
		};

		/**
		 * @brief Checks if this is holding a valid pointer
		 * @return False if pointer is null, true otherwise
		*/
		constexpr bool good() const noexcept
		{
			JCLIB_ASSERT(this->get() != nullptr);
			return true;
		};

		/**
		 * @brief Checks if this is holding a valid pointer
		 * @return False if pointer is null, true otherwise
		*/
		constexpr explicit operator bool() const noexcept
		{
			return this->good();
		};


		/**
		 * @brief Returns the distance between the held pointer and another reference_ptr's held pointer
		 * @param rhs Other reference_ptr
		 * @return Distance between the pointers
		*/
		constexpr difference_type operator-(const reference_ptr& rhs) const noexcept
		{
			return this->get() - rhs.get();
		};


		// Distance functions for held pointer type

		friend inline constexpr difference_type operator-(const reference_ptr& _lhs, pointer _rhs) noexcept
		{
			return _lhs.get() - _rhs;
		};
		friend inline constexpr difference_type operator-(pointer _lhs, const reference_ptr& _rhs) noexcept
		{
			return _lhs - _rhs.get();
		};


		// Pointer arithmetic, returns borrow_ptr

		constexpr jc::borrow_ptr<value_type> operator+(difference_type _count) const noexcept
		{
			return jc::borrow_ptr<value_type>{ this->get() + _count };
		};
		constexpr jc::borrow_ptr<value_type> operator-(difference_type _count) const noexcept
		{
			return jc::borrow_ptr<value_type>{ this->get() - _count };
		};


		// Pointer comparisons

		constexpr bool operator==(const reference_ptr& rhs) const noexcept
		{
			return this->get() == rhs.get();
		};
		friend inline constexpr bool operator==(const reference_ptr& lhs, pointer rhs) noexcept
		{
			return lhs.get() == rhs;
		};
		friend inline constexpr bool operator==(pointer lhs, const reference_ptr& rhs) noexcept
		{
			return rhs == lhs;
		};

		constexpr bool operator!=(const reference_ptr& rhs) const noexcept
		{
			return this->get() != rhs.get();
		};
		friend inline constexpr bool operator!=(const reference_ptr& lhs, pointer rhs) noexcept
		{
			return lhs.get() != rhs;
		};
		friend inline constexpr bool operator!=(pointer lhs, const reference_ptr& rhs) noexcept
		{
			return rhs != lhs;
		};

		constexpr bool operator>(const reference_ptr& rhs) const noexcept
		{
			return this->get() > rhs.get();
		};
		friend inline constexpr bool operator>(const reference_ptr& lhs, pointer rhs) noexcept
		{
			return lhs.get() > rhs;
		};
		friend inline constexpr bool operator>(pointer lhs, const reference_ptr& rhs) noexcept
		{
			return lhs > rhs.get();
		};
		constexpr bool operator>=(const reference_ptr& rhs) const noexcept
		{
			return this->get() >= rhs.get();
		};
		friend inline constexpr bool operator>=(const reference_ptr& lhs, pointer rhs) noexcept
		{
			return lhs.get() >= rhs;
		};
		friend inline constexpr bool operator>=(pointer lhs, const reference_ptr& rhs) noexcept
		{
			return lhs >= rhs.get();
		};

		constexpr bool operator<(const reference_ptr& rhs) const noexcept
		{
			return this->get() < rhs.get();
		};
		friend inline constexpr bool operator<(const reference_ptr& lhs, pointer rhs) noexcept
		{
			return lhs.get() < rhs;
		};
		friend inline constexpr bool operator<(pointer lhs, const reference_ptr& rhs) noexcept
		{
			return lhs < rhs.get();
		};
		constexpr bool operator<=(const reference_ptr& rhs) const noexcept
		{
			return this->get() <= rhs.get();
		};
		friend inline constexpr bool operator<=(const reference_ptr& lhs, pointer rhs) noexcept
		{
			return lhs.get() <= rhs;
		};
		friend inline constexpr bool operator<=(pointer lhs, const reference_ptr& rhs) noexcept
		{
			return lhs <= rhs.get();
		};


		// Reference ptr may never be null
		constexpr reference_ptr() noexcept = delete;

		/**
		 * @brief Sets the held pointer to a user-provided value
		 * @param _ptr Reference to the value to hold
		*/
		constexpr reference_ptr(reference _val) noexcept :
			ptr_{ &_val }
		{};

		/**
		 * @brief Sets the held pointer to a user-provided value
		 * @param _ptr Reference to the value to hold
		 * @return Reference to this object
		*/
		constexpr reference_ptr& operator=(reference _ptr) noexcept
		{
			this->raw_get() = &_ptr;
			return *this;
		};

	private:

		/**
		 * @brief The held pointer
		*/
		pointer ptr_;

	};









};

#endif