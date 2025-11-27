#ifndef BMBZ_GAME_EXPLOSIONTARGET_HPP
#define BMBZ_GAME_EXPLOSIONTARGET_HPP

#include "NetworkTypes/Vector2.hpp"
#include "Game/GameObjects/Map.hpp"
#include "Resources/Characters/CharacterInfo.hpp"

namespace Bmbz
{
namespace Game
{
    struct ExplosionTarget
    {
        const Resource::CharacterInfo* characterInfo;
        Map* map;
        NetType::Vector2<uint8_t> position;
        uint8_t lengthFire;
        uint8_t ownerNumber;
    };
}
}
#endif