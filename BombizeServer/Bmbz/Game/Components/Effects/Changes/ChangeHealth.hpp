#ifndef BMBZ_GAME_COMPONENT_EFFECT_CHANGEHEALTH_HPP
#define BMBZ_GAME_COMPONENT_EFFECT_CHANGEHEALTH_HPP

#include "ChangeParameter.hpp"

namespace Bmbz
{
namespace Game
{
namespace Component
{
namespace Effect
{
    class ChangeHealth final : public ChangeParameter
    {
    public:
        ChangeHealth(int count) : ChangeParameter(count) {}
    protected:
        void ChangeMethod(NetworkData::PlayerInfo& playerInfo) override
        {
            auto oldValue = playerInfo.Health();
            int setValue = static_cast<int>(playerInfo.Health()) + changesCount;
            playerInfo.Health(
                static_cast<uint8_t>(Utils::Clamp<int>(setValue, LOWER_LIMIT, UPPER_LIMIT))
            );
            _diff = static_cast<int>(oldValue - playerInfo.Health());
        }
        void Cancel(NetworkData::PlayerInfo& playerInfo) override
        {
            auto setValue = static_cast<uint8_t>(Utils::Clamp<int>(
                static_cast<int>(playerInfo.Health()) + _diff,
                LOWER_LIMIT,
                UPPER_LIMIT)
            );
            playerInfo.Health(setValue);
            _diff = 0;
        }
    private:
        int _diff = 0;
        inline static constexpr int UPPER_LIMIT = 3;
        inline static constexpr int LOWER_LIMIT = 0;
    };
}
}
}
}
#endif