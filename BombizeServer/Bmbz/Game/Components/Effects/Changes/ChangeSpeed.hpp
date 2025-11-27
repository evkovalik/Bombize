#ifndef BMBZ_GAME_COMPONENT_EFFECT_CHANGESPEED_HPP
#define BMBZ_GAME_COMPONENT_EFFECT_CHANGESPEED_HPP

#include "ChangeParameter.hpp"
#include "Utils/DataConversion.hpp"

namespace Bmbz
{
namespace Game
{
namespace Component
{
namespace Effect
{
    class ChangeSpeed final : public ChangeParameter
    {
    public:
        ChangeSpeed(int count) : ChangeParameter(count) {}
    protected:
        void ChangeMethod(NetworkData::PlayerInfo& playerInfo) override
        {
            auto oldValue = playerInfo.SpeedAsCount();
            int setValue = static_cast<int>(playerInfo.SpeedAsCount()) + changesCount;
            playerInfo.SpeedAsCount(
                static_cast<uint8_t>(Utils::Clamp<int>(setValue, LOWER_LIMIT, UPPER_LIMIT))
            );
            playerInfo.SpeedAsFloat(NetworkData::PlayerSpeed::DefineSpeedFor(playerInfo.SpeedAsCount()));
            _diff = static_cast<int>(oldValue - playerInfo.SpeedAsCount());
        }
        void Cancel(NetworkData::PlayerInfo& playerInfo) override
        {
            auto setValue = static_cast<uint8_t>(Utils::Clamp<int>(
                static_cast<int>(playerInfo.SpeedAsCount()) + _diff,
                LOWER_LIMIT,
                UPPER_LIMIT)
            );
            playerInfo.SpeedAsCount(setValue);
            playerInfo.SpeedAsFloat(NetworkData::PlayerSpeed::DefineSpeedFor(playerInfo.SpeedAsCount()));
            _diff = 0;
        }
    private:
        int _diff = 0;
        inline static constexpr int UPPER_LIMIT = 10;
        inline static constexpr int LOWER_LIMIT = 1;
    };
}
}
}
}
#endif