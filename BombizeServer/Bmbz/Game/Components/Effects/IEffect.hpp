#ifndef BMBZ_GAME_COMPONENT_IEFFECT_HPP
#define BMBZ_GAME_COMPONENT_IEFFECT_HPP

#include "Game/NetworkData/PlayerInfo.hpp"

namespace Bmbz
{
namespace Game
{
namespace Component
{
    struct IEffect
    {
        virtual ~IEffect() = default;
        virtual void Execute(NetworkData::PlayerInfo& playerInfo) = 0;
        virtual bool IsActive() const = 0;
        virtual void Enable() = 0;
        virtual void Disable() = 0;
    };
}
}
}
#endif