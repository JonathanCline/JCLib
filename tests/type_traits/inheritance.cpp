#include <jclib-test.hpp>
#include <jclib/type_traits.h>


void subtest_polymorphic_derived_from()
{
	struct Foo
	{
		virtual ~Foo() = default;
	};
	struct Bar : public Foo {};
	struct Baz : protected Foo {};

	static_assert(jc::is_polymorphic_base_of<Foo, Foo>::value, "");
	static_assert(jc::is_polymorphic_base_of<Foo, Bar>::value, "");
	static_assert(!jc::is_polymorphic_base_of<Foo, Baz>::value, "");

#if JCLIB_FEATURE_INLINE_VARIABLES_V
	static_assert(jc::is_polymorphic_base_of_v<Foo, Foo>, "");
	static_assert(jc::is_polymorphic_base_of_v<Foo, Bar>, "");
	static_assert(!jc::is_polymorphic_base_of_v<Foo, Baz>, "");
#endif
};





int main()
{
	NEWTEST();
	subtest_polymorphic_derived_from();
	PASS();
};