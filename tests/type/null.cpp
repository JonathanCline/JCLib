#include <jclib-test.hpp>
#include <jclib/type.h>

// Essentials

static_assert(nullptr == jc::null, "");
static_assert(jc::null == nullptr, "");
static_assert(!(jc::null != nullptr), "");
static_assert(!(nullptr != jc::null), "");


template <typename T>
int subtest_pointer_type()
{
	NEWTEST();

	using type = int;
	using pointer = type*;

	type a{};
	pointer p = &a;
	pointer np = nullptr;

	{
		ASSERT((p != jc::null) == true, "Valid pointer was considered null!");
		ASSERT((p == jc::null) == false, "Valid pointer was considered null!");
		ASSERT((&a != jc::null) == true, "Valid pointer temporary was considered null!");
		ASSERT((&a != jc::null) == true, "Valid pointer temporary was considered null!");

		ASSERT((np == jc::null) == true, "Null pointer was NOT considered null by jc::null!");
		ASSERT((np != jc::null) == false, "Null pointer was NOT considered null by jc::null!");

		pointer nnp = jc::null;
		ASSERT(nnp == np, "Pointer constructed from jc::null was NOT considered null!");
	};

	static_assert(jc::is_nullable<pointer>::value, "is_nullable<T*> failed!");
	#if JCLIB_FEATURE_INLINE_VARIABLES_V
		static_assert(jc::is_nullable<pointer>::value == jc::is_nullable_v<pointer>, "Mismatch!");
	#endif
	#if JCLIB_FEATURE_CONCEPTS_V
		static_assert(jc::cx_nullable<pointer>);
		static_assert(jc::cx_null_comparable<pointer>);
	#endif

	PASS();
};
int subtest_pointer()
{
	NEWTEST();

	// Test with non-const value pointer.
	SUBTEST(subtest_pointer_type<int>);

	// Test with const value pointer.
	SUBTEST(subtest_pointer_type<const int>);

	PASS();
};


int main()
{
	NEWTEST();

	// Test pointers.
	SUBTEST(subtest_pointer);

	PASS();
};