#pragma once
#ifndef JCLIB_EITHER_H
#define JCLIB_EITHER_H

#define _JCLIB_EITHER_

#include "jclib/config.h"
/*
namespace jc
{
	
	template <typename FirstT, typename SecondT>
	class either
	{
	private:
		enum : bool
		{
			isFirstType = false,
			isSecondType = true
		};

	public:
		using first_type = FirstT;
		using second_type = SecondT;

		struct second_value_t {};
		//JCLIB_CONSTANT second_value_t second_value{};

		JCLIB_CONSTEXPR bool has_first() const noexcept
		{
			return this->type_ == isFirstType;
		};

		first_type& first()
		{
			
		};




		JCLIB_CONSTEXPR either(first_type _fval);
		JCLIB_CONSTEXPR either(second_type _sval);


		~either()
		{
			if (this->type_ == isFirstType)
			{
				this->ft_.~first_type();
			}
			else
			{
				this->st_.~second_type();
			};
		};

	private:
		
		bool type_;
		union
		{
			first_type ft_;
			second_type st_;
		};


	};

};
*/
#endif