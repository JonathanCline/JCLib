#include <jclib/ranges.h>

#include <jclib-test.hpp>


template <typename ContainerT, typename ExpectedValueT = typename ContainerT::value_type>
int subtest_range_element_type()
{
	using value_type = ExpectedValueT;
	using pointer = ExpectedValueT*;
	using reference = ExpectedValueT&;
	using const_pointer = const jc::remove_const_t<ExpectedValueT>*;
	using const_reference = const jc::remove_const_t<ExpectedValueT>&;

	static_assert(jc::is_same<jc::ranges::value_t<ContainerT>, value_type>::value, "value_type mismatch");
	static_assert(jc::is_same<jc::ranges::pointer_t<ContainerT>, pointer>::value, "pointer mismatch");
	static_assert(jc::is_same<jc::ranges::reference_t<ContainerT>, reference>::value, "reference mismatch");
	static_assert(jc::is_same<jc::ranges::const_pointer_t<ContainerT>, const_pointer>::value, "const_pointer mismatch");
	static_assert(jc::is_same<jc::ranges::const_reference_t<ContainerT>, const_reference>::value, "const_reference mismatch");

	PASS();
};

template <typename ContainerT, typename IteratorT = typename ContainerT::iterator>
int subtest_range_iterator_type()
{
	using iterator = IteratorT;
	static_assert(jc::is_same<jc::ranges::iterator_t<ContainerT>, iterator>::value, "iterator mismatch");
	PASS();
};

template <typename ContainerT, typename ConstIteratorT = typename ContainerT::const_iterator>
int subtest_range_const_iterator_type()
{
	using iterator = ConstIteratorT;
	static_assert(jc::is_same<jc::ranges::const_iterator_t<ContainerT>, iterator>::value, "const_iterator mismatch");
	PASS();
};

template <typename ContainerT, typename SizeT = typename ContainerT::size_type>
int subtest_range_size_type()
{
	static_assert(jc::is_same<jc::ranges::size_type_t<ContainerT>, SizeT>::value, "size_type mismatch");
	PASS();
};


// Full test for a container
template <typename ContainerT,
	typename ExpectedValueT = typename ContainerT::value_type,
	typename SizeT = typename ContainerT::size_type,
	typename IteratorT = typename ContainerT::iterator,
	typename ConstIteratorT = typename ContainerT::const_iterator
>
void subtest_range_type_traits()
{
	(void)subtest_range_element_type<ContainerT, ExpectedValueT>();
	(void)subtest_range_size_type<ContainerT, SizeT>();
	(void)subtest_range_iterator_type<ContainerT, IteratorT>();
	(void)subtest_range_const_iterator_type<ContainerT, ConstIteratorT>();
};



#include <map>
#include <list>
#include <array>
#include <vector>


int main()
{
	NEWTEST();

	// Run tests with C-arrays

	{
		using container = int[10];
		subtest_range_type_traits<container, int, size_t, int*, const int*>();
	};

	// Run tests with standard library containers

	subtest_range_type_traits<std::vector<int>>();
	subtest_range_type_traits<std::vector<std::vector<int>>>();

	subtest_range_type_traits<std::list<int>>();
	subtest_range_type_traits<std::list<std::list<int>>>();

	subtest_range_type_traits<std::array<int, 8>>();
	subtest_range_type_traits<std::array<std::array<int, 8>, 8>>();

	subtest_range_type_traits<std::map<int, int>>();
	subtest_range_type_traits<std::map<int, std::map<int, int>>>();

	PASS();
};
