#include <jclib/optional.h>

int main()
{
	{
		jc::optional<int> _opt{};
		if (_opt.has_value())
		{
			// should be nullopt by default
			return 1;
		};
	};
	{
		using trivial_type = jc::optional<int>;
		if (!std::is_trivially_destructible<trivial_type>::value)
		{
			// should be trivially destructible
			return 2;
		};

		struct Foo
		{
			~Foo()
			{
				this->i = 1;
			};
			int i = 0;
		};

		using non_trivial_type = jc::optional<Foo>;
		if (std::is_trivially_destructible<non_trivial_type>::value)
		{
			// should not be trivially destructible
			return 2;
		};
	}



	return 0;
};