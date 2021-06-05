#pragma once
#ifndef JCLIB_FUNCTIONAL_H
#define JCLIB_FUNCTIONAL_H

#include "jclib/config.h"
#include "jclib/type_traits.h"

#define _JCLIB_FUNCTIONAL_

namespace jc
{
	template <typename Op, typename... ArgTs>
	JCLIB_CONSTEXPR inline auto invoke(Op&& _op, ArgTs&&... _args)
		-> invoke_result_t<Op, decltype(std::forward<ArgTs>(std::declval<ArgTs&&>()))...>
	{
		return _op(std::forward<ArgTs>(_args)...);
	};
	template <typename ReturnT, typename ClassT, typename... ArgTs>
	JCLIB_CONSTEXPR inline auto invoke(ReturnT(ClassT::*_op)(ArgTs...), ClassT* _class, ArgTs&&... _args) -> ReturnT
	{
		return (_class->*_op)(std::forward<ArgTs>(_args)...);
	};

};

#endif