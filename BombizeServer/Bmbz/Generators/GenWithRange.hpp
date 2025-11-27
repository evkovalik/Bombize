#ifndef BMBZ_GENERATOR_GENWITHRANGE_HPP
#define BMBZ_GENERATOR_GENWITHRANGE_HPP

#include <random>
#include <vector>

namespace Bmbz
{
namespace Generator
{
	class GenWithRange
	{
	public:
		GenWithRange(uint32_t from, uint32_t to) // [from; to]
		: _customRange(from, to) {}
		virtual ~GenWithRange() = default;

		uint32_t GetValue() { return _gen(); }
		uint32_t GetRangeValue() { return _customRange(_gen); }

	protected:
		std::random_device _rd;
	private:
		std::mt19937 _gen{ _rd() };
		std::uniform_int_distribution<uint32_t> _customRange;
	};
}
}
#endif