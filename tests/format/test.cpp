
#include <jclib/iterator.h>
#include <jclib/algorithm.h>

#include <string>

template <typename CharT>
using basic_format_string = std::basic_string<CharT>;
using format_string = basic_format_string<char>;

template <typename T, typename CharT>
struct formatter;


template <typename CharT>
struct basic_parse_context
{
	using iterator = typename basic_format_string<CharT>::const_iterator;

	iterator begin_;
	iterator end_;

	iterator begin() const noexcept
	{
		return this->begin_;
	};
	iterator end() const noexcept
	{
		return this->end_;
	};
};
template <typename CharT>
struct basic_format_context
{

};

template <>
struct formatter<int, char>
{
private:
	using character_type = char;
	enum class Align : character_type
	{
		None = '\0',
		Left = '<',
		Right = '>',
		Middle = '^'
	};

	constexpr static Align parse_alignment(character_type  _c) noexcept
	{
		Align _out = Align::None;
		switch ((Align)_c)
		{
		case Align::Left:
			_out = Align::Left;
			break;
		case Align::Right:
			_out = Align::Right;
			break;
		case Align::Middle:
			_out = Align::Middle;
			break;
		default:
			break;
		};
		return _out;
	};

public:
	template <typename ContextT>
	auto parse(ContextT& _ctx)
	{
		const auto _begin = _ctx.begin();
		const auto _end = _ctx.end();
		auto _at = _begin;
		int _stage = 0;

		{
			auto& _align = this->align_;
			_align = this->parse_alignment(*_at);
			if (++_at != _end)
			{
				auto _second = this->parse_alignment(*_at);
				if (_second != Align::None)
				{
					_align = _second;
					this->fillchar_ = *_begin;
					++_at;
				};
			};
			if (this->align_ != Align::None && _at != _end && std::isdigit(*_at))
			{
				auto _numberEnd = jc::find_if(jc::views::iter(_at, _end), std::isdigit);
				this->fill_ = std::stoi(std::string{ _at, _numberEnd });
			};
		};

		return _at;
	};

	auto make_str() const noexcept
	{

	};

	template <typename ContextT>
	auto format(const int& _i, ContextT& _ctx)
	{

	};

private:
	size_t fill_ = 0;
	int base_ = 10;
	character_type fillchar_ = 0;
	Align align_ = Align::None;
};




template <typename... Ts>
std::string format(format_string _fmt, Ts&&... _vals)
{

};














int main()
{

	return 0;
};