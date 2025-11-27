#ifndef BMBZ_GENERATOR_GENSTRINGS_HPP
#define BMBZ_GENERATOR_GENSTRINGS_HPP

#include "GenWithRange.hpp"

namespace Bmbz
{
namespace Generator
{
	class GenStrings : GenWithRange
	{
	public:
		GenStrings() : GenWithRange(32, 126) {}

		std::string GetString(int length)
		{
			std::string str;
			str.reserve(length);
			for(int i = 0; i < length; ++i)
			{
				str += static_cast<char>(GetRangeValue());
			}
			return std::move(str);
		}
	};
}
}
#endif