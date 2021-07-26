#pragma once
#ifndef JCLIB_MAYBE_H
#define JCLIB_MAYBE_H

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
#include "jclib/memory.h"
#include "jclib/exception.h"

#define _JCLIB_MAYBE_

namespace jc
{
	/**
	 * @brief Exception thrown on attempted access on an invalid value
	*/
	struct bad_access_exception : public std::exception { using std::exception::exception; };

	/**
	 * @brief Alternate tag type for function overload selection
	*/
	struct alternate_t { constexpr explicit alternate_t() noexcept = default; };
	
	/**
	 * @brief Alternate tag value for function overload selection
	*/
	constexpr static alternate_t alternate{};

	namespace impl
	{
		template <typename MaybeT>
		struct maybe_part
		{
		public:
			~maybe_part() = default;
		protected:
			constexpr inline auto* as_crtp() noexcept { return static_cast<MaybeT*>(this); };
			constexpr inline const auto* as_crtp() const noexcept { return static_cast<const MaybeT*>(this); };
		};

		template <typename MaybeT, typename T, typename AltT>
		struct maybe_destructor_nontrivial : private maybe_part<MaybeT>
		{
		protected:
			constexpr inline void destroy_value() noexcept
			{
				jc::destroy_at(this->as_crtp()->unsafe_value());
			};
			constexpr inline void destroy_alternate() noexcept
			{
				jc::destroy_at(this->as_crtp()->unsafe_alternate());
			};
			constexpr inline void destroy()
			{
				const auto _crtp = this->as_crtp();
				if (_crtp->has_value())
				{
					this->destroy_value();
				}
				else
				{
					this->destroy_alternate();
				};
			};
		public:
			~maybe_destructor_nontrivial() noexcept
			{
				this->destroy();
			};
		};

		template <typename MaybeT, typename T, typename AltT>
		struct maybe_destructor_trivial : private maybe_part<MaybeT>
		{
		protected:
			constexpr inline void destroy_value() noexcept
			{
				
			};
			constexpr inline void destroy_alternate() noexcept
			{

			};
			constexpr inline void destroy()
			{
				
			};
		public:
			~maybe_destructor_trivial() = default;
		};

		template <typename MaybeT, typename T, typename AltT>
		struct maybe_destructor
		{
			using type = std::conditional_t
				<
					jc::is_trivially_destructible<T>::value && jc::is_trivially_destructible<AltT>::value,
					maybe_destructor_trivial<MaybeT, T, AltT>,
					maybe_destructor_nontrivial<MaybeT, T, AltT>
				>;
		};
		template <typename MaybeT, typename T, typename AltT>
		using maybe_destructor_t = typename maybe_destructor<MaybeT, T, AltT>::type;

		template <typename T, typename AltT>
		struct maybe_base : public impl::maybe_destructor_t<maybe_base<T, AltT>, T, AltT>
		{
		private:
			using maybe_destructor_type = impl::maybe_destructor_t<maybe_base<T, AltT>, T, AltT>;
			friend maybe_destructor_type;

		public:
			using value_type = T;
			using pointer = value_type*;
			using reference = value_type&;
			using const_pointer = const value_type*;
			using const_reference = const value_type&;

			using alternate_type = AltT;

		private:
			using this_type = maybe_base<value_type, alternate_type>;

		protected:
			constexpr value_type& unsafe_value() noexcept
			{
				JCLIB_ASSERT(this->has_value());
				return this->value_;
			};
			constexpr const value_type& unsafe_value() const noexcept
			{
				JCLIB_ASSERT(this->has_value());
				return this->value_;
			};
			
			constexpr alternate_type& unsafe_alternate() noexcept
			{
				JCLIB_ASSERT(!this->has_value());
				return this->alt_;
			};
			constexpr const alternate_type& unsafe_alternate() const noexcept
			{
				JCLIB_ASSERT(!this->has_value());
				return this->alt_;
			};

			constexpr void set_has_value(bool _to)
			{
				this->has_value_ = _to;
			};

		public:
			constexpr bool has_value() const noexcept
			{
				return this->has_value_;
			};
			constexpr explicit operator bool() const noexcept
			{
				return this->has_value();
			};

			constexpr value_type& value(nothrow_t) noexcept
			{
				if (!this->has_value())
				{
					JCLIB_ABORT();
				};
				return this->unsafe_value();
			};
			constexpr const value_type& value(nothrow_t) const noexcept
			{
				if (!this->has_value())
				{
					JCLIB_ABORT();
				};
				return this->unsafe_value();
			};

			constexpr value_type& value() noexcept(!JCLIB_EXCEPTIONS)
			{
#if JCLIB_EXCEPTIONS
				if (!this->has_value())
				{
					throw bad_access_exception{ "value is not the active member" };
				};
				return this->unsafe_value();
#else
				return this->value(nothrow);
#endif
			};
			constexpr const value_type& value() const noexcept(!JCLIB_EXCEPTIONS)
			{
#if JCLIB_EXCEPTIONS
				if (!this->has_value())
				{
					throw bad_access_exception{ "value is not the active member" };
				};
				return this->unsafe_value();
#else
				return this->value(nothrow);
#endif
			};

			constexpr value_type& operator*() noexcept(!JCLIB_EXCEPTIONS)
			{
				return this->value();
			};
			constexpr const value_type& operator*() const noexcept(!JCLIB_EXCEPTIONS)
			{
				return this->value();
			};

			constexpr value_type* operator->() noexcept(!JCLIB_EXCEPTIONS)
			{
				return &this->value();
			};
			constexpr const value_type* operator->() const noexcept(!JCLIB_EXCEPTIONS)
			{
				return &this->value();
			};

			constexpr alternate_type& alternate(nothrow_t) noexcept
			{
				if (this->has_value())
				{
					JCLIB_ABORT();
				};
				return this->unsafe_alternate();
			};
			constexpr const alternate_type& alternate(nothrow_t) const noexcept
			{
				if (this->has_value())
				{
					JCLIB_ABORT();
				};
				return this->unsafe_alternate();
			};

			constexpr alternate_type& alternate() noexcept(!JCLIB_EXCEPTIONS)
			{
#if JCLIB_EXCEPTIONS
				if (this->has_value())
				{
					throw bad_access_exception{ "alternate is not the active member" };
				};
				return this->unsafe_alternate();
#else
				return this->alternate(nothrow);
#endif
			};
			constexpr const alternate_type& alternate() const noexcept(!JCLIB_EXCEPTIONS)
			{
#if JCLIB_EXCEPTIONS
				if (this->has_value())
				{
					throw bad_access_exception{ "alternate is not the active member" };
				};
				return this->unsafe_alternate();
#else
				return this->alternate(nothrow);
#endif
			};

			constexpr maybe_base() = default;

			constexpr explicit maybe_base(const value_type& _value)
				noexcept(noexcept(value_type{ std::declval<const value_type&>() })) :
				value_{ _value }, has_value_{ true }
			{};
			constexpr explicit maybe_base(value_type&& _value) noexcept :
				value_{ std::move(_value) }, has_value_{ true }
			{};

			constexpr explicit maybe_base(alternate_t, const alternate_type& _alt)
				noexcept(noexcept(alternate_type{ std::declval<const alternate_type&>() })) :
				alt_{ _alt }, has_value_{ false }
			{};
			constexpr explicit maybe_base(alternate_t, alternate_type&& _alt) noexcept :
				alt_{ std::move(_alt) }, has_value_{ false }
			{};

			constexpr maybe_base(const maybe_base& other)
			{
				if (other.has_value())
				{
					this->value_ = other.unsafe_value();
					this->has_value_ = true;
				}
				else
				{
					this->alt_ = other.unsafe_alternate();
					this->has_value_ = false;
				};
			};
			constexpr maybe_base& operator=(const maybe_base& other)
			{
				if (other.has_value())
				{
					if (!this->has_value())
					{
						maybe_destructor_type::destroy_alternate();
					};
					this->value_ = other.unsafe_value();
					this->has_value_ = true;
				}
				else
				{
					if (this->has_value())
					{
						maybe_destructor_type::destroy_value();
					};
					this->alt_ = other.unsafe_alternate();
					this->has_value_ = false;
				};
				return *this;
			};

			constexpr maybe_base(maybe_base&& other) noexcept
			{
				if (other.has_value())
				{
					this->value_ = std::move(other.unsafe_value());
					this->has_value_ = true;
				}
				else
				{
					this->alt_ = std::move(other.unsafe_alternate());
					this->has_value_ = false;
				};
			};
			constexpr maybe_base& operator=(maybe_base&& other) noexcept
			{
				if (other.has_value())
				{
					if (!this->has_value())
					{
						maybe_destructor_type::destroy_alternate();
					};
					this->value_ = std::move(other.unsafe_value());
					this->has_value_ = true;
				}
				else
				{
					if (this->has_value())
					{
						maybe_destructor_type::destroy_value();
					};
					this->alt_ = std::move(other.unsafe_alternate());
					this->has_value_ = false;
				};
				return *this;
			};


		private:
			union
			{
				value_type value_;
				alternate_type alt_;
			};
			bool has_value_;
		};

	};

	template <typename T, typename AltT, typename = void>
	struct maybe;

	template <typename T, typename AltT>
	struct maybe<T, AltT, enable_if_t<jc::is_convertible<AltT, T>::value>>:
		public impl::maybe_base<T, AltT>
	{
		using impl::maybe_base<T, AltT>::maybe_base;
		using impl::maybe_base<T, AltT>::operator=;
	};

	template <typename T, typename AltT>
	struct maybe<T, AltT, enable_if_t<!jc::is_convertible<AltT, T>::value>> :
		public impl::maybe_base<T, AltT>
	{
	private:
		using parent_type = impl::maybe_base<T, AltT>;
	public:
		constexpr maybe(const T& _val) :
			impl::maybe_base<T, AltT>{ _val }
		{};
		constexpr maybe(T&& _val) noexcept :
			impl::maybe_base<T, AltT>{ std::move(_val) }
		{};

		constexpr maybe& operator=(const T& _val)
		{
			if (!parent_type::has_value())
			{
				parent_type::destroy_alternate();
			};
			parent_type::unsafe_value() = _val;
			return *this;
		};
		constexpr maybe& operator=(T&& _val) noexcept
		{
			if (!parent_type::has_value())
			{
				parent_type::destroy_alternate();
			};
			parent_type::unsafe_value() = std::move(_val);
			return *this;
		};

		constexpr maybe(alternate_t, const AltT& _val) :
			impl::maybe_base<T, AltT>{ jc::alternate, _val }
		{};
		constexpr maybe(alternate_t, AltT&& _val) noexcept :
			impl::maybe_base<T, AltT>{ jc::alternate, std::move(_val) }
		{};

		constexpr maybe(const AltT& _val) :
			maybe{ jc::alternate, _val }
		{};
		constexpr maybe(AltT&& _val) noexcept :
			maybe{ jc::alternate, std::move(_val) }
		{};

		constexpr maybe& operator=(const AltT& _val)
		{
			if (parent_type::has_value())
			{
				parent_type::destroy_value();
			};
			parent_type::unsafe_alternate() = _val;
			return *this;
		};
		constexpr maybe& operator=(AltT&& _val) noexcept
		{
			if (parent_type::has_value())
			{
				parent_type::destroy_value();
			};
			parent_type::unsafe_alternate() = std::move(_val);
			return *this;
		};

		using impl::maybe_base<T, AltT>::operator=;
	};

};

#endif