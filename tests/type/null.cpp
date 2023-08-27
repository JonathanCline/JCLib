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

// Test no_copy_construct
static_assert(jc::is_default_constructible<jc::no_copy_construct>::value == true,	"jc::no_copy_construct should be default constructible");
static_assert(jc::is_copy_constructible<jc::no_copy_construct>::value == false,		"jc::no_copy_construct should not be copy constructible");
static_assert(jc::is_copy_assignable<jc::no_copy_construct>::value == true,			"jc::no_copy_construct should be copy assignable");
static_assert(jc::is_move_constructible<jc::no_copy_construct>::value == true,		"jc::no_copy_construct should be move constructible");
static_assert(jc::is_move_assignable<jc::no_copy_construct>::value == true,			"jc::no_copy_construct should be move assignable");

// Test no_copy_assign
static_assert(jc::is_default_constructible<jc::no_copy_assign>::value == true,	"jc::no_copy_assign should be default constructible");
static_assert(jc::is_copy_constructible<jc::no_copy_assign>::value == true,		"jc::no_copy_assign should be copy constructible");
static_assert(jc::is_copy_assignable<jc::no_copy_assign>::value == false,		"jc::no_copy_assign should not be copy assignable");
static_assert(jc::is_move_constructible<jc::no_copy_assign>::value == true,		"jc::no_copy_assign should be move constructible");
static_assert(jc::is_move_assignable<jc::no_copy_assign>::value == true,		"jc::no_copy_assign should be move assignable");

// Test no_copy
static_assert(jc::is_default_constructible<jc::no_copy>::value == true,	"jc::no_copy should be default constructible");
static_assert(jc::is_copy_constructible<jc::no_copy>::value == false,	"jc::no_copy should not be copy constructible");
static_assert(jc::is_copy_assignable<jc::no_copy>::value == false,		"jc::no_copy should not be copy assignable");
static_assert(jc::is_move_constructible<jc::no_copy>::value == true,	"jc::no_copy should be move constructible");
static_assert(jc::is_move_assignable<jc::no_copy>::value == true,		"jc::no_copy should be move assignable");

// Test no_move_construct
static_assert(jc::is_default_constructible<jc::no_move_construct>::value == true,	"jc::no_move_construct should be default constructible");
static_assert(jc::is_copy_constructible<jc::no_move_construct>::value == true,		"jc::no_move_construct should be copy constructible");
static_assert(jc::is_copy_assignable<jc::no_move_construct>::value == true,			"jc::no_move_construct should be copy assignable");
static_assert(jc::is_move_constructible<jc::no_move_construct>::value == false,		"jc::no_move_construct should not be move constructible");
static_assert(jc::is_move_assignable<jc::no_move_construct>::value == true,			"jc::no_move_construct should be move assignable");

int main()
{
	NEWTEST();

	// Test pointers.
	SUBTEST(subtest_pointer);

	PASS();
};