#pragma once
#ifndef JCLIB_FUNCTOR_H
#define JCLIB_FUNCTOR_H

#define _JCLIB_FUNCTOR_

/*
	Consider this stable.
*/

/*
	Copyright 2020 Jonathan Cline
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
	Confirmed to compile on MSVC for c++ versions 14, 17, 20   (as of 2/28/2021) 
*/


/*
	jc::functor<> works almost identically to the std::function<> type.

	However, instead of having to use std::bind for a member function, there is builtin functionality for "binding" a
	member function.

	This has substantially less compile-time code generation when compiling in Debug mode.

	Example Code:

	#include "jclib/functor.h"
	#include <iostream>

	int foo(const int& _a)
	{
		return _a + 1;
	};

	struct bar
	{
		int foobar(const int& _a)
		{
			return _a + 1;
		};
		bar() = default;
	};

	int main()
	{
		int i = 0;
		bar _b{};
		jc::functor<int(const int&)> _fp = &foo;
		i = _fp(i);
		std::cout << i << '\n';
		_fp = std::pair{ &bar::foobar, &_b };
		i = _fp(i);
		std::cout << i << '\n';
		return 0;
	};
*/

#include "jclib/config.h"

#include "jclib/type.h"
#include "jclib/type_traits.h"


#include <utility>
#include <new>
#include <tuple>


namespace jc
{
	namespace impl
	{
		/**
		 * @brief Base type for function pointer wrapper type erasure
		 * @tparam ReturnT Function return type
		 * @tparam ...ArgTs Function arguement types
		*/
		template <bool isNoexcept, typename ReturnT, typename... ArgTs>
		struct function_pointer_base
		{
		private:
			using this_type = function_pointer_base<isNoexcept, ReturnT, ArgTs...>;
		public:
			// Function return type
			using return_type = ReturnT;

			// Function argument typelist (tuple)
			using argument_typelist = std::tuple<ArgTs...>;

			virtual this_type* clone() const = 0;
			virtual return_type invoke(ArgTs... _a) const = 0;

			virtual ~function_pointer_base() = default;
		};

		/**
		 * @brief Free function pointer wrapper 
		 * @tparam ReturnT Function return type
		 * @tparam ...ArgTs Function arguement types
		*/
		template <bool isNoexcept, typename ReturnT, typename... ArgTs>
		struct free_function_pointer : public function_pointer_base<isNoexcept, ReturnT, ArgTs...>
		{
		private:
			// alias to ease the pain
			using parent_type = function_pointer_base<isNoexcept, ReturnT, ArgTs...>;

		public:
			using return_type = typename parent_type::return_type;
			
			// Internal function pointer type
			using function_pointer_type = std::conditional_t<isNoexcept,
				return_type(*)(ArgTs...) noexcept,
				return_type(*)(ArgTs...)>;

			parent_type* clone() const final
			{
				return new free_function_pointer{ *this };
			};
			return_type invoke(ArgTs... args) const final
			{
				const auto& _function = this->ptr_;
				JCLIB_ASSERT(_function != nullptr);
				return (*_function)(args...);
			};

			JCLIB_CONSTEXPR free_function_pointer(const function_pointer_type& _p) noexcept :
				ptr_{ _p }
			{};
			JCLIB_CONSTEXPR free_function_pointer() noexcept = default;

		private:
			function_pointer_type ptr_;

		};

		/**
		 * @brief Base type for member function pointer type erasure, this is the reason why inheritance is even being used
		 * @tparam ReturnT Function return type
		 * @tparam ClassT The member function's class type
		 * @tparam ...ArgTs Function arguement types
		*/
		template <bool isNoexcept, typename ReturnT, class ClassT, typename... ArgTs>
		struct member_function_pointer : public function_pointer_base<isNoexcept, ReturnT, ArgTs...>
		{
		private:
			using parent_type = function_pointer_base<isNoexcept, ReturnT, ArgTs...>;

		public:
			using class_type = ClassT;
			using return_type = typename parent_type::return_type;

			// Internal function pointer type
			using function_pointer_type = std::conditional_t<isNoexcept,
				return_type(class_type::*)(ArgTs...) noexcept,
				return_type(class_type::*)(ArgTs...)>;

			parent_type* clone() const final
			{
				return new member_function_pointer{ *this };
			};
			return_type invoke(ArgTs... args) const final
			{
				auto& _class = this->class_;
				auto& _function = this->ptr_;
				JCLIB_ASSERT(_class != nullptr && _function != nullptr);
				return (_class->*_function)(args...);
			};

			JCLIB_CONSTEXPR member_function_pointer(const function_pointer_type& _function, class_type* _class) noexcept :
				ptr_{ _function }, class_{ _class }
			{};
			JCLIB_CONSTEXPR member_function_pointer() noexcept = default;

		private:
			function_pointer_type ptr_;
			class_type* class_;

		};

		/**
		 * @brief function_pointer_base RAII wrapper and functor interface implementation
		 * @tparam isNoexcept Specifies the function pointer is noexcept
		 * @tparam ReturnT Function return type
		 * @tparam ...ArgTs Function arguement types
		*/
		template <bool isNoexcept, typename ReturnT, typename... ArgTs>
		struct functor_impl
		{
		private:
			using free_function_object_type = free_function_pointer<isNoexcept, ReturnT, ArgTs...>;
			using free_function_type = typename free_function_object_type::function_pointer_type;

			template <typename ClassT>
			using member_function_object_type = member_function_pointer<isNoexcept, ReturnT, ClassT, ArgTs...>;
			template <typename ClassT>
			using member_function_type = typename member_function_object_type<ClassT>::function_pointer_type;

		public:

			/**
			 * @brief Underlying pointer type owned / managed
			*/
			using pointer = function_pointer_base<isNoexcept, ReturnT, ArgTs...>*;

			using return_type = ReturnT;

			/**
			 * @brief True if the function type is noexcept
			*/
			JCLIB_CONSTANT static inline bool is_noexcept_v = isNoexcept;

			/**
			 * @brief Returns true if the function type is noexcept
			*/
			JCLIB_CONSTEXPR static bool is_noexcept() noexcept
			{
				return is_noexcept_v;
			};

		private:
			JCLIB_CONSTEXPR auto& get() noexcept
			{
				return this->ptr_;
			};
			JCLIB_CONSTEXPR const pointer& get() const noexcept
			{
				return this->ptr_;
			};

		public:

			/**
			 * @brief Returns true if the owned function pointer can be invoked
			 * @return True if possible, false otherwise
			*/
			JCLIB_CONSTEXPR bool good() const noexcept
			{
				return this->get() != nullptr;
			};

			// deprecated in favor of good() to unify semantics
			JCLIB_DEPRECATE_BLOCK
			(
				JCLIB_CONSTEXPR JCLIB_DEPRECATED("use good() instead") bool good_pointer() const noexcept
				{
					return this->good();
				}
			);
			
			/**
			 * @brief Same as good()
			*/
			JCLIB_CONSTEXPR explicit operator bool() const noexcept
			{
				return this->good();
			};

			/**
			 * @brief Releases ownership of the underlying function pointer and sets it to nullptr
			*/
			JCLIB_CONSTEXPR void release() noexcept
			{
				pointer& _ptr = this->get();
				_ptr = nullptr;
			};

			/**
			 * @brief Frees the owned function pointer memory and resets it to nullptr,
			*/
			JCLIB_CONSTEXPR void reset() noexcept
			{
				pointer& _ptr = this->get();
				delete _ptr;
				this->release();
				JCLIB_ASSERT(_ptr == nullptr);
			};

			/**
			 * @brief Releases ownership of the owned function pointer and returns it
			 * @return The owned function pointer as a pointer to the base (impl::function_pointer_base)
			*/
			JCLIB_CONSTEXPR JCLIB_NODISCARD("owning pointer") pointer extract() noexcept
			{
				const pointer _out = this->get();
				this->release();
				return _out;
			};

			/**
			 * @brief Invokes the owned function pointer, undefined if good() would return false
			 * @param _args Arguements to invoke the owned function pointer with
			 * @return Value returned by invoking the owned function pointer, or nothing if void
			*/
			return_type invoke(ArgTs... _args) const noexcept(functor_impl::is_noexcept())
			{
				JCLIB_ASSERT(this->good());
				const pointer& _function = this->get();
				return _function->invoke(_args...);
			};

			/**
			 * @brief Same as invoke()
			*/
			return_type operator()(ArgTs... _args) const noexcept(functor_impl::is_noexcept())
			{
				JCLIB_ASSERT(this->good());
				const pointer& _function = this->get();
				return _function->invoke(_args...);
			};

			/**
			 * @brief Defaults to own nothing, good() will return false.
			*/
			JCLIB_CONSTEXPR functor_impl() noexcept = default;

			/**
			 * @brief Calls the default constructor
			*/
			JCLIB_CONSTEXPR functor_impl(std::nullptr_t) noexcept :
				ptr_{ nullptr }
			{};

			/**
			 * @brief Behaves as if reset() was called
			*/
			functor_impl& operator=(std::nullptr_t) noexcept
			{
				this->reset();
				this->ptr_ = nullptr;
				return *this;
			};

			/**
			 * @param _function Must be an owning pointer to a function object; set to nullptr by the constructor
			*/
			JCLIB_CONSTEXPR explicit functor_impl(pointer _function) noexcept :
				ptr_{ _function }
			{};

			JCLIB_CONSTEXPR functor_impl(free_function_type _function) :
				functor_impl{ new free_function_object_type{_function} }
			{};

			// calls reset()
			functor_impl& operator=(free_function_type _function)
			{
				this->reset();
				this->ptr_ = new free_function_object_type{ _function };
				return *this;
			};

			template <typename ScopeT>
			JCLIB_CONSTEXPR functor_impl(member_function_type<ScopeT> _function, ScopeT* _class) :
				functor_impl{ new member_function_object_type<ScopeT>{ _function, _class } }
			{};

			// calls reset()
			template <typename ScopeT>
			functor_impl& operator=(std::pair<member_function_type<ScopeT>, ScopeT*> _memberFunction)
			{
				this->reset();
				this->ptr_ = new member_function_object_type<ScopeT>
				{
					_memberFunction.first, _memberFunction.second
				};
				return *this;
			};

			JCLIB_CONSTEXPR explicit functor_impl(const functor_impl& _other) :
				ptr_{ (_other.good())? _other.get()->clone() : nullptr }
			{};
			functor_impl& operator=(const functor_impl& _other)
			{
				this->reset();
				this->ptr_ = (_other.good())? _other.get()->clone() : nullptr ;
				return *this;
			};

			JCLIB_CONSTEXPR explicit functor_impl(functor_impl&& _other) noexcept :
				ptr_{ _other.extract() }
			{};
			functor_impl& operator=(functor_impl&& _other) noexcept
			{
				// Check that these are not owning the same function object (this should never happen, ever)
				JCLIB_ASSERT(this->get() != &_other.get());
				this->reset();
				this->ptr_ = _other.extract();
				return *this;
			};

			// Calls reset()
			~functor_impl()
			{
				this->reset();
			};

		private:
			pointer ptr_ = nullptr;
		};

#ifdef __cpp_deduction_guides
		template <typename ReturnT, typename... Args>
		functor_impl(ReturnT(*)(Args...))->functor_impl<false, ReturnT, Args...>;

		template <typename ReturnT, typename ScopeT, typename... Args>
		functor_impl(ReturnT(ScopeT::*)(Args...), ScopeT*)->functor_impl<false, ReturnT, Args...>;

#ifdef __cpp_noexcept_function_type
		template <typename ReturnT, typename... Args>
		functor_impl(ReturnT(*)(Args...) noexcept)->functor_impl<true, ReturnT, Args...>;

		template <typename ReturnT, typename ScopeT, typename... Args>
		functor_impl(ReturnT(ScopeT::*)(Args...) noexcept, ScopeT*)->functor_impl<true, ReturnT, Args...>;
#endif
#endif
	};

	/**
	 * @brief Function object that can point to free functions or member functions that match its signature
	*/
	template <typename T>
	struct functor;

	/**
	 * @brief Function object that can point to free functions or member functions that match its signature
	*/
	template <typename ReturnT, typename... ArgTs>
	struct functor<ReturnT(ArgTs...)> : public impl::functor_impl<false, ReturnT, ArgTs...>
	{
		using impl::functor_impl<false, ReturnT, ArgTs...>::functor_impl;
		using impl::functor_impl<false, ReturnT, ArgTs...>::operator=;
	};

#ifdef __cpp_noexcept_function_type
	/**
	 * @brief Function object that can point to free functions or member functions that match its signature
	*/
	template <typename ReturnT, typename... ArgTs>
	struct functor<ReturnT(ArgTs...) noexcept> : public impl::functor_impl<true, ReturnT, ArgTs...>
	{
		using impl::functor_impl<true, ReturnT, ArgTs...>::functor_impl;
		using impl::functor_impl<true, ReturnT, ArgTs...>::operator=;
	};
#endif
	
#ifdef __cpp_deduction_guides
	template <typename ReturnT, typename... Args>
	functor(ReturnT(*)(Args...))->functor<ReturnT(Args...)>;

	template <typename ReturnT, typename ScopeT, typename... Args>
	functor(ReturnT(ScopeT::*)(Args...), ScopeT*)->functor<ReturnT(Args...)>;

	// noexcept deduction guides if in current c++ version
#ifdef __cpp_noexcept_function_type
	template <typename ReturnT, typename... Args>
	functor(ReturnT(*)(Args...) noexcept)->functor<ReturnT(Args...) noexcept>;

	template <typename ReturnT, typename ScopeT, typename... Args>
	functor(ReturnT(ScopeT::*)(Args...) noexcept, ScopeT*)->functor<ReturnT(Args...) noexcept>;
#endif
#endif
};

#endif