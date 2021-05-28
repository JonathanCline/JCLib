#pragma once
#ifndef JCLIB_ARRAY_VIEW_H
#define JCLIB_ARRAY_VIEW_H

#define _JCLIB_ARRAY_VIEW_


// Implemented as a wrapper for standard span, for now

#if true //defined(__cpp_lib_span)

#include <span>
#include <array>

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

namespace jc
{

	template <typename T>
	class array_view
	{
	public:
		using value_type = T;
		using pointer = value_type*;
		using reference = value_type&;
		using const_pointer = const value_type*;
		using const_reference = const value_type&;

		using size_type = size_t;

	private:
		using container_type = std::span<value_type, std::dynamic_extent>;
	public:
		using iterator = typename container_type::iterator;
		using reverse_iterator = typename container_type::reverse_iterator;

		iterator begin() const noexcept { return this->data_.begin(); };
		iterator end() const noexcept { return this->data_.end(); };

		reverse_iterator rbegin() const noexcept { return this->data_.rbegin(); };
		reverse_iterator rend() const noexcept { return this->data_.rend(); };

		auto data() const noexcept { return this->data_.data(); };
		auto size() const noexcept { return this->data_.size(); };

		auto& front() const noexcept { return this->data_.front(); };
		auto& back() const noexcept { return this->data_.back(); };

		constexpr array_view() noexcept = default;
		constexpr array_view(value_type* _data, size_type _size) noexcept :
			data_{ _data, _size }
		{};

		// CArray constructor + assignment

		template <size_t Len>
		constexpr array_view(value_type (&_data)[Len]) noexcept :
			array_view{ _data, Len }
		{};
		template <size_t Len>
		array_view& operator=(value_type(&_data)[Len]) noexcept
		{
			this->data_ = std::span<value_type, std::dynamic_extent>{ _data, Len };
			return *this;
		};

		template <size_t Len>
		constexpr array_view(std::array<value_type, Len>& _data) noexcept :
			array_view{ _data.data(), Len }
		{};
		template <size_t Len>
		array_view& operator=(std::array<value_type, Len>& _data) noexcept
		{
			this->data_ = std::span<value_type, std::dynamic_extent>{ _data.data(), Len };
			return *this;
		};

	private:
		std::span<value_type, std::dynamic_extent> data_;
	};

	template <typename T, size_t Len>
	array_view(T(&)[Len])->array_view<T>;

	template <typename T, size_t Len>
	array_view(std::array<T, Len>&)->array_view<T>;


};
#endif

#endif