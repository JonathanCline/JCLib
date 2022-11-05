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
		/**
		 * @brief The core union type for jc::maybe
		 * @tparam ValueT Primary type.
		 * @tparam AlternateT Alternate type.
		 * @tparam Enable SFINAE specialization point.
		*/
		template <typename ValueT, typename AlternateT, typename Enable = void>
		union maybe_union
		{
			/**
			 * @brief If this type is trivially destructible.
			*/
			constexpr static bool trivially_destructible_v = false;

			template <typename _T>
			constexpr maybe_union(_T&& _val) :
				val(std::forward<_T>(_val))
			{};
			
			template <typename _T>
			constexpr maybe_union(alternate_t, _T&& _val) :
				alt(std::forward<_T>(_val))
			{};

			constexpr maybe_union(const maybe_union&) = default;
			constexpr maybe_union& operator=(const maybe_union&) = default;
			constexpr maybe_union(maybe_union&&) noexcept = default;
			constexpr maybe_union& operator=(maybe_union&&) noexcept = default;

			~maybe_union() {};

			ValueT val;
			AlternateT alt;
		};
		
		/**
		 * @brief The core union type for jc::maybe
		 * 
		 * Trivially destructible specialization.
		 * 
		 * @tparam ValueT Primary type.
		 * @tparam AlternateT Alternate type.
		*/
		template <typename ValueT, typename AlternateT>
		union maybe_union<ValueT, AlternateT, jc::enable_if_t<jc::is_trivially_destructible<ValueT>::value && jc::is_trivially_destructible<AlternateT>::value>>
		{
			/**
			 * @brief If this type is trivially destructible.
			*/
			constexpr static bool trivially_destructible_v = true;

			template <typename _T>
			constexpr maybe_union(_T&& _val) :
				val(std::forward<_T>(_val))
			{};
			template <typename _T>
			constexpr maybe_union(alternate_t, _T&& _val) :
				alt(std::forward<_T>(_val))
			{};

			constexpr maybe_union(const maybe_union&) = default;
			constexpr maybe_union& operator=(const maybe_union&) = default;
			constexpr maybe_union(maybe_union&&) noexcept = default;
			constexpr maybe_union& operator=(maybe_union&&) noexcept = default;

			~maybe_union() = default;

			ValueT val;
			AlternateT alt;
		};


		// TODO : Trivially copyable optimization
		template <typename V, typename A>
		constexpr inline maybe_union<V, A> copy_maybe_union(const maybe_union<V, A>& _src, bool _active)
		{
			if (_active)
			{
				return maybe_union<V, A>{ _src.val };
			}
			else
			{
				return maybe_union<V, A>{ jc::alternate, _src.alt };
			};
		};

		// TODO : Trivially moveable optimization
		template <typename V, typename A>
		constexpr inline maybe_union<V, A> move_maybe_union(maybe_union<V, A>&& _src, bool _active)
		{
			if (_active)
			{
				return maybe_union<V, A>{ std::move(_src.val) };
			}
			else
			{
				return maybe_union<V, A>{ jc::alternate, std::move(_src.alt) };
			};
		};




		/**
		 * @brief Data type wrapping for jc::maybe.
		 * @tparam UnionT Maybe union type.
		 * @tparam Enable SFINAE specialization point.
		*/
		template <typename UnionT, typename Enable = void>
		struct maybe_data
		{
		public:

			/**
			 * @brief Destroys the active union member.
			*/
			constexpr void destroy() noexcept
			{
				if (this->has_value_)
				{
					jc::destroy_at(this->union_.val);
				}
				else
				{
					jc::destroy_at(this->union_.alt);
				};
			};

			template <typename _T>
			constexpr maybe_data(_T&& _val) :
				union_{ std::forward<_T>(_val) },
				has_value_{ true }
			{};
			template <typename _T>
			constexpr maybe_data(alternate_t, _T&& _val) :
				union_{ jc::alternate, std::forward<_T>(_val) },
				has_value_{ false }
			{};


			// Copy / Move construction using method indirection

			constexpr maybe_data(const maybe_data& other) :
				union_{ copy_maybe_union(other.union_, other.has_value_) },
				has_value_{ other.has_value_ }
			{};
			constexpr maybe_data(maybe_data&& other) :
				union_{ move_maybe_union(std::move(other.union_), other.has_value_) },
				has_value_{ other.has_value_ }
			{};


			// add destructor as we consider non-trivial by default
			~maybe_data()
			{
				this->destroy();
			};

			using union_type = UnionT;

			/**
			 * @brief The union with our members.
			*/
			union_type union_;

			/**
			 * @brief If the primary value type is active.
			*/
			bool has_value_;
		};

		/**
		 * @brief Data type wrapping for jc::maybe.
		 * 
		 * Implements trivially destructible.
		 * 
		 * @tparam UnionT Maybe union type.
		*/
		template <typename UnionT>
		struct maybe_data<UnionT, jc::enable_if_t<UnionT::trivially_destructible_v>>
		{
		public:

			/**
			 * @brief Destroys the active union member.
			*/
			constexpr void destroy() noexcept {};

			template <typename _T>
			constexpr maybe_data(_T&& _val) :
				union_{ std::forward<_T>(_val) },
				has_value_{ true }
			{};
			template <typename _T>
			constexpr maybe_data(alternate_t, _T&& _val) :
				union_{ jc::alternate, std::forward<_T>(_val) },
				has_value_{ false }
			{};

			// Copy / Move construction using method indirection

			constexpr maybe_data(const maybe_data& other) :
				union_{ copy_maybe_union(other.union_, other.has_value_) },
				has_value_{ other.has_value_ }
			{};
			constexpr maybe_data(maybe_data&& other) :
				union_{ move_maybe_union(std::move(other.union_), other.has_value_) },
				has_value_{ other.has_value_ }
			{};

			// Defaulted for triviality
			~maybe_data() = default;

			using union_type = UnionT;

			/**
			 * @brief The union with our members.
			*/
			union_type union_;

			/**
			 * @brief If the primary value type is active.
			*/
			bool has_value_;
		};

		/**
		 * @brief Base implementation for jc::maybe
		 * @tparam T 
		 * @tparam AltT 
		*/
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

			// Data type wrapping
			using data_type = maybe_data<maybe_union<value_type, alternate_type>>;

		protected:

			/*
				Union member access without run time checking
			*/

			constexpr value_type& unsafe_value() noexcept
			{
				JCLIB_ASSERT(this->has_value());
				return this->data_.union_.val;
			};
			constexpr const value_type& unsafe_value() const noexcept
			{
				JCLIB_ASSERT(this->has_value());
				return this->data_.union_.val;
			};
			
			constexpr alternate_type& unsafe_alternate() noexcept
			{
				JCLIB_ASSERT(!this->has_value());
				return this->data_.union_.alt;
			};
			constexpr const alternate_type& unsafe_alternate() const noexcept
			{
				JCLIB_ASSERT(!this->has_value());
				return this->data_.union_.alt;
			};

			// Dangerous !
			constexpr void set_has_value(bool _to) noexcept
			{
				this->data_.has_value_ = _to;
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
				this->data_.destroy();
			};

		public:

			/**
			 * @brief Checks if the value_type is the active member.
			 * 
			 * @return True if value_type is the active member, false otherwise.
			*/
			constexpr bool has_value() const noexcept
			{
				return this->data_.has_value_;
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

			// Sets the primary type as active and sets the bool
			// Does not destroy the active
			template <typename _T>
			constexpr void no_destroy_set_value(_T&& _value)
			{
				// Make sure we construct the object if it is uninitialized, DO NOT ASSIGN
				if (!this->has_value())
				{
					new (&this->data_.union_.val) T(std::forward<_T>(_value));
					this->set_has_value(true);
				}
				else
				{
					this->data_.union_.val = std::forward<_T>(_value);
				};
			};

			// Sets the alternate type as active and sets the bool
			// Does not destroy the active
			template <typename _T>
			constexpr void no_destroy_set_alternate(_T&& _value)
			{
				// Make sure we construct the object if it is uninitialized, DO NOT ASSIGN
				if (this->has_value())
				{
					std::construct_at(&this->data_.union_.alt, std::forward<_T>(_value));
					this->set_has_value(false);
				}
				else
				{
					this->data_.union_.alt = std::forward<_T>(_value);
				};
			};

			// Sets the primary type as active and sets the bool
			template <typename _T>
			constexpr void set_value(_T&& _value) noexcept
			{
				if (!this->has_value())
				{
					this->destroy_alternate();
				};
				this->no_destroy_set_value(std::forward<_T>(_value));
			};

			// Sets the alternate type as active and sets the bool
			template <typename _T>
			constexpr void set_alternate(_T&& _value) noexcept
			{
				if (this->has_value())
				{
					this->destroy_value();
				};
				this->no_destroy_set_alternate(std::forward<_T>(_value));
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
				data_{ _value }
			{};
			constexpr explicit maybe_base(value_type&& _value) noexcept :
				data_{ std::move(_value) }
			{};

			constexpr explicit maybe_base(alternate_t, const alternate_type& _alt)
				noexcept(noexcept(alternate_type{ std::declval<const alternate_type&>() })) :
				data_{ jc::alternate, _alt }
			{};
			constexpr explicit maybe_base(alternate_t, alternate_type&& _alt) noexcept :
				data_{ jc::alternate, std::move(_alt)}
			{};

			constexpr maybe_base(const maybe_base& other) :
				data_{ other.data_ }
			{};
			constexpr maybe_base& operator=(const maybe_base& other)
			{
				if (&other != this)
				{
					if (other.has_value())
					{
						this->set_value(other.unsafe_value());
					}
					else
					{
						this->set_alternate(other.unsafe_alternate());
					};
				};
				return *this;
			};

			constexpr maybe_base(maybe_base&& other) noexcept :
				data_{ std::move(other.data_) }
			{};
			constexpr maybe_base& operator=(maybe_base&& other) noexcept
			{
				const auto q = &other;
				const auto t = this;
				if (q != t)
				{
					if (other.has_value())
					{
						this->set_value(std::move(other.unsafe_value()));
					}
					else
					{
						this->set_alternate(std::move(other.unsafe_alternate()));
					};
				};
				return *this;
			};

			~maybe_base() = default;

		private:
	
			/**
			 * @brief The underlying data object for the maybe
			*/
			data_type data_;

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