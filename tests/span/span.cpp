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

// tests iterator functionality
int subtest_iterator()
{
	NEWTEST();

	std::array<int, 4> _nums{ 0, 1, 2, 3 };
	jc::span<int> _span{ _nums };

	static_assert(jc::is_same<decltype(_span)::iterator, decltype(_span.begin())>::value, "span iterator type mismatch");
	static_assert(jc::is_same<decltype(_span)::iterator, decltype(_span.end())>::value, "span iterator type mismatch");

	{
		const auto _diff = _span.end() - _span.begin();
		ASSERT(_diff == _span.size(), "(end - begin) is not same size as span");
		ASSERT(_span.begin() + _span.size() == _span.end(), "cannot recreate end iterator using begin and size");
		ASSERT(_span.end() > _span.begin(), "begin is not before end");
	};

#if JCLIB_FEATURE_CONCEPTS_V
	// Check iterator using standard library concepts
	static_assert(std::forward_iterator<decltype(_span.begin())>);
	static_assert(std::bidirectional_iterator<decltype(_span.begin())>);
	static_assert(std::random_access_iterator<decltype(_span.begin())>);
	static_assert(std::contiguous_iterator<decltype(_span.begin())>);
#endif

	{
		size_t n = 0;
		for (auto& v : _span)
		{
			ASSERT(_span[n] == v, "range based for over span value mismatch");
			++n;
		};
	};

	{
		ASSERT(_span.begin()[2] == _span[2], "square bracket comparison does not match");
	};

	{
		{
			auto it = _span.begin();
			auto a = ++it;
			ASSERT(a == it, "pre-increment is invalid");
			ASSERT(it != _span.begin(), "pre-increment did not modify the iterator");
		};
		{
			auto it = _span.begin();
			auto a = it++;
			ASSERT(a == _span.begin(), "post-increment is invalid");
			ASSERT(it != _span.begin(), "post-increment did not modify the iterator");
		};
	};

	PASS();
};

// ensures functionality with jc::ranges
int subtest_ranges()
{
	NEWTEST();

	static_assert(jc::is_range<jc::span<int>>::value, "span is not contiguous range");
	static_assert(jc::is_range<jc::span<int, 5>>::value, "span is not contiguous range");

	static_assert(jc::is_contiguous_range<jc::span<int>>::value, "span is not contiguous range");
	static_assert(jc::is_contiguous_range<jc::span<int, 5>>::value, "span is not contiguous range");

	PASS();
};





int main()
{
	NEWTEST();
	SUBTEST(subtest_construction);
	SUBTEST(subtest_iterator);
	SUBTEST(subtest_ranges);
	PASS();
};