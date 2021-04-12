#pragma once
#ifndef JCLIB_TIMER_H
#define JCLIB_TIMER_H

#define _JCLIB_TIMER_H

#include <chrono>

namespace jc
{

	template <typename ClockT>
	class basic_timer
	{
	public:
		using clock = ClockT;
		using duration = typename clock::duration;
		using time_point = typename clock::time_point;

		constexpr void start()
		{
			this->start_ = this->clock().now();
		};
		constexpr time_point start_time() const noexcept
		{
			return this->start_;
		};

		constexpr duration interval() const noexcept
		{
			return this->interval_;
		};
		void set_interval(duration _dur) noexcept
		{
			this->interval_ = _dur;
		};

		constexpr duration elapsed() const noexcept
		{
			return clock::now() - this->start_time();
		};
		constexpr duration remaining() const noexcept
		{
			return this->interval() - this->elapsed();
		};

		constexpr bool finished() const noexcept
		{
			return this->elapsed() > this->interval();
		};

		constexpr basic_timer(duration _interval) noexcept :
			interval_{ _interval }
		{};
		constexpr basic_timer() noexcept = default;

	private:
		time_point start_;
		duration interval_{};
	};

	using timer = basic_timer<std::chrono::steady_clock>;

};

#endif