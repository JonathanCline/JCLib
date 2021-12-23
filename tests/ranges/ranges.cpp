#include "jclib/ranges.h"
#include "jclib/functional.h"
#include "jclib/iterator.h"
#include "jclib/type.h"

#include <jclib-test.hpp>

#include <vector>
#include <numeric>
#include <string>
#include <functional>
#include <array>

#include <iostream>



template <typename RangeT, typename OpT>
int test_transform(const RangeT& _range, OpT _op)
{
	NEWTEST();

	std::vector<jc::ranges::value_t<RangeT>> _expected{};
	for (auto v : _range)
	{
		_expected.push_back(jc::invoke(_op, v));
	};

	auto _expectedIt = _expected.begin();
	for (auto v : _range | jc::views::transform(_op))
	{
		ASSERT(v == *_expectedIt, "transformed value mismatch");
		++_expectedIt;
	};
	ASSERT(_expectedIt == _expected.end(), "transform view size mismatch");

	PASS();
};

// Transform view tests
int test_transform()
{
	NEWTEST();

	const auto _transformFn = [](const auto v)
	{
		return v * 2;
	};

	{	// Testing with a non-const C++ range
		std::vector<int> _ints =
		{
			1, 2, 3, 4
		};
		SUBTEST(test_transform, _ints, _transformFn);
	};

	{	// Testing with a non-const C-Style range

		int _ints[] =
		{
			1, 2, 3, 4
		};
		SUBTEST(test_transform, _ints, _transformFn);
	};

	{	// Testing with a const C++ range
		const std::vector<int> _ints =
		{
			1, 2, 3, 4
		};
		SUBTEST(test_transform, _ints, _transformFn);
	};

	{	// Testing with a const C-Style range
		const int _ints[] =
		{
			1, 2, 3, 4
		};
		SUBTEST(test_transform, _ints, _transformFn);
	};

	PASS();
};






template <typename RangeT, typename IterT>
int test_range_iterator_functions(RangeT&& _range, IterT _begin, IterT _end)
{
	NEWTEST();

	ASSERT(jc::begin(_range) == _begin, "ranges::begin mismatch");
	ASSERT(jc::end(_range) == _end, "ranges::end mismatch");
	ASSERT(jc::ranges::distance(_range) == jc::distance(_begin, _end), "ranges::distance mismatch");

	PASS();
};



int test_range_iterator_functions()
{
	NEWTEST();

	{	// Test with non-const C range

		constexpr size_t _len = 4;
		using value_type = int;
	
		value_type _arr[_len]{ 1, 2, 3, 4 };
		
		SUBTEST(test_range_iterator_functions, _arr, &_arr[0], (&_arr[0] + _len));
	};

	{	// Test with non-const C++ range

		using value_type = int;
		std::vector<value_type> _arr{ 1, 2, 3, 4 };
		SUBTEST(test_range_iterator_functions, _arr, _arr.begin(), _arr.end());
	};

	{	// Test with const C range

		constexpr size_t _len = 4;
		using value_type = int;
		const value_type _arr[_len]{ 1, 2, 3, 4 };
		SUBTEST(test_range_iterator_functions, _arr, &_arr[0], (&_arr[0] + _len));
	};

	{	// Test with const C++ range

		using value_type = int;
		const std::vector<value_type> _arr{ 1, 2, 3, 4 };
		SUBTEST(test_range_iterator_functions, _arr, _arr.begin(), _arr.end());
	};

	PASS();
};





constexpr bool is_even(int i)
{
	return (i % 2) == 0;
};
constexpr auto double_val(int i)
{
	return i * 2;
};

int main()
{
	NEWTEST();

	SUBTEST(test_range_iterator_functions);

	{
		NEWTEST();

		std::vector<int> _ivec{};
		_ivec.resize(10);
		std::iota(_ivec.begin(), _ivec.end(), 1);

		auto _filterView = _ivec | jc::views::filter(&is_even);
		const auto _len = _filterView.size();
		if (_len != 5)
		{
			FAIL("length mismatch on filter view");
		};

		for (auto v : _ivec | jc::views::filter(&is_even))
		{
			if (!is_even(v))
			{
				FAIL("filter view failed to filter");
			};
		};
	};

	{
		NEWTEST();

		constexpr int _completeCount = 10;
		constexpr int _evenCount = _completeCount / 2;
		using iotav = decltype(jc::views::iota(1, _completeCount));

		if (!jc::ranges::is_range<iotav>::value)
		{
			FAIL();
		};

		int _count = 0;
		for (auto v : jc::views::iota(0, _completeCount) | jc::views::filter(&is_even))
		{
			++_count;
		};

		if (_count != _evenCount)
		{
			FAIL();
		};
	};

	{
		NEWTEST();

		std::string _str = "haha cheese";
		auto it = _str.begin();
		for (auto c : _str | jc::views::all)
		{
			if ((*it++) != c)
			{
				FAIL();
			};
		};
		auto _allView = _str | jc::views::all;
		if (_allView.size() != _str.size())
		{
			FAIL();
		};
	};

	{
		NEWTEST();

		constexpr size_t _dropCount = 2;

		const auto _ivec = jc::range_cast<std::vector<int>>(jc::views::iota(1, 10));
		auto _dropped = _ivec | jc::views::drop(_dropCount);
		
		ASSERT(*_dropped.begin() == _ivec.at(_dropCount));
		ASSERT(_dropped.size() == (_ivec.size() - _dropCount));
	};

	// Run transform view tests
	SUBTEST(test_transform);

	PASS();
};