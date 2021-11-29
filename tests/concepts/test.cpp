#include <jclib-test.hpp>
#include <jclib/concepts.h>

#ifdef _JCLIB_CONCEPTS_

void subtest_polymorphic_derived_from()
{
	struct Foo
	{
		virtual ~Foo() = default;
	};
	struct Bar : public Foo {};
	struct Baz : protected Foo {};

	static_assert(jc::cx_polymorphic_derived_from<Foo, Foo>, "");
	static_assert(jc::cx_polymorphic_derived_from<Bar, Foo>, "");
	static_assert(!jc::cx_polymorphic_derived_from<Baz, Foo>, "");
};








#endif

int main()
{
#ifdef _JCLIB_CONCEPTS_
	subtest_polymorphic_derived_from();
#endif
	return 0;
};
