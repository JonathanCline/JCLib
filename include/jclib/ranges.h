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
#include "jclib/concepts.h"
#include "jclib/iterator.h"

#include <algorithm>
#include <iterator>

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




		/**
		 * @brief Returns the begin iterator of a range
		 * @tparam T Range type
		 * @param _value Range value
		 * @return Iterator or iterator-like pointing to the first value in the range
		*/
		template <typename T>
		constexpr auto begin(T& _value) -> iterator_t<T>
		{
			return range_ftor<T>{}.begin(_value);
		};

		/**
		 * @brief Returns the end iterator of a range
		 * @tparam T Range type
		 * @param _value Range value
		 * @return Iterator or iterator-like pointing to one past the last value in a range
		*/
		template <typename T>
		constexpr auto end(T& _value) -> iterator_t<T>
		{
			return range_ftor<T>{}.end(_value);
		};


		namespace impl
		{
			template <typename T>
			struct as_forward
			{
				using type = decltype(std::forward<T>(std::declval<T&&>()));
			};
			template <typename T>
			using as_forward_t = typename as_forward<T>::type;

			template <typename OutT, typename InT, typename = void>
			struct range_cast_impl;

			template <typename RangeT>
			struct range_cast_impl<RangeT, RangeT, enable_if_t<is_range<remove_reference_t<RangeT>>::value>>
			{
				using in_type = RangeT;
				using out_type = as_forward_t<RangeT>;

				constexpr as_forward_t<RangeT> operator()(in_type&& _range)
				{
					return std::forward<RangeT>(_range);
				};
			};

			template <typename OutT, typename InT>
			struct range_cast_impl<OutT, InT,
				enable_if_t<!is_same<OutT, remove_cvref_t<InT>>::value &&
							 is_constructible<OutT, as_forward_t<InT>>::value>
			>
			{
				using in_type = InT;
				using out_type = OutT;

				constexpr OutT operator()(in_type&& _in) const noexcept
				{
					return OutT( std::forward<InT>(_in) );
				};
			};

			template <typename OutT, typename InT>
			struct range_cast_impl<OutT, InT,
				enable_if_t<!is_constructible<OutT, InT>::value>
			>
			{
				using in_type = InT;
				using out_type = OutT;

				constexpr OutT operator()(in_type _in) const noexcept
				{
					return OutT(begin(_in), end(_in));
				};
			};
		};
		
		/**
		 * @brief Generic cast function that constructs the output range from the input
		 * @tparam OutT Output range type to be constructed
		 * @tparam InT Range type to cast from
		 * @param _range Range value that to cast from
		 * @return Casted range
		*/
		template <typename OutT, typename InT>
		constexpr inline OutT range_cast(InT&& _range)
		{
			return impl::range_cast_impl<OutT, InT>{}(std::forward<InT>(_range));
		};

	};

	// Any commonly used ranges functionality should be included here for ease of use

	using ranges::begin;
	using ranges::end;
	using ranges::range_cast;

	namespace ranges
	{
		namespace impl
		{
			template <typename T>
			struct distance_ftor
			{
				constexpr auto operator()(const T& _begin, const T& _end) const noexcept
				{
					return std::distance(_begin, _end);
				};
			};
			template <typename T>
			struct distance_ftor<T*>
			{
				constexpr auto operator()(const T* _begin, const T* _end) const noexcept
				{
					return _end - _begin;
				};
			};
		};

		template <typename RangeT, typename = iterator_t<remove_reference_t<RangeT>>>
		constexpr auto distance(RangeT&& _range) noexcept
		{
			const auto _begin = jc::ranges::begin(_range);
			const auto _end = jc::ranges::end(_range);
			return impl::distance_ftor<iterator_t<remove_reference_t<RangeT>>>{}(_begin, _end);
		};

		namespace impl
		{
			template <typename T>
			struct empty_view_part {};

			template <bool B, typename T>
			struct view_part { using type = std::conditional_t<B, T, empty_view_part<T>>; };
			template <bool B, typename T>
			using view_part_t = typename view_part<B, T>::type;


			/**
			 * @brief Implements the size function for a view
			 * @tparam T View type that is inheriting from this
			*/
			template <typename T>
			struct view_part_size
			{
			private:
				constexpr auto* as_crtp() noexcept
				{
					return static_cast<T*>(this);
				};
				constexpr const auto* as_crtp() const noexcept
				{
					return static_cast<const T*>(this);
				};
			public:
				constexpr auto size() const noexcept
				{
					return jc::ranges::distance(*this->as_crtp());
				};
			};




		};


		/**
		 * @brief CRTP base type implementing common view functionality
		 * @tparam T view type to make functions for
		*/
		template <typename T>
		struct view_interface :
			public impl::view_part_size<T>
		{
		private:
			constexpr auto* as_crtp() noexcept
			{
				return static_cast<T*>(this);
			};
			constexpr const auto* as_crtp() const noexcept
			{
				return static_cast<const T*>(this);
			};
		public:

		};
	};

#ifdef __cpp_concepts
	namespace ranges
	{
		template <typename T>
		concept range = is_range_v<T>;

		template <typename T>
		concept view = is_range_v<T> && std::is_move_constructible_v<T> && std::is_move_assignable_v<T>;
	};

	template <typename T>
	concept cx_range = ranges::range<T>;

	template <typename T>
	concept cx_view = ranges::view<T>;
#endif

	namespace ranges
	{
#ifdef __cpp_concepts
		template <typename T>
		struct is_view : bool_constant<view<T>> {};
#else
		template <typename T, typename = void>
		struct is_view : false_type {};
		template <typename T>
		struct is_view<T, enable_if_t<is_range<T>::value>> : true_type {};
#endif

#ifdef __cpp_inline_variables
		template <typename T>
		constexpr inline bool is_view_v = is_view<T>::value;
#endif
	};

	namespace ranges
	{
		namespace impl
		{
			template <typename UnderlyingT, typename OpT>
			struct condition_iterator
			{
			private:
				using underlying_type = UnderlyingT;
			public:
				using iterator_category = std::forward_iterator_tag;
				using difference_type = std::ptrdiff_t;

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
	};
}

namespace std
{
	template <typename T, typename OpT>
	struct iterator_traits<jc::ranges::impl::condition_iterator<T, OpT>>
	{
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::forward_iterator_tag;
		using reference = decltype(*std::declval<jc::ranges::impl::condition_iterator<T, OpT>>());
		using value_type = std::remove_reference_t<reference>;
		using pointer = value_type*;
	};
};

namespace jc
{
	namespace ranges
	{
#ifdef __cpp_concepts
		template <range RangeT, cx_invocable<value_t<RangeT>> OpT>
		struct filter_view
#else
		template <typename RangeT, typename OpT, typename = void>
		struct filter_view;

		template <typename RangeT, typename OpT>
		struct filter_view <RangeT, OpT,
			enable_if_t<is_range<RangeT>::value&& is_invocable<OpT, value_t<RangeT>>::value>
		>
#endif
			: public view_interface<filter_view<RangeT, OpT>>
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
				friend constexpr inline auto operator|(RangeT&& _range, filter_impl<OpT> _filter) ->
					ranges::filter_view<remove_reference_t<RangeT>, OpT>
				{
					return ranges::filter_view<remove_reference_t<RangeT>, OpT>{ _range, std::move(_filter.op) };
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








		constexpr static impl::filter_t filter{};

	};


	namespace ranges
	{
		template <typename T, typename = void>
		struct iota_view;

		namespace impl
		{
			template <typename T>
			struct iota_iterator
			{
			public:
				using iterator_category = std::bidirectional_iterator_tag;
				using difference_type = std::ptrdiff_t;

				constexpr bool operator==(const iota_iterator& rhs) const noexcept
				{
					return this->at_ == rhs.at_;
				};
				constexpr bool operator!=(const iota_iterator& rhs) const noexcept
				{
					return !(*this == rhs);
				};

				constexpr difference_type operator-(const iota_iterator& rhs) const noexcept
				{
					return (difference_type)(this->at_ - rhs.at_);
				};

				constexpr T& operator*() noexcept { return this->at_; };
				constexpr const T& operator*() const noexcept { return this->at_; };

				constexpr T* operator->() noexcept { return &this->at_; };
				constexpr const T* operator->() const noexcept { return &this->at_; };

				constexpr iota_iterator& operator++()
				{
					++this->at_;
					return *this;
				};
				constexpr iota_iterator operator++(int)
				{
					auto _out{ *this };
					++(*this);
					return _out;
				};
				constexpr iota_iterator& operator+=(difference_type _diff)
				{
					this->at_ += _diff;
					return *this;
				};

				constexpr iota_iterator& operator--()
				{
					--this->at_;
					return *this;
				};
				constexpr iota_iterator operator--(int)
				{
					auto _out{ *this };
					--(*this);
					return _out;
				};
				constexpr iota_iterator& operator-=(difference_type _diff)
				{
					return *this += -_diff;
				};

				constexpr iota_iterator() noexcept = default;
				constexpr iota_iterator(T _at) noexcept :
					at_{ _at }
				{};
			private:
				T at_{ 0 };
			};
		};

		template <typename T>
		struct iota_view<T, enable_if_t<is_integral<T>::value>>
		{
		public:
			using iterator = impl::iota_iterator<T>;
		private:
			iterator begin_;
			iterator end_;
		public:
			constexpr iterator begin() const noexcept { return this->begin_; };
			constexpr iterator end() const noexcept { return this->end_; };

			constexpr iota_view(T _init, T _max) noexcept :
				begin_{ _init },
				end_{ _max }
			{};
		};
	};
}

namespace std
{
	template <typename T>
	struct iterator_traits<jc::ranges::impl::iota_iterator<T>>
	{
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::forward_iterator_tag;
		using reference = decltype(*std::declval<jc::ranges::impl::iota_iterator<T>>());
		using value_type = std::remove_reference_t<reference>;
		using pointer = value_type*;
	};
};

namespace jc
{
	namespace views
	{
		namespace impl
		{
			struct iota_t
			{
				template <typename T>
				constexpr auto operator()(T _init, T _max) const noexcept -> ranges::iota_view<enable_if_t<is_integral<T>::value, T>>
				{
					return ranges::iota_view<T>{ _init, _max };
				};
			};
		};
		constexpr static impl::iota_t iota{};
	};

};

namespace jc
{
	namespace ranges
	{
		template <typename RangeT, typename = iterator_t<RangeT>>
		struct all_view : view_interface<all_view<RangeT>>
		{
		public:
			using iterator = iterator_t<RangeT>;
		private:
			iterator begin_;
			iterator end_;
		public:
			constexpr iterator begin() const noexcept { return this->begin_; };
			constexpr iterator end() const noexcept { return this->end_; };

			constexpr all_view(RangeT& _range) :
				begin_{ ranges::begin(_range) }, end_{ ranges::end(_range) }
			{};
		};
	};
	namespace views
	{
		namespace impl
		{
			struct all_t
			{
				template <typename RangeT>
				constexpr auto operator()(RangeT&& _range) const noexcept -> ranges::all_view<remove_reference_t<RangeT>>
				{
					return ranges::all_view<remove_reference_t<RangeT>>(_range);
				};

				template <typename RangeT>
				constexpr friend inline auto operator|(RangeT&& _range, const all_t& _all) noexcept ->
					ranges::all_view<remove_reference_t<RangeT>>
				{
					return _all(_range);
				};
			};
		};
		constexpr static impl::all_t all{};
	};
};

namespace jc
{
	namespace ranges
	{
		template <typename RangeT, typename = iterator_t<RangeT>>
		struct drop_view : view_interface<drop_view<RangeT>>
		{
		public:
			using iterator = iterator_t<RangeT>;
		private:
			iterator begin_;
			iterator end_;
		public:
			constexpr iterator begin() const noexcept { return this->begin_; };
			constexpr iterator end() const noexcept { return this->end_; };
			constexpr drop_view(RangeT& _range, size_t _count) :
				begin_{ jc::next(ranges::begin(_range), static_cast<jc::difference_type_t<RangeT>>(_count)) }, end_{ ranges::end(_range) }
			{};
		};
	};
	namespace views
	{
		namespace impl
		{
			struct drop_impl_t
			{
				template <typename RangeT>
				constexpr friend inline auto operator|(RangeT&& _range, drop_impl_t _drop) noexcept ->
					ranges::drop_view<remove_reference_t<RangeT>>
				{
					return ranges::drop_view<remove_reference_t<RangeT>>{ std::forward<RangeT>(_range), _drop.count_ };
				};
				size_t count_;
			};

			struct drop_t
			{
				constexpr drop_impl_t operator()(size_t _count) const noexcept
				{
					return drop_impl_t{ _count };
				};
				template <typename RangeT>
				constexpr auto operator()(RangeT&& _range, size_t _count) const noexcept
				{
					return _range | drop_impl_t{ _count };
				};
			};

		};
		constexpr static impl::drop_t drop{};
	};
};

namespace jc
{
	namespace ranges
	{
		template <typename IterT, typename = void>
		struct iter_view;

		template <typename IterT>
		struct iter_view <IterT, enable_if_t<is_iterator<IterT>::value>>
			: public view_interface<iter_view<IterT>>
		{
			using iterator = IterT;
		private:
			iterator begin_;
			iterator end_;
		public:
			JCLIB_CONSTEXPR iterator begin() const noexcept { return this->begin_; };
			JCLIB_CONSTEXPR iterator end() const noexcept { return this->end_; };

			JCLIB_CONSTEXPR iter_view(IterT _begin, IterT _end) noexcept :
				begin_{ _begin }, end_{ _end }
			{};
		};
	};
	namespace views
	{
		namespace impl
		{
			struct iter_t
			{
				template <typename IterT>
				constexpr ranges::iter_view<IterT> operator()(IterT _begin, IterT _end) const noexcept
				{
					return ranges::iter_view<IterT>{ _begin, _end };
				};
				template <typename RangeT>
				constexpr ranges::iter_view<ranges::iterator_t<RangeT>> operator()(RangeT&& _range) const noexcept
				{
					return ranges::iter_view<ranges::iterator_t<RangeT>>
					{
						ranges::begin(_range),
							ranges::end(_range)
					};
				};
			};
		};
		constexpr static impl::iter_t iter{};
	};
};






#endif