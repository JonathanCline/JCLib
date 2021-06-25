#include "jclib/ranges.h"
#include "jclib/functional.h"
#include "jclib/iterator.h"

#include <vector>
#include <numeric>
#include <string>
#include <functional>


namespace jc
{
	namespace ranges
	{
		template <typename RangeT, typename = iterator_t<RangeT>>
		struct all_view : view_interface<all_view<RangeT>>
		{
		public:
			using iterator = iterator_t<RangeT>;
		private:
			iterator begin_;
			iterator end_;
		public:
			constexpr iterator begin() const noexcept { return this->begin_; };
			constexpr iterator end() const noexcept { return this->end_; };

			constexpr all_view(RangeT& _range) :
				begin_{ ranges::begin(_range) }, end_{ ranges::end(_range) }
			{};
		};
	};
	namespace views
	{
		namespace impl
		{
			struct all_t
			{
				template <typename RangeT>
				constexpr auto operator()(RangeT&& _range) const noexcept -> ranges::all_view<remove_reference_t<RangeT>>
				{
					return ranges::all_view<remove_reference_t<RangeT>>(_range);
				};
				template <typename RangeT>
				constexpr friend inline auto operator|(RangeT&& _range, const all_t& _all) noexcept ->
					ranges::all_view<remove_reference_t<RangeT>>
				{
					return _all(_range);
				};
			};
		};
		constexpr static impl::all_t all{};
	};
};



namespace jc
{
	namespace ranges
	{
		template <typename RangeT, typename = iterator_t<RangeT>>
		struct drop_view : view_interface<drop_view<RangeT>>
		{
		public:
			using iterator = iterator_t<RangeT>;
		private:
			iterator begin_;
			iterator end_;
		public:
			constexpr iterator begin() const noexcept { return this->begin_; };
			constexpr iterator end() const noexcept { return this->end_; };
			constexpr drop_view(RangeT& _range, size_t _count) :
				begin_{ jc::next(ranges::begin(_range), static_cast<jc::difference_type_t<RangeT>>(_count)) }, end_{ ranges::end(_range) }
			{};
		};
	};
	namespace views
	{
		namespace impl
		{
			struct drop_impl_t
			{
				template <typename RangeT>
				constexpr friend inline auto operator|(RangeT&& _range, drop_impl_t _drop) noexcept ->
					ranges::drop_view<remove_reference_t<RangeT>>
				{
					return ranges::drop_view<remove_reference_t<RangeT>>{ std::forward<RangeT>(_range), _drop.count_ };
				};
				size_t count_;
			};

			struct drop_t
			{
				constexpr drop_impl_t operator()(size_t _count) const noexcept
				{
					return drop_impl_t{ _count };
				};
				template <typename RangeT>
				constexpr auto operator()(RangeT&& _range, size_t _count) const noexcept
				{
					return _range | drop_impl_t{ _count };
				};
			};

		};
		constexpr static impl::drop_t drop{};
	};
};



constexpr bool is_even(int i)
{
	return (i % 2) == 0;
};

int main()
{
	{
		constexpr size_t _len = 4;
		int _arr[_len]{ 1, 2, 3, 4 };

		static_assert(jc::ranges::is_range<decltype(_arr)>::value, "");
		static_assert(jc::is_same<jc::ranges::value_t<decltype(_arr)>, int>::value, "");
		static_assert(jc::is_same<jc::ranges::iterator_t<decltype(_arr)>, int*>::value, "");

		auto _arrBegin = &_arr[0];

		if (_arrBegin != jc::begin(_arr))
		{
			return 1;
		};
		if (_arrBegin + _len != jc::end(_arr))
		{
			return 1;
		};
	};

	{
		std::vector<int> _ivec{ 1, 2, 3, 4 };
		static_assert(jc::ranges::is_range<decltype(_ivec)>::value, "");
		static_assert(jc::is_same<jc::ranges::value_t<decltype(_ivec)>, int>::value, "");
		static_assert(jc::is_same<jc::ranges::iterator_t<decltype(_ivec)>, typename decltype(_ivec)::iterator>::value, "");

		if (_ivec.begin() != jc::begin(_ivec))
		{
			return 2;
		};
		if (_ivec.end() != jc::end(_ivec))
		{
			return 2;
		};
	
	};


	{
		const std::vector<int> _ivec{ 1, 2, 3, 4 };
		using ree = jc::ranges::value_t<const std::vector<int>>;

		static_assert(jc::is_same<jc::ranges::value_t<decltype(_ivec)>, const int>::value, "");
		static_assert(jc::is_same<jc::ranges::iterator_t<decltype(_ivec)>, typename decltype(_ivec)::const_iterator>::value, "");

		if (_ivec.begin() != jc::begin(_ivec))
		{
			return 2;
		};
		if (_ivec.end() != jc::end(_ivec))
		{
			return 2;
		};
	};
	
	{
		std::vector<int> _ivec{};
		_ivec.resize(10);
		std::iota(_ivec.begin(), _ivec.end(), 1);

		auto _filterView = _ivec | jc::views::filter(&is_even);
		const auto _len = _filterView.size();
		if (_len != 5)
		{
			return 3;
		};

		for (auto v : _ivec | jc::views::filter(&is_even))
		{
			if (!is_even(v))
			{
				return 3;
			};
		};
	};

	{
		constexpr int _completeCount = 10;
		constexpr int _evenCount = _completeCount / 2;
		using iotav = decltype(jc::views::iota(1, _completeCount));

		if (!jc::ranges::is_view<iotav>::value || !jc::ranges::is_range<iotav>::value)
		{
			return 4;
		};

		int _count = 0;
		for (auto v : jc::views::iota(0, _completeCount) | jc::views::filter(&is_even))
		{
			++_count;
		};

		if (_count != _evenCount)
		{
			return 4;
		};
	};

	{
		std::string _str = "haha cheese";
		auto it = _str.begin();
		for (auto c : _str | jc::views::all)
		{
			if ((*it++) != c)
			{
				return 5;
			};
		};
		auto _allView = _str | jc::views::all;
		if (_allView.size() != _str.size())
		{
			return 5;
		};
	};

	{
		constexpr size_t _dropCount = 2;

		const auto _ivec = jc::range_cast<std::vector<int>>(jc::views::iota(1, 10));
		auto _dropped = _ivec | jc::views::drop(_dropCount);
		if (*_dropped.begin() != _ivec.at(_dropCount))
		{
			return 6;
		};

		if (_dropped.size() != _ivec.size() - _dropCount)
		{
			return 6;
		};
	};


	return 0;
};