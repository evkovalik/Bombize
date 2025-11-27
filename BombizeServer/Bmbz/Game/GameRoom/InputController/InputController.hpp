#ifndef BMBZ_GAME_INPUTCONTROLLER_HPP
#define BMBZ_GAME_INPUTCONTROLLER_HPP

#include <vector>
#include "Utils/ReadWriteData.hpp"
#include "Game/GameContext/GameActions/ActionType.hpp"
#include "InputControllerObserver.hpp"

namespace Bmbz
{
namespace Game
{
    class InputController
    {
    public:
        void AddListener(InputControllerObserver* observer);
        void RemoveListener(InputControllerObserver* observer);
        bool ProcessInputData(boost::beast::flat_buffer& inBuffer);
    private:
        std::vector<InputControllerObserver*> _observers;
    };
}
}
#endif