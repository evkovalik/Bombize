#ifndef BMBZ_GAME_COMPONENT_EFFECT_EMPTYEFFECT_HPP
#define BMBZ_GAME_COMPONENT_EFFECT_EMPTYEFFECT_HPP

#include "IEffect.hpp"

namespace Bmbz
{
namespace Game
{
namespace Component
{
namespace Effect
{
    class EmptyEffect : public IEffect
    {
    public:
        void Execute(NetworkData::PlayerInfo& playerInfo) override {}
        bool IsActive() const override { return false; }
        void Enable() override {}
        void Disable() override {}
    };
}
}
}
}
#endif