#include <jclib/memory.h>

#include <iostream>

#include <cassert>

bool destroy_at_called_v = false;

struct Foo
{
	~Foo()
	{
		assert(!destroy_at_called_v);
		assert(!called_destructor);
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

	return 0;
};
