#include <jclib/type_traits.h>

#include <jclib-test.hpp>




// Checks if a function trait has the base noexcept_function_tag type
template <typename T>
struct is_ftrait_with_noexcept :
	jc::bool_constant<jc::is_base_of<jc::impl::noexcept_function_tag, T>::value>
{};

// Checks if a function trait has the base const_function_tag type
template <typename T>
struct is_ftrait_with_const :
	jc::bool_constant<jc::is_base_of<jc::impl::const_function_tag, T>::value>
{};




/*
	Section for function signature traits
*/

#pragma region FUNCTION_SIGNATURES

struct FunctionSignatureTesting
{
	using return_type = void;
	using signature_type = void(int);

	static_assert(jc::is_function<signature_type>::value,
		"is_function value mismatch for function signatures");
	static_assert(jc::is_same<jc::invoke_result_t<signature_type, int>, return_type>::value,
		"signature_type value mismatch for function signatures");

};

#pragma endregion FUNCTION_SIGNATURE



/*
	Section for member function traits
*/

#pragma region MEMBER_FUNCTIONS

struct MemberFunctions_Basic
{
	// This type for member function class type
	using this_type = MemberFunctions_Basic;

	// Member function return type
	using return_type = int;

	// Arguements tuple
	using arguement_typelist = std::tuple<>;

	// Arguement count value
	constexpr static size_t arguement_count = 0;
	
	// Function signature type
	using signature_type = return_type();





	// Member function variations

	return_type get() { return 0; };
	return_type get_noexcept() noexcept { return 0; };
	return_type get_const() const { return 0; };
	return_type get_const_noexcept() const noexcept { return 0; };


	// Member function types

	using get_fn = decltype(&this_type::get);
	using get_noexcept_fn = decltype(&this_type::get_noexcept);
	using get_const_fn = decltype(&this_type::get_const);
	using get_const_noexcept_fn = decltype(&this_type::get_const_noexcept);



	// Function traits handling member functions testing

	using get_fntraits = jc::impl::function_traits_impl<get_fn>;
	using get_noexcept_fntraits = jc::impl::function_traits_impl<get_noexcept_fn>;
	using get_const_fntraits = jc::impl::function_traits_impl<get_const_fn>;
	using get_const_noexcept_fntraits = jc::impl::function_traits_impl<get_const_noexcept_fn>;



	// Is function testing

	static_assert(jc::is_function<get_fn>::value,
		"is_function mismatch for member function");
	static_assert(jc::is_function<get_noexcept_fn>::value,
		"is_function mismatch for noexcept member function");
	static_assert(jc::is_function<get_const_fn>::value,
		"is_function mismatch for const member function");
	static_assert(jc::is_function<get_const_noexcept_fn>::value,
		"is_function mismatch for const noexcept member function");

#if JCLIB_FEATURE_INLINE_VARIABLES_V
	static_assert(jc::is_function<get_fn>::value == jc::is_function_v<get_fn>,
		"is_function_v mismatch for member function");
	static_assert(jc::is_function<get_noexcept_fn>::value == jc::is_function_v<get_noexcept_fn>,
		"is_function_v mismatch for noexcept member function");
	static_assert(jc::is_function<get_const_fn>::value == jc::is_function_v<get_const_fn>,
		"is_function_v mismatch for const member function");
	static_assert(jc::is_function<get_const_noexcept_fn>::value == jc::is_function_v<get_const_noexcept_fn>,
		"is_function_v mismatch for const noexcept member function");
#endif


	// Is function pointer testing

	static_assert(jc::is_function_pointer<get_fn>::value,
		"is_function_pointer mismatch for member function");
	static_assert(jc::is_function_pointer<get_noexcept_fn>::value,
		"is_function_pointer mismatch for noexcept member function");
	static_assert(jc::is_function_pointer<get_const_fn>::value,
		"is_function_pointer mismatch for const member function");
	static_assert(jc::is_function_pointer<get_const_noexcept_fn>::value,
		"is_function_pointer mismatch for const noexcept member function");

#if JCLIB_FEATURE_INLINE_VARIABLES_V
	static_assert(jc::is_function_pointer<get_fn>::value == jc::is_function_pointer_v<get_fn>,
		"is_function_pointer_v mismatch for member function");
	static_assert(jc::is_function_pointer<get_noexcept_fn>::value == jc::is_function_pointer_v<get_noexcept_fn>,
		"is_function_pointer_v mismatch for noexcept member function");
	static_assert(jc::is_function_pointer<get_const_fn>::value == jc::is_function_pointer_v<get_const_fn>,
		"is_function_pointer_v mismatch for const member function");
	static_assert(jc::is_function_pointer<get_const_noexcept_fn>::value == jc::is_function_pointer_v<get_const_noexcept_fn>,
		"is_function_pointer_v mismatch for const noexcept member function");
#endif



	// Is member function pointer testing

	static_assert(jc::is_member_function_pointer<get_fn>::value,
		"is_member_function_pointer mismatch for member function");
	static_assert(jc::is_member_function_pointer<get_noexcept_fn>::value,
		"is_member_function_pointer mismatch for noexcept member function");
	static_assert(jc::is_member_function_pointer<get_const_fn>::value,
		"is_member_function_pointer mismatch for const member function");
	static_assert(jc::is_member_function_pointer<get_const_noexcept_fn>::value,
		"is_member_function_pointer mismatch for const noexcept member function");

#if JCLIB_FEATURE_INLINE_VARIABLES_V
	static_assert(jc::is_member_function_pointer<get_fn>::value ==
			jc::is_member_function_pointer_v<get_fn>,
		"is_member_function_pointer_v mismatch for member function");
	static_assert(jc::is_member_function_pointer<get_noexcept_fn>::value ==
			jc::is_member_function_pointer_v<get_noexcept_fn>,
		"is_member_function_pointer_v mismatch for noexcept member function");
	static_assert(jc::is_member_function_pointer<get_const_fn>::value ==
			jc::is_member_function_pointer_v<get_const_fn>,
		"is_member_function_pointer_v mismatch for const member function");
	static_assert(jc::is_member_function_pointer<get_const_noexcept_fn>::value ==
			jc::is_member_function_pointer_v<get_const_noexcept_fn>,
		"is_member_function_pointer_v mismatch for const noexcept member function");
#endif



	// Is invocable testing

	static_assert(jc::is_invocable<get_fn>::value,
		"is_invocable mismatch for member function");
	static_assert(jc::is_invocable<get_noexcept_fn>::value,
		"is_invocable mismatch for noexcept member function");
	static_assert(jc::is_invocable<get_const_fn>::value,
		"is_invocable mismatch for const member function");
	static_assert(jc::is_invocable<get_const_noexcept_fn>::value,
		"is_invocable mismatch for const noexcept member function");

#if JCLIB_FEATURE_INLINE_VARIABLES_V
	static_assert(jc::is_invocable_v<get_fn> && jc::is_invocable<get_fn>::value,
		"is_invocable_v mismatch for member function");
	static_assert(jc::is_invocable_v<get_noexcept_fn> && jc::is_invocable<get_noexcept_fn>::value,
		"is_invocable_v mismatch for noexcept member function");
	static_assert(jc::is_invocable_v<get_const_fn> && jc::is_invocable<get_const_fn>::value,
		"is_invocable_v mismatch for const member function");
	static_assert(jc::is_invocable_v<get_const_noexcept_fn> && jc::is_invocable<get_const_noexcept_fn>::value,
		"is_invocable_v mismatch for const noexcept member function");
#endif



	// Invoke result testing

	static_assert(jc::is_same<jc::invoke_result_t<get_fn>, return_type>::value,
		"invoke_result_t mismatch for member function");
	static_assert(jc::is_same<jc::invoke_result_t<get_noexcept_fn>, return_type>::value,
		"invoke_result_t mismatch for noexcept member function");
	static_assert(jc::is_same<jc::invoke_result_t<get_const_fn>, return_type>::value,
		"invoke_result_t mismatch for const member function");
	static_assert(jc::is_same<jc::invoke_result_t<get_const_noexcept_fn>, return_type>::value,
		"invoke_result_t mismatch for const noexcept member function");



	// Return type testing

	static_assert(jc::is_same<typename get_fntraits::return_type, return_type>::value,
		"return type mismatch for member function");
	static_assert(jc::is_same<typename get_noexcept_fntraits::return_type, return_type>::value,
		"return type mismatch for noexcept member function");
	static_assert(jc::is_same<typename get_const_fntraits::return_type, return_type>::value,
		"return type mismatch for const member function");
	static_assert(jc::is_same<typename get_const_noexcept_fntraits::return_type, return_type>::value,
		"return type mismatch for const noexcept member function");



	// Function signature testing

	static_assert(jc::is_same<jc::function_signature_t<get_fn>, signature_type>::value,
		"function signature mismatch for member function");
	static_assert(jc::is_same<jc::function_signature_t<get_noexcept_fn>, signature_type>::value,
		"function signature mismatch for noexcept member function");
	static_assert(jc::is_same<jc::function_signature_t<get_const_fn>, signature_type>::value,
		"function signature mismatch for const member function");
	static_assert(jc::is_same<jc::function_signature_t<get_const_noexcept_fn>, signature_type>::value,
		"function signature mismatch for const noexcept member function");



	// Arguement count testing

	static_assert(jc::function_arguement_count<get_fn>::value == arguement_count,
		"arguement count mismatch for member function");
	static_assert(jc::function_arguement_count<get_noexcept_fn>::value == arguement_count,
		"arguement count mismatch for noexcept member function");
	static_assert(jc::function_arguement_count<get_const_fn>::value == arguement_count,
		"arguement count mismatch for const member function");
	static_assert(jc::function_arguement_count<get_const_noexcept_fn>::value == arguement_count,
		"arguement count mismatch for const noexcept member function");



	// Arguement typelist testing

	static_assert(jc::is_same<typename get_fntraits::arguement_typelist, arguement_typelist>::value,
		"arguement typelist mismatch for member function");
	static_assert(jc::is_same<typename get_noexcept_fntraits::arguement_typelist, arguement_typelist>::value,
		"arguement typelist mismatch for noexcept member function");
	static_assert(jc::is_same<typename get_const_fntraits::arguement_typelist, arguement_typelist>::value,
		"arguement typelist mismatch for const member function");
	static_assert(jc::is_same<typename get_const_noexcept_fntraits::arguement_typelist, arguement_typelist>::value,
		"arguement typelist mismatch for const noexcept member function");


	// Arguement typelist testing with actual arguement type trait

	static_assert(jc::is_same<typename get_fntraits::arguement_typelist, arguement_typelist>::value,
		"arguement typelist mismatch for member function");
	static_assert(jc::is_same<typename get_noexcept_fntraits::arguement_typelist, arguement_typelist>::value,
		"arguement typelist mismatch for noexcept member function");
	static_assert(jc::is_same<typename get_const_fntraits::arguement_typelist, arguement_typelist>::value,
		"arguement typelist mismatch for const member function");
	static_assert(jc::is_same<typename get_const_noexcept_fntraits::arguement_typelist, arguement_typelist>::value,
		"arguement typelist mismatch for const noexcept member function");



	// Class type testing for non-const member functions

	static_assert(jc::is_same<typename get_fntraits::class_type, this_type>::value,
		"function traits class type mismatch for member function");
	static_assert(jc::is_same<typename get_noexcept_fntraits::class_type, this_type>::value,
		"function traits class type mismatch for noexcept member function");
	static_assert(jc::is_same<typename get_const_fntraits::class_type, this_type>::value,
		"function traits class type mismatch for const member function");
	static_assert(jc::is_same<typename get_const_noexcept_fntraits::class_type, this_type>::value,
		"function traits class type mismatch for const noexcept member function");



	// Const tag testing

	static_assert(!is_ftrait_with_const<get_fntraits>::value,
		"const function trait value mismatch for member function");
	static_assert(!is_ftrait_with_const<get_noexcept_fntraits>::value,
		"const function trait value mismatch for noexcept member function");

	static_assert(is_ftrait_with_const<get_const_fntraits>::value,
		"const function trait value mismatch for const member function");
	static_assert(is_ftrait_with_const<get_const_noexcept_fntraits>::value,
		"const function trait value mismatch for consst noexcept member function");



	// Noexcept tag testing

	static_assert(!is_ftrait_with_noexcept<get_fntraits>::value,
		"noexcept function trait value mismatch for member function");
	static_assert(!is_ftrait_with_noexcept<get_const_fntraits>::value,
		"noexcept function trait value mismatch for const member function");

#if JCLIB_FEATURE_CPP_NOEXCEPT_FUNCTION_TYPE_V
	static_assert(is_ftrait_with_noexcept<get_noexcept_fntraits>::value,
		"noexcept function trait value mismatch for noexcept member function");
	static_assert(is_ftrait_with_noexcept<get_const_noexcept_fntraits>::value,
		"noexcept function trait value mismatch for const noexcept member function");
#else
	static_assert(!is_ftrait_with_noexcept<get_noexcept_fntraits>::value,
		"noexcept function trait value mismatch for noexcept member function");
	static_assert(!is_ftrait_with_noexcept<get_const_noexcept_fntraits>::value,
		"noexcept function trait value mismatch for const noexcept member function");
#endif

};

struct MemberFunctions_Basic_MultiArgs
{
	// This type for member function class type
	using this_type = MemberFunctions_Basic_MultiArgs;

	// Other type for testing that member function class type differences are spotted
	using other_type = MemberFunctions_Basic;

	// Member function return type
	using return_type = int;

	// Arguements tuple
	using arguement_typelist = std::tuple<int, int>;

	// Arguement count value
	constexpr static size_t arguement_count = 2;

	// Function signature type
	using signature_type = return_type(int, int);





	// Member function variations

	return_type get(int, int) { return 0; };
	return_type get_noexcept(int, int) noexcept { return 0; };
	return_type get_const(int, int) const { return 0; };
	return_type get_const_noexcept(int, int) const noexcept { return 0; };


	// Member function types

	using get_fn = decltype(&this_type::get);
	using get_noexcept_fn = decltype(&this_type::get_noexcept);
	using get_const_fn = decltype(&this_type::get_const);
	using get_const_noexcept_fn = decltype(&this_type::get_const_noexcept);



	// Function traits handling member functions testing

	using get_fntraits = jc::impl::function_traits_impl<get_fn>;
	using get_noexcept_fntraits = jc::impl::function_traits_impl<get_noexcept_fn>;
	using get_const_fntraits = jc::impl::function_traits_impl<get_const_fn>;
	using get_const_noexcept_fntraits = jc::impl::function_traits_impl<get_const_noexcept_fn>;


	// Is function testing

	static_assert(jc::is_function<get_fn>::value,
		"is_function mismatch for member function");
	static_assert(jc::is_function<get_noexcept_fn>::value,
		"is_function mismatch for noexcept member function");
	static_assert(jc::is_function<get_const_fn>::value,
		"is_function mismatch for const member function");
	static_assert(jc::is_function<get_const_noexcept_fn>::value,
		"is_function mismatch for const noexcept member function");

#if JCLIB_FEATURE_INLINE_VARIABLES_V
	static_assert(jc::is_function<get_fn>::value == jc::is_function_v<get_fn>,
		"is_function_v mismatch for member function");
	static_assert(jc::is_function<get_noexcept_fn>::value == jc::is_function_v<get_noexcept_fn>,
		"is_function_v mismatch for noexcept member function");
	static_assert(jc::is_function<get_const_fn>::value == jc::is_function_v<get_const_fn>,
		"is_function_v mismatch for const member function");
	static_assert(jc::is_function<get_const_noexcept_fn>::value == jc::is_function_v<get_const_noexcept_fn>,
		"is_function_v mismatch for const noexcept member function");
#endif


	// Is function pointer testing

	static_assert(jc::is_function_pointer<get_fn>::value,
		"is_function_pointer mismatch for member function");
	static_assert(jc::is_function_pointer<get_noexcept_fn>::value,
		"is_function_pointer mismatch for noexcept member function");
	static_assert(jc::is_function_pointer<get_const_fn>::value,
		"is_function_pointer mismatch for const member function");
	static_assert(jc::is_function_pointer<get_const_noexcept_fn>::value,
		"is_function_pointer mismatch for const noexcept member function");

#if JCLIB_FEATURE_INLINE_VARIABLES_V
	static_assert(jc::is_function_pointer<get_fn>::value == jc::is_function_pointer_v<get_fn>,
		"is_function_pointer_v mismatch for member function");
	static_assert(jc::is_function_pointer<get_noexcept_fn>::value == jc::is_function_pointer_v<get_noexcept_fn>,
		"is_function_pointer_v mismatch for noexcept member function");
	static_assert(jc::is_function_pointer<get_const_fn>::value == jc::is_function_pointer_v<get_const_fn>,
		"is_function_pointer_v mismatch for const member function");
	static_assert(jc::is_function_pointer<get_const_noexcept_fn>::value == jc::is_function_pointer_v<get_const_noexcept_fn>,
		"is_function_pointer_v mismatch for const noexcept member function");
#endif



	// Is member function pointer testing

	static_assert(jc::is_member_function_pointer<get_fn>::value,
		"is_member_function_pointer mismatch for member function");
	static_assert(jc::is_member_function_pointer<get_noexcept_fn>::value,
		"is_member_function_pointer mismatch for noexcept member function");
	static_assert(jc::is_member_function_pointer<get_const_fn>::value,
		"is_member_function_pointer mismatch for const member function");
	static_assert(jc::is_member_function_pointer<get_const_noexcept_fn>::value,
		"is_member_function_pointer mismatch for const noexcept member function");

#if JCLIB_FEATURE_INLINE_VARIABLES_V
	static_assert(jc::is_member_function_pointer<get_fn>::value ==
		jc::is_member_function_pointer_v<get_fn>,
		"is_member_function_pointer_v mismatch for member function");
	static_assert(jc::is_member_function_pointer<get_noexcept_fn>::value ==
		jc::is_member_function_pointer_v<get_noexcept_fn>,
		"is_member_function_pointer_v mismatch for noexcept member function");
	static_assert(jc::is_member_function_pointer<get_const_fn>::value ==
		jc::is_member_function_pointer_v<get_const_fn>,
		"is_member_function_pointer_v mismatch for const member function");
	static_assert(jc::is_member_function_pointer<get_const_noexcept_fn>::value ==
		jc::is_member_function_pointer_v<get_const_noexcept_fn>,
		"is_member_function_pointer_v mismatch for const noexcept member function");
#endif



	// Is invocable testing

	static_assert(jc::is_invocable<get_fn, int, int>::value,
		"is_invocable mismatch for member function");
	static_assert(jc::is_invocable<get_noexcept_fn, int, int>::value,
		"is_invocable mismatch for noexcept member function");
	static_assert(jc::is_invocable<get_const_fn, int, int>::value,
		"is_invocable mismatch for const member function");
	static_assert(jc::is_invocable<get_const_noexcept_fn, int, int>::value,
		"is_invocable mismatch for const noexcept member function");

#if JCLIB_FEATURE_INLINE_VARIABLES_V
	static_assert(jc::is_invocable_v<get_fn, int, int> && jc::is_invocable<get_fn, int, int>::value,
		"is_invocable_v mismatch for member function");
	static_assert(jc::is_invocable_v<get_noexcept_fn, int, int> && jc::is_invocable<get_noexcept_fn, int, int>::value,
		"is_invocable_v mismatch for noexcept member function");
	static_assert(jc::is_invocable_v<get_const_fn, int, int> && jc::is_invocable<get_const_fn, int, int>::value,
		"is_invocable_v mismatch for const member function");
	static_assert(jc::is_invocable_v<get_const_noexcept_fn, int, int> && jc::is_invocable<get_const_noexcept_fn, int, int>::value,
		"is_invocable_v mismatch for const noexcept member function");
#endif



	// Invoke result testing

	static_assert(jc::is_same<jc::invoke_result_t<get_fn, int, int>, return_type>::value,
		"invoke_result_t mismatch for member function");
	static_assert(jc::is_same<jc::invoke_result_t<get_noexcept_fn, int, int>, return_type>::value,
		"invoke_result_t mismatch for noexcept member function");
	static_assert(jc::is_same<jc::invoke_result_t<get_const_fn, int, int>, return_type>::value,
		"invoke_result_t mismatch for const member function");
	static_assert(jc::is_same<jc::invoke_result_t<get_const_noexcept_fn, int, int>, return_type>::value,
		"invoke_result_t mismatch for const noexcept member function");



	// Return type testing

	static_assert(jc::is_same<typename get_fntraits::return_type, return_type>::value,
		"return type mismatch for member function");
	static_assert(jc::is_same<typename get_noexcept_fntraits::return_type, return_type>::value,
		"return type mismatch for noexcept member function");
	static_assert(jc::is_same<typename get_const_fntraits::return_type, return_type>::value,
		"return type mismatch for const member function");
	static_assert(jc::is_same<typename get_const_noexcept_fntraits::return_type, return_type>::value,
		"return type mismatch for const noexcept member function");



	// Function signature testing

	static_assert(jc::is_same<jc::function_signature_t<get_fn>, signature_type>::value,
		"function signature mismatch for member function");
	static_assert(jc::is_same<jc::function_signature_t<get_noexcept_fn>, signature_type>::value,
		"function signature mismatch for noexcept member function");
	static_assert(jc::is_same<jc::function_signature_t<get_const_fn>, signature_type>::value,
		"function signature mismatch for const member function");
	static_assert(jc::is_same<jc::function_signature_t<get_const_noexcept_fn>, signature_type>::value,
		"function signature mismatch for const noexcept member function");



	// Arguement count testing

	static_assert(jc::function_arguement_count<get_fn>::value == arguement_count,
		"arguement count mismatch for member function");
	static_assert(jc::function_arguement_count<get_noexcept_fn>::value == arguement_count,
		"arguement count mismatch for noexcept member function");
	static_assert(jc::function_arguement_count<get_const_fn>::value == arguement_count,
		"arguement count mismatch for const member function");
	static_assert(jc::function_arguement_count<get_const_noexcept_fn>::value == arguement_count,
		"arguement count mismatch for const noexcept member function");



	// Arguement typelist testing

	static_assert(jc::is_same<typename get_fntraits::arguement_typelist, arguement_typelist>::value,
		"arguement typelist mismatch for member function");
	static_assert(jc::is_same<typename get_noexcept_fntraits::arguement_typelist, arguement_typelist>::value,
		"arguement typelist mismatch for noexcept member function");
	static_assert(jc::is_same<typename get_const_fntraits::arguement_typelist, arguement_typelist>::value,
		"arguement typelist mismatch for const member function");
	static_assert(jc::is_same<typename get_const_noexcept_fntraits::arguement_typelist, arguement_typelist>::value,
		"arguement typelist mismatch for const noexcept member function");


	// Arguement typelist testing with actual arguement type trait

	static_assert(jc::is_same<typename get_fntraits::arguement_typelist, arguement_typelist>::value,
		"arguement typelist mismatch for member function");
	static_assert(jc::is_same<typename get_noexcept_fntraits::arguement_typelist, arguement_typelist>::value,
		"arguement typelist mismatch for noexcept member function");
	static_assert(jc::is_same<typename get_const_fntraits::arguement_typelist, arguement_typelist>::value,
		"arguement typelist mismatch for const member function");
	static_assert(jc::is_same<typename get_const_noexcept_fntraits::arguement_typelist, arguement_typelist>::value,
		"arguement typelist mismatch for const noexcept member function");



	// Class type testing for non-const member functions

	static_assert(jc::is_same<typename get_fntraits::class_type, this_type>::value,
		"function traits class type mismatch for member function");
	static_assert(jc::is_same<typename get_noexcept_fntraits::class_type, this_type>::value,
		"function traits class type mismatch for noexcept member function");
	static_assert(jc::is_same<typename get_const_fntraits::class_type, this_type>::value,
		"function traits class type mismatch for const member function");
	static_assert(jc::is_same<typename get_const_noexcept_fntraits::class_type, this_type>::value,
		"function traits class type mismatch for const noexcept member function");

	

	// Other Member function types

	using other_get_fntraits = jc::impl::function_traits_impl<decltype(&other_type::get)>;
	using other_get_noexcept_fntraits = jc::impl::function_traits_impl<decltype(&other_type::get_noexcept)>;
	using other_get_const_fntraits = jc::impl::function_traits_impl<decltype(&other_type::get_const)>;
	using other_get_const_noexcept_fntraits = jc::impl::function_traits_impl<decltype(&other_type::get_const_noexcept)>;


	// Class type DIFFERENCE testing for member functions

	static_assert(!jc::is_same<typename other_get_fntraits::class_type, this_type>::value,
		"function traits class type mismatch for member function");
	static_assert(!jc::is_same<typename other_get_noexcept_fntraits::class_type, this_type>::value,
		"function traits class type mismatch for noexcept member function");
	static_assert(!jc::is_same<typename other_get_const_fntraits::class_type, this_type>::value,
		"function traits class type mismatch for const member function");
	static_assert(!jc::is_same<typename other_get_const_noexcept_fntraits::class_type, this_type>::value,
		"function traits class type mismatch for const noexcept member function");





	// Const tag testing

	static_assert(!is_ftrait_with_const<get_fntraits>::value,
		"const function trait value mismatch for member function");
	static_assert(!is_ftrait_with_const<get_noexcept_fntraits>::value,
		"const function trait value mismatch for noexcept member function");

	static_assert(is_ftrait_with_const<get_const_fntraits>::value,
		"const function trait value mismatch for const member function");
	static_assert(is_ftrait_with_const<get_const_noexcept_fntraits>::value,
		"const function trait value mismatch for consst noexcept member function");



	// Noexcept tag testing

	static_assert(!is_ftrait_with_noexcept<get_fntraits>::value,
		"noexcept function trait value mismatch for member function");
	static_assert(!is_ftrait_with_noexcept<get_const_fntraits>::value,
		"noexcept function trait value mismatch for const member function");

#if JCLIB_FEATURE_CPP_NOEXCEPT_FUNCTION_TYPE_V
	static_assert(is_ftrait_with_noexcept<get_noexcept_fntraits>::value,
		"noexcept function trait value mismatch for noexcept member function");
	static_assert(is_ftrait_with_noexcept<get_const_noexcept_fntraits>::value,
		"noexcept function trait value mismatch for const noexcept member function");
#else
	static_assert(!is_ftrait_with_noexcept<get_noexcept_fntraits>::value,
		"noexcept function trait value mismatch for noexcept member function");
	static_assert(!is_ftrait_with_noexcept<get_const_noexcept_fntraits>::value,
		"noexcept function trait value mismatch for const noexcept member function");
#endif

};

#pragma endregion MEMBER_FUNCTIONS



/*
	
*/

#pragma region FUNCTION_OBJECTS



#pragma endregion FUNCTION_OBJECTS







int main()
{
	NEWTEST();

	PASS();
};