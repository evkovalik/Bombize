#ifndef BMBZ_GAME_COMPONENT_EFFECT_TEMPORARYCHANGE_HPP
#define BMBZ_GAME_COMPONENT_EFFECT_TEMPORARYCHANGE_HPP

#include "Game/Components/Effects/Changes/ChangeParameter.hpp"
#include "Game/Components/LazyTimer.hpp"

namespace Bmbz
{
namespace Game
{
namespace Component
{
namespace Effect
{
    class TemporaryChange : public IEffect
    {
    public:
        TemporaryChange(
            std::unique_ptr<Effect::ChangeParameter>&& tempEffect,
            const std::chrono::milliseconds& time
        )
        : _effect(std::move(tempEffect))
        , _timer(time, false)
        {}

        void Execute(NetworkData::PlayerInfo& playerInfo) override
        {
            if(_isActive && !_timer.IsRunning())
            {
                _effect->Execute(playerInfo);
                _timer.Run();
            }
            else if(_isActive && _timer.Completed())
            {
                _effect->Cancel(playerInfo); // return it as it was
                _timer.Reset();
                _isActive = false;
            }
        }
        bool IsActive() const override { return _isActive; }
        void Enable() override
        {
            _timer.Continue();
            _isActive = true;
        }
        void Disable() override
        {
            if(!_isActive) return;
            _timer.Stop();
            _isActive = false;
        }
    private:
        LazyTimer _timer;
        bool _isActive = true;
        std::unique_ptr<Effect::ChangeParameter> _effect;
    };
}
}
}
}
#endif