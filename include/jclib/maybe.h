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
	struct bad_access_exception : public jc::exception { using jc::exception::exception; };

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
		struct maybe_base
		{
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

			/*
				Union member access without run time checking
			*/

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


			constexpr void set_has_value(bool _to) noexcept
			{
				this->has_value_ = _to;
			};

			constexpr inline void destroy_value()
				noexcept(noexcept(jc::destroy_at(std::declval<value_type&>())))
			{
				jc::destroy_at(this->unsafe_value());
			};

			constexpr inline void destroy_alternate()
				noexcept(noexcept(jc::destroy_at(std::declval<alternate_type&>())))
			{
				jc::destroy_at(this->unsafe_alternate());
			};
			constexpr inline void destroy() noexcept
				(
					noexcept(std::declval<maybe_base>().has_value()) &&
					noexcept(std::declval<maybe_base>().destroy_value()) &&
					noexcept(std::declval<maybe_base>().destroy_alternate())
				)
			{
				if (this->has_value())
				{
					this->destroy_value();
				}
				else
				{
					this->destroy_alternate();
				};
			};

		public:

			/**
			 * @brief Checks if the value_type is the active member.
			 * 
			 * @return True if value_type is the active member, false otherwise.
			*/
			constexpr bool has_value() const noexcept
			{
				return this->has_value_;
			};

			/**
			 * @brief Checks if the value_type is the active member.
			 * 
			 * Same as calling maybe::has_value().
			 * 
			 * @return True if value_type is the active member, false otherwise.
			*/
			constexpr explicit operator bool() const noexcept
			{
				return this->has_value();
			};

			/**
			 * @brief Gets the held value, moving it out.
			 * 
			 * This is only called when invoking value() on a temporary.
			 * Does not throw exceptions.
			 * 
			 * @param _nt No throw tag type.
			 * 
			 * @return Held value by move.
			*/
			constexpr value_type value(nothrow_t _nt) && noexcept
			{
				if (!this->has_value())
				{
					JCLIB_ABORT();
				};
				return std::move(this->unsafe_value());
			};
			
			/**
			 * @brief Gets a reference to the held value.
			 * 
			 * Does not throw exceptions.
			 * 
			 * @param _nt No throw tag type.
			 * 
			 * @return Reference to the held value.
			*/
			constexpr reference value(nothrow_t) & noexcept
			{
				if (!this->has_value())
				{
					JCLIB_ABORT();
				};
				return this->unsafe_value();
			};
					
			/**
			 * @brief Gets a reference to the held value.
			 * 
			 * Does not throw exceptions.
			 * 
			 * @param _nt No throw tag type.
			 * 
			 * @return Reference to the held value.
			*/
			constexpr const_reference value(nothrow_t) const & noexcept
			{
				if (!this->has_value())
				{
					JCLIB_ABORT();
				};
				return this->unsafe_value();
			};

			/**
			 * @brief Gets the held value, moving it out.
			 *
			 * This is only called when invoking value() on a temporary.
			 * This will only throw if jclib's exception usage is enabled. 
			 *
			 * @return Held value by move.
			 * 
			 * @exception bad_access_exception Thrown if the value type is not the active member.
			*/
			constexpr value_type value() && JCLIB_NOEXCEPT
			{
				if (!this->has_value())
				{
					JCLIB_THROW(bad_access_exception("value is not the active member"));
				};
				return std::move(this->unsafe_value());
			};

			/**
			 * @brief Gets a reference to the held value.
			 *
			 * This will only throw if jclib's exception usage is enabled. 
			 * 
			 * @return Reference to the held value.
			 * 
			 * @exception bad_access_exception Thrown if the value type is not the active member.
			*/
			constexpr reference value() & JCLIB_NOEXCEPT
			{
				if (!this->has_value())
				{
					JCLIB_THROW(bad_access_exception("value is not the active member"));
				};
				return this->unsafe_value();
			};

			/**
			 * @brief Gets a reference to the held value.
			 *
			 * This will only throw if jclib's exception usage is enabled. 
			 * 
			 * @return Reference to the held value.
			 * 
			 * @exception bad_access_exception Thrown if the value type is not the active member.
			*/
			constexpr const_reference value() const & JCLIB_NOEXCEPT
			{
				if (!this->has_value())
				{
					JCLIB_THROW(bad_access_exception("value is not the active member"));
				};
				return this->unsafe_value();
			};

			/**
			 * @brief Gets the held alternate value, moving it out.
			 * 
			 * This is only called when invoking alternate() on a temporary.
			 * Does not throw exceptions.
			 * 
			 * @param _nt No throw tag type.
			 * 
			 * @return Held alternate value by move.
			*/
			constexpr alternate_type alternate(nothrow_t) && noexcept
			{
				if (this->has_value())
				{
					JCLIB_ABORT();
				};
				return std::move(this->unsafe_alternate());
			};

			/**
			 * @brief Gets a reference to the held alternate value.
			 * 
			 * Does not throw exceptions.
			 * 
			 * @param _nt No throw tag type.
			 * 
			 * @return Reference to the held alternate value.
			*/
			constexpr alternate_type& alternate(nothrow_t) & noexcept
			{
				if (this->has_value())
				{
					JCLIB_ABORT();
				};
				return this->unsafe_alternate();
			};

			/**
			 * @brief Gets a reference to the held alternate value.
			 * 
			 * Does not throw exceptions.
			 * 
			 * @param _nt No throw tag type.
			 * 
			 * @return Reference to the held alternate value.
			*/
			constexpr const alternate_type& alternate(nothrow_t) const & noexcept
			{
				if (this->has_value())
				{
					JCLIB_ABORT();
				};
				return this->unsafe_alternate();
			};

			/**
			 * @brief Gets the held alternate value, moving it out.
			 *
			 * This is only called when invoking alternate() on a temporary.
			 * This will only throw if jclib's exception usage is enabled. 
			 *
			 * @return Held alternate value by move.
			 * 
			 * @exception bad_access_exception Thrown if the alternate type is not the active member.
			*/
			constexpr alternate_type alternate() && JCLIB_NOEXCEPT
			{
				if (this->has_value())
				{
					JCLIB_THROW(bad_access_exception("alternate is not the active member"));
				};
				return std::move(this->unsafe_alternate());			
			};

			/**
			 * @brief Gets a reference to the held alternate value.
			 *
			 * This will only throw if jclib's exception usage is enabled. 
			 * 
			 * @return Reference to the held alternate value.
			 * 
			 * @exception bad_access_exception Thrown if the alternate type is not the active member.
			*/
			constexpr alternate_type& alternate() & JCLIB_NOEXCEPT
			{
				if (this->has_value())
				{
					JCLIB_THROW(bad_access_exception("alternate is not the active member"));
				};
				return this->unsafe_alternate();
			};
			
			/**
			 * @brief Gets a reference to the held alternate value.
			 *
			 * This will only throw if jclib's exception usage is enabled. 
			 * 
			 * @return Reference to the held alternate value.
			 * 
			 * @exception bad_access_exception Thrown if the alternate type is not the active member.
			*/
			constexpr const alternate_type& alternate() const & JCLIB_NOEXCEPT
			{
				if (this->has_value())
				{
					JCLIB_THROW(bad_access_exception("alternate is not the active member"));
				};
				return this->unsafe_alternate();
			};

		protected:

			constexpr void set_value(const value_type& v) noexcept
				(
					noexcept(std::declval<value_type>() = std::declval<const value_type&>()) &&
					noexcept(std::declval<maybe_base>().destroy_alternate())
				)
			{
				if (!this->has_value())
				{
					this->destroy_alternate();
				};
				this->value_ = v;
				this->set_has_value(true);
			};
			constexpr void set_value(value_type&& v) noexcept
				(
					noexcept(std::declval<maybe_base>().destroy_alternate())
				)
			{
				if (!this->has_value())
				{
					this->destroy_alternate();
				};
				this->value_ = std::move(v);
				this->set_has_value(true);
			};

			constexpr void set_alternate(const alternate_type& a) noexcept
				(
					noexcept(std::declval<alternate_type>() = std::declval<const alternate_type&>()) &&
					noexcept(std::declval<maybe_base>().destroy_value())
				)
			{
				if (this->has_value())
				{
					this->destroy_value();
				};
				this->alt_ = a;
				this->set_has_value(false);
			};
			constexpr void set_alternate(alternate_type&& a) noexcept
				(
					noexcept(std::declval<maybe_base>().destroy_value())
				)
			{
				if (this->has_value())
				{
					this->destroy_value();
				};
				this->alt_ = std::move(a);
				this->set_has_value(false);
			};

		public:
		
			/**
			 * @brief Gets the held value, moving it out.
			 *
			 * This is only called when invoking value() on a temporary.
			 * This will only throw if jclib's exception usage is enabled. 
			 * Same as maybe::value().
			 *
			 * @return Held value by move.
			 * 
			 * @exception bad_access_exception Thrown if the value type is not the active member.
			*/
			constexpr value_type operator*() && JCLIB_NOEXCEPT
			{
				return static_cast<maybe_base&&>(*this).value();
			};

			/**
			 * @brief Gets a reference to the held value.
			 *
			 * This will only throw if jclib's exception usage is enabled. 
			 * Same as maybe::value().
			 *
			 * @return Reference to the held value.
			 * 
			 * @exception bad_access_exception Thrown if the value type is not the active member.
			*/
			constexpr reference operator*() & JCLIB_NOEXCEPT
			{
				return this->value();
			};

			/**
			 * @brief Gets a reference to the held value.
			 *
			 * This will only throw if jclib's exception usage is enabled. 
			 * Same as maybe::value().
			 *
			 * @return Reference to the held value.
			 * 
			 * @exception bad_access_exception Thrown if the value type is not the active member.
			*/
			constexpr const_reference operator*() const & JCLIB_NOEXCEPT
			{
				return this->value();
			};
			
			/**
			 * @brief Gets a pointer to the held value.
			 *
			 * This will only throw if jclib's exception usage is enabled. 
			 * Same as "&maybe::value()".
			 *
			 * @return Pointer to the held value.
			 * 
			 * @exception bad_access_exception Thrown if the value type is not the active member.
			*/
			constexpr pointer operator->() JCLIB_NOEXCEPT
			{
				return &this->value();
			};
						
			/**
			 * @brief Gets a pointer to the held value.
			 *
			 * This will only throw if jclib's exception usage is enabled. 
			 * Same as "&maybe::value()".
			 *
			 * @return Pointer to the held value.
			 * 
			 * @exception bad_access_exception Thrown if the value type is not the active member.
			*/
			constexpr const_pointer operator->() const JCLIB_NOEXCEPT
			{
				return &this->value();
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
						this->destroy_alternate();
					};
					this->value_ = other.unsafe_value();
					this->has_value_ = true;
				}
				else
				{
					if (this->has_value())
					{
						this->destroy_value();
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
						this->destroy_alternate();
					};
					this->value_ = std::move(other.unsafe_value());
					this->has_value_ = true;
				}
				else
				{
					if (this->has_value())
					{
						this->destroy_value();
					};
					this->alt_ = std::move(other.unsafe_alternate());
					this->has_value_ = false;
				};
				return *this;
			};

			~maybe_base() noexcept
			{
				this->destroy();
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
	struct maybe<T, AltT,
		enable_if_t<jc::is_convertible<AltT, T>::value>>
		: public impl::maybe_base<T, AltT>
	{
	public:
		using impl::maybe_base<T, AltT>::maybe_base;
		using impl::maybe_base<T, AltT>::operator=;
	};

	template <typename T, typename AltT>
	struct maybe<T, AltT,
		enable_if_t<!jc::is_convertible<AltT, T>::value>>
		: public impl::maybe_base<T, AltT>
	{
	private:
		using parent_type = impl::maybe_base<T, AltT>;
	public:
		using value_type = typename parent_type::value_type;
		using alternate_type = typename parent_type::alternate_type;



		constexpr maybe(const value_type& _val) :
			impl::maybe_base<T, AltT>{ _val }
		{};
		constexpr maybe(value_type&& _val) noexcept :
			impl::maybe_base<T, AltT>{ std::move(_val) }
		{};

		constexpr maybe& operator=(const value_type& _val)
		{
			parent_type::set_value(_val);
			return *this;
		};
		constexpr maybe& operator=(value_type&& _val) noexcept
		{
			parent_type::set_value(std::move(_val));
			return *this;
		};



		constexpr maybe(alternate_t, const alternate_type& _val) :
			impl::maybe_base<T, AltT>{ jc::alternate, _val }
		{};
		constexpr maybe(alternate_t, alternate_type&& _val) noexcept :
			impl::maybe_base<T, AltT>{ jc::alternate, std::move(_val) }
		{};

		constexpr maybe(const alternate_type& _val) :
			maybe{ jc::alternate, _val }
		{};
		constexpr maybe(alternate_type&& _val) noexcept :
			maybe{ jc::alternate, std::move(_val) }
		{};

		constexpr maybe& operator=(const alternate_type& _val)
		{
			parent_type::set_alternate(_val);
			return *this;
		};
		constexpr maybe& operator=(alternate_type&& _val) noexcept
		{
			parent_type::set_alternate(std::move(_val));
			return *this;
		};

		using impl::maybe_base<T, AltT>::operator=;
	};

};

#endif