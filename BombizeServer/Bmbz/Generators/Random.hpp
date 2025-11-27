#ifndef BMBZ_GENERATOR_RANDOM_HPP
#define BMBZ_GENERATOR_RANDOM_HPP

#include <random>

namespace Bmbz
{
namespace Generator
{
	class Random
	{
	public:
		static unsigned int Value()
		{
			return Instance().gen();
		}
			
		static int RangeInt(int min, int max)
		{
			std::uniform_int_distribution<int> dist(min, max);
			return dist(Instance().gen);
		}

		static float RangeFloat(float min, float max)
		{
			std::uniform_real_distribution<float> dist(min, max);
			return dist(Instance().gen);
		}

		template<typename T>
		static T Range(T min, T max)
		{
			static_assert(
				std::is_integral_v<T> || std::is_floating_point_v<T>, 
				"T must be integral or floating point"
			);

			if constexpr (std::is_integral_v<T>)
			{
				std::uniform_int_distribution<T> dist(min, max);
				return dist(Instance().gen);
			}
			else if constexpr (std::is_floating_point_v<T>)
			{
				std::uniform_real_distribution<T> dist(min, max);
				return dist(Instance().gen);
			}
		}
	private:
		std::mt19937 gen;
		Random() : gen(std::random_device{}()) {}

		static Random& Instance()
		{
			static Random instance;
			return instance;
		}
	};
}
}
#endif