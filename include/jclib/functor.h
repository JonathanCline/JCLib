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
	sae::functor<> works almost identically to the std::function<> type.

	However, instead of having to use std::bind for a member function, there is builtin functionality for "binding" a
	member function.

	This has substantially less compile-time code generation when compiling in Debug mode.

	Example Code:

	#include "SAE_Functor.h"
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
		sae::functor<int(const int&)> _fp = &foo;
		i = _fp(i);
		std::cout << i << '\n';
		_fp = std::pair{ &bar::foobar, &_b };
		i = _fp(i);
		std::cout << i << '\n';
		return 0;
	};
*/

#include "jcLib/config.h"

#include <utility>
#include <new>

namespace jc
{
	
	namespace impl
	{

		template <typename ReturnT, typename... Args>
		struct functionPtr_t
		{
		public:

			virtual inline functionPtr_t<ReturnT, Args...>* clone() const = 0;
			virtual inline ReturnT invoke(Args... _a) const = 0;

			JCLIB_CONSTEXPR functionPtr_t() noexcept = default;
			virtual ~functionPtr_t() {};

		};

		template <typename ReturnT, typename... Args>
		struct freeFunctionPtr_t : public functionPtr_t<ReturnT, Args...>
		{
		private:
			using ParentT = functionPtr_t<ReturnT, Args...>;

		public:

			virtual inline functionPtr_t<ReturnT, Args...>* clone() const final
			{
				return new freeFunctionPtr_t{ *this };
			};
			virtual inline ReturnT invoke(Args... args) const final
			{
				return (*fptr)(args...);
			};

			using function_pointer = ReturnT(*)(Args...);

			JCLIB_CONSTEXPR freeFunctionPtr_t(const function_pointer& _p) noexcept :
				fptr{ _p }
			{};
			JCLIB_CONSTEXPR freeFunctionPtr_t() noexcept = default;

		private:
			function_pointer fptr;

		};
		template <typename ReturnT, class ScopeT, typename... Args>
		struct memberFunctionPtr_t : public functionPtr_t<ReturnT, Args...>
		{
		private:
			using ParentT = functionPtr_t<ReturnT, Args...>;

		public:

			virtual inline functionPtr_t<ReturnT, Args...>* clone() const final
			{
				return new memberFunctionPtr_t{ *this };
			};
			virtual inline ReturnT invoke(Args... args) const final
			{
				return (class_ptr->*fptr)(args...);
			};

			using function_pointer = ReturnT(ScopeT::*)(Args...);


			JCLIB_CONSTEXPR memberFunctionPtr_t(const function_pointer& _f, ScopeT* _c) noexcept :
				fptr{ _f }, class_ptr{ _c }
			{};

			JCLIB_CONSTEXPR memberFunctionPtr_t() noexcept = default;

		private:
			function_pointer fptr;
			ScopeT* class_ptr = nullptr;

		};


		template <bool isNoexcept, typename ReturnT, typename... Args>
		struct functor_impl
		{
		public:
			using return_type = ReturnT;

			JCLIB_CONSTEXPR bool good() const noexcept { return this->ptr_ != nullptr; };

			void release()
			{
				this->ptr_ = nullptr;
			};
			void reset()
			{
				delete this->ptr_;
				this->release();
			};
			
			inline ReturnT invoke(Args... _args) const noexcept(isNoexcept)
			{
				return this->ptr_->invoke(_args...);
			};
			inline ReturnT operator()(Args... _args) const noexcept(isNoexcept)
			{
				return this->ptr_->invoke(_args...);
			};

			JCLIB_CONSTEXPR inline bool is_member_function() const noexcept
			{
				return this->member_function_;
			};
			JCLIB_CONSTEXPR inline bool good_pointer() const noexcept
			{
				return this->good();
			};

			JCLIB_CONSTEXPR explicit operator bool() const noexcept
			{
				return this->good();
			};

			JCLIB_CONSTEXPR functor_impl(ReturnT(*_func)(Args...)) :
				ptr_{ new freeFunctionPtr_t<ReturnT, Args...>{_func} },
				member_function_{ false }
			{};
			template <class ScopeT>
			JCLIB_CONSTEXPR functor_impl(ReturnT(ScopeT::* _func)(Args...), ScopeT* _p = nullptr) :
				ptr_{ new memberFunctionPtr_t<ReturnT, ScopeT, Args...>{_func, _p} },
				member_function_{ true }
			{};

			functor_impl& operator=(ReturnT(*_func)(Args...))
			{
				delete ptr_;
				ptr_ = new impl::freeFunctionPtr_t<ReturnT, Args...>{ _func };
				this->member_function_ = false;
				return *this;
			};
			template <class ScopeT>
			functor_impl& operator=(std::pair<ReturnT(ScopeT::*)(Args...), ScopeT*>&& _memberFunc)
			{
				delete ptr_;
				ptr_ = new memberFunctionPtr_t<ReturnT, ScopeT, Args...>{ _memberFunc.first, _memberFunc.second };
				this->member_function_ = true;
				return *this;
			};

			JCLIB_CONSTEXPR functor_impl() noexcept :
				ptr_{ nullptr }, member_function_{ false }
			{};

			JCLIB_CONSTEXPR explicit functor_impl(const functor_impl& _o) :
				ptr_{ (_o)? _o.ptr_->clone() : nullptr }
			{};
			functor_impl& operator=(const functor_impl& _o)
			{
				delete ptr_;
				ptr_ = (_o)? _o.ptr_->clone() : nullptr ;
				return *this;
			};

			JCLIB_CONSTEXPR explicit functor_impl(functor_impl&& _o) noexcept :
				ptr_{ std::exchange(_o.ptr_, nullptr) }
			{};
			functor_impl& operator=(functor_impl&& _o) noexcept
			{
				delete this->ptr_;
				this->ptr_ = std::exchange(_o.ptr_, nullptr);
				return *this;
			}

			~functor_impl()
			{
				this->reset();
			};

		private:
			bool member_function_ = false;
			functionPtr_t<ReturnT, Args...>* ptr_;
		};

#ifdef __cpp_deduction_guides

		template <typename ReturnT, typename... Args>
		functor_impl(ReturnT(*)(Args...))->functor_impl<false, ReturnT, Args...>;

		template <typename ReturnT, typename... Args>
		functor_impl(ReturnT(*)(Args...) noexcept)->functor_impl<true, ReturnT, Args...>;

		template <typename ReturnT, typename ScopeT, typename... Args>
		functor_impl(ReturnT(ScopeT::*)(Args...), ScopeT*)->functor_impl<false, ReturnT, Args...>;

		template <typename ReturnT, typename ScopeT, typename... Args>
		functor_impl(ReturnT(ScopeT::*)(Args...) noexcept, ScopeT*)->functor_impl<true, ReturnT, Args...>;

#endif

		template <typename T>
		struct functor_base;

		template <typename ReturnT, typename... Args>
		struct functor_base<ReturnT(Args...)> : public functor_impl<false, ReturnT, Args...>
		{
			using functor_impl<false, ReturnT, Args...>::functor_impl;
			using functor_impl<false, ReturnT, Args...>::operator=;
		};
		
#ifdef __cpp_noexcept_function_type
		template <typename ReturnT, typename... Args>
		struct functor_base<ReturnT(Args...) noexcept> : public functor_impl<true, ReturnT, Args...>
		{
			using functor_impl<true, ReturnT, Args...>::functor_impl;
			using functor_impl<true, ReturnT, Args...>::operator=;
		};
#endif

#ifdef __cpp_deduction_guides

		template <typename ReturnT, typename... Args>
		functor_base(ReturnT(*)(Args...))->functor_base<ReturnT(Args...)>;

		template <typename ReturnT, typename... Args>
		functor_base(ReturnT(*)(Args...) noexcept)->functor_base<ReturnT(Args...) noexcept>;

		template <typename ReturnT, typename ScopeT, typename... Args>
		functor_base(ReturnT(ScopeT::*)(Args...), ScopeT*)->functor_base<ReturnT(Args...)>;

		template <typename ReturnT, typename ScopeT, typename... Args>
		functor_base(ReturnT(ScopeT::*)(Args...) noexcept, ScopeT*)->functor_base<ReturnT(Args...) noexcept>;

#endif
	};

#ifdef __cpp_concepts
	template <typename FunctionT> requires requires { impl::functor_base<FunctionT>{}; }
#else
	template <typename FunctionT>
#endif
	struct functor : public impl::functor_base<FunctionT>
	{
	private:
		using parent_type = impl::functor_base<FunctionT>;
	public:
		using parent_type::parent_type;
		using parent_type::operator=;
	};
	
#ifdef __cpp_deduction_guides

	template <typename ReturnT, typename... Args>
	functor(ReturnT(*)(Args...))->functor<ReturnT(Args...)>;

	template <typename ReturnT, typename... Args>
	functor(ReturnT(*)(Args...) noexcept)->functor<ReturnT(Args...) noexcept>;

	template <typename ReturnT, typename ScopeT, typename... Args>
	functor(ReturnT(ScopeT::*)(Args...), ScopeT*)->functor<ReturnT(Args...)>;

	template <typename ReturnT, typename ScopeT, typename... Args>
	functor(ReturnT(ScopeT::*)(Args...) noexcept, ScopeT*)->functor<ReturnT(Args...) noexcept>;

#endif

}

#endif