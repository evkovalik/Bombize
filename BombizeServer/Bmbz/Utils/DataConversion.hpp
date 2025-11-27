#ifndef BMBZ_UTILS_DATACONVERSION_HPP
#define BMBZ_UTILS_DATACONVERSION_HPP

namespace Bmbz
{
    namespace Utils
    {
        /* (maxValue - minValue)/precision + 1 = (20 - (-20)) * 0.01 + 1 = 4001 => 16 bit*/

        template<typename T>
        inline T ConvertToFixed(float value, float minValue, float precision)
        {
            return static_cast<T>((value - minValue) / precision);
        }

        template<typename T>
        inline float ConvertFromFixed(T value, float minValue, float precision)
        {
            return static_cast<float>(value) * precision + minValue;
        }

        template<typename T>
        inline T Clamp(T value, T min, T max)
        {
            return value < max ? (value > min ? value : min) : max;
        }

        template<typename T>
        inline T RoundTo(float value)
        {
            return static_cast<T>(value + 0.5f);
        }
    }
}

#endif