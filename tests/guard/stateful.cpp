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
		this->gstate->reset_called = true;
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
			this->gstate->set_state_alive_called = true;
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
		GuardedState _state{};
		guard_type _g{ _state };
		ASSERT(_state.set_state_alive_called, "not set to alive on construction");
		ASSERT(!_state.set_state_dead_called, "set to dead at construction");
	};

	PASS();
};







int main()
{
	NEWTEST();
	
	SUBTEST(subtest_construction);
	SUBTEST(subtest_release);

	PASS();
};