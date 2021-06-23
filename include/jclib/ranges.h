#pragma once
#ifndef JCLIB_RANGES_H
#define JCLIB_RANGES_H

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

#include "jclib/type_traits.h"
#include "jclib/functional.h"

#define _JCLIB_RANGES_

namespace jc
{
	namespace ranges
	{
		
		template <typename T, typename = void>
		struct range_ftor;

		template <typename T, size_t N>
		struct range_ftor<T[N], void>
		{
			constexpr auto begin(T(&_val)[N]) const noexcept
			{
				return &_val[0];
			};
			constexpr auto end(T(&_val)[N]) const noexcept
			{
				return &_val[N];
			};
		};

		template <typename T>
		struct range_ftor < T, void_t<
			decltype(std::declval<remove_reference_t<T>>().begin()),
			decltype(std::declval<remove_reference_t<T>>().end())
			>>
		{
			constexpr auto begin(remove_reference_t<T>& _val) const noexcept
			{
				return _val.begin();
			};
			constexpr auto end(remove_reference_t<T>& _val) const noexcept
			{
				return _val.end();
			};
		};

		template <typename T, typename = void>
		struct is_range : false_type {};

		template <typename T>
		struct is_range<T,
			void_t<decltype(std::declval<range_ftor<T>>().begin(std::declval<T&>()))>
		> : true_type {};

#ifdef __cpp_inline_variables
		template <typename T>
		constexpr inline bool is_range_v = is_range<T>::value;
#endif


		template <typename T, typename = void>
		struct iterator;
		template <typename T>
		struct iterator<T, enable_if_t<is_range<T>::value>>
		{
			using type = decltype(std::declval<range_ftor<T>>().begin(std::declval<T&>()));
		};

		template <typename T>
		using iterator_t = typename iterator<T>::type;

		template <typename T, typename = void>
		struct value;
		template <typename T>
		struct value <T, enable_if_t<is_range<T>::value>>
		{
			using type = remove_reference_t<decltype(*std::declval<iterator_t<T>>())>;
		};

		template <typename T>
		using value_t = typename value<T>::type;





		template <typename T>
		constexpr auto begin(T& _value) -> iterator_t<T>
		{
			return range_ftor<T>{}.begin(_value);
		};

		template <typename T>
		constexpr auto end(T& _value) -> iterator_t<T>
		{
			return range_ftor<T>{}.end(_value);
		};

	};

	using ranges::begin;
	using ranges::end;

	namespace ranges
	{
#ifdef __cpp_concepts
		template <typename T>
		concept range = is_range_v<T>;
#endif
		namespace impl
		{
			template <typename UnderlyingT, typename OpT>
			struct condition_iterator
			{
			private:
				using underlying_type = UnderlyingT;
			public:
				friend inline constexpr bool operator==(const condition_iterator& _lhs, const underlying_type& _rhs) noexcept
				{
					return _lhs.at_ == _rhs;
				};
				friend inline constexpr bool operator==(const underlying_type& _lhs, const condition_iterator& _rhs) noexcept
				{
					return _rhs == _lhs;
				};
				friend inline constexpr bool operator==(const condition_iterator& _lhs, const condition_iterator& _rhs) noexcept
				{
					return _lhs.at_ == _rhs;
				};

				friend inline constexpr bool operator!=(const condition_iterator& _lhs, const underlying_type& _rhs) noexcept
				{
					return !(_lhs == _rhs);
				};
				friend inline constexpr bool operator!=(const underlying_type& _lhs, const condition_iterator& _rhs) noexcept
				{
					return !(_lhs == _rhs);
				};
				friend inline constexpr bool operator!=(const condition_iterator& _lhs, const condition_iterator& _rhs) noexcept
				{
					return !(_lhs == _rhs);
				};

				constexpr auto& operator*()
				{
					return *this->at_;
				};
				constexpr auto& operator*() const
				{
					return *this->at_;
				};

				constexpr auto* operator->()
				{
					return &*this->at_;
				};
				constexpr auto* operator->() const
				{
					return &*this->at_;
				};

			private:
				constexpr bool check_condition(underlying_type _val)
				{
					return invoke(*this->op_, *_val);
				};

			public:

				constexpr condition_iterator& operator++()
				{
					while (++this->at_ != this->end_ && !this->check_condition(this->at_));
					return *this;
				};
				constexpr condition_iterator operator++(int)
				{
					condition_iterator _out{ *this };
					++(*this);
					return _out;
				};

				constexpr condition_iterator() noexcept :
					op_{ nullptr }
				{};
				constexpr condition_iterator(underlying_type _at, underlying_type _end, OpT& _op) noexcept :
					at_{ _at }, end_{ _end }, op_{ &_op }
				{
					while (this->at_ != this->end_ && !this->check_condition(this->at_))
					{
						++this->at_;
					};
				};

			private:
				underlying_type at_;
				underlying_type end_;
				OpT* op_;
			};
		};




		template <typename RangeT, typename OpT, typename = void>
		struct filter_view;
		
		template <typename RangeT, typename OpT>
		struct filter_view <RangeT, OpT,
			enable_if_t<is_range<RangeT>::value&& is_invocable<OpT, value_t<RangeT>>::value>
		>
		{
		public:
			using iterator = impl::condition_iterator<iterator_t<RangeT>, OpT>;

		private:
			OpT filter_;
			iterator begin_;
			iterator end_;

		public:
			constexpr iterator begin() const noexcept
			{
				return this->begin_;
			};
			constexpr iterator end() const noexcept
			{
				return this->end_;
			};

			constexpr filter_view(RangeT& _range, OpT&& _op) :
				filter_{ std::move(_op) },
				begin_{ ranges::begin(_range), ranges::end(_range),  this->filter_ },
				end_{ ranges::end(_range), ranges::end(_range), this->filter_ }
			{};

		};

	};

	namespace views
	{

		namespace impl
		{
			template <typename OpT>
			struct filter_impl
			{
				template <typename RangeT>
				friend constexpr inline auto operator|(RangeT& _range, filter_impl<OpT> _filter) -> ranges::filter_view<RangeT, OpT>
				{
					return ranges::filter_view<RangeT, OpT>{ _range, std::move(_filter.op) };
				};
			
				constexpr filter_impl(OpT&& _op) noexcept :
					op{ std::move(_op) }
				{};
				constexpr filter_impl(const OpT& _op) :
					op{ _op }
				{};

				OpT op;
			};

			struct filter_t
			{
				template <typename OpT>
				constexpr filter_impl<remove_cvref_t<OpT>> operator()(OpT&& _op) const noexcept
				{
					return filter_impl<remove_cvref_t<OpT>>{ std::forward<OpT>(_op) };
				};
			};
		};

#ifdef __cpp_inline_variables
		constexpr inline impl::filter_t filter{};
#else
		template <typename OpT>
		constexpr auto filter(OpT&& _op)
		{
			return impl::filter_impl<remove_cvref_t<OpT>>{ std::forward<OpT>(_op) };
		};
#endif

	};

};

#endif