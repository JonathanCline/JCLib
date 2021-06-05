#pragma once
#ifndef JCLIB_EITHER_H
#define JCLIB_EITHER_H

/*
	Copyright 2021 Jonathan Cline
	Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files
	(the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge,
	publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do
	so, subject to the following conditions:
	The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
	WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
	COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
	OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
	Unfinished but was included previous versions so the file is provided completely empty
*/

// chad mode #if false
#if false

#include "jclib/config.h"

#define _JCLIB_EITHER_

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

#endif