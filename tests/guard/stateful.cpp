#include <jclib-test.hpp>

#include <jclib/guard.h>


struct GuardedState
{
	// Set once reset is called
	bool reset_called = false;

	// Set once set_state is called
	
	bool set_state_dead_called = false;
	bool set_state_alive_called = false;


	// Set once get_state is called
	bool get_state_called = false;
};



struct GuardedObject
{
	void reset()
	{
		if (!this->gstate)
		{
			std::cout << "Reset called when guard state is dead!!! Line # : " << __LINE__ << '\n';
		}
		else
		{
			this->gstate->reset_called = true;
		};
	};

	void set_state(jc::guard_state _state)
	{
		if (_state == jc::guard_state::dead)
		{
			if (this->gstate)
			{
				this->gstate->set_state_dead_called = true;
			};
			this->gstate = nullptr;
		}
		else
		{
			if (this->gstate)
			{
				this->gstate->set_state_alive_called = true;
			};
		};
	};
	jc::guard_state get_state() const
	{
		if (this->gstate)
		{
			this->gstate->get_state_called = true;
			return jc::guard_state::alive;
		}
		else
		{
			return jc::guard_state::dead;
		};
	};





	GuardedObject(GuardedState& _obj) :
		gstate{ &_obj }
	{};

	GuardedObject(GuardedObject&& other) noexcept :
		gstate{ std::exchange(other.gstate, nullptr) }
	{};
	GuardedObject& operator=(GuardedObject&& other) noexcept
	{
		this->gstate = std::exchange(other.gstate, nullptr);
		return *this;
	};


	GuardedState* gstate;
};

using guard_type = jc::guard<GuardedObject>;



// Test guard.release()
int subtest_release()
{
	NEWTEST();

	GuardedState _state{};

	{
		guard_type _g{ _state };
		_g.release();

		ASSERT(_state.set_state_dead_called, "release did not set state to dead");
		ASSERT(!_state.reset_called, "release called reset");

		ASSERT(!_g.good(), "guard::good() should return false after call to release");
		ASSERT(!(bool)_g, "guard::operator bool() should return false after call to release");
	};

	ASSERT(!_state.reset_called, "released guard still called reset after destruction");

	PASS();
};

// Test guard construction behavior
int subtest_construction()
{
	NEWTEST();

	// Test construction
	{
		GuardedState _gstate{};

		ASSERT(!_gstate.set_state_alive_called, "invalid preconditions");
		ASSERT(!_gstate.set_state_dead_called, "invalid preconditions");

		guard_type _g{ _gstate };

		ASSERT(_gstate.set_state_alive_called, "not set to alive on construction");
		ASSERT(!_gstate.set_state_dead_called, "set to dead at construction");
	};

	PASS();
};

// Test guard.reset()
int subtest_reset()
{
	NEWTEST();

	GuardedState _gstate{};

	guard_type _g{ _gstate };
	ASSERT(!_gstate.set_state_dead_called, "bad preconditions");
	ASSERT(!_gstate.reset_called, "bad preconditions");

	_g.reset();

	ASSERT(_gstate.set_state_dead_called, "reset did not set state to dead");
	ASSERT(_gstate.reset_called, "reset did not call guarded object reset function");

	ASSERT(!_g.good(), "guard::good() should return false after call to reset");
	ASSERT(!(bool)_g, "guard::operator bool() should return false after call to reset");

	PASS();
};

// Test guard.good()
int subtest_good()
{
	NEWTEST();

	// Test good with reset
	{
		GuardedState _gstate{};
		guard_type _obj{ _gstate };

		ASSERT(_obj.good(), "guard good() on alive guard result mismatch");
		ASSERT((bool)_obj, "guard operator bool() on alive guard result mismatch");
		
		// Reset to make good() return false 
		_obj.reset();

		ASSERT(!_obj.good(), "guard good() on dead guard result mismatch");
		ASSERT(!(bool)_obj, "guard operator bool() on dead guard result mismatch");
	};

	// Test good with release
	{
		GuardedState _gstate{};
		guard_type _obj{ _gstate };

		ASSERT(_obj.good(), "guard good() on alive guard result mismatch");
		ASSERT((bool)_obj, "guard operator bool() on alive guard result mismatch");

		// Reset to make good() return false 
		_obj.release();

		ASSERT(!_obj.good(), "guard good() on dead guard result mismatch");
		ASSERT(!(bool)_obj, "guard operator bool() on dead guard result mismatch");
	};

	PASS();
};

// Test guard move semantics
int subtest_move()
{
	NEWTEST();

	// Test move constructor
	{
		GuardedState _gstate{};
		guard_type _obj{ _gstate };

		ASSERT(_obj.good(), "bad test preconditions");

		guard_type _other{ std::move(_obj) };

		ASSERT(!_obj.good(), "moved guard did not get cleaned up");
		ASSERT(_other.good(), "guard constructed from moved guard mismatch");
	};

	// Test move assignment
	{
		GuardedState _gstate{};

		guard_type _obj{ _gstate };
		guard_type _other{ _gstate };
		_other.release();

		ASSERT(_obj.good(), "bad test preconditions");
		ASSERT(!_other.good(), "bad test preconditions");

		_other = std::move(_obj);

		ASSERT(!_obj.good(), "move assignment did not release guard state for moved from guard");
		ASSERT(_other.good(), "move assignment did not move guard state correctly");

		_other = std::move(_obj);

		ASSERT(!_obj.good(), "move assignment from dead guard modfied dead guard state");
		ASSERT(!_other.good(), "move assignment from dead guard did not clear destination guard");
	};

	PASS();
};



int main()
{
	NEWTEST();
	
	SUBTEST(subtest_construction);
	SUBTEST(subtest_release);
	SUBTEST(subtest_reset);
	SUBTEST(subtest_good);
	SUBTEST(subtest_move);

	PASS();
};