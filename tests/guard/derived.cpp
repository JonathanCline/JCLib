#include <jclib-test.hpp>

#include <jclib/guard.h>





struct GuardedObject
{
	void reset() {};
	bool value = false;
};


struct Guard : public jc::guard<GuardedObject>
{
	static_assert(jc::is_same<guarded_type, GuardedObject>::value, "");

	bool& value()
	{
		return this->get_guarded().value;
	};
	const bool& value() const
	{
		return this->get_guarded().value;
	};

	using jc::guard<GuardedObject>::guard;
};


// Test derived guard access behavior
int subtest_derived()
{
	NEWTEST();

	Guard _guard{ GuardedObject{  } };

	const auto _initial = _guard.value();
	_guard.value() = !_guard.value();

	const auto _postval = _guard.value();

	ASSERT(_initial != _postval, "derived guard accessor did not return a reference");

	PASS();
};




int main()
{
	NEWTEST();
	SUBTEST(subtest_derived);
	PASS();
};