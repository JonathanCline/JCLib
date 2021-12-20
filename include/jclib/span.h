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

/*
	Implements a simple C++20-ish span type for referring to contiguous ranges.
*/

#include <jclib/ranges.h>
#include <jclib/feature.h>

#if JCLIB_FEATURE_SPAN_V
// Include standard span if available
#include <span>
#endif

#include <limits>

#if JCLIB_FEATURE_BYTE_V
// Include for std::byte if possible
#include <cstddef>
#endif

// Lets do this

#define _JCLIB_SPAN_

namespace jc
{
	// Define the dynamic extent size value
#if JCLIB_FEATURE_SPAN_V

	/**
	 * @brief The extent size value for dynamic extent span.
	*/
	constexpr size_t dynamic_extent = std::dynamic_extent;

#else

	/**
	 * @brief The extent size value for dynamic extent span.
	*/
	constexpr size_t dynamic_extent = std::numeric_limits<size_t>::max();

#endif


	namespace impl
	{
		/**
		 * @brief Implements the core data for a span, encapsulating the Extent handling.
		 * @tparam T Span value type.
		 * @tparam Extent Span size, may be dynamic_extent.
		*/
		template <typename T, size_t Extent = dynamic_extent>
		struct span_base
		{
		public:

			/**
			 * @brief Gets the size of the span.
			 * @return Size in elements.
			*/
			constexpr size_t size() const noexcept
			{
				return this->size_;
			};

			/**
			 * @brief Gets the pointer to the span's first element.
			 * @return Pointer to first element.
			*/
			constexpr T* data() const noexcept
			{
				return this->data_;
			};

			/**
			 * @brief Constructs an empty span
			*/
			constexpr span_base() noexcept :
				data_{ nullptr }
			{};

			/**
			 * @brief Constructs the span using an iterator size pair.
			 * @param _begin Pointer to the beginning of the array. MUST NOT BE NULL.
			 * @param _size Size of the array in elements.
			*/
			template <typename IterT>
			JCLIB_REQUIRES((jc::is_iterator_to<IterT, T>::value))
			constexpr explicit span_base(IterT _begin, size_t _size) noexcept :
				data_{ &*_begin }
			{
				JCLIB_ASSERT(_size == Extent)
			};

			/**
			 * @brief Constructs the span using an iterator size pair.
			 * @param _begin Pointer to the beginning of the array. MUST NOT BE NULL.
			 * @param _size Size of the array in elements.
			*/
			template <typename IterT>
			JCLIB_REQUIRES((jc::is_iterator_to<IterT, T>::value))
				constexpr explicit span_base(IterT _begin, IterT _end) noexcept :
				span_base{ _begin, static_cast<size_t>(jc::distance(_begin, _end)) }
			{};

			/**
			 * @brief Sets the span to view into a contiguous range.
			 * @tparam RangeT Type of the range to view, must be contiguous.
			 * @param _range Range to view.
			*/
			template <typename RangeT, typename =
				jc::enable_if_t<jc::is_contiguous_range<RangeT>::value>
			>
				constexpr span_base(RangeT& _range) noexcept :
				data_{ &*jc::begin(_range) }
			{
				JCLIB_ASSERT(jc::ranges::distance(_range) == Extent);
			};

		private:

			/**
			 * @brief Pointer to the beginning of the span.
			*/
			T* data_;

		};

		/**
		 * @brief Implements the core data for a span, encapsulating the Extent handling.
		 * 
		 * This is the runtime-provided (dynamic_extent) specialization.
		 * 
		 * @tparam T Span value type.
		 * @tparam Extent Span size, may be dynamic_extent.
		*/
		template <typename T>
		struct span_base<T, dynamic_extent>
		{
		public:

			/**
			 * @brief Gets the size of the span.
			 * @return Size in elements.
			*/
			constexpr size_t size() const noexcept
			{
				return this->size_;
			};

			/**
			 * @brief Gets the pointer to the span's first element.
			 * @return Pointer to first element.
			*/
			constexpr T* data() const noexcept
			{
				return this->data_;
			};

			/**
			 * @brief Constructs an empty span
			*/
			constexpr span_base() noexcept :
				data_{ nullptr }, size_{ 0 }
			{};

			/**
			 * @brief Constructs the span using an iterator size pair.
			 * @param _begin Pointer to the beginning of the array. MUST NOT BE NULL.
			 * @param _size Size of the array in elements.
			*/
			template <typename IterT>
			JCLIB_REQUIRES((jc::is_iterator_to<IterT, T>::value))
			constexpr span_base(IterT _begin, size_t _size) noexcept :
				data_{ &*_begin }, size_{ _size }
			{};

			/**
			 * @brief Constructs the span using an iterator size pair.
			 * @param _begin Pointer to the beginning of the array. MUST NOT BE NULL.
			 * @param _size Size of the array in elements.
			*/
			template <typename IterT>
			JCLIB_REQUIRES((jc::is_iterator_to<IterT, T>::value))
			constexpr span_base(IterT _begin, IterT _end) noexcept :
				span_base{ _begin, static_cast<size_t>(jc::distance(_begin, _end)) }
			{};

			/**
			 * @brief Sets the span to view into a contiguous range.
			 * @tparam RangeT Type of the range to view, must be contiguous.
			 * @param _range Range to view.
			*/
			template <typename RangeT, typename =
				jc::enable_if_t<jc::is_contiguous_range<RangeT>::value>
			>
			constexpr span_base(RangeT& _range) noexcept :
				data_{ &*jc::begin(_range) }, size_{ static_cast<size_t>(jc::ranges::distance(_range)) }
			{};

		private:

			/**
			 * @brief Pointer to the beginning of the span.
			*/
			T* data_;

			/**
			 * @brief The size of this span.
			*/
			size_t size_;
		};

	};

	/**
	 * @brief View into a contiguous range.
	 * @tparam T Type in the range.
	 * @tparam Extent The size of the span, defaults to dynamic_extent (provided at runtime).
	*/
	template <typename T, size_t Extent = dynamic_extent>
	struct span : public impl::span_base<T, Extent>
	{
	private:

		/**
		 * @brief Parent type implementing span's data members
		*/
		using parent_type = impl::span_base<T, Extent>;
	
	public:

		// Standard container aliases

		using value_type = T;
		using pointer = value_type*;
		using reference = value_type&;

		/**
		 * @brief The type used to store the size of the span
		*/
		using size_type = size_t;

		/**
		 * @brief Checks if this is an empty span (size == 0)
		 * @return True if empty, false otherwise.
		*/
		constexpr bool empty() const noexcept
		{
			return this->size() == 0;
		};
		
		/**
		 * @brief Gets the size of the span in bytes.
		 * @return Size in bytes.
		*/
		constexpr size_type size_bytes() const noexcept
		{
			return this->size() * sizeof(value_type);
		};

		/**
		 * @brief Gets the element at an index.
		 * @param _index Index to get.
		 * @return Reference to the element.
		*/
		constexpr reference at(size_type _index) const
		{
			JCLIB_ASSERT(_index < this->size());
			JCLIB_ASSERT(!this->empty());
			return *(this->data() + _index);
		};

		/**
		 * @brief Gets the element at an index.
		 * @param _index Index to get.
		 * @return Reference to the element.
		*/
		constexpr reference operator[](size_type _index) const
		{
			return this->at(_index);
		};

		/**
		 * @brief Gets a reference to the first element in the span.
		 * @return First element reference.
		*/
		constexpr reference front() const
		{
			return this->at(0);
		};

		/**
		 * @brief Gets a reference to the last element in the span.
		 * @return Last element reference.
		*/
		constexpr reference back() const
		{
			return this->at(this->size() - 1);
		};

		/**
		 * @brief Gets a span containing the first n elements.
		 * @param _count Number of elements to get.
		 * @return Span with first elements.
		*/
		constexpr span first(size_type _count) const
		{
			JCLIB_ASSERT(_count <= this->size());
			return span{ this->data(), _count };
		};

		/**
		 * @brief Gets a span containing the last n elements.
		 * @param _count Number of elements to get.
		 * @return Span with last elements.
		*/
		constexpr span last(size_type _count) const
		{
			JCLIB_ASSERT(_count <= this->size());
			const auto _begin = this->data() + _count;
			return span{ _begin, this->size() - _count };
		};

		/**
		 * @brief Obtains a subspan into this span.
		 * @param _offset Offset from the beginning.
		 * @param _count Number of elements for the subspan.
		 * @return Subspan into this span.
		*/
		constexpr span subspan(size_type _offset, size_type _count) const
		{
			JCLIB_ASSERT(_offset <= this->size());
			JCLIB_ASSERT(_count <= this->size());
			JCLIB_ASSERT(_offset + _count <= this->size());
			return span{ this->data() + _offset, _count };
		};


		// Pull down constructors
		using parent_type::parent_type;

	};



	// Add span of bytes conversions if possible

#if JCLIB_FEATURE_BYTE_V

	/**
	 * @brief Creates a span of const bytes from another span.
	 * @tparam T Type held by span.
	 * @tparam Extent Extent of the span.
	 * @param _span The span to get as bytes.
	 * @return Span over the same data as const bytes.
	*/
	template <typename T, size_t Extent>
	constexpr inline span<const std::byte, Extent * sizeof(T)> as_bytes(const span<T, Extent>& _span)
	{
		return span<const std::byte, Extent>
		{
			reinterpret_cast<const std::byte*>(_span.data()), _span.size_bytes()
		};
	};

	/**
	 * @brief Creates a span of non-const bytes from another span.
	 * @tparam T Type held by span.
	 * @tparam Extent Extent of the span.
	 * @param _span The span to get as bytes.
	 * @return Span over the same data as non-const bytes.
	*/
	template <typename T, size_t Extent>
	JCLIB_REQUIRES(!std::is_const<T>::value)
	constexpr inline auto as_writable_bytes(const span<T, Extent>& _span) ->
	JCLIB_RET_SFINAE_CXSWITCH(span JCLIB_TARGS(std::byte, Extent * sizeof(T)), !std::is_const<T>::value)
	{
		return span<std::byte, Extent>
		{
			reinterpret_cast<std::byte*>(_span.data()), _span.size_bytes()
		};
	};


#endif


};

#endif