#include "InputController.hpp"

namespace Bmbz
{
namespace Game
{
    void InputController::AddListener(InputControllerObserver* observer)
    {
        _observers.push_back(observer);
    }

    void InputController::RemoveListener(InputControllerObserver* observer)
    {
        for(auto it = _observers.begin(); it != _observers.end(); ++it)
        {
            if(*it == observer)
            {
                _observers.erase(it);
                break;
            }
        }
    }

    bool InputController::ProcessInputData(boost::beast::flat_buffer& inBuffer)
    {
        ActionType actionType;
        Utils::ReadFromBuffer(inBuffer, actionType);

        bool success = true;
        switch (actionType)
        {
        case ActionType::UpdatePosition:
        {
            NetworkData::PlayerPosition newPosition;
            newPosition.Deserialize(inBuffer);
            for(auto observer : _observers)
                observer->OnInputNewPosition(std::move(newPosition));
            break;
        }
        case ActionType::SetBomb:
        {
            for(auto observer : _observers)
                observer->OnInputSetBomb();
            break;
        }
        case ActionType::Empty:
        {
            // it is just a confirmation of the connection
            break;
        }
        default:
            success = false;
            break;
        }
        return success;
    }
}
}