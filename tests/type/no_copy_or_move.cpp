#include <jclib-test.hpp>
#include <jclib/type.h>

// Test no_copy_construct
static_assert(jc::is_default_constructible<jc::no_copy_construct>::value == true, "jc::no_copy_construct should be default constructible");
static_assert(jc::is_copy_constructible<jc::no_copy_construct>::value == false, "jc::no_copy_construct should not be copy constructible");
static_assert(jc::is_copy_assignable<jc::no_copy_construct>::value == true, "jc::no_copy_construct should be copy assignable");
static_assert(jc::is_move_constructible<jc::no_copy_construct>::value == true, "jc::no_copy_construct should be move constructible");
static_assert(jc::is_move_assignable<jc::no_copy_construct>::value == true, "jc::no_copy_construct should be move assignable");

// Test no_copy_assign
static_assert(jc::is_default_constructible<jc::no_copy_assign>::value == true, "jc::no_copy_assign should be default constructible");
static_assert(jc::is_copy_constructible<jc::no_copy_assign>::value == true, "jc::no_copy_assign should be copy constructible");
static_assert(jc::is_copy_assignable<jc::no_copy_assign>::value == false, "jc::no_copy_assign should not be copy assignable");
static_assert(jc::is_move_constructible<jc::no_copy_assign>::value == true, "jc::no_copy_assign should be move constructible");
static_assert(jc::is_move_assignable<jc::no_copy_assign>::value == true, "jc::no_copy_assign should be move assignable");

// Test no_copy
static_assert(jc::is_default_constructible<jc::no_copy>::value == true, "jc::no_copy should be default constructible");
static_assert(jc::is_copy_constructible<jc::no_copy>::value == false, "jc::no_copy should not be copy constructible");
static_assert(jc::is_copy_assignable<jc::no_copy>::value == false, "jc::no_copy should not be copy assignable");
static_assert(jc::is_move_constructible<jc::no_copy>::value == true, "jc::no_copy should be move constructible");
static_assert(jc::is_move_assignable<jc::no_copy>::value == true, "jc::no_copy should be move assignable");

// Test no_move_construct
static_assert(jc::is_default_constructible<jc::no_move_construct>::value == true, "jc::no_move_construct should be default constructible");
static_assert(jc::is_copy_constructible<jc::no_move_construct>::value == true, "jc::no_move_construct should be copy constructible");
static_assert(jc::is_copy_assignable<jc::no_move_construct>::value == true, "jc::no_move_construct should be copy assignable");
static_assert(jc::is_move_constructible<jc::no_move_construct>::value == false, "jc::no_move_construct should not be move constructible");
static_assert(jc::is_move_assignable<jc::no_move_construct>::value == true, "jc::no_move_construct should be move assignable");

// Test no_move_assign
static_assert(jc::is_default_constructible<jc::no_move_assign>::value == true, "jc::no_move_assign should be default constructible");
static_assert(jc::is_copy_constructible<jc::no_move_assign>::value == true, "jc::no_move_assign should be copy constructible");
static_assert(jc::is_copy_assignable<jc::no_move_assign>::value == true, "jc::no_move_assign should be copy assignable");
static_assert(jc::is_move_constructible<jc::no_move_assign>::value == true, "jc::no_move_assign should be move constructible");
static_assert(jc::is_move_assignable<jc::no_move_assign>::value == false, "jc::no_move_assign should not be move assignable");

// Test no_move
static_assert(jc::is_default_constructible<jc::no_move>::value == true, "jc::no_move should be default constructible");
static_assert(jc::is_copy_constructible<jc::no_move>::value == true, "jc::no_move should be copy constructible");
static_assert(jc::is_copy_assignable<jc::no_move>::value == true, "jc::no_move should be copy assignable");
static_assert(jc::is_move_constructible<jc::no_move>::value == false, "jc::no_move should not be move constructible");
static_assert(jc::is_move_assignable<jc::no_move>::value == false, "jc::no_move should not be move assignable");

// Test no_copy_or_move
static_assert(jc::is_default_constructible<jc::no_copy_or_move>::value == true, "jc::no_copy_or_move should be default constructible");
static_assert(jc::is_copy_constructible<jc::no_copy_or_move>::value == false, "jc::no_copy_or_move should not be copy constructible");
static_assert(jc::is_copy_assignable<jc::no_copy_or_move>::value == false, "jc::no_copy_or_move should not be copy assignable");
static_assert(jc::is_move_constructible<jc::no_copy_or_move>::value == false, "jc::no_copy_or_move should not be move constructible");
static_assert(jc::is_move_assignable<jc::no_copy_or_move>::value == false, "jc::no_copy_or_move should not be move assignable");

int main()
{
	NEWTEST();
	// Always passes as this test is all static asserts.
	PASS();
};