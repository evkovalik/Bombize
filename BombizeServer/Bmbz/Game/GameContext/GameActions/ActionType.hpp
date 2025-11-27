#ifndef BMBZ_GAMEACTION_ACTIONTYPE_HPP
#define BMBZ_GAMEACTION_ACTIONTYPE_HPP

#include <cstdint>

namespace Bmbz
{
namespace Game
{
    enum class ActionType : uint8_t
    {
        Empty,
        Confirm,
        Refuse,
        Ping,
        CreateAvatar,
        UpdatePosition,
        UpdatePlayerInfo,
        SetBomb,
        SetFire,
        RemoveObject,
        CreateResource,
        ReplaceCell,
        PlayerDeath,
        MAX
    };
}
}
#endif