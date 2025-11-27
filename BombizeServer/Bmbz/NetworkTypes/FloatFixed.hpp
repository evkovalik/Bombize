#ifndef BMBZ_NETTYPE_FLOATFIXED16BIT_HPP
#define BMBZ_NETTYPE_FLOATFIXED16BIT_HPP

#include "INetworkData.hpp"
#include "Utils/DataConversion.hpp"

namespace Bmbz
{
    namespace NetType
    {
        class FloatFixed16Bit : public INetworkData
        {
        public:
            static const uint16_t limitRange = 65535;

            FloatFixed16Bit(float minValue, float maxValue, float precision)
            {
                if( static_cast<size_t>((maxValue - minValue) / precision + 1) <= limitRange)
                {
                    _minValue = minValue;
                    _maxValue = maxValue;
                    _precision = precision;
                    _value = minValue;
                }
                else
                    throw "FloatFixed16Bit: The specified range cannot be placed in 16 bits.";
            }

            FloatFixed16Bit(float minValue, float maxValue, float precision, float value)
            : FloatFixed16Bit(minValue, maxValue, precision)
            {
                _value = Utils::Clamp(value, minValue, maxValue);
            }

            void Value(float setValue)
            {
                _value = Utils::Clamp(setValue, _minValue, _maxValue);
            }

            float Value() const
            {
                return _value;
            }

            FloatFixed16Bit& operator=(float rhs)
            {
                Value(rhs);
                return *this;
            }

            FloatFixed16Bit& operator+=(float rhs)
            {
                Value(_value + rhs);
                return *this;
            }

            FloatFixed16Bit& operator-=(float rhs)
            {
                Value(_value - rhs);
                return *this;
            }

            void Serialize(boost::beast::flat_buffer& buffer) const override
            {
                auto rawValue = Utils::ConvertToFixed<uint16_t>(_value, _minValue, _precision);
                Utils::WriteToBuffer(buffer, rawValue);
            }

            void Deserialize(boost::beast::flat_buffer& buffer) override
            {
                uint16_t rawValue;
                Utils::ReadFromBuffer(buffer, rawValue);
                _value = Utils::ConvertFromFixed<uint16_t>(rawValue, _minValue, _precision);
            }

        private:
            float _minValue, _maxValue, _precision, _value;  
        };

        using FloatFixed = FloatFixed16Bit;
    }
}
#endif