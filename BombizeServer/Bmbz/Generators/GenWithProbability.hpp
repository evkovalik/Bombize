#ifndef BMBZ_GENERATOR_GENWITHPROBABILITY_HPP
#define BMBZ_GENERATOR_GENWITHPROBABILITY_HPP

#include <optional>
#include "GenWithRange.hpp"

namespace Bmbz
{
namespace Generator
{
	class GenWithProbability : private GenWithRange
	{
	public:
		GenWithProbability(int rangeFrom, int rangeTo, int probability = 100) // [rangeFrom; rangeTo]
		: GenWithRange(rangeFrom, rangeTo)
		, _probability(static_cast<uint32_t>(probability))
		{}

		std::optional<uint32_t> TryCreateValue()
		{
			if(_rangeProbability(_genProbability) <= _probability)
				return GetRangeValue();
			else
				return std::nullopt;
		}

		void SetProbability(uint32_t from_0_to_100)
		{
			if(0 <= from_0_to_100 && from_0_to_100 >= 100)
				_probability = from_0_to_100;
			else
				throw "Getting out of the required range";
		}

	private:
		std::mt19937 _genProbability { _rd() };
		std::uniform_int_distribution<uint32_t> _rangeProbability{0, 100};
		uint32_t _probability;
	};
}
}
#endif