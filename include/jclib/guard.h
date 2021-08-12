#pragma once
#ifndef JCLIB_GUARD_H
#define JCLIB_GUARD_H

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


#include <jclib/type_traits.h>
#include <jclib/functional.h>

#define _JCLIB_GUARD_

namespace jc
{
	enum class guard_state : bool
	{
		held = true,
		released = false
	};

	template <typename OnRelease, typename = void>
	struct guard;

	template <typename OnRelease>
	struct guard<OnRelease, enable_if_t<is_invocable<OnRelease>::value>>
	{
	private:
		JCLIB_CONSTEXPR static auto on_release() noexcept { return OnRelease{}; };

	public:
		using guard_state = jc::guard_state;

		JCLIB_CONSTEXPR bool held() const noexcept
		{
			return this->state_ == guard_state::held;
		};
		JCLIB_CONSTEXPR explicit operator bool() const noexcept
		{
			return this->held();
		};

		JCLIB_CONSTEXPR guard_state drop() noexcept
		{
			const auto _out = this->state_;
			this->state_ = guard_state::released;
			return _out;
		};
		JCLIB_CONSTEXPR void release() noexcept(noexcept(std::declval<OnRelease>()()))
		{
			if (this->held())
			{
				jc::invoke(this->on_release());
				this->state_ = guard_state::released;
			};
		};

		JCLIB_CONSTEXPR guard() noexcept = default;
		
		JCLIB_CONSTEXPR guard(const guard&) = delete;
		JCLIB_CONSTEXPR guard& operator=(const guard&) = delete;

		JCLIB_CONSTEXPR guard(guard&& other) noexcept :
			state_{ other.drop() }
		{};
		JCLIB_CONSTEXPR guard& operator=(guard&& other) noexcept
		{
			this->release();
			this->state_ = other.drop();
			return *this;
		};

		~guard()
		{
			this->release();
		};

	private:
		guard_state state_ = guard_state::held;
	};

	namespace impl
	{
		template <void(*OnRelease)()>
		struct guard_fwrap
		{
			JCLIB_CONSTEXPR void operator()() const noexcept
			{
				return OnRelease();
			};
		};
	};

	template <void(*OnRelease)()>
	struct fguard : public guard<impl::guard_fwrap<OnRelease>>
	{
	private:
		using parent_type = guard<impl::guard_fwrap<OnRelease>>;
	public:
		using parent_type::parent_type;
		using parent_type::operator=;
	};
};

#endif