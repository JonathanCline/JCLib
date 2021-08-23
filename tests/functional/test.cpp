#include <jclib/functional.h>

#include <vector>
#include <numeric>
#include <string>
#include <functional>
#include <array>

#include <iostream>

#define FAIL_I(_code, _msg) \
{	\
	std::vector<std::string> _parts	\
	{ "Failed test at '", std::to_string(__LINE__), "': ", _msg }; \
	for(auto& v : _parts) { std::cout << v; }; std::cout << '\n';	 \
	return _code; \
}

#define SUBTEST(_fn, ...) { const auto _res = _fn( __VA_ARGS__ ); if (_res != 0) { return _res; };  }

inline int newtest()
{
	static int count = 0;
	return ++count;
};

#define NEWTEST() const auto _testCode = ::newtest(); {}
#define FAIL(_msg) FAIL_I(_testCode, _msg)
#define PASS() { return 0; }

#define TIMPL_FRONT(_first, ...) _first

#define ASSERT(_condition, ...) if(!( _condition )) { FAIL(TIMPL_FRONT(__VA_ARGS__)); }




// jc::plus test
int test_op_plus()
{
	NEWTEST();

	constexpr auto operator_v = jc::plus;
	using value_type = int;

	static_assert(jc::has_operator<decltype(operator_v), value_type, value_type>::value, "missing operator");
	static_assert(jc::has_operator<decltype(operator_v), value_type>::value, "missing self-applied operator");


	constexpr value_type operand_a_v = 2;
	constexpr value_type operand_b_v = 2;
	constexpr value_type expected_v = operand_a_v + operand_b_v;


	const value_type a = operand_a_v;
	const value_type b = operand_b_v;
	
	{
		const value_type q = operator_v(a, b);
		ASSERT(q == expected_v, "plus operator failed");
	};

	{
		const value_type q = b | (a & operator_v);
		ASSERT(q == expected_v, "bound and piped plus operator failed");
	};

	PASS();
};

// jc::minus test
int test_op_minus()
{
	NEWTEST();

	constexpr auto operator_v = jc::minus;

	using value_type = int;

	static_assert(jc::has_operator<decltype(operator_v), value_type, value_type>::value, "missing operator");
	static_assert(jc::has_operator<decltype(operator_v), value_type>::value, "missing self-applied operator");


	constexpr value_type operand_a_v = 12;
	constexpr value_type operand_b_v = 2;
	constexpr value_type expected_v = operand_a_v - operand_b_v;


	const value_type a = operand_a_v;
	const value_type b = operand_b_v;

	{
		const value_type q = operator_v(a, b);
		ASSERT(q == expected_v, "minus operator failed");
	};

	{
		const value_type q = b | (a & operator_v);
		ASSERT(q == expected_v, "bound and piped minus operator failed");
	};

	PASS();
};

// jc::times test
int test_op_times()
{
	NEWTEST();

	constexpr auto operator_v = jc::times;

	using value_type = int;

	static_assert(jc::has_operator<decltype(operator_v), value_type, value_type>::value, "missing operator");
	static_assert(jc::has_operator<decltype(operator_v), value_type>::value, "missing self-applied operator");


	constexpr value_type operand_a_v = 12;
	constexpr value_type operand_b_v = 2;
	constexpr value_type expected_v = operand_a_v * operand_b_v;



	const value_type a = operand_a_v;
	const value_type b = operand_b_v;

	{
		const value_type q = operator_v(a, b);
		ASSERT(q == expected_v, "times operator failed");
	};

	{
		const value_type q = b | (a & operator_v);
		ASSERT(q == expected_v, "bound and piped times operator failed");
	};

	PASS();
};

// jc::divide test
int test_op_divide()
{
	NEWTEST();

	constexpr auto operator_v = jc::divide;

	using value_type = int;

	static_assert(jc::has_operator<decltype(operator_v), value_type, value_type>::value, "missing operator");
	static_assert(jc::has_operator<decltype(operator_v), value_type>::value, "missing self-applied operator");


	constexpr value_type operand_a_v = 12;
	constexpr value_type operand_b_v = 2;
	constexpr value_type expected_v = operand_a_v / operand_b_v;



	const value_type a = operand_a_v;
	const value_type b = operand_b_v;

	{
		const value_type q = operator_v(a, b);
		ASSERT(q == expected_v, "divide operator failed");
	};

	{
		const value_type q = b | (a & operator_v);
		ASSERT(q == expected_v, "bound and piped divide operator failed");
	};

	PASS();
};



// jc::conjunct test
int test_op_conjuct()
{
	NEWTEST();

	constexpr auto operator_v = jc::conjunct;
	using value_type = bool;

	static_assert(jc::has_operator<decltype(operator_v), value_type, value_type>::value, "missing operator");
	static_assert(jc::has_operator<decltype(operator_v), value_type>::value, "missing self-applied operator");

	{
		constexpr value_type operand_a_v = false;
		constexpr value_type operand_b_v = false;
		constexpr value_type expected_v = operand_a_v && operand_b_v;

		const value_type a = operand_a_v;
		const value_type b = operand_b_v;

		{
			const value_type q = operator_v(a, b);
			ASSERT(q == expected_v, "conjunct operator failed");
		};

		{
			const value_type q = operator_v(b, a);
			ASSERT(q == expected_v, "conjunct operator failed");
		};

		{
			const value_type q = b | (a & operator_v);
			ASSERT(q == expected_v, "bound and piped conjunct operator failed");
		};

		{
			const value_type q = a | (b & operator_v);
			ASSERT(q == expected_v, "bound and piped conjunct operator failed");
		};
	};

	{
		constexpr value_type operand_a_v = false;
		constexpr value_type operand_b_v = true;
		constexpr value_type expected_v = operand_a_v && operand_b_v;

		const value_type a = operand_a_v;
		const value_type b = operand_b_v;

		{
			const value_type q = operator_v(a, b);
			ASSERT(q == expected_v, "conjunct operator failed");
		};

		{
			const value_type q = operator_v(b, a);
			ASSERT(q == expected_v, "conjunct operator failed");
		};

		{
			const value_type q = b | (a & operator_v);
			ASSERT(q == expected_v, "bound and piped conjunct operator failed");
		};

		{
			const value_type q = a | (b & operator_v);
			ASSERT(q == expected_v, "bound and piped conjunct operator failed");
		};
	};

	{
		constexpr value_type operand_a_v = true;
		constexpr value_type operand_b_v = true;
		constexpr value_type expected_v = operand_a_v && operand_b_v;

		const value_type a = operand_a_v;
		const value_type b = operand_b_v;

		{
			const value_type q = operator_v(a, b);
			ASSERT(q == expected_v, "conjunct operator failed");
		};

		{
			const value_type q = operator_v(b, a);
			ASSERT(q == expected_v, "conjunct operator failed");
		};

		{
			const value_type q = b | (a & operator_v);
			ASSERT(q == expected_v, "bound and piped conjunct operator failed");
		};

		{
			const value_type q = a | (b & operator_v);
			ASSERT(q == expected_v, "bound and piped conjunct operator failed");
		};
	};

	PASS();
};

// jc::disjunct test
int test_op_disjunct()
{
	NEWTEST();

	constexpr auto operator_v = jc::disjunct;
	using value_type = bool;

	static_assert(jc::has_operator<decltype(operator_v), value_type, value_type>::value, "missing operator");
	static_assert(jc::has_operator<decltype(operator_v), value_type>::value, "missing self-applied operator");

	{
		constexpr value_type operand_a_v = false;
		constexpr value_type operand_b_v = false;
		constexpr value_type expected_v = operand_a_v || operand_b_v;

		const value_type a = operand_a_v;
		const value_type b = operand_b_v;

		{
			const value_type q = operator_v(a, b);
			ASSERT(q == expected_v, "disjunct operator failed");
		};

		{
			const value_type q = operator_v(b, a);
			ASSERT(q == expected_v, "disjunct operator failed");
		};

		{
			const value_type q = b | (a & operator_v);
			ASSERT(q == expected_v, "bound and piped disjunct operator failed");
		};
		
		{
			const value_type q = a | (b & operator_v);
			ASSERT(q == expected_v, "bound and piped disjunct operator failed");
		};
	};

	{
		constexpr value_type operand_a_v = false;
		constexpr value_type operand_b_v = true;
		constexpr value_type expected_v = operand_a_v || operand_b_v;

		const value_type a = operand_a_v;
		const value_type b = operand_b_v;

		{
			const value_type q = operator_v(a, b);
			ASSERT(q == expected_v, "disjunct operator failed");
		};

		{
			const value_type q = operator_v(b, a);
			ASSERT(q == expected_v, "disjunct operator failed");
		};

		{
			const value_type q = b | (a & operator_v);
			ASSERT(q == expected_v, "bound and piped disjunct operator failed");
		};

		{
			const value_type q = a | (b & operator_v);
			ASSERT(q == expected_v, "bound and piped disjunct operator failed");
		};
	};

	{
		constexpr value_type operand_a_v = true;
		constexpr value_type operand_b_v = true;
		constexpr value_type expected_v = operand_a_v || operand_b_v;

		const value_type a = operand_a_v;
		const value_type b = operand_b_v;

		{
			const value_type q = operator_v(a, b);
			ASSERT(q == expected_v, "disjunct operator failed");
		};

		{
			const value_type q = operator_v(b, a);
			ASSERT(q == expected_v, "disjunct operator failed");
		};

		{
			const value_type q = b | (a & operator_v);
			ASSERT(q == expected_v, "bound and piped disjunct operator failed");
		};

		{
			const value_type q = a | (b & operator_v);
			ASSERT(q == expected_v, "bound and piped disjunct operator failed");
		};
	};

	PASS();
};



// jc::dereference test
int test_op_dereference()
{
	NEWTEST();

	constexpr auto operator_v = jc::dereference;

	// Test with non-const value pointer
	{
		using value_type = int;

		static_assert(jc::has_operator<decltype(operator_v), value_type*>::value, "missing operator");

		const value_type initial_v = 0;
		const value_type new_v = 2;

		value_type i = initial_v;
		value_type* iptr = &i;

		ASSERT(i == initial_v, "invalid dereference test condition");

		jc::dereference(iptr) = new_v;
		ASSERT(i == new_v, "dereference operator failed");

		(iptr | jc::dereference) = initial_v;
		ASSERT(i == initial_v, "piped dereference operator failed");
	};

	// Test with const value pointer
	{
		using value_type = const int;

		static_assert(jc::has_operator<decltype(operator_v), const value_type*>::value, "missing operator");

		const value_type initial_v = 0;
		const value_type new_v = 2;

		value_type i = initial_v;
		value_type* iptr = &i;

		ASSERT(i == initial_v, "invalid dereference test condition");

		auto& iref = jc::dereference(iptr);
		ASSERT(&i == &iref, "dereference operator failed on const value");
	};

	PASS();
};

// jc::invert test
int test_op_invert()
{
	NEWTEST();
	constexpr auto operator_v = jc::invert;

	// Test operator
	{
		using value_type = bool;

		static_assert(jc::has_operator<decltype(operator_v), value_type>::value, "missing operator");

		const value_type initial_v = false;
		const value_type new_v = true;

		value_type i = initial_v;

		ASSERT(i == initial_v, "invalid invert test condition");

		i = operator_v(i);
		ASSERT(i == new_v, "invert  operator failed");

		i = (i | operator_v);
		ASSERT(i == initial_v, "piped invert operator failed");
	};

	PASS();
};




// Runs the tests for the various operators defined by jclib/functional.h
int test_operators()
{
	NEWTEST();
	
	// Test arithmatic operators

	SUBTEST(test_op_plus);
	SUBTEST(test_op_minus);
	SUBTEST(test_op_times);
	SUBTEST(test_op_divide);

	SUBTEST(test_op_invert);


	// Test binary arithmetic operators





	// Test logical operatorss
	
	SUBTEST(test_op_conjuct);
	SUBTEST(test_op_disjunct);



	// Test other operators

	SUBTEST(test_op_dereference);


	PASS();
};




namespace jc
{

	template <typename T>
	struct packed_arg
	{
	public:

		constexpr T&& unpack() noexcept
		{
			return std::move(this->val);
		};
		constexpr const T& unpack() const noexcept
		{
			return this->val;
		};

		constexpr operator T&&() noexcept { return std::move(this->unpack()); };
		constexpr operator const T&() const noexcept { return this->unpack(); };



		constexpr packed_arg(T _value) :
			val{ std::move(_value) }
		{};

	private:
		T val;
	};

	template <typename T>
	struct packed_arg<T&>
	{
	public:
		constexpr T& unpack() noexcept { return this->val; };
		constexpr const T& unpack() const noexcept { return this->val; };
		
		constexpr operator T& () noexcept { return this->unpack(); };
		constexpr operator const T& () const noexcept { return this->unpack(); };

		constexpr packed_arg(T& v) : val{ v } {};

	private:
		std::reference_wrapper<T> val;
	};



	template <typename... Ts>
	struct argpack
	{
		using tuple = std::tuple<packed_arg<Ts>...>;

		constexpr argpack(Ts&&... _vals) :
			args{ std::forward<Ts>(_vals)... }
		{};

		tuple args{};
	};






	struct pack_t
	{
		template <typename... Ts>
		constexpr auto operator()(Ts&&... _vals) const noexcept
		{
			return argpack<Ts...>{ std::forward<Ts>(_vals)... };
		};
	};
	constexpr static pack_t pack{};





	template <typename OpT, typename... Ts, size_t... Idxs>
	constexpr inline auto apply(OpT&& _op, argpack<Ts...> _args, std::index_sequence<Idxs...>) ->
		decltype(jc::invoke(std::declval<OpT&&>(), std::declval<Ts>()...))
	{
		return jc::invoke(_op, std::get<Idxs>(_args.args)...);
	};

	template <typename OpT, typename... Ts>
	constexpr inline auto apply(OpT&& _op, argpack<Ts...> _args) ->
		decltype(jc::invoke(std::declval<OpT&&>(), std::declval<Ts>()...))
	{
		return apply(_op, std::move(_args), std::make_index_sequence<sizeof...(Ts)>{});
	};


	/**
	 * @brief Implements arguement pack piping allowing a function object to be invoked with multiple arguements
	 * @tparam ...Ts Packed arguement types
	 * @tparam OpT Function object type
	 * @tparam Enable SFINAE point for ensuring this is a valid pipe expression
	 * @param _args Arguement pack object
	 * @param _op Function object to invoke
	 * @return Result of applying the arguement pack to the function object
	*/
	template <typename... Ts, typename OpT,
		typename Enable = jc::enable_if_t<jc::is_operator<OpT>::value&& jc::is_invocable_with_count<OpT, sizeof...(Ts)>::value>
	>
	constexpr inline auto operator|(jc::argpack<Ts...> _args, const OpT& _op)
	{
		return jc::apply(_op, std::move(_args));
	};

};


namespace jc
{
	/**
	 * @brief Provides a type based wrapper for get_ftor specializations that use a non-type template parameter as the index
	 * @tparam T Index value type
	 * @tparam Value Index value
	*/
	template <typename T, T Value>
	struct get_value_idx : public std::integral_constant<T, Value> {};

	/**
	 * @brief Customization point for "overloading" std::get
	 * @tparam T Type to get from
	 * @tparam IdxT Index type (ie. std::get<IdxT>(T&))
	 * @tparam Enable SFINAE specialization point
	*/
	template <typename T, typename IdxT, typename Enable = void>
	struct get_ftor;

	// std::get where index is a template type parameter
	template <typename T, typename IdxT>
	struct get_ftor<T, IdxT, void_t<decltype( std::get<IdxT>(std::declval<T&>()) )>>
	{
		constexpr auto operator()(T& _from) const -> decltype(std::get<IdxT>(std::declval<T&>()))
		{
			return std::get<IdxT>(_from);
		};
		constexpr auto operator()(const T& _from) const -> decltype(std::get<IdxT>(std::declval<const T&>()))
		{
			return std::get<IdxT>(_from);
		};
	};
	
	// std::get where index is a non-type template parameter
	template <typename T, typename IdxT, IdxT Value>
	struct get_ftor<T, get_value_idx<IdxT, Value>, void_t<decltype(std::get<Value>(std::declval<T&>()))>>
	{
		constexpr auto operator()(T& _from) const -> decltype(std::get<Value>(std::declval<T&>()))
		{
			return std::get<Value>(_from);
		};
		constexpr auto operator()(const T& _from) const -> decltype(std::get<Value>(std::declval<const T&>()))
		{
			return std::get<Value>(_from);
		};
	};




	template <typename IdxT, typename FromT>
	constexpr inline auto get(FromT& _from) ->
		decltype(std::declval<get_ftor<std::remove_const_t<FromT>, IdxT>>()(_from))
	{
		return get_ftor<std::remove_const_t<FromT>, IdxT>{}(_from);
	};

	template <size_t N, typename FromT>
	constexpr inline auto get(FromT& _from) ->
		decltype(std::declval<get_ftor<std::remove_const_t<FromT>, get_value_idx<size_t, N>>>()(_from))
	{
		return get_ftor<std::remove_const_t<FromT>, get_value_idx<size_t, N>>{}(_from);
	};

};


namespace jc
{

	template <size_t N, typename... Ts>
	struct get_ftor<argpack<Ts...>, jc::get_value_idx<size_t, N>, void>
	{
		constexpr auto operator()(argpack<Ts...>& _args) const ->
			decltype(jc::get<N>(std::declval<argpack<Ts...>&>().args).unpack())
		{
			return jc::get<N>(_args.args).unpack();
		};

		constexpr auto operator()(const argpack<Ts...>& _args) const ->
			decltype(jc::get<N>(std::declval<const argpack<Ts...>&>().args).unpack())
		{
			return jc::get<N>(_args.args).unpack();
		};
	};


};




namespace jc
{

	namespace impl
	{

		template <typename T>
		struct repack_impl;

		template <template <typename... Ts> typename T, typename... Ts>
		struct repack_impl<T<Ts...>>
		{
			template <size_t... Idxs>
			constexpr auto operator()(T<Ts...>& _pack, std::index_sequence<Idxs...>) const
			{
				return jc::pack(std::forward<Ts>(jc::get<Idxs>(_pack))...);
			};
			constexpr auto operator()(T<Ts...>& _pack) const
			{
				return (*this)(_pack, std::make_index_sequence<sizeof...(Ts)>{});
			};
		};

		template <template <typename... Ts> typename T, typename... Ts>
		struct repack_impl<const T<Ts...>>
		{
			template <size_t... Idxs>
			constexpr auto operator()(T<Ts...> _pack, std::index_sequence<Idxs...>) const
			{
				return jc::pack(std::forward<Ts>(jc::get<Idxs>(_pack))...);
			};
			constexpr auto operator()(const T<Ts...>& _pack) const
			{
				return (*this)(T<Ts...>(_pack), std::make_index_sequence<sizeof...(Ts)>{});
			};
		};

		template <>
		struct repack_impl<wildcard>
		{
			constexpr auto& operator()(wildcard& w) const noexcept { return w; };
		};

	};

	struct repack_t : public jc::operator_tag
	{
		template <typename T>
		constexpr auto operator()(T&& _val) const
		{
			return impl::repack_impl<jc::remove_reference_t<T>>{}(_val);
		};
	};
	constexpr repack_t repack{};
};



// Tests the arguement pack piping behavior
int test_arguement_pack_piping()
{
	NEWTEST();
	
	// Composed function used for the static assertions
	constexpr auto test_fn = jc::plus | jc::equals & 4;
	
	// Extra testing to ensure function composition maintains the required traits
	static_assert(jc::is_operator<decltype(test_fn)>::value, "composed test function failed is_opertor trait check");
	static_assert(jc::is_invocable_with_count<decltype(test_fn), 2>::value, "composed test function failed is_invocable_with_count check");

	// Test piping of immediate pack
	static_assert(jc::pack(2, 2) | test_fn,
		"failed expansion of piped arguement pack");

	// Ensure that re-repacking doesn't break the piping
	static_assert(jc::pack(2, 2) | jc::repack | test_fn,
		"failed repacking and piping of expanded arguement pack");

	// Test piping of repacked immediate pack
	static_assert(std::pair<int, int>{2, 2} | jc::repack | test_fn,
		"failed repacking and piping of expanded arguement pack");

	// Ensure that re-repacking doesn't break the piping
	static_assert(std::pair<int, int>{ 2, 2} | jc::repack | jc::repack | test_fn,
		"failed repacking and piping of expanded arguement pack");


	
	// Testing for piping of existing pack

	constexpr std::pair<int, int> args_v{ 2, 2 };
	constexpr auto packed_args_v = jc::pack(2, 2);


	// Test piping of existing pack
	static_assert(packed_args_v | test_fn,
		"failed expansion of piped arguement pack");

	// Ensure that re-repacking doesn't break the piping
	static_assert(packed_args_v | jc::repack | test_fn,
		"failed repacking and piping of expanded arguement pack");

	// Test piping of repacked non-pack value
	static_assert(args_v | jc::repack | test_fn,
		"failed repacking and piping of expanded arguement pack");

	// Ensure that re-repacking doesn't break the piping
	static_assert(args_v | jc::repack | jc::repack | test_fn,
		"failed repacking and piping of expanded arguement pack");


	PASS();
};


// Main subtest for all jclib functional piping
int test_piping()
{
	NEWTEST();




	// Test arguement pack piping
	SUBTEST(test_arguement_pack_piping);


	PASS();
};




struct Foo
{
	constexpr auto operator+(int a) const noexcept
	{
		return Foo{ a + this->n };
	};
	constexpr auto operator-(Foo b) const noexcept
	{
		return Foo{ this->n - b.n };
	};
	int n = 0;
};

constexpr auto add(int a, int b) { return a + b; };

int main()
{
	SUBTEST(test_operators);
	SUBTEST(test_piping);

	if (!jc::has_operator<jc::plus_t, Foo, int>::value)
	{
		NEWTEST();
		FAIL("Failed to detect plus operator");
	};
	if (!jc::has_operator<jc::minus_t, Foo>::value)
	{
		NEWTEST();
		FAIL("Failed to detect minus operator");
	};
	if (jc::has_operator<jc::minus_t, Foo, int>::value)
	{
		NEWTEST();
		FAIL("Detected minus(with other type = int) operator when there wasn't one defined");
	};

	{
		std::tuple<int, int> _args{ 2, 3 };
		if (jc::apply(add, _args) != 5)
		{
			NEWTEST();
			FAIL("Apply did not return the correct value");
		};
	};

	PASS();
};