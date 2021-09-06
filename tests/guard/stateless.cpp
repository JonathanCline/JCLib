#include <jclib-test.hpp>

#include <jclib/guard.h>


struct GuardedState
{
	// Set once reset is called
	bool reset_called = false;
};



struct GuardedObject
{
	void reset()
	{
		this->gstate->reset_called = true;
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

		ASSERT(!_gstate.reset_called, "invalid preconditions");

		guard_type _g{ _gstate };

		ASSERT(_g.good(), "guard::good() should return true after guard construction");
		ASSERT((bool)_g, "guard::operator bool() should return true after guard construction");
	};

	PASS();
};

// Test guard.reset()
int subtest_reset()
{
	NEWTEST();

	GuardedState _gstate{};

	guard_type _g{ _gstate };
	ASSERT(!_gstate.reset_called, "bad preconditions");

	_g.reset();

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
	SUBTEST(subtest_good);
	SUBTEST(subtest_release);
	SUBTEST(subtest_reset);
	SUBTEST(subtest_move);
	PASS();
};