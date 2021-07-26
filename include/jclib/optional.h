#pragma once
#ifndef JCLIB_OPTIONAL_H
#define JCLIB_OPTIONAL_H

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
#include "jclib/maybe.h"

#include <memory>

#define _JCLIB_OPTIONAL_

namespace jc
{
	/**
	 * @brief Value type held by an empty optional
	*/
	struct nullopt_t {};

	/**
	 * @brief Value held by an empty optional
	*/
	constexpr static nullopt_t nullopt{};

	/**
	 * @brief Contains either the specified type or nullopt, should be nearly identical to std::optional
	 * @tparam T Optionally held value type
	*/
	template <typename T>
	struct optional : protected maybe<T, nullopt_t>
	{
	private:
		using parent_type = maybe<T, nullopt_t>;

	public:
		using value_type = typename parent_type::value_type;
		using pointer = typename parent_type::pointer;
		using reference = typename parent_type::reference;
		using const_pointer = typename parent_type::const_pointer;
		using const_reference = typename parent_type::const_reference;

		using parent_type::has_value;
		using parent_type::operator bool;

		constexpr void reset() noexcept
		{
			if (parent_type::has_value())
			{
				parent_type::destroy_value();
			};
			parent_type::set_has_value(false);
		};
		
		template <typename... Ts>
		constexpr auto emplace(Ts&&... _values) ->
			enable_if_t<jc::is_constructible<value_type, Ts&&...>::value, reference>
		{
			if (parent_type::has_value())
			{
				parent_type::destroy_value();
			};
			parent_type::set_has_value(true);
			parent_type::unsafe_value().value_type(std::forward<Ts>(_values)...);
			return parent_type::unsafe_value();
		};

		using parent_type::value;
		using parent_type::operator*;
		using parent_type::operator->;

		constexpr value_type value_or(value_type&& _or) const noexcept
		{
			if (this->has_value())
			{
				return parent_type::unsafe_value();
			}
			else
			{
				return std::move(_or);
			};
		};
		constexpr value_type& value_or(value_type& _or) noexcept
		{
			if (parent_type::has_value())
			{
				return parent_type::unsafe_value();
			}
			else
			{
				return _or;
			};
		};
		constexpr const value_type& value_or(const value_type& _or) const noexcept
		{
			if (parent_type::has_value())
			{
				return parent_type::unsafe_value();
			}
			else
			{
				return _or;
			};
		};

		constexpr optional() noexcept :
			parent_type{ jc::alternate, nullopt }
		{};

		constexpr optional(const value_type& _value) noexcept :
			parent_type{ _value }
		{};
		constexpr optional& operator=(const value_type& _value) noexcept
		{
			parent_type::set_has_value(true);
			parent_type::unsafe_value() = _value;
			return *this;
		};

		constexpr optional(value_type&& _value) noexcept :
			parent_type{ std::move(_value) }
		{};
		constexpr optional& operator=(value_type&& _value) noexcept
		{
			parent_type::set_has_value(false);
			parent_type::unsafe_value() = std::move(_value);
			return *this;
		};
	
	};

};

#endif