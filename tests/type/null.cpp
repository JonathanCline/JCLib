#include <jclib-test.hpp>
#include <jclib/type.h>

// Essentials

static_assert(nullptr == jc::null, "");
static_assert(jc::null == nullptr, "");
static_assert(!(jc::null != nullptr), "");
static_assert(!(nullptr != jc::null), "");



int main()
{
	return 0;
};