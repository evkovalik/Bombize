#ifndef BMBZ_GAME_COMPONENT_EFFECT_CHANGEFIRE_HPP
#define BMBZ_GAME_COMPONENT_EFFECT_CHANGEFIRE_HPP

#include "ChangeParameter.hpp"

namespace Bmbz
{
namespace Game
{
namespace Component
{
namespace Effect
{
    class ChangeFire final : public ChangeParameter
    {
    public:
        ChangeFire(int count) : ChangeParameter(count) {}
    protected:
        void ChangeMethod(NetworkData::PlayerInfo& playerInfo) override
        {
            auto oldValue = playerInfo.Fire();
            int setValue = static_cast<int>(playerInfo.Fire()) + changesCount;
            playerInfo.Fire(
                static_cast<uint8_t>(Utils::Clamp<int>(setValue, LOWER_LIMIT, UPPER_LIMIT))
            );
            _diff = static_cast<int>(oldValue - playerInfo.Fire());
        }
        void Cancel(NetworkData::PlayerInfo& playerInfo) override
        {
            auto setValue = static_cast<uint8_t>(Utils::Clamp<int>(
                static_cast<int>(playerInfo.Fire()) + _diff,
                LOWER_LIMIT,
                UPPER_LIMIT)
            );
            playerInfo.Fire(setValue);
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