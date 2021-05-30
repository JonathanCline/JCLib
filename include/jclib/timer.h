#pragma once
#ifndef JCLIB_TIMER_H
#define JCLIB_TIMER_H

#define _JCLIB_TIMER_

/*
	Defines a timer type for determining if a duration has passed or not
*/

#include "jclib/config.h"

// Time support header
#include "jclib/time.h"
#include "jclib/thread.h"

namespace jc
{
	/**
	 * @brief Utility to measure if a duration of time has elapsed
	 * @tparam Clock Clock type to use for measuring time
	*/
	template <typename ClockT, typename DurationT = typename ClockT::duration> JCLIB_REQUIRES(jc::cx_clock<ClockT>)
	class basic_timer
	{
	public:
		/**
		 * @brief Clock used by the timer
		*/
		using clock_type = ClockT;

		using duration = DurationT;
		using time_point = typename clock_type::time_point;

	private:
		
		//	Indirection for getting the current time, this exists in case
		// I decide to support clock types with non-static now()
		JCLIB_CONSTEXPR time_point now() const noexcept
		{
			return clock_type::now();
		};

	public:

		/**
		 * @brief Returns the duration after which the timer is considered finished
		*/
		JCLIB_CONSTEXPR duration get_duration() const noexcept
		{
			return this->tmdur_;
		};
		
		/**
		 * @brief Sets the timer's duration, does not start() the timer
		 * @param _dt Duration after which the timer is finished()
		*/
		JCLIB_CONSTEXPR void set_duration(duration _dt) noexcept
		{
			this->tmdur_ = _dt;
		};

		/**
		 * @brief Returns the time point at which finished() will return true
		*/
		JCLIB_CONSTEXPR time_point finished_at() const noexcept
		{
			return this->tmstart_ + this->get_duration();
		};

		/**
		 * @brief Sets the timer's start time to the given time_point '_timep'
		*/
		JCLIB_CONSTEXPR void start(time_point _timep) noexcept
		{
			this->tmstart_ = _timep;
		};

		/**
		 * @brief Sets the timer's start time to the current time_point from the timer's clock
		*/
		JCLIB_CONSTEXPR void start() noexcept
		{
			this->start(this->now());
		};

		/**
		 * @brief Returns time elapsed since the last call to start()
		*/
		JCLIB_CONSTEXPR duration elapsed() const noexcept
		{
			const time_point _now = this->now();
			return _now - this->tmstart_;
		};

		/**
		 * @brief Returns the duration after which the timer is finished()
		*/
		JCLIB_CONSTEXPR duration remaining() const noexcept
		{
			const duration _elapsed = this->elapsed();
			return this->get_duration() - _elapsed;
		};

		/**
		 * @brief Returns true if the time since the last call to start() is greater than the timer's duration
		*/
		JCLIB_CONSTEXPR bool finished() const noexcept
		{
			return this->elapsed() >= this->get_duration();
		};

		/**
		 * @brief Initializes the timer with a duration equal to zero, does not start the timer
		*/
		JCLIB_CONSTEXPR basic_timer() noexcept = default;

		/**
		 * @brief Initializes the timer with an initial duration, does not start the timer
		 * @param _dur Timer duration
		*/
		JCLIB_CONSTEXPR basic_timer(duration _dur) noexcept :
			tmdur_{ _dur }
		{};

	private:
		/**
		 * @brief The duration the timer is measuring
		*/
		duration tmdur_;

		/**
		 * @brief The last time the timer was started
		*/
		time_point tmstart_;
	};

	/**
	 * @brief Default timer specialization for most common use case
	*/
	using timer = basic_timer<std::chrono::steady_clock>;

	/**
	 * @brief Causes the thread to sleep until the given timer is finished()
	*/
	template <typename Clock, typename DurationT>
	static void sleep_until(const basic_timer<Clock, DurationT>& _timer)
	{
		using timer_type = basic_timer<Clock, DurationT>;
		using duration = typename timer_type::duration;

		const duration _remaining = _timer.remaining();
		if (_remaining > duration::zero())
		{
			jc::sleep(_remaining);
		};
	};

};

#endif