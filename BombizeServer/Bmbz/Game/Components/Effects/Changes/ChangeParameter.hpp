#ifndef BMBZ_GAME_COMPONENT_EFFECT_CHANGEPARAMETER_HPP
#define BMBZ_GAME_COMPONENT_EFFECT_CHANGEPARAMETER_HPP

#include "Game/Components/Effects/IEffect.hpp"

namespace Bmbz
{
namespace Game
{
namespace Component
{
namespace Effect
{
    class ChangeParameter : public IEffect
    {
    public:
        ChangeParameter(int changesCount) : changesCount(changesCount) {}

        void Execute(NetworkData::PlayerInfo& playerInfo) override final
        {
            if(_isActive)
            {
                ChangeMethod(playerInfo);
                _isActive = false;
            }
        }
        bool IsActive() const override final { return _isActive; }
        void Enable() override final { _isActive = true; }
        void Disable() override final { _isActive = false; }

        int changesCount;

        void Invert()
        {
            changesCount *= -1;
        }
        virtual void Cancel(NetworkData::PlayerInfo& playerInfo) = 0;

    protected:
        virtual void ChangeMethod(NetworkData::PlayerInfo& playerInfo) = 0;
    private:
        bool _isActive = true;
    };
}
}
}
}
#endif