#ifndef BMBZ_GAME_INPUTCONTROLLEROBSERVER_HPP
#define BMBZ_GAME_INPUTCONTROLLEROBSERVER_HPP

#include "Game/NetworkData/PlayerPosition.hpp"

namespace Bmbz
{
namespace Game
{
    struct InputControllerObserver
    {
        virtual ~InputControllerObserver() = default;

        virtual void OnInputNewPosition(NetworkData::PlayerPosition position) = 0;
        virtual void OnInputSetBomb() = 0;
    };
}
}
#endif