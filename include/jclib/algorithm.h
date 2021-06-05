#pragma once
#ifndef JCLIB_ALGORITHM_H
#define JCLIB_ALGORITHM_H

#include "jclib/ranges.h"

#include <algorithm>

#define _JCLIB_ALGORITHM_

namespace jc
{
	using std::find;
	using std::find_if;

	template <typename RangeT, typename T> JCLIB_REQUIRES(cx_range<RangeT>)
	JCLIB_CONSTEXPR static auto find(RangeT& _range, const T& _val)
	{
		return find(begin(_range), end(_range), _val);
	};
};

#endif