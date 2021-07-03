#pragma once
#ifndef JCLIB_SPAN_H
#define JCLIB_SPAN_H

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
#include "jclib/functional.h"
#include "jclib/type_traits.h"

#include <limits>
#include <cstdint>
#include <iterator>

#define _JCLIB_SPAN_

namespace jc
{
	/**
	 * @brief Value type for spans with size set on runtime
	*/
	constexpr static size_t dynamic_extent = std::numeric_limits<size_t>::max();


	//		This should make it easier to implement custom iterator types.
	//	Iterator functionality is derived from this
	//
	//	template <typename T, typename = void>
	//	struct iterator_traits;



	namespace impl
	{
		template <typename T>
		struct random_iterator_default_incrementer
		{
			constexpr T operator()(T _at, std::ptrdiff_t _diff) const noexcept
			{
				return _at + _diff;
			};
		};
	};

	template <typename T, typename Incrementer, typename = void>
	struct basic_random_iterator;

	template <typename T, typename Incrementer>
	struct basic_random_iterator<T, Incrementer, enable_if_t<is_invocable<Incrementer, T*, std::ptrdiff_t>::value>>
	{
	private:
		constexpr inline void check_range() const noexcept
		{
#ifdef JCLIB_DEBUG
			const auto& _at = this->at_;
			JCLIB_ASSERT(_at >= this->begin_ && _at <= this->end_);
#endif
		};
		constexpr inline void check_value() const noexcept
		{
#ifdef JCLIB_DEBUG
			const auto& _at = this->at_;
			JCLIB_ASSERT(_at >= this->begin_ && _at < this->end_);
#endif
		};
		constexpr static inline Incrementer incrementer() noexcept
		{
			return Incrementer{};
		};
		
	public:
		using value_type = T;
		using pointer = value_type*;
		using reference = value_type&;

		using iterator_category = std::random_access_iterator_tag;
		using difference_type = std::ptrdiff_t;

	private:
		constexpr static inline pointer increment(pointer _at, difference_type _diff) noexcept
		{
			constexpr auto _inc = incrementer();
			return jc::invoke(_inc, _at, _diff);
		};

	public:

		constexpr bool operator==(pointer _rhs) const noexcept
		{
			return this->at_ == _rhs;
		};
		constexpr bool operator!=(pointer _rhs) const noexcept
		{
			return !(*this == _rhs);
		};

		constexpr bool operator<(pointer _rhs) const noexcept
		{
			return this->at_ < _rhs;
		};
		constexpr bool operator>(pointer _rhs) const noexcept
		{
			return this->at_ > _rhs;
		};
		constexpr bool operator<=(pointer _rhs) const noexcept
		{
			return this->at_ <= _rhs;
		};
		constexpr bool operator>=(pointer _rhs) const noexcept
		{
			return this->at_ >= _rhs;
		};

		constexpr bool operator==(basic_random_iterator _rhs) const noexcept
		{
			return *this == _rhs.at_;
		};
		constexpr bool operator!=(basic_random_iterator _rhs) const noexcept
		{
			return !(*this == _rhs);
		};

		constexpr bool operator<(basic_random_iterator _rhs) const noexcept
		{
			return *this < _rhs.at_;
		};
		constexpr bool operator>(basic_random_iterator _rhs) const noexcept
		{
			return *this > _rhs.at_;
		};
		constexpr bool operator<=(basic_random_iterator _rhs) const noexcept
		{
			return *this <= _rhs.at_;
		};
		constexpr bool operator>=(basic_random_iterator _rhs) const noexcept
		{
			return *this >= _rhs.at_;
		};

		constexpr difference_type operator-(basic_random_iterator _rhs) const noexcept
		{
			return this->at_ - _rhs.at_;
		};

		constexpr basic_random_iterator& operator+=(difference_type _rhs) noexcept
		{
			this->at_ += _rhs;
			this->check_range();
			return *this;
		};
		
		friend constexpr basic_random_iterator operator+(basic_random_iterator _lhs, difference_type _rhs) noexcept
		{
			auto _out{ _lhs };
			_out += _rhs;
			return _out;
		};
		friend constexpr basic_random_iterator operator+(difference_type _lhs, basic_random_iterator _rhs) noexcept
		{
			return _rhs + _lhs;
		};

		constexpr basic_random_iterator& operator-=(difference_type _rhs) noexcept
		{
			this->at_ -= _rhs;
			this->check_range();
			return *this;
		};
		constexpr basic_random_iterator operator-(difference_type _rhs) const noexcept
		{
			auto _out{ *this };
			_out -= _rhs;
			return _out;
		};

		constexpr basic_random_iterator& operator++() noexcept
		{
			(*this) += 1;
			return *this;
		};
		constexpr basic_random_iterator operator++(int) noexcept
		{
			auto _out{ *this };
			++(*this);
			return _out;
		};

		constexpr basic_random_iterator& operator--() noexcept
		{
			(*this) -= 1;
			return *this;
		};
		constexpr basic_random_iterator operator--(int) noexcept
		{
			auto _out{ *this };
			--(*this);
			return _out;
		};

	private:
		constexpr pointer get() const noexcept
		{
			this->check_value();
			return this->at_;
		};

	public:
		constexpr pointer operator->() const noexcept
		{
			return this->get();
		};

		constexpr value_type& operator*() noexcept
		{
			return *this->get();
		};
		constexpr const value_type& operator*() const noexcept
		{
			return *this->get();
		};
		
		constexpr value_type& operator[](difference_type _idx) noexcept
		{
			auto _out = this->get() + _idx;
#ifdef JCLIB_DEBUG
			JCLIB_ASSERT(_out >= this->begin_ && _out < this->end_);
#endif
			return *_out;
		};
		constexpr const value_type& operator*(difference_type _idx) const noexcept
		{
			auto _out = this->get() + _idx;
#ifdef JCLIB_DEBUG
			JCLIB_ASSERT(_out >= this->begin_ && _out < this->end_);
#endif
			return *_out;
		};

#ifndef JCLIB_DEBUG
		constexpr basic_random_iterator(pointer _at) noexcept :
			at_{ _at }
		{};
		constexpr basic_random_iterator(pointer _at, pointer _begin, pointer _end) noexcept :
			at_{ _at }
		{};
#endif
#ifdef JCLIB_DEBUG
		constexpr basic_random_iterator(pointer _at, pointer _begin, pointer _end) noexcept :
			at_{ _at }, begin_{ _begin }, end_{ _end }
		{
			this->check_range();
		};
#endif
	private:
		pointer at_;
#ifdef JCLIB_DEBUG
		pointer begin_;
		pointer end_;
#endif
	};

	template <typename T>
	using random_iterator = basic_random_iterator<T, impl::random_iterator_default_incrementer<T*>>;





	/**
	 * @brief Presents a container like interface to a pointer + size pair, doesn't own the memory
	 * @tparam T Type pointed to by the span
	 * @tparam Extent Size of the span, defaults to dynamic extent
	*/
	template <typename T, size_t Extent = dynamic_extent>
	class span
	{
	public:
		using value_type = T;
		using pointer = value_type*;
		using reference = value_type&;
		using const_pointer = const value_type*;
		using const_reference = const value_type&;

		using size_type = size_t;
		using difference_type = std::ptrdiff_t;
		
		using iterator = random_iterator<value_type>;
		using const_iterator = random_iterator<const value_type>;

		constexpr pointer data() const noexcept { return this->data_; };
		constexpr size_type size() const noexcept { return this->size_; };
	private:
		constexpr pointer data_end() const noexcept { return this->data() + this->size(); };

	public:
		iterator begin() noexcept
		{
#ifdef JCLIB_DEBUG
			return iterator{ this->data(), this->data(), this->data_end() };
#else
			return iterator{ this->data() };
#endif
		};
		const_iterator begin() const noexcept
		{
#ifdef JCLIB_DEBUG
			return iterator{ this->data(), this->data(), this->data_end() };
#else
			return iterator{ this->data() };
#endif
		};
		const_iterator cbegin() const noexcept
		{
			return this->begin();
		};

		iterator end() noexcept
		{
#ifdef JCLIB_DEBUG
			return iterator{ this->data_end(), this->data(), this->data_end() };
#else
			return iterator{ this->data_end() };
#endif
		};
		const_iterator end() const noexcept
		{
#ifdef JCLIB_DEBUG
			return iterator{ this->data_end(), this->data(), this->data_end() };
#else
			return iterator{ this->data_end() };
#endif
		};
		const_iterator cend() const noexcept
		{
			return this->end();
		};

		reference front() noexcept { return *this->data(); };
		const_reference front() const noexcept { return *this->data(); };

		reference back() noexcept { return *(this->data_end() - 1); };
		const_reference back() const noexcept { return *(this->data_end() - 1); };

		constexpr span() noexcept = default;
		constexpr span(pointer _data, size_type _size) noexcept :
			data_{ _data }, size_{ _size  }
		{};

	private:
		pointer data_ = nullptr;
		size_type size_ = 0;
	};


};

#endif