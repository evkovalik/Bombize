#ifndef BMBZ_GAME_COMPONENT_EFFECT_CHANGEBOMB_HPP
#define BMBZ_GAME_COMPONENT_EFFECT_CHANGEBOMB_HPP

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
    class ChangeBomb final : public ChangeParameter
    {
    public:
        ChangeBomb(int count) : ChangeParameter(count) {}
    protected:
        void ChangeMethod(NetworkData::PlayerInfo& playerInfo) override
        {
            auto oldValue = playerInfo.Bombs();
            int setValue = static_cast<int>(playerInfo.Bombs()) + changesCount;
            playerInfo.Bombs(
                static_cast<uint8_t>(Utils::Clamp<int>(setValue, LOWER_LIMIT, UPPER_LIMIT))
            );
            _diff = static_cast<int>(oldValue - playerInfo.Bombs());
        }
        void Cancel(NetworkData::PlayerInfo& playerInfo) override
        {
            auto setValue = static_cast<uint8_t>(Utils::Clamp<int>(
                static_cast<int>(playerInfo.Bombs()) + _diff,
                LOWER_LIMIT,
                UPPER_LIMIT)
            );
            playerInfo.Bombs(setValue);
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