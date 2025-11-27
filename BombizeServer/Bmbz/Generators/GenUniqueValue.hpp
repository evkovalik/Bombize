#ifndef BMBZ_GENERATOR_GENUNIQUEVALUE_HPP
#define BMBZ_GENERATOR_GENUNIQUEVALUE_HPP

#include <random>
#include <vector>

namespace Bmbz
{
namespace Generator
{
	class GenUniqueValue
	{
	public:
		GenUniqueValue() = default;

		uint32_t GetValueUInt32()
		{
			uint32_t value;
			do
			{
				value = _gen();			
			} while (CheckValueInCreated(value));
			
			_createdValues.push_back(value);
			return value;
		}

	private:
		std::random_device _rd;
		std::mt19937 _gen{ _rd() };
		std::vector<uint32_t> _createdValues;

		bool CheckValueInCreated(uint32_t value)
		{
			for (const auto& createdValue : _createdValues)
			{
				if (value == createdValue) return true;
			}
			return false;
		}
	};
}
}
#endif