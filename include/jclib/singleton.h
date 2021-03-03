#pragma once
#ifndef JCLIB_SINGLETON_H
#define JCLIB_SINGLETON_H

#define _JCLIB_SINGLETON_

#include "config.h"

#if defined(__cpp_inline_variables)

#if defined(__cpp_lib_optional)
#include <optional>
namespace jc
{
	template <typename T>
	using optional = std::optional<T>;
};
#else
#error "needs optional support"
#endif

namespace jc
{

#if defined(__cpp_concepts)
	template <typename T> requires
		std::is_default_constructible_v<T>
#else
	template <typename T>
#endif
	struct default_construction_method
	{
		static bool make(std::optional<T>& _opt)
		{
			_opt.emplace();
			return _opt.has_value();
		};
	};

	namespace impl
	{
		template <typename T, typename TagT = void, typename ConstructorT = default_construction_method<T>>
		extern inline std::optional<T> singleton_value_t{ std::nullopt };
	};

	template <typename T, typename TagT = void, typename ConstructorT = default_construction_method<T>>
	static inline T& get_singleton()
	{
		auto& _sval = impl::singleton_value_t<T, TagT, ConstructorT>;
		if (_sval) [[likely]]
		{
			return _sval.value();
		}
		else
		{
			auto _result = ConstructorT::make(_sval);
#ifndef NDEBUG
			if (!_result)
			{
				JCLIB_ABORT();
			};
#endif
			return _sval.value();
		};
	};

};

#endif

#endif