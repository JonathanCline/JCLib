#include <jclib/memory.h>

// Add test support header
#include <jclib-test.hpp>



bool destroy_at_called_v = false;
int destroy_at_result = 0;

struct Foo
{
	~Foo()
	{
		if (destroy_at_called_v != false)
		{
			destroy_at_result = 1;
			return;
		};
		
		if (called_destructor != false)
		{
			destroy_at_result = 2;
			return;
		};

		called_destructor = true;
	};

	bool called_destructor = false;
	int n = 0;
};

int main()
{
	auto _mblock = new char[sizeof(Foo)]{ 0 };
	auto _fooPtr = new(_mblock) Foo{ };
	
	jc::destroy_at(_fooPtr);
	destroy_at_called_v = true;

	delete[] _mblock;

	auto _mblock_2 = new char[sizeof(Foo)]{ 0 };
	auto _fooPtr_2 = new(_mblock_2) Foo{ };

	destroy_at_called_v = false;

	jc::destroy_at(_fooPtr_2);
	destroy_at_called_v = true;

	delete[] _mblock_2;

	auto _ptr = jc::make_unique<int>(2);
	return destroy_at_result;
};
