#ifndef BMBZ_NETWORKDATA_PLAYERSPEED_HPP
#define BMBZ_NETWORKDATA_PLAYERSPEED_HPP

#include "NetworkTypes/FloatFixed.hpp"

namespace Bmbz
{
namespace Game
{
namespace NetworkData
{
    struct PlayerSpeed
    {
        uint8_t asCount{0};
        NetType::FloatFixed16Bit asFloatFixed{0.0f, 10.0f, 0.01f, 0.0f};

        PlayerSpeed() = default;

        PlayerSpeed(uint8_t asCount)
        : asCount(asCount)
        {
            asFloatFixed.Value(DefineSpeedFor(asCount));
        }

        static float DefineSpeedFor(uint8_t speedAsByteValue)
        {
            if(speedAsByteValue == 0) return 0.0f;
            else if(speedAsByteValue == 1) return SPEED_MIN;
            else return SPEED_MIN + static_cast<float>(speedAsByteValue - 1) * SPEED_FOR_UNIT;
        }

        static float DefineIncrementFor(uint8_t speedAsByteValue)
        {
            if(speedAsByteValue == 0) return 0.0f;
            else return static_cast<float>(speedAsByteValue) * SPEED_FOR_UNIT;
        }

        void Serialize(boost::beast::flat_buffer& buffer) const 
        {
            Utils::WriteToBuffer(buffer, asCount);
            asFloatFixed.Serialize(buffer);
        }
    private:
        static constexpr float SPEED_MIN = 2.4f;
        static constexpr float SPEED_FOR_UNIT = 0.05f;
    };
}
}
}
#endif