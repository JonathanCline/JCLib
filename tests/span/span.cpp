#include <jclib/span.h>
#include <jclib-test.hpp>

#include <array>





int subtest_construction()
{
	NEWTEST();

	{
		jc::span<int> _span{};
		jc::span<const int> _cspan{};

		static_assert(jc::is_same<decltype(_span)::value_type, int>::value, "");
		static_assert(jc::is_same<decltype(_span)::reference, int&>::value, "");
		static_assert(jc::is_same<decltype(_span)::pointer, int*>::value, "");

		static_assert(jc::is_same<decltype(_cspan)::value_type, const int>::value, "");
		static_assert(jc::is_same<decltype(_cspan)::reference, const int&>::value, "");
		static_assert(jc::is_same<decltype(_cspan)::pointer, const int*>::value, "");



		static_assert(jc::is_same<decltype(_span.size()), typename jc::span<int>::size_type>::value,
			"span::size did not return size_type");
		static_assert(jc::is_same<jc::span<int>, jc::span<int, jc::dynamic_extent>>::value,
			"dynamic_extent is not default extent for span");
		
		static_assert(sizeof(jc::span<int, 2>) == sizeof(int*),
			"static extent span type size mismatch");
		static_assert(sizeof(jc::span<int>) == (sizeof(int*) + sizeof(size_t)),
			"dynamic extent span type size mismatch");
	};

	{
		std::array<int, 4> _nums{};
		jc::span<int> _span{ _nums };
	};

	PASS();
};












int main()
{
	NEWTEST();
	SUBTEST(subtest_construction);
	PASS();
};