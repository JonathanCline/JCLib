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
	/**
	 * @brief Guard state values
	*/
	enum class guard_state : bool
	{
		/**
		 * @brief Guard is still alive and may be reset
		*/
		alive = true,

		/**
		 * @brief Guard is dead and will not run the 
		*/
		dead = false
	};

	namespace impl
	{
		/**
		 * @brief Checks if a guarded type is considered stateful (has get_state() and set_state() methods)
		 * @tparam T Guarded type to check
		 * @tparam Enable SFINAE specialization point
		*/
		template <typename T, typename Enable = void>
		struct is_stateful_guarded :
			public jc::false_type
		{};



		// Stateful guarded true condition. Requires that T has
		//
		//		const T.good() -> jc::guard_state or bool
		//		T.release() -> void
		//
		template <typename T>
		struct is_stateful_guarded<T, jc::void_t
			<
				// Must have a good() method returning a guard state or bool convertible value
				jc::enable_if_t
				<
					jc::is_any_of
					<
						decltype(std::declval<const T>().good()),
						bool,
						guard_state
					>::value
				>,

				// Must have a set_state(jc::guard_state) method
				decltype(std::declval<T>().release())
			>>
			: public jc::true_type
		{};

		/**
		 * @brief Checks if a type is a valid guarded type for jc::guard
		 * @tparam T type to check
		 * @tparam Enable SFINAE specialization point
		*/
		template <typename T, typename Enable = void>
		struct is_guarded :
			public jc::false_type
		{};
		
		//	True case for is_guarded. Requires that T must have
		//	
		//		T.reset()
		//
		template <typename T>
		struct is_guarded<T, jc::void_t
			<
				// Must have a reset() method
				decltype(std::declval<T>().reset())
			>>
			: public jc::true_type
		{};




		/**
		 * @brief Guard state implementation stateless guardeds
		 * @tparam GuardedT Guarded type
		*/
		template <typename GuardedT, typename Enable = void>
		class guard_base_state
		{
		protected:

			/**
			 * @brief Gets the state of the guard
			 * @parma _guarded Guarded object
			 * @return Guard state value
			*/
			constexpr guard_state get_guard_state(const GuardedT&) const noexcept
			{
				return this->state_;
			};

			/**
			 * @brief Sets the state of the guard
			 * @parma _guarded Guarded object
			 * @param Guard state value
			*/
			constexpr void set_guard_state(GuardedT& _guarded, guard_state _state) noexcept
			{
				this->state_ = _state;
			};

			constexpr guard_base_state() noexcept = default;
			~guard_base_state() = default;

		private:
			/**
			 * @brief Guard state value
			*/
			guard_state state_;
		};


		/**
		 * @brief Guard state implementation for stateful guardeds
		 * @tparam GuardedT Guarded type
		*/
		template <typename GuardedT>
		class guard_base_state<GuardedT, jc::enable_if_t<is_stateful_guarded<GuardedT>::value>>
		{
		protected:

			/**
			 * @brief Gets the state of the guard
			 * @parma _guarded Guarded object
			 * @return Guard state value
			*/
			constexpr guard_state get_guard_state(const GuardedT& _guarded) const noexcept
			{
				return (guard_state)_guarded.good();
			};

			/**
			 * @brief Sets the state of the guard
			 * @parma _guarded Guarded object
			 * @param Guard state value
			*/
			constexpr void set_guard_state(GuardedT& _guarded, guard_state _state) noexcept
			{
				if (_state == guard_state::dead)
				{
					_guarded.release();
				};
			};

			constexpr guard_base_state() noexcept = default;
			~guard_base_state() = default;
		};




		/**
		 * @brief Guard implementation part for managing the guarded object memory
		 * @tparam GuardedT Guarded object
		 * @tparam Enable SFINAE specialization point
		*/
		template <typename GuardedT>
		class guard_base_guarded
		{
		protected:

			/**
			 * @brief Returns the guarded object
			 * @return GuardedT reference
			*/
			constexpr GuardedT& get_guarded() noexcept
			{
				return this->guarded_;
			};

			/**
			 * @brief Returns the guarded object
			 * @return GuardedT reference
			*/
			constexpr const GuardedT& get_guarded() const noexcept
			{
				return this->guarded_;
			};

			/**
			 * @brief Invokes the guarded object, does not check state
			*/
			constexpr void invoke_guarded()
			{
				this->get_guarded().reset();
			};


			constexpr guard_base_guarded() noexcept = default;

			/**
			 * @brief Constructs the guard guarded in place
			*/
			template <typename... Ts, typename = jc::enable_if_t<jc::is_constructible<GuardedT, Ts...>::value>>
			constexpr guard_base_guarded(Ts&&... _args)
				noexcept(noexcept(GuardedT{ std::forward<Ts>(std::declval<Ts&&>())... })) :
				guarded_{ std::forward<Ts>(_args)... }
			{};

		private:
			/**
			 * @brief Guarded object
			*/
			GuardedT guarded_;
		};

	};


	/**
	 * @brief Generic guard type for implementing RAII behavior
	 * @tparam GuardedT Guard guarded object type
	 * @tparam Enable SFINAE specialization point
	*/
	template <typename GuardedT, typename Enable = void>
	class guard;

	/**
	 * @brief Generic guard type for implementing RAII behavior
	 * @tparam GuardedT Guard guarded object type
	*/
	template <typename GuardedT>
	class guard <GuardedT, jc::enable_if_t<impl::is_guarded<GuardedT>::value>> :

		// Add guarded object part
		private impl::guard_base_guarded<GuardedT>,

		// Add state management part
		private impl::guard_base_state<GuardedT>

	{
	private:
		// Guard release object impl type
		using base_guarded = impl::guard_base_guarded<GuardedT>;

		// Guard state impl type
		using base_state = impl::guard_base_state<GuardedT>;

	protected:
		
		/**
		 * @brief Guard guarded type
		*/
		using guarded_type = GuardedT;


		// Expose guarded object accessor

		using base_guarded::get_guarded;

	public:

		/**
		 * @brief Checks if the guard is still alive
		 * @return True if held, false otherwise
		*/
		constexpr bool good() const noexcept
		{
			return this->get_guard_state(this->get_guarded()) == guard_state::alive;
		};

		/**
		 * @brief Checks if the guard is still alive
		 * @return True if held, false otherwise
		*/
		constexpr explicit operator bool() const noexcept
		{
			return this->good();
		};

		/**
		 * @brief Kills the guard without invoking the guarded object
		*/
		constexpr void release() noexcept
		{
			this->set_guard_state(this->get_guarded(), guard_state::dead);
		};
		
		/**
		 * @brief Invokes the guarded object if this guard is alive and sets state to dead
		*/
		constexpr void reset()
		{
			if (this->good())
			{
				this->invoke_guarded();
				this->release();
			};
		};

	private:

		/**
		 * @brief Calls release() and returns the prior guard state
		 * @return Guard state
		*/
		JCLIB_NODISCARD("owning guard state value") constexpr guard_state extract() noexcept
		{
			const auto _out = this->get_guard_state(this->get_guarded());
			this->release();
			return _out;
		};

	protected:

		/**
		 * @brief Constructs the guarded with the given arguements and sets guard state
		 * @brief _args... Guarded object constructor arguements
		*/
		template <typename... Ts, typename = jc::enable_if_t<jc::is_forwardable_to<guarded_type, Ts...>::value>>
		constexpr explicit guard(guard_state _state, Ts&&... _args)
			noexcept(noexcept(
				guarded_type{ std::forward<Ts>(std::declval<Ts&&>())... }
			))
			: base_guarded{ std::forward<Ts>(_args)... }
		{
			this->set_guard_state(this->get_guarded(), _state);
		};

	public:

		/**
		 * @brief Copy constructs the guarded from a given object and puts the guard into the held state
		 * @brief _guarded Guarded object
		*/
		constexpr explicit guard(GuardedT _guarded = GuardedT{}) noexcept :
			guard{  guard_state::alive, std::move(_guarded) }
		{};





		constexpr guard(const guard& _other) = delete;
		constexpr guard& operator=(const guard& _other) noexcept = default;

		constexpr guard(guard&& _other) noexcept :
			guard{ _other.get_guard_state(_other.get_guarded()), std::move(_other.get_guarded()) }
		{
			_other.release();
		};
		constexpr guard& operator=(guard&& _other) noexcept
		{
			this->reset();
			const auto _otherState = _other.get_guard_state(_other.get_guarded());

			this->get_guarded() = std::move(_other.get_guarded());
			this->set_guard_state(this->get_guarded(), _otherState);
			_other.release();

			return *this;
		};

		~guard()
		{
			this->reset();
		};

	};


	namespace impl
	{
		template <void(*OnRelease)()>
		class guard_fwrap
		{
		public:
			constexpr void release() noexcept
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